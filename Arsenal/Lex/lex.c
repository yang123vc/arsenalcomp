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

#include "rgx.h"
#include "lex.h"

AR_NAMESPACE_BEGIN

void	LEX_Init()
{
		RGX_InitNode();
		RGX_InitMisc();
}

void	LEX_UnInit()
{
		RGX_UnInitNode();
		RGX_UnInitMisc();
}


typedef struct __prog_set_tag
{
		rgxProg_t						**prog;
		lexAction_t						*action;
		size_t							count;
		size_t							cap;
}lexProgSet_t;

lexProgSet_t* LEX_CreateProgSet()
{
		lexProgSet_t *res;
		res = AR_NEW0(lexProgSet_t);
		return res;
}


void LEX_ClearProgSet(lexProgSet_t *set)
{
		size_t i;
		AR_ASSERT(set != NULL);

		for(i = 0; i < set->count; ++i)
		{
				RGX_UnInitProg(set->prog[i]);
				AR_DEL(set->prog[i]);
		}

		set->count = 0;
}


void		LEX_DestroyProgSet(lexProgSet_t *set)
{

		if(set)
		{
				LEX_ClearProgSet(set);

				if(set->prog)AR_DEL(set->prog);
				if(set->action)AR_DEL(set->action);
				AR_DEL(set);
		}
}

void LEX_InserToProgSet(lexProgSet_t *set, rgxProg_t *prog, const lexAction_t *act)
{
		AR_ASSERT(set != NULL && prog != NULL && act != NULL);

		if(set->count == set->cap)
		{
				set->cap = (set->cap + 1)*2;
				set->prog = AR_REALLOC(rgxProg_t*, set->prog, set->cap);
				set->action = AR_REALLOC(lexAction_t, set->action, set->cap);
		}
		set->prog[set->count] = prog;
		set->action[set->count] = *act;
		set->count++;
}


static void __exch_set(lexProgSet_t *set, int_t i,int_t j)
{
		rgxProg_t		*prog;
		lexAction_t		act;
		AR_ASSERT(set != NULL);

		prog = set->prog[i];
		set->prog[i] = set->prog[j];
		set->prog[j] = prog;


		act = set->action[i];
		set->action[i] = set->action[j];
		set->action[j] = act;
}

bool_t	LEX_RemoveFromProgSet(lexProgSet_t *set, size_t value)
{
		int_t l,r,cnt;
		bool_t res = false;
		AR_ASSERT(set != NULL);

		l = 0; r = (int_t)set->count - (int_t)1, cnt = (int_t)set->count;

		while(l <= r)
		{
				if(set->action[l].value == value)
				{
						__exch_set(set, l,r);
						r--;
						res = true;
				}
				l++;
		}


		for(l = r + 1; l < (int_t)set->count; ++l)
		{
				rgxProg_t *prog = set->prog[l];
				set->prog[l] = NULL;
				RGX_UnInitProg(prog);
				AR_DEL(prog);
				cnt--;
		}
		AR_ASSERT(cnt >= 0);

		set->count = (size_t)cnt;

		return res;
}



void LEX_SortProgSet(lexProgSet_t *set)
{
		int_t i,j;



		for(i = (int_t)set->count - 1; i > 0; --i)
		{
				if(set->action[i].priority > set->action[i-1].priority)
				{
						__exch_set(set, i, i - 1);
				}
		}



		for(i = 1; i < (int_t)set->count; ++i)
		{
				for(j = i; j > 0 && set->action[j].priority > set->action[j-1].priority; --j)
				{
						__exch_set(set, j, j - 1);
				}
		}


}


/**********************************lex**************************************************/

bool_t	LEX_InsertName(lex_t *lex, const wchar_t *name, const wchar_t *expr)
{
		rgxResult_t res;
		AR_ASSERT(name != NULL && AR_wcslen(name) > 0 && expr != NULL);

		if(RGX_FindFromNameSet(lex->name_tbl, name) != NULL)
		{
				/*AR_error( L"Lex Rule Error : Duplicate name defination %ls: %ls\r\n", name, expr);*/
				AR_printf_ctx(&lex->io_ctx, L"Lex Name Error : Duplicate name defination %ls: %ls\r\n", name, expr);
				return false;
		}

		res = RGX_ParseExpr(expr, lex->name_tbl);

		if(res.node == NULL)
		{
				/*AR_error(L"Lex Rule Error : %ls: %ls\r\n", name, res.err.pos);*/
				AR_printf_ctx(&lex->io_ctx, L"Lex Name Error : %ls: %ls\r\n", name, res.err.pos);
				return false;
		}


		/*RGX_CorrectTree(res.node);*/

		return RGX_InsertToNameSet(lex->name_tbl, name, res.node);
}


bool_t	LEX_RemoveByName(lex_t *lex, const wchar_t *name)
{
		AR_ASSERT(lex != NULL && name != NULL);

		return RGX_RemoveFromNameSet(lex->name_tbl, name);
}



bool_t	LEX_RemoveByValue(lex_t *lex, size_t value)
{
		AR_ASSERT(lex != NULL && lex->prog_set != NULL);

		return LEX_RemoveFromProgSet(lex->prog_set, value);
}





