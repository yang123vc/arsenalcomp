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






typedef struct {
		uint_32_t		session_id;
}daemonHandShake_t;


static snObject_t*		__put_daemonHandShake_t(daemonHandShake_t *stu)
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

		/**********************************************/
		if(__put_UINT32_T_to_dict(obj, L"session_id", stu->session_id) != AR_S_YES)
		{
				goto INVALID_POINT;
		}

		/**********************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}


static bool_t	__get_daemonHandShake_t(snObject_t *obj, daemonHandShake_t *stu)
{
		bool_t	is_ok;
		AR_ASSERT(stu != NULL && obj != NULL);

		is_ok = true;

		if(SN_GetObjectType(obj) != SN_DICT_T)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		/************************************************************************************/
		if(__get_UINT32_T_from_dict(obj, L"session_id", &stu->session_id) != AR_S_YES)
		{
				is_ok = false;
				goto INVALID_POINT;
		}
		
		/************************************************************************************/

		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}



static snObject_t*		__put_daemonHandShake_t_array(daemonHandShake_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);

		stu_list = SN_CreateObject(SN_LIST_T);

		for(i = 0; i < arr_size; ++i)
		{
				/*************************************************************************/
				snObject_t *tmp = __put_daemonHandShake_t(&stu[i]);
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



static bool_t	__get_daemonHandShake_t_array(snObject_t *obj, daemonHandShake_t *stu, size_t arr_size)
{
		bool_t	is_ok;
		size_t list_cnt;
		size_t i;
		AR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);
		AR_ASSERT(SN_GetObjectType(obj) == SN_LIST_T);

		is_ok = true;
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
				
				/***********************************************************/
				if(!__get_daemonHandShake_t(item, &stu[i]))
				{
						is_ok = false;
						goto INVALID_POINT;
				}
				/*********************************************************/
		}
		return true;

INVALID_POINT:
		is_ok = false;
		return is_ok;
}




