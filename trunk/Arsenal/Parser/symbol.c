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

#include "parser_in.h"

#include "symbol.h"


AR_NAMESPACE_BEGIN



const psrSymb_t*		PSR_CreateSymb(const wchar_t *name, psrSymbType_t t, size_t prec, psrAssocType_t assoc, uint_t val)
{
		psrSymb_t		*res;
		res = AR_NEW0(psrSymb_t);
		res->name = PSR_AllocString(name);
		res->type = t;
		res->assoc = assoc;
		res->ref_count = 1;
		res->hash_code = AR_wcshash(res->name);
		res->val = val;
		res->prec = prec;
		return res;
}


const psrSymb_t*		PSR_CopyNewSymb(const psrSymb_t *sour)
{
		/*
		((psrSymb_t*)sour)->ref_count++;
		*/
		AR_ASSERT(sour != NULL && sour->ref_count > 0);

		

		for(;;)
		{
				volatile int_t tmp = sour->ref_count;
				AR_ASSERT(tmp >= 1);
				
				if(AR_CompExchange((uint_t*)&sour->ref_count, tmp+1, tmp) == (uint_t)tmp)
				{
						break;
				}
		}
		return sour;

}


void			PSR_DestroySymb(const psrSymb_t *symb)
{
		psrSymb_t *tmp;

		AR_ASSERT(symb != NULL && symb->ref_count > 0);
		tmp = (psrSymb_t*)symb;
		
		for(;;)
		{
				volatile int_t tmp = symb->ref_count;
				AR_ASSERT(tmp >= 1);
				
				if(AR_CompExchange((uint_t*)&symb->ref_count, tmp-1, tmp) == (uint_t)tmp)
				{
						if(tmp == 1)
						{
								AR_DEL((psrSymb_t*)symb);
						}
						break;
				}
		}
}





int_t				PSR_CompSymb(const psrSymb_t *l, const psrSymb_t *r)
{
		int_t cmp;
		AR_ASSERT(l != NULL && r != NULL);
		if(l == r)return 0;
		
		cmp = AR_CMP(l->type, r->type);
		if(cmp != 0)return cmp;
		cmp = AR_CMP(l->hash_code, r->hash_code);
		if(cmp != 0)return cmp;
		return AR_CMP(l->name, r->name);
}




/***********************************************************************************************************************************************/

/*
typedef struct __parser_symbol_list_tag
{
		const psrSymb_t	**lst;
		size_t			count;
		size_t			cap;	
}psrSymbList_t;
*/

void	PSR_InitSymbList(psrSymbList_t *symb_lst)
{
		AR_ASSERT(symb_lst != NULL);
		AR_memset(symb_lst, 0, sizeof(*symb_lst));
}

void	PSR_UnInitSymbList(psrSymbList_t *symb_lst)
{
		AR_ASSERT(symb_lst != NULL);
		AR_DEL((psrSymb_t**)symb_lst->lst);
		AR_memset(symb_lst, 0, sizeof(*symb_lst));
}

void	PSR_ClearSymbList(psrSymbList_t *symb_lst)
{
		AR_ASSERT(symb_lst != NULL);
		symb_lst->count = 0;
}


void				PSR_CopySymbList(psrSymbList_t *dest, const psrSymbList_t *sour)
{
		size_t i;
		AR_ASSERT(dest != NULL && sour != NULL);
		for(i = 0; i < sour->count; ++i)
		{
				PSR_InsertToSymbList(dest, sour->lst[i]);
		}
}


void	PSR_InsertToSymbList(psrSymbList_t *symb_lst, const psrSymb_t *symb)
{
		AR_ASSERT(symb_lst != NULL && symb != NULL);
		
		if(symb_lst->count == symb_lst->cap)
		{
				symb_lst->cap = (symb_lst->cap + 4)*2;
				symb_lst->lst = (const psrSymb_t**)AR_REALLOC(const psrSymb_t*, (psrSymb_t**)symb_lst->lst, symb_lst->cap);
		}
		symb_lst->lst[symb_lst->count++] = symb;
}


