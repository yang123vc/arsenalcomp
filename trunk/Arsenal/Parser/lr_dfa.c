
/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#include "lr_dfa.h"
#include "parser_in.h"
#include "symbol.h"
#include "lr_dfa_aux.h"

AR_NAMESPACE_BEGIN



psrDFA_t*	PSR_CreateDFA()
{
		psrDFA_t *res;

		res = AR_NEW0(psrDFA_t);

		PSR_InitLRItemTbl(&res->tbl);
		return res;
}

void		PSR_DestroyDFA(psrDFA_t *dfa)
{
		size_t i;
		if(dfa == NULL)return;
		PSR_UnInitLRItemTbl(&dfa->tbl);

		for(i = 0; i < dfa->out_count; ++i)
		{
				PSR_DestroySymb(dfa->out[i].symb);
		}
		AR_DEL(dfa->out);
		AR_DEL(dfa);
}

void		PSR_DestroyDFA_ALL(psrDFA_t *start)
{
		psrDFASet_t set;
		size_t i;
		PSR_InitDFASet(&set);

		PSR_CollectDFA(&set, start);

		for(i = 0; i < set.count; ++i)
		{
				PSR_DestroyDFA(set.set[i]);
		}
		PSR_UnInitDFASet(&set);
}


void		PSR_ConnectDFA(psrDFA_t *start, psrDFA_t *to, const psrSymb_t *symb)
{
		size_t i;

		psrDFAArc_t arc;
		AR_ASSERT(start != NULL && to != NULL && symb != NULL);
		
		for(i = 0; i < start->out_count; ++i)
		{
				if(PSR_CompSymb(start->out[i].symb, symb) == 0)
				{
						AR_ASSERT(to == start->out[i].to);
						return;
				}
		}

		arc.symb = PSR_CopyNewSymb(symb);
		arc.to = to;
		if(start->out_count == start->out_cap)
		{
				start->out_cap = (start->out_cap + 4)*2;
				start->out = AR_REALLOC(psrDFAArc_t, start->out, start->out_cap);
		}
		start->out[start->out_count++] = arc;

}

psrDFA_t*   PSR_GetTransTo(psrDFA_t *state, const psrSymb_t *symb)
{
		size_t i;

		AR_ASSERT(state != NULL && symb != NULL);

		for(i = 0; i < state->out_count; ++i)
		{
				if(PSR_CompSymb(state->out[i].symb, symb) == 0)return state->out[i].to;
		}
		return NULL;
}

/********************************************psrDFASet_t**************************************************/


void	  PSR_InitDFASet(psrDFASet_t *set)
{

		AR_memset(set, 0, sizeof(*set));

}


void	  PSR_UnInitDFASet(psrDFASet_t *set)
{
		AR_DEL(set->set);
		AR_memset(set, 0, sizeof(*set));
}


void	  PSR_InsertToDFASet(psrDFASet_t *set, psrDFA_t *dfa)
{
		if(set->count == set->cap)
		{
				set->cap = (set->cap + 4)*2;
				set->set = AR_REALLOC(psrDFA_t*, set->set, set->cap);
		}
		set->set[set->count++] = dfa;
}


psrDFA_t* PSR_FindDFAByItems(psrDFASet_t *set, const psrLRItemTbl_t *tbl)
{
		size_t i;
		AR_ASSERT(set != NULL && tbl != NULL);

		for(i = 0;  i < set->count; ++i)
		{
				if(PSR_EqualLRItemTbl(&set->set[i]->tbl, tbl))
				{
						return set->set[i];
				}
		}
		return NULL;
}

int_t PSR_IndexOfDFASet(const psrDFASet_t *set, const psrDFA_t *dfa)
{
		size_t i;
		AR_ASSERT(set != NULL && dfa != NULL);
		for(i = 0; i < set->count; ++i)
		{
				if(set->set[i] == dfa)return (int_t)i;
		}
		return -1;
}


