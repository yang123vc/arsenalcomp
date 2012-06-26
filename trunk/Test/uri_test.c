
 
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


#include "../Arsenal/Common/common.h"


AR_NAMESPACE_BEGIN

		


static void uri_parse_test()
{
		arURI_t	*uri;
		arStatus_t status;
		
		status = AR_S_YES;
		arString_t *str;

		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		
		AR_ASSERT(uri != NULL);


		/*******************************************************************************/
		status = AR_SetURI(uri, L"http://www.appinf.com");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);


		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"www.appinf.com") == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_GetStringLength(str) == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_GetStringLength(str) == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_GetStringLength(str) == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));


		/*******************************************************************************/
		status = AR_SetURI(uri, L"http://www.appinf.com/");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"www.appinf.com") == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_GetStringLength(str) == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_GetStringLength(str) == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));





		/*******************************************************************************/
		status = AR_SetURI(uri, L"ftp://anonymous@ftp.appinf.com/pub/");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"ftp") == 0);
		
		AR_ASSERT(AR_GetURIPort(uri) == 21);

		AR_ASSERT(AR_GetURIUserInfo(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"anonymous") == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"anonymous@ftp.appinf.com") == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/pub/") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_GetStringLength(str) == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_GetStringLength(str) == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));



		/*******************************************************************************/
		status = AR_SetURI(uri, L"https://www.appinf.com/index.html#top");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"https") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 443);

		AR_ASSERT(AR_GetURIUserInfo(uri, str) == AR_S_YES);
		AR_ASSERT(AR_GetStringLength(str) == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/index.html") == 0);

		
		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"top") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));


		
		/*******************************************************************************/
		status = AR_SetURI(uri, L"http://www.appinf.com/search.cgi?keyword=test&scope=all");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 80);


		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/search.cgi") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"keyword=test&scope=all") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));

		/*******************************************************************************/
		status = AR_SetURI(uri, L"http://www.appinf.com/search.cgi?keyword=test&scope=all#result");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 80);


		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/search.cgi") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"keyword=test&scope=all") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"result") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));


		/*******************************************************************************/
		status = AR_SetEncodedURI(uri, L"http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 80);


		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/search.cgi") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"keyword=test encoded&scope=all") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"result") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));


		/*******************************************************************************/
		status = AR_SetURI(uri, L"ldap://[2001:db8::7]/c=GB?objectClass?one");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"ldap") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"2001:db8::7") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 389);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"[2001:db8::7]") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/c=GB") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"objectClass?one") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));


		/*******************************************************************************/
		status = AR_SetURI(uri, L"mailto:John.Doe@example.com");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"mailto") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"John.Doe@example.com") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));

		
		/*******************************************************************************/
		status = AR_SetURI(uri, L"tel:+86-010-6970-1111");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"tel") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"+86-010-6970-1111") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));



		/*******************************************************************************/
		status = AR_SetURI(uri, L"telnet://192.0.2.16:80");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"telnet") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"192.0.2.16") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 80);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"192.0.2.16:80") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));


		/*******************************************************************************/
		status = AR_SetURI(uri, L"urn:oasis:names:specification:docbook:dtd:xml:4.1.2");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"urn") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"oasis:names:specification:docbook:dtd:xml:4.1.2") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));




		/*******************************************************************************/
		status = AR_SetURI(uri, L"");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);



		/*******************************************************************************/
		status = AR_SetURI(uri, L"/foo/bar");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/foo/bar") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_IsRelativeURI(uri));


		/*******************************************************************************/
		status = AR_SetURI(uri, L"./foo/bar");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"./foo/bar") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_IsRelativeURI(uri));


		/*******************************************************************************/
		status = AR_SetURI(uri, L"../foo/bar");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"../foo/bar") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_IsRelativeURI(uri));


		/*******************************************************************************/
		status = AR_SetURI(uri, L"index.html");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"index.html") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_IsRelativeURI(uri));


		/*******************************************************************************/
		status = AR_SetURI(uri, L"index.html#frag");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"index.html") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"frag") == 0);

		AR_ASSERT(AR_IsRelativeURI(uri));



		/*******************************************************************************/
		status = AR_SetURI(uri, L"?query=test");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"query=test") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_IsRelativeURI(uri));


		/*******************************************************************************/
		status = AR_SetURI(uri, L"?query=test#frag");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"query=test") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"frag") == 0);

		AR_ASSERT(AR_IsRelativeURI(uri));



		/*******************************************************************************/
		status = AR_SetURI(uri, L"#frag");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"frag") == 0);

		AR_ASSERT(AR_IsRelativeURI(uri));


		
		/*******************************************************************************/
		status = AR_SetURI(uri, L"#");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_IsRelativeURI(uri));




		/*******************************************************************************/
		status = AR_SetURI(uri, L"file:///a/b/c");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"file") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/a/b/c") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));



		/*******************************************************************************/
		status = AR_SetURI(uri, L"file://localhost/a/b/c");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"file") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"localhost") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"localhost") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/a/b/c") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));


		
		/*******************************************************************************/
		status = AR_SetURI(uri, L"file:///c:/Windows/system32/");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"file") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/c:/Windows/system32/") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(!AR_IsRelativeURI(uri));



		/*******************************************************************************/
		status = AR_SetURI(uri, L"./c:/Windows/system32/");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);



		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"./c:/Windows/system32/") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_IsRelativeURI(uri));

		AR_DestroyURI(uri);
		uri = NULL;
		AR_DestroyString(str);
		str = NULL;
}



