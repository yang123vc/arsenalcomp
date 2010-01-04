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






/*****************************************************Memory*******************************/
#if defined(AR_USE_CRT_ALLOCFUNC)


#else

void*	AR_malloc(size_t nbytes)
{
		void *ptr;
		/*
		do{
				ptr = malloc(nbytes);
		}while(ptr == NULL);
		*/
		while((ptr = malloc(nbytes)) == NULL)AR_YiledThread();

		return ptr;
}

void*	AR_calloc(size_t num, size_t size)
{
		void *ptr;
		/*
		do{
			ptr = calloc(num , size);
		}while(ptr == NULL);
		*/
		while((ptr = calloc(num,size)) == NULL)AR_YiledThread();
		return ptr;
}		

void*	AR_realloc(void *block, size_t nbytes)
{
		void *ptr;
		/*
		do{
				ptr = realloc(block, nbytes);		
		}while(ptr == NULL);
		*/
		while((ptr = realloc(block, nbytes)) == NULL)AR_YiledThread();
		return ptr;
}

void	AR_free(void *ptr)
{
		if(ptr)free(ptr);
}

#endif


void	AR_memswap(void *a, void *b, size_t n)
{
		int_t cnt;
		
		AR_ASSERT(a != NULL && b != NULL);

		if(a == b)return;
		
		cnt = (int_t)(n / sizeof(int_t));
		
		while(cnt-- > 0)
		{
				int_t t = *(int_t*)a;
				*(int_t*)a = *(int_t*)b;
				*(int_t*)b = t;
				a = (int_t*)a + 1;
				b = (int_t*)b + 1;
		}

		cnt = (int_t)(n %  sizeof(int_t));

		while(cnt-- > 0)
		{
				byte_t t = *(byte_t*)a;
				*(byte_t*)a = *(byte_t*)b;
				*(byte_t*)b = t;
				a = (byte_t*)a + 1;
				b = (byte_t*)b + 1;
		}

}






AR_NAMESPACE_END