void	  PSR_CollectDFA(psrDFASet_t *empty_set, psrDFA_t *start)
{
		size_t i,j;
		AR_ASSERT(empty_set != NULL && empty_set->count == 0 && start != NULL);
		
		PSR_InsertToDFASet(empty_set, start);
		for(i = 0; i < empty_set->count; ++i)
		{
				psrDFA_t *dfa = empty_set->set[i];
				for(j = 0; j < dfa->out_count; ++j)
				{
						psrDFA_t *to;
						bool find;
						to = dfa->out[j].to;
						find = false;
						if(PSR_IndexOfDFASet(empty_set, to) == -1)PSR_InsertToDFASet(empty_set, to);
				}
		}
}




/*********************************************************************core***********************************************************/
/*
		这里是假如A -> %Epsilon %Epsilon term;这样，那么会建立Item
		.%Epsilon %Epsilon term
		delime 0

		Insert 0一定不会成功，因为它和curr一样。之后delim += 1;
		%Epsilon . %Epsilon term

		这次body还是%Epsilon，所以Insert 1，相当于将前面的项插进去了。

		%Epsilon  %Epsilon. term
		delim = 2;
		现在body是term了，所以失败，但是还有一项没插入，此时delim == 2，所以将delim插入

		当然，一个正常的文法条目如果存在epsilon那么应该只有这一个终结符，再无其他任何符号。
*/

inline bool __handle_epsilon(psrLRItemTbl_t *closure, const psrGrammar_t *grammar, const psrSymb_t *body, const psrLRItem_t *item, const psrRule_t *rule)
{
		psrLRItem_t tmp;
		bool changed;
		PSR_InitLRItem(&tmp, item->rule, item->delim, item->symb);

		changed = false;

		while(body && PSR_CompSymb(body, PSR_EpsilonSymb) == 0)
		{
				if(PSR_InsertToLRItemTbl(closure, &tmp))
				{
						changed = true;
				}

				PSR_UnInitLRItem(&tmp);
				PSR_InitLRItem(&tmp, tmp.rule, tmp.delim + 1, tmp.symb);
				body = PSR_IndexOfSymbList(&rule->body, tmp.delim);
		}

		if(PSR_InsertToLRItemTbl(closure, &tmp))
		{
				changed = true;
		}
		PSR_UnInitLRItem(&tmp);
		return changed;
}



void PSR_Calc_LR0_Closure(psrLRItemTbl_t *closure, const psrGrammar_t *grammar)
{
		
		bool changed;
		bool *added;

		AR_ASSERT(closure != NULL && grammar != NULL);

		added = AR_NEWARR(bool, grammar->count);

		changed = true;
		
		while(changed)
		{
				psrLRItemTblIter_t iter;
				changed = false;
				AR_memset(added, false, grammar->count * sizeof(bool));
				
				for(iter = PSR_GetLRItemTblFirst(closure); iter.curr; PSR_GetLRItemTblNext(closure, &iter))
				{
						
						const psrRule_t *rule;
						const psrSymb_t *body;

						AR_ASSERT(iter.curr->item.symb == NULL);
						rule = iter.curr->item.rule;
						
						/*
						 delim为下一个元素，A-> .abc
						 delim == 0则 indexof(delim) == a
						 A-> abc.
						 A == 3 则indexof(delim) == NULL,证明无转移
						*/
						body = PSR_IndexOfSymbList(&rule->body, iter.curr->item.delim);
						
						if(body == NULL)continue;/*规约状态无转移*/
						
						if(body->type == PSR_TERM)
						{
								if(__handle_epsilon(closure, grammar, body, &iter.curr->item, rule))
								{
										changed = true;
								}
						}else
						{
								size_t i;
								
								for(i = 0; i < grammar->count; ++i)
								{
										const psrRule_t *rule_dest;
										
										if(added[i])continue;/*此产生式闭包已经被加入了*/
										rule_dest = grammar->rules[i];
										
										if(PSR_CompSymb(body, rule_dest->head) == 0)
										{
												psrLRItem_t item;
												PSR_InitLRItem(&item, rule_dest, 0, NULL);

												if(PSR_InsertToLRItemTbl(closure, &item))
												{
														changed = true;
												}

												PSR_UnInitLRItem(&item);
												added[i] = true;
										}
								}
						}
				}
		}
		AR_DEL(added);
}



