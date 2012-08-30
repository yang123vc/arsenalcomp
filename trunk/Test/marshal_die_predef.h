
#include "Arsenal.h"


#if defined(__cplusplus)
using namespace ARSpace;
#endif




static arStatus_t		__put_INT8_T_to_dict(snObject_t *obj, const wchar_t *field_name, int_8_t v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T && AR_wcslen(field_name) > 0);

		ar_status = AR_S_YES;

		ar_status = SN_InsertToDictObjectByWcsInt(obj, field_name, (int_64_t)v);
		
		return ar_status;
}



static arStatus_t		__put_INT8_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, int_8_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*int_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		AR_ASSERT(arr_size > 0);

		ar_status = AR_S_YES;
		int_arr_obj = NULL;

		int_arr_obj = SN_CreateObject(SN_LIST_T);
		if(int_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByInt(int_arr_obj, (int_64_t)arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, int_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(int_arr_obj != NULL)
				{
						SN_DestroyObject(int_arr_obj);
						int_arr_obj = NULL;
				}
		}

		return ar_status;

}



static arStatus_t		__put_UINT8_T_to_dict(snObject_t *obj, const wchar_t *field_name, uint_8_t v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T && AR_wcslen(field_name) > 0);

		ar_status = AR_S_YES;

		ar_status = SN_InsertToDictObjectByWcsUInt(obj, field_name, (uint_64_t)v);
		
		return ar_status;
}



static arStatus_t		__put_UINT8_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, uint_8_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*int_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		AR_ASSERT(arr_size > 0);

		ar_status = AR_S_YES;
		int_arr_obj = NULL;

		int_arr_obj = SN_CreateObject(SN_LIST_T);
		if(int_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByUInt(int_arr_obj, (uint_64_t)arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, int_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(int_arr_obj != NULL)
				{
						SN_DestroyObject(int_arr_obj);
						int_arr_obj = NULL;
				}
		}

		return ar_status;

}



static arStatus_t		__put_INT16_T_to_dict(snObject_t *obj, const wchar_t *field_name, int_16_t v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T && AR_wcslen(field_name) > 0);

		ar_status = AR_S_YES;

		ar_status = SN_InsertToDictObjectByWcsInt(obj, field_name, (int_64_t)v);
		
		return ar_status;
}



static arStatus_t		__put_INT16_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, int_16_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*int_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		AR_ASSERT(arr_size > 0);

		ar_status = AR_S_YES;
		int_arr_obj = NULL;

		int_arr_obj = SN_CreateObject(SN_LIST_T);
		if(int_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByInt(int_arr_obj, (int_64_t)arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, int_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(int_arr_obj != NULL)
				{
						SN_DestroyObject(int_arr_obj);
						int_arr_obj = NULL;
				}
		}

		return ar_status;

}



static arStatus_t		__put_UINT16_T_to_dict(snObject_t *obj, const wchar_t *field_name, uint_16_t v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T && AR_wcslen(field_name) > 0);

		ar_status = AR_S_YES;

		ar_status = SN_InsertToDictObjectByWcsUInt(obj, field_name, (uint_64_t)v);
		
		return ar_status;
}



static arStatus_t		__put_UINT16_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, uint_16_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*int_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		AR_ASSERT(arr_size > 0);

		ar_status = AR_S_YES;
		int_arr_obj = NULL;

		int_arr_obj = SN_CreateObject(SN_LIST_T);
		if(int_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByUInt(int_arr_obj, (uint_64_t)arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, int_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(int_arr_obj != NULL)
				{
						SN_DestroyObject(int_arr_obj);
						int_arr_obj = NULL;
				}
		}

		return ar_status;

}




static arStatus_t		__put_INT32_T_to_dict(snObject_t *obj, const wchar_t *field_name, int_32_t v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T && AR_wcslen(field_name) > 0);

		ar_status = AR_S_YES;

		ar_status = SN_InsertToDictObjectByWcsInt(obj, field_name, (int_64_t)v);
		
		return ar_status;
}



static arStatus_t		__put_INT32_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, int_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*int_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		AR_ASSERT(arr_size > 0);

		ar_status = AR_S_YES;
		int_arr_obj = NULL;

		int_arr_obj = SN_CreateObject(SN_LIST_T);
		if(int_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByInt(int_arr_obj, (int_64_t)arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, int_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(int_arr_obj != NULL)
				{
						SN_DestroyObject(int_arr_obj);
						int_arr_obj = NULL;
				}
		}

		return ar_status;

}



