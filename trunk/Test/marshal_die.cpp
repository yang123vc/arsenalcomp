
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




