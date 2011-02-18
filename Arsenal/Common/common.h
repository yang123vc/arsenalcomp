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



const wchar_t*	AR_Version();


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

/*
 static const wchar_t *AR_BUILD_TIME = AR_WSTR(__DATE__) AR_WSTR(" ") AR_WSTR(__TIME__);
 */



#define AR_report()		AR_printf(L"File (%ls) : Line (%" AR_PLAT_INT_FMT L"d) : Function (%ls)\r\n", AR_WSTR(__FILE__), (size_t)__LINE__, AR_WSTR(AR_FUNC_NAME))

void	AR_check(bool_t cond, const wchar_t *fmt, ...);

#define AR_CHECK		AR_check

#define AR_STATIC_CHECK(_expr)	do {typedef char __static_assert_t[ (bool_t)(_expr) ]; }while(0)






#if defined(AR_DEBUG)

#define AR_DPRINT				AR_printf

#define AR_ASSERT(_cond)		assert((_cond))


#else

#define AR_DPRINT

#define AR_ASSERT(_cond)		/*(_cond)*/

#endif


#define AR_UNUSED(_e)			((void)(_e))


/***********************************************************macro_oper*********************************************************/



#define __AR_CNT_BIT__(_n)		((_n) * AR_BYTE_BITS)

#define AR_BYTEFLIP_16(_v)		( (((uint_16_t)(_v) >> __AR_CNT_BIT__(1)) & 0x00FF)								\
								| (((uint_16_t)(_v) << __AR_CNT_BIT__(1)) & 0xFF00)								\
								)

#define AR_BYTEFLIP_32(_v)		( (((uint_32_t)(_v) >> __AR_CNT_BIT__(3)) & 0x000000FF)							\
								| (((uint_32_t)(_v) >> __AR_CNT_BIT__(1)) & 0x0000FF00)							\
								| (((uint_32_t)(_v) << __AR_CNT_BIT__(1)) & 0x00FF0000)							\
								| (((uint_32_t)(_v) << __AR_CNT_BIT__(3)) & 0xFF000000)							\
								)


#define AR_BYTEFLIP_64(_v)		( (((uint_64_t)(_v) >> __AR_CNT_BIT__(7)) & AR_BIGNUM_U64(0x00000000000000FF))		\
								| (((uint_64_t)(_v) >> __AR_CNT_BIT__(5)) & AR_BIGNUM_U64(0x000000000000FF00))		\
								| (((uint_64_t)(_v) >> __AR_CNT_BIT__(3)) & AR_BIGNUM_U64(0x0000000000FF0000))		\
								| (((uint_64_t)(_v) >> __AR_CNT_BIT__(1)) & AR_BIGNUM_U64(0x00000000FF000000))		\
								| (((uint_64_t)(_v) << __AR_CNT_BIT__(1)) & AR_BIGNUM_U64(0x000000FF00000000))		\
								| (((uint_64_t)(_v) << __AR_CNT_BIT__(3)) & AR_BIGNUM_U64(0x0000FF0000000000))		\
								| (((uint_64_t)(_v) << __AR_CNT_BIT__(5)) & AR_BIGNUM_U64(0x00FF000000000000))		\
								| (((uint_64_t)(_v) << __AR_CNT_BIT__(7)) & AR_BIGNUM_U64(0xFF00000000000000))		\
								)





#if defined(ARCH_LITTLE_ENDIAN)

		#define AR_LTON_16(_n)			AR_BYTEFLIP_16((_n))
		#define AR_LTON_32(_n)			AR_BYTEFLIP_32((_n))
		#define AR_LTON_64(_n)			AR_BYTEFLIP_64((_n))

		#define AR_NTOL_16(_n)			AR_BYTEFLIP_16((_n))
		#define AR_NTOL_32(_n)			AR_BYTEFLIP_32((_n))
		#define AR_NTOL_64(_n)			AR_BYTEFLIP_64((_n))