const psrSymb_t*	PSR_IndexOfSymbList(const psrSymbList_t *symb_lst, size_t idx)
{
		AR_ASSERT(symb_lst != NULL);
		
		if(idx >= symb_lst->count)
		{
				return NULL;
		}else
		{
				return symb_lst->lst[idx];
		}
}

int_t				PSR_FindFromSymbList(const psrSymbList_t *symb_lst, const psrSymb_t* symb)
{
		int_t i;

		for(i = 0; i < (int_t)symb_lst->count; ++i)
		{
				if(PSR_CompSymb(symb_lst->lst[i], symb) == 0)return i;
		}
		return -1;
}



int_t				PSR_BSearchFromSymbList(const psrSymbList_t *symb_lst, const psrSymb_t* symb)
{
		int_t l,r,m,cmp;
		AR_ASSERT(symb_lst != NULL && symb != NULL);


		l = 0; r = symb_lst->count - 1;
		while( l <= r )
		{
				m = (l + r)/2;
				
				cmp = PSR_CompSymb(symb_lst->lst[m], symb);

				switch(cmp)
				{
				case -1:
						l = m + 1;
						break;
				case 0:
						return m;
				default:/*cmp == 1*/
						r = m - 1;
				}
		}
		return -1;
}


static int __comp_symb(const void *left, const void *right)
{
		const psrSymb_t *l,*r;
		l = *(const psrSymb_t**)left;
		r = *(const psrSymb_t**)right;

		AR_ASSERT(l != NULL && r != NULL);
		return (int)PSR_CompSymb(l,r);
}

#if(0)
void			PSR_SortSymbList(psrSymbList_t *symb_lst)
{
		int_t i,j;
		const psrSymb_t *tmp;

		AR_ASSERT(symb_lst != NULL);
		
		for(i = 1; i < (int_t)symb_lst->count; ++i)
		{
				for(j = i; j > 0; --j)
				{
						if(PSR_CompSymb(symb_lst->lst[j], symb_lst->lst[j-1]) < 0)
						{
								
								tmp = symb_lst->lst[j];
								symb_lst->lst[j] = symb_lst->lst[j-1];
								symb_lst->lst[j-1] = tmp;
						}
				}
		}
}
#endif

void			PSR_SortSymbList(psrSymbList_t *symb_lst)
{
		AR_ASSERT(symb_lst != NULL);
		
		if(symb_lst->count > 0)
		{
				AR_qsort((void*)symb_lst->lst, symb_lst->count, sizeof(psrSymb_t*), __comp_symb);
		}
}

/*********************************************************************************************************/

/*
typedef struct __parser_symbol_record_tag
{
		const psrSymb_t							*symb;
		struct __parser_symbol_record_tag		*next;
}psrSymbRec_t;

typedef struct __parser_symbol_table_tag
{
		psrSymbRec_t	**lst;
		size_t			count;
}psrSymbTbl_t;
*/

void					PSR_InitSymbTbl(psrSymbTbl_t *tbl, size_t count)
{
		AR_ASSERT(tbl != NULL);
		if(count < 10)count = 10;
		AR_memset(tbl, 0, sizeof(*tbl));

		tbl->bucket_size = count;
		tbl->lst = AR_NEWARR0(psrSymbRec_t*, count);
}

void					PSR_UnInitSymbTbl(psrSymbTbl_t *tbl)
{
		
		PSR_ClearSymbTbl(tbl);
		AR_memset(tbl, 0, sizeof(*tbl));
}


