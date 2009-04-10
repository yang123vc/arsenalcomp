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


#ifndef __LEX_H__
#define __LEX_H__

#include "..\Common\common.h"



AR_NAMESPACE_BEGIN




#define AR_MAX_LEXNAME					128
#define AR_MAX_LENEXPR					1024

#define LEX_Init()		AR_NOOP
#define LEX_UnInit()	AR_NOOP



typedef struct __lex_action_tag
{
		size_t			type;
		size_t			priority;
}lexAction_t;


typedef struct __lex_tag lex_t;


lex_t*	LEX_Create();
void	LEX_Destroy(lex_t *lex);
bool	LEX_InsertName(lex_t *lex, const wchar_t *name, const wchar_t *expr);
bool	LEX_InsertRule(lex_t *lex, const wchar_t *rule, const lexAction_t *action);
bool	LEX_Insert(lex_t *lex, const wchar_t *input);
bool	LEX_GenerateTransTable(lex_t *lex);
void	LEX_Clear(lex_t *lex);

void	LEX_InsertSkipAction(lex_t *lex, size_t action_type);


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
		bool					is_ok;
		
		wchar_t					*input;
		const wchar_t			*next;
		size_t					line;
		size_t					col;
}lexMatch_t;



void LEX_InitMatch(lexMatch_t *pmatch, const wchar_t *input);

void LEX_UnInitMatch(lexMatch_t *pmatch);

void LEX_ResetMatch(lexMatch_t *pmatch, const wchar_t *input);

const wchar_t* LEX_GetNextInput(const lexMatch_t *match);

bool LEX_Match(lex_t *lex, lexMatch_t *match, lexToken_t *tok);




AR_NAMESPACE_END


















#endif
