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


#include "grammar.h"
#include "lalr.h"
#include "lr_action.h"
#include "parser_aux.h"
#include "parser.h"




AR_NAMESPACE_BEGIN



static void __insert_to_conflict_set(psrConflictView_t *view, psrConflictItem_t *item)
{
		AR_ASSERT(view != NULL && item != NULL);

		if(view->count == view->cap)
		{
				view->cap = (view->cap + 4)*2;
				view->conflict = AR_REALLOC(psrConflictItem_t*, view->conflict, view->cap);
		}
		view->conflict[view->count++] = item;
}


const	psrConflictView_t*		PSR_CreateConflictView(const psrActionTable_t *tbl, const psrGrammar_t *grammar)
{
		psrConflictView_t		*view;
		arString_t				*str = NULL;
		size_t i,k;
		AR_ASSERT(tbl != NULL && grammar != NULL);
	
		str = AR_CreateString();
		view = AR_NEW0(psrConflictView_t);
		
		
		for(i = 0; i < tbl->row; ++i)
		{
				size_t					l = 0;
				psrConflictItem_t		*item = NULL;

				for(k = 0; k < tbl->col; ++k)
				{
						
						const psrAction_t		*act = tbl->actions[AR_TBL_IDX_R(i,k,tbl->col)];
						//const psrRule_t			*rule = PSR_GetRuleOfGrammar(grammar, act->rule_num);
						
						if(act->next == NULL)continue;
						
						AR_ClearString(str);
						item = AR_NEW0(psrConflictItem_t);
						AR_AppendFormatString(str,L"state[%" AR_PLAT_INT_FMT L"d] : %ls",(size_t)i, tbl->term_set.lst[k]->name);
						item->name = AR_wcsdup(AR_GetStrString(str));
						AR_ClearString(str);

						while(act != NULL)
						{
								item->count++;
								act = act->next;
						}

						item->items = AR_NEWARR0(wchar_t*, item->count);

						
						
						for(l = 0, act = tbl->actions[AR_TBL_IDX_R(i,k,tbl->col)]; l < item->count; ++l, act = act->next)
						{
								/*
								psrLRItem_t tmp;
								*/
								lalrConfig_t	tmp;
								const psrRule_t			*rule = PSR_GetRuleOfGrammar(grammar, act->rule_num);
								AR_ASSERT(act != NULL);
								
								PSR_InitConfig(&tmp, rule, act->delim);
								/*
								PSR_InitLRItem(&tmp, rule, act->delim);
								*/

								AR_ClearString(str);
								switch(act->type)
								{
								case PSR_REDUCE:
										AR_AppendFormatString(str,L"Reduce: ");
										PSR_PrintConfig(&tmp,grammar,str);
										break;
								case PSR_SHIFT:
										AR_AppendFormatString(str,L"Shift: ");
										PSR_PrintConfig(&tmp, grammar,str);
										break;
								case PSR_ACCEPT:
										AR_AppendFormatString(str,L"Accept ");
										break;
								default:
										AR_ASSERT(false);
								}
								/*AR_AppendFormatString(str,L"\r\n");*/

								/*PSR_UnInitLRItem(&tmp);*/
								PSR_UnInitConfig(&tmp);
								item->items[l] = AR_wcsdup(AR_GetStrString(str));

						}
						
						__insert_to_conflict_set(view, item);
				}
		}

		AR_DestroyString(str);
		
		return view;
}

void							PSR_DestroyConflictView(const psrConflictView_t *view)
{
		
		psrConflictView_t		*v = (psrConflictView_t*)view;

		if(v != NULL)
		{
				size_t i,k;
				for(i = 0; i < view->count; ++i)
				{
						psrConflictItem_t		*item = view->conflict[i];
						
						AR_DEL(item->name);
						for(k = 0; k < item->count; ++k)
						{
								AR_DEL(item->items[k]);
						}
						AR_DEL(item->items);
						AR_DEL(item);
				}

				AR_DEL(v->conflict);
				AR_DEL(v);
		}

}



