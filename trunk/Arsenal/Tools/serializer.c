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

#include "serializer.h"



AR_NAMESPACE_BEGIN

/***************************************Declaration*************************************************/

typedef struct __sn_int_tag
{
		bool_t	is_signed;
		union{
				uint_64_t		u;
				int_64_t		s;
		};
}snInteger_t;


static void	SN_InitInteger(snInteger_t *dest);
static void	SN_UnInitInteger(snInteger_t *dest);



typedef struct __sn_string_tag
{
		byte_t	*data;
		size_t	len;
}snString_t;

static void	SN_InitString(snString_t	*str);
static void	SN_UnInitString(snString_t	*dest);

static void	SN_SetStringByData(snString_t	*dest, const byte_t *data, size_t len);
static void	SN_SetStringByStr(snString_t	*dest, const char *str);
static void	SN_SetStringByWcs(snString_t	*dest, const wchar_t *str);


static int_t	SN_CompStringByString(const snString_t	*l,  const snString_t	*r);
static int_t	SN_CompStringByData(const snString_t	*l,  const byte_t *data, size_t len);
static int_t	SN_CompStringByStr(const snString_t		*l,	 const char *str);
static int_t	SN_CompStringByWcs(const snString_t		*l,	 const wchar_t *str);



typedef struct __sn_list_tag
{
		snObject_t		**lst;
		size_t			count;
		size_t			cap;
}snList_t;

static void			SN_InitList(snList_t	*lst);
static void			SN_UnInitList(snList_t	*lst);

static void			SN_InsertToList(snList_t	*lst, snObject_t *obj);
static bool_t			SN_RemoveFromList(snList_t	*lst, size_t idx);
static int_t			SN_IndexOfList(const snList_t *lst, const snObject_t *obj);
static snObject_t*		SN_GetFromList(snList_t *lst, size_t idx);



typedef struct __sn_dict_tag
{
		snPair_t		*pairs;		
		size_t			count;
		size_t			cap;
}snDict_t;

static void	SN_InitDict(snDict_t *dict);
static void	SN_UnInitDict(snDict_t *dict);
static bool_t	SN_InsertToDict(snDict_t *dict, snObject_t *key, snObject_t *value);
static bool_t	SN_RemoveFromDict(snDict_t *dict, const snObject_t *key);
static snObject_t* SN_FindFromDict(snDict_t *dict, const snObject_t *key);








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





/*int*/



static void	SN_InitInteger(snInteger_t *dest)
{
		AR_ASSERT(dest != NULL);
		AR_memset(dest, 0, sizeof(*dest));
}

static void	SN_UnInitInteger(snInteger_t *dest)
{
		AR_ASSERT(dest != NULL);
		AR_memset(dest, 0, sizeof(*dest));
}




/*string*/
static void	SN_InitString(snString_t	*str)
{
		AR_ASSERT(str != NULL);
		AR_memset(str, 0, sizeof(*str));
}

static void	SN_UnInitString(snString_t	*str)
{
		if(str->data != NULL)
		{
				AR_DEL(str->data);
		}
		AR_memset(str, 0, sizeof(*str));
}

static void	SN_SetStringByData(snString_t	*dest, const byte_t *data, size_t len)
{
		AR_ASSERT(dest != NULL && data != NULL && len > 0);

		if(dest->data != NULL)
		{
				AR_DEL(dest->data);
				dest->data = NULL;
		}
		
		dest->data = AR_NEWARR(byte_t, len);
		AR_memcpy(dest->data, data, len);
		dest->len = len;
}

static void	SN_SetStringByStr(snString_t	*dest, const char *str)
{
		size_t	l;
		AR_ASSERT(dest != NULL && str != NULL);
		l = AR_strlen(str);
		AR_ASSERT(l > 0);
		SN_SetStringByData(dest, (const byte_t*)str, l);
}

static void	SN_SetStringByWcs(snString_t	*dest, const wchar_t *str)
{
		const char *utf8;
		AR_ASSERT(dest != NULL && str != NULL);
		utf8 = AR_wcs_convto_utf8(str);
		AR_ASSERT(utf8 != NULL);
		SN_SetStringByStr(dest, utf8);
		AR_DEL(utf8);
}


