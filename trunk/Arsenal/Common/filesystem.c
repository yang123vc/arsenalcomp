/*
 * The Arsenal Library
 * Copyright (c) 2009-2012 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#include "common.h"



AR_NAMESPACE_BEGIN

		
#if defined(OS_FAMILY_WINDOWS)

arStatus_t		AR_GetCurrentPath(arString_t *str)
{
		arStatus_t status;
		wchar_t *tmp;
		DWORD l,n;
		AR_ASSERT(str != NULL);

		status = AR_S_YES;
		tmp = NULL;
		l = GetCurrentDirectoryW(0, NULL);
		
		if (l <= 0)
		{
				status = AR_E_FAIL;
				goto END_POINT;
		}

		tmp = AR_NEWARR(wchar_t, l + 3);
		
		if(tmp == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}

		n = GetCurrentDirectoryW(l + 3, tmp);

		if(n <= 0 || n > l)
		{
				AR_DEL(tmp);
				tmp = NULL;
				status = AR_E_FAIL;
				goto END_POINT;
		}
		
		if(tmp[n - 1] != L'\\')
		{
				tmp[n] = L'\\';
				tmp[n+1] = L'\0';
		}

		status = AR_SetString(str, tmp);


END_POINT:
		if(tmp)
		{
				AR_DEL(tmp);
				tmp = NULL;
		}

		return status;
	
}

arStatus_t		AR_GetHomePath(arString_t *str)
{
		arStatus_t status;
		arString_t *tmp;
		AR_ASSERT(str != NULL);

		tmp = NULL;
		status = AR_S_YES;
		AR_ClearString(str);

		tmp = AR_CreateString();

		status = AR_getenv(L"HOMEDRIVE", str);
		
		if(status != AR_S_YES)
		{
				goto END_POINT;
		}
		
		status = AR_getenv(L"HOMEPATH", tmp);

		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

		if(AR_GetStringChar(tmp, AR_GetStringLength(str) - 1) != L'\\')
		{
				status = AR_AppendCharToString(tmp, L'\\');
				if(status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		status = AR_AppendString(str, AR_GetStringCString(tmp));

		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(tmp)
		{
				AR_DestroyString(tmp);
				tmp = NULL;
		}

		return status;
}

arStatus_t		AR_GetTempPath(arString_t *str)
{
		wchar_t tmp[MAX_PATH + 10];
		arStatus_t status;
		DWORD n;
		AR_ASSERT(str != NULL);

		status = AR_S_YES;
		n = GetTempPathW(MAX_PATH, tmp);
		if(n <= 0)
		{
				return AR_E_FAIL;
		}

		n = GetLongPathNameW(tmp, tmp, n);

		if(n <= 0)
		{
				return AR_E_FAIL;
		}

		if(tmp[n - 1] != L'\\')
		{
				tmp[n] = L'\\';
				tmp[n+1] = L'\0';
		}

		return AR_SetString(str, tmp);

}

arStatus_t		AR_GetNullPath(arString_t *str)
{
		AR_ASSERT(str != NULL);
		return AR_SetString(str, L"NUL:");
}

arStatus_t		AR_GetExpandPath(const wchar_t *path, arString_t *expanded_path)
{
		wchar_t tmp[MAX_PATH + 10];
		arStatus_t status;
		DWORD n;
		AR_ASSERT(path != NULL && expanded_path != NULL);

		status = AR_S_YES;
		n = ExpandEnvironmentStringsW(path, tmp, MAX_PATH);
		if(n <= 0 || n > MAX_PATH - 1)
		{
				return AR_SetString(expanded_path, path);
		}else
		{
				return AR_SetString(expanded_path, tmp);
		}
}



#elif defined(OS_FAMILY_UNIX)
		
		
arStatus_t		AR_GetCurrentPath(arString_t *str)
{
		
	
}

arStatus_t		AR_GetHomePath(arString_t *str)
{
		
}

arStatus_t		AR_GetTempPath(arString_t *str)
{
	

}

arStatus_t		AR_GetNullPath(arString_t *str)
{
		AR_ASSERT(str != NULL);
		return AR_SetString(str, L"/dev/null");
}

arStatus_t		AR_GetExpandPath(const wchar_t *path, arString_t *expanded_path)
{
		
}



#else
		#error "Unknown OS!"
#endif


AR_NAMESPACE_END

