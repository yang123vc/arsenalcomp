
#include "generate_type.h"
#include "predef_code.h"


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

#define PUT_ARRAY_FUNC_FMT	L"\r\nstatic snObject_t*\t\t__put_%ls_array(%ls *stu, size_t arr_size)\r\n{\r\n\t\tsnObject_t\t\t*stu_list;\r\n\t\tsize_t i;\r\n\t\tAR_ASSERT(stu != NULL && arr_size > 0);\r\n\r\n\t\tstu_list = SN_CreateObject(SN_LIST_T);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\r\n\t\t\t\t/*************************************************************************/\r\n\t\t\t\tsnObject_t *tmp = __put_%ls(&stu[i]);\r\n\t\t\t\tif(tmp == NULL)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tgoto INVALID_POINT;\r\n\t\t\t\t}\r\n\t\t\t\t/*************************************************************************/\r\n\r\n\r\n\t\t\t\tif(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tgoto INVALID_POINT;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\treturn stu_list;\r\nINVALID_POINT:\r\n\r\n\t\tif(stu_list)\r\n\t\t{\r\n\t\t\t\tSN_DestroyObject(stu_list);\r\n\t\t\t\tstu_list = NULL;\r\n\t\t}\r\n\r\n\t\treturn stu_list;\r\n}\r\n\r\n"

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

#define GET_FUNC_FMT			L"static bool_t\t__get_%ls(snObject_t *obj, %ls *stu)\r\n{\r\n\t\tbool_t\tis_ok;\r\n\t\tsnObject_t *tmp;\r\n\t\tAR_ASSERT(stu != NULL && obj != NULL);\r\n\t\tAR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);\r\n\t\tis_ok = true;\r\n\r\n\t\t/***************************************************************************/\r\n\t\t%ls\r\n\t\t/***************************************************************************/\r\n\r\n\t\treturn true;\r\nINVALID_POINT:\r\n\t\tis_ok = false;\r\n\t\treturn is_ok;\r\n}"


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






#define INPUT_PATH		L"D:\\Code\\Solidus\\Compiler\\Arsenal\\misc\\marshal_die_input.txt"

void marshal_die_main()
{
		init_inner_type();
		arString_t *input = AR_CreateString();
		arString_t *output = AR_CreateString();

		if(AR_LoadBomTextFile(INPUT_PATH, NULL, input) != AR_S_YES)
		{
				AR_error(AR_ERR_FATAL, L"Failed to load '%ls'\r\n", INPUT_PATH);
				AR_abort();
		}


		generate_type_list(AR_GetStringCString(input));

		std::wstring predef_head_code = generate_for_head_code();
		std::wstring predef_tail_code = generate_for_tail_code();

		std::wstring inner_types_codes = generate_for_inner_type();

		std::wstring struct_def_code = generate_for_struct_def();
		std::wstring put_get_types_code = generate_for_put_get_type();
		


		AR_printf(L"%ls\r\n", struct_def_code.c_str());
		
		
		
		std::wstring final_code = struct_def_code + put_get_types_code;
		AR_SaveBomTextFile(L"C:\\Users\\liupeng\\Desktop\\1.txt", AR_TXT_BOM_UTF_8, final_code.c_str());

		AR_DestroyString(input);
		input = NULL;
		AR_DestroyString(output);
		output = NULL;

}




 
static void AR_STDCALL tiny_error(int_t level, const wchar_t* msg, void *ctx)
{
		printf("%ls", msg);
}

static void AR_STDCALL tiny_printf(const wchar_t *msg, void *ctx)
{
		printf("%ls", msg);
}



int main()
{
		arInit_t ai = {{tiny_error, tiny_printf, NULL}};

		Arsenal_Init(&ai);

		marshal_die_main();

		Arsenal_UnInit();

		_CrtDumpMemoryLeaks();
		printf("done\r\n");
		getchar();

		return 0;

}


