#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CEncodeConvert dialog

class CEncodeConvert : public CDialogEx
{
	DECLARE_DYNAMIC(CEncodeConvert)

private:
		typedef enum
		{
				BASE64,
				ROT13,
				HEX,
				URL,
				MD5,
				SHA1,
		}EncodeType;

protected:
		EncodeType				GetEncodeType();
		int						GetEncodeCharset();
		BOOL					IsEncode();

		BOOL					rot13_convert(const CString &input, CString &output);
		BOOL					base64_convert(const CString &input, CString &output);
		BOOL					hex_convert(const CString &input, CString &output);
		BOOL					url_convert(const CString &input, CString &output);

		BOOL					md5_convert(const CString &input, CString &output);
		BOOL					sha1_convert(const CString &input, CString &output);
public:
	CEncodeConvert(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEncodeConvert();
public:
		virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_ENCODE_CONV };

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CRichEditCtrl m_input;
	CRichEditCtrl m_output;


public:
		afx_msg void OnBnClickedButtonConvert();
		afx_msg void OnBnClickedRadioEncode();
		afx_msg void OnBnClickedRadioDecode();
};