const psrActionView_t*	PSR_CreateActionView(const psrActionTable_t *tbl, const psrGrammar_t *grammar)
{
		psrActionView_t		*view;
		size_t r, c, k;
		wchar_t *msg;
		AR_ASSERT(tbl != NULL && grammar != NULL);

		view = AR_NEW0(psrActionView_t);
		
		view->item_cnt = tbl->term_set.count + tbl->nonterm_set.count;

		view->item = AR_NEWARR0(wchar_t*, view->item_cnt);

		
		c = 0;
		for(k = 0; k < tbl->term_set.count; ++k)
		{
				view->item[c] = AR_vtow(L"%ls", tbl->term_set.lst[k]->name);
				c++;
		}

		for(k = 0; k < tbl->nonterm_set.count; ++k)
		{
				view->item[c] = AR_vtow(L"<%ls>", tbl->nonterm_set.lst[k]->name);
				c++;
		}

		AR_ASSERT(c == view->item_cnt);
		

		view->row = tbl->row;
		view->col = tbl->col + tbl->goto_col;

		AR_ASSERT(view->col == view->item_cnt);

		view->action_tbl = AR_NEWARR0(wchar_t*, view->row * view->col);
		
		/*construct action view*/

		for(r = 0; r < tbl->row; ++r)
		{
				c = 0;
				msg = NULL;
				for(k = 0; k < tbl->col; ++k)
				{
						
						const psrAction_t *action = tbl->actions[AR_TBL_IDX_R(r, k, tbl->col)];
						const psrRule_t	 *rule = PSR_GetRuleOfGrammar(grammar, action->rule_num);
						

						switch(action->type)
						{
						case PSR_ACCEPT:
								msg = AR_wcsdup(L"accept");
								break;
						case PSR_SHIFT:
								{
										msg = AR_vtow(L"%" AR_PLAT_INT_FMT L"d", (size_t)action->shift_to);
								}
								break;
						case PSR_REDUCE:
								{
										msg = AR_vtow(L"[<%ls>:%" AR_PLAT_INT_FMT L"d]", rule->head->name, (size_t)action->reduce_count);
								}
								break;
						default:
								msg = AR_vtow(L"error");
								break;
						}
						
						view->action_tbl[AR_TBL_IDX_R(r, c, view->col)] = msg;
						c++;
						msg = NULL;
				}

				for(k = 0; k < tbl->goto_col; ++k)
				{
						ar_int_t state =  tbl->goto_tbl[AR_TBL_IDX_R(r, k, tbl->goto_col)];
						msg = AR_vtow(L"%" AR_PLAT_INT_FMT L"d", state);

						view->action_tbl[AR_TBL_IDX_R(r, c, view->col)] = msg;
						c++;
						msg = NULL;
				}
				AR_ASSERT(c == view->col);
		}

		AR_ASSERT(r == view->row);
		return view;
}


void PSR_DestroyActionView(const psrActionView_t *action_view)
{
		size_t i;
		psrActionView_t *view;
		AR_ASSERT(action_view != NULL);

		view = (psrActionView_t*)action_view;

		for(i = 0; i < view->item_cnt; ++i)
		{
				AR_DEL(view->item[i]);
		}

		for(i = 0; i < view->row * view->col; ++i)
		{
				AR_DEL(view->action_tbl[i]);
		}
		
		if(view->action_tbl)
		{
				AR_DEL(view->action_tbl);
		}

		if(view->item)
		{
				AR_DEL(view->item);
		}
		AR_DEL(view);
}



