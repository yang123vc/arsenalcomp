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


#include "match.h"


AR_NAMESPACE_BEGIN

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
#undef __ALL_FLAGS

void LEX_ResetInput(lexMatch_t *pmatch, const wchar_t *input)
{
		AR_ASSERT(pmatch != NULL && input != NULL);
		LEX_UnInitMatch(pmatch);
		LEX_InitMatch(pmatch, input);
}

void LEX_ResetMatch(lexMatch_t *pmatch)
{
		AR_ASSERT(pmatch != NULL && pmatch->input != NULL);
		
		pmatch->is_ok = true;
		pmatch->col = pmatch->line = 0;
		pmatch->next = pmatch->input;
		pmatch->flags = 0;
}


void LEX_InitMatch(lexMatch_t *pmatch, const wchar_t *input)
{
		AR_ASSERT(pmatch != NULL && input != NULL);

		AR_memset(pmatch, 0, sizeof(*pmatch));
		/*
		pmatch->input = AR_NEWARR0(wchar_t, AR_wcslen(input) + 1);
		AR_wcscpy(pmatch->input, input);
		*/

		pmatch->input = input;
		pmatch->next = pmatch->input;
		pmatch->is_ok = true;
		pmatch->flags = 0;

}


void LEX_UnInitMatch(lexMatch_t *pmatch)
{
		if(pmatch != NULL)
		{
				/*if(pmatch->input)AR_DEL(pmatch->input);*/
				AR_memset(pmatch, 0, sizeof(*pmatch));
		}
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

AR_NAMESPACE_END





