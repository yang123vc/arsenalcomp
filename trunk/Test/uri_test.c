
 
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



static void uri_chinese_host_test()
{
		arURI_t	*uri;
		arStatus_t status;
		status = AR_S_YES;
		arString_t *str;

		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		
		AR_ASSERT(uri != NULL && str != NULL);

		/*******************************************************************************/

		
		status = AR_SetURI(uri, L"http://中文.com/abc");
		AR_ASSERT(status == AR_S_YES);

		AR_GetURI(uri, str);
		const wchar_t *s = AR_GetStringCString(str);
		AR_printf(L"%ls\r\n", s);

		AR_GetURIHost(uri, str);
		s = AR_GetStringCString(str);
		AR_printf(L"%ls\r\n", s);


		AR_ASSERT(!AR_IsRelativeURI(uri));

		

		/*******************************************************************************/
		AR_DestroyURI(uri);
		uri = NULL;
		AR_DestroyString(str);
		str = NULL;
}


static void work_test()
{
		AR_printf(L"\r\n\r\n---------------------\r\n");
		static struct {
				const wchar_t *original;
				const wchar_t *normalized;
		}tbl[] = 
		{
				{L"http://a.b.c.d.com", L"http://a.b.c.d.com"},
				{L"http://a.b.c.d.com.", L"http://a.b.c.d.com"},
				{L"http://a.b.c.d.com/", L"http://a.b.c.d.com/"},

				{L"http://a...b.c.d.com", L"http://a.b.c.d.com"},
				{L"http://a..b.c.d.com", L"http://a.b.c.d.com"},
				{L"http://.a.b.c.d.com", L"http://a.b.c.d.com"},
				{L"http://.a.b.c.d.com.", L"http://a.b.c.d.com"},
				{L"http://.a.b.c.d.com./", L"http://a.b.c.d.com/"},

				{L"http://a.b.c.d.com.//?C=1&a=2&&d=4&sid=pp&B=3&", L"http://a.b.c.d.com/?a=2&b=3&c=1&d=4"},
				{L"http://www.google.com/q?", L"http://www.google.com/q?r?"},
				{L"http://www.google.com/q?r?s", L"http://www.google.com/q?r?s"},

		};

		arURI_t	*uri;
		arStatus_t status;
		status = AR_S_YES;
		arString_t *str;
		const wchar_t *s = NULL;
		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		AR_ASSERT(uri != NULL && str != NULL);

		/*******************************************************************************/
		AR_printf(L"%ls\r\n", L"begin test");
		for(size_t i = 0; i < AR_NELEMS(tbl); ++i)
		{
				status = AR_SetURI(uri, tbl[i].original);
				AR_ASSERT(status == AR_S_YES);
				
				
				status =  AR_GetURI(uri, str);

				AR_ASSERT(status == AR_S_YES);

				s = AR_GetStringCString(str);

				AR_printf(L"before normalize : %ls\r\n", s);




				status = AR_NormalizeURI(uri);
				AR_ASSERT(status == AR_S_YES);

				status =  AR_GetURI(uri, str);

				AR_ASSERT(status == AR_S_YES);

				s = AR_GetStringCString(str);

				AR_printf(L"normalized : %ls\r\n", s);

				//AR_ASSERT(!AR_IsRelativeURI(uri));
		}
		
		AR_printf(L"%ls\r\n", L"end test");
		/*******************************************************************************/
		AR_DestroyURI(uri);
		uri = NULL;
		AR_DestroyString(str);
		str = NULL;
}



