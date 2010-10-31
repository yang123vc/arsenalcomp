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


#if(ARCH == ARCH_IA32 || ARCH == ARCH_ARM || ARCH == ARCH_PPC)

#define AR_ARCH_VER		ARCH_32

#elif(ARCH == ARCH_X64 || ARCH == ARCH_IA64)

#define AR_ARCH_VER		ARCH_64

#else

#error "Unknown platform not supported!"

#endif



#define AR_VC_LEGACY			0x1006
#define AR_VC					0x1009
#define AR_BCB6					0x2006
#define AR_GCC3					0x3003
#define AR_GCC4					0x3004


#if defined(_MSC_VER)

		#if (_MSC_VER < 1300)

				#define AR_COMPILER		AR_VC_LEGACY
		#else
				#define	AR_COMPILER		AR_VC
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



#if(__STDC_VERSION__ >= 199901L)
    #define     AR_HAS_C99_FEATURE		1
#endif


#if defined(__cplusplus) || defined(AR_HAS_C99_FEATURE)
		#define AR_HAS_INLINE	1
#endif




#if defined(__cplusplus)
		#define AR_NAMESPACE_BEGIN		extern "C" { namespace ARSpace {
		#define AR_NAMESPACE_END		} }
#else
		#define AR_NAMESPACE_BEGIN
		#define AR_NAMESPACE_END
#endif








/**********************************************************编译器相关的配置***********************************************************************/

/*
    配置编译器相关的选项
*/
#if(AR_COMPILER == AR_VC_LEGACY || AR_COMPILER == AR_VC)

		#pragma warning(disable : 4100)
		#pragma warning(disable : 4127)
		#pragma warning(disable : 4201)


		#if !defined(_CRT_SECURE_NO_WARNINGS)
				#define _CRT_SECURE_NO_WARNINGS
		#endif

		#if(AR_COMPILER == AR_VC && OS_TYPE != OS_WINDOWS_CE)
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
#include <string.h>
#include <float.h>



#if defined(AR_HAS_C99_FEATURE)
    #include <stdint.h>
	#include <stdbool.h>
#endif

#if defined(_STDINT_H)
    #define AR_HAS_STDINT
#endif

#if defined(__bool_true_false_are_defined) || defined(__cplusplus)
		#define AR_HAS_BOOL_TRUE_FALSE
#endif




/*include相关平台所需的头文件*/

#if defined(OS_FAMILY_WINDOWS)
		#if(AR_COMPILER == AR_VC_LEGACY || OS_TYPE == OS_WINDOWS_CE)
				struct _RPC_ASYNC_STATE;
		#endif

		#include <windows.h>
#elif defined(OS_FAMILY_UNIX)
        #include <unistd.h>
        #include <pthread.h>
        #include <sys/select.h>
        #include <sys/time.h>

		#if(OS_TYPE == OS_MAC_OS_X || OS_TYPE == OS_IPHONE)
			#include <libkern/OSAtomic.h>
		#endif
#else
		#error "Unknown OS not supported!"
#endif



/*设置平台相关类型*/


#if (AR_COMPILER == AR_VC_LEGACY || AR_COMPILER == AR_VC || AR_COMPILER == AR_BCB6)

		#if defined(NDEBUG)
				#define AR_NDEBUG
		#else
				#define AR_DEBUG
		#endif


		#define AR_STDCALL		__stdcall
		#define AR_CCALL		__cdecl

		#define AR_INT8_T		__int8
		#define AR_UINT8_T		unsigned __int8

		#define AR_INT16_T		__int16
		#define AR_UINT16_T		unsigned __int16

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

		#if(AR_COMPILER == AR_VC_LEGACY)
				#define AR_NOOP					((void)0)
				#define AR_FUNC_NAME			"--"

		#elif (AR_COMPILER == AR_VC)

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

		/**/
		#if(AR_ARCH_VER == ARCH_32)
				#define AR_STDCALL		__attribute__((stdcall))
				#define AR_CCALL		__attribute__((cdecl))
		#else
                #define AR_STDCALL
				#define AR_CCALL
		#endif

		#if(OS_TYPE == OS_MAC_OS_X)				
				#if(ARCH == ARCH_PPC)
						#undef	AR_STDCALL
						#define	AR_STDCALL								
				#endif
		#elif(OS_TYPE == OS_IPHONE)
				#undef	AR_STDCALL
				#define	AR_STDCALL
		#endif


		#define AR_INT8_T		signed char
		#define AR_UINT8_T		unsigned char

		#define AR_INT16_T		signed short
		#define AR_UINT16_T		unsigned short

		#define AR_INT32_T		signed int
		#define AR_UINT32_T		unsigned int
		#define AR_INT64_T		signed long long int
		#define AR_UINT64_T		unsigned long long int

		#define AR_CHAR_T		char
		#define AR_UCHAR_T		unsigned char
		#define AR_BYTE_T		AR_UINT8_T
		#define AR_WCHAR_T		wchar_t


		#define AR_BIGNUM_I64(_num)	(_num##LL)
		#define AR_BIGNUM_U64(_num)	(_num##ULL)
		
		#if(OS_TYPE == OS_MAC_OS_X || OS_TYPE == OS_IPHONE)
				#define AR_INT_FMT64		L"q"
		#else
				#define AR_INT_FMT64		L"ll"
		#endif

		#define AR_NOOP

		#define AR_FUNC_NAME			__PRETTY_FUNCTION__


		#define	AR_DEBUG_BREAK				__builtin_trap

