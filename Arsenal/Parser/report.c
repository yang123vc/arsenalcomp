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
#include "parser.h"
#include "parser_in.h"



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


static const	psrConflictView_t*		Parser_CreateConflictView(const psrActionTable_t *tbl, const psrGrammar_t *grammar)
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
						/*
						const psrRule_t			*rule = Parser_GetRuleFromGrammar(grammar, act->rule_num);
						*/
						
						if(act->next == NULL)continue;
						
						AR_ClearString(str);
						item = AR_NEW0(psrConflictItem_t);

						

						/*
						AR_AppendFormatString(str,L"state[%" AR_PLAT_INT_FMT L"d] : %ls",(size_t)i, tbl->term_set.lst[k]->name);
						*/
						AR_AppendFormatString(str,L"state[%Id]",(size_t)i);
						item->name = AR_wcsdup(AR_GetStringCString(str));
						AR_ClearString(str);
						item->lookahead = AR_wcsdup(tbl->term_set.lst[k]->name);

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
								/*const psrRule_t			*rule = Parser_GetRuleFromGrammar(grammar, act->rule_num);*/
								AR_ASSERT(act != NULL);
								
								Parser_InitConfig(&tmp, act->rule_num, act->delim, grammar);
								/*
								Parser_InitLRItem(&tmp, rule, act->delim);
								*/

								AR_ClearString(str);
								switch(act->type)
								{
								case PARSER_REDUCE:
										AR_AppendFormatString(str,L"Reduce: ");
										Parser_PrintConfig(&tmp,grammar,str);
										break;
								case PARSER_SHIFT:
										AR_AppendFormatString(str,L"Shift: ");
										Parser_PrintConfig(&tmp, grammar,str);
										break;
								case PARSER_ACCEPT:
										AR_AppendFormatString(str,L"Accept ");
										break;
								default:
										AR_ASSERT(false);
								}
								/*AR_AppendFormatString(str,L"\r\n");*/

								/*Parser_UnInitLRItem(&tmp);*/
								Parser_UnInitConfig(&tmp);
								item->items[l] = AR_wcsdup(AR_GetStringCString(str));

						}
						
						__insert_to_conflict_set(view, item);
				}
		}

		AR_DestroyString(str);
		
		return view;
}

