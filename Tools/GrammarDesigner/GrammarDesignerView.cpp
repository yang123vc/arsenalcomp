
// GrammarDesignerView.cpp : implementation of the CGrammarDesignerView class
//

#include "stdafx.h"

#include "GrammarDesigner.h"

#include "GrammarDesignerDoc.h"
#include "GrammarDesignerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrammarDesignerView
   
IMPLEMENT_DYNCREATE(CGrammarDesignerView, CRichEditView)

BEGIN_MESSAGE_MAP(CGrammarDesignerView, CRichEditView)
		ON_WM_KEYDOWN()
		ON_COMMAND(ID_EDIT_FONTDLG, &CGrammarDesignerView::OnEditFontdlg)
		
		ON_COMMAND(ID_EDIT_WORDWARP, &CGrammarDesignerView::OnEditWordwarp)
		ON_UPDATE_COMMAND_UI(ID_EDIT_WORDWARP, &CGrammarDesignerView::OnUpdateEditWordwarp)
		ON_COMMAND(ID_EDIT_PASTE, &CGrammarDesignerView::OnEditPaste)


		ON_COMMAND(ID_TEST_TEST, &CGrammarDesignerView::OnTestTest)
END_MESSAGE_MAP()

// CGrammarDesignerView construction/destruction



CGrammarDesignerView::CGrammarDesignerView()
{
	// TODO: add construction code here
		VERIFY(m_font.CreatePointFont(120, TEXT("Consolas")));

		m_acctbl = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

		VERIFY(m_acctbl);

		m_iswarp = true;
}

CGrammarDesignerView::~CGrammarDesignerView()
{

}

BOOL CGrammarDesignerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CRichEditView::PreCreateWindow(cs);

	if(m_iswarp)
	{
		cs.style |= (ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping
	}
	
	return bPreCreated;
}




void CGrammarDesignerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGrammarDesignerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
		((CWinAppEx*)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CGrammarDesignerView diagnostics

#ifdef _DEBUG
void CGrammarDesignerView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CGrammarDesignerView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CGrammarDesignerDoc* CGrammarDesignerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrammarDesignerDoc)));
	return (CGrammarDesignerDoc*)m_pDocument;
}
#endif //_DEBUG


// CGrammarDesignerView message handlers

BOOL CGrammarDesignerView::PreTranslateMessage(MSG* pMsg)
{
		// TODO: Add your specialized code here and/or call the base class

		if(WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST) 
		{ 
				if(::TranslateAccelerator(this->GetSafeHwnd(), m_acctbl, pMsg))
				{
						return true;
				}
		}

		switch(pMsg->message)
		{
		case WM_RBUTTONDOWN:
		{
				GetFocus();
				CPoint point;
				GetCursorPos(&point);
				this->OnContextMenu(this, point);
				return TRUE;
		}
		case WM_KEYDOWN:
		{
				switch(pMsg->wParam)
				{
				case VK_APPS:
				{
						GetFocus();
						CPoint pt = this->GetRichEditCtrl().GetCaretPos();
						this->ClientToScreen(&pt);
						this->OnContextMenu(this, pt);
						return TRUE;
				}
				}
		}
		}

		return CRichEditView::PreTranslateMessage(pMsg);
		
}



void CGrammarDesignerView::OnInitialUpdate()
{
		CRichEditView::OnInitialUpdate();

		
		// TODO: Add your specialized code here and/or call the base class

//		this->GetRichEditCtrl().SetLimitText(1024*1024*10);
		
		this->SetFont(&m_font);
}


void CGrammarDesignerView::OnEditFontdlg()
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









