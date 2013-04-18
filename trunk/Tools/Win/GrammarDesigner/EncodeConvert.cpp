// EncodeConvert.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "GrammarDesigner.h"
#include "EncodeConvert.h"
#include "afxdialogex.h"
#include <string>
#include "md5.h"
#include "sha1.h"

using namespace ARSpace;
// CEncodeConvert dialog

static CString str_convert(int cp, const char *str)
{
		wchar_t *tmp;
		int n;
		ASSERT(str != NULL);
		
		if(strlen(str) == 0)
		{
				return L"";
		}

		n = MultiByteToWideChar(cp, 0, str, strlen(str), NULL, 0);

		if(n <= 0)
		{
				return L"";
		}

		tmp = new wchar_t[n + 1];

		MultiByteToWideChar(cp, 0, str, strlen(str), tmp, n);

		tmp[n] = 0;

		CString ret;
		ret.SetString(tmp);
		delete []tmp;
		tmp = NULL;
		return ret;
}

static std::string wcs_convert(int cp, const wchar_t *wstr)
{
		char *tmp;
		int n;
		if(wcslen(wstr) == 0)
		{
				return "";
		}

		n = WideCharToMultiByte(cp, 0, wstr, wcslen(wstr), NULL, 0, NULL, NULL);

		if(n <= 0)
		{
				return "";
		}

		tmp = new char[n + 1];

		WideCharToMultiByte(cp, 0, wstr, wcslen(wstr), tmp, n, NULL, NULL);

		tmp[n] = 0;

		std::string ret = tmp;
		delete []tmp;
		return ret;

}


IMPLEMENT_DYNAMIC(CEncodeConvert, CDialogEx)

CEncodeConvert::CEncodeConvert(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEncodeConvert::IDD, pParent)
{

}

CEncodeConvert::~CEncodeConvert()
{
}




void CEncodeConvert::DoDataExchange(CDataExchange* pDX)
{
		CDialogEx::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_EDIT_STRING_INPUT, m_input);
		DDX_Control(pDX, IDC_EDIT_STRING_OUTPUT, m_output);

}


BEGIN_MESSAGE_MAP(CEncodeConvert, CDialogEx)
		ON_BN_CLICKED(IDC_BUTTON_CONVERT, &CEncodeConvert::OnBnClickedButtonConvert)
		ON_BN_CLICKED(IDC_RADIO_ENCODE, &CEncodeConvert::OnBnClickedRadioEncode)
		ON_BN_CLICKED(IDC_RADIO_DECODE, &CEncodeConvert::OnBnClickedRadioDecode)
END_MESSAGE_MAP()



CEncodeConvert::EncodeType		CEncodeConvert::GetEncodeType()
{
		


		if( ((CButton*)this->GetDlgItem(IDC_RADIO_ENCTYPE_BASE64))->GetCheck())
		{
				return BASE64;
		}else if( ((CButton*)this->GetDlgItem(IDC_RADIO_ENCTYPE_ROT13))->GetCheck())
		{
				return ROT13;
		}else if( ((CButton*)this->GetDlgItem(IDC_RADIO_ENCTYPE_HEX))->GetCheck())
		{
				return HEX;
		}else if( ((CButton*)this->GetDlgItem(IDC_RADIO_ENCTYPE_URL))->GetCheck())
		{
				return URL;
		}else if( ((CButton*)this->GetDlgItem(IDC_RADIO_ENCTYPE_MD5))->GetCheck())
		{
				return MD5;
		
		}else if( ((CButton*)this->GetDlgItem(IDC_RADIO_ENCTYPE_SHA))->GetCheck())
		{
				return SHA1;
		}else
		{
				AR_ASSERT(false);
				return BASE64;
		}
}
		

int			CEncodeConvert::GetEncodeCharset()
{



		if( ((CButton*)this->GetDlgItem(IDC_RADIO_CS_UTF8))->GetCheck())
		{
				return CP_UTF8;
		}
		
		if( ((CButton*)this->GetDlgItem(IDC_RADIO_CS_GB2312))->GetCheck())
		{
				return 936;
		}
		
		if( ((CButton*)this->GetDlgItem(IDC_RADIO_CS_BIG5))->GetCheck())
		{
				return 950;
		}

		
		return CP_ACP;

}

BOOL					CEncodeConvert::IsEncode()
{
		if( ((CButton*)this->GetDlgItem(IDC_RADIO_ENCODE))->GetCheck())
		{
				return TRUE;
		}else
		{
				return FALSE;
		}
}

