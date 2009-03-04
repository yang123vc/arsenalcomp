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
#include "lr.h"
/*****************************************************语法分析表*******************************************************/

AR_NAMESPACE_BEGIN

#define __TBL_POS(_tbl, _x,_y, _col) ((_tbl)[(_x)*(_col) + _y])

void __sort_symbol_set(psrActionTable_t *tbl)
{
		size_t i,j;
		for(i = 0; i < tbl->term_count; ++i)
		{
				for(j = i; j > 0; --j)
				{
						if(PSR_CompSymb(&tbl->term_set[j] ,&tbl->term_set[j-1]) == -1)
						{

								psrSymb_t tmp = tbl->term_set[j];
								tbl->term_set[j] = tbl->term_set[j-1];
								tbl->term_set[j-1] = tmp;
						}
				}
		}

		for(i = 0; i < tbl->nonterm_count; ++i)
		{
				for(j = i; j > 0; --j)
				{
						if(PSR_CompSymb(&tbl->non_term_set[j] ,&tbl->non_term_set[j-1]) == -1)
						{

								psrSymb_t tmp = tbl->non_term_set[j];
								tbl->non_term_set[j] = tbl->non_term_set[j-1];
								tbl->non_term_set[j-1] = tmp;
						}
				}
		}

}

int __find_term_by_symb(const psrActionTable_t *tbl, const psrSymb_t *symb)
{
		int l,r,m,cmp;

		l = 0; r = (int)tbl->term_count - 1;
		AR_ASSERT(!symb->non_term);
		while(l <= r)
		{
				m = (l + r) /2;
				cmp = PSR_CompSymb(symb, &tbl->term_set[m]);
				if(cmp == 0)
				{
						return m;
				}else if(cmp == -1)
				{
						r = m - 1;
				}else
				{
						l = m + 1;
				}
		}
		return -1;
}

int __find_nonterm_by_symb(const psrActionTable_t *tbl, const psrSymb_t *symb)
{
		int l,r,m,cmp;

		l = 0; r = (int)tbl->nonterm_count- 1;
		AR_ASSERT(symb->non_term);
		while(l <= r)
		{
				m = (l + r) /2;
				cmp = PSR_CompSymb(symb, &tbl->non_term_set[m]);
				if(cmp == 0)
				{
						return m;
				}else if(cmp == -1)
				{
						r = m - 1;
				}else
				{
						l = m + 1;
				}
		}
		return -1;
}




const psrAction_t* PSR_GetAction(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb)
{
		int idx;
		const psrAction_t *act;
		AR_ASSERT(tbl != NULL && state < tbl->row && symb != NULL && !symb->non_term);

		idx = __find_term_by_symb(tbl, symb);
		AR_ASSERT(idx != -1);
		act = __TBL_POS(tbl->actions, state, (size_t)idx, tbl->col);
		return act;
}



int	 PSR_GetState(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb)
{
		int idx;
		AR_ASSERT(tbl != NULL && state < tbl->row && symb != NULL && symb->non_term);
		idx = __find_nonterm_by_symb(tbl, symb);
		AR_ASSERT(idx != -1);

		return __TBL_POS(tbl->goto_tbl, state, (size_t)idx, tbl->goto_col);

}

/*
这里负责用lookahead和产生式本身优先级来解决移入规约冲突问题，基本做法就是，优先级高的把优先级低的替换掉，
如果优先级相同，则查看lookahead，如果是左结合，且当前是移入项，新添加进来的是规约项，那么这种移入规约冲突
就替换移入项为规约项，如果当前是规约项，且新添加进来的也是规约项，则得到一个规约规约冲突，如果新添加进来的不是规约项，
则丢弃掉新添加的，函数返回。
lookahead为右结合时候与之相反，如果lookahead无结合性，则同样算是冲突，将其加入。
*/

static void __copy_action(psrAction_t *l, const psrAction_t *r)
{
		l->item = r->item;
		l->priority = r->priority;
		l->reduce_id = r->reduce_id;
		l->type = r->type;
		l->reduce_count = r->reduce_count;
		
		
}

