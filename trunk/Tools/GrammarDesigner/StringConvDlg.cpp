// StringConvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GrammarDesigner.h"
#include "StringConvDlg.h"


// CStringConvDlg dialog

IMPLEMENT_DYNAMIC(CStringConvDlg, CDialog)

CStringConvDlg::CStringConvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStringConvDlg::IDD, pParent)
{
		VERIFY(m_font.CreatePointFont(120, TEXT("Consolas")));
}

CStringConvDlg::~CStringConvDlg()
{
}

void CStringConvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStringConvDlg, CDialog)
		ON_BN_CLICKED(IDC_BUTTON_TO_CODE, &CStringConvDlg::OnBnClickedButtonToCode)
		ON_BN_CLICKED(IDC_BUTTON_TO_STRING, &CStringConvDlg::OnBnClickedButtonToString)
END_MESSAGE_MAP()


// CStringConvDlg message handlers

void CStringConvDlg::OnBnClickedButtonToCode()
{
		// TODO: Add your control notification handler code here

		CString input;
		this->GetDlgItemText(IDC_EDIT_STRING,input);

		if(input.IsEmpty())
		{
				this->MessageBox(TEXT("Invalid string input!"));
				return;
		}
		
		int_t len = ARSpace::AR_str_to_escstr_buf(NULL, 0, input.GetString());

		AR_ASSERT(len > 0);

		wchar_t *code = new wchar_t[len + 1];
		
		
		len = ARSpace::AR_str_to_escstr_buf(code, len, input.GetString());
		AR_ASSERT(len == len);

		this->SetDlgItemText(IDC_EDIT_CODE, code);

		delete []code;

}

void CStringConvDlg::OnBnClickedButtonToString()
{
		// TODO: Add your control notification handler code here


		CString input;
		this->GetDlgItemText(IDC_EDIT_CODE,input);

		if(input.IsEmpty())
		{
				this->MessageBox(TEXT("Invalid code input!"));
				return;
		}
		
		ARSpace::arEscStrErr_t	err;
		int_t len;
		
		len = ARSpace::AR_escstr_to_str_buf(NULL, 0, input.GetString(), &err);
		
		CString info;
		
		if(err.type == AR_ESCSTR_ERR_VALUE)
		{
				info.Format(L"Invalid value %I64d : too big for wide character\r\n", err.value);
		}else if(err.type == AR_ESCSTR_ERR_CHAR)
		{
				wchar_t c = *err.pos;
				if(iswprint(c))
				{
						info.Format(L"Invalid char %c : unrecognized character escape sequence\r\n", c);
				}else
				{
						info.Format(L"Invalid char \\x%x : unrecognized character escape sequence\r\n", c);
				}
		}else if(err.type == AR_ESCSTR_ERR_BUFFER)
		{
				AR_ASSERT(false);
				return;
		}


		if(!info.IsEmpty())
		{
				this->SetDlgItemText(IDC_EDIT_STRING, info);
				return;
		}
		
		wchar_t *str = new wchar_t[len];
		len = ARSpace::AR_escstr_to_str_buf(str, len, input.GetString(), &err);

		ASSERT(len == len && err.type == AR_ESCSTR_ERR_OK);

		this->SetDlgItemText(IDC_EDIT_STRING, str);

		delete []str;
}

BOOL CStringConvDlg::PreTranslateMessage(MSG* pMsg)
{
		// TODO: Add your specialized code here and/or call the base class
/*
		if (pMsg->message == WM_KEYDOWN && 	(pMsg->wParam == VK_ESCAPE || pMsg->wParam==VK_RETURN))
		{
				return TRUE;
		}    
*/
		return CDialog::PreTranslateMessage(pMsg);
}

BOOL CStringConvDlg::OnInitDialog()
{
		CDialog::OnInitDialog();

		// TODO:  Add extra initialization here

		CEdit *pedit = (CEdit*)this->GetDlgItem(IDC_EDIT_STRING);
		pedit->SetFont(&m_font);

		
		pedit = (CEdit*)this->GetDlgItem(IDC_EDIT_CODE);
		pedit->SetFont(&m_font);

		

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}
