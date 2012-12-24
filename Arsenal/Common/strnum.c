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


ar_int_t AR_wchartodigit(wchar_t ch)
{

		#define DIGIT_RANGE_TEST(zero)  \
		if (ch < zero)					\
				return -1;              \
		if (ch < zero + 10)				\
		{								\
				return ch - zero;       \
		}


		DIGIT_RANGE_TEST(0x0030)			/* 0030;DIGIT ZERO*/
		if (ch < 0xFF10)					/* FF10;FULLWIDTH DIGIT ZERO*/
		{
				DIGIT_RANGE_TEST(0x0660)    /* 0660;ARABIC-INDIC DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x06F0)    /* 06F0;EXTENDED ARABIC-INDIC DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x0966)    /* 0966;DEVANAGARI DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x09E6)    /* 09E6;BENGALI DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x0A66)    /* 0A66;GURMUKHI DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x0AE6)    /* 0AE6;GUJARATI DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x0B66)    /* 0B66;ORIYA DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x0C66)    /* 0C66;TELUGU DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x0CE6)    /* 0CE6;KANNADA DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x0D66)    /* 0D66;MALAYALAM DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x0E50)    /* 0E50;THAI DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x0ED0)    /* 0ED0;LAO DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x0F20)    /* 0F20;TIBETAN DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x1040)    /* 1040;MYANMAR DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x17E0)    /* 17E0;KHMER DIGIT ZERO*/
				DIGIT_RANGE_TEST(0x1810)    /* 1810;MONGOLIAN DIGIT ZERO*/

				return -1;
		}

		if (ch < 0xFF10 + 10)/* FF10;FULLWIDTH DIGIT ZERO*/
		{
				return ch - 0xFF10;
		}
		return -1;

		#undef DIGIT_RANGE_TEST
}


ar_int_t			AR_chartodigit(char ch)
{
		if(ch >= '0' && ch <= '9')
		{
				return ch - '0';
		}else
		{
				return -1;
		}
}



const wchar_t* AR_wtoi64(const wchar_t *in,	 ar_int_64_t  *num, size_t base)
{
		return AR_wtoi64_s(in, in + AR_wcslen(in), num, base);
}

const wchar_t* AR_wtou64(const wchar_t *in,	 ar_uint_64_t  *num, size_t base)
{
		return AR_wtou64_s(in, in + AR_wcslen(in), num, base);
}


const wchar_t* AR_wtoi32(const wchar_t *in,  ar_int_32_t *num, size_t base)
{
		return AR_wtoi32_s(in, in + AR_wcslen(in), num, base);
}

const wchar_t* AR_wtou32(const wchar_t *in,  ar_uint_32_t *num, size_t base)
{
		return AR_wtou32_s(in, in + AR_wcslen(in), num, base);
}

/************************************************************************************/


const char* AR_stoi64(const char *in,	 ar_int_64_t  *num, size_t base)
{
		return AR_stoi64_s(in, in + AR_strlen(in), num, base);
}

const char* AR_stou64(const char *in,	 ar_uint_64_t  *num, size_t base)
{
		return AR_stou64_s(in, in + AR_strlen(in), num, base);
}


const char* AR_stoi32(const char *in,  ar_int_32_t *num, size_t base)
{
		return AR_stoi32_s(in, in + AR_strlen(in), num, base);
}


const char* AR_stou32(const char *in,  ar_uint_32_t *num, size_t base)
{
		return AR_stou32_s(in, in + AR_strlen(in), num, base);
}


/**************************************************************************************/



#define	__BUFFER_LEN	128


ar_int_t			AR_u64tow_buf(wchar_t *out, size_t nbuf, ar_uint_64_t num, size_t radix)
{
		wchar_t buf[__BUFFER_LEN];
		wchar_t *p;
		ar_int_t len;
		static const wchar_t* __tbl = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		AR_ASSERT(radix >= 2 && radix <= 36);

		p = buf + __BUFFER_LEN;
		*--p = 0;
		
        do{
                *--p = __tbl[num % radix];
                
                num /= radix;
                
        }while(num > 0);

		len = (ar_int_t)(buf + __BUFFER_LEN - p);/*返回的是需要元素数组的长度包含0*/

		if(out != NULL)
		{
				if(nbuf < (size_t)len)return -1;
				AR_wcscpy(out, p);
				return len;
		}else
		{
				return len;
		}

}




