//
//  property_list_internal.c
//  Arsenal
//
//  Created by solidus on 13-8-23.
//  Copyright (c) 2013年 none. All rights reserved.
//


#include "property_list_internal.h"

AR_NAMESPACE_BEGIN

/*****plistString_t***********/


arStatus_t      PList_InitString(plistString_t  *str)
{
        AR_ASSERT(str != NULL);
        
        AR_memset(str, 0, sizeof(*str));
        
        str->str = AR_CreateString();
        
        if(str->str == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                return AR_E_NOMEM;
        }else
        {
                return AR_S_YES;
        }
}

void            PList_UnInitString(plistString_t  *str)
{
        AR_ASSERT(str != NULL && str->str != NULL);
        
        AR_DestroyString(str->str);
        str->str = NULL;
        AR_memset(str, 0, sizeof(*str));
}

arStatus_t      PList_ReserveString(plistString_t  *str, size_t cap)
{
        AR_ASSERT(str != NULL);
        return AR_ReserveString(str->str, cap);
}

arStatus_t      PList_CopyString(plistString_t  *dest, plistString_t  *src)
{
        AR_ASSERT(dest != NULL && src != NULL);
        return AR_CopyString(dest->str, src->str);
}

ar_int_t        PList_CompareString(const plistString_t  *l, const plistString_t  *r)
{
        AR_ASSERT(l != NULL && r != NULL);
        AR_ASSERT(l->str && r->str);
        return AR_CompString(l->str, r->str);
}

ar_int_t        PList_CompareStringWithWcs(const plistString_t  *l, const wchar_t *wcs)
{
        AR_ASSERT(l != NULL && wcs != NULL);
        return AR_CompStringWithWcs(l->str, wcs);
}


arStatus_t      PList_AppendString(plistString_t  *str, const wchar_t *wcs)
{
        AR_ASSERT(str != NULL && str->str != NULL && wcs != NULL);
        return AR_AppendString(str->str, wcs);
}

arStatus_t      PList_AppendStringN(plistString_t  *str, const wchar_t *wcs, size_t n)
{
        AR_ASSERT(str != NULL && str->str != NULL && wcs != NULL);
        return AR_AppendStringN(str->str, wcs, n);
}


arStatus_t      PList_AppendCharToString(plistString_t *str, wchar_t ch)
{
        AR_ASSERT(str != NULL && str->str != NULL);
        return AR_AppendCharToString(str->str, ch);
}


ar_bool_t       PList_IsEmptyString(const plistString_t  *str)
{
        AR_ASSERT(str != NULL);
        return AR_IsEmptyString(str->str);
}

const wchar_t*  PList_GetStringCString(const plistString_t *str)
{
        AR_ASSERT(str != NULL && str->str != NULL);
        return AR_CSTR(str->str);
}

size_t          PList_GetStringLength(const plistString_t *str)
{
        AR_ASSERT(str != NULL && str->str != NULL);
        return AR_GetStringLength(str->str);
}

/**********plistDate_t**********/

ar_int_t PList_CompNumber(const plistNumber_t *l, const plistNumber_t *r)
{
        AR_ASSERT(l != NULL && r != NULL);
        
        if(l->type != r->type)
        {
                return AR_CMP(l->type, r->type);
        }
        
        if(l->type == PLIST_NUMBER_INTEGER_T)
        {
                if(l->integer.is_signed)
                {
                        if(r->integer.is_signed)
                        {
                                return AR_CMP(l->integer.signed_num, r->integer.signed_num);
                        }else
                        {
                                return AR_CMP(l->integer.signed_num, (ar_int_64_t)r->integer.unsigned_num);
                        }
                        
                        
                }else
                {
                        if(!r->integer.is_signed)
                        {
                                return AR_CMP(l->integer.unsigned_num, r->integer.unsigned_num);
                        }else
                        {
                                return AR_CMP(l->integer.unsigned_num, (ar_uint_64_t)r->integer.signed_num);
                        }
                }
                
        }else   //PLIST_NUMBER_REAL_T
        {
                return AR_CMP_DBL(l->real.num, r->real.num);
        }
}


/*****plistData_t***********/

arStatus_t      PList_InitData(plistData_t *data)
{
        AR_ASSERT(data != NULL);
        
        AR_memset(data, 0, sizeof(*data));
        
        data->buf = AR_CreateBuffer(0);
        
        if(data->buf == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                return AR_E_NOMEM;
        }else
        {
                return AR_S_YES;
        }
}

void            PList_UnInitData(plistData_t *data)
{
        AR_ASSERT(data != NULL && data->buf != NULL);
        
        AR_DestroyBuffer(data->buf);
        data->buf = NULL;
        AR_memset(data, 0, sizeof(*data));
}

ar_int_t                PList_CompData(const plistData_t *l, const plistData_t *r)
{
        size_t ll, lr;
        AR_ASSERT(l != NULL && r != NULL);
        
        if(l == r)
        {
                return 0;
        }
        
        ll = AR_GetBufferAvailable(l->buf);
        lr = AR_GetBufferAvailable(r->buf);
        
        
        if(ll != lr)
        {
                return AR_CMP(ll,lr);
        }
        
        
        if(ll == 0)
        {
                return 0;
        }else
        {
                return AR_memcmp(AR_GetBufferData(l->buf), AR_GetBufferData(r->buf), ll);
        }
}



