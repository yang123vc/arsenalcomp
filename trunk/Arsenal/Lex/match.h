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
		size_t			value;
		size_t			line;
		size_t			col;
}lexToken_t;


enum
{
		LEX_REPORT_SKIP		=		0x0001,
		LEX_SINGLE_LINE		=		0x0002,
		LEX_IGNORE_CASE		=		0x0004
};

typedef struct __lex_match_result_tag
{
		bool_t				is_ok;
		const wchar_t		*input;
		const wchar_t		*next;
		size_t				line;
		size_t				col;

		uint_t				flags;
}lexMatch_t;



void			LEX_InitMatch(lexMatch_t *pmatch, const wchar_t *input);

void			LEX_UnInitMatch(lexMatch_t *pmatch);


void			LEX_ResetInput(lexMatch_t *pmatch, const wchar_t *input);

void			LEX_ResetMatch(lexMatch_t *pmatch);

const wchar_t*	LEX_GetNextInput(const lexMatch_t *match);

bool_t			LEX_IsError(const lexMatch_t *match);
void			LEX_ClearError(lexMatch_t *match);

/*跳到下一个非空白token*/
void			LEX_Skip(lexMatch_t *pmatch);
/*跳到与tok相同的符号，如果未找到,pmatch直接跳到符号结尾*/
void			LEX_SkipTo(lexMatch_t *pmatch, const wchar_t *tok);

/*丢弃N个字符*/
void			LEX_SkipN(lexMatch_t *pmatch, size_t nchar);


void			LEX_MatchFlags(lexMatch_t *pmatch, uint_t flags, bool_t is_on);


AR_NAMESPACE_END




#endif
