

#include "generate_type.h"
#include "predef_code.h"

using namespace std;

static std::wstring generate_for_head_code()
{
		std::wstring ret;

		for(size_t i = 0; i < g_head_code.size(); ++i)
		{
				ret += g_head_code[i];
				ret += L"\n\n";
		}

		return ret;
}


static std::wstring generate_for_tail_code()
{
		std::wstring ret;

		for(size_t i = 0; i < g_tail_code.size(); ++i)
		{
				ret += g_head_code[i];
				ret += L"\n\n";
		}

		return ret;
}

static std::wstring generate_for_inner_type()
{
		std::wstring ret;

		ret = 
		code_for_macro + L"\n\n" + 
		code_for_byte + L"\n\n" + 
		code_for_bool + L"\n\n" + 
		code_for_char + L"\n\n" + 
		code_for_wchar + L"\n\n" + 
		code_for_integer_8 + L"\n\n" + 
		code_for_integer_16 + L"\n\n" + 
		code_for_integer_32 + L"\n\n" + 
		code_for_integer_64 + L"\n\n" + 
		code_for_float + L"\n\n" + 
		code_for_double + L"\n\n" + 
		L"\n\n\n\n";

		return ret;
}



/****************************************************************************PUT**************************************************************************/

#define PUT_FIELD_FMT			L"\t\tif(%ls(obj, L\"%ls\", stu->%ls) != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\t\t\r\n\t\t}\r\n\r\n"
#define PUT_FIELD_ARRAY_FMT		L"\t\tif(%ls(obj, L\"%ls\", stu->%ls, %Iu) != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\t\t\r\n\t\t}\r\n"
#define PUT_FIELD_STR_FMT		PUT_FIELD_FMT

#define PUT_FIELD_CUSTOM_TYPE_FMT	L"\t\ttmp = __put_%ls(&stu->%ls);\r\n\t\tif(tmp == NULL)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n\r\n\r\n\t\tif(SN_InsertToDictObjectByWcsObject(obj, L\"%ls\", tmp) != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n\r\n"
#define PUT_FIELD_CUSTOM_TYPE_ARRAY_FMT L"\t\ttmp = __put_%ls_array(stu->%ls, %Iu);\r\n\t\tif(tmp == NULL)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n\r\n\t\tif(SN_InsertToDictObjectByWcsObject(obj, L\"%ls\", tmp) != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n"


static std::wstring generate_func_call_for_put_field(const Field_t *field)
{
		std::wstring ret;
		arString_t *str = AR_CreateString();
		const Type_t *field_type = field->type;

		if(is_inner_type(field_type->type))
		{
				if(field->is_array)
				{
						if(field_type->type == WCHAR_T || field_type->type == CHAR_T)
						{
								AR_FormatString(str, PUT_FIELD_STR_FMT, __g_inner_type_marshal_array_call_name[field_type->type], field->name.c_str(), field->name.c_str());
						}else
						{
								AR_FormatString(str, PUT_FIELD_ARRAY_FMT, __g_inner_type_marshal_array_call_name[field_type->type], field->name.c_str(), field->name.c_str(), field->array_size);
						}
				}else
				{
						AR_FormatString(str, PUT_FIELD_FMT, __g_inner_type_marshal_call_name[field_type->type], field->name.c_str(), field->name.c_str());
				}
		}else
		{
				if(field->is_array)
				{
						AR_FormatString(str, PUT_FIELD_CUSTOM_TYPE_ARRAY_FMT, field_type->name.c_str(), field->name.c_str(), field->array_size, field->name.c_str(),field->array_size);
				}else
				{
						AR_FormatString(str, PUT_FIELD_CUSTOM_TYPE_FMT, field_type->name.c_str(), field->name.c_str(), field->name.c_str());
				}
		}

		AR_AppendString(str, L"\n\n");

		ret = AR_GetStringCString(str);
		AR_DestroyString(str);
		str = NULL;
		return ret;
}


#define PUT_FUNC_FMT	L"static snObject_t*\t\t__put_%ls(%ls *stu)\r\n{\r\n\t\tsnObject_t\t\t*obj, *tmp;\r\n\t\tAR_ASSERT(stu != NULL);\r\n\t\tobj = NULL;\r\n\t\ttmp = NULL;\r\n\r\n\t\tobj = SN_CreateObject(SN_DICT_T);\r\n\t\tif(obj == NULL)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n/************************************************************************************************/\r\n%ls\r\n/************************************************************************************************/\r\n\t\treturn obj;\r\nINVALID_POINT:\r\n\t\tif(obj)\r\n\t\t{\r\n\t\t\t\tSN_DestroyObject(obj);\r\n\t\t\t\tobj = NULL;\r\n\t\t}\r\n\t\treturn NULL;\r\n}"




