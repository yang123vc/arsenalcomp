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

const std::wstring __g_inner_type[CUSTOM_T] = 
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
        L"char",
        L"wchar_t",
        L"float",
        L"double",
};


struct Type;

std::vector<Type*>    g_type_list;
typedef std::map<std::wstring, Type*>    FieldMap;

struct Type
{
        FieldType_t     type;
        bool_t          is_inner_type;

        std::wstring    name;
        
        bool            is_array;
        size_t          array_size;
        
        
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


