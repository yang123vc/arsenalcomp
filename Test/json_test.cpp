#include "test.h"


#include <math.h>
#include <time.h>

#include "Tools/json.h"



AR_NAMESPACE_BEGIN

#if defined(__LIB)

jsonObj_t* parse_json_escape_string(const wchar_t *lexer_str, size_t count);



void json_test1()
{
		jsonObj_t *json_str, *json_str2;

		const wchar_t *lex_str = L"abc\\\"\\r\\n\\t\\f\\uaa12345668";

		json_str = parse_json_escape_string(lex_str, AR_wcslen(lex_str));


		const wchar_t *lex_str2 = L"";
		json_str2 = parse_json_escape_string(lex_str2, AR_wcslen(lex_str2));
		
		const wchar_t *wcs = Json_GetWcsFromObject(json_str2);

		Json_DestroyObject(json_str2);
		json_str2 = NULL;

		Json_DestroyObject(json_str);
		json_str = NULL;
}


void json_test2()
{
		jsonObj_t *obj = NULL;
		arStatus_t status = AR_S_YES;

		status = Json_LoadObjectFromFile(L"..\\..\\..\\misc\\json_test\\4.json", &obj);


		if(obj)
		{
				Json_DestroyObject(obj);
				obj = NULL;
		}
}


void json_test3()
{
		jsonObj_t *obj = NULL;
		arStatus_t status = AR_S_YES;

		status = Json_LoadObjectFromFile(L"..\\..\\..\\misc\\json_test\\4.json", &obj);
		AR_ASSERT(status == AR_S_YES);

		status = Json_SaveObjectToFile(obj, L"D:\\1.json");
		AR_ASSERT(status == AR_S_YES);

		if(obj)
		{
				Json_DestroyObject(obj);
				obj = NULL;
		}
}



void json_test4()
{
		jsonObj_t *obj = NULL;
		ar_int_64_t beg, end;
		arStatus_t status = AR_S_YES;
		
		for(size_t i = 0; i < 3; ++i)
		{
				beg = AR_GetTime_Microseconds();
				status = Json_LoadObjectFromFile(L"..\\..\\..\\misc\\json_test\\twitter_public_timeline.json", &obj);
				AR_ASSERT(status == AR_S_YES);
				end = AR_GetTime_Microseconds();

				AR_printf(L"parse elapsed microseconds : %qd\r\n", end - beg);

				
				if(obj)
				{
						Json_DestroyObject(obj);
						obj = NULL;
				}
		}

		
		

		
		if(obj)
		{
				Json_DestroyObject(obj);
				obj = NULL;
		}

		getchar();
}

#endif


void Json_Test()
{
#if defined(__LIB)
		//json_test1();
		//json_test2();
		//json_test3();
		json_test4();
#endif

}





AR_NAMESPACE_END