static void							Parser_DestroyConflictView(const psrConflictView_t *view)
{
		
		psrConflictView_t		*v = (psrConflictView_t*)view;

		if(v != NULL)
		{
				size_t i,k;
				for(i = 0; i < view->count; ++i)
				{
						psrConflictItem_t		*item = view->conflict[i];
						
						AR_DEL(item->name);
						AR_DEL(item->lookahead);
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



static const psrActionView_t*	Parser_CreateActionView(const psrActionTable_t *tbl, const psrGrammar_t *grammar)
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
						const psrRule_t	 *rule = Parser_GetRuleFromGrammar(grammar, action->rule_num);
						

						switch(action->type)
						{
						case PARSER_ACCEPT:
								msg = AR_wcsdup(L"accept");
								break;
						case PARSER_SHIFT:
								{
										msg = AR_vtow(L"%Id", (size_t)action->shift_to);
								}
								break;
						case PARSER_REDUCE:
								{
										msg = AR_vtow(L"[<%ls>:%Id]", rule->head->name, (size_t)action->reduce_count);
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
						int_t state =  tbl->goto_tbl[AR_TBL_IDX_R(r, k, tbl->goto_col)];
						msg = AR_vtow(L"%Id", state);

						view->action_tbl[AR_TBL_IDX_R(r, c, view->col)] = msg;
						c++;
						msg = NULL;
				}
				AR_ASSERT(c == view->col);
		}

		AR_ASSERT(r == view->row);
		return view;
}


static void Parser_DestroyActionView(const psrActionView_t *action_view)
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



static void Parser_PrintActionTable(const psrActionTable_t *tbl, const psrGrammar_t *grammar, size_t width, arString_t *str)
{
		/*这里必须用int，因为printf一族函数的对于%*d这类width的定义就是int*/
		int __WIDTH__;
		size_t i,j;
		wchar_t buf[1024];

		__WIDTH__ = (int)width;
		
		AR_AppendString(str,L"TermSet:\r\n");
		for(i = 0; i < tbl->term_set.count; ++i)
		{
				Parser_PrintSymbol(tbl->term_set.lst[i],str);
				AR_AppendString(str,L"  ");
		}
		AR_AppendString(str,L"\r\n");
		AR_AppendString(str, L"-----------------------------------------\r\n");
		AR_AppendString(str,L"NonTermSet:\r\n");
		for(i = 0; i < tbl->nonterm_set.count; ++i)
		{
				Parser_PrintSymbol(tbl->nonterm_set.lst[i],str);
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
				AR_swprintf(buf, 1024, L"I[%Id]", (size_t)i);
				AR_AppendFormatString(str, L"%*ls:", __WIDTH__,buf);
				for(j = 0; j < tbl->goto_col; ++j)
				{
						AR_AppendFormatString(str, L"%*Id", __WIDTH__, tbl->goto_tbl[AR_TBL_IDX_R(i,j,tbl->goto_col)]);
						/*
						int_t xxx,xxx_idx;
						
						xxx_idx = AR_TBL_IDX_R(i,j,tbl->goto_col);
						xxx = tbl->goto_tbl[xxx_idx];
						printf("xxx_idx == %I64d, xxx == %I64d\r\n", xxx_idx, xxx);
						AR_AppendFormatString(str, L"%*I64d", __WIDTH__, (int_64_t)tbl->goto_tbl[AR_TBL_IDX_R(i,j,tbl->goto_col)]);
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
				AR_swprintf(buf, 1024, L"I[%Id]", i);

				AR_AppendFormatString(str,L"%*ls:", __WIDTH__,buf);
				for(j = 0; j < tbl->col; ++j)
				{
						const psrAction_t *pact;
						const psrRule_t	 *rule;
						pact = tbl->actions[AR_TBL_IDX_R(i,j, tbl->col)];
						rule = Parser_GetRuleFromGrammar(grammar, pact->rule_num);

						switch(pact->type)
						{
						case PARSER_ACCEPT:
								AR_AppendFormatString(str,L"%*ls", __WIDTH__,L"accept");
								break;
						case PARSER_SHIFT:
						{
								AR_AppendFormatString(str,L"%*Id", __WIDTH__, (size_t)pact->shift_to);
						}
								break;
						case PARSER_REDUCE:
						{
								AR_swprintf(buf, 1024, L"[<%ls>:%Id]",rule->head->name, (size_t)pact->reduce_count);
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


static void Parser_ReportConflict(const psrActionTable_t *tbl, const psrGrammar_t *grammar, arString_t *str)
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
						rule = Parser_GetRuleFromGrammar(grammar, action->rule_num);
						AR_ASSERT(action != NULL);
						if(action->next == NULL)continue;


						AR_AppendFormatString(str,L"state[%Id] : %ls\r\n",(size_t)i, tbl->term_set.lst[j]->name);
						
						while(action != NULL)
						{
								
								lalrConfig_t	tmp;

								/*
								psrLRItem_t tmp;
								
								Parser_InitLRItem(&tmp, rule, action->delim);
								*/
								Parser_InitConfig(&tmp, action->rule_num, action->delim, grammar);
								
								switch(action->type)
								{
								case PARSER_REDUCE:
										AR_AppendFormatString(str,L"Reduce: ");
										Parser_PrintConfig(&tmp, grammar, str);
										
										/*Parser_PrintLRItem(&tmp,grammar,str);*/

										break;
								case PARSER_SHIFT:
										AR_AppendFormatString(str,L"Shift: ");
										Parser_PrintConfig(&tmp, grammar,str);
										break;
								case PARSER_ACCEPT:
										AR_AppendFormatString(str,L"Accept ");
										/*Parser_PrintLRItemName(&action->item,grammar);*/
										break;
								default:
										AR_ASSERT(false);
								}
								AR_AppendFormatString(str,L"\r\n");
								action = action->next;
								
								Parser_UnInitConfig(&tmp);
								/*
								Parser_UnInitLRItem(&tmp);
								*/
						}

						AR_AppendFormatString(str,L"\r\n");
						AR_AppendFormatString(str,L"-----------------------------------------------\r\n");
				}
		}
}


static size_t Parser_CountConflict(const psrActionTable_t *tbl)
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


size_t	Parser_CountParserConflict(const parser_t *parser)
{
		AR_ASSERT(parser != NULL);
		AR_ASSERT(parser->tbl != NULL && parser->grammar != NULL);

		return Parser_CountConflict(parser->tbl);
}

void	Parser_PrintParserConflict(const parser_t *parser, arString_t *out)
{
		AR_ASSERT(parser != NULL && out != NULL);
		AR_ASSERT(parser->tbl != NULL && parser->grammar != NULL);
		
		Parser_ReportConflict(parser->tbl, parser->grammar, out);
}

void	Parser_PrintParserActionTable(const parser_t *parser, arString_t *out, size_t width)
{
		AR_ASSERT(parser != NULL && out != NULL);
		AR_ASSERT(parser->tbl != NULL && parser->grammar != NULL);

		Parser_PrintActionTable(parser->tbl, parser->grammar, width, out);
}



const psrActionView_t* Parser_CreateParserActionView(const parser_t *parser)
{
		AR_ASSERT(parser != NULL);
		return Parser_CreateActionView(parser->tbl, parser->grammar);

}

void					Parser_DestroyParserActionView(const psrActionView_t *view)
{
		AR_ASSERT(view != NULL);
		Parser_DestroyActionView(view);

}



const	psrConflictView_t*		Parser_CreateParserConflictView(const parser_t *parser)
{
		AR_ASSERT(parser != NULL);

		return Parser_CreateConflictView(parser->tbl, parser->grammar);
}


void							Parser_DestroyParserConflictView(const psrConflictView_t *view)
{
		AR_ASSERT(view != NULL);

		Parser_DestroyConflictView(view);
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
}psrStatusView_t;
*/

void __insert_to_symtbl_view(psrSymbolMapView_t *map_view, const wchar_t *name, const wchar_t *set)
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




static bool_t __detect_left_recursion(const psrGrammar_t *grammar, const psrSymb_t *head, psrSymbList_t *lst, psrSymbolMapView_t *output)
{
		size_t i;
		bool_t is_recu = false;
		AR_ASSERT(grammar != NULL && head != NULL && lst != NULL);

		/*AR_ASSERT(lst->count > 0);*/

		AR_ASSERT(Parser_FindFromSymbList(lst, head) == -1);
		
		Parser_InsertToSymbList(lst, head);
		
		
		
		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule;
				rule = grammar->rules[i];
				
				if(Parser_CompSymb(rule->head, head) == 0)
				{
						const psrSymb_t *symb;

						size_t x = 0;

						
						if(x < rule->body.count && rule->body.lst[x]->type == PARSER_NONTERM)
						{
								symb = rule->body.lst[x];

								if(Parser_CompSymb(lst->lst[0], symb) == 0)
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
												
												__insert_to_symtbl_view(output, L"Path:", AR_GetStringCString(str));

												AR_DestroyString(str);

										}

								}else if(Parser_FindFromSymbList(lst, symb) != -1)
								{
										continue;
								}else
								{
										__detect_left_recursion(grammar, symb, lst,output);
								}
						}
				}
		}
		
		Parser_RemoveFromSymbListByIndex(lst, lst->count-1);
		return is_recu;
		
}



