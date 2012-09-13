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

#ifndef __ARSENAL_COMMON_FILESYSTEM_POSIX_H__
#define __ARSENAL_COMMON_FILESYSTEM_POSIX_H__

AR_NAMESPACE_BEGIN



#if !defined(PATH_MAX)
        #define PATH_MAX 1024
#endif


static arStatus_t       __map_last_error(int errcode)
{
        
        switch(errcode)
        {
				case 0:
						return AR_S_YES;
                case ERANGE:
                        return AR_E_RANGE;
                case EIO:
                        return AR_E_IO;
                case EPERM:
                        return AR_E_PERM;
                case EACCES:
                        return AR_E_ACCES;
                case ENOENT:
                        return AR_E_NOTFOUND;
                case ENOTDIR:
                        return AR_E_NOTDIR;
                case EISDIR:
                        return AR_E_ISDIR;
                case EROFS:
                        return AR_E_READONLY;
                case EEXIST:
                        return AR_E_EXISTED;
                case ENOSPC:
                case EDQUOT:
                        return AR_E_NOTENOUGH;
                case ENOTEMPTY:
                        return AR_E_NOTEMPTY;
                case ENAMETOOLONG:
                        return AR_E_PATH;
                case ENOMEM:
                         return AR_E_NOMEM;
				case EINTR:
                        return AR_E_INTR;
                case EAGAIN:
                        return AR_E_WOULDBLOCK;
                case ENFILE:
                case EMFILE:
                default:
                        AR_error(AR_ERR_WARNING, L"file error : '%hs'\r\n", strerror(errno));
                        return AR_E_FILE;
        }
}


		
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
                int err_code = errno;
                status = __map_last_error(err_code);
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


/*****************************************************************************************/
static arStatus_t      __insert_item(arList_t *lst, const wchar_t *item)
{
        wchar_t *t;
        AR_ASSERT(lst != NULL && item != NULL);
        t = AR_wcsdup(item);
        if(t == NULL)
        {
                return AR_E_NOMEM;
        }
        return AR_PushListBack(lst, (void*)t);
}


static arStatus_t      __split_path(arList_t *lst, const wchar_t *path)
{
        arStatus_t status;
        const wchar_t *p, *b;
        AR_ASSERT(lst != NULL && path != NULL);
        status = AR_S_YES;
        p = path;
        
        while(*p != L'\0')
        {
                p = AR_wcstrim(p, L"/");
                //AR_ASSERT(*p != L'/');
                b = p;
                while(*p != L'\0' && *p != L'/')
                {
                        ++p;
                }
                
                if(p - b > 0)
                {
                        wchar_t *item = AR_wcsndup(b, p - b);
                        if(item == NULL)
                        {
                                status = AR_E_NOMEM;
                                goto END_POINT;
                        }
                        status = AR_PushListBack(lst, (void*)item);
                        
                        if(status != AR_S_YES)
                        {
                                AR_DEL(item);
                                goto END_POINT;
                        }
                }
                if(*p != L'\0')
                {
                        ++p;
                }
        }
        
END_POINT:
        return status;
        
}