void PSR_Calc_LR0_Goto(const psrLRItemTbl_t *start, psrLRItemTbl_t *to, const psrSymb_t *symb, const psrGrammar_t *grammar)
{
		psrLRItemTblIter_t	iter;

		AR_ASSERT(start != NULL && to != NULL && symb != NULL && grammar != NULL);

		PSR_ClearLRItemTbl(to);

		if(PSR_CompSymb(symb, PSR_EpsilonSymb) == 0)return;

		for(iter = PSR_GetLRItemTblFirst(start); iter.curr; PSR_GetLRItemTblNext(start, &iter))
		{
				const psrRule_t *rule;
				const psrSymb_t *next_symb;
				rule = iter.curr->item.rule;
				next_symb = PSR_IndexOfSymbList(&rule->body, iter.curr->item.delim);
						
				if(next_symb == NULL || PSR_CompSymb(next_symb, symb) != 0)
				{
						continue;
				}else
				{
						psrLRItem_t item;
						PSR_InitLRItem(&item, iter.curr->item.rule, iter.curr->item.delim + 1, NULL);
						PSR_InsertToLRItemTbl(to, &item);
						PSR_UnInitLRItem(&item);
				}
		}
		PSR_Calc_LR0_Closure(to, grammar);
}



psrDFA_t*		PSR_Build_LR0_DFA(const psrGrammar_t *grammar)
{
		psrDFA_t		*start;
		psrDFASet_t		set;
		psrLRItem_t		item;
		size_t			i;
	
		const	psrSymbList_t *symb_lst;
		
		psrLRItemTbl_t	goto_tmp;

		AR_ASSERT(grammar != NULL && grammar->count > 1);

		
		PSR_InitLRItem(&item, PSR_GetStartRule(grammar), 0, NULL);
		start = PSR_CreateDFA();
		PSR_InsertToLRItemTbl(&start->tbl, &item);
		PSR_Calc_LR0_Closure(&start->tbl, grammar);

		PSR_InitDFASet(&set);

		PSR_InsertToDFASet(&set, start);
		symb_lst = PSR_GetSymbList(grammar);
		
		AR_ASSERT(symb_lst != NULL);
		
		PSR_InitLRItemTbl(&goto_tmp);


		for(i = 0; i < set.count; ++i)
		{
				size_t k;
				psrDFA_t *dfa;
				dfa = set.set[i];

				for(k = 0; k < symb_lst->count; ++k)
				{
						const psrSymb_t *symb;
						psrDFA_t *to_dfa;
						symb = symb_lst->lst[k];

						PSR_Calc_LR0_Goto(&dfa->tbl, &goto_tmp, symb, grammar);

						if(goto_tmp.item_count == 0)continue;

						to_dfa = PSR_FindDFAByItems(&set, &goto_tmp);

						if(to_dfa == NULL)
						{
								to_dfa = PSR_CreateDFA();
								PSR_SwapLRItemTbl(&to_dfa->tbl, &goto_tmp);
								PSR_InsertToDFASet(&set, to_dfa);

						}
						PSR_ConnectDFA(dfa, to_dfa, symb);
						PSR_ClearLRItemTbl(&goto_tmp);
				}
		}
		
		PSR_UnInitLRItemTbl(&goto_tmp);
		PSR_UnInitDFASet(&set);
		return start;
}




/****************************************************************************************************************************************************/

