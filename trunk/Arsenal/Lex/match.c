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




AR_NAMESPACE_END





