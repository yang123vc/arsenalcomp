#pragma once
#include "afxdockablepane.h"


class CInputEdit : public CEdit
{
protected:
		CFont	m_font;
		HACCEL	m_acctbl;
public:
		CInputEdit();
		virtual ~CInputEdit();
public:

		virtual BOOL PreTranslateMessage(MSG* pMsg);
		DECLARE_MESSAGE_MAP()
		afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
		afx_msg void OnEditCut();
		afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
		afx_msg void OnEditCopy();
		afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
		afx_msg void OnEditPaste();
		afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
		afx_msg void OnEditSelectAll();
		afx_msg void OnUpdateEditSelectAll(CCmdUI *pCmdUI);
		afx_msg void OnEditClear();
		afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnInputFont();
		afx_msg void OnPopupOpen();
		afx_msg void OnPopupSave();
		
};


class CInputPane :	public CDockablePane
{
protected:
		CInputEdit		m_input;
public:
		CInputPane(void);
		~CInputPane(void);
		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
public:
		afx_msg LRESULT OnLocatePos(WPARAM wp, LPARAM lp);
public:

		int   GetInputLength();
		CString	GetInput()const;
};
