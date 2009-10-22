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
#include "common.h"





AR_NAMESPACE_BEGIN


struct __arsenal_str_rec
{
		wchar_t					 *str;
		size_t					 len;
		struct __arsenal_str_rec *next;
};


struct __string_table_
{
		arStringRec_t	**bucket;
		size_t			count;
};


arStringTable_t*		AR_CreateStrTable(size_t count)
{
		arStringTable_t		*res;

		res = AR_NEW0(arStringTable_t);
		if(count < MIN_BUCKET_SIZE)count = MIN_BUCKET_SIZE;
		res->count = count;
		res->bucket = AR_NEWARR0(arStringRec_t*, res->count);
		return res;
}


void					AR_DestroyStrTable(arStringTable_t* tbl)
{
		size_t i;
		arStringRec_t *curr, *tmp;		
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



const wchar_t*			AR_GetString(arStringTable_t *tbl, const wchar_t *str)
{
		size_t n;
		AR_ASSERT(tbl != NULL && str != NULL);
		n = AR_wcslen(str);
		return AR_GetStringN(tbl, str, n);
}





const wchar_t*			AR_GetStringN(arStringTable_t *tbl, const wchar_t *str, size_t n)
{
		size_t idx;
		arStringRec_t		*record;
		AR_ASSERT(tbl != NULL && str != NULL);

		idx =  AR_wcshash_n(str, n) % tbl->count;

		record = tbl->bucket[idx];

		while(record)
		{
				if(AR_wcsncmp(record->str, str, n) == 0)return record->str;
				record=record->next;
		}
		
		record = AR_NEW0(arStringRec_t);
		record->len = n;
		record->str = AR_NEWARR0(wchar_t, n + 1);
		AR_wcsncpy(record->str, str, n);
		record->str[n] = L'\0';
		record->next = tbl->bucket[idx];
		tbl->bucket[idx] = record;
		return record->str;
}

#if(0)
static const wchar_t *__ctbl = L"0123456789ABCDEF";

const wchar_t*			AR_GetStringInt(arStringTable_t *tbl, int64_t num, size_t radix)
{
		wchar_t buf[100];
		wchar_t *p;

		bool_t is_neg;
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
		return AR_GetString(tbl, p);
}

const wchar_t*			AR_GetStringUInt(arStringTable_t *tbl, uint64_t num, size_t radix)
{
		wchar_t buf[100];
		wchar_t *p;
		
		AR_ASSERT(tbl != NULL && radix > 0 && radix <= 16);
		

		p = buf + sizeof(buf)/sizeof(buf[0]);
		*--p = L'\0';
		
		do{ *--p = __ctbl[num%radix]; }while((num /= radix) > 0);
		
		return AR_GetString(tbl, p);

}
#endif

const wchar_t*			AR_GetStringInt(arStringTable_t *tbl, int64_t num, size_t radix)
{
		wchar_t buf[1024];

		AR_ASSERT(tbl != NULL && radix > 1 && radix <= 16);
		AR_i64tow_buf(buf, 1024, num, radix);
		return AR_GetString(tbl, buf);
}

const wchar_t*			AR_GetStringUInt(arStringTable_t *tbl, uint64_t num, size_t radix)
{
		wchar_t buf[1024];
		
		AR_ASSERT(tbl != NULL && radix > 1 && radix <= 16);
		
		AR_u64tow_buf(buf, 1024, num, radix);
		
		return AR_GetString(tbl, buf);

}


/******************************************************************************************************************/


AR_NAMESPACE_END

