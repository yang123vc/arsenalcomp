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


/*此模块修改自Poco URI模块*/
		
struct __arsenal_uri_tag
{
		arCodePage_t	code_page;
		arString_t		*scheme;
		arString_t		*user_info;
		arString_t		*host;
		arString_t		*path;
		arString_t		*query;
		arString_t		*fragment;
		ar_uint_16_t		port;

		bool_t			_parse_encoded;
};

arURI_t*		AR_CreateURI(arCodePage_t cp)
{
		arURI_t	*uri;
		uri = AR_NEW0(arURI_t);

		if(uri == NULL)
		{
				return NULL;
		}
		uri->code_page = cp;
		uri->scheme = AR_CreateString();
		uri->user_info = AR_CreateString();
		uri->host = AR_CreateString();
		uri->path = AR_CreateString();
		uri->query = AR_CreateString();
		uri->fragment = AR_CreateString();
		uri->_parse_encoded = false;

		if(uri->scheme == NULL || uri->user_info == NULL || uri->host == NULL || uri->path == NULL || uri->query == NULL || uri->fragment == NULL)
		{
				AR_DestroyURI(uri);
				uri = NULL;
		}

		return uri;
}


void			AR_DestroyURI(arURI_t *uri)
{
		AR_ASSERT(uri != NULL);
		if(uri->scheme)
		{
				AR_DestroyString(uri->scheme);
				uri->scheme = NULL;
		}

		if(uri->user_info)
		{
				AR_DestroyString(uri->user_info);
				uri->user_info = NULL;
		}

		if(uri->host)
		{
				AR_DestroyString(uri->host);
				uri->host = NULL;
		}

		if(uri->path)
		{
				AR_DestroyString(uri->path);
				uri->path = NULL;
		}


		if(uri->query)
		{
				AR_DestroyString(uri->query);
				uri->query = NULL;
		}

		if(uri->fragment)
		{
				AR_DestroyString(uri->fragment);
				uri->fragment = NULL;
		}

		AR_DEL(uri);
		uri = NULL;
}

void			AR_ClearURI(arURI_t *uri)
{
		AR_ASSERT(uri != NULL);
		if(uri->scheme)
		{
				AR_ClearString(uri->scheme);
		}

		if(uri->user_info)
		{
				AR_ClearString(uri->user_info);
		}

		if(uri->host)
		{
				AR_ClearString(uri->host);
		}

		if(uri->path)
		{
				AR_ClearString(uri->path);
		}


		if(uri->query)
		{
				AR_ClearString(uri->query);
		}

		if(uri->fragment)
		{
				AR_ClearString(uri->fragment);
		}

		uri->port = 0;

		uri->_parse_encoded = false;
}


arCodePage_t	AR_GetURICodePage(const arURI_t *uri)
{
		AR_ASSERT(uri != NULL);
		return uri->code_page;
}

void			AR_SetURICodePage(arURI_t *uri, arCodePage_t cp)
{
		AR_ASSERT(uri != NULL);
		uri->code_page = cp;
}

ar_int_t			AR_CompURI(const arURI_t *l, const arURI_t *r)
{
		ar_int_t cmp;
		AR_ASSERT(l != NULL && r != NULL);
		cmp = 0;
		cmp =  AR_CompString(l->scheme, r->scheme);
		if(cmp != 0)return cmp;

		cmp =  AR_CompString(l->user_info, r->user_info);
		if(cmp != 0)return cmp;


		cmp =  AR_CompString(l->host, r->host);
		if(cmp != 0)return cmp;


		cmp =  AR_CompString(l->path, r->path);
		if(cmp != 0)return cmp;


		cmp =  AR_CompString(l->query, r->query);
		if(cmp != 0)return cmp;

		cmp =  AR_CompString(l->fragment, r->fragment);
		if(cmp != 0)return cmp;

		cmp = AR_CMP(l->port,r->port);
		if(cmp != 0)return cmp;

		return 0;
}


/*************************************************************Helper***************************************************************/
#define RESERVED_PATH			L"?#"
#define RESERVED_QUERY			L"#"
#define RESERVED_FRAGMENT		L""
#define ILLEGAL					L"%<>{}|\\\"^`"


#define RESERVED_PATH_S			"?#"
#define RESERVED_QUERY_S		"#"
#define RESERVED_FRAGMENT_S		""
#define ILLEGAL_S				"%<>{}|\\\"^`"


/*
这里假定输入全为未编码URL，未编码URL均为unicode，将其转换为指定的url形式(%AB)后，当做unicode存入output
*/
static arStatus_t	__encode(arCodePage_t cp, const wchar_t *begin, const wchar_t *end, const char *reserved, arString_t *output)
{
		arStatus_t status;
		const char *s;
		char *str;
		
		AR_ASSERT(begin != NULL && end != NULL && begin <= end && output != NULL);
		status = AR_S_YES;

		if(begin == end)
		{
				return AR_S_YES;
		}

		str = AR_wcs_to_str(cp, begin, end - begin);
		
		if(str == NULL)
		{
				status = AR_E_BADENCCONV;
				goto END_POINT;
		}
		

		for(s = str; *s != '\0'; ++s)
		{
				ar_uint_8_t c = (ar_uint_8_t)*s;

				if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~')
				{
						status = AR_AppendCharToString(output, (wchar_t)c);
						if(status != AR_S_YES)
						{
								goto END_POINT;
						}
				}else if(c <= 0x20 || c >= 0x7F || AR_strchr(ILLEGAL_S,c) != NULL || AR_strchr(reserved,c) != NULL)
				{
						status = AR_AppendFormatString(output, L"%%%02X", (ar_uint_32_t)c);
						if(status != AR_S_YES)
						{
								goto END_POINT;
						}
				}else
				{
						status = AR_AppendCharToString(output, (wchar_t)c);
						if(status != AR_S_YES)
						{
								goto END_POINT;
						}
				}
		}

END_POINT:
		if(str)
		{
				AR_DEL(str);
				str = NULL;
		}

		return status;
}