void					PSR_ClearSymbTbl(psrSymbTbl_t	*tbl)
{
		psrSymbRec_t	*curr, *tmp;
		size_t i;
		AR_ASSERT(tbl != NULL && tbl->lst != NULL);
		
		if(tbl->item_count == 0)return;

		for(i = 0; i < tbl->bucket_size; ++i)
		{
				curr = tbl->lst[i];
				
				while(curr)
				{
						tmp = curr->next;
						PSR_DestroySymb(curr->symb);
						AR_DEL(curr);
						curr = tmp;
				}

				tbl->lst[i] = NULL;
		}

		tbl->item_count = 0;

}

inline size_t	__get_bucket_idx(size_t count, uint_t hash)
{
		return hash % count;
}


const psrSymb_t*		PSR_FindFromSymbTbl(const psrSymbTbl_t *tbl, const psrSymb_t *symb)
{
		psrSymbRec_t *curr;
		AR_ASSERT(tbl != NULL && symb != NULL);
		
		curr = tbl->lst[__get_bucket_idx(tbl->bucket_size, symb->hash_code)];
		while(curr)
		{
				if(PSR_CompSymb(curr->symb, symb) == 0)return curr->symb;
				curr = curr->next;
		}
		return NULL;
}


void	PSR_SymbTblToSymbList(const psrSymbTbl_t *tbl,  psrSymbList_t *lst)
{
		size_t i;
		psrSymbRec_t	*curr;
		AR_ASSERT(tbl != NULL && lst != NULL);
		
		PSR_ClearSymbList(lst);
		
		for(i = 0; i < tbl->bucket_size; ++i)
		{
				curr = tbl->lst[i];
				while(curr)
				{
						PSR_InsertToSymbList(lst, curr->symb);
						curr = curr->next;
				}
		}
}

void					PSR_CopySymbTbl(psrSymbTbl_t *dest, const psrSymbTbl_t *sour)
{
		psrSymbTblIter_t iter;
		AR_ASSERT(dest != NULL && sour != NULL);

		for(iter = PSR_FirstFromSymbTbl(sour); iter.curr; PSR_NextFromSymbTbl(sour, &iter))
		{
				PSR_InsertToSymbTbl(dest, iter.curr->symb);
		}
}



static bool	__insert_no_resize(psrSymbTbl_t *tbl, const psrSymb_t *symb, size_t idx)
{
		psrSymbRec_t *curr;
		AR_ASSERT(tbl != NULL && symb != NULL && idx < tbl->bucket_size);
		curr = tbl->lst[idx];
		if(curr == NULL)
		{
				curr = AR_NEW(psrSymbRec_t);
				curr->symb = PSR_CopyNewSymb(symb);
				curr->next = NULL;
				tbl->lst[idx] = curr;
				tbl->item_count++;
				return true;
		}else
		{
				while(curr)
				{
						if(PSR_CompSymb(curr->symb, symb) == 0)return false;
						curr = curr->next;
				}

				curr = AR_NEW(psrSymbRec_t);
				curr->symb = PSR_CopyNewSymb(symb);
				curr->next = tbl->lst[idx];
				tbl->lst[idx] = curr;
				tbl->item_count++;
				return true;
		}
}




#define __n_primers 32

static const size_t  __prime_list[__n_primers] =
{
  5,		  9,			17,			 34,
  53,         97,           193,         389,       769,
  1543,       3079,         6151,        12289,     24593,
  49157,      98317,        196613,      393241,    786433,
  1572869,    3145739,      6291469,     12582917,  25165843,
  50331653,   100663319,    201326611,   402653189, 805306457, 
  1610612741, 3221225473ul, 4294967291ul
};

inline size_t __next_prime(size_t curr)
{
		int l = 0, r = __n_primers - 1;
		const size_t *res = NULL;

		while(l <= r)
		{
				int m = (l + r)/2;
				if(__prime_list[m] <= curr)
				{
						l = m + 1;
				}else
				{
						res = __prime_list + (size_t)m;
						r = m - 1;
				}
		}

		return res != NULL ? *res : 4294967291ul;
}


