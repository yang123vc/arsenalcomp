/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express
 * or implied warranty.
 *
 */


#include "platform.h"

#if defined(OS_FAMILY_UNIX)
		#include <iconv.h>
#endif

#include "common.h"



AR_NAMESPACE_BEGIN





#if defined(OS_FAMILY_WINDOWS)


static UINT __get_codepage_for_winapi(arCodePage_t cp)
{
		switch(cp)
		{
		default: /*CP_ACP*/
				return CP_ACP;
		case AR_CP_UTF8:
				return CP_UTF8;
		case AR_CP_GB2312:
				return 936;
		case AR_CP_GB18030:
				return 54936;
		case AR_CP_BIG5:
				return 950;
		}
}



ar_int_t					AR_str_to_wcs_buf(arCodePage_t cp, const char *acp, size_t n, wchar_t *out, size_t out_len)
{
		int len;
		const UINT win_cp = __get_codepage_for_winapi(cp);
		AR_ASSERT(acp != NULL);

		if(n == 0)
		{
				return 0;
		}

		len = MultiByteToWideChar(win_cp, 0, acp, (int)n, 0, 0);

		if(len <= 0)
		{
				return -1;
		}

		if(out_len == 0 || out == NULL)
		{
				return len;
		}

		if((int)out_len < len)
		{
				return -1;
		}

		len = MultiByteToWideChar(win_cp, 0, acp, (int)n, out, (int)out_len);
		if(len == 0)
		{
				return -1;
		}else
		{
				return len;
		}
}


ar_int_t					AR_wcs_to_str_buf(arCodePage_t cp, const wchar_t *input, size_t n, char *out, size_t out_len)
{
		int len;
		const UINT win_cp = __get_codepage_for_winapi(cp);
		AR_ASSERT(input != NULL);

		if(n == 0)
		{
				return 0;
		}

		len = WideCharToMultiByte(win_cp, 0, input, (int)n, 0, 0, NULL, NULL);

		if(len == 0)
		{
				return -1;
		}

		if(out == NULL || out_len == 0)
		{
				return len;
		}

		if((int)out_len < len)
		{
				return 0;
		}

		len = WideCharToMultiByte(win_cp, 0, input, (int)n, out, (int)out_len, NULL, NULL);

		if(len <= 0)
		{
				return -1;
		}else
		{
				return len;
		}
}


char*					AR_wcs_to_str(arCodePage_t cp, const wchar_t *input, size_t in_n)
{
		char *ret;
		int n;
		const UINT win_cp = __get_codepage_for_winapi(cp);
		AR_ASSERT(input != NULL);
		n = (int)in_n;

		if(n == 0)
		{
				return AR_strdup("");
		}else
		{
				int len = WideCharToMultiByte(win_cp, 0, input, n, 0, 0, NULL, NULL);
				if(len == 0)
				{
					
						return NULL;
				}

				ret = AR_NEWARR(char, len + 1);

				if(ret == NULL)
				{
					
						return NULL;
				}

				len = WideCharToMultiByte(win_cp, 0, input, n, ret, len, NULL, NULL);
				
				if(len == 0)
				{
					
						AR_DEL(ret);
						ret = NULL;
				}else
				{
						ret[len] = 0;
				}

				return ret;
		}
}

wchar_t*				AR_str_to_wcs(arCodePage_t cp, const char *input, size_t in_n)
{
		wchar_t *ret;
		const UINT win_cp = __get_codepage_for_winapi(cp);
		AR_ASSERT(input != NULL);
		if(in_n == 0)
		{
				return AR_wcsdup(L"");
		}else
		{
				int len = MultiByteToWideChar(win_cp, 0, input, (int)in_n, 0, 0);
				if(len == 0)
				{
						return NULL;

				}

				ret = AR_NEWARR(wchar_t, len + 1);

				if(ret == NULL)
				{
					
						return NULL;
				}


				len = MultiByteToWideChar(win_cp, 0, input, (int)in_n, ret, len);
				if(len == 0)
				{
					
						AR_DEL(ret);
						ret = NULL;
				}else
				{
						ret[len] = 0;
				}

				return ret;
		}
}




#elif defined(OS_FAMILY_UNIX)





const char *__get_locale_str_for_iconv(arCodePage_t cp)
{
		switch(cp)
		{
		default: 
				return "";
		case AR_CP_UTF8:
				return "UTF-8";
		case AR_CP_GB2312:
				return "GB2312";
		case AR_CP_GB18030:
				return "GB18030";
		case AR_CP_BIG5:
				return "BIG5";
		}
}


#if(OS_TYPE ==  OS_IOS) || (OS_TYPE == OS_MAC_OS_X)


		#if defined(ARCH_LITTLE_ENDIAN)
				#define UNICODE_ENCODING_NAME	"UCS-4LE"
		#else
				#define UNICODE_ENCODING_NAME	"UCS-4"
		#endif

#else
		#define UNICODE_ENCODING_NAME	"wchar_t"
#endif