#if(0)
/*
这里假定输入全为已编码URL(假定其字符范围均符合RFC3986)，已编码URL的编码为参数cp，将其转换为指定的unicode
*/
static arStatus_t	__decode(arCodePage_t cp, const wchar_t *begin, const wchar_t *end, arString_t *output)
{
		arStatus_t		status;
		const wchar_t *s;
		char *buf, *w;
		wchar_t *wbuf;
		AR_ASSERT(begin != NULL && end != NULL && output != NULL);
		
		status = AR_S_YES;

		if(begin == end)/*字符串为空*/
		{
				return AR_S_YES;
		}

		wbuf = NULL;
		
		buf = AR_NEWARR0(char, (end - begin) * 2 + 1);/*绝对够用了，实际上只能比编码后的url小才对*/
		
		if(buf == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}

		w = buf;
		s = begin;

		while(s < end)
		{

				wchar_t c, hi,lo;
				
				c = *s++;
				
				if(c == L'%')
				{
						if(s == end)
						{
								status = AR_E_MALFORMAT;
								goto END_POINT;
						}
						
						hi = *s++;

						if(s == end)
						{
								status = AR_E_MALFORMAT;
								goto END_POINT;
						}

						lo = *s++;

						if(hi >= L'0' && hi <= L'9')
						{
								c = hi - L'0';
						}else if(hi >= L'A' && hi <= 'F')
						{
								c = hi - L'A' + 10;
						}else if(hi >= 'a' && hi <= 'f')
						{
								c = hi - L'a' + 10;
						}else
						{
								status = AR_E_MALFORMAT;
								goto END_POINT;
						}

						c *= 16;

						if(lo >= L'0' && lo <= L'9')
						{
								c += lo - L'0';
						}else if(lo >= L'A' && lo <= 'F')
						{
								c += lo - L'A' + 10;
						}else if(lo >= 'a' && lo <= 'f')
						{
								c += lo - L'a' + 10;
						}else
						{
								status = AR_E_MALFORMAT;
								goto END_POINT;
						}
				}
				*w++ = (char)c;
		}
		*w++ = '\0';

		wbuf = AR_str_to_wcs(cp, buf, AR_strlen(buf));

		if(wbuf == NULL)
		{
				status = AR_E_BADENCCONV;
				goto END_POINT;
		}

		status = AR_AppendString(output, wbuf);
		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(buf)
		{
				AR_DEL(buf);
				buf = NULL;
		}
		if(wbuf)
		{
				AR_DEL(wbuf);
				wbuf = NULL;
		}
		return status;
}
#endif


static arStatus_t	__decode(arCodePage_t cp, const wchar_t *begin, const wchar_t *end, arString_t *output)
{
		arStatus_t		status;
		const char *s;
		char *buf, *w, *str;
		wchar_t *wbuf;
		AR_ASSERT(begin != NULL && end != NULL && output != NULL);
		
		status = AR_S_YES;
		wbuf = NULL;
		str = NULL;
		buf = NULL;

		if(begin == end)/*字符串为空*/
		{
				return AR_S_YES;
		}

		str = AR_wcs_to_str(cp, begin, end - begin);

		if(str == NULL)
		{
				status =  AR_E_BADENCCONV;
				goto END_POINT;
		}
		
		buf = AR_NEWARR0(char, AR_strlen(str) + 1);
		
		if(buf == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}

		w = buf;
		s = str;
		
		while(*s != '\0')
		{

				char c, hi,lo;
				
				c = *s++;
				
				if(c == '%')
				{
						if(*s == '\0')
						{
								status = AR_E_MALFORMAT;
								goto END_POINT;
						}
						
						hi = *s++;

						if(*s == '\0')
						{
								status = AR_E_MALFORMAT;
								goto END_POINT;
						}

						lo = *s++;

						if(hi >= '0' && hi <= '9')
						{
								c = hi - '0';
						}else if(hi >= 'A' && hi <= 'F')
						{
								c = hi - L'A' + 10;
						}else if(hi >= 'a' && hi <= 'f')
						{
								c = hi - 'a' + 10;
						}else
						{
								status = AR_E_MALFORMAT;
								goto END_POINT;
						}

						c *= 16;

						if(lo >= '0' && lo <= '9')
						{
								c += lo - L'0';
						}else if(lo >= 'A' && lo <= 'F')
						{
								c += lo - 'A' + 10;
						}else if(lo >= 'a' && lo <= 'f')
						{
								c += lo - 'a' + 10;
						}else
						{
								status = AR_E_MALFORMAT;
								goto END_POINT;
						}
				}
				*w++ = (char)c;
		}
		*w++ = '\0';

		wbuf = AR_str_to_wcs(cp, buf, AR_strlen(buf));

		if(wbuf == NULL)
		{
				status = AR_E_BADENCCONV;
				goto END_POINT;
		}

		status = AR_AppendString(output, wbuf);
		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(buf)
		{
				AR_DEL(buf);
				buf = NULL;
		}
		if(wbuf)
		{
				AR_DEL(wbuf);
				wbuf = NULL;
		}

		if(str)
		{
				AR_DEL(str);
				str = NULL;
		}

		return status;
}


static ar_uint_16_t	__get_well_known_port(const arURI_t *uri)
{
		AR_ASSERT(uri != NULL);

		if(AR_CompStringWithWcs(uri->scheme, L"ftp") == 0)
		{
				return 21;
		}


		if(AR_CompStringWithWcs(uri->scheme, L"ssh") == 0)
		{
				return 22;
		}

		if(AR_CompStringWithWcs(uri->scheme, L"telnet") == 0)
		{
				return 23;
		}

		if(AR_CompStringWithWcs(uri->scheme, L"http") == 0)
		{
				return 80;
		}

		if(AR_CompStringWithWcs(uri->scheme, L"nntp") == 0)
		{
				return 119;
		}

		if(AR_CompStringWithWcs(uri->scheme, L"ldap") == 0)
		{
				return 389;
		}

		if(AR_CompStringWithWcs(uri->scheme, L"https") == 0)
		{
				return 443;
		}

		if(AR_CompStringWithWcs(uri->scheme, L"rtsp") == 0)
		{
				return 554;
		}

		if(AR_CompStringWithWcs(uri->scheme, L"sip") == 0)
		{
				return 5060;
		}

		if(AR_CompStringWithWcs(uri->scheme, L"sips") == 0)
		{
				return 5061;
		}

		if(AR_CompStringWithWcs(uri->scheme, L"xmpp") == 0)
		{
				return 5222;
		}
		
		return 0;
}

/*************************************************************Parse***************************************************************/



#define __GO_IF_FAIL(_cond, _status, _epos, _label)		do{if(!(_cond)){ret.status = (_status); ret.next = NULL; ret.err_pos = (_epos); goto _label;} }while(0)
#define __GOINV_IF_FAIL(_cond, _status, _epos)			__GO_IF_FAIL(_cond, _status, _epos, INVALID_POINT)
#define __GOEND_IF_FAIL(_cond, _status, _epos)			__GO_IF_FAIL(_cond, _status, _epos, END_POINT)

typedef struct __parse_ret
{
		arStatus_t		status;
		const wchar_t	*next;
		const wchar_t	*err_pos;
}uriParseRet_t;




static uriParseRet_t __parse(arURI_t *uri, const wchar_t *begin, const wchar_t *end);
static uriParseRet_t __parse_authority(arURI_t *uri, const wchar_t *begin, const wchar_t *end);
static uriParseRet_t __parse_host_port(arURI_t *uri, const wchar_t *begin, const wchar_t *end);
static uriParseRet_t __parse_pathetc(arURI_t *uri, const wchar_t *begin, const wchar_t *end);
static uriParseRet_t __parse_path(arURI_t *uri, const wchar_t *begin, const wchar_t *end);
static uriParseRet_t __parse_query(arURI_t *uri, const wchar_t *begin, const wchar_t *end);
static uriParseRet_t __parse_fragment(arURI_t *uri, const wchar_t *begin, const wchar_t *end);