static int_t	SN_CompStringByString(const snString_t	*l,  const snString_t	*r)
{
		AR_ASSERT(l != NULL && r != NULL && l->len > 0 && r->len > 0 && l->data != NULL && r->data != NULL);
		
		if(l->len < r->len)
		{
				return -1;
		}else if(l->len > r->len)
		{
				return 1;
		}else
		{
				return AR_memcmp(l->data, r->data, l->len);
		}
}



static int_t	SN_CompStringByData(const snString_t	*l,  const byte_t *data, size_t len)
{
		snString_t		other;
		int_t cmp;
		AR_ASSERT(l != NULL && l->len > 0 && data != NULL && len > 0);

		SN_InitString(&other);
		SN_SetStringByData(&other, data, len);

		cmp = SN_CompStringByString(l, &other);
		SN_UnInitString(&other);
		return cmp;
}

static int_t	SN_CompStringByStr(const snString_t		*l,	 const char *str)
{
		snString_t		other;
		int_t cmp;
		AR_ASSERT(l != NULL && l->len > 0 && str != NULL);

		SN_InitString(&other);
		SN_SetStringByStr(&other, str);
		cmp = SN_CompStringByString(l, &other);
		SN_UnInitString(&other);
		return cmp;
}

static int_t	SN_CompStringByWcs(const snString_t		*l,	 const wchar_t *str)
{
		snString_t		other;
		int_t cmp;
		AR_ASSERT(l != NULL && l->len > 0 && str != NULL);

		SN_InitString(&other);
		SN_SetStringByWcs(&other, str);
		cmp = SN_CompStringByString(l, &other);
		SN_UnInitString(&other);
		return cmp;
}


/*list*/

static void	SN_InitList(snList_t	*lst)
{
		AR_ASSERT(lst != NULL);
		AR_memset(lst, 0, sizeof(*lst));
}

static void	SN_UnInitList(snList_t	*lst)
{
		size_t i;
		AR_ASSERT(lst != NULL);

		for(i = 0; i < lst->count; ++i)
		{
				SN_DestroyObject(lst->lst[i]);
		}

		if(lst->lst)AR_DEL(lst->lst);
		AR_memset(lst, 0, sizeof(*lst));
}


static void	SN_InsertToList(snList_t	*lst, snObject_t *obj)
{
		AR_ASSERT(lst != NULL && obj != NULL);

		if(lst->count == lst->cap)
		{
				lst->cap = (lst->cap + 4) * 2;
				lst->lst = AR_REALLOC(snObject_t*, lst->lst, lst->cap);
		}
		lst->lst[lst->count++] = obj;
}




static bool_t	SN_RemoveFromList(snList_t	*lst, size_t idx)
{
		
		AR_ASSERT(lst != NULL);
		if(idx >= lst->count)return false;

		SN_DestroyObject(lst->lst[idx]);
		lst->lst[idx] = lst->lst[lst->count-1];
		lst->count--;
		return true;
}


static int_t	SN_IndexOfList(const snList_t *lst, const snObject_t *obj)
{
		int_t i;
		AR_ASSERT(lst != NULL && obj != NULL);

		for(i = 0; i < (int_t)lst->count; ++i)
		{
				if(obj == lst->lst[i])return i;
		}
		return -1;
}


static snObject_t*		SN_GetFromList(snList_t *lst, size_t idx)
{
		AR_ASSERT(lst != NULL);
		if(idx >= lst->count)return NULL;
		return lst->lst[idx];
}






/*dict*/
static void	SN_InitDict(snDict_t *dict)
{
		AR_ASSERT(dict != NULL);
		AR_memset(dict, 0, sizeof(*dict));
}

