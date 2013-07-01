/*
 * The Arsenal Library
 * Copyright (c) 2009-2013 by Solidus
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express
 * or implied warranty.
 *
 */

#ifndef ARSENAL_filesystem_path_posix_apple_h
#define ARSENAL_filesystem_path_posix_apple_h


arStatus_t fixAliasFileExtendAttributeOnCopy(const wchar_t *src, const wchar_t *dest)
{
        arStatus_t status;
        char *s,*d, *data;
        ssize_t ret;
        AR_ASSERT(src != NULL && dest != NULL);
        
        status = AR_S_YES;
        s = AR_wcs_to_str(AR_CP_UTF8, src, AR_wcslen(src));
        d = AR_wcs_to_str(AR_CP_UTF8, dest, AR_wcslen(dest));
        data = AR_NEWARR(char, 1024);
        
        if(s == NULL || d == NULL || data == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        
        ret = getxattr(s, "com.apple.FinderInfo", (void*)data, 4096, 0, XATTR_NOFOLLOW);
        
        if(ret < 0)
        {
                int err_code = errno;
                status = __map_last_error(err_code);
                goto END_POINT;
        }
        
        
        if(setxattr(d, "com.apple.FinderInfo", (void*)data, ret, 0, XATTR_NOFOLLOW) != 0)
        {
                int err_code = errno;
                status = __map_last_error(err_code);
                AR_error(AR_ERR_WARNING, L"failed to set file : '%ls' extend attr\r\n", dest);
                goto END_POINT;
        }
        
        
END_POINT:
        if(data)
        {
                AR_DEL(data);
                data = NULL;
        }
        
        if(s)
        {
                AR_DEL(s);
                s = NULL;
        }
        
        if(d)
        {
                AR_DEL(d);
                d = NULL;
        }

        return status;
}



#endif
