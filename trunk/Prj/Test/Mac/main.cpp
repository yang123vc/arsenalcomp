
#include <iostream>
#include <string>
#include "Arsenal.h"

using namespace ARSpace;



#define INPUT_FILE		L"/Users/solidussnakeex/Desktop/Tengu.gmr"
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
		
		
		if(!AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStringCString(str)))
		{
				AR_ASSERT(false);
		}
		
		
		if(!AR_LoadBomTextFile(OUTPUT_FILE, &bom, str))
		{
				AR_ASSERT(false);
		}
		
		AR_ASSERT(bom == OUTPUT_TYPE);
		
		wprintf(L"%ls\r\n", AR_GetStringCString(str));
				
		AR_DestroyString(str);
		
		

}



void AR_Test2()
{

		text_test_save();
				
		
		
}




void AR_Test3()
{
		uint_64_t beg,end;
		for(size_t i = 0; i < 10; ++i)
		{
				
				beg = AR_GetTime_Milliseconds();
				AR_Sleep(999);
				end = AR_GetTime_Milliseconds();
				
				AR_printf(L"elapsed == %qu\r\n", end - beg);
				
		}
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
		//AR_Test3();
		
		
		Arsenal_UnInit();
		
		
		printf("done\r\n");
		
		
		//getchar();
		
		return 0;
		
}









