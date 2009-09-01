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

#ifndef AR_CONFIG_H
#define AR_CONFIG_H

#include "platform.h"




 

/**********************************************************确定编译器和平台***********************************************************************/


#define ARCH_32			32u
#define ARCH_64			64u


#if(ARCH == ARCH_IA32 || ARCH == ARCH_ARM)

#define AR_ARCH_VER		ARCH_32

#elif(ARCH == ARCH_X64 || ARCH == ARCH_IA64)

#define AR_ARCH_VER		ARCH_64

#else 

#error "UnKnow Platform!"

#endif




#define AR_VC6			1006
#define AR_VC9			1009
#define AR_BCB6			2006
#define AR_GCC3			3003
#define AR_GCC4			3004


#if defined(_MSC_VER)

		#if (_MSC_VER < 1300)

				#define AR_COMPILER		AR_VC6
		#else
				#define	AR_COMPILER		AR_VC9
		#endif

#elif defined(__BORLANDC__)

		#define AR_COMPILER		AR_BCB6

#endif


#if defined(__cplusplus) && !defined(__STDC_VERSION__)

		#define __STDC_VERSION__  0
#endif






/**********************************************************编译器相关的配置***********************************************************************/


/*
    配置编译器相关的选项
*/
#if(AR_COMPILER == AR_VC6 || AR_COMPILER == AR_VC9)

		#pragma warning(disable : 4204)
		#pragma warning(disable : 4244)
		#pragma warning(disable : 4127)
		#pragma warning(disable : 4100)
		#pragma warning(disable : 4201)
		#pragma warning(disable : 4505)
		#pragma warning(disable : 4514)
		#pragma warning(disable : 4710)
		#pragma warning(disable : 4761)
		
		#if !defined(_CRT_SECURE_NO_WARNINGS)
				#define _CRT_SECURE_NO_WARNINGS		
		#endif
#else



#endif



#if(AR_COMPILER == AR_VC6 || AR_COMPILER == AR_VC9 || AR_COMPILER == AR_BCB6)

#include <stdio.h>
#include <wchar.h>

#include <stddef.h>
#include <stdarg.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <math.h>

#else


#endif



#if(AR_COMPILER == AR_VC6 || AR_COMPILER == AR_VC9 || AR_COMPILER == AR_BCB6)

		#if defined(NDEBUG)
				#define AR_NDEBUG
		#else
				#define AR_DEBUG
		#endif
		

		#define AR_STDCALL		__stdcall
		#define AR_CCALL		__cdecl 


		#define AR_INT8_T	__int8
		#define AR_UINT8_T  unsigned __int8
		#define AR_INT32_T	int
		#define AR_UINT32_T unsigned int
		#define AR_INT64_T  __int64
		#define AR_UINT64_T unsigned __int64
		#define AR_CHAR_T	char
		#define AR_UCHAR_T	unsigned char
		#define AR_BYTE_T	AR_UINT8_T
		#define AR_WCHAR_T  wchar_t
		
		#define AR_BIGNUM(num)	(n)
		#define AR_FMT64		"I64"
		
		#if(AR_COMPILER == AR_VC6)
				#define AR_NOOP					NULL	
				#define AR_FUNC_NAME			"--"

		#elif (AR_COMPILER == AR_VC9)

				#define AR_NOOP					__noop
				#define AR_FUNC_NAME			__FUNCSIG__

		#elif (AR_COMPILER == AR_BCB6)
				#define AR_NOOP					NULL
				#define AR_FUNC_NAME			__FUNC__
		#endif


#else




#endif

		




#if defined(__cplusplus)
		#define AR_NAMESPACE_BEGIN		extern "C" { namespace Arsenal {
		#define AR_NAMESPACE_END		} }

#else
		#define AR_NAMESPACE_BEGIN 
		#define AR_NAMESPACE_END
		
		#define	bool					int
		#define	true					1
		#define false					0
		
		#if(__STDC_VERSION__ < 199901L)
				#define inline					static
		#endif
#endif