ar_int_t			AR_i64tow_buf(wchar_t *out, size_t nbuf, ar_int_64_t num, size_t radix)
{
		ar_bool_t is_neg;
		wchar_t buf[__BUFFER_LEN];
		wchar_t *p;
		ar_int_t len;
		/*static const wchar_t* __tbl = L"0123456789ABCDEF";*/
		static const wchar_t* __tbl = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		AR_ASSERT(radix >= 2 && radix <= 36);
		is_neg = false;

		if(num < 0){ is_neg = true; num = -num;}

		p = buf + __BUFFER_LEN;
		*--p = 0;
		do{
				*--p = __tbl[num % radix];
				num /= radix;
		}while(num  > 0);
		if(is_neg)*--p = L'-';

		len = (ar_int_t)(buf + __BUFFER_LEN - p);/*返回的是需要元素数组的长度包含0*/

		if(out != NULL)
		{
				if(nbuf < (size_t)len)return -1;
				AR_wcscpy(out, p);
				return len;
		}else
		{
				return len;
		}
}




/*******************************************************************************************************/

ar_int_t			AR_u64tos_buf(char *out, size_t nbuf, ar_uint_64_t num, size_t radix)
{
		char buf[__BUFFER_LEN];
		char *p;
		ar_int_t len;
		static const char* __tbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		AR_ASSERT(radix >= 2 && radix <= 36);

		p = buf + __BUFFER_LEN;
		*--p = 0;
		do{ *--p = __tbl[num % radix];}while((num /= radix) > 0);

		len = (ar_int_t)(buf + __BUFFER_LEN - p);/*返回的是需要元素数组的长度包含0*/

		if(out != NULL)
		{
				if(nbuf < (size_t)len)return -1;
				AR_strcpy(out, p);
				return len;
		}else
		{
				return len;
		}

}




ar_int_t			AR_i64tos_buf(char *out, size_t nbuf, ar_int_64_t num, size_t radix)
{
		ar_bool_t is_neg;
		char buf[__BUFFER_LEN];
		char *p;
		ar_int_t len;
		/*static const wchar_t* __tbl = L"0123456789ABCDEF";*/
		static const char* __tbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		AR_ASSERT(radix >= 2 && radix <= 36);
		is_neg = false;

		if(num < 0){ is_neg = true; num = -num;}

		p = buf + __BUFFER_LEN;
		*--p = 0;
		do{ *--p = __tbl[num % radix];}while((num /= radix) > 0);
		if(is_neg)*--p = L'-';

		len = (ar_int_t)(buf + __BUFFER_LEN - p);/*返回的是需要元素数组的长度包含0*/

		if(out != NULL)
		{
				if(nbuf < (size_t)len)
				{
						return -1;
				}
				AR_strcpy(out, p);
				return len;
		}else
		{
				return len;
		}
}



#undef __BUFFER_LEN




const wchar_t*	AR_wtod(const wchar_t *in, double *num)
{
/*
		wchar_t *stop;
		AR_ASSERT(in != NULL && num != NULL);

		*num = AR_wcstod(in, &stop);

		if(stop == in)
		{
				return NULL;
		}else
		{
				return stop;
		}
*/

		AR_ASSERT(in != NULL && num != NULL);
		return AR_wtod_s(in, in + AR_wcslen(in), num);
}



const char*	AR_stod(const char *in, double *num)
{

		AR_ASSERT(in != NULL && num != NULL);
		return AR_stod_s(in, in + AR_strlen(in), num);
}





