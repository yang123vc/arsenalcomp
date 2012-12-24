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


/**********************************************************************upper && lower********************************************/

char*			AR_strupr(char *s)
{
		char *r = s;
		AR_ASSERT(s != NULL);
		while(*s)
		{
				*s = (char)AR_toupper(*s);
				++s;
		}
		return r;

}

char*			AR_strlwr(char *s)
{
		char *r = s;
		AR_ASSERT(s != NULL);
		while(*s)
		{
				*s = (char)AR_tolower(*s);
				++s;
		}
		return r;
}

char*	AR_strnlwr(char *s, size_t count)
{
		size_t i;
		AR_ASSERT(s != NULL);

		for(i = 0; i < count && s[i] != '\0'; ++i)
		{
				s[i] = (char)AR_tolower(s[i]);
		}

		return s;
}




char*	AR_strnupr(char *s, size_t count)
{
		size_t i;
		AR_ASSERT(s != NULL);

		for(i = 0; i < count && s[i] != '\0'; ++i)
		{
				s[i] = (char)AR_toupper(s[i]);
		}

		return s;
}


wchar_t*			AR_wcsupr(wchar_t *s)
{
		wchar_t *r = s;
		AR_ASSERT(s != NULL);
		while(*s)
		{
				*s = AR_towupper(*s);
				++s;
		}
		return r;
}

wchar_t*			AR_wcslwr(wchar_t *s)
{
		wchar_t *r = s;
		AR_ASSERT(s != NULL);
		while(*s)
		{
				*s = AR_towlower(*s);
				s++;
		}
		return r;
}


wchar_t* AR_wcsnlwr(wchar_t *s, size_t count)
{
		size_t i;
		AR_ASSERT(s != NULL);

		for(i = 0; i < count && s[i] != L'\0'; ++i)
		{
				s[i] = AR_towlower(s[i]);
		}

		return s;
}




wchar_t* AR_wcsnupr(wchar_t *s, size_t count)
{
		size_t i;
		AR_ASSERT(s != NULL);

		for(i = 0; i < count && s[i] != L'\0'; ++i)
		{
				s[i] = AR_towupper(s[i]);
		}

		return s;
}


/***********************************************************************compare***************************************************/


ar_int_t	AR_stricmp(const char *l, const char *r)
{
		ar_int_t ret;
		AR_ASSERT(l != NULL && r != NULL);

		for(ret = 0; (ret = (AR_tolower(*l) - AR_tolower(*r))) == 0 && *l && *r; ++l, ++r);

		return ret;
}

ar_int_t	AR_strnicmp(const char *l, const char *r, size_t n)
{
		ar_int_t ret;
		size_t i;
		AR_ASSERT(l != NULL && r != NULL);

		for(ret = 0, i = 0; i < n && (ret = (AR_tolower(l[i]) - AR_tolower(r[i]))) == 0 && l[i] && r[i]; ++i);

		return ret;

}

ar_int_t	AR_wcsicmp(const wchar_t *l, const wchar_t *r)
{
		ar_int_t ret;
		AR_ASSERT(l != NULL && r != NULL);

		for(ret = 0; (ret = (AR_towlower(*l) - AR_towlower(*r))) == 0 && *l && *r; ++l, ++r);

		return ret;

}

ar_int_t	AR_wcsnicmp(const wchar_t *l, const wchar_t *r, size_t n)
{
		ar_int_t ret;
		size_t i;
		AR_ASSERT(l != NULL && r != NULL);

		for(ret = 0, i = 0; i < n && (ret = (AR_towlower(l[i]) - AR_towlower(r[i]))) == 0 && l[i] && r[i]; ++i);

		return ret;

}








/**************************************************************************concatenate**********************************************************/


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

char*			AR_strcat(char *dest, const char *sour)
{
		char *d;
		AR_ASSERT(dest != NULL && sour != NULL);
		d = dest;
		while(*d)d++;

		while(*sour)*d++ = *sour++;
		*d = 0;
		return dest;
}

char*			AR_strncat(char *dest, const char *sour, size_t n)
{
		char *d;
		size_t i;
		AR_ASSERT(dest != NULL && sour != NULL);
		d = dest;
		while(*d)d++;

		for(i = 0; i < n && sour[i]; ++i,++d) *d = sour[i];

		*d = 0;

		return dest;
}






