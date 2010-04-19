#pragma once




#include "Arsenal.h"
#pragma comment(lib, "Arsenal.lib")



#if defined(AR_SHARED_LIB)
		#if defined(AR_EXPORTS)
				#define ARAPI __declspec(dllexport)
		#else
				#define ARAPI __declspec(dllimport)	
		#endif
#else
		#define ARAPI
#endif




namespace ArsenalCPP{

using namespace ARSpace;

class ARAPI NonCopyable
{
private:
		NonCopyable(const NonCopyable&);
		const NonCopyable& operator=(const NonCopyable&);
protected:
		NonCopyable() { }
		~NonCopyable() { }
}; 



class ARAPI ARContext : private NonCopyable
{
		bool	m_is_enable;
public:
		virtual void Enable()
		{
				m_is_enable = true;
		}

		virtual void Disable()
		{
				m_is_enable = false;
		}

		virtual bool IsEnable()
		{
				return m_is_enable;
		}
public:
		virtual void OnError(int_t level, const wchar_t *msg) = 0;	
		virtual void OnPrint(const wchar_t *msg) = 0;

		ARContext() : m_is_enable(true) { }
		virtual ~ARContext() = 0{}
};





class ARAPI Arsenal : private NonCopyable
{
private:
		ARContext		*m_ctx;
private:
		Arsenal(ARContext		*ctx);
		~Arsenal();
public:
		ARContext*	Context()const;
public:
		static Arsenal* Instance(ARContext *ctx);
		static void		UnInstance();
		static Arsenal*	Get();
};




/*******************************PreDefined**********************************/

class ARAPI DummyContext : public ARContext
{
public:
		virtual void OnError(int_t level, const wchar_t *msg)
		{

		}

		virtual void OnPrint(const wchar_t *msg)
		{

		}
public:
		DummyContext()
		{

		}

		virtual ~DummyContext()
		{

		}
};


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
		ARContext*		IOContext();
		void ResetContext(ARContext *ctx);
public:
		bool	Insert(const wchar_t *name, const wchar_t *expr);
		bool	Insert(const wchar_t *expr, size_t lex_val, size_t prec = 0, bool is_skip = false);
		bool	Remove(const wchar_t *name);
		bool	Remove(size_t lex_val);
		bool	Generate();
public:
		void	SetInput(const wchar_t *input);
		void	SetFlags(uint_t flags, bool_t is_on = true);
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
		bool	Insert(const wchar_t *name, size_t term_val, psrAssocType_t assoc = PSR_ASSOC_NONASSOC, size_t prec = 0, psrTermFunc_t	leaf_f = NULL);
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
		bool	SetFirstRule(const wchar_t *name);
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

		

		const psrStatusView_t*		CreateStatusView()const;
		void							DestroyStatusView(const psrStatusView_t *view);
};



}