static void	SN_UnInitDict(snDict_t *dict)
{
		size_t i;
		AR_ASSERT(dict != NULL);
		for(i = 0; i < dict->count; ++i)
		{
				SN_DestroyObject(dict->pairs[i].key);
				SN_DestroyObject(dict->pairs[i].val);
		}
		
		if(dict->pairs)AR_DEL(dict->pairs);
		AR_memset(dict, 0, sizeof(*dict));
}


static bool_t	SN_InsertToDict(snDict_t *dict, snObject_t *key, snObject_t *value)
{
		size_t i;
		AR_ASSERT(dict != NULL && key != NULL && key->type == SN_STRING_T && value != NULL);

		for(i = 0; i < dict->count; ++i)
		{
				if(SN_CompStringByString(&key->string, &(dict->pairs[i].key->string)) == 0)
				{
						break;
				}
		}

		if(i < dict->count)
		{
				if(dict->pairs[i].val)
				{
						SN_DestroyObject(dict->pairs[i].val);
				}

				dict->pairs[i].val = value;
				return true;
		}else
		{
				if(dict->count == dict->cap)
				{
						dict->cap = (dict->cap + 4) * 2;
						dict->pairs = AR_REALLOC(snPair_t, dict->pairs, dict->cap);
				}		

				dict->pairs[dict->count].key = key;
				dict->pairs[dict->count].val = value;
				dict->count++;
				return true;
		}
}

static bool_t	SN_RemoveFromDict(snDict_t *dict, const snObject_t *key)
{
		size_t i;
		AR_ASSERT(dict != NULL && key != NULL && key->type == SN_STRING_T);

		for(i = 0; i < dict->count; ++i)
		{
				if(SN_CompStringByString(&key->string, &(dict->pairs[i].key->string)) == 0)
				{
						break;
				}
		}

		if(i == dict->count)return false;

		SN_DestroyObject(dict->pairs[i].key);
		SN_DestroyObject(dict->pairs[i].val);

		dict->pairs[i] = dict->pairs[dict->count-1];
		dict->count--;
		return true;
		
}

static snObject_t* SN_FindFromDict(snDict_t *dict, const snObject_t *key)
{
		size_t i;
		AR_ASSERT(dict != NULL && key != NULL && key->type == SN_STRING_T);

		for(i = 0; i < dict->count; ++i)
		{
				if(SN_CompStringByString(&key->string, &(dict->pairs[i].key->string)) == 0)
				{
						return dict->pairs[i].val;
				}
		}
		return NULL;
}









/*********************************************Object**************************************************************/



snObject_t*		SN_CreateObject(snType_t		type)
{
		snObject_t		*obj;
		obj = AR_NEW0(snObject_t);
		obj->type = type;
		switch(type)
		{
		default:
		{
				AR_ASSERT(false);		
				break;
		}
		case SN_INT_T:
		{
				SN_InitInteger(&obj->integer);
				break;
		}
		case SN_STRING_T:
		{
				SN_InitString(&obj->string);
				break;
		}
		case SN_LIST_T:
		{
				SN_InitList(&obj->list);
				break;
		}
		case SN_DICT_T:
		{
				SN_InitDict(&obj->dict);
				break;
		}
		}
		return obj;
}


void			SN_DestroyObject(snObject_t		*obj)
{
		AR_ASSERT(obj != NULL);

		switch(obj->type)
		{
		default:
		{
				AR_ASSERT(false);		
				break;
		}
		case SN_INT_T:
		{
				SN_UnInitInteger(&obj->integer);
				break;
		}
		case SN_STRING_T:
		{
				SN_UnInitString(&obj->string);
				break;
		}
		case SN_LIST_T:
		{
				SN_UnInitList(&obj->list);
				break;
		}
		case SN_DICT_T:
		{
				SN_UnInitDict(&obj->dict);
				break;
		}
		}

		AR_DEL(obj);
}

snType_t		SN_GetObjectType(const snObject_t *obj)
{
		AR_ASSERT(obj != NULL);
		return obj->type;
}



