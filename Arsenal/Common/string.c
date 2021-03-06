﻿/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
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


/********************************************************String*****************************************************************/


struct __arsenal_string_tag
{
		wchar_t			*str;
		size_t			count;
		size_t			cap;
};

arString_t*	AR_CreateString()
{
		arString_t *str;
		str = AR_NEW0(arString_t);

		if(str == NULL)
		{
				return NULL;
		}

		
		if(AR_ReserveString(str, 8) != AR_S_YES)
		{
				AR_DEL(str);
				str = NULL;
		}

		return str;
}


void	AR_DestroyString(arString_t *str)
{
		AR_ASSERT(str != NULL);
		if(str->str)
		{
				AR_DEL(str->str);
		}
		AR_DEL(str);
}


arStatus_t		AR_SetString(arString_t *str, const wchar_t *wcs)
{
		AR_ASSERT(str != NULL && wcs != NULL);
		AR_ClearString(str);
		return AR_AppendString(str, wcs);
}

arStatus_t		AR_SetStringN(arString_t *str, const wchar_t *wcs, size_t n)
{
		AR_ASSERT(str != NULL && wcs != NULL);
		AR_ClearString(str);
		return AR_AppendStringN(str, wcs, n);
}

arStatus_t		AR_CopyString(arString_t *src, const arString_t *dest)
{
		AR_ASSERT(src != NULL && dest != NULL);
		return AR_SetString(src, AR_GetStringCString(dest));
}

arString_t*		AR_CopyNewString(arString_t *src)
{
		arString_t *new_str;
		AR_ASSERT(src != NULL);
		new_str = AR_CreateString();
		if(new_str == NULL)
		{
				return NULL;
		}

		if(AR_CopyString(new_str, src) != AR_S_YES)
		{
				AR_DestroyString(new_str);
				new_str = NULL;
				return NULL;
		}

		return new_str;
}

const wchar_t*	AR_GetStringCString(const arString_t *str)
{
		AR_ASSERT(str != NULL && str->str != NULL);
		return str->str;
}

size_t	AR_GetStringLength(const arString_t *str)
{
		AR_ASSERT(str != NULL && str->str != NULL);
		return str->count;
}


#if(0)
arStatus_t			AR_ReserveString(arString_t *str, size_t num)
{
		AR_ASSERT(str != NULL);
		num += 1;

		if(str->cap - str->count < num)
		{
				wchar_t *new_tmp;
				size_t	new_cap;

				new_cap = (str->count + num)*2;
				new_tmp = AR_NEWARR0(wchar_t, new_cap);

				if(new_tmp == NULL)
				{
						return AR_E_NOMEM;
				}

				if(str->str)
				{
						AR_wcscpy(new_tmp, str->str);
						AR_DEL(str->str);
						str->str = NULL;
				}else
				{
						new_tmp[0] = L'\0';
				}

				str->str = new_tmp;
				str->cap = new_cap;

				AR_ASSERT(AR_wcslen(new_tmp) == str->count);
		}

		return AR_S_YES;

}
#endif


arStatus_t			AR_ReserveString(arString_t *str, size_t num)
{
		AR_ASSERT(str != NULL);
		num += 1;

		if(str->cap - str->count < num)
		{
				
				size_t	new_cap;
				arStatus_t status;
				new_cap = (size_t)((double)(str->count + num) * 1.5);

				AR_DO_REALLOC(wchar_t, str->str, new_cap, str->count, status);

				if(status != AR_S_YES)
				{
						return status;
				}

				str->str[str->count] = L'\0';
				
				str->cap = new_cap;

				AR_ASSERT(AR_wcslen(str->str) == str->count);
		}

		return AR_S_YES;

}


void			AR_ClearString(arString_t *str)
{
		AR_ASSERT(str != NULL);
		str->str[0] = L'\0';
		str->count = 0;
}

arStatus_t	AR_AppendString(arString_t *str, const wchar_t *sour)
{
		size_t len;
		size_t i;
		arStatus_t ret;
		AR_ASSERT(str != NULL && sour != NULL);
		len = AR_wcslen(sour);
		
		ret = AR_ReserveString(str,len);
		if(ret != AR_S_YES)
		{
				return ret;
		}

		
		for(i = 0; i < len && sour[i] != L'\0'; ++i)
		{
				str->str[str->count++] = sour[i];
		}
		str->str[str->count] = L'\0';
		
		return ret;

}

arStatus_t		AR_AppendStringN(arString_t *str, const wchar_t *sour, size_t n)
{
		size_t i;
		arStatus_t ret;
		AR_ASSERT(str != NULL && sour != NULL);

		if(n == 0)
		{
				return AR_S_YES;
		}

		
		ret = AR_ReserveString(str,n);

		if(ret != AR_S_YES)
		{
				return ret;
		}

		
		for(i = 0; i < n && sour[i] != L'\0'; ++i)
		{
				str->str[str->count++] = sour[i];
		}
		str->str[str->count] = L'\0';
		
		return ret;
}







