// LexerDialog.cpp : implementation file
//

#include "stdafx.h"
#include ".\AR\AR.h"
#include "LexerDialog.h"
#include "UI.h"


using namespace ARSpace;

// LexerDialog dialog

IMPLEMENT_DYNAMIC(LexerDialog, CDialog)

LexerDialog::LexerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(LexerDialog::IDD, pParent),
	m_output(NULL)
{

}

LexerDialog::~LexerDialog()
{
		ARSpace::Arsenal::UnInstance();
}

void LexerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LexerDialog, CDialog)
		ON_BN_CLICKED(IDC_BUTTON_MATCH, &LexerDialog::OnBnClickedButtonMatch)
END_MESSAGE_MAP()


// LexerDialog message handlers

void LexerDialog::OnBnClickedButtonMatch()
{
		// TODO: Add your control notification handler code here
		m_output->Clear();
		wchar_t  *buf = new wchar_t[65536];
		int n = this->GetDlgItemText(IDC_EDIT_LEX_PATTERN, buf, 65536);
		buf[n] = L'\0';
		
		if(!m_lexer.SetPattern(buf))
		{		
				AR_printf(L"Invalid lex pattern!");
				return;
		}

		
		n = this->GetDlgItemText(IDC_EDIT_LEX_INPUT, buf, 65536);
		buf[n] = L'\0';

		m_lexer.SetInput(buf);


		lexToken_t		tok;

		while(m_lexer.GetToken(tok))
		{
				wcsncpy(buf, tok.str, tok.count);
				buf[tok.count] = L'\0';
				AR_printf(L"match == %s\r\n", buf);
				
				if(tok.type == 0)
				{
						AR_printf(L"match successfully\r\n", buf);
						break;
				}
		}

		if(m_lexer.IsError())
		{
				const Lexer::lastError_t  &err = m_lexer.GetLastError();
				AR_printf(L"Match error : (%d,%d)\r\n", err.line,err.col);
		}
}

BOOL LexerDialog::PreTranslateMessage(MSG* pMsg)
{
		
		return FALSE;
}

BOOL LexerDialog::OnInitDialog()
{
		CDialog::OnInitDialog();
		m_output = new LexerOutput(GetDlgItem(IDC_EDIT_LEX_OUTPUT)->m_hWnd);
		ARSpace::Arsenal::Instance(m_output);
		
		return TRUE;
}