/************************************************Macro*****************************************************/

#define __CHECK_ARG(_obj, _fn)	do{ if(SN_GetObjectType((_obj)) != SN_DICT_T || AR_wcslen((_fn)) == 0) {ar_status = AR_E_INVAL; goto END_POINT; }}while(0)


/************************************************ar_byte_t*****************************************************/

static arStatus_t		__put_byte_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_byte_t v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);

		ar_status = AR_S_YES;

		__CHECK_ARG(obj,field_name);
		
		ar_status = SN_InsertToDictObjectByWcsUInt(obj, field_name, (ar_uint_64_t)v);
		
END_POINT:
		return ar_status;
}


static arStatus_t		__put_byte_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, const ar_byte_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*data_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		data_obj = NULL;
		
		__CHECK_ARG(obj,field_name);

		data_obj = SN_CreateObject(SN_STRING_T);

		if(data_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		ar_status = SN_SetStringObjectByData(data_obj, arr, arr_size);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, data_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(data_obj)
				{
						SN_DestroyObject(data_obj);
						data_obj = NULL;
				}
		}
		return ar_status;
}



static arStatus_t		__get_byte_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_byte_t *pv)
{
		arStatus_t		ar_status;
		snObject_t		*int_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		int_obj = NULL;
		
		__CHECK_ARG(obj, field_name);

		int_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		if(int_obj == NULL || SN_GetObjectType(int_obj) != SN_INT_T)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}
		
		*pv = (ar_uint_8_t)SN_GetUIntObject(int_obj);

END_POINT:
		return ar_status;
}

static arStatus_t		__get_byte_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_byte_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*data_obj;
		ar_int_t			ret;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;

		__CHECK_ARG(obj, field_name);

		data_obj = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(data_obj == NULL || SN_GetObjectType(data_obj) != SN_STRING_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		ret = SN_GetDataFromStringObject(data_obj, arr, arr_size);

		if(ret < 0 || (size_t)ret != arr_size)
		{
				ar_status = AR_E_FAIL;
				goto END_POINT;
		}

END_POINT:
		return ar_status;
}




/**********************************************************ar_bool_t***************************************************/



static arStatus_t		__put_bool_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_bool_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);
}


/***********************/

static arStatus_t		__put_bool_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_bool_t *arr, size_t arr_size)
{
		return __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)arr, arr_size);
}





static arStatus_t		__get_bool_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_bool_t *pv)
{
		ar_byte_t v;
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		ar_status = __get_byte_t_from_dict(obj, field_name, &v);
		if(ar_status == AR_S_YES)
		{
				*pv = (ar_bool_t)v ? true : false;
		}
		return ar_status;

}


/*********/
static arStatus_t		__get_bool_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_bool_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);

		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, arr_size);

		if(ar_status == AR_S_YES)
		{
				for(i = 0; i < arr_size; ++i)
				{
						arr[i] = arr[i] ? true : false;
				}
		}

		return ar_status;
}




/**********************************************************char***************************************************/

static arStatus_t		__put_char_to_dict(snObject_t *obj, const wchar_t *field_name, char v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);
}


static arStatus_t		__put_char_array_to_dict(snObject_t *obj, const wchar_t *field_name, const char *v_str)
{
		arStatus_t		ar_status;
		snObject_t *str_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && v_str != NULL);
		
		
		ar_status = AR_S_YES;
		str_obj = NULL;

		__CHECK_ARG(obj, field_name);

		str_obj = SN_CreateObject(SN_STRING_T);

		if(str_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		ar_status = SN_SetStringObjectByStr(str_obj, v_str);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, str_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
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


static arStatus_t		__get_char_from_dict(snObject_t *obj, const wchar_t *field_name, char *pv)
{
		arStatus_t		ar_status;
		ar_byte_t			v;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		ar_status = __get_byte_t_from_dict(obj, field_name, &v);

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = (char)v;

END_POINT:
		return ar_status;
}




static arStatus_t		__get_char_array_from_dict(snObject_t *obj, const wchar_t *field_name, char *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*str_obj;
		ar_int_t			ret;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		str_obj = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(str_obj == NULL || SN_GetObjectType(str_obj) != SN_STRING_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}
		
		ret = SN_GetStrFromStringObject(str_obj, arr, arr_size);

		if(ret < 0)
		{
				ar_status = AR_E_FAIL;
				goto END_POINT;
		}

		arr[ret] = '\0';

END_POINT:
		return ar_status;
}







/**********************************************************wchar_t***************************************************/


static arStatus_t		__put_wchar_t_to_dict(snObject_t *obj, const wchar_t *field_name, wchar_t v)
{
		ar_uint_32_t t;
		AR_ASSERT(obj != NULL && field_name != NULL);

		t = (ar_uint_32_t)v;
		t = AR_LTON_32(t);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&t, sizeof(ar_uint_32_t));
}


