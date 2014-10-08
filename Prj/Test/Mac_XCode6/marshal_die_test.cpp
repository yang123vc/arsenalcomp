//
//  marshal_die_test.cpp
//  Test
//
//  Created by Solidus on 12-12-24.
//  Copyright (c) 2012年 none. All rights reserved.
//

#include "Arsenal.h"





AR_NAMESPACE_BEGIN


#include "marshal_inner_types.h"







typedef struct {
		ar_int_8_t		a1;
		ar_uint_8_t		a2;
		ar_int_16_t		b1;
		ar_uint_16_t		b2;
		ar_int_32_t		c1;
		ar_uint_32_t		c2;
		ar_int_64_t		d1;
		ar_uint_64_t		d2;
		ar_byte_t		e1;
		ar_bool_t		e2;
		wchar_t		f1;
		char		f2;
}test;





typedef struct {
		ar_uint_32_t		agent_cli_main_ver;
		ar_uint_32_t		agent_cli_sub_ver;
		ar_uint_32_t		agent_cli_rev_ver;
		ar_uint_32_t		agent_session_type;
		ar_byte_t		extinfo[4096];
		ar_uint_32_t		extcount;
}agentHandshake_t;





typedef struct {
		ar_uint_32_t		agent_srv_main_ver;
		ar_uint_32_t		agent_srv_sub_ver;
		ar_uint_32_t		agent_srv_rev_ver;
		ar_byte_t		extinfo[4096];
		ar_uint_32_t		extcount;
}agentHandshakeReply_t;





typedef struct {
		ar_uint_32_t		err_code;
		wchar_t		err_msg[1024];
}agentErrorMsg_t;





typedef struct {
		ar_uint_64_t		randkey;
}agentRequestURLRecordCount_t;





typedef struct {
		ar_uint_32_t		count;
}agentURLRecordCount_t;





typedef struct {
		ar_uint_32_t		start;
		ar_uint_32_t		count;
}agentRequestURLRecord_t;





typedef struct {
		ar_uint_64_t		id;
		wchar_t		full_url[1024];
		ar_uint_32_t		type;
		ar_uint_32_t		sub_type;
		ar_uint_64_t		blocked_time_ms;
		wchar_t		browser[128];
		wchar_t		user[128];
}agentURLRecord_t;





typedef struct {
		ar_uint_64_t		start_id;
		ar_uint_64_t		end_id;
}removeURLRecord_t;




enum {
        
		TEST,
		AGENTHANDSHAKE_T,
		AGENTHANDSHAKEREPLY_T,
		AGENTERRORMSG_T,
		AGENTREQUESTURLRECORDCOUNT_T,
		AGENTURLRECORDCOUNT_T,
		AGENTREQUESTURLRECORD_T,
		AGENTURLRECORD_T,
		REMOVEURLRECORD_T,
};


typedef struct {
		ar_uint_32_t		type;
		union{
                test		test_val;
                agentHandshake_t		agentHandshake_t_val;
                agentHandshakeReply_t		agentHandshakeReply_t_val;
                agentErrorMsg_t		agentErrorMsg_t_val;
                agentRequestURLRecordCount_t		agentRequestURLRecordCount_t_val;
                agentURLRecordCount_t		agentURLRecordCount_t_val;
                agentRequestURLRecord_t		agentRequestURLRecord_t_val;
                agentURLRecord_t		agentURLRecord_t_val;
                removeURLRecord_t		removeURLRecord_t_val;
		};
}agentInterprocessMessage_t;

































/******************************test********************************/

static snObject_t*		__put_test(test *stu)
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
		if(__put_int_8_t_to_dict(obj, L"a1", stu->a1) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_8_t_to_dict(obj, L"a2", stu->a2) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_int_16_t_to_dict(obj, L"b1", stu->b1) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_16_t_to_dict(obj, L"b2", stu->b2) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_int_32_t_to_dict(obj, L"c1", stu->c1) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_32_t_to_dict(obj, L"c2", stu->c2) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_int_64_t_to_dict(obj, L"d1", stu->d1) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_64_t_to_dict(obj, L"d2", stu->d2) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_byte_t_to_dict(obj, L"e1", stu->e1) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_bool_t_to_dict(obj, L"e2", stu->e2) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_wchar_t_to_dict(obj, L"f1", stu->f1) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_char_to_dict(obj, L"f2", stu->f2) != AR_S_YES)
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


