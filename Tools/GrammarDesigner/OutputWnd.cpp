
#include "stdafx.h"
#include "GrammarDesigner.h"
#include "OutputItem.h"
#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}



BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	
	// Tab control should cover the whole client area:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}



void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
/*
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_Font);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
*/

}

BOOL	COutputWnd::SetActive(CWnd *pwnd)
{
		ASSERT(pwnd != NULL);
		
		int i = m_wndTabs.GetTabFromHwnd(pwnd->GetSafeHwnd());
		if(i >= 0)
		{
				return m_wndTabs.SetActiveTab(i);
		}else
		{
				return false;
		}
}

BOOL	COutputWnd::Insert(CWnd *pwnd, const CString &lab)
{
		ASSERT(pwnd != NULL);

		ASSERT(m_wndTabs.GetTabFromHwnd(pwnd->GetSafeHwnd()) == -1);
		
		pwnd->SetParent(this);
		m_wndTabs.AddTab(pwnd, lab);

		return SetActive(pwnd);
}


BOOL	COutputWnd::Remove(CWnd *pwnd)
{
		ASSERT(pwnd != NULL);

		int i = m_wndTabs.GetTabFromHwnd(pwnd->GetSafeHwnd());

		if(i == -1)return FALSE;
		
		return m_wndTabs.RemoveTab(i);
}
