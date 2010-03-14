#include "serializer.h"



AR_NAMESPACE_BEGIN

/*int*/



void	SN_InitInteger(snInteger_t *dest)
{
		AR_ASSERT(dest != NULL);
		AR_memset(dest, 0, sizeof(*dest));
}

void	SN_UnInitInteger(snInteger_t *dest)
{
		AR_ASSERT(dest != NULL);
		AR_memset(dest, 0, sizeof(*dest));
}




/*string*/
void	SN_InitString(snString_t	*str)
{
		AR_ASSERT(str != NULL);
		AR_memset(str, 0, sizeof(*str));
}

void	SN_UnInitString(snString_t	*str)
{
		if(str->data != NULL)
		{
				AR_DEL(str->data);
		}
		AR_memset(str, 0, sizeof(*str));
}

void	SN_SetStringByData(snString_t	*dest, const ar_byte_t *data, size_t len)
{
		AR_ASSERT(dest != NULL && data != NULL && len > 0);

		if(dest->data != NULL)
		{
				AR_DEL(dest->data);
		}
		dest->data = AR_NEWARR(ar_byte_t, len);
		AR_memcpy(dest->data, data, len);
		dest->len = len;
}

void	SN_SetStringByStr(snString_t	*dest, const char *str)
{
		size_t	l;
		AR_ASSERT(dest != NULL && str != NULL);
		l = AR_strlen(str);
		AR_ASSERT(l > 0);
		SN_SetStringByData(dest, (const ar_byte_t*)str, l);
}

void	SN_SetStringByWcs(snString_t	*dest, const wchar_t *str)
{
		const char *utf8;
		AR_ASSERT(dest != NULL && str != NULL);
		utf8 = AR_wcs_convto_utf8(str);
		AR_ASSERT(utf8 != NULL);
		SN_SetStringByStr(dest, utf8);
		AR_DEL(utf8);
}


ar_int_t	SN_CompStringByString(const snString_t	*l,  const snString_t	*r)
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



ar_int_t	SN_CompStringByData(const snString_t	*l,  const ar_byte_t *data, size_t len)
{
		snString_t		other;
		ar_int_t cmp;
		AR_ASSERT(l != NULL && l->len > 0 && data != NULL && len > 0);

		SN_InitString(&other);
		SN_SetStringByData(&other, data, len);

		cmp = SN_CompStringByString(l, &other);
		SN_UnInitString(&other);
		return cmp;
}

ar_int_t	SN_CompStringByStr(const snString_t		*l,	 const char *str)
{
		snString_t		other;
		ar_int_t cmp;
		AR_ASSERT(l != NULL && l->len > 0 && str != NULL);

		SN_InitString(&other);
		SN_SetStringByStr(&other, str);
		cmp = SN_CompStringByString(l, &other);
		SN_UnInitString(&other);
		return cmp;
}

ar_int_t	SN_CompStringByWcs(const snString_t		*l,	 const wchar_t *str)
{
		snString_t		other;
		ar_int_t cmp;
		AR_ASSERT(l != NULL && l->len > 0 && str != NULL);

		SN_InitString(&other);
		SN_SetStringByWcs(&other, str);
		cmp = SN_CompStringByString(l, &other);
		SN_UnInitString(&other);
		return cmp;
}


/*list*/

void	SN_InitList(snList_t	*lst)
{
		AR_ASSERT(lst != NULL);
		AR_memset(lst, 0, sizeof(*lst));
}

void	SN_UnInitList(snList_t	*lst)
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


void	SN_InsertToList(snList_t	*lst, snObject_t *obj)
{
		AR_ASSERT(lst != NULL && obj != NULL);

		if(lst->count == lst->cap)
		{
				lst->cap = (lst->cap + 4) * 2;
				lst->lst = AR_REALLOC(snObject_t*, lst->lst, lst->cap);
		}
		lst->lst[lst->count++] = obj;
}




