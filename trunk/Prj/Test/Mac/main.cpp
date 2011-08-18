
#include <iostream>
#include <string>
#include "Arsenal.h"

using namespace ARSpace;



#define INPUT_FILE		L"/Users/solidussnakeex/Desktop/input.txt"
#define OUTPUT_FILE		L"/Users/solidussnakeex/Desktop/output.txt"

#define OUTPUT_TYPE		AR_TXT_BOM_UTF_8

void text_test_save()
{


		arTxtBom_t		bom;
		arString_t *str = AR_CreateString();
		
		
		if(!AR_LoadBomTextFile(INPUT_FILE, &bom, str))
		{
				AR_ASSERT(false);
		}
		
		
		if(!AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStrString(str)))
		{
				AR_ASSERT(false);
		}
		
		
		if(!AR_LoadBomTextFile(OUTPUT_FILE, &bom, str))
		{
				AR_ASSERT(false);
		}
		
		AR_ASSERT(bom == OUTPUT_TYPE);
		
		wprintf(L"%ls\r\n", AR_GetStrString(str));
				
		AR_DestroyString(str);
		
		

}



void AR_Test2()
{

		text_test_save();
				
		
		
}






void AR_STDCALL tiny_error(int_t level, const wchar_t* msg, void *ctx)
{
		
        wprintf(L"%ls\r\n", msg);
}

void AR_STDCALL tiny_printf(const wchar_t *msg, void *ctx)
{
		
        wprintf(L"%ls\r\n", msg);
}




int  main()
{
      
		setlocale(LC_ALL, "");
		
		
				
		//printf("%s\r\n", AR_FUNC_NAME);
        arInit_t ai = {{tiny_error, tiny_printf, NULL}};
		
        //printf("%s\r\n", setlocale(LC_ALL, NULL));
		
        Arsenal_Init(&ai);
		
        AR_Test2();
		
		
		Arsenal_UnInit();
		
		
		printf("done\r\n");
		
		
		getchar();
		
		return 0;
		
}









