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


static arStatus_t       __map_last_error()
{
        
        switch(errno)
        {
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
                        return AR_E_INVAL;
                case ENOMEM:
                         return AR_E_NOMEM;
                case ENFILE:
                case EMFILE:
                default:
                        AR_error(AR_ERR_WARNING, L"file error : %d, '%hs'\r\n", strerror(errno));
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
                status = __map_last_error();
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
        arString_t *expanded_path;
		AR_ASSERT(iter != NULL && path != NULL);
        
		s_tmp = NULL;
        expanded_path = NULL;
		status = AR_S_YES;
        __clear_path_iterator(iter);
        
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
        
        
		iter->hdl = opendir(s_tmp);
		if(iter->hdl == NULL)
		{
                status = __map_last_error();
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

/*********************************************************Path*******************************************************/

#if(0)
arStatus_t      AR_path_is_existed(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        struct stat st;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        status = lstat(utf8, &st) == 0 ? AR_S_YES : AR_S_NO;
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
        
}


arStatus_t  AR_path_is_readable(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        struct stat st;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(lstat(utf8, &st) == 0)
        {
                if(geteuid() == 0)
                {
                        status = AR_S_YES;
                }else if(st.st_uid == geteuid())
                {
                        status = st.st_mode & S_IRUSR ? AR_S_YES : AR_S_NO;
                }else if(st.st_gid == getegid())
                {
                        status = st.st_mode & S_IRGRP ? AR_S_YES : AR_S_NO;
                }else
                {
                        status = st.st_mode & S_IROTH ? AR_S_YES : AR_S_NO;
                }
        }else
        {
                status = AR_E_FILE;
        }
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
}




arStatus_t  AR_path_is_writeable(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        struct stat st;
        AR_ASSERT(path != NULL);

        status = AR_S_YES;
        
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(lstat(utf8, &st) == 0)
        {
                if(geteuid() == 0)
                {
                        status = AR_S_YES;
                }else if(st.st_uid == geteuid())
                {
                        status = st.st_mode & S_IWUSR ? AR_S_YES : AR_S_NO;
                        
                }else if(st.st_gid == getegid())
                {
                        status = st.st_mode & S_IWGRP ? AR_S_YES : AR_S_NO;
                }else
                {
                        status = st.st_mode & S_IWOTH ? AR_S_YES : AR_S_NO;
                }
        }else
        {
                status = AR_E_FILE;
        }
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
}





arStatus_t  AR_path_is_executable(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        struct stat st;
        AR_ASSERT(path != NULL);

        status = AR_S_YES;
        
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        if(lstat(utf8, &st) == 0)
        {
                if(geteuid() == 0 || st.st_uid == geteuid())
                {
                        status = st.st_mode & S_IXUSR ? AR_S_YES : AR_S_NO; 

                }else if(st.st_gid == getegid())
                {
                        status = st.st_mode & S_IXGRP ? AR_S_YES : AR_S_NO;
                }else
                {
                        status = st.st_mode & S_IXOTH ? AR_S_YES : AR_S_NO;
                }
        }else
        {
                status = AR_E_FILE;
        }
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
}



arStatus_t  AR_path_is_file(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        struct stat st;
        AR_ASSERT(path != NULL);

        status = AR_S_YES;
        
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(lstat(utf8, &st) == 0)
        {
                status = S_ISREG(st.st_mode) ? AR_S_YES : AR_S_NO;
        }else
        {
                status = AR_E_FILE;
        }
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;

}


arStatus_t  AR_path_is_dir(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        struct stat st;
        AR_ASSERT(path != NULL);

        status = AR_S_YES;
        
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(lstat(utf8, &st) == 0)
        {
                status = S_ISDIR(st.st_mode) ? AR_S_YES : AR_S_NO;
        }else
        {
                status = AR_E_FILE;
        }
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
        
}



arStatus_t  AR_path_is_link(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        struct stat st;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(lstat(utf8, &st) == 0)
        {
                status = S_ISLNK(st.st_mode) ? AR_S_YES : AR_S_NO;
        }else
        {
                status = AR_E_FILE;
        }
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
        
}



arStatus_t  AR_path_is_dev(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        struct stat st;
        AR_ASSERT(path != NULL);

        status = AR_S_YES;
        
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(lstat(utf8, &st) == 0)
        {
                status = (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) ? AR_S_YES : AR_S_NO;
        }else
        {
                status = AR_E_FILE;
        }
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
        
}


arStatus_t  AR_path_is_hidden(const wchar_t *path)
{
        arStatus_t status;
        const wchar_t *p;
        arString_t *expanded;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        expanded = AR_CreateString();
        
        if(expanded == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        status = AR_GetExpandPath(path, expanded);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        p = AR_reverse_wcschr(AR_GetStringCString(expanded), AR_GetStringLength(expanded), L'/');
        
        if(p == NULL)
        {
                status = AR_S_NO;
                goto END_POINT;
        }
        
        status = *(p + 1) == L'.' ? AR_S_YES : AR_S_NO;
        
        
END_POINT:
        if(expanded)
        {
                AR_DestroyString(expanded);
                expanded = NULL;
        }
        return status;
}


arStatus_t AR_path_get_size(const wchar_t *path, uint_64_t *ps)
{
        arStatus_t status;
        char *utf8;
        struct stat st;
        AR_ASSERT(path != NULL && ps != NULL);

        status = AR_S_YES;
        
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(lstat(utf8, &st) == 0)
        {
                status = AR_S_YES;
                *ps = (uint_64_t)st.st_size;
        }else
        {
                status = __map_last_error();
        }
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;

        
        
}

arStatus_t      AR_path_set_size(const wchar_t *path, uint_64_t size)
{

        arStatus_t status;
        char *utf8;
        AR_ASSERT(path != NULL);

        status = AR_S_YES;
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }

        if(truncate(utf8, (off_t)size) != 0)
        {
                status = __map_last_error();
        }
                
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;

}


arStatus_t      AR_path_set_writeable(const wchar_t *path, bool_t flag)
{
        
        arStatus_t status;
        char *utf8;
        struct stat st;
        mode_t mode;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(lstat(utf8, &st) != 0)
        {
                status = __map_last_error();
                goto END_POINT;
        }
        
        if(flag)
        {
                mode = st.st_mode | S_IWUSR;
        }else
        {
                mode_t wmask = S_IWUSR|S_IWGRP|S_IWOTH;
                mode = st.st_mode & ~wmask;
        }

        if (chmod(utf8, mode) != 0)
        {
                status = __map_last_error();
                goto END_POINT;
        }
                
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
        
}



arStatus_t      AR_path_set_executable(const wchar_t *path, bool_t flag)
{
        
        arStatus_t status;
        char *utf8;
        struct stat st;
        mode_t mode;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(lstat(utf8, &st) != 0)
        {
                status = __map_last_error();
                goto END_POINT;
        }
        
        if(flag)
        {
                mode = st.st_mode | S_IXUSR;
        }else
        {
                mode_t wmask = S_IXUSR | S_IXGRP | S_IXOTH;
                mode = st.st_mode & ~wmask;
        }

        if(chmod(utf8, mode) != 0)
        {
                status = __map_last_error();
                goto END_POINT;
        }
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
        
}


arStatus_t      AR_path_rename(const wchar_t *src_path, const wchar_t *dest_path)
{
        arStatus_t status;
        char *src_utf8,*dest_utf8;
        AR_ASSERT(src_path != NULL && dest_path != NULL);
        
        status = AR_S_YES;
        
        src_utf8 = AR_wcs_to_str(AR_CP_UTF8, src_path, AR_wcslen(src_path));
        dest_utf8 = AR_wcs_to_str(AR_CP_UTF8, dest_path, AR_wcslen(dest_path));
        
        if(src_utf8 == NULL || dest_utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(rename(src_utf8, dest_utf8) != 0)
        {
                status = __map_last_error();
                goto END_POINT;
        }
        
        
END_POINT:
        if(src_utf8)
        {
                AR_DEL(src_utf8);
                src_utf8 = NULL;
        }
        
        if(dest_utf8)
        {
                AR_DEL(dest_utf8);
                dest_utf8 = NULL;
        }
        
        return status;
}



arStatus_t      AR_path_remove(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        int rc;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(AR_path_is_link(path) == AR_S_NO && AR_path_is_dir(path) == AR_S_YES)
        {
                rc = rmdir(utf8);
        }else
        {
                rc = unlink(utf8);
        }
        
        if(rc != 0)
        {
                AR_error(AR_ERR_WARNING,L"remove %ls error : %hs\r\n", path, strerror(errno));
                status = __map_last_error();
                goto END_POINT;
        }

END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;

        
}


arStatus_t AR_path_create_file(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        int n;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        n = open(utf8, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

        if(n != -1)
        {
                status = AR_S_YES;
                goto END_POINT;
        }
        
        if(n == -1 && errno == EEXIST)
        {
                status = AR_E_EXISTED;
                goto END_POINT;
        }else
        {
                AR_error(AR_ERR_WARNING, L"create %ls error : %hs\r\n", path, strerror(errno));
                status = __map_last_error();
                goto END_POINT;
        }
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
        
}





arStatus_t AR_path_create_dir(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        if(utf8 == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        if(AR_path_is_existed(path) == AR_S_YES && AR_path_is_dir(path) == AR_S_YES)
        {
                status = AR_E_EXISTED;
                goto END_POINT;
        }
        
        if(mkdir(utf8, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
        {
                AR_error(AR_ERR_WARNING, L"create %ls error : %hs\r\n", path, strerror(errno));
                status = __map_last_error();
                goto END_POINT;
        }
        
        
END_POINT:
        if(utf8 != NULL)
        {
                AR_DEL(utf8);
                utf8 = NULL;
        }
        return status;
}
#endif








AR_NAMESPACE_END

