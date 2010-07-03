#ifndef __ARSENAL_TEST_H__
#define __ARSENAL_TEST_H__


#define _CRT_SECURE_NO_WARNINGS 1
#include "../Arsenal/Common/common.h"
#include <time.h>

/*
#if defined(__LIB)


#if(AR_COMPILER == AR_VC_LEGACY)
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



#define LOAD_TXT_MAX_BUF  (1024*1024 * 1024)
static  AR_INLINE const wchar_t* __load_txt(const wchar_t *path)
{
		FILE *pf;
		byte_t	*buf;
		size_t rn;
		const wchar_t *ret;
		buf = AR_NEWARR(byte_t, LOAD_TXT_MAX_BUF);

		AR_ASSERT(path != NULL);
		
		//pf = fopen("..\\..\\..\\misc\\book1.gmr", "r");

		pf = _wfopen(path, L"r");

		AR_ASSERT(pf != NULL);
		rn = fread((void*)buf, 1, LOAD_TXT_MAX_BUF, pf);
		fclose(pf);
		AR_ASSERT(rn > 3);
		//printf("read size == %d\r\n", rn);
		buf[rn] = 0;

		AR_ASSERT(buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF);

		//printf("%s\r\n", buf + 3);
		

		ret = AR_utf8_convto_wcs((const char*)(buf + 3));

		AR_DEL(buf);
		return ret;
}


void AR_Test();


AR_NAMESPACE_END

#endif