arStatus_t	AR_FormatString(arString_t *str, const wchar_t *fmt, ...)
{
		arStatus_t ret;
		va_list args;
		AR_ASSERT(str != NULL && fmt != NULL);

		AR_va_start(args, fmt);
		ret = AR_VFormatString(str, fmt, args);
		AR_va_end(args);
		return ret;
}




arStatus_t	AR_AppendFormatString(arString_t *str, const wchar_t *fmt, ...)
{
		arStatus_t ret;
		va_list args;
		AR_ASSERT(str != NULL && fmt != NULL);
		
		AR_va_start(args, fmt);
		ret = AR_AppendVFormatString(str, fmt, args);
		AR_va_end(args);
		return ret;
}



arStatus_t			AR_VFormatString(arString_t *str, const wchar_t *fmt, va_list va_args)
{
		arStatus_t ret;
		ar_int_t len;
		va_list save;
		AR_ASSERT(str != NULL && fmt != NULL);
		AR_ClearString(str);
		
		AR_va_copy(save, va_args);
		len = AR_vscwprintf(fmt, save);
		AR_va_end(save);

		if(len < 0)
		{
				return AR_E_INVAL;
		}
		
		ret = AR_ReserveString(str, (size_t)len + 1);
		if(ret != AR_S_YES)
		{
				return ret;
		}
		
		str->str[0] = L'\0';
		
		AR_va_copy(save, va_args);
		len = AR_vswprintf(str->str, str->cap-1, fmt, save) + 1;
		AR_va_end(save);
		
		if(len < 0)
		{
				return AR_E_INVAL;
		}

		
		str->str[len] = L'\0';
		str->count = AR_wcslen(str->str);
		return AR_S_YES;
}


