/*
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
		
		AR_ReserveString(str, 8);
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



const wchar_t*	AR_GetStrString(const arString_t *str)
{
		AR_ASSERT(str != NULL && str->str != NULL);
		return str->str;
}

size_t	AR_GetLengthString(const arString_t *str)
{
		AR_ASSERT(str != NULL && str->str != NULL);
		return str->count;
}

void			AR_ReserveString(arString_t *str, size_t num)
{
		
		size_t i;
		
		AR_ASSERT(str != NULL);
		num += 1;

		if(str->cap - str->count < num)
		{
				str->cap = (str->count + num)*2;
				str->str = AR_REALLOC(wchar_t, str->str, str->cap);
				for(i = str->count; i < str->cap; ++i)
				{
						str->str[i] = L'\0';
				}
		}
}

void			AR_ClearString(arString_t *str)
{
		AR_ASSERT(str != NULL);
		str->str[0] = L'\0';
		str->count = 0;
}

size_t	AR_AppendString(arString_t *str, const wchar_t *sour)
{
		size_t len;
		size_t i;
		AR_ASSERT(str != NULL && sour != NULL);
		len = AR_wcslen(sour);
		AR_ReserveString(str,len);
		
		for(i = 0; i < len; ++i)
		{
				str->str[str->count++] = sour[i];
		}
		str->str[str->count] = L'\0';
		return len;
}








void	AR_FormatString(arString_t *str, const wchar_t *fmt, ...)
{
		int_t len;
		va_list args;
		AR_ASSERT(str != NULL && fmt != NULL);
		AR_ClearString(str);
		va_start(args, fmt);
		len = AR_vscwprintf(fmt, args);
		AR_ASSERT(len >= 0);
		va_end(args);
		AR_ASSERT(len > 0);
		
		AR_ReserveString(str, (size_t)len);
		
		va_start(args, fmt);
		str->str[0] = L'\0';
		len = AR_vswprintf(str->str, str->cap-1, fmt, args) + 1;
		va_end(args);
		str->str[len] = L'\0';
		str->count = AR_wcslen(str->str);
}




void	AR_AppendFormatString(arString_t *str, const wchar_t *fmt, ...)
{
		int_t len;
		wchar_t *buf;
		va_list args;
		AR_ASSERT(str != NULL && fmt != NULL);
		
		va_start(args, fmt);
		len = AR_vscwprintf(fmt, args);
		va_end(args);
		AR_ASSERT(len >= 0);

		buf = AR_NEWARR0(wchar_t, len + 1);
		va_start(args, fmt);
		len = AR_vswprintf(buf, len + 1, fmt, args);
		va_end(args);
		buf[len] = L'\0';
		AR_AppendString(str, buf);
		AR_DEL(buf);
}


void			AR_AppendCharToString(arString_t *str, wchar_t chr)
{
		AR_ReserveString(str, 1);
		str->str[str->count++] = chr;
}






AR_NAMESPACE_END
