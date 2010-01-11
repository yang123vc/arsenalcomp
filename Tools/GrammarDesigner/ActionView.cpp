// ActionView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Mainfrm.h"
#include "ActionView.h"
#include "GrammarDesigner.h"


BEGIN_MESSAGE_MAP(CActionCtrl, CListCtrl)

		ON_WM_CREATE()
		ON_WM_CONTEXTMENU()
		ON_COMMAND(ID_OUTPUTWND_FONT, &CActionCtrl::OnActionViewFont)
		ON_WM_RBUTTONDOWN()
		ON_COMMAND(ID_EDIT_COPY, &CActionCtrl::OnEditCopy)
		ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CActionCtrl::OnUpdateOutputwndFont)
		ON_COMMAND(ID_EDIT_CLEAR, &CActionCtrl::OnEditClear)
//		ON_COMMAND(ID_VIEW_OUTPUTWND, &CActionCtrl::OnViewAction)
END_MESSAGE_MAP()

CActionCtrl::CActionCtrl()
{
		m_font.CreatePointFont(90, TEXT("Consolas"));
}

CActionCtrl::~CActionCtrl()
{


}


int CActionCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CListCtrl::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here
		
		this->SetFont(&m_font);
		return 0;
}




void CActionCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
		// TODO: Add your message handler code here

		CMenu menu;
		menu.LoadMenu(IDR_OUTPUT_POPUP);

		CMenu* pSumMenu = menu.GetSubMenu(0);
	
		((CWinAppEx*)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(pSumMenu->GetSafeHmenu(),point.x, point.y, this, TRUE);

		SetFocus();
}



//void CActionCtrl::OnViewAction()
//{
//		// TODO: Add your command handler code here
//		CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner()->GetOwner());
//	CMainFrame *pMainFrame =  DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
//	
//	if(pParentBar && pMainFrame)
//	{
//			pMainFrame->SetFocus();
//			pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
//			pMainFrame->RecalcLayout();
//	}
//}


void CActionCtrl::OnActionViewFont()
{
		// TODO: Add your command handler code here

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

static int GetSelectIndex(CListCtrl &box, CPoint pt)
{

		for(int i = 0; i < box.GetItemCount(); ++i)
		{
				CRect rect;
				if(box.GetItemRect(i, &rect, LVIR_BOUNDS) && rect.PtInRect(pt))
				{
						return i;
				}
		}
		return -1;
}



void CActionCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
		// TODO: Add your message handler code here and/or call default
		
		int idx = GetSelectIndex(*this, point);
		
		if(idx != -1)this->SetSelectionMark(idx);

		CListCtrl::OnRButtonDown(nFlags, point);
}


void CActionCtrl::OnEditCopy()
{
		// TODO: Add your command handler code here

		int idx = this->GetSelectionMark();

		if(idx >= 0)
		{
				CString src;
				int ncol = this->GetHeaderCtrl()->GetItemCount();

				for(int i = 0; i < ncol; ++i)
				{
						src.Append(GetItemText(idx, i));
						src.Append(TEXT("\t"));
				}
				
				if (OpenClipboard())
				{
						EmptyClipboard();                        
						HGLOBAL hClip =GlobalAlloc(GMEM_MOVEABLE,(src.GetLength() + 1) * sizeof(TCHAR));  
						TCHAR *buf = (TCHAR*)GlobalLock(hClip); 
						lstrcpy(buf, src.GetString());
						GlobalUnlock(hClip);     
						SetClipboardData(CF_UNICODETEXT,hClip); 
						CloseClipboard();            
				}
		}
}



void CActionCtrl::OnUpdateOutputwndFont(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here

		pCmdUI->Enable(this->GetSelectionMark() >= 0);

}


void CActionCtrl::OnEditClear()
{
		// TODO: Add your command handler code here
		
		this->DeleteAllItems();
		
		int ncol = this->GetHeaderCtrl()->GetItemCount();
		for (int i=0; i < ncol; i++)
		{
				this->DeleteColumn(0);
		}

}

/****************************************************************************/

// CActionView

IMPLEMENT_DYNAMIC(CActionView, CDockablePane)

CActionView::CActionView()
{

}

CActionView::~CActionView()
{
}


BEGIN_MESSAGE_MAP(CActionView, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
END_MESSAGE_MAP()



// CActionView message handlers



int CActionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here
		CRect rectDummy;
		rectDummy.SetRectEmpty();
		
		const DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL 
							| WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

		// Create tabs window:
		if (!m_list.Create(dwStyle, rectDummy, this, 1))
		{
				TRACE0("Failed to create output tab window\n");
				return -1;      // fail to create
		}

		m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

		return 0;
}

void CActionView::OnSize(UINT nType, int cx, int cy)
{
		CDockablePane::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here
		// Tab control should cover the whole client area:
		m_list.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}