void PSR_Calc_LR1_Closure(psrLRItemTbl_t *closure, const psrGrammar_t *grammar, const psrFollow_t *tbl)
{
		bool changed;

		AR_ASSERT(closure != NULL && grammar != NULL && tbl != NULL);



		changed = true;
		
		while(changed)
		{
				psrLRItemTblIter_t iter;
				changed = false;
				
				for(iter = PSR_GetLRItemTblFirst(closure); iter.curr; PSR_GetLRItemTblNext(closure, &iter))
				{
						
						const psrRule_t *rule;
						const psrSymb_t *body;
						AR_ASSERT(iter.curr->item.symb != NULL);
						//rule = PSR_GetRuleByRuleID(grammar, iter.curr->item.rule_id);
						rule = iter.curr->item.rule;
						
						/*
						 delim为下一个元素，A-> .abc
						 delim == 0则 indexof(delim) == a
						 A-> abc.
						 A == 3 则indexof(delim) == NULL,证明无转移
						*/
						body = PSR_IndexOfSymbList(&rule->body, iter.curr->item.delim);
						
						if(body == NULL)continue;/*规约状态无转移*/
						
						if(body->type == PSR_TERM)
						{
								if(__handle_epsilon(closure, grammar, body, &iter.curr->item, rule))
								{
										changed = true;
								}
						}else
						{
								size_t i;
								const psrFollowRec_t* rec;
								//__lr1_calc_first(&rule->body, iter.curr->item.delim + 1, iter.curr->item.symb, first_set, cache);
								rec = PSR_GetFollowRecord(tbl, (size_t)PSR_IndexOfGrammar(grammar, iter.curr->item.rule), iter.curr->item.delim);

								for(i = 0; i < grammar->count; ++i)
								{
										const psrRule_t *rule_dest;

										rule_dest = grammar->rules[i];
										
										if(PSR_CompSymb(body, rule_dest->head) == 0)
										{
												size_t k;
												
												for(k = 0; k < rec->lst.count; ++k)
												{
														const psrSymb_t *tmp_symb;
														psrLRItem_t item;
														tmp_symb = rec->lst.lst[k];
														PSR_InitLRItem(&item, rule_dest, 0, tmp_symb);
														if(PSR_InsertToLRItemTbl(closure, &item))
														{
																changed = true;
														}
														PSR_UnInitLRItem(&item);
												}
												
												if(rec->has_epsilon)
												{
														psrLRItem_t item;
														PSR_InitLRItem(&item, rule_dest, 0, iter.curr->item.symb);
														if(PSR_InsertToLRItemTbl(closure, &item))
														{
																changed = true;
														}
														PSR_UnInitLRItem(&item);
												}
										}
								}
						}
				}
		}
		
}



void PSR_Calc_LR1_Goto(const psrLRItemTbl_t *start, psrLRItemTbl_t *to, const psrSymb_t *symb, const psrGrammar_t *grammar, const psrFollow_t *tbl)
{
		psrLRItemTblIter_t	iter;

		AR_ASSERT(start != NULL && to != NULL && symb != NULL && grammar != NULL);

		PSR_ClearLRItemTbl(to);

		if(PSR_CompSymb(symb, PSR_EpsilonSymb) == 0)return;

		for(iter = PSR_GetLRItemTblFirst(start); iter.curr; PSR_GetLRItemTblNext(start, &iter))
		{
				const psrRule_t *rule;
				const psrSymb_t *next_symb;
				rule = iter.curr->item.rule;
				next_symb = PSR_IndexOfSymbList(&rule->body, iter.curr->item.delim);
				
				if(next_symb == NULL || PSR_CompSymb(next_symb, symb) != 0)
				{
						continue;
				}else
				{
						psrLRItem_t item;
						PSR_InitLRItem(&item, iter.curr->item.rule, iter.curr->item.delim + 1, iter.curr->item.symb);
						PSR_InsertToLRItemTbl(to, &item);
						PSR_UnInitLRItem(&item);
				}
		}
		
		PSR_Calc_LR1_Closure(to, grammar, tbl);
}


