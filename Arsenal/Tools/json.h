/*
 * The Arsenal Library
 * Copyright (c) 2009-2013 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */


#ifndef __ARSENAL_JSONOBJ_H__
#define __ARSENAL_JSONOBJ_H__



#include "../Common/common.h"

AR_NAMESPACE_BEGIN

arStatus_t		Json_Init();
void			Json_UnInit();


typedef enum
{
		JSON_TYPE_DICT_T,
		JSON_TYPE_ARRAY_T,
		JSON_TYPE_STRING_T,
		JSON_TYPE_INT_T,
		JSON_TYPE_FLOAT_T,
		JSON_TYPE_BOOL_T,
		JSON_TYPE_NULL_T,
}jsonType_t;

typedef struct __json_object_tag		jsonObj_t;


jsonObj_t*		Json_CreateObject(jsonType_t t);
void			Json_DestroyObject(jsonObj_t *obj);

arStatus_t		Json_CompareObject(const jsonObj_t *l, const jsonObj_t *r);

jsonType_t		Json_GetObjectType(const jsonObj_t *obj);

arStatus_t		Json_SetIntToObject(jsonObj_t *obj, ar_int_64_t num);
arStatus_t		Json_GetIntFromObject(jsonObj_t *obj, ar_int_64_t *num);

arStatus_t		Json_SetUIntToObject(jsonObj_t *obj, ar_uint_64_t num);
arStatus_t		Json_GetUIntFromObject(jsonObj_t *obj, ar_uint_64_t *num);

arStatus_t		Json_SetFloatToObject(jsonObj_t *obj, double num);
arStatus_t		Json_GetFloatFromObject(jsonObj_t *obj, double *num);

arStatus_t		Json_SetStringToObject(jsonObj_t *obj, const wchar_t *str);
arStatus_t		Json_GetStringFromObject(jsonObj_t *obj, arString_t *str);
const wchar_t*	Json_GetWcsFromObject(jsonObj_t *obj);			/*valid until next call*/

arStatus_t		Json_SetBooleanToObject(jsonObj_t *obj, ar_bool_t b);
arStatus_t		Json_GetBooleanFromObject(jsonObj_t *obj, ar_bool_t *b);


arStatus_t		Json_GetArrayObjectCount(const jsonObj_t *obj, size_t *count);
arStatus_t		Json_SetArrayObjectByIndex(jsonObj_t *obj, size_t idx, jsonObj_t *item);
arStatus_t		Json_GetArrayObjectByIndex(jsonObj_t *obj, size_t idx, jsonObj_t **item);
arStatus_t		Json_AppendToArrayObject(jsonObj_t *obj, jsonObj_t *item);


arStatus_t		Json_GetDictObjectCount(const jsonObj_t *obj, size_t *count);
arStatus_t		Json_SetDictObject(jsonObj_t *obj, const wchar_t *key, jsonObj_t *val, ar_bool_t replace_exist);
arStatus_t		Json_GetDictObject(jsonObj_t *obj, const wchar_t *key, jsonObj_t **val);
arStatus_t		Json_RemoveDictObject(jsonObj_t *obj, const wchar_t *key);



arStatus_t		Json_LoadObjectFromString(const wchar_t *content, jsonObj_t **obj);
arStatus_t		Json_SaveObjectToString(const jsonObj_t *obj, arString_t *str);

arStatus_t		Json_LoadObjectFromFile(const wchar_t *path, jsonObj_t **obj);
arStatus_t		Json_SaveObjectToFile(const jsonObj_t *obj, const wchar_t *path);






AR_NAMESPACE_END

#endif