void PSR_PrintActionTable(const psrActionTable_t *tbl, const psrGrammar_t *grammar, size_t width, arString_t *str)
{
		/*这里必须用int，因为printf一族函数的对于%*d这类width的定义就是int*/
		int __WIDTH__;
		size_t i,j;
		wchar_t buf[1024];

		__WIDTH__ = (int)width;
		
		AR_AppendString(str,L"TermSet:\r\n");
		for(i = 0; i < tbl->term_set.count; ++i)
		{
				PSR_PrintSymbol(tbl->term_set.lst[i],str);
				AR_AppendString(str,L"  ");
		}
		AR_AppendString(str,L"\r\n");
		AR_AppendString(str, L"-----------------------------------------\r\n");
		AR_AppendString(str,L"NonTermSet:\r\n");
		for(i = 0; i < tbl->nonterm_set.count; ++i)
		{
				PSR_PrintSymbol(tbl->nonterm_set.lst[i],str);
				AR_AppendString(str,L"  ");
		}

		AR_AppendString(str,L"\r\n");
		AR_AppendString(str,L"-----------------------------------------\r\n");

		
		AR_AppendString(str,L"Goto Table:\r\n");
		
		AR_AppendFormatString(str, L"%*ls", __WIDTH__,L"NULL");
		for(i = 0; i < tbl->nonterm_set.count; ++i)
		{
				AR_swprintf(buf, 1024, L"<%ls>", tbl->nonterm_set.lst[i]->name);
				AR_AppendFormatString(str, L"%*ls",__WIDTH__, buf);
		}

		AR_AppendString(str,L"\r\n");
		AR_AppendString(str,L"\r\n");
		for(i = 0; i < tbl->goto_row; i++)
		{
				AR_swprintf(buf, 1024, L"I[%" AR_PLAT_INT_FMT L"d]", (size_t)i);
				AR_AppendFormatString(str, L"%*ls:", __WIDTH__,buf);
				for(j = 0; j < tbl->goto_col; ++j)
				{
						AR_AppendFormatString(str, L"%*" AR_PLAT_INT_FMT L"d", __WIDTH__, tbl->goto_tbl[AR_TBL_IDX_R(i,j,tbl->goto_col)]);
						/*
						ar_int_t xxx,xxx_idx;
						
						xxx_idx = AR_TBL_IDX_R(i,j,tbl->goto_col);
						xxx = tbl->goto_tbl[xxx_idx];
						printf("xxx_idx == %I64d, xxx == %I64d\r\n", xxx_idx, xxx);
						AR_AppendFormatString(str, L"%*I64d", __WIDTH__, (ar_int64_t)tbl->goto_tbl[AR_TBL_IDX_R(i,j,tbl->goto_col)]);
						*/
				}
				AR_AppendString(str,L"\r\n");
		}

		AR_AppendString(str,L"-----------------------------------------\r\n");

		AR_AppendString(str,L"Action Table:\r\n");
		
		AR_AppendFormatString(str,L"%*ls", __WIDTH__,L"NULL");
		for(i = 0; i < tbl->term_set.count; ++i)
		{
				AR_AppendFormatString(str,L"%*ls",__WIDTH__, tbl->term_set.lst[i]->name);
		}
		AR_AppendFormatString(str,L"\r\n");
		AR_AppendFormatString(str,L"\r\n");


		for(i = 0; i < tbl->row; ++i)
		{
				AR_swprintf(buf, 1024, L"I[%d]", (ar_uint32_t)i);
				AR_AppendFormatString(str,L"%*ls:", __WIDTH__,buf);
				for(j = 0; j < tbl->col; ++j)
				{
						const psrAction_t *pact;
						const psrRule_t	 *rule;
						pact = tbl->actions[AR_TBL_IDX_R(i,j, tbl->col)];
						rule = PSR_GetRuleOfGrammar(grammar, pact->rule_num);

						switch(pact->type)
						{
						case PSR_ACCEPT:
								AR_AppendFormatString(str,L"%*ls", __WIDTH__,L"accept");
								break;
						case PSR_SHIFT:
						{
								AR_AppendFormatString(str,L"%*" AR_PLAT_INT_FMT L"d", __WIDTH__, (size_t)pact->shift_to);
						}
								break;
						case PSR_REDUCE:
						{
								AR_swprintf(buf, 1024, L"[<%ls>:%" AR_PLAT_INT_FMT L"d]",rule->head->name, (size_t)pact->reduce_count);
								AR_AppendFormatString(str,L"%*ls", __WIDTH__,buf);
								
						}
								break;
						default:
								AR_AppendFormatString(str,L"%*ls", __WIDTH__,L"error");
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
		const psrRule_t	 *rule;
		AR_AppendFormatString(str,L"%ls\r\n", L"Conflict:");
		for(i = 0; i < tbl->row; ++i)
		{
				for(j = 0; j < tbl->col; ++j)
				{
						action = tbl->actions[AR_TBL_IDX_R(i,j,tbl->col)];
						rule = PSR_GetRuleOfGrammar(grammar, action->rule_num);
						AR_ASSERT(action != NULL);
						if(action->next == NULL)continue;


						AR_AppendFormatString(str,L"state[%" AR_PLAT_INT_FMT L"d] : %ls\r\n",(size_t)i, tbl->term_set.lst[j]->name);
						
						while(action != NULL)
						{
								
								lalrConfig_t	tmp;

								/*
								psrLRItem_t tmp;
								
								PSR_InitLRItem(&tmp, rule, action->delim);
								*/
								PSR_InitConfig(&tmp, rule, action->delim);
								
								switch(action->type)
								{
								case PSR_REDUCE:
										AR_AppendFormatString(str,L"Reduce: ");
										PSR_PrintConfig(&tmp, grammar, str);
										
										/*PSR_PrintLRItem(&tmp,grammar,str);*/

										break;
								case PSR_SHIFT:
										AR_AppendFormatString(str,L"Shift: ");
										PSR_PrintConfig(&tmp, grammar,str);
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
								
								PSR_UnInitConfig(&tmp);
								/*
								PSR_UnInitLRItem(&tmp);
								*/
						}

						AR_AppendFormatString(str,L"\r\n");
						AR_AppendFormatString(str,L"-----------------------------------------------\r\n");
				}
		}
}


size_t PSR_CountConflict(const psrActionTable_t *tbl)
{
		
		size_t i, j, count;
		const psrAction_t *action;

		AR_ASSERT(tbl != NULL);
		count = 0;
		for(i = 0; i < tbl->row; ++i)
		{
				for(j = 0; j < tbl->col; ++j)
				{
						action = tbl->actions[AR_TBL_IDX_R(i,j,tbl->col)];
						AR_ASSERT(action != NULL);
						if(action->next != NULL)count++;
				}
		}
		return  count;
}


/**********************************************************************/


/************************Report*********************************/


size_t	PSR_CountParserConflict(const parser_t *parser)
{
		AR_ASSERT(parser != NULL);
		AR_ASSERT(parser->tbl != NULL && parser->grammar != NULL);

		return PSR_CountConflict(parser->tbl);
}

void	PSR_PrintParserConflict(const parser_t *parser, arString_t *out)
{
		AR_ASSERT(parser != NULL && out != NULL);
		AR_ASSERT(parser->tbl != NULL && parser->grammar != NULL);
		
		PSR_ReportConflict(parser->tbl, parser->grammar, out);
}

void	PSR_PrintParserActionTable(const parser_t *parser, arString_t *out, size_t width)
{
		AR_ASSERT(parser != NULL && out != NULL);
		AR_ASSERT(parser->tbl != NULL && parser->grammar != NULL);

		PSR_PrintActionTable(parser->tbl, parser->grammar, width, out);
}



const psrActionView_t* PSR_CreateParserActionView(const parser_t *parser)
{
		AR_ASSERT(parser != NULL);
		return PSR_CreateActionView(parser->tbl, parser->grammar);

}

void					PSR_DestroyParserActionView(const psrActionView_t *view)
{
		AR_ASSERT(view != NULL);
		PSR_DestroyActionView(view);

}



const	psrConflictView_t*		PSR_CreateParserConflictView(const parser_t *parser)
{
		AR_ASSERT(parser != NULL);

		return PSR_CreateConflictView(parser->tbl, parser->grammar);
}


void							PSR_DestroyParserConflictView(const psrConflictView_t *view)
{
		AR_ASSERT(view != NULL);

		PSR_DestroyConflictView(view);
}

/*
typedef struct __sym_tbl_view_tag
{
		wchar_t	**name;
		wchar_t	**name_set;
		size_t	count;
		size_t	cap;
}psrSymbolMapView_t;

typedef struct __first_follow_view_tag
{
		psrSymbolMapView_t		first_set;
		psrSymbolMapView_t		follow_set;
}psrFirstFollowView_t;
*/

static void __insert_to_symtbl_view(psrSymbolMapView_t *map_view, const wchar_t *name, const wchar_t *set)
{
		AR_ASSERT(map_view != NULL && name != NULL && set != NULL);

		if(map_view->count == map_view->cap)
		{
				map_view->cap = (map_view->cap + 4) * 2;
				map_view->name = AR_REALLOC(wchar_t*, map_view->name, map_view->cap);
				map_view->name_set = AR_REALLOC(wchar_t*, map_view->name_set, map_view->cap);
		}

		map_view->name[map_view->count]		= AR_wcsdup(name);
		map_view->name_set[map_view->count] = AR_wcsdup(set);
		map_view->count++;
}




static ar_bool_t __detect_left_recursion(const psrGrammar_t *grammar, const psrSymb_t *head, psrSymbList_t *lst, psrSymbolMapView_t *output)
{
		size_t i;
		ar_bool_t is_recu = false;
		AR_ASSERT(grammar != NULL && head != NULL && lst != NULL);

		/*AR_ASSERT(lst->count > 0);*/

		AR_ASSERT(PSR_FindFromSymbList(lst, head) == -1);
		
		PSR_InsertToSymbList(lst, head);
		
		
		
		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule;
				rule = grammar->rules[i];
				
				if(PSR_CompSymb(rule->head, head) == 0)
				{
						const psrSymb_t *symb;

						size_t x = 0;

						
						if(x < rule->body.count && rule->body.lst[x]->type == PSR_NONTERM)
						{
								symb = rule->body.lst[x];

								if(PSR_CompSymb(lst->lst[0], symb) == 0)
								{
										is_recu = true;
										if(output)
										{
												size_t cnt;
												
												arString_t *str = AR_CreateString();

												for(cnt = 0; cnt < lst->count; ++cnt)
												{
														AR_AppendFormatString(str, L"<%ls> -> ", lst->lst[cnt]->name);
												}
												AR_AppendFormatString(str, L"<%ls> ", lst->lst[0]->name);
												
												__insert_to_symtbl_view(output, L"Path:", AR_GetStrString(str));

												AR_DestroyString(str);

										}

								}else if(PSR_FindFromSymbList(lst, symb) != -1)
								{
										continue;
								}else
								{
										__detect_left_recursion(grammar, symb, lst,output);
								}
						}
				}
		}
		
		PSR_RemoveFromSymbListByIndex(lst, lst->count-1);
		return is_recu;
		
}


ar_bool_t					__report_left_recursion(const psrGrammar_t *grammar, psrSymbolMapView_t *output)
{
		size_t i;
		psrSymbList_t	lst;
		ar_bool_t			ret = false;
		AR_ASSERT(grammar != NULL);
		
		PSR_InitSymbList(&lst);
		
		for(i = 0; i < grammar->symb_list.count; ++i)
		{
				const psrSymb_t *symb;
				
				symb = grammar->symb_list.lst[i];

				PSR_ClearSymbList(&lst);
				
				if(symb->type == PSR_NONTERM)
				{
						if(__detect_left_recursion(grammar, symb, &lst,output))
						{
								ret = true;
						}
				}
		}

		PSR_UnInitSymbList(&lst);
		return ret;
}






const psrFirstFollowView_t*		PSR_CreateParserFirstFollowView(const parser_t *parser)
{
		psrSymbMap_t	first, follow;
		arString_t		*str;
		psrSymbolMapView_t		first_view, follow_view, left_recu;
		psrFirstFollowView_t	*ret;
		size_t i;
		
		AR_ASSERT(parser != NULL && parser->grammar != NULL);

		AR_memset(&first_view, 0, sizeof(first_view));
		AR_memset(&follow_view, 0, sizeof(follow_view));
		
		AR_memset(&left_recu, 0, sizeof(left_recu));

		ret = NULL;

		PSR_InitSymbMap(&first);
		PSR_InitSymbMap(&follow);

		PSR_CalcFirstSet(parser->grammar, &first);
		PSR_CalcFollowSet(parser->grammar, &follow, &first);

		str = AR_CreateString();

		for(i = 0; i < parser->grammar->symb_list.count; ++i)
		{
				const psrMapRec_t		*rec;
				const psrSymb_t	*symb = parser->grammar->symb_list.lst[i];
				
				rec = PSR_GetSymbolFromSymbMap(&first, symb);
				
				AR_ClearString(str);
				if(rec)
				{
						
						PSR_PrintSymbolList(&rec->lst, str);
						
				}
				__insert_to_symtbl_view(&first_view, rec->key->name, AR_GetStrString(str));
		}


		for(i = 0; i < parser->grammar->symb_list.count; ++i)
		{
				const psrMapRec_t		*rec;
				const psrSymb_t	*symb = parser->grammar->symb_list.lst[i];
				
				if(symb->type == PSR_NONTERM)
				{
						rec = PSR_GetSymbolFromSymbMap(&follow, symb);
						
						AR_ClearString(str);
						if(rec)
						{
								
								PSR_PrintSymbolList(&rec->lst, str);
								
						}
						__insert_to_symtbl_view(&follow_view, rec->key->name, AR_GetStrString(str));
				}
		}

		__report_left_recursion(parser->grammar, &left_recu);

		ret = AR_NEW0(psrFirstFollowView_t);
		ret->first_set = first_view;
		ret->follow_set = follow_view;
		ret->left_recursion = left_recu;
		
		PSR_UnInitSymbMap(&first);
		PSR_UnInitSymbMap(&follow);
		if(str)AR_DestroyString(str);
		return ret;
}


void							PSR_DestroyParserFirstFollowView(const psrFirstFollowView_t *follow_view)
{
		size_t i;

		if(follow_view != NULL)
		{
				psrFirstFollowView_t *view = (psrFirstFollowView_t*)follow_view;

				for(i = 0; i < view->first_set.count; ++i)
				{
						AR_DEL(view->first_set.name[i]);
						AR_DEL(view->first_set.name_set[i]);
				}

				AR_DEL(view->first_set.name);
				AR_DEL(view->first_set.name_set);


				for(i = 0; i < view->follow_set.count; ++i)
				{
						AR_DEL(view->follow_set.name[i]);
						AR_DEL(view->follow_set.name_set[i]);
				}

				AR_DEL(view->follow_set.name);
				AR_DEL(view->follow_set.name_set);


				for(i = 0; i < view->left_recursion.count; ++i)
				{
						AR_DEL(view->left_recursion.name[i]);
						AR_DEL(view->left_recursion.name_set[i]);
				}

				AR_DEL(view->left_recursion.name);
				AR_DEL(view->left_recursion.name_set);

				AR_DEL(view);
		}

}





AR_NAMESPACE_END
