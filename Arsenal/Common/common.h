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


#include "config.h"




AR_NAMESPACE_BEGIN


/*
common.h -- misc founctions used in Arsenal

*/






/*******************************************************************************init********************************/
 
typedef void	(AR_STDCALL *AR_error_func_t)(int_t level, const wchar_t *msg, void *ctx);
typedef void	(AR_STDCALL *AR_print_func_t)(const wchar_t *msg, void *ctx);


typedef struct __arsenal_io_context_tag
{
		AR_error_func_t	on_error;
		AR_print_func_t	on_print;
		void			*ctx;
}arIOCtx_t;


typedef struct __ar_init_tag
{
		arIOCtx_t		global_io_ctx;
}arInit_t;


void AR_Init(const arInit_t *info);
void AR_UnInit();

arIOCtx_t*	AR_global_ioctx();

void	AR_printf(const wchar_t *msg,...);

/*库内部错误为负数*/
#define AR_ERR_FATAL		((int_t)-0x0001)
#define AR_ERR_WARNING		((int_t)-0x0002)

void	AR_error(int_t level, const wchar_t *msg, ...);



void	AR_printf_ctx(arIOCtx_t *ctx, const wchar_t *msg,...);
void	AR_error_ctx(arIOCtx_t *ctx, int_t level, const wchar_t *msg, ...);





/***********************************************************************debug************************************************/

#define AR_report()		AR_printf(L"File (%ls) : Line (%" AR_PLAT_INT_FMT L"d) : Function (%ls)\r\n", AR_WSTR(__FILE__), (size_t)__LINE__, AR_WSTR(AR_FUNC_NAME))

#define AR_CHECK(_cond,_msg,_level) do {if(!(_cond))AR_error((_level), L"%ls\r\n", (_msg)); }while(0)

#define AR_STATIC_CHECK(_expr)	typedef char __static_assert_t[ (_expr) ]



#if defined(AR_DEBUG)

#define AR_DPRINT		AR_printf

#define AR_ASSERT(_cond)		assert((_cond))

#else

#define AR_DPRINT

#define AR_ASSERT(_cond)		(_cond)

#endif





/***********************************************************macro_oper*********************************************************/
#define AR_MAX(_a,_b) ((_a) > (_b) ? (_a) : (_b))
#define AR_MIN(_a,_b) ((_a) < (_b) ? (_a) : (_b))


#define AR_CMP(_a,_b) ((_a) < (_b) ? -1 : ((_a) == (_b) ? 0 : 1))
/*行主映射*/
#define AR_TBL_IDX_R(_x, _y, _col_n) ((_x) * (_col_n) + (_y))


#define AR_NELEMS(_arr)	(sizeof((_arr))/sizeof((_arr)[0]))


static AR_INLINE const void* AR_GET_ELEM(const void *base, size_t width, size_t idx)
{
		AR_ASSERT(base != NULL && width > 0);
		return (const void*)((byte_t*)base + (width * idx));
}




/**********************************************************memory***************************************************************/

#if defined(AR_USE_CRT_ALLOCFUNC)

		#define AR_malloc(_bytes)		malloc((_bytes))
		#define	AR_calloc(_n, _size)	calloc((_n), (_size))
		#define AR_realloc(_b, _n)		realloc((_b), (_n))
		#define AR_free(_p)				free(_p)
#else

		void*	AR_malloc(size_t nbytes);
		void*	AR_calloc(size_t num, size_t size);
		void*	AR_realloc(void *block, size_t nbytes);
		void	AR_free(void *ptr);

#endif


void	AR_memswap(void *a, void *b, size_t n);




#define AR_memset				memset
#define AR_memcpy				memcpy
#define AR_memcmp				memcmp







#define AR_NEW(_type) ((_type*)AR_malloc(sizeof(_type)))

#define AR_NEW0(_type) ((_type*)AR_calloc(1, sizeof(_type)))

#define AR_NEWARR(_type, _n) ((_type*)AR_malloc(sizeof(_type) * (_n)))

#define AR_NEWARR0(_type, _n) ((_type*)AR_calloc((_n), sizeof(_type)))

#define AR_REALLOC(_type, _ptr, _new_count) ((_type*)AR_realloc((_ptr), sizeof(_type) * (_new_count)))

