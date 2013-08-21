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





#include "../Lex/lex.h"
#include "property_list.h"

#if defined(OS_FAMILY_WINDOWS)
		#include <time.h>		
#elif defined(OS_FAMILY_UNIX)
		
#else 
		#error "Unknown platform !"
#endif

AR_NAMESPACE_BEGIN


/****************************************************plat dep*********************************/



#if defined(OS_FAMILY_WINDOWS)

		#if !defined(timegm)
                #define __NO_TIMEGM__   1
		#endif

#elif defined(OS_FAMILY_UNIX)

        #if(OS_TYPE == OS_ANDROID)
                #define __NO_TIMEGM__   1
        #endif

#else
		#error "Unknown platform !"
#endif


#if defined(__NO_TIMEGM__)


        static unsigned int __is_leap(unsigned int y)
        {
                y += 1900;
                return (y % 4) == 0 && ((y % 100) != 0 || (y % 400) == 0);
        }

        static time_t __my_timegm (struct tm *tm)
        {
                static const unsigned ndays[2][12] =
                {
                        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
                };
        
        
                time_t res = 0;
                int i;
        
                AR_ASSERT(tm != NULL);
        
                for (i = 70; i < tm->tm_year; ++i)
                        res += __is_leap(i) ? 366 : 365;
        
                for (i = 0; i < tm->tm_mon; ++i)
                        res += ndays[__is_leap(tm->tm_year)][i];
        
                res += tm->tm_mday - 1;
                res *= 24;
                res += tm->tm_hour;
                res *= 60;
                res += tm->tm_min;
                res *= 60;
                res += tm->tm_sec;
        
                return res;
        }


        #define timegm __my_timegm


#endif


static void __absolutetime_to_gmtime(double abstime, ar_uint_16_t *year, ar_uint_16_t *mon, ar_uint_16_t *day, ar_uint_16_t *hour, ar_uint_16_t *min, ar_uint_16_t *sec)
{
        struct tm tm_2001_0101_00_00_00;
        time_t gmtime_2001_0101_00_00_00_val;
        time_t curr_gmt;
        struct tm gm_time;
        AR_memset(&tm_2001_0101_00_00_00, 0, sizeof(tm_2001_0101_00_00_00));
        
        tm_2001_0101_00_00_00.tm_sec = 0;
        tm_2001_0101_00_00_00.tm_min = 0;
        tm_2001_0101_00_00_00.tm_hour = 0;    /* hours (0 - 23) */
        tm_2001_0101_00_00_00.tm_mday = 1;    /* day of month (1 - 31) */
        tm_2001_0101_00_00_00.tm_mon = 0;     /* month of year (0 - 11) */
        tm_2001_0101_00_00_00.tm_year = 2001 - 1900;    /* year - 1900 */
        
        gmtime_2001_0101_00_00_00_val = timegm(&tm_2001_0101_00_00_00);
        
        curr_gmt = gmtime_2001_0101_00_00_00_val + (time_t)abstime;
        gm_time = *gmtime(&curr_gmt);
        
        
        if(year)
        {
                *year = (ar_uint_16_t)gm_time.tm_year + 1900;
        }
        
        if(mon)
        {
                *mon = (ar_uint_16_t)gm_time.tm_mon + 1;
        }
        
        if(day)
        {
                *day = (ar_uint_16_t)gm_time.tm_mday;
        }
        
        
        if(hour)
        {
                *hour = (ar_uint_16_t)gm_time.tm_hour;
        }
        
        
        if(min)
        {
                *min = (ar_uint_16_t)gm_time.tm_min;
        }
        
        if(sec)
        {
                *sec = (ar_uint_16_t)gm_time.tm_sec;
        }
        
}



/***********************************************************************************************/


static const wchar_t __g_plist_tags[13][10]=
{
        {L'p', L'l', L'i', L's', L't', L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'a', L'r', L'r', L'a', L'y', L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'd', L'i', L'c', L't', L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'k', L'e', L'y', L'\0',L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L's', L't', L'r', L'i', L'n', L'g',  L'\0', L'\0', L'\0', L'\0'},
        {L'd', L'a', L't', L'a', L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'd', L'a', L't', L'e', L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'r', L'e', L'a', L'l', L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'i', L'n', L't', L'e', L'g', L'e',  L'r',  L'\0', L'\0', L'\0'},
        {L't', L'r', L'u', L'e', L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'f', L'a', L'l', L's', L'e', L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'D', L'O', L'C', L'T', L'Y', L'P',  L'E',  L'\0', L'\0', L'\0'},
        {L'<', L'!', L'[', L'C', L'D', L'A',  L'T',  L'A',  L'[',  L'\0'}
};



#define PLIST_IX        0
#define ARRAY_IX        1
#define DICT_IX         2
#define KEY_IX          3
#define STRING_IX       4
#define DATA_IX     	5
#define DATE_IX     	6
#define REAL_IX     	7
#define INTEGER_IX      8
#define TRUE_IX         9
#define FALSE_IX        10
#define DOCTYPE_IX      11
#define CDSECT_IX       12

#define PLIST_TAG_LENGTH        5
#define ARRAY_TAG_LENGTH        5
#define DICT_TAG_LENGTH         4
#define KEY_TAG_LENGTH			3
#define STRING_TAG_LENGTH       6
#define DATA_TAG_LENGTH         4
#define DATE_TAG_LENGTH         4
#define REAL_TAG_LENGTH         4
#define INTEGER_TAG_LENGTH      7
#define TRUE_TAG_LENGTH         4
#define FALSE_TAG_LENGTH        5
#define DOCTYPE_TAG_LENGTH      7
#define CDSECT_TAG_LENGTH       9

/*this module is modified from apple CF-550*/



arStatus_t      PList_Init()
{

        arStatus_t status;
        
        status = AR_S_YES;
        
        return status;
}


void            PList_UnInit()
{
         
}



/********************************************************PList elements********************************************************/


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

void            PList_UnInitArray(plistArray_t *arr)
{
        AR_ASSERT(arr != NULL);
        PList_ClearArray(arr);
        
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
                        return 0;
                case PLIST_ELEM_DICT_T:
                        return 0;
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
                        PList_UnInitArray(&elem->array);
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



static arStatus_t __append_xml_indents(arString_t *out, size_t n_indents)
{
        static const wchar_t *tabs = L"\t";
        size_t i;
        AR_ASSERT(out != NULL);
        
        
        for(i = 0; i < n_indents; ++i)
        {
                if(AR_AppendString(out, tabs) != AR_S_YES)
                {
                        return AR_E_NOMEM;
                }
        }
        
        return AR_S_YES;
}

static arStatus_t __apend_xml_escapedstring(arString_t *out, const wchar_t *s, size_t len)
{
        size_t i;
        AR_ASSERT(out != NULL && s != NULL);
        
        for(i = 0; i < len; ++i)
        {
                wchar_t ch = s[i];
                
                switch(ch)
                {
                        case L'<':
                                if(AR_AppendString(out, L"&lt;") != AR_S_YES)
                                {
                                        return AR_E_NOMEM;
                                }
                                break;
                        case L'>':
                                if(AR_AppendString(out, L"&gt;") != AR_S_YES)
                                {
                                        return AR_E_NOMEM;
                                }
                                break;
                        case L'&':
                                if(AR_AppendString(out, L"&amp;") != AR_S_YES)
                                {
                                        return AR_E_NOMEM;
                                }
                                break;
                        default:
                                if(AR_AppendCharToString(out, ch) != AR_S_YES)
                                {
                                        return AR_E_NOMEM;
                                }
                                break;
                                
                }
        }
        return AR_S_YES;
}

static arStatus_t __apend_xml_data_use_base64(arString_t *out, const ar_byte_t *data, size_t len)
{
        ar_int_t l;
        char *buf;
        size_t i;
        AR_ASSERT(out != NULL);
        
        if(len == 0)
        {
                return AR_S_YES;
        }
        
        l = AR_base64_encode(NULL, 0, data, len);
        
        if(l > 0)
        {
                buf = AR_NEWARR(char, l);
                
                if(buf == NULL)
                {
                        return AR_E_NOMEM;
                }
                
                l = AR_base64_encode((ar_byte_t*)buf, l, data, len);
                AR_ASSERT(l >= 0);
                for(i = 0; i < (size_t)l; ++i)
                {
                        wchar_t ch = (wchar_t)buf[i];
                        
                        if(AR_AppendCharToString(out, ch) != AR_S_YES)
                        {
                                AR_DEL(buf);
                                buf = NULL;
                                return AR_E_NOMEM;
                        }
                }
                
                
                AR_DEL(buf);
                buf = NULL;
        }
        return AR_S_YES;
}


#define __CHECK_STATUS_AND_GOTO_ENDPOINT()              \
        do{                                             \
                if(status != AR_S_YES)goto END_POINT;   \
        }while(0)


static arStatus_t __append_xml_0(arString_t *out, const plistElem_t *elem, size_t indentation)
{
        arStatus_t status;
        AR_ASSERT(out != NULL && elem != NULL);
        
        status = AR_S_YES;
        
        switch(PList_GetElemType(elem))
        {
                case PLIST_ELEM_STRING_T:
                {
                        status = AR_AppendString(out, L"<");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendStringN(out, __g_plist_tags[STRING_IX], STRING_TAG_LENGTH);
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendString(out, L">");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = __apend_xml_escapedstring(out, PList_GetStringCString(&elem->str), PList_GetStringLength(&elem->str));
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendString(out, L"</");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendStringN(out, __g_plist_tags[STRING_IX], STRING_TAG_LENGTH);
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendString(out, L">\r\n");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                }
                        break;
                case PLIST_ELEM_BOOLEAN_T:
                {
                        if(elem->boolean.val)
                        {
                                status = AR_AppendString(out, L"<");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[TRUE_IX], TRUE_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L"/>\r\n");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        }else
                        {
                                status = AR_AppendString(out, L"<");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[FALSE_IX], FALSE_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L"/>\r\n");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                        }
                }
                        break;
                case PLIST_ELEM_NUMBER_T:
                {
                        if(elem->number.type == PLIST_NUMBER_INTEGER_T)
                        {
                                status = AR_AppendString(out, L"<");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[INTEGER_IX], INTEGER_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L">");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                if(elem->number.integer.is_signed)
                                {
                                        status = AR_AppendFormatString(out, L"%qd",elem->number.integer.signed_num);
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                }else
                                {
                                        status = AR_AppendFormatString(out, L"%qu",elem->number.integer.unsigned_num);
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                }
                                
                                status = AR_AppendString(out, L"</");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[INTEGER_IX], INTEGER_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L">\r\n");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                        }else
                        {
                                status = AR_AppendString(out, L"<");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[REAL_IX], REAL_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L">");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                if(AR_is_nan_dbl(elem->number.real.num))
                                {
                                        status = AR_AppendString(out, L"nan");
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                }else if(AR_is_inf_dbl(elem->number.real.num))
                                {
                                        status = AR_AppendString(out, L"inf");
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                }else
                                {
                                        status = AR_AppendFormatString(out, L"%.20f",elem->number.real.num);
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        

                                }
                                
                                status = AR_AppendString(out, L"</");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[REAL_IX], REAL_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L">\r\n");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        }
                }
                        break;
                case PLIST_ELEM_DATA_T:
                {
                        status = AR_AppendString(out, L"<");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendStringN(out, __g_plist_tags[DATA_IX], DATA_TAG_LENGTH);
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendString(out, L">\r\n");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = __apend_xml_data_use_base64(out, PList_GetElemDataPointer(elem), PList_GetElemDataLength(elem));
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendString(out, L"\r\n");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = __append_xml_indents(out, indentation);
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendString(out, L"</");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendStringN(out, __g_plist_tags[DATA_IX], DATA_TAG_LENGTH);
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendString(out, L">\r\n");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                }
                        break;
                case PLIST_ELEM_DATE_T:
                {
                        status = AR_AppendString(out, L"<");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendStringN(out, __g_plist_tags[DATE_IX], DATE_TAG_LENGTH);
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendString(out, L">");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendFormatString(out, L"%04d-%02d-%02dT%02d:%02d:%02dZ",
                                              (ar_int_32_t)elem->date.year,
                                              (ar_int_32_t)elem->date.month,
                                              (ar_int_32_t)elem->date.day,
                                              (ar_int_32_t)elem->date.hour,
                                              (ar_int_32_t)elem->date.minute,
                                              (ar_int_32_t)elem->date.second
                                              );
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        
                        status = AR_AppendString(out, L"</");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendStringN(out, __g_plist_tags[DATE_IX], DATE_TAG_LENGTH);
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                        status = AR_AppendString(out, L">\r\n");
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        
                }
                        break;
                case PLIST_ELEM_ARRAY_T:
                {
                        size_t i;
                        size_t count;
                                                
                        count = PList_GetElemArrayCount(elem);
                        
                        if(count == 0)
                        {
                                status = AR_AppendString(out, L"<");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[ARRAY_IX], ARRAY_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L"/>\r\n");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                        }else
                        {
                                status = AR_AppendString(out, L"<");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[ARRAY_IX], ARRAY_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L">\r\n");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();

                        
                                for(i = 0; i < count; ++i)
                                {
                                        const plistElem_t *tmp = PList_GetElemArrayByIndex((plistElem_t*)elem,i);
                                        AR_ASSERT(tmp != NULL);
                                        status = __append_xml_indents(out, indentation + 1);
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                        status = __append_xml_0(out, tmp, indentation + 1);
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                }
                        
                                status = __append_xml_indents(out, indentation);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L"</");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[ARRAY_IX], ARRAY_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L">\r\n");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        }
                }
                        break;
                case PLIST_ELEM_DICT_T:
                {
                        size_t i;
                        size_t count;
                        
                        count = PList_GetElemDictCount(elem);
                        
                        if(count == 0)
                        {
                                status = AR_AppendString(out, L"<");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[DICT_IX], DICT_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L"/>\r\n");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                        }else
                        {
                                status = AR_AppendString(out, L"<");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[DICT_IX], DICT_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L">\r\n");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();

                                for(i = 0; i < count; ++i)
                                {
                                        const wchar_t *key;
                                        const plistElem_t *val;
                                        
                                        key = PList_GetElemDictKeyWcsByIndex((plistElem_t*)elem, i);
                                        val = PList_GetElemDictValueByIndex((plistElem_t*)elem, i);
                                        AR_ASSERT(key != NULL && val != NULL);
                                        
                                        status = __append_xml_indents(out, indentation + 1);
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                        status = AR_AppendString(out, L"<");
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                        status = AR_AppendStringN(out, __g_plist_tags[KEY_IX], KEY_TAG_LENGTH);
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                        status = AR_AppendString(out, L">");
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                        status = __apend_xml_escapedstring(out, key, AR_wcslen(key));
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                        status = AR_AppendString(out, L"</");
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                        status = AR_AppendStringN(out, __g_plist_tags[KEY_IX], KEY_TAG_LENGTH);
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                        status = AR_AppendString(out, L">\r\n");
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        
                                        status = __append_xml_indents(out, indentation + 1);
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                        status = __append_xml_0(out, val, indentation + 1);
                                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                }
                                
                                status = __append_xml_indents(out, indentation);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L"</");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendStringN(out, __g_plist_tags[DICT_IX], DICT_TAG_LENGTH);
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                status = AR_AppendString(out, L">\r\n");
                                __CHECK_STATUS_AND_GOTO_ENDPOINT();
                                
                                
                        }
                }
                        break;
                case PLIST_ELEM_MAX_T:
                default:
                        status = AR_E_INVAL;
                        __CHECK_STATUS_AND_GOTO_ENDPOINT();
                        break;
                        
        }

