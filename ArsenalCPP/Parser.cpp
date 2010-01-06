#include "Parser.h"


namespace ArsenalCPP{


/*********************************************************Grammar*********************************************/


static void		AR_STDCALL __free_func(psrNode_t *node, void *ctx)
{
		NodeContext		*context;
		AR_ASSERT( node != NULL && ctx != NULL);

		context = (NodeContext*)ctx;
		context->Free((Node*)node);
}


static void		AR_STDCALL __error_func(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)
{
		NodeContext		*context;
		AR_ASSERT(tok != NULL && expected != NULL && count > 0 && ctx != NULL);

		context = (NodeContext*)ctx;
		context->Error(tok, expected, count);
}



static void AR_STDCALL __io_error_func(int_t level, const wchar_t *msg, void *ctx)
{
		ARContext		*context = (ARContext*)ctx;
		
		AR_ASSERT(context != NULL);
		context->OnError(level, msg);
}


static void	AR_STDCALL __io_print_func(const wchar_t *msg, void *ctx)
{
		ARContext		*context = (ARContext*)ctx;
		AR_ASSERT(context != NULL);
		context->OnPrint(msg);
}

Grammar::Grammar(NodeContext *psr_ctx, ARContext	*io_ctx): m_psr_ctx(psr_ctx), m_io_ctx(io_ctx)
{
		AR_ASSERT(m_psr_ctx != NULL && m_io_ctx != NULL);

		psrCtx_t		ar_psr;
		arIOCtx_t		ar_io;

		ar_io.on_print	= __io_print_func;
		ar_io.on_error	= __io_error_func;
		ar_io.ctx = (void*)m_io_ctx;
		
		ar_psr.ctx = (void*)m_psr_ctx;
		ar_psr.error_f = __error_func;
		ar_psr.free_f = __free_func;

		
		m_grammar = PSR_CreateGrammar(&ar_psr, &ar_io);

		AR_ASSERT(m_grammar != NULL);
		
}


Grammar::~Grammar()
{
		
		
		PSR_DestroyGrammar(m_grammar);
		delete m_psr_ctx;
		delete m_io_ctx;
}

bool	Grammar::Insert(const wchar_t *name, size_t term_val, psrAssocType_t assoc, size_t prec, psrTermFunc_t	leaf_f)
{
		AR_ASSERT(name != NULL && leaf_f != NULL);
		return PSR_InsertTerm(m_grammar, name, term_val, assoc, prec, leaf_f);
}

bool	Grammar::Insert(const wchar_t *rule, const wchar_t *prec_tok, psrRuleFunc_t rule_f, size_t auto_ret)
{
		AR_ASSERT(rule != NULL);
		return PSR_InsertRuleByStr(m_grammar, rule, prec_tok, rule_f, auto_ret);
}



bool	Grammar::IsValid()const
{
		return PSR_CheckIsValidGrammar(m_grammar);
}

void	Grammar::Clear()
{
		PSR_ClearGrammar(m_grammar);
}

void	Grammar::PrintFirstSet()const
{
		psrSymbMap_t first;
		
		PSR_InitSymbMap(&first);

		PSR_CalcFirstSet(m_grammar, &first);

		arString_t	*str;

		str = AR_CreateString();
		
		AR_ASSERT(str != NULL);
		PSR_PrintSymbolMap(&first, str);

		m_io_ctx->OnPrint(AR_GetStrString(str));
		
		AR_DestroyString(str);
		PSR_UnInitSymbMap(&first);
}


void	Grammar::PrintFollowSet()const
{

		psrSymbMap_t first, follow;
		
		PSR_InitSymbMap(&first);
		PSR_InitSymbMap(&follow);
		
		PSR_CalcFirstSet(m_grammar, &first);
		PSR_CalcFollowSet(m_grammar, &follow, &first);

		arString_t	*str = AR_CreateString();
		AR_ASSERT(str != NULL);
		PSR_PrintSymbolMap(&follow, str);
		
		m_io_ctx->OnPrint(AR_GetStrString(str));
		
		AR_DestroyString(str);
		PSR_UnInitSymbMap(&first);
		PSR_UnInitSymbMap(&follow);

}

bool	Grammar::PrintLeftRecursion()const
{
		arString_t *str;
		bool res;
		str = AR_CreateString();

		res = PSR_ReportLeftRecursion(m_grammar, str);

		if(res)
		{
				m_io_ctx->OnPrint(AR_GetStrString(str));
		}
		AR_DestroyString(str);

		return res;
}

bool	Grammar::IsLeftRecursion()const
{
		return PSR_ReportLeftRecursion(m_grammar, NULL);

}

ARContext*		Grammar::IOContext()
{
		return m_io_ctx;
		
}
/***************************************************************Parser****************************************************/


Parser::Parser(Grammar *grammar, psrModeType_t mode)
{
		AR_ASSERT(grammar != NULL && grammar->IsValid());
		m_grammar = grammar;
		m_parser = PSR_CreateParser(grammar->m_grammar, mode);
		AR_ASSERT(m_parser != NULL);
}


Parser::~Parser()
{
		AR_ASSERT(m_grammar != NULL && m_parser);
		PSR_DestroyParser(m_parser);
		delete m_grammar;
}


void	Parser::Clear()
{
		AR_ASSERT(m_grammar != NULL && m_parser != NULL);
		PSR_Clear(m_parser);
}

bool	Parser::AddToken(const psrToken_t &tok)
{
		AR_ASSERT(m_grammar != NULL && m_parser != NULL);

		return PSR_AddToken(m_parser, &tok);
}

bool	Parser::Accepted()const
{
		AR_ASSERT(m_grammar != NULL && m_parser != NULL);
		return PSR_IsAccepted(m_parser);
}

bool	Parser::InError()const
{
		AR_ASSERT(m_grammar != NULL && m_parser != NULL);
		
		return PSR_IsInError(m_parser);
}

void	Parser::ClearError()
{
		AR_ASSERT(m_grammar != NULL && m_parser != NULL);
		PSR_ClearError(m_parser);
}

Node*	Parser::GetResult()
{
		psrNode_t *node;
		AR_ASSERT(m_grammar != NULL && m_parser != NULL);
		node = PSR_GetResult(m_parser);
		return (Node*)node;
}


void	Parser::PrintParseTable(size_t width)const
{
		AR_ASSERT(m_grammar != NULL && m_parser != NULL);

		arString_t *str = AR_CreateString();

		PSR_PrintParserActionTable(m_parser, str, width);
		
		m_grammar->IOContext()->OnPrint(AR_GetStrString(str));
		AR_DestroyString(str);
}

void	Parser::PrintConflict()const
{
		AR_ASSERT(m_grammar != NULL && m_parser != NULL);

		arString_t *str = AR_CreateString();

		PSR_PrintParserConflict(m_parser, str);
		
		m_grammar->IOContext()->OnPrint(AR_GetStrString(str));
		AR_DestroyString(str);
}

size_t	Parser::CountConflict()const
{
		AR_ASSERT(m_grammar != NULL && m_parser != NULL);
		
		return PSR_CountParserConflict(m_parser);
}


const psrActionView_t*	Parser::CreateActionView()const
{

		return PSR_CreateParserActionView(m_parser);
}

void	Parser::DestroyActionView(const psrActionView_t *view)
{
		PSR_DestroyParserActionView(view);

}



}

