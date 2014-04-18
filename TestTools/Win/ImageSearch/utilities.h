#pragma once

#include "stdhdr.h"



std::string		UTIL_StringConvert(arCodePage_t cp, const wchar_t *wcs);
std::string		UTIL_StringConvert(arCodePage_t cp, const std::wstring &wstr);
std::wstring	UTIL_StringConvert(arCodePage_t cp, const char *s);
std::wstring	UTIL_StringConvert(arCodePage_t cp, const std::string &str);

#define			UTIL_WToUTF8(_w)		UTIL_StringConvert(AR_CP_UTF8, (_w))
#define			UTIL_UTF8ToW(_utf8)		UTIL_StringConvert(AR_CP_UTF8, (_utf8))

arStatus_t		UTIL_GetAbsolutePath(const wchar_t *path, std::wstring &ret);

std::wstring	UTIL_GetModulePath();