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



/*

type_specifier		:	"void"
					|	"byte"
					|	"char"
					|	"short"
					|	"int"
					|	"long"
					|	"float"
					|	"double"
					|	"signed"
					|	"unsigned"
					|	"signed"		"short"
					|	"unsigned"		"short"
					|	"signed"		"int"
					|	"unsigned"		"int"
					|	"signed"		"long"
					|	"unsigned"		"long"
					|	struct_specifier
					|	TYPE_ID
					;
*/



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
		RAY_BAISC_OP,
		RAY_POINTER_OP,
		RAY_ARRAY_OP,
		RAY_FUNCTION_OP,
		RAY_STRUCT_OP,
		RAY_QUAL_OP
}rayTypeOper_t;


typedef enum
{
		RAY_CONST_QUALIFIER
}rayTypeQualifier_t;



struct __ray_type_tag;
typedef struct __ray_type_tag	rayType_t;




typedef struct __ray_basic_tag
{
		rayBasicType_t	basic;
}rayBasic_t;

typedef struct __ray_array_tag
{
		size_t			count;
		rayType_t		*type;
}rayArray_t;



typedef struct __ray_function_tag
{
		rayType_t		*ret;
		rayType_t		**params;
		wchar_t			**param_names;
		size_t			count;
}rayFunc_t;


typedef struct __ray_pointer_tag
{
		rayType_t		*type;
}rayPointer_t;


typedef struct __ray_qual_tag
{
		rayTypeQualifier_t		qual;
		rayType_t				*type;
}rayQual_t;


typedef struct __ray_struct_tag
{
		struct __ray_symbol_block_tag	*fields;
}rayStruct_t;



struct __ray_type_tag
{
		size_t			refcnt;

		rayTypeOper_t	kind;
		size_t			align;
		size_t			size;
		

		union{
				rayBasic_t		basic;
				rayArray_t		arr;
				rayStruct_t		record;
				rayFunc_t		func;
				rayPointer_t	pointer;
				rayQual_t		qual;
		};


};




AR_NAMESPACE_END



#endif