#ifndef __ARSENAL_TEST_H__
#define __ARSENAL_TEST_H__


#define _CRT_SECURE_NO_WARNINGS 1
#include "../Arsenal/Common/common.h"


/*
#if defined(__LIB)


#if(AR_COMPILER == AR_VC6)
struct _RPC_ASYNC_STATE;
#endif


#include <windows.h>

inline unsigned __int64 GetTime()
{
		FILETIME ft;
		ULARGE_INTEGER ts;
		ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME

		GetSystemTimeAsFileTime(&ft);
		epoch.LowPart  = 0xD53E8000;
		epoch.HighPart = 0x019DB1DE;

		ts.LowPart  = ft.dwLowDateTime;
		ts.HighPart = ft.dwHighDateTime;
		ts.QuadPart -= epoch.QuadPart;
		return ts.QuadPart/10 ;
}


#endif
*/

AR_NAMESPACE_BEGIN

void AR_Test();


AR_NAMESPACE_END

#endif

