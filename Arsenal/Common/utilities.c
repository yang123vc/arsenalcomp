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

/******************************************************Print**************************************************************/

AR_NAMESPACE_BEGIN



/*****************************************string && c literal************************************************************************/

static AR_INLINE const wchar_t* __transform_char(const wchar_t *input, wchar_t *out, arEscStrErr_t *error)
{
		const wchar_t *p;
		ar_int_t			err = AR_ESCSTR_ERR_OK;
		ar_uint_64_t		num = 0;
		AR_ASSERT(input != NULL && out != NULL);

		p = input;

		switch(*p)
		{
		case L'b':
				*out = L'\b';
				++p;
				break;
		case L'f':
				*out = L'\f';
				++p;
				break;
		case L'n':
				*out = L'\n';
				++p;
				break;
		case L'r':
				*out = L'\r';
				++p;
				break;
		case L't':
				*out = L'\t';
				++p;
				break;
		case L'v':
				*out = L'\v';
				++p;
				break;
		case L'a':
				*out = L'\a';
				++p;
				break;
		case L'\\':
				*out = L'\\';
				++p;
				break;
		case L'\"':
				*out = L'\"';
				++p;
				break;
		case L'\'':
				*out = L'\'';
				++p;
				break;
		case L'x':
		{
				if(*(p + 1) == 0)
				{
						p = NULL;
						err = AR_ESCSTR_ERR_CHAR;
						goto END_POINT;
				}else
				{
						p = AR_wtou64(p + 1, &num, 16);

						if(p == NULL || num > (ar_uint_64_t)AR_WCHARMAX)
						{
								p = NULL;
								err = AR_ESCSTR_ERR_VALUE;
								goto END_POINT;
						}

						*out = (wchar_t)num;
				}
				break;
		}
		default:
		{
				if(*p >= L'0' && *p <= L'7')
				{
						if(*p == L'0' && *(p + 1) == 0)
						{
								*out = 0;
								++p;
						}else
						{
								size_t i;
								wchar_t c = 0;

								for(i = 0; i < 3 && p[i] && p[i] >= L'0' && p[i] <= L'7'; ++i)
								{
										c *= 8;
										c += p[i] - L'0';
								}

								*out = c;
								p += i;
						}
				}else
				{
						p = NULL;
						err = AR_ESCSTR_ERR_CHAR;
						goto END_POINT;
				}
				break;
		}
		}

END_POINT:
		if(error && err != AR_ESCSTR_ERR_OK)
		{
				error->type = err;
				error->pos = input;
				error->value = num;
		}
		return p;
}


wchar_t*		AR_escstr_to_str(const wchar_t *src, arEscStrErr_t *error)
{
		wchar_t	*res;
		wchar_t *d;
		const wchar_t *s;
		size_t	src_len;
		
		AR_ASSERT(src != NULL);

		src_len = AR_wcslen(src);
		res = AR_NEWARR0(wchar_t, src_len + 1);

		if(res == NULL)
		{
				if(error)
				{
						error->pos = NULL;
						error->type = AR_ESCSTR_ERR_MEMORY;
						error->value = 0;
				}

				goto INVALID_POINT;
		}

		d = res; s = src;

		while(*s)
		{
				if(*s == L'\\')
				{
						const wchar_t *next;
						next = __transform_char(s + 1, d, error);
						if(next == NULL) 
						{
								goto INVALID_POINT;
						}
						
						s = next;
						d++;
				}else
				{
						*d++ = *s++;
				}
		}

		*d = L'\0';

		if(error)
		{
				error->pos = NULL;
				error->type = AR_ESCSTR_ERR_OK;
				error->value = 0;
		}

		return res;

INVALID_POINT:
		if(res)
		{
				AR_DEL(res);
				res = NULL;
		}
		return NULL;
}





