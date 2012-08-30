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
#include "marshal_die_predef.h"



#if defined(__cplusplus)
using namespace ARSpace;
#define MARSHAL_DIE_BEGIN extern "C"{
#define MARSHAL_DIE_END }
#endif



MARSHAL_DIE_BEGIN



typedef enum
{
        BYET_T,

        INT8_T,
        UINTT8_T,
        
        INT16_T,
        UINTT16_T,
        
        INT32_T,
        UINTT32_T,
        
        INT64_T,
        UINTT64_T,

        CHAR_T,
        WCHAR_T,
        
        FLOAT_T,
        DOUBLE_T,
        
        CUSTOM_T,
}FieldType_t;


const wchar_t* __g_inner_type[CUSTOM_T] = 
{
        L"byte_t",
        L"int_8_t",
        L"uint_8_t",
        L"int_16_t",
        L"uint_16_t",
        L"int_32_t",
        L"uint_32_t",
        L"int_64_t",
        L"uint_64_t",

		L"float",
        L"double",

        L"char",
        L"wchar_t",
        
};


static bool_t is_inner_type(FieldType_t t)
{
        switch(t)
        {
                case BYET_T:
                case INT8_T:
                case UINTT8_T:
                case INT16_T:
                case UINTT16_T:
                case INT32_T:
                case UINTT32_T:
                case INT64_T:
                case UINTT64_T:
                case CHAR_T:
                case WCHAR_T:
                        return true;
                case CUSTOM_T:
                default:
                        return false;
        };
}



static bool_t is_inner_type_name(const wchar_t *name)
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

static const wchar_t* get_inner_type_name(FieldType_t t)
{
        AR_ASSERT(is_inner_type(t));
        return __g_inner_type[t];
}




static std::wstring generate_inner_type_marshal_code(const std::wstring &out_buffer_name, const std::wstring &in_obj_pointer, const std::wstring &in_obj_field, FieldType_t t, bool_t is_array, size_t array_size)
{
        std::wstring ret;
        AR_ASSERT(is_inner_type(t));
        
        if(is_array)
        {
                if(array_size == 0)
                {
                        AR_error(AR_ERR_FATAL, L"empty array size for %ls\r\n",  get_inner_type_name(t));
                }

                switch(t)
                {
                        case BYET_T:
								break;
                        case CHAR_T:
								break;
						case INT8_T:
								break;
						case UINTT8_T:
                                break;
						case WCHAR_T:
								break;
                        case INT16_T:
								break;
                        case UINTT16_T:
								break;
                        case INT32_T:
								break;
                        case UINTT32_T:
								break;
                        case INT64_T:
								break;
                        case UINTT64_T:
								break;
                        case FLOAT_T:
								break;
                        case DOUBLE_T:
                                break;
                        default:
                                AR_ASSERT(false);
                                break;
                }
        }else
        {
                
        }
        
        
}


struct Type;

std::vector<Type*>    g_type_list;
typedef std::map<std::wstring, Type*>    FieldMap;

struct Type
{
        FieldType_t             type;
        bool_t                  is_inner_type;

        std::wstring            name;
        
        bool                    is_array;
        size_t                  array_size;
        
        
        FieldMap                fields;
        

        const std::wstring& generate_marshal_code()
        {
                return std::wstring(L"");
        }
        
        const std::wstring& generate_unmarshal_code()
        {
                return std::wstring(L"");
        }
};




std::vector<std::wstring>    g_head_code;
std::vector<std::wstring>    g_tail_code;




void marshal_die_test()
{
        AR_printf(L"%hs\r\n", AR_FUNC_NAME);
        
}



MARSHAL_DIE_END