static void uri_construct_test()
{

		arURI_t	*uri;
		arStatus_t status;
		
		status = AR_S_YES;
		arString_t *str;

		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		
		AR_ASSERT(uri != NULL);

		/**************************************************************************/
		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		
		/**************************************************************************/
		status = AR_SetURIScheme(uri, L"FTP");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"ftp") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 21);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"") == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		/**************************************************************************/
		AR_ClearURI(uri);
		status = AR_SetURIAuthority(uri, L"www.appinf.com");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"www.appinf.com") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 0);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"www.appinf.com") == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		/**************************************************************************/
		AR_ClearURI(uri);
		status = AR_SetURIAuthority(uri, L"user@services.appinf.com:8000");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIUserInfo(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"user") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"services.appinf.com") == 0);

		AR_ASSERT(AR_GetURIPort(uri) == 8000);

		status = AR_GetURIAuthority(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"user@services.appinf.com:8000") == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_SetURIPath(uri, L"/index.html");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/index.html") == 0);


		status = AR_SetURIEncodedPath(uri, L"/file%20with%20spaces.html");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/file with spaces.html") == 0);
		
		status = AR_SetURIPathEtc(uri, L"/query.cgi?query=foo");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/query.cgi") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"query=foo") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIPathEtc(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/query.cgi?query=foo") == 0);




		status = AR_SetURIPathEtc(uri, L"/query.cgi?query=bar#frag");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/query.cgi") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"query=bar") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"frag") == 0);

		status = AR_GetURIPathEtc(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/query.cgi?query=bar#frag") == 0);
		

		status = AR_SetURIQuery(uri, L"query=test");
		AR_ASSERT(status == AR_S_YES);
		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"query=test") == 0);

		status = AR_SetURIFragment(uri, L"result");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"result") == 0);

		status = AR_GetURIPathEtc(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/query.cgi?query=test#result") == 0);



		AR_DestroyURI(uri);
		uri = NULL;
		AR_DestroyString(str);
		str = NULL;
}