arStatus_t			AR_AppendVFormatString(arString_t *str, const wchar_t *fmt, va_list va_args)
{
		arStatus_t ret;
		ar_int_t len;
		wchar_t *buf;
		va_list save;
		AR_ASSERT(str != NULL && fmt != NULL);

		buf = NULL;
		ret = AR_S_YES;
	
		AR_va_copy(save, va_args);
		len = AR_vscwprintf(fmt, save);
		AR_va_end(save);

		if(len < 0)
		{
				ret = AR_E_INVAL;
				goto END_POINT;
		}
		

		buf = AR_NEWARR0(wchar_t, len + 1);

		if(buf == NULL)
		{
				ret = AR_E_NOMEM;
				goto END_POINT;
		}
		
		AR_va_copy(save, va_args);
		len = AR_vswprintf(buf, len + 1, fmt, save);
		AR_va_end(save);

		if(len < 0)
		{
				ret = AR_E_INVAL;
				goto END_POINT;
		}

		buf[len] = L'\0';
		
		ret = AR_AppendString(str, buf);
		if(ret != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(buf)
		{
				AR_DEL(buf);
				buf = NULL;
		}
		
		return ret;
}


arStatus_t			AR_AppendCharToString(arString_t *str, wchar_t chr)
{
		arStatus_t ret;
		AR_ASSERT(str != NULL);

		if(chr == L'\0')
		{
				return AR_S_YES;
		}

		ret = AR_ReserveString(str, 1);
		if(ret != AR_S_YES)
		{
				return ret;
		}

		str->str[str->count++] = chr;
		str->str[str->count] = L'\0';
		return AR_S_YES;
}

wchar_t			AR_GetStringChar(const arString_t *str, size_t index)
{
		AR_ASSERT(str != NULL);
		AR_ASSERT(index < AR_GetStringLength(str));
		return AR_GetStringCString(str)[index];
}

void			AR_SetStringChar(arString_t *str, size_t index, wchar_t c)
{
		AR_ASSERT(str != NULL);
		AR_ASSERT(index < AR_GetStringLength(str));
		
		str->str[index] = c;
}

ar_int_t			AR_CompStringWithWcs(const arString_t *l, const wchar_t *r)
{
		AR_ASSERT(l != NULL && r != NULL);
		return AR_wcscmp(AR_GetStringCString(l), r);
}

ar_int_t			AR_CompString(const arString_t *l, const arString_t *r)
{
		AR_ASSERT(l != NULL && r != NULL);
		return AR_wcscmp(AR_GetStringCString(l), AR_GetStringCString(r));
}

ar_int_t			AR_ICompStringWithWcs(const arString_t *l, const wchar_t *r)
{
		AR_ASSERT(l != NULL && r != NULL);
		return AR_wcsicmp(AR_GetStringCString(l), r);
}

ar_int_t			AR_ICompString(const arString_t *l, const arString_t *r)
{
		AR_ASSERT(l != NULL && r != NULL);
		return AR_wcsicmp(AR_GetStringCString(l), AR_GetStringCString(r));
}

void			AR_SwapString(arString_t *l, arString_t *r)
{
		arString_t tmp;
		AR_ASSERT(l != NULL && r != NULL);
		tmp = *l;
		*l = *r;
		*r = tmp;
}

void			AR_StringToLower(arString_t *str)
{
		size_t i;
		AR_ASSERT(str != NULL);
		for(i = 0; i < str->count; ++i)
		{
				str->str[i] = AR_towlower(str->str[i]);
		}

}

void			AR_StringToUpper(arString_t *str)
{
		size_t i;
		AR_ASSERT(str != NULL);
		for(i = 0; i < str->count; ++i)
		{
				str->str[i] = AR_towupper(str->str[i]);
		}
}


size_t			AR_EraseString(arString_t *str, size_t pos, size_t nchars)
{
		size_t rm_len = 0;
		AR_ASSERT(str != NULL);
		AR_ASSERT(pos < str->count);

		if(nchars == 0)
				return 0;

		rm_len = AR_MIN(str->count - pos, nchars);
		
		str->count -= rm_len;

		AR_memmove(str->str + pos, str->str + pos + rm_len, sizeof(wchar_t) * str->count);

		str->str[str->count] = L'\0';

		return rm_len;
}


static const wchar_t line_terminator[] = 
{
		L'\x000A',		//Line Feed
		L'\x000B',		//Vertical Tab
		L'\x000C',		//Form Feed
		L'\x000D',		//Carriage Return
		L'\x0085',		//Next Line
		L'\x2028',		//Line Separator
		L'\x2029',		//Paragraph Separator
		L'\0'
};

static wchar_t* __find_line_terminator(const wchar_t *s, size_t *line_term_len)
{
		AR_ASSERT(s != NULL);
		AR_ASSERT(line_term_len != NULL);

		while(*s)
		{
				if(AR_wcschr(line_terminator, *s) != NULL)
				{
						*line_term_len = 1;

						if(*s == L'\x000D' && *(s + 1) == L'\x000A')
						{
								*line_term_len = *line_term_len + 1;
						}

						return (wchar_t*)s;
								
				}

				++s;
		}
		*line_term_len = 0;
		return (wchar_t*)s;
}


arStatus_t         AR_GetLineFromString(arString_t *str, arString_t *line)
{
        size_t avail;
        const wchar_t *b, *p, *e;
        size_t need_n, line_sp_len;
        
        AR_ASSERT(str != NULL);
        
		AR_ClearString(line);

		
        
        avail = str->count;
        line_sp_len = 0;
        
        if(avail == 0)
        {
				return AR_E_NOTMATCHED;
        }
        
        b = str->str;
        e = b + avail;
        need_n = 0;
        
        p = __find_line_terminator(b, &line_sp_len);
        
		AR_ASSERT(p != NULL);
        
        need_n = p - b;
        
        if(need_n == 0)	/*empty line*/
        {
                AR_EraseString(str, 0, need_n + line_sp_len);
                return AR_S_YES;
        }
        
        if(line == NULL)
        {
				/*empty line*/
                return AR_S_YES;
        }else
        {
				arStatus_t status = AR_S_YES;
				
				status = AR_SetStringN(line, b, need_n);
				AR_ASSERT(avail >= need_n + line_sp_len);
                AR_EraseString(str, 0, need_n + line_sp_len); /*抹掉\r\n*/
                
				return status;
        }
}

arStatus_t      AR_GetLineFromStringWithLineSP(arString_t *str, arString_t *line, const wchar_t *line_sp)
{
		size_t avail;
        const wchar_t *b, *p, *e;
        size_t need_n, line_sp_len;
        
        AR_ASSERT(str != NULL);
        AR_ASSERT(line_sp != NULL);
		AR_ClearString(line);

		
        
        avail = str->count;
        line_sp_len = AR_wcslen(line_sp);
        AR_ASSERT(line_sp_len > 0);

        if(avail == 0)
        {
				return AR_E_NOTMATCHED;
        }
        
        b = str->str;
        e = b + avail;
        need_n = 0;
        
        p = AR_wcsstr_s(b, e, line_sp, line_sp + line_sp_len);
        
		if(p == 0)
		{
				need_n = e - b;
				line_sp_len = 0;
		}else
		{
				need_n = p - b;
		}
        
        if(need_n == 0)	/*empty line*/
        {
                AR_EraseString(str, 0, need_n + line_sp_len);
                return AR_S_YES;
        }
        
        if(line == NULL)
        {
				/*empty line*/
                return AR_S_YES;
        }else
        {
				arStatus_t status = AR_S_YES;
				
				status = AR_SetStringN(line, b, need_n);
				AR_ASSERT(avail >= need_n + line_sp_len);
                AR_EraseString(str, 0, need_n + line_sp_len); /*抹掉\r\n*/
                
				return status;
        }
}


AR_NAMESPACE_END
