
#include "test.h"
#include <locale.h>



AR_NAMESPACE_BEGIN





void com_str_test_vscwprintf()
{
        int_t l = AR_scwprintf(L"%C : %C : %C : %C\r\n", L'a', L'b', L'c', L'd');

        printf("l == %d\r\n", l);
}




void text_test_save()
{
#define OUTPUT_FILE		L"/root/Desktop/中文测试目录/x.txt"
#define OUTPUT_TYPE		AR_TXT_BOM_ASCII

		arString_t *str = AR_CreateString();

		if(!AR_LoadBomTextFile(L"/root/Desktop/Programming/Arsenal/misc/txt_enc_test/utf16_be_read_test.txt",NULL, str))
		{
				AR_ASSERT(false);
		}


		if(!AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStrString(str)))
		{
				AR_ASSERT(false);
		}


		arTxtBom_t bom;
		if(!AR_LoadBomTextFile(OUTPUT_FILE, &bom, str))
		{
				AR_ASSERT(false);
		}

		AR_ASSERT(bom == OUTPUT_TYPE);


		AR_DestroyString(str);
}


void charset_test_1()
{
    const char *s = "/root/Desktop/中文测试目录/x.txt";
    printf("len(s) == %d\r\n", strlen(s));

    size_t need_l = AR_acp_to_wcs(s, strlen(s), NULL, 0);

    wchar_t *buf = (wchar_t *)malloc(sizeof(wchar_t) * need_l);

    AR_acp_to_wcs(s, strlen(s), buf, need_l);

    wprintf("%ls\r\n", buf);

    free(buf);
}



void charset_test_2()
{

    const wchar_t *s = L"/root/Desktop/中文测试目录/x.txt";

    size_t need_l = AR_wcs_to_acp(s, AR_wcslen(s), NULL, 0);

    char *buf = (char *)malloc(sizeof(char) * need_l);

    AR_wcs_to_acp(s, AR_wcslen(s), buf, need_l);

    printf("%s\r\n", buf);

    free(buf);

}


void com_test()
{
        //setlocale(LC_ALL, "utf-8");

        //text_test_save();

        charset_test_1();

        charset_test_2();



}








AR_NAMESPACE_END




