#pragma once


class CPrintNode :	public ArsenalCPP::Node
{
public:
		CString					m_name;
		CList<CPrintNode*>		m_nodes;
public:
		CPrintNode(const CString &name);
		virtual ~CPrintNode(void);
public:
		void	Insert(CPrintNode *node);
		void Clear();
};


ARSpace::psrNode_t*		AR_STDCALL build_leaf(const ARSpace::psrToken_t *tok, void *ctx);
ARSpace::psrNode_t*	 AR_STDCALL build_rule(ARSpace::psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);



 