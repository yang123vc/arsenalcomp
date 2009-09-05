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

#include "common.h"

AR_NAMESPACE_BEGIN

void			AR_srand(uint64_t seed)
{
		srand((unsigned int)seed);
}


uint64_t AR_rand64()
{
		uint64_t res = rand();
		res ^= (uint64_t)rand() << 15;
		res ^= (uint64_t)rand() << 30;
		res ^= (uint64_t)rand() << 45;
		res ^= (uint64_t)rand() << 60;
		return res;

}

uint32_t AR_rand32()
{
		uint32_t res = rand();
		res ^= (uint32_t)rand() << 15;
		res ^= (uint32_t)rand() << 30;
		return res;
}
















AR_NAMESPACE_END


