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

#include "lalr.h"
#include "lr_action.h"


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




static void __build_goto_table(psrActionTable_t *tbl, const lalrStateSet_t *set)
{
		size_t i,j;
		AR_ASSERT(tbl != NULL && tbl->goto_tbl == NULL && set != NULL && set->count > 0);

		tbl->goto_row = set->count;
		tbl->goto_col = tbl->nonterm_set.count;
		tbl->goto_tbl = AR_NEWARR(int_t, tbl->goto_row * tbl->goto_col);
		AR_memset(tbl->goto_tbl, (int_t)-1, tbl->goto_row * tbl->goto_col * sizeof(int_t));
		

		for(i = 0; i < tbl->goto_row; ++i)
		{
				const lalrState_t *state;
				state = set->set[i];
				
				for(j = 0; j < state->count; ++j)
				{
						if(state->actions[j].symb->type == PSR_NONTERM && state->actions[j].act_type == PSR_SHIFT)
						{
								int_t idx;
								idx = PSR_BSearchFromSymbList(&tbl->nonterm_set, state->actions[j].symb);
								AR_ASSERT(idx != -1);
								tbl->goto_tbl[AR_TBL_IDX_R(i, idx, tbl->goto_col)] = PSR_IndexOfStateSet(set, state->actions[j].to);
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
						
						if(PSR_CompSymb(symb, PSR_ErrorSymb) == 0 )continue;

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



void					PSR_DestroyActionTable(const psrActionTable_t *table)
{
		size_t i;
		psrActionTable_t *tbl;
		AR_ASSERT(table != NULL);
		tbl = (psrActionTable_t*)table;

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




static const psrAction_t error_action = {PSR_ERROR, 0, 0, 0, 0,0, NULL};

const psrAction_t	* const PSR_ErrorAction = &error_action;



/*
这里负责用lookahead和产生式本身优先级来解决移入规约冲突问题，基本做法就是，优先级高的把优先级低的替换掉，
如果优先级相同，则查看lookahead，如果是左结合，且当前是移入项，新添加进来的是规约项，那么这种移入规约冲突
就替换移入项为规约项，如果当前是规约项，且新添加进来的也是规约项，则得到一个规约规约冲突，如果新添加进来的不是规约项，
则丢弃掉新添加的，函数返回。
lookahead为右结合时候与之相反，如果lookahead无结合性，则同样算是冲突，将其加入。
*/

static void __copy_action(psrAction_t *l, const psrAction_t *r)
{
		l->rule_num = r->rule_num;
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
				if(lookahead->assoc == PSR_ASSOC_NONASSOC)
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



psrActionTable_t* __create_action_table(const psrGrammar_t *grammar, psrLRItemType_t type)
{
		psrActionTable_t		*tbl;
		lalrState_t				*start;
		lalrStateSet_t			set;
		size_t i;
		tbl = __create_table(grammar);
		
		PSR_InitStateSet(&set);

		if(type == PSR_SLR)
		{
				start = PSR_Create_LR0_State(grammar);
		}else
		{
				start = PSR_Create_LALR_State(grammar);
		}

		PSR_CollectState(&set, start);
		__build_goto_table(tbl, &set);

		tbl->row = set.count;
		tbl->col = tbl->term_set.count;

		tbl->actions = AR_NEWARR0(psrAction_t*, tbl->row * tbl->col);


		for(i = 0; i < tbl->row; ++i)
		{
				size_t k;
				lalrState_t *state;
				state = set.set[i];
				
				for(k = 0; k <	state->count; ++k)
				{
						const lalrConfig_t *config;
						const psrSymb_t	  *body;
						const psrRule_t	  *rule;
						size_t			  rule_num;
						psrAction_t		  record;
						const lalrAction_t	  *action;
						
						action = &state->actions[k];

						config = action->config;
						rule = action->config->rule;
						rule_num = PSR_IndexOfGrammar(grammar, rule);
						body = PSR_IndexOfSymbList(&rule->body, config->delim);

						
						
						AR_memset(&record, 0, sizeof(record));

						record.rule_num = rule_num;
						record.delim = config->delim;
						record.prec = PSR_GetRulePrecAssocInfo(grammar, rule)->prec;
						record.reduce_count = 0;
						record.shift_to = 0;
						record.next = NULL;


						if(action->act_type == LALR_ACT_REDUCE || action->act_type == LALR_ACT_ACCEPT)
						{
								int_t idx;
								AR_ASSERT(body == NULL && action->to == NULL);
								

								record.reduce_count = rule->body.count;
								record.type = action->act_type == LALR_ACT_REDUCE ? PSR_REDUCE : PSR_ACCEPT;
								
								idx = PSR_FindFromSymbList(&tbl->term_set, action->symb);
								AR_ASSERT(idx != -1);

								__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &record, PSR_GetTermSymbInfo(grammar,action->symb));


						}else if(action->act_type == LALR_ACT_SHIFT && action->symb->type == PSR_TERM)
						{
								int_t trans_to_idx,idx;
								
								AR_ASSERT(action->config != NULL && action->to != NULL && action->symb != NULL);
								
								AR_ASSERT(PSR_CompSymb(body, action->symb) == 0);

								record.type = PSR_SHIFT;


								trans_to_idx = PSR_IndexOfStateSet(&set, action->to);
								AR_ASSERT(trans_to_idx != -1);
								record.shift_to = (size_t)trans_to_idx;
								idx = PSR_FindFromSymbList(&tbl->term_set, action->symb);
								__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &record, PSR_GetTermSymbInfo(grammar,action->symb));
								
						}

				}
		}

		
		PSR_DestroyState_ALL(start);
		PSR_UnInitStateSet(&set);

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

const psrActionTable_t* PSR_CreateActionTable_SLR(const psrGrammar_t *grammar)
{
		return __create_action_table(grammar, PSR_SLR);
}


const psrActionTable_t* PSR_CreateActionTable_LALR(const psrGrammar_t *grammar)
{
		return __create_action_table(grammar, PSR_LALR);
}


/********************************************************************************************************************************************/




AR_NAMESPACE_END

