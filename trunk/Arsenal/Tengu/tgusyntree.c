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


#include "tgusyntree.h"
#include "tguutility.h"

AR_NAMESPACE_BEGIN


/*********************************************Block******************************************************/

tguBlock_t*		TGU_CreateBlock(tguBlock_t	*parent)
{
		return NULL;
}


void			TGU_DestroyBlock(tguBlock_t	*block)
{
		AR_ASSERT(block != NULL);
}

tguVar_t*		TGU_FindFromBlockByName(tguBlock_t	*block, const wchar_t *name, bool_t current_block)
{
		AR_ASSERT(block != NULL && name != NULL && AR_wcslen(name) > 0);
		
		return NULL;
}


/*********************************************************************/

tguSyntaxTree_t*		TGU_CreateSyntaxTree(const wchar_t	*sources_name)
{
		return NULL;
}

void					TGU_DestroySyntaxTree(tguSyntaxTree_t	*tree)
{
		AR_ASSERT(tree != NULL);
}
AR_NAMESPACE_END