static std::wstring generate_for_put_custom_type(const Type_t *type)
{
		std::wstring ret;
		std::wstring field_call;

		arString_t *str = AR_CreateString();
		
		AR_ASSERT(type != NULL && !is_inner_type(type->type));

		for(size_t i = 0; i < type->fields.size(); ++i)
		{
				const Field_t *field = type->fields[i];
				ret += generate_func_call_for_put_field(field);
		}

		AR_FormatString(str, PUT_FUNC_FMT, type->name.c_str(), type->name.c_str(), ret.c_str());
		
		ret = AR_GetStringCString(str);

		//AR_printf(L"%ls\r\n", ret.c_str());
		return ret;
}

#define PUT_ARRAY_FUNC_FMT	L"\r\nstatic snObject_t*\t\t__put_%ls_array(%ls *stu, size_t arr_size)\r\n{\r\n\t\tsnObject_t\t\t*stu_list;\r\n\t\tsize_t i;\r\n\t\tAR_ASSERT(stu != NULL && arr_size > 0);\r\n\r\n\t\tstu_list = SN_CreateObject(SN_LIST_T);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\r\n\t\t\t\t/*************************************************************************/\r\n\t\t\t\tsnObject_t *tmp = __put_%ls(&stu[i]);\r\n\t\t\t\tif(tmp == NULL)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tgoto INVALID_POINT;\r\n\t\t\t\t}\r\n\t\t\t\t/*************************************************************************/\r\n\r\n\r\n\t\t\t\tif(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(tmp);\r\n\t\t\t\t\t\ttmp = NULL;\r\n\t\t\t\t\t\tgoto INVALID_POINT;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\treturn stu_list;\r\nINVALID_POINT:\r\n\r\n\t\tif(stu_list)\r\n\t\t{\r\n\t\t\t\tSN_DestroyObject(stu_list);\r\n\t\t\t\tstu_list = NULL;\r\n\t\t}\r\n\r\n\t\treturn stu_list;\r\n}\r\n\r\n"

static std::wstring generate_for_put_custom_array_type(const Type_t *type)
{
		std::wstring ret;
		std::wstring field_call;

		arString_t *str = AR_CreateString();
		AR_ASSERT(type != NULL && !is_inner_type(type->type));


		AR_FormatString(str, PUT_ARRAY_FUNC_FMT, type->name.c_str(), type->name.c_str(), type->name.c_str());
		
		ret = AR_GetStringCString(str);

		//AR_printf(L"%ls\r\n", ret.c_str());
		return ret;
}






/****************************************************************************GET**************************************************************************/



#define GET_FIELD_FMT			L"\t\tif(%ls(obj, L\"%ls\", &stu->%ls) != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n"
#define GET_FIELD_ARRAY_FMT		L"\t\tif(%ls(obj, L\"%ls\", stu->%ls, %Iu) != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n"

#define GET_FIELD_CUSTOM_FMT			L"\t\ttmp = SN_FindFromDictObjectByWcs(obj, L\"%ls\");\r\n\t\tif(tmp == NULL)\r\n\t\t{\r\n\t\t\t\tis_ok = false;\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n\r\n\t\tif(!__get_%ls(tmp, &stu->%ls))\r\n\t\t{\r\n\t\t\t\tis_ok = false;\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n"
#define GET_FIELD_ARRAY_CUSTOM_FMT		L"\t\ttmp = SN_FindFromDictObjectByWcs(obj, L\"%ls\");\r\n\t\tif(tmp == NULL)\r\n\t\t{\r\n\t\t\t\tis_ok = false;\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n\r\n\t\tif(!__get_%ls_array(tmp, stu->%ls, %Iu))\r\n\t\t{\r\n\t\t\t\tis_ok = false;\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n"


