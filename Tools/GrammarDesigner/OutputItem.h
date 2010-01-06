#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList window


class OutputContext;


class COutputList : public CListBox
{
// Construction
public:
	COutputList();

// Implementation
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
		CFont			m_font;
		OutputContext	*m_ctx;
public:
		OutputContext*	GetContext();
public:
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
		afx_msg void OnOutputwndFont();
protected:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};



/************************************************************************************/

class COutputEdit : public CEdit
{

public:



};