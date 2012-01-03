
#include "test.h"


#include <math.h>
#include <time.h>

#if defined(__LIB)

AR_NAMESPACE_BEGIN




void misc_test()
{
		AR_printf(L"%d\r\n", AR_abs_32(-1));
		AR_printf(L"%d\r\n", AR_abs_32(1));

		AR_printf(L"%qd\r\n", AR_abs_64(-1));
		AR_printf(L"%qd\r\n", AR_abs_64(1));

		AR_printf(L"%f\r\n", AR_abs_dbl(-0.111f));
		AR_printf(L"%f\r\n", AR_abs_dbl(0.111f));
		
}

void vector_test()
{
		//AR_srand(time(NULL));

		arVector_t *vec = AR_CreateVector(2);

#if(0)
		for(size_t i = 0; i < 3; ++i)
		{
				AR_SetVectorValue(vec, i, AR_rand_dbl());
		}
#endif

		AR_SetVectorValue(vec, 0, 3.0);
		AR_SetVectorValue(vec, 1, 4.0);


		{
		arString_t *out;

		out = AR_CreateString();

		AR_VectorToString(vec, out, 30, L",");

		AR_printf(L"%ls\r\n", AR_GetStringCString(out));

		AR_DestroyString(out);
		out = NULL;
		}

		double res = AR_CalcInnerProduct(vec, vec);

		AR_printf(L"%f\r\n", res);


		res = AR_CalcVectorLength(vec);

		AR_printf(L"%f\r\n", res);

		res = AR_CalcVectorLengthSqr(vec);

		AR_printf(L"%f\r\n", res);


		AR_NormalizeVector(vec);

		{
		arString_t *out;

		out = AR_CreateString();

		AR_VectorToString(vec, out, 30, L",");

		AR_printf(L"%ls\r\n", AR_GetStringCString(out));

		AR_DestroyString(out);
		out = NULL;
		}



		AR_DestroyVector(vec);
}







void math_test()
{
		//misc_test();
		vector_test();


}

AR_NAMESPACE_END

#endif
