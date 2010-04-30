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



#include "rayparser.h"
#include "rayparser_impl.h"

AR_NAMESPACE_BEGIN





struct __ray_parser_tag
{
		rayReport_t		report;
		
		bool_t			is_error;
		lex_t			*lex;
		psrGrammar_t	*grammar;
		parser_t		*parser;

		rayBlock_t		*outer_block;
		rayBlock_t		*current_block;

};


/*************************************************default io***************************************/

static void AR_STDCALL __def_print(const wchar_t *msg, void *ctx)
{
		/*
		rayReportMsg_t	report_msg;
		rayReport_t		*report = (rayReport_t*)ctx;

		AR_memset(&report_msg, 0, sizeof(report_msg));
		report_msg.type = RAY_REPORT_MESSAGE;
		report_msg.message = msg;
		report->report_func(&report_msg, report->ctx);
		*/
		AR_ASSERT(false);

}


static void AR_STDCALL __def_error(int_t level, const wchar_t* msg, void *ctx)
{
		/*
		__def_print(msg, ctx);
		*/
		AR_ASSERT(false);
}


/*************************************************default parser ctx***************************************/


static void		AR_STDCALL __def_free_node(psrNode_t *node, void *ctx)
{
		AR_ASSERT(node != NULL);
		if(node)RAY_DestroyParserNode((rayNode_t*)node);

}

static void		AR_STDCALL __def_on_error(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)
{


}


rayParser_t*	RAY_CreateParser(const rayReport_t *report)
{
		arIOCtx_t		io;
		psrCtx_t		psr_ctx;
		rayParser_t		*parser;
		
		AR_ASSERT(report != NULL && report->report_func != NULL);

		parser		=	AR_NEW0(rayParser_t);
		parser->report = *report;

		io.on_error = __def_error;
		io.on_print = __def_print;
		io.ctx = (void*)&parser->report;

		psr_ctx.error_f = __def_on_error;
		psr_ctx.free_f	= __def_free_node;
		psr_ctx.ctx = (void*)parser;

		parser->lex = RAY_BuildLexer(&io);
		AR_ASSERT(parser->lex != NULL);
		parser->grammar = RAY_BuildGrammar(&psr_ctx, &io);
		AR_ASSERT(parser->grammar != NULL);
		parser->parser = PSR_CreateParser(parser->grammar, PSR_LALR);
		AR_ASSERT(parser->parser != NULL);

		return parser;
}

void			RAY_DestroyParser(rayParser_t		*parser)
{
		AR_ASSERT(parser != NULL);

		PSR_DestroyParser(parser->parser);
		PSR_DestroyGrammar(parser->grammar);
		LEX_Destroy(parser->lex);
		AR_DEL(parser);
}



bool_t			RAY_ParserHasError(const rayParser_t	*parser)
{
		AR_ASSERT(parser != NULL);
		return parser->is_error;
}

void			RAY_SetParserError(rayParser_t	*parser, bool_t is_error)
{
		AR_ASSERT(parser != NULL);
		parser->is_error = is_error;
}

/***********************************************Block****************************************/

void			RAY_SetOuterBlock(rayParser_t	*parser, rayBlock_t		*block)
{
		AR_ASSERT(parser != NULL && block != NULL);

		parser->outer_block = block;
}

rayBlock_t*		RAY_GetOuterBlock(const rayParser_t *parser)
{
		AR_ASSERT(parser != NULL);
		return (rayBlock_t*)(parser->outer_block);
}

bool_t	RAY_IsOuterBlock(const rayParser_t *parser, const rayBlock_t *block)
{
		return parser->outer_block == block;
}


rayBlock_t*		RAY_GetCurrentBlock(const rayParser_t *parser)
{
		return (rayBlock_t*)parser->current_block;
}

/**************************************************parser Node****************************************************/



rayNode_t*		RAY_CreateParserNode(rayNodeType_t type)
{
		return NULL;
}

void			RAY_DestroyParserNode(rayNode_t	   *node)
{
		
}




AR_NAMESPACE_END
