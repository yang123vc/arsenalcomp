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


#ifndef __LEX_EXPR_H__
#define __LEX_EXPR_H__

#include "node.h"


AR_NAMESPACE_BEGIN


typedef struct __lex_error_tag
{
		const wchar_t	*pos;

}lexError_t;

typedef struct __lex_name_tag
{
		wchar_t			name[AR_MAX_LEXNAME];
		lexNode_t		*node;
		struct __lex_name_tag *next;
}lexName_t;


bool			LEX_InsertToNameList(lexName_t **dest, const wchar_t *name, lexNode_t *node);
void			LEX_DestroyNameList(lexName_t *lst);
lexNode_t*		LEX_FindFromNameList(lexName_t *lst, const wchar_t *name);



typedef struct __lex_result_tag
{
		lexNode_t*		node;
		const wchar_t	*next;
		lexError_t		err;
}lexResult_t;


lexResult_t		LEX_CompileExpr(const wchar_t *expr, lexName_t *name_set);







AR_NAMESPACE_END















#endif