BOOL	CEncodeConvert::rot13_convert(const CString &input, CString &output)
{
		std::string mbs;
		char *tmp;
		
		
		AR_ASSERT(input.GetLength() > 0);

		if(input.GetLength() == 0)
		{
				output = L"empty input!";
				return FALSE;
		}

		mbs = wcs_convert(this->GetEncodeCharset(), input);
		if(mbs.size() == 0)
		{
				output = L"invalid charset!";
				return FALSE;
		}

		tmp = new char[mbs.size() + 1];
		strcpy(tmp, mbs.c_str());

		ARSpace::AR_strrot13(tmp, strlen(tmp));


		output = str_convert(this->GetEncodeCharset(), tmp);

		delete []tmp;
		tmp = NULL;

		if(output.GetLength() == 0)
		{
				output = L"invalid input!";
				return FALSE;
		}else
		{
				return TRUE;
		}

}



BOOL		CEncodeConvert::base64_convert(const CString &input, CString &output)
{
		
		std::string mbs;
		ar_byte_t *tmp;
		size_t n;
		AR_ASSERT(input.GetLength() > 0);

		if(input.GetLength() == 0)
		{
				output = L"empty input!";
				return FALSE;
		}

		if(this->IsEncode())
		{
				mbs = wcs_convert(this->GetEncodeCharset(), input);

				if(mbs == "")
				{
						output = L"invalid charset!";
						return FALSE;
				}

				n = ARSpace::AR_base64_encode(NULL, 0, (const ar_byte_t*)mbs.c_str(), mbs.size());

				if(n == 0)
				{
						output = L"invalid input!";
						return FALSE;
				}

				tmp = new byte[n + 1];
				n = ARSpace::AR_base64_encode(tmp, n, (const ar_byte_t*)mbs.c_str(), mbs.size());
				tmp[n] = 0;

				output = str_convert(CP_UTF8, (const char*)tmp);

				delete []tmp;
		}else
		{
				mbs = wcs_convert(this->GetEncodeCharset(), input);

				if(mbs == "")
				{
						output = L"invalid charset!";
						return FALSE;
				}

				n = ARSpace::AR_base64_decode(NULL, 0, (const ar_byte_t*)mbs.c_str(), mbs.size());

				if(n == 0)
				{
						output = L"invalid input!";
						return FALSE;
				}

				tmp = new byte[n + 1];

				n = ARSpace::AR_base64_decode(tmp, n, (const ar_byte_t*)mbs.c_str(), mbs.size());
				
				tmp[n] = 0;

				output = str_convert(CP_UTF8, (const char*)tmp);

				delete []tmp;
		}

		return TRUE;
}







BOOL		CEncodeConvert::hex_convert(const CString &input, CString &output)
{
		
		std::string mbs;
		char *tmp;

		AR_ASSERT(input.GetLength() > 0);
		if(input.GetLength() == 0)
		{
				output = L"empty input!";
				return FALSE;
		}

		mbs = wcs_convert(this->GetEncodeCharset(), input);

		if(mbs == "")
		{
				output = L"invalid charset!";
				return FALSE;
		}

		if(this->IsEncode())
		{
				size_t l = mbs.size();
				tmp = new char[l * 2 + 1];

				ar_int_t len = AR_data_to_hexstr((const ar_byte_t*)mbs.c_str(), l, tmp, l * 2 + 1, true);
				tmp[len] = '\0';

				output = str_convert(this->GetEncodeCharset(), tmp);
		}else
		{
				size_t l = mbs.size();
				tmp = new char[l * 2 + 1];
				ar_int_t n = AR_hexstr_to_data(mbs.c_str(), (ar_byte_t*)tmp, l * 2 + 1);
				if(n <= -1)
				{
						output = L"invalid input!";
						delete []tmp;
						return FALSE;
				}
				tmp[n] = 0;

				output = str_convert(this->GetEncodeCharset(), tmp);

		}

		delete []tmp;
		tmp = NULL;

		return TRUE;
		
}


BOOL					CEncodeConvert::url_convert(const CString &input, CString &output)
{
		arURI_t *uri;
		std::string utf8;
		arCodePage_t cp;
		arString_t *str;
		BOOL ret;

		if(input.GetLength() == 0)
		{
				output = L"empty input!";
				return FALSE;
		}

		str = AR_CreateString();
		ret = TRUE;
		switch(this->GetEncodeCharset())
		{
		case CP_UTF8:
				cp = AR_CP_UTF8;
				break;
		case 936:
				cp = AR_CP_GB2312;
				break;
		case 950:
				cp = AR_CP_BIG5;
				break;
		case CP_ACP:
		default:
				cp = AR_CP_ACP;
		}
		
		uri = AR_CreateURI(cp);

		if(this->IsEncode())
		{
				/*
				if(AR_SetURI(uri, input) != AR_S_YES)
				{
						output = L"input is not valid url!";
						ret = FALSE;
						goto END_POINT;
				}

				AR_GetEncodedURI(uri, str);
				*/
				if(AR_EncodeURLString(cp, input, str) != AR_S_YES)
				{
						output = L"input is not valid url!";
						ret = FALSE;
						goto END_POINT;
				}

				output = AR_GetStringCString(str);
		}else
		{
				if(AR_DecodeURLString(cp, input, str) != AR_S_YES)
				{
						output = L"input is not valid url!";
						ret = FALSE;
						goto END_POINT;
				}
				/*
				if(AR_SetEncodedURI(uri, input) != AR_S_YES)
				{
						output = L"input is not valid url!";
						ret = FALSE;
						goto END_POINT;
				}
				AR_GetURI(uri, str);
				*/
				output = AR_GetStringCString(str);
		}
END_POINT:
		AR_DestroyString(str);
		AR_DestroyURI(uri);
		return ret;


}

