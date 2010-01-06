
#include "stdafx.h"
#include "resource.h"
#include "Mainfrm.h"
#include "OutputItem.h"


/////////////////////////////////////////////////////////////////////////////
/*
class OutputContext : public ArsenalCPP::ARContext
{

public:
		virtual void OnError(int_t level, const wchar_t *msg) = 0;	
		virtual void OnPrint(const wchar_t *msg) = 0;
		virtual ~ARContext() = 0{}
};
*/
/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
		m_font.CreatePointFont(90, TEXT("Consolas"));
		m_ctx = NULL;
}

COutputList::~COutputList()
{
}


BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &COutputList::OnUpdateEditCopy)
	ON_COMMAND(ID_OUTPUTWND_FONT, &COutputList::OnOutputwndFont)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers


OutputContext*	COutputList::GetContext()
{
		return m_ctx;
}

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{

	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);
	
	((CWinAppEx*)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(pSumMenu->GetSafeHmenu(),point.x, point.y, this, TRUE);

	SetFocus();

}

void COutputList::OnEditCopy()
{
		int i = this->GetCurSel();

		if(i == -1)return;
		
		CString src;
		this->GetText(i, src);

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

void COutputList::OnEditClear()
{
		this->ResetContent();
}


void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner()->GetOwner());
	CMainFrame *pMainFrame =  DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	
	if(pParentBar && pMainFrame)
	{
			pMainFrame->SetFocus();
			pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
			pMainFrame->RecalcLayout();
	}
}


void COutputList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
		// TODO: Add your message handler code here and/or call default
		
		CListBox::OnLButtonDblClk(nFlags, point);
}




int COutputList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CListBox::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here
		this->SetFont(&m_font);

		this->AddString(TEXT(__FUNCSIG__));

		return 0;
}


static int GetSelectIndex(CListBox &box, CPoint pt)
{

		for(int i = 0; i < box.GetCount(); ++i)
		{
				CRect rect;
				box.GetItemRect(i, &rect);

				if(rect.PtInRect(pt))
				{
						return i;
				}
		}				
		
		return -1;
}



void COutputList::OnRButtonDown(UINT nFlags, CPoint point)
{
		// TODO: Add your message handler code here and/or call default

		int i = GetSelectIndex(*this, point);

		if(i != -1) this->SetCurSel(i);

		CListBox::OnRButtonDown(nFlags, point);
}

void COutputList::OnLButtonDown(UINT nFlags, CPoint point)
{
		// TODO: Add your message handler code here and/or call default
		int i = GetSelectIndex(*this, point);
		
		if(i == -1)
		{
				this->SetCurSel(-1);
		}else
		{
				this->SetCurSel(i);
		}
		
		CListBox::OnLButtonDown(nFlags, point);
}



void COutputList::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here
		
		pCmdUI->Enable(this->GetCurSel() >= 0);

}

void COutputList::OnOutputwndFont()
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

BOOL COutputList::PreCreateWindow(CREATESTRUCT& cs)
{
		// TODO: Add your specialized code here and/or call the base class
		const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
		cs.style = dwStyle;
		return CListBox::PreCreateWindow(cs);
}
