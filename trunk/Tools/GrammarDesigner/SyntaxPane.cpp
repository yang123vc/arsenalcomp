// SyntaxPane.cpp : implementation file
//

#include "stdafx.h"

#include "GrammarDesigner.h"

#include "SyntaxPane.h"


#include "MainFrm.h"

// CSyntaxPane

IMPLEMENT_DYNAMIC(CSyntaxPane, CDockablePane)

CSyntaxPane::CSyntaxPane()
{

}

CSyntaxPane::~CSyntaxPane()
{
}


void CSyntaxPane::Clear()
{
		m_tree.Clear();
}


BEGIN_MESSAGE_MAP(CSyntaxPane, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
END_MESSAGE_MAP()



// CSyntaxPane message handlers
int CSyntaxPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here
		CRect rectDummy;

		rectDummy.SetRectEmpty();

		const DWORD dwStyle = TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES  | TVS_DISABLEDRAGDROP  | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

		if (!this->m_tree.Create(dwStyle, rectDummy, this, 1))
		{
				TRACE0("Failed to create tagview tree window\n");
				return -1;      // fail to create
		}
		
		

		return 0;
}


void CSyntaxPane::OnSize(UINT nType, int cx, int cy)
{
		CDockablePane::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here
		m_tree.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}


void	CSyntaxPane::DrawTree(CPrintNode *node)
{
		m_tree.DrawTree(node);

		

}

/**********************************SyntaxTree*************************************************/
IMPLEMENT_DYNAMIC(CSyntaxTree, CTreeCtrl)

CSyntaxTree::CSyntaxTree()
{
		m_font.CreatePointFont(100, TEXT("Consolas"));
		m_root = NULL;
}

CSyntaxTree::~CSyntaxTree()
{
		
}

void	CSyntaxTree::Clear()
{
		if(m_root != NULL)
		{
				CPrintNode* root = (CPrintNode*)this->GetItemData(m_root);
				this->DeleteAllItems();
				delete root;
				m_root = NULL;
		}
}

BEGIN_MESSAGE_MAP(CSyntaxTree, CTreeCtrl)
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_LBUTTONDBLCLK()
ON_WM_CONTEXTMENU()
ON_NOTIFY_REFLECT(NM_RCLICK, &CSyntaxTree::OnNMRClick)
ON_COMMAND(ID_POPUP_FONT32864, &CSyntaxTree::OnPopupFont32864)
ON_COMMAND(ID_POPUP_CLEAR32865, &CSyntaxTree::OnPopupClear32865)
ON_UPDATE_COMMAND_UI(ID_POPUP_CLEAR32865, &CSyntaxTree::OnUpdatePopupClear32865)
END_MESSAGE_MAP()


int CSyntaxTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here
		
		this->SetFont(&m_font);
		return 0;
}


HTREEITEM CSyntaxTree::InsertText(LPCTSTR lpszItem, LPARAM lParam, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
		return this->InsertItem(TVIF_TEXT|TVIF_PARAM, lpszItem ,0,0,0,0, lParam, hParent,hInsertAfter);
}


HTREEITEM	CSyntaxTree::insert_node(CPrintNode *node, HTREEITEM parent)
{
		HTREEITEM hdl = InsertText(node->m_name, (LPARAM)node, parent);

		POSITION pos = node->m_nodes.GetHeadPosition();

		while(pos != NULL)
		{
				CPrintNode *chd = node->m_nodes.GetNext(pos);
				insert_node(chd, hdl);
		}

		return hdl;
}

void	CSyntaxTree::DrawTree(CPrintNode *node)
{
		Clear();
		ASSERT(node != NULL);
		m_root = insert_node(node, TVI_ROOT);
}

void CSyntaxTree::OnDestroy()
{
		// TODO: Add your message handler code here
		Clear();

		CTreeCtrl::OnDestroy();
		
}

void CSyntaxTree::OnLButtonDblClk(UINT nFlags, CPoint point)
{
		// TODO: Add your message handler code here and/or call default
		HTREEITEM curr = this->GetSelectedItem();

		if(curr != NULL)
		{
				const CPrintNode *node = (const CPrintNode*)this->GetItemData(curr);
				ASSERT(node != NULL);

				CMainFrame *main_frm = (CMainFrame*)::AfxGetMainWnd();
				
				CInputPane &input = main_frm->GetInputPane();

				input.SendMessage(ID_EDIT_LOCATE_POS, (WPARAM)node->m_line);

				

				
		}else
		{

		}
		

		CTreeCtrl::OnLButtonDblClk(nFlags, point);
		CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

void CSyntaxTree::OnContextMenu(CWnd* pWnd, CPoint point)
{
		// TODO: Add your message handler code here

		CMenu menu;
		menu.LoadMenu(IDR_SYNTAXTREE_MENU);

		CMenu* pSumMenu = menu.GetSubMenu(0);

		((CWinAppEx*)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(pSumMenu->GetSafeHmenu(),point.x, point.y, this, TRUE);

		SetFocus();

}

void CSyntaxTree::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
		// TODO: Add your control notification handler code here
		*pResult = 0;
		
		CPoint pt;
		GetCursorPos(&pt);
		OnContextMenu(this, pt);
}

void CSyntaxTree::OnPopupFont32864()
{
		// TODO: Add your command handler code here
		LOGFONT lft;
		m_font.GetLogFont(&lft);
		CFontDialog		dlg(&lft);
		
		if(dlg.DoModal() == IDOK)
		{
				LOGFONT lf; 
				dlg.GetCurrentFont(&lf);
				m_font.Detach();
				VERIFY(m_font.CreateFontIndirect(&lf));
				this->SetFont(&m_font);
		}
}

void CSyntaxTree::OnPopupClear32865()
{
		// TODO: Add your command handler code here

		this->Clear();
}

void CSyntaxTree::OnUpdatePopupClear32865(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here
		pCmdUI->Enable(m_root != NULL);
}
