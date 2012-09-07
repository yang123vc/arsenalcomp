//
//  marshal_die_test.c
//  Test
//
//  Created by Solidus on 12-8-28.
//  Copyright (c) 2012年 none. All rights reserved.
//


#include <map>
#include <vector>
#include <list>
#include <string>
#include <set>

#include "Arsenal.h"
using namespace ARSpace;

#include "marshal_die_predef.h"




clisrvMessage_t		u1,u2;

static void test3()
{
		AR_memset(&u1, 0, sizeof(u1));
		AR_memset(&u2, 0, sizeof(u2));
		
		u1.type = BASICTEST_T;
		AR_wcscpy(u1.basicTest_t_val.w_arr, L"中国字真麻烦！");
		AR_strcpy(u1.basicTest_t_val.c_arr, "中国字真麻烦！");
		
        
		//snObject_t *obj = __put_basicTest_t(&u1.basicTest_t);
        
		
		arBuffer_t		*buf = AR_CreateBuffer(512);
        
		
		
        
		arStatus_t status = clisrvMessage_t_Marshal(&u1, buf);
		
		status = clisrvMessage_t_UnMarshal(&u2, buf);
        
		AR_printf(L"cmp u1 and u2 : %d\r\n", AR_memcmp(&u1.basicTest_t_val, &u2.basicTest_t_val, sizeof(u2.basicTest_t_val)));
        
		AR_DestroyBuffer(buf);
		buf = NULL;
		
        
}


extern "C" void marshal_die_test()
{
        test3();
}