ar_bool_t	SN_RemoveFromList(snList_t	*lst, size_t idx)
{
		
		AR_ASSERT(lst != NULL);
		if(idx >= lst->count)return false;

		SN_DestroyObject(lst->lst[idx]);
		lst->lst[idx] = lst->lst[lst->count-1];
		lst->count--;
		return true;
}


ar_int_t	SN_IndexOfList(const snList_t *lst, const snObject_t *obj)
{
		ar_int_t i;
		AR_ASSERT(lst != NULL && obj != NULL);

		for(i = 0; i < (ar_int_t)lst->count; ++i)
		{
				if(obj == lst->lst[i])return i;
		}
		return -1;
}


snObject_t*		SN_GetObjectFromList(snList_t *lst, size_t idx)
{
		AR_ASSERT(lst != NULL);
		if(idx >= lst->count)return NULL;
		return lst->lst[idx];
}






/*dict*/
void	SN_InitDict(snDict_t *dict)
{
		AR_ASSERT(dict != NULL);
		AR_memset(dict, 0, sizeof(*dict));
}

void	SN_UnInitDict(snDict_t *dict)
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


ar_bool_t	SN_InsertToDict(snDict_t *dict, snObject_t *key, snObject_t *value)
{
		AR_ASSERT(dict != NULL && key != NULL && key->type == SN_STRING_T && value != NULL);

		if(SN_FindFromDict(dict, key) != NULL)return false;
		
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

ar_bool_t	SN_RemoveFromDict(snDict_t *dict, const snObject_t *key)
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

snObject_t* SN_FindFromDict(snDict_t *dict, const snObject_t *key)
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





snObject_t*	__get_int(arBuffer_t	*buffer)
{
		snObject_t		*ret = NULL;
		size_t	buf_len, idx;
		const ar_byte_t	*pbuf;
		
		ar_uint64_t		num = 0;
		ar_bool_t			is_neg = false;
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
				ret->integer.s = 0 - (ar_int64_t)num;
		}else
		{
				ret->integer.u = num;
		}
		AR_EraseBuffer(buffer, idx + 1);
		return ret;
}

