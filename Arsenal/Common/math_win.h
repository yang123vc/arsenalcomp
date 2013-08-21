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

#ifndef Arsenal_math_win_h
#define Arsenal_math_win_h

AR_NAMESPACE_BEGIN

double  AR_nan_value_dbl()
{
        
#if (AR_COMPILER == AR_VC_LEGACY || AR_COMPILER == AR_VC || AR_COMPILER == AR_BCB6)
        static const __int64 nanval = (ar_uint_64_t)(0xfff) << 51;
        return *(const double*)(&nanval);
#else  
        return NAN;
#endif
        
        
}


double  AR_infinity_value_dbl()
{
        return HUGE_VAL;
}


ar_bool_t	AR_is_nan_flt(float num)
{
#if defined(isnan)
		return isnan(num) != 0;
#else
       return _isnan(num) != 0;
#endif
        
}


ar_bool_t	AR_is_nan_dbl(double num)
{
#if defined(isnan)
		return isnan(num) != 0;
#else
        return return _isnan(num) != 0;
#endif
        
}



ar_bool_t	AR_is_inf_flt(float num)
{
#if defined(isinf)
		return isinf(num) != 0;
#else
        return _finite(num) == 0;
#endif

}


ar_bool_t	AR_is_inf_dbl(double num)
{
#if defined(isinf)
		return isinf(num) != 0;
#else
        return _finite(num) == 0;
#endif
}




'
AR_NAMESPACE_END

#endif
