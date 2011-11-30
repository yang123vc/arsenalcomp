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

/******************************************************Misc**************************************************************/

AR_NAMESPACE_BEGIN


double AR_logbase(double a, double base)
{
		return log(a) / log(base);
}





int_32_t		AR_abs_32(int_32_t x)
{
		int_32_t y = x >> 31;
		return ( ( x ^ y ) - y );
}


int_64_t		AR_abs_64(int_64_t x)
{
		int_64_t y = x >> 63;
		return ( ( x ^ y ) - y );
}


double			AR_abs_dbl(double x)
{
		return (double)fabs(x);
}


double AR_floor(double f)
{
		return (double)floor(f);
}

double AR_ceil(double f)
{
		return (double)ceil(f);
}

double			AR_sqrt(double f)
{
		return (double)sqrt(f);
}


double AR_pow(double x, double y)
{
		return (double)pow(x,y);
}

double AR_exp(double f)
{
		return (double)exp(f);
}


double AR_sin(double f)
{
		return (double)sin(f);
}


double AR_cos(double f)
{
		return (double)cos(f);
}


double AR_tan(double f)
{
		return (double)tan(f);
}

double AR_asin(double f)
{
		return (double)asin(f);
}

double AR_acos(double f)
{
		return (double)acos(f);
}

double AR_atan(double f)
{
		return (double)atan(f);
}

double AR_atan2(double y, double x)
{
		return (double)atan2(y,x);
}



AR_NAMESPACE_END






