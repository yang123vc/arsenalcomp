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

#ifndef AR_LEX_H
#define AR_LEX_H

#include "..\Common\Common.h"

AR_NAMESPACE_BEGIN

typedef enum
{
		LEX_NO_ERROR			= 0,	/*0*/
		LEX_PAT_INVALID_LEN,			/*1*/
		LEX_PAT_INVALID_TERMCHAR,		/*2*/
		LEX_PAT_INVALID_TRANSFORMCHAR,	/*3*/
		LEX_PAT_INVALID_OPERCHAR,		/*4*/
		LEX_PAT_INVALID_LOOPCOUNT,		/*5*/
		LEX_PAT_INVALID_CHARSET,		/*6*/
		LEX_PAT_INVALID_EXPRNAME,		/*7*/
		LEX_PAT_INVALID_EMPTYEXPR,		/*8*/
		LEX_PAT_INVALID_EMPTYQUOTE,		/*9*/
		LEX_DEF_NAMENOTFOUND,			/*10*/
		LEX_DEF_NAMEDUP,				/*11*/
		
		LEX_GEN_EMPTY_PATTERN,			/*12*/
		LEX_GEN_INVALID_INPUT,			/*13*/
		
		LEX_RUN_INVALID_INPUTCHAR,		/*14*/
		LEX_RUN_NOTFOUND_PATTERN,		/*15*/
		LEX_RUN_EOI,					/*16*/


		LEX_BUILD_INVALID_NAME,			/*17*/
		LEX_BUILD_INVALID_PATTERN		/*18*/
}lexError_t;


#define AR_MAX_LEXNAME					128
typedef wchar_t		lexName_t[AR_MAX_LEXNAME];

typedef struct __lex_action_tag
{
		size_t			type;
		int				priority;
}lexAction_t;


typedef struct __lex_token_tag
{
		const wchar_t	*tok;
		size_t			count;
		size_t			type;
		size_t			x;
		size_t			y;
}lexToken_t;




typedef struct __lex_tag		lex_t;



lex_t*			LEX_CreateLex(const wchar_t *pattern);
void			LEX_DestroyLex(lex_t *lex);



#define			LEX_DEF(_pat_) L##_pat_##L"\r\n"





typedef struct __lex_match_result_tag
{
		lexError_t				err;
		const wchar_t			*next;
		size_t					x;
		size_t					y;
		lexToken_t				token;

		wchar_t					*input;
}lexMatch_t;


void LEX_InitMatch(lexMatch_t *pmatch, const wchar_t *input);


void LEX_UnInitMatch(lexMatch_t *pmatch);


bool_t	LEX_Match(lex_t *lex, lexMatch_t *match);



AR_NAMESPACE_END


#endif
