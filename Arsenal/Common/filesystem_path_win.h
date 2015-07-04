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


#ifndef __ARSENAL_COMMON_PATH_POSIX_H__
#define __ARSENAL_COMMON_PATH_POSIX_H__

AR_NAMESPACE_BEGIN

arStatus_t      AR_path_is_hidden(const wchar_t *path)
{
		arStatus_t status;
		
		DWORD attr;
		AR_ASSERT(path != NULL);

		status = AR_S_YES;
		attr = 0;
		attr = GetFileAttributesW(path);

		if (attr == 0xFFFFFFFF)
		{
				DWORD err_code = GetLastError();
				status = __map_last_error(err_code);
				
		}else
		{
				status = (attr & FILE_ATTRIBUTE_HIDDEN) ? AR_S_YES : AR_S_NO;
		}
		return status;
		
}


arStatus_t      AR_path_is_dev(const wchar_t *path)
{
		arStatus_t status;
		HANDLE fhd ;
		DWORD type;
		AR_ASSERT(path != NULL);
		status = AR_S_YES;
		fhd = INVALID_HANDLE_VALUE;
		type = 0;

		fhd = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if(fhd == INVALID_HANDLE_VALUE)
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
				goto END_POINT;
		}

		type = GetFileType(fhd);
		if (type == FILE_TYPE_CHAR)
		{
				status = AR_S_YES;
		}else if (type == FILE_TYPE_UNKNOWN)
		{
				DWORD last_err = GetLastError();
				if(last_err != NO_ERROR)
				{
						status = __map_last_error(last_err);
				}else
				{
						status = AR_S_NO;
				}
		}else
		{
				status = AR_S_NO;
		}

END_POINT:
		if(fhd != INVALID_HANDLE_VALUE)
		{
				CloseHandle(fhd);
				fhd = INVALID_HANDLE_VALUE;
		}

		return status;
}


arStatus_t      AR_path_is_dir(const wchar_t *path)
{
		DWORD attr;
		arStatus_t status;
		AR_ASSERT(path != NULL);
		status = AR_S_YES;
		attr = 0;
		
		attr = GetFileAttributesW(path);

		if(attr == 0xFFFFFFFF)
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
		}else
		{
				status = attr & FILE_ATTRIBUTE_DIRECTORY ? AR_S_YES : AR_S_NO;
		}

		return status;
}



arStatus_t      AR_path_is_link(const wchar_t *path)
{
		AR_ASSERT(path != NULL);
		AR_UNUSED(path);
		return AR_S_NO;
}


arStatus_t      AR_path_is_file(const wchar_t *path)
{
		arStatus_t status;
		AR_ASSERT(path != NULL);
		
		status = AR_path_is_dir(path);

		if(status == AR_S_YES)
		{
				return AR_S_NO;
		}else if(status == AR_S_NO)
		{
		
		}else	//error
		{
				return status;
		}

		status = AR_path_is_dev(path);

		if(status == AR_S_YES)
		{
				return AR_S_NO;
		}else if(status == AR_S_NO)
		{

		}else	//error
		{
				return status;
		}

		return AR_S_YES;
}


arStatus_t      AR_path_is_readable(const wchar_t *path)
{
		arStatus_t status;
		DWORD attr;
		AR_ASSERT(path != NULL);
		status = AR_S_YES;
		attr = 0;

		attr = GetFileAttributesW(path);

		if (attr == 0xFFFFFFFF)
		{
				DWORD last_err = GetLastError();
				if(last_err == ERROR_ACCESS_DENIED)
				{
						status = AR_S_NO;
				}else
				{
						status = __map_last_error(last_err);
				}
		}else
		{
				status = AR_S_YES;
		}
	
		return status;
}



arStatus_t      AR_path_is_writeable(const wchar_t *path)
{
		arStatus_t status;
		DWORD attr;
		AR_ASSERT(path != NULL);
		status = AR_S_YES;
		attr = 0;

		attr = GetFileAttributesW(path);

		if (attr == 0xFFFFFFFF)
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
		}else
		{
				if(attr & FILE_ATTRIBUTE_READONLY)
				{
						status = AR_S_NO;
				}else
				{
						status = AR_S_YES;
				}
		}
	
		return status;
}


arStatus_t      AR_path_is_executable(const wchar_t *path)
{
		wchar_t ext[128];
		AR_ASSERT(path != NULL);
		
		ext[0] = L'\0';
		_wsplitpath(path, NULL, NULL, NULL, ext);

		if(AR_wcsicmp(ext, L".exe") == 0)
		{
				return AR_S_YES;
		}else
		{
				return AR_S_NO;
		}
}