static std::wstring generate_func_call_for_get_field(const Field_t *field)
{
		std::wstring ret;
		arString_t *str = AR_CreateString();
		const Type_t *field_type = field->type;

		if(is_inner_type(field_type->type))
		{
				if(field->is_array)
				{
						AR_FormatString(str, GET_FIELD_ARRAY_FMT, __g_inner_type_unmarshal_array_call_name[field_type->type], field->name.c_str(), field->name.c_str(), field->array_size);
				}else
				{
						AR_FormatString(str, GET_FIELD_FMT, __g_inner_type_unmarshal_call_name[field_type->type], field->name.c_str(), field->name.c_str());
				}
		}else
		{
				if(field->is_array)
				{
						AR_FormatString(str, GET_FIELD_ARRAY_CUSTOM_FMT, field->name.c_str(), field_type->name.c_str(), field->name.c_str(), field->array_size);
				}else
				{
						AR_FormatString(str, GET_FIELD_CUSTOM_FMT, field->name.c_str(), field_type->name.c_str(), field->name.c_str());
				}
		}

		AR_AppendString(str, L"\n\n");

		ret = AR_GetStringCString(str);
		AR_DestroyString(str);
		str = NULL;
		return ret;
}

#define GET_FUNC_FMT			L"static bool_t\t__get_%ls(snObject_t *obj, %ls *stu)\r\n{\r\n\t\tbool_t\tis_ok;\r\n\t\tAR_ASSERT(stu != NULL && obj != NULL);\r\n\t\tAR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);\r\n\t\tis_ok = true;\r\n\r\n\t\t/***************************************************************************/\r\n%ls\r\n\t\t/***************************************************************************/\r\n\r\n\t\treturn true;\r\nINVALID_POINT:\r\n\t\tis_ok = false;\r\n\t\treturn is_ok;\r\n}"


static std::wstring generate_for_get_custom_type(const Type_t *type)
{
		std::wstring ret;
		std::wstring field_call;

		arString_t *str = AR_CreateString();
		
		AR_ASSERT(type != NULL && !is_inner_type(type->type));

		for(size_t i = 0; i < type->fields.size(); ++i)
		{
				const Field_t *field = type->fields[i];
				ret += generate_func_call_for_get_field(field);
		}

		AR_FormatString(str, GET_FUNC_FMT, type->name.c_str(), type->name.c_str(), ret.c_str());
		
		ret = AR_GetStringCString(str);

		//AR_printf(L"%ls\r\n", ret.c_str());
		return ret;
}




#define GET_ARRAY_FUNC_FMT	L"\r\nstatic bool_t\t__get_%ls_array(snObject_t *obj, %ls *stu, size_t arr_size)\r\n{\r\n\t\tbool_t\tis_ok;\r\n\t\tsize_t list_cnt;\r\n\t\tsize_t i;\r\n\t\tAR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);\r\n\r\n\t\tis_ok = true;\r\n\r\n\t\tif(SN_GetObjectType(obj) != SN_LIST_T)\r\n\t\t{\r\n\t\t\t\tis_ok = false;\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n\r\n\t\tlist_cnt = SN_GetListObjectCount(obj);\r\n\r\n\t\tif(list_cnt != arr_size)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n\r\n\r\n\t\tfor(i = 0; i < list_cnt; ++i)\r\n\t\t{\r\n\t\t\t\tsnObject_t *item = SN_GetFromListObject(obj, i);\r\n\t\t\t\tif(item == NULL || SN_GetObjectType(item) != SN_DICT_T)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tgoto INVALID_POINT;\r\n\t\t\t\t}\r\n\t\t\t\t\r\n\t\t\t\t/********************************************************************/\r\n\t\t\t\tif(!__get_%ls(item, &stu[i]))\r\n\t\t\t\t{\r\n\t\t\t\t\t\tis_ok = false;\r\n\t\t\t\t\t\tgoto INVALID_POINT;\r\n\t\t\t\t}\r\n\t\t\t\t/********************************************************************/\r\n\t\t}\r\n\t\treturn true;\r\n\r\nINVALID_POINT:\r\n\t\tis_ok = false;\r\n\t\treturn is_ok;\r\n}\r\n\n\n"

static std::wstring generate_for_get_custom_array_type(const Type_t *type)
{
		std::wstring ret;
		std::wstring field_call;

		arString_t *str = AR_CreateString();
		AR_ASSERT(type != NULL && !is_inner_type(type->type));


		AR_FormatString(str, GET_ARRAY_FUNC_FMT, type->name.c_str(), type->name.c_str(), type->name.c_str());
		
		ret = AR_GetStringCString(str);

		//AR_printf(L"%ls\r\n", ret.c_str());
		return ret;
}



/*********************************************************************************************************************************************************/