#else

		#define AR_LTON_16(_n)			(_n)
		#define AR_LTON_32(_n)			(_n)
		#define AR_LTON_64(_n)			(_n)

		#define AR_NTOL_16(_n)			(_n)
		#define AR_NTOL_32(_n)			(_n)
		#define AR_NTOL_64(_n)			(_n)

#endif







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


#define AR_OFFSETOF(_ty, _filed)		((byte_t*)&((_ty*)0)->_filed) - ((byte_t*)(_ty*)0)


/*float macro_oper*/


#define AR_FLT_EQ(_x, _y)		(bool_t)(fabsf( (((float)(_x)) - ((float)(_y)))) < FLT_EPSILON)
#define AR_FLT_LE(_x, _y)		(bool_t)( ((float)(_x)) < ((float)(_y)) )
#define AR_FLT_GE(_x, _y)		(bool_t)( ((float)(_x)) > ((float)(_y)) )
#define AR_FLT_LEEQ(_x, _y)		(bool_t)( AR_FLT_EQ((_x), (_y)) || AR_FLT_LE((_x), (_y)) )
#define AR_FLT_GEEQ(_x, _y)		(bool_t)( AR_FLT_EQ((_x), (_y)) || AR_FLT_GE((_x), (_y)) )

#define AR_FLT_MOD(_x,_y)		((float)(_x) - floor((float)(_x) / (float)(_y)) * (float)(_y))
#define AR_FLT_POW(_x,_y)		(float)pow((float)(_x), (float)(_y))



#define AR_DBL_EQ(_x, _y)		(bool_t)(fabs( (((double)(_x)) - ((double)(_y)))) < DBL_EPSILON)
#define AR_DBL_LE(_x, _y)		(bool_t)(((double)(_x)) < ((double)(_y)))
#define AR_DBL_GE(_x, _y)		(bool_t)(((double)(_x)) > ((double)(_y)))
#define AR_DBL_LEEQ(_x, _y)		(bool_t)( AR_DBL_EQ((_x), (_y)) || AR_DBL_LE((_x), (_y)))
#define AR_DBL_GEEQ(_x, _y)		(bool_t)( AR_DBL_EQ((_x), (_y)) || AR_DBL_GE((_x), (_y)))

#define AR_DBL_MOD(_x,_y)		((double)(_x) - floor((double)(_x) / (double)(_y)) * (double)(_y))
#define AR_DBL_POW(_x,_y)		(double)pow((double)(_x), (double)(_y))


/**********************************************************Heap***************************************************************/

#define AR_HEAP_ALIGN			8
#define AR_ALIGN_SIZE(_bytes)	( ((_bytes) + AR_HEAP_ALIGN - 1 ) & ~(AR_HEAP_ALIGN - 1) )








typedef struct __arsenal_heap_tag		arHeap_t;
arHeap_t*		AR_CreateHeap();
void			AR_DestroyHeap(arHeap_t *heap);
void*			AR_AllocFromHeap(arHeap_t *heap, size_t bytes);
void*			AR_ClearedAllocFromHeap(arHeap_t *heap, size_t num, size_t elem_size);
void*			AR_ReallocFromHeap(arHeap_t *heap, void *ptr, size_t bytes);
void			AR_FreeToHeap(arHeap_t *heap, void *ptr);





/**********************************************************memory***************************************************************/


#define AR_MEM_MAX_ALLOC_RETRY_COUNT	100000

void	AR_InitMemory();
void	AR_UnInitMemory();

#if !defined(AR_USE_CRT_ALLOCFUNC)

void*	AR_malloc(size_t nbytes);
void*	AR_calloc(size_t num, size_t size);
void*	AR_realloc(void *block, size_t nbytes);
void	AR_free(void *ptr);

#else


#define	AR_malloc		malloc
#define	AR_calloc		calloc
#define	AR_realloc		realloc
#define AR_free			free

#endif