void __insert_action_to_action_list(psrAction_t **dest, const psrAction_t *sour, const psrSymb_t *lookahead)
{
		psrAction_t *curr, *prev, *tmp;
		AR_ASSERT(dest != NULL && sour != NULL);

		
		
		curr = *dest;
		prev = NULL;
		if(curr == NULL)
		{
				*dest = AR_NEW0(psrAction_t);
				*(*dest) = *sour;
				(*dest)->next = NULL;
				return;
		}

		if(curr->type == PSR_SHIFT && sour->type == PSR_SHIFT)return;

		if(sour->priority > curr->priority)
		{
				__copy_action(curr, sour);
				return;
				
		}else if(sour->priority < curr->priority)
		{
				return;
		}else
		{
				if(lookahead->asso == PSR_ASSO_NOASSO)
				{
						tmp = AR_NEW0(psrAction_t);
						__copy_action(tmp, sour);

						tmp->next = curr->next;
						curr->next = tmp;
				}else if(lookahead->asso == PSR_ASSO_LEFT)
				{
						/*左结合，优先级相同，优先选规约*/
						if(curr->type == PSR_SHIFT && sour->type == PSR_REDUCE)/*shift-reduce冲突，且左结合*/
						{
								__copy_action(curr, sour);
								/*
								curr->item = sour->item;
								curr->priority = sour->priority;
								curr->reduce_id = sour->reduce_id;
								curr->type = sour->type;
								*/

						/*
						左结合，只要curr为规约，sour不是规约则不变动
						*/
						}else if(curr->type == PSR_REDUCE && sour->type != PSR_REDUCE)
						{
								return;
						}else/*冲突*/
						{

								tmp = AR_NEW0(psrAction_t);
								__copy_action(tmp, sour);
								tmp->next = curr->next;
								curr->next = tmp;
						}
				}else if(lookahead->asso == PSR_ASSO_RIGHT)
				{
						/*右结合，优先级相同，优先选移入*/
						if(curr->type == PSR_REDUCE && sour->type == PSR_SHIFT)
						{
								__copy_action(curr, sour);
						}else if(curr->type == PSR_SHIFT && sour->type != PSR_SHIFT)
						{
								return;
						}else
						{
								tmp = AR_NEW0(psrAction_t);
								__copy_action(tmp, sour);
								tmp->next = curr->next;
								curr->next = tmp;
						}

				}
		}
}



void __destroy_action_from_action_list(psrAction_t *lst)
{
		while(lst != NULL)
		{
				psrAction_t *tmp = lst->next;
				AR_DEL(lst);
				lst = tmp;
		}
}

void  PSR_DestroyActionTable(psrActionTable_t *tbl)
{
		
		if(tbl != NULL)
		{
				size_t i,j;
				for(i = 0; i < tbl->row; ++i)
				{
						for(j = 0; j < tbl->col; ++j)
						{
								psrAction_t *lst = __TBL_POS(tbl->actions, i, j, tbl->col);
								if(lst->type == PSR_ERROR)continue;
								__destroy_action_from_action_list(lst);
						}
				}
				AR_DEL(tbl->actions);
				AR_DEL(tbl->goto_tbl);
				AR_DEL(tbl->term_set);
				AR_DEL(tbl->non_term_set);
				AR_DEL(tbl);
		}



}




static void __init_action_table(psrActionTable_t *tbl, const psrGrammar_t *grammar)
{
		size_t i;
		

		tbl->non_term_set = AR_NEWARR0(psrSymb_t, grammar->non_count);
		tbl->nonterm_count = grammar->non_count;

		for(i = 0; i < tbl->nonterm_count; ++i)
		{
				tbl->non_term_set[i] = *grammar->nonterm_set[i];
		}

		tbl->term_set = AR_NEWARR0(psrSymb_t, grammar->term_count);
		tbl->term_count = grammar->term_count;

		for(i = 0; i < tbl->term_count; ++i)
		{
				tbl->term_set[i] = *grammar->term_set[i];

		}
		
		__sort_symbol_set(tbl);

}









/*********************************************SLR********************************************/


const static psrAction_t error_action = {PSR_ERROR, 0, NULL, 0, {0,1,NULL,NULL}};