static snObject_t*	__get_int(arBuffer_t	*buffer)
{
		snObject_t		*ret = NULL;
		size_t	buf_len, idx;
		const byte_t	*pbuf;
		
		uint_64_t		num = 0;
		bool_t			is_neg = false;
		AR_ASSERT(buffer != NULL);

		buf_len = AR_GetBufferAvailable(buffer);
		pbuf	= AR_GetBufferData(buffer);
		
		if(pbuf == NULL)return NULL;
		idx = 0;
		if(pbuf[0] == '-')
		{
				is_neg = true;
				idx = 1;
		}
		
		while(idx < buf_len && AR_isdigit(pbuf[idx]))
		{
				num *= 10;
				num += (pbuf[idx] - '0');
				idx++;
		}
		
		if(idx == buf_len)return NULL;
		if(pbuf[idx] != 'e')return NULL;

		ret = SN_CreateObject(SN_INT_T);
		ret->integer.is_signed = is_neg;
		if(is_neg)
		{
				ret->integer.s = 0 - (int_64_t)num;
		}else
		{
				ret->integer.u = num;
		}
		AR_EraseBuffer(buffer, idx + 1);
		return ret;
}

static snObject_t*		__get_str(arBuffer_t	*buffer)
{
		snObject_t		*ret = NULL;
		size_t	buf_len, idx;
		const byte_t	*pbuf;
		
		size_t	l = 0;

		AR_ASSERT(buffer != NULL);

		buf_len = AR_GetBufferAvailable(buffer);
		pbuf	= AR_GetBufferData(buffer);
		
		if(pbuf == NULL)return NULL;
		
		for(idx = 0; idx < buf_len && AR_isdigit(pbuf[idx]); ++idx)
		{
				l *= 10;
				l += (pbuf[idx] - '0');
		}

		if(idx  == buf_len || pbuf[idx] != ':' || l == 0)return NULL;

		if(buf_len <  idx + 1 + l)return NULL;

		ret = SN_CreateObject(SN_STRING_T);
		idx++;
		SN_SetStringByData(&ret->string, pbuf + idx, l);
		AR_EraseBuffer(buffer, idx + l);
		return ret;
}

static snObject_t*		__get_list(arBuffer_t	*buffer);
static snObject_t*		__get_dict(arBuffer_t	*buffer);

static snObject_t*		__get_obj(arBuffer_t	*buffer)
{
		byte_t b;
		const byte_t *data;
		AR_ASSERT(buffer != NULL && AR_GetBufferAvailable(buffer) > 0);

		data = AR_GetBufferData(buffer);
		if(data == NULL)return NULL;
		b = data[0];
		
		switch(b)
		{
		case 'd':
				AR_EraseBuffer(buffer, 1);
				return __get_dict(buffer);
		case 'i':
				AR_EraseBuffer(buffer, 1);
				return __get_int(buffer);
		case 'l':
				AR_EraseBuffer(buffer, 1);
				return __get_list(buffer);
		default:
		{
				if(AR_isdigit(b) || b == '-')
				{
						return __get_str(buffer);
				}else
				{
						return NULL;
				}
		}
		}
}


static snObject_t*		__get_list(arBuffer_t	*buffer)
{
		const byte_t	*pb;
		snObject_t		*ret;
		AR_ASSERT(buffer != NULL && AR_GetBufferAvailable(buffer) > 0);
		
		ret = SN_CreateObject(SN_LIST_T);
		
		while(true)
		{
				snObject_t *obj;
				pb = AR_GetBufferData(buffer);
				if(pb == NULL)goto FAILED_POINT;
				if(*pb == 'e')break;
				obj = __get_obj(buffer);
				if(obj == NULL)goto FAILED_POINT;
				SN_InsertToList(&ret->list, obj);
		}
		AR_EraseBuffer(buffer, 1);
		return ret;
FAILED_POINT:
		SN_DestroyObject(ret);
		return NULL;
}