#define AR_DEL(_ptr) AR_free((void*)(_ptr))




/**********************************************************algo*************************************************************/

/*#define AR_qsort		qsort*/



void	AR_qsort(void *base, size_t num, size_t width, int_t (*cmp_f)(const void*, const void*));
int_t	AR_bsearch(const void *key, const void *base, size_t num, size_t width, int_t (*cmp_f)(const void*, const void*));

/*void	AR_qsort(void *base, size_t num, size_t width, int_t (__cdecl *cmp_f)(void*,void*));*/



/**********************************************************rand*************************************************************/
void			AR_srand(uint64_t seed);
uint64_t		AR_rand64();
uint32_t		AR_rand32();


/********************************************************CRT String*****************************************************************/


#define AR_strlen(_s)			strlen((_s))

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
#define AR_towlower(_c)			towlower((_c))
#define AR_iswspace(_c)			iswspace((_c))
#define AR_iswgraph(_c)			iswgraph((_c))
#define AR_iswprint(_c)			iswprint((_c))



wchar_t* AR_wcsdup(const wchar_t *sour);
wchar_t* AR_wcsndup(const wchar_t *sour, size_t len);


const wchar_t*	AR_wcstrim(const wchar_t *in, const wchar_t *trim);
const wchar_t*	AR_wcstrim_space(const wchar_t *in);

wchar_t*		AR_wcscat(wchar_t *dest, const wchar_t *sour);
wchar_t*		AR_wcsncat(wchar_t *dest, const wchar_t *sour, size_t n);

const wchar_t*	AR_wtoi32(const wchar_t *in, int32_t  *num, size_t base);
const wchar_t*	AR_wtou32(const wchar_t *in, uint32_t *num, size_t base);
const wchar_t*	AR_wtoi64(const wchar_t *in, int64_t  *num, size_t base);
const wchar_t*	AR_wtou64(const wchar_t *in, uint64_t  *num, size_t base);

const wchar_t*	AR_wtod(const wchar_t *in, double *num);

/********************************************************************************************************************************************/
const wchar_t*	AR_wcstrim_s(const wchar_t *in, const wchar_t *end, const wchar_t *trim);
const wchar_t*	AR_wcstrim_space_s(const wchar_t *in, const wchar_t *end);

const wchar_t*	AR_wtoi32_s(const wchar_t *in, const wchar_t *end, int32_t  *num, size_t base);
const wchar_t*	AR_wtou32_s(const wchar_t *in, const wchar_t *end, uint32_t *num, size_t base);

const wchar_t*	AR_wtoi64_s(const wchar_t *in, const wchar_t *end, int64_t  *num, size_t base);
const wchar_t*	AR_wtou64_s(const wchar_t *in, const wchar_t *end, uint64_t  *num, size_t base);
const wchar_t*	AR_wtod_s(const wchar_t *in, const wchar_t *end, double *num);


/*此函数相当于修改字符串，因此不存在_s版，且trim之后的字符串结尾为\0*/
wchar_t*		AR_wcstrim_right(wchar_t *in, const wchar_t *trim);

/*返回的是需要元素数组的长度包含0*/
int_t			AR_i64tow_buf(wchar_t *out, size_t nbuf, int64_t num, size_t radix);
int_t			AR_u64tow_buf(wchar_t *out, size_t nbuf, uint64_t num, size_t radix);



#if(AR_ARCH_VER	== ARCH_32)

		#define AR_wtoi AR_wtoi32
		#define AR_wtou AR_wtou32

		#define AR_wtou_s	AR_wtou32_s
		#define AR_wtoi_s	AR_wtoi32_s


#elif(AR_ARCH_VER == ARCH_64)


		#define AR_wtoi AR_wtoi64
		#define AR_wtou AR_wtou64

		#define AR_wtou_s	AR_wtou64_s
		#define AR_wtoi_s	AR_wtoi64_s

#else




#endif








/*返回一个长度，足够容乃fmt + args*/
int_t			AR_vscwprintf(const wchar_t *fmt, va_list args);
int_t			AR_scwprintf(const wchar_t *fmt, ...);
wchar_t*		AR_vtow(const wchar_t *fmt, ...);