arStatus_t      AR_path_is_existed(const wchar_t *path)
{
		arStatus_t status;
		DWORD attr;
		AR_ASSERT(path != NULL);
		status = AR_S_YES;
		attr = GetFileAttributesW(path);

		if(attr == 0xFFFFFFFF)
		{
				DWORD last_err = GetLastError();
				switch (last_err)
				{
				case ERROR_FILE_NOT_FOUND:
				case ERROR_PATH_NOT_FOUND:
				case ERROR_NOT_READY:
				case ERROR_INVALID_DRIVE:
						status = AR_S_NO;
						break;
				default:
						status = __map_last_error(last_err);
						break;
				}
		}else
		{
				status = AR_S_YES;
		}

		return status;
}


arStatus_t      AR_path_set_executable(const wchar_t *path, ar_bool_t flag)
{
		AR_ASSERT(path != NULL);
		AR_UNUSED(path);
		AR_UNUSED(flag);
		
		AR_error(AR_ERR_WARNING, L"not impl in windows\r\n");
		return AR_S_YES;
}

arStatus_t      AR_path_set_writeable(const wchar_t *path, ar_bool_t flag)
{
		arStatus_t status;
		DWORD attr;
		AR_ASSERT(path != NULL);

		attr = GetFileAttributesW(path);

		if(attr == -1)
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
				return status;
		}

		if (flag)
		{
				attr &= ~FILE_ATTRIBUTE_READONLY;
		}else
		{
				attr |= FILE_ATTRIBUTE_READONLY;
		}

		if(!SetFileAttributesW(path, attr))
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
				return status;
		}

		return AR_S_YES;
}


arStatus_t      AR_path_get_size(const wchar_t *path, ar_uint_64_t *ps)
{
		arStatus_t status;
		WIN32_FILE_ATTRIBUTE_DATA fad;
		LARGE_INTEGER li;
		AR_ASSERT(path != NULL && ps != NULL);

		status = AR_S_YES;
		AR_memset(&fad, 0, sizeof(fad));
		
		if(!GetFileAttributesExW(path, GetFileExInfoStandard, &fad)) 
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
		}else
		{
				li.LowPart  = fad.nFileSizeLow;
				li.HighPart = fad.nFileSizeHigh;
				*ps = li.QuadPart;
				status = AR_S_YES;
		}

		return status;
}

arStatus_t      AR_path_set_size(const wchar_t *path, ar_uint_64_t size)
{
		arStatus_t  status;
		HANDLE fhd;
		LARGE_INTEGER li;
		AR_ASSERT(path != NULL);

		status = AR_S_YES;
		fhd = INVALID_HANDLE_VALUE;


		fhd = CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if(fhd == INVALID_HANDLE_VALUE)
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
				goto END_POINT;
		}

	
		li.QuadPart = size;

		if(SetFilePointer(fhd, li.LowPart, &li.HighPart, FILE_BEGIN) == -1)
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
				goto END_POINT;
		}

		if (SetEndOfFile(fhd) == 0)
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
				goto END_POINT;
		}

END_POINT:
		if(fhd != INVALID_HANDLE_VALUE)
		{
				CloseHandle(fhd);
				fhd = INVALID_HANDLE_VALUE;
		}

		return status;
}






arStatus_t      AR_path_get_creationtime(const wchar_t *path, ar_int_64_t *tm)
{
        return AR_E_NOTSUPPORTED;
}



arStatus_t      AR_path_get_modifiedtime(const wchar_t *path, ar_int_64_t *tm)
{
        return AR_E_NOTSUPPORTED;
        
}


arStatus_t      AR_path_set_modifiedtime(const wchar_t *path, ar_int_64_t tm)
{
        return AR_E_NOTSUPPORTED;
}









/**********************************************************************************************/

arStatus_t      AR_path_rename(const wchar_t *src_path, const wchar_t *dest_path)
{
		arStatus_t status;
		AR_ASSERT(src_path != NULL && dest_path != NULL);

		if(!MoveFileW(src_path, dest_path))
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
		}else
		{
				status = AR_S_YES;
		}

		return status;
}


