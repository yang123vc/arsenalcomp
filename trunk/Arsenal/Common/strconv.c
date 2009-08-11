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


static size_t __utf8_to_unicode_char(const byte_t *utf8, size_t len, wchar_t *uch)
{
		size_t v,n,e;
		const byte_t *p;
		
		AR_ASSERT(utf8 != NULL && len > 0);
		
		v = (size_t)(*utf8); n = 0; e= 0; p = utf8;
		
		if(v >= 0xfc)
		{
				n = 6;/*6:<11111100>*/
				if(n > len)return 0;
				e = (p[0] & 0x01) << 30;
				e |= (p[1] & 0x3f) << 24;
				e |= (p[2] & 0x3f) << 18;
				e |= (p[3] & 0x3f) << 12;
				e |= (p[4] & 0x3f) << 6;
				e |= (p[5] & 0x3f);
		}else if(v >= 0xf8)
		{
				n = 5;/*5:<11111000>*/
				if(n > len)return 0;
				 e = (p[0] & 0x03) << 24;
				e |= (p[1] & 0x3f) << 18;
				e |= (p[2] & 0x3f) << 12;
				e |= (p[3] & 0x3f) << 6;
				e |= (p[4] & 0x3f);
		
		}else if(v >= 0xf0)
		{
				n = 4; /*4:<11110000>*/
				if(n > len)return 0;
				e = (p[0] & 0x07) << 18;
				e |= (p[1] & 0x3f) << 12;
				e |= (p[2] & 0x3f) << 6;
				e |= (p[3] & 0x3f);
		}else if(v >= 0xe0)
		{
				n = 3;/*3:<11100000>*/
				if(n > len)return 0;
				e = (p[0] & 0x0f) << 12;
				e |= (p[1] & 0x3f) << 6;
				e |= (p[2] & 0x3f);

		}else if(v >= 0xc0)
		{
				n = 2;/*3:<11000000>*/
				if(n > len)return 0;
				e = (p[0] & 0x1f) << 6;
				e |= (p[1] & 0x3f);
		}else
		{
				n = 1;
				if(n > len)return 0;
				e = p[0];
		}
		if(uch)*uch = (wchar_t)e;
		return n;
}


static size_t __unicode_to_utf8_char(wchar_t uch, byte_t *utf8)
{
		byte_t buf[10];
		byte_t *e;
		uint32_t	uc = (uint32_t)uch;
		
		e = (utf8 ? utf8 : buf);
    
		if(uc < 0x80)
		{
				*e++ = uc;
		}
		else if(uc < 0x800)
		{
				/*<11011111> < 000 0000 0000>*/
				*e++ = ((uc >> 6) & 0x1f)|0xc0;
				*e++ = (uc & 0x3f)|0x80; 
		}
		else if(uc < 0x10000)
		{
				/*<11101111> <0000 0000 0000 0000>*/
				*e++ = ((uc >> 12) & 0x0f)|0xe0; 
				*e++ = ((uc >> 6) & 0x3f)|0x80;
				*e++ = (uc & 0x3f)|0x80; 
		}
		else if(uc < 0x200000)
		{
				/*<11110111> <0 0000 0000 0000 0000 0000>*/
				*e++ = ((uc >> 18) & 0x07)|0xf0; 
				*e++ = ((uc >> 12) & 0x3f)|0x80;
				*e++ = ((uc >> 6) & 0x3f)|0x80;
				*e++ = (uc & 0x3f)|0x80; 
		}
		else if(uc < 0x4000000)
		{
				/*<11111011> <00 0000 0000 0000 0000 0000 0000>*/
				*e++ = ((uc >> 24) & 0x03)|0xf8 ; 
				*e++ = ((uc >> 18) & 0x3f)|0x80;
				*e++ = ((uc >> 12) & 0x3f)|0x80;
				*e++ = ((uc >> 6) & 0x3f)|0x80;
				*e++ = (uc & 0x3f)|0x80; 
		}
		else
		{
				/*<11111101> <0000 0000 0000 0000 0000 0000 0000 0000>*/
				*e++ = ((uc >> 30) & 0x01)|0xfc; 
				*e++ = ((uc >> 24) & 0x3f)|0x80;
				*e++ = ((uc >> 18) & 0x3f)|0x80;
				*e++ = ((uc >> 12) & 0x3f)|0x80;
				*e++ = ((uc >> 6) & 0x3f)|0x80;
				*e++ = (uc & 0x3f)|0x80; 
		}

		return utf8 ? e - utf8 : e - buf;
}


size_t AR_wcs_to_utf8(const wchar_t *unicode, size_t n, char *out, size_t out_len)
{
		char *p;
		size_t i, need;
		AR_ASSERT(unicode != NULL && n > 0);
		for(i = 0,need = 0; i < n; ++i)need += __unicode_to_utf8_char(unicode[i], NULL);

		if(out != NULL)
		{
				p = out;
				if(out_len < need)return 0;
				for(i = 0; i < n; ++i)p += __unicode_to_utf8_char(unicode[i], (byte_t*)p);
		}
		return need;
}

size_t AR_utf8_to_wcs(const char *utf8, size_t n, wchar_t *out, size_t out_len)
{
		const char *p;
		size_t need; int_t l;
		AR_ASSERT(utf8 != NULL && n > 0);
		p = utf8; need = 0; l = (int_t)n;
		
		while(l > 0)
		{
				size_t nc = __utf8_to_unicode_char((const byte_t*)p, (size_t)l, NULL);
				if(nc == 0)return 0;
				need++;
				p += nc;
				l -= nc;
		}
		
		if(out != NULL)
		{
				if(out_len < need)return 0;
				
				l = n; p = utf8; need = 0;
				while(l > 0)
				{
						size_t nc = __utf8_to_unicode_char((const byte_t*)p, (size_t)l, &out[need]);
						need++;
						l -= nc;
						p += nc;
				}
		}

		return need;
}


wchar_t* AR_utf8_convto_wcs(const char *utf8)
{
		wchar_t *buf;
		size_t need;
		size_t in_len;
		
		in_len = AR_strlen(utf8);
		if(in_len == 0)return NULL;

		need = AR_utf8_to_wcs(utf8, in_len, NULL, 0);

		if(need == 0)return NULL;/*输入有问题*/
		
		buf = AR_NEWARR(wchar_t, need + 1); buf[need] = L'\0';
		AR_utf8_to_wcs(utf8, in_len, buf, need);
		return buf;
}

char*  AR_wcs_convto_utf8(const wchar_t *wcs)
{
		char *buf; size_t need; size_t in_len;

		in_len = AR_wcslen(wcs); if(in_len == 0)return NULL;

		need = AR_wcs_to_utf8(wcs,in_len, NULL, 0);

		if(need == 0)return NULL;/*输入有问题*/

		buf = AR_NEWARR(char, need + 1); buf[need] = '\0';

		AR_wcs_to_utf8(wcs,in_len, buf, need);
		return buf;
}




AR_NAMESPACE_END

