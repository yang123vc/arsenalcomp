
#include "test.h"
#include <locale.h>



AR_NAMESPACE_BEGIN





void com_str_test_vscwprintf()
{
        int_t l = AR_scwprintf(L"%C : %C : %C : %C\r\n", L'a', L'b', L'c', L'd');

	printf("l == %d\r\n", l);
}


extern  char* __translate_from_unicode_str(const wchar_t *input);



void com_test()
{
        //setlocale(LC_ALL, "utf-8");

        FILE *f = __AR_open_file(L"/root/Desktop/中文测试目录/x.txt", L"w");

        printf("%p\r\n", f);

        char v[] = "aaaaaaaadfasdfdsf";
        fwrite((void*)v, 1, sizeof(v), f);


        if(f)
        {
            fclose(f);
        }

}








AR_NAMESPACE_END




