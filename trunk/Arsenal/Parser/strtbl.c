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
#include "strtbl.h"


#define BUCKET_NUM		4096


AR_NAMESPACE_BEGIN










psrStringTable_t*		PSR_CreateStrTable(size_t count)
{
		psrStringTable_t		*res;

		res = AR_NEW0(psrStringTable_t);
		if(count < MIN_BUCKET_SIZE)count = MIN_BUCKET_SIZE;

		res->count = count;
		res->bucket = AR_NEWARR0(psrString_t*, res->count);
		return res;
}


void					PSR_DestroyStrTable(psrStringTable_t* tbl)
{
		size_t i;
		psrString_t *curr, *tmp;		
		AR_ASSERT(tbl != NULL);

		for(i = 0; i < tbl->count; ++i)
		{
				curr = tbl->bucket[i];
				while(curr)
				{
						tmp = curr->next;
						AR_DEL(curr->str);
						AR_DEL(curr);
						curr = tmp;
				}
		}
}



const wchar_t*			PSR_GetString(psrStringTable_t *tbl, const wchar_t *str)
{
		size_t n;
		AR_ASSERT(tbl != NULL && str != NULL);
		n = AR_wcslen(str);
		return PSR_GetStringN(tbl, str, n);
}




const wchar_t*			PSR_GetStringN(psrStringTable_t *tbl, const wchar_t *str, size_t n)
{
		size_t idx;
		psrString_t		*record;
		AR_ASSERT(tbl != NULL && str != NULL);

		idx =  AR_wcshash_n(str, n) % tbl->count;

		record = tbl->bucket[idx];

		while(record)
		{
				if(AR_wcsncmp(record->str, str, n) == 0)return record->str;
				record=record->next;
		}
		
		record = AR_NEW0(psrString_t);
		record->len = n;
		record->str = AR_NEWARR0(wchar_t, n + 1);
		AR_wcsncpy(record->str, str, n);
		record->str[n] = L'\0';
		record->next = tbl->bucket[idx];
		tbl->bucket[idx] = record;
		return record->str;
}


static const wchar_t *__ctbl = L"0123456789ABCDEF";

const wchar_t*			PSR_GetStringInt(psrStringTable_t *tbl, int64_t num, size_t radix)
{
		wchar_t buf[100];
		wchar_t *p;

		bool is_neg;
		AR_ASSERT(tbl != NULL && radix > 0 && radix <= 16);

		if(num < 0)
		{
				is_neg = true;
				num = -num;
		}else
		{
				is_neg = false;
		}

		p = buf + sizeof(buf)/sizeof(buf[0]);
		*--p = L'\0';
		
		do{ *--p = __ctbl[num%radix]; }while((num /= radix) > 0);
		
		if(is_neg)*--p = L'-';
		return PSR_GetString(tbl, p);
}

const wchar_t*			PSR_GetStringUInt(psrStringTable_t *tbl, uint64_t num, size_t radix)
{
		wchar_t buf[100];
		wchar_t *p;
		
		AR_ASSERT(tbl != NULL && radix > 0 && radix <= 16);
		

		p = buf + sizeof(buf)/sizeof(buf[0]);
		*--p = L'\0';
		
		do{ *--p = __ctbl[num%radix]; }while((num /= radix) > 0);
		
		return PSR_GetString(tbl, p);

}


/******************************************************************************************************************/


AR_NAMESPACE_END