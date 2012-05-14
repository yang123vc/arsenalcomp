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



#ifndef MAX_PATH_LEN
#undef MAX_PATH_LEN
#endif


#define MAX_PATH_LEN 32767

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
				return AR_E_FAIL;
		}
}


arStatus_t		AR_GetExpandPath(const wchar_t *path, arString_t *expanded_path)
{
		AR_ASSERT(path != NULL && expanded_path != NULL);

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
		wchar_t tmp[MAX_PATH_LEN + 10];
		arStatus_t status;
		DWORD n;
		AR_ASSERT(str != NULL);

		status = AR_S_YES;
		n = GetTempPathW(MAX_PATH_LEN, tmp);
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


arStatus_t		AR_GetExpandPath(const wchar_t *path, arString_t *expanded_path)
{
		wchar_t tmp[MAX_PATH_LEN + 10];
		arStatus_t status;
		DWORD n;
		AR_ASSERT(path != NULL && expanded_path != NULL);

		status = AR_S_YES;
		n = ExpandEnvironmentStringsW(path, tmp, MAX_PATH_LEN);
		if(n <= 0 || n > MAX_PATH_LEN - 1)
		{
				return AR_SetString(expanded_path, path);
		}else
		{
				return AR_SetString(expanded_path, tmp);
		}
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
		bool_t				isdone;
		HANDLE				hdl;
		WIN32_FIND_DATAW	find_data;
		arString_t			*current;
		wchar_t				*path;
};

arPathIter_t*	AR_CreatePathIterator(const wchar_t *path)
{
		arPathIter_t *iter;
		wchar_t *tmp;
		arString_t *expanded_path;
		arStatus_t status;
		AR_ASSERT(path != NULL);

		tmp = NULL;
		iter = NULL;
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

		if(AR_GetStringChar(expanded_path, AR_GetStringLength(expanded_path) - 1) != L'\\')
		{
				status = AR_AppendString(expanded_path, L"\\");
				if(status != AR_S_YES)
				{
						goto END_POINT;
				}
		}


		tmp = AR_NEWARR(wchar_t , AR_GetStringLength(expanded_path) + 10);
		if(tmp == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}

		AR_wcscpy(tmp, AR_GetStringCString(expanded_path));
		
		

		AR_wcscat(tmp, L"*");


		iter = AR_NEW0(arPathIter_t);
        
        if(iter == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
		
		iter->hdl = FindFirstFileW(tmp, &iter->find_data);
		
		if(iter->hdl == INVALID_HANDLE_VALUE)
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

		status = AR_SetString(iter->current, iter->find_data.cFileName);

		if(AR_CompStringWithWcs(iter->current, L".") == 0 || AR_CompStringWithWcs(iter->current, L"..") == 0)
		{
				status = AR_PathIteratorNext(iter);
				if(status == AR_S_YES || status == AR_S_NO)
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

		if(tmp != NULL)
		{
				AR_DEL(tmp);
				tmp = NULL;
		}

		if(status == AR_S_YES || status == AR_S_NO)
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
				AR_ClearString(iter->current);
				
				if(FindNextFileW(iter->hdl, &iter->find_data) != 0)
				{
						status = AR_SetString(iter->current, iter->find_data.cFileName);
				}else
				{
						if(GetLastError() == ERROR_NO_MORE_FILES)
						{
								status = AR_S_NO;
						}else
						{
								status = AR_E_FAIL;
						}

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


/*************************************************************File********************************************/


FILE*	AR_open_file(const wchar_t *path, const wchar_t *mode)
{
		AR_ASSERT(path != NULL && mode != NULL);
		return _wfopen(path, mode);
}

void	AR_close_file(FILE *f)
{
		AR_ASSERT(f != NULL);
		fclose(f);
}

AR_NAMESPACE_END