arStatus_t		AR_GetRealPath(const wchar_t *path, arString_t *full_path)
{
        arList_t        *lst;
        arList_t        *final_list;
        arListNode_t    *node;
        arStatus_t      status;
        const wchar_t   *p;
        
        arString_t      *curr_path;
        size_t path_len;
        AR_ASSERT(path != NULL && full_path != NULL);
        
        AR_ClearString(full_path);
        
        path_len = AR_wcslen(path);
        if(path_len == 0)
        {
                return AR_S_YES;
        }
        
        status = AR_S_YES;
        
        final_list = AR_CreateList(NULL, NULL, NULL);
        lst =  AR_CreateList(NULL, NULL, NULL);
        curr_path = AR_CreateString();
        
        if(curr_path == NULL || lst == NULL || final_list == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        
        p = AR_wcstrim_space(path);
        
        if(*p == L'/')
        {
                status = __insert_item(lst, L"/");
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
                p = AR_wcstrim(p, L"/");
        }else
        {
                const wchar_t *wcs;
                status = AR_GetCurrentPath(curr_path);
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
                
                
                wcs = AR_GetStringCString(curr_path);
                AR_ASSERT(*wcs == L'/');
                
                status = __insert_item(lst, L"/");
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
                
                wcs++;
                
                status = __split_path(lst, wcs);
                
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
        }
        
        status = __split_path(lst, p);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        
        for(node = lst->head; node != NULL; node = node->next)
        {
                const wchar_t *item = (wchar_t*)(node->data);
                AR_ASSERT(item != NULL);
                
                if(AR_wcscmp(item, L".") == 0)
                {
                        
                }else if(AR_wcscmp(item, L"..") == 0)
                {
                        if(AR_GetListCount(final_list) > 1)
                        {
                                AR_PopListBack(final_list);
                        }
                }else
                {
                        status = AR_PushListBack(final_list, (void*)item);
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                }
        }
        
        for(node = final_list->head; node != NULL; node = node->next)
        {
                const wchar_t *item = (wchar_t*)(node->data);
                AR_ASSERT(item != NULL);
                //AR_printf(L"%ls\r\n", item);
                
                status = AR_AppendString(full_path, item);
                
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
                
                if(node->prev == NULL || node->next == NULL)
                {
                }else
                {
                        status = AR_AppendString(full_path, L"/");
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                }
        }
        
        if(path[path_len-1] == L'/')
        {
                const wchar_t *wcs = AR_GetStringCString(full_path);
                if(wcs[AR_GetStringLength(full_path) - 1] != L'/')
                {
                        status = AR_AppendString(full_path, L"/");
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                }
        }
        
        
END_POINT:
        
        if(curr_path)
        {
                AR_DestroyString(curr_path);
                curr_path = NULL;
        }
        
        if(final_list)
        {
                AR_DestroyList(final_list);
                final_list = NULL;
        }
        
        if(lst)
        {
                for(node = lst->head; node != NULL; node = node->next)
                {
                        wchar_t *item = (wchar_t*)(node->data);
                        AR_ASSERT(item != NULL);
                        AR_DEL(item);
                }
                
                AR_DestroyList(lst);
                lst = NULL;
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


arPathIter_t*	AR_CreatePathIterator()
{
        arPathIter_t *iter;
        
		iter = AR_NEW0(arPathIter_t);
		return iter;
}

static void __clear_path_iterator(arPathIter_t *iter)
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
}

void			AR_DestroyPathIterator(arPathIter_t *iter)
{
		AR_ASSERT(iter != NULL);
		__clear_path_iterator(iter);
		AR_DEL(iter);
		iter = NULL;
}


arStatus_t      AR_PathIteratorSetPath(arPathIter_t *iter, const wchar_t *path)
{
        char *s_tmp;
		arStatus_t status;
        arString_t *expanded_path, *absolute_path;
		AR_ASSERT(iter != NULL && path != NULL);
        
		s_tmp = NULL;
        expanded_path = NULL;
		status = AR_S_YES;
        __clear_path_iterator(iter);
        
        expanded_path = AR_CreateString();
        absolute_path = AR_CreateString();
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
        
        status = AR_GetRealPath(AR_GetStringCString(expanded_path), absolute_path);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        /*
         AR_error(AR_ERR_DEBUG, L"absolute_path : %ls\r\n", AR_GetStringCString(absolute_path));
         */
        
		if(AR_GetStringChar(absolute_path, AR_GetStringLength(absolute_path) - 1) != L'/')
		{
				status = AR_AppendString(absolute_path, L"/");
				if(status != AR_S_YES)
				{
						goto END_POINT;
				}
		}
        
        s_tmp = AR_wcs_to_str(AR_CP_ACP, AR_GetStringCString(absolute_path), AR_GetStringLength(absolute_path));
        if(s_tmp == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        
		iter->hdl = opendir(s_tmp);
		if(iter->hdl == NULL)
		{
                int err_code = errno;
                status = __map_last_error(err_code);
				goto END_POINT;
		}
        
		iter->isdone = false;
		iter->path = AR_wcsdup(AR_GetStringCString(absolute_path));
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
        if(absolute_path != NULL)
        {
                AR_DestroyString(absolute_path);
                absolute_path = NULL;
        }
        
		if(status == AR_S_YES)
		{
                
		}else if(status == AR_E_NOMORE)
        {
                status = AR_S_YES;
        }else
		{
                __clear_path_iterator(iter);
        }
		
        
		return status;
}





const wchar_t*	AR_PathIteratorCurrent(const arPathIter_t *iter)
{
		AR_ASSERT(iter != NULL);
        AR_ASSERT(iter->hdl != NULL && iter->path != NULL && iter->current != NULL);
        
        if(iter->current == NULL)
        {
                return NULL;
        }else
        {
                return AR_GetStringCString(iter->current);
        }
}



arStatus_t		AR_PathIteratorNext(arPathIter_t *iter)
{
		arStatus_t status;
		AR_ASSERT(iter != NULL);
        
        if(iter->hdl == NULL)
        {
                AR_ASSERT(iter->path == NULL && iter->current == NULL);
                return AR_E_NOTREADY;
        }
        
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
        AR_ASSERT(iter->hdl != NULL && iter->path != NULL && iter->current != NULL);
		return iter->isdone;
}

const wchar_t*  AR_PathIteratorPath(const arPathIter_t *iter)
{
        AR_ASSERT(iter != NULL && iter->path != NULL);
        AR_ASSERT(iter->hdl != NULL && iter->path != NULL && iter->current != NULL);
        return iter->path;
}




/************************************************************File*************************************************************/


arFile_t*	AR_open_file(const wchar_t *path, const wchar_t *mode)
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

        return (arFile_t*)file;
}


void	AR_close_file(arFile_t *f)
{
        FILE *file;
		AR_ASSERT(f != NULL);
        file = (FILE*)f;
		fclose(file);
        file = NULL;
}




AR_NAMESPACE_END

#endif