static arStatus_t		__get_wchar_t_from_dict(snObject_t *obj, const wchar_t *field_name, wchar_t *pv)
{
		arStatus_t		ar_status;
		ar_uint_32_t		t;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)&t, sizeof(ar_uint_32_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		t = AR_NTOL_32(t);
		*pv = (wchar_t)t;

END_POINT:
		return ar_status;
}



static arStatus_t		__put_wchar_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, const wchar_t *v_str)
{
		arStatus_t		ar_status;
		snObject_t *str_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && v_str != NULL);
		
		ar_status = AR_S_YES;
		str_obj = NULL;

		__CHECK_ARG(obj, field_name);

		str_obj = SN_CreateObject(SN_STRING_T);

		if(str_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		ar_status = SN_SetStringObjectByWcs(str_obj, v_str);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, str_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
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






static arStatus_t		__get_wchar_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, wchar_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*str_obj;
		ar_int_t			ret;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);

		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		str_obj = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(str_obj == NULL || SN_GetObjectType(str_obj) != SN_STRING_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		ret = SN_GetWcsFromStringObject(str_obj, arr, arr_size);

		if(ret < 0)
		{
				ar_status = AR_E_FAIL;
				goto END_POINT;
		}

		arr[ret] = L'\0';

END_POINT:
		return ar_status;
}





/************************************************Integer 8*****************************************************/

static arStatus_t		__put_int_8_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);
}



static arStatus_t		__put_int_8_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t *arr, size_t arr_size)
{
		return __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)arr, arr_size);
}





static arStatus_t		__get_int_8_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t *pv)
{
		ar_byte_t v;
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		ar_status = __get_byte_t_from_dict(obj, field_name, &v);
		if(ar_status == AR_S_YES)
		{
				*pv = (ar_int_8_t)v;
		}
		return ar_status;

}



static arStatus_t		__get_int_8_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);

		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, arr_size);
		return ar_status;
}






static arStatus_t		__put_uint_8_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);
}



static arStatus_t		__put_uint_8_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t *arr, size_t arr_size)
{
		return __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)arr, arr_size);
}





static arStatus_t		__get_uint_8_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t *pv)
{
		ar_byte_t v;
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		ar_status = __get_byte_t_from_dict(obj, field_name, &v);
		if(ar_status == AR_S_YES)
		{
				*pv = (ar_uint_8_t)v;
		}
		return ar_status;

}



static arStatus_t		__get_uint_8_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);

		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, arr_size);
		return ar_status;
}






/************************************************Integer 16*****************************************************/

static arStatus_t		__put_int_16_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_16(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_int_16_t));
}


static arStatus_t		__get_int_16_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_int_16_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_16(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_int_16_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_int_16_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_int_16_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_16(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}

		return ar_status;
}


static arStatus_t		__get_int_16_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_16(arr[i]);
		}

END_POINT:
		
		return ar_status;
}






static arStatus_t		__put_uint_16_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_U16(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_uint_16_t));
}


static arStatus_t		__get_uint_16_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_uint_16_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_U16(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_uint_16_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_uint_16_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_uint_16_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_U16(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}
		
		return ar_status;
}


static arStatus_t		__get_uint_16_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_U16(arr[i]);
		}

END_POINT:
		
		return ar_status;
}









/************************************************Integer 32*****************************************************/

static arStatus_t		__put_int_32_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_32(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_int_32_t));
}


static arStatus_t		__get_int_32_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_int_32_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_32(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_int_32_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_int_32_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_int_32_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_32(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}

		return ar_status;
}


static arStatus_t		__get_int_32_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_32(arr[i]);
		}

END_POINT:
		
		return ar_status;
}






static arStatus_t		__put_uint_32_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_U32(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_uint_32_t));
}


static arStatus_t		__get_uint_32_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_uint_32_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_U32(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_uint_32_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_uint_32_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_uint_32_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_U32(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}
		
		return ar_status;
}


