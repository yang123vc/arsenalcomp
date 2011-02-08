
#include "test.h"

#include "../Arsenal/Tengu/tengu.h"
#include "../Arsenal/Tengu/tguparser.h"


#if defined(__LIB)

AR_NAMESPACE_BEGIN


void AR_STDCALL tengu_test_report(const tguReportInfo_t *report, void *context)
{
		switch(report->type)
		{
		default:
				AR_ASSERT(false);
				break;
		case TGU_REPORT_MESSAGE_T:
		case TGU_REPORT_WARNING_T:
		case TGU_REPORT_ERROR_T:
				printf("%ls : %d\r\n", report->message, report->line);
				break;
		}
}



static tguReport_t		__g_report = 
{
		tengu_test_report,
		NULL
};


void test1()
{
		tguParser_t		*parser;
		const wchar_t	*code = NULL;
		tguSymbTbl_t	*build_in	= TGU_CreateSymbTable();

		parser = TGU_CreateParser(&__g_report, build_in, NULL); 
		
		code = __load_txt(L"..\\..\\..\\misc\\tengu_stmt.txt");

		AR_printf(L"%ls\r\n", code);

		tguBlock_t		*block = TGU_ParseCode(parser, L"tengu_input", code);


		getchar();


		

		AR_DEL(code);
		
		TGU_DestroyParser(parser);
}




void	Tengu_Test()
{
		test1();


}

AR_NAMESPACE_END

#endif

