//
//  main.c
//  Test
//
//  Created by peng liu on 2/13/12.
//  Copyright (c) 2012 none. All rights reserved.
//


#include "Arsenal.h"

extern void test();






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
                NULL,
                NULL,
        }
};



int main (int argc, const char * argv[])
{

        // insert code here...
                
        printf("%s\r\n", setlocale(LC_ALL, "zh_CN.UTF-8"));
        
        
        
        Arsenal_Init(&ctx);
        

        AR_printf(L"%ls\r\n", L"Hellow world! 中文");
        
        //test();
        
        AR_printf(L"%ls\r\n", L"done");
        
        Arsenal_UnInit(NULL);
        return 0;
}

