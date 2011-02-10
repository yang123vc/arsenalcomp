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

#pragma warning(disable : 4100)

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
		__g_grammar	= __build_grammar(&__g_handler , &__def_io_ctx);
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
		TGU_InitUtility();
		__parser_core_init();
}

void	TGU_UnInitParser()
{
		__parser_core_uninit();
		TGU_UnInitUtility();
}


void					TGU_ResetParser(tguParser_t	*parser)
{
		AR_ASSERT(parser != NULL);

		parser->abs_tree	= NULL;
		parser->has_error	= false;
		parser->top_block = NULL;
		parser->current_function = NULL;
		parser->loop_level = 0;
		parser->is_on_function_compound = false;
		parser->on_redef_function = false;
		parser->model_name = NULL;
		Parser_Clear(parser->parser_context);
}

tguParser_t*			TGU_CreateParser(const tguReport_t	*report, tguParserExtern_t *ext)
{
		tguParser_t		*ret;
		AR_ASSERT(report != NULL && ext && ext->build_in != NULL && ext->import_models != NULL && ext->global_constant);

		ret = AR_NEW0(tguParser_t);
		
		ret->report = *report;
		ret->ext = ext;

		ret->match = __build_match();
		ret->parser_context = __build_parser_context((void*)ret);
		TGU_ResetParser(ret);
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
		tok_buf[n] = L'\0';

		Lex_MatchGetCoordinate(parser->match, &line, &col);

		AR_swprintf(buf, 512, L"Invalid Token %ls...(%"AR_PLAT_INT_FMT L"d : %"AR_PLAT_INT_FMT L"d)\r\n", tok_buf, line, col);

		info.type = TGU_REPORT_ERROR_T;
		info.message = buf;

		token.value = 0;
		token.count =	n;	
		token.str =  tok_buf;

		Lex_MatchGetCoordinate(parser->match, &token.line, &token.col);

		info.token = &token;
		parser->report.report_func(&info, parser->report.report_ctx);

		AR_ASSERT(*Lex_GetNextInput(parser->match) != L'\0');

		Lex_SkipTo(parser->match, L";");
		Lex_ClearError(parser->match);
		
		parser->has_error = true;
}




tguParseResult_t				TGU_ParseCode(tguParser_t	*parser, const	wchar_t			*model_name, const wchar_t *code)
{
		tguParseResult_t		ret;
		tguBlock_t		*result = NULL;
		lexToken_t		token;
		psrToken_t		psrtok;
		
		bool_t			is_ok;
		AR_ASSERT(parser != NULL  && model_name &&  AR_wcslen(model_name) > 0 && code != NULL);

		AR_ASSERT(parser->ext->import_models != NULL);
		
		model_name = TGU_AllocString(model_name);

		parser->model_name = model_name;

		Lex_ResetInput(parser->match, code);
		Parser_Clear(parser->parser_context);

		result = TGU_CreateBlock(NULL);
		parser->abs_tree = result;
		parser->top_block = parser->abs_tree;
		
		parser->current_function = NULL;
		parser->has_error = false;
		parser->loop_level = 0;

		AR_ASSERT(TGU_FindSymb(parser->ext->import_models,parser->model_name,  TGU_SYMB_BLOCK_T) != NULL);

		

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

						if(is_ok && token.value == PARSER_EOI_TOKVAL)
						{
								break;
						}
				}

		};

		ret.has_error = false;

		if(!is_ok || parser->has_error)
		{
				ret.has_error = true;
		}

		ret.block = result;

		TGU_ResetParser(parser);
		
		return ret;
}





void					TGU_ParserPushBlock(tguParser_t	*parser)
{
		tguBlock_t		*new_block;
		AR_ASSERT(parser != NULL);
		new_block = TGU_CreateBlock(parser->top_block);
		parser->top_block = new_block;
}

tguBlock_t*			TGU_ParserPopBlock(tguParser_t	*parser)
{
		tguBlock_t		*block;
		AR_ASSERT(parser != NULL && parser->abs_tree != NULL && parser->top_block != parser->abs_tree);
		block = parser->top_block;
		parser->top_block = block->parent;
		return block;
}


AR_NAMESPACE_END