arStatus_t              PList_CopyData(plistData_t *dest, plistData_t *src)
{
        AR_ASSERT(dest != NULL && src != NULL);
        return AR_CopyBuffer(dest->buf, src->buf);
}


const ar_byte_t*        PList_GetDataPointer(const plistData_t *data)
{
        AR_ASSERT(data != NULL);
        
        if(PList_GetDataLength(data) == 0)
        {
                return NULL;
        }else
        {
                return AR_GetBufferData(data->buf);
        }
}


size_t                  PList_GetDataLength(const plistData_t *data)
{
        AR_ASSERT(data != NULL);
        return AR_GetBufferAvailable(data->buf);
}


arStatus_t              PList_SetData(plistData_t *data, const ar_byte_t *buf, size_t length)
{
        AR_ASSERT(data != NULL);
        
        AR_ClearBuffer(data->buf);
        return AR_InsertToBuffer(data->buf, buf, length);
}


/******plistDate_t*************/

ar_int_t        PList_CompDate(const plistDate_t *l, const plistDate_t *r)
{
        AR_ASSERT(l != NULL && r != NULL);
        
        if(l->year != r->year)
        {
                return AR_CMP(l->year, r->year);
        }
        
        if(l->month != r->month)
        {
                return AR_CMP(l->month, r->month);
        }
        
        if(l->day != r->day)
        {
                return AR_CMP(l->day, r->day);
        }
        
        if(l->hour != r->hour)
        {
                return AR_CMP(l->hour, r->hour);
        }
        
        if(l->minute != r->minute)
        {
                return AR_CMP(l->minute, r->minute);
        }
        
        if(l->second != r->second)
        {
                return AR_CMP(l->second, r->second);
        }
        
        return 0;
}

/*****plistArray_t***********/

void      PList_InitArray(plistArray_t *arr)
{
        AR_ASSERT(arr != NULL);
        AR_memset(arr, 0, sizeof(*arr));
}

void            PList_UnInitArray(plistArray_t *arr, ar_bool_t dont_clear_items)
{
        
        AR_ASSERT(arr != NULL);
        
        if(!dont_clear_items)
        {
                PList_ClearArray(arr);
        }
        
        if(arr->items)
        {
                AR_DEL(arr->items);
                arr->items = NULL;
        }
        
        AR_memset(arr, 0, sizeof(*arr));
}




void            PList_ClearArray(plistArray_t *arr)
{
        size_t i;
        AR_ASSERT(arr != NULL);
        
        for(i = 0; i < arr->count; ++i)
        {
                PList_DestroyElem(arr->items[i]);
                arr->items[i] = NULL;
        }
        arr->count = 0;
}


arStatus_t      PList_CopyArray(plistArray_t *dest, plistArray_t *src)
{
        size_t i;
        
        AR_ASSERT(dest != NULL && src != NULL);
        PList_ClearArray(dest);
        
        for(i = 0; i < src->count; ++i)
        {
                arStatus_t status;
                plistElem_t *new_item = PList_CopyNewElem(src->items[i]);
                
                if(new_item == NULL)
                {
                        PList_ClearArray(dest);
                        return AR_E_NOMEM;
                }
                
                status = PList_PushToArray(dest, new_item);
                
                if(status != AR_S_YES)
                {
                        PList_DestroyElem(new_item);
                        new_item = NULL;
                        PList_ClearArray(dest);
                        return status;
                }
        }
        
        return AR_S_YES;
        
}

arStatus_t      PList_PushToArray(plistArray_t *arr, plistElem_t *elem)
{
        AR_ASSERT(arr != NULL && elem != NULL);
        
        if(arr->count == arr->cap)
		{
				size_t          new_cap;
				plistElem_t     **new_items;
                
				new_cap = (arr->cap + 4) * 2;
				new_items = AR_NEWARR(plistElem_t*, new_cap);
                
				if(new_items == NULL)
				{
						return AR_E_NOMEM;
				}
                
				if(arr->count > 0)
				{
						AR_memcpy(new_items, arr->items, arr->count * sizeof(plistElem_t*));
				}
                
				if(arr->items)
				{
						AR_DEL(arr->items);
						arr->items = NULL;
				}
				arr->cap = new_cap;
				arr->items = new_items;
		}
        
        arr->items[arr->count++] = elem;
		return AR_S_YES;
}

plistElem_t*    PList_GetArrayByIndex(plistArray_t *arr, size_t idx)
{
        AR_ASSERT(arr != NULL && idx < arr->count);
        return arr->items[idx];
}

ar_bool_t      PList_RemoveArrayByIndex(plistArray_t *arr, size_t idx)
{
        AR_ASSERT(arr != NULL);
		if(idx >= arr->count)
		{
				return false;
		}
        
		PList_DestroyElem(arr->items[idx]);
		arr->items[idx] = arr->items[arr->count-1];
		arr->count--;
		return true;
}