psrDFA_t*		PSR_Build_LR1_DFA(const psrGrammar_t *grammar)
{

		psrDFA_t		*start;
		psrDFASet_t		set;
		psrLRItem_t		item;
		size_t			i;
		psrFollow_t		follow_tbl;
		
		const	psrSymbList_t *symb_lst;
		
		psrLRItemTbl_t	goto_tmp;

		AR_ASSERT(grammar != NULL && grammar->count > 1);

		PSR_InitFollowTbl(&follow_tbl, grammar);
		

		PSR_InitLRItem(&item, PSR_GetStartRule(grammar), 0, PSR_EOISymb);
		start = PSR_CreateDFA();
		PSR_InsertToLRItemTbl(&start->tbl, &item);
		PSR_Calc_LR1_Closure(&start->tbl, grammar, &follow_tbl); 
		PSR_UnInitLRItem(&item);
		


		PSR_InitDFASet(&set);
		PSR_InsertToDFASet(&set, start);
		
		PSR_InitLRItemTbl(&goto_tmp);

		symb_lst = PSR_GetSymbList(grammar);


		for(i = 0; i < set.count; ++i)
		{
				psrDFA_t *dfa;
				size_t k;
				dfa = set.set[i];

				for(k = 0; k < symb_lst->count; ++k)
				{
						const psrSymb_t *symb;
						psrDFA_t *to_dfa;
						symb = symb_lst->lst[k];

						PSR_ClearLRItemTbl(&goto_tmp);
						PSR_Calc_LR1_Goto(&dfa->tbl, &goto_tmp, symb, grammar, &follow_tbl);

						if(goto_tmp.item_count == 0)continue;

						to_dfa = PSR_FindDFAByItems(&set, &goto_tmp);

						if(to_dfa == NULL)
						{
								to_dfa = PSR_CreateDFA();
								PSR_SwapLRItemTbl(&to_dfa->tbl, &goto_tmp);
								PSR_InsertToDFASet(&set, to_dfa);
						}

						PSR_ConnectDFA(dfa, to_dfa, symb);
						PSR_ClearLRItemTbl(&goto_tmp);
				}
		}

		PSR_UnInitLRItemTbl(&goto_tmp);
		PSR_UnInitDFASet(&set);
		PSR_UnInitFollowTbl(&follow_tbl);
		return start;
}






/*****************************************************LALR************************************************************************/




static bool __calc_symbol_transfer(psrDFA_t *start, psrDFA_t *to, const psrGrammar_t *grammar, const psrFollow_t *tbl, psrLR1ClosureTbl_t *closure_tbl)
{
		bool changed;
		psrLRItemTblIter_t iter;
		const psrLRItemTbl_t	 *lr1_closure;
		
		changed = false;
		lr1_closure = NULL;

		for(iter = PSR_GetLRItemTblFirst(&start->tbl); iter.curr; PSR_GetLRItemTblNext(&start->tbl, &iter))
		{
				psrLRItem_t item;
				psrLRItemTblIter_t lr1_iter;

				PSR_InitLRItem(&item, iter.curr->item.rule, iter.curr->item.delim, PSR_LALRSymb);
				lr1_closure = PSR_GetLR1Closure(closure_tbl, &item, grammar, tbl);
				PSR_UnInitLRItem(&item);
				

				for(lr1_iter = PSR_GetLRItemTblFirst(lr1_closure); lr1_iter.curr; PSR_GetLRItemTblNext(lr1_closure, &lr1_iter))
				{
						psrLRItem_t *plr0_item;
						const psrLRItem_t *plr1_item;
						psrLRItem_t key;
						plr1_item = &lr1_iter.curr->item;
						
						PSR_InitLRItem(&key, plr1_item->rule, plr1_item->delim + 1, NULL);
						/*
								因为计算的是从item start->tbl[x]到 to->tbl[y]的转移中，传递过去了多少个符号
								所以plr1_item的LR0部分，一定在to->tbl中有对应的部分，所以这里直接加1表示经过符号的转移
								当然，这里如果发现没有就直接返回了(例如一个规约状态，delim本身就在最后的符号后面，这时一定不会命中表，因此返回)
								，省略掉的符号的比较部分
						*/
						
						plr0_item  = PSR_FindFromLRItemTbl(&to->tbl, &key);
						if(plr0_item == NULL)continue;
						
						if(PSR_CompSymb(plr1_item->symb, PSR_LALRSymb) != 0)
						{
								AR_ASSERT(plr0_item->lst != NULL);

								if(PSR_InsertToSymbList_Unique(plr0_item->lst, plr1_item->symb))
								{
										changed = true;
								}
						}else
						{
								size_t x;
								
								for(x = 0; x < iter.curr->item.lst->count; ++x)
								{
										if(PSR_InsertToSymbList_Unique(plr0_item->lst, iter.curr->item.lst->lst[x]))
										{
												changed = true;
										}
								}
						}

						PSR_UnInitLRItem(&key);
				}
		}

		return changed;


}