static arStatus_t		__put_UINT32_T_to_dict(snObject_t *obj, const wchar_t *field_name, uint_32_t v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T && AR_wcslen(field_name) > 0);

		ar_status = AR_S_YES;

		ar_status = SN_InsertToDictObjectByWcsUInt(obj, field_name, (uint_64_t)v);
		
		return ar_status;
}



static arStatus_t		__put_UINT32_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, uint_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*int_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		AR_ASSERT(arr_size > 0);

		ar_status = AR_S_YES;
		int_arr_obj = NULL;

		int_arr_obj = SN_CreateObject(SN_LIST_T);
		if(int_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByUInt(int_arr_obj, (uint_64_t)arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, int_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(int_arr_obj != NULL)
				{
						SN_DestroyObject(int_arr_obj);
						int_arr_obj = NULL;
				}
		}

		return ar_status;
}




static arStatus_t		__put_INT64_T_to_dict(snObject_t *obj, const wchar_t *field_name, int_64_t v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T && AR_wcslen(field_name) > 0);

		ar_status = AR_S_YES;

		ar_status = SN_InsertToDictObjectByWcsInt(obj, field_name, (int_64_t)v);
		
		return ar_status;
}



static arStatus_t		__put_INT64_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, int_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*int_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		AR_ASSERT(arr_size > 0);

		ar_status = AR_S_YES;
		int_arr_obj = NULL;

		int_arr_obj = SN_CreateObject(SN_LIST_T);
		if(int_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByInt(int_arr_obj, (int_64_t)arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, int_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(int_arr_obj != NULL)
				{
						SN_DestroyObject(int_arr_obj);
						int_arr_obj = NULL;
				}
		}

		return ar_status;

}



static arStatus_t		__put_UINT64_T_to_dict(snObject_t *obj, const wchar_t *field_name, uint_64_t v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T && AR_wcslen(field_name) > 0);

		ar_status = AR_S_YES;

		ar_status = SN_InsertToDictObjectByWcsUInt(obj, field_name, (uint_64_t)v);
		
		return ar_status;
}



static arStatus_t		__put_UINT64_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, uint_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*int_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		AR_ASSERT(arr_size > 0);

		ar_status = AR_S_YES;
		int_arr_obj = NULL;

		int_arr_obj = SN_CreateObject(SN_LIST_T);
		if(int_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByUInt(int_arr_obj, (uint_64_t)arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, int_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(int_arr_obj != NULL)
				{
						SN_DestroyObject(int_arr_obj);
						int_arr_obj = NULL;
				}
		}

		return ar_status;

}




static arStatus_t		__put_BYTE_T_to_dict(snObject_t *obj, const wchar_t *field_name, byte_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_UINT8_T_to_dict(obj, field_name, (uint_8_t)v);
}

static arStatus_t		__put_BYTE_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, const byte_t *arr, size_t arr_size)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_UINT8_T_array_to_dict(obj, field_name, (uint_8_t*)arr, arr_size);
}


static arStatus_t		__put_CHAR_T_to_dict(snObject_t *obj, const wchar_t *field_name, char v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_UINT8_T_to_dict(obj, field_name, (uint_8_t)v);
}


static arStatus_t		__put_WCHAR_T_to_dict(snObject_t *obj, const wchar_t *field_name, wchar_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_UINT32_T_to_dict(obj, field_name, (uint_32_t)v);
}


#if(0)
static arStatus_t		__put_CHAR_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, const char *v_str)
{
		arStatus_t		ar_status;
		snObject_t *str_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && v_str != NULL && AR_strlen(v_str) > 0);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		
		ar_status = AR_S_YES;
		str_obj = NULL;

		str_obj = SN_CreateObject(SN_STRING_T);

		if(str_obj == NULL)
		{

		}


END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(str_obj)
				{
						SN_DestroyObject(str_obj);
						str_obj = NULL;
				}
		}
		return ar_status;
}
#endif





/**********************************************************************get*********************************************************************/