static snObject_t*		__put_test_array(test *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);
        
		stu_list = SN_CreateObject(SN_LIST_T);
        
		for(i = 0; i < arr_size; ++i)
		{
                
				/*************************************************************************/
				snObject_t *tmp = __put_test(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/
                
                
				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
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



static ar_bool_t	__get_test(snObject_t *obj, test *stu)
{
		ar_bool_t	is_ok;
		snObject_t	*tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;
		tmp = NULL;
        
		/***************************************************************************/
		if(__get_int_8_t_from_dict(obj, L"a1", &stu->a1) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_8_t_from_dict(obj, L"a2", &stu->a2) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_int_16_t_from_dict(obj, L"b1", &stu->b1) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_16_t_from_dict(obj, L"b2", &stu->b2) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_int_32_t_from_dict(obj, L"c1", &stu->c1) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"c2", &stu->c2) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_int_64_t_from_dict(obj, L"d1", &stu->d1) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_64_t_from_dict(obj, L"d2", &stu->d2) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_byte_t_from_dict(obj, L"e1", &stu->e1) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_bool_t_from_dict(obj, L"e2", &stu->e2) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_wchar_t_from_dict(obj, L"f1", &stu->f1) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_char_from_dict(obj, L"f2", &stu->f2) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
        
		/***************************************************************************/
        
		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static ar_bool_t	__get_test_array(snObject_t *obj, test *stu, size_t arr_size)
{
		ar_bool_t	is_ok;
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
				if(!__get_test(item, &stu[i]))
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








/******************************agentHandshake_t********************************/

static snObject_t*		__put_agentHandshake_t(agentHandshake_t *stu)
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
		if(__put_uint_32_t_to_dict(obj, L"agent_cli_main_ver", stu->agent_cli_main_ver) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_32_t_to_dict(obj, L"agent_cli_sub_ver", stu->agent_cli_sub_ver) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_32_t_to_dict(obj, L"agent_cli_rev_ver", stu->agent_cli_rev_ver) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_32_t_to_dict(obj, L"agent_session_type", stu->agent_session_type) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_byte_t_array_to_dict(obj, L"extinfo", stu->extinfo, 4096) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
		if(__put_uint_32_t_to_dict(obj, L"extcount", stu->extcount) != AR_S_YES)
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


static snObject_t*		__put_agentHandshake_t_array(agentHandshake_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);
        
		stu_list = SN_CreateObject(SN_LIST_T);
        
		for(i = 0; i < arr_size; ++i)
		{
                
				/*************************************************************************/
				snObject_t *tmp = __put_agentHandshake_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/
                
                
				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
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



static ar_bool_t	__get_agentHandshake_t(snObject_t *obj, agentHandshake_t *stu)
{
		ar_bool_t	is_ok;
		snObject_t	*tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;
		tmp = NULL;
        
		/***************************************************************************/
		if(__get_uint_32_t_from_dict(obj, L"agent_cli_main_ver", &stu->agent_cli_main_ver) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"agent_cli_sub_ver", &stu->agent_cli_sub_ver) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"agent_cli_rev_ver", &stu->agent_cli_rev_ver) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"agent_session_type", &stu->agent_session_type) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_byte_t_array_from_dict(obj, L"extinfo", stu->extinfo, 4096) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"extcount", &stu->extcount) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
        
		/***************************************************************************/
        
		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static ar_bool_t	__get_agentHandshake_t_array(snObject_t *obj, agentHandshake_t *stu, size_t arr_size)
{
		ar_bool_t	is_ok;
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
				if(!__get_agentHandshake_t(item, &stu[i]))
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








/******************************agentHandshakeReply_t********************************/

static snObject_t*		__put_agentHandshakeReply_t(agentHandshakeReply_t *stu)
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
		if(__put_uint_32_t_to_dict(obj, L"agent_srv_main_ver", stu->agent_srv_main_ver) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_32_t_to_dict(obj, L"agent_srv_sub_ver", stu->agent_srv_sub_ver) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_32_t_to_dict(obj, L"agent_srv_rev_ver", stu->agent_srv_rev_ver) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_byte_t_array_to_dict(obj, L"extinfo", stu->extinfo, 4096) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
		if(__put_uint_32_t_to_dict(obj, L"extcount", stu->extcount) != AR_S_YES)
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


static snObject_t*		__put_agentHandshakeReply_t_array(agentHandshakeReply_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);
        
		stu_list = SN_CreateObject(SN_LIST_T);
        
		for(i = 0; i < arr_size; ++i)
		{
                
				/*************************************************************************/
				snObject_t *tmp = __put_agentHandshakeReply_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/
                
                
				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
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



static ar_bool_t	__get_agentHandshakeReply_t(snObject_t *obj, agentHandshakeReply_t *stu)
{
		ar_bool_t	is_ok;
		snObject_t	*tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;
		tmp = NULL;
        
		/***************************************************************************/
		if(__get_uint_32_t_from_dict(obj, L"agent_srv_main_ver", &stu->agent_srv_main_ver) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"agent_srv_sub_ver", &stu->agent_srv_sub_ver) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"agent_srv_rev_ver", &stu->agent_srv_rev_ver) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_byte_t_array_from_dict(obj, L"extinfo", stu->extinfo, 4096) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"extcount", &stu->extcount) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
        
		/***************************************************************************/
        
		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static ar_bool_t	__get_agentHandshakeReply_t_array(snObject_t *obj, agentHandshakeReply_t *stu, size_t arr_size)
{
		ar_bool_t	is_ok;
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
				if(!__get_agentHandshakeReply_t(item, &stu[i]))
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








/******************************agentErrorMsg_t********************************/

static snObject_t*		__put_agentErrorMsg_t(agentErrorMsg_t *stu)
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
		if(__put_uint_32_t_to_dict(obj, L"err_code", stu->err_code) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_wchar_t_array_to_dict(obj, L"err_msg", stu->err_msg) != AR_S_YES)
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


static snObject_t*		__put_agentErrorMsg_t_array(agentErrorMsg_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);
        
		stu_list = SN_CreateObject(SN_LIST_T);
        
		for(i = 0; i < arr_size; ++i)
		{
                
				/*************************************************************************/
				snObject_t *tmp = __put_agentErrorMsg_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/
                
                
				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
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



static ar_bool_t	__get_agentErrorMsg_t(snObject_t *obj, agentErrorMsg_t *stu)
{
		ar_bool_t	is_ok;
		snObject_t	*tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;
		tmp = NULL;
        
		/***************************************************************************/
		if(__get_uint_32_t_from_dict(obj, L"err_code", &stu->err_code) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_wchar_t_array_from_dict(obj, L"err_msg", stu->err_msg, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
        
		/***************************************************************************/
        
		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static ar_bool_t	__get_agentErrorMsg_t_array(snObject_t *obj, agentErrorMsg_t *stu, size_t arr_size)
{
		ar_bool_t	is_ok;
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
				if(!__get_agentErrorMsg_t(item, &stu[i]))
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








/******************************agentRequestURLRecordCount_t********************************/

static snObject_t*		__put_agentRequestURLRecordCount_t(agentRequestURLRecordCount_t *stu)
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
		if(__put_uint_64_t_to_dict(obj, L"randkey", stu->randkey) != AR_S_YES)
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


static snObject_t*		__put_agentRequestURLRecordCount_t_array(agentRequestURLRecordCount_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);
        
		stu_list = SN_CreateObject(SN_LIST_T);
        
		for(i = 0; i < arr_size; ++i)
		{
                
				/*************************************************************************/
				snObject_t *tmp = __put_agentRequestURLRecordCount_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/
                
                
				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
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



static ar_bool_t	__get_agentRequestURLRecordCount_t(snObject_t *obj, agentRequestURLRecordCount_t *stu)
{
		ar_bool_t	is_ok;
		snObject_t	*tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;
		tmp = NULL;
        
		/***************************************************************************/
		if(__get_uint_64_t_from_dict(obj, L"randkey", &stu->randkey) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
        
		/***************************************************************************/
        
		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static ar_bool_t	__get_agentRequestURLRecordCount_t_array(snObject_t *obj, agentRequestURLRecordCount_t *stu, size_t arr_size)
{
		ar_bool_t	is_ok;
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
				if(!__get_agentRequestURLRecordCount_t(item, &stu[i]))
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








/******************************agentURLRecordCount_t********************************/

static snObject_t*		__put_agentURLRecordCount_t(agentURLRecordCount_t *stu)
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
		if(__put_uint_32_t_to_dict(obj, L"count", stu->count) != AR_S_YES)
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


static snObject_t*		__put_agentURLRecordCount_t_array(agentURLRecordCount_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);
        
		stu_list = SN_CreateObject(SN_LIST_T);
        
		for(i = 0; i < arr_size; ++i)
		{
                
				/*************************************************************************/
				snObject_t *tmp = __put_agentURLRecordCount_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/
                
                
				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
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



static ar_bool_t	__get_agentURLRecordCount_t(snObject_t *obj, agentURLRecordCount_t *stu)
{
		ar_bool_t	is_ok;
		snObject_t	*tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;
		tmp = NULL;
        
		/***************************************************************************/
		if(__get_uint_32_t_from_dict(obj, L"count", &stu->count) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
        
		/***************************************************************************/
        
		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static ar_bool_t	__get_agentURLRecordCount_t_array(snObject_t *obj, agentURLRecordCount_t *stu, size_t arr_size)
{
		ar_bool_t	is_ok;
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
				if(!__get_agentURLRecordCount_t(item, &stu[i]))
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








/******************************agentRequestURLRecord_t********************************/

static snObject_t*		__put_agentRequestURLRecord_t(agentRequestURLRecord_t *stu)
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
		if(__put_uint_32_t_to_dict(obj, L"start", stu->start) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_32_t_to_dict(obj, L"count", stu->count) != AR_S_YES)
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


static snObject_t*		__put_agentRequestURLRecord_t_array(agentRequestURLRecord_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);
        
		stu_list = SN_CreateObject(SN_LIST_T);
        
		for(i = 0; i < arr_size; ++i)
		{
                
				/*************************************************************************/
				snObject_t *tmp = __put_agentRequestURLRecord_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/
                
                
				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
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



static ar_bool_t	__get_agentRequestURLRecord_t(snObject_t *obj, agentRequestURLRecord_t *stu)
{
		ar_bool_t	is_ok;
		snObject_t	*tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;
		tmp = NULL;
        
		/***************************************************************************/
		if(__get_uint_32_t_from_dict(obj, L"start", &stu->start) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"count", &stu->count) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
        
		/***************************************************************************/
        
		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static ar_bool_t	__get_agentRequestURLRecord_t_array(snObject_t *obj, agentRequestURLRecord_t *stu, size_t arr_size)
{
		ar_bool_t	is_ok;
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
				if(!__get_agentRequestURLRecord_t(item, &stu[i]))
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








/******************************agentURLRecord_t********************************/

static snObject_t*		__put_agentURLRecord_t(agentURLRecord_t *stu)
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
		if(__put_uint_64_t_to_dict(obj, L"id", stu->id) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_wchar_t_array_to_dict(obj, L"full_url", stu->full_url) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_32_t_to_dict(obj, L"type", stu->type) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_32_t_to_dict(obj, L"sub_type", stu->sub_type) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_64_t_to_dict(obj, L"blocked_time_ms", stu->blocked_time_ms) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_wchar_t_array_to_dict(obj, L"browser", stu->browser) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_wchar_t_array_to_dict(obj, L"user", stu->user) != AR_S_YES)
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


static snObject_t*		__put_agentURLRecord_t_array(agentURLRecord_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);
        
		stu_list = SN_CreateObject(SN_LIST_T);
        
		for(i = 0; i < arr_size; ++i)
		{
                
				/*************************************************************************/
				snObject_t *tmp = __put_agentURLRecord_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/
                
                
				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
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



static ar_bool_t	__get_agentURLRecord_t(snObject_t *obj, agentURLRecord_t *stu)
{
		ar_bool_t	is_ok;
		snObject_t	*tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;
		tmp = NULL;
        
		/***************************************************************************/
		if(__get_uint_64_t_from_dict(obj, L"id", &stu->id) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_wchar_t_array_from_dict(obj, L"full_url", stu->full_url, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"type", &stu->type) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_32_t_from_dict(obj, L"sub_type", &stu->sub_type) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_64_t_from_dict(obj, L"blocked_time_ms", &stu->blocked_time_ms) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_wchar_t_array_from_dict(obj, L"browser", stu->browser, 128) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_wchar_t_array_from_dict(obj, L"user", stu->user, 128) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
        
		/***************************************************************************/
        
		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static ar_bool_t	__get_agentURLRecord_t_array(snObject_t *obj, agentURLRecord_t *stu, size_t arr_size)
{
		ar_bool_t	is_ok;
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
				if(!__get_agentURLRecord_t(item, &stu[i]))
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








/******************************removeURLRecord_t********************************/

static snObject_t*		__put_removeURLRecord_t(removeURLRecord_t *stu)
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
		if(__put_uint_64_t_to_dict(obj, L"start_id", stu->start_id) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}
        
        
        
		if(__put_uint_64_t_to_dict(obj, L"end_id", stu->end_id) != AR_S_YES)
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


static snObject_t*		__put_removeURLRecord_t_array(removeURLRecord_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);
        
		stu_list = SN_CreateObject(SN_LIST_T);
        
		for(i = 0; i < arr_size; ++i)
		{
                
				/*************************************************************************/
				snObject_t *tmp = __put_removeURLRecord_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/
                
                
				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
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



static ar_bool_t	__get_removeURLRecord_t(snObject_t *obj, removeURLRecord_t *stu)
{
		ar_bool_t	is_ok;
		snObject_t	*tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;
		tmp = NULL;
        
		/***************************************************************************/
		if(__get_uint_64_t_from_dict(obj, L"start_id", &stu->start_id) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
		if(__get_uint_64_t_from_dict(obj, L"end_id", &stu->end_id) != AR_S_YES)
		{
				goto INVALID_POINT;
		}
        
        
        
		/***************************************************************************/
        
		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static ar_bool_t	__get_removeURLRecord_t_array(snObject_t *obj, removeURLRecord_t *stu, size_t arr_size)
{
		ar_bool_t	is_ok;
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
				if(!__get_removeURLRecord_t(item, &stu[i]))
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





static arStatus_t		agentInterprocessMessage_t_Marshal(agentInterprocessMessage_t *uni_type, arBuffer_t *out)
{
		arStatus_t		ar_status;
		snObject_t		*key, *val;
		snObject_t		*final_obj;
		AR_ASSERT(uni_type != NULL && out != NULL);
		ar_status = AR_S_YES;
		key = NULL;
		val	= NULL;
		final_obj = NULL;
		
		key = SN_CreateObject(SN_INT_T);
		if(key == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}
        
		SN_SetUIntObject(key, uni_type->type);
        
		switch(uni_type->type)
		{
                        /**********************************************/
                case TEST :
                        val = __put_test(&uni_type->test_val);
                        break;
                case AGENTHANDSHAKE_T :
                        val = __put_agentHandshake_t(&uni_type->agentHandshake_t_val);
                        break;
                case AGENTHANDSHAKEREPLY_T :
                        val = __put_agentHandshakeReply_t(&uni_type->agentHandshakeReply_t_val);
                        break;
                case AGENTERRORMSG_T :
                        val = __put_agentErrorMsg_t(&uni_type->agentErrorMsg_t_val);
                        break;
                case AGENTREQUESTURLRECORDCOUNT_T :
                        val = __put_agentRequestURLRecordCount_t(&uni_type->agentRequestURLRecordCount_t_val);
                        break;
                case AGENTURLRECORDCOUNT_T :
                        val = __put_agentURLRecordCount_t(&uni_type->agentURLRecordCount_t_val);
                        break;
                case AGENTREQUESTURLRECORD_T :
                        val = __put_agentRequestURLRecord_t(&uni_type->agentRequestURLRecord_t_val);
                        break;
                case AGENTURLRECORD_T :
                        val = __put_agentURLRecord_t(&uni_type->agentURLRecord_t_val);
                        break;
                case REMOVEURLRECORD_T :
                        val = __put_removeURLRecord_t(&uni_type->removeURLRecord_t_val);
                        break;
                        /**********************************************/
                default:
                        ar_status = AR_E_INVAL;
                        goto END_POINT;
		}
		
        
		if(val == NULL)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}
        
        
		final_obj = SN_CreateObject(SN_LIST_T);
        
		if(final_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}
        
		ar_status = SN_InsertToListObject(final_obj, key);
		
		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}
        
		key = NULL;
        
		ar_status = SN_InsertToListObject(final_obj, val);
		
		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}
        
		val = NULL;
        
		ar_status = SN_PutObject(out, final_obj);
        
		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}
        
END_POINT:
        
		if(key)
		{
				SN_DestroyObject(key);
				key = NULL;
		}
		
		if(val)
		{
				SN_DestroyObject(val);
				val = NULL;
		}
        
		if(final_obj)
		{
				SN_DestroyObject(final_obj);
				final_obj = NULL;
		}
        
		return ar_status;
}


static arStatus_t		agentInterprocessMessage_t_UnMarshal(agentInterprocessMessage_t *uni_type, arBuffer_t *in)
{
		snRetVal_t		sn_ret;
		arStatus_t		ar_status;
		snObject_t		*type_obj, *data_obj;
		AR_ASSERT(uni_type != NULL && in != NULL);
        
		ar_status = AR_S_YES;
		type_obj = NULL;
		data_obj = NULL;
		AR_memset(uni_type, 0, sizeof(*uni_type));
        
		
		sn_ret = SN_GetObject(in);
        
		ar_status = sn_ret.status;
        
		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}
        
		if(SN_GetObjectType(sn_ret.obj) != SN_LIST_T || SN_GetListObjectCount(sn_ret.obj) != 2)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}
		
		type_obj = SN_GetFromListObject(sn_ret.obj, 0);
        
		if(type_obj == NULL || SN_GetObjectType(type_obj) != SN_INT_T)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}
        
		data_obj = SN_GetFromListObject(sn_ret.obj, 1);
        
		if(data_obj == NULL || SN_GetObjectType(data_obj) != SN_DICT_T)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}
		uni_type->type = (ar_uint_32_t)SN_GetUIntObject(type_obj);
		switch(uni_type->type)
		{
                        /*******************************************************************************************/
                case TEST :
                        if(!__get_test(data_obj, &uni_type->test_val))
                        {
								ar_status = AR_E_INVAL;
								goto END_POINT;
                        };
                        break;
                case AGENTHANDSHAKE_T :
                        if(!__get_agentHandshake_t(data_obj, &uni_type->agentHandshake_t_val))
                        {
								ar_status = AR_E_INVAL;
								goto END_POINT;
                        };
                        break;
                case AGENTHANDSHAKEREPLY_T :
                        if(!__get_agentHandshakeReply_t(data_obj, &uni_type->agentHandshakeReply_t_val))
                        {
								ar_status = AR_E_INVAL;
								goto END_POINT;
                        };
                        break;
                case AGENTERRORMSG_T :
                        if(!__get_agentErrorMsg_t(data_obj, &uni_type->agentErrorMsg_t_val))
                        {
								ar_status = AR_E_INVAL;
								goto END_POINT;
                        };
                        break;
                case AGENTREQUESTURLRECORDCOUNT_T :
                        if(!__get_agentRequestURLRecordCount_t(data_obj, &uni_type->agentRequestURLRecordCount_t_val))
                        {
								ar_status = AR_E_INVAL;
								goto END_POINT;
                        };
                        break;
                case AGENTURLRECORDCOUNT_T :
                        if(!__get_agentURLRecordCount_t(data_obj, &uni_type->agentURLRecordCount_t_val))
                        {
								ar_status = AR_E_INVAL;
								goto END_POINT;
                        };
                        break;
                case AGENTREQUESTURLRECORD_T :
                        if(!__get_agentRequestURLRecord_t(data_obj, &uni_type->agentRequestURLRecord_t_val))
                        {
								ar_status = AR_E_INVAL;
								goto END_POINT;
                        };
                        break;
                case AGENTURLRECORD_T :
                        if(!__get_agentURLRecord_t(data_obj, &uni_type->agentURLRecord_t_val))
                        {
								ar_status = AR_E_INVAL;
								goto END_POINT;
                        };
                        break;
                case REMOVEURLRECORD_T :
                        if(!__get_removeURLRecord_t(data_obj, &uni_type->removeURLRecord_t_val))
                        {
								ar_status = AR_E_INVAL;
								goto END_POINT;
                        };
                        break;
                        /*******************************************************************************************/
                default:
                        ar_status = AR_E_INVAL;
                        goto END_POINT;
                        break;
		}
        
        
END_POINT:
		if(sn_ret.obj)
		{
				SN_DestroyObject(sn_ret.obj);
				sn_ret.obj = NULL;
		}
        
		return ar_status;
}






AR_NAMESPACE_END

