
// GrammarDesignerDoc.h : interface of the CGrammarDesignerDoc class
//


#pragma once


class CGrammarDesignerDoc : public CDocument
{
protected: // create from serialization only
	CGrammarDesignerDoc();
	DECLARE_DYNCREATE(CGrammarDesignerDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CGrammarDesignerDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