bool_t	LEX_InsertRule(lex_t *lex, const wchar_t *rule, const lexAction_t *action)
{
		rgxResult_t res;
		rgxNode_t	*cat, *final;
		rgxProg_t	*prog;
		AR_ASSERT(lex != NULL && rule != NULL && action != NULL);
		AR_ASSERT(AR_wcslen(rule) > 0);

		res = RGX_ParseExpr(rule, lex->name_tbl);

		if(res.node == NULL)
		{
				/*AR_error(AR_LEX, L"Lex Rule Error : %d : %ls\n", action->type, res.err.pos);*/
				/*AR_error(L"Lex Rule Error : %" AR_PLAT_INT_FMT L"d : %ls\n", (size_t)action->type, (size_t)res.err.pos);*/

				AR_printf_ctx(&lex->io_ctx, L"Lex Rule Error : %" AR_PLAT_INT_FMT L"d : %ls\n", (size_t)action->value, res.err.pos);
				return false;
		}

		cat = RGX_CreateNode(RGX_CAT_T);
		final = RGX_CreateNode(RGX_FINAL_T);
		final->final_val = (int_t)action->value;

		cat->left = res.node;
		cat->right = final;

		/*RGX_CorrectTree(cat);*/

		prog = AR_NEW(rgxProg_t);

		RGX_InitProg(prog);

		RGX_Compile(prog, cat);

		LEX_InserToProgSet(lex->prog_set, prog, action);
		RGX_DestroyNode(cat);

		return true;
}



bool_t	LEX_Insert(lex_t *lex, const wchar_t *input)
{
		const wchar_t *p;

		AR_ASSERT(lex != NULL && input != NULL);
		p = AR_wcstrim_space(input);

		if(AR_iswdigit(*p) || *p == L'%')/*action*/
		{
				lexAction_t		act;
				bool_t			is_skip;

				is_skip = false;
				if(*p == L'%')
				{
						if(AR_wcsstr(p, L"%skip") == NULL)return false;

						p = AR_wcstrim_space(p + AR_wcslen(L"%skip"));

						is_skip = true;
				}

				act.priority = act.value = 0;
				p = AR_wtou(p, (uint_t*)&act.value, 10);
				if(p == NULL)return false;

				p = AR_wcstrim_space(p);

				if(*p == L',')
				{
						p = AR_wtou(++p, (uint_t*)&act.priority, 10);
						if(p == NULL)return false;
				}

				p = AR_wcstrim_space(p);

				act.is_skip = is_skip;
				return LEX_InsertRule(lex, p, &act);

		}else if(AR_iswalpha(*p) || *p == L'_')
		{
				wchar_t name[AR_RGX_MAXNAME + 1];
				size_t i;
				i = 0;
				while(AR_iswalnum(*p) || *p == L'_')
				{
						name[i++] = *p++;
				}
				name[i] = L'\0';
				p = AR_wcstrim_space(p);

				if(*p != L'=')return false;
				p = AR_wcstrim_space(++p);
				return LEX_InsertName(lex, name, p);

		}else
		{
				/*AR_error(L"Lex Rule Error : Invalid Input %ls\r\n", p);*/

				AR_printf_ctx(&lex->io_ctx, L"Lex Rule Error : Invalid Input %ls\r\n", p);
				return false;
		}


}


bool_t	LEX_GenerateTransTable(lex_t *lex)
{
		AR_ASSERT(lex != NULL && lex->prog_set != NULL);

		LEX_SortProgSet(lex->prog_set);
		return (bool_t)(lex->prog_set->count > 0);
}

#define LEX_MAX_EMPTY_MATCH_CNT 1

bool_t LEX_Match(lex_t *lex, lexMatch_t *match, lexToken_t *tok)
{
		size_t i;

		size_t empty_match_cnt = 0;

		AR_ASSERT(lex != NULL && match != NULL && tok != NULL);
REMATCH:
		if(empty_match_cnt > LEX_MAX_EMPTY_MATCH_CNT)
		{
				AR_printf_ctx(&lex->io_ctx, L"%ls\r\n", L"Invalid empty pattern\r\n");
				AR_ASSERT(false);
				return false;
		}

		for(i = 0; i < lex->prog_set->count; ++i)
		{
				if(RGX_Match(lex->prog_set->prog[i], match, tok))
				{
						/*
						{
								arString_t *str = AR_CreateString();
								RGX_ProgToString(lex->prog_set->prog[i],str);
								AR_printf_ctx(&lex->io_ctx, L"Matched : %ls\r\n", AR_GetStrString(str));
								AR_DestroyString(str);
						}
						*/

						if(lex->prog_set->action[i].is_skip)
						{
								if(tok->count == 0)
								{
										empty_match_cnt++;
								}

								if(match->flags & LEX_REPORT_SKIP && tok->count > 0)
								{
										wchar_t *tmp = AR_wcsndup(tok->str, tok->count);
										AR_printf_ctx(&lex->io_ctx, L"Skip token \"%ls\"\r\n", tmp);
										AR_DEL(tmp);
								}
								goto REMATCH;
						}else
						{
								return true;
						}
				}
		}

		match->is_ok = false;
		return false;
}

lex_t*	LEX_Create(const arIOCtx_t *io)
{
		lex_t *res;

		res = AR_NEW0(lex_t);


		res->prog_set = LEX_CreateProgSet();
		res->name_tbl = AR_NEW(rgxNameSet_t);
		RGX_InitNameSet(res->name_tbl);

		res->io_ctx = io == NULL ? *AR_global_ioctx() : *io;

		return res;
}



void	LEX_Clear(lex_t *lex)
{

		RGX_ClearNameSet(lex->name_tbl);
		LEX_ClearProgSet(lex->prog_set);
}





void	LEX_Destroy(lex_t *lex)
{
		AR_ASSERT(lex != NULL);

		LEX_DestroyProgSet(lex->prog_set);
		RGX_UnInitNameSet(lex->name_tbl);

		AR_DEL(lex->name_tbl);
		AR_DEL(lex);
}

void	LEX_ResetIOContext(lex_t *lex, const arIOCtx_t *io)
{
		AR_ASSERT(lex != NULL && io != NULL);

		lex->io_ctx = io == NULL ? *AR_global_ioctx() : *io;


}





AR_NAMESPACE_END
