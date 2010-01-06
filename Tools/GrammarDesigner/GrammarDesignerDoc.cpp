
// GrammarDesignerDoc.cpp : implementation of the CGrammarDesignerDoc class
//

#include "stdafx.h"

#include "GrammarDesigner.h"
#include "GrammarDesignerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrammarDesignerDoc

IMPLEMENT_DYNCREATE(CGrammarDesignerDoc, CDocument);

BEGIN_MESSAGE_MAP(CGrammarDesignerDoc, CDocument)

		ON_COMMAND_RANGE(ID_ENDCODING_ASCII, ID_ENDCODING_UTF8, &CGrammarDesignerDoc::OnEndcodingChange)
		ON_UPDATE_COMMAND_UI_RANGE(ID_ENDCODING_ASCII, ID_ENDCODING_UTF8, &CGrammarDesignerDoc::OnEndcodingChangeUI)
END_MESSAGE_MAP()


// CGrammarDesignerDoc construction/destruction

CGrammarDesignerDoc::CGrammarDesignerDoc()
{
	// TODO: add one-time construction code here
		m_encoding = CTextFileBase::ASCII;
}

CGrammarDesignerDoc::~CGrammarDesignerDoc()
{
}

BOOL CGrammarDesignerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(NULL);
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	this->SetTitle(TEXT("New Grammar"));

	return TRUE;
}





// CGrammarDesignerDoc diagnostics

#ifdef _DEBUG
void CGrammarDesignerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGrammarDesignerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGrammarDesignerDoc commands

BOOL CGrammarDesignerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
		/*
		if (!CDocument::OnOpenDocument(lpszPathName))
				return FALSE;
		*/
		
		
		// TODO:  Add your specialized creation code here

		CTextFileRead fr(lpszPathName);

		CString txt;
		
		if(!fr.Read(txt))
		{
				return FALSE;
		}else
		{
				m_encoding = fr.GetEncoding();

				CEditView *view = (CEditView*)m_viewList.GetHead();

				ASSERT(view != NULL);
				//this->UpdateAllViews(view, NULL, this);
				view->GetEditCtrl().SetWindowText(txt);
		}
	
		
		return TRUE;
}

BOOL CGrammarDesignerDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
		// TODO: Add your specialized code here and/or call the base class

		//return CDocument::OnSaveDocument(lpszPathName);

		CEditView *view = (CEditView*)m_viewList.GetHead();

		CTextFileWrite fw(lpszPathName, m_encoding);
		
		CString buf;
		view->GetWindowText(buf);

		fw.Write(buf);

		return TRUE;
}

BOOL CGrammarDesignerDoc::SaveModified()
{
		// TODO: Add your specialized code here and/or call the base class

		return CDocument::SaveModified();
}

void CGrammarDesignerDoc::OnEndcodingChange(UINT nID)
{
		// TODO: Add your command handler code here

		switch(nID)
		{
		case ID_ENDCODING_ASCII:
				m_encoding = CTextFileBase::ASCII;
				break;
		case ID_ENDCODING_UNICODEBE:
				m_encoding = CTextFileBase::UNI16_BE;
				break;
		case ID_ENDCODING_UNICODELE:
				m_encoding = CTextFileBase::UNI16_LE;
				break;
		case ID_ENDCODING_UTF8:
				m_encoding = CTextFileBase::UTF_8;
				break;
		default:
				ASSERT(false);
		}

}

void CGrammarDesignerDoc::OnEndcodingChangeUI(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here

		switch(pCmdUI->m_nID)
		{
		case ID_ENDCODING_ASCII:
				pCmdUI->SetCheck(m_encoding == CTextFileBase::ASCII);
				break;
		case ID_ENDCODING_UNICODEBE:
				pCmdUI->SetCheck(m_encoding == CTextFileBase::UNI16_BE);
				break;
		case ID_ENDCODING_UNICODELE:
				pCmdUI->SetCheck(m_encoding == CTextFileBase::UNI16_LE);
				break;
		case ID_ENDCODING_UTF8:
				pCmdUI->SetCheck(m_encoding == CTextFileBase::UTF_8);
				break;
		default:
				ASSERT(false);
		}

}
