
// GrammarDesigner.h : main header file for the GrammarDesigner application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGrammarDesignerApp:
// See GrammarDesigner.cpp for the implementation of this class
//

class CGrammarDesignerApp : public CWinAppEx
{
public:
	CGrammarDesignerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

public:
		
		//BOOL OnThreadMessage(MSG* pMsg);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CGrammarDesignerApp theApp;
