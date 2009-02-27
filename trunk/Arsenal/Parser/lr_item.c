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
#include "lr.h"

int PSR_CompLRItem(const psrLRItem_t *left, const psrLRItem_t *right)
{
		size_t l_id,r_id;
		AR_ASSERT(left != NULL && right != NULL);
		
		l_id = left->rule_id;
		r_id = right->rule_id;

		if(l_id < r_id)
		{
				return -1;
		}else if(l_id == r_id)
		{
				if(left->delim < right->delim)
				{
						return -1;
				}else if(left->delim == right->delim)
				{
						return PSR_CompSymbList(left->symb, right->symb);/*只有在规范LR(1)或者LALR(1)时用到*/

				}else/*left->delim > right->delim*/
				{
						return 1;
				}
		}else/*l_id > r_id*/
		{
				return 1;
		}
}



bool_t			PSR_InsertLRItem_Unique(psrLRItem_t **dest, const psrLRItem_t *item)
{
		int cmp;
		psrLRItem_t *curr, *prev;
		AR_ASSERT(dest != NULL && item != NULL);

		

		curr = *dest; prev = NULL;

		while(curr != NULL)
		{
				cmp = PSR_CompLRItem(curr, item);
				if(cmp == 0)
				{
						return False;
				}else if(cmp == -1)
				{
						prev = curr;
						curr = curr->next;
				}else /*curr > item*/
				{
						break;
				}
		}

		if(prev == NULL)
		{
				AR_ASSERT(curr == *dest);
				*dest = PSR_CopyNewLRItem(item);
				(*dest)->next = curr;
		}else
		{
				prev->next = PSR_CopyNewLRItem(item);
				prev->next->next = curr;
		}
		return True;

}


psrLRItem_t*	PSR_CopyNewLRItem(const psrLRItem_t *sour)
{
		psrLRItem_t *res;
		res = AR_NEW0(psrLRItem_t);
		res->rule_id = sour->rule_id;
		res->delim = sour->delim;
		res->symb = PSR_CopyNewSymbList(sour->symb);
		return res;
}

psrLRItem_t*	PSR_CopyNewLRItemList(const psrLRItem_t *sour)
{
		psrLRItem_t *dest, **last;
		const psrLRItem_t *curr;
		dest = NULL; 
		last = &dest;
		curr = sour;
		
		while(curr != NULL)
		{
				*last = PSR_CopyNewLRItem(curr);
				last = &(*last)->next;
				curr = curr->next;
		}
		
		return dest;
}

psrLRItem_t*	PSR_DestroyLRItem(psrLRItem_t *item)
{
		psrLRItem_t *next;

		if(item == NULL)return NULL;

		next = item->next;
		PSR_DestroySymbList(item->symb);
		AR_DEL(item);
		return next;
		

}
void			PSR_DestroyLRItemList(psrLRItem_t *lst)
{
		while(lst != NULL)
		{
				lst = 	PSR_DestroyLRItem(lst);
		}


}


psrLRItem_t*	PSR_FindLRItem(psrLRItem_t *sour, const psrLRItem_t *key)
{
		psrLRItem_t *curr;

		curr = sour;
		
		while(curr != NULL)
		{
				if(PSR_CompLRItem(curr, key) == 0)return curr;
				curr = curr->next;
		}
		return NULL;
}



int				PSR_CompLRItemList(const psrLRItem_t *left, const psrLRItem_t *right)
{
		const psrLRItem_t *l, *r;

		l = left; r = right;
		
		while(l != NULL && r != NULL)
		{
				int cmp;
				cmp = PSR_CompLRItem(l,r);
				if(cmp == 0)
				{
						l = l->next;
						r = r->next;
				}else if(cmp == -1)
				{
						return -1;
				}else
				{
						return 1;
				}
		}

		if(l == NULL && r == NULL)
		{
				return 0;
		}else if(l == NULL)
		{
				return -1;
		}else
		{
				return 1;
		}
}

bool_t	 PSR_IsIncludesLRItemList(const psrLRItem_t *left, const psrLRItem_t *right)
{
		const psrLRItem_t *l, *r;
		l = left; r = right;
		
		while(l != NULL && r != NULL)
		{
				int cmp;
				
				cmp = PSR_CompLRItem(l, r);
				if(cmp == 0)
				{
						l = l->next;
						r = r->next;
				}else if(cmp == 1)
				{
						r = r->next;
				}else/*cmp == 0 curr_l < curr->r */
				{
						return False;
				}
		}

		return l == NULL ? True : False;
}