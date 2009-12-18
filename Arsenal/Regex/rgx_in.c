
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


#include "rgx_in.h"

AR_NAMESPACE_BEGIN



void					RGX_InitNameSet(rgxNameSet_t	*set)
{
		AR_ASSERT(set != NULL);
		AR_memset(set, 0, sizeof(*set));
}

extern void			RGX_DestroyNode(rgxNode_t *node);

void					RGX_UnInitNameSet(rgxNameSet_t	*set)
{
		size_t i;

		AR_ASSERT(set != NULL);

		for(i = 0; i < set->count; ++i)
		{
				AR_ASSERT(set->name[i].node);
				RGX_DestroyNode(set->name[i].node);
		}

		if(set->name)AR_DEL(set->name);

		AR_memset(set,0,sizeof(*set));
}

bool_t					RGX_InsertToNameSet(rgxNameSet_t	*set, const wchar_t	*name, rgxNode_t *node)
{
		
		AR_ASSERT(set != NULL && name != NULL && node != NULL);
		if(RGX_FindFromNameSet(set, name) != NULL)return false;
		
		if(set->count == set->cap)
		{
				set->cap = (set->cap + 4)*2;
				set->name = (rgxName_t*)AR_realloc(set->name, sizeof(rgxName_t) * set->cap);
		}

		AR_wcscpy(set->name[set->count].name, name);
		set->name[set->count].node = node;
		
		return true;
}


bool_t					RGX_RemoveFromNameSet(rgxNameSet_t	*set, const wchar_t	*name)
{
		size_t i;
		AR_ASSERT(set != NULL && name != NULL);

		for(i = 0; i < set->count; ++i)
		{
				if(AR_wcscmp(set->name[i].name, name) == 0)
				{
						RGX_DestroyNode(set->name[i].node);
						break;
				}
		}
		
		if(i == set->count)return false;

		while(i < set->count - 1)
		{
				set->name[i] = set->name[i + 1];
				i++;
		}
		set->count--;

		return true;
}




const rgxNode_t*		RGX_FindFromNameSet(const rgxNameSet_t	*set, const wchar_t *name)
{
		size_t i;
		AR_ASSERT(set != NULL && name != NULL);

		for(i = 0; i < set->count; ++i)
		{
				if(AR_wcscmp(set->name[i].name, name) == 0)
				{
						AR_ASSERT(set->name[i].node != NULL);
						return set->name[i].node;
				}
		}

		return NULL;
}



/*char class set*/


void	RGX_InitCClass(rgxCClass_t *cclass)
{
		AR_ASSERT(cclass != NULL);
		AR_memset(cclass, 0, sizeof(*cclass));
}

void	RGX_UnInitCClass(rgxCClass_t *cclass)
{
		AR_ASSERT(cclass != NULL);

		if(cclass->cclass)AR_DEL(cclass->cclass);
		AR_memset(cclass, 0, sizeof(*cclass));
}


int_t	RGX_FindIndexFromCClass(const rgxCClass_t *cclass, wchar_t c)
{
		int_t l,r,m;
		rgxCClassRange_t *tmp;
		AR_ASSERT(cclass != NULL);
		
		l = 0; r = (int_t)cclass->count - 1;

		while(l <= r)
		{
				m = (l + r)/2;
				tmp = cclass->cclass + m;
				if(c < tmp->beg)
				{
						r = m - 1;
				}else if(c > tmp->end)
				{
						l = m + 1;

				}else
				{
						return m;
				}
		}

		return -1;
}


void	RGX_CopyCClass(rgxCClass_t *dest, const rgxCClass_t *other)
{
		size_t i;
		AR_ASSERT(dest != NULL && other != NULL);
		RGX_UnInitCClass(dest);
		
		for(i = 0; i < other->count; ++i)
		{
				RGX_InsertToCClass(dest, &other->cclass[i]);
		}
}


void	RGX_InsertToCClass(rgxCClass_t *dest, const rgxCClassRange_t *range)
{
		AR_ASSERT(dest != NULL && range != NULL && range->beg <= range->end);

		if(dest->count == dest->cap)
		{
				dest->cap = (dest->cap + 1)*2;
				dest->cclass = AR_REALLOC(rgxCClassRange_t, dest->cclass, dest->cap);

				//dest->cclass = (rgxCClassRange_t*)AR_realloc(dest->cclass, dest->cap * sizeof(rgxCClassRange_t*));
		}
		dest->cclass[dest->count++] = *range;
}



/*Char Set*/




void RGX_InitCharSet(rgxCharSet_t *cset)
{
		AR_ASSERT(cset != NULL);
		AR_memset(cset, 0, sizeof(*cset));
}

void RGX_UnInitCharSet(rgxCharSet_t *cset)
{
		AR_ASSERT(cset != NULL);

		while(cset->range)
		{
				rgxCharRange_t		*tmp = cset->range;
				cset->range = cset->range->next;
				AR_DEL(tmp);
		}
}

void RGX_CopyCharSet(rgxCharSet_t *dest, const rgxCharSet_t *sour)
{
		rgxCharRange_t *curr;
		AR_ASSERT(dest != NULL && sour != NULL);
		
		RGX_UnInitCharSet(dest);
		RGX_InitCharSet(dest);
		dest->is_neg = sour->is_neg;

		for(curr = sour->range; curr; curr = curr->next)
		{
				RGX_InsertRangeToCharSet(dest, curr);
		}
}



static rgxCharRange_t* __new_range(wchar_t beg, wchar_t end, rgxCharRange_t *next)
{
		rgxCharRange_t *res;
		AR_ASSERT(beg <= end);
		res = AR_NEW0(rgxCharRange_t);

		res->beg = beg; res->end = end; res->next = next;
		return res;
}