#define AR_memset				memset
#define AR_memcpy				memcpy
#define AR_memcmp				memcmp
#define	AR_memmove				memmove
void	AR_memswap(void *a, void *b, size_t n);




/*

#define AR_NEW(_type) ((_type*)malloc(sizeof(_type)))
#define AR_NEW0(_type) ((_type*)calloc(1, sizeof(_type)))
#define AR_NEWARR(_type, _n) ((_type*)malloc(sizeof(_type) * (_n)))
#define AR_NEWARR0(_type, _n) ((_type*)calloc((_n), sizeof(_type)))
#define AR_REALLOC(_type, _ptr, _new_count) ((_type*)realloc((_ptr), sizeof(_type) * (_new_count)))
#define AR_DEL(_ptr) free((void*)(_ptr))

*/


#define AR_NEW(_type) ((_type*)AR_malloc(sizeof(_type)))
#define AR_NEW0(_type) ((_type*)AR_calloc(1, sizeof(_type)))
#define AR_NEWARR(_type, _n) ((_type*)AR_malloc(sizeof(_type) * (_n)))
#define AR_NEWARR0(_type, _n) ((_type*)AR_calloc((_n), sizeof(_type)))
#define AR_REALLOC(_type, _ptr, _new_count) ((_type*)AR_realloc((_ptr), sizeof(_type) * (_new_count)))
#define AR_DEL(_ptr) AR_free((void*)(_ptr))



#define AR_HEAP_NEW(_heap, _type)						((_type*)AR_AllocFromHeap((_heap), sizeof(_type)))
#define AR_HEAP_NEW0(_heap, _type)						((_type*)AR_ClearedAllocFromHeap((_heap),1, sizeof(_type)))
#define AR_HEAP_NEWARR(_heap, _type, _n)				((_type*)AR_AllocFromHeap((_heap), sizeof(_type) * (_n)))
#define AR_HEAP_NEWARR0(_heap, _type, _n)				((_type*)AR_ClearedAllocFromHeap((_heap),(_n), sizeof(_type)))
#define AR_HEAP_REALLOC(_heap, _type, _ptr, _new_count) ((_type*)AR_ReallocFromHeap((_heap), (_ptr), sizeof(_type) * (_new_count)))
#define AR_HEAP_DEL(_heap, _ptr)						AR_FreeToHeap((_heap),(void*)(_ptr))



/**********************************************************algo*************************************************************/

/*#define AR_qsort		qsort*/



void	AR_qsort(void *base, size_t num, size_t width, int_t (*cmp_f)(const void*, const void*));
int_t	AR_bsearch(const void *key, const void *base, size_t num, size_t width, int_t (*cmp_f)(const void*, const void*));

/*void	AR_qsort(void *base, size_t num, size_t width, int_t (__cdecl *cmp_f)(void*,void*));*/



/**********************************************************rand*************************************************************/
void			AR_srand(uint_64_t seed);
uint_64_t		AR_rand64();
uint_32_t		AR_rand32();


/********************************************************CRT String*****************************************************************/



int_t			AR_stricmp(const char *l, const char *r);
int_t			AR_strnicmp(const char *l, const char *r, size_t n);
int_t			AR_wcsicmp(const wchar_t *l, const wchar_t *r);
int_t			AR_wcsnicmp(const wchar_t *l, const wchar_t *r, size_t n);




#define AR_strlen(_s)			strlen((_s))
#define AR_strcmp(_l, _r)		strcmp((_l), (_r))
#define AR_strncmp(_l, _r,_n)	strncmp((_l), (_r), (_n))
#define AR_strcpy(_d, _s)		strcpy((_d), (_s))
#define AR_strncpy(_d, _s, _n)	strncpy((_d), (_s), (_n))
#define AR_strchr(_s, _c)		strchr((_s),(_c))
#define AR_strstr(_s1, _s2)		strstr((_s1), (_s2))
#define AR_isalpha(_c)			isalpha((_c))
#define AR_isalnum(_c)			isalnum((_c))
#define AR_isdigit(_c)			isdigit((_c))
#define AR_tolower(_c)			tolower((_c))
#define AR_toupper(_c)			toupper((_c))
#define AR_isspace(_c)			isspace((_c))
#define AR_isgraph(_c)			isgraph((_c))
#define AR_isprint(_c)			isprint((_c))

