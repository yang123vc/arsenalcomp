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

#include "lex.h"
#include "expr.h"
#include "dfa.h"
AR_NAMESPACE_BEGIN




typedef struct __lex_tag 
{
		lexName_t		*name_tbl;
		lexNode_t		*uni_root;
		
		lexStateTable_t	tbl;
		lexCClass_t		cclass;

		size_t			*skip_list;
		size_t			skip_count;
		size_t			skip_cap;
}lex_t;


lex_t*	LEX_Create()
{
		lex_t *res;
		res = AR_NEW0(lex_t);
		LEX_InitStateTable(&res->tbl);
		LEX_InitCClass(&res->cclass);
		
		res->uni_root = LEX_CreateNode(LEX_BRANCH);

		res->skip_list = NULL;
		res->skip_count = 0;
		res->skip_cap = 0;

		return res;
}






void	LEX_Destroy(lex_t *lex)
{
		LEX_DestroyNameList(lex->name_tbl);
		LEX_UnInitStateTable(&lex->tbl);
		LEX_UnInitCClass(&lex->cclass);
		LEX_DestroyNode(lex->uni_root);
		if(lex->skip_list)
		{
				AR_DEL(lex->skip_list);
		}
		AR_DEL(lex);
}


bool	LEX_InsertName(lex_t *lex, const wchar_t *name, const wchar_t *expr)
{
		lexResult_t res;
		AR_ASSERT(name != NULL && AR_wcslen(name) > 0 && expr != NULL);
		
		if(LEX_FindFromNameList(lex->name_tbl, name) != NULL)
		{
				AR_error(AR_LEX, L"Lex Rule Error : Duplicate name defination %s: %s\r\n", name, expr);
				return false;
		}

		res = LEX_CompileExpr(expr, lex->name_tbl);
		
		if(res.node == NULL)
		{
				AR_error(AR_LEX, L"Lex Rule Error : %s: %s\r\n", name, res.err.pos);
				return false;
		}
		return LEX_InsertToNameList(&lex->name_tbl, name, res.node);
}


bool	LEX_InsertRule(lex_t *lex, const wchar_t *rule, const lexAction_t *action)
{
		lexResult_t res;
		lexNode_t	*cat, *final;
		AR_ASSERT(lex != NULL && rule != NULL && action != NULL);
		AR_ASSERT(AR_wcslen(rule) > 0);

		res = LEX_CompileExpr(rule, lex->name_tbl);

		if(res.node == NULL)
		{
				AR_error(AR_LEX, L"Lex Rule Error : %d : %s\n", action->type, res.err.pos);
				return false;
		}

		cat = LEX_CreateNode(LEX_CAT);
		final = LEX_CreateNode(LEX_FINAL);
		final->action = *action;
		
		LEX_InsertNodeToCat(&cat->cat, res.node);
		LEX_InsertNodeToCat(&cat->cat, final);
		LEX_InsertNodeToBranch(&lex->uni_root->branch, cat);
		return true;
}


bool	LEX_Insert(lex_t *lex, const wchar_t *input)
{
		const wchar_t *p;
		
		AR_ASSERT(lex != NULL && input != NULL);
		p = AR_wcstrim(input, L" \t");
		
		if(AR_iswdigit(*p))/*action*/
		{
				lexAction_t		act;
				act.priority = act.type = 0;
				p = AR_wtou(p, &act.type, 10);
				if(p == NULL)return false;

				p = AR_wcstrim(p, L" \t");

				if(*p == L',')
				{
						p = AR_wtou(++p, &act.priority, 10);
						if(p == NULL)return false;
				}

				p = AR_wcstrim(p, L" \t");
				
				return LEX_InsertRule(lex, p, &act);
		}else if(AR_iswalpha(*p) || *p == L'_')
		{
				wchar_t name[AR_MAX_LEXNAME + 1];
				size_t i;
				i = 0;
				while(AR_iswalpha(*p) || *p == L'_')
				{
						name[i++] = *p++;
				}
				name[i] = L'\0';
				p = AR_wcstrim(p, L" \t");
				if(*p != L'=')return false;
				p = AR_wcstrim(++p, L" \t");
				return LEX_InsertName(lex, name, p);
				
		}else
		{
				AR_error(AR_LEX, L"Lex Rule Error : Invalid Input %s\r\n", p);
				return false;
		}
		
		
}


void	LEX_Clear(lex_t *lex)
{
		LEX_DestroyNode(lex->uni_root);
		LEX_DestroyNameList(lex->name_tbl);
		lex->skip_count = 0;
		lex->uni_root = LEX_CreateNode(LEX_BRANCH);
		lex->name_tbl = NULL;
}


bool	LEX_GenerateTransTable(lex_t *lex)
{
		lexDFA_t *dfa;
		lexLeafSet_t *set;
		AR_ASSERT(lex != NULL);
		LEX_UnInitStateTable(&lex->tbl);
		LEX_UnInitCClass(&lex->cclass);

		if(lex->uni_root->branch.count == 0)return false;

		LEX_CalcCClass(&lex->cclass, lex->uni_root);

		set = LEX_BuildLeafSet(lex->uni_root, &lex->cclass);
		dfa = LEX_BuildDFA(&lex->cclass, set);
		LEX_BuildStateTable(&lex->tbl, dfa, &lex->cclass);
		
		LEX_DestroyLeafSet(set);
		LEX_DestroyDFA_ALL(dfa);
		
		return true;
}



