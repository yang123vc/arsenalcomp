/*
 * The Arsenal Library
 * Copyright (c) 2009-2013 by Solidus
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express
 * or implied warranty.
 *
 */


#ifndef Arsenal_math_posix_h
#define Arsenal_math_posix_h

AR_NAMESPACE_BEGIN

double  AR_nan_value_dbl()
{
        return NAN;
        
}


double  AR_inf_value_dbl()
{
        return INFINITY;
}


float  AR_nan_value_flt()
{
        return NAN;
        
}


float  AR_inf_value_flt()
{
        return INFINITY;
}



ar_bool_t	AR_is_nan_flt(float num)
{
		return isnan(num) != 0;

}


ar_bool_t	AR_is_nan_dbl(double num)
{
		return isnan(num) != 0;
}



ar_bool_t	AR_is_inf_flt(float num)
{
		return isinf(num) != 0;
}


ar_bool_t	AR_is_inf_dbl(double num)
{
		return isinf(num) != 0;
}



AR_NAMESPACE_END


#endif
