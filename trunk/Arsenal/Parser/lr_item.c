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

#include "lr_item.h"



AR_NAMESPACE_BEGIN



static uint_t __hash_uint(uint_t hash, uint_t n)
{
		hash += n;
		hash += hash << 10;
		hash ^= hash >> 6;
		return hash;
}

void			PSR_InitLRItem(psrLRItem_t *item, const psrRule_t	*rule, size_t delim, const psrSymb_t *symb)
{
		AR_ASSERT(item != NULL);
		item->rule = rule;
		item->delim = delim;
		item->hash_code = 0;
		item->symb = NULL;
		item->lst = NULL;

		item->hash_code = __hash_uint(item->hash_code, (uint_t)item->rule);
		item->hash_code ^= __hash_uint(item->hash_code, item->delim);

		if(symb)
		{
				item->symb = PSR_CopyNewSymb(symb);
				
				item->hash_code ^= symb->hash_code;
		}
}

void			PSR_UnInitLRItem(psrLRItem_t *item)
{
		AR_ASSERT(item != NULL);
		if(item->symb)
		{
				PSR_DestroySymb(item->symb);
		}

		if(item->lst)
		{
				PSR_UnInitSymbList(item->lst);
				AR_DEL(item->lst);
		}
}

int_t			PSR_CompLRItem(const psrLRItem_t *l, const psrLRItem_t *r)
{
		uint_t cmp;
		AR_ASSERT(l != NULL && r != NULL);
		AR_ASSERT((l->symb == NULL && r->symb == NULL) || (l->symb != NULL && r->symb != NULL));

		cmp = AR_CMP(l->hash_code, r->hash_code);
		if(cmp != 0)return cmp;

		cmp = AR_CMP(l->rule, r->rule);
		if(cmp != 0)return cmp;
		
		cmp = AR_CMP(l->delim, r->delim);
		if(cmp != 0)return cmp;

		if(l->symb != NULL)
		{
				return PSR_CompSymb(l->symb, r->symb);
		}else
		{
				return 0;
		}
		
}

void			PSR_CopyLRItem(psrLRItem_t *dest, const psrLRItem_t *sour)
{
		PSR_UnInitLRItem(dest);
		PSR_InitLRItem(dest, sour->rule, sour->delim, sour->symb);
		
		if(sour->lst)
		{
				/*现在对LR_Item的应用决定了这里永远不会被执行到*/
				AR_ASSERT(false);
				dest->lst = AR_NEW0(psrSymbList_t);
				PSR_InitSymbList(dest->lst);
				PSR_CopySymbList(dest->lst, sour->lst);
		}
}

/*******************************************************************************************************************************/


static bool	__remove_lr_record(psrLRItemRec_t	**prec, const psrLRItem_t *item)
{
		int_t cmp;
		psrLRItemRec_t *curr, *prev;
		AR_ASSERT(prec != NULL && item != NULL);
		curr = *prec;
		prev = NULL;
		
		while(curr)
		{
				cmp = PSR_CompLRItem(&curr->item, item);
				
				if(cmp == -1)
				{
						prev = curr;
						curr = curr->next;
				}else if(cmp == 0)
				{
						if(prev == NULL)
						{
								*prec = curr->next;
								PSR_UnInitLRItem(&curr->item);
								AR_DEL(curr);
						}else
						{
								prev->next = curr->next;
								PSR_UnInitLRItem(&curr->item);
								AR_DEL(curr);
						}
						return true;
						
				}else
				{
						return false;
				}
		}
		
		return false;

}

static bool __insert_lr_record_unique(psrLRItemRec_t **prec, const psrLRItem_t *item)
{
		int_t cmp;
		psrLRItemRec_t *curr, *prev,*new_rec;
		AR_ASSERT(prec != NULL && item != NULL);
		curr = *prec;
		prev = NULL;
		
		while(curr)
		{
				cmp = PSR_CompLRItem(&curr->item, item);
				
				if(cmp == -1)
				{
						prev = curr;
						curr = curr->next;
				}else if(cmp == 0)
				{
						return false;
				}else
				{
						break;
				}
		}

		new_rec = AR_NEW0(psrLRItemRec_t);
		PSR_CopyLRItem(&new_rec->item, item);
		new_rec->next = NULL;
		
		if(prev == NULL)
		{
				AR_ASSERT(curr == *prec);
				new_rec->next = curr;
				*prec = new_rec;
		}else
		{
				new_rec->next = prev->next;
				prev->next = new_rec;
		}
		return true;
}

