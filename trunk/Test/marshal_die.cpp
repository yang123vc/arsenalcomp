
#include "marshal_die_predef.h"































typedef struct {
		uint_32_t		unused;
		int_32_t		x[1024];
}daemonKeepalive_t;





typedef struct {
		uint_32_t		t1;
		uint_32_t		t2;
		wchar_t		t3[1024];
}anonymous_type_1;





typedef struct {
		anonymous_type_1		test;
		daemonKeepalive_t		kp;
		daemonKeepalive_t		kp_arr[1024];
}daemonTest_t;





typedef struct {
		uint_32_t		unused;
		daemonTest_t		unused2;
}daemonQueryCPUTemp_t;



































/******************************daemonKeepalive_t********************************/

static snObject_t*		__put_daemonKeepalive_t(daemonKeepalive_t *stu)
{
		snObject_t		*obj, *tmp;
		AR_ASSERT(stu != NULL);
		obj = NULL;
		tmp = NULL;

		obj = SN_CreateObject(SN_DICT_T);
		if(obj == NULL)
		{
				goto INVALID_POINT;
		}
/************************************************************************************************/
		if(__put_uint_32_t_to_dict(obj, L"unused", stu->unused) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_int_32_t_array_to_dict(obj, L"x", stu->x, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



/************************************************************************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}


static snObject_t*		__put_daemonKeepalive_t_array(daemonKeepalive_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);

		stu_list = SN_CreateObject(SN_LIST_T);

		for(i = 0; i < arr_size; ++i)
		{

				/*************************************************************************/
				snObject_t *tmp = __put_daemonKeepalive_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/


				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						goto INVALID_POINT;
				}
		}

		return stu_list;
INVALID_POINT:

		if(stu_list)
		{
				SN_DestroyObject(stu_list);
				stu_list = NULL;
		}

		return stu_list;
}



