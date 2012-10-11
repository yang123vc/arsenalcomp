//
//  main.c
//  Test
//
//  Created by peng liu on 2/13/12.
//  Copyright (c) 2012 none. All rights reserved.
//


#include "stdhdr.h"

extern void test();

int main (int argc, const char * argv[])
{

        // insert code here...
        //printf("%s\r\n", setlocale(LC_ALL, "zh_CN.UTF-8"));
        
        Arsenal_Init(NULL);
        

        AR_printf(L"%ls\r\n", L"Hellow world! 中文");
        
        test();
        
        AR_printf(L"%ls\r\n", L"done");
        
        Arsenal_UnInit(NULL);
        return 0;
}