void	LEX_InsertSkipAction(lex_t *lex, size_t action_type)
{
		size_t i;

		AR_ASSERT(lex != NULL);
		
		if(lex->skip_count == lex->skip_cap)
		{
				lex->skip_cap = (lex->skip_cap + 4)*2;
				lex->skip_list = AR_REALLOC(size_t, lex->skip_list, lex->skip_cap);
		}

		for(i = 0; i < lex->skip_count; ++i)
		{
				if(lex->skip_list[i] == action_type)return;		
		}

		lex->skip_list[lex->skip_count++] = action_type;
}


/*********************************lexMatch_t***************************/
void LEX_ResetMatch(lexMatch_t *pmatch, const wchar_t *input)
{
		LEX_UnInitMatch(pmatch);
		LEX_InitMatch(pmatch, input);

}


void LEX_InitMatch(lexMatch_t *pmatch, const wchar_t *input)
{
		AR_ASSERT(pmatch != NULL && input != NULL);

		AR_memset(pmatch, 0, sizeof(*pmatch));
		pmatch->input = AR_NEWARR0(wchar_t, AR_wcslen(input) + 1);
		AR_wcscpy(pmatch->input, input);
		pmatch->next = pmatch->input;
		pmatch->is_ok = true;

}


void LEX_UnInitMatch(lexMatch_t *pmatch)
{
		if(pmatch != NULL)
		{
				AR_DEL(pmatch->input);
				AR_memset(pmatch, 0, sizeof(*pmatch));
		}
}

static bool __is_skiped(const lex_t *lex, size_t type)
{
		size_t i;
		AR_ASSERT(lex != NULL);

		for(i = 0; i < lex->skip_count; ++i)
		{
				if(lex->skip_list[i] == type)return true;
		}
		return false;
}

bool LEX_Match(lex_t *lex, lexMatch_t *match, lexToken_t *tok)
{
		const wchar_t *forward;
		int id;
		size_t count;
		int_t	curr, accepted;
		size_t	x,y;

RE_MATCH_POINT:
		if(!match->is_ok)return match->is_ok;
		AR_ASSERT(lex != NULL && match != NULL && match->next != NULL && tok != NULL);

		x = match->line;
		y = match->col;
		curr = 0;
		accepted = -1;
		count = 0;
		
		tok->line = match->line;
		tok->col = match->col;
		
		forward = match->next;
		tok->count = 0;
		tok->str = forward;

		while(curr != -1)
		{
				wchar_t c = *forward;

				if(c == L'\n')
				{
						x++; y = 0;
				}else
				{
						if(c != L'\0')y++;
				}

				id = LEX_FindIndexFromCClass(&lex->cclass, c);
				AR_ASSERT((size_t)id < lex->tbl.col);
				curr = LEX_GetNextState(&lex->tbl, curr, id);
				
				if(curr != -1)
				{
						if(c != L'\0')
						{
								count++;
								forward++;
						}
						
						if(lex->tbl.is_final[curr])
						{
								accepted = curr;
								match->line = x;
								match->col = y;
						}
				}

				if(c == L'\0')break;

		}

		

		if(accepted != -1)
		{
				tok->type = lex->tbl.actions[accepted].type;
				tok->count = count;
				match->next = match->next + count;
				match->is_ok = true;
				
				if(__is_skiped(lex, tok->type))goto RE_MATCH_POINT;

		}else
		{
				match->is_ok = false;
		}

		return match->is_ok;
}


const wchar_t* LEX_GetNextInput(const lexMatch_t *match)
{
		return match->next;
}



#if(0)
int_t LEX_Match(lex_t *lex, lexMatch_t *match, lexToken_t *tok)
{
		const wchar_t *forward;
		int id;
		size_t count;
		int_t	curr, accepted;
		size_t	x,y;
		
		if(match->state != LEX_MATCH_OK)return match->state;
		AR_ASSERT(lex != NULL && match != NULL && match->next != NULL && tok != NULL);

		x = match->line;
		y = match->col;
		curr = 0;
		accepted = -1;
		count = 0;
		
		tok->line = match->line;
		tok->col = match->col;
		
		
		if(*match->next == L'\0')
		{
				match->state = LEX_MATCH_EOI;
				return match->state;
		}

		forward = match->next;
		tok->count = 0;
		tok->str = forward;

		while(*forward != L'\0' && curr != -1)
		{
				wchar_t c = *forward;

				if(c == L'\n')
				{
						x++; y = 0;
				}else
				{
						y++;
				}

				id = LEX_FindIndexFromCClass(&lex->cclass, c);
				AR_ASSERT((size_t)id < lex->tbl.col);
				curr = LEX_GetNextState(&lex->tbl, curr, id);
				
				if(curr != -1)
				{
						count++;
						forward++;
						
						if(lex->tbl.is_final[curr])
						{
								accepted = curr;
								match->line = x;
								match->col = y;
						}
				}
		}



		if(accepted != -1)
		{
				tok->type = lex->tbl.actions[accepted].type;
				tok->count = count;
				match->next = match->next + count;
		}else
		{
				match->state = LEX_MATCH_ERROR;
		}

		return match->state;
}
#endif







AR_NAMESPACE_END
