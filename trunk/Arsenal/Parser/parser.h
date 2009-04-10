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

#include "..\Common\common.h"
#include "..\Lex\lex.h"

AR_NAMESPACE_BEGIN




#define PSR_EOI_TOKVAL			0/*end of input符号的词法值*/
#define PSR_EPSILON_TOKVAL		1
#define PSR_LALR_TOKVAL			2
#define PSR_ERROR_TOKVAL		3

#define PSR_MIN_PRIOTERMVAL		0xFFFF
#define PSR_MIN_TOKENVAL		257





/**********************************必须在所有其它PSR族函数之前调用，否则行为未定义***************************/

void	PSR_Init();
void	PSR_UnInit();


typedef enum
{
		PSR_LR0,
		PSR_SLR = PSR_LR0,
		PSR_LR1,
		PSR_LALR
}psrLRItemType_t;

typedef psrLRItemType_t psrModeType_t;

typedef void		psrNode_t;

typedef lexToken_t		psrToken_t;



typedef psrNode_t* (AR_STDCALL *psrLeafFunc_t)(const psrToken_t *tok,void *ctx);
typedef psrNode_t* (AR_STDCALL *psrNodeFunc_t)(size_t rule_id, const wchar_t *rule_name, psrNode_t **nodes, size_t count, void *ctx);
typedef void	   (AR_STDCALL *psrFreeFunc_t)(psrNode_t *node, void *ctx);

typedef void		(AR_STDCALL *psrErrorFunc_t)(const psrToken_t *tok, void *ctx);

typedef struct __parser_context_tag
{
		psrLeafFunc_t	leaf_f;
		psrNodeFunc_t	node_f;
		psrFreeFunc_t	free_f;
		psrErrorFunc_t	error_f;
		void			*ctx;
}psrCtx_t;


typedef struct __parser_grammar_tag		psrGrammar_t;
typedef struct __parser_action_tag		psrActionTable_t;
typedef struct __parser_symbol_tag		psrSymb_t;

typedef struct __parser_stack_tag		psrStack_t;
typedef struct __parser_node_set_tag	psrNodeSet_t;

typedef struct __parser_tag 
{
		psrGrammar_t			*grammar;
		psrActionTable_t		*tbl;
		psrStack_t				*stack;
		psrNodeSet_t			*node_set;
		bool					is_inerr;
		psrCtx_t				ctx;
		
		const psrSymb_t			**term_set;
		size_t					term_count;

		bool					is_accepted;
}parser_t;



parser_t* PSR_CreateParser(const psrGrammar_t *grammar, psrModeType_t type, const psrCtx_t *ctx);
 
void	  PSR_DestroyParser(parser_t *parser);

void	  PSR_Clear(parser_t *parser);

psrNode_t* PSR_GetResult(parser_t *parser);/*在状态为accepted之后才可以调用*/






typedef enum
{
		PSR_ERR_INVALID_TOKEN,
		PSR_ERR_INVALID_SYNTAX,
		PSR_ERR_ACCEPT,
		PSR_ERR_OK,
}psrError_t;

/*由于采用了一个增广的文法，所以当EOI被增加到stack中时，只可能出现错误或者成为接受状态，EOI永远不会被SHIFT到parser中*/
psrError_t PSR_AddToken(parser_t *parser, const psrToken_t *tok);








AR_NAMESPACE_END




#endif
