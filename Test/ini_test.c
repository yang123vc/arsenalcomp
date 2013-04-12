#include "test.h"


#include <math.h>
#include <time.h>

#include "Tools/iniobj.h"

#if defined(__LIB)

AR_NAMESPACE_BEGIN


#define _CHECK_AND_JMP(__stmt)	do{  if(__stmt == AR_E_NOMEM){ goto END_POINT; }}while( 0 )


static void ini_test1()
{
		arString_t	*str;
		iniObject_t *obj;
		str = AR_CreateString();
		
		obj = NULL;

		if(AR_LoadBomTextFile(L"D:\\1.ini", NULL, str) != AR_S_YES)
		{
				goto END_POINT;
		}

		obj = Ini_CreateObject();

		if(obj == NULL)
		{
				goto END_POINT;
		}

		if(Ini_LoadObjectFromString(obj, AR_GetStringCString(str)) != AR_S_YES)
		{
				goto END_POINT;
		}
		

		
		printf("Ini_GetInt == %d\r\n", Ini_GetInt(obj,  INI_EMPTY_SECTION_NAME, L"a", 0));
		printf("Ini_GetUInt == %u\r\n", Ini_GetUInt(obj,  INI_EMPTY_SECTION_NAME, L"b", 0));
		printf("Ini_GetFloat == %g\r\n", Ini_GetFloat(obj,  INI_EMPTY_SECTION_NAME, L"c", 0.0f));

		

		_CHECK_AND_JMP(Ini_SetComment(obj, INI_EMPTY_SECTION_NAME, L"optimize3", L";����Ini_SetComment"));

		_CHECK_AND_JMP(Ini_RemoveKey(obj, INI_EMPTY_SECTION_NAME, L"optimize"));
		_CHECK_AND_JMP(Ini_RemoveKey(obj, INI_EMPTY_SECTION_NAME, L"optimize2"));
		//Ini_RemoveKey(obj, INI_EMPTY_SECTION_NAME, L"optimize3");
		_CHECK_AND_JMP(Ini_RemoveKey(obj, INI_EMPTY_SECTION_NAME, L"optimize4"));

		_CHECK_AND_JMP(Ini_RemoveSection(obj, L"OPTIMIZE"));
		_CHECK_AND_JMP(Ini_RemoveSection(obj, L"OPTIMIZATION1"));
		
		_CHECK_AND_JMP(Ini_InsertSection(obj, L"Test", L";test"));
		_CHECK_AND_JMP(Ini_SetString(obj, L"Test", L"	a		", L"3", L"comment1"));
		_CHECK_AND_JMP(Ini_SetString(obj, L"Test", L"	b		", NULL, L"comment2"));
		
		_CHECK_AND_JMP(Ini_SetInt(obj, L"Test", L"c", -33, NULL));
		_CHECK_AND_JMP(Ini_SetUInt(obj, L"Test", L"d", 44, L"comment4"));
		_CHECK_AND_JMP(Ini_SetFloat(obj, L"Test", L"e", 0.123456789, L"comment5"));


		/*bad section name*/
		_CHECK_AND_JMP(Ini_InsertSection(obj, L"[ ]", L""));
		_CHECK_AND_JMP(Ini_InsertSection(obj, L"   ", L""));

		_CHECK_AND_JMP(Ini_InsertSection(obj, L"		p		", L""));

		_CHECK_AND_JMP(Ini_SaveObjectToString(obj, str));

		::MessageBoxW(NULL, AR_GetStringCString(str), 0,0);

END_POINT:
		if(obj)
		{
				Ini_DestroyObject(obj);
				obj = NULL;
		}

		if(str)
		{
				AR_DestroyString(str);
				str = NULL;
		}
}

static void ini_test2()
{
		arString_t	*str;
		iniObject_t *obj;
		str = AR_CreateString();
		
		obj = NULL;

		if(AR_LoadBomTextFile(L"C:\\Users\\liupeng\\Desktop\\360zipplugin.ini", NULL, str) != AR_S_YES)
		{
				goto END_POINT;
		}

		obj = Ini_CreateObject();

		if(obj == NULL)
		{
				goto END_POINT;
		}

		if(Ini_LoadObjectFromString(obj, AR_GetStringCString(str)) != AR_S_YES)
		{
				goto END_POINT;
		}
		
		/*
		printf("Ini_GetInt == %d\r\n", Ini_GetInt(obj,  INI_EMPTY_SECTION_NAME, L"a", 0));
		printf("Ini_GetUInt == %u\r\n", Ini_GetUInt(obj,  INI_EMPTY_SECTION_NAME, L"b", 0));
		printf("Ini_GetFloat == %g\r\n", Ini_GetFloat(obj,  INI_EMPTY_SECTION_NAME, L"c", 0.0f));
		*/


		{
				const wchar_t *files = Ini_GetString(obj, L"360main", L"files0");
				printf("%ls\r\n", files);
		}

		
		{
				const wchar_t *files = Ini_GetString(obj, L"RarFiles.lst", L"URL");
				printf("%ls\r\n", files);
		}

END_POINT:
		if(obj)
		{
				Ini_DestroyObject(obj);
				obj = NULL;
		}

		if(str)
		{
				AR_DestroyString(str);
				str = NULL;
		}
}

void ini_test3()
{
		iniObject_t *obj = Ini_CreateObject();
		AR_ASSERT(obj != NULL);

		arStatus_t status;
		status = Ini_LoadObjectFromFile(obj, L"G:\\Temp\\1.ini");
		AR_ASSERT(status == AR_S_YES);

		status = Ini_SaveObjectToFile(obj, L"G:\\Temp\\2.ini");
		AR_ASSERT(status == AR_S_YES);
		
		Ini_DestroyObject(obj);
		obj = NULL;

}


void ini_test4()
{
		
		arString_t	*str;
		iniObject_t *obj;
		str = AR_CreateString();
		
		obj = NULL;

		if(AR_LoadBomTextFile(L"D:\\2.sig", NULL, str) != AR_S_YES)
		{
				AR_ASSERT(false);
		}
		AR_printf(L"%ls\r\n", AR_CSTR(str));
		
		AR_ASSERT(AR_GetStringLength(str) == 512);

		AR_DestroyString(str);
		str = NULL;
}

void Ini_Test()
{
		//ini_test1();
		//ini_test2();

		ini_test4();

}


AR_NAMESPACE_END

#endif