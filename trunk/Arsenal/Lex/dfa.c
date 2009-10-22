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

#include "dfa.h"




AR_NAMESPACE_BEGIN



lexDFA_t*		LEX_CreateDFA()
{
		return AR_NEW0(lexDFA_t);
}

void			LEX_DestroyDFA(lexDFA_t* vtx)/*会先调用LEX_DisConnectVertex清除一切相关连的边，之后调用AR_DEL清理vtx内存*/
{
		lexDFAArc_t arc;
		AR_ASSERT(vtx != NULL);

		while(vtx->out_count > 0)
		{
				/*arc = vtx->out[0];*/
				arc = vtx->out[vtx->out_count-1];
				LEX_DisConnect(vtx, &arc);
		}
		
		AR_DEL(vtx->out);
		LEX_DestroyNodeList(vtx->pos_lst);
		AR_DEL(vtx);

}


void			LEX_ConnectDFA(lexDFA_t *from, lexDFA_t *to, int_t match_id)
{
		lexDFAArc_t		arc;
		AR_ASSERT(from != NULL && to != NULL);
		arc.to = to;
		arc.match_id = match_id;
		
		if(from->out_count == from->out_cap)
		{
				from->out_cap = (from->out_cap + 4)*2;
				from->out = AR_REALLOC(lexDFAArc_t, from->out, from->out_cap);
		}

		from->out[from->out_count++] = arc;
}

void			LEX_DisConnect(lexDFA_t *vtx, lexDFAArc_t *arc)
{
		size_t i;
		AR_ASSERT(vtx != NULL && arc != NULL);
		
		for(i = 0; i < vtx->out_count; ++i)
		{
				if(vtx->out[i].match_id == arc->match_id)
				{
						break;
				}
		}

		if(i < vtx->out_count)
		{
				while(i < vtx->out_count - 1)
				{
						vtx->out[i] = vtx->out[i + 1];
						i++;
				}
				vtx->out_count--;
		}

}


/***********************************************DFA_Set*****************************************/

void	LEX_InitDFASet(lexDFASet_t *set)
{
		AR_memset(set, 0, sizeof(*set));
}


void	LEX_UnInitDFASet(lexDFASet_t *set)
{
		if(set != NULL)
		{
				AR_DEL(set->set);
				AR_memset(set, 0, sizeof(*set));
		}
}


void	LEX_InsertToDFASet(lexDFASet_t *set, lexDFA_t *vtx)
{
		AR_ASSERT(set != NULL && vtx != NULL);

		if(set->count == set->cap)
		{
				set->cap = (set->cap + 1)*2;
				set->set = AR_REALLOC(lexDFA_t*, set->set, set->cap);
		}
		set->set[set->count++] = vtx;
}

void	LEX_CollectDFA(lexDFASet_t *empty_set, lexDFA_t *start)
{
		size_t i,j,k;

		lexDFA_t	*tmp, *out;

		AR_ASSERT(empty_set != NULL && start != NULL && empty_set->count == 0);
		
		LEX_InsertToDFASet(empty_set, start);

		for(i = 0; i < empty_set->count; ++i)
		{
				tmp = empty_set->set[i];
				for(k = 0; k < tmp->out_count; ++k)
				{
						out = tmp->out[k].to;
						for(j= 0; j < empty_set->count; ++j)
						{
								if(out == empty_set->set[j])
								{
										out = NULL;
										break;
								}
						}

						if(out != NULL)
						{
								LEX_InsertToDFASet(empty_set, out);
						}
				}
		}
}


void			LEX_DestroyDFA_ALL(lexDFA_t *start)
{
		lexDFASet_t set;
		size_t i;

		LEX_InitDFASet(&set);
		LEX_CollectDFA(&set, start);

		
		for(i = 0; i < set.count; ++i)
		{
				LEX_DestroyDFA(set.set[i]);
		}
		
		LEX_UnInitDFASet(&set);
}



