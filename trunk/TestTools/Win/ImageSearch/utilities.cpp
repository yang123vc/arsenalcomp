#include "utilities.h"


arStatus_t  UTIL_GetAbsolutePath(const wchar_t *path, std::wstring &ret)
{
		arStatus_t ar_status;
		arString_t *tmp = NULL;
		AR_ASSERT(path != NULL);
		ar_status = AR_S_YES;
		tmp = AR_CreateString();

		if(tmp == NULL)
		{
				AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
				goto END_POINT;
		}

		ar_status = AR_GetExpandRealPath(path, tmp);
		if(ar_status != AR_S_YES)
		{
				AR_error(AR_ERR_WARNING, L"failed to get real : %ls\r\n", path);
				goto END_POINT;
		}

		ret = AR_CSTR(tmp);

END_POINT:
		if(tmp)
		{
				AR_DestroyString(tmp);
				tmp = NULL;
		}

		return ar_status;
}




std::wstring UTIL_GetModulePath()
{
		std::wstring path;
		wchar_t buf[MAX_PATH * 2];
		DWORD len = 0;

		len = ::GetModuleFileName(GetModuleHandle(NULL), buf, MAX_PATH * 2);

		if(len <= 0)
		{
				AR_error(AR_ERR_WARNING, L"failed to get module path : %hs\r\n", AR_FUNC_NAME);
				return L"";
		}
		
		buf[len] = L'\0';

		wchar_t *p = (wchar_t*)AR_reverse_wcschr(buf, len, L'\\');

		if(p)
		{
				*p = L'\0';
		}

		UTIL_GetAbsolutePath(buf, path);
		return path;
}





std::string		UTIL_StringConvert(arCodePage_t cp, const wchar_t *wcs)
{

		AR_ASSERT(cp < AR_CP_MAX);
		AR_ASSERT(wcs != NULL);

		std::string ret = "";

		char *s = AR_wcs_to_str(cp, wcs, AR_wcslen(wcs));

		if(s == NULL)
		{
				AR_error(AR_ERR_WARNING, L"failed to convert '%ls' to string\r\n", wcs);
		}else
		{
				ret = s;
				AR_DEL(s);
				s = NULL;
		}
		return ret;
}


std::string		UTIL_StringConvert(arCodePage_t cp, const std::wstring &wstr)
{
		AR_ASSERT(cp < AR_CP_MAX);
		return UTIL_StringConvert(cp, wstr.c_str());
}

std::wstring	UTIL_StringConvert(arCodePage_t cp, const char *s)
{
		AR_ASSERT(cp < AR_CP_MAX);
		AR_ASSERT(s != NULL);
		std::wstring ret = L"";
		const wchar_t *wcs = AR_str_to_wcs(cp, s, AR_strlen(s));
		if(wcs == NULL)
		{
				AR_error(AR_ERR_WARNING, L"failed to convert '%hs' to wcstring\r\n", s);

		}else
		{
				ret = wcs;
				AR_DEL(wcs);
				wcs = NULL;
		}

		return ret;
}

std::wstring	UTIL_StringConvert(arCodePage_t cp, const std::string &str)
{
		AR_ASSERT(cp < AR_CP_MAX);
		return UTIL_StringConvert(cp, str.c_str());
}
