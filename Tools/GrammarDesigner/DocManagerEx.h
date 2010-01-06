#pragma once


class CDocManagerEx :	public CDocManager
{

		DECLARE_DYNAMIC(CDocManagerEx)
public:
		CDocManagerEx(void);
		~CDocManagerEx(void);
public:
		
		virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
};
