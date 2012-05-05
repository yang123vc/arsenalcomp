
#include "test.h"
#include <locale.h>



AR_NAMESPACE_BEGIN





void text_test_save()
{
#define OUTPUT_FILE		L"/root/Desktop/���Ĳ���Ŀ¼/x.txt"
#define OUTPUT_TYPE		AR_TXT_BOM_ASCII

		arString_t *str = AR_CreateString();

		if(AR_LoadBomTextFile(L"/root/Desktop/Programming/Arsenal/misc/txt_enc_test/utf16_be_read_test.txt",NULL, str) != AR_S_YES)
		{
				AR_ASSERT(false);
		}

		if(AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStringCString(str)) != AR_S_YES)
		{
		}
#if(0)
		if(AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStringCString(str)) != AR_S_YES)
		{
				AR_ASSERT(false);
		}


		arTxtBom_t bom;
		if(AR_LoadBomTextFile(OUTPUT_FILE, &bom, str)  != AR_S_YES)
		{
				AR_ASSERT(false);
		}

		AR_ASSERT(bom == OUTPUT_TYPE);
#endif

		AR_DestroyString(str);
}




void com_test()
{
        //setlocale(LC_ALL, "utf-8");

       text_test_save();



}








AR_NAMESPACE_END