typedef struct {
		uint_32_t				u;
		uint_32_t				u_arr[1024];
		daemonHandShake_t		h;
		daemonHandShake_t		h_arr[1024];

		byte_t					b;
		byte_t					b_arr[1024];

		char					c;
		char					c_str[1024];

		wchar_t					w;
		wchar_t					w_str[1024];

}daemonKeepalive_t;



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

		/**********************************************/
		if(__put_UINT32_T_to_dict(obj, L"u", stu->u) != AR_S_YES)
		{
				goto INVALID_POINT;
		}

		if(__put_UINT32_T_array_to_dict(obj, L"u_arr", stu->u_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__put_BYTE_T_to_dict(obj, L"b", stu->b) != AR_S_YES)
		{
				goto INVALID_POINT;
		}

		if(__put_BYTE_T_array_to_dict(obj, L"b_arr", stu->b_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__put_CHAR_T_to_dict(obj, L"c", stu->c) != AR_S_YES)
		{
				goto INVALID_POINT;
		}

		if(__put_CHAR_T_array_to_dict(obj, L"c_str", stu->c_str) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__put_WCHAR_T_to_dict(obj, L"w", stu->w) != AR_S_YES)
		{
				goto INVALID_POINT;
		}

		if(__put_WCHAR_T_array_to_dict(obj, L"w_str", stu->w_str) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		/***********************************生成单个结构********************************/
		tmp = __put_daemonHandShake_t(&stu->h);
		if(tmp == NULL)
		{
				goto INVALID_POINT;
		}


		if(SN_InsertToDictObjectByWcsObject(obj, L"h", tmp) != AR_S_YES)
		{
				goto INVALID_POINT;
		}

		/***********************************生成结构数组********************************/
		tmp = __put_daemonHandShake_t_array(stu->h_arr, 1024);
		if(tmp == NULL)
		{
				goto INVALID_POINT;
		}

		if(SN_InsertToDictObjectByWcsObject(obj, L"h_arr", tmp) != AR_S_YES)
		{
				goto INVALID_POINT;
		}

		/**********************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}



static bool_t	__get_daemonKeepalive_t(snObject_t *obj, daemonKeepalive_t *stu)
{
		bool_t	is_ok;
		snObject_t *tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;



		/***************************************************************************/
		if(__get_UINT32_T_from_dict(obj, L"u", &stu->u) != AR_S_YES)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(__get_UINT32_T_array_from_dict(obj, L"u_arr", stu->u_arr, 1024) != AR_S_YES)
		{
				is_ok = false;
				goto INVALID_POINT;
		}
		

		if(__get_BYTE_T_from_dict(obj, L"b", &stu->b) != AR_S_YES)
		{
				goto INVALID_POINT;
		}

		if(__get_BYTE_T_array_from_dict(obj, L"b_arr", stu->b_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_CHAR_T_from_dict(obj, L"c", &stu->c) != AR_S_YES)
		{
				goto INVALID_POINT;
		}

		if(__get_CHAR_T_array_from_dict(obj, L"c_str", stu->c_str, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_WCHAR_T_from_dict(obj, L"w", &stu->w) != AR_S_YES)
		{
				goto INVALID_POINT;
		}

		if(__get_WCHAR_T_array_from_dict(obj, L"w_str", stu->w_str, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



		tmp = SN_FindFromDictObjectByWcs(obj, L"h");
		if(tmp == NULL)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(!__get_daemonHandShake_t(tmp, &stu->h))
		{
				is_ok = false;
				goto INVALID_POINT;
		}


		tmp = SN_FindFromDictObjectByWcs(obj, L"h_arr");
		if(tmp == NULL)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(!__get_daemonHandShake_t_array(tmp, stu->h_arr, 1024))
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


daemonKeepalive_t ka;
daemonKeepalive_t kb;

static void marshal_die_test1()
{
		
		AR_memset(&ka, 0, sizeof(ka));
		AR_memset(&kb, 0, sizeof(kb));
		AR_printf(L"sizeof daemonKeepalive_t = %u KB\r\n", sizeof(ka) / 1024);


		ka.h.session_id = 123;
		
		for(size_t i = 0; i < AR_NELEMS(ka.h_arr); ++i)
		{
				ka.h_arr[i].session_id = i;
		}

		ka.u = 456;

		for(size_t i = 0; i < AR_NELEMS(ka.u_arr); ++i)
		{
				ka.u_arr[i] = i;
		}


		ka.b = 129;

		for(size_t i = 0; i < AR_NELEMS(ka.b_arr); ++i)
		{
				ka.b_arr[i] = i % 255;
		}

		ka.c = 'x';
		AR_strcpy(ka.c_str, "ka.c");

		ka.w = L'X';
		AR_wcscpy(ka.w_str, L"ka.w");



		uint_64_t beg = AR_GetTime_Milliseconds();
		snObject_t *obj = __put_daemonKeepalive_t(&ka);
		


		if(!__get_daemonKeepalive_t(obj, &kb))
		{
				AR_ASSERT(false);
		}
		
		

		uint_64_t end = AR_GetTime_Milliseconds();
		AR_printf(L"elapsed = %u\r\n", end - beg);

		arBuffer_t *buf = AR_CreateBuffer(128);

		if(SN_PutObject(buf, obj) != AR_S_YES)
		{
				AR_ASSERT(false);
		}

		AR_printf(L"serialize = %u KB\r\n", AR_GetBufferAvailable(buf) / 1024);

		SN_DestroyObject(obj);
		obj = NULL;

		AR_DestroyBuffer(buf);
		buf = NULL;
		
		AR_printf(L"AR_memcmp(&ka, &kb, sizeof(ka)) = %u\r\n", AR_memcmp(&ka, &kb, sizeof(ka)));

}


void marshal_die_test()
{
        AR_printf(L"%hs\r\n", AR_FUNC_NAME);

		marshal_die_test1();
        
}



MARSHAL_DIE_END