psrActionTable_t* PSR_BuildActionTable_SLR(const psrGrammar_t *grammar)
{
		psrActionTable_t *tbl;
		lrItemsCollection_t *clt;

		psrAction_t act;
		size_t i;
		
		tbl = AR_NEW0(psrActionTable_t);
		__init_action_table(tbl, grammar);

		clt = LR0_CreateItemsCollection(grammar);

		tbl->goto_row = clt->count;
		tbl->goto_col = grammar->non_count;
		tbl->goto_tbl = AR_NEWARR0(int, tbl->goto_row * tbl->goto_col);
		AR_MSET(tbl->goto_tbl, -1, tbl->goto_row * tbl->goto_col * sizeof(int));
		


		for(i = 0; i < clt->count; ++i)
		{		
				size_t j;
				for(j = 0; j < tbl->nonterm_count; ++j)
				{
						int idx = LR_FindStateFromGotoMap(&clt->goto_map[i], &tbl->non_term_set[j]);
						
						__TBL_POS(tbl->goto_tbl, i, j, tbl->goto_col) = idx;
				}
		}

		tbl->row = clt->count;
		tbl->col = tbl->term_count;
		tbl->actions = AR_NEWARR0(psrAction_t*, tbl->row * tbl->col);


		AR_MSET0(&act, sizeof(act));
		act.next = NULL;
		act.type = PSR_ERROR;
		act.reduce_id = 0;
		act.reduce_count = 0;
		

		for(i = 0; i < clt->count; ++i)
		{		
				const psrLRItem_t *item;
				
				for(item = clt->items[i]; item; item = item->next)
				{
						const psrSymb_t *head, *body;
						head = PSR_GetRuleHeadByRuleID(grammar,item->rule_id);
						body = PSR_GetRuleBodyByRuleID(grammar,item->rule_id);

						AR_ASSERT(head != NULL && body != NULL);
						body = PSR_IndexOfSymbList(body, item->delim);

						AR_MSET0(&act, sizeof(act));
						act.item = *item;
						act.next = NULL;
						act.priority = head->priority;

						if(body == NULL)/*规约状态*/
						{
								int symb_idx;
								const psrSymb_t *follow;
								if(item->rule_id == 0)
								{
										AR_ASSERT(item->delim == 1);
										symb_idx = __find_term_by_symb(tbl, PSR_EOISymb);
										AR_ASSERT(symb_idx != -1);

										act.type = PSR_ACCEPT;
										act.reduce_id = 0;
										
										act.reduce_count = item->delim;

										AR_ASSERT(__TBL_POS(tbl->actions, i, symb_idx, tbl->col) == NULL);
										__insert_action_to_action_list((&__TBL_POS(tbl->actions, i, symb_idx, tbl->col)), &act, PSR_EOISymb);
										
										
								}else
								{
										act.type = PSR_REDUCE;
										act.reduce_id = item->rule_id;

										body = PSR_GetRuleBodyByRuleID(grammar,item->rule_id);
										
										if(PSR_IsNullable(body))/*任何产生式首节点如果为空，则产生式一定是空*/
										{
												act.reduce_count = 0;
										}else
										{
												act.reduce_count = act.item.delim;
										}

										follow = PSR_GetFollowSetBySymb(grammar, head);
										
										for(follow; follow; follow = follow->next)
										{
												symb_idx = __find_term_by_symb(tbl, follow);
												AR_ASSERT(symb_idx != -1);
												__insert_action_to_action_list((&(__TBL_POS(tbl->actions, i, symb_idx, tbl->col))), &act, &tbl->term_set[symb_idx]);
										}
								}
						
						}else/*移入状态*/
						{
								act.reduce_count = 0;
								if(!body->non_term)
								{
										int idx, symb_idx;
										
										idx = LR_FindStateFromGotoMap(&clt->goto_map[i], body);
						
										if(idx != -1)
										{
												act.shift_to = (size_t)idx;
												act.type = PSR_SHIFT;

												symb_idx = __find_term_by_symb(tbl, body);
												AR_ASSERT(symb_idx != -1);
												__insert_action_to_action_list((&(__TBL_POS(tbl->actions, i, symb_idx, tbl->col))), &act,&tbl->term_set[symb_idx]);
										}
								}
						}

				}
		}


		for(i = 0; i < tbl->row * tbl->col; ++i)
		{
				if(tbl->actions[i] == NULL)tbl->actions[i] = (psrAction_t*)&error_action;
		}
		
		return tbl;
}

/***************************************************LR1*************************************************/