/*此函数貌似不太完整，对溢出检测也没做到位*/
const wchar_t*	AR_wtod_s(const wchar_t *in, const wchar_t *end, double *out)
{
		
		const wchar_t *p;
		double num = 0.0f,frac = 0.0f, exp = 1.0f;
		double result = 0.0f;
		ar_bool_t is_neg = false, is_ok = false;
		
		wchar_t decimal_point;
		AR_ASSERT(in != NULL && end != NULL && in <= end && out != NULL);
		
		{
				const struct lconv *conv;
				conv =  localeconv();
				decimal_point = (wchar_t)*conv->decimal_point;
		}

		/*p = AR_wcstrim(in, L" \t");*/
		p = AR_wcstrim_space_s(in, end);

		if(*p == L'-')
		{
				++p;
				is_neg = true;
		}else if(*p == L'+')
		{
				is_neg = false;
				++p;
		}


		while(p < end && *p >= L'0' && *p <= L'9')
		{
				num *= 10.0f;
				num += (double)(*p - L'0');
				++p;
				is_ok = true;
		}
		
		if(p < end && *p == decimal_point)
		{
				++p;
				while(p < end && *p >= L'0' && *p <= L'9')
				{
						exp *= 10.0f;
						frac *= 10.0f;
						frac += (double)(*p - L'0');
						++p;
						is_ok = true;
				}
		}
		result = num + frac / exp;


		if(p < end && (*p == L'e' || *p == L'E'))
		{
				ar_bool_t factor;
				size_t i, e;
				++p;

				if(*p == L'+')
				{
						factor = true;
						++p;
				}else if(*p == L'-')
				{
						factor = false;
						++p;
				}else
				{
						factor = true;
				}
				
				e = 0;
				is_ok = false;
				while(p < end && *p >= L'0' && *p <= L'9')
				{
						e *= 10;
						e += (*p - L'0');
						++p;
						is_ok = true;
				}

				for(i = 0; i < e; ++i)
				{
						if(factor)
						{
								result *= 10.0f;
						}else
						{
								result /= 10.0f;
						}
				}
		}
		if(is_neg)result = -result;
		*out = result;
		return is_ok ? p : NULL;
}




#define __UNSIGNED   0x01
#define __NEG        0x02
#define __OVERFLOW   0x04
#define __READDIGIT  0x08


static const wchar_t* __wtou64_s(const wchar_t *in, const wchar_t *end, ar_uint_64_t  *num, size_t base, ar_uint_32_t flag)
{
		ar_uint_64_t val,digit, maxval;
		const wchar_t *p;

		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		/*p = AR_wcstrim_s(in, end, L" \t");*/
		p = AR_wcstrim_space_s(in,end);

		if(p >= end)return NULL;

		if(*p == L'-')
		{
				flag |= __NEG;
				++p;
		}else if(*p == L'+')
		{
				++p;
		}

		if(flag & __NEG && flag & __UNSIGNED)return NULL;
		if(p >= end)return NULL;
		if(base == 0)
		{
				if(AR_wchartodigit(*p) != 0)
				{
						base = 10;
				}else if(*(p + 1) == L'x' || *(p + 1) == L'X')
				{
						base = 16;
				}else
				{
						base = 8;
				}
		}

		if(base == 16)
		{
				if(AR_wchartodigit(*p) == 0 && (*(p + 1) == L'x' || *(p + 1) == L'X'))
				{
						p += 2;
				}
		}

		if(p >= end)return NULL;

		maxval = AR_UINT64_MAX / base;
		digit = 0;
		val = 0;

		while(p < end)
		{
				ar_int_t tmp;
				if((tmp = AR_wchartodigit(*p)) != -1)
				{
						/*tmp = (*p - L'0');*/
				}else if(*p >= L'A' && *p <= L'Z')
				{
						tmp = ((*p - L'A') + 10);
				}else if(*p >= L'a' && *p <= L'z')
				{
						tmp = ((*p - L'a') + 10);
				}else
				{
						break;
				}

				digit = (ar_uint_64_t)tmp;
				if(digit >= base)break;
				flag |= __READDIGIT;



				if (val < maxval || (val == maxval && digit <= AR_UINT64_MAX % base))
				{
						val = val * base + digit;
				}else
				{
						flag |= __OVERFLOW;
				}
				++p;
		}

		if(!(flag & __READDIGIT))
		{
				val = 0;
				p = NULL;
		}else if((flag & __OVERFLOW) || ((!(flag & __UNSIGNED)) && (((flag & __NEG) && (val > -AR_INT64_MIN)) || (!(flag & __NEG) && (val > AR_INT64_MAX)))))
		{
				p = NULL;
				/*errno = ERANGE;*/
				
				if(flag & __UNSIGNED)
				{
						val = AR_UINT64_MAX;
				} else if (flag & __NEG )
				{
						val = (ar_uint_64_t)(-AR_INT64_MIN);
				}else
				{
						val = AR_INT64_MAX;
				}
		}

		if(flag & __NEG)
		{
				val = (ar_uint_64_t)(-(ar_int_64_t)val);
		}
		*num = val;
		return p;
}