END_POINT:
        return AR_S_YES;
        
}




arStatus_t              PList_SaveElemToXML(const plistElem_t *elem, arString_t *out)
{
        arStatus_t status;
        AR_ASSERT(elem != NULL && out != NULL);
        status = AR_S_YES;
        AR_ClearString(out);
        
        
        status = AR_AppendString(out, L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
        status = AR_AppendString(out, L"<!DOCTYPE ");
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
        status = AR_AppendStringN(out, __g_plist_tags[PLIST_IX], PLIST_TAG_LENGTH);
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
        status = AR_AppendString(out, L" PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\r\n");
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
        status = AR_AppendString(out, L"<");
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
        status = AR_AppendStringN(out, __g_plist_tags[PLIST_IX], PLIST_TAG_LENGTH);
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
        status = AR_AppendString(out, L" version=\"1.0\">\r\n");
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
        
        status = __append_xml_0(out, elem, 0);
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
        status = AR_AppendString(out, L"</");
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
        status = AR_AppendStringN(out, __g_plist_tags[PLIST_IX], PLIST_TAG_LENGTH);
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
        status = AR_AppendString(out, L">\r\n");
        __CHECK_STATUS_AND_GOTO_ENDPOINT();
        
END_POINT:
        return status;
}



/********************************************************XML Parser********************************************************/


plistXMLParser_t*       PList_CreateXMLParser()
{
        plistXMLParser_t        *parser;
        
        parser = NULL;
        
        parser = AR_NEW(plistXMLParser_t);
        
        if(parser == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        parser->content = AR_CreateString();
        parser->errmsg = AR_CreateString();
        
        if(parser->content == NULL || parser->errmsg == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        parser->begin = NULL;
        parser->curr = NULL;
        parser->end = NULL;
        parser->has_error = false;
        
        
        return parser;
        
        
INVALID_POINT:
        
        if(parser && parser->content)
        {
                AR_DestroyString(parser->content);
                parser->content = NULL;
        }
        
        if(parser && parser->errmsg)
        {
                AR_DestroyString(parser->errmsg);
                parser->errmsg = NULL;
        }
        
        if(parser)
        {
                AR_DEL(parser);
                parser = NULL;
        }
        return parser;
}

void                    PList_DestroyXMLParser(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL);
        
        AR_DestroyString(parser->content);
        parser->content = NULL;
        
        AR_DestroyString(parser->errmsg);
        parser->errmsg = NULL;
        
        AR_DEL(parser);
        parser = NULL;
}


void                    PList_ClearXMLParser(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL);
        
        AR_ClearString(parser->content);
        AR_ClearString(parser->errmsg);
        parser->begin = NULL;
        parser->curr = NULL;
        parser->end = NULL;
        parser->has_error = false;
}



arStatus_t              PList_SetXMLParserWithWcs(plistXMLParser_t *parser, const wchar_t *xml, size_t length)
{
        arStatus_t      status;
        AR_ASSERT(parser != NULL && xml != NULL && length > 0);
        
        status = AR_S_YES;
        PList_ClearXMLParser(parser);
        
        status = AR_SetStringN(parser->content, xml, length);
        
        if(status != AR_S_YES)
        {
                PList_ClearXMLParser(parser);
                goto END_POINT;
        }
        
        parser->begin = AR_CSTR(parser->content);
        parser->curr = parser->begin;
        parser->end = parser->begin + AR_GetStringLength(parser->content);
        
END_POINT:
        return status;
}


const wchar_t*          PList_GetXMLParserErrorMessage(const plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL);
        if(!parser->has_error)
        {
                return L"";
        }else
        {
                return AR_CSTR(parser->errmsg);
        }
}







ar_bool_t __match_string(const wchar_t *l, const wchar_t *r, size_t n)
{
        AR_ASSERT(l != NULL && r != NULL);
        if(AR_wcsnicmp(l, r, n) == 0)
        {
                return true;
        }else
        {
                return false;
        }
}


static size_t __calc_linenumber(const plistXMLParser_t *parser)
{
        const wchar_t *p;
        size_t count;
        AR_ASSERT(parser != NULL);
        
        if(parser->begin == NULL)
        {
                AR_error(AR_ERR_WARNING, L"parser not input xml content\r\n");
                return 0;
        }
        
        p = parser->begin;
        count = 0;
        
        /*AR_printf(L"%ls\r\n", parser->curr);*/
        
        while (p <= parser->curr)
        {
                if(!Lex_IsLineTerminator(*p))
                {
                        ++p;
                }else
                {
                        count++;
                        
                        if(*p == L'\r' && *(p + 1) == '\n')
                        {
                                ++p;
                        }
                        
                        ++p;
                }
        }
        
        return count;
}

static void __skip_whitespace(plistXMLParser_t *parser)
{
        
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);
        
        parser->curr = AR_wcstrim_space(parser->curr);
        
}

// parser should be just past "<!--"
static void __skip_xml_comment(plistXMLParser_t *parser)
{
        const wchar_t *p, *end;

        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);
        p = parser->curr;
        end = parser->end - 3;   // "-->" Need at least 3 characters to compare against
        
        while(p < end)
        {
                if(*p == L'-' && *(p + 1) == L'-' && *(p + 2) == L'>')
                {
                        parser->curr = p + 3;
                        return;
                }else
                {
                        ++p;
                }
        }
        
        parser->has_error = true;
        AR_FormatString(parser->errmsg, L"Unterminated comment started on line %Iu", __calc_linenumber(parser));
        
}

// parser should be set to the first character after "<?"
static void __skip_xml_processing_instruction(plistXMLParser_t *parser)
{
        const wchar_t *beg, *end;
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);
        
        beg = parser->curr;
        end = parser->end - 2;  // Looking for "?>" so we need at least 2 characters
        
        while(parser->curr < end)
        {
                if(* (parser->curr) == L'?' && *(parser->curr+1) == L'>')
                {
                        parser->curr += 2;
                        return;
                }else
                {
                        parser->curr++;
                }
        }
        
        parser->curr = beg;
        
        parser->has_error = true;
        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing the processing instruction begun on line %Iu", __calc_linenumber(parser));
}


static void __skip_xml_inline_dtd(plistXMLParser_t *parser);

        

// first character should be immediately after the "<!"
static void __skip_xml_dtd(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);

        if(parser->end - parser->curr < DOCTYPE_TAG_LENGTH || !__match_string(parser->curr, __g_plist_tags[DOCTYPE_IX], DOCTYPE_TAG_LENGTH))
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Malformed DTD on line %Iu", __calc_linenumber(parser));
                return;
        }
        
        parser->curr += DOCTYPE_TAG_LENGTH;
        __skip_whitespace(parser);
        
 
        
        // Look for either the beginning of a complex DTD or the end of the DOCTYPE structure
        while (parser->curr < parser->end)
        {
                wchar_t ch = *(parser->curr);
                
                if (ch == L'[') // inline DTD
                {
                        break;
                }
                
                if (ch == L'>') // End of the DTD
                {
                        parser->curr++;
                        return;
                }
                parser->curr++;
        }
        
        if (parser->curr >= parser->end)
        {
                
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing DTD");
                return;
        }
        
        __skip_xml_inline_dtd(parser);
        
        if(parser->has_error)
        {
                return;
        }
        
        __skip_whitespace(parser);
        
        
        if (parser->curr < parser->end)
        {
                if(*(parser->curr) == L'>')
                {
                        parser->curr++;
                }else
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %Iu while parsing DTD", *(parser->curr), __calc_linenumber(parser));
                }
        }else
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing DTD");
        }
}

static void __skip_xml_peref(plistXMLParser_t *parser);



// First character should be just past '['
static void __skip_xml_inline_dtd(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);

        
        while (!parser->has_error && parser->curr < parser->end)
        {
                wchar_t ch;
                __skip_whitespace(parser);
                ch = *parser->curr;
                
                if (ch == L'%')
                {
                        parser->curr++;
                        __skip_xml_peref(parser);
                        
                }else if(ch == L'<')
                {
                        parser->curr++;
                        
                        if (parser->curr >= parser->end)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing inline DTD");
                                return;
                        }
                        ch = *parser->curr;
                        if (ch == L'?')
                        {
                                parser->curr++;
                                __skip_xml_processing_instruction(parser);
                        }else if(ch == L'!')
                        {
                                if (parser->curr + 2 < parser->end && (*(parser->curr+1) == '-' && *(parser->curr+2) == '-'))
                                {
                                        parser->curr += 3;
                                        __skip_xml_comment(parser);
                                }else
                                {
                                        // Skip the myriad of DTD declarations of the form "<!string" ... ">"
                                        parser->curr ++; // Past both '<' and '!'
                                        while (parser->curr < parser->end)
                                        {
                                                if (*(parser->curr) == L'>')
                                                {
                                                        break;
                                                }
                                                parser->curr++;
                                        }
                                        
                                        if (*(parser->curr) != '>')
                                        {
                                                parser->has_error = true;
                                                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing inline DTD");
                                                return;
                                        }
                                        parser->curr++;
                                }
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %d while parsing inline DTD", ch, __calc_linenumber(parser));
                                return;
                        }
                } else if(ch == L']')
                {
                        parser->curr ++;
                        return;
                }else
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %d while parsing inline DTD", ch, __calc_linenumber(parser));
                        return;
                }
        }
        
        if(!parser->has_error)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing inline DTD");
        }
}

static void __skip_xml_peref(plistXMLParser_t *parser)
{
        const wchar_t *p;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        p = parser->curr;
        
        while(p < parser->end)
        {
                if(*p == L';')
                {
                        parser->curr = p + 1;
                        return;
                }
                ++p;
        }
        
        parser->has_error = true;
        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing percent-escape sequence begun on line %Iu", __calc_linenumber(parser));
}



static plistElem_t*     __parse_xml_element(plistXMLParser_t *parser);

static plistElem_t*     __get_xml_content_object(plistXMLParser_t *parser)
{
        
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        while (!parser->has_error && parser->curr < parser->end)
        {
                __skip_whitespace(parser);
                
                if(parser->curr >= parser->end)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                        return NULL;
                }
                
                if(*parser->curr != L'<')
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %Iu", *parser->curr, __calc_linenumber(parser));
                        return NULL;
                }
                parser->curr++;

                if (parser->curr >= parser->end)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                        return NULL;
                }
                
                switch (*(parser->curr))
                {
                case '?':
                                // Processing instruction
                                __skip_xml_processing_instruction(parser);
                                break;
                case '!':
                                // Could be a comment
                                if(parser->curr+2 >= parser->end)
                                {
                                        parser->has_error = true;
                                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                                        return NULL;
                                }
                                
                                if(*(parser->curr+1) == '-' && *(parser->curr+2) == '-')
                                {
                                        parser->curr += 2;
                                        __skip_xml_comment(parser);
                                }else
                                {
                                        parser->has_error = true;
                                        AR_FormatString(parser->errmsg, L"Encountered unexpected item %s begun on line %Iu", parser->curr, __calc_linenumber(parser));
                                        return NULL;
                                }
                                break;
                        case '/':
                                // Whoops!  Looks like we got to the end tag for the element whose content we're parsing
                                parser->curr--;// Back off to the '<'
                                return NULL;
                        default:
                                // Should be an element
                                return __parse_xml_element(parser);
                }
        }
        // Do not set the error string here; if it wasn't already set by one of the recursive parsing calls, the caller will quickly detect the failure (b/c pInfo->curr >= pInfo->end) and provide a more useful one of the form "end tag for <blah> not found"
        return NULL;
}

static plistElem_t* __parse_xml_plisttag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_arraytag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_dicttag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_datatag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_datetag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_realtag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_integertag(plistXMLParser_t *parser);

static plistElem_t* __get_string(plistXMLParser_t *parser);
static ar_bool_t    __check_for_closetag(plistXMLParser_t *parser, const wchar_t *tag, size_t tag_len);
static plistElem_t*     __parse_xml_element(plistXMLParser_t *parser);


static ar_bool_t __cat_from_mark_to_buf(const wchar_t *beg, const wchar_t *end, plistElem_t *str)
{
        AR_ASSERT(beg != NULL && beg <= end);
        AR_ASSERT(str != NULL && PList_GetElemType(str) == PLIST_ELEM_STRING_T);
        
        if(end - beg == 0)
        {
                return true;
        }
        
        if(PList_AppendStringN(&str->str, beg, end - beg) != AR_S_YES)
        {
                return false;
        }else
        {
                return true;
        }
}

static void __parse_xml_cdsect_pl(plistXMLParser_t *parser, plistElem_t *str);
static void __parse_xml_entity_reference_pl(plistXMLParser_t *parser, plistElem_t *str);