static snObject_t*		__get_dict(arBuffer_t	*buffer)
{
		const byte_t	*pb;
		snObject_t		*ret;
		AR_ASSERT(buffer != NULL && AR_GetBufferAvailable(buffer) > 0);
		ret = SN_CreateObject(SN_DICT_T);
		
		while(true)
		{
				snObject_t *key, *val;
				
				pb = AR_GetBufferData(buffer);
				if(pb == NULL)goto FAILED_POINT;
				if(*pb == 'e')break;

				key = __get_str(buffer);
				if(key == NULL)goto FAILED_POINT;
				val = __get_obj(buffer);
				if(val == NULL)
				{
						SN_DestroyObject(key);
						goto FAILED_POINT;
				}
				
				if(!SN_InsertToDict(&ret->dict, key, val))
				{
						goto FAILED_POINT;
				}
		}
		AR_EraseBuffer(buffer, 1);
		return ret;
FAILED_POINT:
		SN_DestroyObject(ret);
		return NULL;
}




snObject_t*		SN_GetObject(arBuffer_t	*buffer)
{
		return __get_obj(buffer);
}




static void		__put_int(arBuffer_t	*buffer, const snInteger_t *integer)
{
		char buf[256];
		char *p;
		bool_t	is_neg = false;
		uint_64_t		num;
		static const char _tbl[] = "0123456789";

		AR_ASSERT(buf != NULL && integer != NULL);

		if(integer->is_signed)
		{
				if(integer->s < 0)
				{
						is_neg = true;
						num = (uint_64_t)(-integer->s);
				}else
				{
						num = (uint_64_t)integer->s;
				}
		}else
		{
				num= integer->u;
		}
		
		p = buf + sizeof(buf);
		*--p = '\0';
		*--p = 'e';
		do{ *--p = _tbl[num % 10];}while((num /= 10));

		if(is_neg)*--p = '-';
		*--p = 'i';
		AR_InsertBuffer(buffer, (const byte_t*)p, AR_strlen(p));
}

static void __put_string(arBuffer_t	*buffer, const snString_t *string)
{
		char buf[256];
		char *p;
		size_t	l;
		static const char _tbl[] = "0123456789";

		AR_ASSERT(buf != NULL && string != NULL && string->len > 0 && string->data != NULL);
		
		l = string->len;
		
		p = buf + sizeof(buf);
		*--p = '\0';
		*--p = ':';

		do{ *--p = _tbl[l % 10];}while((l /= 10));

		AR_InsertBuffer(buffer, (const byte_t*)p, AR_strlen(p));
		AR_InsertBuffer(buffer, string->data, string->len);
}


static void __put_list(arBuffer_t	*buffer, const snList_t *lst);
static void __put_dict(arBuffer_t	*buffer, const snDict_t *dict);

static void __put_obj(arBuffer_t	*buffer, const snObject_t *obj)
{
		switch(obj->type)
		{
		default:
		{
				AR_ASSERT(false);
				break;
		}
		case SN_INT_T:
		{
				__put_int(buffer, &obj->integer);
				break;
		}
		case SN_STRING_T:
		{
				__put_string(buffer, &obj->string);
				break;
		}
		case SN_LIST_T:
		{
				__put_list(buffer, &obj->list);
				break;
		}
		case SN_DICT_T:
		{
				__put_dict(buffer, &obj->dict);
				break;
		}
		}
}

static void __put_list(arBuffer_t	*buffer, const snList_t *lst)
{
		static const char *h = "l";
		static const char *e = "e";
		size_t i;
		AR_ASSERT(buffer != NULL && lst != NULL);

		AR_InsertBuffer(buffer, (const byte_t*)h, 1);

		for(i = 0; i < lst->count; ++i)
		{
				__put_obj(buffer, lst->lst[i]);
		}

		AR_InsertBuffer(buffer, (const byte_t*)e, 1);
}

static void __put_dict(arBuffer_t	*buffer, const snDict_t *dict)
{
		static const char *h = "d";
		static const char *e = "e";
		size_t i;
		AR_ASSERT(buffer != NULL && dict != NULL);
		

		AR_InsertBuffer(buffer, (const byte_t*)h, 1);

		for(i = 0; i < dict->count; ++i)
		{
				__put_string(buffer, &dict->pairs[i].key->string);
				__put_obj(buffer,  dict->pairs[i].val);
		}
		AR_InsertBuffer(buffer, (const byte_t*)e, 1);
}


