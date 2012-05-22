//
//  File.c
//  Test
//
//  Created by solidus on 12-4-27.
//  Copyright (c) 2012年 none. All rights reserved.
//

#include "lfu_cache.h"
#include "TestFileSystem.h"




#if defined(__cplusplus)
using namespace ARSpace;
#include <vector>
#include <queue>
#include <string>

extern "C"{
        
#endif
        
        
        
        
        static void evt_test()
        {
                arStatus_t status;
                arEvent_t *evt = AR_CreateEvent(false);
                
                status = AR_WaitEventWithTimeout(evt, 5000);
                
                printf("%d\r\n", status);
                
                AR_SetEvent(evt);
                
                status = AR_WaitEventWithTimeout(evt, 5000);
                
                printf("%d\r\n", status);
                
                AR_ResetEvent(evt);
                
                
                status = AR_WaitEventWithTimeout(evt, 5000);
                
                printf("%d\r\n", status);
        }
        
        
        
        static void output_thread(void *data)
        {
                arStatus_t status;
                arAsyncQueue_t	*queue;
                AR_ASSERT(data != NULL);
                queue = (arAsyncQueue_t*)data;
                
                while(true)
                {
                        status = AR_GetFromAsyncQueueWithTimeout(queue, &data, 2000);
                        if(status == AR_S_YES)
                        {
                                const char *s = (const char*)data;
                                AR_ASSERT(s != NULL);
                                printf("thread == %p : %s\r\n", pthread_self(), s);
                                
                                if(strcmp(s, "quit") == 0)
                                {
                                        
                                        AR_DEL(s);
                                        break;
                                }else
                                {
                                        AR_DEL(s);
                                }
                                
                        }else if(status == AR_E_TIMEOUT)
                        {
                                printf("thread %p timeout\r\n", pthread_self());
                                continue;
                        }else
                        {
                                AR_ASSERT(false);
                        }
                }
                
                
        }
        
        
        
        
#define THREAD_CNT  20
        static void async_queue_test()
        {
                int i;
                char buf[1024];
                arThread_t *thread_set[THREAD_CNT];
                arAsyncQueue_t	queue;
                AR_InitAsyncQueue(&queue);
                
                for(i = 0; i < THREAD_CNT; ++i)
                {
                        thread_set[i] = AR_CreateThread(output_thread, (void*)&queue);
                }
                
                printf("«Î ‰»Î:\r\n");
                while(true)
                {
                        char *s = NULL;
                        gets(buf);
                        s = AR_NEWARR(char, 1024);
                        strcpy(s, buf);
                        AR_PutToAsyncQueue(&queue, (void*)s);
                        if(strcmp(buf, "quit")== 0)break;
                }
                
                for(i = 0; i < THREAD_CNT; ++i)
                {
                        char *s = AR_NEWARR(char, 1024);
                        strcpy(s, "quit");
                        AR_PutToAsyncQueue(&queue, (void*)s);
                }
                
                for(i = 0; i < THREAD_CNT; ++i)
                {
                        AR_DestroyThread(thread_set[i]);
                        thread_set[i] = NULL;
                }
                
                while(!AR_AsyncQueueIsEmpty(&queue))
                {
                        const char *s;
                        AR_GetFromAsyncQueue(&queue, (void**)&s);
                        printf("delete string == %s\r\n", s);
                        AR_DEL(s);
                }
                
                AR_UnInitAsyncQueue(&queue);
        }
        
        
        
        
void thd_test()
{
       // evt_test();
        async_queue_test();
}
        


static void str_test12()
{
        //wprintf(L"%S\r\n", "abcdefg");
                
        AR_printf(L"%ls\r\n", L"ls");
        AR_printf(L"%lS\r\n", L"lS");
        AR_printf(L"%s\r\n", L"s");
        AR_printf(L"%S\r\n", L"S");
                
                
        AR_printf(L"%hs\r\n", "hs");
        AR_printf(L"%hS\r\n", "hS");
        AR_printf(L"%s\r\n", L"s");
        AR_printf(L"%S\r\n", L"S");
                
        AR_printf(L"%hS : %lS\r\n", "hS", L"lS");
        AR_printf(L"%s : %S\r\n", "s", L"S");
        AR_printf(L"%hs : %ls\r\n", "ls", L"lS");
                
}
        
        
void mem_test()
{
        byte_t *b = AR_NEWARR(byte_t, 1024);
        b = AR_NEWARR(byte_t, 4021);
                
}
 
        
        
        
uint_64_t		hash_func(void *key, void *usr_ctx)
{
        std::wstring *wcs = (std::wstring*)key;
        AR_ASSERT(key != NULL);
        return AR_wcshash(wcs->c_str());
}
        
int_t			comp_func(void *l, void *r, void *usr_ctx)
{
        std::wstring *ls = (std::wstring*)l;
        std::wstring *rs = (std::wstring*)r;
        
        AR_ASSERT(ls != NULL && rs != NULL);
        return AR_wcscmp(ls->c_str(), rs->c_str());
}
        
arStatus_t		copy_wcs_func(void *data, void **pnew_data, void *usr_ctx)
{
        std::wstring *src = (std::wstring*)data;
        std::wstring **pdest = (std::wstring**)pnew_data;
        
        AR_printf(L"%ls\r\n", src->c_str());
        *pdest = AR_NEW(std::wstring);
        
        *pdest = new ((void*)*pdest) std::wstring;
        
        **pdest = *src;
        return AR_S_YES;
        
}
        
void destroy_wcs_func(void *data, void *usr_ctx)
{
        std::wstring *src = (std::wstring*)data;
        AR_ASSERT(data != NULL);
        src->~basic_string();
        AR_DEL(src);
}
        
        /*
        typedef struct __cache_lfu_context_tag
        {
                Cache_hash_func_t       hash_f;
                Cache_comp_func_t       comp_f;
                
                Cache_copy_func_t       copy_key_f;
                Cache_copy_func_t       copy_data_f;
                
                Cache_destroy_func_t    destroy_key_f;
                Cache_destroy_func_t    destroy_data_f;
                void    *usr_ctx;
        }cacheLFUCtx_t;
        */

void cache_test()
{
        cacheLFUCtx_t ctx = 
        {
                hash_func,
                comp_func,
                copy_wcs_func,
                copy_wcs_func,
                destroy_wcs_func,
                destroy_wcs_func,
                NULL
        };

        using namespace ARSpace;
        
        cacheLFU_t *lfu = Cache_CreateLFU(&ctx, 100);
        
        for(int i = 0; i < 1000; ++i)
		{
				wchar_t buf[512];
                AR_swprintf(buf, 512, L"test:%d", i);
                std::wstring key, val;
                val = buf;
                AR_swprintf(buf, 512, L"%d", i);
                key = buf;
                
                if(Cache_InsertToLFU(lfu, &key, &val) != AR_S_YES)
                {
                        AR_ASSERT(false);
                }
		}
        
        
		for(int i = 0; i < 1000; ++i)
		{
                std::wstring key;
                wchar_t buf[512];
                AR_swprintf(buf, 512, L"%d", rand() % 1000);
                key = buf;
                std::wstring *val;

                if(Cache_AccessFromLFU(lfu, (void*)&key, (void**)&val) == AR_S_YES)
                {
                        AR_printf(L"%ls\r\n", val->c_str());

                }
		}
        
		for(int i = 0; i < 100; i++)
		{
				//AR_ASSERT(cache.Delete(i));
		}
        
        
        Cache_DestroyLFU(lfu);
        lfu = NULL;
}
        
        
void common_test()
{
        AR_printf(L"On common_test\r\n");
        
        //thd_test();
        //str_test12();
        //mem_test();
        cache_test();
}
        
        
        
        
        
        
        
        
#if defined(__cplusplus)
}
#endif
