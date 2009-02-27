/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#ifndef AR_COMMON_H
#define AR_COMMON_H

#include "config.h"
#include "spec.h"

#include <stddef.h>
#include <stdarg.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <wchar.h>

#include<stdio.h>




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

#define True			AR_TRUE
#define False			AR_FALSE
typedef AR_BOOL_T		bool_t;

#define AR_CHARMAX		WCHAR_MAX

#define AR_SIZE_MAX		((uint_t)(~((uint_t)0)))

#define AR_BYTE_BITS	CHAR_BIT





/*******************************************************************************error handler********************************/

#define AR_MAXTRC_LEN 4096


typedef enum
{
		AR_MESSAGE,
		AR_WARNING,
		AR_CRITICAL,
		AR_FATAL
};

typedef void (AR_STDCALL *ar_errhandler_t)(int, const wchar_t*);

AR_API ar_errhandler_t ar_set_errhdl(ar_errhandler_t hdl);

AR_API void ar_error(int level,  const wchar_t *msg, ...);


typedef void (AR_STDCALL *ar_disphandler_t)(const wchar_t*);

AR_API ar_disphandler_t ar_set_disphdl(ar_disphandler_t hdl);

AR_API void ar_printf(const wchar_t *msg,...);

/**************************************************IO**********************************************************************/

#define AR_Printf		ar_printf
#define AR_Error		ar_error



/***********************************************************************debug************************************************/


#define AR_ASSERT		assert

#define AR_ERROR(l, msg) AR_Error((l), L"err msg == %s : %s\n", (msg), AR_WSTR(AR_FUNC_NAME))

#define AR_CHECK(cond, l, msg) do {if(!(cond))AR_ERROR(l,msg); }while(0)


#if defined(AR_DEBUG)
		#define AR_DBGPRINT		AR_Printf
#else
		#define AR_DBGPRINT		AR_NOOP
#endif


/***********************************************************************memory************************************************/


#define AR_MSET(_p, _val, _size) memset((_p), (_val), (_size))
#define AR_MSET0(_p, _size) AR_MSET((_p), 0, (_size))

#define AR_MCPY(_d, _s, _nsize) memcpy((_d), (_s), (_nsize))
#define AR_MCPYARR(_d, _s, _size, _nitem) memcpy((_d), (_s), (_nitem * _size))


#define AR_NEW(type) ((type*)malloc(sizeof(type)))

#define AR_NEW0(type) ((type*)calloc(1, sizeof(type)))

#define AR_NEWARR(type, n) ((type*)malloc(sizeof(type) * n))

#define AR_NEWARR0(type, n) ((type*)calloc(n, sizeof(type)))

#define AR_REALLOC(type, ptr, new_count) ((type*)realloc(ptr, sizeof(type) * new_count))

#define AR_DEL(ptr) free(ptr)


#define AR_CPYNEW(_type, _dest, _src) do{_dest = AR_NEW(_type);  AR_MCPY(_dest, _src, sizeof(_type));}while(0)
#define AR_CPYNEWARR(_type, _dest, _src, _nitem) do{_dest = AR_NEWARR(_type, _nitem);  AR_MCPYARR(_dest, _src, sizeof(_type), _nitem);}while(0)

#define AR_MCMP(_l,_r, _size) memcmp((_l), (_r), (_size))

#define AR_MCMPARR(_l, _r, _size, _nitem) AR_MCMP((_l),(_r), (_size) * (_nitem))

/**********************************************************strings***************************************************************/

#define AR_WSTRCMP(_l, _r)		wcscmp((_l), (_r))
#define AR_WSTRLEN(_s)			wcslen((_s))
#define AR_WSTRCPY(_d, _s)		wcscpy((_d), (_s))
#define AR_WSTRNCPY(_d, _s, _n)	wcsncpy((_d), (_s), (_n))
#define AR_WSTRCHR(_s, _c)		wcschr((_s),(_c))



/***********************************************************macro_oper*********************************************************/

#define AR_CMP(_a,_b) ((_a) < (_b) ? -1 : ((_a) == (_b) ? 0 : 1))


#endif