void			SN_PutObject(arBuffer_t	*buffer, const snObject_t *obj)
{
		AR_ASSERT(buffer != NULL && obj != NULL);
		AR_ClearBuffer(buffer);
		__put_obj(buffer, obj);
}



















/*****************************************************************************************************/


snObject_t*		SN_FindObjectByWcsPath(snObject_t *obj, const wchar_t *path)
{
		
		wchar_t *beg, *p;
		size_t len = 0;
		wchar_t *pbuf = NULL;
		wchar_t	save;
		AR_ASSERT(obj != NULL && path != NULL );

		pbuf = AR_wcsdup(path);
		save = L'\0';

		beg = pbuf;
		p = beg;

		

		while(obj != NULL && *p != L'\0')
		{
				while(*p != L'\0' && *p != L'/') ++p;
				len = p - beg;

				/*
						///a///b///c///d...
						a/b/c
						/b/c
				*/
				

				if(len == 0)
				{
						if(*p == L'\0')
						{
								obj = NULL;
								goto END_POINT;
						}

						AR_ASSERT(*p == L'/');
				}else
				{
						save  = *p;
						*p = 0;

						if(obj->type == SN_LIST_T)
						{
								size_t idx;
								AR_wtou(beg, (uint_t*)&idx, 10);
								obj = SN_GetFromList(&obj->list, idx);

						}else if(obj->type == SN_DICT_T)
						{
								snObject_t		*key = SN_CreateObject(SN_STRING_T);
								SN_SetStringByWcs(&key->string, beg);

								obj = SN_FindFromDict(&obj->dict, key);
								SN_DestroyObject(key);
						}else
						{

								obj = NULL;
								goto END_POINT;
						}
						*p = save;
				}

				if(*p != L'\0')
				{
						beg = ++p;
				}else
				{
						beg = p;
				}
		}

END_POINT:
		if(pbuf != NULL)
		{
				AR_DEL(pbuf);
				pbuf = NULL;
		}

		return obj;
}



snObject_t*		SN_FindObjectByStrPath(snObject_t *obj, const char *path)
{
		wchar_t *wcs = NULL;
		AR_ASSERT(obj != NULL && path != NULL);

		wcs = AR_utf8_convto_wcs(path);

		if(wcs == NULL)
		{
				return NULL;
		}

		obj = SN_FindObjectByWcsPath(obj, wcs);

		AR_DEL(wcs);
		return obj;
}



/***************************************************************************************************************************/


bool_t			SN_InsertToDictObject(snObject_t *obj, snObject_t *key, snObject_t *value)
{
		AR_ASSERT(obj != NULL && obj->type == SN_DICT_T);
		AR_ASSERT(key != NULL && value != NULL);
		
		if(obj->type != SN_DICT_T)
		{
				return false;
		}

		return SN_InsertToDict(&obj->dict, key, value);
}


bool_t			SN_RemoveFromDictObject(snObject_t *obj, const snObject_t *key)
{
		AR_ASSERT(obj != NULL && obj->type == SN_DICT_T);
		AR_ASSERT(key != NULL);
		
		if(obj->type != SN_DICT_T)
		{
				return false;
		}

		return SN_RemoveFromDict(&obj->dict,key);
}


snObject_t*		SN_FindFromDictObject(snObject_t *obj, const snObject_t *key)
{
		AR_ASSERT(obj != NULL && obj->type == SN_DICT_T);
		AR_ASSERT(key != NULL);
		
		if(obj->type != SN_DICT_T)
		{
				return false;
		}

		return SN_FindFromDict(&obj->dict,key);
}




snPair_t*		SN_GetFromDictObject(snObject_t *obj, size_t idx)
{
		AR_ASSERT(obj != NULL && obj->type == SN_DICT_T);
		AR_ASSERT(idx < obj->dict.count);
		return &obj->dict.pairs[idx];

}

