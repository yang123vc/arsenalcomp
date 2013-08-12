//
//  plist_test.c
//  Test
//
//  Created by solidus on 13-8-9.
//  Copyright (c) 2013年 none. All rights reserved.
//


#include "Arsenal.h"





AR_NAMESPACE_BEGIN


static void base64_test_for_plist()
{
        const char *b = "ENi/X/9/AACupOyH/38AADAGAAAAAAAAMAQQAAEAAADwv552/38AADgugAABAAAAYNi/\r\n"
                        "X/9/AABupm2S/38AAAAAAAAAAAAAACiAAAEAAAAAMAAAAQAAAGjYHHj/fwAAAQAAAAAA\r\n"
                        "AADwv552/38AAAAAAAAAAAAAAZGhdv9/AABg279f/38AAD6jbpL/fwAAAQAAAAAAAAAB\r\n"
                        "2r9f/38AAAAAAAAAAAAA0K2edv9/AAAAAAAAAAAAAAH4v1//fwAAoNu/X/9/AACwahAA\r\n"
                        "AQAAABBznXb/fwAAoHMQAAEAAADAKZ52/38AAAIAAAAAAAAA//////////8AAAAAAAAA\r\n"
                        "AAAwAAABAAAAAAAAAAAAAAAAagAAAQAAAABqAAABAAAAwNm/X/9/AAC5e+2H/38AANDZ\r\n"
                        "v1//fwAAAQAAAAAAAAABAAAAAAAAAKqwbZL/fwAA8EAAAAEAAAABAO2HAAAAAARqAAAB\r\n"
                        "AAAAAQAAAAEAAABADhAAAQAAABA8gAABAAAAADAAAAEAAAD//////////wAAAAAAAAAA\r\n"
                        "AAAAAAAAAAAAMAAAAQAAAJCxnnb/fwAAAGoAAAEAAAABAAAAAAAAAP//////////AAAA\r\n"
                        "AAAAAAAAMAAAAQAAAAAAAAAAAAAAAGoAAAEAAAAAagAAAQAAAJDav1//fwAAuXvth/9/\r\n"
                        "AACQ2r9f/38AAAEAAAAAAAAA/v////////+qsG2S/38AAAAwAAABAAAAAAAAAAAAAAAA\r\n"
                        "agAAAQAAAIDBnnb/fwAAQA4QAAEAAAAQPIAAAQAAAJCxnnb/fwAAAgAAAAAAAAD4////\r\n"
                        "/////+F7EAABAAAAADAAAAEAAADQwZ52/38AAABqAAABAAAAAGoAAAEAAAAw279f/38A\r\n"
                        "ALl77Yf/fwAAAAAAAAAAAAAEAAAAAAAAADgAAAAAAAAA+HkQAAEAAADwQAAAAQAAAAQA\r\n"
                        "gAABAAAABGoAAAEAAAAEAAAAAAAAAAAAAAAAAAAAINu/X/9/AAAQ279f/38AAHCm7If/\r\n"
                        "fwAAACiAAAEAAAAfAAAAAAAAADAEEAABAAAA8CmAAAEAAADQwZ52/38AAAAAAAAAAAAA\r\n"
                        "YNu/X/9/AAD8pW2S/38AAAAAAAAAAAAAACiAAAEAAAAAMAAAAQAAAGjYHHj/fwAAAQAA\r\n"
                        "AAAAAADQwZ52/38AAAAAAAAAAAAAAZGhdv9/AABg3r9f/38AAD6jbpL/fwAAEHQQAAEA\r\n"
                        "AAABAAAAAAAAAAEAAAAAAAAAATADAAEAAABQ3L9f/38AADh77Yf/fwAAAAAAAAAAAAAI\r\n"
                        "AAAAAAAAAIAAAAAAAAAAsH0QAAEAAACQL552/38AAAgAAAAAAAAABGoAAAEAAABA2Bx4\r\n"
                        "/38AAA==\r\n";
        
        
        //ar_int_t AR_base64_decode(ar_byte_t  *out, size_t olen, const ar_byte_t *input, size_t ilen)
        
        ar_byte_t out[2048];
        
        ar_int_t l = AR_base64_decode(out, 2048, (const ar_byte_t*)b, AR_strlen(b));
        
        AR_ASSERT(l == 1024);
}

static void format_test()
{
        wprintf(L"%hs\r\n", "abcdef");
        
        AR_printf(L"%hc\r\n", 'a');
        //AR_printf(L"%c\r\n", L'中');
}

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
        //format_test();
        //base64_test_for_plist();
        //load_test();
}



AR_NAMESPACE_END


