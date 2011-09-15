#pragma once
#include "afxwin.h"


// CStringConvDlg dialog

class CStringEdit : public CRichEditCtrl  //CEdit
{
private:
		HACCEL	m_acctbl;
		CFont	m_font;
public:
		CStringEdit();
		virtual ~CStringEdit();
		DECLARE_MESSAGE_MAP()
		afx_msg void OnEditSelectAll();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnEditCopy();
		afx_msg void OnEditCut();
		afx_msg void OnEditPaste();
		afx_msg void OnEditClear();
		afx_msg void OnEditUndo();
		afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


class CStringConvDlg : public CDialog
{
	DECLARE_DYNAMIC(CStringConvDlg)

public:
		CFont	m_font;
public:
	CStringConvDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStringConvDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_STR_CODE_CONV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnBnClickedButtonToCode();
		afx_msg void OnBnClickedButtonToString();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual BOOL OnInitDialog();
		afx_msg void OnEnChangeEditString();
public:
		CStringEdit m_input_string;
		CStringEdit m_input_code;
};
