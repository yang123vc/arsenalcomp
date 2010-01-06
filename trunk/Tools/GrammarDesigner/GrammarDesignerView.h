
// GrammarDesignerView.h : interface of the CGrammarDesignerView class
//


#pragma once


class CGrammarDesignerView : public CRichEditView 
{
protected: // create from serialization only
	CGrammarDesignerView();
	DECLARE_DYNCREATE(CGrammarDesignerView)

// Attributes
public:
	CGrammarDesignerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CGrammarDesignerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual void OnInitialUpdate();
		afx_msg void OnEditFontdlg();

private:
		CFont	m_font;
		HACCEL	m_acctbl;
		BOOL	m_iswarp;
private:
		void Highlight(long start, long end, COLORREF color);
public:
		BOOL   SetWordWrap(BOOL   bWordWrap);
		afx_msg void OnTestTest();
		afx_msg void OnEditWordwarp();
		afx_msg void OnUpdateEditWordwarp(CCmdUI *pCmdUI);
		afx_msg void OnEditPaste();
};

#ifndef _DEBUG  // debug version in GrammarDesignerView.cpp
inline CGrammarDesignerDoc* CGrammarDesignerView::GetDocument() const
   { return reinterpret_cast<CGrammarDesignerDoc*>(m_pDocument); }
#endif