arStatus_t      AR_path_remove(const wchar_t *path)
{
		arStatus_t status;
		AR_ASSERT(path != NULL);

		if (AR_path_is_dir(path) == AR_S_YES)
		{
				if(!RemoveDirectoryW(path))
				{
						DWORD last_err = GetLastError();
						status = __map_last_error(last_err);
						return status;
				}else
				{
						return AR_S_YES;
				}
		}else
		{
				if(!DeleteFileW(path))
				{
						DWORD last_err = GetLastError();
						status = __map_last_error(last_err);
						return status;
				}else
				{
						return AR_S_YES;
				}
		}
}


arStatus_t      AR_path_create_file(const wchar_t *path)
{
		arStatus_t status;
		HANDLE hfd;
		AR_ASSERT(path != NULL);

		status = AR_S_YES;
		hfd = INVALID_HANDLE_VALUE;
		
		hfd = CreateFileW(path, GENERIC_WRITE, 0, 0, CREATE_NEW, 0, 0);

		if (hfd != INVALID_HANDLE_VALUE)
		{
				status = AR_S_YES;
				goto END_POINT;
		}else
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
		}

END_POINT:
		if(hfd != INVALID_HANDLE_VALUE)
		{
				CloseHandle(hfd);
				hfd = INVALID_HANDLE_VALUE;
		}
		return status;
}

arStatus_t      AR_path_create_dir(const wchar_t *path)
{
		arStatus_t status;
		AR_ASSERT(path != NULL);

		if(AR_path_is_existed(path) == AR_S_YES && AR_path_is_dir(path) == AR_S_YES)
		{
				return AR_E_EXISTED;
		}
		
		if(!CreateDirectoryW(path, NULL))
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
				
		}else
		{
				status = AR_S_YES;
		}
		return status;

}

arStatus_t      AR_path_copyfile(const wchar_t *src, const wchar_t *dest, ar_bool_t truncated)
{
		arStatus_t status;
		BOOL trunc;
		AR_ASSERT(src != NULL && dest != NULL);

		status = AR_S_YES;
		trunc = truncated ? FALSE : TRUE;

		if(!CopyFileW(src, dest, trunc))
		{
				DWORD last_err = GetLastError();
				status = __map_last_error(last_err);
		}else
		{
				status = AR_path_set_writeable(dest, true);
		}

		return status;
}



arStatus_t      AR_path_copylink(const wchar_t *src, const wchar_t *dest, ar_bool_t truncated)
{
		AR_ASSERT(src != NULL && dest != NULL);
		
		AR_UNUSED(src);
		AR_UNUSED(dest);
		AR_UNUSED(truncated);
		AR_error(AR_ERR_WARNING, L"not impl in windows\r\n");

		return AR_S_YES;
}



