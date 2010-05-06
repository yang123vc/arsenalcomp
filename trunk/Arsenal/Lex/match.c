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
#include "rgx.h"



AR_NAMESPACE_BEGIN


struct __prog_set_tag
{
		rgxProg_t						**prog;
		lexAction_t						*action;
		size_t							count;
		size_t							cap;
};

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

/*********************************lexMatch_t***************************/

#define __ALL_FLAGS		(LEX_REPORT_SKIP|LEX_IGNORE_CASE|LEX_SINGLE_LINE)

void			LEX_MatchFlags(lexMatch_t *pmatch, uint_t flags, bool_t is_on)
{
		AR_ASSERT(pmatch != NULL);
		flags &= __ALL_FLAGS;
		if(is_on)
		{
				pmatch->flags |= flags;
		}else
		{
				pmatch->flags &= ~flags;
		}
}

void			LEX_MatchClearFlags(lexMatch_t *pmatch)
{
		AR_ASSERT(pmatch != NULL);
		pmatch->flags = 0;
}

#undef __ALL_FLAGS



void LEX_ResetInput(lexMatch_t *pmatch, const wchar_t *input)
{
		AR_ASSERT(pmatch != NULL && input != NULL);
		pmatch->input = input;
		pmatch->is_ok = true;
		pmatch->col = pmatch->line = 0;
		pmatch->next = pmatch->input;
}

void			LEX_ClearInput(lexMatch_t *pmatch)
{
		LEX_ResetInput(pmatch, L"");
}


void LEX_ResetMatch(lexMatch_t *pmatch)
{
		AR_ASSERT(pmatch != NULL && pmatch->input != NULL);
		
		pmatch->is_ok = true;
		pmatch->col = pmatch->line = 0;
		pmatch->next = pmatch->input;
}










void			LEX_InitMatch(lexMatch_t *pmatch, const lex_t *lex, const arIOCtx_t *io)
{
		size_t i;
		AR_ASSERT(pmatch != NULL && lex != NULL);

		AR_memset(pmatch, 0, sizeof(*pmatch));

		pmatch->io_ctx = io == NULL ? *AR_global_ioctx() : *io;

		pmatch->input = L"";
		pmatch->next = pmatch->input;
		pmatch->is_ok = true;
		pmatch->flags = 0;
		
		pmatch->prog_set = LEX_CreateProgSet();
		
		for(i = 0; i < lex->rule_set.count; ++i)
		{
				rgxProg_t *prog;
				prog = AR_NEW(rgxProg_t);
				RGX_InitProg(prog);
				RGX_Compile(prog, lex->rule_set.nodes[i]);
				LEX_InserToProgSet(pmatch->prog_set, prog, &lex->rule_set.action[i]);
		}
		LEX_SortProgSet(pmatch->prog_set);
}


void LEX_UnInitMatch(lexMatch_t *pmatch)
{
		if(pmatch != NULL)
		{
				/*if(pmatch->input)AR_DEL(pmatch->input);*/
				LEX_DestroyProgSet(pmatch->prog_set);
				AR_memset(pmatch, 0, sizeof(*pmatch));
		}
}


void			LEX_ResetMatchIOContext(lexMatch_t *pmatch, const arIOCtx_t *io)
{
		AR_ASSERT(pmatch != NULL && io != NULL);
		pmatch->io_ctx = io == NULL ? *AR_global_ioctx() : *io;
}


const wchar_t* LEX_GetNextInput(const lexMatch_t *match)
{
		AR_ASSERT(match != NULL);
		return match->next;
}

bool_t	LEX_IsError(const lexMatch_t *match)
{
		AR_ASSERT(match != NULL);
		return !match->is_ok;
}

void	LEX_ClearError(lexMatch_t *match)
{
		AR_ASSERT(match != NULL);
		match->is_ok = true;
}


void	LEX_Skip(lexMatch_t *pmatch)
{
		AR_ASSERT(pmatch != NULL && pmatch->next != NULL);

		
		while(*pmatch->next && !AR_iswspace(*pmatch->next))
		{
				pmatch->next++;
				pmatch->col++;
		}


		while(*pmatch->next && AR_iswspace(*pmatch->next))
		{
				if(*pmatch->next == L'\n')
				{
						pmatch->line++;
						pmatch->col = 0;
				}else
				{
						pmatch->col++;
				}
				pmatch->next++;
		}
}

void			LEX_SkipTo(lexMatch_t *pmatch, const wchar_t *tok)
{
		const wchar_t *next;
		AR_ASSERT(pmatch != NULL && pmatch->next != NULL);
		next = AR_wcsstr(pmatch->next, tok);

		if(next == NULL)
		{
				while(*pmatch->next != L'\0')
				{
						if(*pmatch->next == L'\n')
						{
								pmatch->line++;
								pmatch->col = 0;
						}else
						{
								pmatch->col++;
						}
						pmatch->next++;
				}
		}else
		{
				while(pmatch->next != next)
				{
						if(*pmatch->next == L'\n')
						{
								pmatch->line++;
								pmatch->col = 0;
						}else
						{
								pmatch->col++;
						}
						pmatch->next++;
				}
		}
}



void			LEX_SkipN(lexMatch_t *pmatch, size_t nchar)
{
		size_t i;
		AR_ASSERT(pmatch != NULL && pmatch->next != NULL);
		
		for(i = 0; *pmatch->next != L'\0' && i < nchar; ++i)
		{
				if(*pmatch->next == L'\n')
				{
						pmatch->line++;
						pmatch->col = 0;
				}else
				{
						pmatch->col++;
				}
				pmatch->next++;
		}
}


void			LEX_PutBack(lexMatch_t *pmatch, const lexToken_t *tok)
{
		AR_ASSERT(pmatch !=  NULL && tok != NULL && tok->str != NULL);

#if defined(AR_DEBUG)
		{
				const wchar_t *beg = pmatch->input, *end = pmatch->input + AR_wcslen(pmatch->input);
				AR_ASSERT(tok->str >= beg && tok->str < end);
		}
#endif
		pmatch->next = tok->str;
		pmatch->line = tok->line;
		pmatch->col = tok->col;
}












#define LEX_MAX_EMPTY_MATCH_CNT 1

bool_t LEX_Match(lexMatch_t *match, lexToken_t *tok)
{
		size_t i;

		size_t empty_match_cnt = 0;

		AR_ASSERT(match != NULL && tok != NULL);
REMATCH:
		if(empty_match_cnt > LEX_MAX_EMPTY_MATCH_CNT)
		{
				AR_printf_ctx(&match->io_ctx, L"%ls\r\n", L"Invalid empty pattern\r\n");
				AR_ASSERT(false);
				return false;
		}

		for(i = 0; i < match->prog_set->count; ++i)
		{
				if(RGX_Match(match->prog_set->prog[i], match, tok))
				{
						/*
						{
								arString_t *str = AR_CreateString();
								RGX_ProgToString(lex->prog_set->prog[i],str);
								AR_printf_ctx(&lex->io_ctx, L"Matched : %ls\r\n", AR_GetStrString(str));
								AR_DestroyString(str);
						}
						*/

						if(match->prog_set->action[i].is_skip)
						{
								if(tok->count == 0)
								{
										empty_match_cnt++;
								}

								if(match->flags & LEX_REPORT_SKIP && tok->count > 0)
								{
										wchar_t *tmp = AR_wcsndup(tok->str, tok->count);
										AR_printf_ctx(&match->io_ctx, L"Skip token \"%ls\"\r\n", tmp);
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







AR_NAMESPACE_END





