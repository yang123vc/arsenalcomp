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

#include "lr_action.h"
#include "lr_dfa.h"


AR_NAMESPACE_BEGIN



/******************************************************************************aux*****************************/



const psrAction_t*		PSR_GetAction(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb)
{
		int_t idx;
		AR_ASSERT(state < tbl->row && symb->type == PSR_TERM);
		idx = PSR_BSearchFromSymbList(&tbl->term_set,symb);
		AR_ASSERT(idx != -1);
		return tbl->actions[AR_TBL_IDX_R(state,idx, tbl->col)];


}

int_t					PSR_GetState(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb)
{
		int_t idx;
		AR_ASSERT(state < tbl->goto_row && symb->type == PSR_NONTERM);
		idx = PSR_BSearchFromSymbList(&tbl->nonterm_set,symb);
		return tbl->goto_tbl[AR_TBL_IDX_R(state,idx, tbl->goto_col)];
}

const psrSymbList_t*	PSR_GetExpectedSymb(const psrActionTable_t *tbl, size_t state)
{
		AR_ASSERT(state < tbl->row);
		return &tbl->expected_set[state];
}


static psrActionTable_t* __create_table(const psrGrammar_t *gmr)
{
		psrActionTable_t *tbl;
		size_t i;
		
		const psrSymbList_t *symb_lst;
		AR_ASSERT(gmr != NULL && gmr->count > 1);

		tbl = AR_NEW0(psrActionTable_t);
		symb_lst = PSR_GetSymbList(gmr);
		
		PSR_InitSymbList(&tbl->term_set);
		PSR_InitSymbList(&tbl->nonterm_set);

		for(i = 0; i < symb_lst->count; ++i)
		{
				const psrSymb_t *curr;
				curr = symb_lst->lst[i];

				if(curr->type == PSR_TERM)
				{
						PSR_InsertToSymbList(&tbl->term_set, PSR_CopyNewSymb(curr));
				}else
				{
						PSR_InsertToSymbList(&tbl->nonterm_set, PSR_CopyNewSymb(curr));
				}
		}

		PSR_SortSymbList(&tbl->term_set);
		PSR_SortSymbList(&tbl->nonterm_set);

		return tbl;
}





static void __build_goto_table(psrActionTable_t *tbl, const psrDFASet_t *set)
{
		size_t i,j;
		AR_ASSERT(tbl != NULL && tbl->goto_tbl == NULL && set != NULL && set->count > 0);

		tbl->goto_row = set->count;
		tbl->goto_col = tbl->nonterm_set.count;
		tbl->goto_tbl = AR_NEWARR(int_t, tbl->goto_row * tbl->goto_col);
		AR_memset(tbl->goto_tbl, (int_t)-1, tbl->goto_row * tbl->goto_col * sizeof(int_t));
		

		for(i = 0; i < tbl->goto_row; ++i)
		{
				const psrDFA_t *dfa;
				dfa = set->set[i];
				
				for(j = 0; j < dfa->out_count; ++j)
				{
						if(dfa->out[j].symb->type == PSR_NONTERM)
						{
								int_t idx;
								idx = PSR_BSearchFromSymbList(&tbl->nonterm_set, dfa->out[j].symb);
								AR_ASSERT(idx != -1);
								tbl->goto_tbl[AR_TBL_IDX_R(i, idx, tbl->goto_col)] = PSR_IndexOfDFASet(set, dfa->out[j].to);
						}
				}
		}

}

/*此函数将在建立完毕后调用*/
static void __build_expected_list(psrActionTable_t *tbl)
{
		size_t i,j;
		AR_ASSERT(tbl != NULL && tbl->expected_set == NULL && tbl->row > 0);
		
		tbl->expected_set = AR_NEWARR0(psrSymbList_t, tbl->row);

		for(i = 0; i < tbl->row; ++i)PSR_InitSymbList(&tbl->expected_set[i]);
		
		for(i = 0; i < tbl->row; ++i)
		{
				for(j = 0; j < tbl->term_set.count; ++j)
				{
						const psrAction_t	*act;
						const psrSymb_t *symb = tbl->term_set.lst[j];
						
						if(PSR_CompSymb(symb, PSR_ErrorSymb) == 0 || PSR_CompSymb(symb, PSR_EpsilonSymb) == 0)continue;

						act = PSR_GetAction(tbl, i, symb);
						AR_ASSERT(act != NULL);
						
						if(act->type != PSR_ERROR)
						{
								PSR_InsertToSymbList(&tbl->expected_set[i], symb);
						}
				}
		}
}

