
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

		ON_COMMAND_RANGE(ID_PARSER_MODE_SLR, ID_PARSER_MODE_LALR, &CGrammarDesignerDoc::OnChangeParserMode)
		ON_UPDATE_COMMAND_UI(ID_PARSER_MODE_LALR, &CGrammarDesignerDoc::OnUpdateParserModeLalr)
		ON_UPDATE_COMMAND_UI(ID_PARSER_MODE_LR, &CGrammarDesignerDoc::OnUpdateParserModeLr)
		ON_UPDATE_COMMAND_UI(ID_PARSER_MODE_SLR, &CGrammarDesignerDoc::OnUpdateParserModeSlr)
		ON_COMMAND(ID_EDIT_GOTO_DECL, &CGrammarDesignerDoc::OnEditGotoDecl)
		ON_UPDATE_COMMAND_UI(ID_EDIT_GOTO_DECL, &CGrammarDesignerDoc::OnUpdateEditGotoDecl)
		ON_COMMAND(ID_PARSER_BUILD, &CGrammarDesignerDoc::OnParserBuild)
		ON_COMMAND(ID_PARSER_SHOWACTIONTABLE, &CGrammarDesignerDoc::OnParserShowactiontable)
		ON_UPDATE_COMMAND_UI(ID_PARSER_SHOWACTIONTABLE, &CGrammarDesignerDoc::OnUpdateParserShowactiontable)
		ON_COMMAND(ID_PARSER_SHOWCONFLICT, &CGrammarDesignerDoc::OnParserShowconflict)
		ON_UPDATE_COMMAND_UI(ID_PARSER_SHOWCONFLICT, &CGrammarDesignerDoc::OnUpdateParserShowconflict)
		ON_COMMAND(ID_PARSER_SHOWFIRSTFOLLOW, &CGrammarDesignerDoc::OnParserShowfirstfollow)
		ON_UPDATE_COMMAND_UI(ID_PARSER_SHOWFIRSTFOLLOW, &CGrammarDesignerDoc::OnUpdateParserShowfirstfollow)
END_MESSAGE_MAP()


// CGrammarDesignerDoc construction/destruction

CGrammarDesignerDoc::CGrammarDesignerDoc()
{
	// TODO: add one-time construction code here
		m_encoding		= CTextFileBase::ASCII;
		m_parser_mode	= ARSpace::PSR_LALR;
		m_lexer			= NULL;
		m_parser		=	NULL;
		
}

CGrammarDesignerDoc::~CGrammarDesignerDoc()
{
		ClearParser();
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



void CGrammarDesignerDoc::OnChangeParserMode(UINT nID)
{
		switch(nID)
		{
		case ID_PARSER_MODE_SLR:
				m_parser_mode = ARSpace::PSR_SLR;
				break;
		case ID_PARSER_MODE_LR:
				m_parser_mode = ARSpace::PSR_LR1;
				break;
		case ID_PARSER_MODE_LALR:
				m_parser_mode = ARSpace::PSR_LALR;
				break;
		default:
				VERIFY(false);
		}
}


static void AR_STDCALL report_tag_func(const ARSpace::cfgReportInfo_t *report, void *context)
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

		ARSpace::cfgReport_t	report = {report_tag_func, NULL};
		ARSpace::cfgConfig_t *cfg = ARSpace::CFG_CollectGrammarConfig(str.GetString(), &report);

		if(cfg != NULL)
		{
				CMainFrame *main_frm = (CMainFrame*)::AfxGetMainWnd();

				CTagView &tag = main_frm->GetTagView();

				tag.UpdateTag(cfg);

				ARSpace::CFG_DestroyGrammarConfig(cfg);
		}
}


void CGrammarDesignerDoc::OnUpdateParserModeLalr(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(m_parser_mode == ARSpace::PSR_LALR);
}

void CGrammarDesignerDoc::OnUpdateParserModeLr(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(m_parser_mode == ARSpace::PSR_LR1);
}

void CGrammarDesignerDoc::OnUpdateParserModeSlr(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(m_parser_mode == ARSpace::PSR_SLR);
}


void CGrammarDesignerDoc::OnEditGotoDecl()
{
		// TODO: Add your command handler code here
		
		CGrammarDesignerView *view = (CGrammarDesignerView*)reinterpret_cast<CGrammarDesignerView*>(m_viewList.GetHead());


		CString sel = view->GetRichEditCtrl().GetSelText();

		sel.TrimLeft();
		sel.TrimRight();

		if(!sel.IsEmpty())
		{
		
				CMainFrame *main_frm = (CMainFrame*)::AfxGetMainWnd();

				CTagView &tag = main_frm->GetTagView();

				const CSrcInfo *info = tag.LookupByName(sel);

				if(info)
				{
						view->OnLocatePos(info->m_line, NULL);
				}else
				{
						CString str;
						str.Format(TEXT("The symbol \"%ls\" not declared"), sel.GetString());
						view->MessageBox(str);
				}
		}
	
}

