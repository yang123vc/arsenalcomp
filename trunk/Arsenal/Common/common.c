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

/******************************************************Print**************************************************************/

AR_NAMESPACE_BEGIN


/*******************************************************************************init********************************/


void AR_STDCALL AR_def_error(int level, const wchar_t* msg)
{
		wprintf(L"%d:%s", level, msg);
}


void AR_STDCALL AR_def_print(const wchar_t *msg)
{
		wprintf(L"%s", msg);

}



static arInit_t	__ctx = { AR_def_error, AR_def_print};

void AR_Init(const arInit_t *info)
{
		__ctx = *info;

}

void AR_UnInit()
{


}





void AR_error(int level,  const wchar_t *msg, ...)
{
		wchar_t *buf;
		int len;
		va_list arg_ptr;
		
		if(__ctx.error_f != NULL)
		{
				
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);

				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
		
				__ctx.error_f(level, buf);
		
				AR_DEL(buf);
		}
}



void AR_printf(const wchar_t *msg,...)
{
		wchar_t *buf;
		int len;
		va_list arg_ptr;
		if(__ctx.print_f != NULL)
		{
		
				
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);

				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
		
				__ctx.print_f(buf);
		
				AR_DEL(buf);

		}

}




/**********************************************************string*************************************************************/


const wchar_t* AR_wcstrim(const wchar_t *in, const wchar_t *trim)
{
		AR_ASSERT(in != NULL && trim != NULL);
		
		while(*in != L'\0' && AR_wcschr(trim, *in) != NULL)in++;
		return in;
}


const wchar_t* AR_wtoi64(const wchar_t *in,	 int64_t  *num, size_t base)
{
		uint64_t val;
		const wchar_t *p;
		bool is_neg;

		p = AR_wcstrim(in, L" ");
		is_neg = false;
		if(*p == L'-')
		{
				is_neg = true;
				++p;
		}
		p = AR_wtou64(p, &val, base);
		*num = (int64_t)val;
		if(is_neg)*num = -(*num);
		return p;
}

const wchar_t* AR_wtou64(const wchar_t *in,	 uint64_t  *num, size_t base)
{
		int64_t val;
		const wchar_t *p;
		bool is_ok;
		uint64_t tmp;
		AR_ASSERT(in != NULL && num != NULL && base > 0 && base <= 16);
		
		is_ok = false; 
		p = AR_wcstrim(in,L" ");
		
		val = 0;

		while(*p != L'\0')
		{
				if(*p >= L'0' && *p <= L'9')
				{
						tmp = (*p - L'0');
				}else if(*p >= L'A' && *p <= L'F')
				{
						tmp = ((*p - L'A') + 10);
				}else if(*p >= L'a' && *p <= L'f')
				{
						tmp = ((*p - L'a') + 10);
				}else
				{
						break;
				}
				if(tmp >= base)break;

				val *= base;
				val += tmp;
				p++;
				is_ok = true;
		}
		
		if(!is_ok)return NULL;
		*num = val;
		return p;
}

const wchar_t* AR_wtoi32(const wchar_t *in,  int32_t *num, size_t base)
{
		int64_t val;
		const wchar_t *p;
		p = AR_wtoi64(in, &val, base);
		*num = (int32_t)val;
		return p;
}

const wchar_t* AR_wtou32(const wchar_t *in,  uint32_t *num, size_t base)
{
		uint64_t val;
		const wchar_t *p;
		p = AR_wtou64(in, &val, base);
		*num = (uint32_t)val;
		return p;
}


uint_t	AR_wcshash(const wchar_t *str)
{
		uint_t	ret;
		size_t	i,n;
		AR_ASSERT(str != NULL);
		n = AR_wcslen(str);
		if(n == 0)return 0;
		
		ret = 0;
		for(i = 0; i < n; ++i)
		{
				ret ^= (str[i] << (i & 0x0F));
		}
		return ret;
}

uint_t		AR_wcshash_n(const wchar_t *str, size_t n)
{
		uint_t	ret;
		size_t	i;
		if(n == 0)return 0;
		AR_ASSERT(str != NULL);
		
		ret = 0;
		for(i = 0; i < n; ++i)
		{
				ret ^= (str[i] << (i & 0x0F));
		}
		return ret;
}


wchar_t* AR_wcsdup(const wchar_t *sour)
{
		AR_ASSERT(sour != NULL);
		return AR_wcsndup(sour, AR_wcslen(sour));
}


wchar_t* AR_wcsndup(const wchar_t *sour, size_t len)
{
		wchar_t *result;
		AR_ASSERT(sour != NULL && len > 0);
		result = AR_NEWARR(wchar_t, len + 1);
		AR_wcsncpy(result, sour, len);
		result[len] = L'\0';
		return result;
}


AR_NAMESPACE_END