char*			AR_strupr(char *s);
char*			AR_strlwr(char *s);
char*			AR_strnlwr(char *s, size_t count);
char*			AR_strnupr(char *s, size_t count);


const char*		AR_stristr(const char *s, const char *p);


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
#define AR_towupper(_c)			towupper((_c))
#define AR_iswspace(_c)			iswspace((_c))
#define AR_iswgraph(_c)			iswgraph((_c))
#define AR_iswprint(_c)			iswprint((_c))


wchar_t*			AR_wcsupr(wchar_t *s);
wchar_t*			AR_wcslwr(wchar_t *s);
wchar_t*			AR_wcsnlwr(wchar_t *s, size_t count);
wchar_t*			AR_wcsnupr(wchar_t *s, size_t count);

const wchar_t*		AR_wcsistr(const wchar_t *s, const wchar_t *p);


wchar_t*		AR_wcsdup(const wchar_t *sour);

/*len == 0时返回空串L""*/
wchar_t*		AR_wcsndup(const wchar_t *sour, size_t len);


const wchar_t*	AR_wcstrim(const wchar_t *in, const wchar_t *trim);
const wchar_t*	AR_wcstrim_space(const wchar_t *in);

wchar_t*		AR_wcscat(wchar_t *dest, const wchar_t *sour);
wchar_t*		AR_wcsncat(wchar_t *dest, const wchar_t *sour, size_t n);

const wchar_t*	AR_wtoi32(const wchar_t *in, int_32_t  *num, size_t base);
const wchar_t*	AR_wtou32(const wchar_t *in, uint_32_t *num, size_t base);
const wchar_t*	AR_wtoi64(const wchar_t *in, int_64_t  *num, size_t base);
const wchar_t*	AR_wtou64(const wchar_t *in, uint_64_t  *num, size_t base);

const wchar_t*	AR_wtod(const wchar_t *in, double *num);

/********************************************************************************************************************************************/
const wchar_t*	AR_wcstrim_s(const wchar_t *in, const wchar_t *end, const wchar_t *trim);
const wchar_t*	AR_wcstrim_space_s(const wchar_t *in, const wchar_t *end);

const wchar_t*	AR_wtoi32_s(const wchar_t *in, const wchar_t *end, int_32_t  *num, size_t base);
const wchar_t*	AR_wtou32_s(const wchar_t *in, const wchar_t *end, uint_32_t *num, size_t base);

const wchar_t*	AR_wtoi64_s(const wchar_t *in, const wchar_t *end, int_64_t  *num, size_t base);
const wchar_t*	AR_wtou64_s(const wchar_t *in, const wchar_t *end, uint_64_t  *num, size_t base);
const wchar_t*	AR_wtod_s(const wchar_t *in, const wchar_t *end, double *num);


/*此函数相当于修改字符串，因此不存在_s版，且trim之后的字符串结尾为\0*/
wchar_t*		AR_wcstrim_right(wchar_t *in, const wchar_t *trim);
wchar_t*		AR_wcstrim_right_space(wchar_t *in);

/*返回的是需要元素数组的长度包含0*/
int_t			AR_i64tow_buf(wchar_t *out, size_t nbuf, int_64_t num, size_t radix);
int_t			AR_u64tow_buf(wchar_t *out, size_t nbuf, uint_64_t num, size_t radix);



const wchar_t* AR_wcsstr_kmp(const wchar_t *s, const wchar_t *p);
const wchar_t* AR_wcsstr_kmp_s(const wchar_t *beg, const wchar_t *end, const wchar_t *p);



