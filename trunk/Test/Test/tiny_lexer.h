#pragma once

#include "tiny.h"
#include <cassert>

class Lexer
{
private:
		AR_lex_t		*m_lex;
		AR_match_t		*m_match;
public:
		Lexer()
		{
				m_lex = AR_LEX_Create();
				m_match = NULL;
		}

		~Lexer()
		{
				AR_LEX_Destroy(m_lex);
				
				if(m_match)
				{
						AR_LEX_DestroyMatch(m_match);
				}
		}
public:
		std::wstring Config(const std::wstring &pattern)
		{
				const AR_wchar_t *result = AR_LEX_Config(m_lex, (const AR_wchar_t*)pattern.c_str());
				if(result == NULL)
				{
						throw TinyException(L"Invalid Lex Pattern\r\n");
				}else
				{
						return std::wstring((const wchar_t*)result);
				}
		}

		void	SetInput(const std::wstring &input)
		{
				if(m_match != NULL)
				{
						AR_LEX_DestroyMatch(m_match);
				}
				m_match = AR_LEX_CreateMatch((const AR_wchar_t*)input.c_str());
		}

		bool Match(AR_Token_t *tok)
		{
				assert(tok != NULL && m_match != NULL);
				return AR_LEX_Match(m_lex, m_match, tok) == AR_True;
		}
};