size_t			SN_GetDictObjectCount(const snObject_t *obj)
{
		AR_ASSERT(obj != NULL && obj->type == SN_DICT_T);
		return obj->dict.count;
}





void			SN_InsertToListObject(snObject_t	*lst, snObject_t *obj)
{
		AR_ASSERT(lst != NULL && lst->type == SN_LIST_T);
		AR_ASSERT(obj != NULL);
		if(lst->type != SN_LIST_T)
		{
				return;
		}

		SN_InsertToList(&lst->list, obj);
}

bool_t			SN_RemoveFromListObject(snObject_t	*lst, size_t idx)
{
		AR_ASSERT(lst != NULL && lst->type == SN_LIST_T);
		if(lst->type != SN_LIST_T)
		{
				return false;
		}
		
		return SN_RemoveFromList(&lst->list, idx);
}

int_t			SN_IndexOfListObject(const snObject_t *lst, const snObject_t *obj)
{
		AR_ASSERT(lst != NULL && lst->type == SN_LIST_T);
		AR_ASSERT(obj != NULL);
		
		if(lst->type != SN_LIST_T)
		{
				return -1;
		}
		
		return SN_IndexOfList(&lst->list, obj);
}


snObject_t*		SN_GetFromListObject(snObject_t *lst, size_t idx)
{
		AR_ASSERT(lst != NULL && lst->type == SN_LIST_T);
		if(lst->type != SN_LIST_T)
		{
				return NULL;
		}
		
		return SN_GetFromList(&lst->list, idx);
}


size_t			SN_GetListObjectCount(const snObject_t *lst)
{
		AR_ASSERT(lst != NULL && lst->type == SN_LIST_T);
		
		if(lst->type != SN_LIST_T)
		{
				return 0;
		}

		return lst->list.count;
}

void			SN_SetStringObjectByData(snObject_t	*dest, const byte_t *data, size_t len)
{
		AR_ASSERT(dest != NULL && dest->type == SN_STRING_T);
		AR_ASSERT(data != NULL && len > 0);
		
		if(dest->type != SN_STRING_T)
		{
				return;
		}

		SN_SetStringByData(&dest->string, data, len);
}

void			SN_SetStringObjectByStr(snObject_t	*dest, const char *str)
{
		AR_ASSERT(dest != NULL && dest->type == SN_STRING_T);
		AR_ASSERT(str != NULL);

		if(dest->type != SN_STRING_T)
		{
				return;
		}

		SN_SetStringByStr(&dest->string, str);
}

void			SN_SetStringObjectByWcs(snObject_t	*dest, const wchar_t *str)
{

		AR_ASSERT(dest != NULL && dest->type == SN_STRING_T);
		AR_ASSERT(str != NULL);

		if(dest->type != SN_STRING_T)
		{
				return;
		}

		SN_SetStringByWcs(&dest->string, str);
}




int_t			SN_GetDataFromStringObject(const snObject_t	*obj, byte_t *buf, size_t len)
{
		AR_ASSERT(obj != NULL && obj->type == SN_STRING_T);
		if(obj->type != SN_STRING_T)
		{
				return -1;
		}

		if(buf != NULL)
		{
				if(len < obj->string.len)
				{
						return -1;
				}

				AR_memcpy(buf, obj->string.data, obj->string.len);
		}
		
		return (int_t)obj->string.len;
}		


int_t			SN_GetStrFromStringObject(const snObject_t	*obj, char *buf, size_t len)
{
		int_t l;
		
		AR_ASSERT(obj != NULL && obj->type == SN_STRING_T);

		if(obj->type != SN_STRING_T)
		{
				return -1;
		}

		l = SN_GetDataFromStringObject(obj, NULL, 0);
		
		if(l < 0)return l;
		
		if(buf == NULL)
		{
				return l + 1;
		}else
		{
				if((int_t)len < (l + 1))return -1;

				l = SN_GetDataFromStringObject(obj, (byte_t*)buf, l);

				if(l < 0)return l;
				buf[l] = 0;
				return l + 1;
		}
}


