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


/*****************************************************cmp********************************************************************/


int_i_t	AR_stricmp(const char *l, const char *r)
{
		int_i_t ret;
		AR_ASSERT(l != NULL && r != NULL);

		for(ret = 0; (ret = (AR_tolower(*l) - AR_tolower(*r))) == 0 && *l && *r; ++l, ++r);
		
		return ret;
}

int_i_t	AR_strnicmp(const char *l, const char *r, size_t n)
{
		int_i_t ret;
		size_t i;
		AR_ASSERT(l != NULL && r != NULL);

		for(ret = 0, i = 0; i < n && (ret = (AR_tolower(l[i]) - AR_tolower(r[i]))) == 0 && l[i] && r[i]; ++i)
		{

		}
		
		return ret;

}

int_i_t	AR_wcsicmp(const wchar_t *l, const wchar_t *r)
{
		int_i_t ret;
		AR_ASSERT(l != NULL && r != NULL);

		for(ret = 0; (ret = (AR_towlower(*l) - AR_towlower(*r))) == 0 && *l && *r; ++l, ++r);
		
		return ret;

}

int_i_t	AR_wcsnicmp(const wchar_t *l, const wchar_t *r, size_t n)
{
		int_i_t ret;
		size_t i;
		AR_ASSERT(l != NULL && r != NULL);

		for(ret = 0, i = 0; i < n && (ret = (AR_towlower(l[i]) - AR_towlower(r[i]))) == 0 && l[i] && r[i]; ++i);
		
		return ret;

}



/**********************************************************string*************************************************************/


#define __MODIFIER_ANSI			0x10000
#define __MODIFIER_UNICODE		0x20000
#define	__MODIFIER_INT64		0x40000