static void work_test2()
{

		AR_printf(L"\r\n\r\n---------------------\r\n");
		static struct {
				const wchar_t *original;
				const wchar_t *normalized;
		}tbl[] = 
		{
				{L"http://%31%36%38%2e%31%38%38%2e%39%39%2e%32%36/%2E%73%65%63%75%72%65/%77%77%77%2E%65%62%61%79%2E%63%6F%6D/", L"http://168.188.99.26/.secure/www.ebay.com/"},

				{L"http://195.127.0.11/uploads/%20%20%20%20/.verify/.eBaysecure=updateuserdataxplimnbqmn-xplmvalidateinfoswqpcmlx=hgplmcx/", L"http://195.127.0.11/uploads/    /.verify/.ebaysecure=updateuserdataxplimnbqmn-xplmvalidateinfoswqpcmlx=hgplmcx/"},

				//{L"http://host%23.com/%257Ea%2521b%2540c%2523d%2524e%25f%255E00%252611%252A22%252833%252944_55%252B", L"http://host%23.com/~a!b@c%23d$e%25f^00&11*22(33)44_55+"},
		};
		
		//http%3A%2F%2Fhost%2523%2Ecom%2F%7Ea%21b%40c%2523d%24e%2525f%5E00%2611%2A22%2833%2944%5F55%2B
		arURI_t	*uri;
		arStatus_t status;
		status = AR_S_YES;
		arString_t *str;
		const wchar_t *s = NULL;
		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		AR_ASSERT(uri != NULL && str != NULL);

		/*******************************************************************************/
		AR_printf(L"%ls\r\n", L"begin test");
		for(size_t i = 0; i < AR_NELEMS(tbl); ++i)
		{
				status = AR_DecodeURLString(AR_CP_UTF8, tbl[i].original, str);
				AR_ASSERT(status == AR_S_YES);

				s = AR_GetStringCString(str);
				AR_printf(L"%ls\r\n", s);

				
				arString_t *str2 = AR_CreateString();

				status = AR_DecodeURLString(AR_CP_UTF8, s, str2);
				AR_ASSERT(status == AR_S_YES);
				s = AR_GetStringCString(str2);
				AR_printf(L"%ls\r\n", s);
				AR_printf(L"---------------------------------------\r\n");

				AR_DestroyString(str2);
				str2 = NULL;
				
		}
		
		AR_printf(L"%ls\r\n", L"end test");
		/*******************************************************************************/
		AR_DestroyURI(uri);
		uri = NULL;
		AR_DestroyString(str);
		str = NULL;



		/*

                    # test delete  \r \n \t
                    ('http://a.b.c.d.com.//\r\n/\t/', http://a.b.c.d.com/),
                    ("http://www.google.com/foo\tbar\rbaz\n2", http://www.google.com/foobarbaz2),

                    # punycode test，保证测试的中文是 gbk 编码
                    ("http://测试.com/", http://xn--0zwm56d.com/),

                    # query
                    ('http://a.b.c.d.com.//?C=1&a=2&&d=4&sid=pp&B=3&', http://a.b.c.d.com/?a=2&b=3&c=1&d=4),
                    ("http://www.google.com/q?", http://www.google.com/q),
                    ("http://www.google.com/q?r?", http://www.google.com/q?r?),
                    ("http://www.google.com/q?r?s", http://www.google.com/q?r?s),

                    # host2ip
                    ("http://3279880203/blah", http://195.127.0.11/blah),

                    # decode
                

                    ("", ),

                    ("http://host%23.com/%257Ea%2521b%2540c%2523d%2524e%25f%255E00%252611%252A22%252833%252944_55%252B", ),

                    #   relative path
                    ("http://www.google.com/blah/..", http://www.google.com/),
                    ("http://www.google.com.../", http://www.google.com/),

                    # fragment
                    ("http://www.evil.com/blah#frag", http://www.evil.com/blah),
                    ("http://evil.com/foo#bar#baz", http://evil.com/foo),

                    # upper to lower
                    ("http://www.GOOgle.com", http://www.google.com/),

                    ("http://evil.com/foo;", http://evil.com/foo;),

                    ("http://evil.com/foo?bar;", http://evil.com/foo?bar;),

                    #
                    ("http://\x01\x80.com/", http://%01%80.com/),
                    ("http://notrailingslash.com", http://notrailingslash.com/),
                    ("http://www.gotaport.com:1234/", http://www.gotaport.com:1234/),
                    ("  http://www.google.com/  ", http://www.google.com/),
                    ("http:// leadingspace.com/", http://%20leadingspace.com/),
                    ("http://%20leadingspace.com/", http://%20leadingspace.com/),
                    ("https://www.securesite.com/", https://www.securesite.com/),
                    ("http://host.com/ab%23cd", http://host.com/ab%23cd),
                    ("http://host.com//twoslashes?more//slashes", http://host.com/twoslashes?more//slashes),
                    ("http://host/%25%32%35", http://host/%25),
                    ("http://host/%25%32%35%25%32%35", http://host/%25%25),
                    ("http://host/%2525252525252525", http://host/%25),
                    ("http://host/asdf%25%32%35asd", http://host/asdf%25asd),
                    ("http://host/%%%25%32%35asd%%", http://host/%25%25%25asd%25%25),
                    ("http://www.google.com/", http://www.google.com/),
                    ("http://%31%36%38%2e%31%38%38%2e%39%39%2e%32%36/%2E%73%65%63%75%72%65/%77%77%77%2E%65%62%61%79%2E%63%6F%6D/", http://168.188.99.26/.secure/www.ebay.com/),

		*/

}