ar_int_t        PList_CompArray(const plistArray_t *l, const plistArray_t *r)
{
        size_t i;
        AR_ASSERT(l != NULL && r != NULL);
        
        if(l == r)
        {
                return 0;
        }
        
        if(l->count != r->count)
        {
                return AR_CMP(l->count, r->count);
        }
        
        for(i = 0; i < l->count; ++i)
        {
                ar_int_t cmp;
                cmp = PList_CompElem(l->items[i], r->items[i]);
                
                if(cmp != 0)
                {
                        return cmp;
                }
        }
        
        return 0;
}

/*****plistDict_t***********/

void      PList_InitDict(plistDict_t *dict)
{
        AR_ASSERT(dict != NULL);
        AR_memset(dict, 0, sizeof(*dict));
        
        
        
}

void            PList_UnInitDict(plistDict_t *dict)
{
        AR_ASSERT(dict != NULL);
        PList_ClearDict(dict);
        
        if(dict->keys)
        {
                AR_DEL(dict->keys);
                dict->keys = NULL;
        }
        
        if(dict->values)
        {
                AR_DEL(dict->values);
                dict->values = NULL;
        }
        AR_memset(dict, 0, sizeof(*dict));
}


void            PList_ClearDict(plistDict_t *dict)
{
        size_t i;
        AR_ASSERT(dict != NULL);
        
        for(i = 0; i < dict->count; ++i)
        {
                PList_DestroyElem(dict->keys[i]);
                dict->keys[i] = NULL;
                PList_DestroyElem(dict->values[i]);
                dict->values[i] = NULL;
        }
        dict->count = 0;
}

arStatus_t      PList_CopyDict(plistDict_t *dest, plistDict_t *src)
{
        size_t i;
        
        AR_ASSERT(dest != NULL && src != NULL);
        PList_ClearDict(dest);
        
        for(i = 0; i < src->count; ++i)
        {
                arStatus_t status;
                plistElem_t *new_key = PList_CopyNewElem(src->keys[i]);
                plistElem_t *new_val = PList_CopyNewElem(src->values[i]);
                
                if(new_key == NULL || new_val == NULL)
                {
                        if(new_key)
                        {
                                PList_DestroyElem(new_key);
                                new_key = NULL;
                        }
                        
                        if(new_val)
                        {
                                PList_DestroyElem(new_val);
                                new_val = NULL;
                        }
                        
                        PList_ClearDict(dest);
                        return AR_E_NOMEM;
                }
                
                status = PList_SetDictValueForKey(dest, new_key, new_val);
                
                if(status != AR_S_YES)
                {
                        
                        PList_DestroyElem(new_key);
                        new_key = NULL;
                        PList_DestroyElem(new_val);
                        new_val = NULL;
                        PList_ClearDict(dest);
                        return status;
                }
        }
        
        return AR_S_YES;
        
}


ar_int_t        PList_CompDict(const plistDict_t *l, const plistDict_t *r)
{
        size_t i;
        AR_ASSERT(l != NULL && r != NULL);
        
        if(l == r)
        {
                return 0;
        }
        
        if(l->count != r->count)
        {
                return AR_CMP(l->count, r->count);
        }
        
        for(i = 0; i < l->count; ++i)
        {
                const plistElem_t *lk, *lv, *rk, *rv;
                ar_int_t cmp;
                
                lk = l->keys[i];
                lv = l->values[i];
                
                rk = r->keys[i];
                rv = r->values[i];
                
                cmp = PList_CompElem(lk,rk);
                
                if(cmp != 0)
                {
                        return cmp;
                }
                
                
                cmp = PList_CompElem(lv,rv);
                
                if(cmp != 0)
                {
                        return cmp;
                }
                
        }
        return 0;
        
}


plistElem_t*    PList_FindValueByElem(plistDict_t *dict, const plistElem_t *key)
{
        size_t i;
        AR_ASSERT(dict != NULL && key != NULL);
        AR_ASSERT(PList_GetElemType(key) == PLIST_ELEM_STRING_T);
        
        for(i = 0; i < dict->count; ++i)
        {
                ar_int_t cmp;
                AR_ASSERT(PList_GetElemType(dict->keys[i]) == PLIST_ELEM_STRING_T);
                cmp = PList_CompareString(&dict->keys[i]->str, &key->str);
                
                if(cmp == 0)
                {
                        return dict->values[i];
                }
        }
        return NULL;
}

plistElem_t*    PList_FindValueByString(plistDict_t *dict, const wchar_t *key)
{
        size_t i;
        AR_ASSERT(dict != NULL && key != NULL);
        
        for(i = 0; i < dict->count; ++i)
        {
                ar_int_t cmp;
                AR_ASSERT(PList_GetElemType(dict->keys[i]) == PLIST_ELEM_STRING_T);
                cmp = PList_CompareStringWithWcs(&dict->keys[i]->str, key);
                
                if(cmp == 0)
                {
                        return dict->values[i];
                }
        }
        return NULL;
}

