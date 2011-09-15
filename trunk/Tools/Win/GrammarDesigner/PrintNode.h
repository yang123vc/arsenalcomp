#pragma once


class CPrintNode
{
private:
		
public:
		CString					m_name;
		size_t					m_line;
		size_t					m_col;
		size_t					m_cnt;
		bool					m_is_term;
		CList<CPrintNode*>		m_nodes;
public:
		CPrintNode(const CString &name, size_t line = 0, size_t col = 0, size_t cnt = 0, bool is_term = false);
		virtual ~CPrintNode(void);
public:
		void	Insert(CPrintNode *node);
		void Clear();
};


ARSpace::psrNode_t*	 AR_STDCALL build_leaf(const ARSpace::psrToken_t *tok, void *ctx);
ARSpace::psrNode_t*	 AR_STDCALL build_rule(ARSpace::psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);





 