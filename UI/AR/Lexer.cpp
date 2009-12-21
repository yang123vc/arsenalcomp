//#include "stdafx.h"

#include "AR.h"

AR_NAMESPACE_BEGIN

#if(0)

Lexer::Lexer()
{
		m_lex =  LEX_Create();
		m_match = NULL;
		m_is_err = false;
		m_error.str = NULL;
		m_error.line = 0;
		m_error.col = 0;

}

Lexer::~Lexer()
{
		AR_ASSERT(m_lex != NULL);
		LEX_Destroy(m_lex);
		
}

void Lexer::Clear()
{
		AR_ASSERT(m_lex != NULL);
		
		m_is_err = false;
		m_error.str = NULL;
		m_error.line = 0;
		m_error.col = 0;

		if(m_match != NULL)
		{
				LEX_UnInitMatch(m_match);
				delete m_match;
				m_match = NULL;
		}
}


bool	Lexer::SetPattern(const wchar_t *pattern)
{
		AR_ASSERT(m_lex != NULL && pattern != NULL);
		
		LEX_Clear(m_lex);
		return LEX_Config(m_lex, pattern) != NULL;
}

bool	Lexer::SetInput(const wchar_t *input)
{
		AR_ASSERT(m_lex != NULL && input != NULL);

		Clear();
		m_match = new lexMatch_t;
		LEX_InitMatch(m_match, input);
		return true;
}


void	Lexer::Reset()
{
		AR_ASSERT(m_match != NULL);
		if(m_match)
		{
				m_is_err = false;
				m_error.str = NULL;
				m_error.line = 0;
				m_error.col = 0;
				LEX_ResetMatch(m_match);

		}
}

bool	Lexer::IsError()const
{
		return m_is_err;
}

bool	Lexer::GetToken(lexToken_t &token)
{
		AR_ASSERT(m_lex != NULL && m_match != NULL);
		AR_ASSERT(!m_is_err);

		if(!LEX_Match(m_lex, m_match, &token))m_is_err = true;
		
		if(m_is_err)
		{
				m_error.str = m_match->next;
				m_error.col = m_match->col;
				m_error.line = m_match->line;
		}
		return !m_is_err;
}

const Lexer::lastError_t& Lexer::GetLastError()const
{
		AR_ASSERT(m_is_err);
		return m_error;

}

#endif


AR_NAMESPACE_END



