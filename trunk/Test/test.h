#ifndef __ARSENAL_TEST_H__
#define __ARSENAL_TEST_H__


#define _CRT_SECURE_NO_WARNINGS 1
#include "../Arsenal/Common/common.h"
#include <time.h>

AR_NAMESPACE_BEGIN



#define LOAD_TXT_MAX_BUF  (1024*1024 * 1024)
static  AR_INLINE const wchar_t* __load_txt(const wchar_t *path)
{
		FILE *pf;
		byte_t	*buf;
		size_t rn;
		const wchar_t *ret;
		size_t l = LOAD_TXT_MAX_BUF;
		buf = AR_NEWARR(byte_t, l);

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