int_i_t AR_vscwprintf(const wchar_t *fmt, va_list args)
{
		int_i_t res;
		va_list save;
		AR_ASSERT(fmt != NULL && args != NULL);
		res = 0;
		AR_memcpy(&save, &args, sizeof(va_list));

		while(*fmt)
		{
				int_i_t len = 0, width = 0, prec = 0, modifier = 0;
				if(*fmt != L'%' || *(++fmt) == L'%')
				{
						fmt++;
						res++;
						continue;
				}

				/*处理%后面的*/


				while(*fmt)
				{
						if(*fmt == L'#')
						{
								res += 2; /*0x*/
						}else if(*fmt == L'*')
						{
								width = va_arg(args, int);
						}else if(*fmt == L'-' || *fmt == L'+' || *fmt == L'0' || *fmt == L' ')
						{
						}else
						{
								break;
						}
						fmt++;
				}

				if(width == 0)
				{
						const wchar_t *s; int_i_t w;
						s = AR_wtoi(fmt, &w, 10);
						if(s != NULL)
						{
								fmt = s;
								width = w;
						}
						AR_ASSERT(fmt != NULL && width >= 0);
						if(fmt == NULL || width < 0)return (int_i_t)-1;
				}

				if(*fmt == L'.')
				{
						/*width.prec*/
						fmt++;
						if(*fmt == L'*')
						{
								prec = va_arg(args, int_32_t);
								fmt++;
						}else
						{
								const wchar_t *s; int_i_t p;
								s = AR_wtoi(fmt, &p, 10);
								if(s != NULL)
								{
										fmt = s;
										prec = p;
								}
								AR_ASSERT(fmt != NULL && prec >= 0);
								if(fmt == NULL || prec < 0)return -1;
						}

				}

				modifier = 0;

				if(*fmt == L'I' && *(fmt + 1) == L'6' && *(fmt + 2) == L'4')/*VC,BCB等*/
				{
						/*例如%I64d*/
						fmt += 3;
						modifier |= __MODIFIER_INT64;
				}else if(*fmt == L'I' && *(fmt + 1) == L'3' && *(fmt + 2) == L'2')/*VC,BCB等*/
				{
						/*例如%I32d*/
						fmt += 3;
				}else if(*fmt == L'I')
				{
						/*例如%Id*/
						fmt += 1;

						#if(AR_ARCH_VER == AR_ARCH_64)
								modifier |= __MODIFIER_INT64
						#elif(AR_ARCH_VER == AR_ARCH_32)

						#endif

				}else if(*fmt == L'l' && *(fmt + 1) == L'l')/*VC gcc一族编译器*/
				{
						/*例如%lld*/
						fmt += 2;
						modifier |= __MODIFIER_INT64;
				}else
				{
						switch (*fmt)
						{
								//强制ANSI
						case L'h':
								modifier |= __MODIFIER_ANSI;
								fmt++;
								break;
						case L'l':
								modifier |= __MODIFIER_UNICODE;
								fmt++;
								break;
								//无用
						case L'F':
						case L'N':
						case L'L':
								fmt++;
								break;
						}
				}

				switch (*fmt | modifier)
				{
				case L'c':
				case L'C':
						len = 2;
						va_arg(args, wint_t);
						break;
				case L'c' | __MODIFIER_ANSI:
				case L'C' | __MODIFIER_ANSI:

						len = 2;

#if(AR_COMPILER == AR_GCC3 || AR_COMPILER == AR_GCC4)
						va_arg(args, int);
#else
						va_arg(args, char);
#endif

						break;
				case L'c' | __MODIFIER_UNICODE:
				case L'C' | __MODIFIER_UNICODE:
						len = 2;
						va_arg(args, wchar_t);
						break;
				case L's':
				{
						const wchar_t *str = va_arg(args, const wchar_t*);
						if(str == NULL)
						{
								len = 6;/*(null)*/
						}else
						{
								len =(int_i_t)AR_wcslen(str);
								len = AR_MAX(len, 1);
						}
						break;
				}
				case L'S':
				{
						const char *str = va_arg(args, const char*);
						if(str == NULL)
						{
								len = 6;/*(null)*/
						}else
						{
								len = (int_i_t)AR_strlen(str);
								len = AR_MAX(len,1);
						}
						break;
				}
				case L's' | __MODIFIER_ANSI:
				case L'S' | __MODIFIER_ANSI:
				{
						const char *str = va_arg(args, const char*);
						if(str == NULL)
						{
								len = 6;/*(null)*/
						}else
						{
								len = (int_i_t)AR_strlen(str);
								len = AR_MAX(len,1);
						}
						break;
				}
				case L's' | __MODIFIER_UNICODE:
				case L'S' | __MODIFIER_UNICODE:
				{
						const wchar_t *str = va_arg(args, const wchar_t*);
						if(str == NULL)
						{
								len = 6;/*(null)*/
						}else
						{
								len = (int_i_t)AR_wcslen(str);
								len = AR_MAX(len, 1);
						}
						break;

				}
				}

				if(len != 0)
				{
						len = AR_MAX(len, width);
						if(prec != 0)len = AR_MIN(len, prec);
				}else
				{
						switch (*fmt)
						{
						case L'd':
						case L'i':
						case L'u':
						case L'x':
						case L'X':
						case L'o':
						{
								if(modifier & __MODIFIER_INT64)
								{
										va_arg(args, int_64_t);
								}else
								{
										va_arg(args, int_32_t);
								}
								len = 32;
								len = AR_MAX(len, width + prec);
								break;
						}
						case L'e':
						case L'E':
						case L'g':
						case L'G':
						{
								va_arg(args, double);
								len = 128;
								len = AR_MAX(len, width + prec);
								break;
						}
						case L'f':
						{
								int_i_t cclen;
								wchar_t *buf;
								double f= va_arg(args, double);

								/*
										312 == wcslen(L"-1+(0{309})")
										double 最大精度为.0{309},这里精度默认设置为6
								*/

								cclen = AR_MAX(width, 312 + prec + 6);/*取最大值*/

								buf = AR_NEWARR0(wchar_t, cclen);
								AR_swprintf(buf, cclen, L"%*.*f", width, prec + 6, f);
								len = AR_wcslen(buf);
								AR_DEL(buf);
								break;
						}
						case L'p':
						{
								va_arg(args, void*);
								len = AR_MAX(32, width + prec);
								break;
						}
						case L'n':
						{
								va_arg(args, int*);
								break;
						}
						default:
						{
								AR_ASSERT(false);
								return -1;
						}

						}
				}
				fmt++;
				res += len;
		}
		va_end(save);
		return res;
}
#undef	__MODIFIER_ANSI
#undef	__MODIFIER_UNICODE
#undef	__MODIFIER_INT64



int_i_t			AR_scwprintf(const wchar_t *fmt, ...)
{
		int_i_t len = -1;
		va_list	arg_ptr;
		AR_ASSERT(fmt != NULL);

		va_start(arg_ptr, fmt);
		len = AR_vscwprintf(fmt, arg_ptr);
		va_end(arg_ptr);
		return len;
}



