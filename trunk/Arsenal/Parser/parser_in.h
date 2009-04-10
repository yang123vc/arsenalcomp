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
#ifndef __PARSER_IN__H__
#define __PARSER_IN__H__

#include "parser.h"

AR_NAMESPACE_BEGIN


typedef struct __parser_symbol_tag psrSymb_t;

/*以下为终结符*/
extern const psrSymb_t	*PSR_EpsilonSymb;/*表示空*/
extern const psrSymb_t	*PSR_EOISymb;   /*表示输入结尾符号*/
extern const psrSymb_t	*PSR_LALRSymb; /*表示为不在所有字符值集合中的字符，计算LALR语法分析表时使用*/
extern const psrSymb_t	*PSR_ErrorSymb;/*特殊的错误处理产生式标记终结符，*/

/*以下都为非终结符*/
extern const psrSymb_t	*PSR_StartSymb;/*第一个符号，被转换为 %Start -> gmr->head[0]*/

const wchar_t*	PSR_AllocString(const wchar_t *str);
const wchar_t*	PSR_AllocStringN(const wchar_t *str, size_t n);

const wchar_t*  PSR_AllocStringInt( int64_t num, size_t radix);
const wchar_t*  PSR_AllocStringUInt(uint64_t num, size_t radix);


#define PSR_STRTBL_BUCKET		2048*100


void	PSR_Init();
void	PSR_UnInit();

AR_NAMESPACE_END

#endif

