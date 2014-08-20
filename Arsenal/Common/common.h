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


/*******************************************************************************error code********************************/

#include "status.h"






/*
common.h -- misc founctions used in Arsenal

*/

const wchar_t*	AR_Version();


/*******************************************************************************init********************************/

typedef void	(AR_STDCALL *AR_error_func_t)(ar_int_t level, const wchar_t *msg, void *ctx);
typedef void	(AR_STDCALL *AR_print_func_t)(const wchar_t *msg, void *ctx);



typedef struct __arsenal_io_context_tag
{
		AR_error_func_t	on_error;
		AR_print_func_t	on_print;
		void			*ctx;
}arIOCtx_t;


typedef size_t	(AR_STDCALL *AR_backtrace_func_t)(void **callstack, size_t callstack_cnt);
typedef size_t	(AR_STDCALL *AR_backtrace_symbol_t)(void **callstack, size_t callstack_cnt, char *str, size_t len);

typedef struct __arsenal_callstack_tag
{
		AR_backtrace_func_t				gen_backtrace;
		AR_backtrace_symbol_t			gen_backtrace_sym;
}arBacktrace_t;


typedef struct __ar_init_tag
{
		arIOCtx_t				global_io_ctx;
		arBacktrace_t			backtrace;
}arInit_t;






arStatus_t AR_CommonInit(const arInit_t *info);
arStatus_t AR_CommonUnInit();

const arBacktrace_t*	AR_GetBacktrace();

arStatus_t	AR_printf(const wchar_t *msg,...);
arStatus_t	AR_debug_print(const wchar_t *msg, ...);

/*The libraries internal error is negative*/
#define AR_ERR_DEBUG		((ar_int_t)-0x0098)
#define AR_ERR_MESSAGE		((ar_int_t)-0x0099)
#define AR_ERR_WARNING		((ar_int_t)-0x0100)
#define AR_ERR_FATAL		((ar_int_t)-0x0101)



arStatus_t	AR_error(ar_int_t level, const wchar_t *msg, ...);







/*
arStatus_t	AR_printf_ctx(arIOCtx_t *ctx, const wchar_t *msg,...);
arStatus_t	AR_error_ctx(arIOCtx_t *ctx, ar_int_t level, const wchar_t *msg, ...);
*/




/***********************************************************************debug************************************************/

/*
 static const wchar_t *AR_BUILD_TIME = AR_WSTR(__DATE__) AR_WSTR(" ") AR_WSTR(__TIME__);
 */



#define AR_report()		AR_error(AR_ERR_MESSAGE, L"File (%hs) : Line (%Id) : Function (%hs)\r\n", __FILE__, (size_t)__LINE__, AR_FUNC_NAME)

void	AR_check(ar_bool_t cond, const wchar_t *fmt, ...);

#define AR_CHECK		AR_check

#define AR_STATIC_CHECK(_expr)	do {typedef char __static_assert_t[ (ar_bool_t)(_expr) ]; }while(0)






#if defined(AR_DEBUG)

#define AR_DPRINT				AR_debug_print

#define AR_ASSERT(_cond)		assert((_cond))


#else

#define AR_DPRINT               false && AR_debug_print

#define AR_ASSERT(_cond)		/*(_cond)*/

#endif


#define AR_UNUSED(_e)			((void)(_e))





/***********************************************************macros*********************************************************/


#define AR_MAX(_a,_b) ((_a) > (_b) ? (_a) : (_b))
#define AR_MIN(_a,_b) ((_a) < (_b) ? (_a) : (_b))


#define AR_CMP(_a,_b) ((_a) < (_b) ? -1 : ((_a) == (_b) ? 0 : 1))
/*行主映射*/
#define AR_TBL_IDX_R(_x, _y, _col_n) ((_x) * (_col_n) + (_y))
#define AR_TBL_IDX_C(_x, _y, _row_n) ((_x) + (_y) * (_row_n))

#define AR_NELEMS(_arr)	(sizeof((_arr))/sizeof((_arr)[0]))


static AR_INLINE const void* AR_GET_ELEM(const void *base, size_t width, size_t idx)
{
		AR_ASSERT(base != NULL && width > 0);
		return (const void*)((ar_byte_t*)base + (width * idx));
}


#define AR_OFFSETOF(_ty, _filed)		((ar_byte_t*)&((_ty*)0)->_filed) - ((ar_byte_t*)(_ty*)0)

















/**********************************************************memory management***************************************************************/

/*Defined AR_USE_CRT_ALLOCFUNC, using CRT memory alloc functions，otherwise, use the 'arHeap_t'
*/

#define AR_MEM_MAX_ALLOC_RETRY_COUNT	1000000

void	AR_InitMemory();
void	AR_UnInitMemory();




#if !defined(AR_USE_CRT_ALLOCFUNC)

		#if defined(AR_ENABLE_MEMORY_LEAK_TEST)

		void*	AR_debug_malloc(size_t nbytes, const char *file_name, ar_int_t line);
		void*	AR_debug_calloc(size_t num, size_t size, const char *file_name, ar_int_t line);
		void	AR_debug_free(void *ptr, const char *file_name, ar_int_t line);

		#define AR_malloc(_nb)			AR_debug_malloc((_nb), __FILE__, __LINE__)
		#define AR_calloc(_n, _s)		AR_debug_calloc((_n),(_s), __FILE__, __LINE__)
		#define AR_free(_p)				AR_debug_free((_p), __FILE__, __LINE__)
		
		#else
		
		void*	AR_malloc(size_t nbytes);
		void*	AR_calloc(size_t num, size_t size);
		void	AR_free(void *ptr);

		#endif


#else


#define	AR_malloc		malloc
#define	AR_calloc		calloc

#define AR_free			free


#endif







#define AR_memset				memset
#define AR_memcpy				memcpy
#define AR_memcmp				memcmp
#define	AR_memmove				memmove
void	AR_memswap(void *a, void *b, size_t n);





#define AR_NEW(_type) ((_type*)AR_malloc(sizeof(_type)))
#define AR_NEW0(_type) ((_type*)AR_calloc(1, sizeof(_type)))
#define AR_NEWARR(_type, _n) ((_type*)AR_malloc(sizeof(_type) * (_n)))
#define AR_NEWARR0(_type, _n) ((_type*)AR_calloc((_n), sizeof(_type)))

/*
#define AR_REALLOC(_type, _ptr, _new_count) ((_type*)AR_realloc((_ptr), sizeof(_type) * (_new_count)))
*/


#define AR_DO_REALLOC(_type, _org_ptr, _new_cap, _copy_cnt, _status)                            \
		do{                                                                                     \
				_type *new_arr;                                                                 \
				new_arr = AR_NEWARR(_type, (_new_cap));                                         \
				if(new_arr == NULL)                                                             \
				{                                                                               \
                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);           \
						(_status) = AR_E_NOMEM;                                                 \
                        break;                                                                  \
				}                                                                               \
                                                                                                \
                if((_copy_cnt) > 0 && (_org_ptr) != NULL)                                       \
                {                                                                               \
                        AR_memcpy(new_arr, (_org_ptr), (_copy_cnt) * sizeof(_type));            \
                }                                                                               \
                if((_org_ptr))                                                                  \
                {                                                                               \
                        AR_DEL((_org_ptr));                                                     \
                        (_org_ptr) = NULL;                                                      \
                }                                                                               \
                (_org_ptr) = new_arr;                                                           \
                (_status) = AR_S_YES;                                                           \
		}while(0);


#define AR_DEL(_ptr) AR_free((void*)(_ptr))














/***********************************************************Byte Order operations*********************************************************/




ar_int_16_t		AR_BYTEFLIP_16(ar_int_16_t val);
ar_uint_16_t	AR_BYTEFLIP_U16(ar_uint_16_t val);


ar_int_32_t		AR_BYTEFLIP_32(ar_int_32_t val);
ar_uint_32_t	AR_BYTEFLIP_U32(ar_uint_32_t val);


ar_int_64_t		AR_BYTEFLIP_64(ar_int_64_t val);
ar_uint_64_t	AR_BYTEFLIP_U64(ar_uint_64_t val);


void			AR_BYTEFLIP_16B(ar_byte_t *b);
void			AR_BYTEFLIP_32B(ar_byte_t *b);
void			AR_BYTEFLIP_64B(ar_byte_t *b);


#if defined(ARCH_LITTLE_ENDIAN)

		#define AR_LTON_16(_n)			AR_BYTEFLIP_16((_n))
		#define AR_LTON_32(_n)			AR_BYTEFLIP_32((_n))
		#define AR_LTON_64(_n)			AR_BYTEFLIP_64((_n))
		#define AR_LTON_U16(_n)			AR_BYTEFLIP_U16((_n))
		#define AR_LTON_U32(_n)			AR_BYTEFLIP_U32((_n))
		#define AR_LTON_U64(_n)			AR_BYTEFLIP_U64((_n))

		#define AR_LTON_16B(_n)			AR_BYTEFLIP_16B((_n))
		#define AR_LTON_32B(_n)			AR_BYTEFLIP_32B((_n))
		#define AR_LTON_64B(_n)			AR_BYTEFLIP_64B((_n))

		#define AR_NTOL_16(_n)			AR_BYTEFLIP_16((_n))
		#define AR_NTOL_32(_n)			AR_BYTEFLIP_32((_n))
		#define AR_NTOL_64(_n)			AR_BYTEFLIP_64((_n))
		#define AR_NTOL_U16(_n)			AR_BYTEFLIP_U16((_n))
		#define AR_NTOL_U32(_n)			AR_BYTEFLIP_U32((_n))
		#define AR_NTOL_U64(_n)			AR_BYTEFLIP_U64((_n))

		#define AR_NTOL_16B(_n)			AR_BYTEFLIP_16B((_n))
		#define AR_NTOL_32B(_n)			AR_BYTEFLIP_32B((_n))
		#define AR_NTOL_64B(_n)			AR_BYTEFLIP_64B((_n))
		