static void __destroy_expected_list(psrActionTable_t *tbl)
{
		size_t i;
		AR_ASSERT(tbl != NULL && tbl->row > 0);
		
		for(i = 0; i < tbl->row; ++i)PSR_UnInitSymbList(&tbl->expected_set[i]);
		AR_DEL(tbl->expected_set);
		tbl->expected_set = NULL;
}



void					PSR_DestroyActionTable(psrActionTable_t *tbl)
{
		size_t i;
		
		__destroy_expected_list(tbl);

		for(i = 0; i < tbl->term_set.count; ++i)
		{
				PSR_DestroySymb(tbl->term_set.lst[i]);
		}
		
		for(i = 0; i < tbl->nonterm_set.count; ++i)
		{
				PSR_DestroySymb(tbl->nonterm_set.lst[i]);
		}

		PSR_UnInitSymbList(&tbl->nonterm_set);
		PSR_UnInitSymbList(&tbl->term_set);
		AR_DEL(tbl->goto_tbl);

		for(i = 0; i < tbl->row * tbl->col; ++i)
		{
				psrAction_t *action;

				action = tbl->actions[i];

				if(action->type != PSR_ERROR)
				{
						while(action != NULL)
						{
								psrAction_t *tmp;
								tmp = action->next;
								AR_DEL(action);
								action = tmp;
						}
				}
		}
		
		AR_DEL(tbl->actions);
		AR_DEL(tbl);

}




static const psrAction_t error_action = {PSR_ERROR, 0, NULL, 0, 0,0, NULL};

const psrAction_t	*PSR_ErrorAction = &error_action;



/*
这里负责用lookahead和产生式本身优先级来解决移入规约冲突问题，基本做法就是，优先级高的把优先级低的替换掉，
如果优先级相同，则查看lookahead，如果是左结合，且当前是移入项，新添加进来的是规约项，那么这种移入规约冲突
就替换移入项为规约项，如果当前是规约项，且新添加进来的也是规约项，则得到一个规约规约冲突，如果新添加进来的不是规约项，
则丢弃掉新添加的，函数返回。
lookahead为右结合时候与之相反，如果lookahead无结合性，则同样算是冲突，将其加入。
*/

static void __copy_action(psrAction_t *l, const psrAction_t *r)
{
		l->rule = r->rule;
		l->prec = r->prec;
		l->type = r->type;
		l->delim = r->delim;
		l->reduce_count = r->reduce_count;
		l->shift_to = r->shift_to;
}



static void __insert_action_to_action_list(psrAction_t **dest, const psrAction_t *sour, const psrTermInfo_t *lookahead)
{
		psrAction_t *curr, *prev, *tmp;
		AR_ASSERT(dest != NULL && sour != NULL && lookahead != NULL);
		
		curr = *dest;
		prev = NULL;
		if(curr == NULL)
		{
				*dest = AR_NEW0(psrAction_t);
				__copy_action(*dest, sour);
				return;
		}

		/*
				可能存在shift，但这里不是冲突，因此直接返回，因为不可能在同一符号上的移入导致的状态转换不被前面的DFA建立检测出来
		*/
		if(curr->type == PSR_SHIFT && sour->type == PSR_SHIFT)return;

		if(curr->type == PSR_ACCEPT)
		{
				tmp = AR_NEW0(psrAction_t);
				__copy_action(tmp, sour);
				tmp->next = curr->next;
				curr->next = tmp;
				return;
		}
		
		
		if(sour->prec > curr->prec)
		{
				/*
						新加入的优先级高于原项，则不被认为是冲突，因此插入到头结点，curr现在为head结点
				*/
				__copy_action(curr, sour);
				return;
				
		}else if(sour->prec < curr->prec)
		{
				/*
						新加入的优先级低于原项，则不被认为是冲突，且不错任何修改，函数返回
				*/
				return;
		}else
		{
				/*
						相同优先级的情况下：
				*/
				if(lookahead->assoc == PSR_ASSOC_NOASSOC)
				{

						tmp = AR_NEW0(psrAction_t);
						__copy_action(tmp, sour);
						
						if(curr->type != PSR_SHIFT)
						{
								/*如果不是shift，则将新的action放在前面*/
								tmp->next = *dest;
								*dest = tmp;
						}else
						{
								/*如果是shift，则选择curr在前*/
								tmp->next = curr->next;
								curr->next = tmp;
						}

				}else if(lookahead->assoc == PSR_ASSOC_LEFT)
				{
						/*左结合，优先级相同，优先选规约*/
						if(curr->type == PSR_SHIFT && sour->type == PSR_REDUCE)/*shift-reduce冲突，且左结合*/
						{
								__copy_action(curr, sour);
						
						}else if(curr->type == PSR_REDUCE && sour->type != PSR_REDUCE)
						{
								/*左结合，只要curr为规约，sour不是规约则不变动*/
								return;
						}else/*冲突*/
						{
								/*这地方一定是规约规约冲突*/
								AR_ASSERT(curr->type == PSR_REDUCE && sour->type == PSR_REDUCE);
								tmp = AR_NEW0(psrAction_t);
								__copy_action(tmp, sour);
								tmp->next = curr->next;
								curr->next = tmp;
						}
				}else if(lookahead->assoc == PSR_ASSOC_RIGHT)
				{
						/*右结合，优先级相同，优先选移入*/
						if(curr->type == PSR_REDUCE && sour->type == PSR_SHIFT)
						{
								__copy_action(curr, sour);
						}else if(curr->type == PSR_SHIFT && sour->type != PSR_SHIFT)
						{
								/*右结合，只要curr为移入，sour不是移入则不变东*/
								return;
						}else/*冲突*/
						{
								/*这地方一定是规约规约冲突*/
								AR_ASSERT(curr->type == PSR_REDUCE && sour->type == PSR_REDUCE);
								tmp = AR_NEW0(psrAction_t);
								__copy_action(tmp, sour);
								tmp->next = curr->next;
								curr->next = tmp;
						}
				}
		}
}

