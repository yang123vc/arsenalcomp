#include "test.h"


#include <math.h>
#include <time.h>

#include "Tools/iniobj.h"

#if defined(__LIB)

AR_NAMESPACE_BEGIN



void Ini_Test()
{
		arString_t	*str;
		iniObject_t *obj;
		str = AR_CreateString();
		

		if(!AR_LoadBomTextFile(L"D:\\1.ini", NULL, str))
		{
				AR_abort();
		}

		obj = Ini_CreateObject();

		if(!Ini_LoadObjectFromString(obj, AR_GetStrString(str)))
		{
				AR_abort();
		}
		

		
		printf("Ini_GetInt == %d\r\n", Ini_GetInt(obj,  INI_EMPTY_SECTION_NAME, L"a", 0));
		printf("Ini_GetUInt == %u\r\n", Ini_GetUInt(obj,  INI_EMPTY_SECTION_NAME, L"b", 0));
		printf("Ini_GetFloat == %g\r\n", Ini_GetFloat(obj,  INI_EMPTY_SECTION_NAME, L"c", 0.0f));

		

		Ini_SetComment(obj, INI_EMPTY_SECTION_NAME, L"optimize3", L";≤‚ ‘Ini_SetComment");

		Ini_RemoveKey(obj, INI_EMPTY_SECTION_NAME, L"optimize");
		Ini_RemoveKey(obj, INI_EMPTY_SECTION_NAME, L"optimize2");
		//Ini_RemoveKey(obj, INI_EMPTY_SECTION_NAME, L"optimize3");
		Ini_RemoveKey(obj, INI_EMPTY_SECTION_NAME, L"optimize4");

		Ini_RemoveSection(obj, L"OPTIMIZE");
		Ini_RemoveSection(obj, L"OPTIMIZATION1");
		
		Ini_InsertSection(obj, L"Test", L";test");
		Ini_SetString(obj, L"Test", L"	a		", L"3", L"comment1");
		Ini_SetString(obj, L"Test", L"	b		", NULL, L"comment2");
		
		Ini_SetInt(obj, L"Test", L"c", -33, NULL);
		Ini_SetUInt(obj, L"Test", L"d", 44, L"comment4");
		Ini_SetFloat(obj, L"Test", L"e", 0.123456789, L"comment5");


		/*bad section name*/
		Ini_InsertSection(obj, L"[ ]", L"");
		Ini_InsertSection(obj, L"   ", L"");

		Ini_InsertSection(obj, L"		p		", L"");

		Ini_SaveObjectToString(obj, str);

		::MessageBoxW(NULL, AR_GetStrString(str), 0,0);

		
		Ini_DestroyObject(obj);
		obj = NULL;
		AR_DestroyString(str);
		str = NULL;
}


AR_NAMESPACE_END

#endif