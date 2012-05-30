#include "test.h"

#include "curl_helper.h"

AR_NAMESPACE_BEGIN

#if defined(__LIB)


void curl_test()
{
		curl_global_init(CURL_GLOBAL_ALL);




		curl_global_cleanup();


}

#endif


AR_NAMESPACE_END