int_t			SN_GetWcsFromStringObject(const snObject_t	*obj, wchar_t *buf, size_t len)
{
		int_t l;
		char *tmp;
		wchar_t *wtmp;
		AR_ASSERT(obj != NULL && obj->type == SN_STRING_T);

		if(obj->type != SN_STRING_T)
		{
				return -1;
		}

		l = SN_GetStrFromStringObject(obj, NULL, 0);
		if(l < 0)return l;

		tmp = AR_NEWARR(char, l);
		
		l = SN_GetStrFromStringObject(obj, tmp, l);

		if(l < 0)
		{
				AR_DEL(tmp);
				return -1;
		}

		wtmp = AR_utf8_convto_wcs(tmp);

		if(tmp == NULL)
		{
				AR_DEL(tmp);
				return -1;
		}
		
		if(buf == NULL)
		{
				int_t ret = (int_t)(AR_wcslen(wtmp) + 1);
				AR_DEL(tmp);
				AR_DEL(wtmp);
				return ret;
		}else
		{
				int_t ret = (int_t)(AR_wcslen(wtmp) + 1);

				if((int_t)len < ret)
				{
						AR_DEL(tmp);
						AR_DEL(wtmp);
						return -1;
				}
				AR_wcscpy(buf, wtmp);
				AR_DEL(tmp);
				AR_DEL(wtmp);
				return ret;
		}
}




int_t			SN_CompStringObjectByStringObject(const snObject_t	*l, const snObject_t	*r)
{
		AR_ASSERT(l != NULL && l->type == SN_STRING_T);
		AR_ASSERT(r != NULL && r->type == SN_STRING_T);
		
		if(l->type != SN_STRING_T)
		{
				return -1;
		}

		if(r->type != SN_STRING_T)
		{
				return 1;
		}
		return SN_CompStringByString(&l->string, &r->string);
}


int_t			SN_CompStringObjectByData(const snObject_t	*l,			const byte_t *data, size_t len)
{
		AR_ASSERT(l != NULL && l->type == SN_STRING_T);
		AR_ASSERT(data != NULL && len > 0);
		
		if(l->type != SN_STRING_T)
		{
				return -1;
		}

		return SN_CompStringByData(&l->string, data, len);
}


int_t			SN_CompStringObjectByStr(const snObject_t		*l,		const char *str)
{
		AR_ASSERT(l != NULL && l->type == SN_STRING_T);
		AR_ASSERT(str != NULL);
		
		if(l->type != SN_STRING_T)
		{
				return -1;
		}
		
		return SN_CompStringByStr(&l->string, str);
}


int_t			SN_CompStringObjectByWcs(const snObject_t		*l,		const wchar_t *str)
{
		AR_ASSERT(l != NULL && l->type == SN_STRING_T);
		AR_ASSERT(str != NULL);
		
		if(l->type != SN_STRING_T)
		{
				return -1;
		}
		
		return SN_CompStringByWcs(&l->string, str);

}




void			SN_SetIntObject( snObject_t	*obj,	int_64_t num)
{
		AR_ASSERT(obj != NULL && obj->type == SN_INT_T);

		if(obj->type != SN_INT_T)
		{
				return;
		}

		obj->integer.is_signed = true;
		obj->integer.s = num;
}


void			SN_SetUIntObject(snObject_t	*obj,	uint_64_t num)
{
		AR_ASSERT(obj != NULL && obj->type == SN_INT_T);

		if(obj->type != SN_INT_T)
		{
				return;
		}

		obj->integer.is_signed = false;
		obj->integer.u = num;
}



uint_64_t		SN_GetUIntObject(const snObject_t	*obj)
{
		AR_ASSERT(obj != NULL && obj->type == SN_INT_T);

		if(obj->type != SN_INT_T)
		{
				return 0;
		}

		return obj->integer.u;
}


int_64_t		SN_GetIntObject(const snObject_t	*obj)
{
		AR_ASSERT(obj != NULL && obj->type == SN_INT_T);

		if(obj->type != SN_INT_T)
		{
				return 0;
		}

		return obj->integer.s;
}





AR_NAMESPACE_END