wchar_t*		AR_vtow(const wchar_t *fmt, ...)
{
		va_list			args;

		wchar_t			*buf;
		int_i_t			len;

		AR_ASSERT(fmt != NULL);

		va_start (args, fmt);
		len = AR_vscwprintf(fmt, args);
		buf = AR_NEWARR(wchar_t, len + 1);
		AR_vswprintf(buf, len + 1, fmt, args);
		va_end (args);
		return buf;
}






int_i_t AR_wchartodigit(wchar_t ch)
{
#define DIGIT_RANGE_TEST(zero)  \
    if (ch < zero)              \
        return -1;              \
    if (ch < zero + 10)         \
    {                           \
        return ch - zero;       \
    }

    DIGIT_RANGE_TEST(0x0030)        // 0030;DIGIT ZERO
    if (ch < 0xFF10)                // FF10;FULLWIDTH DIGIT ZERO
    {
        DIGIT_RANGE_TEST(0x0660)    // 0660;ARABIC-INDIC DIGIT ZERO
        DIGIT_RANGE_TEST(0x06F0)    // 06F0;EXTENDED ARABIC-INDIC DIGIT ZERO
        DIGIT_RANGE_TEST(0x0966)    // 0966;DEVANAGARI DIGIT ZERO
        DIGIT_RANGE_TEST(0x09E6)    // 09E6;BENGALI DIGIT ZERO
        DIGIT_RANGE_TEST(0x0A66)    // 0A66;GURMUKHI DIGIT ZERO
        DIGIT_RANGE_TEST(0x0AE6)    // 0AE6;GUJARATI DIGIT ZERO
        DIGIT_RANGE_TEST(0x0B66)    // 0B66;ORIYA DIGIT ZERO
        DIGIT_RANGE_TEST(0x0C66)    // 0C66;TELUGU DIGIT ZERO
        DIGIT_RANGE_TEST(0x0CE6)    // 0CE6;KANNADA DIGIT ZERO
        DIGIT_RANGE_TEST(0x0D66)    // 0D66;MALAYALAM DIGIT ZERO
        DIGIT_RANGE_TEST(0x0E50)    // 0E50;THAI DIGIT ZERO
        DIGIT_RANGE_TEST(0x0ED0)    // 0ED0;LAO DIGIT ZERO
        DIGIT_RANGE_TEST(0x0F20)    // 0F20;TIBETAN DIGIT ZERO
        DIGIT_RANGE_TEST(0x1040)    // 1040;MYANMAR DIGIT ZERO
        DIGIT_RANGE_TEST(0x17E0)    // 17E0;KHMER DIGIT ZERO
        DIGIT_RANGE_TEST(0x1810)    // 1810;MONGOLIAN DIGIT ZERO


        return -1;
    }
#undef DIGIT_RANGE_TEST

                                    // FF10;FULLWIDTH DIGIT ZERO
    if (ch < 0xFF10 + 10)
    {
        return ch - 0xFF10;
    }
    return -1;

}







const wchar_t* AR_wcstrim_s(const wchar_t *in, const wchar_t *end, const wchar_t *trim)
{
		AR_ASSERT(in != NULL && end != NULL && trim != NULL);

		while(in < end && AR_wcschr(trim, *in) != NULL)in++;
		return in;
}



const wchar_t*	AR_wcstrim_space_s(const wchar_t *in, const wchar_t *end)
{
		AR_ASSERT(in != NULL && end != NULL);
		while(in < end && AR_iswspace(*in))in++;
		return in;
}


/************************************************************************************************************************************/


wchar_t*		AR_wcscat(wchar_t *dest, const wchar_t *sour)
{
		wchar_t *d;
		AR_ASSERT(dest != NULL && sour != NULL);
		d = dest;
		while(*d)d++;

		while(*sour)*d++ = *sour++;
		*d = 0;
		return dest;
}


wchar_t*		AR_wcsncat(wchar_t *dest, const wchar_t *sour, size_t n)
{
		wchar_t *d;
		size_t i;
		AR_ASSERT(dest != NULL && sour != NULL);
		d = dest;
		while(*d)d++;

		for(i = 0; i < n && sour[i]; ++i,++d) *d = sour[i];

		*d = 0;

		return dest;


}


wchar_t*		AR_wcstrim_right(wchar_t *in, const wchar_t *trim)
{
		wchar_t *p = NULL, *plast = NULL;
		AR_ASSERT(in != NULL && trim != NULL);
		p = in;
		while(*p != L'\0')
		{
				if(AR_wcschr(trim, *p) == NULL)
				{
						plast = NULL;
				}else if(plast == NULL)
				{
						plast = p;
				}
				++p;
		}

		if(plast)*plast = L'\0';
		return in;
}