wchar_t*		AR_str_to_escstr(const wchar_t *src)
{
		wchar_t	*res;
		wchar_t *d;
		const wchar_t *s;
		size_t	src_len;
		ar_bool_t	print_escape_char;

		if(src == NULL)
		{
				return NULL;
		}
		print_escape_char = false;  /*Consider the case of non-print characters and digital, example : \x3601234*/
		src_len = AR_wcslen(src);
		res = AR_NEWARR0(wchar_t, (src_len) * 10 + 1);

		if(res == NULL)
		{
				goto INVALID_POINT;
		}

		d = res; s = src;

		while(*s)
		{
				switch(*s)
				{
				case L'\b': 
						*d++ = L'\\';
						*d++ = L'b';
						break;
				case L'\f':
						*d++ = L'\\';
						*d++ = L'f';
						break;
				case L'\n':
						*d++ = L'\\';
						*d++ = L'n';
						break;
				case L'\r':
						*d++ = L'\\';
						*d++ = L'r';
						break;
				case L'\t':
						*d++ = L'\\';
						*d++ = L't';
						break;
				case L'\v':
						*d++ = L'\\';
						*d++ = L'v';
						break;
				case L'\a':
						*d++ = L'\\';
						*d++ = L'a';
						break;
				case L'\\': 
				case L'\"':
						*d++ = L'\\';
						*d++ = *s;
						break;
				default:
				{
						if(!print_escape_char && AR_iswprint(*s))
						{
								*d++ = *s;
						}else
						{
								ar_int_t l = 0;
								wchar_t buf[64];
								l = AR_u64tow_buf(buf, AR_NELEMS(buf), (ar_uint_64_t)(*s), 16);
								AR_ASSERT(l > 0);
								*d++ = L'\\';
								*d++ = L'x';
								for(l = 0; buf[l]; ++l)
								{
										*d++ = buf[l];
								}
								print_escape_char = true;
						}
				}
				}

				s++;
		}

		*d = L'\0';

		return res;
INVALID_POINT:
		if(res)
		{
				AR_DEL(res);
				res = NULL;
		}
		return NULL;
}



wchar_t*		AR_escstr_to_str_n(const wchar_t *src, size_t n, arEscStrErr_t *error)
{
		wchar_t	*res;
		wchar_t *d;
		size_t	i;
		const wchar_t *s;
		
		AR_ASSERT(src != NULL);

		res = AR_NEWARR0(wchar_t, n + 1);

		if(res == NULL)
		{
				if(error)
				{
						error->pos = NULL;
						error->type = AR_ESCSTR_ERR_MEMORY;
						error->value = 0;
				}

				goto INVALID_POINT;
		}

		d = res; s = src;
		
		i = 0; 
		while(i < n && s[i] != L'\0')
		{
				if(s[i] == L'\\')
				{
						const wchar_t *next;
						next = __transform_char(s + i + 1, d, error);
						if(next == NULL)
						{
								goto INVALID_POINT;
						}
						
						i = next - s;
						d++;
				}else
				{
						*d++ = s[i++];
				}
		}

		*d = L'\0';

		if(error)
		{
				error->pos = NULL;
				error->type = AR_ESCSTR_ERR_OK;
				error->value = 0;
		}

		return res;

INVALID_POINT:
		if(res)
		{
				AR_DEL(res);
				res = NULL;
		}
		return NULL;		
}



wchar_t*		AR_str_to_escstr_n(const wchar_t *src, size_t n)
{
		wchar_t	*res;
		wchar_t *d;
		const wchar_t *s;
		size_t i;
		ar_bool_t	print_escape_char;
		if(src == NULL)return NULL;
		print_escape_char = false;
		res = AR_NEWARR0(wchar_t, (n + 5) * 5 + 1);

		if(res == NULL)
		{
				goto INVALID_POINT;
		}


		d = res; s = src;
		i = 0;
		while(i < n && s[i] != L'\0')
		{
				switch(s[i])
				{
				case L'\b': 
						*d++ = L'\\';
						*d++ = L'b';
						break;
				case L'\f':
						*d++ = L'\\';
						*d++ = L'f';
						break;
				case L'\n':
						*d++ = L'\\';
						*d++ = L'n';
						break;
				case L'\r':
						*d++ = L'\\';
						*d++ = L'r';
						break;
				case L'\t':
						*d++ = L'\\';
						*d++ = L't';
						break;
				case L'\v':
						*d++ = L'\\';
						*d++ = L'v';
						break;
				case L'\a':
						*d++ = L'\\';
						*d++ = L'a';
						break;
				case L'\\': 
				case L'\"':
						*d++ = L'\\';
						*d++ = s[i];
						break;
				default:
				{
						if(!print_escape_char && AR_iswprint(s[i]))
						{
								*d++ = s[i];
						}else
						{
								ar_int_t l = 0;
								wchar_t buf[64];
								l = AR_u64tow_buf(buf, AR_NELEMS(buf), (ar_uint_64_t)(s[i]), 16);
								AR_ASSERT(l > 0);
								*d++ = L'\\';
								*d++ = L'x';
								for(l = 0; buf[l]; ++l)
								{
										*d++ = buf[l];
								}
								print_escape_char = true;
						}
				}
				}

				i++;
		}

		*d = L'\0';

		return res;
INVALID_POINT:
		if(res)
		{
				AR_DEL(res);
				res = NULL;
		}
		return NULL;
}







