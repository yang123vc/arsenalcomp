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


#include "tguparser.h"
#include "tguutility.h"

AR_NAMESPACE_BEGIN

#include "tgu_parser_impl.h"




/*
此arIOCtx_t定义只会被lex_t,psrGrammar_t,以及parser_t内部使用，如果建构parser,lex,以及grammar时如果没有出现错误则不可能调用到此函数，
因此，此函数永不会被调用。
*/

static void	AR_STDCALL tgu_on_error(int_t level, const wchar_t *msg, void *ctx)
{
		AR_ASSERT(false);

}


void	AR_STDCALL tgu_on_print(const wchar_t *msg, void *ctx)
{
		AR_ASSERT(false);
}



static arIOCtx_t	__def_io_ctx = 
{		
		tgu_on_error,
		tgu_on_print,
		NULL
};

/***********************************************************************************************************/




/************************************************************parser core***********************************/

static			arSpinLock_t	__g_lock;
static	const	lex_t			*__g_lex	 = NULL;
static	const	psrGrammar_t	*__g_grammar = NULL;
static	const	parser_t		*__g_parser = NULL;

static	void	__parser_core_init()
{
		AR_InitSpinLock(&__g_lock);
		__g_lex     = __build_lex(&__def_io_ctx);
		__g_grammar	= __build_grammar(&__def_handler_ctx, &__def_io_ctx);
		__g_parser	= Parser_CreateParser(__g_grammar, PARSER_LALR);
}



static	void	__parser_core_uninit()
{
		Parser_DestroyParser(__g_parser);
		Parser_DestroyGrammar((psrGrammar_t*)__g_grammar);
		Lex_Destroy((lex_t*)__g_lex);

		__g_lex	= NULL;
		__g_grammar = NULL;
		__g_parser = NULL;
		AR_UnInitSpinLock(&__g_lock);
}



static lexMatch_t*		__build_match()
{
		lexMatch_t		*match;
		AR_ASSERT(__g_lex != NULL);
		
		AR_LockSpinLock(&__g_lock);
		match = Lex_CreateMatch(__g_lex, &__def_io_ctx);
		AR_UnLockSpinLock(&__g_lock);
		return match;
}

static void			__release_match(lexMatch_t	*match)
{
		AR_ASSERT(match != NULL);
		Lex_DestroyMatch(match);
		
}



static psrContext_t*		__build_parser_context(void *ctx)
{
		psrContext_t	*parser_context;
		AR_ASSERT(__g_parser != NULL && __g_grammar != NULL);
		AR_LockSpinLock(&__g_lock);
		parser_context = Parser_CreateContext(__g_parser, ctx);
		AR_UnLockSpinLock(&__g_lock);
		return parser_context;
}


static void				__release_parser_context(psrContext_t		*parser_context)
{
		AR_ASSERT(__g_parser != NULL && __g_grammar != NULL);
		AR_ASSERT(parser_context != NULL);
		AR_LockSpinLock(&__g_lock);
		Parser_DestroyContext(parser_context);
		AR_UnLockSpinLock(&__g_lock);
}





void	TGU_InitParser()
{
		__parser_core_init();
}

void	TGU_UnInitParser()
{
		__parser_core_uninit();
}




tguParser_t*	TGU_CreateParser(tguReport_t	*report)
{
		tguParser_t		*ret;
		AR_ASSERT(report != NULL && report->report_func);

		ret = AR_NEW0(tguParser_t);
		ret->report = *report;
		
		ret->match = __build_match();
		ret->parser_context = __build_parser_context((void*)ret);

		return ret;
}

void			TGU_DestroyParser(tguParser_t	*parser)
{
		AR_ASSERT(parser != NULL);
		
		__release_match(parser->match);
		__release_parser_context(parser->parser_context);

		AR_DEL(parser);
}


static void __on_lex_error(tguParser_t	*parser)
{
		size_t n;
		size_t	line, col;
		lexToken_t		token;
		tguReportInfo_t	info;

		wchar_t tok_buf[128];
		wchar_t buf[512];

		AR_ASSERT(parser != NULL);



		AR_memset(&info, 0, sizeof(info));
		n = AR_wcslen(Lex_GetNextInput(parser->match));
		if(n > 10)
		{
				n = 10;
		}
		AR_wcsncpy(tok_buf, Lex_GetNextInput(parser->match), n);

		Lex_MatchGetCoordinate(parser->match, &line, &col);

		AR_swprintf(buf, 512, L"Invalid Token %ls...(%"AR_PLAT_INT_FMT L"d : %"AR_PLAT_INT_FMT L"d)\r\n", tok_buf, line, col);

		info.type = TGU_REPORT_ERROR_LEX_T;
		info.lex_error.msg = buf;

		token.value = 0;
		token.count =	n;	
		token.str =  tok_buf;

		Lex_MatchGetCoordinate(parser->match, &token.line, &token.col);

		info.lex_error.tok = &token;
		parser->report.report_func(&info, parser->report.report_ctx);

		AR_ASSERT(*Lex_GetNextInput(parser->match) != L'\0');

		Lex_SkipTo(parser->match, L";");
		Lex_ClearError(parser->match);
		
		parser->has_error = true;
}

tguSyntaxTree_t*	TGU_ParseCode(tguParser_t	*parser, const wchar_t *sources_name, const wchar_t *code)
{
		tguSyntaxTree_t	*result = NULL;
		lexToken_t		token;
		psrToken_t		psrtok;
		
		bool_t			is_ok;
		AR_ASSERT(parser != NULL && code != NULL);

		Lex_ResetInput(parser->match, code);
		Parser_Clear(parser->parser_context);

		result = TGU_CreateSyntaxTree(sources_name);
		parser->result = result;
		parser->top_block = result->global_block;
		parser->current_function = NULL;
		parser->has_error = false;
		parser->loop_level = 0;

		

		is_ok = true;
		while(is_ok)
		{
				is_ok = Lex_Match(parser->match, &token);

				if(!is_ok)
				{
						__on_lex_error(parser);
						is_ok = true;
						

				}else
				{
						PARSER_TOTERMTOK(&token, &psrtok);
						is_ok = Parser_AddToken(parser->parser_context, &psrtok);
						if(is_ok && token.value == PARSER_EOI_TOKVAL)break;
				}

		};


		if(!is_ok)
		{
				TGU_DestroySyntaxTree(result);
				result = NULL;
		}

		return result;
}


AR_NAMESPACE_END


