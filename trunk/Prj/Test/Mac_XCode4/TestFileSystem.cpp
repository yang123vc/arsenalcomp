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
   

        
static void path_iter_test()
{
        arPathIter_t *it;
        arStatus_t status;
        it = AR_CreatePathIterator();
        AR_ASSERT(it != NULL);
        
        {
                status = AR_PathIteratorSetPath(it, L"~/Desktop");
                
                
                while(status == AR_S_YES && !AR_PathIteratorIsDone(it))
                {
                        AR_printf(L"%ls%ls\r\n", AR_PathIteratorPath(it), AR_PathIteratorCurrent(it));
                        status = AR_PathIteratorNext(it);
                }
        }
        
        
        {
                status = AR_PathIteratorSetPath(it, L"/Applications/App Store.app/Contents/");
                
                
                while(status == AR_S_YES && !AR_PathIteratorIsDone(it))
                {
                        AR_printf(L"%ls%ls\r\n", AR_PathIteratorPath(it), AR_PathIteratorCurrent(it));
                        status = AR_PathIteratorNext(it);
                }
               
        }
        
        
        {
                status = AR_PathIteratorSetPath(it, L"/Users/solidus/Desktop/2");
                
                
                while(status == AR_S_YES && !AR_PathIteratorIsDone(it))
                {
                        AR_printf(L"%ls%ls\r\n", AR_PathIteratorPath(it), AR_PathIteratorCurrent(it));
                        status = AR_PathIteratorNext(it);
                }
                
        }
        
        
        AR_DestroyPathIterator(it);
        it = NULL;
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

        
static void full_path_test()
{
        arString_t *str = AR_CreateString();
        
        arStatus_t status = AR_GetRealPath(L"/Library/Preferences/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_printf(L"%ls\r\n", AR_GetStringCString(str));
        

        status = AR_GetRealPath(L"./Library/Preferences/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_printf(L"%ls\r\n", AR_GetStringCString(str));
        
        
        status = AR_GetRealPath(L"../Library/Preferences/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_printf(L"%ls\r\n", AR_GetStringCString(str));
        
        
        status = AR_GetRealPath(L".././Library/Preferences/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_printf(L"%ls\r\n", AR_GetStringCString(str));
        
        
        status = AR_GetRealPath(L"../../Library/Preferences/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_printf(L"%ls\r\n", AR_GetStringCString(str));

        
        status = AR_GetRealPath(L"..../Library/Preferences/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_printf(L"%ls\r\n", AR_GetStringCString(str));
        
        
        status = AR_GetRealPath(L"./..../Library/Preferences/", str);
        AR_ASSERT(status == AR_S_YES);
        AR_printf(L"%ls\r\n", AR_GetStringCString(str));
        
        
        static const wchar_t *path_tbl[] = 
        {
                L"",
                L"/",
                L"/usr",
                L"/usr/",
                L"usr/",
                L"/usr/local",
                L"/usr/local/bin",
                L"//usr/local/bin/",
                L"/usr//local/bin/",
                L"/usr/local//bin/",
                L"/usr/local/bin//",
                L"/usr/local/./bin/",
                L"./usr/local/bin/",
                L"./usr/local/bin/./",
                L"./usr/local/bin/.",
                L"/usr/local//bin/",
                L"/usr/local/bin//",
                L"/usr/local/./bin/",
                L"./usr/local/bin/",
                L"./usr/local/bin/./",
                L"./usr/local/bin/.",
                L"/usr/local/lib/../bin/",
                L"/usr/local/lib/../bin/",
                L"/usr/local/lib/../../",
                L"/usr/local/lib/..",
                L"../usr/local/lib/",
                L"/usr/../lib/",
                L"/usr/../../lib/",
                L"local/../../lib/",
                L"a/b/c/d",
                
        };
        
        
        for(size_t i = 0; i < AR_NELEMS(path_tbl); ++i)
        {
                status = AR_GetRealPath(path_tbl[i], str);
                AR_ASSERT(status == AR_S_YES);
                AR_printf(L"%Iu : %ls\r\n", i, AR_GetStringCString(str));
        }
        
        AR_DestroyString(str);
        str = NULL;

                       
}
        
        
void file_sys_test()
{
//        env_test();
//        path_test();
      path_iter_test();
        
        
//        startup_items_test();
        
        full_path_test();
}


        
        
        
        
        
        
#if defined(__cplusplus)
}
#endif
