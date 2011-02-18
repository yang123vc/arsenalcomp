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

#include "../Common/common.h"


AR_NAMESPACE_BEGIN





void	Lex_Init();
void	Lex_UnInit();



typedef struct __lex_action_tag
{
		size_t			value;
		size_t			priority;
		bool_t			is_skip;
}lexAction_t;





typedef struct __lex_rule_set
{
		lexAction_t				*action;
		struct __rgx_node_tag	**nodes;
		size_t					count;
		size_t					cap;
}lexRuleSet_t;




typedef struct __lex_tag 
{
		struct __rgx_name_set_tag		*name_tbl;
		
		lexRuleSet_t					rule_set;

		arString_t						*last_err_msg;
}lex_t;


lex_t*	Lex_Create();
void	Lex_Destroy(lex_t *lex);

const wchar_t*	Lex_GetLastError(const lex_t *lex);
void			Lex_ClearLastError(lex_t *lex);

bool_t	Lex_InsertName(lex_t *lex, const wchar_t *name, const wchar_t *expr);
bool_t	Lex_InsertRule(lex_t *lex, const wchar_t *rule, const lexAction_t *action);

bool_t	Lex_RemoveByName(lex_t *lex, const wchar_t *name);
bool_t	Lex_RemoveByValue(lex_t *lex, size_t value);

bool_t	Lex_Insert(lex_t *lex, const wchar_t *input);
bool_t	Lex_GenerateTransTable(lex_t *lex);
void	Lex_Clear(lex_t *lex);








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
		LEX_SINGLE_LINE		=		0x0002,
		LEX_IGNORE_CASE		=		0x0004
};



struct __prog_set_tag;
typedef struct __prog_set_tag	lexProgSet_t;

struct __lex_match_result_tag;
typedef struct __lex_match_result_tag	lexMatch_t;



struct __lex_match_result_tag
{
		bool_t							is_ok;
		const wchar_t					*input;
		const wchar_t					*next;
		size_t							line;
		size_t							col;
		uint_32_t						next_action;

		uint_t							flags;
		lexProgSet_t					*prog_set;
};



lexMatch_t*		Lex_CreateMatch(const lex_t *lex);

void			Lex_DestroyMatch(lexMatch_t *pmatch);

void			Lex_ResetInput(lexMatch_t *pmatch, const wchar_t *input);

void			Lex_ClearInput(lexMatch_t *pmatch);

void			Lex_ResetMatchState(lexMatch_t *pmatch);

void			Lex_ResetMatchIoContext(lexMatch_t *pmatch, const arIOCtx_t *io);

const wchar_t*	Lex_GetNextInput(const lexMatch_t *match);

bool_t			Lex_IsError(const lexMatch_t *match);

void			Lex_ClearError(lexMatch_t *match);

/*跳到下一个非空白token*/
void			Lex_Skip(lexMatch_t *pmatch);
/*跳到与tok相同的符号，如果未找到,pmatch直接跳到符号结尾*/
void			Lex_SkipTo(lexMatch_t *pmatch, const wchar_t *tok);

/*丢弃N个字符*/
void			Lex_SkipN(lexMatch_t *pmatch, size_t nchar);

/*Lex_PutBack会重置Match_t的行列号及next指针为此token的，并不会簿记一个队列*/
void			Lex_PutBack(lexMatch_t *pmatch, const lexToken_t *tok);


void			Lex_MatchFlags(lexMatch_t *pmatch, uint_t flags, bool_t is_on);
void			Lex_MatchClearFlags(lexMatch_t *pmatch);

void			Lex_MatchGetCoordinate(const lexMatch_t *pmatch, size_t *line, size_t *col);



bool_t			Lex_Match(lexMatch_t *match, lexToken_t *tok);


AR_NAMESPACE_END


















#endif
