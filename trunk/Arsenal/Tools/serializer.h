/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#ifndef __ARSENAL_SERIALIZATION_H__
#define __ARSENAL_SERIALIZATION_H__

#include "../Common/common.h"

AR_NAMESPACE_BEGIN

struct __serialize_object_tag;
typedef	struct __serialize_object_tag	snObject_t;


typedef struct __sn_int_tag
{
		bool_t	is_signed;
		union{
				uint_64_t		u;
				int_64_t		s;
		};
}snInteger_t;


void	SN_InitInteger(snInteger_t *dest);
void	SN_UnInitInteger(snInteger_t *dest);



typedef struct __sn_string_tag
{
		byte_t	*data;
		size_t	len;
}snString_t;

void	SN_InitString(snString_t	*str);
void	SN_UnInitString(snString_t	*dest);

void	SN_SetStringByData(snString_t	*dest, const byte_t *data, size_t len);
void	SN_SetStringByStr(snString_t	*dest, const char *str);
void	SN_SetStringByWcs(snString_t	*dest, const wchar_t *str);


int_t	SN_CompStringByString(const snString_t	*l,  const snString_t	*r);
int_t	SN_CompStringByData(const snString_t	*l,  const byte_t *data, size_t len);
int_t	SN_CompStringByStr(const snString_t		*l,	 const char *str);
int_t	SN_CompStringByWcs(const snString_t		*l,	 const wchar_t *str);



typedef struct __sn_list_tag
{
		snObject_t		**lst;
		size_t			count;
		size_t			cap;
}snList_t;

void	SN_InitList(snList_t	*lst);
void	SN_UnInitList(snList_t	*lst);

void	SN_InsertToList(snList_t	*lst, snObject_t *obj);
bool_t	SN_RemoveFromList(snList_t	*lst, size_t idx);
int_t	SN_IndexOfList(const snList_t *lst, const snObject_t *obj);
snObject_t*		SN_GetObjectFromList(snList_t *lst, size_t idx);

typedef struct __sn_dict_pair_tag
{
		snObject_t		*key;
		snObject_t		*val;
}snPair_t;

typedef struct __sn_dict_tag
{
		snPair_t		*pairs;		
		size_t			count;
		size_t			cap;
}snDict_t;

void	SN_InitDict(snDict_t *dict);
void	SN_UnInitDict(snDict_t *dict);
bool_t	SN_InsertToDict(snDict_t *dict, snObject_t *key, snObject_t *value);
bool_t	SN_RemoveFromDict(snDict_t *dict, const snObject_t *key);
snObject_t* SN_FindFromDict(snDict_t *dict, const snObject_t *key);






typedef enum
{
		SN_INT_T,
		SN_STRING_T,
		SN_LIST_T,
		SN_DICT_T
}snType_t;


struct __serialize_object_tag
{
		snType_t		type;
		union{
				snInteger_t		integer;
				snString_t		string;
				snList_t		list;
				snDict_t		dict;
		};
};

snObject_t*		SN_CreateObject(snType_t		type);
void			SN_DestroyObject(snObject_t		*obj);

snObject_t*		SN_GetObject(arBuffer_t	*buffer);
void			SN_PutObject(arBuffer_t	*buffer, const snObject_t *obj);



snObject_t*		SN_FindPathByWcs(snObject_t *pobj, const wchar_t *path);
snObject_t*		SN_FindPathByStr(snObject_t *pobj, const char *path);




AR_NAMESPACE_END


#endif