static uriParseRet_t __parse(arURI_t *uri, const wchar_t *begin, const wchar_t *end)
{
		uriParseRet_t	ret;
		const wchar_t *s, *e;
		arString_t *tmp;
		AR_ASSERT(uri != NULL && begin != NULL && end != NULL && begin <= end);

		ret.status = AR_S_YES;
		ret.next = NULL;
		ret.err_pos = NULL;
		
		tmp = AR_CreateString();
		__GOEND_IF_FAIL(tmp != NULL, AR_E_NOMEM, NULL);
		
		s = begin;
		e = end;

		if(s == e)
		{
				goto END_POINT;
		}

		if(*s != L'/' && *s != L'.' && *s != L'?' && *s != L'#')
		{
				while(s < end && *s != L':' && *s != L'?' && *s != L'#' && *s != L'/')
				{
						ret.status = AR_AppendCharToString(tmp, *s);
						__GOEND_IF_FAIL(ret.status == AR_S_YES, ret.status, NULL);
						++s;
				}
				
				if(s < end && *s == L':')
				{
						++s;
						__GOEND_IF_FAIL(s < e, AR_E_MALFORMAT, s - 1);
						AR_StringToLower(tmp);
						AR_SwapString(uri->scheme, tmp);
						AR_ClearString(tmp);
						
						if(*s == L'/')
						{
								++s;
								if(s < end && *s == L'/')
								{
										++s;
										ret = __parse_authority(uri, s, end);
										__GOEND_IF_FAIL(ret.status == AR_S_YES, ret.status, ret.err_pos);
										s = ret.next;
										AR_ASSERT(s != NULL && s > begin && s <= end);
								}else
								{
										--s;
								}
						}

						ret = __parse_pathetc(uri, s, end);
				}else
				{
						ret = __parse_pathetc(uri, begin, end);
				}
		}else
		{
				ret = __parse_pathetc(uri, begin, end);
		}
		
		/*
		if(uri->port == 0)
		{
				uri->port = __get_well_known_port(uri);
		}
		*/

END_POINT:
		if(tmp != NULL)
		{
				AR_DestroyString(tmp);
				tmp = NULL;
		}

		return ret;
}


static uriParseRet_t __parse_authority(arURI_t *uri, const wchar_t *begin, const wchar_t *end)
{
		uriParseRet_t	ret;
		const wchar_t *s, *part_begin;
		AR_ASSERT(uri != NULL && begin != NULL && end != NULL && begin <= end);
		
		ret.status = AR_S_YES;
		ret.err_pos = NULL;
		ret.next = NULL;

		/*
				input	: 'solidus@www.test.com/'

				user_info = solidus;
				part = www.test.com;

		*/
		
		s = begin;
		part_begin = s;
		while(s < end && *s != L'/' && *s != L'?' && *s != L'#')
		{
				if(*s == L'@')
				{
						if(uri->_parse_encoded)
						{
								ret.status = __decode(uri->code_page, part_begin, s, uri->user_info);
						}else
						{
								ret.status = AR_AppendStringN(uri->user_info, part_begin, s - part_begin);
						}
						__GOEND_IF_FAIL(ret.status == AR_S_YES, ret.status, NULL);
						part_begin = s + 1;
				}
				++s;
		}
		
		ret = __parse_host_port(uri, part_begin, s);

		if(ret.status == AR_S_YES)
		{
				ret.next = s;
				AR_ASSERT(ret.err_pos == NULL);
		}
		
END_POINT:
		return ret;
}


static uriParseRet_t __parse_host_port(arURI_t *uri, const wchar_t *begin, const wchar_t *end)
{
		uriParseRet_t ret;
		arString_t *host;
		const wchar_t *s;
		AR_ASSERT(uri != NULL && begin != NULL && end != NULL && begin <= end);
		
		ret.status = AR_S_YES;
		ret.err_pos = NULL;
		ret.next = NULL;

		if(begin == end)
		{
				ret.next = begin;
				return ret;
		}

		host = AR_CreateString();
		__GOEND_IF_FAIL(host != NULL, AR_E_NOMEM, NULL);
		s = begin;

		/*		input : ipv6格式 'http://[xx::xx::xx::xx....]/path/x.php'
				host = [xx::xx::xx::xx...];

				input : 'http://www.a.b.c.d.baidu.com/path/x.php'
				host = www.a.b.c.d.baidu.com;

		*/

		if(*s == L'[')
		{
				++s;
				while(s < end && *s != L']')
				{
						ret.status = AR_AppendCharToString(host, *s);
						__GOEND_IF_FAIL(ret.status == AR_S_YES, ret.status, NULL);
						++s;
				}
				
				__GOEND_IF_FAIL(s < end, AR_E_MALFORMAT, begin);
				++s;
		}else
		{
				while(s < end && *s != L':')
				{
						ret.status = AR_AppendCharToString(host, *s);
						__GOEND_IF_FAIL(ret.status == AR_S_YES, ret.status, NULL);
						++s;
				}
		}

		if(s < end && *s == L':')
		{
				++s;

				if(AR_iswdigit(*s))
				{
						ar_uint_32_t port;
						const wchar_t *s_next;
						
						s_next = AR_wtou32_s(s, end, &port, 10);

						if(s_next == NULL)
						{
								__GOEND_IF_FAIL(false, AR_E_MALFORMAT, s);
						}

						if(port == 0 || port > 65535)
						{
								__GOEND_IF_FAIL(false, AR_E_RANGE, s);
						}

						uri->port = (ar_uint_16_t)port;
						s = s_next;
				}else
				{
						uri->port = __get_well_known_port(uri);
				}
		}else
		{
				uri->port = __get_well_known_port(uri);
		}

		AR_StringToLower(host);
		AR_SwapString(uri->host, host);
		ret.next = s;
		ret.err_pos = NULL;

END_POINT:
		if(host)
		{
				AR_DestroyString(host);
				host = NULL;
		}
		return ret;
}


static uriParseRet_t __parse_pathetc(arURI_t *uri, const wchar_t *begin, const wchar_t *end)
{
		uriParseRet_t	ret;
		const wchar_t *s;
		AR_ASSERT(uri != NULL && begin != NULL && end != NULL && begin <= end);

		ret.status = AR_S_YES;
		ret.next = NULL;
		ret.err_pos = NULL;
		s = begin;

		if(s == end)
		{
				ret.next = s;
				goto END_POINT;
		}

		if(*s != L'?' && *s != L'#')
		{
				ret = __parse_path(uri, s, end);
				__GOEND_IF_FAIL(ret.status == AR_S_YES, ret.status, ret.err_pos);
				s = ret.next;
				AR_ASSERT(s != NULL);
		}

		if(s < end && *s == L'?')
		{
				++s;
				ret = __parse_query(uri, s, end);
				__GOEND_IF_FAIL(ret.status == AR_S_YES, ret.status, ret.err_pos);
				s = ret.next;
		}
		
		if(s < end && *s == L'#')
		{
				++s;
				ret = __parse_fragment(uri, s, end);
		}


END_POINT:
		return ret;
}