int_t				AR_wchartodigit(wchar_t ch);



uint_t			AR_wcshash(const wchar_t *str);
uint_t			AR_wcshash_n(const wchar_t *str, size_t n);


wchar_t*		AR_str_to_escstr(const wchar_t *src);


/********************************************************String*****************************************************************/

typedef struct __arsenal_string_tag		arString_t;




arString_t*		AR_CreateString();
void			AR_DestroyString(arString_t *str);

void			AR_ReserveString(arString_t *str, size_t num);
void			AR_ClearString(arString_t *str);
size_t			AR_AppendString(arString_t *str, const wchar_t *sour);

void			AR_FormatString(arString_t *str, const wchar_t *fmt, ...);
void			AR_AppendFormatString(arString_t *str, const wchar_t *fmt, ...);

void			AR_AppendCharToString(arString_t *str, wchar_t chr);

const wchar_t*	AR_GetStrString(const arString_t *str);
size_t			AR_GetLengthString(const arString_t *str);


#define			AR_StrPrint(_s) do{ AR_printf(L"%ls\r\n", AR_GetStrString((_s))); }while(0)
#define			AR_StrPrintCtx(_ctx, _s)do{ AR_printf_ctx((_ctx), L"%ls\r\n", AR_GetStrString((_s))); }while(0)





/********************************************************StringTable*****************************************************************/




typedef struct __arsenal_str_rec		arStringRec_t;
typedef struct __string_table_			arStringTable_t;




#define		MIN_BUCKET_SIZE		(6151 / AR_MEM_POLICY)


arStringTable_t*		AR_CreateStrTable(size_t count);
void					AR_DestroyStrTable(arStringTable_t* tbl);


const wchar_t*			AR_GetString(arStringTable_t *tbl, const wchar_t *str);
const wchar_t*			AR_GetStringN(arStringTable_t *tbl, const wchar_t *str, size_t n);

const wchar_t*			AR_GetStringUInt(arStringTable_t *tbl, uint64_t num, size_t radix);
const wchar_t*			AR_GetStringInt(arStringTable_t *tbl, int64_t num, size_t radix);



/*********************************************************String Convert****************************************************/

size_t AR_wcs_to_utf8(const wchar_t *unicode, size_t n, char *out, size_t out_len);

size_t AR_utf8_to_wcs(const char *utf8, size_t n, wchar_t *out, size_t out_len);

char*  AR_wcs_convto_utf8(const wchar_t *wcs);

wchar_t* AR_utf8_convto_wcs(const char *utf8);



/**********************************************************Threading*************************************************************/

/*thread模块初始化函数，只能由AR_Init调用*/
void			AR_InitThread();
void			AR_UnInitThread();

/*uint_t			AR_CompExchange(volatile uint_t *dest, uint_t exch, uint_t compval);*/

int_t			AR_AtomicInc(volatile int_t *dest);
int_t			AR_AtomicDec(volatile int_t *dest);

#define			AR_MAXSPIN_COUNT		8192

typedef			volatile int		arSpinLock_t;

void			AR_InitSpinLock(arSpinLock_t *lock);
void			AR_UnInitSpinLock(arSpinLock_t *lock);
void			AR_LockSpinLock(arSpinLock_t *lock);
void			AR_UnLockSpinLock(arSpinLock_t *lock);

void			AR_YieldThread();
void			AR_Sleep(size_t millisecond);

uint64_t		AR_GetClock_US();
#define			AR_GetClock_MS()		(AR_GetClock_US() / 1000LL)

/*******************************************************BitMark****************************************************************/


/*#define AR_BIT_MARK(_pos)	(((uint64_t)0x01) << ((uint64_t)(_pos)))*/

static AR_INLINE uint64_t AR_BIT_MARK(uint64_t pos) { return AR_BIGNUM_U64(0x01) << pos; }

#define AR_BIT_TEST(_val, _pos)  ((((_val) & (AR_BIT_MARK((_pos))))))

#define AR_BIT_SET(_val, _pos)   ((_val) |= (AR_BIT_MARK((_pos))))

#define AR_BIT_CLR(_val, _pos)   (((_val)) &=  (~(AR_BIT_MARK((_pos)))))




AR_NAMESPACE_END




#endif
