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


#ifndef __RAY_PARSER_IMPL_H__
#define __RAY_PARSER_IMPL_H__


#include "ray.h"




AR_NAMESPACE_BEGIN

void				RAY_InitParserImpl();
void				RAY_UnInitParserImpl();

lexMatch_t*			RAY_BuildLexer();
void				RAY_ReleaseLexer(lexMatch_t	*match);

psrContext_t*		RAY_BuildParserContext(void *ctx);
void				RAY_ReleaseParserContext(psrContext_t		*parser_context);



AR_NAMESPACE_END




#endif

