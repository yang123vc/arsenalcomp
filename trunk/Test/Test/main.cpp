
#include <iostream>
#include "Arsenal.h"
#pragma comment(lib, "Arsenal.lib")



//using namespace std;
using namespace Arsenal;


const wchar_t *test =
L"{"

L"		int i; int j;  float[100] a; float v; float x;"
L"	bool x; char[1024] carr;"

L"	while(true)"
L"	{"
L"		do i = i + 1; while(a[i] < v);"
L"		do j = j-1; while(a[j] > v);"
L"		if(i >= j)break;"
L"		x = a[i]; a[i] = a[j]; a[j] = x;"
L"	}"
L"}"
;


void AR_STDCALL tiny_error(int level, const wchar_t* msg, void *ctx)
{
		wprintf(L"%d : %s\r\n", level, msg);
}

void AR_STDCALL tiny_printf(const wchar_t *msg, void *ctx)
{
		wprintf(L"%s\r\n", msg);
}


void AR_STDCALL	tiny_fatal(const wchar_t *msg, void *ctx)
{
		tiny_printf(msg, ctx);
}






int main()
{
		arInit_t ai = {tiny_error, tiny_printf, tiny_fatal};
		Arsenal_Init(&ai);
		
		AR_Test();

		

		Arsenal_UnInit();

		printf("done\r\n");

		
		getchar();

}