BOOL					CEncodeConvert::md5_convert(const CString &input, CString &output)
{
		std::string mbs;
		if(input.GetLength() == 0)
		{
				output = L"empty input!";
				return FALSE;
		}

		mbs = wcs_convert(this->GetEncodeCharset(), input);
		if(mbs.size() == 0)
		{
				output = L"invalid charset!";
				return FALSE;
		}

		MD5_CTX	ctx;
		MD5Init(&ctx);

		MD5Update(&ctx, (unsigned char*)mbs.c_str(), mbs.size());

		MD5Final(&ctx);
		char buf[33];
		AR_data_to_hexstr((const ar_byte_t*)ctx.digest, 16, buf, 33, true);
		buf[32] = 0;

		output = str_convert(CP_UTF8, buf);

		return TRUE;
}


BOOL					CEncodeConvert::sha1_convert(const CString &input, CString &output)
{
		std::string mbs;
		if(input.GetLength() == 0)
		{
				output = L"empty input!";
				return FALSE;
		}

		mbs = wcs_convert(this->GetEncodeCharset(), input);
		if(mbs.size() == 0)
		{
				output = L"invalid charset!";
				return FALSE;
		}

		unsigned char hash[20];
		char hex_str[41];
		sha1(hash, (unsigned char*)mbs.c_str(), mbs.size());
		
		ar_int_t l = AR_data_to_hexstr((const ar_byte_t*)hash, 20, hex_str, 41, true);
		hex_str[l] = '\0';
		

		output = str_convert(CP_UTF8, hex_str);

		return TRUE;
}


// CEncodeConvert message handlers

void CEncodeConvert::OnBnClickedButtonConvert()
{
		// TODO: Add your control notification handler code here
		CString input, output;
		BOOL is_valid = FALSE;
		m_input.GetWindowText(input);
		if(input.GetLength() == 0)
		{
				return;
		}

		switch(GetEncodeType())
		{
		case BASE64:
				is_valid = base64_convert(input, output);
				break;
		case ROT13:
				is_valid = rot13_convert(input, output);
				break;
		case HEX:
				is_valid = hex_convert(input, output);
				break;
		case URL:
				is_valid = url_convert(input, output);
				break;
		case MD5:
				is_valid = md5_convert(input, output);
				break;
		case SHA1:
				sha1_convert(input, output);
				break;
		}

		this->m_output.SetWindowText(output);

}


void CEncodeConvert::OnBnClickedRadioEncode()
{
		// TODO: Add your control notification handler code here
		CButton *btn_encode = (CButton*)this->GetDlgItem(IDC_RADIO_ENCODE);
		CButton *btn_decode = (CButton*)this->GetDlgItem(IDC_RADIO_DECODE);

		VERIFY(btn_encode != NULL && btn_decode != NULL);

		//btn_encode->SetCheck(TRUE);
		//btn_decode->SetCheck(FALSE);
		//btn_decode->SetState(BST_UNCHECKED);
		//btn_encode->SetState(BST_CHECKED);
}


void CEncodeConvert::OnBnClickedRadioDecode()
{
		// TODO: Add your control notification handler code here
		CButton *btn_encode = (CButton*)this->GetDlgItem(IDC_RADIO_ENCODE);
		CButton *btn_decode = (CButton*)this->GetDlgItem(IDC_RADIO_DECODE);

		VERIFY(btn_encode != NULL && btn_decode != NULL);

		//btn_encode->SetCheck(FALSE);
		//btn_decode->SetCheck(TRUE);
}




BOOL CEncodeConvert::OnInitDialog()
{
		CDialog::OnInitDialog();

		// TODO:  Add extra initialization here

		/*
		CRichEditCtrl *pedit = (CRichEditCtrl*)this->GetDlgItem(IDC_EDIT_STRING);
		pedit->SetFont(&m_font);
		pedit->LimitText(0xffffffff);
		
		
		pedit = (CRichEditCtrl*)this->GetDlgItem(IDC_EDIT_CODE);
		pedit->SetFont(&m_font);
		pedit->LimitText(0xffffffff);
		*/

		((CButton*)this->GetDlgItem(IDC_RADIO_ENCODE))->SetCheck(TRUE);

		((CButton*)this->GetDlgItem(IDC_RADIO_CS_UTF8))->SetCheck(TRUE);
		
		((CButton*)this->GetDlgItem(IDC_RADIO_ENCTYPE_BASE64))->SetCheck(TRUE);
		
		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}