static std::wstring generate_for_put_get_type()
{
		std::wstring ret;

		for(size_t i = 0; i < g_type_list.size(); ++i)
		{
				ret += L"\n\n";

				const Type_t *type = g_type_list[i];
				AR_ASSERT(type != NULL);

				if(!is_inner_type(type->type))
				{
						ret += L"\r\n\r\n";
						ret += L"/******************************";
						ret += type->name;
						ret += L"********************************/";
						ret += L"\r\n\r\n";

						ret += generate_for_put_custom_type(type);
						ret += L"\n\n";
						ret += generate_for_put_custom_array_type(type);
						ret += L"\n\n";

						ret += generate_for_get_custom_type(type);
						ret += L"\n\n";
						ret += generate_for_get_custom_array_type(type);
						ret += L"\n\n";

				}
		}
		
		return ret;
}

/************************************************************struct def*******************************************************************/

std::wstring generate_for_struct_def_type(const Type_t *type)
{
		std::wstring ret;

		ret += L"typedef struct {\r\n";

		for(size_t i = 0; i < type->fields.size(); ++i)
		{
				const Field_t *field = type->fields[i];
				ret += L"\t\t" + field->type->name + L"\t\t" + field->name;
				if(field->is_array)
				{
						wchar_t buf[128];
						AR_swprintf(buf, 128, L"[%Iu]", field->array_size);
						ret += buf;
				}

				ret += L";";
				ret += L"\r\n";
		}

		ret += L"}" + type->name + L";\r\n\r\n";

		return ret;
}




std::wstring generate_for_struct_def()
{
		std::wstring ret;

		for(size_t i = 0; i < g_type_list.size(); ++i)
		{
				ret += L"\n\n";

				const Type_t *type = g_type_list[i];
				AR_ASSERT(type != NULL);

				if(!is_inner_type(type->type))
				{
						ret += generate_for_struct_def_type(type);
						ret += L"\r\n\r\n";
				}
		}

		return ret;
}



/************************************************************uni struct def*******************************************************************/

static std::wstring type_unistruct_name(const Type_t *type)
{
		std::wstring upper;
		AR_ASSERT(type != NULL && !is_inner_type(type->type));

		for(size_t k = 0; k < type->name.size(); ++k)
		{
				upper += towupper(type->name[k]);
		}
		return upper;
}


