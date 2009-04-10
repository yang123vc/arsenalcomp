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



void PSR_PrintActionTable(const psrActionTable_t *tbl, const psrGrammar_t *grammar, size_t width, arString_t *str)
{
		size_t __WIDTH__;
		size_t i,j;
		wchar_t buf[1024];

		__WIDTH__ = width;
		
		AR_AppendString(str,L"TermSet:\r\n");
		for(i = 0; i < tbl->term_set.count; ++i)
		{
				PSR_PrintSymbol(tbl->term_set.lst[i],false,str);
				AR_AppendString(str,L"  ");
		}
		AR_AppendString(str,L"\r\n");
		AR_AppendString(str, L"-----------------------------------------\r\n");
		AR_AppendString(str,L"NonTermSet:\r\n");
		for(i = 0; i < tbl->nonterm_set.count; ++i)
		{
				PSR_PrintSymbol(tbl->nonterm_set.lst[i],false,str);
				AR_AppendString(str,L"  ");
		}

		AR_AppendString(str,L"\r\n");
		AR_AppendString(str,L"-----------------------------------------\r\n");

		
		AR_AppendString(str,L"Goto Table:\r\n");
		
		AR_AppendFormatString(str, L"%*s", __WIDTH__,L"NULL");
		for(i = 0; i < tbl->nonterm_set.count; ++i)
		{
				AR_swprintf(buf, 1024, L"<%s>", tbl->nonterm_set.lst[i]->name);
				AR_AppendFormatString(str, L"%*s",__WIDTH__, buf);
		}

		AR_AppendString(str,L"\r\n");
		AR_AppendString(str,L"\r\n");
		for(i = 0; i < tbl->goto_row; i++)
		{
				AR_swprintf(buf, 1024, L"I[%d]", i);
				AR_AppendFormatString(str, L"%*s:", __WIDTH__,buf);
				for(j = 0; j < tbl->goto_col; ++j)
				{
						AR_AppendFormatString(str, L"%*d", __WIDTH__,tbl->goto_tbl[AR_TBL_IDX_R(i,j,tbl->goto_col)]);
				}
				AR_AppendString(str,L"\r\n");
		}

		AR_AppendString(str,L"-----------------------------------------\r\n");

		AR_AppendString(str,L"Action Table:\r\n");
		
		AR_AppendFormatString(str,L"%*s", __WIDTH__,L"NULL");
		for(i = 0; i < tbl->term_set.count; ++i)
		{
				AR_AppendFormatString(str,L"%*s",__WIDTH__, tbl->term_set.lst[i]->name);
		}
		AR_AppendFormatString(str,L"\r\n");
		AR_AppendFormatString(str,L"\r\n");



		for(i = 0; i < tbl->row; ++i)
		{
				AR_swprintf(buf, 1024, L"I[%d]", i);
				AR_AppendFormatString(str,L"%*s:", __WIDTH__,buf);
				for(j = 0; j < tbl->col; ++j)
				{
						const psrAction_t *pact;

						pact = tbl->actions[AR_TBL_IDX_R(i,j, tbl->col)];
						switch(pact->type)
						{
						case PSR_ACCEPT:
								AR_AppendFormatString(str,L"%*s", __WIDTH__,L"accept");
								break;
						case PSR_SHIFT:
						{
								AR_AppendFormatString(str,L"%*d", __WIDTH__,pact->shift_to);
						}
								break;
						case PSR_REDUCE:
						{
								const psrRule_t *rule;
								rule = PSR_GetRuleByRuleID(grammar, pact->reduce_id);
								AR_swprintf(buf, 1024, L"[<%s:%d>:%d]",rule->head->name, pact->reduce_id,pact->reduce_count);
								AR_AppendFormatString(str,L"%*s", __WIDTH__,buf);
								
						}
								break;
						default:
								AR_AppendFormatString(str,L"%*s", __WIDTH__,L"error");
								break;
						}
				}
				AR_AppendFormatString(str,L"\r\n");
		}
		AR_AppendFormatString(str,L"-----------------------------------------\r\n");
}



