
#include "test.h"

#include "../Arsenal/Tengu/tengu.h"
#include "../Arsenal/Tengu/tguparser.h"


#if defined(__LIB)

AR_NAMESPACE_BEGIN


void AR_STDCALL tengu_test_report(const tguReportInfo_t *report, void *context)
{

}



static tguReport_t		__g_report = 
{
		tengu_test_report,
		NULL
};

void	Tengu_Test()
{
		tguParser_t		*parser;
		const wchar_t	*code = NULL;
		parser = TGU_CreateParser(&__g_report, NULL); 
		
		code = __load_txt(L"..\\..\\..\\misc\\tengu_input.txt");

		AR_printf(L"%ls\r\n", code);

		TGU_ParseCode(parser, code);

		AR_DEL(code);
		
		TGU_DestroyParser(parser);
}

AR_NAMESPACE_END

#endif