static void __destroy_lr_record_lst(psrLRItemRec_t *rec)
{
		psrLRItemRec_t *tmp;
		
		while(rec)
		{
				tmp = rec->next;
				PSR_UnInitLRItem(&rec->item);
				AR_DEL(rec);
				rec = tmp;
		}
}


psrLRItemRec_t* __copy_lr_record(const psrLRItemRec_t *sour)
{
		psrLRItemRec_t *res, **curr;
		res = NULL;
		curr = &res;

		while(sour)
		{
				*curr = AR_NEW0(psrLRItemRec_t);
				(*curr)->next = NULL;
				PSR_CopyLRItem(&(*curr)->item, &sour->item);
				curr = &((*curr)->next);
				sour = sour->next;
		}
		return res;
}


psrLRItemRec_t* __find_lr_record(const psrLRItemRec_t *lst, const psrLRItem_t *key)
{
		int_t cmp;
		AR_ASSERT(key != NULL);

		while(lst != NULL)
		{
				cmp = PSR_CompLRItem(&lst->item, key);

				if(cmp <  0)
				{
						lst = lst->next;
				}else if(cmp == 0)
				{
						return (psrLRItemRec_t*)lst;
				}else
				{
						return NULL;
				}
		}
		return NULL;
}

psrLRItem_t*	PSR_FindFromLRItemTbl(psrLRItemTbl_t *tbl, const psrLRItem_t *key)
{
		psrLRItemRec_t *lst;
		AR_ASSERT(tbl != NULL && key != NULL);
		if(tbl->item_count == 0)return NULL;
		
		lst = __find_lr_record(tbl->bucket[key->hash_code % LR_ITEM_BUCKET_SIZE], key);
		if(lst)
		{
				return &lst->item;
		}else
		{
				return NULL;
		}
}


bool	PSR_InsertToLRItemTbl(psrLRItemTbl_t *tbl, const psrLRItem_t *item)
{
		psrLRItemRec_t **prec;
		AR_ASSERT(tbl != NULL && item != NULL);
		prec = &tbl->bucket[item->hash_code % LR_ITEM_BUCKET_SIZE];
		if(__insert_lr_record_unique(prec, item))
		{
				tbl->item_count++;
				return true;
		}else
		{
				return false;
		}
}

bool PSR_RemoveFromLRItemTbl(psrLRItemTbl_t *tbl, const psrLRItem_t *item)
{
		psrLRItemRec_t **prec;
		AR_ASSERT(tbl != NULL && item != NULL);
		prec = &tbl->bucket[item->hash_code % LR_ITEM_BUCKET_SIZE];
		if(__remove_lr_record(prec, item))
		{
				tbl->item_count--;
				return true;
		}else
		{
				return false;
		}

}


bool	PSR_EqualLRItemTbl(const psrLRItemTbl_t *l, const psrLRItemTbl_t *r)
{
		size_t i;
		const psrLRItemRec_t *pl, *pr;
		AR_ASSERT(l != NULL && r != NULL);
		if(l == r)return 0;
		
		for(i = 0; i < LR_ITEM_BUCKET_SIZE; ++i)
		{
				pl = l->bucket[i];
				pr = r->bucket[i];
				for(; pl && pr && PSR_CompLRItem(&pl->item, &pr->item) == 0; pl = pl->next, pr = pr->next)
				{
				}

				if(pl || pr)return false;
		}
		return true;
}

void	PSR_InitLRItemTbl(psrLRItemTbl_t *tbl)
{
		AR_ASSERT(tbl != NULL);
		AR_memset(tbl, 0, sizeof(*tbl));
}


void	PSR_UnInitLRItemTbl(psrLRItemTbl_t *tbl)
{
		AR_ASSERT(tbl != NULL);
		PSR_ClearLRItemTbl(tbl);
}