arStatus_t      PList_SetDictValueForKey(plistDict_t *dict, plistElem_t *key, plistElem_t *val)
{
        size_t i;
		AR_ASSERT(dict != NULL && key != NULL && PList_GetElemType(key) == PLIST_ELEM_STRING_T && val != NULL);
        
		for(i = 0; i < dict->count; ++i)
		{
				AR_ASSERT(dict->keys[i] != NULL && dict->values[i] != NULL);
                AR_ASSERT(PList_GetElemType(dict->keys[i]) == PLIST_ELEM_STRING_T);
                
				if(PList_CompareString(&dict->keys[i]->str, &key->str) == 0)
				{
						break;
				}
		}
        
		if(i < dict->count)
		{
                PList_DestroyElem(dict->keys[i]);
                dict->keys[i] = key;
                
                PList_DestroyElem(dict->values[i]);
                dict->values[i] = val;
                
                
				return AR_S_YES;
		}else
		{
				if(dict->count == dict->cap)
				{
						size_t			new_cap;
						plistElem_t		**new_keys, **new_values;
                        
						new_cap = (dict->cap + 4) * 2;
						new_keys = AR_NEWARR(plistElem_t*, new_cap);
						new_values = AR_NEWARR(plistElem_t*, new_cap);
                        
						if(new_keys == NULL || new_values == NULL)
						{
                                if(new_keys)
                                {
                                        AR_DEL(new_keys);
                                        new_keys = NULL;
                                }
                                if(new_values)
                                {
                                        AR_DEL(new_values);
                                        new_values = NULL;
                                }
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
								return AR_E_NOMEM;
						}
                        
						if(dict->count > 0)
						{
								AR_memcpy(new_keys, dict->keys, dict->count * sizeof(plistElem_t*));
								AR_memcpy(new_values, dict->values, dict->count * sizeof(plistElem_t*));
						}
                        
						if(dict->keys)
						{
								AR_DEL(dict->keys);
								dict->keys = NULL;
						}
                        
                        if(dict->values)
                        {
                                AR_DEL(dict->values);
                                dict->values = NULL;
                        }
                        
						dict->cap = new_cap;
                        dict->keys = new_keys;
                        dict->values = new_values;
				}
                
				dict->keys[dict->count] = key;
				dict->values[dict->count] = val;
				dict->count++;
                
#if defined(AR_DEBUG)
				for(i = 0; i < dict->count; ++i)
				{
						AR_ASSERT(dict->keys[i] != NULL && dict->values[i] != NULL);
				}
#endif
				
				return AR_S_YES;
		}
}

arStatus_t      PList_RemoveValueForKey(plistDict_t *dict, plistElem_t *key)
{
        size_t i;
		AR_ASSERT(dict != NULL && key != NULL && PList_GetElemType(key) == PLIST_ELEM_STRING_T);
        
		for(i = 0; i < dict->count; ++i)
		{
				AR_ASSERT(dict->keys[i] != NULL && dict->values[i] != NULL);
                AR_ASSERT(PList_GetElemType(dict->keys[i]) == PLIST_ELEM_STRING_T);
                
				if(PList_CompareString(&dict->keys[i]->str, &key->str) == 0)
				{
						break;
				}
		}
        
        
		if(i >= dict->count)
		{
                return AR_E_NOTFOUND;
		}
        
        PList_DestroyElem(dict->keys[i]);
        PList_DestroyElem(dict->values[i]);
        
        dict->keys[i] = dict->keys[dict->count-1];
        dict->values[i] = dict->values[dict->count-1];
        
		dict->count--;
		return AR_S_YES;
}

const wchar_t*  PList_GetDictKeyWcsByIndex(plistDict_t *dict, size_t idx)
{
		AR_ASSERT(dict != NULL);
        AR_ASSERT(idx < dict->count);
        return PList_GetElemCString(dict->keys[idx]);
        
}
plistElem_t*    PList_GetDictValueByIndex(plistDict_t *dict, size_t idx)
{
        AR_ASSERT(dict != NULL);
        AR_ASSERT(idx < dict->count);
        return dict->values[idx];
}


plistElem_t*    PList_GetDictKeyByIndex(plistDict_t *dict, size_t idx)
{
        AR_ASSERT(dict != NULL);
        AR_ASSERT(idx < dict->count);
        return dict->keys[idx];
}



/********************************************************PList elements********************************************************/


/*plistElem_t*/


