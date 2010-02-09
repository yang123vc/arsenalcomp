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


#ifndef __RAY_PARSER_H__
#define __RAY_PARSER_H__


#include "ray.h"
#include "raysymbol.h"



AR_NAMESPACE_BEGIN


lex_t*			RAY_BuildLexer(const arIOCtx_t *io);
psrGrammar_t*	RAY_BuildGrammar(const arIOCtx_t *io);
parser_t*		RAY_BuildParser(psrGrammar_t *grammar);


AR_NAMESPACE_END





#endif


