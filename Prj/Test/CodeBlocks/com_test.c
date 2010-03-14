
#include "test.h"




AR_NAMESPACE_BEGIN





void com_str_test_vscwprintf()
{
        ar_int_t l = AR_scwprintf(L"%C : %C : %C : %C\r\n", L'a', L'b', L'c', L'd');

	printf("l == %d\r\n", l);
}



void com_test()
{
        //printf("%s\r\n", "aaaaaaaaa");
        AR_printf(L"%ls\r\n", L"abcdef");
}









AR_NAMESPACE_END




