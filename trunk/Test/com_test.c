#include "test.h"


#include <math.h>
#include <time.h>

#if defined(__LIB)

AR_NAMESPACE_BEGIN



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
		
		ar_int32_t num;
		
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
		

//		MessageBoxW(0, wtmp, 0, 0);

		AR_DEL(tmp); AR_DEL(wtmp);



}



AR_INLINE ar_uint_t str_hash_test(const wchar_t *str, size_t n)
{
		ar_uint_t	ret;
		size_t	i;
		
		AR_ASSERT(str != NULL);
		
		ret = 0;
		for(i = 0; i < n; ++i)
		{
				ret ^= (str[i] << (i & 0x0F));
		}
		return ret;
}

AR_INLINE ar_uint_t str_hash_test2(const wchar_t *s)
{
		return str_hash_test(s, AR_wcslen(s));

}


AR_INLINE ar_uint_t str_hash_test3(const wchar_t *str, size_t n)
{
		ar_uint_t	ret;
		size_t	i;

		const size_t a = 1;
		if(n == 0)return 0;

		AR_ASSERT(str != NULL);

		ret = 0;
		for(i = 0; i < n; ++i)
		{
				ret += ((a << (n-i)) * (ar_uint_t)str[i]);
		}
		return ret;
}

AR_INLINE ar_uint_t str_hash_test4(const wchar_t *s)
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
		ar_int_t len;
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
		ar_uint64_t  x = 7332201052963203716;

		__format_v(L"%       .f%*.*f\r\n", 3344.333333333333333333333333, 20,20,3344.333333333333333333333333);

		__format_v(L"%33.54f", 3.14);

		__format_v(L"%d:%u:%I64d:%ls:%f\r\n", 33,44,(ar_uint64_t)198401010, L"aaaaaaaaa",3.1415);

		__format_v(L"abcdefg");
		
		
		
		
		__format_v(L"%lld", x);
		__format_v(L"%I64d", x);
		__format_v(L"%I64ld", x);

}


void com_vscwprintf_test2()
{
		__format_v(L"%Id : %I32d : %I64d : %lld : %d : %ld\r\n", (int)111, (ar_int32_t)222, (ar_int64_t)333, (ar_int64_t)444, (int)555, (int)666);
		__format_v(L"%Id : %Id\r\n", (size_t)1024, (size_t)2048);

}

/*****************************************************************************************************************/

typedef struct __ac_automaton_tag		arACAutomaton_t;
arACAutomaton_t*	AR_CreateACAutomaton();
void				AR_DestroyACAutomaton(arACAutomaton_t *atm);
bool_t				AR_InsertToACAutomaton(arACAutomaton_t *atm, const wchar_t *str, void *val);
bool_t				AR_RemoveFromACAutomaton(arACAutomaton_t *atm, const wchar_t *str);
bool_t				AR_BuildACAutomaton(arACAutomaton_t *atm);

bool_t				AR_FindFromACAutomaton(arACAutomaton_t *atm, const wchar_t *key, bool_t sub_str);



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
				ar_int64_t		num;
				const wchar_t *s, *p;
				s = L"-11111111111111111111111111111111";
				
				p = AR_wtoi64(s, &num, 10);
				

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %I64d\r\n", num);
//				printf("_wcstoi64 == %I64d\r\n", _wcstoi64(s, (wchar_t**)&next, 10));
				//AR_printf(L"next == %ls\r\n", next);
		}
		

		{
				ar_uint32_t		num;
				const wchar_t *s = L"11111111111111111111111111111111";

				const wchar_t *p = AR_wtou32(s, &num, 10);
				const wchar_t *next;

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %u\r\n", num);
				printf("strtoul == %u\r\n", wcstoul(s, (wchar_t**)&next, 10));
				AR_printf(L"next == %ls\r\n", next);
		}


		{
				ar_int32_t		num;
				const wchar_t *s = L"-11111111111111111111111111111111";
				
				const wchar_t *p = AR_wtoi32(s, &num, 10);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));

		}


		{
				ar_int32_t		num;
				const wchar_t *s = L"11111111111111111111111111111111";
				
				const wchar_t *p = AR_wtoi32(s, &num, 10);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));

		}

		{
				ar_int32_t		num;
				const wchar_t *s = L"0x468321xyz";
				
				const wchar_t *p = AR_wtoi32(s, &num, 0);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));


		}


		{
				ar_int32_t		num;
				const wchar_t *s = L"-0x468321xyz";
				
				const wchar_t *p = AR_wtoi32(s, &num, 0);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));
		}


		{
				ar_int32_t		num;
				const wchar_t *s = L"0464321xyz";
				
				const wchar_t *p = AR_wtoi32(s, &num, 0);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));
		}


				{
				ar_int32_t		num;
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
		ar_int_t len;

		len = AR_scwprintf(L"%ls : %ls : %ls\r\n", L"ABCDEFÖí°Ë½ä",L"ABCDEFÖí°Ë½ä",L"ABCDEFÖí°Ë½ä");

		printf("len == %d\r\n", len);

}

