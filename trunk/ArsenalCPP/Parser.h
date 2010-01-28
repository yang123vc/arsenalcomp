#pragma once

#include "Global.h"

namespace ArsenalCPP{


class ARAPI Node
{
public:
		Node(){ }

		virtual ~Node() = 0
		{

		}
};



class ARAPI NodeContext : NonCopyable
{
public:
		virtual void	Free(Node *node) = 0;
		virtual void	Error(const psrToken_t *tok, const wchar_t *expected[], size_t count) = 0;
public:
		virtual ~NodeContext() = 0
		{

		}
};


class ARAPI DummyNodeContext : public NodeContext
{
public:
		virtual void	Free(Node *node)
		{
				delete node;
		}
		
		virtual void	Error(const psrToken_t *tok, const wchar_t *expected[], size_t count)
		{

		}
public:
		DummyNodeContext()
		{

		}

		virtual ~DummyNodeContext()
		{

		}
};





class ARAPI Parser;

class ARAPI Grammar : NonCopyable
{
public:
		psrGrammar_t	*m_grammar;
		NodeContext		*m_psr_ctx;
		ARContext		*m_io_ctx;
private:
		friend class Parser;
public:
		Grammar(NodeContext *psr_ctx, ARContext		*io_ctx);
		~Grammar();

		void	ResetIOContext(ARContext		*io_ctx);
		void	ResetParseContext(NodeContext *psr_ctx);
public:
		bool	Insert(const wchar_t *name, size_t term_val, psrAssocType_t assoc = PSR_ASSOC_NOASSOC, size_t prec = 0, psrTermFunc_t	leaf_f = NULL);
		bool	Insert(const wchar_t *rule, const wchar_t *prec_tok = NULL, psrRuleFunc_t rule_f = NULL, size_t auto_ret = 0);

public:
		psrTermInfo_t*	GetTermInfo(const wchar_t *name);
		psrTermInfo_t*	GetTermInfo(size_t term_val);
public:
		bool	IsValid()const;
		void	Clear();
		void	PrintFirstSet()const;
		void	PrintFollowSet()const;
		bool	PrintLeftRecursion()const;
		bool	IsLeftRecursion()const;
public:
		ARContext*		IOContext();

};




class ARAPI Parser : NonCopyable
{
private:
		Grammar			*m_grammar;
		parser_t		*m_parser;
public:
		Parser(Grammar *grammar, psrModeType_t mode = PSR_SLR);
		~Parser();
public:
		void	Clear();
		void	ClearError();

		bool	AddToken(const psrToken_t &tok);
		bool	AddToken(const lexToken_t &tok);
		bool	Accepted()const;
		bool	InError()const;
		Node*	GetResult();

public:
		void	PrintParseTable(size_t width = 20)const;
		void	PrintConflict()const;
		size_t	CountConflict()const;

public:
		void	ResetIOContext(ARContext		*io_ctx);
		void	ResetParseContext(NodeContext *psr_ctx);

public:
		const psrActionView_t*			CreateActionView()const;
		void							DestroyActionView(const psrActionView_t *view);
		
		const psrConflictView_t*		CreateConflictView()const;
		void							DestroyConflictView(const psrConflictView_t *view);

		const psrFirstFollowView_t*		CreateFirstFollowView()const;
		void							DestroyFirstFollowView(const psrFirstFollowView_t *view);
};



}

