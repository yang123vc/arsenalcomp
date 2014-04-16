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
				/*执行到此，保证[lo->loguy) <= mid*/
				
				do{
						higuy -= width;
				} while (higuy > mid && cmp_f(higuy, mid) > 0);
				/*执行到此，保证(higuy->hi] > mid*/


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





#define __QSORT_CUTOFF			64
/*log(N)-log(cutoff) + 1*/
#define __QSORT_STKSIZE			(64 * sizeof(void*))



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



void AR_nth_elem(void *arr, size_t count, size_t width, size_t idx, ar_int_t (*cmp_f)(const void*, const void*))
{
		qsort_range_t range;
		ar_int_t l, r;

		AR_ASSERT(arr != NULL && count > 0 && width > 0 && idx < count && cmp_f != NULL);

		l = 0;
		r = (ar_int_t)count - 1;

		while(r > l)
		{
				size_t lo_beg_idx, lo_end_idx, hi_beg_idx, hi_end_idx;
				range = __partition((ar_byte_t*)AR_GET_ELEM(arr, width, l), (ar_byte_t*)AR_GET_ELEM(arr, width, r), width, cmp_f);

				lo_beg_idx = (range.l_beg - (ar_byte_t*)arr) / width;
				lo_end_idx = (range.l_end - (ar_byte_t*)arr) / width;

				hi_beg_idx = (range.r_beg - (ar_byte_t*)arr) / width;
				hi_end_idx = (range.r_end - (ar_byte_t*)arr) / width;


				if(lo_end_idx >= lo_beg_idx && idx >= lo_beg_idx && idx <= lo_end_idx)
				{
						l = (ar_int_t)lo_beg_idx;
						r = (ar_int_t)lo_end_idx;

				}else if(hi_end_idx >= hi_beg_idx && idx >= hi_beg_idx && idx <= hi_end_idx)
				{
						l = (ar_int_t)hi_beg_idx;
						r = (ar_int_t)hi_end_idx;
				}else
				{
						break;
				}
		}
}



/*
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
*/


ar_int_t AR_bsearch(const void *key, const void *base, size_t num, size_t width, ar_int_t (*cmp_f)(const void*, const void*))
{
		const ar_byte_t	*lo, *hi, *mid;
		size_t	half;
		ar_int_t	result;
		AR_ASSERT(base != NULL && width > 0 && cmp_f != NULL && key != NULL);

		lo = (const ar_byte_t*)base; 
		hi = (const ar_byte_t*)base + (num - 1) * width;
		
		while(lo <= hi)
		{
				if((half = num /2) != 0)
				{
						mid = lo + (num & 1 ? half : (half - 1)) * width;
						result = cmp_f(key, mid);
						
						if(result == 0)
						{
								return (mid - (const ar_byte_t*)base) / width;

						}else if(result < 0)
						{
								hi = mid - width;
								num = num & 1 ? half : half - 1;
						}else
						{
								lo = mid + width;
								num = half;
						}
				}else if(num != 0)
				{
						result = cmp_f(key, (const void*)lo);
						if(result == 0)
						{
								return (lo - (const ar_byte_t*)base) / width;
						}else
						{
								break;
						}
				}else
				{
						break;
				}
				
		}
		return -1;
}


/******************************************************************Hash Memory***********************************************/



/*出自：http://www.azillionmonkeys.com/qed/hash.html*/

#define get16bits(d) ((((ar_uint_32_t)(((const ar_uint_8_t *)(d))[1])) << 8)\
                       +(ar_uint_32_t)(((const ar_uint_8_t *)(d))[0]) )
 
