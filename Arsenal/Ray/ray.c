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

#include "ray.h"



AR_NAMESPACE_BEGIN


void	RAY_Init()
{
		RAY_InitParser();
}

void	RAY_UnInit()
{
		RAY_UnInitParser();

}

AR_NAMESPACE_END

