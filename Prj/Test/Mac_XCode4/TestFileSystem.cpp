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
      
        

static void env_test()
{
        arString_t *str = AR_CreateString();
        AR_setenv(L"TEST", L"刘鹏");

        {

                const char *t = getenv("TEST");
                printf("%s\r\n", t);
        }
        
        AR_getenv(L"TEST", str);
        AR_StrPrint(str);
        
        AR_getenv(L"PWD", str);
        AR_StrPrint(str);
        
        AR_getenv(L"HOME", str);
        AR_StrPrint(str);
        
        
        AR_getenv(L"TMPDIR", str);
        AR_StrPrint(str);
        
        AR_DestroyString(str);
        str = NULL;
        
}
       

static void path_test()
{
        arString_t *str;
        arStatus_t status;
        str = AR_CreateString();
        
        status = AR_GetCurrentPath(str);
        
        AR_ASSERT(status == AR_S_YES);
        AR_StrPrint(str);
        
        
        status = AR_GetHomePath(str);
        AR_ASSERT(status == AR_S_YES);
        AR_StrPrint(str);
        
        
        status = AR_GetTempPath(str);
        AR_ASSERT(status == AR_S_YES);
        AR_StrPrint(str);
        
        status = AR_GetNullPath(str);
        AR_ASSERT(status == AR_S_YES);
        AR_StrPrint(str);
        
        
        status = AR_GetExpandPath(L"~/abc/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_StrPrint(str);

        
        status = AR_GetExpandPath(L"~/$LOGNAME$Apple_Ubiquity_Message/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_StrPrint(str);
        
        status = AR_GetExpandPath(L"~/$LOGNAME${TMPDIR}/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_StrPrint(str);

        status = AR_GetExpandPath(L"~/${TMPDIR}/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_StrPrint(str);
        
        
        status = AR_GetExpandPath(L"~///////tmp", str);
        AR_ASSERT(status == AR_S_YES);
        AR_StrPrint(str);
        
        AR_DestroyString(str);
        str = NULL;
}
        

static void startup_items_test()
{
        static const wchar_t *startup_paths[] = 
        {
                L"/Library/LaunchAgents/",
                L"/Library/LaunchDaemons/",
                L"/System/Library/LaunchAgents/",
                L"/System/Library/LaunchDaemons/",
                L"~/Library/LaunchAgents/",
                L"~/Library/LaunchDaemons/",
        };
        
        static const wchar_t *loginwindow_plist = L"~/Library/Preferences/loginwindow.plist";
        static const wchar_t *com_apple_loginitems_plist = L"~/Library/Preferences/com.apple.loginitems.plist";
        
        
        
        
        
}
        
        
        
void file_sys_test()
{
//        env_test();
        path_test();
        
        
//        startup_items_test();
}


        
        
        
        
        
        
#if defined(__cplusplus)
}
#endif