// String could be comprised of characters, CDSects, or references to one of the "well-known" entities ('<', '>', '&', ''', '"')
// returns a retained object in *string.
static plistElem_t* __get_string(plistXMLParser_t *parser)
{
        plistElem_t *str;
        const wchar_t *mark;
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);
        
        str = NULL;
        mark = parser->curr; // At any time in the while loop below, the characters between mark and p have not yet been added to *string
        
        str = PList_CreateElem(PLIST_ELEM_STRING_T);
        
        if(str == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        while(!parser->has_error && parser->curr < parser->end)
        {
                wchar_t ch = *parser->curr;
                
                if(ch == L'<')
                {
                        
                        if(parser->curr + 1 >= parser->end)
                        {
                                break;
                        }
                        
                        // Could be a CDSect; could be the end of the string
                        
                        if(*(parser->curr + 1) != L'!') // End of the string
                        {
                                break;
                        }
                        
                        if(!__cat_from_mark_to_buf(mark, parser->curr, str))
                        {
                                parser->has_error = true;
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                goto INVALID_POINT;
                        }
                        
                        __parse_xml_cdsect_pl(parser, str);
                        mark = parser->curr;
                        
                }else if(ch == L'&')
                {
                        if(!__cat_from_mark_to_buf(mark, parser->curr, str))
                        {
                                parser->has_error = true;
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                goto INVALID_POINT;
                        }
                        
                        __parse_xml_entity_reference_pl(parser, str);
                        mark = parser->curr;
                }else
                {
                        ++parser->curr;
                }
        }
        
        
        if(parser->has_error)
        {
                goto INVALID_POINT;
        }
        
        if(!__cat_from_mark_to_buf(mark, parser->curr, str))
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        
        return str;
        
        
INVALID_POINT:
        if(str)
        {
                PList_DestroyElem(str);
                str = NULL;
        }
        
        return NULL;
}



static void __parse_xml_cdsect_pl(plistXMLParser_t *parser, plistElem_t *str)
{
        const wchar_t *begin, *end;
        
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        AR_ASSERT(str != NULL && PList_GetElemType(str) == PLIST_ELEM_STRING_T);
        
        if(parser->end - parser->curr < CDSECT_TAG_LENGTH)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                return;
        }
        
        if(!__match_string(parser->curr, __g_plist_tags[CDSECT_IX], CDSECT_TAG_LENGTH))
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered improper CDATA opening at line %Iu", __calc_linenumber(parser));
                return;
        }
        
        parser->curr += CDSECT_TAG_LENGTH;
        
        begin = parser->curr;
        end = parser->end - 2;
        
        while(parser->curr < end)
        {
                if(*parser->curr == L']' && *(parser->curr + 1) == L']' && *(parser->curr + 2) == L'>')
                {
                        if(!__cat_from_mark_to_buf(begin, parser->curr, str))
                        {
                                parser->has_error = true;
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                return;
                        }
                        
                        parser->curr += 3;
                        return;
                }
                
                parser->curr++;
        }
        
        parser->curr = begin;
        parser->has_error = true;
        AR_FormatString(parser->errmsg, L"Could not find end of CDATA started on line %Iu", __calc_linenumber(parser));
        return;
}



static void __parse_xml_entity_reference_pl(plistXMLParser_t *parser, plistElem_t *str)
{
        ar_int_t len;
        wchar_t ch;
        
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        AR_ASSERT(str != NULL && PList_GetElemType(str) == PLIST_ELEM_STRING_T);

        parser->curr++;
        
        len = parser->end - parser->curr;
        
        if(len < 1)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                return;
        }
        
        
        switch (*(parser->curr))
        {
                case L'l':  // "lt"
                {
                        if (len >= 3 && *(parser->curr+1) == 't' && *(parser->curr+2) == ';')
                        {
                                ch = L'<';
                                parser->curr += 3;
                                break;
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unknown ampersand-escape sequence at line %Iu", __calc_linenumber(parser));
                                return;
                        }
                }
                        break;
                case L'g': // "gt"
                {
                        if (len >= 3 && *(parser->curr+1) == L't' && *(parser->curr+2) == L';')
                        {
                                ch = L'>';
                                parser->curr += 3;
                                break;
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unknown ampersand-escape sequence at line %Iu", __calc_linenumber(parser));
                                return;
                        }
                }
                        break;
                case L'a': // "apos" or "amp"
                {
                        if (len < 4)    // Not enough characters for either conversion
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                                return;
                        }
                        
                        if (*(parser->curr+1) == L'm')
                        {
                                // "amp"
                                if(*(parser->curr+2) == L'p' && *(parser->curr+3) == L';')
                                {
                                        ch = L'&';
                                        parser->curr += 4;
                                        break;
                                }
                                
                        } else if (*(parser->curr+1) == L'p')
                        {
                                // "apos"
                                if (len > 4 && *(parser->curr+2) == 'o' && *(parser->curr+3) == L's' && *(parser->curr+4) == L';')
                                {
                                        ch = L'\'';
                                        parser->curr += 5;
                                        break;
                                }
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unknown ampersand-escape sequence at line %Iu", __calc_linenumber(parser));
                                return;
                        }
                }
                        break;
                case L'q':  // "quote"
                {
                        if (len >= 5 && *(parser->curr+1) == L'u' && *(parser->curr+2) == L'o' && *(parser->curr+3) == L't' && *(parser->curr+4) == L';')
                        {
                                ch = L'\"';
                                parser->curr += 5;
                                break;
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unknown ampersand-escape sequence at line %Iu", __calc_linenumber(parser));
                                return;
                        }
                }
                        break;
                case L'#':
                {
                        ar_uint_16_t num = 0;
                        ar_bool_t is_hex = false;
                        
                        if(len < 4)   // Not enough characters to make it all fit!  Need at least "&#d;"
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                                return;
                        }
                        
                        parser->curr++;
                        if (*(parser->curr) == 'x')
                        {
                                is_hex = true;
                                parser->curr ++;
                        }
                        
                        while (parser->curr < parser->end)
                        {
                                ch = *(parser->curr);
                                parser->curr ++;
                                
                                if(ch == ';')
                                {
                                        ch = (wchar_t)num;
                                        if(!__cat_from_mark_to_buf(&ch, &ch + 1, str))
                                        {
                                                parser->has_error = true;
                                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                                return;
                                        }
                                        return;
                                }
                                
                                if(!is_hex)
                                {
                                        num = num * 10;
                                }else
                                {
                                        num = num * 16;
                                }
                                
                                if(ch <= L'9' && ch >= L'0')
                                {
                                        num += (ch - L'0');
                                        
                                }else if(!is_hex)
                                {
                                        parser->has_error = true;
                                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c at line %Iu", ch, __calc_linenumber(parser));
                                        return;
                                }else if(ch >= L'a' && ch <= L'f')
                                {
                                        num += 10 + (ch - L'a');
                                        
                                }else if(ch >= L'A' && ch <= L'F')
                                {
                                        num += 10 + (ch - L'A');
                                        
                                }else
                                {
                                        parser->has_error = true;
                                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c at line %Iu", ch, __calc_linenumber(parser));
                                        return;
                                }
                                
                                
                        }
                        
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                        return;
                }
                default:
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unknown ampersand-escape sequence at line %Iu", __calc_linenumber(parser));
                        return;
        }
        
        if(!__cat_from_mark_to_buf(&ch, &ch + 1, str))
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                return;
        }
}


static ar_bool_t    __check_for_closetag(plistXMLParser_t *parser, const wchar_t *tag, size_t tag_len)
{
        
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        AR_ASSERT(tag != NULL && tag_len > 0);
        
		if((size_t)(parser->end - parser->curr) < (tag_len + 3))
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                }
                
                return false;
        }
        
//        AR_DPRINT(L"%ls\r\n", parser->curr);
                
        if(*parser->curr != L'<' || *(++parser->curr) != L'/')
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %Iu", *(parser->curr), __calc_linenumber(parser));
                }
                return false;
        }
        
        parser->curr++;
        
//        AR_DPRINT(L"%ls\r\n", parser->curr);
//        AR_DPRINT(L"%ls\r\n", tag);

        
        if(!__match_string(parser->curr, tag, tag_len))
        {
                
                if(!parser->has_error)
                {
                        wchar_t buf[128];
                        AR_ASSERT(tag_len < 128);
                        AR_wcsncpy(buf, tag, tag_len);
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Close tag on line %Iu does not match open tag %ls", __calc_linenumber(parser), buf);
                }
                return false;
        }

        parser->curr += tag_len;
        
        __skip_whitespace(parser);
        
        if(parser->curr == parser->end)
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                }
                
                return false;
        }
        
        
        if(*parser->curr != L'>')
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %Iu", *(parser->curr), __calc_linenumber(parser));
                }
                return false;
        }
        
        parser->curr++;
        return true;
}



// pInfo should be set to the first content character of the <plist>
static plistElem_t* __parse_xml_plisttag(plistXMLParser_t *parser)
{
        plistElem_t *result, *tmp;
        const wchar_t *save;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        result = NULL;
        tmp = NULL;
        
        result = __get_xml_content_object(parser);
        
        if(!result)
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered empty plist tag");
                }
                return NULL;
        }
        
        save = parser->curr;
        
        tmp = __get_xml_content_object(parser);
        
        if(tmp)
        {
                PList_DestroyElem(result);
                result = NULL;
                PList_DestroyElem(tmp);
                tmp = NULL;
                parser->curr = save;
                
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected element at line %Iu (plist can only include one object)",  __calc_linenumber(parser));
                return NULL;
        }
        
        if(parser->has_error)
        {
                // Parse failed catastrophically
                PList_DestroyElem(result);
                result = NULL;

                return NULL;
        }
        
        
        if(__check_for_closetag(parser, __g_plist_tags[PLIST_IX], PLIST_TAG_LENGTH))
        {
                return result;
        }else
        {
                PList_DestroyElem(result);
                result = NULL;
                return NULL;
        }
}


static plistElem_t* __parse_xml_arraytag(plistXMLParser_t *parser)
{
        plistElem_t     *array, *tmp;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        array = PList_CreateElem(PLIST_ELEM_ARRAY_T);
        tmp = NULL;
        if(array == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        tmp = __get_xml_content_object(parser);
        while(tmp)
        {
                if(PList_PushToArray(&array->array, tmp) != AR_S_YES)
                {
                        PList_DestroyElem(tmp);
                        tmp = NULL;
                        parser->has_error = true;
                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                        goto INVALID_POINT;
                }
                tmp = __get_xml_content_object(parser);
        }
        
        if(parser->has_error)
        {
                goto INVALID_POINT;
        }
        
        
        if(!__check_for_closetag(parser, __g_plist_tags[ARRAY_IX], ARRAY_TAG_LENGTH))
        {
                goto INVALID_POINT;
        }
        
        return array;
INVALID_POINT:
        if(array)
        {
                PList_DestroyElem(array);
                array = NULL;
        }
        return NULL;
}

static plistElem_t* __parse_xml_dicttag(plistXMLParser_t *parser)
{
        plistElem_t     *dict, *k, *v;
        const wchar_t   *base;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        dict = NULL;
        k = NULL;
        v = NULL;
        
        dict = PList_CreateElem(PLIST_ELEM_DICT_T);

        if(dict == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        base = parser->curr;
        
        k = __get_xml_content_object(parser);
        
        while(k)
        {
                if(PList_GetElemType(k) != PLIST_ELEM_STRING_T)
                {
                        PList_DestroyElem(k);
                        k = NULL;
                        
                        if(!parser->has_error)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Found non-string inside <dict> at line %Iu)",  __calc_linenumber(parser));
                        }
                        
                        goto INVALID_POINT;
                }
                
                v = __get_xml_content_object(parser);
                
                if(v == NULL)
                {
                        PList_DestroyElem(k);
                        k = NULL;
                        
                        if(!parser->has_error)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Value missing for key inside <dict> at line %Iu)",  __calc_linenumber(parser));
                        }
                        
                        goto INVALID_POINT;
                }
                
                if(PList_SetDictValueForKey(&dict->dict, k, v) != AR_S_YES)
                {
                        parser->has_error = true;
                        parser->curr = base;
                        AR_FormatString(parser->errmsg, L"Failed to set for key %ls inside <dict> at line %Iu)", PList_GetStringCString(&k->str), __calc_linenumber(parser));
                        
                        
                        PList_DestroyElem(k);
                        k = NULL;
                        PList_DestroyElem(v);
                        v = NULL;
                        
                        goto INVALID_POINT;
                }
                
                k = NULL;
                v = NULL;
                
                base = parser->curr;
                k = __get_xml_content_object(parser);
        }
        
        if(parser->has_error)
        {
                goto INVALID_POINT;
        }
        
        
        if(!__check_for_closetag(parser, __g_plist_tags[DICT_IX], DICT_TAG_LENGTH))
        {
                goto INVALID_POINT;
        }
        
        
        return dict;
        
INVALID_POINT:
        if(dict)
        {
                PList_DestroyElem(dict);
                dict = NULL;
        }
        return NULL;

}


static plistElem_t* __parse_xml_datatag(plistXMLParser_t *parser)
{
        plistElem_t *data;
        const wchar_t *mark;
        char *base64_str;
        size_t i, base64_str_len;
        ar_int_t l;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        data = NULL;
        mark = parser->curr;
        data = PList_CreateElem(PLIST_ELEM_DATA_T);
        base64_str = NULL;
        base64_str_len = 0;
        
        if(data == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        mark = parser->curr;
        
        while(parser->curr < parser->end)
        {
                wchar_t ch = (char)*parser->curr;  /*valid base64 string always ascii*/
                if(ch == L'<')
                {
                        break;
                }else
                {
                        parser->curr++;
                }
        }
        
        if(parser->curr >= parser->end)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                goto INVALID_POINT;
        }
        
        base64_str_len = parser->curr - mark;
        
        if(base64_str_len > 0)
        {
        
                base64_str = AR_NEWARR(char, base64_str_len);
        
                if(base64_str == NULL)
                {
                        parser->has_error = true;
                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                        goto INVALID_POINT;
                }
                
                //AR_DPRINT(L"%ls\r\n", mark);
                
                for(i = 0; i < (size_t)(parser->curr - mark); ++i)
                {
                        base64_str[i] = (char)mark[i];
                }
                
                l = AR_base64_decode(NULL, 0, (const ar_byte_t*)base64_str, base64_str_len);
                
                if(l > 0)
                {
                        ar_int_t real_l;
                        ar_byte_t *tmp = AR_AllocFromBuffer(data->data.buf, (size_t)l);
                        
                        if(tmp == NULL)
                        {
                                parser->has_error = true;
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                goto INVALID_POINT;
                        }
                        
                        real_l = AR_base64_decode(tmp, l, (const ar_byte_t*)base64_str, base64_str_len);
                        AR_ASSERT(real_l < l);
                        AR_EraseBufferBack(data->data.buf, l - real_l);
                }
        }
        
        
        if(!__check_for_closetag(parser, __g_plist_tags[DATA_IX], DATA_TAG_LENGTH))
        {
                goto INVALID_POINT;
        }
        
        if(base64_str)
        {
                AR_DEL(base64_str);
                base64_str = NULL;
        }

        return data;
        
        
        
INVALID_POINT:
        
        if(data)
        {
                PList_DestroyElem(data);
                data = NULL;
        }
        
        if(base64_str)
        {
                AR_DEL(base64_str);
                base64_str = NULL;
        }
        
        return NULL;

}