static uriParseRet_t __parse_path(arURI_t *uri, const wchar_t *begin, const wchar_t *end)
{
		
		uriParseRet_t	ret;
		const wchar_t *s;
		const wchar_t *path_begin;
		AR_ASSERT(uri != NULL && begin != NULL && end != NULL && begin <= end);

		
		ret.status = AR_S_YES;
		ret.next = NULL;
		ret.err_pos = NULL;
		
		s = begin;
		path_begin = s;

		while(s < end && *s != L'?' && *s != L'#')
		{
				++s;
		}
		
		if(uri->_parse_encoded)
		{
				ret.status = __decode(uri->code_page, path_begin, s, uri->path);
		}else
		{
				ret.status = AR_SetStringN(uri->path, path_begin, s - path_begin);
		}

		__GOEND_IF_FAIL(ret.status == AR_S_YES, ret.status, begin);
		ret.next = s;
END_POINT:
		return ret;
}


static uriParseRet_t __parse_query(arURI_t *uri, const wchar_t *begin, const wchar_t *end)
{
		uriParseRet_t	ret;
		const wchar_t *s;
		const wchar_t *query_begin;
		AR_ASSERT(uri != NULL && begin != NULL && end != NULL && begin <= end);
		
		ret.status = AR_S_YES;
		ret.next = NULL;
		ret.err_pos = NULL;

		s = begin;
		query_begin = s;

		while(s < end && *s != L'#')
		{
				s++;
		}
		
		if(uri->_parse_encoded)
		{
				ret.status = __decode(uri->code_page, query_begin, s, uri->query);
		}else
		{
				ret.status = AR_SetStringN(uri->query, query_begin, s - query_begin);
		}

		__GOEND_IF_FAIL(ret.status == AR_S_YES, ret.status, begin);

		ret.next = s;

END_POINT:
		return ret;
}

static uriParseRet_t __parse_fragment(arURI_t *uri, const wchar_t *begin, const wchar_t *end)
{
		uriParseRet_t	ret;
		const wchar_t *s;
		const wchar_t *fragment_begin;
		AR_ASSERT(uri != NULL && begin != NULL && end != NULL && begin <= end);
		
		ret.status = AR_S_YES;
		ret.next = NULL;
		ret.err_pos = NULL;
		s = begin;
		fragment_begin = s;

		while(s < end)
		{
				s++;
		}
		
		if(uri->_parse_encoded)
		{
				ret.status = __decode(uri->code_page, fragment_begin, s, uri->fragment);
		}else
		{
				ret.status = AR_SetStringN(uri->fragment, fragment_begin, s - fragment_begin);
		}
		__GOEND_IF_FAIL(ret.status == AR_S_YES, ret.status, begin);
		ret.next = s;
END_POINT:

		return ret;
}


/**************************************************************Public*************************************************/

bool_t	AR_IsRelativeURI(const arURI_t *uri)
{
		AR_ASSERT(uri != NULL);
		return AR_GetStringLength(uri->scheme) == 0 ? true : false;
}



arStatus_t		AR_GetURIScheme(const arURI_t *uri, arString_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);

		return AR_CopyString(str, uri->scheme);
}

arStatus_t		AR_GetURIUserInfo(const arURI_t *uri, arString_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);
		return AR_CopyString(str, uri->user_info);
		/*return __encode(uri->code_page, AR_GetStringCString(uri->user_info), AR_GetStringCString(uri->user_info) + AR_GetStringLength(uri->user_info), ILLEGAL_S, str);*/
}

arStatus_t		AR_GetURIHost(const arURI_t *uri, arString_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);

		return AR_CopyString(str, uri->host);
}

ar_uint_16_t		AR_GetURIPort(const arURI_t *uri)
{
		AR_ASSERT(uri != NULL);
		return uri->port;
}

#define __GO_IF_FAIL2(_cond, _status, _label)do{			if(!(_cond)){status = (_status); goto _label;}  }while(0)
#define __GOEND_IF_FAIL2(_cond, _status)		__GO_IF_FAIL2(_cond, _status, END_POINT)

arStatus_t		AR_GetURIAuthority(const arURI_t *uri, arString_t *str)
{
		arStatus_t status;
		AR_ASSERT(uri != NULL && str != NULL);
		status = AR_S_YES;

		AR_ClearString(str);

		if(AR_GetStringLength(uri->user_info) > 0)
		{
				/*status = __encode(uri->code_page, AR_GetStringCString(uri->user_info), AR_GetStringCString(uri->user_info) + AR_GetStringLength(uri->user_info), ILLEGAL_S, str);*/

				status = AR_AppendString(str, AR_GetStringCString(uri->user_info));
				
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = AR_AppendString(str, L"@");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

		if(AR_wcsstr(AR_GetStringCString(uri->host), L":") != NULL)
		{
				status = AR_AppendString(str, L"[");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = AR_AppendString(str, AR_GetStringCString(uri->host));
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = AR_AppendString(str, L"]");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}else
		{
				status = AR_AppendString(str, AR_GetStringCString(uri->host));
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

		if(uri->port > 0 && __get_well_known_port(uri) != uri->port)
		{
				status = AR_AppendString(str, L":");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
				
				status = AR_AppendFormatString(str, L"%u", (ar_uint_32_t)uri->port);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

END_POINT:
		return status;
}


arStatus_t		AR_GetURIEncodedAuthority(const arURI_t *uri, arString_t *str)
{
		arStatus_t status;
		AR_ASSERT(uri != NULL && str != NULL);
		status = AR_S_YES;

		AR_ClearString(str);

		if(AR_GetStringLength(uri->user_info) > 0)
		{
				status = __encode(uri->code_page, AR_GetStringCString(uri->user_info), AR_GetStringCString(uri->user_info) + AR_GetStringLength(uri->user_info), ILLEGAL_S, str);
				
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = AR_AppendString(str, L"@");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

		if(AR_wcsstr(AR_GetStringCString(uri->host), L":") != NULL)
		{
				status = AR_AppendString(str, L"[");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = AR_AppendString(str, AR_GetStringCString(uri->host));
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = AR_AppendString(str, L"]");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}else
		{
				status = AR_AppendString(str, AR_GetStringCString(uri->host));
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

		if(uri->port > 0 && __get_well_known_port(uri) != uri->port)
		{
				status = AR_AppendString(str, L":");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
				
				status = AR_AppendFormatString(str, L"%u", (ar_uint_32_t)uri->port);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

END_POINT:
		return status;
}

arStatus_t		AR_GetURIPath(const arURI_t *uri, arString_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);
		return AR_CopyString(str, uri->path);
}

arStatus_t		AR_GetURIEncodedPath(const arURI_t *uri, arString_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);
		return __encode(uri->code_page, AR_GetStringCString(uri->path), AR_GetStringCString(uri->path) + AR_GetStringLength(uri->path), RESERVED_PATH_S, str);
}


arStatus_t		AR_GetURIQuery(const arURI_t *uri, arString_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);
		return AR_CopyString(str, uri->query);
}


arStatus_t		AR_GetURIEncodedQuery(const arURI_t *uri, arString_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);
		return __encode(uri->code_page, AR_GetStringCString(uri->query), AR_GetStringCString(uri->query) + AR_GetStringLength(uri->query), RESERVED_QUERY_S, str);
}

arStatus_t		AR_GetURIFragment(const arURI_t *uri, arString_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);
		return AR_CopyString(str, uri->fragment);
}

arStatus_t		AR_GetURIEncodedFragment(const arURI_t *uri, arString_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);
		return __encode(uri->code_page, AR_GetStringCString(uri->fragment), AR_GetStringCString(uri->fragment) + AR_GetStringLength(uri->fragment), RESERVED_FRAGMENT_S, str);
}


