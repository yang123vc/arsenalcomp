#include "test.h"

AR_NAMESPACE_BEGIN

#if defined(AR_DEBUG)

arString_t*		AR_CreateString();
void			AR_DestroyString(arString_t *str);

void			AR_ReserveString(arString_t *str, size_t num);
void			AR_ClearString(arString_t *str);
size_t			AR_AppendString(arString_t *str, const wchar_t *sour);


void			AR_FormatString(arString_t *str, const wchar_t *fmt, ...);
void			AR_AppendFormatString(arString_t *str, const wchar_t *fmt, ...);


const wchar_t*	AR_GetStrString(const arString_t *str);
size_t			AR_GetLengthString(const arString_t *str);

void str_test()
{
		arString_t *str;
		size_t i;
		str = AR_CreateString();
		
		for(i = 0; i < 100; ++i)
		{
				//AR_AppendFormatString(str, L"%s:%d", L"solidus", 8512);
				AR_AppendString(str, L"abcd");
		}

		
		printf("len  == %d\r\n", AR_GetLengthString(str));
		getchar();
		wprintf(L"%s\r\n", AR_GetStrString(str));

		AR_DestroyString(str);
		
		getchar();
}

void com_test()
{
		const wchar_t *str = L"    -abcdef";
		int32_t num;
		str = AR_wtoi32(str, &num, 16);

		wprintf(L"%s : %d\n", str, num);
		getchar();

}
#endif

AR_NAMESPACE_END