plistElem_t*    PList_CreateElem(plistElemType_t        t)
{
        
        plistElem_t *elem = NULL;
        
        AR_ASSERT(t < PLIST_ELEM_MAX_T);
        
        elem = AR_NEW0(plistElem_t);
        
        if(elem == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        elem->type = t;
        
        switch(elem->type)
        {
                case PLIST_ELEM_STRING_T:
                        if(PList_InitString(&elem->str) != AR_S_YES)
                        {
                                goto INVALID_POINT;
                        }
                        break;
                case PLIST_ELEM_BOOLEAN_T:
                        break;
                case PLIST_ELEM_NUMBER_T:
                        break;
                case PLIST_ELEM_DATA_T:
                        if(PList_InitData(&elem->data) != AR_S_YES)
                        {
                                goto INVALID_POINT;
                        }
                        break;
                case PLIST_ELEM_DATE_T:
                        break;
                case PLIST_ELEM_ARRAY_T:
                        PList_InitArray(&elem->array);
                        break;
                case PLIST_ELEM_DICT_T:
                        PList_InitDict(&elem->dict);
                        break;
                case PLIST_ELEM_MAX_T:
                default:
                        AR_error(AR_ERR_WARNING, L"invalid plist element type : %u\r\n", t);
                        goto INVALID_POINT;
        }
        
        return elem;
        
INVALID_POINT:
        if(elem)
        {
                AR_DEL(elem);
                elem = NULL;
        }
        return NULL;
}


plistElem_t*    PList_CopyNewElem(plistElem_t            *elem)
{
        plistElem_t *new_elem;
        AR_ASSERT(elem != NULL);
        
        new_elem = PList_CreateElem(PList_GetElemType(elem));
        
        if(new_elem == NULL)
        {
                return NULL;
        }
        
        
        switch(PList_GetElemType(elem))
        {
                case PLIST_ELEM_BOOLEAN_T:
                        AR_memcpy(&new_elem->boolean, &elem->boolean, sizeof(elem->boolean));
                        break;
                case PLIST_ELEM_NUMBER_T:
                        AR_memcpy(&new_elem->number, &elem->number, sizeof(elem->number));
                        break;
                case PLIST_ELEM_DATE_T:
                        AR_memcpy(&new_elem->date, &elem->date, sizeof(elem->date));
                        break;
                case PLIST_ELEM_STRING_T:
                        if(PList_CopyString(&new_elem->str, &elem->str) != AR_S_YES)
                        {
                                PList_DestroyElem(new_elem);
                                new_elem = NULL;
                        }
                        break;
                case PLIST_ELEM_DATA_T:
                        if(PList_CopyData(&new_elem->data, &elem->data) != AR_S_YES)
                        {
                                PList_DestroyElem(new_elem);
                                new_elem = NULL;
                        }
                        break;
                case PLIST_ELEM_ARRAY_T:
                        if(PList_CopyArray(&new_elem->array, &elem->array) != AR_S_YES)
                        {
                                PList_DestroyElem(new_elem);
                                new_elem = NULL;
                        }
                        break;
                case PLIST_ELEM_DICT_T:
                        if(PList_CopyDict(&new_elem->dict, &elem->dict) != AR_S_YES)
                        {
                                PList_DestroyElem(new_elem);
                                new_elem = NULL;
                        }
                        break;
                case PLIST_ELEM_MAX_T:
                default:
                        AR_error(AR_ERR_WARNING, L"invalid plist element type : %u\r\n", PList_GetElemType(elem));
                        AR_ASSERT(false);
        }
        
        return new_elem;
}


ar_int_t        PList_CompElem(const plistElem_t *l, const plistElem_t *r)
{
        
        AR_ASSERT(l != NULL && r != NULL);
        
        if(l == r)
        {
                return 0;
        }
        
        if(PList_GetElemType(l) != PList_GetElemType(r))
        {
                return AR_CMP(PList_GetElemType(l),PList_GetElemType(r));
        }
        
        
        switch(PList_GetElemType(l))
        {
                case PLIST_ELEM_BOOLEAN_T:
                        return AR_CMP(l->boolean.val, r->boolean.val);
                case PLIST_ELEM_NUMBER_T:
                        return PList_CompNumber(&l->number, &r->number);
                case PLIST_ELEM_DATE_T:
                        return PList_CompDate(&l->date, &r->date);
                case PLIST_ELEM_STRING_T:
                        return PList_CompareString(&l->str, &r->str);
                case PLIST_ELEM_DATA_T:
                        return PList_CompData(&l->data, &r->data);
                case PLIST_ELEM_ARRAY_T:
                        return PList_CompArray(&l->array, &r->array);
                case PLIST_ELEM_DICT_T:
                        return PList_CompDict(&l->dict, &r->dict);
                case PLIST_ELEM_MAX_T:
                default:
                        AR_error(AR_ERR_WARNING, L"invalid plist element type : %u\r\n", PList_GetElemType(l));
                        AR_ASSERT(false);
                        return 0;
        }
}



ar_uint_64_t    PList_HashElem(const plistElem_t *elem)
{
        AR_ASSERT(elem != NULL);
        
        
        
        switch(PList_GetElemType(elem))
        {
                case PLIST_ELEM_BOOLEAN_T:
                        if(elem->boolean.val)
                        {
                                return 1;
                        }else
                        {
                                return 0;
                        }
                case PLIST_ELEM_NUMBER_T:
                        
                        if(elem->number.type == PLIST_NUMBER_INTEGER_T)
                        {
                                return elem->number.integer.unsigned_num;
                        }else
                        {
                                return AR_memhash((const ar_byte_t*)&elem->number.real,  sizeof(elem->number.real));
                        }
                case PLIST_ELEM_DATE_T:
                {
                        return (ar_uint_64_t)(elem->date.year + elem->date.month + elem->date.day + elem->date.hour + elem->date.minute + elem->date.second);
                }
                case PLIST_ELEM_STRING_T:
                        return AR_wcshash(PList_GetElemCString(elem));
                case PLIST_ELEM_DATA_T:
                        if(PList_GetElemDataPointer(elem) == NULL)
                        {
                                return 0;
                        }else
                        {
                                return AR_memhash(PList_GetElemDataPointer(elem), PList_GetElemDataLength(elem));
                        }
                case PLIST_ELEM_ARRAY_T:
                {
                        
                        //return (ar_uint_64_t)elem;
                        
                        plistElem_t *first_val = NULL;
                        
                        if(PList_GetElemArrayCount(elem) > 0)
                        {
                                first_val = PList_GetElemArrayByIndex((plistElem_t*)elem,0);
                                AR_ASSERT(first_val != NULL);
                                return PList_HashElem(first_val);
                        }else
                        {
                                return 0;
                        }
                }
                        break;
                case PLIST_ELEM_DICT_T:
                {
                        //return (ar_uint_64_t)elem;
                        
                        plistElem_t *first_key = NULL;
                        
                        if(PList_GetElemDictCount(elem) > 0)
                        {
                                first_key = PList_GetElemDictKeyByIndex((plistElem_t*)elem, 0);
                                AR_ASSERT(first_key != NULL);
                                return PList_HashElem(first_key);
                        }else
                        {
                                return 0;
                        }
                        
                }
                        break;
                case PLIST_ELEM_MAX_T:
                default:
                        AR_error(AR_ERR_WARNING, L"invalid plist element type : %u\r\n", PList_GetElemType(elem));
                        AR_ASSERT(false);
                        return 0;
        }
}




void            PList_DestroyElem(plistElem_t            *elem)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) < PLIST_ELEM_MAX_T);
        
        
        switch(PList_GetElemType(elem))
        {
                case PLIST_ELEM_STRING_T:
                        PList_UnInitString(&elem->str);
                        break;
                case PLIST_ELEM_BOOLEAN_T:
                        break;
                case PLIST_ELEM_NUMBER_T:
                        break;
                case PLIST_ELEM_DATA_T:
                        PList_UnInitData(&elem->data);
                        break;
                case PLIST_ELEM_DATE_T:
                        break;
                case PLIST_ELEM_ARRAY_T:
                        PList_UnInitArray(&elem->array, false);
                        break;
                case PLIST_ELEM_DICT_T:
                        PList_UnInitDict(&elem->dict);
                        break;
                case PLIST_ELEM_MAX_T:
                default:
                        AR_error(AR_ERR_WARNING, L"invalid plist element type : %u\r\n", PList_GetElemType(elem));
        }
        
        if(elem)
        {
                AR_DEL(elem);
                elem = NULL;
        }
        
}