static ar_bool_t __read_2_digit_number(plistXMLParser_t *parser, ar_uint_32_t *num)
{
        wchar_t ch1, ch2;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        

        if(parser->curr + 2 >= parser->end)
        {
                return false;
        }
        
        ch1 = *parser->curr;
        ch2 = *(parser->curr + 1);
        parser->curr += 2;
        
        if(!AR_iswdigit(ch1) || !AR_isdigit(ch2))
        {
                return false;
        }
        *num = (ch1 - L'0') * 10 + (ch2 - L'0');
        return true;
}


// YYYY '-' MM '-' DD 'T' hh ':' mm ':' ss 'Z'
static plistElem_t* __parse_xml_datetag(plistXMLParser_t *parser)
{
        ar_uint_32_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;

        ar_bool_t bad_form = false;
        
        AR_ASSERT(parser != NULL && parser->curr != NULL);

        while(parser->curr < parser->end && AR_iswdigit(*parser->curr))
        {
                year = 10 * year + (*parser->curr) - L'0';
                parser->curr++;
        }
        
        if (parser->curr >= parser->end || *parser->curr != L'-')
        {
                bad_form = true;
        }else
        {
                parser->curr++;
        }

        
        if(!bad_form && __read_2_digit_number(parser, &month) && parser->curr < parser->end && *parser->curr == L'-')
        {
                parser->curr++;
        }else
        {
                bad_form = true;
        }
        
        
        if(!bad_form && __read_2_digit_number(parser, &day) && parser->curr < parser->end && *parser->curr == L'T')
        {
                parser->curr++;
        }else
        {
                bad_form = true;
        }
        
        
        if(!bad_form && __read_2_digit_number(parser, &hour) && parser->curr < parser->end && *parser->curr == L':')
        {
                parser->curr++;
        }else
        {
                bad_form = true;
        }
        
        
        
        if(!bad_form && __read_2_digit_number(parser, &minute) && parser->curr < parser->end && *parser->curr == L':')
        {
                parser->curr++;
        }else
        {
                bad_form = true;
        }
        
        
        if(!bad_form && __read_2_digit_number(parser, &second) && parser->curr < parser->end && *parser->curr == L'Z')
        {
                parser->curr++;
        }else
        {
                bad_form = true;
        }
        
        if(bad_form)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Could not interpret <date> at line %Iu)", __calc_linenumber(parser));
                return NULL;
        }
        
        
        if(!__check_for_closetag(parser, __g_plist_tags[DATE_IX], DATE_TAG_LENGTH))
        {
                return NULL;
        }

        
        {
                plistElem_t *date = PList_CreateElem(PLIST_ELEM_DATE_T);
                
                if(date == NULL)
                {
                        parser->has_error = true;
                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                        return NULL;
                }

                date->date.year = (ar_uint_16_t)year;
                date->date.month = (ar_uint_8_t)month;
                date->date.day = (ar_uint_8_t)day;
                date->date.hour = (ar_uint_8_t)hour;
                date->date.minute = (ar_uint_8_t)minute;
                date->date.second = (ar_uint_8_t)second;
                return date;
        }
}



static plistElem_t* __parse_xml_realtag(plistXMLParser_t *parser)
{
        plistElem_t *str, *num;
        const wchar_t *wcs;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        str = NULL;
        num = NULL;
        wcs = NULL;
        
        str = __get_string(parser);
        
        if(str == NULL)
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered empty <real> on line %Iu)", __calc_linenumber(parser));
                }
                goto INVALID_POINT;
        }
        
        num = PList_CreateElem(PLIST_ELEM_NUMBER_T);

        if(num == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        
        wcs = PList_GetStringCString(&str->str);
        num->number.type = PLIST_NUMBER_REAL_T;
        
        if(AR_wcsicmp(wcs, L"nan") == 0)
        {
                num->number.real.num = AR_nan_value_dbl();

        }else if(AR_wcsicmp(wcs, L"+infinity") == 0)
        {
                num->number.real.num = AR_inf_value_dbl();
                
        }else if(AR_wcsicmp(wcs, L"-infinity") == 0)
        {
                num->number.real.num = AR_inf_value_dbl();
                
        }else if(AR_wcsicmp(wcs, L"infinity") == 0)
        {
                num->number.real.num = AR_inf_value_dbl();
                
        }else if(AR_wcsicmp(wcs, L"inf") == 0)
        {
                num->number.real.num = AR_inf_value_dbl();
                
        }else if(AR_wcsicmp(wcs, L"-inf") == 0)
        {
                num->number.real.num = AR_inf_value_dbl();
                
        }else if(AR_wcsicmp(wcs, L"+inf") == 0)
        {
                num->number.real.num = AR_inf_value_dbl();
        }else
        {
                if(AR_wtod(wcs, &num->number.real.num) == NULL)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered misformatted real %ls on line %Iu)", wcs, __calc_linenumber(parser));
                        goto INVALID_POINT;
                }
                
        }
        
        if(!__check_for_closetag(parser, __g_plist_tags[REAL_IX], REAL_TAG_LENGTH))
        {
                goto INVALID_POINT;
        }
        
        if(str)
        {
                PList_DestroyElem(str);
                str = NULL;
        }
        
        return num;
INVALID_POINT:
        
        if(num)
        {
                PList_DestroyElem(num);
                num = NULL;
        }
        
        if(str)
        {
                PList_DestroyElem(str);
                str = NULL;
        }
        
        return NULL;
}



static plistElem_t* __parse_xml_integertag(plistXMLParser_t *parser)
{
        
        plistElem_t *str, *num;
        const wchar_t *wcs;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        str = NULL;
        num = NULL;
        wcs = NULL;
        
        str = __get_string(parser);
        
        if(str == NULL)
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered empty <integer> on line %Iu)", __calc_linenumber(parser));
                }
                goto INVALID_POINT;
        }
        
        num = PList_CreateElem(PLIST_ELEM_NUMBER_T);
        
        if(num == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        wcs = PList_GetStringCString(&str->str);
        num->number.type = PLIST_NUMBER_INTEGER_T;
        
        if(AR_wtou64(wcs, &num->number.integer.unsigned_num, 0) == NULL)
        {
                if(AR_wtoi64(wcs, &num->number.integer.signed_num, 0) == NULL)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Invalid <integer> %ls on line %Iu)", wcs, __calc_linenumber(parser));
                        goto INVALID_POINT;
                        
                }else
                {
                        num->number.integer.is_signed = true;
                }
        }else
        {
                num->number.integer.is_signed = false;
        }
        
        
        if(!__check_for_closetag(parser, __g_plist_tags[INTEGER_IX], INTEGER_TAG_LENGTH))
        {
                goto INVALID_POINT;
        }
        

        
        if(str)
        {
                PList_DestroyElem(str);
                str = NULL;
        }
        
        return num;
INVALID_POINT:
        
        if(num)
        {
                PList_DestroyElem(num);
                num = NULL;
        }
        
        if(str)
        {
                PList_DestroyElem(str);
                str = NULL;
        }
        
        return NULL;
   
}

static plistElem_t*     __parse_xml_element(plistXMLParser_t *parser)
{
        const wchar_t *marker;
        ar_int_t        marker_length;
        ar_bool_t       is_empty;
        ar_int_t       marker_idx;
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);
        
        marker = parser->curr;
        marker_length = -1;
        is_empty = false;
        marker_idx = -1;
        
        while(parser->curr < parser->end)
        {
                wchar_t ch = *parser->curr;
                
                if(AR_iswspace(ch))
                {
                        if(marker_length == -1)
                        {
                                marker_length = parser->curr - marker;
                        }
                }else if(ch == L'>')
                {
                        break;
                }
                
                parser->curr++;
        }
        
        if(parser->curr >= parser->end)
        {
                return NULL;
        }
        
        if( *(parser->curr - 1) == L'/')
        {
                is_empty = true;
        }
        
        if(marker_length == -1)
        {
                marker_length = parser->curr - (is_empty ? 1 : 0) - marker;
        }
        
        parser->curr++; // Advance past '>'
        
        
        if(marker_length == 0)
        {
                parser->curr = marker;
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Malformed tag on line %Iu", __calc_linenumber(parser));
                return NULL;
                
        }
        
        switch (*marker)
        {
                case L'A':
                case L'a':   // Array
                        if(marker_length == ARRAY_TAG_LENGTH && __match_string(marker, __g_plist_tags[ARRAY_IX], ARRAY_TAG_LENGTH))
                        {
                                marker_idx = ARRAY_IX;
                        }
                        break;
                case L'D': // Dictionary, data, or date; Fortunately, they all have the same marker length....
                case L'd':
                        if (marker_length != DICT_TAG_LENGTH)
                        {
                                break;
                        }
                        
                        if(__match_string(marker, __g_plist_tags[DICT_IX], DICT_TAG_LENGTH))
                        {
                                marker_idx = DICT_IX;
                        }else if(__match_string(marker, __g_plist_tags[DATA_IX], DATA_TAG_LENGTH))
                        {
                                marker_idx = DATA_IX;
                        }else if(__match_string(marker, __g_plist_tags[DATE_IX], DATE_TAG_LENGTH))
                        {
                                marker_idx = DATE_IX;
                        }
                        
                        break;
                        
                case L'I':
                case L'i': // integer
                        if(marker_length == INTEGER_TAG_LENGTH && __match_string(marker, __g_plist_tags[INTEGER_IX], INTEGER_TAG_LENGTH))
                        {
                                marker_idx = INTEGER_IX;
                        }
                        break;
                        
                case L'K':
                case L'k': // Key of a dictionary
                        if(marker_length == KEY_TAG_LENGTH && __match_string(marker, __g_plist_tags[KEY_IX], KEY_TAG_LENGTH))
                        {
                                marker_idx = KEY_IX;
                        }
                        break;
                        
                case L'P':
                case L'p': // Plist
                        
                        if(marker_length == PLIST_TAG_LENGTH && __match_string(marker, __g_plist_tags[PLIST_IX], PLIST_TAG_LENGTH))
                        {
                                marker_idx = PLIST_IX;
                        }
                        break;
                case L'R':
                case L'r': // real
                        
                        if(marker_length == REAL_TAG_LENGTH && __match_string(marker, __g_plist_tags[REAL_IX], REAL_TAG_LENGTH))
                        {
                                marker_idx = REAL_IX;
                        }
                        break;
                        
                case L'S':
                case L's': // String
                        
                        if(marker_length == STRING_TAG_LENGTH && __match_string(marker, __g_plist_tags[STRING_IX], STRING_TAG_LENGTH))
                        {
                                marker_idx = STRING_IX;
                        }
                        break;
                        
                case L'T':
                case L't': // true (boolean)
                        if(marker_length == TRUE_TAG_LENGTH && __match_string(marker, __g_plist_tags[TRUE_IX], TRUE_TAG_LENGTH))
                        {
                                marker_idx = TRUE_IX;
                        }
                        break;
                case L'F':
                case L'f': // false (boolean)
                        
                        if(marker_length == FALSE_TAG_LENGTH && __match_string(marker, __g_plist_tags[FALSE_IX], FALSE_TAG_LENGTH))
                        {
                                marker_idx = FALSE_IX;
                        }
                        break;
        }
        
        
        
        
        switch (marker_idx)
        {
                case PLIST_IX:
                        if (is_empty)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered empty plist tag", __calc_linenumber(parser));
                                return NULL;
                        }
                        return __parse_xml_plisttag(parser);
                case ARRAY_IX:
                        
                        if (is_empty)
                        {
                                plistElem_t *arr = PList_CreateElem(PLIST_ELEM_ARRAY_T);
                                
                                if(arr == NULL)
                                {
                                        parser->has_error = true;
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        return NULL;
                                }else
                                {
                                        return arr;
                                }
                        }else
                        {
                                return __parse_xml_arraytag(parser);
                        }
                case DICT_IX:
                        if (is_empty)
                        {
                                plistElem_t *dict = PList_CreateElem(PLIST_ELEM_DICT_T);
                                
                                if(dict == NULL)
                                {
                                        parser->has_error = true;
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        return NULL;
                                }else
                                {
                                        return dict;
                                }
                        }else
                        {
                                return __parse_xml_dicttag(parser);
                        }
                case KEY_IX:
                case STRING_IX:
                {
                        size_t tag_len = 0;
                        plistElem_t *str = NULL;
                        
                        tag_len = marker_idx == KEY_IX ? KEY_TAG_LENGTH : STRING_TAG_LENGTH;
                        
                        if(is_empty)
                        {
                                str = PList_CreateElem(PLIST_ELEM_STRING_T);
                                if(str == NULL)
                                {
                                        parser->has_error = true;
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        return NULL;
                                }else
                                {
                                        return str;
                                }
                        }else
                        {
                                str = __get_string(parser);
                                
                                if(str == NULL) // getString will already have set the error string
                                {
                                        return NULL;
                                }
                                
                                if(!__check_for_closetag(parser, __g_plist_tags[marker_idx], tag_len))
                                {
                                        PList_DestroyElem(str);
                                        str = NULL;
                                        return NULL;
                                }
                                
                                return str;
                        }
                }
                case DATA_IX:
                        if(is_empty)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered empty <data> on line %Iu", __calc_linenumber(parser));
                                return NULL;
                        }else
                        {
                                return __parse_xml_datatag(parser);
                        }
                case DATE_IX:
                        if (is_empty)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered empty <date> on line %Iu", __calc_linenumber(parser));
                                return NULL;
                        } else {
                                return __parse_xml_datetag(parser);
                        }
                case TRUE_IX:
                        if (!is_empty)
                        {
                                if(!__check_for_closetag(parser, __g_plist_tags[TRUE_IX], TRUE_TAG_LENGTH))
                                {
                                        return NULL;
                                }
                        }else
                        {
                                plistElem_t *true_elem = PList_CreateElem(PLIST_ELEM_BOOLEAN_T);
                                
                                if(true_elem == NULL)
                                {
                                        return NULL;
                                }else
                                {
                                        true_elem->boolean.val = true;
                                        return true_elem;
                                }
                        }
                case FALSE_IX:
                        if (!is_empty)
                        {
                                if(!__check_for_closetag(parser, __g_plist_tags[FALSE_IX], FALSE_TAG_LENGTH))
                                {
                                        return NULL;
                                }
                        }else
                        {
                                plistElem_t *false_elem = PList_CreateElem(PLIST_ELEM_BOOLEAN_T);
                                
                                if(false_elem == NULL)
                                {
                                        return NULL;
                                }else
                                {
                                        false_elem->boolean.val = false;
                                        return false_elem;
                                }
                        }
                        
                case REAL_IX:
                        if(is_empty)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered empty <real> on line %Iu", __calc_linenumber(parser));
                                return NULL;
                        }else
                        {
                                return __parse_xml_realtag(parser);
                        }
                case INTEGER_IX:
                        if(is_empty)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered empty <integer> on line %Iu", __calc_linenumber(parser));
                                return NULL;
                        }else
                        {
                                return __parse_xml_integertag(parser);
                        }
                default:
                {
                        const wchar_t *marker_str = AR_wcsndup(marker, marker_length);
                        
                        parser->has_error = true;
                        parser->curr = marker;
                        
                        if(marker_str == NULL)
                        {
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unknown tag %ls on line %Iu", marker_str, __calc_linenumber(parser));
                                AR_DEL(marker_str);
                        }
                        return NULL;
                }
        }
        
        
        
        
}

