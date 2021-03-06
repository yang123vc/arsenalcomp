﻿/*
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

#ifndef __ARSENAL_COMMON_FILESYSTEM_WIN_H__
#define __ARSENAL_COMMON_FILESYSTEM_WIN_H__


AR_NAMESPACE_BEGIN



#ifndef MAX_PATH_LEN
#undef MAX_PATH_LEN
#endif

#define MAX_PATH_LEN 32767

static arStatus_t       __map_last_error(DWORD last_error)
{
        switch(last_error)
        {
		case 0:
				return AR_S_YES;
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
				return AR_E_NOTFOUND;
		case ERROR_BAD_NETPATH:
		case ERROR_CANT_RESOLVE_FILENAME:
		case ERROR_INVALID_DRIVE:
		case ERROR_INVALID_NAME:
		case ERROR_FILENAME_EXCED_RANGE:
		case ERROR_BAD_PATHNAME:
				return AR_E_PATH;
		case ERROR_ACCESS_DENIED:
				return AR_E_ACCES;
		case ERROR_ALREADY_EXISTS:
		case ERROR_FILE_EXISTS:
				return AR_E_EXISTED;
		case ERROR_FILE_READ_ONLY:
				return AR_E_READONLY;
		case ERROR_DIR_NOT_EMPTY:
				return AR_E_NOTEMPTY;
		case ERROR_HANDLE_DISK_FULL:
		case ERROR_DISK_FULL:
				return AR_E_NOTENOUGH;
		case ERROR_DIRECTORY:
				return AR_E_ISDIR;
		case ERROR_NEGATIVE_SEEK:
		case ERROR_LOCK_VIOLATION:
		case ERROR_HANDLE_EOF:
		case ERROR_SHARING_VIOLATION:
		case ERROR_CANNOT_MAKE:
		case ERROR_WRITE_FAULT:
		case ERROR_READ_FAULT:
		default:
				return AR_E_FILE;
        }
}









#if(OS_TYPE == OS_WINDOWS_CE)

arStatus_t		AR_GetCurrentPath(arString_t *str)
{
		AR_ASSERT(str != NULL);
		return AR_SetString(str, L"\\");
}


arStatus_t		AR_GetHomePath(arString_t *str)
{
		AR_ASSERT(str != NULL);
		return AR_SetString(str, L"\\");
}


arStatus_t		AR_GetTempPath(arString_t *str)
{
		wchar_t buf[MAX_PATH_LEN + 10];
		DWORD n;
		AR_ASSERT(str != NULL);
		n = GetTempPathW(MAX_PATH_LEN, buf);
		if (n > 0)
		{
				if(buf[n-1] != L'\\')
				{
						buf[n] = L'\\';
						buf[n+1] = L'\0';
				}
				return AR_SetString(str, buf);
		}else
		{
				DWORD last_error = GetLastError();
				return __map_last_error(last_error);
		}
}


arStatus_t		AR_GetExpandPath(const wchar_t *path, arString_t *expanded_path)
{
		AR_ASSERT(path != NULL && expanded_path != NULL);

		return AR_E_NOTSUPPORTED;

}



arStatus_t		AR_GetRealPath(const wchar_t *path, arString_t *full_path)
{
		AR_UNUSED(path);
		AR_UNUSED(full_path);
		return AR_E_NOTSUPPORTED;
}


#else

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
				DWORD last_error = GetLastError();
				status = __map_last_error(last_error);
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
				DWORD last_error = GetLastError();
				AR_DEL(tmp);
				tmp = NULL;
				status = __map_last_error(last_error);
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
		wchar_t tmp[MAX_PATH_LEN + 10];
		arStatus_t status;
		DWORD n;
		AR_ASSERT(str != NULL);

		status = AR_S_YES;
		n = GetTempPathW(MAX_PATH_LEN, tmp);
		if(n <= 0)
		{
				DWORD last_error = GetLastError();
				return __map_last_error(last_error);
		}

		n = GetLongPathNameW(tmp, tmp, n);

		if(n <= 0)
		{
				DWORD last_error = GetLastError();
				return __map_last_error(last_error);
		}

		if(tmp[n - 1] != L'\\')
		{
				tmp[n] = L'\\';
				tmp[n+1] = L'\0';
		}

		return AR_SetString(str, tmp);

}


arStatus_t		AR_GetExpandPath(const wchar_t *path, arString_t *expanded_path)
{
		wchar_t *tmp;
		DWORD n;
		AR_ASSERT(path != NULL && expanded_path != NULL);

		n = ExpandEnvironmentStringsW(path, NULL, 0);

		if(n <= 0)
		{
				DWORD last_error = GetLastError();
				return __map_last_error(last_error);
		}

		tmp = AR_NEWARR(wchar_t, n);
		
		if(tmp == NULL)
		{
				return AR_E_NOMEM;
		}

		n = ExpandEnvironmentStringsW(path, tmp, n);

		if(n <= 0)
		{
				DWORD last_error = GetLastError();
				
				AR_DEL(tmp);
				tmp = NULL;
				

				return __map_last_error(last_error);
		}else
		{
				arStatus_t status;
				status = AR_SetString(expanded_path, tmp);
				AR_DEL(tmp);
				tmp = NULL;
				return status;
		}
}


arStatus_t		AR_GetRealPath(const wchar_t *path, arString_t *full_path)
{
		arStatus_t status;
		wchar_t *buf;
		DWORD l;
		AR_ASSERT(path != NULL && full_path != NULL);
		status = AR_S_YES;
		buf = AR_NEWARR(wchar_t, 4096);
		if(buf == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}

		l = GetFullPathNameW(path, 4096, buf, NULL);

		if(l <= 0)
		{
				DWORD err_code = GetLastError();
				status = __map_last_error(err_code);
				goto END_POINT;
		}else
		{
				buf[l] = L'\0';
		}

		status = AR_SetString(full_path, buf);

END_POINT:

		if(buf)
		{
				AR_DEL(buf);
				buf = NULL;
		}
		return status;
}


#endif

arStatus_t		AR_GetNullPath(arString_t *str)
{
		AR_ASSERT(str != NULL);
		return AR_SetString(str, L"NUL:");
}



/*********************************Path Iterator**************************/