static void __print_query_items(const arHash_t *hash)
{
		arHashIter_t	iter; 

		AR_ASSERT(hash != NULL);


		AR_InitHashIterator((arHash_t*)hash, &iter);


		while(!AR_HashIteratorIsDone(&iter))
		{
				const wchar_t *key = (const wchar_t*)AR_GetHashIteratorKey(&iter);
				const wchar_t *value = (const wchar_t*)AR_GetHashIteratorData(&iter);
				AR_ASSERT(key != NULL && value != NULL);
				AR_printf(L"%ls=%ls\r\n", key, value);
				AR_HashIteratorNext(&iter);
		}

END_POINT:
		AR_UnInitHashIterator(&iter);

}

static void __query_table_test()
{
		arURI_t	*uri;
		arStatus_t status;
		status = AR_S_YES;
		arString_t *str;
		arHash_t		*query_items;
		uri = AR_CreateURI(AR_CP_UTF8);
		str = AR_CreateString();
		query_items = AR_CreateURIQueryTable();
		AR_ASSERT(uri != NULL && str != NULL && query_items != NULL);

		/*******************************************************************************/

		{
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_ClearHash(query_items);
				status = AR_SetURI(uri, L"https://www.google.com.hk/search?hl=en&safe=off&client=aff-cs-360chromium&hs=KZr&q=http+%2F%2Fandroid.hkjc.com&oq=http%3A%2F&gs_l=serp.3.1.0l6j0i10j0l3.269398.272080.0.274290.16.9.0.0.0.0.369.995.0j2j1j1.5.0...0.0...1c.iXo9HsOWgZg");
				AR_ASSERT(status == AR_S_YES);


				status = AR_GetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);
				__print_query_items(query_items);

				status = AR_SetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);

				AR_printf(L"%ls\r\n", L"-------------------------------");
				status = AR_GetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);
				__print_query_items(query_items);
		}


		{
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_ClearHash(query_items);
				status = AR_SetURI(uri, L"https://www.google.com.hk/search?hl=en&safe=off&client=aff-cs-360chromium&hs=KZr&q=http+%2F%2Fandroid.hkjc.com&oq=http%3A%2F&");
				AR_ASSERT(status == AR_S_YES);


				status = AR_GetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);
				__print_query_items(query_items);

				status = AR_SetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);

				AR_printf(L"%ls\r\n", L"-------------------------------");
				status = AR_GetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);
				__print_query_items(query_items);
		}


		{
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_ClearHash(query_items);
				status = AR_SetURI(uri, L"https://www.google.com.hk/search?hl=en&safe=off");
				AR_ASSERT(status == AR_S_YES);


				status = AR_GetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);
				__print_query_items(query_items);

				status = AR_SetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);

				AR_printf(L"%ls\r\n", L"-------------------------------");
				status = AR_GetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);
				__print_query_items(query_items);
		}


		{
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_ClearHash(query_items);
				status = AR_SetURI(uri, L"https://www.google.com.hk/search?hl=en&");
				AR_ASSERT(status == AR_S_YES);


				status = AR_GetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);
				__print_query_items(query_items);

				status = AR_SetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);

				AR_printf(L"%ls\r\n", L"-------------------------------");
				status = AR_GetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);
				__print_query_items(query_items);
		}


		{
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_ClearHash(query_items);
				status = AR_SetURI(uri, L"https://www.google.com.hk/search?&");
				AR_ASSERT(status == AR_S_YES);
		}


		{
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_printf(L"%ls\r\n", L"-------------------------------");
				AR_ClearHash(query_items);
				status = AR_SetURI(uri, L"https://www.google.com.hk/search?");
				AR_ASSERT(status == AR_S_YES);


				status = AR_GetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);
				__print_query_items(query_items);

				status = AR_SetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);

				AR_printf(L"%ls\r\n", L"-------------------------------");
				status = AR_GetURIQueryItems(uri, query_items);
				AR_ASSERT(status == AR_S_YES);
				__print_query_items(query_items);
		}

		/*******************************************************************************/

		AR_DestroyURIQueryTable(query_items);
		query_items = NULL;

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

		uri_chinese_host_test();

		work_test();
		work_test2();

		__query_table_test();
}






AR_NAMESPACE_END