void str_test4()
{
		const wchar_t *p;
		wchar_t buf[] = L"  abcdefgh    ";
		p = AR_wcstrim_right(buf, L" abcdefgh");
		wprintf(L"%ls\r\n", p);

}

void str_test5()
{
		//AR_report();
		AR_printf(L"%lld  :  %ld\r\n", (ar_uint64_t)345,(ar_uint64_t)453);

}


static void test_bittest()
{
		ar_uint32_t x = 0;

		x = (ar_uint32_t)AR_BIT_MARK(32);

		AR_BIT_SET(x,0);


		AR_BIT_CLR(x,0);
/*
		

		if(AR_BIT_TEST(x, 3)){printf("xxx\r\n");}

		AR_BIT_CLR(x, 48);
*/


}

void bug_test()
{
		char c = 'c';
        AR_scwprintf(L"%hc\r\n", c);
}


ar_int_t __cmp_size_t(const void *l, const void *r)
{
		return AR_CMP(*(size_t*)l, *(size_t*)r);

}

int __cmp_size_t_2(const void *l, const void *r)
{
		return (int)AR_CMP(*(size_t*)l, *(size_t*)r);

}


#define  SORT_TEST_CNT (30*1024 * 1024)

//#define  SORT_TEST_CNT (30)
#if(0)
void sort_test()
{
		size_t *vi = AR_NEWARR(size_t, SORT_TEST_CNT);
		int i;
		DWORD beg,end;
		ar_int_t cnt = 3;
		AR_srand(time(NULL));

		__int64 n = (__int64)SORT_TEST_CNT;
		while(cnt-- > 0)
		{
				for(i = 0; i < SORT_TEST_CNT; ++i)
				{
						vi[i] = AR_rand32() ;//% 40960;
						
						//vi[i] = (size_t)--n;

				}

				printf("-----------beg------------\r\n");
				beg = GetTickCount();
				
				AR_qsort(vi,SORT_TEST_CNT , sizeof(size_t),__cmp_size_t);
				//qsort(vi, SORT_TEST_CNT, sizeof(size_t), __cmp_size_t_2);

				

				//std::sort(vi, vi + SORT_TEST_CNT);

				end = GetTickCount();

				printf("elapsed == %u\r\n", end - beg);
				printf("-----------end------------\r\n");

				

				for(i = 1; i < SORT_TEST_CNT; ++i)
				{
						if(vi[i-1] <= vi[i])
						{

						}else
						{
								system("pause");
								abort();
						}
				}
		}

}
#endif

