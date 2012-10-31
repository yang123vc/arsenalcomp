//
//  path_posix.h
//  Common
//
//  Created by Solidus on 12-8-6.
//  Copyright (c) 2012年 none. All rights reserved.
//

#ifndef __ARSENAL_COMMON_PATH_POSIX_H__
#define __ARSENAL_COMMON_PATH_POSIX_H__

AR_NAMESPACE_BEGIN

/*********************************************************Path*******************************************************/




arStatus_t      AR_path_is_existed(const wchar_t *path)
{
        arStatus_t status;
        char *utf8;
        struct stat st;
        AR_ASSERT(path != NULL);
        /*
         AR_error(AR_ERR_WARNING, L"%hs : %ls\r\n", AR_FUNC_NAME, path);
         */
        
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
        }else
        {
                int err_code = errno;
                status = __map_last_error(err_code);
                
                if(status == AR_E_NOTFOUND)
                {
                        status = AR_S_NO;
                }
        }
        
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                status = __map_last_error(err_code);
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
                int err_code = errno;
                AR_error(AR_ERR_WARNING,L"remove %ls error : %hs\r\n", path, strerror(err_code));
                status = __map_last_error(err_code);
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
        int err_code;
        AR_ASSERT(path != NULL);
        
        /*
         AR_error(AR_ERR_WARNING, L"%hs : %ls\r\n", AR_FUNC_NAME, path);
         */
        
        
        status = AR_S_YES;
        err_code = 0;
        utf8 = AR_wcs_to_str(AR_CP_UTF8, path, AR_wcslen(path));
        
        /*
         AR_error(AR_ERR_WARNING, L"%hs : utf8 : %hs\r\n", AR_FUNC_NAME, utf8);
         */
        
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
        err_code = errno;
        if(n == -1 && err_code == EEXIST)
        {
                status = AR_E_EXISTED;
                goto END_POINT;
        }else
        {
                AR_error(AR_ERR_WARNING, L"create %ls error : %hs\r\n", path, strerror(err_code));
                status = __map_last_error(err_code);
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
                int err_code = errno;
                AR_error(AR_ERR_WARNING, L"create %ls error : %hs\r\n", path, strerror(err_code));
                status = __map_last_error(err_code);
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







arStatus_t      AR_path_create_path(const wchar_t *path)
{
        arStatus_t      status;
        arString_t      *expanded, *absolute;
        const wchar_t   *p;
        wchar_t         *b;
        wchar_t         *buf;
        AR_ASSERT(path != NULL);
        /*
         AR_error(AR_ERR_WARNING, L"%hs : %ls\r\n", AR_FUNC_NAME, path);
         */
        
        if(AR_wcslen(path) == 0)
        {
                return AR_S_YES;
        }
        
        
        
        status = AR_S_YES;
        buf = NULL;
        
        expanded = AR_CreateString();
        absolute = AR_CreateString();
        if(expanded == NULL || absolute == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        
        status = AR_GetExpandPath(path, expanded);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        /*
         AR_error(AR_ERR_WARNING, L"%hs : expanded %ls\r\n", AR_FUNC_NAME, path, AR_GetStringCString(expanded));
         */
        
        status = AR_GetRealPath(AR_GetStringCString(expanded), absolute);
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        /*
         AR_error(AR_ERR_WARNING, L"%hs : expanded absolute %ls\r\n", AR_FUNC_NAME, path, AR_GetStringCString(absolute));
         */
        
        buf = AR_NEWARR(wchar_t , AR_GetStringLength(absolute) + 1);
        
        if(buf == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        b = buf;
        p = AR_GetStringCString(absolute);
        
        
        p = (wchar_t*)AR_wcstrim_space(p);
        p = (wchar_t*)AR_wcstrim(p, L"/"); /*ÂéªÊéâÊâÄÊúâÂâçÁºÄ'/'*/
        *b++ = L'/';
        
        while(*p != L'\0')
        {
                if(*p == L'/')
                {
                        *b = L'\0';
                        
                        status = AR_path_create_dir(buf);
                        
                        if(status == AR_S_YES || status == AR_E_EXISTED)
                        {
                                status = AR_path_is_dir(buf);
                                if(status == AR_S_YES)
                                {
                                        status = AR_S_YES;
                                        
                                }else if(status == AR_S_NO)
                                {
                                        status = AR_E_NOTDIR;
                                }else
                                {
                                        goto END_POINT;
                                }
                        }
                        
                        if(status != AR_S_YES)
                        {
                                AR_error(AR_ERR_WARNING, L"AR_path_create_path : create dir '%ls' failed!\r\n", buf);                                
                                goto END_POINT;
                        }
                        
                        *b = L'/';
                        
                        while(*p == L'/')
                        {
                                ++p;
                        }
                        ++b;
                }else
                {
                        *b++ = *p++;
                }
        }
        
        *b = L'\0';
        
        if(*(b-1) == L'/')
        {
                /*
                 AR_error(AR_ERR_DEBUG, L"create dir %ls\r\n", buf);
                 */
        }else
        {
                status = AR_path_create_file(buf);
                if(status == AR_S_YES || status == AR_E_EXISTED)
                {
                        status = AR_path_is_file(buf);
                        if(status == AR_S_YES)
                        {
                                status = AR_S_YES;
                                
                        }else if(status == AR_S_NO)
                        {
                                status = AR_E_ISDIR;
                        }else
                        {
                                goto END_POINT;
                        }
                        
                }
                
                if(status != AR_S_YES)
                {
                        AR_error(AR_ERR_WARNING, L"AR_path_create_path : create file '%ls' failed!\r\n", buf);
                        goto END_POINT;
                }
        }
        
        
END_POINT:
        if(expanded)
        {
                AR_DestroyString(expanded);
                expanded = NULL;
        }
        
        if(absolute)
        {
                AR_DestroyString(absolute);
                absolute = NULL;
        }
        
        if(buf)
        {
                AR_DEL(buf);
                buf = NULL;
        }
        
        return status;
        
}



/********************************************************************************/



static arStatus_t      __remove_path(const wchar_t *path);

static arStatus_t      __delete_file_or_dir(const wchar_t *path)
{
        arStatus_t      status;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        
        status = AR_path_is_dir(path);
        
        if(status == AR_S_YES)
        {
                status = __remove_path(path);
                
        }else if(status == AR_S_NO)
        {
                status = AR_path_remove(path);
        }else
        {
                goto END_POINT;
        }
        
END_POINT:
        return status;
}



static arStatus_t       __remove_dir(const wchar_t *path)
{
        arStatus_t              status;
        arPathIter_t            *iter;
        wchar_t *buf;
        AR_ASSERT(path != NULL);
        
        
        status = AR_S_YES;
        iter = NULL;
        buf = NULL;
        
        status = AR_path_is_dir(path);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        iter = AR_CreatePathIterator();
        
        if(iter == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        status = AR_PathIteratorSetPath(iter, path);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        buf = AR_NEWARR(wchar_t, 2048);
        
        if(buf == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        while(status == AR_S_YES && !AR_PathIteratorIsDone(iter))
        {
                int_t ret;
                ret = AR_swprintf(buf, 2048, L"%ls%ls", AR_PathIteratorPath(iter), AR_PathIteratorCurrent(iter));
                
                if(ret <= 0)
                {
                        status = AR_E_IO;
                        goto END_POINT;
                }
                
                __delete_file_or_dir(buf);
                
                if(AR_PathIteratorNext(iter) != AR_S_YES)
                {
                        break;
                }
        }
        
        status = AR_path_remove(path);
        
END_POINT:
        
        if(iter)
        {
                AR_DestroyPathIterator(iter);
                iter = NULL;
        }
        
        if(buf)
        {
                AR_DEL(buf);
                buf = NULL;
        }
        
        return status;
        
}

static arStatus_t       __remove_path(const wchar_t *path)
{
        arStatus_t status;
        AR_ASSERT(path != NULL);
        status = AR_S_YES;
        
        status = AR_path_is_dir(path);
        
        if(status == AR_S_YES)
        {
                status = __remove_dir(path);
                
        }else if(status == AR_S_NO)
        {
                status = AR_path_remove(path);
        }else 
        {
                goto END_POINT;
        }
        
        
        
END_POINT:
        return status;
}


arStatus_t      AR_path_remove_path(const wchar_t *path)
{
        arStatus_t      status;
        arString_t      *expanded, *absolute;
        AR_ASSERT(path != NULL);
        
        if(AR_wcslen(path) == 0)
        {
                return AR_S_YES;
        }
        
        expanded = AR_CreateString();
        absolute = AR_CreateString();
        
        if(expanded == NULL || absolute == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        
        status = AR_GetExpandPath(path, expanded);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        status = AR_GetRealPath(AR_GetStringCString(expanded), absolute);
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        status = __remove_path(AR_GetStringCString(absolute));
        
END_POINT:
        
        if(expanded)
        {
                AR_DestroyString(expanded);
                expanded = NULL;
        }
        
        if(absolute)
        {
                AR_DestroyString(absolute);
                absolute = NULL;
        }
        
        return status;
}




arStatus_t      AR_path_copyfile(const wchar_t *src, const wchar_t *dest, bool_t truncated)
{
        arStatus_t status;
        arFile_t *s, *d;
        AR_ASSERT(src != NULL || dest != NULL);
        status = AR_S_YES;
        s = NULL;
        d = NULL;
        
        status = AR_open_file(&s, src, L"rb");
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        
        if(truncated)
        {
                status = AR_open_file(&d, dest, L"wb+");
                
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
                
        }else
        {
                status = AR_path_is_existed(dest);
                
                if(status == AR_S_YES)
                {
                        status = AR_E_EXISTED;
                        goto END_POINT;
                        
                }else if(status == AR_S_NO)
                {
                        status = AR_open_file(&d, dest, L"wb+");
                        
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                        
                }else
                {
                        goto END_POINT;
                }
        }
        
        
        
        while(AR_eof_file(s) == AR_S_NO)
        {
                byte_t buf[4096];
                size_t rn, wn;
                status = AR_read_file(s, buf, 4096, &rn);
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
                
                status = AR_write_file(d, buf, rn, &wn);
                
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
                
                if(AR_error_file(s) != AR_S_NO || AR_error_file(d) != AR_S_NO)
                {
                        status = AR_E_FAIL;
                        goto END_POINT;
                }
        }
        
END_POINT:
        if(s)
        {
                AR_close_file(s);
                s = NULL;
        }
        
        if(d)
        {
                AR_close_file(d);
                d = NULL;
        }
        
        return status;
}



AR_NAMESPACE_END

#endif



