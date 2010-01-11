
// GrammarDesignerDoc.cpp : implementation of the CGrammarDesignerDoc class
//

#include "stdafx.h"

#include "MainFrm.h"

#include "GrammarDesigner.h"
#include "GrammarDesignerDoc.h"
#include "GrammarDesignerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrammarDesignerDoc

IMPLEMENT_DYNCREATE(CGrammarDesignerDoc, CDocument);

BEGIN_MESSAGE_MAP(CGrammarDesignerDoc, CDocument)

		ON_COMMAND_RANGE(ID_ENDCODING_ASCII, ID_ENDCODING_UTF8, &CGrammarDesignerDoc::OnEndcodingChange)
		ON_UPDATE_COMMAND_UI_RANGE(ID_ENDCODING_ASCII, ID_ENDCODING_UTF8, &CGrammarDesignerDoc::OnEndcodingChangeUI)

		ON_COMMAND(ID_TOOLS_REBUILDTAGS, &CGrammarDesignerDoc::OnToolsRebuildtags)
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

	reinterpret_cast<CRichEditView*>(m_viewList.GetHead())->SetWindowText(NULL);
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	this->SetTitle(TEXT("Untitled-Grammar"));

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

				CRichEditView *view = (CRichEditView*)m_viewList.GetHead();

				ASSERT(view != NULL);
				//this->UpdateAllViews(view, NULL, this);
				view->GetRichEditCtrl().SetWindowText(txt);
		}
	
		
		return TRUE;
}

BOOL CGrammarDesignerDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
		// TODO: Add your specialized code here and/or call the base class

		//return CDocument::OnSaveDocument(lpszPathName);

		CRichEditView *view = (CRichEditView*)m_viewList.GetHead();

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


static void AR_STDCALL report_func(const ARSpace::cfgReportInfo_t *report, void *context)
{
		switch(report->type)
		{
		case ARSpace::CFG_REPORT_MESSAGE_T:
				//AR_printf(L"%ls\r\n", report->message);
				break;
		case ARSpace::CFG_REPORT_ERROR_T:
				//::AfxMessageBox(report->message);
				//AR_printf(L"%ls : %d\r\n", report->message, report->err_level);
				break;
		case ARSpace::CFG_REPORT_ERR_LEX_T:
				//::AfxMessageBox(report->message);
				//AR_printf(L"lex error %ls\r\n", report->message);
				break;
		case ARSpace::CFG_REPORT_ERR_SYNTAX_T:
				//::AfxMessageBox(report->message);
				//AR_printf(L"syntax error %ls\r\n", report->message);
				break;
		default:
				AR_ASSERT(false);
		}
}


void CGrammarDesignerDoc::OnToolsRebuildtags()
{
		// TODO: Add your command handler code here
		
		CGrammarDesignerView *view = (CGrammarDesignerView*)reinterpret_cast<CGrammarDesignerView*>(m_viewList.GetHead());
		

		ASSERT(view != NULL);
		CString str;

		view->GetWindowText(str);

		if(str != m_src_cache)
		{
				m_src_cache = str;
		}else
		{
				return;
		}

		ARSpace::cfgReport_t	report = {report_func, NULL};
		ARSpace::cfgConfig_t *cfg = ARSpace::CFG_CollectGrammarConfig(str.GetString(), &report);

		if(cfg != NULL)
		{
				CMainFrame *main_frm = (CMainFrame*)::AfxGetMainWnd();

				CTagView &tag = main_frm->GetTagView();

				tag.UpdateTag(cfg);

				ARSpace::CFG_DestroyGrammarConfig(cfg);
		}
}
