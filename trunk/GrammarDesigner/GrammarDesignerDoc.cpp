
// GrammarDesignerDoc.cpp : implementation of the CGrammarDesignerDoc class
//

#include "stdafx.h"
#include "GrammarDesigner.h"

#include "GrammarDesignerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrammarDesignerDoc

IMPLEMENT_DYNCREATE(CGrammarDesignerDoc, CDocument)

BEGIN_MESSAGE_MAP(CGrammarDesignerDoc, CDocument)
END_MESSAGE_MAP()


// CGrammarDesignerDoc construction/destruction

CGrammarDesignerDoc::CGrammarDesignerDoc()
{
	// TODO: add one-time construction code here

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




// CGrammarDesignerDoc serialization

void CGrammarDesignerDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
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
