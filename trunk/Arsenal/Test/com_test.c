#include "test.h"
#include <math.h>
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
				AR_AppendFormatString(str, L"%ls:%d", L"solidus", 8512);
				//AR_AppendString(str, L"abcd");
		}

		
		printf("len  == %d\r\n", AR_GetLengthString(str));
		getchar();
		AR_printf(L"%ls\r\n", AR_GetStrString(str));

		AR_DestroyString(str);
		
		getchar();
}

void str_test2()
{
		wchar_t dest[4096] = {0};

		const wchar_t *s1 = L"abcdefg";
		const wchar_t *s2 = L"0123456789";

		AR_printf(L"%ls\r\n", AR_wcscat(dest, s1));
		AR_printf(L"%ls\r\n", AR_wcscat(dest, L""));

		AR_printf(L"%ls\r\n", AR_wcsncat(dest, s2, 20));

		AR_printf(L"%ls\r\n", AR_wcsncat(dest, L"", 20));
}

void com_test2()
{
		const wchar_t *p;
		const wchar_t *str = L"			 -abcdef";
		
		int32_t num;
		
		p = AR_wtoi32(str, &num, 16);
		AR_printf(L"%ls : %d\n", p, num);

		p = AR_wtoi32_s(str, str + wcslen(str), &num, 16);
		AR_printf(L"ver_s = %ls : %d\n", p, num);
		getchar();
}

void __wtods_test(const wchar_t *str)
{
		double num;
		str = AR_wtod_s(str, str + AR_wcslen(str) + 1, &num);
		AR_printf(L"%ls : %f\r\n", str, num);

}
void com_test3()
{
		{
		const wchar_t *p;
		const wchar_t *str = L"3a.1415926This stopped it";
		
		double num;
		
		p = AR_wtod(str, &num);
		AR_printf(L"%ls : %f\n", p, num);
		
		p = AR_wtod_s(str, str + 5, &num);
		AR_printf(L"%ls : %f\n", p, num);

		getchar();
		}


		{
		const wchar_t *p;
		const wchar_t *str = L"3.14159265This stopped it";
		
		double num;
		
		p = AR_wtod(str, &num);
		AR_printf(L"%ls : %f\n", p, num);
		
		p = AR_wtod_s(str, str + wcslen(str) + 1, &num);
		AR_printf(L"%ls : %f\n", p, num);

		getchar();
		}

		{
		const wchar_t *p;
		const wchar_t *str = L"3.14159265e+1000This stopped it";
		
		double num;
		
		p = AR_wtod(str, &num);
		AR_printf(L"%ls : %f\n", p, num);

		AR_ASSERT(num == HUGE_VAL);
		
		p = AR_wtod_s(str, str + wcslen(str) + 1, &num);
		AR_printf(L"%ls : %f\n", p, num);

		getchar();
		}


		{
		const wchar_t *p;
		//const wchar_t *str = L"-3.14159265e+1000This stopped it";
		const wchar_t *str = L"-.14159265e+1This stopped it";
		
		double num;
		
		p = AR_wtod(str, &num);
		
		AR_printf(L"%ls : %f\n", p, num);
		
		p = AR_wtod_s(str, str + AR_wcslen(str) + 1, &num);
		AR_printf(L"%ls : %f\n", p, num);

		getchar();
		}

		{
				const wchar_t *str 
						= 
						L"		-198.401E+0abc"
						, *str1 = L"-3.e+1abc"
						, *str2 = L"-.0314e+1x"
						, *str3 = L"-.abc"
						, *str4 = L"-"
						, *str5 = L"3.e+"
						, *str6 = L".3e+0"
						, *str7 = L"		-199999999999999999999999999999999999999999999999999990.999999999999999999999999999999999401E+0abc"
						, *str8 = L"-0.0000e-8"
						, *str9 = L"+0.0001e+1"
						;

				__wtods_test(str);
				__wtods_test(str1);
				__wtods_test(str2);
				__wtods_test(str3);
				__wtods_test(str4);
				__wtods_test(str5);
				__wtods_test(str6);
				__wtods_test(str7);
				__wtods_test(str8);
				__wtods_test(str9);
				
				getchar();
		}
}


void com_conv()
{
		char *tmp;
		wchar_t *wtmp;
		const wchar_t *s = L"ÎÒ°®ÎäÌÙÀ¼!aaaaaaaaaaaaaaaaaaaaaÎÒ°®ÎäÌÙÀ¼";
		
		tmp = AR_wcs_convto_utf8(s);

		wtmp = AR_utf8_convto_wcs(tmp);
		

		MessageBoxW(0, wtmp, 0, 0);

		AR_DEL(tmp); AR_DEL(wtmp);



}



inline uint_t str_hash_test(const wchar_t *str, size_t n)
{
		uint_t	ret;
		size_t	i;
		
		AR_ASSERT(str != NULL);
		
		ret = 0;
		for(i = 0; i < n; ++i)
		{
				ret ^= (str[i] << (i & 0x0F));
		}
		return ret;
}

inline uint_t str_hash_test2(const wchar_t *s)
{
		return str_hash_test(s, AR_wcslen(s));

}


inline uint_t str_hash_test3(const wchar_t *str, size_t n)
{
		uint_t	ret;
		size_t	i;

		const size_t a = 1;
		if(n == 0)return 0;

		AR_ASSERT(str != NULL);

		ret = 0;
		for(i = 0; i < n; ++i)
		{
				ret += ((a << (n-i)) * (uint_t)str[i]);
		}
		return ret;
}

