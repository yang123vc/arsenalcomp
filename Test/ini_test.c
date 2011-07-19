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

		Ini_SaveObjectToString(obj, str);

		::MessageBoxW(NULL, AR_GetStrString(str), 0,0);

		
		Ini_DestroyObject(obj);
		obj = NULL;
		AR_DestroyString(str);
		str = NULL;
}


AR_NAMESPACE_END

#endif