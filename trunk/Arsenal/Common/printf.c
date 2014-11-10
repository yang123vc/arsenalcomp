/*
 * The Arsenal Library
 * Copyright (c) 2009-2012 by Solidus
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

#define __MODIFIER_ANSI			0x10000
#define __MODIFIER_UNICODE		0x20000
#define	__MODIFIER_INT64		0x40000


/**********************************************************str version*************************************************************/


static ar_int_t __str_format_preprocess(const char *fmt, char *out)
{
		char *p;
		ar_int_t	need_l;
		AR_ASSERT(fmt != NULL);

		if(AR_strlen(fmt) == 0)
		{
				return 1;
		}

		p = out;
		need_l = 0;

		while(*fmt)
		{
				ar_int_t modifier = 0;

				if(*fmt != '%')
				{
						if(p)
						{
								*p++ = *fmt;
						}
						fmt++;
						need_l++;

						continue;
				}else if(*fmt == '%' && *(fmt + 1) == '%')
				{
						if(p)
						{
								*p++ = *fmt;
								*p++ = *fmt;
						}
						need_l += 2;
						fmt += 2;
						continue;
				}else
				{
						if(p)
						{
								*p++ = *fmt;
						}
						need_l++;
						fmt++;
				}

				/*Dealing with '%' behind*/


				while(*fmt)
				{
						if(*fmt == '#')
						{
								if(p)
								{
										*p++ = *fmt;/*0x*/
								}

								need_l++;
								fmt++;
						}else if(*fmt == '*')
						{
								if(p)
								{
										*p++ = *fmt;
								}

								need_l++;
								fmt++;
						}else if(*fmt == '-' || *fmt == '+' || *fmt == '0' || *fmt == ' ')
						{
								if(p)
								{
										*p++ = *fmt;
								}

								need_l++;
								fmt++;
						}else
						{
								break;
						}
				}

				while(*fmt && AR_isdigit(*fmt))
				{
						if(p)
						{
								*p++ = *fmt;
						}
						need_l++;
						fmt++;
				}


				if(*fmt == '.')
				{
						/*width.prec*/
						if(p)
						{
								*p++ = *fmt;
						}
						need_l++;
						fmt++;

						if(*fmt == '*')
						{
								if(p)
								{
										*p++ = *fmt;
								}
								need_l++;
								fmt++;
						}else
						{
								while(*fmt && AR_isdigit(*fmt))
								{
										if(p)
										{
												*p++ = *fmt;
										}
										need_l++;
										fmt++;
								}
						}

				}




				switch (*fmt)
				{
				case 'I':
				{
						#if(AR_ARCH_VER == ARCH_64)
						{
								const char *pfmt64 = AR_FMT64_STR;
								while(*pfmt64 != '\0')
								{
										if(p)
										{
												*p++ = *pfmt64;
										}
										pfmt64++;
										need_l++;
								}
						}
						#elif(AR_ARCH_VER == AR_ARCH_32)

						#endif

						fmt++;
				}
						break;
				case 'q': /*64bit*/
				{
						const char *pfmt64 = AR_FMT64_STR;
						while(*pfmt64 != '\0')
						{
								if(p)
								{
										*p++ = *pfmt64;
								}
								pfmt64++;
								need_l++;
						}

						fmt++;
				}
						break;
				case 'h':		/*Forced ANSI*/
						modifier |= __MODIFIER_ANSI;
						fmt++;
						break;
				case 'l':
						modifier |= __MODIFIER_UNICODE;
						fmt++;
						break;
				case 'F':
				case 'N':
				case 'L':
						if(p)
						{
								*p++ = *fmt;
						}

						need_l++;
						fmt++;
						break;
				}

				switch (*fmt | modifier)
				{
				case 'd':
				case 'i':
				case 'u':
				case 'x':
				case 'X':
				case 'o':
				case 'e':
				case 'E':
				case 'g':
				case 'G':
				case 'f':
				case 'p':
				case 'n':
						if(p)
						{
								*p++ = *fmt;
						}
						fmt++;
						need_l++;
						break;
				case 'c':
				case 'C':
				case 'c' | __MODIFIER_ANSI:
				case 'C' | __MODIFIER_ANSI:
						if(p)
						{
								*p++ = 'h';
								*p++ = 'c';
						}
						fmt++;
						need_l += 2;
						break;
				case 's':
				case 'S':
				case 's' | __MODIFIER_ANSI:
				case 'S' | __MODIFIER_ANSI:
						if(p)
						{
								*p++ = 'h';
								*p++ = 's';
						}
						fmt++;
						need_l += 2;
						break;
				case 'c' | __MODIFIER_UNICODE:
				case 'C' | __MODIFIER_UNICODE:
						if(p)
						{
								*p++ = 'l';
								*p++ = 'c';
						}
						fmt++;
						need_l += 2;
						break;
				case 's' | __MODIFIER_UNICODE:
				case 'S' | __MODIFIER_UNICODE:
						if(p)
						{
								*p++ = 'l';
								*p++ = 's';
						}
						fmt++;
						need_l += 2;
						break;
				default:
						AR_ASSERT(false);
						return -1;
						break;
				}
		}

		if(p)
		{
				*p = '\0';
		}

		need_l++;
		return need_l;
}







