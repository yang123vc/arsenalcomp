
 
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


#include "../Arsenal/Common/common.h"
#include "../Arsenal/Lex/lex.h"
#include "../Arsenal/Parser/parser.h"
#include "../Arsenal/Parser/lr_action.h"

AR_NAMESPACE_BEGIN

#include "v3_checker_impl.h"


static lex_t 				*__g_v3_lex = NULL;
static psrGrammar_t			*__g_v3_grammar = NULL;
static const parser_t		*__g_v3_parser = NULL;


arStatus_t V3_CheckerInit()
{
		arStatus_t status;
		status = AR_S_YES;

		__g_v3_lex = __build_lex();

		if(__g_v3_lex == NULL)
		{
				status = AR_E_FAIL;
				goto INVLAID_POINT;
		}

		__g_v3_grammar = __build_grammar(&__g_expr_handler);

		if(__g_v3_grammar == NULL)
		{
				status = AR_E_FAIL;
				goto INVLAID_POINT;
		}
		__g_v3_parser = Parser_CreateParser(__g_v3_grammar, PARSER_LALR);

		if(__g_v3_parser == NULL)
		{
				status = AR_E_FAIL;
				goto INVLAID_POINT;
		}
		
		return status;

INVLAID_POINT:
		
		if(__g_v3_parser)
		{
				Parser_DestroyParser(__g_v3_parser);
				__g_v3_parser = NULL;
		}

		if(__g_v3_grammar)
		{
				Parser_DestroyGrammar(__g_v3_grammar);
				__g_v3_grammar = NULL;
		}
		if(__g_v3_lex)
		{
				Lex_Destroy(__g_v3_lex);
				__g_v3_lex = NULL;
		}

		return status;
}


void V3_CheckerUnInit()
{
		if(__g_v3_parser)
		{
				Parser_DestroyParser(__g_v3_parser);
				__g_v3_parser = NULL;
		}

		if(__g_v3_grammar)
		{
				Parser_DestroyGrammar(__g_v3_grammar);
				__g_v3_grammar = NULL;
		}

		if(__g_v3_lex)
		{
				Lex_Destroy(__g_v3_lex);
				__g_v3_lex = NULL;
		}

}



exprNode_t*	v3_build_check_expr(const wchar_t *expr)
{
#if(0)
		psrContext_t 			*parser_ctx;
		lexMatch_t			*match;
		lexToken_t			token;
		AR_ASSERT(expr != NULL);

		 = Parser_CreateContext(parser, NULL);
		= Lex_CreateMatch(lex);

		Lex_ResetInput(match, input.c_str());
		Lex_MatchClearFlags(match);
		
		
		AR_memset(&token, 0, sizeof(token));
		
		arStatus_t status = AR_S_YES;

		while(status == AR_S_YES)
		{

				status = ARSpace::Lex_Match(match, &token);

				if(status == AR_S_YES)
				{
						ARSpace::psrToken_t		psr_tok;

						PARSER_TOTERMTOK(&token, &psr_tok);

						status = ARSpace::Parser_AddToken(parser_ctx, &psr_tok);
						
						if(token.value == 0)
						{
								break;
						}

				}else if(status == AR_S_NO)
				{
						size_t len = wcslen(ARSpace::Lex_GetNextInput(match));
						if(len > 20) len = 20;

						size_t line;
						ARSpace::Lex_MatchGetCoordinate(match, NULL, &line, NULL);

						
						wchar_t msg[1024];
						AR_wcsncpy(msg, ARSpace::Lex_GetNextInput(match),  (int)len);
						AR_error(AR_ERR_FATAL, L"Input Error : %ls line : %Iu", msg, line);
						AR_abort();

				}else
				{
						AR_error(AR_ERR_FATAL, L"inner error !\r\n");
						AR_abort();
				}

		}

		Parser_DestroyContext(parser_ctx);
		parser_ctx = NULL;

		Parser_DestroyParser(parser);
		parser = NULL;

		Parser_DestroyGrammar(gmr);
		gmr = NULL;

		Lex_Destroy(lex);
		lex = NULL;

		Lex_DestroyMatch(match);
		match = NULL;
#endif

		return NULL;
}





ar_bool_t v3check(const wchar_t *expr)
{
		return false;
}





void v3checker_test()
{
		wchar_t expr[1024];

		while(true)
		{
				_getws(expr);

				if(AR_wcscmp(expr, L"quit") == 0)
				{
						break;
				}

				ar_bool_t is_valid = v3check(expr);

				if(is_valid)
				{
						AR_printf(L"'%ls' is valid\r\n", expr);
				}else
				{
						AR_printf(L"'%ls' failed\r\n", expr);
				}
		}
		
} 



AR_NAMESPACE_END