struct __arsenal_path_iterator_tag
{
		ar_bool_t				isdone;
		HANDLE				hdl;
		WIN32_FIND_DATAW	find_data;
		arString_t			*current;
		wchar_t				*path;
};

arPathIter_t*	AR_CreatePathIterator()
{
		arPathIter_t *iter;
		iter = AR_NEW0(arPathIter_t);
		iter->hdl = INVALID_HANDLE_VALUE;
		return iter;
}


static void __clear_path_iterator(arPathIter_t *iter)
{
		AR_ASSERT(iter != NULL);
		if(iter->hdl != INVALID_HANDLE_VALUE && iter->hdl != NULL)
		{
				FindClose(iter->hdl);
				iter->hdl = INVALID_HANDLE_VALUE;
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
		wchar_t *tmp;
		arString_t *expanded_path, *absolute_path;
		arStatus_t status;
		AR_ASSERT(iter != NULL && path != NULL);

		tmp = NULL;
		status = AR_S_YES;
		__clear_path_iterator(iter);

		expanded_path = AR_CreateString();
		absolute_path = AR_CreateString();

		if(expanded_path == NULL || absolute_path == NULL)
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

		if(AR_GetStringChar(absolute_path, AR_GetStringLength(absolute_path) - 1) != L'\\')
		{
				status = AR_AppendString(absolute_path, L"\\");
				if(status != AR_S_YES)
				{
						goto END_POINT;
				}
		}


		tmp = AR_NEWARR(wchar_t , AR_GetStringLength(absolute_path) + 10);
		if(tmp == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}

		AR_wcscpy(tmp, AR_GetStringCString(absolute_path));
		
		

		AR_wcscat(tmp, L"*");

/****************************************************************************************/
		
		iter->hdl = FindFirstFileW(tmp, &iter->find_data);
		
		if(iter->hdl == INVALID_HANDLE_VALUE)
		{
				DWORD last_error = GetLastError();
				status = __map_last_error(last_error);
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

		status = AR_SetString(iter->current, iter->find_data.cFileName);
		
		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

		if(AR_CompStringWithWcs(iter->current, L".") == 0 || AR_CompStringWithWcs(iter->current, L"..") == 0)
		{
				status = AR_PathIteratorNext(iter);
				if(status == AR_S_YES || status == AR_E_NOMORE)
				{
						
				}else
				{
						goto END_POINT;
				}
		}

		


END_POINT:

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

		if(tmp != NULL)
		{
				AR_DEL(tmp);
				tmp = NULL;
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
		AR_ASSERT(iter->current != NULL && iter->hdl != INVALID_HANDLE_VALUE && iter->path != NULL);
		return AR_GetStringCString(iter->current);
}


arStatus_t		AR_PathIteratorNext(arPathIter_t *iter)
{
		arStatus_t status;
		AR_ASSERT(iter != NULL);
		AR_ASSERT(iter->current != NULL && iter->hdl != INVALID_HANDLE_VALUE && iter->path != NULL);

		if(iter->hdl == INVALID_HANDLE_VALUE || iter->hdl == NULL)
		{
				return AR_E_NOTREADY;
		}

		status = AR_S_YES;

		do{
				AR_ClearString(iter->current);
				
				if(FindNextFileW(iter->hdl, &iter->find_data) != 0)
				{
						status = AR_SetString(iter->current, iter->find_data.cFileName);
						if(status != AR_S_YES)
						{
								iter->isdone = true;
						}
				}else
				{
						if(GetLastError() == ERROR_NO_MORE_FILES)
						{
								status = AR_E_NOMORE;
						}else
						{
								DWORD last_error = GetLastError();
								status = __map_last_error(last_error);
						}

						iter->isdone = true;
				}
		
		}while(status == AR_S_YES && (AR_CompStringWithWcs(iter->current, L".") == 0 || AR_CompStringWithWcs(iter->current, L"..") == 0));

		return status;
}

ar_bool_t		AR_PathIteratorIsDone(const arPathIter_t *iter)
{
		AR_ASSERT(iter != NULL);
		AR_ASSERT(iter->current != NULL && iter->hdl != INVALID_HANDLE_VALUE && iter->path != NULL);
		return iter->isdone;
}

const wchar_t*  AR_PathIteratorPath(const arPathIter_t *iter)
{
		AR_ASSERT(iter != NULL && iter->path != NULL);
		AR_ASSERT(iter->current != NULL && iter->hdl != INVALID_HANDLE_VALUE && iter->path != NULL);
		return iter->path;
}

/*************************************************************File********************************************/

arStatus_t				AR_open_file(arFile_t **pfile, const wchar_t *path, const wchar_t *mode)
{
		arStatus_t		status;
		AR_ASSERT(pfile != NULL);
		AR_ASSERT(path != NULL && mode != NULL);

		status = AR_S_YES;

		*pfile = _wfopen(path, mode);

		if(*pfile == NULL)
		{
				DWORD last_error = GetLastError();
				status = __map_last_error(last_error);
		}
		
		return status;
}

void					AR_close_file(arFile_t *f)
{
		FILE *file;
		AR_ASSERT(f != NULL);
		file = (FILE*)f;
		fclose(file);
}


arStatus_t		AR_read_file(arFile_t *file, ar_byte_t *data, size_t len, size_t *rn)
{
		size_t ret;
		arStatus_t status;
		AR_ASSERT(file != NULL && data != NULL && len > 0);

		status = AR_S_YES;
		if(rn)
		{
				*rn = 0;
		}

		ret = fread((void*)data, 1, (size_t)len, (FILE*)file);

		/*
		if(ret != len)
		{
                int err_code = errno;
				status = __map_last_error(err_code);
		}else
		{
				status = AR_S_YES;
		}
		*/

		if(rn)
		{
				*rn = ret;
		}

		return status;
}


arStatus_t		AR_write_file(arFile_t *file, const ar_byte_t *data, size_t len, size_t *wn)
{
		
		size_t ret;
		arStatus_t status;
		AR_ASSERT(file != NULL && data != NULL && len > 0);

		status = AR_S_YES;

		ret = fwrite((const void*)data, 1, (size_t)len, (FILE*)file);

		
		if(ret != len)
		{
                int errcode = GetLastError();
				status = __map_last_error(errcode);
		}else
		{
				status = AR_S_YES;
		}
		

		if(wn)
		{
				*wn = ret;
		}
		return status;
}


arStatus_t				AR_seek_file(arFile_t *file, ar_int_64_t offset, arFileSeekType_t where)
{
		int origin,ret;

		AR_ASSERT(file != NULL);

		switch(where)
		{
		case AR_FILE_SEEK_BEG:
				origin = SEEK_SET;
				break;
		case AR_FILE_SEEK_CUR:
				origin = SEEK_CUR;
				break;
		case AR_FILE_SEEK_END:
				origin = SEEK_END;
				break;
		default:
				return AR_E_INVAL;
		}

		ret = _fseeki64((FILE*)file, (__int64)offset, origin);

		if(ret == 0)
		{
				return AR_S_YES;
		}else
		{
				int errcode = GetLastError();
				return __map_last_error(errcode);
		}
}

arStatus_t				AR_tell_file(arFile_t *file, ar_uint_64_t *offset)
{
		__int64 ret;
		AR_ASSERT(file != NULL && offset != NULL);

		ret = _ftelli64((FILE*)file);
  
		if(ret < 0)
		{
				int errcode = GetLastError();
				return __map_last_error(errcode);
		}else
		{
				*offset = (ar_uint_64_t)ret;
				return AR_S_YES;
		}
}


AR_NAMESPACE_END


#endif
