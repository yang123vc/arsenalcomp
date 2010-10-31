

#if(0)
#include "test.h"
#include <iostream>
#include "Arsenal.h"
#pragma comment(lib, "Arsenal.lib")



//using namespace std;
using namespace ARSpace;



const wchar_t *test =
L"{"

L"		int i; int j;  float[100] a; float v; float x;"
L"	bool_t x; char[1024] carr;"

L"	while(true)"
L"	{"
L"		do i = i + 1; while(a[i] < v);"
L"		do j = j-1; while(a[j] > v);"
L"		if(i >= j)break;"
L"		x = a[i]; a[i] = a[j]; a[j] = x;"
L"	}"
L"}"
;


void AR_STDCALL tiny_error(int_t level, const wchar_t* msg, void *ctx)
{
#if(OS_TYPE == OS_WINDOWS_CE)
		::MessageBoxW(NULL, msg, L"MessageBox", 0);
#else
		printf("%ls", msg);
#endif

}

void AR_STDCALL tiny_printf(const wchar_t *msg, void *ctx)
{
#if(OS_TYPE == OS_WINDOWS_CE)
		::MessageBoxW(NULL, msg, L"MessageBox", 0);
#else
		printf("%ls", msg);
#endif
}




#if(OS_TYPE == OS_WINDOWS_CE)
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
#else
int main()
#endif
{
		arInit_t ai = {{tiny_error, tiny_printf, NULL}};

#if(OS_TYPE != OS_WINDOWS_CE)		
		printf("current locale == %ls\r\n", setlocale(LC_ALL,NULL));
#endif

		Arsenal_Init(&ai);
		
		AR_Test();

		Arsenal_UnInit();


#if(OS_TYPE == OS_WINDOWS_CE)
		::MessageBoxW(NULL, L"Done", L"Message",0);
#else
		_CrtDumpMemoryLeaks();
		printf("done\r\n");
		getchar();
#endif

		return 0;

}
#endif





#include <iostream>
#include <string>
#include "Arsenal.h"



using namespace ARSpace;

void AR_STDCALL report_message(const cfgReportInfo_t *report, void *context)
{
		
		AR_ASSERT(report != NULL && context == NULL);

		switch(report->type)
		{
				case CFG_REPORT_MESSAGE_T:
						AR_printf(L"Message == %ls\r\n", report->std_msg.message);
						break;
				case CFG_REPORT_ERROR_T:
						AR_printf(L"%ls == %d\r\n", report->error.err_msg,report->error.err_level);
						break;
				case CFG_REPORT_ERROR_LEX_T:
						AR_printf(L"Lex Error == %ls\r\n", report->lex_error.msg);
						break;
				case CFG_REPORT_ERROR_SYNTAX_T:
						AR_printf(L"Syntax Error == %ls\r\n", report->syntax_error.msg);
						break;
				case CFG_REPORT_WARNING_SYNTAX_T:
						AR_printf(L"Warning == %ls\r\n", report->warning.msg);						
						break;
				default:
						AR_ASSERT(false);
						
		}
}




bool	generate_parser(const std::wstring &grammar, std::wstring &out)
{

		bool_t			is_ok = true;
		cfgConfig_t		*cfg = NULL;
		arString_t		*ar_str = NULL;
		cfgReport_t		report = {report_message, NULL };

		
		cfg = CFG_CollectGrammarConfig(grammar.c_str(), &report);
		
		if(cfg == NULL) { is_ok = false; goto END_POINT; }
				
		ar_str = AR_CreateString();
		
		if(!CFG_ConfigToCode(cfg, ar_str)) { is_ok = false; goto  END_POINT;}
		
		
		out = AR_GetStrString(ar_str);
		
	
		


END_POINT:
		   if(cfg)
		   {
				   CFG_DestroyGrammarConfig(cfg);
				   cfg = NULL;
		   }
		   
		   if(ar_str)
		   {
				   AR_DestroyString(ar_str);
				   ar_str = NULL;
		   }
		   
		   return is_ok;
		   
}


int main (int argc, char * const argv[]) 
{
		
		
		using namespace ARSpace;
		Arsenal_Init(NULL);
		
		std::wstring in;
		std::wstring out;
		
		in = L"xxxxxxxxxxxxxxxxxx\r\n";
		
	
		generate_parser(in, out);
		
		Arsenal_UnInit();

		
		
		printf("done\r\n");
		getchar();
		return 0;
		
}





