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


static void	AR_STDCALL tgu_free(psrNode_t *node, void *ctx)
{
		AR_ASSERT(false);
}


static void		AR_STDCALL tgu_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
{
		AR_ASSERT(false);
}






static psrHandler_t		__def_handler_ctx = 
{
		tgu_error,
		tgu_free,
};


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
		match = Lex_CreateMatch(__g_lex, NULL);
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

/*********************************************Block******************************************************/

tguBlock_t*		TGU_CreateBlock(tguBlock_t	*parent)
{
		return NULL;
}


void			TGU_DestroyBlock(tguBlock_t	*block)
{
		AR_ASSERT(block != NULL);
}


AR_NAMESPACE_END