psrActionTable_t* PSR_BuildActionTable_LR1(const psrGrammar_t *grammar)
{
		psrActionTable_t *tbl;
		lrItemsCollection_t *clt;

		
		psrAction_t act;
		size_t i;
		
		tbl = AR_NEW0(psrActionTable_t);
		__init_action_table(tbl, grammar);


		clt = LR1_CreateItemsCollection(grammar);

		tbl->goto_row = clt->count;
		tbl->goto_col = grammar->non_count;
		tbl->goto_tbl = AR_NEWARR0(int, tbl->goto_row * tbl->goto_col);
		AR_MSET(tbl->goto_tbl, -1, tbl->goto_row * tbl->goto_col * sizeof(int));
		
		
		for(i = 0; i < clt->count; ++i)
		{		
				size_t j;
				for(j = 0; j < tbl->nonterm_count; ++j)
				{
						int idx = LR_FindStateFromGotoMap(&clt->goto_map[i], &tbl->non_term_set[j]);
						
						__TBL_POS(tbl->goto_tbl, i, j, tbl->goto_col) = idx;
				}
		}


		tbl->row = clt->count;
		tbl->col = tbl->term_count;
		tbl->actions = AR_NEWARR0(psrAction_t*, tbl->row * tbl->col);


		AR_MSET0(&act, sizeof(act));
		act.next = NULL;
		act.type = PSR_ERROR;
		act.reduce_id = 0;


		for(i = 0; i < clt->count; ++i)
		{		
				const psrLRItem_t *item;
				
				for(item = clt->items[i]; item; item = item->next)
				{
						const psrSymb_t *head, *body;
						head = PSR_GetRuleHeadByRuleID(grammar,item->rule_id);
						body = PSR_GetRuleBodyByRuleID(grammar,item->rule_id);

						AR_ASSERT(head != NULL && body != NULL);
						body = PSR_IndexOfSymbList(body, item->delim);

						AR_MSET0(&act, sizeof(act));
						act.item = *item;
						act.next = NULL;
						act.priority = head->priority;

						if(body == NULL)/*规约状态*/
						{
								int symb_idx;
								
								if(item->rule_id == 0 && PSR_CompSymb(item->symb, PSR_EOISymb) == 0)
								{
										AR_ASSERT(item->delim == 1);
										symb_idx = __find_term_by_symb(tbl, PSR_EOISymb);
										AR_ASSERT(symb_idx != -1);

										act.type = PSR_ACCEPT;
										act.reduce_id = 0;
										
										act.reduce_count = item->delim;

										AR_ASSERT(__TBL_POS(tbl->actions, i, symb_idx, tbl->col) == NULL);
										__insert_action_to_action_list((&__TBL_POS(tbl->actions, i, symb_idx, tbl->col)), &act, PSR_EOISymb);
										
										
								}else
								{
										act.type = PSR_REDUCE;
										act.reduce_id = item->rule_id;

										body = PSR_GetRuleBodyByRuleID(grammar,item->rule_id);
										
										if(PSR_IsNullable(body))/*任何产生式首节点如果为空，则产生式一定是空*/
										{
												act.reduce_count = 0;
										}else
										{
												act.reduce_count = act.item.delim;
										}

										symb_idx = __find_term_by_symb(tbl, item->symb);
										AR_ASSERT(symb_idx != -1);
										__insert_action_to_action_list((&(__TBL_POS(tbl->actions, i, symb_idx, tbl->col))), &act, &tbl->term_set[symb_idx]);
								}
						
						}else/*移入状态*/
						{
								act.reduce_count = 0;
								if(!body->non_term)
								{
										int idx, symb_idx;
										
										idx = LR_FindStateFromGotoMap(&clt->goto_map[i], body);
						
										if(idx != -1)
										{
												act.shift_to = (size_t)idx;
												act.type = PSR_SHIFT;

												symb_idx = __find_term_by_symb(tbl, body);
												AR_ASSERT(symb_idx != -1);
												__insert_action_to_action_list((&(__TBL_POS(tbl->actions, i, symb_idx, tbl->col))), &act,&tbl->term_set[symb_idx]);
										}
								}
						}

				}
		}


		for(i = 0; i < tbl->row * tbl->col; ++i)
		{
				if(tbl->actions[i] == NULL)tbl->actions[i] = (psrAction_t*)&error_action;
		}
		
		return tbl;
}










