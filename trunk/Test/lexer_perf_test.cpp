#include "test.h"




#if defined(__LIB)


AR_NAMESPACE_BEGIN



static void perf_for_large_xml()
{
		arString_t *str = AR_CreateString();
		arTxtBom_t bom;
		arStatus_t status;

		status = AR_LoadBomTextFileWithCodePage(L"C:\\Users\\solidus\\Desktop\\rule_6_level_28.xml", &bom, str, AR_CP_UTF8);
		AR_ASSERT(status == AR_S_YES);

		status = AR_SaveBomTextFileWithCodePage(L"C:\\Users\\solidus\\Desktop\\rule_6_level_28.txt", AR_TXT_BOM_NONE, AR_CSTR(str), AR_CP_GB2312);
		AR_ASSERT(status == AR_S_YES);


		if(str)
		{
				AR_DestroyString(str);
				str = NULL;
		}
}

void lex_perf_test()
{
		perf_for_large_xml();
}

AR_NAMESPACE_END



#endif