static plistElem_t*     __parse_property_list(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        
        while(!parser->has_error && parser->curr < parser->end)
        {
                wchar_t ch;
                __skip_whitespace(parser);
                
                if(parser->curr + 1 >= parser->end)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"No XML content found");
                        return NULL;
                }
                
                if(*parser->curr != L'<')
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Unexpected character %c at line %Iu", *parser->curr, __calc_linenumber(parser));
                        return NULL;
                }
                
                parser->curr++;
                ch = *parser->curr;
                
                if(ch == L'!')
                {
                        /*comment or DTD*/
                        parser->curr++;
                        if(parser->curr + 1 < parser->end && *parser->curr == L'-' && *(parser->curr + 1) == L'-')
                        {
                                /*Comment*/
                                parser->curr += 2;
                                __skip_xml_comment(parser);
                        }else
                        {
                                __skip_xml_dtd(parser);
                        }
                }else if(ch == L'?')
                {
                        parser->curr++;
                        __skip_xml_processing_instruction(parser);
                }else
                {
                        return __parse_xml_element(parser);
                }
        }
        
        if(!parser->has_error)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
        }
        return NULL;
}



plistElem_t*            PList_ParseXML(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        return __parse_property_list(parser);
        
}







/*********************************************Binary Plist Parser*********************************************/


/*
 
 typedef struct __plist_binary_parser
 {
 arBuffer_t      *data;
 
 arString_t      *errmsg;
 ar_bool_t       has_error;
 plistBinaryTrailer_t    trailer;
 }plistBinaryParser_t;
 */



enum {
        kCFBinaryPlistMarkerNull = 0x00,
        kCFBinaryPlistMarkerFalse = 0x08,
        kCFBinaryPlistMarkerTrue = 0x09,
        kCFBinaryPlistMarkerFill = 0x0F,
        kCFBinaryPlistMarkerInt = 0x10,
        kCFBinaryPlistMarkerReal = 0x20,
        kCFBinaryPlistMarkerDate = 0x33,
        kCFBinaryPlistMarkerData = 0x40,
        kCFBinaryPlistMarkerASCIIString = 0x50,
        kCFBinaryPlistMarkerUnicode16String = 0x60,
        kCFBinaryPlistMarkerUID = 0x80,
        kCFBinaryPlistMarkerArray = 0xA0,
        kCFBinaryPlistMarkerSet = 0xC0,
        kCFBinaryPlistMarkerDict = 0xD0
};



typedef struct {
        ar_uint_8_t     _magic[6];
        ar_uint_8_t     _version[2];
}CFBinaryPlistHeader;



typedef struct {
        ar_uint_8_t     _unused[5];
        ar_uint_8_t     _sortVersion;
        ar_uint_8_t     _offsetIntSize;
        ar_uint_8_t     _objectRefSize;
        ar_uint_64_t	_numObjects;
        ar_uint_64_t	_topObject;
        ar_uint_64_t	_offsetTableOffset;
}CFBinaryPlistTrailer;


static AR_INLINE ar_uint_64_t _getSizedInt(const ar_byte_t *data, ar_uint_8_t valSize)
{
        if (valSize == 1)
        {
                return (ar_uint_64_t)*data;
                
        } else if (valSize == 2)
        {
                ar_uint_16_t val = *(ar_uint_16_t*)data;
                return (ar_uint_64_t)AR_NTOL_U16(val);
                
        } else if (valSize == 4)
        {
                ar_uint_32_t val = *(ar_uint_32_t*)data;
                return (ar_uint_64_t)AR_NTOL_U32(val);

        } else if (valSize == 8)
        {
                ar_uint_64_t val = *(ar_uint_64_t*)data;
                return AR_NTOL_U64(val);
        }else
        {
                // Compatability with existing archives, including anything with a non-power-of-2 size and 16-byte values
                ar_uint_64_t res = 0;
                size_t idx;
                for (idx = 0; idx < valSize; idx++)
                {
                        res = (res << 8) + data[idx];
                }
                return res;
        }
        
}




enum {
        CF_NO_ERROR = 0,
        CF_OVERFLOW_ERROR = (1 << 0),
};


static AR_INLINE ar_uint_32_t __check_uint32_add_unsigned_unsigned(ar_uint_32_t x, ar_uint_32_t y, ar_int_t *err)
{
        if((AR_UINT32_MAX - y) < x)
                *err = *err | CF_OVERFLOW_ERROR;
        return x + y;
}

static AR_INLINE ar_uint_64_t __check_uint64_add_unsigned_unsigned(ar_uint_64_t x, ar_uint_64_t y, ar_int_t *err)
{
        if((AR_UINT64_MAX - y) < x)
                *err = *err | CF_OVERFLOW_ERROR;
        return x + y;
}

static AR_INLINE ar_uint_32_t __check_uint32_mul_unsigned_unsigned(ar_uint_32_t x, ar_uint_32_t y, ar_int_t *err)
{
        ar_uint_64_t tmp = (ar_uint_64_t) x * (ar_uint_64_t) y;
        /* If any of the upper 32 bits touched, overflow */
        if(tmp & 0xffffffff00000000ULL)
                *err = *err | CF_OVERFLOW_ERROR;
        return (ar_uint_32_t) tmp;
}

static AR_INLINE ar_uint_64_t __check_uint64_mul_unsigned_unsigned(ar_uint_64_t x, ar_uint_64_t y, ar_int_t *err)
{
        if(x == 0)
        {
                return 0;
        }
        
        if(AR_UINT64_MAX/x < y)
        {
                *err = *err | CF_OVERFLOW_ERROR;
        }
        return x * y;
}


#if AR_ARCH_VER == ARCH_64

        #define check_ptr_add(p, a, err)	(const ar_byte_t*)__check_uint64_add_unsigned_unsigned((ar_uint_64_t)p, (ar_uint_64_t)a, err)
        #define check_size_t_mul(b, a, err)	(size_t)__check_uint64_mul_unsigned_unsigned((size_t)b, (size_t)a, err)
#else
        #define check_ptr_add(p, a, err)	(const ar_byte_t*)__check_uint32_add_unsigned_unsigned((ar_uint_32_t)p, (ar_uint_32_t)a, err)
        #define check_size_t_mul(b, a, err)	(size_t)__check_uint32_mul_unsigned_unsigned((size_t)b, (size_t)a, err)
#endif



static AR_INLINE ar_bool_t _readInt(const ar_byte_t *ptr, const ar_byte_t *end_byte_ptr, ar_uint_64_t *bigint, const ar_byte_t **newptr)
{
        
        ar_byte_t marker;
        ar_uint_64_t cnt;
        ar_int_t err;
        const ar_byte_t *extent;
        
        if (end_byte_ptr < ptr)
        {
                return false;
        }
        
        marker = *ptr++;
        
        if ((marker & 0xf0) != kCFBinaryPlistMarkerInt)
        {
                return false;
        }
        
        cnt = (ar_uint_64_t)(1 << (marker & 0x0f));
        
        err = CF_NO_ERROR;
        extent = check_ptr_add(ptr, cnt, &err) - 1;
        
        if(CF_NO_ERROR != err)
        {
                return false;
        }
        
        if(end_byte_ptr < extent)
        {
                return false;
        }
        
        // integers are not required to be in the most compact possible representation, but only the last 64 bits are significant currently
        *bigint = _getSizedInt(ptr, (ar_uint_8_t)cnt);
        
        ptr += cnt;
        
        if(newptr)
        {
                *newptr = ptr;
        }
        
        return true;
}






static ar_bool_t __get_binary_plist_toplevelinfo(const ar_byte_t *data, size_t length, ar_uint_8_t *marker, ar_uint_64_t *offset, CFBinaryPlistTrailer *trailer)
{
        
        AR_ASSERT(data != NULL && marker != NULL && offset != NULL && trailer != NULL);
        
        if(length < sizeof(*trailer) + 8 + 1)
        {
                return false;
        }
        
        // Tiger and earlier will parse "bplist00"
        // Leopard will parse "bplist00" or "bplist01"
        // SnowLeopard will parse "bplist0?" where ? is any one character

        if (AR_memcmp((const ar_byte_t*)"bplist0", data, 7) != 0)
        {
                return false;
        }

        AR_memcpy((ar_byte_t*)trailer, data + length - sizeof(*trailer), sizeof(*trailer));
        
        
        // In Leopard, the unused bytes in the trailer must be 0 or the parse will fail
        // This check is not present in Tiger and earlier or after Leopard

        // In Leopard, the unused bytes in the trailer must be 0 or the parse will fail
        // This check is not present in Tiger and earlier or after Leopard
        trailer->_numObjects = AR_NTOL_64(trailer->_numObjects);
        trailer->_topObject = AR_NTOL_64(trailer->_topObject);
        trailer->_offsetTableOffset = AR_NTOL_64(trailer->_offsetTableOffset);
        
        if (LONG_MAX < trailer->_numObjects)
        {
                return false;
        }
        
        if (LONG_MAX < trailer->_offsetTableOffset)
        {
                return false;
        }
        
        if (trailer->_numObjects < 1)
        {
                return false;
        }
        
        if (trailer->_numObjects <= trailer->_topObject)
        {
                return false;
        }
        
        if (trailer->_offsetTableOffset < 9)
        {
                return false;
        }
        
        if (length - sizeof(*trailer) <= trailer->_offsetTableOffset)
        {
                return false;
        }
        
        if (trailer->_offsetIntSize < 1)
        {
                return false;
        }
        
        if (trailer->_objectRefSize < 1)
        {
                return false;
        }
        
        
        {
                ar_int_t err = CF_NO_ERROR;
                ar_uint_64_t offsetIntSize;
                ar_uint_64_t offsetTableSize;
                ar_uint_64_t objectDataSize;
                ar_uint_64_t tmpSum;
                const ar_byte_t *objectsFirstByte;
                const ar_byte_t *offsetsFirstByte;
                const ar_byte_t *offsetsLastByte;
                const ar_byte_t *bytesptr;
                ar_uint_64_t maxOffset;
                size_t idx;
                ar_uint_64_t off;
                offsetIntSize = trailer->_offsetIntSize;
                offsetTableSize = __check_uint64_mul_unsigned_unsigned(trailer->_numObjects, offsetIntSize, &err);
                
                if (CF_NO_ERROR != err)
                {
                        return false;
                }
                
                if(offsetTableSize < 1)
                {
                        return false;
                }
             
                
                objectDataSize = trailer->_offsetTableOffset - 8;
                tmpSum = __check_uint64_add_unsigned_unsigned(8, objectDataSize, &err);
                
                tmpSum = __check_uint64_add_unsigned_unsigned(tmpSum, offsetTableSize, &err);
                tmpSum = __check_uint64_add_unsigned_unsigned(tmpSum, sizeof(*trailer), &err);
                
                if (CF_NO_ERROR != err)
                {
                        return false;
                }
                
                
                if (length != tmpSum)
                {
                        return false;
                }
                
                if (trailer->_objectRefSize < 8 && (1ULL << (8 * trailer->_objectRefSize)) <= trailer->_numObjects)
                {
                        return false;
                }
                
                if (trailer->_offsetIntSize < 8 && (1ULL << (8 * trailer->_offsetIntSize)) <= trailer->_offsetTableOffset)
                {
                        return false;
                }

                
                
                objectsFirstByte = check_ptr_add(data, 8, &err);
                
                if (CF_NO_ERROR != err)
                {
                        return false;
                }
                
                offsetsFirstByte = check_ptr_add(data, trailer->_offsetTableOffset, &err);
                
                if (CF_NO_ERROR != err)
                {
                        return false;
                }
                
                
                offsetsLastByte = check_ptr_add(offsetsFirstByte, offsetTableSize - 1, &err);
                
                if (CF_NO_ERROR != err)
                {
                        return false;
                }
                
                bytesptr = data + trailer->_offsetTableOffset;
                
                maxOffset = trailer->_offsetTableOffset - 1;
                
                for (idx = 0; idx < trailer->_numObjects; idx++)
                {
                        ar_uint_64_t offset = _getSizedInt(bytesptr, trailer->_offsetIntSize);
                        if (maxOffset < offset)
                        {
                                return false;
                        }
                        bytesptr += trailer->_offsetIntSize;
                }
                
                bytesptr = data + trailer->_offsetTableOffset + trailer->_topObject * trailer->_offsetIntSize;
                
                
                off = _getSizedInt(bytesptr, trailer->_offsetIntSize);
                
                if(off < 8 || trailer->_offsetTableOffset <= off)
                {
                        return false;
                }
                
                
                *offset = off;
                *marker = *(data + off);
        }

        return true;
        
}


