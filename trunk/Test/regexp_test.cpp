#include "test.h"
#include "./extlib/deelx/deelx.h"



#if defined(__LIB)


AR_NAMESPACE_BEGIN

static void deelx_test1()
{
		AR_printf(L"%hs\r\n", AR_FUNC_NAME);

		const wchar_t * str1 = L"12345";
		const wchar_t * str2 = L"12345 abcde";


		CRegexpT<wchar_t> regexp(L"\\d+");

		// test
		MatchResult result = regexp.MatchExact(str2);

		// matched or not
		int ret = result.IsMatched();

		AR_printf(L"%d\r\n", ret);


}

void rgx_test()
{
		deelx_test1();
		
}

AR_NAMESPACE_END



#endif

