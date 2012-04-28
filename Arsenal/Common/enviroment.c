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

/******************************************************Print**************************************************************/

AR_NAMESPACE_BEGIN



#if defined(OS_FAMILY_WINDOWS)

arStatus_t		AR_getenv(const wchar_t *key, arString_t *val)
{
		arStatus_t status;
		DWORD len;
		wchar_t *tmp;
		AR_ASSERT(key != NULL && val != NULL);

		AR_ClearString(val);
		status = AR_S_YES;
		tmp = NULL;
		len = GetEnvironmentVariableW(key, 0, 0);
		
		if(len <= 0)
		{
				status = AR_E_NOTFOUND;
				goto END_POINT;
		}
		
		tmp = AR_NEWARR(wchar_t, len + 1);

		if(tmp == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}
		len = GetEnvironmentVariableW(key, tmp, len);

		if(len == 0)
		{
				status = AR_E_INVAL;
				goto END_POINT;
		}

		status = AR_SetString(val, tmp);
		
END_POINT:
		if(tmp)
		{
				AR_DEL(tmp);
				tmp = NULL;
		}
		return status;
}


arStatus_t		AR_setenv(const wchar_t *key, const wchar_t *val)
{
		AR_ASSERT(key != NULL && val != NULL);
		
		if(SetEnvironmentVariableW(key, val) == 0)
		{
				return AR_E_FAIL;
		}else
		{
				return AR_S_YES;
		}
}

arStatus_t		AR_hasenv(const wchar_t *key)
{
		AR_ASSERT(key != NULL);
		return GetEnvironmentVariableW(key, 0, 0) > 0 ? AR_S_YES : AR_S_NO;
}


#elif defined(OS_FAMILY_UNIX)

#else
		#error "Unknown OS!"
#endif




AR_NAMESPACE_END