static const wchar_t* __wtou32_s(const wchar_t *in, const wchar_t *end, ar_uint_32_t  *num, size_t base, ar_uint_32_t flag)
{
		ar_uint_32_t val,digit, maxval;
		const wchar_t *p;

		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		/*p = AR_wcstrim_s(in, end, L" \t");*/
		p = AR_wcstrim_space_s(in,end);

		if(p >= end)return NULL;

		if(*p == L'-')
		{
				flag |= __NEG;
				++p;
		}else if(*p == L'+')
		{
				++p;
		}

		if(flag & __NEG && flag & __UNSIGNED)return NULL;
		if(p >= end)return NULL;
		if(base == 0)
		{
				if(AR_wchartodigit(*p) != 0)
				{
						base = 10;
				}else if(*(p + 1) == L'x' || *(p + 1) == L'X')
				{
						base = 16;
				}else
				{
						base = 8;
				}
		}

		if(base == 16)
		{
				if(AR_wchartodigit(*p) == 0 && (*(p + 1) == L'x' || *(p + 1) == L'X'))
				{
						p += 2;
				}
		}

		if(p >= end)return NULL;

		maxval = AR_UINT32_MAX / (ar_uint_32_t)base;
		digit = 0;
		val = 0;

		while(p < end)
		{
				ar_int_t tmp;
				if((tmp = AR_wchartodigit(*p)) != -1)
				{
						/*tmp = (*p - L'0');*/
				}else if(*p >= L'A' && *p <= L'Z')
				{
						tmp = ((*p - L'A') + 10);
				}else if(*p >= L'a' && *p <= L'z')
				{
						tmp = ((*p - L'a') + 10);
				}else
				{
						break;
				}

				digit = (ar_uint_32_t)tmp;
				if(digit >= base)break;
				flag |= __READDIGIT;



				if (val < maxval || (val == maxval && digit <= AR_UINT32_MAX % base))
				{
						val = val * (ar_uint_32_t)base + digit;
				}else
				{
						flag |= __OVERFLOW;
				}
				++p;
		}

		if(!(flag & __READDIGIT))
		{
				val = 0;
				p = NULL;
		}else if((flag & __OVERFLOW) || ((!(flag & __UNSIGNED)) && (((flag & __NEG) && (val > -AR_INT32_MIN)) || (!(flag & __NEG) && (val > AR_INT32_MAX)))))
		{
				p = NULL;
				/*errno = ERANGE;*/
				if ( flag & __UNSIGNED )
				{
						val = AR_UINT32_MAX;
				} else if (flag & __NEG )
				{
						val = (ar_uint_32_t)(-AR_INT32_MIN);
				}else
				{
						val = AR_INT32_MAX;
				}
		}

		if(flag & __NEG)
		{
				val = (ar_uint_32_t)(-(ar_int_32_t)val);
		}
		*num = val;
		return p;
}






const wchar_t* AR_wtoi64_s(const wchar_t *in, const wchar_t *end, ar_int_64_t  *num, size_t base)
{
		ar_uint_32_t flag = 0;
		ar_uint_64_t val;
		const wchar_t *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __wtou64_s(in, end, &val, base, flag);

		*num = (ar_int_64_t)val;

		return p;
}


const wchar_t* AR_wtou64_s(const wchar_t *in, const wchar_t *end, ar_uint_64_t  *num, size_t base)
{
		ar_uint_32_t flag = __UNSIGNED;
		const wchar_t *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __wtou64_s(in, end, num, base, flag);

		return p;

}





const wchar_t* AR_wtoi32_s(const wchar_t *in, const wchar_t *end, ar_int_32_t  *num, size_t base)
{
		ar_uint_32_t flag = 0;
		ar_uint_32_t val;
		const wchar_t *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __wtou32_s(in, end, &val, base, flag);

		*num = (ar_int_32_t)val;

		return p;
}