static void uri_tostring_test()
{
		arURI_t	*uri;
		arStatus_t status;
		
		status = AR_S_YES;
		arString_t *str;

		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		
		AR_ASSERT(uri != NULL && str != NULL);

		/**********************************************************************************/

		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com") == 0);


		
		AR_ASSERT(AR_SetURI(uri, L"ftp://anonymous@ftp.appinf.com/pub/") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"ftp://anonymous@ftp.appinf.com/pub/") == 0);


		AR_ASSERT(AR_SetURI(uri, L"ftp://anonymous@ftp.appinf.com/%e4%b8%ad%e5%9b%bd%e5%ad%97") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"ftp://anonymous@ftp.appinf.com/%e4%b8%ad%e5%9b%bd%e5%ad%97") == 0);

		AR_SetURICodePage(uri, AR_CP_GB2312);
		AR_ASSERT(AR_SetURI(uri, L"ftp://anonymous@ftp.appinf.com/%d6%d0%b9%fa%d7%d6") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"ftp://anonymous@ftp.appinf.com/%d6%d0%b9%fa%d7%d6") == 0);
		AR_SetURICodePage(uri, AR_CP_UTF8);


		AR_ASSERT(AR_SetEncodedURI(uri, L"ftp://anonymous@ftp.appinf.com/%e4%b8%ad%e5%9b%bd%e5%ad%97") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"ftp://anonymous@ftp.appinf.com/中国字") == 0);
		

		AR_ASSERT(AR_SetURI(uri, L"https://www.appinf.com/index.html#top") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"https://www.appinf.com/index.html#top") == 0);
		
	
		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/search.cgi?keyword=test&scope=all") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/search.cgi?keyword=test&scope=all") == 0);

		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/search.cgi?keyword=test&scope=all#result") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/search.cgi?keyword=test&scope=all#result") == 0);
		
		
		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result") == 0);


		AR_ASSERT(AR_SetURI(uri, L"ldap://[2001:db8::7]/c=GB?objectClass?one") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"ldap://[2001:db8::7]/c=GB?objectClass?one") == 0);


		AR_ASSERT(AR_SetURI(uri, L"mailto:John.Doe@example.com") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"mailto:John.Doe@example.com") == 0);


		AR_ASSERT(AR_SetURI(uri, L"tel:+1-816-555-1212") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"tel:+1-816-555-1212") == 0);

		AR_ASSERT(AR_SetURI(uri, L"telnet://192.0.2.16:80") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"telnet://192.0.2.16:80") == 0);



		AR_SetURICodePage(uri, AR_CP_GB2312);
		AR_ASSERT(AR_SetURI(uri, L"ftp://anonymous@ftp.appinf.com") == AR_S_YES);

		AR_ASSERT(AR_SetURIPath(uri, L"/中国字") == AR_S_YES);

		AR_ASSERT(AR_GetEncodedURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"ftp://anonymous@ftp.appinf.com/%d6%d0%b9%fa%d7%d6") == 0);
		AR_SetURICodePage(uri, AR_CP_UTF8);


		AR_ASSERT(AR_SetURI(uri, L"urn:oasis:names:specification:docbook:dtd:xml:4.1.2") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"urn:oasis:names:specification:docbook:dtd:xml:4.1.2") == 0);


		AR_ASSERT(AR_SetURI(uri, L"") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);
		

		AR_ASSERT(AR_SetURI(uri, L"/foo/bar") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/foo/bar") == 0);
		

		AR_ASSERT(AR_SetURI(uri, L"./foo/bar") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"./foo/bar") == 0);

		AR_ASSERT(AR_SetURI(uri, L"../foo/bar") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"../foo/bar") == 0);

		AR_ASSERT(AR_SetURI(uri, L"//foo/bar") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"//foo/bar") == 0);


		AR_ASSERT(AR_SetURI(uri, L"index.html") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"index.html") == 0);

		AR_ASSERT(AR_SetURI(uri, L"index.html#frag") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"index.html#frag") == 0);

		AR_ASSERT(AR_SetURI(uri, L"?query=test") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"?query=test") == 0);

		AR_ASSERT(AR_SetURI(uri, L"?query=test#frag") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"?query=test#frag") == 0);


		AR_ASSERT(AR_SetURI(uri, L"#frag") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"#frag") == 0);

		AR_ASSERT(AR_SetURI(uri, L"#") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);


		AR_ASSERT(AR_SetURI(uri, L"file:///a/b/c") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"file:///a/b/c") == 0);

		AR_ASSERT(AR_SetURI(uri, L"file://localhost/a/b/c") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"file://localhost/a/b/c") == 0);

		AR_ASSERT(AR_SetURI(uri, L"file:///c:/Windows/system32/") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"file:///c:/Windows/system32/") == 0);

		AR_ASSERT(AR_SetURI(uri, L"./c:/Windows/system32/") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"./c:/Windows/system32/") == 0);


		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com") == AR_S_YES);
		AR_ASSERT(AR_SetURIQuery(uri, L"query=test") == AR_S_YES);

		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/?query=test") == 0);

		/**********************************************************************************/

		AR_ASSERT(AR_SetURI(uri, L"http://www.example.com/List of holidays.xml") == AR_S_YES);
		AR_ASSERT(AR_GetEncodedURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.example.com/List%20of%20holidays.xml") == 0);


		AR_ASSERT(AR_SetEncodedURI(uri, L"http://qt.nokia.com/List%20of%20holidays.xml") == AR_S_YES);
		AR_ASSERT(AR_GetEncodedURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://qt.nokia.com/List%20of%20holidays.xml") == 0);
		
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://qt.nokia.com/List of holidays.xml") == 0);



		AR_SetURICodePage(uri, AR_CP_UTF8);
		AR_ASSERT(AR_SetURI(uri, L"http://www.example.com/List of holidays.xml?搜索1=关键字1a=b关键字2=搜索2") == AR_S_YES);
		AR_ASSERT(AR_GetEncodedURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.example.com/List%20of%20holidays.xml?%e6%90%9c%e7%b4%a21=%e5%85%b3%e9%94%ae%e5%ad%971a=b%e5%85%b3%e9%94%ae%e5%ad%972=%e6%90%9c%e7%b4%a22") == 0);

		
		AR_ASSERT(AR_SetEncodedURI(uri, L"http://www.example.com/List%20of%20holidays.xml?%e6%90%9c%e7%b4%a21=%e5%85%b3%e9%94%ae%e5%ad%971a=b%e5%85%b3%e9%94%ae%e5%ad%972=%e6%90%9c%e7%b4%a22") == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.example.com/List of holidays.xml?搜索1=关键字1a=b关键字2=搜索2") == 0);
		

		AR_DestroyURI(uri);
		uri = NULL;
		AR_DestroyString(str);
		str = NULL;
}

static void uri_compare_test()
{
		
		arURI_t	*u1, *u2;
		arString_t *str;
		arStatus_t status;
		status = AR_S_YES;

		u1 = AR_CreateURI(AR_CP_UTF8);
		u2 = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		AR_ASSERT(u1 != NULL && u2 != NULL && str != NULL);

		AR_SetURI(u1, L"http://www.appinf.com");
		AR_SetURI(u2, L"HTTP://www.appinf.com:80");

		AR_ASSERT(AR_CompURI(u1, u2) == 0);

		AR_ASSERT(AR_GetURI(u1, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com") == 0);

		AR_ASSERT(AR_GetURI(u2, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com") == 0);
		


		AR_SetURI(u1, L"?query");
		AR_ASSERT(AR_GetURI(u1, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"?query") == 0);
		AR_ASSERT(AR_CompStringWithWcs(str, L"?query2") != 0);


		AR_SetURI(u1, L"#frag");
		AR_ASSERT(AR_GetURI(u1, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"#frag") == 0);
		AR_ASSERT(AR_CompStringWithWcs(str, L"#frag2") != 0);


		AR_SetURI(u1, L"/index.html#frag");
		AR_ASSERT(AR_GetURI(u1, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/index.html#frag") == 0);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/index.html") != 0);


		AR_SetURICodePage(u1, AR_CP_GB2312);
		AR_SetURICodePage(u2, AR_CP_GB2312);
		AR_SetURI(u1, L"http://www.appinf.COM/pub/中国字");
		AR_SetEncodedURI(u2, L"HTTP://www.appinf.com:80/pub/%d6%d0%b9%fa%d7%d6");

		AR_ASSERT(AR_CompURI(u1, u2) == 0);

		AR_ASSERT(AR_GetURI(u1, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/pub/中国字") == 0);

		AR_ASSERT(AR_GetURI(u2, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/pub/中国字") == 0);


		AR_SetURICodePage(u1, AR_CP_UTF8);
		AR_SetURI(u1, L"http://www.appinf.COM/pub/中国字");

		AR_SetURICodePage(u2, AR_CP_GB2312);
		AR_SetEncodedURI(u2, L"HTTP://www.appinf.com:80/pub/%d6%d0%b9%fa%d7%d6");


		AR_ASSERT(AR_CompURI(u1, u2) == 0);

		AR_SetURICodePage(u1, AR_CP_GB2312);
		AR_ASSERT(AR_GetEncodedURI(u1, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"HTTP://www.appinf.com/pub/%d6%d0%b9%fa%d7%d6") == 0);

		AR_SetURICodePage(u2, AR_CP_UTF8);
		AR_ASSERT(AR_GetURI(u2, str) == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://www.appinf.com/pub/中国字") == 0);

		AR_ASSERT(AR_GetEncodedURI(u2, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/pub/%e4%b8%ad%e5%9b%bd%e5%ad%97") == 0);


		
		
		AR_DestroyURI(u1);
		AR_DestroyURI(u2);
		AR_DestroyString(str);
		u1 = NULL;
		u2 = NULL;
		str = NULL;
}


static void uri_normalize_test()
{

		arURI_t	*uri;
		arStatus_t status;
		
		status = AR_S_YES;
		arString_t *str;

		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		
		AR_ASSERT(uri != NULL && str != NULL);

		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com") == 0);


		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/") == 0);


		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/foo/bar/./index.html") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/foo/bar/index.html") == 0);


		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/foo/bar/../index.html") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/foo/index.html") == 0);


		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/foo/./bar/../index.html") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/foo/index.html") == 0);



		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/foo/./bar/../index.html") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/foo/index.html") == 0);


		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/foo/bar/../../index.html") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/index.html") == 0);



		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/foo/bar/../../../index.html") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/index.html") == 0);


		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/foo/bar/.././../index.html") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/index.html") == 0);



		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/./foo/bar/index.html") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/foo/bar/index.html") == 0);



		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/../foo/bar/index.html") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/foo/bar/index.html") == 0);


		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/../foo/bar/") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/foo/bar/") == 0);


		AR_ASSERT(AR_SetURI(uri, L"http://www.appinf.com/../foo/../") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"http://www.appinf.com/") == 0);


		AR_ASSERT(AR_SetURI(uri, L"file:///c:/Windows/system32/") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"file:///c:/Windows/system32/") == 0);

		AR_ClearURI(uri);
		AR_ASSERT(AR_SetURIPath(uri, L"c:/windows/system32/") == AR_S_YES);
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"./c:/windows/system32/") == 0);
		

		AR_ASSERT(AR_SetURI(uri, L"//.././../index.html") == AR_S_YES);
		AR_ASSERT(AR_IsRelativeURI(uri));
		AR_ASSERT(AR_NormalizeURI(uri) == AR_S_YES);
		AR_ASSERT(AR_GetURI(uri, str) == AR_S_YES);
		AR_ASSERT(AR_ICompStringWithWcs(str, L"/../../index.html") == 0);

		AR_DestroyURI(uri);
		uri = NULL;
		AR_DestroyString(str);
		str = NULL;
}

