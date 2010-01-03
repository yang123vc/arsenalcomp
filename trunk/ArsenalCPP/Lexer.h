#pragma once

#include "Global.h"

namespace ArsenalCPP{



/**************************************************************************************************/

class ARAPI Lexer : private NonCopyable
{
public:
		typedef struct __last_error_tag
		{
				const wchar_t			*str;
				size_t					line;
				size_t					col;
		}lastError_t;
private:
		ARContext		*m_ctx;
private:

		lex_t			*m_lex;
		lexMatch_t		*m_match;
		lastError_t		m_error;
		
public:
		Lexer(ARContext *ctx);
		~Lexer();
public:
		bool	Insert(const wchar_t *name, const wchar_t *expr);
		bool	Insert(const wchar_t *expr, size_t lex_val, size_t prec = 0, bool is_skip = false);
		bool	Remove(const wchar_t *name);
		bool	Remove(size_t lex_val);
		bool	Generate();
public:
		void	SetInput(const wchar_t *input);
		void	Reset();
		void	Clear();
public:
		void	ClearError();
		void	Skip();
		bool	IsError()const;
		bool	GetToken(lexToken_t &token);
public:
		const lastError_t& GetLastError()const;
};

}