const wchar_t* AR_wtou32_s(const wchar_t *in, const wchar_t *end, ar_uint_32_t  *num, size_t base)
{
		ar_uint_32_t flag = __UNSIGNED;
		const wchar_t *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __wtou32_s(in, end, num, base, flag);

		return p;

}

/***********************************************************str********************************************************************/


static const char* __stou64_s(const char *in, const char *end, ar_uint_64_t  *num, size_t base, ar_uint_32_t flag)
{
		ar_uint_64_t val,digit, maxval;
		const char *p;

		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		/*p = AR_wcstrim_s(in, end, L" \t");*/
		p = AR_strtrim_space_s(in,end);

		if(p >= end)
		{
				return NULL;
		}

		if(*p == '-')
		{
				flag |= __NEG;
				++p;
		}else if(*p == '+')
		{
				++p;
		}

		if(flag & __NEG && flag & __UNSIGNED)
		{
				return NULL;
		}

		if(p >= end)
		{
				return NULL;
		}

		if(base == 0)
		{
				if(AR_chartodigit(*p) != 0)
				{
						base = 10;
				}else if(*(p + 1) == 'x' || *(p + 1) == 'X')
				{
						base = 16;
				}else
				{
						base = 8;
				}
		}

		if(base == 16)
		{
				if(AR_chartodigit(*p) == 0 && (*(p + 1) == 'x' || *(p + 1) == 'X'))
				{
						p += 2;
				}
		}

		if(p >= end)
		{
				return NULL;
		}

		maxval = AR_UINT64_MAX / base;
		digit = 0;
		val = 0;

		while(p < end)
		{
				ar_int_t tmp;
				if((tmp = AR_chartodigit(*p)) != -1)
				{
						/*tmp = (*p - L'0');*/
				}else if(*p >= 'A' && *p <= 'Z')
				{
						tmp = ((*p - 'A') + 10);
				}else if(*p >= 'a' && *p <= 'z')
				{
						tmp = ((*p - 'a') + 10);
				}else
				{
						break;
				}

				digit = (ar_uint_64_t)tmp;
				if(digit >= base)
				{
						break;
				}

				flag |= __READDIGIT;



				if (val < maxval || (val == maxval && digit <= AR_UINT64_MAX % base))
				{
						val = val * base + digit;
				}else
				{
						flag |= __OVERFLOW;
				}
				++p;
		}

		if(!(flag & __READDIGIT))
		{
				val = 0;
				p = NULL;
		}else if((flag & __OVERFLOW) || ((!(flag & __UNSIGNED)) && (((flag & __NEG) && (val > -AR_INT64_MIN)) || (!(flag & __NEG) && (val > AR_INT64_MAX)))))
		{
				p = NULL;
				/*errno = ERANGE;*/
				
				if(flag & __UNSIGNED)
				{
						val = AR_UINT64_MAX;
				} else if (flag & __NEG )
				{
						val = (ar_uint_64_t)(-AR_INT64_MIN);
				}else
				{
						val = AR_INT64_MAX;
				}
		}

		if(flag & __NEG)
		{
				val = (ar_uint_64_t)(-(ar_int_64_t)val);
		}
		*num = val;
		return p;
}



