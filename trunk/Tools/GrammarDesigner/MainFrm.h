
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "OutputWnd.h"
#include "ActionView.h"
#include "TagView.h"
#include "InputPane.h"
#include "SyntaxPane.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	
	CFont			  m_status_font;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	
/**/	
	COutputWnd			m_wndOutput;
	CActionView			m_wndActView;
	CTagView			m_wndTag;

	CInputPane			m_inputPane;
	CSyntaxPane			m_syntaxPane;	

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void CMainFrame::OnUpdateStatusBarPanes(CCmdUI* pCmdUI);

	

	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
		
public:

		CMFCStatusBar& GetStatusBar(void);
		CMFCMenuBar& GetMenuBar(void);
		CMFCToolBar& GetToolBar(void);
		CTagView&	GetTagView();
		COutputWnd&		GetOutputView();
		CActionView&	GetActionView();
		CInputPane&		GetInputPane();
		CSyntaxPane&	GetSyntaxPnae();
		void			ClearShow();
		
public:
		afx_msg void OnSize(UINT nType, int cx, int cy);

public:
		afx_msg void OnTestTest();
		afx_msg void OnShowOutput();
		afx_msg void OnShowSyntaxtree();
		afx_msg void OnShowActionview();
		afx_msg void OnShowTagview();
		afx_msg void OnShowInput();
};