#else

		#define AR_LTON_16(_n)			(_n)
		#define AR_LTON_32(_n)			(_n)
		#define AR_LTON_64(_n)			(_n)
		#define AR_LTON_U16(_n)			(_n)
		#define AR_LTON_U32(_n)			(_n)
		#define AR_LTON_U64(_n)			(_n)

		#define AR_LTON_16B(_n)			(_n)
		#define AR_LTON_32B(_n)			(_n)
		#define AR_LTON_64B(_n)			(_n)
		

		#define AR_NTOL_16(_n)			(_n)
		#define AR_NTOL_32(_n)			(_n)
		#define AR_NTOL_64(_n)			(_n)
		#define AR_NTOL_U16(_n)			(_n)
		#define AR_NTOL_U32(_n)			(_n)
		#define AR_NTOL_U64(_n)			(_n)

		#define AR_NTOL_16B(_n)			(_n)
		#define AR_NTOL_32B(_n)			(_n)
		#define AR_NTOL_64B(_n)			(_n)

#endif














/**********************************************************algo*************************************************************/

/*#define AR_qsort		qsort*/



void		AR_qsort(void *base, size_t num, size_t width, ar_int_t (*cmp_f)(const void*, const void*));
ar_int_t	AR_bsearch(const void *key, const void *base, size_t num, size_t width, ar_int_t (*cmp_f)(const void*, const void*));
void		AR_nth_elem(void *arr, size_t count, size_t width, size_t idx, ar_int_t (*cmp_f)(const void*, const void*));



/**********************************************************rand*************************************************************/
void			AR_srand(ar_uint_64_t seed);

ar_uint_32_t		AR_rand32();
ar_uint_64_t		AR_rand64();
float			AR_rand_flt();
double			AR_rand_dbl();

/********************************************************CRT String*****************************************************************/



ar_int_t			AR_stricmp(const char *l, const char *r);
ar_int_t			AR_strnicmp(const char *l, const char *r, size_t n);
ar_int_t			AR_wcsicmp(const wchar_t *l, const wchar_t *r);
ar_int_t			AR_wcsnicmp(const wchar_t *l, const wchar_t *r, size_t n);




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
#define AR_ispunct(_c)			ispunct((_c))

char*			AR_strupr(char *s);
char*			AR_strlwr(char *s);
char*			AR_strnlwr(char *s, size_t count);
char*			AR_strnupr(char *s, size_t count);



const char*		AR_strtrim(const char *in, const char *trim);
const char*		AR_strtrim_space(const char *in);


const char*		AR_strtrim_s(const char *in, const char *end, const char *trim);
const char*		AR_strtrim_space_s(const char *in, const char *end);


char*			AR_strtrim_right(char *in, const char *trim);
char*			AR_strtrim_right_space(char *in);


#define AR_wcscmp(_l, _r)		wcscmp((_l), (_r))
#define AR_wcsncmp(_l, _r,_n)	wcsncmp((_l), (_r), (_n))
#define AR_wcslen(_s)			wcslen((_s))
#define AR_wcscpy(_d, _s)		wcscpy((_d), (_s))
#define AR_wcsncpy(_d, _s, _n)	wcsncpy((_d), (_s), (_n))
#define AR_wcschr(_s, _c)		wcschr((_s),(_c))
#define AR_wcsstr(_s1, _s2)		wcsstr((_s1), (_s2))

#define AR_towlower(_c)			towlower((_c))
#define AR_towupper(_c)			towupper((_c))

#define AR_iswalpha(_c)			iswalpha((_c))
#define AR_iswalnum(_c)			iswalnum((_c))
#define AR_iswdigit(_c)			iswdigit((_c))
#define AR_iswxdigit(_c)		iswxdigit((_c))
#define AR_iswspace(_c)			iswspace((_c))
#define AR_iswgraph(_c)			iswgraph((_c))
#define AR_iswprint(_c)			iswprint((_c))
#define AR_iswcntrl(_c)			iswcntrl((_c))
#define AR_iswupper(_c)			iswupper((_c))
#define AR_iswlower(_c)			iswlower((_c))
#define AR_iswpunct(_c)			iswpunct((_c))

ar_int_t AR_iswblank(wchar_t c);



wchar_t*			AR_wcsupr(wchar_t *s);
wchar_t*			AR_wcslwr(wchar_t *s);
wchar_t*			AR_wcsnlwr(wchar_t *s, size_t count);
wchar_t*			AR_wcsnupr(wchar_t *s, size_t count);



wchar_t*		AR_wcsdup(const wchar_t *sour);

/*returns an empty string(L"") when len == 0*/
wchar_t*		AR_wcsndup(const wchar_t *sour, size_t len);


char*			AR_strdup(const char *sour);

/*returns an empty string("") when len == 0*/
char*			AR_strndup(const char *sour, size_t len);


const wchar_t*	AR_wcstrim(const wchar_t *in, const wchar_t *trim);
const wchar_t*	AR_wcstrim_space(const wchar_t *in);

wchar_t*		AR_wcscat(wchar_t *dest, const wchar_t *sour);
wchar_t*		AR_wcsncat(wchar_t *dest, const wchar_t *sour, size_t n);

char*			AR_strcat(char *dest, const char *sour);
char*			AR_strncat(char *dest, const char *sour, size_t n);



/********************************************************************************************************************************************/
const wchar_t*	AR_wcstrim_s(const wchar_t *in, const wchar_t *end, const wchar_t *trim);
const wchar_t*	AR_wcstrim_space_s(const wchar_t *in, const wchar_t *end);



/*This function to modify the string, so there is no _s version, and trim the end of the string L'\0'*/
wchar_t*		AR_wcstrim_right(wchar_t *in, const wchar_t *trim);
wchar_t*		AR_wcstrim_right_space(wchar_t *in);


const char*		AR_strstr_s(const char *s, const char *se, const char *p, const char *pe);
const wchar_t*	AR_wcsstr_s(const wchar_t *s, const wchar_t *se, const wchar_t *p, const wchar_t *pe);
const char*		AR_stristr_s(const char *s, const char *se, const char *p, const char *pe);
const wchar_t*	AR_wcsistr_s(const wchar_t *s, const wchar_t *se, const wchar_t *p, const wchar_t *pe);

const char*		AR_stristr(const char *s, const char *p);
const wchar_t*	AR_wcsistr(const wchar_t *s, const wchar_t *p);



const wchar_t*	AR_reverse_wcschr(const wchar_t* str, size_t l, wchar_t c);
const wchar_t*	AR_reverse_wcsstr(const wchar_t *str, size_t l,  const wchar_t *match, size_t ml);

const wchar_t* AR_reverse_wcsichr(const wchar_t* str, size_t l, wchar_t c);
const wchar_t* AR_reverse_wcsistr(const wchar_t *str, size_t l,  const wchar_t *match, size_t ml);



const char*		AR_reverse_strchr(const char* str, size_t l, char c);
const char*		AR_reverse_strstr(const char *str, size_t l,  const char *match, size_t ml);

const char*		AR_reverse_strichr(const char* str, size_t l, char c);
const char*		AR_reverse_stristr(const char *str, size_t l,  const char *match, size_t ml);




/******************************************************string format*********************************************/


ar_int_t			AR_vscwprintf(const wchar_t *fmt, va_list args);/*Returns a length sufficient to accommodate fmt + args*/
ar_int_t			AR_scwprintf(const wchar_t *fmt, ...);
wchar_t*			AR_vtow(const wchar_t *fmt, ...);

ar_int_t			AR_swprintf(wchar_t *dest, size_t count, const wchar_t *fmt, ...);
ar_int_t			AR_vswprintf(wchar_t *dest, size_t count, const wchar_t *fmt, va_list args);

ar_int_t			AR_swprintf_nonalloc(wchar_t *dest, size_t count, const wchar_t *fmt, ...);
ar_int_t			AR_vswprintf_nonalloc(wchar_t *dest, size_t count, const wchar_t *fmt, va_list args);

ar_int_t			AR_vscprintf(const char *fmt, va_list args);	/*Returns a length sufficient to accommodate fmt + args*/
ar_int_t			AR_scprintf(const char *fmt, ...);
char*				AR_vtos(const char *fmt, ...);

ar_int_t			AR_sprintf(char *dest, size_t count, const char *fmt, ...);
ar_int_t			AR_vsprintf(char *dest, size_t count, const char *fmt, va_list args);




/*************************************************String Number Convert**************************************************/


/*Returned need the length of the array of elements contains 0*/
ar_int_t			AR_i64tos_buf(char *out, size_t nbuf, ar_int_64_t num, size_t radix);
ar_int_t			AR_u64tos_buf(char *out, size_t nbuf, ar_uint_64_t num, size_t radix);