BOOL   CGrammarDesignerView::SetWordWrap(BOOL   bWordWrap)
{   


		if(bWordWrap)
		{
				this->m_nWordWrap = WrapNone;
		}else
		{
				this->m_nWordWrap = WrapToWindow;
		}
		
		WrapChanged();
		Invalidate();
		return TRUE;
#if(0)
		
#if defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86)

		#define	WNDPROC_PLAT	GWL_WNDPROC

#elif defined(__x86_64__) || defined(_M_X64)

		#define	WNDPROC_PLAT	GWLP_WNDPROC

#else
		#error "Not support platform\r\n"
#endif

		

		bWordWrap   =   !bWordWrap;         //   make   sure   ==TRUE   ||   ==FALSE   
		
		if(m_iswarp   ==  bWordWrap)
				return   FALSE;

		m_iswarp = !m_iswarp;

		

		//   preserve   original   control's   state.   
		CFont*   pFont   =   GetFont();   
		
		int   nLen   =   GetBufferLength();   
		TCHAR*   pSaveText   =   new   TCHAR[GetBufferLength()+1];   
		GetWindowText(pSaveText,   nLen+1);   

		//   create   new   edit   control   with   appropriate   style   and   size.   
		DWORD   dwStyle   =   dwStyleDefault   &   ~(ES_AUTOHSCROLL|WS_HSCROLL|WS_VISIBLE|WS_BORDER);   
		if   (!bWordWrap)   
				dwStyle   |=   ES_AUTOHSCROLL|WS_HSCROLL;   

		CWnd*   pParent   =   GetParent();   
		CRect   rect;   
		GetWindowRect(rect);   
		pParent->ScreenToClient(rect);   
		CWnd*   pFocus   =   GetFocus();   

		UINT   nID   =   GetDlgCtrlID();   

		HWND   hWnd   =   ::CreateWindowEx(0,   TEXT("edit"),   NULL,   dwStyle,   
				rect.left,   rect.top,   rect.right-rect.left,   rect.bottom-rect.top,   
				pParent->m_hWnd,   (HMENU)nID,   AfxGetInstanceHandle(),   NULL);   

		if   (hWnd   ==   NULL)   
		{   
				delete[]   pSaveText;   
				return   FALSE;   
		}   

		//   set   the   window   text   to   nothing   to   make   sure   following   set   doesn't   fail   
		SetWindowText(NULL);   

		//   restore   visual   state   
		::SetWindowText(hWnd,   pSaveText);   
		delete[]   pSaveText;   
		if   (pFont   !=   NULL)   
		{   
				ASSERT(pFont->m_hObject   !=   NULL);   
				::SendMessage(hWnd,   WM_SETFONT,   (WPARAM)pFont->m_hObject,   0);   
		}   


		//   detach   old   window,   attach   new   
		SetDlgCtrlID(nID+1);   
		HWND   hWndOld   =   Detach();   

		::SetWindowLong(hWndOld,   WNDPROC_PLAT,   (LONG)*GetSuperWndProcAddr());   

		ASSERT(m_hWnd   ==   NULL);   
		SubclassWindow(hWnd);   
		ASSERT(m_hWnd   ==   hWnd);   
		GetParentFrame()->SendMessage(WM_RECALCPARENT);   

		UINT   nTabStops   =   m_nTabStops;   
		GetRichEditCtrl().SetTabStops(nTabStops);   

		GetClientRect(&rect);   
		SetWindowPos(NULL,   0,   0,   0,   0,   
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);   
		SetWindowPos(NULL,   0,   0,   0,   0,   
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_DRAWFRAME);   
		SetWindowPos(NULL,   0,   0,   0,   0,   SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);   
		UpdateWindow();   

		//   destroy   old   
		::SetWindowPos(hWndOld,   NULL,   0,   0,   0,   0,   
				SWP_HIDEWINDOW|SWP_NOREDRAW|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|   
				SWP_NOZORDER);   
		::DestroyWindow(hWndOld);   

		//   restore   rest   of   state...   
		GetRichEditCtrl().LimitText(nMaxSize);   
		if   (pFocus   ==   this)   
				SetFocus();   


		ASSERT_VALID(this);   
		return   TRUE;  
#undef WNDPROC_PLAT
#endif

		

}




void CGrammarDesignerView::OnEditWordwarp()
{
		// TODO: Add your command handler code here
		VERIFY(SetWordWrap(m_iswarp));
		m_iswarp = !m_iswarp;
}


void CGrammarDesignerView::OnUpdateEditWordwarp(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here

		pCmdUI->SetCheck(m_iswarp);
}


void CGrammarDesignerView::OnEditPaste()
{
		// TODO: Add your command handler code here

		CRichEditView::OnEditPaste();
}

void CGrammarDesignerView::Highlight(long start, long end, COLORREF color)
{
		CHARFORMAT cf;
        ZeroMemory(&cf, sizeof(CHARFORMAT));
		this->GetRichEditCtrl().GetDefaultCharFormat(cf);

		cf.dwMask  |= CFM_COLOR;
		cf.dwEffects = 0;
		cf.crTextColor = color;
		long x,y;

		this->GetRichEditCtrl().GetSel(x,y);

		this->GetRichEditCtrl().SetSel(start, end);
        this->GetRichEditCtrl().SetSelectionCharFormat(cf);
		this->GetRichEditCtrl().SetSel(x,y);
		
}
void CGrammarDesignerView::OnTestTest()
{
		// TODO: Add your command handler code here
		
		static bool is_highlight = false;

		if(!is_highlight)
		{
				this->Highlight(1,5,RGB(255, 0, 0));
		}else
		{
				this->Highlight(1,5,RGB(0, 0, 0));
		}


		is_highlight = !is_highlight;
}

