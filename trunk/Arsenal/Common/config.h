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


#define ARCH_32			0x20u
#define ARCH_64			0x40u


#if(ARCH == ARCH_IA32 || ARCH == ARCH_ARM)

#define AR_ARCH_VER		ARCH_32

#elif(ARCH == ARCH_X64 || ARCH == ARCH_IA64)

#define AR_ARCH_VER		ARCH_64

#else

#error "Unknown platform not supported!"

#endif



#define AR_VC6			0x1006
#define AR_VC9			0x1009
#define AR_BCB6			0x2006
#define AR_GCC3			0x3003
#define AR_GCC4			0x3004


#if defined(_MSC_VER)

		#if (_MSC_VER < 1300)

				#define AR_COMPILER		AR_VC6
		#else
				#define	AR_COMPILER		AR_VC9
		#endif


#elif defined(__BORLANDC__)

		#define AR_COMPILER		AR_BCB6

#elif defined(__GNUC__)

		#if(__GNUC__ >= 4)
				#define	AR_COMPILER		AR_GCC4
		#else
				#define	AR_COMPILER		AR_GCC3
		#endif

#else

		#error "Unknow Compiler!"

#endif


#if defined(__cplusplus) && !defined(__STDC_VERSION__)

		#define __STDC_VERSION__  0
#endif




/**********************************************************编译器相关的配置***********************************************************************/

/*
    配置编译器相关的选项
*/
#if(AR_COMPILER == AR_VC6 || AR_COMPILER == AR_VC9)
		
		#pragma warning(disable : 4100)
		#pragma warning(disable : 4127)
		#pragma warning(disable : 4201)

/*

		#pragma warning(disable : 4204)
		#pragma warning(disable : 4244)
		#pragma warning(disable : 4505)
		#pragma warning(disable : 4514)
		#pragma warning(disable : 4710)
		#pragma warning(disable : 4761)
*/

/*
#if(OS_TYPE == OS_WINDOWS_CE)
		#pragma warning(disable : 4214)
		#pragma warning(disable : 4201)
		#pragma warning(disable : 4090)
#endif
*/

		#if !defined(_CRT_SECURE_NO_WARNINGS)
				#define _CRT_SECURE_NO_WARNINGS
		#endif
		
		#if(AR_COMPILER == AR_VC9 && OS_TYPE != OS_WINDOWS_CE)
				#if !defined(NDEBUG)
						#define AR_USE_CRT_ALLOCFUNC
						#define	AR_DISABLE_CRTSTDLIB
						#define _CRTDBG_MAP_ALLOC 
						#include<stdlib.h> 
						#include<crtdbg.h> 
				#else


				#endif

		#endif

#elif(AR_COMPILER == AR_BCB6)

                #pragma warn -8004
                #pragma warn -8019
                #pragma warn -8058
                #pragma warn -8065
#else


#endif



//////////////////////////////////////
#if !defined(AR_DISABLE_CRTSTDLIB)
		#include<stdlib.h>
#endif

#include <stdio.h>
#include <wchar.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <wctype.h>

#if defined(OS_FAMILY_WINDOWS)
		#if(AR_COMPILER == AR_VC6 || OS_TYPE == OS_WINDOWS_CE)
				struct _RPC_ASYNC_STATE;
		#endif

		#include <windows.h>
#elif defined(OS_FAMILY_UNIX)

#else
		#error "Unknown OS not supported!"
#endif



#if (AR_COMPILER == AR_VC6 || AR_COMPILER == AR_VC9 || AR_COMPILER == AR_BCB6)
		
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

		#define AR_BIGNUM_I64(_num)	(_num##i64)
		#define AR_BIGNUM_U64(_num)	(_num##ui64)

		#define AR_INT_FMT64		L"I64"

		#if(AR_COMPILER == AR_VC6)
				#define AR_NOOP					((void)0)
				#define AR_FUNC_NAME			"--"

		#elif (AR_COMPILER == AR_VC9)

				#define AR_NOOP					__noop
				#define AR_FUNC_NAME			__FUNCSIG__

		#elif (AR_COMPILER == AR_BCB6)
				#define AR_NOOP					NULL
				#define AR_FUNC_NAME			__FUNC__
		#endif

		

		#define	AR_DEBUG_BREAK				__debugbreak