void CActionView::DrawActionView(const ARSpace::psrActionView_t *view)
{
		ASSERT(view != NULL);
		this->Clear();
		
		for(int i = 0; view && i < (int)view->item_cnt; ++i)
		{
				this->m_list.InsertColumn(i, PSR_IndexActionViewItem(view, i), 0, 100, -1);
		}

		for(size_t i = 0; view && i < view->row; ++i)
		{
				for(size_t k = 0; k < view->col; ++k)
				{
						const wchar_t *s = PSR_IndexActionViewAction(view, i,k);

						if(k == 0)
						{
								this->m_list.InsertItem((int)i, s);
						}else
						{
								LVITEM	item;
								memset(&item, 0, sizeof(item));

								item.mask   =   LVIF_TEXT;   
								item.pszText   =   (LPWSTR)s;
								item.cchTextMax   =  (int)( wcslen(s) + 1);
								item.iItem   =  (int)i;
								item.iSubItem = (int)k;
								this->m_list.SetItem(&item);
						}

				}
		}

		this->ShowPane(TRUE, TRUE, TRUE);
}


void	CActionView::DrawConflictView(const ARSpace::psrConflictView_t *view)
{

		size_t	i,k;

		ASSERT(view != NULL);
		this->Clear();

		

		CRect rect;
		this->GetWindowRect(&rect);

		this->m_list.InsertColumn(0, TEXT("Name"), 0, rect.Width()/ 3 * 1);
		this->m_list.InsertColumn(1, TEXT("Conflict"), 0, rect.Width() / 3 * 2);


		for(i = 0; i < view->count; ++i)
		{
				const ARSpace::psrConflictItem_t *item = view->conflict[i];
				ASSERT(item->count > 0);
				this->m_list.InsertItem(this->m_list.GetItemCount(), item->name);
				
				for(k = 0; k < item->count; ++k)
				{
						LVITEM	lv;
						memset(&lv, 0, sizeof(lv));
						lv.mask   =   LVIF_TEXT;   
						lv.pszText   =   (LPWSTR)item->items[k];
						lv.cchTextMax   =  (int)(wcslen(item->items[k]) + 1);
						
						lv.iSubItem = 1;

						if(k == 0)
						{
								lv.iItem   =  this->m_list.GetItemCount() - 1;

						}else
						{
								VERIFY(this->m_list.InsertItem(this->m_list.GetItemCount(), TEXT("")));
								lv.iItem   =  this->m_list.GetItemCount() - 1;

						}

						VERIFY(this->m_list.SetItem(&lv));
				}
		}

		this->ShowPane(TRUE, TRUE, TRUE);
}


void	CActionView::DrawFirstFollowView(const ARSpace::psrFirstFollowView_t *view)
{
		size_t	i;
		ASSERT(view != NULL);
		
		this->Clear();

		

		CRect rect;
		this->GetWindowRect(&rect);

		this->m_list.InsertColumn(0, TEXT("Name"), 0, rect.Width()/ 3 * 1);
		this->m_list.InsertColumn(1, TEXT("Set"), 0, rect.Width() / 3 * 2);

		this->m_list.InsertItem(0, TEXT("First Set:"));

		for(i = 0; i < view->first_set.count; ++i)
		{
				LVITEM	lv;
				memset(&lv, 0, sizeof(lv));
				lv.mask   =   LVIF_TEXT;   
				lv.pszText   =   (LPWSTR)view->first_set.name[i];
				lv.cchTextMax   =  (int)(wcslen(lv.pszText) + 1);
				lv.iItem   =  this->m_list.GetItemCount();
				lv.iSubItem = 0;

				this->m_list.InsertItem(&lv);

				lv.pszText   =   (LPWSTR)view->first_set.name_set[i];
				lv.cchTextMax   =  (int)(wcslen(lv.pszText) + 1);
				lv.iSubItem = 1;
				this->m_list.SetItem(&lv);
		}
		
		this->m_list.InsertItem(this->m_list.GetItemCount(), TEXT(""));
		this->m_list.InsertItem(this->m_list.GetItemCount(), TEXT(""));
		this->m_list.InsertItem(this->m_list.GetItemCount(), TEXT(""));
		this->m_list.InsertItem(this->m_list.GetItemCount(), TEXT("Follow Set:"));

		for(i = 0; i < view->follow_set.count; ++i)
		{
				LVITEM	lv;
				memset(&lv, 0, sizeof(lv));
				lv.mask   =   LVIF_TEXT;   
				lv.pszText   =   (LPWSTR)view->follow_set.name[i];
				lv.cchTextMax   =  (int)(wcslen(lv.pszText) + 1);
				lv.iItem   =  this->m_list.GetItemCount();
				lv.iSubItem = 0;

				this->m_list.InsertItem(&lv);

				lv.pszText   =   (LPWSTR)view->follow_set.name_set[i];
				lv.cchTextMax   =  (int)(wcslen(lv.pszText) + 1);
				lv.iSubItem = 1;
				this->m_list.SetItem(&lv);
		}


}

void CActionView::Clear()
{
		this->m_list.OnEditClear();

}

