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
#ifndef AR_COMMON_H
#define AR_COMMON_H


#define _CRT_SECURE_NO_WARNINGS 1

#include<stdio.h>
#include <wchar.h>
#include "config.h"
#include "spec.h"

AR_NAMESPACE_BEGIN


/*
common.h -- misc founctions used in Arsenal

*/






/*******************************************************************************init********************************/

typedef void (AR_STDCALL *AR_error_func)(int, const wchar_t*);
typedef void (AR_STDCALL *AR_print_func)(const wchar_t *);

typedef struct __ar_init_tag
{
		AR_error_func	error_f;
		AR_print_func	print_f;
}arInit_t;


void AR_Init(const arInit_t *info);
void AR_UnInit();


typedef enum
{
		AR_LEX,
		AR_GRAMMAR,
		AR_PARSER
};

void AR_error(int level,  const wchar_t *msg, ...);
void AR_printf(const wchar_t *msg,...);


/***********************************************************************debug************************************************/

#define AR_CHECK(cond, l, msg) do {if(!(cond))AR_error(l,msg); }while(0)

#if defined(AR_DEBUG)

#define AR_DPRINT		AR_printf

#else

#define AR_DPRINT		AR_NOOP

#endif




/***********************************************************macro_oper*********************************************************/

#define AR_CMP(_a,_b) ((_a) < (_b) ? -1 : ((_a) == (_b) ? 0 : 1))
/*ÐÐÖ÷Ó³Éä*/
#define AR_TBL_IDX_R(_x, _y, _col_n) ((_x) * (_col_n) + (_y))


#define AR_NELEMS(_arr)	(sizeof((_arr))/sizeof((_arr)[0]))
/**********************************************************memory***************************************************************/

#define AR_malloc				malloc
#define AR_calloc				calloc
#define AR_realloc				realloc
#define AR_free					free



#define AR_memset				memset
#define AR_memcpy				memcpy
#define AR_memcmp				memcmp




#define AR_NEW(_type) ((_type*)AR_malloc(sizeof(_type)))

#define AR_NEW0(_type) ((_type*)AR_calloc(1, sizeof(_type)))

#define AR_NEWARR(_type, _n) ((_type*)AR_malloc(sizeof(_type) * (_n)))

#define AR_NEWARR0(_type, _n) ((_type*)AR_calloc((_n), sizeof(_type)))

#define AR_REALLOC(_type, _ptr, _new_count) ((_type*)AR_realloc((_ptr), sizeof(_type) * (_new_count)))

#define AR_DEL(_ptr) AR_free((_ptr))



/******************************************************debug*************************************************************************/


#define AR_ASSERT		assert



/**********************************************************algo*************************************************************/

#define AR_qsort		qsort

/**********************************************************string*************************************************************/

#define AR_wcscmp(_l, _r)		wcscmp((_l), (_r))
#define AR_wcsncmp(_l, _r,_n)	wcsncmp((_l), (_r), (_n))
#define AR_wcslen(_s)			wcslen((_s))
#define AR_wcscpy(_d, _s)		wcscpy((_d), (_s))
#define AR_wcsncpy(_d, _s, _n)	wcsncpy((_d), (_s), (_n))
#define AR_wcschr(_s, _c)		wcschr((_s),(_c))
#define AR_wcsstr(_s1, _s2)		wcsstr((_s1), (_s2))
#define AR_iswalpha(_c)			iswalpha((_c))
#define AR_iswalnum(_c)			iswalnum((_c))
#define AR_iswdigit(_c)			iswdigit((_c))

wchar_t* AR_wcsdup(const wchar_t *sour);
wchar_t* AR_wcsndup(const wchar_t *sour, size_t len);




const wchar_t* AR_wcstrim(const wchar_t *in, const wchar_t *trim);

const wchar_t* AR_wtoi32(const wchar_t *in,	 int32_t  *num, size_t base);
const wchar_t* AR_wtou32(const wchar_t *in,  uint32_t *num, size_t base);
const wchar_t* AR_wtoi64(const wchar_t *in,	 int64_t  *num, size_t base);
const wchar_t* AR_wtou64(const wchar_t *in,	 uint64_t  *num, size_t base);



#if(ARCH == ARCH_IA32)

#define AR_wtoi AR_wtoi32
#define AR_wtou AR_wtou32

#elif(ARCH == ARCH_IA64 || ARCH == ARCH_X64)

#define AR_wtoi AR_wtoi64
#define AR_wtou AR_wtou64

#else


#endif



uint_t		AR_wcshash(const wchar_t *str);
uint_t		AR_wcshash_n(const wchar_t *str, size_t n);



typedef struct __arsenal_string_tag		arString_t;


arString_t*		AR_CreateString();
void			AR_DestroyString(arString_t *str);

void			AR_ReserveString(arString_t *str, size_t num);
void			AR_ClearString(arString_t *str);
size_t			AR_AppendString(arString_t *str, const wchar_t *sour);


void			AR_FormatString(arString_t *str, const wchar_t *fmt, ...);
void			AR_AppendFormatString(arString_t *str, const wchar_t *fmt, ...);


const wchar_t*	AR_GetStrString(const arString_t *str);
size_t			AR_GetLengthString(const arString_t *str);


/**********************************************************Threading*************************************************************/

void			AR_YiledThread();

uint_t			AR_CompExchange(volatile uint_t *dest, uint_t exch, uint_t compval);


#define			AR_MAXSPIN_COUNT		400

typedef			uint_t		arSpinLock_t;

void			AR_InitSpinLock(volatile arSpinLock_t *lock);
void			AR_UnInitSpinLock(volatile arSpinLock_t *lock);
void			AR_LockSpinLock(volatile arSpinLock_t *lock);
void			AR_UnLockSpinLock(volatile arSpinLock_t *lock);






/********************************************************StringTable*****************************************************************/



AR_NAMESPACE_END


#endif
