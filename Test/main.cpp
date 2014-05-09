





#define WINVER 0x0601
#define _WIN32_WINNT 0x0601
#define _WIN32_WINDOWS 0x0410




#include "test.h"

#include <iostream>


#include "Arsenal.h"

#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")





#pragma comment(lib, "Arsenal.lib")

#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib,"odbc32.lib")
#pragma comment(lib,"odbcbcp.lib")
#pragma comment(lib,"OdbcCP32.Lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Mpr.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")


//using namespace std;
using namespace ARSpace;





void AR_STDCALL tiny_error(ar_int_t level, const wchar_t* msg, void *ctx)
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
		//::MessageBoxW(NULL, msg, L"MessageBox", 0);
		printf("%ls", msg);
#endif
}

#if(OS_TYPE != OS_WINDOWS_CE)

static size_t AR_STDCALL build_backtrace(void **callstack, size_t callstack_cnt)
{
		AR_ASSERT(callstack != NULL && callstack_cnt > 0);
		return (size_t)CaptureStackBackTrace(1, callstack_cnt, callstack, NULL);
}

static size_t	AR_STDCALL build_backtrace_symbol(void **callstack, size_t callstack_cnt, char *str, size_t len)
{
		unsigned int	i;
		SYMBOL_INFO		*symbol;
		HANDLE			process;
		char			*p;
		size_t			remain;
		
		AR_ASSERT(callstack != NULL && str != NULL && len > 0);
		
		if(callstack_cnt == 0)
		{
				str[0] = '\0';
                return 0;
		}

		process = GetCurrentProcess();

		SymInitialize( process, NULL, TRUE );
		symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
		symbol->MaxNameLen   = 255;
		symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

		p = str;
		remain = len - 1;

		for(i = 0; i < callstack_cnt; i++ )
		{
				size_t n;
				SymFromAddr( process, (DWORD64)(callstack[i]), 0, symbol );

				if(_snprintf(p, remain, "%s\r\n", symbol->Name) <= 0)
				{
						break;
				}
				
				n = strlen(p);
				
				if(n == 0)
				{
						break;
				}
				
				p += n;
				remain -= n;
		}
		
		*p = '\0';
		free(symbol);
		return (size_t)strlen(str);
}



#endif




#if(OS_TYPE == OS_WINDOWS_CE)
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
#else
int main()
#endif
{
		arInit_t ai = 
		{
				{tiny_error, tiny_printf, NULL},
				{build_backtrace, build_backtrace_symbol},
		};

#if(OS_TYPE != OS_WINDOWS_CE)		
	//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		printf("current locale == %ls\r\n", setlocale(LC_CTYPE, "UTF-8"));
#else
		
#endif


		Arsenal_Init(&ai);


		AR_Test();

		Arsenal_UnInit();


#if(OS_TYPE == OS_WINDOWS_CE)
		::MessageBoxW(NULL, L"Done", L"Message",0);
#else
		//_CrtDumpMemoryLeaks();
		printf("done\r\n");
		//getchar();
#endif

		return 0;

}