ar_int_t			AR_i64tow_buf(wchar_t *out, size_t nbuf, ar_int_64_t num, size_t radix);
ar_int_t			AR_u64tow_buf(wchar_t *out, size_t nbuf, ar_uint_64_t num, size_t radix);



ar_bool_t			AR_wcs_is_float(const wchar_t *in, const wchar_t *end);
ar_bool_t			AR_wcs_is_int(const wchar_t *in, const wchar_t *end);

ar_bool_t			AR_str_is_float(const char *in, const char *end);
ar_bool_t			AR_str_is_int(const char *in, const char *end);


ar_int_t			AR_wchartodigit(wchar_t ch);
ar_int_t			AR_chartodigit(char ch);


const wchar_t*	AR_wtoi32_s(const wchar_t *in, const wchar_t *end, ar_int_32_t  *num, size_t base);
const wchar_t*	AR_wtou32_s(const wchar_t *in, const wchar_t *end, ar_uint_32_t *num, size_t base);
const wchar_t*	AR_wtoi64_s(const wchar_t *in, const wchar_t *end, ar_int_64_t  *num, size_t base);
const wchar_t*	AR_wtou64_s(const wchar_t *in, const wchar_t *end, ar_uint_64_t  *num, size_t base);


const wchar_t*	AR_wtoi32(const wchar_t *in, ar_int_32_t  *num, size_t base);
const wchar_t*	AR_wtou32(const wchar_t *in, ar_uint_32_t *num, size_t base);
const wchar_t*	AR_wtoi64(const wchar_t *in, ar_int_64_t  *num, size_t base);
const wchar_t*	AR_wtou64(const wchar_t *in, ar_uint_64_t  *num, size_t base);


const char*		AR_stoi64(const char *in,	 ar_int_64_t  *num, size_t base);
const char*		AR_stou64(const char *in,	 ar_uint_64_t  *num, size_t base);
const char*		AR_stoi32(const char *in,  ar_int_32_t *num, size_t base);
const char*		AR_stou32(const char *in,  ar_uint_32_t *num, size_t base);

const char*		AR_stoi64_s(const char *in, const char *end, ar_int_64_t  *num, size_t base);
const char*		AR_stou64_s(const char *in, const char *end, ar_uint_64_t  *num, size_t base);
const char*		AR_stoi32_s(const char *in, const char *end, ar_int_32_t  *num, size_t base);
const char*		AR_stou32_s(const char *in, const char *end, ar_uint_32_t  *num, size_t base);


const wchar_t*	AR_wtod(const wchar_t *in, double *num);
const wchar_t*	AR_wtod_s(const wchar_t *in, const wchar_t *end, double *num);
const char*		AR_stod_s(const char *in, const char *end, double *out);
const char*		AR_stod(const char *in, double *num);



/*********************************************************hash function*********************************************/

ar_uint_t			AR_wcshash(const wchar_t *str);
ar_uint_t			AR_wcshash_n(const wchar_t *str, size_t n);

ar_uint_t			AR_strhash(const char *str);
ar_uint_t			AR_strhash_n(const char *str, size_t n);

ar_uint_t			AR_memhash(const ar_byte_t *data, size_t len);



#if(AR_ARCH_VER	== ARCH_32)

		#define AR_wtoi			AR_wtoi32
		#define AR_wtou			AR_wtou32

		#define AR_wtou_s		AR_wtou32_s
		#define AR_wtoi_s		AR_wtoi32_s


		#define AR_stoi			AR_stoi32
		#define AR_stou			AR_stou32

		#define AR_stou_s		AR_stou32_s
		#define AR_stoi_s		AR_stoi32_s

#elif(AR_ARCH_VER == ARCH_64)

		#define AR_wtoi			AR_wtoi64
		#define AR_wtou			AR_wtou64

		#define AR_wtou_s		AR_wtou64_s
		#define AR_wtoi_s		AR_wtoi64_s

		#define AR_stoi			AR_stoi64
		#define AR_stou			AR_stou64

		#define AR_stou_s		AR_stou64_s
		#define AR_stoi_s		AR_stoi64_s

#else
		
		#error "Not Support Platform!"

#endif





void AR_make_heap(void *arr, size_t count, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*));
void AR_pop_heap(void *arr, size_t count, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*));
void AR_push_heap(void *arr, size_t count, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*));
void AR_sort_heap(void *arr, size_t count, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*));
void AR_remove_heap(void *arr, size_t count, size_t idx, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*));

/********************************************************Basic Data Structure*********************************************************/

/*
List
*/
typedef arStatus_t		(*AR_list_copy_func_t)(void *data, void **pnew_data, void *ctx);
typedef void			(*AR_list_destroy_func_t)(void *data, void *ctx);

typedef struct __arsenal_list_node_tag 
{
        void *data;
        struct __arsenal_list_node_tag  *next;
        struct __arsenal_list_node_tag  *prev;
}arListNode_t;

typedef struct __arsenal_list_tag
{
        arListNode_t            *head;
        arListNode_t            *tail;
        size_t                  count;
        
        void                    *usr_ctx;
		AR_list_copy_func_t		copy_f;
        AR_list_destroy_func_t  dtor_f;
}arList_t;


arList_t*       AR_CreateList(AR_list_copy_func_t copy_f, AR_list_destroy_func_t dtor_f, void *ctx);
void            AR_DestroyList(arList_t *lst);
void			AR_ClearList(arList_t *lst);
arStatus_t      AR_InsertToList(arList_t *lst, arListNode_t *node, void *data);
void			AR_RemoveFromList(arList_t *lst, arListNode_t *node);
size_t          AR_GetListCount(const arList_t *lst);

arStatus_t		AR_PushListBack(arList_t *lst, void *data);
arStatus_t		AR_PushListFront(arList_t *lst, void *data);

arStatus_t		AR_PopListBack(arList_t *lst);
arStatus_t		AR_PopListFront(arList_t *lst);

arStatus_t		AR_GetListFront(arList_t *lst, void **pdata);
arStatus_t		AR_GetListBack(arList_t *lst, void **pdata);

#define			AR_IsEmptyList(_lst)	(AR_GetListCount((_lst)) == 0 ? true : false)


/*
Hash
*/

typedef ar_uint_64_t    (*AR_hash_hash_func_t)(void *key, void *ctx);
typedef ar_int_t        (*AR_hash_comp_func_t)(void *l, void *r, void *ctx);

typedef arStatus_t      (*AR_hash_copy_func_t)(void *data, void **pnew_data, void *ctx);
typedef void            (*AR_hash_destroy_func_t)(void *key, void *ctx);
typedef ar_bool_t       (*AR_hash_visit_func_t)(void *key, void *data, void *ctx);/*When it returns false loop terminates*/


typedef struct __arsenal_hash_node_tag
{
		void	*key;
		void	*val;
		struct __arsenal_hash_node_tag	*next;
}arHashNode_t;

typedef struct __arsenal_hash_tag
{
		arHashNode_t	**bucket;
		ar_uint_64_t		bucket_size;
		ar_uint_64_t		item_count;

		AR_hash_hash_func_t		hash_f;
		AR_hash_comp_func_t		comp_f;
		AR_hash_copy_func_t		copykey_f;
		AR_hash_copy_func_t		copyval_f;
		
		AR_hash_destroy_func_t	destroy_key_f;
		AR_hash_destroy_func_t	destroy_val_f;
		void					*usr_ctx;
}arHash_t;

arHash_t*		AR_CreateHash(size_t bucket_size, AR_hash_hash_func_t hash_f, AR_hash_comp_func_t comp_f, AR_hash_copy_func_t copykey_f, AR_hash_copy_func_t copyval_f, AR_hash_destroy_func_t destroy_key_f, AR_hash_destroy_func_t destroy_val_f, void *usr_ctx);
void			AR_DestroyHash(arHash_t *hash);
void			AR_ClearHash(arHash_t *hash);

arStatus_t		AR_InsertToHash(arHash_t *hash, void *key, void *val);
arStatus_t		AR_SetToHash(arHash_t *hash, void *key, void *val);

arStatus_t		AR_RemoveFromHash(arHash_t *hash, void *key);
arStatus_t		AR_FindFromHash(arHash_t *hash, void *key, void **pval);
size_t			AR_GetHashCount(arHash_t *hash);
#define			AR_IsEmptyHash(_h)	(AR_GetHashCount((_h)) == 0 ? true : false)

void			AR_HashForEach(arHash_t *hash, AR_hash_visit_func_t visit);


typedef struct __arsenal_hash_iterator_tag
{
		arHash_t		*hash;
		size_t			bucket_idx;
		arHashNode_t	*curr;
}arHashIter_t;

void	AR_InitHashIterator(arHash_t *hash, arHashIter_t *iter);
void	AR_UnInitHashIterator(arHashIter_t *iter);

ar_bool_t	AR_HashIteratorIsDone(const arHashIter_t *iter);
void	AR_HashIteratorNext(arHashIter_t *iter);
void*	AR_GetHashIteratorKey(const arHashIter_t *iter);
void*	AR_GetHashIteratorData(const arHashIter_t *iter);



/*********************************************************String Convert****************************************************/

/*
Under the windows need to modify __get_codepage_for_winapi，If the platform supports iconv library, modify __get_locale_str_for_iconv


*/