void CGrammarDesignerDoc::OnUpdateEditGotoDecl(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here

		CGrammarDesignerView *view = (CGrammarDesignerView*)reinterpret_cast<CGrammarDesignerView*>(m_viewList.GetHead());

		CString sel = view->GetRichEditCtrl().GetSelText();
		sel.TrimLeft();
		sel.TrimRight();
		pCmdUI->Enable(!sel.IsEmpty());
}




static void AR_STDCALL report_build_func(const ARSpace::cfgReportInfo_t *report, void *context)
{
		COutputWnd *output;
		COutputList::Param		param;
		CString str;
		ASSERT(context != NULL);

		output = (COutputWnd*)context;

		switch(report->type)
		{
		case ARSpace::CFG_REPORT_MESSAGE_T:
				param.type = COutputList::MSG_MESSAGE;
				param.line = 0;
				output->Append(report->message, param);
				//AR_printf(L"%ls\r\n", report->message);
				break;
		case ARSpace::CFG_REPORT_ERROR_T:
				str.Format(TEXT("%ls : %d"), report->message, report->err_level);
				param.type = COutputList::MSG_ERROR;
				param.line = report->tok->line;
				output->Append(str, param);
				break;
		case ARSpace::CFG_REPORT_ERR_LEX_T:
				str.Format(TEXT("Lex error %ls"), report->message);
				param.type = COutputList::MSG_ERROR;
				param.line = report->tok->line;
				output->Append(str, param);

				//::AfxMessageBox(report->message);
				//AR_printf(L"lex error %ls\r\n", report->message);
				break;
		case ARSpace::CFG_REPORT_ERR_SYNTAX_T:
				str.Format(TEXT("Syntax error %ls"), report->message);
				param.type = COutputList::MSG_ERROR;
				param.line = report->tok->line;
				output->Append(str, param);
				break;
		default:
				AR_ASSERT(false);
		}
}




void	CGrammarDesignerDoc::ClearParser()
{
		
		ASSERT((m_lexer == NULL && m_parser == NULL) || (m_lexer != NULL && m_parser != NULL));
		
		if(m_lexer != NULL && m_parser != NULL)
		{
				delete m_lexer;
				delete m_parser;
		}
		/*

		if(m_lexer != NULL)
		{
				delete m_lexer;
		}

		if(m_parser != NULL)
		{
				delete m_parser;
		}

		*/
}



class ReportIOContext : public ArsenalCPP::ARContext
{
public:
		COutputWnd		&m_output;
public:
		virtual void OnError(int_t level, const wchar_t *msg)
		{
				m_output.Append(msg);
		}

		virtual void OnPrint(const wchar_t *msg)
		{
				m_output.Append(msg);
		}
public:
		ReportIOContext(COutputWnd &output) : m_output(output)
		{

		}

		virtual ~ReportIOContext()
		{
		}
};






bool CGrammarDesignerDoc::BuildParser(const ARSpace::cfgConfig_t		*cfg)
{
		
		CMainFrame *main_frm = (CMainFrame*)::AfxGetMainWnd();
		COutputWnd		&output = main_frm->GetOutputView();

		bool has_error = false;
		
		ClearParser();

		ArsenalCPP::Lexer		*lexer = new ArsenalCPP::Lexer(new ReportIOContext(output));
		ArsenalCPP::Grammar		*grammar = new ArsenalCPP::Grammar(new ArsenalCPP::DummyNodeContext(), new ReportIOContext(output));



		for(size_t i = 0; i < cfg->name_cnt; ++i)
		{
				const ARSpace::cfgName_t		*name = &cfg->name[i];
				if(!lexer->Insert(name->name, name->regex))
				{
						CString msg;
						msg.Format(TEXT("Name Error : \"%ls : %ls\""), name->name, name->regex);
						output.Append(msg, COutputList::MSG_ERROR, name->line);
						has_error = true;
				}
		}

		for(size_t i = 0; i < cfg->tok_cnt; ++i)
		{
				const ARSpace::cfgToken_t		*tok = &cfg->tok[i];

				if(!lexer->Insert(tok->regex, tok->tokval, tok->lex_prec, tok->is_skip))
				{
						CString msg;
						msg.Format(TEXT("Token Error :  \"%ls : %ls\""), tok->name, tok->regex);
						output.Append(msg, COutputList::MSG_ERROR, tok->line);
						has_error = true;
						continue;
				}

				if(tok->is_skip || tok->tokval == 0)continue;
				if(!grammar->Insert(tok->name, tok->tokval))
				{
						CString msg;

						msg.Format(TEXT("Token Error : \"%ls : %ls\""), tok->name, tok->regex);
						output.Append(msg, COutputList::MSG_ERROR, tok->line);
						has_error = true;
				}
		}

		
		for(size_t i = 0; i < cfg->prec_cnt; ++i)
		{
				const ARSpace::cfgPrec_t		*prec = &cfg->prec[i];
				ASSERT(prec->prec_tok != NULL);
				if(!grammar->Insert(prec->prec_tok, prec->prec_tok_val, prec->assoc, prec->prec_level))
				{
						CString msg;
						msg.Format(TEXT("Prec Error : \"%ls\"!"), prec->prec_tok);
						output.Append(msg, COutputList::MSG_ERROR, prec->line);
						has_error = true;
				}
		}


		for(size_t i = 0; i < cfg->rule_cnt; ++i)
		{
				const ARSpace::cfgRule_t		*rule = &cfg->rule[i];
				CString str;
				str.Format(TEXT("%ls : %ls"), rule->lhs, rule->rhs);
				if(!grammar->Insert(str.GetString(), rule->prec_tok))
				{
						CString msg;
						msg.Format(TEXT("Rule Error : \"%ls\"!"), str.GetString());
						output.Append(msg, COutputList::MSG_ERROR, rule->line);
						has_error = true;
				}
		}

		if(!grammar->IsValid())
		{
				has_error = true;
		}


		if(has_error)
		{
				delete lexer;
				delete grammar;
				return false;
		}else
		{
				DWORD beg, end;

				m_lexer = lexer;
				m_lexer->Generate();


				beg = GetTickCount();
				
				m_parser = new ArsenalCPP::Parser(grammar, m_parser_mode);
				
				end = GetTickCount();

				CString str;

				str.Format(TEXT("Build Parser Tick count %d"), end - beg);

				output.Append(str);
				
				return true;
		}
}



