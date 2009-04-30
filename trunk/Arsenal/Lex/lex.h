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
		bool			is_skip;
}lexAction_t;


typedef struct __lex_tag lex_t;


lex_t*	LEX_Create();
void	LEX_Destroy(lex_t *lex);
bool	LEX_InsertName(lex_t *lex, const wchar_t *name, const wchar_t *expr);
bool	LEX_InsertRule(lex_t *lex, const wchar_t *rule, const lexAction_t *action);
bool	LEX_Insert(lex_t *lex, const wchar_t *input);
bool	LEX_GenerateTransTable(lex_t *lex);
void	LEX_Clear(lex_t *lex);


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


/****************************************config**********************************************/


typedef enum {LEX_NAME, LEX_PATTERN} arPatternT_t;

typedef struct __lex_cfg_name_tag
{
		wchar_t	*name;
		wchar_t	*expr;
}lexCfgName_t;


typedef struct __lex_cfg_pattern_tag
{
		wchar_t			*pattern;
		lexAction_t		action;	/*action∫Õpattern”√”⁄LEX_PATTERN*/
}lexCfgPattern_t;


typedef struct __lex_config_tag
{
		arPatternT_t	type;
		union{
				lexCfgName_t		name;
				lexCfgPattern_t		pattern;
		};
		struct __lex_config_tag	*next;
}lexConfig_t;


const lexConfig_t*	LEX_CreateConfig(const wchar_t *pattern, const wchar_t **next_input);
void				LEX_DestroyConfig(const lexConfig_t *pattern);


const wchar_t*		  LEX_Config(lex_t *lex, const wchar_t *pattern);


AR_NAMESPACE_END


















#endif
