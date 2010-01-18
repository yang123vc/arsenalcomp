#include "StdAfx.h"
#include "PrintNode.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPrintNode::CPrintNode(const CString &name, size_t line, size_t col, size_t cnt) : m_name(name), m_line(line), m_col(col), m_cnt(cnt)
{
}

CPrintNode::~CPrintNode(void)
{
		Clear();
}


void	CPrintNode::Insert(CPrintNode *node)
{
		ASSERT(node != NULL);
		m_nodes.AddTail(node);
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
		
		CPrintNode *node = new CPrintNode(str, tok->line, tok->col, tok->str_cnt);
		return (ARSpace::psrNode_t*)node;
}


ARSpace::psrNode_t*	 AR_STDCALL build_rule(ARSpace::psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		
		ASSERT(nodes != NULL && count > 0 && name != NULL && ctx != NULL);

		
		CPrintNode *node = new CPrintNode(CString(name));

		CPrintNode **ns = (CPrintNode**)nodes;
		
		bool is_assign = false;
		for(size_t i = 0; i < count; ++i)
		{
				if(ns[i] == NULL)
				{
						node->Insert(new CPrintNode(TEXT("%EMPTY")));
				}else
				{
						if(is_assign)
						{
								node->m_line = ns[i]->m_line;
								node->m_col = ns[i]->m_col;
								node->m_cnt = ns[i]->m_cnt;
								is_assign = false;
						}

						node->Insert(ns[i]);
						ns[i] = NULL;
				}
		}

		return (ARSpace::psrNode_t*)node;
}
