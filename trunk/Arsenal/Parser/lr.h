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

#include "parser_in.h"





typedef struct __lr_goto_map_tag
{
		psrSymb_t		*symb;	/*例如:symbol[0] == id;则意味着在下面的slrItemsCollection_t中和itemset[x]对应的状态有id输入时将转换到state[0]对应的状态*/
		size_t			*state;
		size_t			count;
		size_t			cap;
}lrGotoMap_t;


AR_INLINE void LR_InitGotoMap(lrGotoMap_t *map)
{
		AR_MSET0(map, sizeof(*map));

}

AR_INLINE void LR_UnInitGotoMap(lrGotoMap_t *map)
{
		AR_DEL(map->symb);
		AR_DEL(map->state);
		AR_MSET0(map, sizeof(*map));
}

AR_INLINE int	 LR_FindStateFromGotoMap(const lrGotoMap_t *map, const psrSymb_t *symbol)
{
		size_t i;
		for(i = 0; i < map->count; ++i)
		{
				if(PSR_CompSymb(symbol, &map->symb[i]) == 0)
				{
						return (int)map->state[i];
				}
		}
		return -1;
}


AR_INLINE void LR_InsertToGotoMap(lrGotoMap_t *map, const psrSymb_t *symbol, size_t state)
{
		int stat;

		stat = LR_FindStateFromGotoMap(map, symbol);
		//AR_ASSERT(stat == -1 || (size_t)stat == state);

		if(stat != -1)return;
		
		if(map->count == map->cap)
		{
				map->cap = (map->cap + 1)*2;
				map->symb = AR_REALLOC(psrSymb_t, map->symb, map->cap);
				map->state = AR_REALLOC(size_t, map->state, map->cap);
		}
		
		map->symb[map->count] = *symbol;
		map->state[map->count] = state;
		map->count++;
}




typedef struct __lr_itemset_collection_tag
{
		psrLRItem_t		**items;
		lrGotoMap_t		*goto_map;
		
		size_t			count;
		size_t			cap;
}lrItemsCollection_t;


AR_INLINE void LR_DestroyItemsCollection(lrItemsCollection_t *clt)
{
		size_t i;
		for(i = 0; i < clt->count; ++i)
		{
				PSR_DestroyLRItemList(clt->items[i]);
				LR_UnInitGotoMap(&clt->goto_map[i]);
		}
		AR_DEL(clt->items);
		AR_DEL(clt->goto_map);
		AR_DEL(clt);
}


AR_INLINE int LR_FindItemsFromCollection(const lrItemsCollection_t *clt, const psrLRItem_t *itemset)
{
		size_t i;
		for(i = 0; i < clt->count; ++i)
		{
				//if(PSR_IsIncludesLRItemList(itemset, clt->items[i]))
				if(PSR_CompLRItemList(itemset, clt->items[i]) == 0)
				{
						return (int)i;
				}
		}
		return -1;
}


AR_INLINE bool_t LR_InsertToItemsCollection_Unique(lrItemsCollection_t *clt, const psrLRItem_t *itemset)
{
		AR_ASSERT(clt != NULL && itemset != NULL);
		
		if(LR_FindItemsFromCollection(clt, itemset) != -1)return False;
		
		if(clt->count == clt->cap)
		{
				clt->cap = (clt->cap + 1)*2;
				clt->items = AR_REALLOC(psrLRItem_t*, clt->items, clt->cap);
				clt->goto_map = AR_REALLOC(lrGotoMap_t, clt->goto_map, clt->cap);
		}

		clt->items[clt->count] = NULL;
		LR_InitGotoMap(&clt->goto_map[clt->count]);
		clt->items[clt->count] = PSR_CopyNewLRItemList(itemset);
		clt->count++;
		return True;
}

AR_INLINE void	LR_ClearNonKernelItems(lrItemsCollection_t *clt)
{
		size_t i;
		for(i = 0; i < clt->count; ++i)
		{
				psrLRItem_t *curr;
				psrLRItem_t *items = clt->items[i];
				clt->items[i] = NULL;
				for(curr = items; curr != NULL; curr = curr->next)
				{
						if(curr->rule_id == 0 || curr->delim > 0)
						{
								PSR_InsertLRItem_Unique(&(clt->items[i]), curr);
						}
				}

				PSR_DestroyLRItemList(items);
		}
		
}



lrItemsCollection_t* LR0_CreateItemsCollection(const psrGrammar_t *grammar);

lrItemsCollection_t* LR1_CreateItemsCollection(const psrGrammar_t *grammar);

lrItemsCollection_t* LALR_CreateItemsCollection(const psrGrammar_t *grammar);


















#endif

