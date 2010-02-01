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

#ifndef __RAY_SYMBOL_TABLE_H__
#define __RAY_SYMBOL_TABLE_H__

#include "ray.h"
#include "raytype.h"


AR_NAMESPACE_BEGIN


struct __ray_symbol_tag;
typedef struct __ray_symbol_tag	raySymbol_t;

struct __ray_symbol_block_tag;
typedef struct __ray_symbol_block_tag	raySymbBlock_t;

struct __ray_symbol_table_tag;
typedef struct __ray_symbol_table_tag	raySymbTable_t;


#define RAY_SYMBOL_BUCKET_SIZE			139

struct __ray_symbol_tag
{
		const wchar_t		*name;
		const rayType_t		*type;
};

struct __ray_symbol_block_tag
{
		raySymbol_t		*bucket[RAY_SYMBOL_BUCKET_SIZE];
		size_t			count;
		raySymbBlock_t	*up_level;
};


struct __ray_symbol_table_tag
{
		raySymbBlock_t	*global;
		raySymbBlock_t	*current;
};




AR_NAMESPACE_END



#endif