void PSR_ReportConflict(const psrActionTable_t *tbl, const psrGrammar_t *grammar, arString_t *str)
{
		size_t i,j;
		const psrAction_t *action;
		
		AR_AppendFormatString(str,L"%s\r\n", L"Conflict:");
		for(i = 0; i < tbl->row; ++i)
		{
				for(j = 0; j < tbl->col; ++j)
				{
						action = tbl->actions[AR_TBL_IDX_R(i,j,tbl->col)];
						AR_ASSERT(action != NULL);
						if(action->next == NULL)continue;
						AR_AppendFormatString(str,L"state[%d] : %s\r\n",i, tbl->term_set.lst[j]->name);
						
						while(action != NULL)
						{
								psrLRItem_t tmp;
								
								PSR_InitLRItem(&tmp, action->rule_id, action->delim, NULL);
								
								switch(action->type)
								{
								case PSR_REDUCE:
										AR_AppendFormatString(str,L"Reduce: ");
										PSR_PrintLRItem(&tmp,grammar,str);
										break;
								case PSR_SHIFT:
										AR_AppendFormatString(str,L"Shift: ");
										PSR_PrintLRItem(&tmp, grammar,str);
										break;
								case PSR_ACCEPT:
										AR_AppendFormatString(str,L"Accept ");
										//PSR_PrintLRItemName(&action->item,grammar);
										break;
								default:
										AR_ASSERT(false);
								}
								AR_AppendFormatString(str,L"\r\n");
								action = action->next;
								
								PSR_UnInitLRItem(&tmp);
						}

						AR_AppendFormatString(str,L"\r\n");
						AR_AppendFormatString(str,L"-----------------------------------------------\r\n");
				}
		}


}


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
		AR_memset(tbl->goto_tbl, -1, tbl->goto_row * tbl->goto_col * sizeof(int));
		

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

void					PSR_DestroyActionTable(psrActionTable_t *tbl)
{
		size_t i;
		
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




const static psrAction_t error_action = {PSR_ERROR, L"error", 0, 0,0, 0, NULL, 0};

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
		l->rule_name = PSR_AllocString(r->rule_name);
		l->rule_id = r->rule_id;
		l->prec = r->prec;
		l->reduce_id = r->reduce_id;
		l->type = r->type;
		l->delim = r->delim;
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
				__copy_action(*dest, sour);
				return;
		}

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
				__copy_action(curr, sour);
				return;
				
		}else if(sour->prec < curr->prec)
		{
				return;
		}else
		{
				if(lookahead->assoc == PSR_ASSOC_NOASSOC)
				{
						tmp = AR_NEW0(psrAction_t);
						__copy_action(tmp, sour);
						tmp->next = curr->next;
						curr->next = tmp;
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
						rule = PSR_GetRuleByRuleID(grammar, item->rule_id);
						body = PSR_IndexOfSymbList(&rule->body, item->delim);
						AR_memset(&action, 0, sizeof(action));
						
						action.rule_id = rule->rule_id;
						action.delim = item->delim;
						action.rule_name = PSR_AllocString(rule->head->name);
						action.prec = rule->head->prec;
						action.reduce_count = 0;
						action.shift_to = 0;

						action.next = NULL;
						

						if(body == NULL)
						{
								size_t x;
								
								action.reduce_id = rule->rule_id;
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
										psrSymbTblIter_t		symb_iter;
										const psrMapRec_t		*rec;
										rec = PSR_GetSymbolFromSymbMap(&follow, rule->head);
										AR_ASSERT(rec != NULL);
										
										for(symb_iter = PSR_FirstFromSymbTbl(&rec->tbl); symb_iter.curr; PSR_NextFromSymbTbl(&rec->tbl, &symb_iter))
										{
												int_t idx;
												idx = PSR_FindFromSymbList(&tbl->term_set, symb_iter.curr->symb);
												AR_ASSERT(idx != -1);
												
												__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action,symb_iter.curr->symb);
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
										__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, body);
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
						rule = PSR_GetRuleByRuleID(grammar, item->rule_id);
						body = PSR_IndexOfSymbList(&rule->body, item->delim);
						AR_memset(&action, 0, sizeof(action));

						action.rule_id = rule->rule_id;
						action.delim = item->delim;
						action.rule_name = PSR_AllocString(rule->head->name);
						action.prec = rule->head->prec;
						action.reduce_count = 0;
						action.shift_to = 0;

						action.next = NULL;
						
						if(body == NULL)
						{
								size_t x;
								
								action.reduce_id = rule->rule_id;
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
										__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, iter.curr->item.symb);
										
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
										__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, body);
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
						rule = PSR_GetRuleByRuleID(grammar, item->rule_id);
						body = PSR_IndexOfSymbList(&rule->body, item->delim);
						AR_memset(&action, 0, sizeof(action));

						action.rule_id = rule->rule_id;
						action.delim = item->delim;
						action.rule_name = PSR_AllocString(rule->head->name);
						action.prec = rule->head->prec;
						action.reduce_count = 0;
						action.shift_to = 0;

						action.next = NULL;
						
						if(body == NULL)
						{
								size_t x;
								
								action.reduce_id = rule->rule_id;
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
										psrSymbTblIter_t symb_iter;
										AR_ASSERT(iter.curr->item.tbl != NULL);
										AR_ASSERT(iter.curr->item.symb == NULL);
										for(symb_iter = PSR_FirstFromSymbTbl(iter.curr->item.tbl); symb_iter.curr; PSR_NextFromSymbTbl(iter.curr->item.tbl, &symb_iter))
										{
												int_t idx;
												idx = PSR_FindFromSymbList(&tbl->term_set, symb_iter.curr->symb);
												AR_ASSERT(idx != -1);
												__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, symb_iter.curr->symb);
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
										__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, body);
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

		return tbl;
}






AR_NAMESPACE_END
