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

#ifndef __PARSER_LR_H__
#define __PARSER_LR_H__

#include "grammar.h"

AR_NAMESPACE_BEGIN


typedef struct __parser_lr_item_tag psrLRItem_t;



typedef struct __parser_lr_item_tag
{
		size_t					rule_id;
		size_t					delim;
		const psrSymb_t			*symb;/*只有在LR1，LALR时会用到*/
		psrSymbTbl_t			*tbl;/*只有在LALR时会用到*/
		uint_t					hash_code;
}psrLRItem_t;


void			PSR_InitLRItem(psrLRItem_t *item, size_t rule_id, size_t delim, const psrSymb_t *symb);
void			PSR_UnInitLRItem(psrLRItem_t *item);
void			PSR_CopyLRItem(psrLRItem_t *dest, const psrLRItem_t *sour);

int_t			PSR_CompLRItem(const psrLRItem_t *l, const psrLRItem_t *r);



/**************************************************************************************************/

typedef struct __lr_item_rec_tag
{
		psrLRItem_t					item;
		struct __lr_item_rec_tag	*next;
}psrLRItemRec_t;

/*
		这里必须是一个固定的桶数量，因为需要做项集之间的比对，所以Insert在产生冲突时是按PSR_CompLRItem
来排序插入的，必须确定hash_code与桶号的对应关系

*/
#define LR_ITEM_BUCKET_SIZE		1024

typedef struct __lr_item_hash_table_tag
{
		psrLRItemRec_t	*bucket[LR_ITEM_BUCKET_SIZE];
		size_t			item_count;
}psrLRItemTbl_t;

void	PSR_InitLRItemTbl(psrLRItemTbl_t *tbl);
void	PSR_UnInitLRItemTbl(psrLRItemTbl_t *tbl);
void	PSR_ClearLRItemTbl(psrLRItemTbl_t *tbl);

void	PSR_CopyLRItemTbl(psrLRItemTbl_t *dest, const psrLRItemTbl_t *sour);
void	PSR_SwapLRItemTbl(psrLRItemTbl_t *l, psrLRItemTbl_t *r);


bool	PSR_EqualLRItemTbl(const psrLRItemTbl_t *l, const psrLRItemTbl_t *r);

bool	PSR_InsertToLRItemTbl(psrLRItemTbl_t *tbl, const psrLRItem_t *item);
bool	PSR_RemoveFromLRItemTbl(psrLRItemTbl_t *tbl, const psrLRItem_t *item);

psrLRItem_t*	PSR_FindFromLRItemTbl(psrLRItemTbl_t *tbl, const psrLRItem_t *key);

typedef struct __lr_item_tbl_iterator_tag
{
		size_t					bucket_idx;
		const psrLRItemRec_t	*curr;
}psrLRItemTblIter_t;

psrLRItemTblIter_t PSR_GetLRItemTblFirst(const psrLRItemTbl_t *tbl);
bool			   PSR_GetLRItemTblNext(const psrLRItemTbl_t *tbl, psrLRItemTblIter_t *iter);



void PSR_PrintLRItem(const psrLRItem_t *item, const psrGrammar_t *gmr, arString_t *str);
void PSR_PrintLRItemTable(const psrLRItemTbl_t *tbl, const psrGrammar_t *gmr, arString_t *str);

AR_NAMESPACE_END


#endif

