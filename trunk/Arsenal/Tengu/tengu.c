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

#include "tengu.h"
#include "tguparser.h"


AR_NAMESPACE_BEGIN


void	TGU_Init()
{
		TGU_InitParser();
}

void	TGU_UnInit()
{
		TGU_UnInitParser();

}



AR_NAMESPACE_END


