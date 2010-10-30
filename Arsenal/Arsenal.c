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



#include "Arsenal.h"

AR_NAMESPACE_BEGIN



void	Arsenal_Init(const arInit_t *ctx)
{
		/*AR_ASSERT(ctx != NULL);*/
		AR_Init(ctx);
		Lex_Init();
		Parser_Init();
		Tools_Init();

		TGU_Init();
}




void	Arsenal_UnInit()
{
		TGU_UnInit();
		Tools_UnInit();
		Parser_UnInit();
		Lex_UnInit();
		AR_UnInit();
}


/*************************************************************************************************/



AR_NAMESPACE_END
