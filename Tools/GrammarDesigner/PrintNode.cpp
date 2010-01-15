#include "StdAfx.h"
#include "PrintNode.h"

CPrintNode::CPrintNode(const CString &name) : m_name(name)
{
}

CPrintNode::~CPrintNode(void)
{
		Clear();
}


void	CPrintNode::Insert(CPrintNode *node)
{
		ASSERT(node != NULL);

}
		
void CPrintNode::Clear()
{
		POSITION pos = m_nodes.GetHeadPosition();
		
		while(pos != NULL)
		{
				CPrintNode *node =  m_nodes.GetNext(pos);
				delete node;
		}

		m_nodes.RemoveAll();

}

ARSpace::psrNode_t*		AR_STDCALL build_leaf(const ARSpace::psrToken_t *tok, void *ctx)
{
		ASSERT(tok != NULL && ctx != NULL);
		CString str;

		if(tok->term_val == 0)
		{
				str = TEXT("EOI");
		}else
		{
				ASSERT(tok->str_cnt > 0);
				str.Append(tok->str, tok->str_cnt);
		}
		
		CPrintNode *node = new CPrintNode(str);
		return (ARSpace::psrNode_t*)node;
}


ARSpace::psrNode_t*	 AR_STDCALL build_rule(ARSpace::psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		
		ASSERT(nodes != NULL && count > 0 && name != NULL && ctx != NULL);

		
		CPrintNode *node = new CPrintNode(CString(name));

		CPrintNode **ns = (CPrintNode**)nodes;

		for(size_t i = 0; i < count; ++i)
		{
				if(ns[i] == NULL)
				{
						node->Insert(new CPrintNode(TEXT("%EMPTY")));
				}else
				{
						node->Insert(ns[i]);
						ns[i] = NULL;
				}
		}

		return (ARSpace::psrNode_t*)node;
}