ar_int_t AR_vscprintf(const char *fmt, va_list va_args)
{
		ar_int_t res;
		va_list args;
		AR_ASSERT(fmt != NULL);
		res = 0;

		AR_va_copy(args, va_args);

		while(*fmt)
		{
				ar_int_t len = 0, width = 0, prec = 0, modifier = 0;
				if(*fmt != '%' || *(++fmt) == '%')
				{
						fmt++;
						res++;
						continue;
				}

				/*Dealing with '%' behind*/


				while(*fmt)
				{
						if(*fmt == '#')
						{
								res += 2; /*0x*/
						}else if(*fmt == '*')
						{
								width = AR_va_arg(args, int);
						}else if(*fmt == '-' || *fmt == '+' || *fmt == '0' || *fmt == ' ')
						{

						}else
						{
								break;
						}
						fmt++;
				}

				if(width == 0)
				{
						while(*fmt && AR_isdigit(*fmt))
						{
								width *= 10;
								width += *fmt - '0';
								fmt++;
						}
				}

				if(*fmt == '.')
				{
						/*width.prec*/
						fmt++;
						if(*fmt == '*')
						{
								prec = AR_va_arg(args, ar_int_32_t);
								fmt++;
						}else
						{
								prec = 0;
								while(*fmt && AR_isdigit(*fmt))
								{
										prec *= 10;
										prec += *fmt - '0';
										fmt++;
								}
						}

				}

				modifier = 0;

				if(*fmt == 'I')
				{
						/*%Id*/
						fmt += 1;

						#if(AR_ARCH_VER == ARCH_64)
								modifier |= __MODIFIER_INT64;
						#elif(AR_ARCH_VER == ARCH_32)

						#endif
				}else if(*fmt == 'q')
				{
						fmt += 1;
						modifier |= __MODIFIER_INT64;
				}else
				{
						switch (*fmt)
						{
								/*forced ANSI*/
						case 'h':
								modifier |= __MODIFIER_ANSI;
								fmt++;
								break;
						case 'l':
								modifier |= __MODIFIER_UNICODE;
								fmt++;
								break;
								/*useless*/
						case 'F':
						case 'N':
						case 'L':
								fmt++;
								break;
						}
				}

				switch (*fmt | modifier)
				{
				case 'c':
				case 'C':
						len = 2;
#if defined(AR_VAARGS_TREAT_WCHAR_AS_INT)
						AR_va_arg(args, int);
#else
						AR_va_arg(args, wint_t);
#endif
						break;
				case 'c' | __MODIFIER_ANSI:
				case 'C' | __MODIFIER_ANSI:
						len = 2;

#if defined(AR_VAARGS_TREAT_CHAR_AS_INT)
						AR_va_arg(args, int);
#else
						AR_va_arg(args, char);
#endif
						break;
				case 'c' | __MODIFIER_UNICODE:
				case 'C' | __MODIFIER_UNICODE:
						len = 2;
#if defined(AR_VAARGS_TREAT_WCHAR_AS_INT)
						AR_va_arg(args, int);
#else
						AR_va_arg(args, wchar_t);
#endif
						break;
				case 's':
				case 'S':
				case 's' | __MODIFIER_ANSI:
				case 'S' | __MODIFIER_ANSI:
				{
						const char *str = AR_va_arg(args, const char*);
						if(str == NULL)
						{
								len = 6;/*(null)*/
						}else
						{
								len =(ar_int_t)AR_strlen(str);
								len = AR_MAX(len, 1);
						}
						break;
				}
				case 's' | __MODIFIER_UNICODE:
				case 'S' | __MODIFIER_UNICODE:
				{
						const wchar_t *str = AR_va_arg(args, const wchar_t*);
						if(str == NULL)
						{
								len = 6;/*(null)*/
						}else
						{
								len = (ar_int_t)AR_wcslen(str) * 4;
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
						case 'd':
						case 'i':
						case 'u':
						case 'x':
						case 'X':
						case 'o':
						{
								if(modifier & __MODIFIER_INT64)
								{
										AR_va_arg(args, ar_int_64_t);
								}else
								{
										AR_va_arg(args, ar_int_32_t);
								}
								len = 32;
								len = AR_MAX(len, width + prec);
								break;
						}
						case 'e':
						case 'E':
						case 'g':
						case 'G':
						{
								AR_va_arg(args, double);
								len = 128;
								len = AR_MAX(len, width + prec);
								break;
						}
						case 'f':
						{
								ar_int_t cclen;
								char *buf;
								double f= AR_va_arg(args, double);

								/*
										312 == strlen("-1+(0{309})")
										double maximum accuracy .0 {309}, 
										precision default is set to 6
								*/

								cclen = AR_MAX(width, 312 + prec + 6);/*Take the maximum*/

								buf = AR_NEWARR0(char, cclen);

								if(buf == NULL)
								{
										return -1;
								}
								
								AR_SPRINTF(buf, cclen, "%*.*f", (int)width, (int)prec + 6, f);
								len = AR_strlen(buf);
								AR_DEL(buf);
								break;
						}
						case 'p':
						{
								AR_va_arg(args, void*);
								len = AR_MAX(32, width + prec);
								break;
						}
						case 'n':
						{
								AR_va_arg(args, int*);
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
		AR_va_end(args);
		return res;
}




ar_int_t			AR_scprintf(const char *fmt, ...)
{
		ar_int_t len = -1;
		va_list	arg_ptr;

		AR_ASSERT(fmt != NULL);

		AR_va_start(arg_ptr, fmt);
		len = AR_vscprintf(fmt, arg_ptr);
		AR_va_end(arg_ptr);
		return len;
}



char*		AR_vtos(const char *fmt, ...)
{
		va_list			args;

		char			*buf;
		ar_int_t			len;

		AR_ASSERT(fmt != NULL);

		AR_va_start (args, fmt);
		len = AR_vscprintf(fmt, args);
		AR_va_end (args);

		if(len < 0)
		{
				return NULL;
		}
		
		buf = AR_NEWARR(char, len + 1);

		if(buf == NULL)
		{
				return NULL;
		}

		AR_va_start (args, fmt);
		
		if(AR_vsprintf(buf, len + 1, fmt, args) < 0)
		{
				AR_DEL(buf);
				buf = NULL;
		}
		AR_va_end (args);
		return buf;
}




ar_int_t			AR_vsprintf(char *dest, size_t count, const char *fmt, va_list args)
{
		ar_int_t res;
		va_list save;

		char *src_fmt;
		ar_int_t need_l;

		AR_ASSERT(dest != NULL && fmt != NULL);

		res = 0;
		need_l = 0;
		src_fmt = NULL;



		/********************converted to the format of the target CRT***************/
		need_l = __str_format_preprocess(fmt, NULL);
		if(need_l <= 0)
		{
				res = -1;
				goto END_POINT;
		}

		src_fmt = AR_NEWARR(char, need_l);

		if(src_fmt == NULL)
		{
				res = -1;
				goto END_POINT;
		}

		__str_format_preprocess(fmt, src_fmt);

		AR_va_copy(save, args);
		res = AR_VSPRINTF(dest, count, src_fmt, save);
		AR_va_end(save);

	   /*****************************************************************************/


END_POINT:


		if(src_fmt)
		{
				AR_DEL(src_fmt);
				src_fmt = NULL;
		}

#if(0)
		if(res <= 0)						/*ignored such errors*/
		{
				dest[0] = '\0';
				res = 0;
		}
#endif


		return res;
}



ar_int_t			AR_sprintf(char *dest, size_t count, const char *fmt, ...)
{
		ar_int_t len = -1;
		va_list	arg_ptr;
		AR_ASSERT(fmt != NULL);

		AR_va_start(arg_ptr, fmt);
		len = AR_vsprintf(dest, count, fmt, arg_ptr);
		AR_va_end(arg_ptr);
		
		return len;
}


/**********************************************************wcs version*************************************************************/




static ar_int_t __wcs_format_preprocess(const wchar_t *fmt, wchar_t *out)
{
		wchar_t *p;
		ar_int_t	need_l;
		AR_ASSERT(fmt != NULL);

		if(AR_wcslen(fmt) == 0)
		{
				return 1;
		}

		p = out;
		need_l = 0;

		while(*fmt)
		{
				ar_int_t modifier = 0;

				if(*fmt != L'%')
				{
						if(p)
						{
								*p++ = *fmt;
						}
						fmt++;
						need_l++;

						continue;
				}else if(*fmt == L'%' && *(fmt + 1) == L'%')
				{
						if(p)
						{
								*p++ = *fmt;
								*p++ = *fmt;
						}
						need_l += 2;
						fmt += 2;
						continue;
				}else
				{
						if(p)
						{
								*p++ = *fmt;
						}
						need_l++;
						fmt++;
				}

				/*dealing with '%' behind*/


				while(*fmt)
				{
						if(*fmt == L'#')
						{
								if(p)
								{
										*p++ = *fmt;/*0x*/
								}

								need_l++;
								fmt++;
						}else if(*fmt == L'*')
						{
								if(p)
								{
										*p++ = *fmt;
								}

								need_l++;
								fmt++;
						}else if(*fmt == L'-' || *fmt == L'+' || *fmt == L'0' || *fmt == L' ')
						{
								if(p)
								{
										*p++ = *fmt;
								}

								need_l++;
								fmt++;
						}else
						{
								break;
						}
				}

				while(*fmt && AR_iswdigit(*fmt))
				{
						if(p)
						{
								*p++ = *fmt;
						}
						need_l++;
						fmt++;
				}


				if(*fmt == L'.')
				{
						/*width.prec*/
						if(p)
						{
								*p++ = *fmt;
						}
						need_l++;
						fmt++;

						if(*fmt == L'*')
						{
								if(p)
								{
										*p++ = *fmt;
								}
								need_l++;
								fmt++;
						}else
						{
								while(*fmt && AR_iswdigit(*fmt))
								{
										if(p)
										{
												*p++ = *fmt;
										}
										need_l++;
										fmt++;
								}
						}

				}




				switch (*fmt)
				{
				case L'I':
				{
						#if(AR_ARCH_VER == ARCH_64)
						{
								const wchar_t *pfmt64 = AR_FMT64;
								while(*pfmt64 != L'\0')
								{
										if(p)
										{
												*p++ = *pfmt64;
										}
										pfmt64++;
										need_l++;
								}
						}
						#elif(AR_ARCH_VER == AR_ARCH_32)

						#endif

						fmt++;
				}
						break;
				case L'q': /*64bit*/
				{
						const wchar_t *pfmt64 = AR_FMT64;
						while(*pfmt64 != L'\0')
						{
								if(p)
								{
										*p++ = *pfmt64;
								}
								pfmt64++;
								need_l++;
						}

						fmt++;
				}
						break;
				case L'h':		/*forced ANSI*/
						modifier |= __MODIFIER_ANSI;
						fmt++;
						break;
				case L'l':
						modifier |= __MODIFIER_UNICODE;
						fmt++;
						break;
				case L'F':
				case L'N':
				case L'L':
						if(p)
						{
								*p++ = *fmt;
						}

						need_l++;
						fmt++;
						break;
				}

				switch (*fmt | modifier)
				{
				case L'd':
				case L'i':
				case L'u':
				case L'x':
				case L'X':
				case L'o':
				case L'e':
				case L'E':
				case L'g':
				case L'G':
				case L'f':
				case L'p':
				case L'n':
						if(p)
						{
								*p++ = *fmt;
						}
						fmt++;
						need_l++;
						break;
				case L'c':
				case L'C':
				case L'c' | __MODIFIER_UNICODE:
				case L'C' | __MODIFIER_UNICODE:
						if(p)
						{
								*p++ = L'l';
								*p++ = L'c';
						}
						fmt++;
						need_l += 2;
						break;
				case L's':
				case L'S':
				case L's' | __MODIFIER_UNICODE:
				case L'S' | __MODIFIER_UNICODE:
						if(p)
						{
								*p++ = L'l';
								*p++ = L's';
						}
						fmt++;
						need_l += 2;
						break;
				case L'c' | __MODIFIER_ANSI:
				case L'C' | __MODIFIER_ANSI:
						if(p)
						{
								*p++ = L'h';
								*p++ = L'c';
						}
						fmt++;
						need_l += 2;
						break;
				case L's' | __MODIFIER_ANSI:
				case L'S' | __MODIFIER_ANSI:
						if(p)
						{
								*p++ = L'h';
								*p++ = L's';
						}
						fmt++;
						need_l += 2;
						break;
				default:
						AR_ASSERT(false);
						return -1;
						break;
				}
		}

		if(p)
		{
				*p = L'\0';
		}

		need_l++;
		return need_l;
}



ar_int_t AR_vscwprintf(const wchar_t *fmt, va_list va_args)
{
		ar_int_t res;
		va_list args;
		AR_ASSERT(fmt != NULL);
		res = 0;

		AR_va_copy(args, va_args);

		while(*fmt)
		{
				ar_int_t len = 0, width = 0, prec = 0, modifier = 0;
				if(*fmt != L'%' || *(++fmt) == L'%')
				{
						fmt++;
						res++;
						continue;
				}

				/*dealing with '%' behind*/


				while(*fmt)
				{
						if(*fmt == L'#')
						{
								res += 2; /*0x*/
						}else if(*fmt == L'*')
						{
								width = AR_va_arg(args, int);
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
						while(*fmt && AR_iswdigit(*fmt))
						{
								width *= 10;
								width += AR_wchartodigit(*fmt);
								fmt++;
						}
				}

				if(*fmt == L'.')
				{
						/*width.prec*/
						fmt++;
						if(*fmt == L'*')
						{
								prec = AR_va_arg(args, ar_int_32_t);
								fmt++;
						}else
						{
								prec = 0;
								while(*fmt && AR_iswdigit(*fmt))
								{
										prec *= 10;
										prec += AR_wchartodigit(*fmt);
										fmt++;
								}
						}

				}

				modifier = 0;

				if(*fmt == L'I')
				{
						/*%Id*/
						fmt += 1;

						#if(AR_ARCH_VER == ARCH_64)
								modifier |= __MODIFIER_INT64;
						#elif(AR_ARCH_VER == ARCH_32)

						#endif
				}else if(*fmt == L'q')
				{
						/*%lld*/
						fmt += 1;
						modifier |= __MODIFIER_INT64;
				}else
				{
						switch (*fmt)
						{
								/*forced ANSI*/
						case L'h':
								modifier |= __MODIFIER_ANSI;
								fmt++;
								break;
						case L'l':
								modifier |= __MODIFIER_UNICODE;
								fmt++;
								break;
								/*useless*/
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
#if defined(AR_VAARGS_TREAT_WCHAR_AS_INT)
						AR_va_arg(args, int);
#else
						AR_va_arg(args, wint_t);
#endif
						break;
				case L'c' | __MODIFIER_ANSI:
				case L'C' | __MODIFIER_ANSI:

						len = 2;

#if defined(AR_VAARGS_TREAT_CHAR_AS_INT)
						AR_va_arg(args, int);
#else
						AR_va_arg(args, char);
#endif

						break;
				case L'c' | __MODIFIER_UNICODE:
				case L'C' | __MODIFIER_UNICODE:
						len = 2;
#if defined(AR_VAARGS_TREAT_WCHAR_AS_INT)
						AR_va_arg(args, int);
#else
						AR_va_arg(args, wchar_t);
#endif
						break;
				case L's':
				case L'S':
				{
						const wchar_t *str = AR_va_arg(args, const wchar_t*);
						if(str == NULL)
						{
								len = 6;/*(null)*/
						}else
						{
								len =(ar_int_t)AR_wcslen(str);
								len = AR_MAX(len, 1);
						}
						break;
				}
				case L's' | __MODIFIER_ANSI:
				case L'S' | __MODIFIER_ANSI:
				{
						const char *str = AR_va_arg(args, const char*);
						if(str == NULL)
						{
								len = 6;/*(null)*/
						}else
						{
								len = (ar_int_t)AR_strlen(str);
								len = AR_MAX(len,1);
						}
						break;
				}
				case L's' | __MODIFIER_UNICODE:
				case L'S' | __MODIFIER_UNICODE:
				{
						const wchar_t *str = AR_va_arg(args, const wchar_t*);
						if(str == NULL)
						{
								len = 6;/*(null)*/
						}else
						{
								len = (ar_int_t)AR_wcslen(str);
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
										AR_va_arg(args, ar_int_64_t);
								}else
								{
										AR_va_arg(args, ar_int_32_t);
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
								AR_va_arg(args, double);
								len = 128;
								len = AR_MAX(len, width + prec);
								break;
						}
						case L'f':
						{
								ar_int_t cclen;
								wchar_t *buf;
								double f= AR_va_arg(args, double);

								/*
										312 == wcslen(L"-1+(0{309})")
										double 最大精度为.0{309},这里精度默认设置为6
								*/

								cclen = AR_MAX(width, 312 + prec + 6);/*取最大值*/

								buf = AR_NEWARR0(wchar_t, cclen);

								if(buf == NULL)
								{
										return -1;
								}

								/*
								AR_swprintf(buf, cclen, L"%*.*f", width, prec + 6, f);
								*/
								AR_SWPRINTF(buf, cclen, L"%*.*f", width, prec + 6, f);
								len = AR_wcslen(buf);
								AR_DEL(buf);
								break;
						}
						case L'p':
						{
								AR_va_arg(args, void*);
								len = AR_MAX(32, width + prec);
								break;
						}
						case L'n':
						{
								AR_va_arg(args, int*);
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
		AR_va_end(args);
		return res;
}


ar_int_t			AR_scwprintf(const wchar_t *fmt, ...)
{
		ar_int_t len = -1;
		va_list	arg_ptr;

		AR_ASSERT(fmt != NULL);

		AR_va_start(arg_ptr, fmt);
		len = AR_vscwprintf(fmt, arg_ptr);
		AR_va_end(arg_ptr);
		return len;
}



wchar_t*		AR_vtow(const wchar_t *fmt, ...)
{
		va_list			args;

		wchar_t			*buf;
		ar_int_t			len;

		AR_ASSERT(fmt != NULL);

		AR_va_start (args, fmt);
		len = AR_vscwprintf(fmt, args);
		AR_va_end (args);
		if(len < 0)
		{
				return NULL;
		}

		buf = AR_NEWARR(wchar_t, len + 1);

		if(buf == NULL)
		{
				return NULL;
		}

		AR_va_start (args, fmt);
		
		if(AR_vswprintf(buf, len + 1, fmt, args) < 0)
		{
				AR_DEL(buf);
				buf = NULL;
		}
		AR_va_end (args);
		return buf;
}



ar_int_t			AR_vswprintf(wchar_t *dest, size_t count, const wchar_t *fmt, va_list args)
{
		ar_int_t res;
		va_list save;

		wchar_t *src_fmt;
		ar_int_t need_l;

		AR_ASSERT(dest != NULL && fmt != NULL);

		res = 0;
		need_l = 0;
		src_fmt = NULL;



		/********************converted to the format of the target CRT***************/
		need_l = __wcs_format_preprocess(fmt, NULL);
		if(need_l <= 0)
		{
				res = -1;
				goto END_POINT;
		}

		src_fmt = AR_NEWARR(wchar_t, need_l);

		if(src_fmt == NULL)
		{
				res = -1;
				goto END_POINT;
		}

		__wcs_format_preprocess(fmt, src_fmt);

		AR_va_copy(save, args);
		res = AR_VSWPRINTF(dest, count, src_fmt, save);
		AR_va_end(save);

	   /*****************************************************************************/


END_POINT:


		if(src_fmt)
		{
				AR_DEL(src_fmt);
				src_fmt = NULL;
		}
		
#if(0)
		if(res <= 0)						/*忽略掉此种错误*/
		{
				dest[0] = L'\0';
				res = 0;
		}
#endif

		return res;
}


ar_int_t			AR_swprintf(wchar_t *dest, size_t count, const wchar_t *fmt, ...)
{
		ar_int_t len = -1;
		va_list	arg_ptr;
		AR_ASSERT(fmt != NULL);

		AR_va_start(arg_ptr, fmt);
		len = AR_vswprintf(dest, count, fmt, arg_ptr);
		AR_va_end(arg_ptr);
		
		return len;
}







ar_int_t			AR_vswprintf_nonalloc(wchar_t *dest, size_t count, const wchar_t *fmt, va_list args)
{
		ar_int_t res;
		va_list save;
		ar_int_t need_l;
		wchar_t *src_fmt;


		AR_ASSERT(dest != NULL && fmt != NULL);

		res = 0;
		need_l = 0;
		src_fmt = NULL;


		/******************** converted to the format of the target CRT***************/
		
		need_l = __wcs_format_preprocess(fmt, NULL);

		if(need_l <= 0)
		{
				dest[0] = L'\0';
				return -1;
		}
		
		src_fmt = (wchar_t*)malloc( (need_l + 1) * sizeof(wchar_t));
		if(src_fmt == NULL)
		{
				dest[0] = L'\0';
				return -1;
		}

		if(__wcs_format_preprocess(fmt, src_fmt) <= 0)
		{
				free(src_fmt);
				src_fmt = NULL;
				dest[0] = L'\0';
				return -1;
		}
		

		AR_va_copy(save, args);
		res = AR_VSWPRINTF(dest, count, src_fmt, save);
		AR_va_end(save);

		if(res < 0)
		{
				free(src_fmt);
				src_fmt = NULL;
				dest[0] = L'\0';
				return -1;
		}
	   /*****************************************************************************/
		free(src_fmt);
		src_fmt = NULL;
		return res;

}



ar_int_t			AR_swprintf_nonalloc(wchar_t *dest, size_t count, const wchar_t *fmt, ...)
{
		ar_int_t len = -1;
		va_list	arg_ptr;
		AR_ASSERT(fmt != NULL);

		AR_va_start(arg_ptr, fmt);
		len = AR_vswprintf_nonalloc(dest, count, fmt, arg_ptr);
		
		AR_va_end(arg_ptr);
		
		return len;
}



#undef	__MODIFIER_ANSI
#undef	__MODIFIER_UNICODE
#undef	__MODIFIER_INT64



AR_NAMESPACE_END