arStatus_t		AR_GetURIPathEtc(const arURI_t *uri, arString_t *str)
{
		arStatus_t status;
		AR_ASSERT(uri != NULL && str != NULL);

		AR_ClearString(str);
		status = AR_CatString(str, uri->path);

		__GOEND_IF_FAIL2(status == AR_S_YES, status);

		if(AR_GetStringLength(uri->query) > 0)
		{
				status = AR_AppendString(str, L"?");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
				status = AR_AppendString(str, AR_GetStringCString(uri->query));
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

		if(AR_GetStringLength(uri->fragment) > 0)
		{
				status = AR_AppendString(str, L"#");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = AR_AppendString(str, AR_GetStringCString(uri->fragment));
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

END_POINT:
		return status;

}


arStatus_t		AR_GetURIEncodedPathEtc(const arURI_t *uri, arString_t *str)
{
		arStatus_t status;
		AR_ASSERT(uri != NULL && str != NULL);

		AR_ClearString(str);
		status = __encode(uri->code_page, AR_GetStringCString(uri->path), AR_GetStringCString(uri->path) + AR_GetStringLength(uri->path), RESERVED_PATH_S, str);

		__GOEND_IF_FAIL2(status == AR_S_YES, status);

		if(AR_GetStringLength(uri->query) > 0)
		{
				status = AR_AppendString(str, L"?");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = __encode(uri->code_page, AR_GetStringCString(uri->query), AR_GetStringCString(uri->query) + AR_GetStringLength(uri->query), RESERVED_QUERY_S, str);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

		if(AR_GetStringLength(uri->fragment) > 0)
		{
				status = AR_AppendString(str, L"#");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = __encode(uri->code_page, AR_GetStringCString(uri->fragment), AR_GetStringCString(uri->fragment) + AR_GetStringLength(uri->fragment), RESERVED_FRAGMENT_S, str);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}
END_POINT:
		return status;
}



arStatus_t		AR_SetURIScheme(arURI_t *uri, const wchar_t *str)
{
		arStatus_t status;
		AR_ASSERT(uri != NULL && str != NULL);
		status = AR_SetString(uri->scheme, str);
		if(status == AR_S_YES)
		{
				AR_StringToLower(uri->scheme);
				uri->port = __get_well_known_port(uri);
		}
		
		return status;

}

arStatus_t		AR_SetURIUserInfo(arURI_t *uri, const wchar_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->user_info);
		return __decode(uri->code_page, str, str + AR_wcslen(str), uri->user_info);
}

arStatus_t		AR_SetURIEncodedUserInfo(arURI_t *uri, const wchar_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->user_info);
		return AR_SetString(uri->user_info, str);
}


arStatus_t		AR_SetURIHost(arURI_t *uri, const wchar_t *str)
{
		arStatus_t status;
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->host);
		status =  AR_SetString(uri->host, str);
		if(status == AR_S_YES)
		{
				AR_StringToLower(uri->host);
		}

		return status;
}


void			AR_SetURIPort(arURI_t *uri, ar_uint_16_t port)
{
		AR_ASSERT(uri != NULL);
		uri->port = port;
}

arStatus_t		AR_SetURIAuthority(arURI_t *uri, const wchar_t *str)
{
		uriParseRet_t	ret;
		AR_ASSERT(uri != NULL && str != NULL);
		ret.status = AR_S_YES;
		ret.err_pos = NULL;
		ret.next = NULL;
		AR_ClearString(uri->user_info);
		AR_ClearString(uri->host);
		uri->port = 0;
		uri->_parse_encoded = false;
		ret = __parse_authority(uri, str, str + AR_wcslen(str));
		return ret.status;
}

arStatus_t		AR_SetURIEncodedAuthority(arURI_t *uri, const wchar_t *str)
{
		uriParseRet_t	ret;
		AR_ASSERT(uri != NULL && str != NULL);
		ret.status = AR_S_YES;
		ret.err_pos = NULL;
		ret.next = NULL;
		AR_ClearString(uri->user_info);
		AR_ClearString(uri->host);
		uri->port = 0;
		uri->_parse_encoded = true;
		ret = __parse_authority(uri, str, str + AR_wcslen(str));
		return ret.status;
}



arStatus_t		AR_SetURIPath(arURI_t *uri, const wchar_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->path);
		return AR_SetString(uri->path, str);
}

arStatus_t		AR_SetURIEncodedPath(arURI_t *uri, const wchar_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->path);
		return __decode(uri->code_page, str, str + AR_wcslen(str), uri->path);
}


arStatus_t		AR_SetURIQuery(arURI_t *uri, const wchar_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->query);
		return AR_SetString(uri->query, str);
}


arStatus_t		AR_SetURIEncodedQuery(arURI_t *uri, const wchar_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->query);
		return __decode(uri->code_page, str, str + AR_wcslen(str), uri->query);
}



arStatus_t		AR_SetURIFragment(arURI_t *uri, const wchar_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->fragment);
		return AR_SetString(uri->fragment, str);
}

arStatus_t		AR_SetURIEncodedFragment(arURI_t *uri, const wchar_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->fragment);
		return __decode(uri->code_page, str, str + AR_wcslen(str), uri->fragment);
}

arStatus_t		AR_SetURIPathEtc(arURI_t *uri, const wchar_t *str)
{

		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->path);
		AR_ClearString(uri->query);
		AR_ClearString(uri->fragment);
		uri->_parse_encoded = false;
		return __parse_pathetc(uri, str, str + AR_wcslen(str)).status;
}

arStatus_t		AR_SetURIEncodedPathEtc(arURI_t *uri, const wchar_t *str)
{
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(uri->path);
		AR_ClearString(uri->query);
		AR_ClearString(uri->fragment);
		uri->_parse_encoded = true;
		return __parse_pathetc(uri, str, str + AR_wcslen(str)).status;
}



arStatus_t		AR_SetURI(arURI_t *uri, const wchar_t *str)
{
		arStatus_t status;
		AR_ASSERT(uri != NULL && str != NULL);
		
		str = AR_wcstrim_space(str);

		AR_ClearURI(uri);
		uri->_parse_encoded = false;
		status = __parse(uri, str, str + AR_wcslen(str)).status;
		
		if(status != AR_S_YES)
		{
				AR_ClearURI(uri);
		}
		
		return status;
}

