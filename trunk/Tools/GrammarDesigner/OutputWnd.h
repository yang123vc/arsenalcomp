
#pragma once



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
//	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
		CFont			m_font;
public:
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
		afx_msg void OnOutputwndFont();
protected:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};






class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd();

// Attributes
protected:
	COutputList			m_output;


protected:

// Implementation
public:
	virtual ~COutputWnd();
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
		

	DECLARE_MESSAGE_MAP()
};



