
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


void test1()
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


/*

typedef enum 
{
		TGU_SYMB_NULL_T,
		TGU_SYMB_INT_T,
		TGU_SYMB_FLOAT_T,
		TGU_SYMB_BOOL_T,
		TGU_SYMB_STRING_T,

		TGU_SYMB_VAR_T,
		TGU_SYMB_FUNC_T,
		TGU_SYMB_CFUNC_T,
		TGU_SYMB_BLOCK_T
}tguSymbType_t;
*/
void test2()
{
		tguConstant_t const_val;
		wchar_t *name;
		tguSymb_t *symb_boolean, *symb_null, *symb_float, *symb_int, *symb_string;

		const_val.boolean = true;
		
		name = TGU_ConstantToName(TGU_SYMB_BOOL_T, &const_val);
		symb_boolean = TGU_CreateSymb(TGU_SYMB_BOOL_T, name);

		AR_DEL(name);
		name = NULL;
		
}

void	Tengu_Test()
{
		//test1();


}

AR_NAMESPACE_END

#endif
