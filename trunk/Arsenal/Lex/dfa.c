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
/************************************************DFA Vertex**********************************************************************/



lexDFAVertex_t*	LEX_CreateDFAVertex()
{
		lexDFAVertex_t*res;
		res = AR_NEW0(lexDFAVertex_t);

		LEX_InitPositionSet(&res->pos_set);
		return res;

}



static void __insert_start_arc(lexDFAVertex_t *vtx, const lexDFAArc_t *arc)
{
		AR_ASSERT(vtx != NULL && arc != NULL && arc->from == vtx);
		if(vtx->out_count == vtx->out_cap)
		{
				vtx->out_cap = (vtx->out_cap + 1)*2;
				vtx->out = AR_REALLOC(lexDFAArc_t, vtx->out, vtx->out_cap);
		}

		vtx->out[vtx->out_count++] = *arc;

}


static void __insert_end_arc(lexDFAVertex_t *vtx, const lexDFAArc_t *arc)
{
		AR_ASSERT(vtx != NULL && arc != NULL && arc->to == vtx);
		if(vtx->in_count == vtx->in_cap)
		{
				vtx->in_cap = (vtx->in_cap + 1)*2;
				vtx->in = AR_REALLOC(lexDFAArc_t, vtx->in, vtx->in_cap);
		}

		vtx->in[vtx->in_count++] = *arc;
}




void	LEX_ConnectDFAVertex(lexDFAVertex_t* from, lexDFAVertex_t* to, size_t match_id)
{
		lexDFAArc_t			arc;

#if defined(AR_DEBUG)
		size_t i;

		for(i = 0; i < from->out_count; ++i)
		{
				AR_ASSERT(from->out[i].match_id != match_id);
		}
#endif

		arc.from = from;
		arc.to = to;
		arc.match_id = match_id;
		__insert_start_arc(from, &arc);
		__insert_end_arc(to, &arc);


}





void			LEX_DisConnectVertex(lexDFAVertex_t *vtx, lexDFAArc_t *arc)
{
		size_t i;
		for(i = 0; i < vtx->out_count; ++i)
		{
				if(vtx->out[i].match_id == arc->match_id && vtx->out[i].from == arc->from && vtx->out[i].to == arc->to)
				{
						break;
				}
		}
		
		if(i >= vtx->out_count)return;

		while(i < vtx->out_count - 1)
		{
				vtx->out[i] = vtx->out[i + 1];
				i++;
		}
		
		vtx->out_count--;


		for(i = 0; i < arc->to->in_count; ++i)
		{
				if(arc->to->in[i].match_id == arc->match_id && arc->to->in[i].to == arc->to && arc->to->in[i].from == arc->from)
				{
						break;
				}
		}
		
		AR_ASSERT(i < arc->to->in_count);


		while(i < arc->to->in_count - 1)
		{
				arc->to->in[i] = arc->to->in[i + 1];
				i++;
		}
		
		arc->to->in_count--;
}




void			LEX_DestroyDFAVertex(lexDFAVertex_t* vtx)/*会先调用LEX_DisConnectVertex清除一切相关连的边，之后调用AR_DEL清理vtx内存*/
{
		lexDFAArc_t arc;

		while(vtx->out_count > 0)
		{
				arc = vtx->out[0];
				LEX_DisConnectVertex(vtx, &arc);
		}
		

		while(vtx->in_count > 0)
		{
				arc = vtx->in[0];
				AR_ASSERT(arc.to == vtx);
				LEX_DisConnectVertex(arc.from, &arc);
		}
		
		AR_DEL(vtx->in);
		AR_DEL(vtx->out);
		AR_DEL(vtx->action);
		LEX_UnInitPositionSet(&vtx->pos_set);
		AR_DEL(vtx);
}



lexDFAVertex_t* LEX_GetNextVertex(lexDFAVertex_t *vtx, size_t match_id)
{
		size_t i;

		for(i = 0; i < vtx->out_count; ++i)
		{
				if(vtx->out[i].match_id == match_id)return vtx->out[i].to;
		}

		return NULL;
}		












/************************************************DFA_SET**********************************************************************/

void	LEX_InitDFAVertexSet(lexDFAVertexSet_t *set)
{
		
		AR_MSET0(set, sizeof(*set));


}


void	LEX_UnInitDFAVertexSet(lexDFAVertexSet_t *set)
{
		AR_ASSERT(set != NULL);
		AR_DEL(set->set);
		AR_MSET0(set, sizeof(*set));
}