typedef enum
{
		AR_CP_ACP,
		AR_CP_UTF8,
		AR_CP_BIG5,
		AR_CP_GB2312,
		AR_CP_GB18030,
		AR_CP_MAX
}arCodePage_t;

ar_int_t                AR_str_to_wcs_buf(arCodePage_t cp, const char *acp, size_t n, wchar_t *out, size_t out_len);
ar_int_t                AR_wcs_to_str_buf(arCodePage_t cp, const wchar_t *input, size_t n, char *out, size_t out_len);


char*                   AR_wcs_to_str(arCodePage_t cp, const wchar_t *input, size_t in_n);
wchar_t*				AR_str_to_wcs(arCodePage_t cp, const char *input, size_t in_n);


/*******************************************************BitMark****************************************************************/


/*#define AR_BIT_MARK(_pos)	(((ar_uint_64_t)0x01) << ((ar_uint_64_t)(_pos)))*/

static AR_INLINE ar_uint_64_t AR_BIT_MARK(ar_uint_64_t pos) { return AR_BIGNUM_U64(0x01) << pos; }

#define AR_BIT_TEST(_val, _pos)  ((((_val) & (AR_BIT_MARK((_pos))))))

#define AR_BIT_SET(_val, _pos)   ((_val) |= (AR_BIT_MARK((_pos))))

#define AR_BIT_CLR(_val, _pos)   (((_val)) &=  (~(AR_BIT_MARK((_pos)))))



#define AR_MASK1(_n,_p)	( (~( (~(ar_uint_64_t)0) << (_n))) << (_p) )

#define AR_MASK0(_n,_p)	(~AR_MASK1((_n),(_p)))






/***************************************************************Misc**********************************************************/
#define	AR_ESCSTR_ERR_OK				0x00
#define	AR_ESCSTR_ERR_VALUE				0x01
#define	AR_ESCSTR_ERR_CHAR				0x02
#define AR_ESCSTR_ERR_BUFFER			0x03
#define AR_ESCSTR_ERR_MEMORY			0x04

typedef struct __escape_string_error_tag
{
		ar_int_t			type;
		const wchar_t	*pos;
		ar_uint_64_t		value;
}arEscStrErr_t;


wchar_t*		AR_escstr_to_str(const wchar_t *src, arEscStrErr_t *error);
wchar_t*		AR_str_to_escstr(const wchar_t *src);

wchar_t*		AR_escstr_to_str_n(const wchar_t *src, size_t n, arEscStrErr_t *error);
wchar_t*		AR_str_to_escstr_n(const wchar_t *src, size_t n);


ar_int_t 			AR_escstr_to_str_buf(wchar_t *dest, size_t len, const wchar_t *src, arEscStrErr_t *error);
ar_int_t 			AR_str_to_escstr_buf(wchar_t *dest, size_t len, const wchar_t *src);


ar_int_t 			AR_escstr_to_str_buf_n(wchar_t *dest, size_t len, const wchar_t *src, size_t n,arEscStrErr_t *error);
ar_int_t 			AR_str_to_escstr_buf_n(wchar_t *dest, size_t len, const wchar_t *src, size_t n);


ar_int_t		AR_hexstr_to_data(const char *s, ar_byte_t *data, size_t len);
ar_int_t		AR_hexstr_to_data_s(const char *b, const char *e, ar_byte_t *data, size_t len);
ar_int_t		AR_data_to_hexstr(const ar_byte_t *data, size_t l, char *out, size_t len, ar_bool_t is_upper);
ar_int_t		AR_hexwcs_to_data(const wchar_t *s, ar_byte_t *data, size_t len);
ar_int_t		AR_hexwcs_to_data_s(const wchar_t *b, const wchar_t *e, ar_byte_t *data, size_t len);
ar_int_t		AR_data_to_hexwcs(const ar_byte_t *data, size_t l, wchar_t *out, size_t len, ar_bool_t is_upper);

/*************************************************encode && decode**************************************************/

/**************************************************************rot13*********************************************************/
char*			AR_strrot13(char *s, size_t n);

/**************************************************************base64*********************************************************/
ar_int_t        AR_base64_encode(ar_byte_t  *out, size_t olen, const ar_byte_t *input, size_t ilen);
ar_int_t        AR_base64_decode(ar_byte_t  *out, size_t olen, const ar_byte_t *input, size_t ilen);



size_t          AR_str_similar_text(const char *s1, size_t l1, const char *s2, size_t l2);
size_t          AR_wcs_similar_text(const wchar_t *s1, size_t l1, const wchar_t *s2, size_t l2);


ar_bool_t		AR_str_match_wildcard(const char *s, const char *p);
ar_bool_t		AR_str_match_wildcard_s(const char *beg, const char *end, const char *pb, const char *pe);
ar_bool_t		AR_wcs_match_wildcard(const wchar_t *s, const wchar_t *p);
ar_bool_t		AR_wcs_match_wildcard_s(const wchar_t *beg, const wchar_t *end, const wchar_t *pb, const wchar_t *pe);




/********************************************************String*****************************************************************/

typedef struct __arsenal_string_tag		arString_t;




arString_t*		AR_CreateString();
void			AR_DestroyString(arString_t *str);

/*Space reserved num of wchar_t*/
arStatus_t		AR_ReserveString(arString_t *str, size_t num);
void			AR_ClearString(arString_t *str);

arStatus_t		AR_AppendString(arString_t *str, const wchar_t *sour);
arStatus_t		AR_AppendStringN(arString_t *str, const wchar_t *sour, size_t n);

#define			AR_CatString(_dest, _str)		AR_AppendString((_dest), AR_GetStringCString((_str)))

arStatus_t		AR_FormatString(arString_t *str, const wchar_t *fmt, ...);
arStatus_t		AR_AppendFormatString(arString_t *str, const wchar_t *fmt, ...);

arStatus_t		AR_VFormatString(arString_t *str, const wchar_t *fmt, va_list va_args);
arStatus_t		AR_AppendVFormatString(arString_t *str, const wchar_t *fmt, va_list va_args);


arStatus_t		AR_AppendCharToString(arString_t *str, wchar_t chr);

arStatus_t		AR_SetString(arString_t *str, const wchar_t *wcs);
arStatus_t		AR_SetStringN(arString_t *str, const wchar_t *wcs, size_t n);

arStatus_t		AR_CopyString(arString_t *src, const arString_t *dest);
arString_t*		AR_CopyNewString(arString_t *src);

const wchar_t*	AR_GetStringCString(const arString_t *str);
#define			AR_CSTR		AR_GetStringCString

size_t			AR_GetStringLength(const arString_t *str);

#define			AR_IsEmptyString(_str)	(AR_GetStringLength((_str)) == 0 ? true : false)

wchar_t			AR_GetStringChar(const arString_t *str, size_t index);
void			AR_SetStringChar(arString_t *str, size_t index, wchar_t c);


#define			AR_StrPrint(_s) do{ AR_printf(L"%ls\r\n", AR_GetStringCString((_s))); }while(0)
#define			AR_StrPrintCtx(_ctx, _s)do{ AR_printf_ctx((_ctx), L"%ls\r\n", AR_GetStringCString((_s))); }while(0)

ar_int_t		AR_CompStringWithWcs(const arString_t *l, const wchar_t *r);
ar_int_t		AR_CompString(const arString_t *l, const arString_t *r);

ar_int_t		AR_ICompStringWithWcs(const arString_t *l, const wchar_t *r);
ar_int_t		AR_ICompString(const arString_t *l, const arString_t *r);

void			AR_StringToLower(arString_t *l);
void			AR_StringToUpper(arString_t *l);

void			AR_SwapString(arString_t *l, arString_t *r);

size_t			AR_EraseString(arString_t *str, size_t pos, size_t nchars);

arStatus_t      AR_GetLineFromString(arString_t *str, arString_t *line);
arStatus_t      AR_GetLineFromStringWithLineSP(arString_t *str, arString_t *line, const wchar_t *line_sp);

/********************************************************StringTable*****************************************************************/




typedef struct __string_table_			arStringTable_t;




#define		MIN_BUCKET_SIZE		(139)


arStringTable_t*		AR_CreateStrTable(size_t count);
void					AR_DestroyStrTable(arStringTable_t* tbl);


const wchar_t*			AR_GetString(arStringTable_t *tbl, const wchar_t *str);
const wchar_t*			AR_GetStringN(arStringTable_t *tbl, const wchar_t *str, size_t n);

arStatus_t				AR_HasString(const arStringTable_t *tbl, const wchar_t *str);
arStatus_t				AR_HasStringN(const arStringTable_t *tbl, const wchar_t *str, size_t n);

const wchar_t*			AR_GetStringUInt(arStringTable_t *tbl, ar_uint_64_t num, size_t radix);
const wchar_t*			AR_GetStringInt(arStringTable_t *tbl, ar_int_64_t num, size_t radix);
const wchar_t*			AR_GetStringFloat(arStringTable_t *tbl, double num, size_t prec);





/**************************************************Buffer**********************************************************************/

struct arsenal_buffer_tag;
typedef struct arsenal_buffer_tag		arBuffer_t;

arBuffer_t*		AR_CreateBuffer(size_t nbytes);
void			AR_DestroyBuffer(arBuffer_t		*buffer);

void			AR_ClearBuffer(arBuffer_t		*buffer);
void            AR_SwapBuffer(arBuffer_t *l, arBuffer_t *r);