static void __clear_lr0_nonkernel(psrLRItemTbl_t *tbl, psrLRItemTbl_t *backup, const psrGrammar_t *grammar)
{
		psrLRItemTblIter_t iter;
		
		AR_ASSERT(tbl != NULL && backup != NULL);
		PSR_ClearLRItemTbl(backup);

		for(iter = PSR_GetLRItemTblFirst(tbl); iter.curr; PSR_GetLRItemTblNext(tbl, &iter))
		{
				if(iter.curr->item.delim > 0 || iter.curr->item.rule == PSR_GetStartRule(grammar))
				{
						AR_ASSERT(iter.curr->item.symb == NULL);/*确定其为LR0项集*/
						PSR_InsertToLRItemTbl(backup, &iter.curr->item);
				}
		}
		PSR_SwapLRItemTbl(backup, tbl);
}






psrDFA_t*		PSR_Build_LALR_DFA(const psrGrammar_t *grammar)
{
		psrDFA_t		*lr0_start;
		psrDFASet_t		set;
		size_t			i,k;
		psrFollow_t		follow_tbl;
		psrLR1ClosureTbl_t	 lr1_closure;
		bool			changed;

		psrLRItemTbl_t	*backup_set;
		
		lr0_start = PSR_Build_LR0_DFA(grammar);
		
		PSR_InitDFASet(&set);
		PSR_CollectDFA(&set, lr0_start);
		
		backup_set = AR_NEWARR(psrLRItemTbl_t, set.count);
		

		for(i = 0; i < set.count; ++i)
		{
				psrLRItemTblIter_t iter;
				PSR_InitLRItemTbl(&backup_set[i]);
				__clear_lr0_nonkernel(&(set.set[i]->tbl),&backup_set[i], grammar);

				for(iter = PSR_GetLRItemTblFirst(&(set.set[i]->tbl)); iter.curr; PSR_GetLRItemTblNext(&(set.set[i]->tbl), &iter))
				{
						psrLRItem_t *pitem;
						
						pitem = (psrLRItem_t*)&(iter.curr->item);
						pitem->lst = AR_NEW0(psrSymbList_t);
						PSR_InitSymbList(pitem->lst);

						if(pitem->rule == PSR_GetStartRule(grammar) && pitem->delim == 0)
						{
								/*augmentation grammar*/
								PSR_InsertToSymbList(pitem->lst, PSR_CopyNewSymb(PSR_EOISymb));
						}
				}
		}
		
		PSR_InitFollowTbl(&follow_tbl, grammar);
		PSR_InitLR1ClosureTbl(&lr1_closure);
		changed = true;

		while(changed)
		{
				changed = false;

				for(i = 0; i < set.count; ++i)
				{
						psrDFA_t *start, *to;
						start = set.set[i];

						for(k = 0; k < start->out_count; ++k)
						{
								to = start->out[k].to;
								if(__calc_symbol_transfer(start, to, grammar, &follow_tbl, &lr1_closure))
								{
										changed = true;
								}
						}

						if(__calc_symbol_transfer(start, start, grammar, &follow_tbl, &lr1_closure))
						{
								changed = true;
						}
				}
		}
		
		for(i = 0; i < set.count; ++i)
		{
				psrLRItemTbl_t *tbl;
				psrLRItemTblIter_t iter;
				tbl = &backup_set[i];
				
				for(iter = PSR_GetLRItemTblFirst(tbl); iter.curr; PSR_GetLRItemTblNext(tbl, &iter))
				{

						PSR_InsertToLRItemTbl(&(set.set[i]->tbl), &iter.curr->item);
				}
				PSR_UnInitLRItemTbl(tbl);
		}

		AR_DEL(backup_set);
		
		PSR_UnInitDFASet(&set);
		PSR_UnInitFollowTbl(&follow_tbl);
		PSR_UnInitLR1ClosureTbl(&lr1_closure);
		return lr0_start;
}

AR_NAMESPACE_END