const wchar_t* AR_wcstrim(const wchar_t *in, const wchar_t *trim)
{
		AR_ASSERT(in != NULL && trim != NULL);

		while(*in != L'\0' && AR_wcschr(trim, *in) != NULL)in++;
		return in;
}

const wchar_t*	AR_wcstrim_space(const wchar_t *in)
{
		AR_ASSERT(in != NULL);
		while(*in != L'\0' && AR_iswspace(*in))in++;
		return in;
}


const wchar_t* AR_wtoi64(const wchar_t *in,	 int_64_t  *num, size_t base)
{
		return AR_wtoi64_s(in, in + AR_wcslen(in), num, base);
}

const wchar_t* AR_wtou64(const wchar_t *in,	 uint_64_t  *num, size_t base)
{
		return AR_wtou64_s(in, in + AR_wcslen(in), num, base);
}


const wchar_t* AR_wtoi32(const wchar_t *in,  int_32_t *num, size_t base)
{
		return AR_wtoi32_s(in, in + AR_wcslen(in), num, base);
}

const wchar_t* AR_wtou32(const wchar_t *in,  uint_32_t *num, size_t base)
{
		return AR_wtou32_s(in, in + AR_wcslen(in), num, base);
}







uint_u_t	AR_wcshash(const wchar_t *str)
{
		uint_u_t	ret;
		size_t	i;
		AR_ASSERT(str != NULL);

		ret = 0;
		for(i = 0; str[i]; ++i)
		{
				ret ^= (str[i] << (i & 0x0F));
		}
		return ret;
}


uint_u_t		AR_wcshash_n(const wchar_t *str, size_t n)
{
		uint_u_t	ret;
		size_t	i;
		if(n == 0)return 0;
		AR_ASSERT(str != NULL);

		ret = 0;
		for(i = 0; i < n && str[i]; ++i)
		{
				ret ^= (str[i] << (i & 0x0F));
		}
		return ret;
}


wchar_t* AR_wcsdup(const wchar_t *sour)
{
		AR_ASSERT(sour != NULL);
		return AR_wcsndup(sour, AR_wcslen(sour));
}



wchar_t* AR_wcsndup(const wchar_t *sour, size_t len)
{
		wchar_t *result;
		if(len == 0)
		{
				result = AR_NEWARR0(wchar_t, 2);
				result[0] = L'\0';
				return result;
		}
		AR_ASSERT(sour != NULL && len > 0);
		result = AR_NEWARR(wchar_t, len + 1);
		AR_wcsncpy(result, sour, len);
		result[len] = L'\0';
		return result;
}




#define	__BUFFER_LEN	256


