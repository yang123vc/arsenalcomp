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


#ifndef __LEX_MATCH_H__
#define __LEX_MATCH_H__

#include "../Common/common.h"


AR_NAMESPACE_BEGIN


typedef struct __lex_token_tag
{
		const wchar_t	*str;
		size_t			count;
		size_t			type;
		size_t			line;
		size_t			col;
}lexToken_t;

typedef struct __lex_match_result_tag
{
		bool_t					is_ok;
		
		wchar_t					*input;
		const wchar_t			*next;
		size_t					line;
		size_t					col;
}lexMatch_t;



void LEX_InitMatch(lexMatch_t *pmatch, const wchar_t *input);

void LEX_UnInitMatch(lexMatch_t *pmatch);

void LEX_ResetInput(lexMatch_t *pmatch, const wchar_t *input);

void LEX_ResetMatch(lexMatch_t *pmatch);

const wchar_t* LEX_GetNextInput(const lexMatch_t *match);


AR_NAMESPACE_END




#endif
