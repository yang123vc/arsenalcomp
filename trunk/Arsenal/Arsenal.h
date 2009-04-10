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

#ifndef AR_HEADER_H
#define AR_HEADER_H

#include ".\Common\common.h"
#include ".\Lex\lex.h"
#include ".\Parser\grammar.h"
#include ".\Parser\parser.h"



AR_NAMESPACE_BEGIN


void	Arsenal_Init(arInit_t *start_info);
void	Arsenal_UnInit();


/******************************************************************************************************/




void AR_Test();


AR_NAMESPACE_END


#endif

