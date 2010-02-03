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

#include "raygmr.h"
AR_NAMESPACE_BEGIN


lex_t*	Ray_BuildLexer(const arIOCtx_t *io)
{
		lex_t	*lex;
		size_t i;
		
		lex = LEX_Create(io);


		for(i = 0; i < sizeof(__g_lex_name)/ sizeof(__g_lex_name[0]); ++i)
		{
				if(!LEX_Insert(lex, __g_lex_name[i]))
				{
						LEX_Destroy(lex);
						AR_error(AR_ERR_FATAL, L"Ray_BuildLexer failed : %ls\r\n",   __g_lex_name[i]);
						AR_ASSERT(false);
						return NULL;
				}
		}

		for(i = 0; i < sizeof(__g_lex_pattern) / sizeof(__g_lex_pattern[0]); ++i)
		{
				lexAction_t		act;
				act.is_skip = __g_lex_pattern[i].is_skip;
				act.priority = __g_lex_pattern[i].prec;
				act.value = __g_lex_pattern[i].val;
				if(!LEX_InsertRule(lex, __g_lex_pattern[i].regex, &act))
				{
						LEX_Destroy(lex);
						AR_error(AR_ERR_FATAL, L"Ray_BuildLexer failed : %ls\r\n",   __g_lex_pattern[i].regex);
						AR_ASSERT(false);
						return NULL;
				}
		}


		return lex;

}





static void	AR_STDCALL ray_parser_free(psrNode_t *node, void *ctx)
{
		AR_printf(L"%ls\r\n", L"ray_parser_free");
}

static void		AR_STDCALL ray_parser_error(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)
{
		AR_printf(L"%ls\r\n", L"ray_parser_error");

}


psrNode_t*		AR_STDCALL ray_handle_token(const psrToken_t *tok,void *ctx)
{
		return NULL;
}


static const psrCtx_t		__g_default_psrctx = 
{
		ray_parser_error,
		ray_parser_free,
		NULL
};


psrGrammar_t*	Ray_BuildGrammar(const arIOCtx_t *io)
{
		psrGrammar_t	*grammar;
		size_t i;


		grammar = PSR_CreateGrammar(&__g_default_psrctx, io);
		
		for(i = 0; i < AR_NELEMS(__ray_term); ++i)
		{
				if(!PSR_InsertTerm(grammar, __ray_term[i].name, __ray_term[i].val, __ray_term[i].assoc, __ray_term[i].prec, __ray_term[i].leaf_f))
				{
						PSR_DestroyGrammar(grammar);
						grammar = NULL;
						AR_error(AR_ERR_FATAL, L"Failed to build ray grammar  : Term : %ls\r\n",   __ray_term[i].name);
						AR_ASSERT(false);
						return NULL;
				}
		}

		for(i = 0; i < AR_NELEMS(__ray_rule); ++i)
		{
				if(!PSR_InsertRuleByStr(grammar, __ray_rule[i].rule, __ray_rule[i].prec_token, __ray_rule[i].handler, __ray_rule[i].auto_ret))
				{
						PSR_DestroyGrammar(grammar);
						grammar = NULL;
						AR_error(AR_ERR_FATAL, L"Failed to build ray grammar  : Rule : %ls\r\n",   __ray_rule[i].rule);
						AR_ASSERT(false);
						return NULL;
				}

		}
		

		if(!PSR_CheckIsValidGrammar(grammar))
		{
				PSR_DestroyGrammar(grammar);
				grammar = NULL;
				AR_error(AR_ERR_FATAL, L"%ls\r\n", L"Ray_BuildGrammar failed");
				AR_ASSERT(false);
				return NULL;
		}

		return grammar;
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