void search_test()
{
		int i;
		int arr[] = {0,1,2,3,4,5,6,7,8,9};
		
		for(i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
		{
				ar_int_t m = AR_bsearch(&i, arr,sizeof(arr)/sizeof(arr[0]), sizeof(int), __cmp_size_t);
		
				printf("arr[%d] == %d\r\n", m,arr[m]);
		}



}


void str_test6()
{
		wprintf(L"%ls\r\n", L"\\\"=\\\"");

}

void escstr_test0()
{

		//const wchar_t *s = L"\\a\\b\\c\\d\\s";
		wchar_t buf[1024];

		while(true)
		{
				_getws(buf);
				if(AR_wcscmp(buf, L"quit") == 0)break;

				wchar_t *d = AR_str_to_escstr(buf);

				wprintf(L"escape == %ls\r\n", d);
				AR_DEL(d);

		}
}


void escstr_test1()
{
		wchar_t tmp[1024];

		while(true)
		{
				_getws(tmp);
				if(AR_wcscmp(tmp, L"quit") == 0)break;

				arEscStrErr_t	err;
				wchar_t *d = AR_escstr_to_str(tmp, &err);
				
				wchar_t buf[1024];
				if(err.type == AR_ESCSTR_ERR_VALUE)
				{
						AR_swprintf(buf, 1024, L"invalid value %I64d\r\n", err.value);
				}else if(err.type == AR_ESCSTR_ERR_CHAR)
				{
						AR_swprintf(buf, 1024, L"invalid input %ls\r\n", err.pos);
				}else
				{
						AR_swprintf(buf, 1024, L"%ls\r\n", d);
				}

				//wprintf(L"string == %ls\r\n", d);
				::MessageBoxW(NULL, buf, 0,0);
				
				AR_DEL(d);

		}

}




void escstr_test_buf0()
{

		//const wchar_t *s = L"\\a\\b\\c\\d\\s";
		wchar_t buf[1024], tmp[1024];

		while(true)
		{
				_getws(buf);
				if(AR_wcscmp(buf, L"quit") == 0)break;

				ar_int_t l = AR_str_to_escstr_buf(tmp, 1024, buf);

				AR_ASSERT(l >= 0);
				
				wprintf(L"escape == %ls\r\n", tmp);
				

		}
}



void escstr_test_buf1()
{
		wchar_t tmp[1024], out[1024];

		while(true)
		{
				_getws(tmp);
				if(AR_wcscmp(tmp, L"quit") == 0)break;

				arEscStrErr_t	err;
				ar_int_t l = AR_escstr_to_str_buf(out, 1024, tmp, &err);
				
				wchar_t buf[1024];
				if(err.type == AR_ESCSTR_ERR_BUFFER)
				{
						AR_ASSERT(false);
						
				}else if(err.type == AR_ESCSTR_ERR_VALUE)
				{
						AR_swprintf(buf, 1024, L"invalid value %I64d\r\n", err.value);
				}else if(err.type == AR_ESCSTR_ERR_CHAR)
				{
						AR_swprintf(buf, 1024, L"invalid input %ls\r\n", err.pos);
				}else
				{
						AR_swprintf(buf, 1024, L"%ls\r\n", out);
				}

				//wprintf(L"string == %ls\r\n", d);
				::MessageBoxW(NULL, buf, 0,0);
				
				

		}

}


void buffer_test()
{
		arBuffer_t		*buffer;
		
		buffer = AR_CreateBuffer(0);

		FILE *f = fopen("d:\\temp\\lcc.rar", "rb");
		FILE *tmp = fopen("d:\\temp\\tmp.rar", "wb");
		assert(f != NULL && tmp != NULL);

		ar_byte_t buf[384];
		
		while(!ferror(f) && !feof(f))
		{
				size_t rn = fread(buf, sizeof(ar_byte_t), sizeof(buf), f);
				if(rn == 0)break;
				AR_InsertToBuffer(buffer, buf, rn);

				if(AR_GetBufferReadableLength(buffer) > 4096)
				{
						fwrite((void*)AR_GetBufferReadableData(buffer), 1, 4096, tmp);
						AR_EraseFromBuffer(buffer, 4096);
				}
		}

		fclose(f);

		if(AR_GetBufferReadableLength(buffer) > 0)
		{
				fwrite((void*)AR_GetBufferReadableData(buffer), 1, AR_GetBufferReadableLength(buffer), tmp);
				AR_EraseFromBuffer(buffer, AR_GetBufferReadableLength(buffer));
		}

		fclose(tmp);
		
		AR_DestroyBuffer(buffer);

}

void com_timer_test()
{
		ar_uint64_t		beg = AR_GetTime_Milliseconds();
		AR_Sleep(1000);
		ar_uint64_t		end = AR_GetTime_Milliseconds();

		printf("elapsed == %I64d\r\n", end - beg);

}



void com_str_test_vscwprintf()
{
		ar_int_t l = AR_scwprintf(L"%C : %C : %C : %C\r\n", L'a', L'b', L'c', L'd');

		printf("l == %d\r\n", l);

}

void com_test()
{

		//str_test1();
		//str_test2();
		//str_test3();
		//str_test4();
		//str_test5();

		//str_test6();
		
		//com_test3();
		//com_conv();
		//com_hash_test();

		//com_vscwprintf_test();
		//com_vscwprintf_test2();
		//str_test();
		//itow_test();

		//test_bittest();
		//bug_test();

		//sort_test();
		//search_test();

		//escstr_test0();
		//escstr_test1();

		//escstr_test_buf0();

		//escstr_test_buf1();

		//buffer_test();

		//com_timer_test();

		com_str_test_vscwprintf();

}



AR_NAMESPACE_END

#endif
