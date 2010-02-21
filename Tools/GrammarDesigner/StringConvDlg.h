#pragma once


// CStringConvDlg dialog

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
};