bool_t					__report_left_recursion(const psrGrammar_t *grammar, psrSymbolMapView_t *output)
{
		size_t i;
		psrSymbList_t	lst;
		bool_t			ret = false;
		const psrSymbList_t		*symblist;
		AR_ASSERT(grammar != NULL);
		
		Parser_InitSymbList(&lst);

		symblist = Parser_GetSymbList(grammar);
		
		for(i = 0; i < symblist->count; ++i)
		{
				const psrSymb_t *symb;
				
				symb = symblist->lst[i];

				Parser_ClearSymbList(&lst);
				
				if(symb->type == PARSER_NONTERM)
				{
						if(__detect_left_recursion(grammar, symb, &lst,output))
						{
								ret = true;
						}
				}
		}

		Parser_UnInitSymbList(&lst);
		return ret;
}





/************************************************************************************************************/




static size_t __calc_leftfactor(const psrRule_t *l, const psrRule_t *r)
{
		size_t cnt = 0;
		AR_ASSERT(l != NULL && r != NULL);

		while(cnt < l->body.count && cnt < r->body.count && Parser_CompSymb(l->body.lst[cnt], r->body.lst[cnt]) == 0)
		{
				cnt++;
		}
		return cnt;
}



static bool_t	__report_rule_left_factor(const psrSymb_t *lhs, const psrRule_t **rules, size_t n, psrSymbolMapView_t *output)
{
		size_t i,k;
		size_t	*bk;
		size_t			max,cnt;
		bool_t			has_left_factor = false;
		arString_t		*tmp;
		AR_ASSERT(lhs != NULL && rules != NULL);

		if(n < 2)return false;
		tmp = AR_CreateString();
		bk = AR_NEWARR0(size_t, n);

RECHECK_POINT:
		
		max = 0;
		cnt = 0;

		for(i = 0; i < n; ++i)
		{
				if(rules[i] == NULL)continue;
				AR_ASSERT(Parser_CompSymb(lhs, rules[i]->head) == 0);
				for(k = 0; k < n; ++k)
				{
						if(rules[k] == NULL)continue;

						if(k != i)
						{
								size_t x = __calc_leftfactor(rules[i], rules[k]);
								
								if(x > max)
								{
										max = x;
										AR_memset(bk, 0, sizeof(size_t)*n);
										cnt = 0;
										bk[cnt++] = i;
										bk[cnt++] = k;
								}else if(x > 0 && x == max && __calc_leftfactor(rules[bk[0]], rules[i]) == x)
								{
										size_t j;
										bool_t need_i = true, need_k = true;
										for(j = 0; j < n; ++j)
										{
												if(bk[j] == i)need_i = false;
												if(bk[j] == k)need_k = false;
										}
										
										if(need_i)bk[cnt++] = i;
										if(need_k)bk[cnt++] = k;
								}else
								{
								}
						}
				}
		}

		if(cnt == 0)
		{
				goto RETURN_POINT;
		}else
		{
				has_left_factor = true;
		}
		
		
		for(i = 0; i < cnt ; ++i)
		{
				if(output)
				{
						AR_ClearString(tmp);
						AR_AppendString(tmp, lhs->name);
						AR_AppendString(tmp, L"\t:\t");
						Parser_PrintSymbolList(&rules[bk[i]]->body, tmp);
						AR_AppendFormatString(tmp, L"\t:\t%Id", max);
						__insert_to_symtbl_view(output, lhs->name, AR_GetStringCString(tmp));
				}
				rules[bk[i]] = NULL;
		}
		
		if(cnt > 0)
		{
				if(output)
				{
						__insert_to_symtbl_view(output, lhs->name, L"");
				}
				goto RECHECK_POINT;
		}

RETURN_POINT:
		AR_DEL(bk);
		AR_DestroyString(tmp);
		return has_left_factor;
}