/************************************CreatePath***********************/


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

        
        while(*p != L'\0')
        {
                if(*p == L'\\')
                {
                        *b = L'\0';

						//AR_wcscat(buf, L"\\");
						status = AR_path_create_dir(buf);

						if(status == AR_S_YES || status == AR_E_EXISTED)
						{
								if(AR_path_is_dir(buf) == AR_S_YES)
								{
										status = AR_S_YES;
								}else
								{
										status = AR_S_NO;
								}

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
                        
                        *b = L'\\';
                        
						while(*p == L'\\')
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
        
        if(*(b-1) == L'\\')
        {

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


/******************************************remove path**************************************/



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
                ar_int_t ret;
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




/*********************************************************************************************/


static arStatus_t __copy_dir(const wchar_t *src, const wchar_t *dest)
{
        arStatus_t status;
        wchar_t *src_buf, *dest_buf;
        arPathIter_t    *iter;
        
        AR_ASSERT(src != NULL && dest != NULL);
        
        status = AR_S_YES;
        iter = NULL;
        src_buf = NULL;
        dest_buf = NULL;
        AR_DPRINT(L"%ls\r\n", src);
        AR_DPRINT(L"%ls\r\n", dest);
        status = AR_path_is_dir(src);
        
        if(status != AR_S_YES)
        {
                AR_error(AR_ERR_WARNING, L"invalid path : '%ls'\r\n", src);
                status = AR_E_INVAL;
                goto END_POINT;
        }
        
        
        if(AR_path_is_existed(dest) != AR_S_YES)
        {
                status = AR_path_create_path(dest);
                
                if(status != AR_S_YES)
                {
                        AR_error(AR_ERR_WARNING, L"failed to create path : '%ls\r\n", dest);
                        goto END_POINT;
                }
        }
        
        
        iter  = AR_CreatePathIterator();
        
        if(iter == NULL)
        {
                status = AR_E_NOMEM;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto END_POINT;
        }
        
        
        
        status = AR_PathIteratorSetPath(iter, src);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        
        src_buf = AR_NEWARR(wchar_t, 2048);
        dest_buf = AR_NEWARR(wchar_t, 2048);
        
        if(src_buf == NULL || dest_buf == NULL)
        {
                status = AR_E_NOMEM;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto END_POINT;
                
        }
        
        while(status == AR_S_YES && !AR_PathIteratorIsDone(iter))
        {
                ar_int_t ret;
                ret = AR_swprintf(src_buf, 2048, L"%ls%ls", src, AR_PathIteratorCurrent(iter));
                
                if(ret <= 0)
                {
                        status = AR_E_IO;
                        goto END_POINT;
                }
                
                ret = AR_swprintf(dest_buf, 2048, L"%ls%ls", dest, AR_PathIteratorCurrent(iter));
                
                if(ret <= 0)
                {
                        status = AR_E_IO;
                        goto END_POINT;
                }
                

                if(AR_path_is_file(src_buf) == AR_S_YES)
                {
                        status = AR_path_copyfile(src_buf, dest_buf, true);
                        if(status != AR_S_YES)
                        {
                                AR_error(AR_ERR_WARNING, L"failed to copy file to : '%ls'\r\n", dest_buf);
                                goto END_POINT;
                        }
                        
                }else if(AR_path_is_dir(src_buf) == AR_S_YES)
                {
                        AR_wcscat(src_buf, L"\\");
                        AR_wcscat(dest_buf, L"\\");
                        
                        status = __copy_dir(src_buf, dest_buf);
                        
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                        
                }else if(AR_path_is_dev(src_buf) == AR_S_YES)
                {
                        AR_error(AR_ERR_MESSAGE, L"Can not copy device file : '%ls'\r\n", src_buf);
                }else
                {
                        AR_error(AR_ERR_WARNING, L"invalid file type : '%ls'\r\n", src_buf);
                }
                

                if(AR_PathIteratorNext(iter) != AR_S_YES)
                {
                        break;
                }
        }
        
        
        if(status == AR_E_NOMORE)
        {
                status = AR_S_YES;
        }
        
END_POINT:
        
        
        if(iter != NULL)
        {
                AR_DestroyPathIterator(iter);
                iter = NULL;
        }
        
        if(src_buf)
        {
                AR_DEL(src_buf);
                src_buf = NULL;
        }
        
        if(dest_buf)
        {
                AR_DEL(dest_buf);
                dest_buf = NULL;
        }
        
        
        return status;
        
}



arStatus_t      AR_path_copydir(const wchar_t *src, const wchar_t *dest)
{
        arStatus_t status;
        wchar_t *sbuf, *dbuf;
        AR_ASSERT(src != NULL && dest != NULL);
        
        status = AR_S_YES;
        sbuf = NULL;
        dbuf = NULL;
        
        
        status = AR_path_is_dir(src);
        
        if(status != AR_S_YES)
        {
                AR_error(AR_ERR_WARNING, L"'%ls' is not directory!\r\n", src);
                status = AR_E_INVAL;
                goto END_POINT;
        }
        
        
        if(AR_path_is_existed(dest) == AR_S_YES)
        {
                AR_error(AR_ERR_WARNING, L"path : '%ls' already existed!\r\n", dest);
                status = AR_E_EXISTED;
                goto END_POINT;
        }
        
        sbuf = AR_NEWARR0(wchar_t, 2048);
        dbuf = AR_NEWARR0(wchar_t, 2048);
        
        if(sbuf == NULL || dbuf == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto END_POINT;
        }
        AR_wcscpy(sbuf, src);
        AR_wcscpy(dbuf, dest);
        if(sbuf[AR_wcslen(sbuf)-1] != L'\\')
        {
                AR_wcscat(sbuf, L"\\");
        }
        
        if(dbuf[AR_wcslen(dbuf)-1] != L'\\')
        {
                AR_wcscat(dbuf, L"\\");
        }
        
        status = __copy_dir(sbuf, dbuf);
        
        
END_POINT:
        if(sbuf)
        {
                AR_DEL(sbuf);
                sbuf = NULL;
        }
        
        if(dbuf)
        {
                AR_DEL(dbuf);
                dbuf = NULL;
        }
        
        return status;
}





AR_NAMESPACE_END

#endif
