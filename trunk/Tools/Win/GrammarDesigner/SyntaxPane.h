#pragma once
#include "PrintNode.h"

class CSyntaxTree		: public CTreeCtrl
{
private:
		DECLARE_DYNAMIC(CSyntaxTree)
private:
		CFont			m_font;
		HTREEITEM		m_root;
public:
		CSyntaxTree();
		virtual ~CSyntaxTree();
public:
		void	Clear();
		void	DrawTree(CPrintNode *node);

		void	ExpandAll(DWORD action = TVE_EXPAND);
		void	ExpandItem(HTREEITEM	item, DWORD action = TVE_EXPAND);
		void	ExpandLevel(size_t level);
protected:
		HTREEITEM InsertText(LPCTSTR lpszItem, LPARAM lParam, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);

		HTREEITEM	insert_node(CPrintNode *node, HTREEITEM parent);
public:
		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnDestroy();
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
		afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnPopupFont32864();
		afx_msg void OnPopupClear32865();
		afx_msg void OnUpdatePopupClear32865(CCmdUI *pCmdUI);
		afx_msg void OnSyntaxMenuExpand();
		afx_msg void OnUpdateSyntaxMenuExpand(CCmdUI *pCmdUI);
		afx_msg void OnPopupCollapse();
		afx_msg void OnUpdatePopupCollapse(CCmdUI *pCmdUI);
};

// CSyntaxPane

class CSyntaxPane : public CDockablePane
{
private:
		DECLARE_DYNAMIC(CSyntaxPane)
private:
		CSyntaxTree		m_tree;
public:
		CSyntaxPane();
		virtual ~CSyntaxPane();

public:
		void	Clear();
		void	DrawTree(CPrintNode *node);
protected:
		DECLARE_MESSAGE_MAP()

public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
};


