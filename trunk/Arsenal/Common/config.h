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


		
/*
    配置编译器相关的选项
*/
		//#define _CRT_SECURE_NO_WARNINGS	

		#pragma warning(disable : 4204)
		#pragma warning(disable : 4244)
		#pragma warning(disable : 4127)
		#pragma warning(disable : 4100)
		#pragma warning(disable : 4201)
		#pragma warning(disable : 4505)
		#pragma warning(disable : 4514)
		#pragma warning(disable : 4710)
		#pragma warning(disable : 4761)

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
		#define inline					static
#endif



/*********************************平台相关***************************************/


#if(ARCH == ARCH_IA32)

#define AR_PLAT_INT				AR_INT32_T
#define AR_PLAT_UINT	        AR_UINT32_T


#elif(ARCH == ARCH_IA64 || ARCH == ARCH_X64)

#define AR_PLAT_INT				AR_INT64_T
#define AR_PLAT_UINT	        AR_UINT64_T

#else


#endif







/************************平台无关的定义***********************************/


#if !defined(WCHAR_MAX)
		#define WCHAR_MAX		0xffff
#endif


#define __AR_WIDE_STR(_String) L ## _String
#define AR_WSTR(_String) __AR_WIDE_STR(_String)


#define __AR_APPEND_STR(_Value1, _Value2) _Value1 ## _Value2
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

typedef AR_PLAT_INT				int_t;/*跟所在处理器等长的有符号整数*/

typedef AR_PLAT_UINT			uint_t;/*跟所在处理器等长的无符号整数*/

typedef uint8_t					byte_t;

typedef void*					ptr_t;

//#define AR_CHARMAX				CHAR_MAX
#define AR_WCHARMAX				WCHAR_MAX
#define AR_BYTE_BITS			CHAR_BIT
#define AR_SIZE_MAX				((uint_t)(~((uint_t)0)))





#include "spec.h"

#endif