void	PSR_ClearLRItemTbl(psrLRItemTbl_t *tbl)
{
		size_t i;
		AR_ASSERT(tbl != NULL);
		
		if(tbl->item_count == 0)return;

		for(i = 0; i < LR_ITEM_BUCKET_SIZE; ++i)
		{
				__destroy_lr_record_lst(tbl->bucket[i]);
				tbl->bucket[i] = NULL;
		}
		tbl->item_count = 0;
}


void	PSR_CopyLRItemTbl(psrLRItemTbl_t *dest, const psrLRItemTbl_t *sour)
{
		size_t i;
		
		for(i = 0; i < LR_ITEM_BUCKET_SIZE; ++i)
		{
				if(sour->bucket[i])
				{
						dest->bucket[i] = __copy_lr_record(sour->bucket[i]);
				}
		}
		dest->item_count = sour->item_count;
}

void	PSR_SwapLRItemTbl(psrLRItemTbl_t *l, psrLRItemTbl_t *r)
{
		psrLRItemRec_t *rec;
		size_t i;
		AR_ASSERT(l != NULL && r != NULL);
		
		for(i = 0; i < LR_ITEM_BUCKET_SIZE; ++i)
		{
				rec = l->bucket[i];
				l->bucket[i] = r->bucket[i];
				r->bucket[i] = rec;
		}

		i = l->item_count;
		l->item_count = r->item_count;
		r->item_count = i;
}


psrLRItemTblIter_t PSR_GetLRItemTblFirst(const psrLRItemTbl_t *tbl)
{
		size_t i;
		psrLRItemTblIter_t		iter = {0, NULL};

		for(i = 0; i < LR_ITEM_BUCKET_SIZE; ++i)
		{
				if(tbl->bucket[i] != NULL)
				{
						iter.bucket_idx = i;
						iter.curr = tbl->bucket[i];
						break;
				}
		}
		return iter;
}

bool			   PSR_GetLRItemTblNext(const psrLRItemTbl_t *tbl, psrLRItemTblIter_t *iter)
{
		AR_ASSERT(tbl != NULL && iter->bucket_idx < LR_ITEM_BUCKET_SIZE);
		
		if(iter->curr->next != NULL)
		{
				iter->curr = iter->curr->next;
				return true;
		}else
		{
				
				while(++iter->bucket_idx < LR_ITEM_BUCKET_SIZE)
				{
						iter->curr = tbl->bucket[iter->bucket_idx];
						if(iter->curr)
						{
								return true;
						}
				}
				iter->curr = NULL;
				return false;
		}
}



void			PSR_PrintLRItem(const psrLRItem_t *item, const psrGrammar_t *gmr, arString_t *str)
{
		const psrRule_t *rule;
		
		size_t i;
		AR_ASSERT(item != NULL && gmr != NULL && str != NULL);
		rule = item->rule;

		AR_AppendString(str, L"[ ");
		
		AR_AppendFormatString(str, L"<%ls> -> ", rule->head->name);
		
		i = 0;
		
		for(i = 0; i < item->delim; ++i)
		{
				const psrSymb_t	*curr;
				curr = PSR_IndexOfSymbList(&rule->body, i);
				PSR_PrintSymbol(curr, str);
		}
		
		AR_AppendString(str, L". ");

		for(i; i < rule->body.count; ++i)
		{
				const psrSymb_t	*curr;
				curr = PSR_IndexOfSymbList(&rule->body, i);
				PSR_PrintSymbol(curr, str);
		}

		if(item->symb)
		{
				AR_AppendString(str, L" : ");
				PSR_PrintSymbol(item->symb, str);
		}

		if(item->lst)
		{
				AR_AppendString(str, L" : ");
				PSR_PrintSymbolList(item->lst, str);
		}
		AR_AppendString(str, L" ]");
}



void PSR_PrintLRItemTable(const psrLRItemTbl_t *tbl, const psrGrammar_t *gmr, arString_t *str)
{
		psrLRItemTblIter_t iter;
		
		for(iter = PSR_GetLRItemTblFirst(tbl); iter.curr; PSR_GetLRItemTblNext(tbl, &iter))
		{
				PSR_PrintLRItem(&iter.curr->item, gmr, str);
				AR_AppendString(str, L"\r\n");
		}
}






AR_NAMESPACE_END