static arStatus_t		__get_uint_32_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_U32(arr[i]);
		}

END_POINT:
		
		return ar_status;
}








/************************************************Integer 64*****************************************************/

static arStatus_t		__put_int_64_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_64(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_int_64_t));
}


static arStatus_t		__get_int_64_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_int_64_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_64(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_int_64_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_int_64_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_int_64_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_64(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}

		return ar_status;
}


static arStatus_t		__get_int_64_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_64(arr[i]);
		}

END_POINT:
		
		return ar_status;
}






static arStatus_t		__put_uint_64_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_U64(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_uint_64_t));
}


static arStatus_t		__get_uint_64_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_uint_64_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_U64(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_uint_64_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_uint_64_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_uint_64_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_U64(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}
		
		return ar_status;
}


static arStatus_t		__get_uint_64_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_U64(arr[i]);
		}

END_POINT:
		
		return ar_status;
}






/******************************************************float****************************************************************/



static arStatus_t		__put_float_to_dict(snObject_t *obj, const wchar_t *field_name, float v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		

		ar_status = SN_InsertToDictObjectByWcsFloat(obj, field_name, (double)v);

END_POINT:

		return ar_status;
}

static arStatus_t		__get_float_from_dict(snObject_t *obj, const wchar_t *field_name, float *pv)
{
		arStatus_t		ar_status;
		snObject_t		*flt_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		flt_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		if(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}

		*pv = (float)SN_GetFloatObject(flt_obj);

END_POINT:
		return ar_status;
}



static arStatus_t		__put_float_array_to_dict(snObject_t *obj, const wchar_t *field_name, float *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*float_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(arr_size > 0);

		ar_status = AR_S_YES;
		float_arr_obj = NULL;
		
		__CHECK_ARG(obj, field_name);


		float_arr_obj = SN_CreateObject(SN_LIST_T);
		if(float_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByFloat(float_arr_obj, (double)arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, float_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(float_arr_obj != NULL)
				{
						SN_DestroyObject(float_arr_obj);
						float_arr_obj = NULL;
				}
		}

		return ar_status;

}

static arStatus_t		__get_float_array_from_dict(snObject_t *obj, const wchar_t *field_name, float *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*arr_list;
		size_t			i;
		size_t			list_cnt;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);

		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		arr_list = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(arr_list == NULL || SN_GetObjectType(arr_list) != SN_LIST_T)
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
				snObject_t *flt_obj = SN_GetFromListObject(arr_list, i);
				if(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)
				{
						ar_status = AR_E_NOTMATCHED;
						goto END_POINT;
				}

				arr[i] = (float)SN_GetFloatObject(flt_obj);
		}
END_POINT:
		return ar_status;
}



/******************************************************double****************************************************************/



static arStatus_t		__put_double_to_dict(snObject_t *obj, const wchar_t *field_name, double v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);

		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		ar_status = SN_InsertToDictObjectByWcsFloat(obj, field_name, v);

END_POINT:
		return ar_status;
}



static arStatus_t		__put_double_array_to_dict(snObject_t *obj, const wchar_t *field_name, double *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*float_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		

		ar_status = AR_S_YES;
		float_arr_obj = NULL;

		__CHECK_ARG(obj, field_name);

		float_arr_obj = SN_CreateObject(SN_LIST_T);
		if(float_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByFloat(float_arr_obj, arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, float_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(float_arr_obj != NULL)
				{
						SN_DestroyObject(float_arr_obj);
						float_arr_obj = NULL;
				}
		}

		return ar_status;

}







static arStatus_t		__get_double_from_dict(snObject_t *obj, const wchar_t *field_name, double *pv)
{
		arStatus_t		ar_status;
		snObject_t		*flt_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		flt_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		
		if(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		*pv = SN_GetFloatObject(flt_obj);

END_POINT:
		return ar_status;
}



static arStatus_t		__get_double_array_from_dict(snObject_t *obj, const wchar_t *field_name, double *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*arr_list;
		size_t			i;
		size_t			list_cnt;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		

		ar_status = AR_S_YES;

		__CHECK_ARG(obj, field_name);

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
				snObject_t *flt_obj = SN_GetFromListObject(arr_list, i);
				if(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)
				{
						ar_status = AR_E_NOTMATCHED;
						goto END_POINT;
				}

				arr[i] = SN_GetFloatObject(flt_obj);
		}
END_POINT:
		return ar_status;
}







