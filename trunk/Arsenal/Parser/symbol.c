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



const psrSymb_t*		PSR_CreateSymb(const wchar_t *name, psrSymbType_t t)
{
		psrSymb_t		*res;
		res = AR_NEW0(psrSymb_t);
		res->name = PSR_AllocString(name);
		res->type = t;
		res->hash_code = AR_wcshash(res->name);
		res->ref_count = 1;
		return res;

}




#if defined(AR_DEBUG)
static void __trace_symb_refcount(const psrSymb_t *symb)
{
		if(symb == PSR_StartSymb)//PSR_CompSymb(PSR_EOISymb, symb) == 0)
		{

				int x;

				x = 0;
				++x;
				
		}
}

#else

#define __trace_symb_refcount(_ptr) do{}while(0)

#endif



const psrSymb_t*		PSR_CopyNewSymb(const psrSymb_t *sour)
{
		AR_ASSERT(sour != NULL && sour->ref_count > 0);

		__trace_symb_refcount(sour);
		/*
		for(;;)
		{
				volatile int_t tmp = sour->ref_count;
				AR_ASSERT(tmp >= 1);
				
				if(AR_CompExchange((uint_t*)&sour->ref_count, tmp+1, tmp) == (uint_t)tmp)
				{
						break;
				}
		}
		*/
		AR_AtomicInc((volatile int_t*)&sour->ref_count);
		return sour;

}

void			PSR_DestroySymb(const psrSymb_t *symb)
{
		
		AR_ASSERT(symb != NULL && symb->ref_count > 0);
		
		__trace_symb_refcount(symb);
		/*
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
		*/

		if(AR_AtomicDec((volatile int_t*)&symb->ref_count) == 0)
		{
				AR_DEL((psrSymb_t*)symb);
		}

}



/*
const psrSymb_t*		PSR_CopyNewSymb(const psrSymb_t *sour)
{
		AR_ASSERT(sour != NULL && sour->ref_count > 0);

		((psrSymb_t*)sour)->ref_count++;
		return sour;

}

void			PSR_DestroySymb(const psrSymb_t *symb)
{
		
		AR_ASSERT(symb != NULL && symb->ref_count > 0);
		
		if(--((psrSymb_t*)symb)->ref_count == 0)
		{
				AR_DEL((psrSymb_t*)symb);
		}
}

*/






int_t					PSR_CompSymb(const psrSymb_t *l, const psrSymb_t *r)
{
		int_t cmp;
		AR_ASSERT(l != NULL && r != NULL);
		if(l == r)return 0;
		cmp = AR_CMP(l->type, r->type);
		if(cmp != 0)return cmp;
		cmp = AR_CMP(l->hash_code, r->hash_code);
		if(cmp != 0)return cmp;
		/*为何靠对比指针来断定字符串请参阅parser_in.h*/


		return AR_CMP(l->name, r->name);		

}



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



bool_t				PSR_RemoveFromSymbListByIndex(psrSymbList_t *symb_lst, size_t index)
{
		size_t i;
		AR_ASSERT(symb_lst != NULL && index < symb_lst->count);
		
		if(index >= symb_lst->count)return false;

		i = index;
		while(i < symb_lst->count - 1)
		{
				symb_lst->lst[i] = symb_lst->lst[i+1];
				i++;
		}
		
		symb_lst->count--;
		return true;
		
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

void			PSR_SortSymbList(psrSymbList_t *symb_lst)
{
		AR_ASSERT(symb_lst != NULL);
		
		if(symb_lst->count > 0)
		{
				AR_qsort((void*)symb_lst->lst, symb_lst->count, sizeof(psrSymb_t*), __comp_symb);
		}
}


bool_t				PSR_InsertToSymbList_Unique(psrSymbList_t *symb_lst, const psrSymb_t *symb)
{
		AR_ASSERT(symb_lst != NULL && symb != NULL);
		if(PSR_FindFromSymbList(symb_lst, symb) != -1)return false;

		PSR_InsertToSymbList(symb_lst, symb);
		return true;


}

/********************************************************************************************************/
void			PSR_PrintSymbol(const psrSymb_t *symb, arString_t *str)
{
		if(symb->type == PSR_TERM)
		{
				AR_AppendFormatString(str, L"%ls ", symb->name);
				
		}else
		{
				AR_AppendFormatString(str, L"<%ls> ", symb->name);
		}
}


void			PSR_PrintSymbolList(const psrSymbList_t *lst, arString_t *str)
{
		size_t i;

		for(i = 0; i < lst->count; ++i)
		{
				PSR_PrintSymbol(lst->lst[i], str);
				AR_AppendFormatString(str, L" ");
		}
}








AR_NAMESPACE_END

