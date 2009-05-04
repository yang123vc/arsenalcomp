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
#ifndef __AR_SPEC_H__
#define __AR_SPEC_H__


#include <stddef.h>
#include <stdarg.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <wchar.h>
#include "config.h"



#if (AR_COMPILER == AR_VC6)

#define AR_swprintf						_snwprintf
#define AR_vsprintf(buf, nelems, fmt, arg_ptr)		do{vsprintf(buf, fmt, arg_ptr);}while(0)
#define AR_vswprintf								_vsnwprintf


inline int AR_vscwprintf(const wchar_t *fmt, va_list argptr) { int n; size_t buf_size; n = -1; buf_size = 1048576; while(n == -1){ wchar_t *buf; buf_size *= 2; buf = (wchar_t*)malloc(sizeof(wchar_t) * buf_size); n = AR_vswprintf(buf, buf_size, fmt, argptr); free(buf); } return n; }

#elif (AR_COMPILER == AR_VC9)

#define AR_swprintf						swprintf
#define AR_vsprintf						vsprintf_s
#define AR_vswprintf					vswprintf_s

#define AR_vscwprintf					_vscwprintf



#elif(AR_COMPILER == AR_BCB6)

/*#define AR_swprintf						swprintf*/
#define AR_sprintf		sprintf
#define AR_vsprintf(buf, count, fmt, args)	        vsprintf(buf, fmt, args)
#define AR_vswprintf(buf, count, fmt, args)	        vswprintf(buf, fmt, args)

#define AR_sscanf		sscanf
#define AR_swscanf		swscanf



inline int AR_swprintf(wchar_t *buffer,  size_t count, const wchar_t *fmt, ...)
{
		int res;
		va_list  arg_ptr;
		va_start(arg_ptr, fmt);
		res = vwprintf(fmt, arg_ptr);
		va_end(arg_ptr);
		return res;
}

/*这里只能先凑合凑合，因为各个编译器的printf一族的函数标准实在是太不一样了，这东西我应该自己实现一套才对，所以只能先凑合凑合了*/

inline int AR_vscwprintf(const wchar_t *fmt, va_list argptr) 
{ 
		int n; size_t buf_size; n = -1; buf_size = 1048576; 
		while(n == -1)
		{ 
				wchar_t *buf; 
				buf_size *= 2; 
				buf = (wchar_t*)malloc(sizeof(wchar_t) * buf_size); 
				n = AR_vswprintf(buf, buf_size, fmt, argptr); 
				free(buf); 
		}
		return n; 
}


#else


#endif















#endif