arStatus_t		AR_SetEncodedURI(arURI_t *uri, const wchar_t *str)
{
		arStatus_t status;
		wchar_t *tmp;
		const wchar_t *s;
		AR_ASSERT(uri != NULL && str != NULL);
		tmp = AR_NEWARR(wchar_t, AR_wcslen(str) + 1);
		if(tmp == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}
		AR_wcscpy(tmp, str);

		s = AR_wcstrim_space(tmp);
		AR_wcstrim_right_space(tmp);

		AR_ClearURI(uri);
		uri->_parse_encoded = true;
		status = __parse(uri, s, s + AR_wcslen(s)).status;
		
		if(status != AR_S_YES)
		{
				AR_ClearURI(uri);
		}
		uri->_parse_encoded = false;

END_POINT:
		if(tmp)
		{
				AR_DEL(tmp);
				tmp = NULL;
		}

		return status;
}



arStatus_t		AR_GetEncodedURI(const arURI_t *uri, arString_t *str)
{
		arStatus_t status;
		arString_t *auth;
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);
		status = AR_S_YES;
		auth = NULL;

		if(AR_IsRelativeURI(uri))
		{
				status = __encode(uri->code_page, AR_GetStringCString(uri->path), AR_GetStringCString(uri->path) + AR_GetStringLength(uri->path), RESERVED_PATH_S, str);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}else
		{
				status = AR_AppendString(str, AR_GetStringCString(uri->scheme));
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
				status = AR_AppendString(str, L":");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				auth = AR_CreateString();
				__GOEND_IF_FAIL2(auth != NULL, AR_E_NOMEM);

				status = AR_GetURIAuthority(uri, auth);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				if(!AR_IsEmptyString(auth) || AR_CompStringWithWcs(uri->scheme, L"file") == 0)
				{
						status = AR_AppendString(str, L"//");
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
						
						status = AR_CatString(str, auth);
						
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
				}

				if(!AR_IsEmptyString(uri->path))
				{
						if(!AR_IsEmptyString(auth) && AR_GetStringChar(uri->path, 0) != L'/')
						{
								status = AR_AppendCharToString(str, L'/');
								__GOEND_IF_FAIL2(status == AR_S_YES, status);
						}

						status = __encode(uri->code_page, AR_GetStringCString(uri->path), AR_GetStringCString(uri->path) + AR_GetStringLength(uri->path), RESERVED_PATH_S, str);
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
				}else if(!AR_IsEmptyString(uri->query) || !AR_IsEmptyString(uri->fragment))
				{
						status = AR_AppendCharToString(str, L'/');
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
				}
		}

		if(!AR_IsEmptyString(uri->query))
		{
				status = AR_AppendCharToString(str, L'?');
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = __encode(uri->code_page, AR_GetStringCString(uri->query), AR_GetStringCString(uri->query) + AR_GetStringLength(uri->query), RESERVED_QUERY_S, str);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}


		if(!AR_IsEmptyString(uri->fragment))
		{
				status = AR_AppendCharToString(str, L'#');
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = __encode(uri->code_page, AR_GetStringCString(uri->fragment), AR_GetStringCString(uri->fragment) + AR_GetStringLength(uri->fragment), RESERVED_FRAGMENT_S, str);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}


END_POINT:
		if(auth != NULL)
		{
				AR_DestroyString(auth);
				auth = NULL;
		}

		return status;
}


arStatus_t		AR_GetURI(const arURI_t *uri, arString_t *str)
{
		arStatus_t status;
		arString_t *auth;
		AR_ASSERT(uri != NULL && str != NULL);
		AR_ClearString(str);
		status = AR_S_YES;
		auth = NULL;

		if(AR_IsRelativeURI(uri))
		{
				status = AR_CatString(str, uri->path);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}else
		{
				status = AR_CatString(str, uri->scheme);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
				status = AR_AppendString(str, L":");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				auth = AR_CreateString();
				__GOEND_IF_FAIL2(auth != NULL, AR_E_NOMEM);

				status = AR_GetURIAuthority(uri, auth);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				if(!AR_IsEmptyString(auth) || AR_CompStringWithWcs(uri->scheme, L"file") == 0)
				{
						status = AR_AppendString(str, L"//");
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
						
						status = AR_CatString(str, auth);
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
				}

				if(!AR_IsEmptyString(uri->path))
				{
						if(!AR_IsEmptyString(auth) && AR_GetStringChar(uri->path, 0) != L'/')
						{
								status = AR_AppendCharToString(str, L'/');
								__GOEND_IF_FAIL2(status == AR_S_YES, status);
						}

						status = AR_CatString(str, uri->path);
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
				}else if(!AR_IsEmptyString(uri->query) || !AR_IsEmptyString(uri->fragment))
				{
						status = AR_AppendCharToString(str, L'/');
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
				}
		}

		if(!AR_IsEmptyString(uri->query))
		{
				status = AR_AppendCharToString(str, L'?');
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = AR_CatString(str, uri->query);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}


		if(!AR_IsEmptyString(uri->fragment))
		{
				status = AR_AppendCharToString(str, L'#');
				__GOEND_IF_FAIL2(status == AR_S_YES, status);

				status = AR_CatString(str, uri->fragment);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}


END_POINT:
		if(auth != NULL)
		{
				AR_DestroyString(auth);
				auth = NULL;
		}

		return status;
}

/******************************************************************************************************************************/
static arStatus_t	seg_copy_func(void *data, void **pnew_data, void *ctx)
{
		const wchar_t *src;
		wchar_t **pdest;
		
		AR_ASSERT(data != NULL && pnew_data != NULL);
		AR_UNUSED(ctx);
		src = (const wchar_t*)data;
		pdest = (wchar_t**)pnew_data;
		*pdest = AR_wcsdup(src);
		return *pdest == NULL ? AR_E_NOMEM : AR_S_YES;
}

static void	seg_destroy_func(void *data, void *ctx)
{
		AR_ASSERT(data != NULL);
		AR_UNUSED(ctx);
		AR_DEL(data);
}


static arStatus_t		__get_uri_path_segments(const arURI_t *uri, arList_t *lst)
{
		const wchar_t *p;
		arString_t		*tmp;
		arStatus_t		status;
		AR_ASSERT(uri != NULL && lst != NULL);
		status = AR_S_YES;
		AR_ClearList(lst);

		tmp = AR_CreateString();
		__GOEND_IF_FAIL2(tmp != NULL, AR_E_NOMEM);

		p = AR_GetStringCString(uri->path);

		while(*p)
		{
				if(*p == L'/')
				{
						if(!AR_IsEmptyString(tmp))
						{
								status = AR_PushListBack(lst, (void*)AR_GetStringCString(tmp));
								__GOEND_IF_FAIL2(status == AR_S_YES, status);
								AR_ClearString(tmp);
						}
				}else
				{
						status = AR_AppendCharToString(tmp, *p);
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
				}
				++p;
		}

		if(!AR_IsEmptyString(tmp))
		{
				status = AR_PushListBack(lst, (void*)AR_GetStringCString(tmp));
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}
END_POINT:
		if(tmp)
		{
				AR_DestroyString(tmp);
				tmp = NULL;
		}

		return status;
}


