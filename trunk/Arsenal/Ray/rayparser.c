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



lex_t*	Ray_BuildLexer(const arIOCtx_t *io)
{
		return __ray_build_lex_impl(io);
}





static void	AR_STDCALL ray_parser_free(psrNode_t *node, void *ctx)
{
		AR_printf(L"%ls\r\n", L"ray_parser_free");
}

static void		AR_STDCALL ray_parser_error(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)
{
		AR_printf(L"%ls\r\n", L"ray_parser_error");

}


static const psrCtx_t		__g_default_psrctx = 
{
		ray_parser_error,
		ray_parser_free,
		NULL
};




psrGrammar_t*	Ray_BuildGrammar(const arIOCtx_t *io)
{
		return __ray_build_grammar_impl(&__g_default_psrctx, io);
		
}





parser_t*		Ray_BuildParser(psrGrammar_t *grammar)
{
		parser_t *parser;
		AR_ASSERT(grammar != NULL);
		
		parser = PSR_CreateParser(grammar, PSR_LALR);

		if(PSR_CountParserConflict(parser))
		{
				{
						arString_t		*str = AR_CreateString();
						PSR_PrintParserConflict(parser, str);
						AR_StrPrint(str);
						AR_DestroyString(str);
				}
				
				AR_error(AR_ERR_FATAL, L"%ls\r\n", L"Ray_BuildParser failed");
				AR_ASSERT(false);
				PSR_DestroyParser(parser);
				return NULL;
		}

		return parser;
}



AR_NAMESPACE_END
