//
//  main.c
//  Test
//
//  Created by peng liu on 2/13/12.
//  Copyright (c) 2012 none. All rights reserved.
//


#include "stdhdr.h"
#include <execinfo.h>

extern void test();




static size_t AR_STDCALL __build_backtrace(void **call_stack, size_t call_stack_cnt)
{
        size_t i, nstack;
        
        AR_ASSERT(call_stack != NULL && call_stack_cnt > 0);
        
        nstack = backtrace(call_stack, (int)call_stack_cnt);
        
        for(i = nstack; i < call_stack_cnt; ++i)
        {
                call_stack[i] = NULL;
        }
        
        return nstack;
}

static size_t AR_STDCALL __build_backtrace_string(void **call_stack, size_t call_stack_cnt, char *str, size_t l)
{
        char **strings;
        char *p;
        size_t remain;
        size_t i, n;
        
        AR_ASSERT(call_stack != NULL);
        AR_ASSERT(str != NULL && l > 0);
        
        if(call_stack_cnt == 0)
        {
                str[0] = '\0';
                return 0;
        }
        
        strings = backtrace_symbols(call_stack, (int)call_stack_cnt);
        if(strings == NULL) 
        {
                str[0] = '\0';
                return 0;
        }
        
        remain = l - 1;
        p = str;
        
        for (i = 1; i < call_stack_cnt && remain > 0; i++)
        {       
                if(snprintf(p, remain, "%s\r\n", strings[i]) <= 0)
                {
                        break;
                }
                
                n = strlen(p);
                
                if(n == 0)
                {
                        break;
                }
                
                p += n;
                remain -= n;
        }
        
        *p = '\0';
        
        size_t ret = (size_t)strlen(str);
        
        free(strings);
        
        
        return ret;
        
}




static void AR_STDCALL __on_error(ar_int_t level, const wchar_t* msg, void *ctx)
{
		AR_UNUSED(level);
		AR_UNUSED(ctx);
        
        printf("%ls", msg);
}


static void AR_STDCALL __on_print(const wchar_t *msg, void *ctx)
{
		/*AR_UNUSED(msg);*/
		AR_UNUSED(ctx);
        
		printf("%ls", msg);
}


arInit_t ctx = 
{
        {
                __on_error,
                __on_print,
                NULL,
        },
        
        {
                __build_backtrace,
                __build_backtrace_string,
        }
};



int main (int argc, const char * argv[])
{

        // insert code here...
                
        printf("%s\r\n", setlocale(LC_ALL, "zh_CN.UTF-8"));
        
        
        
        Arsenal_Init(&ctx);
        
        AR_NEWARR(char, 10240);

        AR_printf(L"%ls\r\n", L"Hellow world! 中文");
        
        test();
        
        AR_printf(L"%ls\r\n", L"done");
        
        Arsenal_UnInit(NULL);
        return 0;
}