#if(AR_ARCH_VER	== ARCH_32)

		#define AR_wtoi			AR_wtoi32
		#define AR_wtou			AR_wtou32

		#define AR_wtou_s		AR_wtou32_s
		#define AR_wtoi_s		AR_wtoi32_s


#elif(AR_ARCH_VER == ARCH_64)


		#define AR_wtoi			AR_wtoi64
		#define AR_wtou			AR_wtou64

		#define AR_wtou_s		AR_wtou64_s
		#define AR_wtoi_s		AR_wtoi64_s

#else
		
		#error "Not Support Platform!"

#endif








/*返回一个长度，足够容纳fmt + args*/
int_t			AR_vscwprintf(const wchar_t *fmt, va_list args);
int_t			AR_scwprintf(const wchar_t *fmt, ...);
wchar_t*		AR_vtow(const wchar_t *fmt, ...);

int_t			AR_swprintf(wchar_t *dest, size_t count, const wchar_t *fmt, ...);
int_t			AR_vsprintf(char *dest, size_t count, const char *fmt, va_list args);
int_t			AR_vswprintf(wchar_t *dest, size_t count, const wchar_t *fmt, va_list args);



int_t			AR_wchartodigit(wchar_t ch);



uint_t			AR_wcshash(const wchar_t *str);
uint_t			AR_wcshash_n(const wchar_t *str, size_t n);




/********************************************************String*****************************************************************/

typedef struct __arsenal_string_tag		arString_t;




arString_t*		AR_CreateString();
void			AR_DestroyString(arString_t *str);

/*预留num个wchar_t的空间*/
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




typedef struct __string_table_			arStringTable_t;




#define		MIN_BUCKET_SIZE		(139)


arStringTable_t*		AR_CreateStrTable(size_t count);
void					AR_DestroyStrTable(arStringTable_t* tbl);


const wchar_t*			AR_GetString(arStringTable_t *tbl, const wchar_t *str);
const wchar_t*			AR_GetStringN(arStringTable_t *tbl, const wchar_t *str, size_t n);

bool_t					AR_HasString(const arStringTable_t *tbl, const wchar_t *str);
bool_t					AR_HasStringN(const arStringTable_t *tbl, const wchar_t *str, size_t n);

const wchar_t*			AR_GetStringUInt(arStringTable_t *tbl, uint_64_t num, size_t radix);
const wchar_t*			AR_GetStringInt(arStringTable_t *tbl, int_64_t num, size_t radix);
const wchar_t*			AR_GetStringFloat(arStringTable_t *tbl, double num, size_t prec);


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


#define			AR_MAXSPIN_COUNT		5000

#if defined(OS_FAMILY_UNIX)
	
	#if(OS_TYPE == OS_IPHONE || OS_TYPE == OS_MAC_OS_X)
		typedef			int_t						arSpinLock_t;	
#else
		typedef			pthread_spinlock_t	        arSpinLock_t;

	#endif

#elif defined(OS_FAMILY_WINDOWS)

    typedef         volatile int_t					arSpinLock_t;
#else

#endif

void			AR_InitSpinLock(arSpinLock_t *lock);
void			AR_UnInitSpinLock(arSpinLock_t *lock);
void			AR_LockSpinLock(arSpinLock_t *lock);
void			AR_UnLockSpinLock(arSpinLock_t *lock);

void			AR_YieldThread();
void			AR_Sleep(size_t millisecond);

uint_64_t		AR_GetTime_Microseconds();
#define			AR_GetTime_Milliseconds()		(AR_GetTime_Microseconds() / 1000LL)


/*******************************************************BitMark****************************************************************/


/*#define AR_BIT_MARK(_pos)	(((uint_64_t)0x01) << ((uint_64_t)(_pos)))*/

static AR_INLINE uint_64_t AR_BIT_MARK(uint_64_t pos) { return AR_BIGNUM_U64(0x01) << pos; }

#define AR_BIT_TEST(_val, _pos)  ((((_val) & (AR_BIT_MARK((_pos))))))