static arStatus_t __build_path(arURI_t *uri, const arList_t *segments, bool_t leading_slash, bool_t  trailing_slash)
{
		arStatus_t		status;
		const arListNode_t	*node;
		bool_t	is_first;
		AR_ASSERT(uri != NULL && segments != NULL);

		status = AR_S_YES;
		is_first = true;
		AR_ClearString(uri->path);
		for(node = segments->head; node != NULL; node = node->next)
		{
				const wchar_t *seg;

				seg = (const wchar_t*)node->data;
				AR_ASSERT(seg != NULL);

				if(is_first)
				{
						
						is_first = false;
						if(leading_slash)
						{
								status = AR_AppendString(uri->path, L"/");
								__GOEND_IF_FAIL2(status == AR_S_YES, status);
						}else if(AR_IsEmptyString(uri->scheme) && AR_wcsstr(seg, L":") != NULL)
						{
								status = AR_AppendString(uri->path, L"./");
								__GOEND_IF_FAIL2(status == AR_S_YES, status);
						}
				}else
				{
						status = AR_AppendString(uri->path, L"/");
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
				}

				status = AR_AppendString(uri->path, seg);
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

		if(trailing_slash)
		{
				status = AR_AppendString(uri->path, L"/");
				__GOEND_IF_FAIL2(status == AR_S_YES, status);
		}

END_POINT:
		return status;

}


static arStatus_t		__remove_path_dot_segments(arURI_t *uri, bool_t is_remove_leading_slash)
{
		arList_t		*segments, *normalized;
		const arListNode_t *node;
		arStatus_t		status;
		bool_t leading_slash, trailing_slash;
		
		AR_ASSERT(uri != NULL);
		status = AR_S_YES;

		if(AR_IsEmptyString(uri->path))
		{
				return AR_S_YES;
		}
		
		normalized = AR_CreateList(seg_copy_func, seg_destroy_func, NULL);
		segments = AR_CreateList(seg_copy_func, seg_destroy_func, NULL);

		__GOEND_IF_FAIL2(normalized != NULL && segments != NULL, AR_E_NOMEM);
		
		status = __get_uri_path_segments(uri, segments);
		
		__GOEND_IF_FAIL2(status == AR_S_YES, status);

		leading_slash = AR_GetStringCString(uri->path)[0] == L'/' ? true : false;
		trailing_slash = AR_GetStringCString(uri->path)[AR_GetStringLength(uri->path) - 1] == L'/' ? true : false;

		for(node = segments->head; node != NULL; node = node->next)
		{
				const wchar_t *seg = (const wchar_t*)node->data;
				AR_ASSERT(seg != NULL);

				if(AR_wcscmp(seg, L"..") == 0)
				{
						if(!AR_IsEmptyList(normalized))
						{
								const wchar_t *tmp = NULL;
								AR_GetListBack(normalized, (void**)&tmp);
								AR_ASSERT(tmp != NULL);
								if(AR_wcscmp(tmp, L"..") == 0)
								{
										status = AR_PushListBack(normalized, (void*)seg);
										__GOEND_IF_FAIL2(status == AR_S_YES, status);
								}else
								{
										AR_PopListBack(normalized);
								}
						}else if(!is_remove_leading_slash)
						{
								status = AR_PushListBack(normalized, (void*)seg);
								__GOEND_IF_FAIL2(status == AR_S_YES, status);
						}

				}else if(AR_wcscmp(seg, L".") != 0)
				{
						status = AR_PushListBack(normalized, (void*)seg);
						__GOEND_IF_FAIL2(status == AR_S_YES, status);
				}
		}

		status = __build_path(uri, normalized, leading_slash, trailing_slash);

END_POINT:
		
		if(segments)
		{
				AR_DestroyList(segments);
				segments = NULL;
		}

		if(normalized)
		{
				AR_DestroyList(normalized);
				normalized = NULL;
		}


		return status;
}


static arStatus_t	__normalize_host(arURI_t *uri)
{
		arString_t *str, *new_host;
		arStatus_t status;
		const wchar_t *p;
		AR_ASSERT(uri != NULL);

		status = AR_S_YES;

		str = AR_CreateString();
		new_host = AR_CreateString();
		if(str == NULL || new_host == NULL )
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}


		status = AR_GetURIHost(uri, str);

		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

		p = AR_GetStringCString(str);
		
		p = AR_wcstrim_space(p);
		if(*p == L'\0')
		{
				goto END_POINT;
		}

		p = AR_wcstrim(p, L".");
		
		if(*p == L'\0')
		{
				goto END_POINT;
		}


		while(*p != L'\0')
		{
				while(*p != L'\0' && *p != L'.')
				{
						status = AR_AppendCharToString(new_host, AR_towlower(*p));
						if(status != AR_S_YES)
						{
								goto END_POINT;
						}
						++p;
				}

				if(*p == L'.')
				{
						while(*p == L'.')
						{
								p = AR_wcstrim(p, L".");		/*如果是最后一个，就不加.了*/
								p = AR_wcstrim_space(p);		/*如果是最后一个，就不加.了*/
						}

						if(*p == L'\0')
						{

						}else
						{
								status = AR_AppendCharToString(new_host, L'.');
								if(status != AR_S_YES)
								{
										goto END_POINT;
								}
						}
				}
		}

END_POINT:

		if(status == AR_S_YES)
		{
				status = AR_SetURIHost(uri, AR_GetStringCString(new_host));
		}

		if(str)
		{
				AR_DestroyString(str);
				str = NULL;
		}

		if(new_host)
		{
				AR_DestroyString(new_host);
				new_host = NULL;
		}

		return status;
}


arStatus_t		AR_NormalizeURI(arURI_t *uri)
{
		arStatus_t		status;
		AR_ASSERT(uri != NULL);
		
		status = __normalize_host(uri);

		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

		status = __remove_path_dot_segments(uri, !AR_IsRelativeURI(uri));

		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		return status;
}


/************************************************************************************************************************/



static arStatus_t	__item_copy_func(void *data, void **pnew_data, void *ctx)
{
        wchar_t *nwcs;
        AR_ASSERT(data != NULL && pnew_data != NULL);
        AR_UNUSED(ctx);
        nwcs = AR_wcsdup((const wchar_t*)data);
        
        if(nwcs)
        {
                *pnew_data = (void*)nwcs;
                return AR_S_YES;
        }else
        {
                return AR_E_NOMEM;
        }
}


static void __item_destroy_func(void *data, void *ctx)
{
        AR_ASSERT(data != NULL);
        AR_UNUSED(ctx);

        AR_DEL(data);
        data = NULL;
}