void CGrammarDesignerDoc::OnParserBuild()
{
		// TODO: Add your command handler code here

		CGrammarDesignerView *view = (CGrammarDesignerView*)reinterpret_cast<CGrammarDesignerView*>(m_viewList.GetHead());

		CString str;
		view->GetRichEditCtrl().GetWindowText(str);
		
		
		CMainFrame *main_frm = (CMainFrame*)::AfxGetMainWnd();
		
		COutputWnd		&output = main_frm->GetOutputView();
		
		output.Clear();

		ARSpace::cfgReport_t	report = {report_build_func, (void*)&output};
		ARSpace::cfgConfig_t *cfg = ARSpace::CFG_CollectGrammarConfig(str.GetString(), &report);
		
		if(cfg == NULL || cfg->has_error || !BuildParser(cfg))
		{
				goto FAILED_POINT;		
		}


		if(cfg)ARSpace::CFG_DestroyGrammarConfig(cfg);
		output.Append(TEXT("Build Parser successful!"));
		main_frm->ShowPane(&output, TRUE, TRUE, TRUE);
		return;

FAILED_POINT:
		if(cfg)ARSpace::CFG_DestroyGrammarConfig(cfg);
		output.Append(TEXT("Build Parser failed!"));
		main_frm->ShowPane(&output, TRUE, TRUE, TRUE);
		return;
}


void CGrammarDesignerDoc::OnParserShowactiontable()
{
		// TODO: Add your command handler code here

		ASSERT(m_parser != NULL);

		const ARSpace::psrActionView_t		*view = m_parser->CreateActionView();

		ASSERT(view != NULL);

		CMainFrame *main_frm = (CMainFrame*)::AfxGetMainWnd();
		CActionView	&action = main_frm->GetActionView();

		action.DrawActionView(view);
		
		m_parser->DestroyActionView(view);

		main_frm->ShowPane(&action, TRUE, TRUE, TRUE);

}

void CGrammarDesignerDoc::OnUpdateParserShowactiontable(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here

		pCmdUI->Enable(m_parser != NULL);
}

void CGrammarDesignerDoc::OnParserShowconflict()
{
		// TODO: Add your command handler code here
		ASSERT(m_parser != NULL);

		const ARSpace::psrConflictView_t		*view = m_parser->CreateConflictView();
		

		ASSERT(view != NULL);

		CMainFrame *main_frm = (CMainFrame*)::AfxGetMainWnd();
		CActionView	&action = main_frm->GetActionView();

		action.DrawConflictView(view);
		
		m_parser->DestroyConflictView(view);

		main_frm->ShowPane(&action, TRUE, TRUE, TRUE);
}


void CGrammarDesignerDoc::OnUpdateParserShowconflict(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here
		pCmdUI->Enable(m_parser != NULL);
}

void CGrammarDesignerDoc::OnParserShowfirstfollow()
{
		// TODO: Add your command handler code here

		ASSERT(m_parser != NULL);

		const ARSpace::psrFirstFollowView_t		*view = m_parser->CreateFirstFollowView();
		

		ASSERT(view != NULL);

		CMainFrame *main_frm = (CMainFrame*)::AfxGetMainWnd();
		CActionView	&action = main_frm->GetActionView();
		
		action.DrawFirstFollowView(view);

		m_parser->DestroyFirstFollowView(view);
		main_frm->ShowPane(&action, TRUE, TRUE, TRUE);

}

void CGrammarDesignerDoc::OnUpdateParserShowfirstfollow(CCmdUI *pCmdUI)
{
		// TODO: Add your command update UI handler code here
		pCmdUI->Enable(m_parser != NULL);
}