std::wstring generate_for_unistruct_def()
{

		std::wstring ret, enum_type, uni_type, marshal_code, unmarshal_code;

		/****************************************generate_for_enum****************************************/
		enum_type += L"enum {\r\n\r\n";

		for(size_t i = 0; i < g_type_list.size(); ++i)
		{
				const Type_t *type = g_type_list[i];
				AR_ASSERT(type != NULL);

				if(is_inner_type(type->type))
				{
						continue;
				}

				if(type->is_anonymous_type)
				{
						continue;
				}
				enum_type += L"\t\t";
				enum_type += type_unistruct_name(type);
				enum_type += L",\r\n";
		}
		
		enum_type += L"};\r\n\r\n";


		/****************************************generate_for_enum****************************************/
		/*
		typedef struct {
				uint_32_t                       type;
				union{
						daemonKeppalive_t		daemonKeppalive_t
				};
		}
		*/
		
		uni_type += L"typedef struct {\r\n\t\tuint_32_t\t\ttype;\r\n\t\tunion{\r\n";

		for(size_t i = 0; i < g_type_list.size(); ++i)
		{
				const Type_t *type = g_type_list[i];
				AR_ASSERT(type != NULL);

				if(is_inner_type(type->type))
				{
						continue;
				}

				if(type->is_anonymous_type)
				{
						continue;
				}
				
				uni_type += (L"\t\t" + type->name	+ L"\t\t" + type->name + L"_val;\r\n");
		}
		
		uni_type += L"\t\t};\r\n}" + g_uni_name + L";\r\n";
		
		/****************************************marshal uni_type_code****************************************/

		marshal_code = L"\r\nstatic arStatus_t\t\t" + g_uni_name + L"_Marshal(" + g_uni_name + L" *uni_type, arBuffer_t *out)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*key, *val;\r\n\t\tsnObject_t\t\t*final_obj;\r\n\t\tAR_ASSERT(uni_type != NULL && out != NULL);\r\n\t\tar_status = AR_S_YES;\r\n\t\tkey = NULL;\r\n\t\tval\t= NULL;\r\n\t\tfinal_obj = NULL;\r\n\t\t\r\n\t\tkey = SN_CreateObject(SN_INT_T);\r\n\t\tif(key == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tSN_SetUIntObject(key, uni_type->type);\r\n\r\n\t\tswitch(uni_type->type)\r\n\t\t{\r\n/**********************************************/\r\n";


		for(size_t i = 0; i < g_type_list.size(); ++i)
		{
				std::wstring case_stat;
				const Type_t *type = g_type_list[i];
				AR_ASSERT(type != NULL);

				if(is_inner_type(type->type))
				{
						continue;
				}

				if(type->is_anonymous_type)
				{
						continue;
				}

				case_stat += (L"\t\tcase " + type_unistruct_name(type) + L" :\r\n");

				wchar_t tmp[1024];
                std::wstring uni_val_type_name = type->name + L"_val";
				AR_swprintf(tmp, 1024, L"val = __put_%ls(&uni_type->%ls)", type->name.c_str(), uni_val_type_name.c_str());
				case_stat += L"\t\t\t\t" + (std::wstring)tmp + L";\r\n";
				case_stat += L"\t\t\t\tbreak;\r\n";
				
				marshal_code += case_stat;
		}

		marshal_code += L"/**********************************************/\r\n\t\tdefault:\r\n\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\t\t\r\n\r\n\t\tif(val == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\r\n\t\tfinal_obj = SN_CreateObject(SN_LIST_T);\r\n\r\n\t\tif(final_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToListObject(final_obj, key);\r\n\t\t\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tkey = NULL;\r\n\r\n\t\tar_status = SN_InsertToListObject(final_obj, val);\r\n\t\t\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tval = NULL;\r\n\r\n\t\tar_status = SN_PutObject(out, final_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\r\n\t\tif(key)\r\n\t\t{\r\n\t\t\t\tSN_DestroyObject(key);\r\n\t\t\t\tkey = NULL;\r\n\t\t}\r\n\t\t\r\n\t\tif(val)\r\n\t\t{\r\n\t\t\t\tSN_DestroyObject(val);\r\n\t\t\t\tval = NULL;\r\n\t\t}\r\n\r\n\t\tif(final_obj)\r\n\t\t{\r\n\t\t\t\tSN_DestroyObject(final_obj);\r\n\t\t\t\tfinal_obj = NULL;\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n}\r\n";


		/****************************************unmarshal uni_type_code****************************************/
		unmarshal_code = L"\r\nstatic arStatus_t\t\t" + g_uni_name + L"_UnMarshal(" + g_uni_name + L" *uni_type, arBuffer_t *in)\r\n{\r\n\t\tsnRetVal_t\t\tsn_ret;\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*type_obj, *data_obj;\r\n\t\tAR_ASSERT(uni_type != NULL && in != NULL);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\ttype_obj = NULL;\r\n\t\tdata_obj = NULL;\r\n\t\tAR_memset(uni_type, 0, sizeof(*uni_type));\r\n\r\n\t\t\r\n\t\tsn_ret = SN_GetObject(in);\r\n\r\n\t\tar_status = sn_ret.status;\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tif(SN_GetObjectType(sn_ret.obj) != SN_LIST_T || SN_GetListObjectCount(sn_ret.obj) != 2)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\t\t\r\n\t\ttype_obj = SN_GetFromListObject(sn_ret.obj, 0);\r\n\r\n\t\tif(type_obj == NULL || SN_GetObjectType(type_obj) != SN_INT_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tdata_obj = SN_GetFromListObject(sn_ret.obj, 1);\r\n\r\n\t\tif(data_obj == NULL || SN_GetObjectType(data_obj) != SN_DICT_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\t\tuni_type->type = (uint_32_t)SN_GetUIntObject(type_obj);\r\n\t\tswitch(uni_type->type)\r\n\t\t{\r\n\t\t/*******************************************************************************************/\r\n";

		for(size_t i = 0; i < g_type_list.size(); ++i)
		{
				std::wstring case_stat;
				const Type_t *type = g_type_list[i];
				AR_ASSERT(type != NULL);

				if(is_inner_type(type->type))
				{
						continue;
				}

				if(type->is_anonymous_type)
				{
						continue;
				}

				case_stat += (L"\t\tcase " + type_unistruct_name(type) + L" :\r\n");

				wchar_t tmp[2048];
                std::wstring uni_val_type_name = type->name + L"_val";
				AR_swprintf(tmp, 2048, L"if(!__get_%ls(data_obj, &uni_type->%ls))\r\n\t\t\t\t{\r\n\t\t\t\t\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\t\t\t\t\tgoto END_POINT;\r\n\t\t\t\t}", type->name.c_str(), uni_val_type_name.c_str());
				case_stat += L"\t\t\t\t" + (std::wstring)tmp + L";\r\n";
				case_stat += L"\t\t\t\tbreak;\r\n";
				
				unmarshal_code += case_stat;
		}



		unmarshal_code += L"/*******************************************************************************************/\r\n\t\tdefault:\r\n\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t\t\tbreak;\r\n\t\t}\r\n\r\n\r\nEND_POINT:\r\n\t\tif(sn_ret.obj)\r\n\t\t{\r\n\t\t\t\tSN_DestroyObject(sn_ret.obj);\r\n\t\t\t\tsn_ret.obj = NULL;\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\n";
		/****************************************final code****************************************/

		ret = L"\r\n" + enum_type + L"\r\n" + uni_type + L"\r\n" + marshal_code + L"\r\n" + unmarshal_code + L"\r\n";
		
		return ret;

}





