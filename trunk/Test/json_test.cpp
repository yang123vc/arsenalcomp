#include "test.h"


#include <math.h>
#include <time.h>

#include "Tools/json.h"



AR_NAMESPACE_BEGIN

#if defined(_LIB)

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

		status = Json_LoadObjectFromFile(L"..\\..\\..\\misc\\json_test\\1.json", &obj);


		if(obj)
		{
				Json_DestroyObject(obj);
				obj = NULL;
		}
}

#endif
void Json_Test()
{
#if defined(_LIB)
		//json_test1();

		json_test2();
#endif

}





AR_NAMESPACE_END