static AR_INLINE ar_uint_64_t _getOffsetOfRefAt(const ar_byte_t *databytes, const ar_byte_t *bytesptr, const CFBinaryPlistTrailer *trailer)
{
        // *trailer contents are trusted, even for overflows -- was checked when the trailer was parsed;
        // this pointer arithmetic and the multiplication was also already done once and checked,
        // and the offsetTable was already validated.
        const ar_byte_t *objectsFirstByte = databytes + 8;
        const ar_byte_t *offsetsFirstByte = databytes + trailer->_offsetTableOffset;
        ar_uint_64_t ref;
        ar_uint_64_t off;
        
        if (bytesptr < objectsFirstByte || offsetsFirstByte - trailer->_objectRefSize < bytesptr)
        {
                return AR_UINT64_MAX;
        }
        
        ref = _getSizedInt(bytesptr, trailer->_objectRefSize);
        if (trailer->_numObjects <= ref)
        {
                return AR_UINT64_MAX;
        }
        
        bytesptr = databytes + trailer->_offsetTableOffset + ref * trailer->_offsetIntSize;
        off = _getSizedInt(bytesptr, trailer->_offsetIntSize);
        return off;
}




/******************save binary******************************/




static arStatus_t _flattenPlist(const plistElem_t *plist, plistArray_t *objlist, arHash_t *objtable, arHash_t *uniquingset)
{
        
        plistElem_t *unique;
        ar_uint_32_t refnum;
        plistElemType_t type;
        size_t idx;
        
        AR_ASSERT(plist != NULL);

        type = PList_GetElemType(plist);
        
        
        if(type == PLIST_ELEM_STRING_T || type == PLIST_ELEM_BOOLEAN_T || type == PLIST_ELEM_NUMBER_T || type == PLIST_ELEM_DATA_T || type == PLIST_ELEM_DATE_T)
        {
                
                if(AR_FindFromHash(uniquingset, (void*)plist, (void**)&unique) != AR_S_YES)
                {
                        if(AR_SetToHash(uniquingset, (void*)plist, (void*)plist) != AR_S_YES)
                        {
                                return AR_E_NOMEM;
                        }
                }else
                {
                        if(unique != plist)
                        {
                                size_t tmp;
                                arStatus_t status;
                                status = AR_FindFromHash(objtable, (void*)unique, (void**)&tmp);
                                AR_ASSERT(status == AR_S_YES);

                                refnum = (ar_uint_32_t)tmp;
                                
                                status = AR_SetToHash(objtable, (void*)plist, (void*)refnum);
                                
                                return status;
                        }else
                        {
                                return AR_S_YES;
                        }
                }
        }
        
        
        refnum = (ar_uint_32_t)PList_GetArrayCount(objlist);
        
        if(PList_PushToArray(objlist,(plistElem_t*)plist) != AR_S_YES)
        {
                return AR_E_NOMEM;
        }
        
        if(AR_SetToHash(objtable, (void*)plist, (void*)refnum) != AR_S_YES)
        {
                return AR_E_NOMEM;
        }
        
        
        if(PList_GetElemType(plist) == PLIST_ELEM_DICT_T)
        {
                
                size_t count = PList_GetElemDictCount(plist);
                
                
                for(idx = 0; idx < count; ++idx)
                {
                        const plistElem_t *key = PList_GetElemDictKeyByIndex((plistElem_t*)plist, idx);
                        const plistElem_t *val = PList_GetElemDictValueByIndex((plistElem_t*)plist, idx);
                        arStatus_t status;
                        AR_ASSERT(key != NULL && val != NULL);
                        
                        
                        status = _flattenPlist(key, objlist, objtable, uniquingset);
                        
                        if(status != AR_S_YES)
                        {
                                return status;
                        }
                        
                        status = _flattenPlist(val, objlist, objtable, uniquingset);
                        
                        if(status != AR_S_YES)
                        {
                                return status;
                        }
                }

        }else if(PList_GetElemType(plist) == PLIST_ELEM_ARRAY_T)
        {
                size_t count = PList_GetElemArrayCount(plist);
                
                for (idx = 0; idx < count; idx++)
                {
                        arStatus_t status;
                        const plistElem_t *item = PList_GetElemArrayByIndex((plistElem_t*)plist, idx);
                        AR_ASSERT(item != NULL);
                        status = _flattenPlist(item, objlist, objtable, uniquingset);
                        
                        if(status != AR_S_YES)
                        {
                                return status;
                        }
                }
        }
        
        return AR_S_YES;
        
}


/* Get the number of bytes required to hold the value in 'count'. Will return a power of 2 value big enough to hold 'count'.
 */
static AR_INLINE ar_byte_t _byteCount(ar_uint_64_t count)
{
        ar_uint_64_t mask = ~(ar_uint_64_t)0;
        ar_byte_t size = 0;
        
        // Find something big enough to hold 'count'
        while (count & mask)
        {
                size++;
                mask = mask << 8;
        }
        
        // Ensure that 'count' is a power of 2
        // For sizes bigger than 8, just use the required count
        while ((size != 1 && size != 2 && size != 4 && size != 8) && size <= 8)
        {
                size++;
        }
        
        return size;
}



static arStatus_t _appendInt(arBuffer_t *buf, ar_uint_64_t bigint)
{
        ar_byte_t marker;
        ar_byte_t *bytes;
        size_t nbytes;
        
        if(bigint <= (ar_uint_64_t)0xff)
        {
                nbytes = 1;
                marker = kCFBinaryPlistMarkerInt | 0;
                
        } else if (bigint <= (ar_uint_64_t)0xffff)
        {
                nbytes = 2;
                marker = kCFBinaryPlistMarkerInt | 1;
                
        } else if (bigint <= (ar_uint_64_t)0xffffffff)
        {
                nbytes = 4;
                marker = kCFBinaryPlistMarkerInt | 2;
                
        } else
        {
                nbytes = 8;
                marker = kCFBinaryPlistMarkerInt | 3;
        }
        
        bigint = AR_LTON_U64(bigint);
        bytes = (ar_byte_t*)&bigint + sizeof(bigint) - nbytes;
        
        if(AR_InsertToBuffer(buf, &marker, 1) != AR_S_YES)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                return AR_E_NOMEM;
        }
        
        if(AR_InsertToBuffer(buf, bytes, nbytes) != AR_S_YES)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                return AR_E_NOMEM;
        }
        
        return AR_S_YES;
        
}





/*******************/

static ar_uint_64_t    __flattenplist_hash_func(void *key, void *ctx)
{
        AR_UNUSED(ctx);
        AR_ASSERT(key != NULL);
        return PList_HashElem((const plistElem_t*)key);
}

static ar_int_t   __flattenplist_comp_func(void *l, void *r, void *ctx)
{
        AR_UNUSED(ctx);
        return PList_CompElem((plistElem_t*)l, (plistElem_t*)r);
}



