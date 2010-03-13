
#include "test.h"




AR_NAMESPACE_BEGIN





void com_str_test_vscwprintf()
{
		int_t l = AR_scwprintf(L"%C : %C : %C : %C\r\n", L'a', L'b', L'c', L'd');

		printf("l == %d\r\n", l);

}



void com_test()
{
    com_str_test_vscwprintf();
}









AR_NAMESPACE_END