#else


		#error "Unknown Compiler not supported!"


#endif



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






/*sizeof(bool_t) 必须等于1*/

#if defined(AR_HAS_BOOL_TRUE_FALSE)
		typedef bool					bool_t;
#else
		typedef	AR_INT8_T				bool_t;
		#define	true					1
		#define false					0
#endif


#if defined(AR_HAS_INLINE)
		#define AR_INLINE				inline
#else
		#define AR_INLINE				/*C99标准之前的编译器定义无*/
#endif






/************************平台无关的定义***********************************/




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



typedef AR_INT8_T				int_8_t;

typedef AR_UINT8_T				uint_8_t;

typedef AR_INT16_T				int_16_t;

typedef AR_UINT16_T				uint_16_t;

typedef AR_INT32_T				int_32_t;

typedef AR_UINT32_T				uint_32_t;

typedef AR_INT64_T				int_64_t;

typedef AR_UINT64_T				uint_64_t;


typedef AR_PLAT_INT_T			int_t;/*跟所在处理器等长的有符号整数*/

typedef AR_PLAT_UINT_T			uint_t;/*跟所在处理器等长的无符号整数,理论上总是等于size_t*/

typedef uint_8_t				byte_t;

typedef void*					ptr_t;


/**************************以下均为未定义时的默认值****************************/

#if !defined(CHAR_BIT)
		#define	CHAR_BIT		8		
#endif


#if !defined(CHAR_MAX)
		#define	CHAR_MAX		((~(0xFFFFFFFFFFFFFFFFUL << (CHAR_BIT * sizeof(char))))/2)
#endif


#if !defined(WCHAR_MAX)
		#define WCHAR_MAX		(~(0xFFFFFFFFFFFFFFFFUL << (CHAR_BIT * sizeof(wchar_t))))
#endif
/**********************************************************************************/

#define AR_CHARMAX				CHAR_MAX
#define AR_WCHARMAX				WCHAR_MAX
#define AR_BYTE_BITS			CHAR_BIT
#define AR_SIZE_MAX				((size_t)(~((size_t)0)))















/******************************************Spec***************************************************************/



#if (AR_COMPILER == AR_VC_LEGACY)

		#define AR_swprintf						_snwprintf
		#define AR_vsprintf						_vsnprintf
		#define AR_vswprintf					_vsnwprintf
		#define AR_abort						abort


#elif (AR_COMPILER == AR_VC)

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

		#if defined(OS_FAMILY_WINDOWS)
				#define AR_swprintf				_snwprintf
				#define AR_vsprintf				_vsnprintf
				#define AR_vswprintf			_vsnwprintf
		#else
				#define AR_swprintf						swprintf
				#define AR_vsprintf						vsprintf
				#define AR_vswprintf					vswprintf
		#endif


		#define AR_abort	            abort

#else

		#error "Unknow Compiler!"

#endif


/*************************************************************************************************************/


#if(OS_TYPE == OS_WINDOWS_CE || OS_TYPE == OS_IPHONE)
		#define AR_LOW_MEM_POLICY		1
#endif


#endif
