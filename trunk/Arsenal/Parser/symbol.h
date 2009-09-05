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


typedef struct __parser_symbol_tag
{
		psrSymbType_t	type;
		const wchar_t	*name;
		uint_t			hash_code;
		size_t			ref_count;
}psrSymb_t;


const psrSymb_t*		PSR_CreateSymb(const wchar_t *name, psrSymbType_t t);
const psrSymb_t*		PSR_CopyNewSymb(const psrSymb_t *sour);
void					PSR_DestroySymb(const psrSymb_t *symb);
int_t					PSR_CompSymb(const psrSymb_t *l, const psrSymb_t *r);


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
const psrSymb_t*	PSR_IndexOfSymbList(const psrSymbList_t *symb_lst, size_t idx);
int_t				PSR_FindFromSymbList(const psrSymbList_t *symb_lst, const psrSymb_t* symb);

int_t				PSR_BSearchFromSymbList(const psrSymbList_t *symb_lst, const psrSymb_t* symb);
void				PSR_SortSymbList(psrSymbList_t *symb_lst);


bool				PSR_InsertToSymbList_Unique(psrSymbList_t *symb_lst, const psrSymb_t *symb);


/********************************************************************************************************/
void			PSR_PrintSymbol(const psrSymb_t *symb,	arString_t *str);
void			PSR_PrintSymbolList(const psrSymbList_t *lst, arString_t *str);








AR_NAMESPACE_END


#endif

