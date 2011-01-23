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

// CntrItem.h : interface of the CTenguEditorCntrItem class
//

#pragma once

class CTenguEditorDoc;
class CTenguEditorView;

class CTenguEditorCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CTenguEditorCntrItem)

// Constructors
public:
	CTenguEditorCntrItem(REOBJECT* preo = NULL, CTenguEditorDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer

// Attributes
public:
	CTenguEditorDoc* GetDocument()
		{ return reinterpret_cast<CTenguEditorDoc*>(CRichEditCntrItem::GetDocument()); }
	CTenguEditorView* GetActiveView()
		{ return reinterpret_cast<CTenguEditorView*>(CRichEditCntrItem::GetActiveView()); }

// Implementation
public:
	~CTenguEditorCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

