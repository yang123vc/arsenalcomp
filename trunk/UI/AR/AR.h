#pragma once

#include "Arsenal.h"


AR_NAMESPACE_BEGIN
 
/*
typedef void	(AR_STDCALL *AR_error_func)(int, const wchar_t *msg, void *ctx);
typedef void	(AR_STDCALL *AR_print_func)(const wchar_t *msg, void *ctx);
typedef void	(AR_STDCALL *AR_fatal_func)(const wchar_t *msg, void *ctx);

typedef struct __ar_init_tag
{
		AR_error_func	error_f;
		AR_print_func	print_f;
		AR_fatal_func	fatal_f;
		void			*ctx;
}arInit_t;
*/


class NonCopyable
{
private:
		NonCopyable(const NonCopyable&);
		const NonCopyable& operator=(const NonCopyable&);
protected:
		NonCopyable() { }
		~NonCopyable() { }
}; 

class ARContext : private NonCopyable
{

public:
		virtual void OnError(int level, const wchar_t *msg) = 0;
		virtual void OnFatal(const wchar_t *msg) = 0;
		virtual void OnPrint(const wchar_t *msg) = 0;
		virtual ~ARContext(){}
};




class Arsenal : private NonCopyable
{
private:
		ARContext		*m_ctx;
private:
		Arsenal(ARContext		*ctx);
		~Arsenal();
public:
		static Arsenal* Instance(ARContext *ctx);
		static void		UnInstance();
};


/**************************************************************************************************/

class Lexer : private NonCopyable
{
public:
		typedef struct __last_error_tag
		{
				const wchar_t			*str;
				size_t					line;
				size_t					col;
		}lastError_t;
private:
		lex_t			*m_lex;
		lexMatch_t		*m_match;
		lastError_t		m_error;
		bool			m_is_err;
public:
		Lexer();
		~Lexer();
public:
		bool	SetPattern(const wchar_t *pattern);
		bool	SetInput(const wchar_t *input);
		void	Reset();
		void	Clear();

		bool	IsError()const;
		bool	GetToken(lexToken_t &token);
		const lastError_t& GetLastError()const;
};



class Parser : private NonCopyable
{
public:
		typedef enum 
		{
				SLR = PSR_SLR,
				LR1 = PSR_LR1,
				LALR = PSR_LALR
		}ParserType_t;
private:
		psrGrammar_t	*m_grammar;
		parser_t		*m_parser;
		arString_t		*m_strbuf;
		//printNode_t		*m_result;
public:
		void Clear();
		bool SetGrammar(const wchar_t *gmr, Parser::ParserType_t type = Parser::LALR);
		bool AddToken(const psrToken_t *tok);
		
		const printNode_t *GetResult()const;
public:
		void PrintGrammar()const;
		void PrintFirstFollow()const;
		void ReportConflict()const;
		void PrintActionTable()const;
public:
		Parser();
		~Parser();
};



AR_NAMESPACE_END