/*********************************************************************************************************************************/

psrActionTable_t* PSR_CreateActionTable_SLR(const psrGrammar_t *grammar)
{
		psrActionTable_t *tbl;
		psrDFA_t	*start;
		psrDFASet_t set;
		psrSymbMap_t first, follow;

		size_t i;
		tbl = __create_table(grammar);
		
		PSR_InitDFASet(&set);
		start = PSR_Build_LR0_DFA(grammar);
		PSR_CollectDFA(&set, start);
		__build_goto_table(tbl, &set);


		PSR_InitSymbMap(&first);
		PSR_InitSymbMap(&follow);
		PSR_CalcFirstSet(grammar, &first);
		PSR_CalcFollowSet(grammar, &follow, &first);
		
		tbl->row = set.count;
		tbl->col = tbl->term_set.count;

		tbl->actions = AR_NEWARR0(psrAction_t*, tbl->row * tbl->col);

		for(i = 0; i < tbl->row; ++i)
		{
				const psrDFA_t *dfa;
				psrLRItemTblIter_t iter;
				dfa = set.set[i];
				
				for(iter = PSR_GetLRItemTblFirst(&dfa->tbl); iter.curr; PSR_GetLRItemTblNext(&dfa->tbl, &iter))
				{
						const psrLRItem_t *item;
						const psrSymb_t	  *body;
						const psrRule_t	   *rule;
						psrAction_t		  action;
						
						item = &iter.curr->item;
						rule = item->rule;
						body = PSR_IndexOfSymbList(&rule->body, item->delim);
						AR_memset(&action, 0, sizeof(action));
						
						action.rule = rule;
						action.delim = item->delim;
						action.prec = PSR_GetRulePrecAssocInfo(grammar, rule)->prec;
						action.reduce_count = 0;
						action.shift_to = 0;
						action.next = NULL;
						

						if(body == NULL)
						{
								size_t x;
								action.reduce_count = rule->body.count;
								
								/*
										这里有点麻烦，我的原出目的是RULE中允许存在空终结符，所以，这样reduce_count就要减去所有的
								这些空终结符，但是在实际的文法中，每条规则有且只有一个空终结符，否则为错误.所以，例如文法
								A -> .;的reduce_count就为0，这样任何输入进来之后，直接会先规约出A，且parser stack不弹出任何符号，直接压入A。
								*/
								AR_ASSERT(action.reduce_count > 0);
								for(x = 0; x < rule->body.count; ++x)
								{
										if(PSR_CompSymb(rule->body.lst[x], PSR_EpsilonSymb) == 0)
										{
												--action.reduce_count;
										}
								}


								if(PSR_CompSymb(rule->head, PSR_StartSymb) == 0)/*接受状态*/
								{
										action.type = PSR_ACCEPT;
								}else
								{
										action.type = PSR_REDUCE;
								}

								{
										
										const psrMapRec_t		*rec;
										rec = PSR_GetSymbolFromSymbMap(&follow, rule->head);
										AR_ASSERT(rec != NULL);
										
										for(x = 0; x < rec->lst.count; ++x)
										{
												int_t idx;
												const psrSymb_t *symb_tmp1 = rec->lst.lst[x];
												idx = PSR_FindFromSymbList(&tbl->term_set, symb_tmp1);
												AR_ASSERT(idx != -1);
												
												__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action,PSR_GetTermSymbInfo(grammar, symb_tmp1));
										}
								}
						}else
						{
								if(body->type == PSR_TERM && PSR_CompSymb(body, PSR_EpsilonSymb) != 0)
								{
										const psrDFA_t *trans_to;
										int_t trans_to_idx,idx;

										action.type = PSR_SHIFT;
										trans_to = PSR_GetTransTo((psrDFA_t*)dfa, body);
										AR_ASSERT(trans_to != NULL);
										trans_to_idx = PSR_IndexOfDFASet(&set, trans_to);
										AR_ASSERT(trans_to_idx != -1);
										action.shift_to = (size_t)trans_to_idx;
										
										idx = PSR_FindFromSymbList(&tbl->term_set, body);
										__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, PSR_GetTermSymbInfo(grammar, body));
								}
								
						}

				}
		}

		PSR_UnInitSymbMap(&first);
		PSR_UnInitSymbMap(&follow);

		PSR_DestroyDFA_ALL(start);
		PSR_UnInitDFASet(&set);

		for(i = 0; i < tbl->row * tbl->col; ++i)
		{
				if(tbl->actions[i] == NULL)
				{
						tbl->actions[i] = (psrAction_t*)PSR_ErrorAction;
				}
		}

		__build_expected_list(tbl);
		return tbl;
}




