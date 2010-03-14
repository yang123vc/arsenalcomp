#include "common.h"


AR_NAMESPACE_BEGIN




/*************************************************************************qsort*******************************************************************/



typedef ar_int_t (*cmp_func_t)(const void *l, const void *r);




void __insertion_sort(ar_byte_t *lo, ar_byte_t *hi, size_t width, cmp_func_t cmp_f)
{
		ar_byte_t *p,*t;
		AR_ASSERT(lo != NULL && hi != NULL && lo < hi && width > 0 && cmp_f != NULL);
		
		for(p = hi; p > lo; p -= width)
		{
				if(cmp_f(p, p - width) < 0)AR_memswap(p,p - width,width);
		}

		for(t = lo + width; t <= hi; t += width)
		{
				for(p = t; p > lo; p -= width)
				{


						if(cmp_f(p,p - width) < 0)
						{
								AR_memswap(p,p-width,width);
						}else
						{
								break;
						}
				}
		}
}

typedef struct __qsort_range_tag
{
		ar_byte_t*	l_beg;
		ar_byte_t*	l_end;

		ar_byte_t*	r_beg;
		ar_byte_t*	r_end;
}qsort_range_t;


static qsort_range_t __partition(ar_byte_t *lo, ar_byte_t *hi, size_t width, cmp_func_t cmp_f)
{
		qsort_range_t res;
		
		ar_byte_t *mid, *loguy, *higuy;

		AR_ASSERT(lo != NULL && hi != NULL && lo < hi && width > 0 && cmp_f != NULL);
		
		mid = lo + ((hi - lo)/width + 1) /2 * width;

		/*以下三步保证 mid >= lo && mid <= hi*/
        if(cmp_f(lo,mid) > 0)
		{
				AR_memswap(lo,mid, width);
        }
        
		if(cmp_f(lo, hi) > 0) 
		{
				AR_memswap(lo,hi, width);
        }

        if(cmp_f(mid, hi) > 0)
		{
				AR_memswap(mid,hi, width);
        }

		loguy = lo;
		higuy = hi;

		for (;;) 
		{
				if(mid > loguy)
				{
						do{
								loguy += width;
						}while(loguy < mid && cmp_f(loguy, mid) <= 0);
				}
				
				if(mid <= loguy)
				{
						do{
								loguy += width;
						}while(loguy <= hi && cmp_f(loguy, mid) <= 0);
				}
				
				do{
						higuy -= width;
				} while (higuy > mid && cmp_f(higuy, mid) > 0);


				if(higuy < loguy)break;
				
				AR_memswap(loguy, higuy, width);

				if (mid == higuy)mid = loguy;
        }

		higuy += width;
        
		if (mid < higuy)
		{
            do{
					higuy -= width;
            }while(higuy > mid && cmp_f(higuy, mid) == 0);
        }

        if(mid >= higuy)
		{
            do{
					higuy -= width;
            }while(higuy > lo && cmp_f(higuy, mid) == 0);
        }


		res.l_beg = lo;
		res.l_end = higuy;

		res.r_beg = loguy;
		res.r_end = hi;
		
		return res;
}





#define __QSORT_CUTOFF			32
/*log(N)-log(cutoff) + 1*/
#define __QSORT_STKSIZE			(32 * sizeof(void*))



static void __qsort(ar_byte_t *lo, ar_byte_t *hi, size_t width, cmp_func_t cmp_f)
{
		ar_byte_t	*lo_stk[__QSORT_STKSIZE], *hi_stk[__QSORT_STKSIZE];/*这玩意儿要还是溢出了也没得办法~~*/
		ar_int_t	stk_top = 0;
		
		qsort_range_t	range;
		AR_ASSERT(lo != NULL && hi != NULL && lo <= hi && width > 0 && cmp_f != NULL);

		if(lo >= hi)return;

__RECURSE_POINT:

		AR_ASSERT(lo < hi);
		
		if((hi - lo) / width + 1 <= __QSORT_CUTOFF)
		{
				__insertion_sort(lo,hi,width, cmp_f);
		}else
		{
				range = __partition(lo,hi, width, cmp_f);

				
				if(range.l_end - range.l_beg >= range.r_end - range.r_beg)
				{
						if(range.l_beg < range.l_end)
						{
								lo_stk[stk_top] = range.l_beg;
								hi_stk[stk_top] = range.l_end;
								stk_top++;
						}

						if(range.r_beg < range.r_end)
						{
								lo = range.r_beg;
								hi = range.r_end;
								goto __RECURSE_POINT;
						}
				}else
				{
						if(range.r_beg < range.r_end)
						{
								lo_stk[stk_top] = range.r_beg;
								hi_stk[stk_top] = range.r_end;
								stk_top++;
						}

						if(range.l_beg < range.l_end)
						{
								lo = range.l_beg;
								hi = range.l_end;
								goto __RECURSE_POINT;
						}
				}
		}

		stk_top--;

		if(stk_top >= 0)
		{
				lo = lo_stk[stk_top];
				hi = hi_stk[stk_top];
				
				goto __RECURSE_POINT;
		}
}



void AR_qsort(void *base, size_t num, size_t width, ar_int_t (*cmp_f)(const void*, const void*))
{
		AR_ASSERT(base != NULL && width > 0 && cmp_f != NULL);
		
		if(num > 0)
		{
				__qsort((ar_byte_t*)base, (ar_byte_t*)base + (width * (num-1)), width,cmp_f);
		}
}


ar_int_t AR_bsearch(const void *key, const void *base, size_t num, size_t width, ar_int_t (*cmp_f)(const void*, const void*))
{
		ar_int_t l,r,m,cmp;
		AR_ASSERT(base != NULL && width > 0 && cmp_f != NULL && key != NULL);

		l = 0; r = (ar_int_t)(num -1);

		while(l <= r)
		{
				m = (l + r)/2;

				cmp = cmp_f(AR_GET_ELEM(base, width, (size_t)m), key);

				if(cmp < 0)
				{
						l = m + 1;
				}else if(cmp > 0)
				{
						r = m - 1;
				}else
				{
						return m;
				}
		}

		return -1;
}


AR_NAMESPACE_END



