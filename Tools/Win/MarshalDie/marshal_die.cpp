
#include "generate_type.h"
#include "predef_code.h"


static const std::wstring generate_for_head_code()
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

static std::wstring generate_for_inner_type(Type_t *type)
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




#define PUT_FIELD_FMT			L"\t\tif(%ls(obj, L\"%ls\", stu->%ls) != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\t\t\r\n\t\t}\r\n\r\n"
#define PUT_FIELD_ARRAY_FMT		L"\t\tif(%ls(obj, L\"%ls\", stu->%ls, %Iu) != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\t\t\r\n\t\t}\r\n"

static std::wstring generate_func_call_for_put_field(const Field_t *field)
{
		std::wstring ret;
		arString_t *str = AR_CreateString();
		const Type_t *field_type = field->type;

		if(is_inner_type(field_type->type))
		{
				if(field->is_array)
				{
						AR_FormatString(str, PUT_FIELD_ARRAY_FMT, __g_inner_type_marshal_array_call_name[field_type->type], field->name.c_str(), field->name.c_str(), field->array_size);
				}else
				{
						AR_FormatString(str, PUT_FIELD_FMT, __g_inner_type_marshal_call_name[field_type->type], field->name.c_str(), field->name.c_str());
				}
		}else
		{
				if(field->is_array)
				{
						wchar_t put_call_name[1024];
						AR_swprintf(put_call_name, 1024, L"__put_%ls_array_to_dict", field_type->name.c_str());
						AR_FormatString(str, PUT_FIELD_ARRAY_FMT, put_call_name, field->name.c_str(), field->name.c_str(), field->array_size);
				}else
				{
						wchar_t put_call_name[1024];
						AR_swprintf(put_call_name, 1024, L"__put_%ls_to_dict", field_type->name.c_str());
						AR_FormatString(str, PUT_FIELD_FMT, put_call_name, field->name.c_str(), field->name.c_str());
				}
		}

		AR_AppendString(str, L"\n\n");

		ret = AR_GetStringCString(str);
		AR_DestroyString(str);
		str = NULL;
		return ret;
}


#define PUT_FUNC_FMT	L"static snObject_t*\t\t__put_%ls_to_dict(%ls *stu)\r\n{\r\n\t\tsnObject_t\t\t*obj, *tmp;\r\n\t\tAR_ASSERT(stu != NULL);\r\n\t\tobj = NULL;\r\n\t\ttmp = NULL;\r\n\r\n\t\tobj = SN_CreateObject(SN_DICT_T);\r\n\t\tif(obj == NULL)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n/************************************************************************************************/\r\n%ls\r\n/************************************************************************************************/\r\n\t\treturn obj;\r\nINVALID_POINT:\r\n\t\tif(obj)\r\n\t\t{\r\n\t\t\t\tSN_DestroyObject(obj);\r\n\t\t\t\tobj = NULL;\r\n\t\t}\r\n\t\treturn NULL;\r\n}"




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

		AR_printf(L"%ls\r\n", ret.c_str());
		return ret;
}




static void handle_types()
{
		for(size_t i = 0; i < g_type_list.size(); ++i)
		{
				const Type_t *type = g_type_list[i];
				AR_ASSERT(type != NULL);

				if(!is_inner_type(type->type))
				{
						generate_for_put_custom_type(type);
				}

		}
}


#define INPUT_PATH		L"D:\\Code\\Solidus\\Compiler\\Arsenal\\misc\\marshal_die_input.txt"

#define PUT_FUNC_FMT	L"static snObject_t*\t\t__put_%ls(%ls *stu)\r\n{\r\n\t\tsnObject_t\t\t*obj, *tmp;\r\n\t\tAR_ASSERT(stu != NULL);\r\n\t\tobj = NULL;\r\n\t\ttmp = NULL;\r\n\r\n\t\tobj = SN_CreateObject(SN_DICT_T);\r\n\t\tif(obj == NULL)\r\n\t\t{\r\n\t\t\t\tgoto INVALID_POINT;\r\n\t\t}\r\n/************************************************************************************************/\r\n%ls\r\n/************************************************************************************************/\r\n\t\treturn obj;\r\nINVALID_POINT:\r\n\t\tif(obj)\r\n\t\t{\r\n\t\t\t\tSN_DestroyObject(obj);\r\n\t\t\t\tobj = NULL;\r\n\t\t}\r\n\t\treturn NULL;\r\n}"

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

		handle_types();


		


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


