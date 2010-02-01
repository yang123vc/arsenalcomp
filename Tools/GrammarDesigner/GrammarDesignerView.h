
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
		UINT	m_timer_interval;
private:
		void Highlight(long start, long end, COLORREF color);
public:
		BOOL   SetWordWrap(BOOL   bWordWrap);
		afx_msg void OnTestTest();

		afx_msg LRESULT OnBuildTagCompleted(WPARAM wp, LPARAM lp);

		afx_msg void OnEditWordwarp();
		afx_msg void OnUpdateEditWordwarp(CCmdUI *pCmdUI);
		afx_msg void OnEditPaste();
		afx_msg LRESULT OnLocatePos(WPARAM wp, LPARAM lp);

		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnClose();

public:
		
		afx_msg void OnResetTimer(UINT nID);
		afx_msg void OnUpdateSetrebuildtime100ms(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSetrebuildtime500ms(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSetrebuildtime1000ms(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSetrebuildtime3000ms(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSetrebuildtime5000ms(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSetrebuildtimeDisable(CCmdUI *pCmdUI);
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnEnChange();
		
		

		afx_msg void OnEditOpen();
		afx_msg void OnEditSaveFile();
//		virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
		afx_msg void OnDropFiles(HDROP hDropInfo);
protected:
		virtual HRESULT QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, DWORD dwReco, BOOL bReally, HGLOBAL hMetaFile);
public:
		afx_msg void OnGoto();
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in GrammarDesignerView.cpp
inline CGrammarDesignerDoc* CGrammarDesignerView::GetDocument() const
   { return reinterpret_cast<CGrammarDesignerDoc*>(m_pDocument); }
#endif