static const char* __stou32_s(const char *in, const char *end, ar_uint_32_t  *num, size_t base, ar_uint_32_t flag)
{
		ar_uint_32_t val,digit, maxval;
		const char *p;

		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		/*p = AR_wcstrim_s(in, end, L" \t");*/
		p = AR_strtrim_space_s(in,end);

		if(p >= end)
		{
				return NULL;
		}

		if(*p == '-')
		{
				flag |= __NEG;
				++p;
		}else if(*p == '+')
		{
				++p;
		}

		if(flag & __NEG && flag & __UNSIGNED)
		{
				return NULL;
		}

		if(p >= end)
		{
				return NULL;
		}

		if(base == 0)
		{
				if(AR_chartodigit(*p) != 0)
				{
						base = 10;
				}else if(*(p + 1) == 'x' || *(p + 1) == 'X')
				{
						base = 16;
				}else
				{
						base = 8;
				}
		}

		if(base == 16)
		{
				if(AR_chartodigit(*p) == 0 && (*(p + 1) == 'x' || *(p + 1) == 'X'))
				{
						p += 2;
				}
		}

		if(p >= end)
		{
				return NULL;
		}

		maxval = AR_UINT32_MAX / (ar_uint_32_t)base;
		digit = 0;
		val = 0;

		while(p < end)
		{
				ar_int_t tmp;
				if((tmp = AR_chartodigit(*p)) != -1)
				{
						/*tmp = (*p - L'0');*/
				}else if(*p >= 'A' && *p <= 'Z')
				{
						tmp = ((*p - 'A') + 10);
				}else if(*p >= 'a' && *p <= 'z')
				{
						tmp = ((*p - 'a') + 10);
				}else
				{
						break;
				}

				digit = (ar_uint_32_t)tmp;
				if(digit >= base)
				{
						break;
				}

				flag |= __READDIGIT;



				if (val < maxval || (val == maxval && digit <= AR_UINT32_MAX % base))
				{
						val = val * (ar_uint_32_t)base + digit;
				}else
				{
						flag |= __OVERFLOW;
				}
				++p;
		}

		if(!(flag & __READDIGIT))
		{
				val = 0;
				p = NULL;
		}else if((flag & __OVERFLOW) || ((!(flag & __UNSIGNED)) && (((flag & __NEG) && (val > -AR_INT32_MIN)) || (!(flag & __NEG) && (val > AR_INT32_MAX)))))
		{
				p = NULL;
				/*errno = ERANGE;*/
				if ( flag & __UNSIGNED )
				{
						val = AR_UINT32_MAX;
				} else if (flag & __NEG )
				{
						val = (ar_uint_32_t)(-AR_INT32_MIN);
				}else
				{
						val = AR_INT32_MAX;
				}
		}

		if(flag & __NEG)
		{
				val = (ar_uint_32_t)(-(ar_int_32_t)val);
		}
		*num = val;
		return p;
}




const char* AR_stoi64_s(const char *in, const char *end, ar_int_64_t  *num, size_t base)
{
		ar_uint_32_t flag = 0;
		ar_uint_64_t val;
		const char *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __stou64_s(in, end, &val, base, flag);

		*num = (ar_int_64_t)val;

		return p;
}


const char* AR_stou64_s(const char *in, const char *end, ar_uint_64_t  *num, size_t base)
{
		ar_uint_32_t flag = __UNSIGNED;
		const char *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __stou64_s(in, end, num, base, flag);

		return p;

}


const char* AR_stoi32_s(const char *in, const char *end, ar_int_32_t  *num, size_t base)
{
		ar_uint_32_t flag = 0;
		ar_uint_32_t val;
		const char *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __stou32_s(in, end, &val, base, flag);

		*num = (ar_int_32_t)val;

		return p;
}


const char* AR_stou32_s(const char *in, const char *end, ar_uint_32_t  *num, size_t base)
{
		ar_uint_32_t flag = __UNSIGNED;
		const char *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __stou32_s(in, end, num, base, flag);

		return p;

}



/*此函数貌似不太完整，对溢出检测也没做到位*/
const char*	AR_stod_s(const char *in, const char *end, double *out)
{
		
		const char *p;
		double num = 0.0f,frac = 0.0f, exp = 1.0f;
		double result = 0.0f;
		ar_bool_t is_neg = false, is_ok = false;
		
		char decimal_point;
		AR_ASSERT(in != NULL && end != NULL && in <= end && out != NULL);
		
		{
				const struct lconv *conv;
				conv =  localeconv();
				decimal_point = *conv->decimal_point;
		}

		/*p = AR_wcstrim(in, L" \t");*/
		p = AR_strtrim_space_s(in, end);

		if(*p == L'-')
		{
				++p;
				is_neg = true;
		}else if(*p == L'+')
		{
				is_neg = false;
				++p;
		}


		while(p < end && *p >= '0' && *p <= '9')
		{
				num *= 10.0f;
				num += (double)(*p - '0');
				++p;
				is_ok = true;
		}
		
		if(p < end && *p == decimal_point)
		{
				++p;
				while(p < end && *p >= '0' && *p <= '9')
				{
						exp *= 10.0f;
						frac *= 10.0f;
						frac += (double)(*p - '0');
						++p;
						is_ok = true;
				}
		}
		result = num + frac / exp;


		if(p < end && (*p == 'e' || *p == 'E'))
		{
				ar_bool_t factor;
				size_t i, e;
				++p;

				if(*p == '+')
				{
						factor = true;
						++p;
				}else if(*p == '-')
				{
						factor = false;
						++p;
				}else
				{
						factor = true;
				}
				
				e = 0;
				is_ok = false;
				while(p < end && *p >= '0' && *p <= '9')
				{
						e *= 10;
						e += (*p - '0');
						++p;
						is_ok = true;
				}

				for(i = 0; i < e; ++i)
				{
						if(factor)
						{
								result *= 10.0f;
						}else
						{
								result /= 10.0f;
						}
				}
		}
		if(is_neg)result = -result;
		*out = result;
		return is_ok ? p : NULL;
}



