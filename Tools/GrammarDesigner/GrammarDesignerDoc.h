
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
		/*enum TEXTENCODING { ASCII, UNI16_BE, UNI16_LE, UTF_8 };*/
		typedef CTextFileBase::TEXTENCODING		EncodingType;
		
		EncodingType	m_encoding;
		CString				m_src_cache;
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();

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
public:
		virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
		virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
protected:
		virtual BOOL SaveModified();
public:
		void OnEndcodingChange(UINT nID);
		afx_msg void OnEndcodingChangeUI(CCmdUI *pCmdUI);
		afx_msg void OnToolsRebuildtags();
};


