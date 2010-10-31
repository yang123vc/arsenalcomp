


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




