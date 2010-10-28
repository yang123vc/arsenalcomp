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


#ifndef __ARSENAL_TGUUTILITY_H__
#define __ARSENAL_TGUUTILITY_H__

#include "tengu.h"




AR_NAMESPACE_BEGIN

void	TGU_InitUtility();
void	TGU_UnInitUtility();



/*所有parser配置相关的符号都由以下函数分配，此功能为符号对比提供了O(1)的性能(只需对比指针)*/

#define TGU_STRTBL_BUCKET		(1536)


const wchar_t*	TGU_AllocString(const wchar_t *str);
const wchar_t*	TGU_AllocStringN(const wchar_t *str, size_t n);

const wchar_t*  TGU_AllocStringInt(int_64_t num, size_t radix);
const wchar_t*  TGU_AllocStringUInt(uint_64_t num, size_t radix);



AR_NAMESPACE_END



#endif
