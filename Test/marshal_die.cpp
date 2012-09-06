
#include "Arsenal.h"


#if defined(__cplusplus)
using namespace ARSpace;
#endif


#include "marshal_die_predef.h"


/**********************************************************************************************************************/

#if(0)
static daemonTest_t t1, t2;

static void test1()
{
		AR_memset(&t1, 0, sizeof(t1));
		AR_memset(&t2, 0, sizeof(t2));

		AR_printf(L"sizeof daemonTest_t == %Iu KB\r\n", sizeof(t1) / 1024);
		
		AR_memset(t1.kp.x, 0xab, 1024);
		t1.kp.unused = 44;

		t1.test.t1 = 0x01;
		t1.test.t2 = 0x02;
		AR_wcscpy(t1.test.t3, L"中国字真麻烦！");

		snObject_t *obj = __put_daemonTest_t(&t1);

		__get_daemonTest_t(obj, &t2);

		AR_ASSERT(AR_memcmp(&t1, &t2, sizeof(t1)) == 0);

		SN_DestroyObject(obj);
		obj = NULL;
}


clisrvMessage_t		uni_type, uni_type_2;

static void test2()
{
		AR_memset(&t1, 0, sizeof(t1));
		AR_memset(&t2, 0, sizeof(t2));

		AR_printf(L"sizeof daemonTest_t == %Iu KB\r\n", sizeof(t1) / 1024);
		
		AR_memset(t1.kp.x, 0xab, 1024);
		t1.kp.unused = 44;

		t1.test.t1 = 0x01;
		t1.test.t2 = 0x02;
		AR_wcscpy(t1.test.t3, L"中国字真麻烦！");

		arBuffer_t		*buf = AR_CreateBuffer(128);

		uni_type.type = DAEMONTEST_T;
		uni_type.daemonTest_t = t1;

		arStatus_t status = clisrvMessage_t_Marshal(&uni_type, buf);
		
		status = clisrvMessage_t_UnMarshal(&uni_type_2, buf);

		AR_printf(L"cmp uni_type and uni_type_2 : %d\r\n", AR_memcmp(&uni_type, &uni_type_2, sizeof(uni_type_2)));

		AR_DestroyBuffer(buf);
		buf = NULL;
}

#endif

clisrvMessage_t		u1,u2;

static void test3()
{
		AR_memset(&u1, 0, sizeof(u1));
		AR_memset(&u2, 0, sizeof(u2));
		
		u1.type = BASICTEST_T;
		AR_wcscpy(u1.basicTest_t.w_arr, L"中国字真麻烦！");
		AR_strcpy(u1.basicTest_t.c_arr, "中国字真麻烦！");


		//snObject_t *obj = __put_basicTest_t(&u1.basicTest_t);

		
		arBuffer_t		*buf = AR_CreateBuffer(512);

		
		

		arStatus_t status = clisrvMessage_t_Marshal(&u1, buf);
		
		status = clisrvMessage_t_UnMarshal(&u2, buf);

		AR_printf(L"cmp u1 and u2 : %d\r\n", AR_memcmp(&u1, &u2, sizeof(u2)));

		AR_DestroyBuffer(buf);
		buf = NULL;
		

}


extern "C" void marshal_result_test()
{
		
		//test2();
		test3();

}





