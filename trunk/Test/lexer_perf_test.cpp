#include "test.h"




#if defined(__LIB)


AR_NAMESPACE_BEGIN



static void perf_for_large_xml()
{
		arString_t *str = AR_CreateString();
		arString_t *line = AR_CreateString();

		arString_t *out_str = AR_CreateString();

		arTxtBom_t bom;
		arStatus_t status;

		status = AR_LoadBomTextFileWithCodePage(L"C:\\Users\\solidus\\Desktop\\rule_6_level_28.xml", &bom, str, AR_CP_UTF8);
		AR_ASSERT(status == AR_S_YES);

		while(!AR_IsEmptyString(str))
		{
				AR_GetLineFromString(str, line);
				//AR_printf(L"%ls\r\n", AR_CSTR(line));

				if(AR_IsEmptyString(line))
				{
						AR_DEBUG_BREAK();
				}

				AR_AppendString(out_str, AR_CSTR(line));
				AR_AppendString(out_str, L"\r\n-----------------------------------------------------------------\r\n\r\n");
		}

		status = AR_SaveBomTextFileWithCodePage(L"C:\\Users\\solidus\\Desktop\\rule_6_level_28.txt", AR_TXT_BOM_UTF_8, AR_CSTR(out_str), AR_CP_MAX);
		AR_ASSERT(status == AR_S_YES);

		if(str)
		{
				AR_DestroyString(str);
				str = NULL;
		}

		if(line)
		{
				AR_DestroyString(line);
				line = NULL;
		}

		if(out_str)
		{
				AR_DestroyString(out_str);
				out_str = NULL;
		}
}

void lex_perf_test()
{
		perf_for_large_xml();
}

AR_NAMESPACE_END



#endif

