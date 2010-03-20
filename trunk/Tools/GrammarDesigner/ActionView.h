#pragma once


// CActionView



class CActionCtrl : public CListCtrl
{
private:
		DECLARE_DYNAMIC(CActionCtrl)
public:
		CActionCtrl();
		virtual ~CActionCtrl();
protected:
		DECLARE_MESSAGE_MAP()
private:
		CFont			m_font;
public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
		afx_msg void OnActionViewFont();
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnEditCopy();
		afx_msg void OnUpdateOutputwndFont(CCmdUI *pCmdUI);
		afx_msg void OnEditClear();
//		afx_msg void OnViewAction();
};


class CActionView : public CDockablePane
{
	DECLARE_DYNAMIC(CActionView)
		
public:
	CActionView();
	virtual ~CActionView();

protected:
	DECLARE_MESSAGE_MAP()

private:
		CActionCtrl		m_list;
public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);

public:
		void	DrawActionView(const ARSpace::psrActionView_t *view);
		void	DrawConflictView(const ARSpace::psrConflictView_t *view);
		
		
		void	DrawFirstFollowView(const ARSpace::psrFirstFollowView_t *view);

		void	DrawLeftRecursionView(const ARSpace::psrFirstFollowView_t *view);
		void	DrawLeftFactorView(const ARSpace::psrFirstFollowView_t *view);
		
		void	Clear();
};