ar_int_t 		AR_escstr_to_str_buf(wchar_t *dest, size_t len, const wchar_t *src, arEscStrErr_t *error)
{
		size_t ret;
		wchar_t *str;
		AR_ASSERT(src != NULL);

		str = AR_escstr_to_str(src, error);

		if(str == NULL)
		{
				return -1;
		}

		ret = AR_wcslen(str);

		if(dest == NULL)
		{
				AR_DEL(str);
				return (ar_int_t)ret + 1;
		}else
		{
				if(len <= ret) 
				{ 
						AR_DEL(str);
                        if(error)
                        {
                                error->type = AR_ESCSTR_ERR_BUFFER;
                        }
						return -1;
				}else
				{
						AR_wcsncpy(dest, str, ret);
						dest[ret] = L'\0';
						AR_DEL(str);
						return (ar_int_t)ret + 1;
				}
		}
}


ar_int_t 		AR_str_to_escstr_buf(wchar_t *dest, size_t len, const wchar_t *src)
{
		size_t ret;
		wchar_t *str;
		AR_ASSERT(src != NULL);

		str = AR_str_to_escstr(src);
		
		if(str == NULL)
		{
				return -1;
		}

		ret = AR_wcslen(str);

		if(dest == NULL)
		{
				AR_DEL(str);
				return (ar_int_t)ret + 1;
		}else
		{
				if(len <= ret) 
				{ 
						AR_DEL(str); 
						return -1;
				}

				AR_wcsncpy(dest, str, ret);
				dest[ret] = L'\0';

				AR_DEL(str);
				return (ar_int_t)ret + 1;
		}
}


ar_int_t 			AR_escstr_to_str_buf_n(wchar_t *dest, size_t len, const wchar_t *src, size_t n,arEscStrErr_t *error)
{
		size_t ret;
		wchar_t *str;
		AR_ASSERT(src != NULL);

		str = AR_escstr_to_str_n(src, n, error);
		
		if(str == NULL)
		{
				return -1;
		}

		ret = AR_wcslen(str);

		if(dest == NULL)
		{
				AR_DEL(str);
				return (ar_int_t)ret + 1;
		}else
		{
				if(len <= ret) 
				{
						AR_DEL(str);
                        if(error)
                        {
                                error->type = AR_ESCSTR_ERR_BUFFER;
                        }
						return -1;
				}else
				{
						AR_wcsncpy(dest, str, ret);
						dest[ret] = L'\0';
						AR_DEL(str);
						return (ar_int_t)ret + 1;
				}
		}
}


ar_int_t 			AR_str_to_escstr_buf_n(wchar_t *dest, size_t len, const wchar_t *src, size_t n)
{
		size_t ret;
		wchar_t *str;
		AR_ASSERT(src != NULL);

		str = AR_str_to_escstr_n(src, n);
		
		if(str == NULL)
		{
				return -1;
		}

		ret = AR_wcslen(str);

		if(dest == NULL)
		{
				AR_DEL(str);
				return (ar_int_t)ret + 1;
		}else
		{
				if(len <= ret) 
				{ 
						AR_DEL(str); 
						return -1;
				}

				AR_wcsncpy(dest, str, ret);
				dest[ret] = L'\0';

				AR_DEL(str);
				return (ar_int_t)ret + 1;
		}
}

/********************************************************************************************************************/


static arStatus_t  __hex_char_to_digit(char c, ar_byte_t *d)
{
		AR_ASSERT(d != NULL);
        
		if(c >= '0' && c <= '9')
		{
				*d = c - '0';
		}else if(c >= 'a' && c <= 'f')
		{
				*d = 10 + c - 'a';
		}else if(c >= 'A' && c <= 'F')
		{
				*d = 10 + c - 'A';
		}else
		{
				return AR_E_INVAL;
		}
		return AR_S_YES;
}

ar_int_t           AR_hexstr_to_data(const char *s, ar_byte_t *data, size_t len)
{
        AR_ASSERT(s != NULL);
        return AR_hexstr_to_data_s(s, s + AR_strlen(s), data, len);
}


ar_int_t	AR_hexstr_to_data_s(const char *b, const char *e, ar_byte_t *data, size_t len)
{
		ar_byte_t *p;
		arStatus_t status;
		size_t need_l;
		AR_ASSERT(b != NULL && e != NULL && b < e);
		
		need_l = e - b;
        
		if(need_l % 2 != 0)
		{
				return -1;
		}
        
		if(data == NULL)
		{
				return (ar_int_t)need_l / 2;
		}
        
		if(len < need_l / 2)
		{
				return -1;
		}
		
		p = data;
		while(b < e)
		{
				ar_byte_t d1,d2;
				
				status = __hex_char_to_digit(*b, &d1);
                
				if(status != AR_S_YES)
				{
						return -1;
				}
                
				b++;
				AR_ASSERT(b < e);
                
				status = __hex_char_to_digit(*b, &d2);
                
				if(status != AR_S_YES)
				{
						return -1;
				}
                
				*p++ = d1 * 16 + d2;
				++b;
		}
		
		return need_l / 2;
}




