#pragma once


// LexerDialog dialog

class LexerOutput : public ARSpace::ARContext
{
private:
		CEdit	m_edit;
public:
		virtual void OnError(int level, const wchar_t *msg)
		{
				append(msg);
				append(L"\r\n");
		}		

		virtual void OnFatal(const wchar_t *msg)
		{
				append(msg);
				append(L"\r\n");
		}

		virtual void OnPrint(const wchar_t *msg)
		{
				append(msg);
				append(L"\r\n");
		}

		void	Clear()
		{
				int   len   =   GetWindowTextLength(m_edit.m_hWnd);  
				m_edit.SetSel(0,  len);  
				m_edit.Clear();

		}
private:
		void append(const wchar_t *msg)
		{
				int   len   =   GetWindowTextLength(m_edit.m_hWnd);  
				m_edit.SetSel(len,  len);  
				m_edit.ReplaceSel(msg);
		}
public:
		LexerOutput(HWND output)
		{
				m_edit.Attach(output);
						
		}
		virtual ~LexerOutput()
		{
				m_edit.Detach();
		}
};




class LexerDialog : public CDialog
{
		DECLARE_DYNAMIC(LexerDialog)
		
public:
		LexerDialog(CWnd* pParent = NULL);   // standard constructor
		virtual ~LexerDialog();

		// Dialog Data
		enum { IDD = IDD_LEXERDIALOG };
		
public:
//		ARSpace::Lexer			m_lexer;
		LexerOutput				*m_output;
protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		virtual BOOL OnInitDialog();
		
		virtual BOOL PreTranslateMessage(MSG* pMsg);

		DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnBnClickedButtonMatch();
};