ar_bool_t               PList_GetElemBooleanValue(const plistElem_t *elem)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_BOOLEAN_T);
        return elem->boolean.val;
}

void                    PList_SetElemBooleanValue(plistElem_t *elem, ar_bool_t val)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_BOOLEAN_T);
        elem->boolean.val = val;
}



const wchar_t*  PList_GetElemCString(const plistElem_t *elem)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_STRING_T);
        return PList_GetStringCString(&elem->str);
}


arStatus_t              PList_AppendCharToElemString(plistElem_t *elem, wchar_t ch)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_STRING_T);
        return PList_AppendCharToString(&elem->str, ch);
}

arStatus_t              PList_ReserveElemString(plistElem_t *elem, size_t cap)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_STRING_T);
        return PList_ReserveString(&elem->str, cap);
}

const ar_byte_t*        PList_GetElemDataPointer(const plistElem_t *elem)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_DATA_T);
        
        
        return PList_GetDataPointer(&elem->data);
}

size_t                  PList_GetElemDataLength(const plistElem_t *elem)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_DATA_T);
        return PList_GetDataLength(&elem->data);
}


arStatus_t              PList_SetElemData(plistElem_t *elem, const ar_byte_t *data, size_t length)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_DATA_T);
        
        if(data == NULL || length == 0)
        {
                return AR_S_YES;
        }else
        {
                return PList_SetData(&elem->data, data, length);
        }
        
}

const plistNumber_t*    PList_GetElemNumber(const plistElem_t *elem)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_NUMBER_T);
        return &elem->number;
}


void    PList_SetElemSignedInteger(plistElem_t *elem, ar_int_64_t num)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_NUMBER_T);
        
        elem->number.type = PLIST_NUMBER_INTEGER_T;
        elem->number.integer.is_signed = true;
        elem->number.integer.signed_num = num;
        
}

void                    PList_SetElemUnsignedInteger(plistElem_t *elem, ar_uint_64_t num)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_NUMBER_T);
        
        elem->number.type = PLIST_NUMBER_INTEGER_T;
        elem->number.integer.is_signed = false;
        elem->number.integer.unsigned_num = num;
}

void                    PList_SetElemReal(plistElem_t *elem, double num)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_NUMBER_T);
        
        elem->number.type = PLIST_NUMBER_REAL_T;
        elem->number.real.num = num;
        
}



void                    PList_SetElemGMTDate(plistElem_t *elem, ar_uint_16_t year, ar_uint_16_t mon, ar_uint_16_t day, ar_uint_16_t hour, ar_uint_16_t min, ar_uint_16_t sec)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_DATE_T);
        
        elem->date.year = year;
        elem->date.month = (ar_uint_8_t)mon;
        elem->date.day = (ar_uint_8_t)day;
        elem->date.hour = (ar_uint_8_t)hour;
        elem->date.minute = (ar_uint_8_t)min;
        elem->date.second = (ar_uint_8_t)sec;
        
}