arStatus_t		AR_CopyBuffer(arBuffer_t *dest, const arBuffer_t *src);
arBuffer_t*		AR_CopyNewBuffer(const arBuffer_t *buf);

/*nbytes bytes are allocated for use*/
ar_byte_t*			AR_AllocFromBuffer(arBuffer_t *buffer, size_t	nbytes);
/*Nbytes bytes to be written to the buffer*/
arStatus_t		AR_InsertToBuffer(arBuffer_t *buffer, const ar_byte_t *data, size_t len);




/*Nbytes bytes from the buffer in front erase*/
size_t			AR_EraseBuffer(arBuffer_t *buffer, size_t nbytes);
size_t			AR_EraseBufferBack(arBuffer_t *buffer, size_t nbytes);

/*Returns the number of bytes not reallocate memory can also write*/
size_t			AR_GetBufferCapacity(const arBuffer_t *buffer);

/*AR_ReserveBuffer调用成功后，AR_GetBufferCapacity(buffer) >= nbytes*/
arStatus_t		AR_ReserveBuffer(arBuffer_t *buffer, size_t nbytes);

/*Readable memory block*/
const ar_byte_t*	AR_GetBufferData(const arBuffer_t *buffer);
/*The readable memory block length*/
size_t			AR_GetBufferAvailable(const arBuffer_t *buffer);

void            AR_ResetBufferData(arBuffer_t *buffer, size_t offset, const ar_byte_t *data, size_t len);

size_t			AR_ReadBufferData(arBuffer_t *buffer, ar_byte_t *dest, size_t len);

/*************************************helper****************************************/
arStatus_t		AR_InsertCStringToBuffer(arBuffer_t *buffer, const char *str);
arStatus_t		AR_InsertBufferToBuffer(arBuffer_t *buffer, const arBuffer_t *other);
arStatus_t      AR_GetLineFromBufferWithLineSP(arBuffer_t *buf, char *line, size_t *l, const char *line_sp);



/************************************************************URI****************************************************************/


struct __arsenal_uri_tag;
typedef struct __arsenal_uri_tag		arURI_t;


arURI_t*		AR_CreateURI(arCodePage_t cp);
void			AR_DestroyURI(arURI_t *uri);
void			AR_ClearURI(arURI_t *uri);
ar_int_t		AR_CompURI(const arURI_t *l, const arURI_t *r);
arStatus_t		AR_NormalizeURI(arURI_t *uri);

arCodePage_t	AR_GetURICodePage(const arURI_t *uri);
void			AR_SetURICodePage(arURI_t *uri, arCodePage_t cp);

ar_bool_t		AR_IsRelativeURI(const arURI_t *uri);


/*这里默认认为输入的URI完全为编码后的URL，之后根据URI设置的code page将编码后的uri解码，并转换为unicode*/
arStatus_t		AR_SetURI(arURI_t *uri, const wchar_t *str);
arStatus_t		AR_SetEncodedURI(arURI_t *uri, const wchar_t *str);

/*get unicode encoded URI*/
arStatus_t		AR_GetURI(const arURI_t *uri, arString_t *str);
arStatus_t		AR_GetEncodedURI(const arURI_t *uri, arString_t *str);



