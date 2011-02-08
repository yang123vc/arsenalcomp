
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
		tguParseResult_t		parse_result;
		tguParser_t				*parser;
		const wchar_t			*code = NULL;
		tguSymbTbl_t			*build_in	= TGU_CreateSymbTable();
		tguSymbTbl_t			*import_models = TGU_CreateSymbTable();


		parser = TGU_CreateParser(&__g_report, build_in, import_models); 
		
		code = __load_txt(L"..\\..\\..\\misc\\tengu_stmt.txt");

		AR_printf(L"%ls\r\n", code);

		tguSymb_t		*model_symb = TGU_CreateSymb(TGU_SYMB_BLOCK_T, L"tengu_input");
		TGU_InsertToSymbTable(import_models, model_symb);

		parse_result = TGU_ParseCode(parser, L"tengu_input", code);
		model_symb->block = parse_result.block;

		if(parse_result.has_error)
		{
				TGU_RemoveFromSymbTable(import_models, model_symb->name, model_symb->type);
		}else
		{
				AR_ASSERT(parse_result.block != NULL);
		}


		getchar();


		

		AR_DEL(code);
		
		TGU_DestroyParser(parser);

		TGU_DestroySymbTable(build_in);
		TGU_DestroySymbTable(import_models);
}




void	Tengu_Test()
{
		test1();


}

AR_NAMESPACE_END

#endif