ar_uint_32_t __super_fast_hash_32(const ar_byte_t * data, size_t len)
{
		ar_uint_32_t hash, tmp;
		size_t rem;
		AR_ASSERT(data != NULL && len > 0);
		
		if(len == 0 || data == NULL)
		{
				return 0;
		}
		
		hash = (ar_uint_32_t)len;

		rem = len & 3;
		len >>= 2;

		/* Main loop */

		for (;len > 0; len--) 
		{
				hash  += get16bits (data);
				tmp   = (get16bits (data+2) << 11) ^ hash;
				hash  = (hash << 16) ^ tmp;
				data  += 2 * sizeof(ar_uint_16_t);
				hash  += hash >> 11;
		}
		
		/* Handle end cases */
		switch (rem) 
		{
		case 3: hash += get16bits (data);
				hash ^= hash << 16;
				hash ^= data[sizeof (ar_uint_16_t)] << 18;
				hash += hash >> 11;
				break;
		case 2: hash += get16bits (data);
				hash ^= hash << 11;
				hash += hash >> 17;
				break;
		case 1: hash += *data;
				hash ^= hash << 10;
				hash += hash >> 1;
		}

		/* Force "avalanching" of final 127 bits */
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;
		return hash;
}



ar_uint_64_t __murmur_hash_64_a(const ar_byte_t *key, size_t len, ar_uint_64_t seed)
{
		static const ar_uint_64_t m = AR_BIGNUM_U64(0xc6a4a7935bd1e995);
		static const ar_uint_64_t r = 47;
		ar_uint_64_t h;
		const ar_uint_64_t *data;
		const ar_uint_64_t *end;
		const unsigned char * data2;
		AR_ASSERT(key != NULL && len > 0);

		h = seed ^ (len * m);
		
		data = (const ar_uint_64_t *)key;
		end = data + (len / 8);

		while(data != end)
		{
				ar_uint_64_t k = *data++;

				k *= m; 
				k ^= k >> r; 
				k *= m; 

				h ^= k;
				h *= m; 
		}

		data2 = (const unsigned char*)data;

		switch(len & 7)
		{
		case 7: 
				h ^= (ar_uint_64_t)data2[6] << 48;
		case 6: 
				h ^= (ar_uint_64_t)data2[5] << 40;
		case 5: 
				h ^= (ar_uint_64_t)data2[4] << 32;
		case 4: 
				h ^= (ar_uint_64_t)data2[3] << 24;
		case 3: 
				h ^= (ar_uint_64_t)data2[2] << 16;
		case 2: 
				h ^= (ar_uint_64_t)data2[1] << 8;
		case 1: 
				h ^= (ar_uint_64_t)data2[0];
				h *= m;
				break;
		default:
				break;
		};

		h ^= h >> r;
		h *= m;
		h ^= h >> r;

		return h;
} 

ar_uint_t AR_memhash(const ar_byte_t *data, size_t len)
{
#if(AR_ARCH_VER == ARCH_64)
		return __murmur_hash_64_a(data, len, 0);
#elif(AR_ARCH_VER == ARCH_32)
		return __super_fast_hash_32(data, len);
#else
		#error "Not Support Platform!"
#endif

}




/***********************heap****************************/





static void __heap_fixup(void *arr, size_t count, size_t idx, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*) )
{
		count = count;
		AR_ASSERT(idx < count);
		AR_UNUSED(count);

		while(idx > 0)
		{
				size_t p = (idx - 1)/ 2;
                
				if(cmp_f(AR_GET_ELEM(arr,element_size, p), AR_GET_ELEM(arr, element_size, idx)) >= 0)
                {
                        break;
                }
				
				swap_f((void*)AR_GET_ELEM(arr,element_size, p), (void*)AR_GET_ELEM(arr, element_size, idx));
                
				idx = p;
		}
}

static void __heap_fixdown(void *arr, size_t count, size_t idx, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*))
{
		size_t c;
		AR_ASSERT(idx < count);
        AR_ASSERT(arr != NULL);


		while(idx  < count / 2)
		{
				c = idx * 2 + 1;
                
				if(c < count - 1 && cmp_f(AR_GET_ELEM(arr, element_size, c), AR_GET_ELEM(arr, element_size, c + 1)) < 0)
                {
                        ++c;
                }
                
				if(cmp_f(AR_GET_ELEM(arr, element_size, idx), AR_GET_ELEM(arr, element_size, c)) >= 0)
                {
                        break;
                }
                
				swap_f((void*)AR_GET_ELEM(arr, element_size, idx), (void*)AR_GET_ELEM(arr, element_size, c));
				idx = c;
		}
}


