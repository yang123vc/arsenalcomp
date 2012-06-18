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
#include "heap.h"

AR_NAMESPACE_BEGIN


#if !defined(AR_USE_CRT_ALLOCFUNC)

		static arSpinLock_t		__g_lock;
		static arHeap_t			*__g_heap = NULL;

		static AR_INLINE void*	malloc_mem(size_t bytes);
		static AR_INLINE void	free_mem(void *ptr);


		#if defined(AR_ENABLE_MEMORY_LEAK_TEST)

		typedef struct __debug_memory_record_tag
		{
				const char *							file;
				int_t									line;
				int_t									size;
				struct __debug_memory_record_tag* 		prev;
				struct __debug_memory_record_tag		*next;

		}debugMemory_t;

		static arSpinLock_t		__g_debug_mem_lock;
		static debugMemory_t	*__g_debug_alloc_mem = NULL;
		static debugMemory_t	*__g_debug_free_mem = NULL;

		static void init_debug_memory()
		{
				AR_InitSpinLock(&__g_debug_mem_lock);
				__g_debug_alloc_mem = NULL;
				__g_debug_free_mem = NULL;
		}

		static void check_debug_memory()
		{
				debugMemory_t *dm;

				
				AR_LockSpinLock(&__g_debug_mem_lock);

				dm = __g_debug_alloc_mem;

				while(dm)
				{
						AR_ASSERT(dm->size > 0);

						AR_error(AR_ERR_DEBUG, L"size: %6d Bytes : %hs, line: %d\r\n", dm->size, dm->file, dm->line);
						dm = dm->next;
				}

				AR_UnLockSpinLock(&__g_debug_mem_lock);
		}

		static void free_debug_memory()
		{
				debugMemory_t *dm, *tmp;

				dm = __g_debug_alloc_mem;

				while(dm)
				{
						tmp = dm->next;
						free_mem((void*)dm);
						dm = tmp;
				}

				dm = __g_debug_free_mem;

				while(dm)
				{
						AR_ASSERT(dm->size < 0);
						tmp = dm->next;
						free_mem((void*)dm);
						dm = tmp;
				}
				__g_debug_free_mem = NULL;
				__g_debug_alloc_mem = NULL;
		}

		static void uninit_debug_memory()
		{
				check_debug_memory();
				free_debug_memory();
				__g_debug_free_mem = NULL;
				__g_debug_alloc_mem = NULL;
				AR_UnInitSpinLock(&__g_debug_mem_lock);
		}

		#endif

#endif


void	AR_InitMemory()
{
#if !defined(AR_USE_CRT_ALLOCFUNC)
		AR_InitSpinLock(&__g_lock);
		__g_heap = AR_CreateHeap();
		if(__g_heap == NULL)
		{
				AR_error(AR_ERR_FATAL, L"AR_InitMemory failure\r\n");
		}

		#if defined(AR_ENABLE_MEMORY_LEAK_TEST)
				init_debug_memory();
		#endif

#endif
}


void	AR_UnInitMemory()
{
#if !defined(AR_USE_CRT_ALLOCFUNC)

		arHeapUsage_t	usage;


		#if defined(AR_ENABLE_MEMORY_LEAK_TEST)
				uninit_debug_memory();
		#endif


		


		usage = AR_GetHeapMemUsage(__g_heap);

		AR_error(AR_ERR_MESSAGE, L"Peak memroy used : %qu KB" L"\r\n", usage.peak_mem_used/1024);


		AR_DestroyHeap(__g_heap);
		__g_heap = NULL;
		AR_UnInitSpinLock(&__g_lock);
#endif
}





#if !defined(AR_USE_CRT_ALLOCFUNC)

static AR_INLINE void*	malloc_mem(size_t bytes)
{
		void *ret;
		AR_ASSERT(bytes > 0);
		if(bytes == 0)
		{
				return NULL;
		}

		AR_LockSpinLock(&__g_lock);
		ret = AR_AllocFromHeap(__g_heap, bytes);
		AR_UnLockSpinLock(&__g_lock);
		return ret;
}

static AR_INLINE void	free_mem(void *ptr)
{
		AR_ASSERT(ptr != NULL);
		AR_LockSpinLock(&__g_lock);
		AR_FreeToHeap(__g_heap, ptr);
		
		AR_UnLockSpinLock(&__g_lock);
}



/*****************************************************Memory*******************************/


