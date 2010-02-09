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

#ifndef __RAY_TYPE_H__
#define __RAY_TYPE_H__

#include "ray.h"

AR_NAMESPACE_BEGIN


/*type_specifier	:	void */
/*type_specifier	:	byte */
/*type_specifier	:	char */
/*type_specifier	:	short */
/*type_specifier	:	int */
/*type_specifier	:	long */
/*type_specifier	:	float */
/*type_specifier	:	double */
/*type_specifier	:	signed */
/*type_specifier	:	unsigned */
/*type_specifier	:	struct_specifier */
/*type_specifier	:	TYPE_ID */
typedef enum
{
		RAY_VOID,
		RAY_UINT_8,
		RAY_INT_8,
		RAY_UINT_16,
		RAY_INT_16,
		RAY_UINT_32,
		RAY_INT_32,
		RAY_UINT_64,
		RAY_INT_64,
		RAY_FP_32,
		RAY_FP_64,
		RAY_BYTE	=	RAY_UINT_8,
		RAY_CHAR	=	RAY_UINT_16
}rayBasicType_t;


typedef enum
{
		RAY_BAISC_T,
		RAY_POINTER_T,
		RAY_ARRAY_T,
		RAY_FUNCTION_T,
		RAY_STRUCT_T
}rayCompoundType_t;


struct __ray_type_tag;

typedef struct __ray_type_tag	rayType_t;



AR_NAMESPACE_END



#endif