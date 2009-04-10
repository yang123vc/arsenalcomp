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
#include "parser_in.h"
#include "symbol.h"
#include "strtbl.h"


AR_NAMESPACE_BEGIN

static volatile arSpinLock_t		__tbl_lock;
static psrStringTable_t *__tbl = NULL;

const psrSymb_t	*PSR_EpsilonSymb = NULL;/*表示空*/
const psrSymb_t	*PSR_EOISymb = NULL;   /*表示输入结尾符号*/
const psrSymb_t	*PSR_LALRSymb= NULL; /*表示为不在所有字符值集合中的字符，计算LALR语法分析表时使用*/
const psrSymb_t	*PSR_ErrorSymb= NULL;/*特殊的错误处理产生式标记终结符，*/

/*以下都为非终结符*/
const psrSymb_t	*PSR_StartSymb = NULL;





void	PSR_Init()
{
		AR_InitSpinLock(&__tbl_lock);
		__tbl		= PSR_CreateStrTable(PSR_STRTBL_BUCKET);

		PSR_EpsilonSymb =		PSR_CreateSymb(L"%EPSILON", PSR_TERM, 0, PSR_ASSOC_NOASSOC,PSR_EPSILON_TOKVAL);
		PSR_EOISymb		=		PSR_CreateSymb(L"%EOI", PSR_TERM, 0, PSR_ASSOC_NOASSOC,PSR_EOI_TOKVAL);
		PSR_LALRSymb	=		PSR_CreateSymb(L"%LALR", PSR_TERM, 0, PSR_ASSOC_NOASSOC,PSR_LALR_TOKVAL);
		PSR_ErrorSymb	=		PSR_CreateSymb(L"error", PSR_TERM, 0, PSR_ASSOC_NOASSOC,PSR_ERROR_TOKVAL);
		PSR_StartSymb	=		PSR_CreateSymb(L"%Start", PSR_NONTERM, 0, PSR_ASSOC_NOASSOC,0);
}

void	PSR_UnInit()
{
		PSR_DestroySymb((psrSymb_t*)PSR_EpsilonSymb);
		PSR_DestroySymb((psrSymb_t*)PSR_EOISymb);
		PSR_DestroySymb((psrSymb_t*)PSR_LALRSymb);
		PSR_DestroySymb((psrSymb_t*)PSR_ErrorSymb);
		PSR_DestroySymb((psrSymb_t*)PSR_StartSymb);

		PSR_DestroyStrTable(__tbl);
		AR_UnInitSpinLock(&__tbl_lock);
		__tbl = NULL;
}



/***********************************************StringTable************************************/

const wchar_t*	PSR_AllocString(const wchar_t *str)
{
		const wchar_t *res;
		AR_LockSpinLock(&__tbl_lock);
		res = PSR_GetString(__tbl, str);
		AR_UnLockSpinLock(&__tbl_lock);
		return res;
}

const wchar_t*	PSR_AllocStringN(const wchar_t *str, size_t n)
{
		const wchar_t *res;
		AR_LockSpinLock(&__tbl_lock);
		res = PSR_GetStringN(__tbl, str,n);
		AR_UnLockSpinLock(&__tbl_lock);
		return res;
}


const wchar_t*  PSR_AllocStringInt(int64_t num, size_t radix)
{
		const wchar_t *res;
		AR_LockSpinLock(&__tbl_lock);
		res = PSR_GetStringInt(__tbl, num, radix);
		AR_UnLockSpinLock(&__tbl_lock);
		return res;
}

const wchar_t*  PSR_AllocStringUInt(uint64_t num, size_t radix)
{
		const wchar_t *res;
		AR_LockSpinLock(&__tbl_lock);
		res = PSR_GetStringUInt(__tbl, num, radix);
		AR_UnLockSpinLock(&__tbl_lock);
		return res;
}

AR_NAMESPACE_END