arStatus_t              PList_SaveElemToBinary(const plistElem_t *elem, arBuffer_t *out)
{
        arStatus_t status;
        
        plistArray_t objlist;
        arHash_t *objtable;
        arHash_t *uniquingset;
        
        ar_uint_64_t *offsets, length_so_far;
        ar_uint_64_t refnum;
        size_t  idx, idx2, cnt;
        
        CFBinaryPlistTrailer trailer;
        
        AR_ASSERT(elem != NULL && out != NULL);
        
        status = AR_S_YES;
        objtable = NULL;
        uniquingset = NULL;
        offsets = NULL;
        length_so_far = 0;
        refnum = 0;
        
        PList_InitArray(&objlist);
        
        objtable = AR_CreateHash(123, __flattenplist_hash_func, __flattenplist_comp_func, NULL, NULL, NULL, NULL, NULL);
        uniquingset = AR_CreateHash(123, __flattenplist_hash_func, __flattenplist_comp_func, NULL, NULL, NULL, NULL, NULL);
        
        if(objtable == NULL || uniquingset == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        AR_ClearBuffer(out);
        
        status = _flattenPlist(elem, &objlist, objtable, uniquingset);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        
        status = AR_InsertToBuffer(out, (const ar_byte_t*)"bplist00", 8);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        
        cnt = PList_GetArrayCount(&objlist);
        
        offsets = AR_NEWARR0(ar_uint_64_t, cnt);
        
        if(offsets == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        
        AR_memset(&trailer, 0, sizeof(trailer));
        
        trailer._numObjects = AR_LTON_U64((ar_uint_64_t)cnt);
        trailer._topObject = 0;	// true for this implementation
        trailer._objectRefSize = _byteCount(cnt);

        for(idx = 0; idx < cnt; ++idx)
        {
                plistElem_t *item = PList_GetArrayByIndex(&objlist, (size_t)idx);
				plistElemType_t type ;
                AR_ASSERT(item != NULL);
                type = PList_GetElemType(item);
                offsets[idx] = (ar_uint_64_t)AR_GetBufferAvailable(out);
                
                if (type == PLIST_ELEM_STRING_T)
                {
                        
                        ar_uint_16_t *chars = NULL, chars_buf[512];
                        
                        const wchar_t *wcs = PList_GetElemCString(elem);
                        size_t  count = AR_wcslen(wcs);
                        ar_byte_t marker;
                        
                        if(count > 0)
                        {
                                if(count <= 512)
                                {
                                        chars = chars_buf;
                                }else
                                {
                                        chars = AR_NEWARR0(ar_uint_16_t, count);
                                        
                                        if(chars == NULL)
                                        {
                                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                                status = AR_E_NOMEM;
                                                goto END_POINT;
                                        }
                                }
                        }
                        
                        
                        marker = (ar_byte_t)(kCFBinaryPlistMarkerUnicode16String | (count < 15 ? count : 0xf));
                        
                        status = AR_InsertToBuffer(out, &marker, 1);
                        
                        if(status != AR_S_YES)
                        {
                                if(chars && chars != chars_buf)
                                {
                                        AR_DEL(chars);
                                        chars = NULL;
                                }
                                
                                goto END_POINT;
                        }
                        
                        if(count >= 15)
                        {
                                status = _appendInt(out, (ar_uint_64_t)count);
                                
                                if(status != AR_S_YES)
                                {
                                        if(chars && chars != chars_buf)
                                        {
                                                AR_DEL(chars);
                                                chars = NULL;
                                        }
                                        goto END_POINT;
                                }
                        }
                        
                        for(idx2 = 0; idx2 < count; ++idx2)
                        {
                                chars[idx2] = AR_LTON_U16((ar_uint_16_t)wcs[idx2]);
                        }
                        
                        status = AR_InsertToBuffer(out, (const ar_byte_t*)chars, count * sizeof(ar_uint_16_t));
                        
                        if(status != AR_S_YES)
                        {
                                if(chars && chars != chars_buf)
                                {
                                        AR_DEL(chars);
                                        chars = NULL;
                                }
                                goto END_POINT;
                        }

                        if(chars && chars != chars_buf)
                        {
                                AR_DEL(chars);
                                chars = NULL;
                        }
                        
                } else if (type == PLIST_ELEM_NUMBER_T)
                {
                        ar_byte_t marker;
                        ar_uint_64_t bigint;
                        const plistNumber_t *number = PList_GetElemNumber(elem);
                        AR_ASSERT(number != NULL);
                        
                        if(number->type == PLIST_NUMBER_REAL_T)
                        {
                                ar_uint_64_t swapped64;
                                
                                AR_memcpy((ar_byte_t*)&swapped64, (const ar_byte_t*)&number->real.num, sizeof(number->real.num));
                                swapped64 = AR_LTON_U64(swapped64);
                                
                                
                                marker = kCFBinaryPlistMarkerReal | 3;
                                status = AR_InsertToBuffer(out, &marker, 1);
                                if(status != AR_S_YES)
                                {
                                        goto END_POINT;
                                }
                                
                                status = AR_InsertToBuffer(out, (const ar_byte_t*)&swapped64, 8);
                                if(status != AR_S_YES)
                                {
                                        goto END_POINT;
                                }
                                
                        }else
                        {
                                bigint = number->integer.unsigned_num;
                                status = _appendInt(out, bigint);
                                
                                if(status != AR_S_YES)
                                {
                                        goto END_POINT;
                                }

                        }
                }else if(type == PLIST_ELEM_BOOLEAN_T)
                {
                        
                }
                
        }

        
        
END_POINT:
        
        if(offsets)
        {
                AR_DEL(offsets);
                offsets = NULL;
        }
        
        if(objtable)
        {
                AR_DestroyHash(objtable);
                objtable = NULL;
        }
        
        if(uniquingset)
        {
                AR_DestroyHash(uniquingset);
                uniquingset = NULL;
        }
        PList_UnInitArray(&objlist);
        
        
        
        return status;
        
}


/*******************/

static ar_uint_64_t    __set_hash_func(void *key, void *ctx)
{
        AR_UNUSED(ctx);
        return (ar_uint_64_t)key;
}

ar_int_t   __set_comp_func(void *l, void *r, void *ctx)
{
        AR_UNUSED(ctx);
        return AR_CMP(l, r);
}




ar_bool_t      __parse_binary_plist_object(const ar_byte_t *databytes, size_t datalen, ar_uint_64_t startOffset, const CFBinaryPlistTrailer *trailer, arHash_t *objects, arHash_t *set, size_t curr_depth, plistElem_t **pelem)
{

        ar_uint_64_t objectsRangeStart, objectsRangeEnd;
        
        ar_uint_64_t off;
        plistElem_t **list;
        plistElem_t *buf[256];
        ar_byte_t       marker;
        AR_ASSERT(databytes != NULL && datalen > 0 && trailer != NULL && pelem != NULL);

        
        
        if(objects)
        {
				size_t offset = (size_t)startOffset;
                plistElem_t *val = NULL;
                AR_FindFromHash(objects, (void*)offset, (void**)&val);
                
                if(val)
                {
                        *pelem = PList_CopyNewElem(val);
                        if(*pelem == NULL)
                        {
                                return false;
                        }else
                        {
                                /*
                                if(PList_GetElemType(*pelem) == PLIST_ELEM_STRING_T)
                                {
                                        AR_DPRINT(L"%ls\r\n", PList_GetElemCString(*pelem));
                                }
                                */
                                return true;
                        }
                }
        }
        
        if(set)
        {
				size_t offset = (size_t)startOffset;
                if(AR_FindFromHash(set, (void*)offset, NULL) == AR_S_YES)
                {
                        return false;
                }
        }

        
        objectsRangeStart = 8;
        objectsRangeEnd = trailer->_offsetTableOffset - 1;
        
        if(startOffset < objectsRangeStart || objectsRangeEnd < startOffset)
        {
                return false;
                
        }
        
        marker = *(databytes + startOffset);
        
        switch(marker & 0xf0)
        {
                case kCFBinaryPlistMarkerNull:
                {
                        switch(marker)
                        {
                                case kCFBinaryPlistMarkerNull:
                                {
                                        *pelem = PList_CreateElem(PLIST_ELEM_NUMBER_T);
                                        if(*pelem == NULL)
                                        {
                                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                                return false;
                                        }
                                        
                                        PList_SetElemSignedInteger(*pelem, 0);
                                        return true;
                                }
                                case kCFBinaryPlistMarkerFalse:
                                {
                                        *pelem = PList_CreateElem(PLIST_ELEM_BOOLEAN_T);
                                        if(*pelem == NULL)
                                        {
                                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                                return false;
                                        }
                                        PList_SetElemBooleanValue(*pelem, false);
                                        return true;
                                }
                                case kCFBinaryPlistMarkerTrue:
                                {
                                        *pelem = PList_CreateElem(PLIST_ELEM_BOOLEAN_T);
                                        if(*pelem == NULL)
                                        {
                                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                                return false;
                                        }
                                        PList_SetElemBooleanValue(*pelem, true);

                                        return true;
                                }
                                default:
                                {
                                        return false;
                                }
                        }
                }
                        break;
                case kCFBinaryPlistMarkerInt:
                {
                        const ar_byte_t *ptr = (databytes + startOffset);
                        ar_int_t err = CF_NO_ERROR;
                        ar_uint_64_t cnt;
                        const ar_byte_t *extent;
                        
                        ar_uint_64_t bigint;
                        
                        ptr = check_ptr_add(ptr, 1, &err);
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        cnt = (ar_uint_64_t)(1 << (marker & 0x0f));
                        extent = check_ptr_add(ptr, cnt, &err) - 1;
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        if(databytes + objectsRangeEnd < extent)
                        {
                                return false;
                        }
                        
                        if (cnt > 16)
                        {
                                return false;
                        }
                        
                        // in format version '00', 1, 2, and 4-byte integers have to be interpreted as unsigned,
                        // whereas 8-byte integers are signed (and 16-byte when available)
                        // negative 1, 2, 4-byte integers are always emitted as 8 bytes in format '00'
                        // integers are not required to be in the most compact possible representation, but only the last 64 bits are significant currently
                        
                        bigint = _getSizedInt(ptr, (ar_uint_8_t)cnt);
                        ptr += cnt;
                        
                        *pelem = PList_CreateElem(PLIST_ELEM_NUMBER_T);
                        
                        if(*pelem == NULL)
                        {
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                return false;
                        }
                        
                        if(cnt < 8)
                        {
                                PList_SetElemUnsignedInteger(*pelem, (ar_uint_32_t)bigint);
                        }else
                        {
                                PList_SetElemSignedInteger(*pelem, (ar_int_64_t)bigint);
                        }
                        
                        if(objects && *pelem)
                        {
								size_t offset = (size_t)startOffset;
                                if(AR_SetToHash(objects, (void*)offset, *pelem) != AR_S_YES)
                                {
                                        PList_DestroyElem(*pelem);
                                        *pelem = NULL;
                                        
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                }
                        }
                        
                        if(*pelem == NULL)
                        {
                                return false;
                        }else
                        {
                                return true;
                        }
                }
                        break;
                case kCFBinaryPlistMarkerReal:
                {
                        switch (marker & 0x0f)
                        {
                        case 2:
                        {
                                const ar_byte_t *ptr = (databytes + startOffset);
                                ar_int_t err = CF_NO_ERROR;
                                
                                ar_uint_32_t swapped32;
                                const ar_byte_t *extent;
                                float f;
                                ptr = check_ptr_add(ptr, 1, &err);
                                
                                if (CF_NO_ERROR != err)
                                {
                                        return false;
                                }
                                
                                extent = check_ptr_add(ptr, 4, &err) - 1;
                                
                                if(CF_NO_ERROR != err)
                                {
                                        return false;
                                }
                                
                                if (databytes + objectsRangeEnd < extent)
                                {
                                        return false;
                                }
                                
                                
                                AR_memcpy(&swapped32, ptr, 4);
                                
                                swapped32 = AR_NTOL_U32(swapped32);
                                AR_memcpy((void*)&f, (void*)&swapped32, 4);
                                
                                
                                *pelem = PList_CreateElem(PLIST_ELEM_NUMBER_T);
                                
                                if(*pelem == NULL)
                                {
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        return false;
                                }

                                PList_SetElemReal(*pelem, (double)f);
                                
                                if (objects && *pelem)
                                {
										size_t offset = (size_t)startOffset;
                                        if(AR_SetToHash(objects, (void*)offset, *pelem) != AR_S_YES)
                                        {
                                                PList_DestroyElem(*pelem);
                                                *pelem = NULL;
                                                
                                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        }
                                }
                                
                                if(*pelem == NULL)
                                {
                                        return false;
                                }else
                                {
                                        return true;
                                }
                        }
                        case 3:
                        {
                                const ar_byte_t *ptr = (databytes + startOffset);
                                
                                ar_int_t err = CF_NO_ERROR;
                                const ar_byte_t *extent;
                                ar_uint_64_t swapped64;
                                double d;
                                
                                ptr = check_ptr_add(ptr, 1, &err);
                                
                                if (CF_NO_ERROR != err)
                                {
                                        return false;
                                }
                                
                                extent = check_ptr_add(ptr, 8, &err) - 1;
                                
                                if (CF_NO_ERROR != err)
                                {
                                        return false;
                                }
                                
                                if (databytes + objectsRangeEnd < extent)
                                {
                                        return false;
                                }
                                
                                
                                
                                AR_memcpy(&swapped64, ptr, 8);
                                swapped64 = AR_NTOL_U64(swapped64);

                                AR_memcpy((void*)&d, &swapped64, 8);
                                
                                
                                *pelem = PList_CreateElem(PLIST_ELEM_NUMBER_T);
                                
                                if(*pelem == NULL)
                                {
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        return false;
                                }
                                
                                
                                PList_SetElemReal(*pelem, d);
                                

                                if (objects && *pelem)
                                {
										size_t offset = (size_t)startOffset;
                                        if(AR_SetToHash(objects, (void*)offset, *pelem) != AR_S_YES)
                                        {
                                                PList_DestroyElem(*pelem);
                                                *pelem = NULL;
                                                
                                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        }
                                }
                                
                                if(*pelem == NULL)
                                {
                                        return false;
                                }else
                                {
                                        return true;
                                }
                                
                        }
                        default:
                                return false;
                        }

                }
                        break;
                case kCFBinaryPlistMarkerDate & 0xf0:
                {
                        switch (marker)
                        {
                                case kCFBinaryPlistMarkerDate:
                                {
                                        const ar_byte_t *extent;
                                        const ar_byte_t *ptr = (databytes + startOffset);
                                        ar_int_t err = CF_NO_ERROR;
                                        ar_uint_64_t swapped64;
                                        double d;
                                        ar_uint_16_t year, mon, day, hour, min, sec;
                                        
                                        ptr = check_ptr_add(ptr, 1, &err);
                                        
                                        if (CF_NO_ERROR != err)
                                        {
                                                return false;
                                        }
                                        
                                        extent = check_ptr_add(ptr, 8, &err) - 1;
                                        
                                        if (CF_NO_ERROR != err)
                                        {
                                                return false;
                                        }
                                        
                                        if(databytes + objectsRangeEnd < extent)
                                        {
                                                return false;
                                        }
                                        
                                        AR_memcpy(&swapped64, ptr, 8);
                                        
                                        swapped64 = AR_NTOL_U64(swapped64);
                                        AR_memcpy((void*)&d, &swapped64, 8);
                                        
                                        
                                        __absolutetime_to_gmtime(d, &year, &mon, &day, &hour, &min, &sec);
                                        
                                        
                                        *pelem = PList_CreateElem(PLIST_ELEM_DATE_T);
                                        
                                        if(*pelem == NULL)
                                        {
                                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                                return false;
                                        }
                                        
                                        
                                        PList_SetElemGMTDate(*pelem, year, mon, day, hour,min, sec);

                                        
                                        // these are always immutable
                                        if (objects && *pelem)
                                        {
												size_t offset = (size_t)startOffset;
                                                if(AR_SetToHash(objects, (void*)offset, *pelem) != AR_S_YES)
                                                {
                                                        PList_DestroyElem(*pelem);
                                                        *pelem = NULL;
                                                        
                                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                                }
                                        }
                                        
                                        if(*pelem == NULL)
                                        {
                                                return false;
                                        }else
                                        {
                                                return true;
                                        }

                                }
                        }
                        return false;
                }
                        
                        break;
                case kCFBinaryPlistMarkerData:
                {
                        const ar_byte_t *ptr = databytes + startOffset;
                        ar_int_t err = CF_NO_ERROR;
                        size_t  cnt;
                        
                        const ar_byte_t *extent;
                        
                        ptr = check_ptr_add(ptr, 1, &err);
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        cnt = (size_t)(marker & 0x0f);
                        
                        if (0xf == cnt)
                        {
                                ar_uint_64_t bigint = 0;
                                
                                if (!_readInt(ptr, databytes + objectsRangeEnd, &bigint, &ptr))
                                {
                                        return false;
                                }
                                
                                if (LONG_MAX < bigint)
                                {
                                        return false;
                                }
                                
                                cnt = (size_t)bigint;
                        }
                        
                        extent = check_ptr_add(ptr, cnt, &err) - 1;
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        if (databytes + objectsRangeEnd < extent)
                        {
                                return false;
                        }
                        
                        
                        *pelem = PList_CreateElem(PLIST_ELEM_DATA_T);
                        
                        if(*pelem == NULL)
                        {
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                return false;
                        }
                        
                        if(PList_SetElemData(*pelem, ptr, cnt) != AR_S_YES)
                        {
                                PList_DestroyElem(*pelem);
                                *pelem = NULL;
                                return false;
                        }
                        
                        
                        if (objects && *pelem)
                        {
								size_t offset = (size_t)startOffset;
                                if(AR_SetToHash(objects, (void*)offset, *pelem) != AR_S_YES)
                                {
                                        PList_DestroyElem(*pelem);
                                        *pelem = NULL;
                                        
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                }
                        }
                        
                        if(*pelem == NULL)
                        {
                                return false;
                        }else
                        {
                                return true;
                        }
                }
                        
                        break;
                        
                case kCFBinaryPlistMarkerASCIIString:
                {
                        const ar_byte_t *ptr = databytes + startOffset;
                        ar_int_t err = CF_NO_ERROR;
                        size_t cnt;
                        const ar_byte_t *extent;
                        size_t i;
                        
                        ptr = check_ptr_add(ptr, 1, &err);
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        cnt = (size_t)(marker & 0x0f);
                        
                        if (0xf == cnt)
                        {
                                ar_uint_64_t bigint = 0;
                                if (!_readInt(ptr, databytes + objectsRangeEnd, &bigint, &ptr))
                                {
                                        return false;
                                }
                                
                                if (LONG_MAX < bigint)
                                {
                                        return false;
                                }
                                
                                cnt = (size_t)bigint;
                        }
                        
                        extent = check_ptr_add(ptr, cnt, &err) - 1;
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        if (databytes + objectsRangeEnd < extent)
                        {
                                return false;
                        }
                        
                        
                        *pelem = PList_CreateElem(PLIST_ELEM_STRING_T);
                        
                        if(*pelem == NULL)
                        {
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                return false;
                        }
                        
                        if(PList_ReserveElemString(*pelem, cnt) != AR_S_YES)
                        {
                                PList_DestroyElem(*pelem);
                                *pelem = NULL;
                                return false;
                        }
                        
                        for(i = 0; i < cnt; ++i)
                        {
                                wchar_t ch = (wchar_t)ptr[i];
                                PList_AppendCharToElemString(*pelem, ch);
                        }
                        
                        //AR_DPRINT(L"kCFBinaryPlistMarkerASCIIString : %ls\r\n", PList_GetElemCString(*pelem));
                        
                        if (objects && *pelem)
                        {
								size_t offset = (size_t)startOffset;
                                if(AR_SetToHash(objects, (void*)offset, *pelem) != AR_S_YES)
                                {
                                        PList_DestroyElem(*pelem);
                                        *pelem = NULL;
                                        
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                }
                        }
                        
                        if(*pelem == NULL)
                        {
                                return false;
                        }else
                        {
                                return true;
                        }
                }
                        break;
                case kCFBinaryPlistMarkerUnicode16String:
                {
                        const ar_byte_t *ptr = databytes + startOffset;
                        ar_int_t err = CF_NO_ERROR;
                        size_t cnt;
                        const ar_byte_t *extent;
                        size_t byte_cnt;
                        size_t i;
                        ptr = check_ptr_add(ptr, 1, &err);
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        cnt = (size_t)(marker & 0x0f);
                        
                        if(0xf == cnt)
                        {
                                ar_uint_64_t bigint = 0;
                                if (!_readInt(ptr, databytes + objectsRangeEnd, &bigint, &ptr))
                                {
                                        return false;
                                }
                                
                                if (LONG_MAX < bigint)
                                {
                                        return false;
                                }
                                cnt = (size_t)bigint;
                        }
                        
                        extent = check_ptr_add(ptr, cnt, &err) - 1;
                        
                        extent = check_ptr_add(extent, cnt, &err);	// 2 bytes per character
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        if (databytes + objectsRangeEnd < extent)
                        {
                                return false;
                        }
                        
                        byte_cnt = check_size_t_mul(cnt, sizeof(ar_uint_16_t), &err);
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        
                        *pelem = PList_CreateElem(PLIST_ELEM_STRING_T);
                        
                        if(*pelem == NULL)
                        {
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                return false;
                        }

                        
                        if(PList_ReserveElemString(*pelem, cnt) != AR_S_YES)
                        {
                                PList_DestroyElem(*pelem);
                                *pelem = NULL;
                                return false;
                        }

                        
                        i = 0;
                        while(i < cnt)
                        {
                                ar_uint_16_t *val = ((ar_uint_16_t*)ptr) + i;
                                wchar_t ch = (wchar_t)AR_NTOL_U16(*val);

                                PList_AppendCharToElemString(*pelem, ch);
                                ++i;
                        }
                        

                        //AR_DPRINT(L"kCFBinaryPlistMarkerUnicode16String : %ls\r\n", PList_GetElemCString(*pelem));

                        if( objects && *pelem)
                        {
								size_t offset = (size_t)startOffset;
                                if(AR_SetToHash(objects, (void*)offset, *pelem) != AR_S_YES)
                                {
                                        PList_DestroyElem(*pelem);
                                        *pelem = NULL;
                                        
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                }
                        }

                        if(*pelem == NULL)
                        {
                                return false;
                        }else
                        {
                                return true;
                        }
                        
                }

                        break;

                case kCFBinaryPlistMarkerUID:
                {
                        const ar_byte_t *ptr = databytes + startOffset;
                        size_t cnt;
                        const ar_byte_t *extent;
                        ar_int_t err = CF_NO_ERROR;
                        ar_uint_64_t bigint;
                        ptr = check_ptr_add(ptr, 1, &err);
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        cnt = (marker & 0x0f) + 1;
                        
                        extent = check_ptr_add(ptr, cnt, &err) - 1;
                        
                        if(CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        if (databytes + objectsRangeEnd < extent)
                        {
                                return false;
                        }
                        
                        // uids are not required to be in the most compact possible representation, but only the last 64 bits are significant currently
                        bigint = _getSizedInt(ptr, (ar_uint_8_t)cnt);
                        ptr += cnt;
                        if (AR_UINT32_MAX < bigint)
                        {
                                return false;
                        }

                        *pelem = PList_CreateElem(PLIST_ELEM_NUMBER_T);
                        
                        if(*pelem == NULL)
                        {
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                return false;
                        }
                        
                        PList_SetElemUnsignedInteger(*pelem, (ar_uint_32_t)bigint);
                        
                        if(objects && *pelem)
                        {
								size_t offset = (size_t)startOffset;
                                if(AR_SetToHash(objects, (void*)offset, *pelem) != AR_S_YES)
                                {
                                        PList_DestroyElem(*pelem);
                                        *pelem = NULL;
                                        
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                }
                        }
                        
                        if(*pelem == NULL)
                        {
                                return false;
                        }else
                        {
                                return true;
                        }
                }
                        break;

                case kCFBinaryPlistMarkerArray:
                case kCFBinaryPlistMarkerSet:
                {
                        const ar_byte_t *ptr = databytes + startOffset;
                        ar_int_t err = CF_NO_ERROR;
                        size_t cnt;
                        size_t byte_cnt;
                        const ar_byte_t *extent;
                        ar_bool_t madeSet;
                        size_t idx;
                        ptr = check_ptr_add(ptr, 1, &err);
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        cnt = marker & 0x0f;
                        
                        if(cnt == 0xf)
                        {
                                ar_uint_64_t bigint = 0;
                                
                                if (!_readInt(ptr, databytes + objectsRangeEnd, &bigint, &ptr))
                                {
                                        return false;
                                }
                                
                                if(LONG_MAX < bigint)
                                {
                                        return false;
                                }
                                
                                cnt = (size_t)bigint;
                        }
                        
                        byte_cnt = check_size_t_mul(cnt, trailer->_objectRefSize, &err);
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        extent = check_ptr_add(ptr, byte_cnt, &err) - 1;
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        if (databytes + objectsRangeEnd < extent)
                        {
                                return false;
                        }
                        
                        byte_cnt = check_size_t_mul(cnt, sizeof(plistElem_t*), &err);
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        if(cnt <= 256)
                        {
                                list = buf;
                        }else
                        {
                                list = AR_NEWARR(plistElem_t*, cnt);
                                
                                if(list == NULL)
                                {
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        return false;
                                }
                        }
                        
                        
                        madeSet = false;
                        
                        if (!set && curr_depth > 15)
                        {
                                set = AR_CreateHash(123, __set_hash_func, __set_comp_func, NULL, NULL, NULL, NULL, NULL);
                                madeSet = set ? true : false;
                        }
                        
                        if(set)
                        {
								size_t offset = (size_t)startOffset;
                                if(AR_SetToHash(set, (void*)offset, NULL) != AR_S_YES)
                                {
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        
                                        if(list != buf)
                                        {
                                                AR_DEL(list);
                                                list = NULL;
                                        }
                                        
                                        if(madeSet)
                                        {
                                                AR_DestroyHash(set);
                                                set = NULL;
                                                madeSet = false;
                                        }
                                        return false;
                                }
                        }
                        
                        
                        for (idx = 0; idx < cnt; idx++)
                        {
                                plistElem_t *pl;

                                off = _getOffsetOfRefAt(databytes, ptr, trailer);
                                
                                if (!__parse_binary_plist_object(databytes, datalen, off, trailer,objects, set, curr_depth + 1, &pl))
                                {
                                        while(idx--)
                                        {
                                                PList_DestroyElem(list[idx]);
                                                list[idx] = NULL;
                                        }
                                        
                                        if (list != buf)
                                        {
                                                AR_DEL(list);
                                                list = NULL;
                                        }
                                        
                                        if(madeSet)
                                        {
                                                AR_DestroyHash(set);
                                                set = NULL;
                                                madeSet = false;
                                        }
                                        
                                        return false;
                                }
                                
                                list[idx] = pl;
                                ptr += trailer->_objectRefSize;
                        }
                        
                        
                        if(set)
                        {
								size_t offset = (size_t)startOffset;
                                AR_RemoveFromHash(set, (void*)offset);
                        }
                        
                        if(madeSet)
                        {
                                AR_DestroyHash(set);
                                set = NULL;
                                madeSet = false;
                        }
                        
                        
                        *pelem = PList_CreateElem(PLIST_ELEM_ARRAY_T);
                        
                        if(*pelem == NULL)
                        {
                                size_t i;
                                for(i = 0; i < cnt; i++)
                                {
                                        PList_DestroyElem(list[i]);
                                        list[i] = NULL;
                                }
                                
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                
                                return false;
                        }
                        
                        for(idx = 0; idx < cnt; ++idx)
                        {
                                if(PList_AppendToElemArray(*pelem, list[idx]) != AR_S_YES)
                                {
                                        size_t i;
                                        for(i = 0; i < cnt; i++)
                                        {
                                                if(list[i])
                                                {
                                                        PList_DestroyElem(list[i]);
                                                        list[i] = NULL;
                                                }
                                        }
                                        
                                        PList_DestroyElem(*pelem);
                                        *pelem = NULL;
                                }else
                                {
                                        list[idx] = NULL;
                                }
                        }
                        
                        if(list != buf)
                        {
                                AR_DEL(list);
                                list = NULL;
                        }
                        
                        if(objects && *pelem)
                        {
								size_t offset = (size_t)startOffset;
                                if(AR_SetToHash(objects, (void*)offset, *pelem) != AR_S_YES)
                                {
                                        PList_DestroyElem(*pelem);
                                        *pelem = NULL;
                                        
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                }
                        }
                        
                        if(*pelem == NULL)
                        {
                                return false;
                        }else
                        {
                                return true;
                        }
                }

                        break;
                case kCFBinaryPlistMarkerDict:
                {
                        const ar_byte_t *ptr = databytes + startOffset;
                        ar_int_t err = CF_NO_ERROR;
                        size_t cnt;
                        size_t byte_cnt;
                        const ar_byte_t *extent;
                        ar_bool_t madeSet;
                        size_t idx;
                        
                        ptr = check_ptr_add(ptr, 1, &err);
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        cnt = (size_t)(marker & 0x0f);
                        
                        if(0xf == cnt)
                        {
                                ar_uint_64_t bigint = 0;
                                
                                if (!_readInt(ptr, databytes + objectsRangeEnd, &bigint, &ptr))
                                {
                                        return false;
                                }
                                
                                if (LONG_MAX < bigint)
                                {
                                        return false;
                                }
                                
                                cnt = (size_t)bigint;
                        }
                        cnt = check_size_t_mul(cnt, 2, &err);
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        byte_cnt = check_size_t_mul(cnt, trailer->_objectRefSize, &err);
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        extent = check_ptr_add(ptr, byte_cnt, &err) - 1;
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        if (databytes + objectsRangeEnd < extent)
                        {
                                return false;
                        }
                        
                        byte_cnt = check_size_t_mul(cnt, sizeof(plistElem_t*), &err);
                        
                        if (CF_NO_ERROR != err)
                        {
                                return false;
                        }
                        
                        if(cnt <= 256)
                        {
                                list = buf;
                        }else
                        {
                                list = AR_NEWARR(plistElem_t*, cnt);
                                
                                if(list == NULL)
                                {
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        return false;
                                }
                        }
                        

                        
                        madeSet = false;
                        
                        if (set == NULL && curr_depth > 15)
                        {
                                set = AR_CreateHash(123, __set_hash_func, __set_comp_func, NULL, NULL, NULL, NULL, NULL);
                                madeSet = set ? true : false;
                        }
                        
                        if(set)
                        {
								size_t offset = (size_t)startOffset;
                                if(AR_SetToHash(set, (void*)offset, NULL) != AR_S_YES)
                                {
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        
                                        if(list != buf)
                                        {
                                                AR_DEL(list);
                                                list = NULL;
                                        }
                                        
                                        if(madeSet)
                                        {
                                                AR_DestroyHash(set);
                                                set = NULL;
                                                madeSet = false;
                                        }
                                        return false;
                                }
                        }

                        
                        for (idx = 0; idx < cnt; idx++)
                        {
                                plistElem_t *pl = NULL;
                                
                                off = _getOffsetOfRefAt(databytes, ptr, trailer);
                                
                                if (!__parse_binary_plist_object(databytes, datalen, off, trailer,objects, set, curr_depth + 1, &pl))
                                {
                                        while(idx--)
                                        {
                                                PList_DestroyElem(list[idx]);
                                                list[idx] = NULL;
                                        }
                                        
                                        if (list != buf)
                                        {
                                                AR_DEL(list);
                                        }
                                        
                                        list = NULL;
                                        
                                        if(madeSet)
                                        {
                                                AR_DestroyHash(set);
                                                set = NULL;
                                                madeSet = false;
                                        }
                                        
                                        return false;
                                }
                                
                                list[idx] = pl;
                                ptr += trailer->_objectRefSize;
                        }
                        
                        
                        if(set)
                        {
								size_t offset = (size_t)startOffset;
                                AR_RemoveFromHash(set, (void*)offset);
                        }
                        
                        if(madeSet)
                        {
                                AR_DestroyHash(set);
                                set = NULL;
                                madeSet = false;
                        }
                        
                        *pelem = PList_CreateElem(PLIST_ELEM_DICT_T);
                        
                        if(*pelem == NULL)
                        {
                                size_t i;
                                for(i = 0; i < cnt; i++)
                                {
                                        PList_DestroyElem(list[i]);
                                        list[i] = NULL;
                                }
                                
                                if(list != buf)
                                {
                                        AR_DEL(list);
                                }
                                
                                list = NULL;
                                
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                
                                return false;
                        }
                        
                        for(idx = 0; idx < cnt / 2; ++idx)
                        {
                                if(PList_SetElemDictValueForKey(*pelem, list[idx], list[idx + cnt/2]) != AR_S_YES)
                                {
                                        size_t i;
                                        for(i = 0; i < cnt; i++)
                                        {
                                                PList_DestroyElem(list[i]);
                                                list[i] = NULL;
                                        }
                                        
                                        if(list != buf)
                                        {
                                                AR_DEL(list);
                                        }
                                        
                                        list = NULL;

                                        
                                        PList_DestroyElem(*pelem);
                                        *pelem = NULL;
                                        
                                        return false;

                                }else
                                {
                                        list[idx] = list[idx + cnt/2] = NULL;
                                }
                        }
                        
                        if(list != buf)
                        {
                                AR_DEL(list);
                        }
                        
                        list = NULL;
                        
                        if(objects && *pelem)
                        {
								size_t offset = (size_t)startOffset;
                                if(AR_SetToHash(objects, (void*)offset, *pelem) != AR_S_YES)
                                {
                                        PList_DestroyElem(*pelem);
                                        *pelem = NULL;
                                        
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                }
                        }
                        
                        if(*pelem == NULL)
                        {
                                return false;
                        }else
                        {
                                return true;
                        }
                }

        }


        return false;
}



arStatus_t      PList_TryParseBinaryPlist(const ar_byte_t *data, size_t length, plistElem_t **pelem, arString_t *errmsg)
{
        ar_uint_8_t marker;
        CFBinaryPlistTrailer trailer;
        ar_uint_64_t offset;
        arHash_t *objects;
        arStatus_t status;
        plistElem_t *result;
        AR_ASSERT(data != NULL && length > 0 && pelem != NULL);
        
        status = AR_S_YES;
        objects = NULL;
        result = NULL;
        if(length < 8)
        {
                if(errmsg)
                {
                        AR_FormatString(errmsg, L"Invalid data length : %Iu!", length);
                }
                status = AR_E_MALFORMAT;
                goto END_POINT;
        }
        
        
        
        if(!__get_binary_plist_toplevelinfo(data, length, &marker, &offset, &trailer))
        {
                if(errmsg)
                {
                        AR_FormatString(errmsg, L"Encountered invalid plist header : %Iu!", length);
                }
                status = AR_E_MALFORMAT;
                goto END_POINT;
        }
        
        // FALSE: We know for binary plist parsing that the result objects will be retained
        // by their containing collections as the parsing proceeds, so we do not need
        // to use retaining callbacks for the objects map in this case. WHY: the file might
        // be malformed and contain hash-equal keys for the same dictionary (for example)
        // and the later key will cause the previous one to be released when we set the second
        // in the dictionary.

        objects = AR_CreateHash(139, __set_hash_func, __set_comp_func, NULL, NULL, NULL, NULL, NULL);
        
        if(objects == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        
        if(__parse_binary_plist_object(data, length, offset, &trailer, objects, NULL, 0, &result))
        {
                *pelem = result;
        }else
        {
                *pelem = NULL;
                if(errmsg)
                {
                        AR_FormatString(errmsg, L"%ls", L"binary data is corrupt");
                }
                
                status = AR_E_MALFORMAT;
                
                
        }
        
        
        
END_POINT:
        if(objects)
        {
                AR_DPRINT(L"total objects : %Iu\r\n", AR_GetHashCount(objects));
                AR_DestroyHash(objects);
                objects = NULL;
        }
        return status;

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