/*************************************************************************copy*****************************************/




wchar_t* AR_wcsdup(const wchar_t *sour)
{
		AR_ASSERT(sour != NULL);
		return AR_wcsndup(sour, AR_wcslen(sour));
}




wchar_t* AR_wcsndup(const wchar_t *sour, size_t len)
{
		wchar_t *result;
		size_t i;
		AR_ASSERT(sour != NULL);

		result = AR_NEWARR(wchar_t, len + 1);
		
		if(result == NULL)
		{
				return NULL;
		}

		for(i = 0; i < len; ++i)result[i] = sour[i];
		result[len] = L'\0';
		return result;
}


char*			AR_strdup(const char *sour)
{
		AR_ASSERT(sour != NULL);
		return AR_strndup(sour, AR_strlen(sour));
}

char*			AR_strndup(const char *sour, size_t len)
{
		char *result;
		size_t i;
		AR_ASSERT(sour != NULL);

		result = AR_NEWARR(char, len + 1);

		if(result == NULL)
		{
				return NULL;
		}

		for(i = 0; i < len; ++i)result[i] = sour[i];
		result[len] = '\0';
		return result;
}







/**********************************************trim***************************************/



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

wchar_t*	AR_wcstrim_right_space(wchar_t *in)
{
		wchar_t *p = NULL, *plast = NULL;
		AR_ASSERT(in != NULL);
		p = in;
		while(*p != L'\0')
		{
				/*if(AR_wcschr(trim, *p) == NULL)*/
				if(AR_iswspace(*p) == 0)
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





const char* AR_strtrim_s(const char *in, const char *end, const char *trim)
{
		AR_ASSERT(in != NULL && end != NULL && trim != NULL);
		while(in < end && AR_strchr(trim, *in) != NULL)in++;
		return in;
}




const char*	AR_strtrim_space_s(const char *in, const char *end)
{
		AR_ASSERT(in != NULL && end != NULL);
		while(in < end && AR_isspace(*in))in++;
		return in;
}


const char* AR_strtrim(const char *in, const char *trim)
{
		AR_ASSERT(in != NULL && trim != NULL);
		while(*in != '\0' && AR_strchr(trim, *in) != NULL)in++;
		return in;
}


const char*	AR_strtrim_space(const char *in)
{
		AR_ASSERT(in != NULL);
		while(*in != '\0' && AR_isspace(*in))in++;
		return in;
}



char*		AR_strtrim_right(char *in, const char *trim)
{
		char *p = NULL, *plast = NULL;
		AR_ASSERT(in != NULL && trim != NULL);
		p = in;
		while(*p != '\0')
		{
				if(AR_strchr(trim, *p) == NULL)
				{
						plast = NULL;
				}else if(plast == NULL)
				{
						plast = p;
				}
				++p;
		}

		if(plast)*plast = '\0';
		return in;
}

char*	AR_strtrim_right_space(char *in)
{
		char *p = NULL, *plast = NULL;
		AR_ASSERT(in != NULL);
		p = in;
		while(*p != '\0')
		{
				/*if(AR_wcschr(trim, *p) == NULL)*/
				if(AR_isspace(*p) == 0)
				{
						plast = NULL;
				}else if(plast == NULL)
				{
						plast = p;
				}
				++p;
		}

		if(plast)*plast = '\0';
		return in;
}




/**************************************************************string search*******************************************/

#if(0)
const char*		AR_strstr_s(const char *s, const char *se, const char *p, const char *pe)
{
		size_t i;
		
		AR_ASSERT(s != NULL && se != NULL && s <= se);
		AR_ASSERT(p != NULL && pe != NULL && p <= pe);

		if(p == pe || *p == '\0')
		{
				return s;
		}

		while(s < se)
		{
				for(i = 0; s < se && s[i] && p < pe && p[i] && s[i] == p[i]; ++i);
				
				if(p + i >= pe || p[i] == '\0')
				{
						return s;
				}
				++s;
		}
		
		return NULL;
}

const wchar_t*	AR_wcsstr_s(const wchar_t *s, const wchar_t *se, const wchar_t *p, const wchar_t *pe)
{
		size_t i;
		
		AR_ASSERT(s != NULL && se != NULL && s <= se);
		AR_ASSERT(p != NULL && pe != NULL && p <= pe);

		if(p == pe || *p == L'\0')
		{
				return s;
		}

		while(s < se)
		{
				for(i = 0; s < se && s[i] && p < pe && p[i] && s[i] == p[i]; ++i);

				if(p + i >= pe || p[i] == L'\0')
				{
						return s;
				}
				++s;
		}
		
		return NULL;
}

const char*		AR_stristr_s(const char *s, const char *se, const char *p, const char *pe)
{
		size_t i;
		
		AR_ASSERT(s != NULL && se != NULL && s <= se);
		AR_ASSERT(p != NULL && pe != NULL && p <= pe);

		if(p == pe || *p == '\0')
		{
				return s;
		}

		while(s < se)
		{
				for(i = 0; s < se && s[i] && p < pe && p[i] && AR_tolower(s[i]) == AR_tolower(p[i]); ++i);
				if(p + i >= pe || p[i] == '\0')
				{
						return s;
				}
				++s;
		}
		
		return NULL;
}

const wchar_t*	AR_wcsistr_s(const wchar_t *s, const wchar_t *se, const wchar_t *p, const wchar_t *pe)
{
		size_t i;
		
		AR_ASSERT(s != NULL && se != NULL && s <= se);
		AR_ASSERT(p != NULL && pe != NULL && p <= pe);

		if(p == pe || *p == L'\0')
		{
				return s;
		}

		while(s < se)
		{
				for(i = 0; s < se && s[i] && p < pe && p[i] && AR_tolower(s[i]) == AR_tolower(p[i]); ++i);

				if(p + i >= pe || p[i] == L'\0')
				{
						return s;
				}
				++s;
		}
		
		return NULL;
}

#endif



const char*		AR_strstr_s(const char *s, const char *se, const char *p, const char *pe)
{
		size_t i, l, pl;
		AR_ASSERT(s != NULL && se != NULL && s <= se);
		AR_ASSERT(p != NULL && pe != NULL && p <= pe);
		
		l = se - s;
		pl = pe - p;

		if (pl > l)
		{
				return NULL;
		}

		if(pl == 0 || *p == '\0')
		{
				return s;
		}


		
		if(AR_UINT64_MAX / AR_UCHARMAX > pl)
		{
				ar_uint_64_t search_hash, pattern_hash;
				search_hash = 0;
				pattern_hash = 0;

				for(i = 0; i < pl; ++i)
				{
						search_hash += (ar_uint_64_t)s[i];
						pattern_hash += (ar_uint_64_t)p[i];

				}

				i = 0;

				while(search_hash != pattern_hash || AR_strncmp(s + i, p, pl) != 0)
				{
						if(l - (i + 1) < pl)
						{
								return NULL;
						}

						search_hash -= (ar_uint_64_t)s[i];
						search_hash += (ar_uint_64_t)s[i + pl];
						i++;
				}

				return s + i;
		}else
		{
				i = 0;

				while(AR_strncmp(s + i, p, pl) != 0)
				{
						if(l - (i + 1) < pl)
						{
								return NULL;
						}

						i++;
				}
				return s + i;
		}
}


const wchar_t*	AR_wcsstr_s(const wchar_t *s, const wchar_t *se, const wchar_t *p, const wchar_t *pe)
{
		size_t i, l, pl;
		AR_ASSERT(s != NULL && se != NULL && s <= se);
		AR_ASSERT(p != NULL && pe != NULL && p <= pe);
		
		l = se - s;
		pl = pe - p;
		
		if (pl > l)
		{
				return NULL;
		}

		if(pl == 0 || *p == L'\0')
		{
				return s;
		}
		
		if(AR_UINT64_MAX / AR_WCHARMAX > pl)
		{
				ar_uint_64_t search_hash, pattern_hash;
				search_hash = 0;
				pattern_hash = 0;

				for(i = 0; i < pl; ++i)
				{
						search_hash += (ar_uint_64_t)s[i];
						pattern_hash += (ar_uint_64_t)p[i];

				}

				i = 0;

				while(search_hash != pattern_hash || AR_wcsncmp(s + i, p, pl) != 0)
				{
						if(l - (i + 1) < pl)
						{
								return NULL;
						}

						search_hash -= (ar_uint_64_t)s[i];
						search_hash += (ar_uint_64_t)s[i + pl];
						i++;
				}

				return s + i;
		}else
		{
				i = 0;

				while(AR_wcsncmp(s + i, p, pl) != 0)
				{
						if(l - (i + 1) < pl)
						{
								return NULL;
						}

						i++;
				}

				return s + i;
		}
}




const char*		AR_stristr_s(const char *s, const char *se, const char *p, const char *pe)
{
		size_t i, l, pl;
		AR_ASSERT(s != NULL && se != NULL && s <= se);
		AR_ASSERT(p != NULL && pe != NULL && p <= pe);
		
		l = se - s;
		pl = pe - p;

		if (pl > l)
		{
				return NULL;
		}

		if(pl == 0 || *p == '\0')
		{
				return s;
		}
		
		if(AR_UINT64_MAX / AR_UCHARMAX > pl)
		{
				ar_uint_64_t search_hash, pattern_hash;

				search_hash = 0;
				pattern_hash = 0;

				for(i = 0; i < pl; ++i)
				{
						search_hash += (ar_uint_64_t)AR_tolower(s[i]);
						pattern_hash += (ar_uint_64_t)AR_tolower(p[i]);

				}

				i = 0;

				while(search_hash != pattern_hash || AR_strnicmp(s + i, p, pl) != 0)
				{
						if(l - (i + 1) < pl)
						{
								return NULL;
						}

						search_hash -= (ar_uint_64_t)AR_tolower(s[i]);
						search_hash += (ar_uint_64_t)AR_tolower(s[i + pl]);
						i++;
				}

				return s + i;
		}else
		{
				i = 0;

				while(AR_strnicmp(s + i, p, pl) != 0)
				{
						if(l - (i + 1) < pl)
						{
								return NULL;
						}
						i++;
				}

				return s + i;
		}
}


const wchar_t*	AR_wcsistr_s(const wchar_t *s, const wchar_t *se, const wchar_t *p, const wchar_t *pe)
{
		size_t i, l, pl;
		AR_ASSERT(s != NULL && se != NULL && s <= se);
		AR_ASSERT(p != NULL && pe != NULL && p <= pe);
		
		l = se - s;
		pl = pe - p;
		
		if (pl > l)
		{
				return NULL;
		}

		if(pl == 0 || *p == L'\0')
		{
				return s;
		}
		
        
		if(AR_UINT64_MAX / AR_WCHARMAX > pl)
		{
				ar_uint_64_t search_hash, pattern_hash;

				search_hash = 0;
				pattern_hash = 0;

				for(i = 0; i < pl; ++i)
				{
						search_hash += (ar_uint_64_t)AR_towlower(s[i]);
						pattern_hash += (ar_uint_64_t)AR_towlower(p[i]);

				}

				i = 0;

				while(search_hash != pattern_hash || AR_wcsnicmp(s + i, p, pl) != 0)
				{
						if(l - (i + 1) < pl)
						{
								return NULL;
						}

						search_hash -= (ar_uint_64_t)AR_towlower(s[i]);
						search_hash += (ar_uint_64_t)AR_towlower(s[i + pl]);
						i++;
				}

				return s + i;
		}else
		{
				i = 0;

				while(AR_wcsnicmp(s + i, p, pl) != 0)
				{
						if(l - (i + 1) < pl)
						{
								return NULL;
						}

						i++;
				}

				return s + i;
		}
}







const char*		AR_stristr(const char *s, const char *p)
{
		size_t i;
		AR_ASSERT(s != NULL && p != NULL);

		if(*p == '\0')
		{
				return s;
		}

		while(*s)
		{
				for(i = 0; s[i] && p[i] && AR_tolower(s[i]) == AR_tolower(p[i]); ++i);
				if(p[i] == '\0')return s;
				++s;
		}
		return NULL;

}

const wchar_t*		AR_wcsistr(const wchar_t *s, const wchar_t *p)
{
		size_t i;
		AR_ASSERT(s != NULL && p != NULL);

		if(*p == L'\0')
		{
				return s;
		}

		while(*s)
		{
				for(i = 0; s[i] && p[i] && AR_towlower(s[i]) == AR_towlower(p[i]); ++i);
				if(p[i] == L'\0')return s;
				++s;
		}
		return NULL;
}



//从右到左查找

const wchar_t* AR_reverse_wcschr(const wchar_t* str, size_t l, wchar_t c)
{
		size_t idx;

		AR_ASSERT(str != NULL);

		if(l == 0)
		{
				return str + l;
		}

		idx = l - 1;
		while(str[idx] != c)
		{
				if(idx == 0)
				{
						break;
				}

				idx--;
		}

		if(idx == 0 && str[0] != c)
		{
				return NULL;
		}else
		{
				return str + idx;
		}
}






const wchar_t* AR_reverse_wcsstr(const wchar_t *str, size_t l,  const wchar_t *match, size_t ml)
{
		size_t			delta;
		
		size_t i;
		AR_ASSERT(str != NULL && match != NULL);

		if (ml > l)
		{
				return NULL;
		}

		if(ml == 0)
		{
				return str + l;
		}

		if(ml == 1)
		{
				return AR_reverse_wcschr(str, l, match[0]);
		}

		delta = l - ml;

		if(AR_UINT64_MAX / AR_WCHARMAX > ml)
		{
				ar_uint_64_t		search_hash, match_hash;
				search_hash = 0;
				match_hash = 0;

				for(i = 0; i < ml; ++i)
				{
						search_hash += (ar_uint_64_t)str[delta + i];
						match_hash += (ar_uint_64_t)match[i];
				}


				while(search_hash != match_hash || AR_wcsncmp(str + delta, match, ml) != 0)
				{
						if(delta == 0)
						{
								return NULL;
						}

						delta--;
						search_hash -= (ar_uint_64_t)str[delta + ml];
						search_hash += (ar_uint_64_t)str[delta];
				}

				return str + delta;
		}else
		{
				while(AR_wcsncmp(str + delta, match, ml) != 0)
				{
						if(delta == 0)
						{
								return NULL;
						}
						
						delta--;
				}

				return str + delta;
		}
}



const wchar_t* AR_reverse_wcsichr(const wchar_t* str, size_t l, wchar_t c)
{
		size_t idx;
		wchar_t low_c;
		AR_ASSERT(str != NULL);

		if(l == 0)
		{
				return str + l;
		}

		low_c = AR_towlower(c);
		idx = l - 1;

		while(AR_towlower(str[idx]) != low_c)
		{
				if(idx == 0)
				{
						break;
				}

				idx--;
		}

		if(idx == 0 && AR_towlower(str[0]) != c)
		{
				return NULL;
		}else
		{
				return str + idx;
		}
}


const wchar_t* AR_reverse_wcsistr(const wchar_t *str, size_t l,  const wchar_t *match, size_t ml)
{
		size_t			delta;
		size_t i;
		AR_ASSERT(str != NULL && match != NULL);

		if (ml > l)
		{
				return NULL;
		}

		if(ml == 0)
		{
				return str + l;
		}

		if(ml == 1)
		{
				return AR_reverse_wcsichr(str, l, match[0]);
		}


		delta = l - ml;


		if(AR_UINT64_MAX / AR_WCHARMAX > ml)
		{
				ar_uint_64_t		search_hash, match_hash;
				search_hash = 0;
				match_hash = 0;

				for(i = 0; i < ml; ++i)
				{
						search_hash += (ar_uint_64_t)AR_towlower(str[delta + i]);
						match_hash += (ar_uint_64_t)AR_towlower(match[i]);
				}


				while(search_hash != match_hash || AR_wcsnicmp(str + delta, match, ml) != 0)
				{
						if(delta == 0)
						{
								return NULL;
						}

						delta--;
						search_hash -= (ar_uint_64_t)AR_towlower(str[delta + ml]);
						search_hash += (ar_uint_64_t)AR_towlower(str[delta]);
				}

				return str + delta;
		}else
		{
				while(AR_wcsnicmp(str + delta, match, ml) != 0)
				{
						if(delta == 0)
						{
								return NULL;
						}

						delta--;
				}
				return str + delta;
		}
}



const char* AR_reverse_strchr(const char* str, size_t l, char c)
{
		size_t idx;

		AR_ASSERT(str != NULL);

		if(l == 0)
		{
				return str + l;
		}

		idx = l - 1;
		while(str[idx] != c)
		{
				if(idx == 0)
				{
						break;
				}

				idx--;
		}

		if(idx == 0 && str[0] != c)
		{
				return NULL;
		}else
		{
				return str + idx;
		}
}

const char* AR_reverse_strstr(const char *str, size_t l,  const char *match, size_t ml)
{
		size_t			delta;
		size_t i;
		AR_ASSERT(str != NULL && match != NULL);

		if (ml > l)
		{
				return NULL;
		}

		if(ml == 0)
		{
				return str + l;
		}

		if(ml == 1)
		{
				return AR_reverse_strchr(str, l, match[0]);
		}


		delta = l - ml;

		if(AR_UINT64_MAX / AR_UCHARMAX > ml)
		{
				ar_uint_64_t		search_hash, match_hash;
				search_hash = 0;
				match_hash = 0;

				for(i = 0; i < ml; ++i)
				{
						search_hash += (ar_uint_64_t)str[delta + i];
						match_hash += (ar_uint_64_t)match[i];
				}


				while(search_hash != match_hash || AR_strncmp(str + delta, match, ml) != 0)
				{
						if(delta == 0)
						{
								return NULL;
						}

						delta--;
						search_hash -= (ar_uint_64_t)str[delta + ml];
						search_hash += (ar_uint_64_t)str[delta];
				}

				return str + delta;
		}else
		{
				while(AR_strncmp(str + delta, match, ml) != 0)
				{
						if(delta == 0)
						{
								return NULL;
						}

						delta--;
				}

				return str + delta;
		}
}



const char* AR_reverse_strichr(const char* str, size_t l, char c)
{
		size_t idx;
		char low_c;
		AR_ASSERT(str != NULL);

		if(l == 0)
		{
				return str + l;
		}

		low_c = (char)AR_tolower(c);
		idx = l - 1;

		while(AR_tolower(str[idx]) != low_c)
		{
				if(idx == 0)
				{
						break;
				}

				idx--;
		}

		if(idx == 0 && AR_tolower(str[0]) != c)
		{
				return NULL;
		}else
		{
				return str + idx;
		}
}

const char* AR_reverse_stristr(const char *str, size_t l,  const char *match, size_t ml)
{
		size_t delta;
		size_t i;
		AR_ASSERT(str != NULL && match != NULL);

		if (ml > l)
		{
				return NULL;
		}

		if(ml == 0)
		{
				return str + l;
		}

		if(ml == 1)
		{
				return AR_reverse_strichr(str, l, match[0]);
		}


		delta = l - ml;

		if(AR_UINT64_MAX / AR_UCHARMAX > ml)
		{
				ar_uint_64_t	search_hash, match_hash;
				search_hash = 0;
				match_hash = 0;

				for(i = 0; i < ml; ++i)
				{
						search_hash += (ar_uint_64_t)AR_tolower(str[delta + i]);
						match_hash += (ar_uint_64_t)AR_tolower(match[i]);
				}


				while(search_hash != match_hash || AR_strnicmp(str + delta, match, ml) != 0)
				{
						if(delta == 0)
						{
								return NULL;
						}

						delta--;
						search_hash -= (ar_uint_64_t)AR_tolower(str[delta + ml]);
						search_hash += (ar_uint_64_t)AR_tolower(str[delta]);
				}

				return str + delta;
		}else
		{
				while(AR_strnicmp(str + delta, match, ml) != 0)
				{
						if(delta == 0)
						{
								return NULL;
						}

						delta--;
				}

				return str + delta;
		}

}


/************************************************hash*****************************************/



ar_uint_t	AR_wcshash(const wchar_t *str)
{
		ar_uint_t	ret;
		size_t	i;
		AR_ASSERT(str != NULL);

		ret = 0;
		for(i = 0; str[i]; ++i)
		{
				ret ^= (str[i] << (i & 0x0F));
		}
		return ret;
}


ar_uint_t		AR_wcshash_n(const wchar_t *str, size_t n)
{
		ar_uint_t	ret;
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


ar_uint_t	AR_strhash(const char *str)
{
		ar_uint_t	ret;
		size_t	i;
		AR_ASSERT(str != NULL);

		ret = 0;
		
		for(i = 0; str[i]; ++i)
		{
				ret = str[i] + (ret << 6) + (ret << 16) - ret;
		}
		
		return ret;
}


ar_uint_t		AR_strhash_n(const char *str, size_t n)
{
		ar_uint_t	ret;
		size_t	i;
		if(n == 0)return 0;
		AR_ASSERT(str != NULL);

		ret = 0;
		
		for(i = 0; i < n && str[i]; ++i)
		{
				ret = str[i] + (ret << 6) + (ret << 16) - ret;
		}
		
		return ret;
}




/*************************************************************misc*******************************************************************/


char* AR_strrot13(char *s, size_t n)
{
		size_t i;
		AR_ASSERT(s != NULL);
		for(i = 0; i < n && s[i] != '\0'; ++i)
		{
				char b = s[i];
				if( (b >= 'a' && b <= 'm') || (b >= 'A' && b <= 'M'))
				{
						s[i] = b + 13;
				}else if((b >= 'n' && b <= 'z') || (b >= 'N' && b <= 'Z'))
				{
						s[i] = b - 13;
				}else
				{

				}
		}
		
		return s;
}



/***********************************************************************************************************************************/

#if(0)

const wchar_t* AR_wcsstr_kmp(const wchar_t *s, const wchar_t *p)
{
		AR_ASSERT(s != NULL && p != NULL);
		return AR_wcsstr_kmp_s(s, s + AR_wcslen(s), p);

}


const wchar_t* AR_wcsstr_kmp_s(const wchar_t *beg, const wchar_t *end, const wchar_t *p)
{

		size_t i,k,m,n;
		size_t *next;
		const wchar_t *ret, *s;
		AR_ASSERT(beg != NULL && p != NULL && end != NULL);
		n = end - beg;
		m = AR_wcslen(p);
		if(n == 0 || m == 0 || m > n)return NULL;

		next = AR_NEWARR0(size_t, m);

		if(!next)
		{
				return NULL;
		}

		next[0] = 0;

		for(i = 1,k = 0; i < m; ++i)
		{
				/*
				如果p[k]处不相等， 则证明有k个前缀相等，则所对应的真后缀为next[k-1];
				因为改变k长度的只有下面k++一行，且只有k > 0时才有读取next数组的操作，
				所以k读取的永远时已经被计算好的
				*/
				while(k > 0 && p[k] != p[i])k = next[k-1];
				if(p[k] == p[i])k++;
				next[i] = k;
		}

		ret = NULL;
		s = beg;

		for(k = 0; k < m && s != end; ++s)
		{
				while(k > 0 && p[k] != *s) k = next[k-1];
				if(p[k] == *s)k++;
		}

		ret = k == m ? s - m : NULL;

		AR_DEL(next);
		return ret;
}


const wchar_t* AR_wcsstr_sunday_s(const wchar_t *s_b, const wchar_t *s_e, const wchar_t *p_b, const wchar_t *p_e)
{
		ar_int_t pl, sl, s_pos, f_pos,k;

		AR_ASSERT(s_b != NULL && s_e != NULL && s_b <= s_e);
		AR_ASSERT(p_b != NULL && p_e != NULL && p_b <= p_e);

		sl = s_e - s_b;
		pl = p_e - p_b;

		if(pl == 0)
		{
				return s_b + sl;
		}

		if(sl == 0)
		{
				return NULL;
		}

		s_pos = 0;
		
		while(s_pos + pl <= sl)
		{
				
				for(f_pos = 0; f_pos < pl && s_b[s_pos + f_pos] == p_b[f_pos]; ++f_pos);
				
				if(f_pos == pl)
				{
						return s_b + s_pos;
				}

				k = 0;
				for(k = pl - 1; k >= 0; --k)
				{
						if(s_b[s_pos + pl] == p_b[k])
						{
								s_pos += pl - k;
								break;
						}
				}

				if(k < 0)
				{
						s_pos = s_pos + pl + 1;
				}
		}

		return NULL;
}



const wchar_t* AR_wcsstr_sunday(const wchar_t *s, const wchar_t *p)
{
		return AR_wcsstr_sunday_s(s, s + AR_wcslen(s), p, p + AR_wcslen(p));
}


#endif



AR_NAMESPACE_END