ar_int_t					AR_str_to_wcs_buf(arCodePage_t cp, const char *acp, size_t n, wchar_t *out, size_t out_len)
{
		size_t len;
		size_t ret;
		wchar_t *wstr;
		AR_ASSERT(acp != NULL);
        
        if(n == 0)
        {
                return 0;
        }
        
		wstr = AR_str_to_wcs(cp, acp, n);
		if(wstr == NULL)
		{
				ret = -1;
				goto CLEAN_POINT;
		}

		len = AR_wcslen(wstr);
        
        if(len == 0)
        {
                ret = -1;
                goto CLEAN_POINT;
        }

		if(out == NULL || out_len == 0)
		{
				ret = len;
				goto CLEAN_POINT;
		}

		if(out_len < len)
		{
				ret = -1;
				goto CLEAN_POINT;
		}

		AR_wcsncpy(out, wstr, len);
		ret = len;
        
CLEAN_POINT:
		if(wstr)
		{
				AR_DEL(wstr);
				wstr = NULL;
		}
		return ret;
}

ar_int_t					AR_wcs_to_str_buf(arCodePage_t cp, const wchar_t *input, size_t n, char *out, size_t out_len)
{
		size_t len;
		size_t ret;
		char *str;
		AR_ASSERT(input != NULL);
        
        if(n == 0)
        {
                return 0;
        }
        
		str = AR_wcs_to_str(cp, input, n);

		if(str == NULL)
		{
				ret = -1;
				goto CLEAN_POINT;
		}

		len = AR_strlen(str);
        
        if(len == 0)
        {
                ret = -1;
                goto CLEAN_POINT;
        }

		if(out == NULL || out_len == 0)
		{
				ret = len;
				goto CLEAN_POINT;
		}

		if(out_len < len)
		{
				ret = -1;
				goto CLEAN_POINT;
		}

		AR_strncpy(out, str, len);
		ret = len;

CLEAN_POINT:
		if(str)
		{
				AR_DEL(str);
				str = NULL;
		}
		return ret;

}




wchar_t*				AR_str_to_wcs(arCodePage_t cp, const char *input, size_t in_n)
{

        char   *out    = NULL;
        size_t out_len = 0;

        char   *inbuf  = NULL;
        char   *outbuf = NULL;
        size_t inleft;
        size_t outleft;
        iconv_t cd;
		const char *cp_iconv = __get_locale_str_for_iconv(cp);
		ar_bool_t	is_ok = true;

		AR_ASSERT(input != NULL);

		if(in_n == 0)
        {
				return AR_wcsdup(L"");
        }

        out_len = sizeof(wchar_t) * (in_n + 1);
        out = (char*)AR_NEWARR0(wchar_t , in_n + 1);

		if(out == NULL)
		{
				is_ok = false;
				cd = NULL;
				goto CLEAN_POINT;
		}

		cd = iconv_open(UNICODE_ENCODING_NAME, cp_iconv);
		//cd = iconv_open("wchar_t", cp_iconv);


        if(cd == (iconv_t)-1)
        {
                int err_code = errno;
				AR_error(AR_ERR_WARNING, L"iconv_open failed : %hs\r\n", strerror(err_code));
                
				is_ok = false;
				cd = NULL;
				goto CLEAN_POINT;
        }

        inbuf = (char*)input;
        outbuf = out;
        inleft = in_n;
        outleft = out_len;

        if(iconv(cd, &inbuf, &inleft, &outbuf, &outleft) == (size_t)-1)
        {
                int err_code = errno;
				AR_error(AR_ERR_WARNING, L"iconv failed : %hs\r\n", strerror(err_code));
				is_ok = false;
				goto CLEAN_POINT;
        }

CLEAN_POINT:
		if(cd)
		{
				iconv_close(cd);
				cd = NULL;
		}

		if(!is_ok)
		{
				if(out)
				{
						AR_DEL(out);
						out = NULL;
				}
		}

        return (wchar_t*)out;
}



char*					AR_wcs_to_str(arCodePage_t cp, const wchar_t *input, size_t in_n)
{
        char   *in     = NULL;
        size_t in_len;
        size_t len ;
        char   *out    = NULL;
        size_t out_len = 0;

        iconv_t cd  = NULL;
        ar_bool_t is_ok;
		const char *cp_iconv = __get_locale_str_for_iconv(cp);
        AR_ASSERT(input != NULL);

        in = (char*)input;
        len = in_n;

        if(len == 0)
        {
				return  AR_strdup("");
        }

        is_ok = true;
        in_len = len * sizeof(wchar_t);

        out_len = (len + 1) * 6;
		out = AR_NEWARR0(char, out_len);

		if(out == NULL)
		{
				is_ok = false;
				cd = NULL;
				goto CLEAN_POINT;
		}


        cd = iconv_open(cp_iconv,  UNICODE_ENCODING_NAME);

        if(cd == (iconv_t)-1)
        {
				int err_code = errno;
				AR_error(AR_ERR_WARNING, L"iconv_open failed : %hs\r\n", strerror(err_code));

				is_ok = false;
				cd = NULL;
				goto CLEAN_POINT;
        }

        {
            char   *inbuf  = in;
            char   *outbuf = out;
            size_t inleft  = in_len;
            size_t outleft = out_len;

            if(iconv(cd, &inbuf, &inleft, &outbuf, &outleft) != 0)
            {
                    int err_code = errno;
                    AR_error(AR_ERR_WARNING, L"iconv failed : %hs\r\n", strerror(err_code));

					is_ok = false;
					goto CLEAN_POINT;
            }
        }

CLEAN_POINT:

        if(cd)
        {
            iconv_close(cd);
            cd = NULL;
        }

        if(!is_ok && out)
        {
				AR_DEL(out);
				out = NULL;
        }

        return out;

}






#else

#error Unknown platform

#endif





AR_NAMESPACE_END

