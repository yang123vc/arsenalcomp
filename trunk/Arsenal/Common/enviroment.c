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

arStatus_t		AR_getenv(const wchar_t *key, arString_t *val)
{
		arStatus_t status;

        char *s_val, *s_key;

        wchar_t *tmp;
		AR_ASSERT(key != NULL && val != NULL);

        status = AR_S_YES;
        s_key = NULL;
        s_val = NULL;
        tmp = NULL;
        s_key = AR_wcs_to_str(AR_CP_ACP, key, AR_wcslen(key));
        
        if(s_key == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        s_val = getenv(s_key);
        if(s_val == NULL)
        {
                status = AR_E_NOTFOUND;
                goto END_POINT;
        }
        
        tmp = AR_str_to_wcs(AR_CP_ACP, s_val, AR_strlen(s_val));
        
        if(tmp == NULL)
        {
                status = AR_E_NOTFOUND;
                goto END_POINT;
        }
        
        status = AR_SetString(val, tmp);

        
END_POINT:
        if(s_key)
        {
                AR_DEL(s_key);
                s_key = NULL;
        }
        
        if(tmp)
        {
                AR_DEL(tmp);
                tmp = NULL;
        }
        
		
		return status;
}


arStatus_t		AR_setenv(const wchar_t *key, const wchar_t *val)
{
        arStatus_t status;
        char *s_key, *s_val;
		AR_ASSERT(key != NULL && val != NULL);
        
        status = AR_S_YES;
        s_key = NULL;
        s_val = NULL;
        
        s_key = AR_wcs_to_str(AR_CP_ACP, key, AR_wcslen(key));
        s_val = AR_wcs_to_str(AR_CP_ACP, val, AR_wcslen(val));
        
        if(s_key == NULL || s_val == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        
        if(setenv(s_key, s_val, 1) != 0)
        {
                status = AR_E_FAIL;
                goto END_POINT;
        }
END_POINT:
        if(s_key)
        {
                AR_DEL(s_key);
                s_key = NULL;
        }
        
        if(s_val)
        {
                AR_DEL(s_val);
                s_val = NULL;
        }
        
        return status;
		
}


arStatus_t		AR_hasenv(const wchar_t *key)
{
        arStatus_t status;
        arString_t *str;
		AR_ASSERT(key != NULL);
        
        str = AR_CreateString();
        
        if(str == NULL)
        {
                return AR_E_NOMEM;
        }
        
        status = AR_getenv(key, str);
        AR_DestroyString(str);
        str = NULL;
        if(status == AR_S_YES)
        {
                return AR_S_YES;
        }else
        {
                return AR_S_NO;
        }
}

#else
		#error "Unknown OS!"
#endif




AR_NAMESPACE_END