void                    PList_GetElemGMTDate(const plistElem_t *elem, ar_uint_16_t *year, ar_uint_16_t *mon, ar_uint_16_t *day, ar_uint_16_t *hour, ar_uint_16_t *min, ar_uint_16_t *sec)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_DATE_T);
        
        if(year)
        {
                *year = elem->date.year;
        }
        
        if(mon)
        {
                *mon = elem->date.month;
        }
        
        if(day)
        {
                *day = elem->date.day;
        }
        
        if(hour)
        {
                *hour = elem->date.hour;
        }
        
        if(min)
        {
                *min = elem->date.minute;
        }
        
        if(sec)
        {
                *sec = elem->date.second;
        }
}



size_t            PList_GetElemArrayCount(const plistElem_t *elem)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_ARRAY_T);
        return elem->array.count;
}

plistElem_t*            PList_GetElemArrayByIndex(plistElem_t *elem, size_t idx)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_ARRAY_T);
        
        return PList_GetArrayByIndex(&elem->array, idx);
        
}

arStatus_t              PList_AppendToElemArray(plistElem_t *elem, plistElem_t *item)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_ARRAY_T);
        AR_ASSERT(item != NULL);
        
        return PList_PushToArray(&elem->array, item);
}

size_t                  PList_GetElemDictCount(const plistElem_t *elem)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_DICT_T);
        return elem->dict.count;
}


plistElem_t*            PList_FindElemDictValueByWcs(plistElem_t *elem, const wchar_t *key)
{
        AR_ASSERT(elem != NULL && key != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_DICT_T);
        
        return PList_FindValueByString(&elem->dict, key);
        
}


arStatus_t              PList_SetElemDictValueForKey(plistElem_t *elem, plistElem_t *key, plistElem_t *val)
{
        AR_ASSERT(elem != NULL && key != NULL && val != NULL);
        return PList_SetDictValueForKey(&elem->dict, key, val);
}


const wchar_t*          PList_GetElemDictKeyWcsByIndex(plistElem_t *elem, size_t idx)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_DICT_T);
        AR_ASSERT(idx < PList_GetElemDictCount(elem));
        
        return PList_GetDictKeyWcsByIndex(&elem->dict, idx);
        
}


plistElem_t*            PList_GetElemDictKeyByIndex(plistElem_t *elem, size_t idx)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_DICT_T);
        AR_ASSERT(idx < PList_GetElemDictCount(elem));
        return PList_GetDictKeyByIndex(&elem->dict, idx);
}


plistElem_t*            PList_GetElemDictValueByIndex(plistElem_t *elem, size_t idx)
{
        AR_ASSERT(elem != NULL);
        AR_ASSERT(PList_GetElemType(elem) ==  PLIST_ELEM_DICT_T);
        AR_ASSERT(idx < PList_GetElemDictCount(elem));
        return PList_GetDictValueByIndex(&elem->dict, idx);
}


/**********************************************Utility*****************************************************/


static arCodePage_t __detect_xml_codepage(const char *xml, size_t length)
{
        arCodePage_t cp;
        const char *idx, *end;
        char quote;
        AR_ASSERT(xml != NULL && length > 0);
        
        cp = AR_CP_MAX;
        
        
        // Scan for the <?xml.... ?> opening
        if(length < 5 || AR_strncmp(xml, "<?xml", 5) != 0)
        {
                return AR_CP_UTF8;
        }
        
        idx = xml + 5;
        end = xml + length;
        
        // Found "<?xml"; now we scan for "encoding"
        while (idx < end)
        {
                char ch = *idx;
                const char *scan;
                
                if(ch == '?' || ch == '>')
                {
                        return AR_CP_UTF8;
                }
                
                idx++;
                scan = idx;
                
                if (ch == 'e' && *scan++ == 'n' && *scan++ == 'c' && *scan++ == 'o' && *scan++ == 'd' && *scan++ == 'i' && *scan++ == 'n' && *scan++ == 'g' && *scan++ == '=')
                {
                        idx = scan;
                        break;
                }
        }
        
        if (idx >= end)
        {
                return AR_CP_UTF8;
        }
        
        quote = *idx;
        if (quote != '\'' && quote != '\"')
        {
                return AR_CP_UTF8;
                
        }else
        {
                const char *base = idx+1;
                size_t len;
                
                idx++;
                
                while(idx < end && *idx != quote)
                {
                        idx++;
                }
                
                if (idx >= end)
                {
                        return AR_CP_UTF8;
                }
                
                len = idx - base;
                
                
                
                if(len == 7)
                {
                        if(AR_strnicmp(base, "GB10830", 7) == 0)
                        {
                                return AR_CP_GB18030;
                        }else
                        {
                                return AR_CP_MAX;
                        }
                }
                
                
                if(len == 6)
                {
                        if(AR_strnicmp(base, "GB2312", 6) == 0)
                        {
                                return AR_CP_GB2312;
                        }else
                        {
                                return AR_CP_MAX;
                        }
                }
                
                if(len == 5)
                {
                        if(AR_strnicmp(base, "UTF-8", 5) == 0)
                        {
                                return AR_CP_UTF8;
                        }else
                        {
                                return AR_CP_MAX;
                        }
                        
                }
                
                if(len == 4)
                {
                        if(AR_strnicmp(base, "BIG5", 4) == 0)
                        {
                                return AR_CP_BIG5;
                        }else
                        {
                                return AR_CP_MAX;
                        }
                }
                
                if(len == 3)
                {
                        if(AR_strnicmp(base, "GBK", 3) == 0)
                        {
                                return AR_CP_GB18030;
                        }else
                        {
                                return AR_CP_MAX;
                        }
                }
                
                
                return AR_CP_MAX;
        }
        
}