void	LEX_InsertDFAVertexSet(lexDFAVertexSet_t *set, lexDFAVertex_t *vtx)
{
		AR_ASSERT(set != NULL && vtx != NULL);

		if(set->count == set->cap)
		{
				set->cap = (set->cap + 1)*2;
				set->set = AR_REALLOC(lexDFAVertex_t*, set->set, set->cap);
		}
		set->set[set->count++] = vtx;
}


void	LEX_CollectDFAVertex(lexDFAVertexSet_t *empty_set, lexDFAVertex_t *start_vtx)
{
		size_t i,j,k;

		lexDFAVertex_t	*tmp, *out;

		AR_ASSERT(empty_set != NULL && start_vtx != NULL && empty_set->count == 0);
		
		LEX_InsertDFAVertexSet(empty_set, start_vtx);

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
								LEX_InsertDFAVertexSet(empty_set, out);
						}
				}
		}
}





/******************************************************************************************************************************/

void			LEX_DestroyDFA(lexDFAVertex_t  *dfa)
{
		lexDFAVertexSet_t	vtx_set; 
		size_t i;
		LEX_InitDFAVertexSet(&vtx_set);
		LEX_CollectDFAVertex(&vtx_set, dfa);
		AR_ASSERT(vtx_set.count > 0);

		for(i = 0; i < vtx_set.count; ++i)
		{
				LEX_DestroyDFAVertex(vtx_set.set[i]);
		}
		LEX_UnInitDFAVertexSet(&vtx_set);
		
}

static void __handle_final_state(lexDFAVertexSet_t *vtx_set, const lexLeafSet_t *leafset)
{
		size_t i;
		for(i = 0; i < vtx_set->count; ++i)
		{

				size_t j;
				vtx_set->set[i]->act_count = 0;
				for(j = 0; j < vtx_set->set[i]->pos_set.count; ++j)
				{
						size_t p;
						p = vtx_set->set[i]->pos_set.pos[j];
						if(leafset->leaf[p]->t == LEX_FINAL)
						{
								vtx_set->set[i]->act_count++;
						}
				}

				if(vtx_set->set[i]->act_count > 0)
				{
						size_t k;
						vtx_set->set[i]->is_final = True;
						vtx_set->set[i]->action = AR_NEWARR0(lexAction_t, vtx_set->set[i]->act_count);
						
						for(k = 0, j = 0;  j < vtx_set->set[i]->pos_set.count; ++j)
						{

								size_t p;
								p = vtx_set->set[i]->pos_set.pos[j];

								if(leafset->leaf[p]->t == LEX_FINAL)
								{
										vtx_set->set[i]->action[k++] = leafset->leaf[p]->action;
								}

						}

				}


		}
}


lexDFAVertex_t* LEX_BuildDFA(const lexCClass_t *cclass, const lexLeafSet_t *leafset)
{
		lexDFAVertex_t *start_vtx;

		lexDFAVertexSet_t	vtx_set; 
		lexPositionSet_t	state_set;

		size_t	i,j;
		
		AR_ASSERT(cclass != NULL && leafset != NULL && cclass->count > 0 && leafset->count > 0);
		
		LEX_InitDFAVertexSet(&vtx_set);
		
		LEX_InitPositionSet(&state_set);
		
		start_vtx	= LEX_CreateDFAVertex();

		LEX_UnionPositionSet(&start_vtx->pos_set, &leafset->root_first);

		LEX_InsertDFAVertexSet(&vtx_set, start_vtx);

		for(i = 0; i < vtx_set.count; ++i)
		{
				lexDFAVertex_t *next_vtx;
				size_t ccid;
				lexDFAVertex_t *tmp_vtx;
				
				tmp_vtx = vtx_set.set[i];
				
				for(ccid = 0; ccid < cclass->count; ++ccid)/*对于所有符号*/
				{
						LEX_ClearPositionSet(&state_set);

						/*遍历当前未标记状态tmp_vtx
						  得到此状态中与ccid相同的位置p，并求所有p的follow的并集
						*/
						for(j = 0; j < tmp_vtx->pos_set.count; ++j)
						{
								
								size_t p = tmp_vtx->pos_set.pos[j];
								/*LEX_FINAL的follow_set永远为空*/
								AR_ASSERT(leafset->leaf[p]->t == LEX_FINAL || leafset->leaf[p]->t == LEX_CCLASS_ID);
								if(leafset->leaf[p]->t != LEX_FINAL && leafset->leaf[p]->cclass_id == ccid)
								{
										LEX_UnionPositionSet(&state_set, &leafset->follow_set[p]);
								}
						}
						if(state_set.count == 0)continue;
						LEX_SortPositionSet(&state_set);
						LEX_UniquePositionSet(&state_set);

						
						next_vtx = NULL;

						for(j = 0; j < vtx_set.count; ++j)/*这里对vtx_set的搜索并不在意此状态是否是未标记的，只在意是不是有相同的状态*/
						{
								if(LEX_ComparePositionSet(&vtx_set.set[j]->pos_set, &state_set))
								{
										next_vtx = vtx_set.set[j];
								}
						}

						if(next_vtx == NULL)
						{
								next_vtx = LEX_CreateDFAVertex();
								LEX_UnionPositionSet(&next_vtx->pos_set, &state_set);
								LEX_InsertDFAVertexSet(&vtx_set, next_vtx);
						}
						/*此vertex到next_vtx，有一条标记为ccid的转移弧，next_vtx同样可能是自身*/
						LEX_ConnectDFAVertex(tmp_vtx, next_vtx, ccid);
				}
		}

		
		__handle_final_state(&vtx_set, leafset);
		

		LEX_UnInitDFAVertexSet(&vtx_set);
		LEX_UnInitPositionSet(&state_set);
		return start_vtx;
}

