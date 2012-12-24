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
        ar_byte_t *b = AR_NEWARR(ar_byte_t, 1024);
        b = AR_NEWARR(ar_byte_t, 4021);
                
}
 

void misc_test()
{
		int **arr = (int**)100;

		printf("%d\r\n", arr - 1);

}

        
        
        
        
        static void uri_parse_test()
        {
                arURI_t	*uri;
                arStatus_t status;
                
                status = AR_S_YES;
                arString_t *str;
                
                uri = AR_CreateURI(AR_CP_UTF8);
                str = AR_CreateString();
                
                AR_ASSERT(uri != NULL);
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"http://www.appinf.com");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);
                
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"www.appinf.com") == 0);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_GetStringLength(str) == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_GetStringLength(str) == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_GetStringLength(str) == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"http://www.appinf.com/");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"www.appinf.com") == 0);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_GetStringLength(str) == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_GetStringLength(str) == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"ftp://anonymous@ftp.appinf.com/pub/");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"ftp") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 21);
                
                AR_ASSERT(AR_GetURIUserInfo(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"anonymous") == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"anonymous@ftp.appinf.com") == 0);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/pub/") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_GetStringLength(str) == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_GetStringLength(str) == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"https://www.appinf.com/index.html#top");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"https") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 443);
                
                AR_ASSERT(AR_GetURIUserInfo(uri, str) == AR_S_YES);
                AR_ASSERT(AR_GetStringLength(str) == 0);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/index.html") == 0);
                
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"top") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"http://www.appinf.com/search.cgi?keyword=test&scope=all");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 80);
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/search.cgi") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"keyword=test&scope=all") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"http://www.appinf.com/search.cgi?keyword=test&scope=all#result");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 80);
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/search.cgi") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"keyword=test&scope=all") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"result") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetEncodedURI(uri, L"http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 80);
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/search.cgi") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"keyword=test encoded&scope=all") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"result") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"ldap://[2001:db8::7]/c=GB?objectClass?one");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"ldap") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"2001:db8::7") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 389);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"[2001:db8::7]") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/c=GB") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"objectClass?one") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"mailto:John.Doe@example.com");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"mailto") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"John.Doe@example.com") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"tel:+86-010-6970-1111");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"tel") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"+86-010-6970-1111") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"telnet://192.0.2.16:80");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"telnet") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"192.0.2.16") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 80);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"192.0.2.16:80") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"urn:oasis:names:specification:docbook:dtd:xml:4.1.2");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"urn") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"oasis:names:specification:docbook:dtd:xml:4.1.2") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"/foo/bar");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/foo/bar") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"./foo/bar");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"./foo/bar") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"../foo/bar");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"../foo/bar") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"index.html");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"index.html") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"index.html#frag");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"index.html") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"frag") == 0);
                
                AR_ASSERT(AR_IsRelativeURI(uri));
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"?query=test");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"query=test") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_IsRelativeURI(uri));
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"?query=test#frag");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"query=test") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"frag") == 0);
                
                AR_ASSERT(AR_IsRelativeURI(uri));
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"#frag");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"frag") == 0);
                
                AR_ASSERT(AR_IsRelativeURI(uri));
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"#");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_IsRelativeURI(uri));
                
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"file:///a/b/c");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"file") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/a/b/c") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"file://localhost/a/b/c");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"file") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"localhost") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"localhost") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/a/b/c") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"file:///c:/Windows/system32/");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"file") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/c:/Windows/system32/") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(!AR_IsRelativeURI(uri));
                
                
                
                /*******************************************************************************/
                status = AR_SetURI(uri, L"./c:/Windows/system32/");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"./c:/Windows/system32/") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_IsRelativeURI(uri));
                
                AR_DestroyURI(uri);
                uri = NULL;
                AR_DestroyString(str);
                str = NULL;
        }
        
        
        
        static void uri_construct_test()
        {
                
                arURI_t	*uri;
                arStatus_t status;
                
                status = AR_S_YES;
                arString_t *str;
                
                uri = AR_CreateURI(AR_CP_UTF8);
                str = AR_CreateString();
                
                AR_ASSERT(uri != NULL);
                
                /**************************************************************************/
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                
                /**************************************************************************/
                status = AR_SetURIScheme(uri, L"FTP");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"ftp") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 21);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                /**************************************************************************/
                AR_ClearURI(uri);
                status = AR_SetURIAuthority(uri, L"www.appinf.com");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 0);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"www.appinf.com") == 0);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                /**************************************************************************/
                AR_ClearURI(uri);
                status = AR_SetURIAuthority(uri, L"user@services.appinf.com:8000");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIScheme(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIUserInfo(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"user") == 0);
                
                status = AR_GetURIHost(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"services.appinf.com") == 0);
                
                AR_ASSERT(AR_GetURIPort(uri) == 8000);
                
                status = AR_GetURIAuthority(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"user@services.appinf.com:8000") == 0);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_SetURIPath(uri, L"/index.html");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/index.html") == 0);
                
                
                status = AR_SetURIEncodedPath(uri, L"/file%20with%20spaces.html");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/file with spaces.html") == 0);
                
                status = AR_SetURIPathEtc(uri, L"/query.cgi?query=foo");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/query.cgi") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"query=foo") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                status = AR_GetURIPathEtc(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/query.cgi?query=foo") == 0);
                
                
                
                
                status = AR_SetURIPathEtc(uri, L"/query.cgi?query=bar#frag");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIPath(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/query.cgi") == 0);
                
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"query=bar") == 0);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"frag") == 0);
                
                status = AR_GetURIPathEtc(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/query.cgi?query=bar#frag") == 0);
                
                
                status = AR_SetURIQuery(uri, L"query=test");
                AR_ASSERT(status == AR_S_YES);
                status = AR_GetURIQuery(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"query=test") == 0);
                
                status = AR_SetURIFragment(uri, L"result");
                AR_ASSERT(status == AR_S_YES);
                
                status = AR_GetURIFragment(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"result") == 0);
                
                status = AR_GetURIPathEtc(uri, str);
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/query.cgi?query=test#result") == 0);
                
                
                
                AR_DestroyURI(uri);
                uri = NULL;
                AR_DestroyString(str);
                str = NULL;
        }
        
        
        static void uri_tostring_test()
        {
                arURI_t	*uri;
                arStatus_t status;
                
                status = AR_S_YES;
                arString_t *str;
                
                uri = AR_CreateURI(AR_CP_UTF8);
                str = AR_CreateString();
                
                AR_ASSERT(uri != NULL && str != NULL);
                
                /**********************************************************************************/
                
                AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com") == 0);
                
                
                
                AR_ASSERT(AR_SetURI(uri, L"ftp://anonymous@ftp.appinf.com/pub/") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"ftp://anonymous@ftp.appinf.com/pub/") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"ftp://anonymous@ftp.appinf.com/%e4%b8%ad%e5%9b%bd%e5%ad%97") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_ICompStringWithWcs(str, L"ftp://anonymous@ftp.appinf.com/%e4%b8%ad%e5%9b%bd%e5%ad%97") == 0);
                
                AR_SetURICodePage(uri, AR_CP_GB2312);
                AR_ASSERT(AR_SetURI(uri, L"ftp://anonymous@ftp.appinf.com/%d6%d0%b9%fa%d7%d6") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_ICompStringWithWcs(str, L"ftp://anonymous@ftp.appinf.com/%d6%d0%b9%fa%d7%d6") == 0);
                AR_SetURICodePage(uri, AR_CP_UTF8);
                
                
                AR_ASSERT(AR_SetEncodedURI(uri, L"ftp://anonymous@ftp.appinf.com/%e4%b8%ad%e5%9b%bd%e5%ad%97") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_ICompStringWithWcs(str, L"ftp://anonymous@ftp.appinf.com/中国字") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"https://www.appinf.com/index.html#top") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"https://www.appinf.com/index.html#top") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/search.cgi?keyword=test&scope=all") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/search.cgi?keyword=test&scope=all") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/search.cgi?keyword=test&scope=all#result") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/search.cgi?keyword=test&scope=all#result") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"ldap://[2001:db8::7]/c=GB?objectClass?one") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"ldap://[2001:db8::7]/c=GB?objectClass?one") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"mailto:John.Doe@example.com") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"mailto:John.Doe@example.com") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"tel:+1-816-555-1212") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"tel:+1-816-555-1212") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"telnet://192.0.2.16:80") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"telnet://192.0.2.16:80") == 0);
                
                
                
                AR_SetURICodePage(uri, AR_CP_GB2312);
                AR_ASSERT(AR_SetURI(uri, L"ftp://anonymous@ftp.appinf.com") == AR_S_YES);
                
                AR_ASSERT(AR_SetURIPath(uri, L"/中国字") == AR_S_YES);
                
                AR_ASSERT(AR_GetEncodedURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_ICompStringWithWcs(str, L"ftp://anonymous@ftp.appinf.com/%d6%d0%b9%fa%d7%d6") == 0);
                AR_SetURICodePage(uri, AR_CP_UTF8);
                
                
                AR_ASSERT(AR_SetURI(uri, L"urn:oasis:names:specification:docbook:dtd:xml:4.1.2") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"urn:oasis:names:specification:docbook:dtd:xml:4.1.2") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"/foo/bar") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/foo/bar") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"./foo/bar") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"./foo/bar") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"../foo/bar") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"../foo/bar") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"//foo/bar") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"//foo/bar") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"index.html") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"index.html") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"index.html#frag") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"index.html#frag") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"?query=test") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"?query=test") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"?query=test#frag") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"?query=test#frag") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"#frag") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"#frag") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"#") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"file:///a/b/c") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"file:///a/b/c") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"file://localhost/a/b/c") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"file://localhost/a/b/c") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"file:///c:/Windows/system32/") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"file:///c:/Windows/system32/") == 0);
                
                AR_ASSERT(AR_SetURI(uri, L"./c:/Windows/system32/") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"./c:/Windows/system32/") == 0);
                
                
                AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com") == AR_S_YES);
                AR_ASSERT(AR_SetURIQuery(uri, L"query=test") == AR_S_YES);
                
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/?query=test") == 0);
                
                /**********************************************************************************/
                
                AR_ASSERT(AR_SetURI(uri, L"http://www.example.com/List of holidays.xml") == AR_S_YES);
                AR_ASSERT(AR_GetEncodedURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.example.com/List%20of%20holidays.xml") == 0);
                
                
                AR_ASSERT(AR_SetEncodedURI(uri, L"http://qt.nokia.com/List%20of%20holidays.xml") == AR_S_YES);
                AR_ASSERT(AR_GetEncodedURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://qt.nokia.com/List%20of%20holidays.xml") == 0);
                
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://qt.nokia.com/List of holidays.xml") == 0);
                
                
                
                AR_SetURICodePage(uri, AR_CP_UTF8);
                AR_ASSERT(AR_SetURI(uri, L"http://www.example.com/List of holidays.xml?搜索1=关键字1a=b关键字2=搜索2") == AR_S_YES);
                AR_ASSERT(AR_GetEncodedURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.example.com/List%20of%20holidays.xml?%e6%90%9c%e7%b4%a21=%e5%85%b3%e9%94%ae%e5%ad%971a=b%e5%85%b3%e9%94%ae%e5%ad%972=%e6%90%9c%e7%b4%a22") == 0);
                
                
                AR_ASSERT(AR_SetEncodedURI(uri, L"http://www.example.com/List%20of%20holidays.xml?%e6%90%9c%e7%b4%a21=%e5%85%b3%e9%94%ae%e5%ad%971a=b%e5%85%b3%e9%94%ae%e5%ad%972=%e6%90%9c%e7%b4%a22") == AR_S_YES);
                AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.example.com/List of holidays.xml?搜索1=关键字1a=b关键字2=搜索2") == 0);
                
                
                AR_DestroyURI(uri);
                uri = NULL;
                AR_DestroyString(str);
                str = NULL;
        }
        
        static void uri_compare_test()
        {
                
                arURI_t	*u1, *u2;
                arString_t *str;
                arStatus_t status;
                status = AR_S_YES;
                
                u1 = AR_CreateURI(AR_CP_UTF8);
                u2 = AR_CreateURI(AR_CP_UTF8);
                str = AR_CreateString();
                AR_ASSERT(u1 != NULL && u2 != NULL && str != NULL);
                
                AR_SetURI(u1, L"http://www.appinf.com");
                AR_SetURI(u2, L"HTTP://www.appinf.com:80");
                
                AR_ASSERT(AR_CompURI(u1, u2) == 0);
                
                AR_ASSERT(AR_GetURI(u1, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com") == 0);
                
                AR_ASSERT(AR_GetURI(u2, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com") == 0);
                
                
                
                AR_SetURI(u1, L"?query");
                AR_ASSERT(AR_GetURI(u1, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"?query") == 0);
                AR_ASSERT(AR_CompStringWithWcs(str, L"?query2") != 0);
                
                
                AR_SetURI(u1, L"#frag");
                AR_ASSERT(AR_GetURI(u1, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"#frag") == 0);
                AR_ASSERT(AR_CompStringWithWcs(str, L"#frag2") != 0);
                
                
                AR_SetURI(u1, L"/index.html#frag");
                AR_ASSERT(AR_GetURI(u1, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/index.html#frag") == 0);
                AR_ASSERT(AR_CompStringWithWcs(str, L"/index.html") != 0);
                
                
                AR_SetURICodePage(u1, AR_CP_GB2312);
                AR_SetURICodePage(u2, AR_CP_GB2312);
                AR_SetURI(u1, L"http://www.appinf.COM/pub/中国字");
                AR_SetEncodedURI(u2, L"HTTP://www.appinf.com:80/pub/%d6%d0%b9%fa%d7%d6");
                
                AR_ASSERT(AR_CompURI(u1, u2) == 0);
                
                AR_ASSERT(AR_GetURI(u1, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/pub/中国字") == 0);
                
                AR_ASSERT(AR_GetURI(u2, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/pub/中国字") == 0);
                
                
                AR_SetURICodePage(u1, AR_CP_UTF8);
                AR_SetURI(u1, L"http://www.appinf.COM/pub/中国字");
                
                AR_SetURICodePage(u2, AR_CP_GB2312);
                AR_SetEncodedURI(u2, L"HTTP://www.appinf.com:80/pub/%d6%d0%b9%fa%d7%d6");
                
                
                AR_ASSERT(AR_CompURI(u1, u2) == 0);
                
                AR_SetURICodePage(u1, AR_CP_GB2312);
                AR_ASSERT(AR_GetEncodedURI(u1, str) == AR_S_YES);
                AR_ASSERT(AR_ICompStringWithWcs(str, L"HTTP://www.appinf.com/pub/%d6%d0%b9%fa%d7%d6") == 0);
                
                AR_SetURICodePage(u2, AR_CP_UTF8);
                AR_ASSERT(AR_GetURI(u2, str) == AR_S_YES);
                AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/pub/中国字") == 0);
                
                AR_ASSERT(AR_GetEncodedURI(u2, str) == AR_S_YES);
                AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/pub/%e4%b8%ad%e5%9b%bd%e5%ad%97") == 0);
                
                
                
                
                AR_DestroyURI(u1);
                AR_DestroyURI(u2);
                AR_DestroyString(str);
                u1 = NULL;
                u2 = NULL;
                str = NULL;
                
        }
        
        static void uri_test()
        {
                uri_construct_test();
                uri_parse_test();
                uri_tostring_test();
                uri_compare_test();
        }
        

void charset_test()
{
        const wchar_t *s = L"中国";
        
        char *str = AR_wcs_to_str(AR_CP_BIG5, s, AR_wcslen(s));
        
}

        
        
void text_test_load_save()
{

#define INPUT_FILE		L"/Users/solidus/Desktop/Programming/Arsenal/misc/txt_enc_test/utf8_read_test.txt"
#define OUTPUT_FILE		L"/Users/solidus/Desktop/test.txt"
#define OUTPUT_TYPE		AR_TXT_BOM_UTF_8
                
        arString_t *str = AR_CreateString();
        arTxtBom_t bom;
                
        if(AR_LoadBomTextFile(INPUT_FILE,&bom, str) != AR_S_YES)
        {
                AR_ASSERT(false);
        }
                
        if(AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStringCString(str)) != AR_S_YES)
        {
                AR_ASSERT(false);
        }
                
                

        if(AR_LoadBomTextFile(OUTPUT_FILE, &bom, str) != AR_S_YES)
        {
                AR_ASSERT(false);
        }
                
        AR_ASSERT(bom == OUTPUT_TYPE);
                
        AR_DestroyString(str);
}
 
        
        
        static void str_test19()
        {
                const char *str =    "lazy";
                const char *string = "The quick brown dog jumps over the lazy fox";
                char fmt1[] =   "         1         2         3         4         5";
                char fmt2[] =   "12345678901234567890123456789012345678901234567890";
                
                char *pdest;
                int  result;
                printf( "String to be searched:\n   %s\n", string );
                printf( "   %s\n   %s\n\n", fmt1, fmt2 );
                pdest = strstr((char*)string, str );
                result = (int)(pdest - string + 1);
                if ( pdest != NULL )
                {
                        printf( "%s found at position %d\n", str, result );
                }
                else
                {
                        printf( "%s not found\n", str );
                }
                
                
                pdest = (char*)AR_strstr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
                result = (int)(pdest - string + 1);
                if ( pdest != NULL )
                {
                        printf( "%s found at position %d\n", str, result );
                }
                else
                {
                        printf( "%s not found\n", str );
                }
                
                str = "fox";
                pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
                result = (int)(pdest - string + 1);
                if ( pdest != NULL )
                {
                        printf( "%s found at position %d\n", str, result );
                }
                else
                {
                        printf( "%s not found\n", str );
                }
                
                
                /*******************************************************************************/
                
                str = "";
                pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
                
                if ( pdest != NULL )
                {
                        printf( "found str : '%s'\n", pdest);
                }
                else
                {
                        printf( "%s not found\n", str );
                }
                
                pdest = (char*)AR_strstr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
                
                if ( pdest != NULL )
                {
                        printf( "found str : '%s'\n", pdest);
                }
                else
                {
                        printf( "%s not found\n", pdest);
                }
                
                /*******************************************************************************/
                
                str = "";
                string = "";
                
                pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
                
                if ( pdest != NULL )
                {
                        printf( "found str : '%s'\n", pdest);
                }
                else
                {
                        printf( "%s not found\n", str );
                }
                
                pdest = (char*)AR_strstr(string, str);
                
                if ( pdest != NULL )
                {
                        printf( "found str : '%s'\n", pdest);
                }
                else
                {
                        printf( "%s not found\n", pdest);
                }
                
                
                /*******************************************************************************/
                
                str = "fox abc";
                string = "The quick brown dog jumps over the lazy fox";
                
                pdest = (char*)AR_strstr_s(string, string + AR_strlen(string), str, str + 3);
                
                if (pdest != NULL)
                {
                        AR_ASSERT(pdest == AR_strstr(string, "fox"));
                        printf( "found str : '%s'\n", pdest);
                }
                else
                {
                        printf( "%s not found\n", str );
                }
                
                /*******************************************************************************/
                
                str = "FOX abc";
                string = "The quick brown dog jumps over the lazy fox";
                
                pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + 3);
                
                if (pdest != NULL)
                {
                        AR_ASSERT(pdest == AR_strstr(string, "fox"));
                        printf( "found str : '%s'\n", pdest);
                }
                else
                {
                        printf( "%s not found\n", str );
                }
                
                
                /*******************************************************************************/
                
                str = "FOX abc";
                string = "The quick brown dog jumps over the lazy fox";
                
                pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + 4);
                
                if (pdest != NULL)
                {
                        AR_ASSERT(false);
                        printf( "found str : '%s'\n", pdest);
                }
                else
                {
                        printf( "%s not found\n", str );
                }
                
                
                
                /*******************************************************************************/
                const wchar_t *wcs = L"FOX abc";
                const wchar_t *wstr = L"The quick brown dog jumps over the lazy fox";
                const wchar_t *pwdest;
                
                pwdest = AR_wcsistr_s(wstr, wstr + AR_wcslen(wstr), wcs, wcs + 3);
                
                if(pwdest != NULL)
                {
                        AR_ASSERT(pwdest == AR_wcsstr(wstr, L"fox"));
                        printf("found str : '%ls'\n", pwdest);
                }
                else
                {
                        printf( "%ls not found\n", wstr);
                }
                
                
                /*******************************************************************************/
                wcs = L"FOX abc";
                wstr = L"The quick brown dog jumps over the lazy fox";
                
                
                pwdest = AR_wcsistr_s(wstr, wstr + AR_wcslen(wstr), wcs, wcs + 4);
                
                if(pwdest != NULL)
                {
                        AR_ASSERT(false);
                        printf("found str : '%ls'\n", pwdest);
                }
                else
                {
                        printf( "'%ls' not found\n", wstr);
                }
                
                
                /*******************************************************************************/
                wcs = L"FOX abc";
                wstr = L"The quick brown dog jumps over the lazy fox";
                
                
                pwdest = AR_wcsistr_s(wstr, wstr + AR_wcslen(wstr), wcs, wcs + 4);
                
                if(pwdest != NULL)
                {
                        AR_ASSERT(false);
                        printf("found str : '%ls'\n", pwdest);
                }
                else
                {
                        printf( "'%ls' not found\n", wstr);
                }
        }
        
        
static void str_test20()
{
        char buf[1024];
        
        ar_int_t n = AR_wcs_to_str_buf(AR_CP_UTF8, L"中文", 2, buf, 1024);
        
        buf[n] = 0;
        printf("%s\r\n", buf);
        
        
        {
                wchar_t wbuf[1024];
                const char *utf8 = "中文";
                ar_int_t n = AR_str_to_wcs_buf(AR_CP_GB2312, utf8, strlen(utf8), wbuf, 1024);
                wbuf[n] = 0;
                AR_printf(L"%ls\r\n", wbuf);
        }
}
        
        
static void str_test21()
{
        const wchar_t *s = L"一、360安全卫士安装许可协议：";
        wchar_t *esc_s = AR_str_to_escstr(s);
        
        AR_printf(L"%ls\r\n", esc_s);
        
        arEscStrErr_t err;
        
        const wchar_t *src_s = AR_escstr_to_str(esc_s, &err);
        
        
        AR_printf(L"%ls\r\n", src_s);
}
 
        
static void str_test22()
{
        wchar_t buf[64], buf2[64];
        ar_uint_64_t v = L'、', v2 = L'中';

        
        ar_int_t l = AR_u64tow_buf(buf, AR_NELEMS(buf), v, 16);
        ar_int_t l2 = AR_u64tow_buf(buf2, AR_NELEMS(buf2), v2, 16);
        
        AR_ASSERT(l > 0);
        
}
        
void common_test()
{
        AR_printf(L"On common_test\r\n");
        
        
        //thd_test();
        //str_test12();
        //mem_test();
        //misc_test();
        //uri_test();
        //charset_test();
        
        text_test_load_save();
        
        //str_test19();
        //str_test20();
 
        //str_test21();
        //str_test22();
}
        
        
        
        
        
        
        
        
#if defined(__cplusplus)
}
#endif
