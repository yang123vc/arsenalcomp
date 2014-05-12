//
//  test.h
//  Test
//
//  Created by solidus on 12-4-27.
//  Copyright (c) 2012年 none. All rights reserved.
//

#include "stdhdr.h"
#include "TestFileSystem.h"
#include "commonTest.h"
#include "plist_test.h"
#include "lex_test.h"

extern void Json_Test();

void test()
{
        //file_sys_test();
        common_test();
        //marshal_die_test();
        //plist_test();
        //lex_test();
        
        Json_Test();
        
}
