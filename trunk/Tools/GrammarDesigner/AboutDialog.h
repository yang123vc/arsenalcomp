#pragma once
#include "HyperLink.h"


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
private:
		CString			m_name;
		CString			m_ver;
		CString			m_arsenal_ver;
		CString			m_copyright;
		CString			m_homepage;
private:
		CHyperLink		m_hplink;
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
		virtual BOOL OnInitDialog();
		afx_msg void OnStnClickedHomepage();
};
