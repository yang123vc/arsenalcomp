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
#ifndef __PARSER_SYMBOL_H__
#define __PARSER_SYMBOL_H__

#include "parser.h"

AR_NAMESPACE_BEGIN


typedef enum
{
		PSR_TERM,
		PSR_NONTERM
}psrSymbType_t;

typedef struct __parser_symbol_tag psrSymb_t;


struct __parser_symbol_tag
{
		psrSymbType_t	type;
		const wchar_t	*name;
		ar_uint_t			hash_code;
		size_t			ref_count;
};


const psrSymb_t*		PSR_CreateSymb(const wchar_t *name, psrSymbType_t t);
const psrSymb_t*		PSR_CopyNewSymb(const psrSymb_t *sour);
void					PSR_DestroySymb(const psrSymb_t *symb);
ar_int_t					PSR_CompSymb(const psrSymb_t *l, const psrSymb_t *r);


/***************************************symbol_list***************************************************/

typedef struct __parser_symbol_list_tag
{
		const psrSymb_t **lst;
		size_t			count;
		size_t			cap;	
}psrSymbList_t;

void				PSR_InitSymbList(psrSymbList_t *symb_lst);
void				PSR_UnInitSymbList(psrSymbList_t *symb_lst);
void				PSR_ClearSymbList(psrSymbList_t *symb_lst);

void				PSR_CopySymbList(psrSymbList_t *dest, const psrSymbList_t *sour);

void				PSR_InsertToSymbList(psrSymbList_t *symb_lst, const psrSymb_t *symb);

/*
#define			PSR_InsertToSymbList(_l, _b)											\
		do{																				\
				if((_l)->count == (_l)->cap)											\
				{																		\
						(_l)->cap = ((_l)->cap + 4)*2;								\
						(_l)->lst = AR_REALLOC(const psrSymb_t*, (psrSymb_t**)(_l)->lst, (_l)->cap);		\
				}																						\
				(_l)->lst[(_l)->count++] = (_b);														\
		}while(0)
*/

const psrSymb_t*	PSR_IndexOfSymbList(const psrSymbList_t *symb_lst, size_t idx);
ar_int_t				PSR_FindFromSymbList(const psrSymbList_t *symb_lst, const psrSymb_t* symb);

ar_bool_t				PSR_RemoveFromSymbListByIndex(psrSymbList_t *symb_lst, size_t index);

ar_int_t				PSR_BSearchFromSymbList(const psrSymbList_t *symb_lst, const psrSymb_t* symb);
void				PSR_SortSymbList(psrSymbList_t *symb_lst);



ar_bool_t				PSR_InsertToSymbList_Unique(psrSymbList_t *symb_lst, const psrSymb_t *symb);



/********************************************************************************************************/
void			PSR_PrintSymbol(const psrSymb_t *symb,	arString_t *str);
void			PSR_PrintSymbolList(const psrSymbList_t *lst, arString_t *str);



/**************************************Symbol Map**********************************************************/



typedef struct __parser_symbmap_record_tag
{
		const psrSymb_t			*key;
		psrSymbList_t			lst;
		ar_bool_t					can_empty;
		struct __parser_symbmap_record_tag		*next;
}psrMapRec_t;


#define MAP_BUCKET_SIZE (153)

typedef struct __parser_symbmap_tag
{
		psrMapRec_t		*bucket[MAP_BUCKET_SIZE];
		size_t			item_count;
}psrSymbMap_t;


void					PSR_InitSymbMap(psrSymbMap_t *map);
void					PSR_UnInitSymbMap(psrSymbMap_t *map);

ar_bool_t					PSR_InsertToSymbMap(psrSymbMap_t *map, const psrSymb_t *key, const psrSymb_t *val);
ar_bool_t					PSR_SetSymbEpsilon(psrSymbMap_t *map, const psrSymb_t *key, ar_bool_t is_epsilon);

psrMapRec_t*			PSR_GetSymbolFromSymbMap(const psrSymbMap_t *map, const psrSymb_t *key);

void					PSR_PrintSymbolMap(const psrSymbMap_t *map, arString_t *str);




AR_NAMESPACE_END


#endif