lexDFA_t* LEX_BuildDFA(lexCClass_t *cclass, lexLeafSet_t *leafset)
{
		lexDFA_t *start;
		lexDFASet_t set;
		size_t i,j;
		AR_ASSERT(cclass != NULL && leafset != NULL && leafset->leafset != NULL && leafset->root_first != NULL);

		start = LEX_CreateDFA();
		LEX_MergeNodeList(&start->pos_lst, leafset->root_first);
		start->is_final = false;
		
		LEX_InitDFASet(&set);
		LEX_InsertToDFASet(&set, start);

		for(i = 0; i < set.count; ++i)
		{
				size_t ccid;
				lexDFA_t *dfa;
				
				dfa = set.set[i];
				
				
				for(ccid = 0; ccid < cclass->count; ++ccid)
				{
						lexDFA_t *to;
						lexNodeList_t *new_list, *curr;
						new_list = NULL;

						for(curr = dfa->pos_lst; curr; curr = curr->next)
						{
								AR_ASSERT(curr->node->attr.is_leaf);
/*
		经过符号ccid之后将达到follow集合中的节点，因此，这里等于是dfa -> to经过ccid的转移，所以将所有follow集合中的节点
加入到to的nodes集合中，构成新的DFA to;
*/
								if(curr->node->t != LEX_FINAL && curr->node->cclass_id == ccid)
								{
										LEX_MergeNodeList(&new_list, curr->node->attr.follow);
								}
						}

						if(new_list == NULL)continue;

						/*搜索新的集合new_list是否存在于dfa_set中，如果存在，则得到已存在的集合to，否则建立新的集合*/
						to = NULL;
						for(j = 0; j < set.count; ++j)
						{
								if(LEX_CompareNodeList(set.set[j]->pos_lst, new_list) == 0)
								{
										to = set.set[j];
										break;
								}
						}

						if(to != NULL)
						{
								LEX_ConnectDFA(dfa, to, ccid);
								LEX_DestroyNodeList(new_list);
						}else
						{
								lexDFA_t *new_dfa;
								new_dfa = LEX_CreateDFA();
								new_dfa->pos_lst = new_list;
								
								/*
								处理优先级，例如 

								290,0			{identifier} 
								291,1			int

								这里，因为这两个rule的final节点会落在同一个dfa中，因此，会将dfa->action设置为int对应的action;

								注意，这里的{identifier}只有在int对应的状态上才会变成int的type，否则例如出现了intp，则会落在下一个状态上，哪个状态会是intp对应的
								type,在这里是290,0			{identifier} 
								*/

								for(curr = new_list; curr; curr = curr->next)
								{
										if(curr->node->t == LEX_FINAL)
										{
												new_dfa->is_final = true;
												if(curr->node->action.priority >= new_dfa->action.priority)
												{
														new_dfa->action = curr->node->action;
												}
										}
								}
								LEX_ConnectDFA(dfa, new_dfa, ccid);
								LEX_InsertToDFASet(&set, new_dfa);
						}
				}
		}

		LEX_UnInitDFASet(&set);
		return start;
}






/*
typedef struct __lex_state_table_tag
{
		lexAction_t		*actions;
		bool_t			*is_final;
		size_t			count;
		
		int				*tbl;
		size_t			row;
		size_t			col;
}lexStateTable_t;
*/


void LEX_InitStateTable(lexStateTable_t *tbl)
{
		AR_memset(tbl, 0, sizeof(*tbl));

}


void LEX_UnInitStateTable(lexStateTable_t *tbl)
{
		AR_DEL(tbl->actions);
		AR_DEL(tbl->is_final);
		AR_DEL(tbl->tbl);
		AR_memset(tbl, 0, sizeof(*tbl));
}


void LEX_BuildStateTable(lexStateTable_t *tbl, lexDFA_t *dfa, const lexCClass_t *cclass)
{
		lexDFASet_t set;
		size_t i,j,k;
		AR_ASSERT(tbl != NULL && dfa != NULL && cclass != NULL);
		LEX_InitDFASet(&set);
		LEX_CollectDFA(&set, dfa);
		
		tbl->row = set.count;
		tbl->col = cclass->count;
		tbl->count = tbl->row;
		tbl->actions = AR_NEWARR0(lexAction_t, tbl->count);
		tbl->is_final = AR_NEWARR0(bool_t, tbl->count);

		for(i = 0; i < tbl->count; ++i)
		{
				tbl->is_final[i] = set.set[i]->is_final;
				
				if(tbl->is_final[i])
				{
						tbl->actions[i] = set.set[i]->action;
				}
		}
		
		tbl->tbl = AR_NEWARR0(int_t, tbl->row * tbl->col);
		AR_memset(tbl->tbl, -1, tbl->row * tbl->col * sizeof(int_t));

		for(i = 0; i < tbl->row; ++i)
		{
				lexDFA_t *dfa = set.set[i];
				
				for(j = 0; j < tbl->col; ++j)
				{
						
						for(k = 0; k < dfa->out_count; ++k)
						{
								if(dfa->out[k].match_id == (int)j)
								{
										size_t idx;
										for(idx = 0; idx < set.count; ++idx)
										{
												if(set.set[idx] == dfa->out[k].to)
												{
														break;
												}
										}
										AR_ASSERT(idx < set.count);

										tbl->tbl[AR_TBL_IDX_R(i, j, tbl->col)] = (int_t)idx;
								}
						}
						
				}
		}
		LEX_UnInitDFASet(&set);
}









AR_NAMESPACE_END