#define AR_BIT_SET(_val, _pos)   ((_val) |= (AR_BIT_MARK((_pos))))

#define AR_BIT_CLR(_val, _pos)   (((_val)) &=  (~(AR_BIT_MARK((_pos)))))



/* 在第p位上设置n个1 */
#define AR_MASK1(_n,_p)	( (~( (~(uint_64_t)0) << (_n))) << (_p) )

/* 在第p位上设置n个0 */
#define AR_MASK0(_n,_p)	(~AR_MASK1((_n),(_p)))



/**************************************************Buffer**********************************************************************/

struct arsenal_buffer_tag;
typedef struct arsenal_buffer_tag		arBuffer_t;

arBuffer_t*		AR_CreateBuffer(size_t nbytes);
void			AR_DestroyBuffer(arBuffer_t		*buffer);

void			AR_ClearBuffer(arBuffer_t		*buffer);

/*分配nbytes个字节以供使用*/
byte_t*			AR_AllocBuffer(arBuffer_t *buffer, size_t	nbytes);
/*向buffer写入nbytes个字节*/
void			AR_InsertBuffer(arBuffer_t *buffer, const byte_t *data, size_t len);

/*从buffer头擦除nbytes个字节*/
size_t			AR_EraseBuffer(arBuffer_t *buffer, size_t nbytes);

/*返回不重新分配内存还可以写的字节数*/
size_t			AR_GetBufferCapacity(const arBuffer_t *buffer);

/*AR_ReserveBuffer调用成功后，AR_GetBufferCapacity(buffer) >= nbytes*/
void			AR_ReserveBuffer(arBuffer_t *buffer, size_t nbytes);

/*可读内存块*/
const byte_t*	AR_GetBufferData(const arBuffer_t *buffer);
/*可读内存块长度*/
size_t			AR_GetBufferAvailable(const arBuffer_t *buffer);







/***************************************************************Misc**********************************************************/
#define	AR_ESCSTR_ERR_OK				0x00
#define	AR_ESCSTR_ERR_VALUE				0x01
#define	AR_ESCSTR_ERR_CHAR				0x02
#define AR_ESCSTR_ERR_BUFFER			0x03

typedef struct __escape_string_error_tag
{
		int_t			type;
		const wchar_t	*pos;
		uint_64_t		value;
}arEscStrErr_t;


wchar_t*		AR_escstr_to_str(const wchar_t *src, arEscStrErr_t *error);
wchar_t*		AR_str_to_escstr(const wchar_t *src);

wchar_t*		AR_escstr_to_str_n(const wchar_t *src, size_t n, arEscStrErr_t *error);
wchar_t*		AR_str_to_escstr_n(const wchar_t *src, size_t n);


int_t 			AR_escstr_to_str_buf(wchar_t *dest, size_t len, const wchar_t *src, arEscStrErr_t *error);
int_t 			AR_str_to_escstr_buf(wchar_t *dest, size_t len, const wchar_t *src);


int_t 			AR_escstr_to_str_buf_n(wchar_t *dest, size_t len, const wchar_t *src, size_t n,arEscStrErr_t *error);
int_t 			AR_str_to_escstr_buf_n(wchar_t *dest, size_t len, const wchar_t *src, size_t n);



/***************************************************************Text**********************************************************/


typedef enum
{
		AR_TXT_BOM_ASCII		=		0x01,
		AR_TXT_BOM_UTF16_BE		=		0x02,
		AR_TXT_BOM_UTF16_LE		=		0x04,
		AR_TXT_BOM_UTF_8		=		0x08,
		AR_TXT_BOM_UTF32_LE		=		0x10,
		AR_TXT_BOM_UTF32_BE		=		0x20
}arTxtBom_t;

bool_t	AR_LoadBomTextFile(const wchar_t *path, arTxtBom_t *bom, const wchar_t *line_sp, arString_t *out);




AR_NAMESPACE_END




#endif
