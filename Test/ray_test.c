#include <typeinfo.h>

#include "test.h"


#if defined(__LIB)

AR_NAMESPACE_BEGIN



void ray_test()
{
		typedef int int_t;

		{
				typedef double int_t;
				int_t x;
				AR_printf(L"typedef name == %hs\r\n", typeid(x).name());
		}
		
		int_t x;
		AR_printf(L"typedef name == %hs\r\n", typeid(x).name());
}


AR_NAMESPACE_END

#endif