#if defined(AR_ENABLE_MEMORY_LEAK_TEST)

		void*	AR_debug_malloc(size_t nbytes, const char *file_name, int_t line)
		{
				debugMemory_t *m;
				AR_ASSERT(nbytes > 0 && file_name != NULL);

				m = (debugMemory_t*)malloc_mem(nbytes + sizeof(debugMemory_t));

				if(m == NULL)
				{
						return NULL;
				}


				AR_LockSpinLock(&__g_debug_mem_lock);
				
				AR_memset(m, 0, nbytes + sizeof(debugMemory_t));

				m->file = file_name;
				m->line = line;
				m->size = nbytes;
				m->prev = NULL;
				m->next = __g_debug_alloc_mem;

				if(__g_debug_alloc_mem)
				{
						__g_debug_alloc_mem->prev = m;
				}

				__g_debug_alloc_mem = m;

				
				
				
				AR_UnLockSpinLock(&__g_debug_mem_lock);


				return (void*)(((byte_t*)m) + sizeof(debugMemory_t));
		}
		


		void*	AR_debug_calloc(size_t num, size_t size, const char *file_name, int_t line)
		{
				void *ptr;
				AR_ASSERT(num > 0 && size > 0 && file_name != NULL);
				ptr = AR_debug_malloc(num * size, file_name, line);

				if(ptr)
				{
						AR_memset(ptr, 0, num * size);
				}

				return ptr;

		}
		

		void	AR_debug_free(void *ptr, const char *file_name, int_t line)
		{
				debugMemory_t *dm;
				AR_ASSERT(file_name != NULL);
				if(!ptr)
				{
						return;
				}
				
				
				AR_LockSpinLock(&__g_debug_mem_lock);

				dm = (debugMemory_t *) ( ( (byte_t *) ptr ) - sizeof( debugMemory_t ) );

#if defined(AR_DEBUG_MEMORY_CHECK_TWICE_FREE)
				if(dm->size < 0)
				{
						AR_error(AR_ERR_FATAL,  L"memory freed twice, first from %hs line %Iu, now from %hs : %Iu\r\n", dm->file, dm->line, file_name, line);
						
				}
#endif
				

				
				if(dm->next)
				{
						dm->next->prev = dm->prev;
				}
				
				if(dm->prev)
				{
						AR_ASSERT(__g_debug_alloc_mem != dm);
						dm->prev->next = dm->next;
				}else 
				{
						AR_ASSERT(__g_debug_alloc_mem == dm);
						__g_debug_alloc_mem = dm->next;
				}

#if defined(AR_DEBUG_MEMORY_CHECK_TWICE_FREE)
				dm->file = file_name;
				dm->line = line;
				dm->size = -dm->size;
				dm->next = __g_debug_free_mem;
				__g_debug_free_mem = dm;
#else
				AR_UNUSED(line);
				free_mem((void*)dm);
#endif		
				
				AR_UnLockSpinLock(&__g_debug_mem_lock);

		}


#else


		void*	AR_malloc(size_t nbytes)
		{
				void *ptr;
				size_t i;
				AR_ASSERT(nbytes > 0);

				i = 0;
				ptr = NULL;

				while(i < AR_MEM_MAX_ALLOC_RETRY_COUNT && (ptr = malloc_mem(nbytes)) == NULL)
				{
						AR_YieldThread();
						++i;
				}

				/*
				if(!ptr)
				{
				AR_error(AR_ERR_FATAL, L"malloc failure for %Iu\r\n", nbytes);
				}
				*/

				return ptr;
		}


		void*	AR_calloc(size_t num, size_t size)
		{
				size_t i;
				void *ptr;
				AR_ASSERT(num > 0 && size > 0);

				ptr = NULL;
				i = 0; 


				while(i < AR_MEM_MAX_ALLOC_RETRY_COUNT)
				{
						ptr = malloc_mem(num *size);

						if(ptr != NULL)
						{
								AR_memset(ptr, 0, num * size);
								return ptr;
						}else
						{
								AR_YieldThread();
								++i;
						}
				}

				/*
				if(!ptr)
				{
				AR_error(AR_ERR_FATAL, L"calloc failure for %Iu\r\n", num * size);
				}
				*/

				return ptr;
		}		



		void	AR_free(void *ptr)
		{
				if(ptr)
				{
						free_mem(ptr);
				}
		}




#endif



#endif


void	AR_memswap(void *a, void *b, size_t n)
{
		int_t cnt;
		
		AR_ASSERT(a != NULL && b != NULL);

		if(a == b)
		{
				return;
		}


		if((size_t)a % sizeof(uint_t) != 0 || (size_t)b % sizeof(uint_t) != 0)
		{
				cnt = (int_t)n;

				while(cnt-- > 0)
				{
						byte_t t = *(byte_t*)a;
						*(byte_t*)a = *(byte_t*)b;
						*(byte_t*)b = t;
						a = (byte_t*)a + 1;
						b = (byte_t*)b + 1;
				}
		}else
		{
				cnt = (int_t)(n / sizeof(int_t));

				while(cnt-- > 0)
				{
						uint_t t = *(uint_t*)a;
						*(uint_t*)a = *(uint_t*)b;
						*(uint_t*)b = t;
						a = (uint_t*)a + 1;
						b = (uint_t*)b + 1;
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

}




AR_NAMESPACE_END