/*********************************************************State_Table********************/

void LEX_InitStateTable(lexStateTable_t *tbl)
{
		AR_MSET0(tbl, sizeof(*tbl));

}
void LEX_UnInitStateTable(lexStateTable_t *tbl)
{
		if(tbl == NULL)return;

		AR_DEL(tbl->actions);
		AR_DEL(tbl->tbl);
		AR_DEL(tbl->is_final);
		AR_MSET0(tbl, sizeof(*tbl));
}

#define __POS(_x,_y,_col) ((_x) * (_col) + (_y))
void LEX_BuildStateTable(lexStateTable_t *tbl, lexDFAVertex_t *dfa, const lexCClass_t *cclass)
{
		lexDFAVertexSet_t set;
		size_t i,k;
		LEX_InitDFAVertexSet(&set);
		LEX_CollectDFAVertex(&set, dfa);
		
		tbl->actions = AR_NEWARR0(lexAction_t, set.count);
		tbl->is_final = AR_NEWARR0(bool_t, set.count);
		tbl->count = set.count;

		for(i = 0; i < set.count; ++i)
		{
				if(set.set[i]->is_final)
				{
						AR_ASSERT(set.set[i]->act_count > 0);
						if(set.set[i]->act_count > 1)
						{
								size_t act_id;
								lexDFAVertex_t *accepted = set.set[i]; 
								for(act_id = 0, i = 0; i < accepted->act_count; ++i)
								{
										if(accepted->action[act_id].priority < accepted->action[i].priority)
										{
												act_id = i;
										}
								}
								if(act_id > 0)
								{
										lexAction_t tmp = accepted->action[0];
										accepted->action[0] = accepted->action[act_id];
										accepted->action[act_id] = tmp;
								}
						}

						tbl->actions[i] = set.set[i]->action[0];
						tbl->is_final[i] = True;
				}else
				{
						tbl->is_final[i] = False;
				}
		}
		
		tbl->row = set.count;
		tbl->col = cclass->count;

		tbl->tbl = AR_NEWARR0(int, tbl->row * tbl->col);
		AR_MSET(tbl->tbl, -1, tbl->row * tbl->col * sizeof(int));
		
		for(i = 0; i < tbl->row; ++i)
		{
				for(k = 0; k < set.set[i]->out_count; ++k)
				{
						
						lexDFAVertex_t *to;
						size_t x,y,t; 
						x = i;
						y = set.set[i]->out[k].match_id;
						to = set.set[i]->out[k].to;
						
						for(t = 0; t < set.count; ++t)
						{
								if(set.set[t] == to)
								{
										break;
								}
						}
						AR_ASSERT(t < set.count);

						tbl->tbl[__POS(x,y,tbl->col)] = (int)t;
/*
						printf("state[%d] out edge to== state[%d]", i,t);
						if(tbl->is_final[t])printf(" : final");
						printf("\n");
*/
				}
		}
	
		/*	printf("tbl size == %d : %d\n", tbl->row, tbl->col);*/
		LEX_UnInitDFAVertexSet(&set);
}

#undef __POS


AR_NAMESPACE_END


