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


#include "./Common/common.h"
#include "./Lex/lex.h"

#include "./Parser/grammar.h"
#include "./Parser/parser.h"
#include "./Tools/tools.h"


AR_NAMESPACE_BEGIN

typedef struct __arsenal_context_tag
{
		arInit_t		init;
}arCtx_t;

void	Arsenal_Init(const arCtx_t *ctx);
void	Arsenal_UnInit();




AR_NAMESPACE_END


#endif