static arStatus_t		__get_INT8_T_from_dict(snObject_t *obj, const wchar_t *field_name, int_8_t *pv)
{
		arStatus_t		ar_status;
		snObject_t		*int_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		int_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		if(int_obj == NULL)
		{
				ar_status = AR_E_NOTFOUND;
				goto END_POINT;
		}

		if(SN_GetObjectType(int_obj) != SN_INT_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		*pv = (int_8_t)SN_GetIntObject(int_obj);

END_POINT:
		return ar_status;
}



static arStatus_t		__get_INT8_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, int_8_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*arr_list;
		size_t			i;
		size_t			list_cnt;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		ar_status = AR_S_YES;

		arr_list = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(SN_GetObjectType(arr_list) != SN_LIST_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		list_cnt = SN_GetListObjectCount(arr_list);

		if(list_cnt != arr_size)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		for(i = 0; i < SN_GetListObjectCount(arr_list); ++i)
		{
				snObject_t *int_obj = SN_GetFromListObject(arr_list, i);
				if(int_obj == NULL || SN_GetObjectType(int_obj) != SN_INT_T)
				{
						ar_status = AR_E_NOTMATCHED;
						goto END_POINT;
				}
				arr[i] = (int_8_t)SN_GetIntObject(int_obj);
		}
END_POINT:
		return ar_status;
}




static arStatus_t		__get_UINT8_T_from_dict(snObject_t *obj, const wchar_t *field_name, uint_8_t *pv)
{
		arStatus_t		ar_status;
		snObject_t		*int_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		int_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		if(int_obj == NULL)
		{
				ar_status = AR_E_NOTFOUND;
				goto END_POINT;
		}

		if(SN_GetObjectType(int_obj) != SN_INT_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		*pv = (int_8_t)SN_GetUIntObject(int_obj);

END_POINT:
		return ar_status;
}



static arStatus_t		__get_UINT8_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, uint_8_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*arr_list;
		size_t			i;
		size_t			list_cnt;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		ar_status = AR_S_YES;

		arr_list = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(SN_GetObjectType(arr_list) != SN_LIST_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		list_cnt = SN_GetListObjectCount(arr_list);

		if(list_cnt != arr_size)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		for(i = 0; i < SN_GetListObjectCount(arr_list); ++i)
		{
				snObject_t *int_obj = SN_GetFromListObject(arr_list, i);
				if(int_obj == NULL || SN_GetObjectType(int_obj) != SN_INT_T)
				{
						ar_status = AR_E_NOTMATCHED;
						goto END_POINT;
				}
				arr[i] = (uint_8_t)SN_GetUIntObject(int_obj);
		}
END_POINT:
		return ar_status;
}





static arStatus_t		__get_INT32_T_from_dict(snObject_t *obj, const wchar_t *field_name, int_32_t *pv)
{
		arStatus_t		ar_status;
		snObject_t		*int_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		int_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		if(int_obj == NULL)
		{
				ar_status = AR_E_NOTFOUND;
				goto END_POINT;
		}

		if(SN_GetObjectType(int_obj) != SN_INT_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		*pv = (int_32_t)SN_GetIntObject(int_obj);

END_POINT:
		return ar_status;
}



static arStatus_t		__get_INT32_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, int_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*arr_list;
		size_t			i;
		size_t			list_cnt;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		ar_status = AR_S_YES;

		arr_list = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(SN_GetObjectType(arr_list) != SN_LIST_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		list_cnt = SN_GetListObjectCount(arr_list);

		if(list_cnt != arr_size)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		for(i = 0; i < SN_GetListObjectCount(arr_list); ++i)
		{
				snObject_t *int_obj = SN_GetFromListObject(arr_list, i);
				if(int_obj == NULL || SN_GetObjectType(int_obj) != SN_INT_T)
				{
						ar_status = AR_E_NOTMATCHED;
						goto END_POINT;
				}
				arr[i] = (int_32_t)SN_GetIntObject(int_obj);
		}
END_POINT:
		return ar_status;
}




static arStatus_t		__get_UINT32_T_from_dict(snObject_t *obj, const wchar_t *field_name, uint_32_t *pv)
{
		arStatus_t		ar_status;
		snObject_t		*int_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		int_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		if(int_obj == NULL)
		{
				ar_status = AR_E_NOTFOUND;
				goto END_POINT;
		}

		if(SN_GetObjectType(int_obj) != SN_INT_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		*pv = (int_32_t)SN_GetUIntObject(int_obj);

END_POINT:
		return ar_status;
}



static arStatus_t		__get_UINT32_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, uint_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*arr_list;
		size_t			i;
		size_t			list_cnt;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		ar_status = AR_S_YES;

		arr_list = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(SN_GetObjectType(arr_list) != SN_LIST_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		list_cnt = SN_GetListObjectCount(arr_list);

		if(list_cnt != arr_size)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		for(i = 0; i < SN_GetListObjectCount(arr_list); ++i)
		{
				snObject_t *int_obj = SN_GetFromListObject(arr_list, i);
				if(int_obj == NULL || SN_GetObjectType(int_obj) != SN_INT_T)
				{
						ar_status = AR_E_NOTMATCHED;
						goto END_POINT;
				}
				arr[i] = (uint_32_t)SN_GetUIntObject(int_obj);
		}
END_POINT:
		return ar_status;
}



static arStatus_t		__get_INT64_T_from_dict(snObject_t *obj, const wchar_t *field_name, int_64_t *pv)
{
		arStatus_t		ar_status;
		snObject_t		*int_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		int_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		if(int_obj == NULL)
		{
				ar_status = AR_E_NOTFOUND;
				goto END_POINT;
		}

		if(SN_GetObjectType(int_obj) != SN_INT_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		*pv = (int_64_t)SN_GetIntObject(int_obj);

END_POINT:
		return ar_status;
}



static arStatus_t		__get_INT64_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, int_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*arr_list;
		size_t			i;
		size_t			list_cnt;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		ar_status = AR_S_YES;

		arr_list = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(SN_GetObjectType(arr_list) != SN_LIST_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		list_cnt = SN_GetListObjectCount(arr_list);

		if(list_cnt != arr_size)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		for(i = 0; i < SN_GetListObjectCount(arr_list); ++i)
		{
				snObject_t *int_obj = SN_GetFromListObject(arr_list, i);
				if(int_obj == NULL || SN_GetObjectType(int_obj) != SN_INT_T)
				{
						ar_status = AR_E_NOTMATCHED;
						goto END_POINT;
				}
				arr[i] = (int_64_t)SN_GetIntObject(int_obj);
		}
END_POINT:
		return ar_status;
}




static arStatus_t		__get_UINT64_T_from_dict(snObject_t *obj, const wchar_t *field_name, uint_64_t *pv)
{
		arStatus_t		ar_status;
		snObject_t		*int_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		int_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		if(int_obj == NULL)
		{
				ar_status = AR_E_NOTFOUND;
				goto END_POINT;
		}

		if(SN_GetObjectType(int_obj) != SN_INT_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		*pv = (int_64_t)SN_GetUIntObject(int_obj);

END_POINT:
		return ar_status;
}



static arStatus_t		__get_UINT64_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, uint_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*arr_list;
		size_t			i;
		size_t			list_cnt;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		ar_status = AR_S_YES;

		arr_list = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(SN_GetObjectType(arr_list) != SN_LIST_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		list_cnt = SN_GetListObjectCount(arr_list);

		if(list_cnt != arr_size)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		for(i = 0; i < SN_GetListObjectCount(arr_list); ++i)
		{
				snObject_t *int_obj = SN_GetFromListObject(arr_list, i);
				if(int_obj == NULL || SN_GetObjectType(int_obj) != SN_INT_T)
				{
						ar_status = AR_E_NOTMATCHED;
						goto END_POINT;
				}
				arr[i] = (uint_64_t)SN_GetUIntObject(int_obj);
		}
END_POINT:
		return ar_status;
}



static arStatus_t		__get_WCHAR_T_from_dict(snObject_t *obj, const wchar_t *field_name, wchar_t *pv)
{
		arStatus_t		ar_status;
		uint_32_t		v;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		
		ar_status = __get_UINT32_T_from_dict(obj, field_name, &v);

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}
		*pv = (wchar_t)v;
		
		return ar_status;
}


static arStatus_t		__get_CHAR_T_from_dict(snObject_t *obj, const wchar_t *field_name, char *pv)
{
		arStatus_t		ar_status;
		uint_8_t		v;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		ar_status = __get_UINT8_T_from_dict(obj, field_name, &v);

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = (char)v;
		
		return ar_status;
}


static arStatus_t		__get_BYTE_T_from_dict(snObject_t *obj, const wchar_t *field_name, byte_t *pv)
{
		arStatus_t		ar_status;
		uint_8_t		v;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);

		ar_status = __get_UINT8_T_from_dict(obj, field_name, &v);

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = (byte_t)v;
		return ar_status;
}

static arStatus_t		__get_BYTE_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, byte_t *arr, size_t arr_size)
{
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		return __get_UINT8_T_array_from_dict(obj, field_name, (uint_8_t*)arr, arr_size);
}