static ar_uint_64_t	__item_hash_func(void *key, void *ctx)
{
        AR_ASSERT(key != NULL);
        AR_UNUSED(ctx);
        return (ar_uint_64_t)AR_wcshash((const wchar_t*)key);
}

static ar_int_t		__item_comp_func(void *l, void *r, void *ctx)
{
        AR_ASSERT(l != NULL && r != NULL);
        AR_UNUSED(ctx);
        return AR_wcsicmp((const wchar_t*)l, (const wchar_t*)r);
}


arHash_t*		AR_CreateURIQueryTable()
{
		return AR_CreateHash(31, __item_hash_func, __item_comp_func, __item_copy_func, __item_copy_func, __item_destroy_func, __item_destroy_func, NULL);
}

void			AR_DestroyURIQueryTable(arHash_t *tbl)
{
		AR_ASSERT(tbl != NULL);
		AR_DestroyHash(tbl);
		tbl = NULL;
}



static arStatus_t __parse_query_items(const wchar_t *query, arHash_t *hash)
{
        arStatus_t  status;

        wchar_t *name, *value;
        const wchar_t *b, *p;
        size_t l;                
        AR_ASSERT(query != NULL && hash != NULL);
		status = AR_S_YES;
        
        l = AR_wcslen(query);
        
        if(l == 0)
        {
                return status;
        }
        
        name = AR_NEWARR(wchar_t, l + 1);
        value = AR_NEWARR(wchar_t, l + 1);
        
        if(name == NULL || value == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        
        b = NULL;
        p = AR_wcstrim_space(query);
        

		while(p != NULL && *p != L'\0')
        {
                b = p;
                p = AR_wcsstr(b, L"=");
                if(p == NULL || p == b)
                {
                        status = AR_E_MALFORMAT;
                        goto END_POINT;
                }
                AR_wcsncpy(name, b, p-b);
                name[p-b] = L'\0';

                ++p;
                if(*p == L'\0')
                {
                        status = AR_E_MALFORMAT;
                        goto END_POINT;
                }

                
                b = p;
                p = AR_wcsstr(p, L"&");
                
                if(p == NULL)
                {
                        p = b + AR_wcslen(b);
                }
                 
                if(p == b)/*形如?x=  */
                {
                        status = AR_E_MALFORMAT;
                        goto END_POINT;
                }

                AR_wcsncpy(value, b, p - b);
                value[p-b] = L'\0';
                
				if(*p == L'&')
                {
                        ++p;
                }
                /*
				AR_DPRINT(AR_ERR_DEBUG, L"insert query : '%ls : %ls'\r\n", name, value);
				*/

                if(AR_InsertToHash(hash, (void*)name, (void*)value) != AR_S_YES)
                {
                        status = AR_E_NOMEM;
                        goto END_POINT;
                }
        }

END_POINT:
        if(name)
        {
                AR_DEL(name);
                name = NULL;
        }
        
        if(value)
        {
                AR_DEL(value);
                value = NULL;
        }

        return status;
}



static arStatus_t __generate_query(const arHash_t *hash, arString_t *out)
{
		arStatus_t status;
		arHashIter_t	iter; 
		size_t cnt;

		AR_ASSERT(hash != NULL && out != NULL);

		status = AR_S_YES;
		cnt = 0;
		AR_InitHashIterator((arHash_t*)hash, &iter);

		AR_ClearString(out);
		/*
		status = AR_AppendString(out, L"?");
		if(status != AR_S_YES)
		{
				goto END_POINT;
		}
		*/

		
		while(!AR_HashIteratorIsDone(&iter))
		{
				const wchar_t *key = (const wchar_t*)AR_GetHashIteratorKey(&iter);
				const wchar_t *value = (const wchar_t*)AR_GetHashIteratorData(&iter);
				AR_ASSERT(key != NULL && value != NULL);

				status = AR_AppendFormatString(out, L"%ls=%ls", key, value);
				if(status != AR_S_YES)
				{
						goto END_POINT;
				}
				cnt++;
				AR_HashIteratorNext(&iter);

				if(!AR_HashIteratorIsDone(&iter))
				{
						status = AR_AppendString(out, L"&");
						if(status != AR_S_YES)
						{
								goto END_POINT;
						}
				}

		}

		if(cnt == 0)
		{
				AR_ClearString(out);
		}

END_POINT:
		AR_UnInitHashIterator(&iter);
		return status;

}

arStatus_t		AR_GetURIQueryItems(const arURI_t *uri, arHash_t *hash)
{
		arStatus_t		status;
		const wchar_t *query;
		AR_ASSERT(uri != NULL && hash != NULL);
		status = AR_S_YES;
		AR_ClearHash(hash);

		query = AR_GetStringCString(uri->query);

		if(query == NULL || AR_wcslen(query) == 0)
		{
				goto END_POINT;
		}

		status = __parse_query_items(query, hash);

END_POINT:
		return status;
}

arStatus_t		AR_SetURIQueryItems(arURI_t *uri, const arHash_t *hash)
{
		arStatus_t		status;
		status = AR_S_YES;
		AR_ClearString(uri->query);
		status = __generate_query(hash, uri->query);
		return status;
}






/**********************************************************URLEncoder && URLDecoder*************************************************/

static arStatus_t	__url_encode(arCodePage_t cp, const wchar_t *begin, const wchar_t *end, arString_t *output)
{
		arStatus_t status;
		const char *s;
		char *str;
		
		AR_ASSERT(begin != NULL && end != NULL && begin <= end && output != NULL);
		status = AR_S_YES;

		if(begin == end)
		{
				return AR_S_YES;
		}

		str = AR_wcs_to_str(cp, begin, end - begin);
		
		if(str == NULL)
		{
				status = AR_E_BADENCCONV;
				goto END_POINT;
		}
		


		for(s = str; *s != '\0'; ++s)
		{
				ar_uint_8_t c = (ar_uint_8_t)*s;

				if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
						status = AR_AppendCharToString(output, (wchar_t)c);
						if(status != AR_S_YES)
						{
								goto END_POINT;
						}
				}else
				{
						status = AR_AppendFormatString(output, L"%%%02X", (ar_uint_32_t)c);
						if(status != AR_S_YES)
						{
								goto END_POINT;
						}
				}
		}

END_POINT:
		if(str)
		{
				AR_DEL(str);
				str = NULL;
		}

		return status;
}



arStatus_t		AR_EncodeURLString(arCodePage_t cp, const wchar_t *uri, arString_t *out)
{
		AR_ASSERT(uri != NULL && out != NULL);
		AR_ClearString(out);
		
		return __url_encode(cp, uri, uri + AR_wcslen(uri), out);
}


arStatus_t		AR_DecodeURLString(arCodePage_t cp, const wchar_t *uri, arString_t *out)
{
		
		AR_ASSERT(uri != NULL && out != NULL);
		AR_ClearString(out);
		/*uri decode兼容url encode*/
		return __decode(cp, uri, uri + AR_wcslen(uri), out);
}


/***************************************************************************************************************************/







AR_NAMESPACE_END

