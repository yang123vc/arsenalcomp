// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// CntrItem.cpp : implementation of the CTenguEditorCntrItem class
//

#include "stdafx.h"
#include "TenguEditor.h"

#include "TenguEditorDoc.h"
#include "TenguEditorView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTenguEditorCntrItem implementation

IMPLEMENT_SERIAL(CTenguEditorCntrItem, CRichEditCntrItem, 0)

CTenguEditorCntrItem::CTenguEditorCntrItem(REOBJECT* preo, CTenguEditorDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
}

CTenguEditorCntrItem::~CTenguEditorCntrItem()
{
	// TODO: add cleanup code here
}


// CTenguEditorCntrItem diagnostics

#ifdef _DEBUG
void CTenguEditorCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CTenguEditorCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