arStatus_t		AR_GetURIScheme(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIScheme(arURI_t *uri, const wchar_t *str);

arStatus_t		AR_GetURIUserInfo(const arURI_t *uri, arString_t *str);

arStatus_t		AR_SetURIUserInfo(arURI_t *uri, const wchar_t *str);
arStatus_t		AR_SetURIEncodedUserInfo(arURI_t *uri, const wchar_t *str);

arStatus_t		AR_GetURIHost(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIHost(arURI_t *uri, const wchar_t *str);

ar_uint_16_t		AR_GetURIPort(const arURI_t *uri);
void			AR_SetURIPort(arURI_t *uri, ar_uint_16_t port);


arStatus_t		AR_GetURIAuthority(const arURI_t *uri, arString_t *str);
arStatus_t		AR_GetURIEncodedAuthority(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIAuthority(arURI_t *uri, const wchar_t *str);
arStatus_t		AR_SetURIEncodedAuthority(arURI_t *uri, const wchar_t *str);


arStatus_t		AR_GetURIPath(const arURI_t *uri, arString_t *str);
arStatus_t		AR_GetURIEncodedPath(const arURI_t *uri, arString_t *str);

arStatus_t		AR_SetURIPath(arURI_t *uri, const wchar_t *str);
arStatus_t		AR_SetURIEncodedPath(arURI_t *uri, const wchar_t *str);
		
arStatus_t		AR_GetURIQuery(const arURI_t *uri, arString_t *str);
arStatus_t		AR_GetURIEncodedQuery(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIQuery(arURI_t *uri, const wchar_t *str);
arStatus_t		AR_SetURIEncodedQuery(arURI_t *uri, const wchar_t *str);


arStatus_t		AR_GetURIFragment(const arURI_t *uri, arString_t *str);
arStatus_t		AR_GetURIEncodedFragment(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIFragment(arURI_t *uri, const wchar_t *str);
arStatus_t		AR_SetURIEncodedFragment(arURI_t *uri, const wchar_t *str);


arStatus_t		AR_GetURIPathEtc(const arURI_t *uri, arString_t *str);
arStatus_t		AR_GetURIEncodedPathEtc(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIPathEtc(arURI_t *uri, const wchar_t *str);
arStatus_t		AR_SetURIEncodedPathEtc(arURI_t *uri, const wchar_t *str);


arStatus_t		AR_EncodeURLString(arCodePage_t cp, const wchar_t *uri, arString_t *out);
arStatus_t		AR_DecodeURLString(arCodePage_t cp, const wchar_t *uri, arString_t *out);



/********************************************************************************/

arHash_t*		AR_CreateURIQueryTable();
void			AR_DestroyURIQueryTable(arHash_t *tbl);
#define         AR_ClearURIQueryTable(_tbl)     AR_ClearHash((_tbl))

arStatus_t		AR_GetURIQueryItems(const arURI_t *uri, arHash_t *hash);
arStatus_t		AR_SetURIQueryItems(arURI_t *uri, const arHash_t *hash);


/***************************************************************Text**********************************************************/


typedef enum
{
		AR_TXT_BOM_NONE		=		0x01,
		AR_TXT_BOM_UTF16_BE		=		0x02,
		AR_TXT_BOM_UTF16_LE		=		0x04,
		AR_TXT_BOM_UTF_8		=		0x08,
		AR_TXT_BOM_UTF32_LE		=		0x10,
		AR_TXT_BOM_UTF32_BE		=		0x20
}arTxtBom_t;

arStatus_t  AR_DetectTextBom(const ar_byte_t *data, size_t length, arTxtBom_t *bom, size_t *bom_len);

arStatus_t	AR_LoadBomTextFromBinary(arBuffer_t *input, arTxtBom_t *bom, arString_t *out);
arStatus_t	AR_LoadBomTextFile(const wchar_t *path, arTxtBom_t *bom, arString_t *out);

arStatus_t	AR_SaveBomTextFile(const wchar_t *path, arTxtBom_t bom, const wchar_t *input);
arStatus_t	AR_SaveBomTextToBinary(arBuffer_t *output, arTxtBom_t bom, const wchar_t *input);


arStatus_t	AR_LoadBomTextFromBinaryWithCodePage(arBuffer_t *input, arTxtBom_t *bom, arString_t *out, arCodePage_t code_page);
arStatus_t	AR_LoadBomTextFileWithCodePage(const wchar_t *path, arTxtBom_t *bom, arString_t *out, arCodePage_t code_page);

arStatus_t	AR_SaveBomTextToBinaryWithCodePage(arBuffer_t *output, arTxtBom_t bom, const wchar_t *input, arCodePage_t code_page);
arStatus_t	AR_SaveBomTextFileWithCodePage(const wchar_t *path, arTxtBom_t bom, const wchar_t *input, arCodePage_t code_page);


/************************************************************************************************************************************************************************/


/**********************************************************System*************************************************************/



/**********************************************************Threading*************************************************************/

/*The thread module initialization function, and can only be invoked by AR_Init*/
void			AR_InitThread();
void			AR_UnInitThread();


/**********************************************************atomic*************************************************************/
ar_int_t			AR_AtomicInc(volatile ar_int_t *dest);
ar_int_t			AR_AtomicDec(volatile ar_int_t *dest);
ar_bool_t			AR_AtomicCompExch(volatile ar_int_t *dest, ar_int_t val, ar_int_t cmpval);


/**********************************************************spinlock*************************************************************/

#define			AR_MAXSPIN_COUNT		5000

#if defined(OS_FAMILY_UNIX)
	
		#if(OS_TYPE == OS_IOS || OS_TYPE == OS_MAC_OS_X || OS_TYPE == OS_ANDROID)
				typedef			volatile ar_int_t				arSpinLock_t;	
		#else
				typedef			pthread_spinlock_t              arSpinLock_t;
		#endif

#elif defined(OS_FAMILY_WINDOWS)
		typedef         volatile ar_int_t					arSpinLock_t;
#else

#endif


void			AR_InitSpinLock(arSpinLock_t *lock);
void			AR_UnInitSpinLock(arSpinLock_t *lock);
void			AR_LockSpinLock(arSpinLock_t *lock);
void			AR_UnLockSpinLock(arSpinLock_t *lock);
ar_bool_t		AR_TryLockSpinLock(arSpinLock_t *lock);

void			AR_YieldThread();
void			AR_Sleep(ar_int_64_t millisecond);

ar_int_64_t		AR_GetTime_Microseconds();
ar_int_64_t		AR_GetTime_Milliseconds();

ar_int_64_t		AR_GetTime_TickCount();

/**********************************************************thread*************************************************************/

struct __arsenal_thread_tag;
typedef struct __arsenal_thread_tag arThread_t;

typedef	void	(*arThreadFunc_t)(void *data);

arThread_t*		AR_CreateThread(arThreadFunc_t func, void *data);
void			AR_DestroyThread(arThread_t *thd);
arStatus_t		AR_JoinThread(arThread_t *thd);
arStatus_t		AR_JoinThreadWithTimeout(arThread_t *thd, ar_int_64_t milliseconds);
ar_uint_64_t	AR_GetThreadId(arThread_t *thd);

typedef enum
{
		AR_THREAD_PREC_LOW,
		AR_THREAD_PREC_NORMAL, 
		AR_THREAD_PREC_HIGH
}arThreadPrio_t;

arStatus_t		AR_SetThreadPriority(arThread_t *thd, arThreadPrio_t prio);
arStatus_t		AR_GetThreadPriority(arThread_t *thd, arThreadPrio_t *p_prio);




/**********************************************************mutex*************************************************************/

typedef struct __arsenal_mutex_tag		arMutex_t;

arMutex_t*		AR_CreateMutex();
void			AR_DestroyMutex(arMutex_t *mtx);
arStatus_t		AR_LockMutex(arMutex_t *mtx);
arStatus_t		AR_TryLockMutex(arMutex_t *mtx);
arStatus_t		AR_UnLockMutex(arMutex_t *mtx);


/**********************************************************event*************************************************************/

typedef struct __arsenal_event_tag		arEvent_t;

arEvent_t*		AR_CreateEvent(ar_bool_t is_auto_reset);
void			AR_DestroyEvent(arEvent_t *evt);
arStatus_t		AR_SetEvent(arEvent_t *evt);
arStatus_t		AR_WaitEvent(arEvent_t *evt);
arStatus_t		AR_WaitEventWithTimeout(arEvent_t *evt, ar_int_64_t milliseconds);
arStatus_t		AR_TryWaitEvent(arEvent_t *evt);
arStatus_t		AR_ResetEvent(arEvent_t *evt);



/**********************************************************async queue*************************************************************/


struct __arsenal_async_queue_tag;
typedef struct __arsenal_async_queue_tag arAsyncQueue_t;


struct __async_data_node_tag;
typedef struct __async_data_node_tag	asyncDataNode_t;

struct async_wait_info_tag;
typedef struct async_wait_info_tag		asyncWaitInfo_t;

struct __async_wait_node_tag;
typedef struct __async_wait_node_tag	asyncWaitNode_t;


struct __arsenal_async_queue_tag
{
		arSpinLock_t			mutex;
		asyncWaitNode_t			*wait_head;
		asyncWaitNode_t			*wait_tail;
		size_t					wait_cnt;

		asyncDataNode_t			*data_head;
		asyncDataNode_t			*data_tail;
		size_t					data_cnt;
};



void	AR_InitAsyncQueue(arAsyncQueue_t *queue);
void	AR_UnInitAsyncQueue(arAsyncQueue_t *queue);
void	AR_ClearAsyncQueue(arAsyncQueue_t *queue);

arStatus_t	AR_GetFromAsyncQueue(arAsyncQueue_t *queue, void **pdata);
arStatus_t	AR_GetFromAsyncQueueWithTimeout(arAsyncQueue_t *queue, void **pdata, ar_int_64_t	millisecond);
arStatus_t	AR_PutToAsyncQueue(arAsyncQueue_t *queue, void *data);
arStatus_t	AR_PutToAsyncQueueFront(arAsyncQueue_t *queue, void *data);/*when there is no waiting thread, this Data to the front of the queue*/

arStatus_t	AR_AsyncQueueIsEmpty(const arAsyncQueue_t *queue);
arStatus_t	AR_HasIdleThreadInAsyncQueue(const arAsyncQueue_t *queue); 

size_t		AR_GetAsyncQueueWaitCount(const arAsyncQueue_t *queue);
size_t		AR_GetAsyncQueueDataCount(const arAsyncQueue_t *queue);


arStatus_t	AR_PeekFromAsyncQueue(arAsyncQueue_t *queue, void **pdata); /*this function can only be used in single consumer model*/




/***************************************************************************************************************************************/



/**********************************************************Environment*************************************************************/

arStatus_t		AR_getenv(const wchar_t *key, arString_t *val);
arStatus_t		AR_setenv(const wchar_t *key, const wchar_t *val);
arStatus_t		AR_hasenv(const wchar_t *key);




/***************************************************************************************************************************************/




/**********************************************************File System*************************************************************/

/*Path*/
arStatus_t		AR_GetCurrentPath(arString_t *str);
arStatus_t		AR_GetHomePath(arString_t *str);
arStatus_t		AR_GetTempPath(arString_t *str);
arStatus_t		AR_GetExpandPath(const wchar_t *path, arString_t *expanded_path);
arStatus_t		AR_GetNullPath(arString_t *str);
arStatus_t		AR_GetRealPath(const wchar_t *path, arString_t *full_path);
arStatus_t		AR_GetExpandRealPath(const wchar_t *path, arString_t *full_path);

/*Path Iterator*/

struct __arsenal_path_iterator_tag;
typedef struct __arsenal_path_iterator_tag arPathIter_t; 



arPathIter_t*	AR_CreatePathIterator();
void			AR_DestroyPathIterator(arPathIter_t *iter);
arStatus_t      AR_PathIteratorSetPath(arPathIter_t *iter, const wchar_t *path);

const wchar_t*	AR_PathIteratorCurrent(const arPathIter_t *iter);
arStatus_t		AR_PathIteratorNext(arPathIter_t *iter);
ar_bool_t		AR_PathIteratorIsDone(const arPathIter_t *iter);
const wchar_t*  AR_PathIteratorPath(const arPathIter_t *iter);


/***********************************************************File*********************************/
typedef void			arFile_t;

arStatus_t				AR_open_file(arFile_t **pfile, const wchar_t *path, const wchar_t *mode);
void					AR_close_file(arFile_t *f);

arStatus_t				AR_read_file(arFile_t *file, ar_byte_t *data, size_t len, size_t *rn);
arStatus_t				AR_write_file(arFile_t *file, const ar_byte_t *data, size_t len, size_t *wn);
arStatus_t				AR_eof_file(arFile_t *file);
arStatus_t				AR_error_file(arFile_t *file);

typedef enum
{
		AR_FILE_SEEK_BEG,
		AR_FILE_SEEK_CUR,
		AR_FILE_SEEK_END
}arFileSeekType_t;

arStatus_t				AR_seek_file(arFile_t *file, ar_int_64_t offset, arFileSeekType_t where);
arStatus_t				AR_tell_file(arFile_t *file, ar_uint_64_t *offset);
/***************************************************************************************************************************************/



/***********************************************************Path********************************************************/


arStatus_t      AR_path_is_hidden(const wchar_t *path);
arStatus_t      AR_path_is_dev(const wchar_t *path);
arStatus_t      AR_path_is_dir(const wchar_t *path);

arStatus_t      AR_path_is_link(const wchar_t *path);
arStatus_t      AR_path_is_file(const wchar_t *path);


arStatus_t      AR_path_is_executable(const wchar_t *path);
arStatus_t      AR_path_is_writeable(const wchar_t *path);
arStatus_t      AR_path_is_readable(const wchar_t *path);

arStatus_t      AR_path_is_existed(const wchar_t *path);

arStatus_t      AR_path_set_executable(const wchar_t *path, ar_bool_t flag);
arStatus_t      AR_path_set_writeable(const wchar_t *path, ar_bool_t flag);
arStatus_t      AR_path_rename(const wchar_t *src_path, const wchar_t *dest_path);
arStatus_t      AR_path_remove(const wchar_t *path);
arStatus_t      AR_path_create_file(const wchar_t *path);
arStatus_t      AR_path_create_dir(const wchar_t *path);

arStatus_t      AR_path_create_path(const wchar_t *path);
arStatus_t      AR_path_remove_path(const wchar_t *path);


arStatus_t      AR_path_copyfile(const wchar_t *src, const wchar_t *dest, ar_bool_t truncated);
arStatus_t      AR_path_copylink(const wchar_t *src, const wchar_t *dest, ar_bool_t truncated);

arStatus_t      AR_path_get_size(const wchar_t *path, ar_uint_64_t *ps);
arStatus_t      AR_path_set_size(const wchar_t *path, ar_uint_64_t size);


arStatus_t      AR_path_copydir(const wchar_t *src, const wchar_t *dest);


/*******************************************************************Math*************************************************************************************************/

#define AR_DBL_PI	3.141592653589793238462643383
#define AR_FLT_PI	3.14159265358979323846f

float  AR_nan_value_flt();
float  AR_inf_value_flt();

double  AR_nan_value_dbl();
double  AR_inf_value_dbl();




ar_bool_t	AR_is_inf_flt(float num);
ar_bool_t	AR_is_inf_dbl(double num);
ar_bool_t	AR_is_nan_flt(float num);
ar_bool_t	AR_is_nan_dbl(double num);


#define AR_FLOAT_PRINT_PRECISION		3



ar_int_32_t		AR_abs_32(ar_int_32_t x);
ar_int_64_t		AR_abs_64(ar_int_64_t x);


ar_bool_t		AR_is_equal_flt(float x, float y, float epsilon);

float			AR_logbase_flt(float a, float base);
float			AR_loge_flt(float a);
float			AR_abs_flt(float x);
float			AR_ceil_flt(float f);
float			AR_floor_flt(float f);
float			AR_sqrt_flt(float f);
float			AR_exp_flt(float f);
float			AR_pow_flt(float x, float y);

float			AR_sin_flt(float f);
float			AR_cos_flt(float f);
float			AR_tan_flt(float f);
float			AR_asin_flt(float f);
float			AR_acos_flt(float f);
float			AR_atan_flt(float f);
float			AR_atan2_flt(float y, float x);


ar_bool_t		AR_is_equal_dbl(double x, double y, double epsilon);

double			AR_logbase_dbl(double a, double base);
double			AR_loge_dbl(double a);
double			AR_abs_dbl(double x);
double			AR_ceil_dbl(double f);
double			AR_floor_dbl(double f);
double			AR_sqrt_dbl(double f);
double			AR_exp_dbl(double f);
double			AR_pow_dbl(double x, double y);

double			AR_sin_dbl(double f);
double			AR_cos_dbl(double f);
double			AR_tan_dbl(double f);
double			AR_asin_dbl(double f);
double			AR_acos_dbl(double f);
double			AR_atan_dbl(double f);
double			AR_atan2_dbl(double y, double x);



/*float macro_oper*/

#define AR_FLT_EQ(_x, _y)		AR_is_equal_flt((float)(_x), (float)(_y), FLT_EPSILON)
#define AR_FLT_LE(_x, _y)		(ar_bool_t) ((((float)(_x)) < ((float)(_y))))
#define AR_FLT_GE(_x, _y)		(ar_bool_t) ((((float)(_x)) > ((float)(_y))))
#define AR_FLT_LEEQ(_x, _y)		(ar_bool_t)( AR_FLT_EQ((_x), (_y)) || AR_FLT_LE((_x), (_y)))
#define AR_FLT_GEEQ(_x, _y)		(ar_bool_t)( AR_FLT_EQ((_x), (_y)) || AR_FLT_GE((_x), (_y)))

#define AR_FLT_MOD(_x,_y)		((float)(_x) - AR_floor_flt((float)(_x) / (float)(_y)) * (float)(_y))
#define AR_FLT_POW(_x,_y)		(float)AR_pow_flt((float)(_x), (float)(_y))


#define AR_DBL_EQ(_x, _y)		AR_is_equal_dbl((double)(_x), (double)(_y), DBL_EPSILON)
#define AR_DBL_LE(_x, _y)		(ar_bool_t) ((((double)(_x)) < ((double)(_y))))
#define AR_DBL_GE(_x, _y)		(ar_bool_t) ((((double)(_x)) > ((double)(_y))))
#define AR_DBL_LEEQ(_x, _y)		(ar_bool_t)( AR_DBL_EQ((_x), (_y)) || AR_DBL_LE((_x), (_y)))
#define AR_DBL_GEEQ(_x, _y)		(ar_bool_t)( AR_DBL_EQ((_x), (_y)) || AR_DBL_GE((_x), (_y)))

#define AR_DBL_MOD(_x,_y)		((double)(_x) - AR_floor_dbl((double)(_x) / (double)(_y)) * (double)(_y))
#define AR_DBL_POW(_x,_y)		(double)AR_pow_dbl((double)(_x), (double)(_y))



#define AR_CMP_FLT(_a,_b) (  AR_FLT_LE( (_a), (_b) ) ? -1 : ( AR_FLT_EQ((_a),(_b)) ? 0 : 1))
#define AR_CMP_DBL(_a,_b) (  AR_DBL_LE( (_a), (_b) ) ? -1 : ( AR_DBL_EQ((_a),(_b)) ? 0 : 1))



/***************************************************************Vector**********************************************************/



struct __arsenal_vector_tag;
typedef struct __arsenal_vector_tag arVector_t;


arVector_t*		AR_CreateVector(size_t size);
void			AR_DestroyVector(arVector_t *vec);
arVector_t*		AR_CopyNewVector(const arVector_t *vec);
arStatus_t		AR_CopyVector(arVector_t *dest, const arVector_t *src);

arStatus_t		AR_SetVectorSize(arVector_t *vec, size_t size);
size_t			AR_GetVectorSize(const arVector_t *vec);

arStatus_t		AR_SetVectorData(arVector_t *vec, size_t size, const double *data);
const double*	AR_GetVectorData(const arVector_t *vec);

void			AR_SwapElements(arVector_t *vec, size_t l, size_t r);

double			AR_GetVectorValue(const arVector_t *vec, size_t idx);
void			AR_SetVectorValue(arVector_t *vec, size_t idx, double val);

ar_int_t		AR_CompareVector(const arVector_t *l, const arVector_t *r, double epsilon);


void			AR_ZeroVector(arVector_t *vec);
void			AR_NegateVector(arVector_t *vec);
void			AR_ClampVector(arVector_t *vec, double minval, double maxval);
void			AR_RandomVector(arVector_t *vec);
void			AR_NormalizeVector(arVector_t *vec);


void			AR_CalcVectorLength(const arVector_t *vec, double *ret);
void			AR_CalcVectorLengthSqr(const arVector_t *vec, double *ret);
void			AR_CalcVectorInnerProduct(const arVector_t *vec, const arVector_t *other, double *ret);
arStatus_t		AR_CalcVectorDistanceByVector(const arVector_t *vec, const arVector_t *other, double *ret);




void			AR_AddVectorByVectorSelf(arVector_t *vec, const arVector_t *other);
void			AR_SubVectorByVectorSelf(arVector_t *vec, const arVector_t *other);
void			AR_MulVectorByValSelf(arVector_t *vec, double val);
void			AR_DivVectorByValSelf(arVector_t *vec, double val);



arStatus_t		AR_AddVectorByVector(const arVector_t *vec, const arVector_t *other, arVector_t *dest);
arStatus_t		AR_SubVectorByVector(const arVector_t *vec, const arVector_t *other, arVector_t *dest);
arStatus_t		AR_MulVectorByVal(const arVector_t *vec, double val, arVector_t *dest);
arStatus_t		AR_DivVectorByVal(const arVector_t *vec, double val, arVector_t *dest);




typedef enum
{
		AR_VEC_NORM_1,
		AR_VEC_NORM_2,
		AR_VEC_NORM_MAX,
		AR_VEC_NORM_NEGAMAX
}arVectorNormType_t;


arStatus_t		AR_CalcVectorNormNumber(const arVector_t *vec, arVectorNormType_t t, double *ret);



arStatus_t		AR_VectorToString(const arVector_t *vec, arString_t *str, size_t precision, const wchar_t *sp_str);


/*************************************************Vector transform**Matrix**********************************************************/

arStatus_t		AR_VecotrTransform_DCT2(arVector_t *vec);
arStatus_t		AR_VecotrInverseTransform_DCT2(arVector_t *vec);

/***************************************************************Matrix**********************************************************/

struct __arsenal_matrix_tag;
typedef struct __arsenal_matrix_tag		arMatrix_t;



arMatrix_t*		AR_CreateMatrix(size_t rows, size_t cols);
void			AR_DestroyMatrix(arMatrix_t *mat);

arMatrix_t*		AR_CopyNewMatrix(const arMatrix_t *mat);
arStatus_t		AR_CopyMatrix(arMatrix_t *dest, const arMatrix_t *src);

arStatus_t		AR_SetMatrixSize(arMatrix_t *mat, size_t rows, size_t cols);
arStatus_t		AR_SetMatrixData(arMatrix_t *mat, size_t row, size_t col, const double *data);

ar_int_t		AR_CompareMatrix(const arMatrix_t *l, const arMatrix_t *r, double epsilon);

size_t			AR_GetMatrixNumRows(const arMatrix_t *mat);
size_t			AR_GetMatrixNumColumns(const arMatrix_t *mat);

void			AR_SetMatrixValue(arMatrix_t *mat, size_t row, size_t col, double val);
double			AR_GetMatrixValue(const arMatrix_t *mat, size_t row, size_t col);


void			AR_SetMatrixRowByRawData(arMatrix_t *mat, size_t row,  const double *data);
void			AR_SetMatrixColumnByRawData(arMatrix_t *mat, size_t col, const double *data);


void			AR_SetMatrixRow(arMatrix_t *mat, size_t row,  const arVector_t *vec);
void			AR_SetMatrixColumn(arMatrix_t *mat, size_t col,  const arVector_t *vec);
arStatus_t		AR_GetMatrixRow(const arMatrix_t *mat, size_t row,  arVector_t *out);
arStatus_t		AR_GetMatrixColumn(const arMatrix_t *mat, size_t col,  arVector_t *out);

const double*	AR_GetMatrixRawData(const arMatrix_t *mat);


/****************************************生成标准矩阵类型******************************************/
void			AR_ZeroMatrix(arMatrix_t *mat);
void			AR_IdentityMatrix(arMatrix_t *mat);
arStatus_t		AR_DiagonalMatrix(arMatrix_t *mat, const arVector_t *vec);
void			AR_RandomMatrix(arMatrix_t *mat);
void			AR_NegateMatrix(arMatrix_t *mat);
void			AR_ClampMatrix(arMatrix_t *mat, double min_val, double max_val);
arStatus_t		AR_SwapMatrixRows(arMatrix_t *mat, size_t l, size_t r);
arStatus_t		AR_SwapMatrixColumns(arMatrix_t *mat, size_t l, size_t r);
void			AR_RemoveMatrixRow(arMatrix_t *mat, size_t r);
void			AR_RemoveMatrixColumn(arMatrix_t *mat, size_t c);
void			AR_ClearMatrixUpperTriangle(arMatrix_t *mat);
void			AR_ClearMatrixLowerTriangle(arMatrix_t *mat);
void			AR_SetMatrixByValue(arMatrix_t *mat, double val);

/****************************************判断矩阵类型******************************************/

arStatus_t		AR_IsSquareMatrix(const arMatrix_t *mat);


arStatus_t		AR_IsZeroMatrix(const arMatrix_t *mat, double epsilon);
arStatus_t		AR_IsIdentityMatrix(const arMatrix_t *mat, double epsilon);
arStatus_t		AR_IsDiagonalMatrix(const arMatrix_t *mat, double epsilon);
arStatus_t		AR_IsTriDiagonalMatrix(const arMatrix_t *mat, double epsilon);
arStatus_t		AR_IsSymmetricMatrix(const arMatrix_t *mat, double epsilon);
arStatus_t		AR_IsOrthogonalMatrix(const arMatrix_t *mat, double epsilon);
arStatus_t		AR_IsOrthonormalMatrix(const arMatrix_t *mat, double epsilon);

/*是否正定矩阵*/
arStatus_t		AR_IsPositiveDefiniteMatrix(const arMatrix_t *mat, double epsilon);
arStatus_t		AR_IsSymmetricPositiveDefinite(const arMatrix_t *mat, double epsilon);

/*是否半正定矩阵*/
arStatus_t		AR_IsPositiveSemiDefinite(const arMatrix_t *mat, double epsilon);
arStatus_t		AR_IsSymmetricPositiveSemiDefinite(const arMatrix_t *mat, double epsilon);


/****************************************基本矩阵计算******************************************/

arStatus_t		AR_MultiplyMatrixByScalar(const arMatrix_t *mat, double value, arMatrix_t *dest);
arStatus_t		AR_MultiplyMatrixByScalarSelf(arMatrix_t *mat, double value);

arStatus_t		AR_MultiplyMatrixByVector(const arMatrix_t *mat, const arVector_t *other, arVector_t *dest);			/*mat * other */
arStatus_t		AR_MultiplyTransposeMatrixByVector(const arMatrix_t *mat, const arVector_t *other, arVector_t *dest);	/*transpose(mat) * other */

arStatus_t		AR_MultiplyMatrixByMatrix(const arMatrix_t *mat, const arMatrix_t *other, arMatrix_t *dest);			/*mat * other */
arStatus_t		AR_MultiplyTransposeMatrixByMatrix(const arMatrix_t *mat, const arMatrix_t *other, arMatrix_t *dest);	/*transpose(mat) * other */
arStatus_t		AR_MultiplyMatrixByTransposeMatrix(const arMatrix_t *mat, const arMatrix_t *other, arMatrix_t *dest);	/*mat * transpose(other) */

arStatus_t		AR_MultiplyMatrixByMatrixSelf(arMatrix_t *mat, const arMatrix_t *other);								/*mat = mat * other */
arStatus_t		AR_MultiplyTransposeMatrixByMatrixSelf(arMatrix_t *mat, const arMatrix_t *other);						/*mat = transpose(mat) * other */
arStatus_t		AR_MultiplyMatrixByTransposeMatrixSelf(arMatrix_t *mat, const arMatrix_t *other);						/*mat = mat * transpose(other) */




arStatus_t		AR_AddMatrixByMatrix(const arMatrix_t *mat, const arMatrix_t *other, arMatrix_t *dest);
arStatus_t		AR_AddMatrixByMatrixSelf(arMatrix_t *mat, const arMatrix_t *other);

arStatus_t		AR_SubMatrixByMatrix(const arMatrix_t *mat, const arMatrix_t *other, arMatrix_t *dest);
arStatus_t		AR_SubMatrixByMatrixSelf(arMatrix_t *mat, const arMatrix_t *other);





arStatus_t		AR_CalcMatrixTrace(const arMatrix_t *mat, double *ret);
arStatus_t		AR_CalcMatrixDeterminant(const arMatrix_t *mat, double *ret);
arStatus_t		AR_CalcMatrixRank(const arMatrix_t *mat, size_t *ret);
arStatus_t		AR_TransposeMatrix(const arMatrix_t *mat, arMatrix_t *dest);
arStatus_t		AR_TransposeMatrixSelf(arMatrix_t *mat);


/*取逆矩阵*/
arStatus_t		AR_InverseMatrixSelf(arMatrix_t *mat);

arStatus_t		AR_InverseLowerTriangularMatrixSelf(arMatrix_t *mat);
arStatus_t		AR_InverseUpperTriangularMatrixSelf(arMatrix_t *mat);

arStatus_t		AR_InverseMatrixByGaussJordanSelf(arMatrix_t *mat);
arStatus_t		AR_InverseSolveMatrix(const arMatrix_t *mat, arVector_t *x, const arVector_t *b);

/*matlab rref(A)*/
void			AR_ReduceMatrixToEchelonFormSelf(arMatrix_t *mat, size_t *index);
arStatus_t		AR_ReduceMatrixToEchelonForm(const arMatrix_t *mat, size_t *index, arMatrix_t *rm);



/****************************************矩阵分解******************************************/

/*三对角矩阵*/
void			AR_TriDiagonalClearMatrixSelf(arMatrix_t *mat);
arStatus_t		AR_TriDiagonalSolveMatrix(const arMatrix_t *mat, arVector_t *x, const arVector_t *b);
arStatus_t		AR_TriDiagonalInverseMatrix(const arMatrix_t *mat, arMatrix_t *inv);


/*LU分解*/

arStatus_t		AR_LUFactorMatrixSelf(arMatrix_t *mat, size_t *index, double *det);
arStatus_t		AR_LUInverseMatrix(const arMatrix_t *mat, const size_t *index, arMatrix_t *inv);
arStatus_t		AR_LUSolveMatrix(const arMatrix_t *mat, const size_t *index, arVector_t *x,const arVector_t *b);
arStatus_t		AR_UnpackMatrixLUFactors(const arMatrix_t *mat, arMatrix_t *L, arMatrix_t *U);
arStatus_t		AR_MultiplyMatrixLUFactors(const arMatrix_t *mat, const size_t *index, arMatrix_t *original_mat);



/*
LDLT分解 A = L*D*L^t,矩阵为对称方阵
*/

arStatus_t		AR_LDLTFactorMatrixSelf(arMatrix_t *mat);
void			AR_LDLTSolveMatrix(const arMatrix_t *mat, arVector_t *x, const arVector_t *b);
arStatus_t		AR_LDLTInverseMatrix(const arMatrix_t *mat, arMatrix_t *inv);
arStatus_t		AR_UnpackMatrixLDLTFactors(const arMatrix_t *mat, arMatrix_t *L, arMatrix_t *D);
arStatus_t		AR_MultiplyMatrixLDLTFactors(const arMatrix_t *mat, arMatrix_t *original_mat);



/*Cholesky分解*/
arStatus_t		AR_CholeskyFactorMatrixSelf(arMatrix_t *mat);
arStatus_t		AR_CholeskySolveMatrix(const arMatrix_t *mat, arVector_t *x, const arVector_t *b);
arStatus_t		AR_CholeskyInverseMatrix(const arMatrix_t *mat, arMatrix_t *inv);
arStatus_t		AR_MultiplyMatrixCholeskyFactors(const arMatrix_t *mat, arMatrix_t *original_mat);


/*SVD分解*/
arStatus_t		AR_SVDFactorMatrixSelf(arMatrix_t *mat, arVector_t *w, arMatrix_t *v);
arStatus_t		AR_SVDSloveMatrix(const arMatrix_t *mat, const arVector_t *w, const arVector_t *v, arVector_t *x, const arVector_t *b);
arStatus_t		AR_SVDInverseMatrix(const arMatrix_t *mat, const arVector_t *w, const arVector_t *v, arMatrix_t *inv);
arStatus_t		AR_MultiplyMatrixSVDFactors(const arMatrix_t *mat, const arVector_t *w, const arVector_t *v, arMatrix_t *original_matrix);





/*************************************************************其他功能*************************************************************/
arStatus_t		AR_MatrixToString(const arMatrix_t *mat, arString_t *str, size_t precision, const wchar_t *sp_str, const wchar_t *row_sp);



/*************************************************************Transform***************************************************/

arStatus_t		AR_GenerateTransformMatrix_DCT2(arMatrix_t *mat, size_t n);

arStatus_t		AR_MatrixTransform_DCT2(arMatrix_t *mat, const arMatrix_t *dct_matrix);
arStatus_t		AR_MatrixInverseTransform_DCT2(arMatrix_t *mat, const arMatrix_t *dct_matrix);


AR_NAMESPACE_END




#endif
