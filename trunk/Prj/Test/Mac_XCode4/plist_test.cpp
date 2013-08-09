//
//  plist_test.c
//  Test
//
//  Created by solidus on 13-8-9.
//  Copyright (c) 2013年 none. All rights reserved.
//


#include "Arsenal.h"





AR_NAMESPACE_BEGIN

static void load_test()
{
        arStatus_t status;
        arString_t *xml = AR_CreateString();
        
        system("pwd");
        status = PList_LoadXMLFromFile(L"/Users/solidus/Desktop/Code/Arsenal/misc/plist_enc_test/plist_big5.plist", xml);
        
        AR_printf(L"%ls\r\n", AR_CSTR(xml));
        
        AR_DestroyString(xml);
        xml = NULL;
}



void plist_test()
{
        load_test();
}



AR_NAMESPACE_END