psrActionTable_t* PSR_CreateActionTable_LR1(const psrGrammar_t *grammar)
{
		psrActionTable_t *tbl;
		psrDFA_t	*start;
		psrDFASet_t set;
		size_t i;

		
		tbl = __create_table(grammar);
		
		PSR_InitDFASet(&set);
		
		

		start = PSR_Build_LR1_DFA(grammar);

		

		PSR_CollectDFA(&set, start);
		__build_goto_table(tbl, &set);

		tbl->row = set.count;
		tbl->col = tbl->term_set.count;

		tbl->actions = AR_NEWARR0(psrAction_t*, tbl->row * tbl->col);

		for(i = 0; i < tbl->row; ++i)
		{
				const psrDFA_t *dfa;
				psrLRItemTblIter_t iter;
				dfa = set.set[i];

				for(iter = PSR_GetLRItemTblFirst(&dfa->tbl); iter.curr; PSR_GetLRItemTblNext(&dfa->tbl, &iter))
				{
						const psrLRItem_t *item;
						const psrSymb_t	  *body;
						const psrRule_t	   *rule;
						psrAction_t		  action;
					
						item = &iter.curr->item;
						rule = item->rule;
						body = PSR_IndexOfSymbList(&rule->body, item->delim);
						AR_memset(&action, 0, sizeof(action));

						action.rule = rule;
						action.delim = item->delim;
						action.prec = PSR_GetRulePrecAssocInfo(grammar, rule)->prec;
						action.reduce_count = 0;
						action.shift_to = 0;
						action.next = NULL;
						
						if(body == NULL)
						{
								size_t x;
								
								action.reduce_count = rule->body.count;
								
								for(x = 0; x < rule->body.count; ++x)
								{
										if(PSR_CompSymb(rule->body.lst[x], PSR_EpsilonSymb) == 0)
										{
												--action.reduce_count;
										}
								}

								if(PSR_CompSymb(rule->head, PSR_StartSymb) == 0)/*接受状态*/
								{
										action.type = PSR_ACCEPT;
								}else
								{
										action.type = PSR_REDUCE;
								}
								{
										int_t idx;
										AR_ASSERT(iter.curr->item.symb != NULL);
										idx = PSR_FindFromSymbList(&tbl->term_set, iter.curr->item.symb);
										__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, PSR_GetTermSymbInfo(grammar, item->symb));
										
								}
						}else
						{
								if(body->type == PSR_TERM && PSR_CompSymb(body, PSR_EpsilonSymb) != 0)
								{
										const psrDFA_t *trans_to;
										int_t trans_to_idx,idx;

										action.type = PSR_SHIFT;
										trans_to = PSR_GetTransTo((psrDFA_t*)dfa, body);
										AR_ASSERT(trans_to != NULL);
										trans_to_idx = PSR_IndexOfDFASet(&set, trans_to);
										AR_ASSERT(trans_to_idx != -1);
										action.shift_to = (size_t)trans_to_idx;
										
										idx = PSR_FindFromSymbList(&tbl->term_set, body);
										__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, PSR_GetTermSymbInfo(grammar, body));
								}
								
						}

				}

		}


		PSR_DestroyDFA_ALL(start);
		PSR_UnInitDFASet(&set);

		for(i = 0; i < tbl->row * tbl->col; ++i)
		{
				if(tbl->actions[i] == NULL)
				{
						tbl->actions[i] = (psrAction_t*)PSR_ErrorAction;
				}
		}

		__build_expected_list(tbl);
		return tbl;
}





