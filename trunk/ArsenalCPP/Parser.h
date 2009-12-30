#pragma once

#include "Global.h"

namespace ArsenalCPP{


class Node
{
public:
		Node(){ }
		virtual ~Node() = 0;
};



class NodeContext : NonCopyable
{
private:
		ARContext		*m_io_context;
public:
		virtual void	Free(Node *node) = 0;
		virtual void	Error(const psrToken_t *tok, const wchar_t *expected[], size_t count) = 0;
public:
		NodeContext(ARContext *io_ctx) : m_io_context(io_ctx)
		{
				if(m_io_context == NULL)
				{
						m_io_context = Arsenal::Get()->Context();
				}
		}

		virtual ~NodeContext() = 0;
public:
		ARContext* Context()
		{
				return m_io_context;
		}
};


class Parser;

class Grammar : NonCopyable
{
private:
		psrGrammar_t	*m_grammar;
		NodeContext		*m_context;
private:
		friend class Parser;
public:
		Grammar(NodeContext *context);
		~Grammar();
public:
		ARContext* IOContext();
public:
		bool	Insert(const wchar_t *name, size_t term_val, psrAssocType_t assoc = PSR_ASSOC_NOASSOC, size_t prec = 0, psrTermFunc_t	leaf_f = NULL);
		bool	Insert(const wchar_t *rule, const wchar_t *prec_tok = NULL, psrRuleFunc_t rule_f = NULL);
public:
		bool	IsValid()const;
		void	Clear();
		void	PrintFirstSet()const;
		void	PrintFollowSet()const;
		bool	PrintLeftRecursion()const;
		bool	IsLeftRecursion()const;
};




class Parser : NonCopyable
{
private:
		Grammar			*m_grammar;
		parser_t		*m_parser;
public:
		Parser(Grammar *grammar, psrModeType_t mode = PSR_LALR);
		~Parser();
public:
		void	Clear();
		void	ClearError();

		bool	AddToken(const psrToken_t &tok);
		bool	Accepted()const;
		bool	InError()const;
		Node*	GetResult();

public:
		void	PrintParseTable(size_t width = 20)const;
		void	PrintConflict()const;
		size_t	CountConflict()const;
};



}

