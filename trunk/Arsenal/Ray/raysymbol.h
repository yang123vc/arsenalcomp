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



struct __ray_attribute_tag;
typedef struct __ray_attribute_tag		rayAttr_t;

struct __ray_symbol_tag;
typedef struct __ray_symbol_tag	raySymbol_t;

struct __ray_symbol_block_tag;
typedef struct __ray_symbol_block_tag	raySymbBlock_t;

struct __ray_symbol_table_tag;
typedef struct __ray_symbol_table_tag	raySymbTable_t;


typedef struct __ray_coordinate_tag
{
		size_t	line;
		size_t	col;
}rayCoord_t;

struct __ray_symbol_tag
{
		const wchar_t		*name;
		rayCoord_t			coord;

		rayAttr_t			*attr;
		raySymbol_t			*next;
};




#define RAY_SYMBOL_BUCKET_SIZE			139

struct __ray_symbol_block_tag
{
		raySymbol_t		*bucket[RAY_SYMBOL_BUCKET_SIZE];
		size_t			count;
		raySymbBlock_t	*up;
		
/*内部使用*/
		raySymbBlock_t	*next;
};

struct __ray_symbol_table_tag
{
		raySymbBlock_t	*global;
		raySymbBlock_t	*current;
};

void			RAY_InitSymbTable(raySymbTable_t *tbl);
void			RAY_UnInitSymbTable(raySymbTable_t *tbl);

void			RAY_EnterBlock(raySymbTable_t *tbl);
void			RAY_LeaveBlock(raySymbTable_t *tbl);
raySymbBlock_t*	RAY_CurrentBlock(raySymbTable_t *tbl);


bool_t			RAY_InsertName(raySymbBlock_t *block,	const wchar_t *name, rayCoord_t coord, const rayAttr_t *attr);
bool_t			RAY_RemoveName(raySymbBlock_t *block,	const wchar_t *name);
raySymbol_t*	RAY_GetSymbol(raySymbBlock_t *block,	const wchar_t *name);






AR_NAMESPACE_END



#endif