void marshal_die_main(const wchar_t *input_path, const wchar_t *output_path)
{
		init_inner_type();
        
		arString_t      *input = AR_CreateString();
		arString_t      *output = AR_CreateString();
        
        arString_t      *real_in = AR_CreateString();
        arString_t      *real_out = AR_CreateString();

        
		AR_GetExpandPath(input_path, input);
		AR_GetExpandPath(output_path, output);
        
        if(AR_GetRealPath(AR_GetStringCString(input), real_in) != AR_S_YES)
        {
                AR_error(AR_ERR_FATAL, L"failed to get real path '%ls'\r\n", AR_GetStringCString(input));
                exit(-1);
        }
        
        if(AR_GetRealPath(AR_GetStringCString(output), real_out) != AR_S_YES)
        {
                AR_error(AR_ERR_FATAL, L"failed to get real path '%ls'\r\n", AR_GetStringCString(output));
                exit(-1);
        }
        

		if(AR_LoadBomTextFile(AR_GetStringCString(real_in), NULL, input) != AR_S_YES)
		{
				AR_error(AR_ERR_FATAL, L"Failed to load '%ls'\r\n", AR_GetStringCString(real_in));
				exit(-1);
		}


		generate_type_list(AR_GetStringCString(input));

		std::wstring predef_head_code = generate_for_head_code();
		std::wstring predef_tail_code = generate_for_tail_code();

		std::wstring inner_types_codes = generate_for_inner_type();

		std::wstring struct_def_code = generate_for_struct_def();
		std::wstring put_get_types_code = generate_for_put_get_type();
		
		std::wstring unistruct_code = generate_for_unistruct_def();


		AR_printf(L"%ls\r\n", unistruct_code.c_str());
        
        
		std::wstring final_code = predef_head_code + inner_types_codes + struct_def_code + put_get_types_code +  unistruct_code + predef_tail_code;

		if(AR_SaveBomTextFile(AR_GetStringCString(real_out), AR_TXT_BOM_UTF_8, final_code.c_str()) != AR_S_YES)
		{
				AR_error(AR_ERR_FATAL, L"Failed to save '%ls'\r\n", output_path);
				exit(-1);
		}

		AR_DestroyString(input);
		input = NULL;
		AR_DestroyString(output);
		output = NULL;
        AR_DestroyString(real_in);
		real_in = NULL;
		AR_DestroyString(real_out);
		real_out = NULL;
}




 
static void AR_STDCALL tiny_error(int_t level, const wchar_t* msg, void *ctx)
{
		printf("%ls", msg);
		if(level == AR_ERR_FATAL)
		{
				exit(-1);
		}
}

static void AR_STDCALL tiny_printf(const wchar_t *msg, void *ctx)
{
		printf("%ls", msg);

}




int main(int argc, const char **argv)
{
        wchar_t *input, *output;

        if(argc != 3)
        {
                printf("invalid command line\r\n");
                printf("MarshalDie 'input path' 'output path'\r\n");
                return -1;
        }
        

		

		arInit_t ai = {{tiny_error, tiny_printf, NULL}};

		Arsenal_Init(&ai);
        
        
        
        
        input = AR_str_to_wcs(AR_CP_UTF8, argv[1], AR_strlen(argv[1]));
        output = AR_str_to_wcs(AR_CP_UTF8, argv[2], AR_strlen(argv[2]));
		
        if(input == NULL || output == NULL)
        {
                AR_error(AR_ERR_FATAL, L"bad convert path\r\n");
                exit(-1);
        }

        
		marshal_die_main(input, output);


        AR_DEL(input);
        input = NULL;
        AR_DEL(output);
        output = NULL;
        
		Arsenal_UnInit();

        
		printf("done\r\n");

		return 0;

}


