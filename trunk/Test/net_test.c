
#include <wchar.h>
#include "test.h"
#include "curl/curl.h"





void net_test()
{
		wchar_t buf[1024];
		AR_DPRINT(L"On func : %hs\r\n", AR_FUNC_NAME);

	
		AR_error(AR_ERR_MESSAGE, L"%#x\r\n", 0x1234);
		wprintf(L"%#x\r\n", 0x1234);



		

}