static void uri_misc_test()
{

		arURI_t	*uri;
		arStatus_t status;
		status = AR_S_YES;
		arString_t *str;

		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		
		AR_ASSERT(uri != NULL && str != NULL);


		/*******************************************************************************/
		status = AR_SetEncodedURI(uri, L"http://google.com/search?q=hello%25world#frag%20ment");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"google.com") == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/search") == 0);

		status = AR_GetURIEncodedQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"q=hello%25world") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"q=hello%world") == 0);


		status = AR_GetURIEncodedFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"frag%20ment") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"frag ment") == 0);

		status = AR_GetEncodedURI(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://google.com/search?q=hello%25world#frag%20ment") == 0);

		status = AR_GetURIEncodedPathEtc(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/search?q=hello%25world#frag%20ment") == 0);
		AR_ASSERT(!AR_IsRelativeURI(uri));




		/*******************************************************************************/
		status = AR_SetURIQuery(uri, L"q=goodbye cruel world");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"http") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"google.com") == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/search") == 0);

		status = AR_GetURIEncodedQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"q=goodbye%20cruel%20world") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"q=goodbye cruel world") == 0);


		status = AR_GetURIEncodedFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"frag%20ment") == 0);

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"frag ment") == 0);

		status = AR_GetEncodedURI(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://google.com/search?q=goodbye%20cruel%20world#frag%20ment") == 0);

		status = AR_GetURIEncodedPathEtc(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/search?q=goodbye%20cruel%20world#frag%20ment") == 0);
		AR_ASSERT(!AR_IsRelativeURI(uri));

		/*******************************************************************************/
		status = AR_SetURIEncodedQuery(uri, L"q=pony%7eride");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"q=pony~ride") == 0);

		status = AR_GetURIEncodedQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"q=pony~ride") == 0);


		status = AR_GetEncodedURI(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"http://google.com/search?q=pony~ride#frag%20ment") == 0);


		status = AR_GetURIEncodedPathEtc(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/search?q=pony~ride#frag%20ment") == 0);


		AR_ASSERT(!AR_IsRelativeURI(uri));




		/*******************************************************************************/
		status = AR_SetURI(uri, L"https://developer.apple.com/library/safari/#documentation/Tools/Conceptual/SafariExtensionGuide/ExtensionsOverview/ExtensionsOverview.html#//apple_ref/doc/uid/TP40009977-CH15-SW1");
		AR_ASSERT(status == AR_S_YES);

		status = AR_GetURIScheme(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_wcscmp(AR_GetStringCString(str), L"https") == 0);

		status = AR_GetURIHost(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"developer.apple.com") == 0);

		status = AR_GetURIPath(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"/library/safari/") == 0);

		status = AR_GetURIEncodedQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		status = AR_GetURIQuery(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"") == 0);

		
		status = AR_GetURIEncodedFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"documentation/Tools/Conceptual/SafariExtensionGuide/ExtensionsOverview/ExtensionsOverview.html#//apple_ref/doc/uid/TP40009977-CH15-SW1") == 0);
		

		status = AR_GetURIFragment(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"documentation/Tools/Conceptual/SafariExtensionGuide/ExtensionsOverview/ExtensionsOverview.html#//apple_ref/doc/uid/TP40009977-CH15-SW1") == 0);

		
		status = AR_GetEncodedURI(uri, str);
		AR_ASSERT(status == AR_S_YES);
		AR_ASSERT(AR_CompStringWithWcs(str, L"https://developer.apple.com/library/safari/#documentation/Tools/Conceptual/SafariExtensionGuide/ExtensionsOverview/ExtensionsOverview.html#//apple_ref/doc/uid/TP40009977-CH15-SW1") == 0);
		


		
		AR_DestroyURI(uri);
		uri = NULL;
		AR_DestroyString(str);
		str = NULL;

}

