

#include <vector>
#include <list>
#include <string>
#include <set>
#include <map>
#include "Arsenal.h"
#pragma comment(lib, "Arsenal.lib")

using namespace ARSpace;




typedef enum
{
		BOOL_T,
        BYET_T,

        INT8_T,
        UINTT8_T,
        
        INT16_T,
        UINTT16_T,
        
        INT32_T,
        UINTT32_T,
        
        INT64_T,
        UINTT64_T,

		FLOAT_T,
        DOUBLE_T,
        
        CHAR_T,
        WCHAR_T,

        CUSTOM_T,
}FieldType_t;


static const wchar_t* __g_inner_type[CUSTOM_T] = 
{
		L"ar_bool_t",
        L"ar_byte_t",
        L"ar_int_8_t",
        L"ar_uint_8_t",
        L"ar_int_16_t",
        L"ar_uint_16_t",
        L"ar_int_32_t",
        L"ar_uint_32_t",
        L"ar_int_64_t",
        L"ar_uint_64_t",

		L"float",
        L"double",

        L"char",
        L"wchar_t",
        
};







static AR_INLINE  ar_bool_t is_inner_type(FieldType_t t)
{
        switch(t)
        {
		   case BOOL_T:
		   case BYET_T:
           case INT8_T:
           case UINTT8_T:
           case INT16_T:
           case UINTT16_T:
           case INT32_T:
           case UINTT32_T:
           case INT64_T:
           case UINTT64_T:
		   case FLOAT_T:
		   case DOUBLE_T:
           case CHAR_T:
           case WCHAR_T:
				   return true;
           case CUSTOM_T:
		   default:
				   return false;
        };
}




static AR_INLINE  ar_bool_t is_inner_type_name(const wchar_t *name)
{
        for(size_t i = 0; i < CUSTOM_T; ++i)
        {
                if(AR_wcscmp(name, __g_inner_type[i]) == 0)
                {
                        return true;
                }
        }
        
        return false;
}

static AR_INLINE  const wchar_t* get_inner_type_name(FieldType_t t)
{
        AR_ASSERT(is_inner_type(t));
        return __g_inner_type[t];
}


typedef struct Type						Type_t;
typedef struct Field					Field_t;
typedef std::vector<Field_t*>    		FieldList;



typedef struct Field
{
	std::wstring		name;
	Type_t				*type;
	ar_bool_t				is_array;
	size_t				array_size;
}Field_t;



typedef struct Type
{
        FieldType_t        type;
		std::wstring       name;
		ar_bool_t			   is_anonymous_type;
        ar_bool_t             is_inner_type;
        FieldList		   fields;
}Type_t;


static AR_INLINE void insert_field_to_type(Type_t *type, Field_t *field)
{
	size_t i;

	AR_ASSERT(type != NULL && field != NULL);

	for(size_t i = 0; i < type->fields.size(); ++i)
	{
		if(type->fields[i]->name == field->name)
		{
			AR_error(AR_ERR_FATAL, L"duplicate field name '%ls'\r\n", field->name.c_str());
			exit(-1);
		}
	}

	if(is_inner_type_name(field->name.c_str()))
	{
		AR_error(AR_ERR_FATAL, L"invalid field name '%ls'\r\n", field->name.c_str());
		exit(-1);
	}
	
	type->fields.push_back(field);
	
}




extern std::vector<Type_t*>    				g_type_list;
extern std::vector<std::wstring>			g_head_code;
extern std::vector<std::wstring>			g_tail_code;
extern std::wstring							g_uni_name;



static AR_INLINE Type_t*	find_type(const std::wstring &name)
{
		for(size_t i = 0; i < g_type_list.size(); ++i)
		{
				if(g_type_list[i]->name == name)
				{
						return g_type_list[i];
				}
		}
		return NULL;

}

static AR_INLINE void		insert_type(Type_t *type)
{
		AR_ASSERT(type != NULL);

		if(find_type(type->name) != NULL)
		{
				AR_error(AR_ERR_FATAL, L"duplicate type name '%ls'\r\n", type->name.c_str());
				exit(-1);
		}
		
		g_type_list.push_back(type);
}



static AR_INLINE void init_inner_type()
{
	for(size_t i = 0; i < CUSTOM_T; ++i)
	{
		Type_t *type = new Type_t;
		type->name = get_inner_type_name((FieldType_t)i);
		type->type = (FieldType_t)i;
		type->is_inner_type = true;
		insert_type(type);
	}
}



extern "C" void generate_type_list(const std::wstring &input);