#elif(AR_COMPILER == AR_GCC3 || AR_COMPILER == AR_GCC4)

		#if defined(NDEBUG)
				#define AR_NDEBUG
		#else
				#define AR_DEBUG
		#endif

		#define AR_STDCALL		__attribute__((stdcall))
		#define AR_CCALL		__attribute__((cdecl))

		#define AR_INT8_T		__int8_t
		#define AR_UINT8_T		__uint8_t
		#define AR_INT32_T		__int32_t
		#define AR_UINT32_T		__uint32_t
		#define AR_INT64_T		__int64_t
		#define AR_UINT64_T		__uint64_t
		#define AR_CHAR_T		char
		#define AR_UCHAR_T		unsigned char
		#define AR_BYTE_T		AR_UINT8_T
		#define AR_WCHAR_T		wchar_t


		#define AR_BIGNUM_I64(_num)	(_num##LL)
		#define AR_BIGNUM_U64(_num)	(_num##ULL)

		#define AR_INT_FMT64		L"ll"

		#define AR_NOOP

		#define AR_FUNC_NAME			__PRETTY_FUNCTION__

		
		#define	AR_DEBUG_BREAK				__builtin_trap

#else

		
		#error "Unknown Compiler not supported!"


#endif






#define			AR_MEM_POLICY		(1)


/*sizeof(bool_t) 必须等于1*/


#if defined(__cplusplus)
		#define AR_NAMESPACE_BEGIN		extern "C" { namespace ARSpace {
		#define AR_NAMESPACE_END		} }


		typedef bool					bool_t;
		
		#define AR_INLINE				inline
#else
		

		#define AR_NAMESPACE_BEGIN
		#define AR_NAMESPACE_END

		typedef	AR_INT8_T				bool_t;

		#define	true					1
		#define false					0

		#if(__STDC_VERSION__ < 199901L) /*C99标准之前的编译器定义为无*/
				#define AR_INLINE				/*static*/
        #else
                #define AR_INLINE				inline
		#endif

#endif



/*********************************平台相关***************************************/


#if(AR_ARCH_VER	== ARCH_32)

		#define AR_PLAT_INT_T			AR_INT32_T
		#define AR_PLAT_UINT_T	        AR_UINT32_T

		#define AR_PLAT_INT_FMT			L""

#elif(AR_ARCH_VER == ARCH_64)


		#define AR_PLAT_INT_T			AR_INT64_T
		#define AR_PLAT_UINT_T	        AR_UINT64_T

		#define AR_PLAT_INT_FMT			AR_INT_FMT64

#else

		#error "Not Support Platform!"

#endif









/************************平台无关的定义***********************************/


#ifndef WCHAR_MAX
#if defined(OS_FAMILY_UNIX)
		#define WCHAR_MAX		0xFFFFFFFF
#else
		#define WCHAR_MAX		0xFFFF
#endif

#endif


/*
#undef ULLONG_MAX
#undef LLONG_MAX
#undef LLONG_MIN
*/

#ifndef ULLONG_MAX
		#define ULLONG_MAX		AR_BIGNUM_U64(0xffffffffffffffff)
#endif

#ifndef LLONG_MAX
		#define LLONG_MAX		AR_BIGNUM_I64(9223372036854775807)
#endif

#ifndef LLONG_MIN
		#define LLONG_MIN		(AR_BIGNUM_I64(-9223372036854775807) - 1)
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

typedef AR_PLAT_UINT_T			uint_t;/*跟所在处理器等长的无符号整数,理论上总是等于size_t*/

typedef uint8_t					byte_t;

typedef void*					ptr_t;


#define AR_CHARMAX				CHAR_MAX
#define AR_WCHARMAX				WCHAR_MAX
#define AR_BYTE_BITS			CHAR_BIT
#define AR_SIZE_MAX				((uint_t)(~((uint_t)0)))















/******************************************Spec***************************************************************/



#if (AR_COMPILER == AR_VC6)

		#define AR_swprintf						_snwprintf
		#define AR_vsprintf						_vsnprintf
		#define AR_vswprintf					_vsnwprintf
		#define AR_abort						abort


#elif (AR_COMPILER == AR_VC9)

		#define AR_swprintf						_snwprintf
		#define AR_vsprintf						_vsnprintf
		#define AR_vswprintf					_vsnwprintf

		#if(OS_TYPE == OS_WINDOWS_CE)
				#define AR_abort()				exit(3)
		#else
				#define AR_abort				abort
		#endif



#elif(AR_COMPILER == AR_BCB6)
                #define AR_swprintf				_snwprintf
				#define AR_vsprintf				_vsnprintf
				#define AR_vswprintf			_vsnwprintf
                #define AR_abort	            abort



#elif(AR_COMPILER == AR_GCC3 || AR_COMPILER == AR_GCC4)

		#define AR_swprintf						swprintf
		#define AR_vsprintf						vsprintf
		#define AR_vswprintf					vswprintf
		#define AR_abort						abort

#else

		#error "Unknow Compiler!"

#endif


/*************************************************************************************************************/






#endif
