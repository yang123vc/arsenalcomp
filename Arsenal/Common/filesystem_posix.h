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


AR_NAMESPACE_BEGIN

#if !defined(PATH_MAX)
        #define PATH_MAX 1024
#endif
		
arStatus_t		AR_GetCurrentPath(arString_t *str)
{
        arStatus_t status;
        wchar_t *w_tmp;
        char tmp[PATH_MAX];
		AR_ASSERT(str != NULL);
        
        w_tmp = NULL;
        status = AR_S_YES;
        
        if(getcwd(tmp, PATH_MAX) == NULL)
        {
                status = AR_E_FAIL;
                goto END_POINT;
        }
        
        w_tmp = AR_str_to_wcs(AR_CP_ACP, tmp, AR_strlen(tmp));
        
        if(w_tmp == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        status = AR_SetString(str, w_tmp);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        if(AR_GetStringChar(str, AR_GetStringLength(str) - 1) != L'/')
        {
                status = AR_AppendCharToString(str, L'/');
        }
       
        
        
END_POINT:
        if(w_tmp)
        {
                AR_DEL(w_tmp);
                w_tmp = NULL;
        }
        return status;
}


arStatus_t		AR_GetHomePath(arString_t *str)
{
        char *path;
        wchar_t *w_tmp;
        arStatus_t status;
        struct passwd *pwd;
        AR_ASSERT(str != NULL);
        
        status = AR_S_YES;
        path = NULL;
        w_tmp = NULL;
        pwd = getpwuid(getuid());
        
        if(pwd != NULL)
        {
                path = pwd->pw_dir;  
        }else
        {
                pwd = getpwuid(geteuid());
                if(pwd != NULL)
                {
                        path = pwd->pw_dir;
                }else
                {
                        path = NULL;
                }
        }
        
        if(path == NULL)
        {
                status = AR_getenv(L"HOME", str);
                
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
        }else
        {
                w_tmp = AR_str_to_wcs(AR_CP_ACP, path, AR_strlen(path));
                
                if(w_tmp == NULL)
                {
                        status = AR_E_BADENCCONV;
                        goto END_POINT;
                }
                
                status = AR_SetString(str, w_tmp);
                
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
        }
        
        if(AR_GetStringChar(str, AR_GetStringLength(str) - 1) != L'/')
        {
                status = AR_AppendCharToString(str, L'/');
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
        }
       
END_POINT:
        if(w_tmp != NULL)
        {
                AR_DEL(w_tmp);
                w_tmp = NULL;
        }
                
        return status;
}

arStatus_t		AR_GetTempPath(arString_t *str)
{
        arStatus_t status;
        AR_ASSERT(str != NULL);

        status = AR_S_YES;
        
        status = AR_getenv(L"TMPDIR", str);
        
        if(status != AR_S_YES)
        {
                status = AR_SetString(str, L"/tmp/");
                
                if(status != AR_S_YES)
                {
                        return status;
                }
        }
        
        
        if(AR_GetStringChar(str, AR_GetStringLength(str) - 1) != L'/')
        {
                status = AR_AppendCharToString(str, L'/');
                if(status != AR_S_YES)
                {
                        return status;
                }
        }
        
        return AR_S_YES;
}


arStatus_t		AR_GetNullPath(arString_t *str)
{
		AR_ASSERT(str != NULL);
		return AR_SetString(str, L"/dev/null");
}

arStatus_t		AR_GetExpandPath(const wchar_t *path, arString_t *expanded_path)
{
        arStatus_t status;
        const wchar_t *b,*e;
        arString_t *tmp, *var;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        b = path; 
        e = path + AR_wcslen(path);
        AR_ClearString(expanded_path);
        
        tmp = AR_CreateString();
        var = AR_CreateString();
        if(tmp == NULL || var == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        if(b < e && *b == L'~')
        {
                ++b;
                
                if(b < e && *b == L'/')
                {
                        status = AR_GetHomePath(tmp);
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                       
                        status = AR_AppendString(expanded_path, AR_GetStringCString(tmp));
                        
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                        ++b;
                }else
                {
                        status = AR_AppendString(expanded_path, L"~");
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                }
        }
        
        
        while(b < e)
        {
                if(*b == L'$')
                {
                        AR_ClearString(var);
                        ++b;
                        if(b < e && *b == L'{')
                        {
                                ++b;
                                while(b < e && *b != L'}')
                                {
                                        status = AR_AppendCharToString(var, *b);
                                        if(status != AR_S_YES)
                                        {
                                                goto END_POINT;
                                        }
                                        
                                        ++b;
                                }
                                
                                if(b < e)
                                {
                                        ++b;
                                }
                        }else
                        {
                                while(b < e && (AR_iswalnum(*b) || *b == L'_'))
                                {
                                        status = AR_AppendCharToString(var, *b);
                                        if(status != AR_S_YES)
                                        {
                                                goto END_POINT;
                                        }
                                        ++b;
                                }
                                
                        }
                        
                        status = AR_getenv(AR_GetStringCString(var), tmp);
                        
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                        
                        status = AR_AppendString(expanded_path, AR_GetStringCString(tmp));
                        
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                }else
                {
                        status = AR_AppendCharToString(expanded_path, *b);
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                        ++b;
                }
        }
        
        
        status = AR_SetString(tmp, AR_GetStringCString(expanded_path));
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        AR_ClearString(expanded_path);
        
        b = AR_GetStringCString(tmp);
        e = b + AR_wcslen(b);
        
        while(b < e)
        {
                if(*b == L'/')
                {
                        if(b + 1 < e && *(b + 1) == L'/')
                        {
                                b++;
                                continue;
                        }
                        
                }
                
                status = AR_AppendCharToString(expanded_path, *b);
                        
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
                
                ++b;
        }
                                                 
END_POINT:
        if(tmp != NULL)
        {
                AR_DestroyString(tmp);
                tmp = NULL;
        }
        
        if(var != NULL)
        {
                AR_DestroyString(var);
                var = NULL;
        }
        
        return status;
}



/*********************************Path iterator******************/

struct __arsenal_path_iterator_tag
{
		bool_t				isdone;
        DIR                 *hdl;
		arString_t			*current;
		wchar_t				*path;
};

arPathIter_t*	AR_CreatePathIterator(const wchar_t *path)
{
		arPathIter_t *iter;
        char *s_tmp;
		arStatus_t status;
        arString_t *expanded_path;
		AR_ASSERT(path != NULL);
        
		s_tmp = NULL;
		iter = NULL;
        expanded_path = NULL;
		status = AR_S_YES;
        
        expanded_path = AR_CreateString();
        if(expanded_path == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        status = AR_GetExpandPath(path, expanded_path);

        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
		if(AR_GetStringChar(expanded_path, AR_GetStringLength(expanded_path) - 1) != L'/')
		{
				status = AR_AppendString(expanded_path, L"/");
				if(status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

        s_tmp = AR_wcs_to_str(AR_CP_ACP, AR_GetStringCString(expanded_path), AR_GetStringLength(expanded_path));
        if(s_tmp == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
		iter = AR_NEW0(arPathIter_t);
		
        if(iter == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
		iter->hdl = opendir(s_tmp);
		if(iter->hdl == NULL)
		{
				status = AR_E_FAIL;
				goto END_POINT;
		}
        
		iter->isdone = false;
		iter->path = AR_wcsdup(AR_GetStringCString(expanded_path));
		iter->current = AR_CreateString();
		
		if(iter->path == NULL || iter->current == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}
        
        status = AR_PathIteratorNext(iter);
        
END_POINT:
		if(s_tmp != NULL)
		{
				AR_DEL(s_tmp);
				s_tmp = NULL;
		}
        
        if(expanded_path != NULL)
        {
                AR_DestroyString(expanded_path);
                expanded_path = NULL;
        }
        
		if(status == AR_S_YES || status == AR_E_NOMORE)
		{
                
		}else
		{
				if(iter != NULL)
				{
						AR_DestroyPathIterator(iter);
						iter = NULL;
				}
		}
		
        
		return iter;
}


void			AR_DestroyPathIterator(arPathIter_t *iter)
{
		AR_ASSERT(iter != NULL);
		if(iter->hdl != NULL)
		{
				closedir(iter->hdl);
				iter->hdl = NULL;
		}
        
		if(iter->current)
		{
				AR_DestroyString(iter->current);
				iter->current = NULL;
		}
        
		if(iter->path)
		{
				AR_DEL(iter->path);
				iter->path = NULL;
		}
        
		AR_DEL(iter);
		iter = NULL;
}



const wchar_t*	AR_PathIteratorCurrent(const arPathIter_t *iter)
{
		AR_ASSERT(iter != NULL);
		AR_ASSERT(iter->current != NULL);
		return AR_GetStringCString(iter->current);
}



arStatus_t		AR_PathIteratorNext(arPathIter_t *iter)
{
		arStatus_t status;
		AR_ASSERT(iter != NULL);
        
		status = AR_S_YES;
        
		do{
                struct dirent* entry;
				
                AR_ClearString(iter->current);
				
                entry = readdir(iter->hdl);
                
				if(entry != NULL)
				{
                        wchar_t *tmp = AR_str_to_wcs(AR_CP_ACP, entry->d_name, AR_strlen(entry->d_name));

                        if(tmp == NULL)
                        {
                                status = AR_E_BADENCCONV;
                                iter->isdone = true;
                        }
						
                        status = AR_SetString(iter->current, tmp);
                        
                        if(tmp)
                        {
                                AR_DEL(tmp);
                                tmp = NULL;
                        }
                        
                        if(status != AR_S_YES)
                        {
                                iter->isdone = true;
                        }
				}else
				{
                        status = AR_E_NOMORE;
                        iter->isdone = true;
				}
                
		}while(status == AR_S_YES && (AR_CompStringWithWcs(iter->current, L".") == 0 || AR_CompStringWithWcs(iter->current, L"..") == 0));
        
		return status;
}


bool_t		AR_PathIteratorIsDone(const arPathIter_t *iter)
{
		AR_ASSERT(iter != NULL);
		return iter->isdone;
}

const wchar_t*  AR_PathIteratorPath(const arPathIter_t *iter)
{
        AR_ASSERT(iter != NULL && iter->path != NULL);
        return iter->path;
}
/************************************************************File*************************************************************/


FILE*	AR_open_file(const wchar_t *path, const wchar_t *mode)
{
        FILE    *file;
        char    *str_path;
        char     *str_mode;
		AR_ASSERT(path != NULL && mode != NULL);

		str_path = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
		str_mode = AR_wcs_to_str(AR_CP_UTF8, mode, AR_wcslen(mode));

		file = NULL;

		if(str_path && str_mode)
		{
				file = fopen(str_path, str_mode);
		}else
		{
				AR_error(AR_ERR_WARNING, L"Failed to convert path('%ls') and mode('%ls') to utf8 strings\r\n", path, mode);
		}

        if(str_path)
        {
            AR_DEL(str_path);
            str_path = NULL;
        }

        if(str_mode)
        {
            AR_DEL(str_mode);
            str_mode = NULL;
        }

        return file;
}


void	AR_close_file(FILE *f)
{
		AR_ASSERT(f != NULL);
		fclose(f);
}


AR_NAMESPACE_END