inline uint_t str_hash_test4(const wchar_t *s)
{
		return str_hash_test3(s, AR_wcslen(s));

}


void com_hash_test()
{
		const wchar_t *str = L"aaaaabcdefabcedef";
		const wchar_t *str1 = L"aaaaabcdfebacdef";
		AR_printf(L"hash1 string == %ls : value == %u\r\n", str, str_hash_test2(str));
		AR_printf(L"hash1 string == %ls : value == %u\r\n", str1, str_hash_test2(str1));

		AR_printf(L"hash2 string == %ls : value == %u\r\n", str, str_hash_test4(str));
		AR_printf(L"hash2 string == %ls : value == %u\r\n", str1, str_hash_test4(str1));

		

}


void __format_v(const wchar_t *fmt, ...)
{
		int_t len;
		va_list	arg_ptr;
		va_start(arg_ptr, fmt);
		len = AR_vscwprintf(fmt, arg_ptr);
		printf("--------------------------\r\n");
		printf("AR_vscwprintf == %d\r\n", len);
		//printf("_vscwprintf == %d\r\n", _vscwprintf(fmt, arg_ptr));
		va_end(arg_ptr);
}

void com_vscwprintf_test()
{
		uint64_t  x = 7332201052963203716;

		__format_v(L"%       .f%*.*f\r\n", 3344.333333333333333333333333, 20,20,3344.333333333333333333333333);

		__format_v(L"%33.54f", 3.14);

		__format_v(L"%d:%u:%I64d:%ls:%f\r\n", 33,44,(uint64_t)198401010, L"aaaaaaaaa",3.1415);

		__format_v(L"abcdefg");
		
		
		
		
		__format_v(L"%lld", x);
		__format_v(L"%I64d", x);
		__format_v(L"%I64ld", x);

}

/*****************************************************************************************************************/

typedef struct __ac_automaton_tag		arACAutomaton_t;
arACAutomaton_t*	AR_CreateACAutomaton();
void				AR_DestroyACAutomaton(arACAutomaton_t *atm);
bool				AR_InsertToACAutomaton(arACAutomaton_t *atm, const wchar_t *str, void *val);
bool				AR_RemoveFromACAutomaton(arACAutomaton_t *atm, const wchar_t *str);
bool				AR_BuildACAutomaton(arACAutomaton_t *atm);

bool				AR_FindFromACAutomaton(arACAutomaton_t *atm, const wchar_t *key, bool sub_str);



/*****************************************************************************************************************/




void itow_test()
{
		size_t i;
		wchar_t buf[1024];
		
		for(i = 2; i <= 16; ++i)
		{
				AR_u64tow_buf(buf, 1024, 0x1234567890, i);
				AR_printf(L"%ls\r\n", buf);
		}

		for(i = 2; i <= 16; ++i)
		{
				AR_i64tow_buf(buf, 1024, -0x1234567890, i);
				AR_printf(L"%ls\r\n", buf);
		}
}


void str_test1()
{

		{
				int64_t		num;
				const wchar_t *next, *s, *p;
				s = L"-11111111111111111111111111111111";
				
				p = AR_wtoi64(s, &num, 10);
				

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %I64d\r\n", num);
				printf("_wcstoi64 == %I64d\r\n", _wcstoi64(s, (wchar_t**)&next, 10));
				AR_printf(L"next == %ls\r\n", next);
		}
		

		{
				uint32_t		num;
				const wchar_t *s = L"11111111111111111111111111111111";

				const wchar_t *p = AR_wtou32(s, &num, 10);
				const wchar_t *next;

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %u\r\n", num);
				printf("strtoul == %u\r\n", wcstoul(s, (wchar_t**)&next, 10));
				AR_printf(L"next == %ls\r\n", next);
		}


		{
				int32_t		num;
				const wchar_t *s = L"-11111111111111111111111111111111";
				
				const wchar_t *p = AR_wtoi32(s, &num, 10);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));

		}


		{
				int32_t		num;
				const wchar_t *s = L"11111111111111111111111111111111";
				
				const wchar_t *p = AR_wtoi32(s, &num, 10);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));

		}

		{
				int32_t		num;
				const wchar_t *s = L"0x468321xyz";
				
				const wchar_t *p = AR_wtoi32(s, &num, 0);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));


		}


		{
				int32_t		num;
				const wchar_t *s = L"-0x468321xyz";
				
				const wchar_t *p = AR_wtoi32(s, &num, 0);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));
		}


		{
				int32_t		num;
				const wchar_t *s = L"0464321xyz";
				
				const wchar_t *p = AR_wtoi32(s, &num, 0);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));
		}


				{
				int32_t		num;
				const wchar_t *s = L"0464321xyz";
				
				const wchar_t *p = AR_wtoi32(s, &num, 0);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));
		}
#if(0)
#endif
}




void str_test3()
{
		int_t len;

		len = AR_scwprintf(L"%ls : %ls : %ls\r\n", L"ABCDEFÖí°Ë½ä",L"ABCDEFÖí°Ë½ä",L"ABCDEFÖí°Ë½ä");

		printf("len == %d\r\n", len);

}


void com_test()
{
		//str_test1();
		//str_test2();
		str_test3();
		//com_test3();
		//com_conv();
		//com_hash_test();

		//com_vscwprintf_test();
		//str_test();
		//itow_test();

}

#endif

AR_NAMESPACE_END