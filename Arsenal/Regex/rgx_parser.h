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


#include "rgx_in.h"
#include "rgx_node.h"

AR_NAMESPACE_BEGIN


/*NameSetEnd*/


typedef struct __rgx_result_tag	rgxResult_t;
typedef struct __rgx_error_tag	rgxError_t;

struct __rgx_error_tag
{
		const wchar_t	*pos;
};


struct __rgx_result_tag
{
		rgxNode_t*		node;
		const wchar_t	*next;
		rgxError_t		err;
};


rgxResult_t	RGX_ParseExpr(const wchar_t *expr, const rgxNameSet_t *name_set);




AR_NAMESPACE_END

