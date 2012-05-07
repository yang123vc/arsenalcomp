//
//  File.c
//  Test
//
//  Created by solidus on 12-4-27.
//  Copyright (c) 2012年 none. All rights reserved.
//

#include "TestFileSystem.h"



#if defined(__cplusplus)
using namespace ARSpace;
#include <vector>
#include <queue>

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
        

        
void common_test()
{
        AR_printf(L"On common_test\r\n");
        
        thd_test();
}
        
        
        
        
        
        
        
        
#if defined(__cplusplus)
}
#endif
