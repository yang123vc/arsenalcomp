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

#include "tguutility.h"



AR_NAMESPACE_BEGIN



/*******************************************tengu string table*****************************************************/


static arSpinLock_t		__g_tbl_lock;
static arStringTable_t *__g_tbl = NULL;


static void		__init_string_table()
{
		AR_ASSERT(__g_tbl == NULL);
		AR_InitSpinLock(&__g_tbl_lock);
		__g_tbl			=		AR_CreateStrTable(TGU_STRTBL_BUCKET);
}

static void __uninit_string_table()
{
		AR_ASSERT(__g_tbl != NULL);
		AR_DestroyStrTable(__g_tbl);
		AR_UnInitSpinLock(&__g_tbl_lock);
		__g_tbl = NULL;

}




const wchar_t*	TGU_AllocString(const wchar_t *str)
{
		const wchar_t *res;
		AR_LockSpinLock(&__g_tbl_lock);
		res = AR_GetString(__g_tbl, str);
		AR_UnLockSpinLock(&__g_tbl_lock);
		return res;
}

const wchar_t*	TGU_AllocStringN(const wchar_t *str, size_t n)
{
		const wchar_t *res;
		AR_LockSpinLock(&__g_tbl_lock);
		res = AR_GetStringN(__g_tbl, str,n);
		AR_UnLockSpinLock(&__g_tbl_lock);
		return res;
}


const wchar_t*  TGU_AllocStringInt(int_64_t num, size_t radix)
{
		const wchar_t *res;
		AR_LockSpinLock(&__g_tbl_lock);
		res = AR_GetStringInt(__g_tbl, num, radix);
		AR_UnLockSpinLock(&__g_tbl_lock);
		return res;
}

const wchar_t*  TGU_AllocStringUInt(uint_64_t num, size_t radix)
{
		const wchar_t *res;
		AR_LockSpinLock(&__g_tbl_lock);
		res = AR_GetStringUInt(__g_tbl, num, radix);
		AR_UnLockSpinLock(&__g_tbl_lock);
		return res;
}

const wchar_t*  TGU_AllocStringFloat(double num)
{
		const wchar_t *res;
		AR_LockSpinLock(&__g_tbl_lock);
		res = AR_GetStringFloat(__g_tbl, num);
		AR_UnLockSpinLock(&__g_tbl_lock);
		return res;
}

/*************************************************************************************************************************************/

void	TGU_InitUtility()
{
		__init_string_table();
}

void	TGU_UnInitUtility()
{
		__uninit_string_table();
}


AR_NAMESPACE_END