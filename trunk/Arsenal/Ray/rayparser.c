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



#include "ray.h"
#include "rayparser_impl.h"

AR_NAMESPACE_BEGIN


void			RAY_InitParser()
{
		RAY_InitParserImpl();


}

void			RAY_UnInitParser()
{
		RAY_UnInitParserImpl();
}






struct __ray_parser_tag
{
		rayReport_t		report;
		
		bool_t			is_error;
		lexMatch_t		*lexer;
		psrContext_t	*parser_ctx;

		rayBlock_t		*outer_block;
		rayBlock_t		*current_block;

		size_t			*alignment;
		size_t			align_cnt;
		size_t			align_cap;

};


/*************************************************default parser ctx***************************************/


#define RAY_DEF_ALIGNMENT		4

rayParser_t*	RAY_CreateParser(const rayReport_t *report)
{
		rayParser_t		*parser;
		
		AR_ASSERT(report != NULL && report->report_func != NULL);

		parser		=	AR_NEW0(rayParser_t);
		parser->report = *report;
		
		parser->lexer = RAY_BuildLexer();
		AR_ASSERT(parser->lexer != NULL);
		
		parser->parser_ctx = RAY_BuildParserContext((void*)parser);
		AR_ASSERT(parser->parser_ctx != NULL);
		
		RAY_PushAlignment(parser, RAY_DEF_ALIGNMENT);
		
		/*
		LEX_MatchFlags(parser->lexer, LEX_REPORT_SKIP, true);
		*/

		return parser;
}

void			RAY_DestroyParser(rayParser_t		*parser)
{
		AR_ASSERT(parser != NULL);

		RAY_ReleaseParserContext(parser->parser_ctx);
		RAY_ReleaseLexer(parser->lexer);

		if(parser->alignment)AR_DEL(parser->alignment);
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




size_t			RAY_GetAlignment(const rayParser_t	*parser)
{
		AR_ASSERT(parser != NULL && parser->align_cnt > 0);
		return parser->alignment[parser->align_cnt-1];
}

void			RAY_PushAlignment(rayParser_t	*parser, size_t align)
{
		AR_ASSERT(parser != NULL);
		if(parser->align_cnt == parser->align_cap)
		{
				parser->align_cap = (parser->align_cap + 1) * 2;
				parser->alignment = AR_REALLOC(size_t, parser->alignment, parser->align_cap);
		}
		parser->alignment[parser->align_cnt++] = align;
}

void			RAY_PopAlignment(rayParser_t	*parser)
{
		AR_ASSERT(parser != NULL);
		if(parser->align_cnt > 1)parser->align_cnt--;
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





/*******************************************************************************************************/
#if(0)
rayNode_t*		RAY_Parse(rayParser_t			*parser, const wchar_t *src)
{
		lexToken_t		tok;
		psrToken_t		psr_tok;
		bool_t			has_error;

		rayNode_t		*result;
		AR_ASSERT(parser != NULL && src != NULL);

		result = NULL;
		LEX_ResetInput(parser->lexer, src);
		
		has_error = false;
		
		while(!has_error)
		{
				if(LEX_Match(parser->lexer, &tok))
				{
						/*
						wchar_t buf[1024];
						AR_wcsncpy(buf, tok.str, tok.count);
						buf[tok.count] = 0;
						AR_printf(L"%ls\r\n",buf);
						*/

						PSR_TOTERMTOK(&tok, &psr_tok);
						if(psr_tok.term_val == TOK_EOI)
						{
								psr_tok.term_val = TOK_DONE_ID;
								if(!PSR_AddToken(parser->parser_ctx, &psr_tok))
								{
										has_error = true;
										continue;
								}
								psr_tok.term_val = TOK_EOI;
						}
						
						if(!PSR_AddToken(parser->parser_ctx, &psr_tok))
						{
								has_error = true;
						}
				}else
				{
						has_error = true;
				}
		}

		if(has_error)
		{
				parser->is_error = true;
		}else
		{
				result = (rayNode_t*)PSR_GetResult(parser->parser_ctx);
		}
		
		LEX_ClearInput(parser->lexer);
		PSR_Clear(parser->parser_ctx);
		return NULL;
}
#endif

rayNode_t*		RAY_Parse(rayParser_t			*parser, const wchar_t *src)
{
		lexToken_t		tok;

		bool_t			has_error;

		rayNode_t		*result;
		AR_ASSERT(parser != NULL && src != NULL);

		result = NULL;
		LEX_ResetInput(parser->lexer, src);
		
		has_error = false;
		
		while(!has_error)
		{
				if(LEX_Match(parser->lexer, &tok))
				{
						wchar_t buf[1024];
						AR_wcsncpy(buf, tok.str, tok.count);
						buf[tok.count] = 0;
						AR_printf(L"%ls\r\n",buf);

						if(tok.value == TOK_EOI)
						{
								break;
						}
						
						
				}else
				{
						has_error = true;
				}
		}

		if(has_error)
		{
				parser->is_error = true;
		}else
		{
				AR_printf(L"%ls\r\n", L"success!");
		}
		
		LEX_ClearInput(parser->lexer);
		PSR_Clear(parser->parser_ctx);
		return NULL;
}



AR_NAMESPACE_END
