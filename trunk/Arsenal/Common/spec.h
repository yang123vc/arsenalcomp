#ifndef AR_SPEC_H
#define AR_SPEC_H

#include "config.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>



#if defined(_MSC_VER) || defined(__BORLANDC__)

#if !defined(WCHAR_MAX)
		#define WCHAR_MAX		0xffff
#endif



#define __AR_WIDE_STR(_String) L ## _String
#define AR_WSTR(_String) __AR_WIDE_STR(_String)


#define __AR_APPEND_STR(_Value1, _Value2) _Value1 ## _Value2
#define AR_APPSTR(_Value1, _Value2) __AR_APPEND_STR(_Value1, _Value2)


#define __AR_STRINGIZE_STR(_Value) #_Value
#define AR_TOSTR(_Value) __AR_STRINGIZE_STR(_Value)

#define AR_LNUM(num) (n)

#define AR_FMT64		"I64"

#endif



#if defined(_MSC_VER)



#if (_MSC_VER < 1300)

#define ar_vsprintf(buf, nelems, fmt, arg_ptr)		do{vsprintf(buf, fmt, arg_ptr);}while(0)

#define ar_vswprintf	_vsnwprintf

#define ar_vscprintf(fmt, args)			(4096)

#define ar_vscwprintf(fmt, args)		(4096)


#define AR_FUNC_NAME	"--"

#else


#define ar_vsprintf				vsprintf_s

#define ar_vswprintf			vswprintf_s

#define ar_vscprintf			_vscprintf

#define ar_vscwprintf			_vscwprintf


#define AR_FUNC_NAME			__FUNCSIG__

#endif


#define ar_sprintf		sprintf

#define ar_swprintf		swprintf

#define ar_sscanf		sscanf

#define ar_swscanf		swscanf



#elif defined(__BORLANDC__)


#define ar_sprintf		sprintf

AR_INLINE int ar_swprintf(wchar_t *buffer,  size_t count, const wchar_t *fmt, ...)
{
        int res;
        va_list  arg_ptr;
        va_start(arg_ptr, fmt);
        res = vwprintf(fmt, arg_ptr);
        va_end(arg_ptr);
        return res;
}


#define ar_vsprintf(buf, count, fmt, args)	        vsprintf(buf, fmt, args)

#define ar_vswprintf(buf, count, fmt, args)	        vswprintf(buf, fmt, args)


#define ar_vscprintf(fmt, args)		        	(4096)

#define ar_vscwprintf(fmt, args)	        	(4096)


#define ar_sscanf		sscanf

#define ar_swscanf		swscanf


#define AR_FUNC_NAME	__FUNC__


#else

#error "Unkonw Platform"



#endif










#endif

