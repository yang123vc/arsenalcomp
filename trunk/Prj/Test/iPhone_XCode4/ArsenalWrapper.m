//
//  ArsenalWrapper.cpp
//  Test
//
//  Created by peng liu on 2/20/12.
//  Copyright (c) 2012 none. All rights reserved.
//


#include <execinfo.h>
#include "ArsenalWrapper.h"

//using namespace ARSpace;



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






/************************************************************************************************************************/


static void AR_STDCALL tiny_error(int_t level, const wchar_t* msg, void *ctx)
{
        wprintf(L"%ls\r\n", msg);
}

static void AR_STDCALL tiny_printf(const wchar_t *msg, void *ctx)
{
		
        wprintf(L"%ls\r\n", msg);
		
		
}



void    Initialize_Arsenal()
{
        
        arInit_t ai = 
        {
                {
                        tiny_error, 
                        tiny_printf,
                        NULL
                },
                
                {
                        NULL,
                        NULL
                        
                }
        };
		
        if(Arsenal_Init(&ai) != AR_S_YES)
        {
                abort();
        }
       
}

void    UnInitialize_Arsenal()
{
        Arsenal_UnInit();
}