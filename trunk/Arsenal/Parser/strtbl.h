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
#ifndef __PSR_STRTBL_H__
#define __PSR_STRTBL_H__

#include "parser.h"




AR_NAMESPACE_BEGIN

typedef struct __psr_str_tbl
{
		wchar_t					*str;
		size_t					len;
		struct __psr_str_tbl	*next;
}psrString_t;



typedef struct __string_table_
{
		psrString_t		**bucket;
		size_t			count;
}psrStringTable_t;


#define		MIN_BUCKET_SIZE		1024


psrStringTable_t*		PSR_CreateStrTable(size_t count);
void					PSR_DestroyStrTable(psrStringTable_t* tbl);


const wchar_t*			PSR_GetString(psrStringTable_t *tbl, const wchar_t *str);
const wchar_t*			PSR_GetStringN(psrStringTable_t *tbl, const wchar_t *str, size_t n);

const wchar_t*			PSR_GetStringUInt(psrStringTable_t *tbl, uint64_t num, size_t radix);
const wchar_t*			PSR_GetStringInt(psrStringTable_t *tbl, int64_t num, size_t radix);



AR_NAMESPACE_END





#endif