static void __digit_to_hex_char(ar_byte_t b, char tmp[2], ar_bool_t is_upper)
{
		ar_uint_32_t v = (ar_uint_32_t)b;
		static const char *__tbl =NULL;
        
        if(is_upper)
        {
                __tbl = "0123456789ABCDEF";
        }else
        {
                __tbl = "0123456789abcdef";
        }
        
		tmp[1] = __tbl[v % 16];
		v /= 16;
		tmp[0] = __tbl[v % 16];
}

ar_int_t           AR_data_to_hexstr(const ar_byte_t *data, size_t l, char *out, size_t len, ar_bool_t is_upper)
{
		size_t i, si;
		AR_ASSERT(data != NULL && l > 0);
        
		if(out == NULL)
		{
				return l * 2;
		}else
		{
				if(len < l * 2)
				{
						return -1;
				}
                
				for(i = 0,si = 0; i < l; ++i)
				{
						__digit_to_hex_char(data[i], out + si, is_upper);
						si += 2;
                        //AR_ASSERT(si < len);
				}
				return si;
		}
}





/****************************************************************/


static arStatus_t  __hex_wchar_t_to_digit(wchar_t c, ar_byte_t *d)
{
		AR_ASSERT(d != NULL);
        
		if(c >= L'0' && c <= L'9')
		{
				*d = (ar_byte_t)(c - L'0');
		}else if(c >= L'a' && c <= L'f')
		{
				*d = (ar_byte_t)(10 + c - L'a');
		}else if(c >= L'A' && c <= L'F')
		{
				*d = (ar_byte_t)(10 + c - L'A');
		}else
		{
				return AR_E_INVAL;
		}
		return AR_S_YES;
}


ar_int_t           AR_hexwcs_to_data(const wchar_t *s, ar_byte_t *data, size_t len)
{
        AR_ASSERT(s != NULL);
        return AR_hexwcs_to_data_s(s, s + AR_wcslen(s), data, len);
}



ar_int_t	AR_hexwcs_to_data_s(const wchar_t *b, const wchar_t *e, ar_byte_t *data, size_t len)
{
		ar_byte_t *p;
		arStatus_t status;
		size_t need_l;
		AR_ASSERT(b != NULL && e != NULL && b < e);
		
		need_l = e - b;
        
		if(need_l % 2 != 0)
		{
				return -1;
		}
        
		if(data == NULL)
		{
				return (ar_int_t)need_l / 2;
		}
        
		if(len < need_l / 2)
		{
				return -1;
		}
		
		p = data;
		while(b < e)
		{
				ar_byte_t d1,d2;
				
				status = __hex_wchar_t_to_digit(*b, &d1);
                
				if(status != AR_S_YES)
				{
						return -1;
				}
                
				b++;
				AR_ASSERT(b < e);
                
				status = __hex_wchar_t_to_digit(*b, &d2);
                
				if(status != AR_S_YES)
				{
						return -1;
				}
                
				*p++ = d1 * 16 + d2;
				++b;
		}
		
		return need_l / 2;
}




static void __digit_to_hex_wchar_t(ar_byte_t b, wchar_t tmp[2], ar_bool_t is_upper)
{
		ar_uint_32_t v = (ar_uint_32_t)b;
		static const wchar_t *__tbl =NULL;
        
        if(is_upper)
        {
                __tbl = L"0123456789ABCDEF";
        }else
        {
                __tbl = L"0123456789abcdef";
        }
        
		tmp[1] = __tbl[v % 16];
		v /= 16;
		tmp[0] = __tbl[v % 16];
}


ar_int_t           AR_data_to_hexwcs(const ar_byte_t *data, size_t l, wchar_t *out, size_t len, ar_bool_t is_upper)
{
		size_t i, si;
		AR_ASSERT(data != NULL && l > 0);
        
		if(out == NULL)
		{
				return l * 2;
		}else
		{
				if(len < l * 2)
				{
						return -1;
				}
                
				for(i = 0,si = 0; i < l; ++i)
				{
						__digit_to_hex_wchar_t(data[i], out + si, is_upper);
						si += 2;
                        //AR_ASSERT(si < len);
				}
				return si;
		}
}


AR_NAMESPACE_END