psrActionTable_t* PSR_CreateActionTable_LALR(const psrGrammar_t *grammar)
{
		psrActionTable_t *tbl;
		psrDFA_t	*start;
		psrDFASet_t set;
		size_t i;
		tbl = __create_table(grammar);
		
		PSR_InitDFASet(&set);
		start = PSR_Build_LALR_DFA(grammar);
		PSR_CollectDFA(&set, start);
		__build_goto_table(tbl, &set);

		tbl->row = set.count;
		tbl->col = tbl->term_set.count;

		tbl->actions = AR_NEWARR0(psrAction_t*, tbl->row * tbl->col);

		for(i = 0; i < tbl->row; ++i)
		{
				const psrDFA_t *dfa;
				psrLRItemTblIter_t iter;
				dfa = set.set[i];

				for(iter = PSR_GetLRItemTblFirst(&dfa->tbl); iter.curr; PSR_GetLRItemTblNext(&dfa->tbl, &iter))
				{
						const psrLRItem_t *item;
						const psrSymb_t	  *body;
						const psrRule_t	   *rule;
						psrAction_t		  action;
					
						item = &iter.curr->item;
						rule = item->rule;
						body = PSR_IndexOfSymbList(&rule->body, item->delim);
						AR_memset(&action, 0, sizeof(action));

						action.rule = rule;
						action.delim = item->delim;
						action.prec = PSR_GetRulePrecAssocInfo(grammar, rule)->prec;
						action.reduce_count = 0;
						action.shift_to = 0;
						action.next = NULL;
						
						if(body == NULL)
						{
								size_t x;
								
								action.reduce_count = rule->body.count;
								
								for(x = 0; x < rule->body.count; ++x)
								{
										if(PSR_CompSymb(rule->body.lst[x], PSR_EpsilonSymb) == 0)
										{
												--action.reduce_count;
										}
								}

								if(PSR_CompSymb(rule->head, PSR_StartSymb) == 0)/*接受状态*/
								{
										action.type = PSR_ACCEPT;
								}else
								{
										action.type = PSR_REDUCE;
								}
								{
										
										AR_ASSERT(iter.curr->item.lst != NULL);
										AR_ASSERT(iter.curr->item.symb == NULL);
										
										//for(symb_iter = PSR_FirstFromSymbTbl(iter.curr->item.tbl); symb_iter.curr; PSR_NextFromSymbTbl(iter.curr->item.tbl, &symb_iter))
										for(x = 0; x < iter.curr->item.lst->count; ++x)
										{
												int_t idx;
												const psrSymb_t *lookahead_symb = iter.curr->item.lst->lst[x];
												idx = PSR_FindFromSymbList(&tbl->term_set, lookahead_symb);

												AR_ASSERT(idx != -1);
												__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, PSR_GetTermSymbInfo(grammar,lookahead_symb));
										}
										
								}
						}else
						{
								if(body->type == PSR_TERM && PSR_CompSymb(body, PSR_EpsilonSymb) != 0)
								{
										const psrDFA_t *trans_to;
										int_t trans_to_idx,idx;

										action.type = PSR_SHIFT;
										trans_to = PSR_GetTransTo((psrDFA_t*)dfa, body);
										AR_ASSERT(trans_to != NULL);
										trans_to_idx = PSR_IndexOfDFASet(&set, trans_to);
										AR_ASSERT(trans_to_idx != -1);
										action.shift_to = (size_t)trans_to_idx;
										
										idx = PSR_FindFromSymbList(&tbl->term_set, body);
										__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, PSR_GetTermSymbInfo(grammar,body));
								}
								
						}
				}
		}

		PSR_DestroyDFA_ALL(start);
		PSR_UnInitDFASet(&set);

		for(i = 0; i < tbl->row * tbl->col; ++i)
		{
				if(tbl->actions[i] == NULL)
				{
						tbl->actions[i] = (psrAction_t*)PSR_ErrorAction;
				}
		}
		__build_expected_list(tbl);
		return tbl;
}






AR_NAMESPACE_END