static void __heap_remove_at(void *arr, size_t count, size_t idx, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*))
{
		size_t i,p;
		AR_ASSERT(arr != NULL && idx < count);
		AR_ASSERT(swap_f != NULL && cmp_f != NULL);

		swap_f((void*)AR_GET_ELEM(arr, element_size, idx), (void*)AR_GET_ELEM(arr, element_size, count - 1));
		
		count--;
		
		if(count == 0 || idx == count)
		{
				return;
		}

		i = idx;
		
		while(i > 0)
		{
				p = (i - 1) / 2;
				
				if(cmp_f(AR_GET_ELEM(arr, element_size, p), AR_GET_ELEM(arr, element_size, i)) >= 0)
				{
						break;
				}
				
				swap_f((void*)AR_GET_ELEM(arr, element_size, p), (void*)AR_GET_ELEM(arr, element_size, i));/*否则交换数组及其索引*/
				
				i = p;
		}


		__heap_fixdown(arr, count, i, element_size, cmp_f, swap_f);

}


#if defined(AR_DEBUG)

static ar_bool_t __heap_verify(void *arr, size_t count, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*))
{
		size_t i,l,r;
		AR_ASSERT(arr != NULL);
		i = 0;
		while(i < count / 2)
		{
				l = (2 * i) + 1;
				r = l + 1;

				if(cmp_f(AR_GET_ELEM(arr, element_size, i), AR_GET_ELEM(arr, element_size, l)) < 0)
				{
						return false;
				}

				if(r < count && cmp_f(AR_GET_ELEM(arr, element_size, i), AR_GET_ELEM(arr, element_size, r)) < 0)
				{
						return false;
				}
				
				i += 1;
		}

		return true;
}

#endif



void AR_make_heap(void *arr, size_t count, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*))
{
		ar_int_t n;
        
		AR_ASSERT(arr != NULL && count > 0 && element_size > 0 && cmp_f != NULL && swap_f != NULL);

		
		for(n = (ar_int_t)count / 2 - 1; n >= 0; n--)
        {
                __heap_fixdown(arr, count, (size_t)n, element_size, cmp_f, swap_f);
        }

		AR_ASSERT(__heap_verify(arr, count, element_size, cmp_f));
}

void AR_sort_heap(void *arr, size_t count, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*))
{
		AR_ASSERT(arr != NULL && count > 0 && element_size > 0 && cmp_f != NULL && swap_f != NULL);
		
		while(count > 1)
		{
				swap_f((void*)AR_GET_ELEM(arr, element_size, 0), (void*)AR_GET_ELEM(arr, element_size, count-1));
				count -= 1;
				__heap_fixdown(arr, count, 0, element_size, cmp_f, swap_f);
		}
}



void AR_push_heap(void *arr, size_t count, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*))
{
		
		AR_ASSERT(arr != NULL && count > 0 && element_size > 0 && cmp_f != NULL && swap_f != NULL);
		
		__heap_fixup(arr, count, count-1, element_size, cmp_f, swap_f);

		AR_ASSERT(__heap_verify(arr, count, element_size, cmp_f));
}


void AR_pop_heap(void *arr, size_t count, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*))
{
		
		AR_ASSERT(arr != NULL && count > 0 && element_size > 0 && cmp_f != NULL && swap_f != NULL);
		
		if(count == 1)
		{
				return;
		}

		swap_f((void*)AR_GET_ELEM(arr, element_size, 0), (void*)AR_GET_ELEM(arr, element_size, count-1));
		
		__heap_fixdown(arr, count-1, 0, element_size, cmp_f, swap_f);

		AR_ASSERT(__heap_verify(arr, count - 1, element_size, cmp_f));
}



void AR_remove_heap(void *arr, size_t count, size_t idx, size_t element_size, ar_int_t (*cmp_f)(const void*, const void*), void (*swap_f)(void*, void*))
{
		AR_ASSERT(arr != NULL && count > 0 && idx < count && element_size > 0 && cmp_f != NULL && swap_f != NULL);
		__heap_remove_at(arr, count, idx, element_size, cmp_f, swap_f);
		AR_ASSERT(__heap_verify(arr, count - 1, element_size, cmp_f));
}









AR_NAMESPACE_END



