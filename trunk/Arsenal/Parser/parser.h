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

#ifndef __PARSER_H__
#define __PARSER_H__

#include "..\Common\Common.h"
#include "..\Lex\lex.h"

AR_NAMESPACE_BEGIN

typedef enum
{
		PSR_NO_ERROR = 0,
		PSR_INVALID_NAMELEN,
		PSR_INVALID_TOKEN,
		PSR_DUPLICATE_TOKEN_DEFINE,
		PSR_INVALID_PRIO,
		PSR_INVALID_NONTERM_ID,
		PSR_INVALID_RULE,
		PSR_DUPLICATE_NONTERM_DEFINE,
		PSR_INVALID_EPSILON_DEFINE,
		PSR_INVALID_GRAMMAR_PATTERN
}psrError_t;



#define PARSER_MAXNAME	256		/*non-term || term 最长名称*/

#define PSR_MAX_NAMELEN			(PARSER_MAXNAME-1)
#define PSR_MIN_TOKENVAL		257
#define PSR_MIN_NONTERM_ID		129
#define PSR_MIN_PRIOVAL			1
#define PSR_MAX_PRIOVAL			128
#define PSR_MIN_PRIOTERMVAL		0xFFFF
#define PSR_NONTERM_ID_INRULE	(PSR_MIN_NONTERM_ID-1)/*在产生式体中的非终结符ID*/

/******************************************************Parser********************************************************/

#define PSR_MAX_TOKENLEN		4096
#define PSR_EOI_TOKVAL			1

typedef void			psrNode_t;

typedef lexToken_t		psrToken_t;


typedef struct __parser_context_tag
{
		bool_t	   (*token_f)(psrToken_t *tok, void *ctx);		/*parser调用此函数得到下一个词法值*/
		psrNode_t* (*leaf_f)(const psrToken_t *tok,void *ctx);/*通过词法值创建叶节点*/
		psrNode_t* (*node_f)(size_t rule_id, const wchar_t *rule_name, psrNode_t **nodes, size_t count, void *ctx); /*通过子节点和产生式ID创建父节点*/
		void	   (*destroy_f)(psrNode_t *node, void *ctx);/*如果产生语法或者词法错误，则通过此函数析构所有已建立的节点*/
		void		*ctx;
}psrCtx_t;


typedef struct __parser_tag parser_t;

typedef enum
{
		PSR_SLR,
		PSR_LR1,
		PSR_LALR
}psrEngineType_t;

parser_t* PSR_CreateParser(const wchar_t *grammar, psrEngineType_t type);

void	  PSR_DestroyParser(parser_t *parser);

psrNode_t* PSR_Parse(parser_t *parser, psrCtx_t *ctx);



AR_NAMESPACE_END


#endif
