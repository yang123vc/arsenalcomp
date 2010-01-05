//#include "stdafx.h"

#include "Lexer.h"


namespace ArsenalCPP{


static void AR_STDCALL __error_func(int_t level, const wchar_t *msg, void *ctx)
{
		ARContext		*context = (ARContext*)ctx;
		
		AR_ASSERT(context != NULL);
		context->OnError(level, msg);
}


static void	AR_STDCALL __print_func(const wchar_t *msg, void *ctx)
{
		ARContext		*context = (ARContext*)ctx;
		AR_ASSERT(context != NULL);
		context->OnPrint(msg);
}



Lexer::Lexer(ARContext *ctx) : m_ctx(ctx)
{
		if(m_ctx == NULL)
		{
				m_ctx = Arsenal::Get()->Context();
		}

		arIOCtx_t		io;

		io.ctx = (void*)m_ctx;
		io.on_error = __error_func;
		io.on_print = __print_func;

		m_lex = LEX_Create(&io);

		m_match = new lexMatch_t;


		LEX_InitMatch(m_match, L"");
		m_error.str = NULL;
		m_error.line = 0;
		m_error.col = 0;
}


Lexer::~Lexer()
{
		AR_ASSERT(m_lex != NULL && m_match != NULL);
		LEX_UnInitMatch(m_match);
		delete m_match;
		LEX_Destroy(m_lex);
		delete m_ctx;
}

bool	Lexer::Generate()
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);

		return LEX_GenerateTransTable(m_lex);

}

bool	Lexer::Insert(const wchar_t *name, const wchar_t *expr)
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		AR_ASSERT(name != NULL && expr != NULL);
		return LEX_InsertName(m_lex, name, expr);
}

bool	Lexer::Insert(const wchar_t *expr, size_t lex_val, size_t prec, bool is_skip)
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		AR_ASSERT(expr != NULL);
		lexAction_t act;
		act.value = lex_val;
		act.is_skip = is_skip;
		act.priority = prec;
		return LEX_InsertRule(m_lex, expr, &act);
}

bool	Lexer::Remove(const wchar_t *name)
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		AR_ASSERT(name != NULL);
		return LEX_RemoveByName(m_lex, name);
}

bool	Lexer::Remove(size_t lex_val)
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		return LEX_RemoveByValue(m_lex, lex_val);
}


void Lexer::Clear()
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		
		LEX_ResetMatch(m_match);

		m_error.str = NULL;
		m_error.line = 0;
		m_error.col = 0;
}


void	Lexer::SetInput(const wchar_t *input)
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		Clear();
		LEX_ResetInput(m_match, input);
}


void	Lexer::Reset()
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);

		m_error.str = NULL;
		m_error.line = 0;
		m_error.col = 0;
		LEX_ResetMatch(m_match);
}

void	Lexer::ClearError()
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		LEX_ClearError(m_match);
}

void	Lexer::Skip()
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		LEX_Skip(m_match);
}

bool	Lexer::IsError()const
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		return LEX_IsError(m_match);
}



const Lexer::lastError_t& Lexer::GetLastError()const
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		return m_error;

}



bool	Lexer::GetToken(lexToken_t &token)
{
		AR_ASSERT(m_lex != NULL && m_ctx != NULL && m_match != NULL);
		AR_ASSERT(!IsError());

		LEX_Match(m_lex, m_match, &token);
		
		if(IsError())
		{
				m_error.str = m_match->next;
				m_error.col = m_match->col;
				m_error.line = m_match->line;
		}
		
		return !IsError();
}




}

