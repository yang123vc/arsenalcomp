
#ifndef AR_CONFIG_H
#define AR_CONFIG_H

#include "platform.h"


#if !defined(__STDC__) && !defined(__cplusplus)  /* traditional C */

#endif  /* end traditional C */



#if defined(__cplusplus)

#define AR_C_LINKAGE extern "C"

#else

#define AR_C_LINKAGE extern


#endif






#if defined(_MSC_VER) || defined(__BORLANDC__)


/*
		语言特性配置
*/

		#if defined(__cplusplus)
				#define AR_HAS_BOOLEAN			1
				#define AR_INLINE				inline
				#define	AR_NO_THROW				__declspec(nothrow)
		#else
				#define AR_INLINE				__inline
				#define AR_NO_THROW
		#endif



/*
    配置预定义宏
*/

		#if defined(NDEBUG) && defined(_DEBUG)
				#error Inconsistent build settings (check for /MD[d])
		#endif

		#if defined(NDEBUG)
				#define AR_NDEBUG
		#else
				#define AR_DEBUG
		#endif
		
		#if defined(AR_SHARED_LIB)
				#if defined(AR_EXPORTS)
						#define AR_API AR_C_LINKAGE AR_NO_THROW __declspec(dllexport)
				#else
						#define AR_API AR_C_LINKAGE AR_NO_THROW __declspec(dllimport)
				#endif
		#else

				#define AR_API AR_C_LINKAGE AR_NO_THROW
		#endif
		
		#define AR_STDCALL __stdcall

		#define AR_CCALL __cdecl 




/*
    配置编译器相关的类型
*/



		#define AR_INT8_T	__int8
		#define AR_UINT8_T  unsigned __int8
		#define AR_INT32_T	int
		#define AR_UINT32_T unsigned int
		#define AR_INT64_T  __int64
		#define AR_UINT64_T unsigned __int64
		#define AR_CHAR_T	char
		#define AR_UCHAR_T	unsigned char
		#define AR_WCHAR_T  wchar_t
		#define AR_BYTE_T	AR_UINT8_T
		

		
		
		#if defined(AR_HAS_BOOLEAN)
				#define AR_BOOL_T		bool
				#define AR_TRUE			true
				#define AR_FALSE		false
		#else
				#define AR_BOOL_T		int
				#define AR_TRUE			(AR_BOOL_T)1
				#define AR_FALSE		(AR_BOOL_T)0
		#endif


		
		#define _CRT_SECURE_NO_WARNINGS	1

		#pragma warning(disable : 4204)
		#pragma warning(disable : 4244)
		#pragma warning(disable : 4127)
		#pragma warning(disable : 4100)
		#pragma warning(disable : 4201)
		#pragma warning(disable : 4505)
		#pragma warning(disable : 4514)
		#pragma warning(disable : 4710)
		#pragma warning(disable : 4761)
		
		#if (_MSC_VER < 1300)
				#define AR_NOOP	NULL
		#else
				#define AR_NOOP	__noop
		#endif

#else

#error "Unknow platform"

#endif







/*********************************平台相关***************************************/


#if(ARCH == ARCH_IA32)

#define AR_PLAT_INT				AR_INT32_T
#define AR_PLAT_UINT	        AR_UINT32_T


#elif(ARCH == ARCH_IA64 || ARCH == ARCH_X64)

#define AR_PLAT_INT				AR_INT64_T
#define AR_PLAT_UINT	        AR_UINT64_T

#else

#error "Unknow Platform"

#endif






#endif
