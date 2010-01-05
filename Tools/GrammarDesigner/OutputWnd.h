
#pragma once






class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd();

// Attributes
protected:
	CMFCTabCtrl	m_wndTabs;


protected:

	void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
		BOOL	Insert(CWnd *pwnd, const CString &lab);
		BOOL	Remove(CWnd  *pwnd);
		BOOL	SetActive(CWnd *pwnd);

	DECLARE_MESSAGE_MAP()
};