static void uri_exception_test()
{
		arURI_t	*uri;
		arStatus_t status;
		status = AR_S_YES;
		arString_t *str;

		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		
		AR_ASSERT(uri != NULL && str != NULL);


		/*******************************************************************************/
		status = AR_SetEncodedURI(uri, L"http://google.com/search?q=hello%25world#frag%0ment");
		AR_ASSERT(status != AR_S_YES);

		status = AR_SetURI(uri, L"http:///searchq=hello%2world#frag%0ment");
		AR_ASSERT(status == AR_S_YES);

		AR_GetURI(uri, str);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));



		/*******************************************************************************/
		status = AR_SetEncodedURI(uri, L"itunes.apple.com/cn/app/id523405218?mt=8");
		AR_ASSERT(status == AR_S_YES);

		AR_GetURI(uri, str);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));



		/*******************************************************************************/
		status = AR_SetEncodedURI(uri, L"		http:////itunes.apple.com/cn/app/id523405218?mt=8				");
		AR_ASSERT(status == AR_S_YES);

		AR_GetURI(uri, str);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));

		AR_ASSERT(!AR_IsRelativeURI(uri));
		
		/*******************************************************************************/
		status = AR_SetEncodedURI(uri, L"		http:////itunes			.apple.com/cn/app/id523405218?mt=8				");
		AR_ASSERT(status == AR_S_YES);

		AR_GetURI(uri, str);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));

		AR_ASSERT(!AR_IsRelativeURI(uri));
		

		/*******************************************************************************/
		status = AR_SetEncodedURI(uri, L"		http://user@/cn/app/id523405218?mt=8				");
		AR_ASSERT(status == AR_S_YES);

		AR_GetURI(uri, str);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));

		AR_ASSERT(!AR_IsRelativeURI(uri));



		/*******************************************************************************/
		AR_DestroyURI(uri);
		uri = NULL;
		AR_DestroyString(str);
		str = NULL;
}


static void uri_misc_test2()
{
		arURI_t	*uri;
		arStatus_t status;
		status = AR_S_YES;
		arString_t *str;

		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		
		AR_ASSERT(uri != NULL && str != NULL);

		/*******************************************************************************/

		status = AR_SetURI(uri, L"pdown://p2=663AD0730A4071E5AF91D8CEC70998357A804A14");
		AR_ASSERT(status == AR_S_YES);

		AR_GetURI(uri, str);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));

		AR_ASSERT(!AR_IsRelativeURI(uri));

		

		/*******************************************************************************/
		AR_DestroyURI(uri);
		uri = NULL;
		AR_DestroyString(str);
		str = NULL;
}

void uri_test()
{
		uri_construct_test();
		uri_parse_test();
		uri_tostring_test();
		uri_compare_test();
		uri_normalize_test();
		uri_misc_test();
		uri_exception_test();

		uri_misc_test2();
}






AR_NAMESPACE_END
