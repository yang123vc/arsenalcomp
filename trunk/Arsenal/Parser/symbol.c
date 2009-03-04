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

AR_NAMESPACE_BEGIN

/**********************************************************************************************/

/*以下为终结符*/
static const psrSymb_t __epsilon_symb	= {L"%EPSILON",	False, 0, PSR_ASSO_NOASSO, NULL, 0};
static const psrSymb_t __eoi_symb		= {L"%EOI",		False, 0, PSR_ASSO_NOASSO, NULL, PSR_EOI_TOKVAL};
static const psrSymb_t __lalr_symb		= {L"%LALR",	False, 0, PSR_ASSO_NOASSO, NULL, 2};


static const psrSymb_t __error_symb		= {L"error",	False, 0, PSR_ASSO_NOASSO, NULL, 3};
/*static const psrSymb_t __error_ok_symb	= {L"error_ok",	False, 0, PSR_ASSO_NOASSO, NULL, 4};*/

const psrSymb_t	*PSR_EpsilonSymb		= &__epsilon_symb;
const psrSymb_t	*PSR_EOISymb			= &__eoi_symb;
const psrSymb_t *PSR_LALRSymb			= &__lalr_symb;
const psrSymb_t	*PSR_ErrorSymb			= &__error_symb;
/*const psrSymb_t	*PSR_ErrorOKSymb		= &__error_ok_symb;*/


/*以下为非终结符*/
static const psrSymb_t __start_symb		= {L"%Start",	True,  0, PSR_ASSO_NOASSO, NULL, 0};
const psrSymb_t	*PSR_StartSymb			= &__start_symb;


/*********************************************************************************************/

psrError_t PSR_SetSymb(psrSymb_t *symb, const wchar_t *name, bool_t is_nt, size_t prio, psrAssoType_t asso, size_t val)
{
		size_t len;
		AR_ASSERT(symb != NULL && name != NULL);
		len = AR_WSTRLEN(name);
		AR_MSET0(symb, sizeof(*symb));

		if(len == 0 || len > PARSER_MAXNAME) return PSR_INVALID_TOKEN;
		if(prio < PSR_MIN_PRIOVAL || prio > PSR_MAX_PRIOVAL)return PSR_INVALID_PRIO;

		if(is_nt)
		{
				if(val < PSR_MIN_NONTERM_ID)return PSR_INVALID_NONTERM_ID;
		}else
		{
				if(val < PSR_MIN_TOKENVAL) return PSR_INVALID_TOKEN;
		}
		
		AR_WSTRCPY(symb->name, name);
		symb->non_term = is_nt;
		symb->priority = prio;
		symb->asso = asso;
		symb->val = val;
		return PSR_NO_ERROR;
}





bool_t PSR_IsNullable(const psrSymb_t *symb)
{
		return (PSR_CompSymb(symb, PSR_EpsilonSymb) == 0 ? True : False);
}


int PSR_CompSymb(const psrSymb_t *left, const psrSymb_t *right)
{
		int res;
		
		if(left->non_term != right->non_term)
		{
				
				res = (left->non_term == False ? -1 : 1);
		}else
		{
				if(left->non_term)
				{
						res = AR_WSTRCMP(left->name, right->name);
				}else
				{
						res = AR_CMP(left->val, right->val);
				}
		}

		return res;


}


psrSymb_t* PSR_FindSymb(psrSymb_t *start, const psrSymb_t *key)
{
		psrSymb_t *curr;
		
		for(curr = start; curr; curr = curr->next)
		{
				if(PSR_CompSymb(curr, key) == 0)return curr;
		}
		return NULL;
}


psrSymb_t* PSR_CopyNewSymb(const psrSymb_t *sour)
{
		psrSymb_t *new_symb;
		new_symb = AR_NEW0(psrSymb_t);
		AR_MCPY(new_symb, sour, sizeof(*sour));
		new_symb->next = NULL;
		return new_symb;
}


void	PSR_InsertSymb(psrSymb_t **psymbs, const psrSymb_t *item)/*这里必须保证插入顺序，因为后面需要保证产生式体的序列*/
{
		psrSymb_t *new_node, *prev, *curr;
		new_node = PSR_CopyNewSymb(item);
		
		prev = NULL; curr = *psymbs;
		while(curr != NULL)
		{
				prev = curr;
				curr = curr->next;
		}

		if(prev == NULL)
		{
				AR_ASSERT(*psymbs == NULL);
				*psymbs = new_node;
		}else
		{
				prev->next = new_node;
		}

}


bool_t PSR_InsertSymb_Unique(psrSymb_t **psymbs, const psrSymb_t *item)
{
		if(PSR_FindSymb(*psymbs, item) == NULL)
		{
				PSR_InsertSymb(psymbs, item);
				return True;
		}else
		{
				return False;
		}

}

psrSymb_t* PSR_DestroySymb(psrSymb_t *sour)
{
		psrSymb_t *next;
		next = sour->next;
		AR_DEL(sour);
		return next;
}

void	PSR_DestroySymbList(psrSymb_t *list)
{		
		while(list != NULL)
		{
				list = PSR_DestroySymb(list);
		}
}



/***********************for lr itemset*****************************************/

int		PSR_CompSymbList(const psrSymb_t *left, const psrSymb_t *right)/*must be ordered*/
{
		const psrSymb_t *l, *r;

		l = left; r = right;
		
		while(l != NULL && r != NULL)
		{
				int cmp;
				cmp = PSR_CompSymb(l,r);
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





bool_t	PSR_InsertSymbByOrder_Unique(psrSymb_t **psymbs, const psrSymb_t *item)
{
		int cmp;
		psrSymb_t *curr, *prev;
		AR_ASSERT(psymbs != NULL && item != NULL);

		if(*psymbs == NULL)
		{
				*psymbs = PSR_CopyNewSymb(item);
				return True;
		}else
		{

				curr = *psymbs; prev = NULL;
				
				while(curr != NULL)
				{
						cmp = PSR_CompSymb(curr, item);
						if(cmp == 0)
						{
								return False;
						}else if(cmp == -1)
						{
								prev = curr;
								curr = curr->next;
						}else /*curr > item*/
						{
								/*prev一定小于item或者为NULL*/
								if(prev == NULL)
								{
										AR_ASSERT(curr == *psymbs);
										*psymbs = PSR_CopyNewSymb(item);
										(*psymbs)->next = curr;
								}else
								{
										prev->next = PSR_CopyNewSymb(item);
										prev->next->next = curr;
								}
								break;
						}
				}

				return True;
		}
}


psrSymb_t* PSR_CopyNewSymbList(const psrSymb_t *sour)
{
		psrSymb_t *dest, **last;
		const psrSymb_t *curr;
		dest = NULL; 
		last = &dest;
		curr = sour;
		
		while(curr != NULL)
		{
				*last = PSR_CopyNewSymb(curr);
				last = &(*last)->next;
				curr = curr->next;
		}
		
		return dest;
}


size_t			PSR_CountSymbList(const psrSymb_t *lst)
{
		size_t count;
		const psrSymb_t *curr;
		count = 0;
		curr = lst;
		while(curr != NULL)
		{
				count++;
				curr = curr->next;
		}
		return count;
}

const psrSymb_t* PSR_IndexOfSymbList(const psrSymb_t *lst, size_t index)
{
		int idx;
		const psrSymb_t *curr;
		idx = (int)index;
		curr = lst;
		while(curr != NULL && idx > 0)
		{
				curr = curr->next;
				idx--;
		}
		return curr;
}


AR_NAMESPACE_END