static void __inc_bucket(psrSymbTbl_t *tbl, size_t inc_count)
{
		size_t new_size, i;
		psrSymbRec_t	**new_lst;
		AR_ASSERT(tbl != NULL);
		if(inc_count == 0)return;

		new_size = __next_prime(tbl->item_count + 1);
		AR_ASSERT(new_size >= tbl->bucket_size);

		new_lst = AR_NEWARR0(psrSymbRec_t*, new_size);
		
		for(i = 0; i < tbl->bucket_size; ++i)
		{
				psrSymbRec_t	*curr, *tmp;
				curr = tbl->lst[i];
				
				while(curr)
				{
						size_t new_idx;
						
						new_idx  = __get_bucket_idx(new_size, curr->symb->hash_code);
						tmp = curr->next;
						curr->next = new_lst[new_idx];
						new_lst[new_idx] = curr;
						curr = tmp;
				}
		}

		AR_DEL(tbl->lst);
		tbl->lst = new_lst;
		tbl->bucket_size = new_size;
		
}

bool					PSR_InsertToSymbTbl(psrSymbTbl_t *tbl, const psrSymb_t *symb)
{
		if(tbl->bucket_size <= tbl->item_count)
		{
				__inc_bucket(tbl, 1);
		}
		return __insert_no_resize(tbl, symb, __get_bucket_idx(tbl->bucket_size, symb->hash_code));
}



psrSymbTblIter_t		PSR_FirstFromSymbTbl(const psrSymbTbl_t *tbl)
{
		size_t i;
		psrSymbTblIter_t		iter = {0, NULL};
		
		for(i = 0; i < tbl->bucket_size; ++i)
		{
				if(tbl->lst[i] != NULL)
				{
						iter.bucket = i;
						iter.curr = tbl->lst[i];
						return iter;
				}
		}
		return iter;
}


bool					PSR_NextFromSymbTbl(const psrSymbTbl_t *tbl, psrSymbTblIter_t *iter)
{
		size_t i;
		AR_ASSERT(tbl != NULL && iter != NULL && iter->bucket < tbl->bucket_size);
		
		if(iter->curr == NULL)return false;
		
		if(iter->curr != NULL)
		{
				iter->curr = iter->curr->next;
		}

		if(iter->curr != NULL)return true;

		for(i = iter->bucket + 1; i < tbl->bucket_size; ++i)
		{
				if(tbl->lst[i] != NULL)
				{
						iter->bucket = i;
						iter->curr = tbl->lst[i];
						break;
				}
		}

		return iter->curr != NULL;
}




/*************************************************************************************************/

void			PSR_PrintSymbol(const psrSymb_t *symb, bool detail, arString_t *str)
{
		if(symb->type == PSR_TERM)
		{
				AR_AppendFormatString(str, L"%s ", symb->name);
				
		}else
		{
				AR_AppendFormatString(str, L"<%s> ", symb->name);
		}

		if(detail)
		{
				if(symb->type == PSR_NONTERM)
				{
						AR_AppendFormatString(str, L" : prec %d : assoc %d", symb->prec, symb->assoc);
				}else
				{
						AR_AppendFormatString(str, L" : prec %d : value %d : assoc %d", symb->prec, symb->val, symb->assoc);
				}
		}
}


void			PSR_PrintSymbolList(const psrSymbList_t *lst, bool detail, arString_t *str)
{
		size_t i;

		for(i = 0; i < lst->count; ++i)
		{
				PSR_PrintSymbol(lst->lst[i], detail,str);
				if(detail)
				{
						AR_AppendFormatString(str, L"\r\n");
				}else
				{
						AR_AppendFormatString(str, L" ");
				}
		}
}

void			PSR_PrintSymbolTable(const psrSymbTbl_t *tbl, bool detail, arString_t *str)
{
		psrSymbList_t lst;

		PSR_InitSymbList(&lst);
		PSR_SymbTblToSymbList(tbl, &lst);
		PSR_PrintSymbolList(&lst, detail, str);
		PSR_UnInitSymbList(&lst);
}




AR_NAMESPACE_END