snObject_t*		__get_str(arBuffer_t	*buffer)
{
		snObject_t		*ret = NULL;
		size_t	buf_len, idx;
		const ar_byte_t	*pbuf;
		
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

snObject_t*		__get_list(arBuffer_t	*buffer);
snObject_t*		__get_dict(arBuffer_t	*buffer);

snObject_t*		__get_obj(arBuffer_t	*buffer)
{
		ar_byte_t b;
		AR_ASSERT(buffer != NULL && AR_GetBufferAvailable(buffer) > 0);

		b = *(AR_GetBufferData(buffer));
		
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


snObject_t*		__get_list(arBuffer_t	*buffer)
{
		const ar_byte_t	*pb;
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


snObject_t*		__get_dict(arBuffer_t	*buffer)
{
		const ar_byte_t	*pb;
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
RE_INSERT:
				if(!SN_InsertToDict(&ret->dict, key, val))
				{
						SN_RemoveFromDict(&ret->dict, key);
						goto RE_INSERT;
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




void		__put_int(arBuffer_t	*buffer, const snInteger_t *integer)
{
		char buf[256];
		char *p;
		ar_bool_t	is_neg = false;
		ar_uint64_t		num;
		static const char _tbl[] = "0123456789";

		AR_ASSERT(buf != NULL && integer != NULL);

		if(integer->is_signed)
		{
				if(integer->s < 0)
				{
						is_neg = true;
						num = (ar_uint64_t)(-integer->s);
				}else
				{
						num = (ar_uint64_t)integer->s;
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
		AR_InsertBuffer(buffer, (const ar_byte_t*)p, AR_strlen(p));
}

void __put_string(arBuffer_t	*buffer, const snString_t *string)
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

		AR_InsertBuffer(buffer, (const ar_byte_t*)p, AR_strlen(p));
		AR_InsertBuffer(buffer, string->data, string->len);
}


void __put_list(arBuffer_t	*buffer, const snList_t *lst);
void __put_dict(arBuffer_t	*buffer, const snDict_t *dict);

void __put_obj(arBuffer_t	*buffer, const snObject_t *obj)
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

void __put_list(arBuffer_t	*buffer, const snList_t *lst)
{
		static const char *h = "l";
		static const char *e = "e";
		size_t i;
		AR_ASSERT(buffer != NULL && lst != NULL);

		AR_InsertBuffer(buffer, (const ar_byte_t*)h, 1);

		for(i = 0; i < lst->count; ++i)
		{
				__put_obj(buffer, lst->lst[i]);
		}

		AR_InsertBuffer(buffer, (const ar_byte_t*)e, 1);
}

void __put_dict(arBuffer_t	*buffer, const snDict_t *dict)
{
		static const char *h = "d";
		static const char *e = "e";
		size_t i;
		AR_ASSERT(buffer != NULL && dict != NULL);
		

		AR_InsertBuffer(buffer, (const ar_byte_t*)h, 1);

		for(i = 0; i < dict->count; ++i)
		{
				__put_string(buffer, &dict->pairs[i].key->string);
				__put_obj(buffer,  dict->pairs[i].val);
		}
		AR_InsertBuffer(buffer, (const ar_byte_t*)e, 1);
}


void			SN_PutObject(arBuffer_t	*buffer, const snObject_t *obj)
{
		AR_ASSERT(buffer != NULL && obj != NULL);
		AR_ClearBuffer(buffer);
		__put_obj(buffer, obj);
}



















/*****************************************************************************************************/


snObject_t*		SN_FindPathByWcs(snObject_t *pobj, const wchar_t *path)
{
		const wchar_t *p, *beg;
		size_t len = 0;
		wchar_t *pbuf = NULL;
		
		AR_ASSERT(pobj != NULL && path != NULL);

		beg = path;
		p = beg;

		while(pobj != NULL)
		{
				while(*p != L'/' && *p != L'\0') ++p;
				len = p - beg;
				pbuf = AR_NEWARR0(wchar_t, len + 1);
				AR_wcsncpy(pbuf, beg, len);
				pbuf[len] = L'\0';
				
				if(pobj->type == SN_LIST_T)
				{
						
						size_t idx;
						AR_wtou(pbuf, (ar_uint_t*)&idx, 10);
						pobj = SN_GetObjectFromList(&pobj->list, idx);

				}else if(pobj->type == SN_DICT_T)
				{
						snObject_t		*key = SN_CreateObject(SN_STRING_T);
						SN_SetStringByWcs(&key->string, pbuf);

						pobj = SN_FindFromDict(&pobj->dict, key);
						SN_DestroyObject(key);
				}else
				{
						AR_DEL(pbuf);
						return NULL;
				}
				
				AR_DEL(pbuf);
				pbuf = NULL;
				if(*p == L'\0')break;
				beg = ++p;
		}
		return pobj;

}



snObject_t*		SN_FindPathByStr(snObject_t *pobj, const char *path)
{
		const char *p, *beg;
		size_t len = 0;
		char *pbuf = NULL;
		
		AR_ASSERT(pobj != NULL && path != NULL);

		beg = path;
		p = beg;

		while(pobj != NULL)
		{
				while(*p != '/' && *p != '\0') ++p;
				len = p - beg;
				pbuf = AR_NEWARR0(char, len + 1);
				AR_memcpy(pbuf, beg, len);
				pbuf[len] = '\0';
				
				if(pobj->type == SN_LIST_T)
				{
						size_t idx = (size_t)atoi(pbuf);
						pobj = SN_GetObjectFromList(&pobj->list, idx);

				}else if(pobj->type == SN_DICT_T)
				{
						snObject_t		*key = SN_CreateObject(SN_STRING_T);
						SN_SetStringByStr(&key->string, pbuf);

						pobj = SN_FindFromDict(&pobj->dict, key);
						SN_DestroyObject(key);
				}else
				{
						AR_DEL(pbuf);
						return NULL;
				}
				
				AR_DEL(pbuf);
				pbuf = NULL;
				if(*p == '\0')break;
				beg = ++p;
		}
		return pobj;
}



AR_NAMESPACE_END