/*********************************平台相关***************************************/


#if(AR_ARCH_VER	== ARCH_32)

#define AR_PLAT_INT_T			AR_INT32_T
#define AR_PLAT_UINT_T	        AR_UINT32_T


#elif(AR_ARCH_VER == ARCH_64)


#define AR_PLAT_INT_T			AR_INT64_T
#define AR_PLAT_UINT_T	        AR_UINT64_T

#else


#endif







/************************平台无关的定义***********************************/


#ifndef WCHAR_MAX
#if defined(OS_FAMILY_UNIX)
		#define WCHAR_MAX		0xFFFFFFFF
#else
		#define WCHAR_MAX		0xFFFF
#endif

#endif

#ifndef ULLONG_MAX
		#define ULLONG_MAX		0xffffffffffffffffui64
#endif

#ifndef LLONG_MAX
		#define LLONG_MAX		9223372036854775807i64
#endif

#ifndef LLONG_MIN
		#define LLONG_MIN		(-9223372036854775807i64 - 1)
#endif


#define	AR_UINT32_MAX	UINT_MAX
#define AR_UINT64_MAX	ULLONG_MAX 

#define AR_INT32_MAX	LONG_MAX
#define AR_INT32_MIN	LONG_MIN

#define AR_INT64_MAX	LLONG_MAX
#define AR_INT64_MIN	LLONG_MIN



 



#define __AR_WIDE_STR(_String) L ## _String
#define AR_WSTR(_String) __AR_WIDE_STR(_String)


#define __AR_APPEND_STR(_Value1, _Value2) _Value1  _Value2
#define AR_APPSTR(_Value1, _Value2) __AR_APPEND_STR(_Value1, _Value2)


#define __AR_STRINGIZE_STR(_Value) #_Value
#define AR_TOSTR(_Value) __AR_STRINGIZE_STR(_Value)






typedef AR_INT8_T				int8_t;

typedef AR_UINT8_T				uint8_t;

typedef short					int16_t;

typedef unsigned short			uint16_t;

typedef AR_INT32_T				int32_t;

typedef AR_UINT32_T				uint32_t;

typedef AR_INT64_T				int64_t;

typedef AR_UINT64_T				uint64_t;

typedef AR_PLAT_INT_T			int_t;/*跟所在处理器等长的有符号整数*/

typedef AR_PLAT_UINT_T			uint_t;/*跟所在处理器等长的无符号整数*/

typedef uint8_t					byte_t;

typedef void*					ptr_t;


#define AR_CHARMAX				CHAR_MAX
#define AR_WCHARMAX				WCHAR_MAX
#define AR_BYTE_BITS			CHAR_BIT
#define AR_SIZE_MAX				((uint_t)(~((uint_t)0)))















/******************************************Spec***************************************************************/



#if (AR_COMPILER == AR_VC6)

#define AR_swprintf						_snwprintf
#define AR_vsprintf(buf, nelems, fmt, arg_ptr)		do{vsprintf(buf, fmt, arg_ptr);}while(0)
#define AR_vswprintf								_vsnwprintf

/*
已经不用了
inline int AR_vscwprintf(const wchar_t *fmt, va_list argptr) { int n; size_t buf_size; n = -1; buf_size = 1048576; while(n == -1){ wchar_t *buf; buf_size *= 2; buf = (wchar_t*)malloc(sizeof(wchar_t) * buf_size); n = AR_vswprintf(buf, buf_size, fmt, argptr); free(buf); } return n; }
*/
#elif (AR_COMPILER == AR_VC9)

#define AR_swprintf						_snwprintf
/*#define AR_swprintf						swprintf*/
#define AR_vsprintf						vsprintf_s
#define AR_vswprintf					vswprintf_s

/*
已经不用了
#define AR_vscwprintf					_vscwprintf
*/



#elif(AR_COMPILER == AR_BCB6)

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


/*
测试时有可能会用到，遗留代码
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
*/

#else

#error "Unknow Compiler!"

#endif


/*************************************************************************************************************/






#endif