static bool_t					__report_left_factor(const psrGrammar_t *grammar, psrSymbolMapView_t *output)
{
		const psrRule_t	**rules;
		bool_t has_left_factor;
		size_t	cnt;
		size_t	i,k;
		const psrSymbList_t *symblist;
		AR_ASSERT(grammar != NULL);
		
		cnt = 0;
		has_left_factor = false;
		symblist = Parser_GetSymbList(grammar);
		rules = AR_NEWARR0(const psrRule_t*, symblist->count);
		
		for(i = 0; i < symblist->count; ++i)
		{
				const psrSymb_t *lhs = symblist->lst[i];
				if(lhs->type == PARSER_TERM)continue;
				
				AR_memset((void*)rules, 0, sizeof(const psrRule_t*) * symblist->count);
				
				cnt = 0;
				
				for(k = 0; k < grammar->count; ++k)
				{
						if(Parser_CompSymb(lhs, grammar->rules[k]->head) == 0)
						{
								rules[cnt++] = grammar->rules[k];
						}
				}
		
				if(__report_rule_left_factor(lhs, rules, cnt,output))
				{
						has_left_factor = true;
				}
		}

		AR_DEL(rules);
		return has_left_factor;
}


/**************************************************************************************************************/



const psrStatusView_t*		Parser_CreateParserStatusView(const parser_t *parser)
{
		psrSymbMap_t	first, follow;
		arString_t		*str;
		psrSymbolMapView_t		first_view, follow_view, left_recu, left_factor;
		const psrSymbList_t		*symb_list;
		psrStatusView_t	*ret;
		size_t i;
		
		AR_ASSERT(parser != NULL && parser->grammar != NULL);

		AR_memset(&first_view, 0, sizeof(first_view));
		AR_memset(&follow_view, 0, sizeof(follow_view));
		
		AR_memset(&left_recu, 0, sizeof(left_recu));
		AR_memset(&left_factor, 0, sizeof(left_factor));

		symb_list = Parser_GetSymbList(parser->grammar);

		ret = NULL;

		Parser_InitSymbMap(&first);
		Parser_InitSymbMap(&follow);

		Parser_CalcFirstSet(parser->grammar, &first);
		Parser_CalcFollowSet(parser->grammar, &follow, &first);

		str = AR_CreateString();

		for(i = 0; i < symb_list->count; ++i)
		{
				const psrMapRec_t		*rec;
				const psrSymb_t	*symb = symb_list->lst[i];
				
				rec = Parser_GetSymbolFromSymbMap(&first, symb);
				
				AR_ClearString(str);
				if(rec)
				{
						
						Parser_PrintSymbolList(&rec->lst, str);
						
				}
				__insert_to_symtbl_view(&first_view, rec->key->name, AR_GetStringCString(str));
		}


		for(i = 0; i < symb_list->count; ++i)
		{
				const psrMapRec_t		*rec;
				const psrSymb_t	*symb = symb_list->lst[i];
				
				if(symb->type == PARSER_NONTERM)
				{
						rec = Parser_GetSymbolFromSymbMap(&follow, symb);
						
						AR_ClearString(str);
						if(rec)
						{
								
								Parser_PrintSymbolList(&rec->lst, str);
								
						}
						__insert_to_symtbl_view(&follow_view, rec->key->name, AR_GetStringCString(str));
				}
		}

		__report_left_recursion(parser->grammar, &left_recu);
		__report_left_factor(parser->grammar, &left_factor);

		ret = AR_NEW0(psrStatusView_t);
		
		

		ret->first_set = first_view;
		ret->follow_set = follow_view;
		ret->left_recursion = left_recu;
		ret->left_factor = left_factor;
		
		Parser_UnInitSymbMap(&first);
		Parser_UnInitSymbMap(&follow);
		if(str)AR_DestroyString(str);
		return ret;
}


void							Parser_DestroyParserStatusView(const psrStatusView_t *follow_view)
{
		size_t i;

		if(follow_view != NULL)
		{
				psrStatusView_t *view = (psrStatusView_t*)follow_view;

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

				for(i = 0; i < view->left_factor.count; ++i)
				{
						AR_DEL(view->left_factor.name[i]);
						AR_DEL(view->left_factor.name_set[i]);
				}

				AR_DEL(view->left_factor.name);
				AR_DEL(view->left_factor.name_set);

				AR_DEL(view);
		}
}






AR_NAMESPACE_END