void RGX_InsertRangeToCharSet(rgxCharSet_t *cset, const rgxCharRange_t *new_range)
{
		rgxCharRange_t		*curr, *prev;
		rgxCharRange_t		range;
		AR_ASSERT(cset != NULL && new_range != NULL && new_range->beg <= new_range->end);
		
		prev = NULL; curr = cset->range; 
		range	= *new_range;

		while(curr != NULL)
		{
				if(curr->beg < range.beg)
				{
						if(curr->end < range.beg)
						{
								/*
							[ curr ]
										[ range ]
								*/


						}else if(curr->end < range.end)
						{
								/*
							[ curr ]
								[ range ]
								*/
								
								rgxCharRange_t		*tmp = __new_range(curr->beg, range.beg - 1, curr);
								
								if(prev == NULL)
								{
										cset->range = tmp;
										prev = tmp;
								}else 
								{
										prev->next = tmp; 
								}

								curr->beg = range.beg; range.beg = curr->end + 1;
						
						}else if(curr->end == range.end)
						{
								/*
							[     curr      ]
									 [ range]
							
								*/
								
								rgxCharRange_t		*tmp = __new_range(curr->beg, range.beg - 1, curr);
								if(prev == NULL)
								{
										cset->range = tmp;
										prev = tmp;
								}else 
								{
										prev->next = tmp; 
								}

								curr->beg = range.beg;
								return;
						}else if(curr->end > range.end)
						{
								
								/*
								[  curr      ]
								  [ range ]
								*/
								rgxCharRange_t		*tmp1, *tmp2;
								tmp1 = __new_range(curr->beg, range.beg - 1, NULL);
								if(prev == NULL)
								{
										cset->range = tmp1;
										prev = tmp1;
								}else 
								{
										prev->next = tmp1; 
								}

								tmp2 = __new_range(range.beg, range.end, curr);
								tmp1->next = tmp2;
								curr->beg = range.end + 1;
								return;
						}

				}else if(curr->beg == range.beg)
				{
						if(curr->end < range.end)
						{
								/*
								[curr]
								[ range ]
								*/
								range.beg = curr->end + 1;
						
						}else if(curr->end == range.end)
						{
								/*
								[curr ]
								[range]
								*/

								return;
						}else if(curr->end > range.end)
						{
								/*
								[curr         ]
								[  range  ]

								*/
								
								rgxCharRange_t		*tmp;
								tmp = __new_range(range.beg, range.end, curr);
								if(prev == NULL)
								{
										cset->range = tmp;
										prev = tmp;
								}else 
								{
										prev->next = tmp;
								}
								curr->beg = range.end + 1;
								return;
						}

				}else /*if(curr->beg > range->beg)*/
				{
						if(curr->end < range.end)
						{
								/*
								  [curr]
								[ range  ]
								*/

								rgxCharRange_t		*tmp;
								tmp = __new_range(range.beg, curr->beg - 1, curr);
								if(prev == NULL)
								{
										cset->range = tmp;
										prev = tmp;
								}else 
								{
										prev->next = tmp;
								}

								range.beg = curr->end + 1;

						}else if(curr->end == range.end)
						{
								/*
								    [curr]
								[ range  ]
								*/

								rgxCharRange_t		*tmp;
								tmp = __new_range(range.beg, curr->beg - 1, curr);
								if(prev == NULL)
								{
										cset->range = tmp;
										prev = tmp;
								}else 
								{
										prev->next = tmp;
								}
								return;
						}else/*curr->end > range->end*/
						{
								if(curr->beg <=range.end)
								{

										/*
									     	[curr]
										[range]
										*/

										rgxCharRange_t		*tmp1,*tmp2;

										tmp1 = __new_range(range.beg, curr->beg - 1, NULL);
										if(prev == NULL)
										{
												cset->range = tmp1;
												prev = tmp1;
										}else 
										{
												prev->next = tmp1;
										}

										tmp2 = __new_range(curr->beg, range.end, curr);
										tmp1->next = tmp2;
										curr->beg = range.end + 1;
										return;
								}else
								{
										/*
										       [curr]
									   [range]
										*/

										rgxCharRange_t		*tmp;
										tmp = __new_range(range.beg, range.end, curr);

										if(prev == NULL)
										{
												cset->range = tmp;
												prev = tmp;
										}else
										{
												prev->next = tmp;
										}
										return;
								}
						}
				}
				
				prev = curr;
				curr = curr->next;
		}

		if(cset->range == NULL)
		{
				cset->range = __new_range(range.beg, range.end, NULL);
		}else
		{
				prev->next = __new_range(range.beg, range.end, NULL);
		}
		

}

void RGX_ReverseNegativeCharSet(rgxCharSet_t *cset)
{
		rgxCharRange_t			*curr, *prev, old;
		rgxCharSet_t			new_set;
		
		AR_ASSERT(cset != NULL);
		if(!cset->is_neg)return;

		cset->is_neg = false;

		RGX_InitCharSet(&new_set);
		
		old.beg =  L'\0'; old.end = AR_WCHARMAX;
		
		prev = NULL;
		for(curr = cset->range; curr != NULL; curr = curr->next)
		{
				if(curr->beg > old.beg)
				{
						old.end = curr->beg - 1;
						RGX_InsertRangeToCharSet(&new_set, &old);
				}
				
				old.beg = curr->end < AR_WCHARMAX ? curr->end + 1 : curr->end;
				old.end = AR_WCHARMAX;
				prev = curr;
		}

		if(old.beg < AR_WCHARMAX) RGX_InsertRangeToCharSet(&new_set, &old);
		
		curr = cset->range;
		cset->range = new_set.range;
		new_set.range = curr;
		RGX_UnInitCharSet(&new_set);
}

/*CharSet end*/

AR_NAMESPACE_END

