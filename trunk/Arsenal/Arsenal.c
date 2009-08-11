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


void	Arsenal_Init(arInit_t *start_info)
{
		AR_Init(start_info);
		LEX_Init();
		PSR_Init();
		
}

void	Arsenal_UnInit()
{
		PSR_UnInit();
		LEX_UnInit();
		AR_UnInit();
}


/*************************************************************************************************/




AR_NAMESPACE_END