psrActionTable_t* PSR_BuildActionTable_LALR(const psrGrammar_t *grammar)
{
		psrActionTable_t *tbl;
		lrItemsCollection_t *clt;

		
		psrAction_t act;
		size_t i;
		
		tbl = AR_NEW0(psrActionTable_t);
		__init_action_table(tbl, grammar);


		clt = LR1_CreateItemsCollection(grammar);

		tbl->goto_row = clt->count;
		tbl->goto_col = grammar->non_count;
		tbl->goto_tbl = AR_NEWARR0(int, tbl->goto_row * tbl->goto_col);
		AR_MSET(tbl->goto_tbl, -1, tbl->goto_row * tbl->goto_col * sizeof(int));
		
		
		for(i = 0; i < clt->count; ++i)
		{		
				size_t j;
				for(j = 0; j < tbl->nonterm_count; ++j)
				{
						int idx = LR_FindStateFromGotoMap(&clt->goto_map[i], &tbl->non_term_set[j]);
						
						__TBL_POS(tbl->goto_tbl, i, j, tbl->goto_col) = idx;
				}
		}


		tbl->row = clt->count;
		tbl->col = tbl->term_count;
		tbl->actions = AR_NEWARR0(psrAction_t*, tbl->row * tbl->col);


		AR_MSET0(&act, sizeof(act));
		act.next = NULL;
		act.type = PSR_ERROR;
		act.reduce_id = 0;


		for(i = 0; i < clt->count; ++i)
		{		
				const psrLRItem_t *item;
				
				for(item = clt->items[i]; item; item = item->next)
				{
						const psrSymb_t *head, *body;
						head = PSR_GetRuleHeadByRuleID(grammar,item->rule_id);
						body = PSR_GetRuleBodyByRuleID(grammar,item->rule_id);

						AR_ASSERT(head != NULL && body != NULL);
						body = PSR_IndexOfSymbList(body, item->delim);

						AR_MSET0(&act, sizeof(act));
						act.item = *item;
						act.next = NULL;
						act.priority = head->priority;

						if(body == NULL)/*规约状态*/
						{
								int symb_idx;
								
								if(item->rule_id == 0 && PSR_FindSymb(item->symb, PSR_EOISymb) != NULL)
								{
										AR_ASSERT(item->delim == 1);
										symb_idx = __find_term_by_symb(tbl, PSR_EOISymb);
										AR_ASSERT(symb_idx != -1);

										act.type = PSR_ACCEPT;
										act.reduce_id = 0;
										
										act.reduce_count = item->delim;

										AR_ASSERT(__TBL_POS(tbl->actions, i, symb_idx, tbl->col) == NULL);
										__insert_action_to_action_list((&__TBL_POS(tbl->actions, i, symb_idx, tbl->col)), &act, PSR_EOISymb);
								}else
								{
										const psrSymb_t *lst;
										act.type = PSR_REDUCE;
										act.reduce_id = item->rule_id;

										body = PSR_GetRuleBodyByRuleID(grammar,item->rule_id);
										
										if(PSR_IsNullable(body))/*任何产生式首节点如果为空，则产生式一定是空*/
										{
												act.reduce_count = 0;
										}else
										{
												act.reduce_count = act.item.delim;
										}
										
										for(lst = item->symb; lst; lst = lst->next)
										{
												symb_idx = __find_term_by_symb(tbl, lst);
												AR_ASSERT(symb_idx != -1);
												__insert_action_to_action_list((&(__TBL_POS(tbl->actions, i, symb_idx, tbl->col))), &act, &tbl->term_set[symb_idx]);
										}
								}
						
						}else/*移入状态*/
						{
								act.reduce_count = 0;
								if(!body->non_term)
								{
										int idx, symb_idx;
										
										idx = LR_FindStateFromGotoMap(&clt->goto_map[i], body);
						
										if(idx != -1)
										{
												act.shift_to = (size_t)idx;
												act.type = PSR_SHIFT;

												symb_idx = __find_term_by_symb(tbl, body);
												AR_ASSERT(symb_idx != -1);
												__insert_action_to_action_list((&(__TBL_POS(tbl->actions, i, symb_idx, tbl->col))), &act,&tbl->term_set[symb_idx]);
										}
								}
						}

				}
		}


		for(i = 0; i < tbl->row * tbl->col; ++i)
		{
				if(tbl->actions[i] == NULL)tbl->actions[i] = (psrAction_t*)&error_action;
		}
		
		return tbl;


}


AR_NAMESPACE_END