ar_bool_t	AR_wcs_is_float(const wchar_t *in, const wchar_t *end)
{
		const wchar_t *p;
		
		ar_bool_t is_float;

		wchar_t decimal_point;
		
		AR_ASSERT(in != NULL && end != NULL && in <= end);

		{
				const struct lconv *conv;
				conv =  localeconv();
				decimal_point = (wchar_t)*conv->decimal_point;
		}

		is_float = false;

		p = AR_wcstrim_space_s(in, end);

		if(*p == L'-' || *p == L'+')
		{
				++p;
		}


		while(p < end && *p >= L'0' && *p <= L'9')/*可能是个整数*/
		{
				++p;
		}

		if(p < end && *p == decimal_point)/*也许是浮点，但可能有错误*/
		{
				is_float = true;
				++p;

				while(p < end && *p >= L'0' && *p <= L'9')
				{
						++p;
				}
		}

		if(p < end && (*p == L'e' || *p == L'E'))/*可能是浮点，但是会有错误*/
		{
				++p;

				if(*p == L'+' || *p == L'-')
				{
						++p;
				}
				
				is_float = false;

				while(p < end && *p >= L'0' && *p <= L'9')/*e后必须跟随指数*/
				{
						++p;
						is_float = true;
				}
		}
		return is_float;
}

ar_bool_t	AR_wcs_is_int(const wchar_t *in, const wchar_t *end)
{
		ar_int_64_t un;

		AR_ASSERT(in != NULL && end != NULL && in <= end);

		if(AR_wcs_is_float(in, end))
		{
				return false;
		}

		
		if(AR_wtoi64_s(in, end, &un, 0) != NULL)
		{
				return true;
		}

		if(AR_wtou64_s(in, end, (ar_uint_64_t*)&un, 0) != NULL)
		{
				return true;
		}

		return false;
}

/**********************************************************str*************************************/


ar_bool_t	AR_str_is_float(const char *in, const char *end)
{
		const char *p;
		
		ar_bool_t is_float;

		char decimal_point;
		
		AR_ASSERT(in != NULL && end != NULL && in <= end);

		{
				const struct lconv *conv;
				conv =  localeconv();
				decimal_point = *conv->decimal_point;
		}

		is_float = false;

		p = AR_strtrim_space_s(in, end);

		if(*p == '-' || *p == '+')
		{
				++p;
		}


		while(p < end && *p >= '0' && *p <= '9')/*可能是个整数*/
		{
				++p;
		}

		if(p < end && *p == decimal_point)/*也许是浮点，但可能有错误*/
		{
				is_float = true;
				++p;

				while(p < end && *p >= '0' && *p <= '9')
				{
						++p;
				}
		}

		if(p < end && (*p == 'e' || *p == 'E'))/*可能是浮点，但是会有错误*/
		{
				++p;

				if(*p == '+' || *p == '-')
				{
						++p;
				}
				
				is_float = false;

				while(p < end && *p >= '0' && *p <= '9')/*e后必须跟随指数*/
				{
						++p;
						is_float = true;
				}
		}
		return is_float;
}


ar_bool_t	AR_str_is_int(const char *in, const char *end)
{
		ar_int_64_t un;

		AR_ASSERT(in != NULL && end != NULL && in <= end);

		if(AR_str_is_float(in, end))
		{
				return false;
		}

		
		if(AR_stoi64_s(in, end, &un, 0) != NULL)
		{
				return true;
		}

		if(AR_stou64_s(in, end, (ar_uint_64_t*)&un, 0) != NULL)
		{
				return true;
		}

		return false;
}



AR_NAMESPACE_END