static bool_t	__get_daemonKeepalive_t(snObject_t *obj, daemonKeepalive_t *stu)
{
		bool_t	is_ok;
		snObject_t *tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;

		/***************************************************************************/
				if(__get_uint_32_t_from_dict(obj, L"unused", &stu->unused) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_int_32_t_array_from_dict(obj, L"x", stu->x, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



		/***************************************************************************/

		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static bool_t	__get_daemonKeepalive_t_array(snObject_t *obj, daemonKeepalive_t *stu, size_t arr_size)
{
		bool_t	is_ok;
		size_t list_cnt;
		size_t i;
		AR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);

		is_ok = true;

		if(SN_GetObjectType(obj) != SN_LIST_T)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		list_cnt = SN_GetListObjectCount(obj);

		if(list_cnt != arr_size)
		{
				goto INVALID_POINT;
		}


		for(i = 0; i < list_cnt; ++i)
		{
				snObject_t *item = SN_GetFromListObject(obj, i);
				if(item == NULL || SN_GetObjectType(item) != SN_DICT_T)
				{
						goto INVALID_POINT;
				}
				
				/********************************************************************/
				if(!__get_daemonKeepalive_t(item, &stu[i]))
				{
						is_ok = false;
						goto INVALID_POINT;
				}
				/********************************************************************/
		}
		return true;

INVALID_POINT:
		is_ok = false;
		return is_ok;
}








/******************************anonymous_type_1********************************/

static snObject_t*		__put_anonymous_type_1(anonymous_type_1 *stu)
{
		snObject_t		*obj, *tmp;
		AR_ASSERT(stu != NULL);
		obj = NULL;
		tmp = NULL;

		obj = SN_CreateObject(SN_DICT_T);
		if(obj == NULL)
		{
				goto INVALID_POINT;
		}
/************************************************************************************************/
		if(__put_uint_32_t_to_dict(obj, L"t1", stu->t1) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_uint_32_t_to_dict(obj, L"t2", stu->t2) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_wchar_t_array_to_dict(obj, L"t3", stu->t3) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}




/************************************************************************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}


static snObject_t*		__put_anonymous_type_1_array(anonymous_type_1 *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);

		stu_list = SN_CreateObject(SN_LIST_T);

		for(i = 0; i < arr_size; ++i)
		{

				/*************************************************************************/
				snObject_t *tmp = __put_anonymous_type_1(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/


				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						goto INVALID_POINT;
				}
		}

		return stu_list;
INVALID_POINT:

		if(stu_list)
		{
				SN_DestroyObject(stu_list);
				stu_list = NULL;
		}

		return stu_list;
}



static bool_t	__get_anonymous_type_1(snObject_t *obj, anonymous_type_1 *stu)
{
		bool_t	is_ok;
		snObject_t *tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;

		/***************************************************************************/
				if(__get_uint_32_t_from_dict(obj, L"t1", &stu->t1) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_uint_32_t_from_dict(obj, L"t2", &stu->t2) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_wchar_t_array_from_dict(obj, L"t3", stu->t3, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



		/***************************************************************************/

		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static bool_t	__get_anonymous_type_1_array(snObject_t *obj, anonymous_type_1 *stu, size_t arr_size)
{
		bool_t	is_ok;
		size_t list_cnt;
		size_t i;
		AR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);

		is_ok = true;

		if(SN_GetObjectType(obj) != SN_LIST_T)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		list_cnt = SN_GetListObjectCount(obj);

		if(list_cnt != arr_size)
		{
				goto INVALID_POINT;
		}


		for(i = 0; i < list_cnt; ++i)
		{
				snObject_t *item = SN_GetFromListObject(obj, i);
				if(item == NULL || SN_GetObjectType(item) != SN_DICT_T)
				{
						goto INVALID_POINT;
				}
				
				/********************************************************************/
				if(!__get_anonymous_type_1(item, &stu[i]))
				{
						is_ok = false;
						goto INVALID_POINT;
				}
				/********************************************************************/
		}
		return true;

INVALID_POINT:
		is_ok = false;
		return is_ok;
}








/******************************daemonTest_t********************************/

static snObject_t*		__put_daemonTest_t(daemonTest_t *stu)
{
		snObject_t		*obj, *tmp;
		AR_ASSERT(stu != NULL);
		obj = NULL;
		tmp = NULL;

		obj = SN_CreateObject(SN_DICT_T);
		if(obj == NULL)
		{
				goto INVALID_POINT;
		}
/************************************************************************************************/
		tmp = __put_anonymous_type_1(&stu->test);
		if(tmp == NULL)
		{
				goto INVALID_POINT;
		}


		if(SN_InsertToDictObjectByWcsObject(obj, L"test", tmp) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



		tmp = __put_daemonKeepalive_t(&stu->kp);
		if(tmp == NULL)
		{
				goto INVALID_POINT;
		}


		if(SN_InsertToDictObjectByWcsObject(obj, L"kp", tmp) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



		tmp = __put_daemonKeepalive_t_array(stu->kp_arr, 1024);
		if(tmp == NULL)
		{
				goto INVALID_POINT;
		}

		if(SN_InsertToDictObjectByWcsObject(obj, L"kp_arr", tmp) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



/************************************************************************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}


static snObject_t*		__put_daemonTest_t_array(daemonTest_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);

		stu_list = SN_CreateObject(SN_LIST_T);

		for(i = 0; i < arr_size; ++i)
		{

				/*************************************************************************/
				snObject_t *tmp = __put_daemonTest_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/


				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						goto INVALID_POINT;
				}
		}

		return stu_list;
INVALID_POINT:

		if(stu_list)
		{
				SN_DestroyObject(stu_list);
				stu_list = NULL;
		}

		return stu_list;
}



static bool_t	__get_daemonTest_t(snObject_t *obj, daemonTest_t *stu)
{
		bool_t	is_ok;
		snObject_t *tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;

		/***************************************************************************/
				tmp = SN_FindFromDictObjectByWcs(obj, L"test");
		if(tmp == NULL)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(!__get_anonymous_type_1(tmp, &stu->test))
		{
				is_ok = false;
				goto INVALID_POINT;
		}


		tmp = SN_FindFromDictObjectByWcs(obj, L"kp");
		if(tmp == NULL)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(!__get_daemonKeepalive_t(tmp, &stu->kp))
		{
				is_ok = false;
				goto INVALID_POINT;
		}


		tmp = SN_FindFromDictObjectByWcs(obj, L"kp_arr");
		if(tmp == NULL)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(!__get_daemonKeepalive_t_array(tmp, stu->kp_arr, 1024))
		{
				is_ok = false;
				goto INVALID_POINT;
		}



		/***************************************************************************/

		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static bool_t	__get_daemonTest_t_array(snObject_t *obj, daemonTest_t *stu, size_t arr_size)
{
		bool_t	is_ok;
		size_t list_cnt;
		size_t i;
		AR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);

		is_ok = true;

		if(SN_GetObjectType(obj) != SN_LIST_T)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		list_cnt = SN_GetListObjectCount(obj);

		if(list_cnt != arr_size)
		{
				goto INVALID_POINT;
		}


		for(i = 0; i < list_cnt; ++i)
		{
				snObject_t *item = SN_GetFromListObject(obj, i);
				if(item == NULL || SN_GetObjectType(item) != SN_DICT_T)
				{
						goto INVALID_POINT;
				}
				
				/********************************************************************/
				if(!__get_daemonTest_t(item, &stu[i]))
				{
						is_ok = false;
						goto INVALID_POINT;
				}
				/********************************************************************/
		}
		return true;

INVALID_POINT:
		is_ok = false;
		return is_ok;
}








/******************************daemonQueryCPUTemp_t********************************/

static snObject_t*		__put_daemonQueryCPUTemp_t(daemonQueryCPUTemp_t *stu)
{
		snObject_t		*obj, *tmp;
		AR_ASSERT(stu != NULL);
		obj = NULL;
		tmp = NULL;

		obj = SN_CreateObject(SN_DICT_T);
		if(obj == NULL)
		{
				goto INVALID_POINT;
		}
/************************************************************************************************/
		if(__put_uint_32_t_to_dict(obj, L"unused", stu->unused) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		tmp = __put_daemonTest_t(&stu->unused2);
		if(tmp == NULL)
		{
				goto INVALID_POINT;
		}


		if(SN_InsertToDictObjectByWcsObject(obj, L"unused2", tmp) != AR_S_YES)
		{
				goto INVALID_POINT;
		}




/************************************************************************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}


static snObject_t*		__put_daemonQueryCPUTemp_t_array(daemonQueryCPUTemp_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);

		stu_list = SN_CreateObject(SN_LIST_T);

		for(i = 0; i < arr_size; ++i)
		{

				/*************************************************************************/
				snObject_t *tmp = __put_daemonQueryCPUTemp_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/


				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						goto INVALID_POINT;
				}
		}

		return stu_list;
INVALID_POINT:

		if(stu_list)
		{
				SN_DestroyObject(stu_list);
				stu_list = NULL;
		}

		return stu_list;
}



static bool_t	__get_daemonQueryCPUTemp_t(snObject_t *obj, daemonQueryCPUTemp_t *stu)
{
		bool_t	is_ok;
		snObject_t *tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;

		/***************************************************************************/
				if(__get_uint_32_t_from_dict(obj, L"unused", &stu->unused) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		tmp = SN_FindFromDictObjectByWcs(obj, L"unused2");
		if(tmp == NULL)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(!__get_daemonTest_t(tmp, &stu->unused2))
		{
				is_ok = false;
				goto INVALID_POINT;
		}



		/***************************************************************************/

		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static bool_t	__get_daemonQueryCPUTemp_t_array(snObject_t *obj, daemonQueryCPUTemp_t *stu, size_t arr_size)
{
		bool_t	is_ok;
		size_t list_cnt;
		size_t i;
		AR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);

		is_ok = true;

		if(SN_GetObjectType(obj) != SN_LIST_T)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		list_cnt = SN_GetListObjectCount(obj);

		if(list_cnt != arr_size)
		{
				goto INVALID_POINT;
		}


		for(i = 0; i < list_cnt; ++i)
		{
				snObject_t *item = SN_GetFromListObject(obj, i);
				if(item == NULL || SN_GetObjectType(item) != SN_DICT_T)
				{
						goto INVALID_POINT;
				}
				
				/********************************************************************/
				if(!__get_daemonQueryCPUTemp_t(item, &stu[i]))
				{
						is_ok = false;
						goto INVALID_POINT;
				}
				/********************************************************************/
		}
		return true;

INVALID_POINT:
		is_ok = false;
		return is_ok;
}





enum {

		DAEMONKEEPALIVE_T,
		DAEMONTEST_T,
		DAEMONQUERYCPUTEMP_T,
};


typedef struct {
		int		type;
		daemonKeepalive_t		daemonKeepalive_t;
		daemonTest_t		daemonTest_t;
		daemonQueryCPUTemp_t		daemonQueryCPUTemp_t;
}uniType_t;


static arStatus_t		UnionType_Marshal(uniType_t *uni_type, arBuffer_t *out)
{
		arStatus_t		ar_status;
		snObject_t		*key, *val;
		snObject_t		*final_obj;
		AR_ASSERT(uni_type != NULL && out != NULL);
		ar_status = AR_S_YES;
		key = NULL;
		val	= NULL;
		final_obj = NULL;
		
		key = SN_CreateObject(SN_INT_T);
		if(key == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		SN_SetIntObject(key, uni_type->type);

		switch(uni_type->type)
		{
/**********************************************/
		case DAEMONKEEPALIVE_T :
				val = __put_daemonKeepalive_t(&uni_type->daemonKeepalive_t);
				break;
		case DAEMONTEST_T :
				val = __put_daemonTest_t(&uni_type->daemonTest_t);
				break;
		case DAEMONQUERYCPUTEMP_T :
				val = __put_daemonQueryCPUTemp_t(&uni_type->daemonQueryCPUTemp_t);
				break;
/**********************************************/
		default:
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}
		

		if(val == NULL)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}


		final_obj = SN_CreateObject(SN_LIST_T);

		if(final_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		ar_status = SN_InsertToListObject(final_obj, key);
		
		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		key = NULL;

		ar_status = SN_InsertToListObject(final_obj, val);
		
		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		val = NULL;

		ar_status = SN_PutObject(out, final_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:

		if(key)
		{
				SN_DestroyObject(key);
				key = NULL;
		}
		
		if(val)
		{
				SN_DestroyObject(val);
				val = NULL;
		}

		if(final_obj)
		{
				SN_DestroyObject(final_obj);
				final_obj = NULL;
		}

		return ar_status;
}






/**********************************************************************************************************************/


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


uniType_t		uni_type;

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

		arStatus_t status = UnionType_Marshal(&uni_type, buf);
		
		//AR_DestroyBuffer(buf);
		buf = NULL;
}

extern "C" void marshal_result_test()
{
		
		test2();

}





