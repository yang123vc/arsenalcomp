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


#ifndef __ARSENAL_RAY_H__
#define __ARSENAL_RAY_H__

#include "../Common/common.h"
#include "../Lex/lex.h"
#include "../Parser/grammar.h"
#include "../Parser/parser.h"
AR_NAMESPACE_BEGIN



void	RAY_Init();
void	RAY_UnInit();


struct __ray_parser_tag;
typedef struct __ray_parser_tag		rayParser_t;


struct __ray_compiler_tag;
typedef struct __ray_compiler_tag		rayCompiler_t;



AR_NAMESPACE_END

#endif