arStatus_t            PList_LoadXMLFromBinary(arBuffer_t *buf, arString_t *out)
{
        arStatus_t      status;
        arTxtBom_t      bom;
        
        
        AR_ASSERT(buf != NULL && out != NULL);
        AR_ASSERT(AR_GetBufferAvailable(buf) > 0);
        
        status = AR_S_YES;
        
        bom = AR_TXT_BOM_NONE;
        
        status = AR_DetectTextBom(AR_GetBufferData(buf), AR_GetBufferAvailable(buf), &bom, NULL);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        
        if(bom != AR_TXT_BOM_NONE)      //unicode
        {
                AR_ClearString(out);
                status = AR_LoadBomTextFromBinary(buf, NULL, out);
                
                if(status != AR_S_YES)
                {
                        AR_error(AR_ERR_WARNING, L"bad text bom : %d\r\n", bom);
                        goto END_POINT;
                }
        }else
        {
                arCodePage_t    cp = AR_CP_MAX;
                wchar_t         *str = NULL;
                const char *xml = (const char*)AR_GetBufferData(buf);
                size_t length =   AR_GetBufferAvailable(buf);
                
                cp = __detect_xml_codepage(xml, length);
                
                if(cp == AR_CP_MAX)
                {
                        status = AR_E_BADENCCONV;
                        goto END_POINT;
                }
                
                str = AR_str_to_wcs(cp, xml, length);
                
                if(str == NULL)
                {
                        status = AR_E_BADENCCONV;
                        AR_error(AR_ERR_WARNING, L"invalid code page : %u\r\n", (ar_uint_32_t)cp);
                        goto END_POINT;
                }
                
                status = AR_SetString(out, str);
                
                if(str)
                {
                        AR_DEL(str);
                        str = NULL;
                }
        }
        
END_POINT:
        
        return status;
        
}



arStatus_t              PList_LoadXMLFromFile(const wchar_t *path, arString_t *out)
{
        arStatus_t		ret;
		arBuffer_t		*buf;
		
		AR_ASSERT(path != NULL && out != NULL);
        
		ret = AR_S_YES;
		buf = NULL;
        
		buf = AR_CreateBuffer(0);
        
		if(buf == NULL)
		{
				ret = AR_E_NOMEM;
				goto FAILED_POINT;
		}
        
        ret = PList_LoadBinaryFromFile(path, buf);
        
        if(ret != AR_S_YES)
        {
                goto FAILED_POINT;
        }
        
        {
                static const ar_byte_t tmp = '\0';
                
                ret = AR_InsertToBuffer(buf, &tmp, 1);
                if(ret != AR_S_YES)
                {
                        goto FAILED_POINT;
                }
        }
        
		ret = PList_LoadXMLFromBinary(buf, out);
        
FAILED_POINT:
        
		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}
        
		return ret;
}



arStatus_t              PList_LoadBinaryFromFile(const wchar_t *path, arBuffer_t *buf)
{
        
        
        arStatus_t		ret;
		arFile_t		*file = NULL;
		
		AR_ASSERT(path != NULL && buf != NULL);
        
		ret = AR_S_YES;
		file = NULL;
        
        AR_ClearBuffer(buf);
        
		ret = AR_open_file(&file, path, L"rb");
        
		if(ret != AR_S_YES)
		{
				AR_error(AR_ERR_WARNING, L"__AR_open_file failed for %ls in function '%hs'\r\n", path, AR_FUNC_NAME);
				goto FAILED_POINT;
		}
        
        
		{
				size_t	rn;
				ar_byte_t	tmp[256];
				
				do{
						ret = AR_read_file(file, tmp, 256, &rn);
						/*rn = fread((void*)tmp, 1, sizeof(tmp), file);*/
						if(rn > 0)
						{
								ret = AR_InsertToBuffer(buf, tmp, rn);
								if(ret == AR_E_NOMEM)
								{
										goto FAILED_POINT;
								}
						}
                        //}while(!feof(file) && !ferror(file));
				}while(AR_eof_file(file) != AR_S_YES && AR_error_file(file) != AR_S_YES);
                
				if(AR_error_file(file) == AR_S_YES)
				{
						ret = AR_E_FILE;
						AR_error(AR_ERR_WARNING, L"fread failed for %ls in function '%hs'\r\n", path, AR_FUNC_NAME);
						goto FAILED_POINT;
				}
		}
        
FAILED_POINT:
		if(file)
		{
				AR_close_file(file);
				file = NULL;
		}
        
		return ret;
}






AR_NAMESPACE_END