int_i_t			AR_u64tow_buf(wchar_t *out, size_t nbuf, uint_64_t num, size_t radix)
{
		wchar_t buf[__BUFFER_LEN];
		wchar_t *p;
		int_i_t len;
		static const wchar_t* __tbl = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		AR_ASSERT(radix >= 2 && radix <= 36);

		p = buf + __BUFFER_LEN;
		*--p = 0;
		do{ *--p = __tbl[num % radix];}while((num /= radix) > 0);

		len = (int_i_t)(buf + __BUFFER_LEN - p);/*返回的是需要元素数组的长度包含0*/

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




int_i_t			AR_i64tow_buf(wchar_t *out, size_t nbuf, int_64_t num, size_t radix)
{
		ar_bool_t is_neg;
		wchar_t buf[__BUFFER_LEN];
		wchar_t *p;
		int_i_t len;
		//static const wchar_t* __tbl = L"0123456789ABCDEF";
		static const wchar_t* __tbl = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		AR_ASSERT(radix >= 2 && radix <= 36);
		is_neg = false;

		if(num < 0){ is_neg = true; num = -num;}

		p = buf + __BUFFER_LEN;
		*--p = 0;
		do{ *--p = __tbl[num % radix];}while((num /= radix) > 0);
		if(is_neg)*--p = L'-';

		len = (int_i_t)(buf + __BUFFER_LEN - p);/*返回的是需要元素数组的长度包含0*/

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


/*
const wchar_t*	AR_wtod_s(const wchar_t *in, const wchar_t *end, double *num)
{
		const wchar_t *stop;
		wchar_t *buf;
		AR_ASSERT(in != NULL && end != NULL && num != NULL);

		if(in >= end)return NULL;
		buf  = AR_wcsndup(in, end - in);

		stop = AR_wtod(buf, num);

		if(stop == NULL)
		{
				AR_DEL(buf);
				return NULL;
		}else
		{
				const wchar_t *res;
				res = &in[stop - buf];
				AR_DEL(buf);
				return res;
		}
}
*/


/*此函数貌似不太完整，对溢出检测也没做到位*/
const wchar_t*	AR_wtod_s(const wchar_t *in, const wchar_t *end, double *out)
{
		const wchar_t *p;
		double num = 0.0f,frac = 0.0f, exp = 1.0f;
		double result = 0.0f;
		ar_bool_t is_neg = false, is_ok = false;

		AR_ASSERT(in != NULL && end != NULL && in <= end && out != NULL);

		/*p = AR_wcstrim(in, L" \t");*/
		p = AR_wcstrim_space_s(in, end);

		if(*p == L'-')
		{
				++p;
				is_neg = true;
		}else if(*p == L'+')
		{
				++p;
		}


		while(p < end && *p >= L'0' && *p <= L'9')
		{
				num *= 10.0f;
				num += (double)(*p - L'0');
				++p;
				is_ok = true;
		}

		if(p < end && *p == L'.')
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
				}else if(*p == L'-')
				{
						factor = false;
				}else
				{
						return NULL;
				}
				++p;
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


static const wchar_t* __wtou64_s(const wchar_t *in, const wchar_t *end, uint_64_t  *num, size_t base, uint_32_t flag)
{
		uint_64_t val,digit, maxval;
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
				int_i_t tmp;
				if((tmp = AR_wchartodigit(*p)) != -1)
				{
						//tmp = (*p - L'0');
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

				digit = (uint_64_t)tmp;
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
				if ( flag & __UNSIGNED )
				{
						val = AR_UINT64_MAX;
				} else if (flag & __NEG )
				{
						val = (uint_64_t)(-AR_INT64_MIN);
				}else
				{
						val = AR_INT64_MAX;
				}
		}

		if(flag & __NEG)
		{
				val = (uint_64_t)(-(int_64_t)val);
		}
		*num = val;
		return p;
}



static const wchar_t* __wtou32_s(const wchar_t *in, const wchar_t *end, uint_32_t  *num, size_t base, uint_32_t flag)
{
		uint_32_t val,digit, maxval;
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

		maxval = AR_UINT32_MAX / (uint_32_t)base;
		digit = 0;
		val = 0;

		while(p < end)
		{
				int_i_t tmp;
				if((tmp = AR_wchartodigit(*p)) != -1)
				{
						//tmp = (*p - L'0');
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

				digit = (uint_32_t)tmp;
				if(digit >= base)break;
				flag |= __READDIGIT;



				if (val < maxval || (val == maxval && digit <= AR_UINT32_MAX % base))
				{
						val = val * (uint_32_t)base + digit;
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
						val = (uint_32_t)(-AR_INT32_MIN);
				}else
				{
						val = AR_INT32_MAX;
				}
		}

		if(flag & __NEG)
		{
				val = (uint_32_t)(-(int_32_t)val);
		}
		*num = val;
		return p;
}






const wchar_t* AR_wtoi64_s(const wchar_t *in, const wchar_t *end, int_64_t  *num, size_t base)
{
		uint_32_t flag = 0;
		uint_64_t val;
		const wchar_t *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __wtou64_s(in, end, &val, base, flag);

		*num = (int_64_t)val;

		return p;
}


const wchar_t* AR_wtou64_s(const wchar_t *in, const wchar_t *end, uint_64_t  *num, size_t base)
{
		uint_32_t flag = __UNSIGNED;
		const wchar_t *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __wtou64_s(in, end, num, base, flag);

		return p;

}





const wchar_t* AR_wtoi32_s(const wchar_t *in, const wchar_t *end, int_32_t  *num, size_t base)
{
		uint_32_t flag = 0;
		uint_32_t val;
		const wchar_t *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __wtou32_s(in, end, &val, base, flag);

		*num = (int_32_t)val;

		return p;
}


const wchar_t* AR_wtou32_s(const wchar_t *in, const wchar_t *end, uint_32_t  *num, size_t base)
{
		uint_32_t flag = __UNSIGNED;
		const wchar_t *p;
		AR_ASSERT(in != NULL && end != NULL && num != NULL && (base == 0 || (base > 1 && base <= 36)));

		p = __wtou32_s(in, end, num, base, flag);

		return p;

}





AR_NAMESPACE_END

