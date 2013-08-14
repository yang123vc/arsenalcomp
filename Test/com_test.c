
#include "test.h"

#include <string>
#include <math.h>
#include <time.h>

#include "Base64Transcoder.h"

#include "lfu_cache.h"



#if defined(__LIB)

AR_NAMESPACE_BEGIN


					

arString_t*		AR_CreateString();
void			AR_DestroyString(arString_t *str);

arStatus_t		AR_ReserveString(arString_t *str, size_t num);
void			AR_ClearString(arString_t *str);
arStatus_t		AR_AppendString(arString_t *str, const wchar_t *sour);

arStatus_t		AR_FormatString(arString_t *str, const wchar_t *fmt, ...);
arStatus_t		AR_AppendFormatString(arString_t *str, const wchar_t *fmt, ...);


const wchar_t*	AR_GetStringCString(const arString_t *str);
size_t			AR_GetStringLength(const arString_t *str);



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

		
		printf("len  == %d\r\n", AR_GetStringLength(str));
		getchar();
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));

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
		
		ar_int_32_t num;
		
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
		const wchar_t *s = L"中文中文中文中文";


		//const wchar_t *s = L"";
		
		tmp = AR_wcs_to_str(AR_CP_UTF8, s, AR_wcslen(s));

		wtmp = AR_str_to_wcs(AR_CP_UTF8,tmp, AR_strlen(tmp));
		
		AR_DEL(tmp); AR_DEL(wtmp);
}


void com_conv2()
{
		char buf[1024];
		size_t l;
		l = AR_wcs_to_str_buf(AR_CP_UTF8, L"", 0, buf, 1024);
		buf[l] = 0;

		const wchar_t *s = L"中文";
		l = AR_wcs_to_str_buf(AR_CP_UTF8, s, AR_wcslen(s), buf, 1024);
		buf[l] = 0;
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
		AR_va_start(arg_ptr, fmt);
		len = AR_vscwprintf(fmt, arg_ptr);
		printf("--------------------------\r\n");
		printf("AR_vscwprintf == %d\r\n", len);
		//printf("_vscwprintf == %d\r\n", _vscwprintf(fmt, arg_ptr));
		
		AR_va_end(arg_ptr);
}


void com_vscwprintf_test()
{
		ar_uint_64_t  x = 7332201052963203716;

		__format_v(L"%       .f%*.*f\r\n", 3344.333333333333333333333333, 20,20,3344.333333333333333333333333);

		__format_v(L"%33.54f", 3.14);

		__format_v(L"%d:%u:%qd:%ls:%f\r\n", 33,44,(ar_uint_64_t)198401010, L"aaaaaaaaa",3.1415);

		__format_v(L"abcdefg");
		
		
		
		
		__format_v(L"%qd", x);
		__format_v(L"%I64d", x); //崩溃
		__format_v(L"%I64ld", x); //崩溃

}


void com_vscwprintf_test2()
{
		__format_v(L"%*333.333Id : %Id\r\n", 0,(size_t)1024, (size_t)2048);

		__format_v(L"%*Id : %d : %qd : %qd : %d : %ld\r\n", 3,(size_t)111, (ar_int_32_t)222, (ar_int_64_t)333, (ar_int_64_t)444, (int)555, (int)666);
		__format_v(L"%*Id : %Id\r\n", 5,(size_t)1024, (size_t)2048);

}

/*****************************************************************************************************************/

typedef struct __ac_automaton_tag		arACAutomaton_t;
arACAutomaton_t*	AR_CreateACAutomaton();
void				AR_DestroyACAutomaton(arACAutomaton_t *atm);
ar_bool_t				AR_InsertToACAutomaton(arACAutomaton_t *atm, const wchar_t *str, void *val);
ar_bool_t				AR_RemoveFromACAutomaton(arACAutomaton_t *atm, const wchar_t *str);
ar_bool_t				AR_BuildACAutomaton(arACAutomaton_t *atm);

ar_bool_t				AR_FindFromACAutomaton(arACAutomaton_t *atm, const wchar_t *key, ar_bool_t sub_str);



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
				ar_int_64_t		num;
				const wchar_t *s, *p;
				s = L"-11111111111111111111111111111111";
				
				p = AR_wtoi64(s, &num, 10);
				

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %I64d\r\n", num);
//				printf("_wcstoi64 == %I64d\r\n", _wcstoi64(s, (wchar_t**)&next, 10));
				//AR_printf(L"next == %ls\r\n", next);
		}
		

		{
				ar_uint_32_t		num;
				const wchar_t *s = L"11111111111111111111111111111111";

				const wchar_t *p = AR_wtou32(s, &num, 10);
				const wchar_t *next;

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %u\r\n", num);
				printf("strtoul == %u\r\n", wcstoul(s, (wchar_t**)&next, 10));
				AR_printf(L"next == %ls\r\n", next);
		}


		{
				ar_int_32_t		num;
				const wchar_t *s = L"-11111111111111111111111111111111";
				
				const wchar_t *p = AR_wtoi32(s, &num, 10);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));

		}


		{
				ar_int_32_t		num;
				const wchar_t *s = L"11111111111111111111111111111111";
				
				const wchar_t *p = AR_wtoi32(s, &num, 10);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));

		}

		{
				ar_int_32_t		num;
				const wchar_t *s = L"0x468321xyz";
				
				const wchar_t *p = AR_wtoi32(s, &num, 0);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));


		}


		{
				ar_int_32_t		num;
				const wchar_t *s = L"-0x468321xyz";
				
				const wchar_t *p = AR_wtoi32(s, &num, 0);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));
		}


		{
				ar_int_32_t		num;
				const wchar_t *s = L"0464321xyz";
				
				const wchar_t *p = AR_wtoi32(s, &num, 0);

				AR_printf(L"next == %ls\r\n", p);
				printf("num == %d\r\n", num);
				printf("wcstol == %d\r\n", wcstol(s, NULL, 10));
		}


				{
				ar_int_32_t		num;
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

		len = AR_scwprintf(L"%ls : %ls : %ls\r\n", L"ABCDEF猪八戒",L"ABCDEF猪八戒",L"ABCDEF猪八戒");

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
		AR_printf(L"%lld  :  %ld\r\n", (ar_uint_64_t)345,(ar_uint_64_t)453);

}


static void test_bittest()
{
		ar_uint_32_t x = 0;

		x = (ar_uint_32_t)AR_BIT_MARK(32);

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
						AR_swprintf(buf, 1024, L"invalid value %qd\r\n", err.value);
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
						AR_swprintf(buf, 1024, L"invalid value %qd\r\n", err.value);
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

				if(AR_GetBufferAvailable(buffer) > 4096)
				{
						fwrite((void*)AR_GetBufferData(buffer), 1, 4096, tmp);
						AR_EraseBuffer(buffer, 4096);
				}
		}

		fclose(f);

		if(AR_GetBufferAvailable(buffer) > 0)
		{
				fwrite((void*)AR_GetBufferData(buffer), 1, AR_GetBufferAvailable(buffer), tmp);
				AR_EraseBuffer(buffer, AR_GetBufferAvailable(buffer));
		}

		fclose(tmp);
		
		AR_DestroyBuffer(buffer);

}

void buffer_test2()
{
		arBuffer_t *buf = AR_CreateBuffer(1);

		AR_InsertCStringToBuffer(buf, "0123456789abcdef");
		const char *d = "zzzzzz";
		
		AR_ResetBufferData(buf, 15, (const ar_byte_t*)d, AR_strlen(d));

		ar_byte_t b = 0;
		AR_InsertToBuffer(buf, &b, 1);

		AR_printf(L"%hs\r\n", AR_GetBufferData(buf));

		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}

}

void buffer_test3()
{

		arBuffer_t *buf = AR_CreateBuffer(1);

		AR_InsertCStringToBuffer(buf, "0123456789abcdef");
		const char *d = "zzzzzz";
		
		AR_ResetBufferData(buf, 15, (const ar_byte_t*)d, AR_strlen(d));

		ar_byte_t b = 0;
		AR_InsertToBuffer(buf, &b, 1);

		AR_printf(L"%hs\r\n", AR_GetBufferData(buf));


		while(AR_GetBufferAvailable(buf) > 1)
		{
				size_t en = AR_EraseBufferBack(buf, 2);
				ar_byte_t b = 0;
				AR_InsertToBuffer(buf, &b, 1);

				AR_printf(L"%hs\r\n", AR_GetBufferData(buf));
				
				
		}

		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}
		
}

void com_timer_test()
{
		ar_uint_64_t		beg = AR_GetTime_Milliseconds();
		AR_Sleep(1000);
		ar_uint_64_t		end = AR_GetTime_Milliseconds();

		AR_printf(L"elapsed == %qd\r\n", end - beg);

}



void com_str_test_vscwprintf()
{
		ar_int_t l = AR_scwprintf(L"%C : %C : %C : %C\r\n", L'a', L'b', L'c', L'd');
		printf("l == %d\r\n", l);
}




void com_str_test_cmp()
{
		printf("AR_stricmp == %d\r\n", AR_stricmp("abcdef", "abcdefg"));
		printf("AR_strnicmp == %d\r\n", AR_strnicmp("abcdef", "abcdefg", 3));

		printf("AR_wcsicmp == %d\r\n", AR_wcsicmp(L"abcdef", L"abcdefg"));
		printf("AR_wcsnicmp == %d\r\n", AR_wcsnicmp(L"abcdef", L"abcdefg", 3));
}


void str_tbl_test()
{
		arStringTable_t	*tbl =	AR_CreateStrTable(1);

		const wchar_t *a, *b;

		a = AR_GetString(tbl, L"abc");
		b = AR_GetString(tbl, L"abc");


		AR_ASSERT(a == b);


}

#if(0)
void kmp_test()
{
		const wchar_t *s = L"abcdef";
		const wchar_t *p = L"c";
		size_t	n = AR_wcslen(s);
		AR_printf(L"%ls\r\n", AR_wcsstr_kmp(s,p));

		AR_printf(L"%ls\r\n", AR_wcsstr_kmp_s(s,s + n,p));
}
#endif

const wchar_t *AR_wcsstr_ex(const wchar_t *s, const wchar_t *p)
{
		for(s; *s != L'\0'; ++s)
		{
				size_t i;
				for(i = 0; p[i] != L'\0' && s[i] != L'\0' && p[i] == s[i]; ++i);

				if(p[i] == L'\0')return s;
		}

		return NULL;
		
}

#if(0)
void kmp_test2()
{
		const wchar_t *src = NULL;//__load_txt(L"z:\\1.txt");

		//AR_printf(L"%ls\r\n",src);

		for(int a = 0; a < 3; ++a)
		{
				const wchar_t *s = src;
				const wchar_t *p = L"abcdefhigklmnopqrstuvwxyz";
				size_t m = AR_wcslen(p);

				ar_uint_64_t beg = AR_GetTime_Milliseconds();
				size_t cnt = 0;
				while((s = AR_wcsstr_ex(s,p)) != NULL)
				{
						s += m;
						cnt ++;
				}
				ar_uint_64_t end = AR_GetTime_Milliseconds();
				AR_printf(L"elapsed == %qd match == %Id\r\n", end - beg, cnt);
		}
		
		getchar();

		for(int a = 0; a < 3; ++a)
		{
				const wchar_t *s = src;
				const wchar_t *p = L"abcdefhigklmnopqrstuvwxyz";
				size_t m = AR_wcslen(p);
				ar_uint_64_t beg = AR_GetTime_Milliseconds();
				size_t cnt = 0;
				while((s = AR_wcsstr_kmp(s,p)) != NULL)
				{
						s += m;
						cnt ++;
				}
				ar_uint_64_t end = AR_GetTime_Milliseconds();
				AR_printf(L"elapsed == %qd match == %d\r\n", end - beg, cnt);
		}


		AR_DEL(src);
		getchar();
}

#endif

#if(AR_COMPILER == AR_VC_LEGACY || AR_COMPILER == AR_VC)

static void test_align()
{
#pragma pack(push, 16)
		//32 actual align == 4
		//64 actual align == 8

		struct test
		{
				char a;			//offset == 0, 占用1字节
				//char __pad1[3];
				int  b;			//填充前offset == 1,A =4, A - 1 % A == 3,则补充3字节，填充后，offset = 4,
				long c;			//offset ==8 , A == 4,  8 % 4 == 0,						无填充
				short d;		//offset == 12, A == 2 12 % 2 == 0,						无填充
				//char __pad2[2];
				int *p;			//填充前offset == 14, A = 8, 14 % 8 == 6, A-6 == 2，   填充2,填充后，offset == 16, 占8字节
		};
#pragma pack(pop)
		
		printf("a == %d\r\n", AR_OFFSETOF(struct test, a));
		printf("b == %d\r\n", AR_OFFSETOF(struct test, b));
		printf("c == %d\r\n", AR_OFFSETOF(struct test, c));
		printf("d == %d\r\n", AR_OFFSETOF(struct test, d));
		printf("p == %d\r\n", AR_OFFSETOF(struct test, p));


		
		AR_printf(L"sizeof(test) == %d\r\n", sizeof(test));
}



static void test_align2()
{
#pragma pack(push,16)
		struct test
		{
				char	a;		//offset == 0, 占用1字节
				//char __pad[1];
				short	b;		//填充前 offset == 1, A = 2; 1 % A = 1; A-1 = 1; 填充1后，offset == 2;
				int		c;		//offset == 4, A = 4; 4 % 4 == 0
				long	d;		//offset == 8, A = 4; 8 % 4 == 0
				int		*p;		//offset == 12, A = 4; 12 % 4 == 0
		};
#pragma pack(pop)
		
		printf("a == %d\r\n", AR_OFFSETOF(struct test, a));
		printf("b == %d\r\n", AR_OFFSETOF(struct test, b));
		printf("c == %d\r\n", AR_OFFSETOF(struct test, c));
		printf("d == %d\r\n", AR_OFFSETOF(struct test, d));
		printf("p == %d\r\n", AR_OFFSETOF(struct test, p));


		
		AR_printf(L"sizeof(test) == %d\r\n", sizeof(test));
}


static void test_align3()
{

#pragma pack(push,16)
		struct test
		{
				int		a;
				double	b;
				char	c;
		};
#pragma pack(pop)

		
		printf("a == %d\r\n", AR_OFFSETOF(struct test, a));
		printf("b == %d\r\n", AR_OFFSETOF(struct test, b));
		printf("c == %d\r\n", AR_OFFSETOF(struct test, c));


		
		AR_printf(L"sizeof(test) == %d\r\n", sizeof(test));
}



#else

#define test_align
#define test_align2

#endif



/*
t_uint32 ByteOrder::FlipBytes(t_uint32 val)
{
		return (
				((val >> 24) &0x000000FF)|
				((val >> 8) & 0x0000FF00)|
				((val << 8) & 0x00FF0000)|
				((val << 24) & 0xFF000000)
				);
}

t_uint64 ByteOrder::FlipBytes(t_uint64 val)
{
		t_uint32 hi = t_uint32( val >> 32);
		t_uint32 lo = t_uint32(val  & 0xFFFFFFFF);

		t_uint64 fhi = t_uint64(FlipBytes(hi));
		t_uint64 flo = t_uint64(FlipBytes(lo));

		flo <<= 32;
		return (flo | fhi);

}
*/


#if defined(ARCH_LITTLE_ENDIAN)

#define AR_LTON_16(_n)			AR_BYTEFLIP_16((_n))

#define AR_LTON_32(_n)			AR_BYTEFLIP_32((_n))

#define AR_LTON_64(_n)			AR_BYTEFLIP_64((_n))

#else

#define AR_LTON_16(_n)			(_n)

#define AR_LTON_32(_n)			(_n)

#define AR_LTON_64(_n)			(_n)

#endif





void byte_filp_test()
{
		ar_int_16_t		val16 = -0x1234;

		ar_int_32_t		val32 = -0x12345678;
		
		ar_int_64_t		val64 = -0xAA12345678ABCDEF;


		AR_printf(L"%d\r\n", val16);
		AR_printf(L"%d\r\n", val32);
		AR_printf(L"%qd\r\n", val64);

		
		AR_printf(L"%d\r\n", AR_BYTEFLIP_16(val16));
		AR_printf(L"%d\r\n", AR_BYTEFLIP_32(val32));
		AR_printf(L"%qd\r\n", AR_BYTEFLIP_64(val64));

		AR_printf(L"%d\r\n", AR_BYTEFLIP_16(AR_BYTEFLIP_16(val16)));
		AR_printf(L"%d\r\n", AR_BYTEFLIP_32(AR_BYTEFLIP_32(val32)));
		AR_printf(L"%qd\r\n", AR_BYTEFLIP_64(AR_BYTEFLIP_64(val64)));

		printf("------------------------------\r\n");

		ar_uint_16_t		uval16 = 0x1234;

		ar_uint_32_t		uval32 = 0x12345678;
		
		ar_uint_64_t		uval64 = 0xAA12345678ABCDEF;


		AR_printf(L"0x%X\r\n", uval16);
		AR_printf(L"0x%X\r\n", uval32);
		AR_printf(L"0x%qX\r\n", uval64);
		printf("------------------------------\r\n");
		
		AR_printf(L"0x%X\r\n", AR_BYTEFLIP_U16(uval16));
		AR_printf(L"0x%X\r\n", AR_BYTEFLIP_U32(uval32));
		AR_printf(L"0x%qX\r\n", AR_BYTEFLIP_U64(uval64));
		printf("------------------------------\r\n");

		AR_printf(L"0x%X\r\n", AR_BYTEFLIP_U16(AR_BYTEFLIP_U16(uval16)));
		AR_printf(L"0x%X\r\n", AR_BYTEFLIP_U32(AR_BYTEFLIP_U32(uval32)));
		AR_printf(L"0x%qX\r\n", AR_BYTEFLIP_U64(AR_BYTEFLIP_U64(uval64)));
		printf("------------------------------\r\n");


		


		
		//AR_printf(L"0x%X\r\n", AR_BYTEFLIP_32(val32));
		//AR_printf(L"0x%qX\r\n", AR_BYTEFLIP_64(val64));



/*
		AR_printf(L"0x%X\r\n", AR_LTON_16(val16));
		AR_printf(L"0x%X\r\n", AR_LTON_32(val32));
		AR_printf(L"0x%qX\r\n", AR_LTON_64(val64));
*/

}


static void shuffle(ar_int_t *arr, size_t n)
{
		ar_int_t i;
		for(i = (ar_int_t)n - 1; i > 0; --i)
		{
				ar_int_t idx = rand()%i;
				ar_int_t t = arr[i];
				arr[i] = arr[idx];
				arr[idx] = t;
		}
}

void algo_test1()
{
		ar_int_t a[] = {1,2,3,4,5,6}, i = 0;

		shuffle(a, sizeof(a)/sizeof(a[0]));

		for(i = 0; i < sizeof(a)/sizeof(a[0]); ++i)
		{
				AR_printf(L"%d ", a[i]);
		}

		AR_printf(L"\r\n");

}


static ar_int_t cmp_int(const void *l, const void *r)
{
		int a = *(int*)l;
		int b = *(int*)r;
		return AR_CMP(a,b);
}
void bsearch_test()
{
		size_t i;
		AR_printf(L"%ls\r\n", L"bsearch test");
		
		int arr[] = {0,1, 4, 42, 55, 67, 87, 100, 245};
		
		int key = 42;
		AR_ASSERT(AR_bsearch(&key, arr, AR_NELEMS(arr), sizeof(int), cmp_int) == 3);
		key = 43;
		AR_ASSERT(AR_bsearch(&key, arr, AR_NELEMS(arr), sizeof(int), cmp_int) == -1);

		for(i = 0; i < AR_NELEMS(arr); ++i)
		{
				ar_int_t result;
				key = arr[i];
				result = AR_bsearch(&key, arr, AR_NELEMS(arr), sizeof(int), cmp_int);
				AR_printf(L"key == %d : idx == %d\r\n", key, result);
				AR_ASSERT(result == i);
		}
		
		//assertEquals(2, Util.binarySearch(arrayWith42, 42));
		//assertEquals(-1, Util.binarySearch(arrayWith42, 43));
}




void float_test()
{
		
		AR_printf(L"%f\r\n", AR_FLT_MOD(5,0.8));
		AR_printf(L"%f\r\n", AR_FLT_MOD(5,0.8));

		AR_printf(L"%f\r\n", AR_DBL_MOD(5,4));
		AR_printf(L"%f\r\n", AR_DBL_MOD(5,4));

		AR_ASSERT(AR_DBL_EQ(AR_DBL_MOD(5,4), 1));
		AR_ASSERT(AR_DBL_EQ(AR_DBL_MOD(5,3), 2));
		AR_ASSERT(AR_DBL_EQ(AR_DBL_MOD(4,5), 4));


		double x = 0.11111111111111111111111111111;
		double y = 0.1111111111111111111111;

		AR_printf(L"x == y ? %ls\r\n", AR_DBL_LEEQ(x , y) ? L"true" : L"false");

		AR_printf(L"%.30f\r\n", DBL_EPSILON);
		AR_printf(L"%.30f\r\n", exp(3.0f));


}


static void float_test_2()
{
		const wchar_t *s = L"0123";
		
		AR_ASSERT(!AR_wcs_is_float(s, s + AR_wcslen(s)));


		s = L"0.123e";
		
		AR_ASSERT(!AR_wcs_is_float(s, s + AR_wcslen(s)));

		s = L"0.123e20";
		AR_ASSERT(AR_wcs_is_float(s, s + AR_wcslen(s)));

		s = L"0.123e+20";
		AR_ASSERT(AR_wcs_is_float(s, s + AR_wcslen(s)));

		s = L"0.123e-20";
		AR_ASSERT(AR_wcs_is_float(s, s + AR_wcslen(s)));


		s = L"0.123e20";
		double n = 0.0;

		s = AR_wtod_s(s, s + wcslen(s), &n);


		s = L"0.123e+20";
		n = 0.0;

		s = AR_wtod_s(s, s + wcslen(s), &n);


		s = L"0.123e-20";
		n = 0.0;

		s = AR_wtod_s(s, s + wcslen(s), &n);


		s = L"0.123e";
		n = 0.0;

		s = AR_wtod_s(s, s + wcslen(s), &n);

		{
				s = L"0.123e1";
				AR_ASSERT(!AR_wcs_is_int(s, s + wcslen(s)));

				s = L"0.123e";
				AR_ASSERT(AR_wcs_is_int(s, s + wcslen(s)));


				s = L"0x468321xyz";
				AR_ASSERT(AR_wcs_is_int(s, s + wcslen(s)));

				s = L"0464321xyz";
				AR_ASSERT(AR_wcs_is_int(s, s + wcslen(s)));

				s = L"0464321xyz";
				AR_ASSERT(AR_wcs_is_int(s, s + wcslen(s)));

				s = L"11111111111111111111111111111111";
				AR_ASSERT(!AR_wcs_is_int(s, s + wcslen(s)));

				s = L"-11111111111111111111111111111111";
				AR_ASSERT(!AR_wcs_is_int(s, s + wcslen(s)));

				s = L"11111111111111111111111111111111";
				AR_ASSERT(!AR_wcs_is_int(s, s + wcslen(s)));

				s = L"-0x468321xyz";
				AR_ASSERT(AR_wcs_is_int(s, s + wcslen(s)));



		}

		

		int x=33;

		
}



char*			AR_strupr(char *s);
char*			AR_strlwr(char *s);
char*			AR_strnlwr(char *s, size_t count);
char*			AR_strnupr(char *s, size_t count);
wchar_t*			AR_wcsupr(wchar_t *s);
wchar_t*			AR_wcslwr(wchar_t *s);
wchar_t*			AR_wcsnlwr(wchar_t *s, size_t count);
wchar_t*			AR_wcsnupr(wchar_t *s, size_t count);

void str_test7()
{
		{
		char s[] = "abcdefg";
		AR_printf(L"%hs\r\n", AR_strupr(s));
		}


		{
		char s[] = "ABCDEFG";
		AR_printf(L"%hs\r\n", AR_strlwr(s));
		}


		{
		wchar_t s[] = L"abcdefg";
		AR_printf(L"%ls\r\n", AR_wcsupr(s));
		}


		{
		wchar_t s[] = L"ABCDEFG";
		AR_printf(L"%ls\r\n", AR_wcslwr(s));
		}


		{
		char s[] = "abcdefg";
		AR_printf(L"%hs\r\n", AR_strnupr(s, 3));
		}

		{
		char s[] = "ABCDEFG";
		AR_printf(L"%hs\r\n", AR_strnlwr(s, 3));
		}

		{
		wchar_t s[] = L"abcdefg";
		AR_printf(L"%ls\r\n", AR_wcsnupr(s, 3));
		}

		{
		wchar_t s[] = L"ABCDEFG";
		AR_printf(L"%ls\r\n", AR_wcsnlwr(s, 3));
		}

}


void com_str_test_vcprintf()
{
		//const char *s = "abc";
		ar_int_t n11,n12,n21,n22;
		n11 = AR_scwprintf(L"%33s", L"abc");
		n12 = AR_scwprintf(L"%.10s", L"abc");


		n21 = AR_scprintf("%33s", "abc");
		n22 = AR_scprintf("%.10s", "abc");


}


void com_test_srpintf()
{
		printf("%llu\r\n", 0xffffffffffffffffui64);


}






void escstr_n_test1()
{
		wchar_t tmp[1024];

		while(true)
		{
				_getws(tmp);
				if(AR_wcscmp(tmp, L"quit") == 0)break;

				arEscStrErr_t	err;
				wchar_t *d = AR_escstr_to_str_n(tmp, rand() % 100, &err);
				
				wchar_t buf[1024];
				if(err.type == AR_ESCSTR_ERR_VALUE)
				{
						AR_swprintf(buf, 1024, L"invalid value %qd\r\n", err.value);
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


void escstr_n_test0()
{

		//const wchar_t *s = L"\\a\\b\\c\\d\\s";
		wchar_t buf[1024];

		while(true)
		{
				_getws(buf);
				if(AR_wcscmp(buf, L"quit") == 0)break;

				wchar_t *d = AR_str_to_escstr_n(buf, 10);

				wprintf(L"escape == %ls\r\n", d);
				AR_DEL(d);

		}
}


void escstr_test2()
{
		
		wchar_t *str = AR_escstr_to_str(L"\x652F\x6301\x7F51\x76FE\x7684\x62E6\x622A\x6B3A\x8BC8\x9493\x9C7C\x7F51\x7AD9\x3001\x62E6\x622A\x6728\x9A6C\x7F51\x7AD9\x4EE5\x53CA\x7F51\x8D2D\x4FDD\x9556\x7B49\x529F\x80FD\x3002", NULL);
		
		AR_error(AR_ERR_WARNING, L"%ls\r\n", str);
		
		
		AR_DEL(str);
		str = NULL;

		wchar_t *escstr = 	AR_str_to_escstr(L"\x13|360网页防护");

		AR_DEL(escstr);
		escstr = NULL;
		
}

void align_test()
{
		ar_byte_t *p1 = (ar_byte_t*)AR_malloc(37);
		ar_byte_t *p2 = (ar_byte_t*)AR_malloc(37);

		AR_memset(p1, 0, 37);
		AR_memset(p2, 0xdd, 37);

		AR_memswap(p1,p2, 37);

}




void text_test_load_save()
{

//#define INPUT_FILE		L"..\\..\\..\\misc\\txt_enc_test\\empty_ascii.txt"
#define INPUT_FILE		L"D:\\Code\\Solidus\\Compiler\\Arsenal\\misc\\txt_enc_test\\utf8_read_test.txt"
#define OUTPUT_FILE		L"E:\\test.txt"
#define OUTPUT_TYPE		AR_TXT_BOM_UTF_8

		arString_t *str = AR_CreateString();
		
		arTxtBom_t bom;

		if(AR_LoadBomTextFile(INPUT_FILE,&bom, str) != AR_S_YES)
		{
				AR_ASSERT(false);
		}


		if(AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStringCString(str)) != AR_S_YES)
		{
				AR_ASSERT(false);
		}


		
		if(AR_LoadBomTextFile(OUTPUT_FILE, &bom, str) != AR_S_YES)
		{
				AR_ASSERT(false);
		}

		AR_ASSERT(bom == OUTPUT_TYPE);


		AR_DestroyString(str);
}


static void str_vfmt_test(arString_t *str, const wchar_t *fmt, ...)
{
		va_list args;

		AR_va_start(args, fmt);
		AR_AppendVFormatString(str, fmt, args);

		
		//printf("%ls\r\n", AR_GetStringCString(str));

		AR_va_end(args);


		
}


void str_test8()
{
		arString_t *str	 = AR_CreateString();

		str_vfmt_test(str, L"%ls:%d:%f\r\n", L"aaa", 33, 3.14159265f);



		AR_DestroyString(str);
		str = NULL;
		
}



void str_test9()
{
		arString_t *str	 = AR_CreateString();

		for(int i = 0; i < 10; ++i)
		{

				str_vfmt_test(str, L"%ls:%d:%f\r\n", L"aaa", 33, 3.14159265f);
				str_vfmt_test(str, L"%ls", L"\r\n");
		}

		printf("%ls\r\n", AR_GetStringCString(str));

		AR_DestroyString(str);
		str = NULL;
		
}

void rand_test()
{
		AR_srand(time(NULL));
		for(size_t i = 0; i <10; ++i)
		{
				AR_printf(L"%f\r\n", AR_rand_dbl());
				AR_Sleep(1500);
		}
}


/*
size_t AR_base64_encode(ar_byte_t  *out, size_t olen, const ar_byte_t *input, size_t ilen);
size_t AR_base64_decode(ar_byte_t  *out, size_t olen, const ar_byte_t *input, size_t ilen);
*/


static void env_test()
{
		arString_t *str;

		str = AR_CreateString();

		AR_setenv(L"solidus", L"abc_xyz");

		AR_getenv(L"solidus", str);
		AR_ASSERT(AR_hasenv(L"solidus") == AR_S_YES);
		AR_StrPrint(str);


		AR_getenv(L"HOMEDRIVE", str);
		AR_StrPrint(str);

		AR_getenv(L"HOMEPATH", str);
		AR_StrPrint(str);
}

static void path_test()
{
		arString_t *str;
		arStatus_t status;
		str = AR_CreateString();

		status = AR_GetCurrentPath(str);

		AR_ASSERT(status == AR_S_YES);
		AR_StrPrint(str);


		status = AR_GetHomePath(str);
		AR_ASSERT(status == AR_S_YES);
		AR_StrPrint(str);


		status = AR_GetTempPath(str);
		AR_ASSERT(status == AR_S_YES);
		AR_StrPrint(str);

		status = AR_GetExpandPath(L"C:\\temp", str);
		AR_ASSERT(status == AR_S_YES);
		AR_StrPrint(str);

		AR_DestroyString(str);
		str = NULL;
}


static void path_iter_test()
{
		
		arPathIter_t *it;
		arStatus_t status;
		it = AR_CreatePathIterator();


		AR_ASSERT(it != NULL);


		status = AR_PathIteratorSetPath(it, L"%Temp%\\");

		while(status == AR_S_YES && !AR_PathIteratorIsDone(it))
		{
				//::MessageBoxW(NULL, AR_PathIteratorCurrent(it), 0,0);

				wchar_t buf[2048];
				AR_swprintf(buf, 2048, L"%ls%ls", AR_PathIteratorPath(it), AR_PathIteratorCurrent(it));

				AR_printf(L"%ls\r\n", buf);
				
				status = AR_PathIteratorNext(it);
		}



		status = AR_PathIteratorSetPath(it, L"D:\\1");

		while(status == AR_S_YES && !AR_PathIteratorIsDone(it))
		{
				//::MessageBoxW(NULL, AR_PathIteratorCurrent(it), 0,0);

				wchar_t buf[2048];
				AR_swprintf(buf, 2048, L"%ls%ls", AR_PathIteratorPath(it), AR_PathIteratorCurrent(it));

				AR_printf(L"%ls\r\n", buf);
				
				status = AR_PathIteratorNext(it);
		}


		status = AR_PathIteratorSetPath(it, L"D:\\nodir");

		while(status == AR_S_YES && !AR_PathIteratorIsDone(it))
		{
				//::MessageBoxW(NULL, AR_PathIteratorCurrent(it), 0,0);

				wchar_t buf[2048];
				AR_swprintf(buf, 2048, L"%ls%ls", AR_PathIteratorPath(it), AR_PathIteratorCurrent(it));

				AR_printf(L"%ls\r\n", buf);
				
				status = AR_PathIteratorNext(it);
		}

		status = AR_PathIteratorSetPath(it, L"..");

		while(status == AR_S_YES && !AR_PathIteratorIsDone(it))
		{
				//::MessageBoxW(NULL, AR_PathIteratorCurrent(it), 0,0);

				wchar_t buf[2048];
				AR_swprintf(buf, 2048, L"%ls%ls", AR_PathIteratorPath(it), AR_PathIteratorCurrent(it));

				AR_printf(L"%ls\r\n", buf);
				
				status = AR_PathIteratorNext(it);
		}


		AR_DestroyPathIterator(it);
		it = NULL;
}



static void evt_test()
{
		arStatus_t status;
		arEvent_t *evt = AR_CreateEvent(false);

		status = AR_WaitEventWithTimeout(evt, 5000);

		printf("%d\r\n", status);

		AR_SetEvent(evt);

		status = AR_WaitEventWithTimeout(evt, 5000);

		printf("%d\r\n", status);

		AR_ResetEvent(evt);


		status = AR_WaitEventWithTimeout(evt, 5000);

		printf("%d\r\n", status);
}



static void output_thread(void *data)
{
		arStatus_t status;
		arAsyncQueue_t	*queue;
		AR_ASSERT(data != NULL);
		queue = (arAsyncQueue_t*)data;

		while(true)
		{
				status = AR_GetFromAsyncQueueWithTimeout(queue, &data, 2000);
				if(status == AR_S_YES)
				{
						const char *s = (const char*)data;
						AR_ASSERT(s != NULL);
						printf("thread == %d : %s\r\n", GetCurrentThreadId(), s);
						
						if(strcmp(s, "quit") == 0)
						{

								AR_DEL(s);
								break;
						}else
						{
								AR_DEL(s);
						}

				}else if(status == AR_E_TIMEOUT)
				{
						printf("thread %d timeout\r\n", GetCurrentThreadId());
						continue;
				}else
				{
						AR_ASSERT(false);
				}
		}

		
}




#define THREAD_CNT  20
static void async_queue_test()
{
		int i;
		char buf[1024];
		arThread_t *thread_set[THREAD_CNT];
		arAsyncQueue_t	queue;
		AR_InitAsyncQueue(&queue);

		for(i = 0; i < THREAD_CNT; ++i)
		{
				thread_set[i] = AR_CreateThread(output_thread, (void*)&queue);
		}

		printf("请输入:\r\n");
		while(true)
		{
				char *s = NULL;
				gets(buf);
				s = AR_NEWARR(char, 1024);
				strcpy(s, buf);
				AR_PutToAsyncQueue(&queue, (void*)s);
				if(strcmp(buf, "quit")== 0)break;
		}

		for(i = 0; i < THREAD_CNT; ++i)
		{
				char *s = AR_NEWARR(char, 1024);
				strcpy(s, "quit");
				AR_PutToAsyncQueue(&queue, (void*)s);
		}

		for(i = 0; i < THREAD_CNT; ++i)
		{
				AR_DestroyThread(thread_set[i]);
				thread_set[i] = NULL;
		}

		while(AR_AsyncQueueIsEmpty(&queue) != AR_S_YES)
		{
				const char *s;
				AR_GetFromAsyncQueue(&queue, (void**)&s);
				printf("delete string == %s\r\n", s);
				AR_DEL(s);
		}

		AR_UnInitAsyncQueue(&queue);
}



static void async_queue_test2()
{
		arAsyncQueue_t	queue;
		arStatus_t status;
		void *result;
		AR_InitAsyncQueue(&queue);

		const char *s = "abcdefg";

		/*
		status = AR_PutToAsyncQueue(&queue, (void*)s);

		if(status != AR_S_YES)
		{
				AR_abort();
		}
		*/

		status = AR_GetFromAsyncQueueWithTimeout(&queue, &result, 5000);




		AR_UnInitAsyncQueue(&queue);
}

void thd_test()
{
		//evt_test();
		async_queue_test();
		//async_queue_test2();
}

#define fail_if(expr, msg)                              \
		if(expr) {                                            \
		fprintf(stderr, "%s:%d Assertion '%s' met: %s\n" ,  \
		__FILE__, __LINE__, #expr, msg);            \
		unitfail++;                                         \
		}


#define fail_unless(expr, msg)                           \
  if(!(expr)) {                                          \
    fprintf(stderr, "%s:%d Assertion '%s' failed: %s\n", \
            __FILE__, __LINE__, #expr, msg);             \
    unitfail++;                                          \
  }

#define verify_memory(dynamic, check, len)                              \
  if(dynamic && memcmp(dynamic, check, len)) {                          \
    fprintf(stderr, "%s:%d The dynamic string didn't match '%s'\n",     \
            __FILE__, __LINE__, check);                                 \
    unitfail++;                                                         \
  }

/* fail() is for when the test case figured out by itself that a check
   proved a failure */
#define fail(msg) do {                                                 \
    fprintf(stderr, "%s:%d test failed: '%s'\n",                       \
            __FILE__, __LINE__, msg);                                  \
    unitfail++;                                                        \
  } WHILE_FALSE


/* The abort macros mark the current test step as failed, and exit the test */
#define abort_if(expr, msg)                                   \
  if(expr) {                                                  \
    fprintf(stderr, "%s:%d Abort assertion '%s' met: %s\n" ,  \
            __FILE__, __LINE__, #expr, msg);                  \
    unitfail++;                                               \
    goto unit_test_abort;                                     \
  }

#define abort_unless(expr, msg)                                \
  if(!(expr)) {                                                \
    fprintf(stderr, "%s:%d Abort assertion '%s' failed: %s\n", \
            __FILE__, __LINE__, #expr, msg);                   \
    unitfail++;                                                \
    goto unit_test_abort;                                      \
  }

#define abort_test(msg) do {                                  \
    fprintf(stderr, "%s:%d test aborted: '%s'\n",             \
            __FILE__, __LINE__, msg);                         \
    unitfail++;                                               \
    goto unit_test_abort;                                     \
  } WHILE_FALSE



extern int unitfail;

#define UNITTEST_START                          \
  int test(char *arg)                           \
  {                                             \
  (void)arg;                                    \
  if (unit_setup()) {                           \
    fail("unit_setup() failure");               \
  } else {

#define UNITTEST_STOP                           \
    goto unit_test_abort; /* avoid warning */   \
unit_test_abort:                                \
    unit_stop();                                \
  }                                             \
  return unitfail;                              \
  }



/*

arList_t*       AR_CreateList(AR_list_destroy_func_t dtor, void *ctx);
void            AR_DestroyList(arList_t *lst);
void			AR_ClearList(arList_t *lst);
arStatus_t      AR_InsertToList(arList_t *lst, arListNode_t *node, void *data);
void			AR_RemoveFromList(arList_t *lst, arListNode_t *node);
size_t          AR_GetListCount(const arList_t *lst);
*/


static arStatus_t		ds_copy_func(void *data, void **pnew_data, void *ctx)
{
		AR_UNUSED(ctx);
		*pnew_data = data;
		return AR_S_YES;
}

static void ds_dtor_test(void *data, void *ctx)
{
		AR_UNUSED(data);
		AR_UNUSED(ctx);
}
static void *usr_ctx = (void*)0x1234;

static void list_test()
{
		arList_t *lst = AR_CreateList(ds_copy_func, ds_dtor_test, usr_ctx), *lst_dest = AR_CreateList(ds_copy_func, ds_dtor_test, usr_ctx);

		if(lst == NULL || lst_dest == NULL)
		{
				AR_CHECK(false, L"Create list failed");
		}

		/**************************************************************/

		int unusedData_case1 = 1;
		int unusedData_case2 = 2;
		int unusedData_case3 = 3;
		arListNode_t *head;
		arListNode_t *node_next;
		arListNode_t *node_prev;
		arListNode_t *to_remove;
		size_t llist_size = AR_GetListCount(lst);
		int err_code = 0;

		/**
		* testing llist_init
		* case 1:
		* list initiation
		* @assumptions:
		* 1: list size will be 0
		* 2: list head will be NULL
		* 3: list tail will be NULL
		* 4: list dtor will be NULL
		*/
		
		AR_CHECK(lst->count == 0, L"list initial size should be zero");
		AR_CHECK(lst->head == NULL, L"list head should initiate to NULL");
		AR_CHECK(lst->tail == NULL, L"list tail should intiate to NULL");
		AR_CHECK(lst->copy_f == ds_copy_func, L"list dtor shold initiate to ds_dtor_test");
		AR_CHECK(lst->dtor_f == ds_dtor_test, L"list dtor shold initiate to ds_dtor_test");





		arStatus_t status;
		status = AR_InsertToList(lst, lst->head, &unusedData_case1);
		if(status == AR_S_YES) 
		{
				AR_CHECK(AR_GetListCount(lst) == 1,L"List size should be 1 after adding a new element");
				/*test that the list head data holds my unusedData */
				AR_CHECK(lst->head->data == &unusedData_case1, L"List size should be 1 after adding a new element");
				/*same goes for the list tail */
				AR_CHECK(lst->tail == lst->head, L"List size should be 1 after adding a new element");

				/**
				* testing AR_InsertToList
				* case 2:
				* list has 1 element, adding one element after the head
				* @assumptions:
				* 1: the element next to head should be our newly created element
				* 2: the list tail should be our newly created element
				*/

				status = AR_InsertToList(lst, lst->head, &unusedData_case3);
				if(status == AR_S_YES) 
				{
						AR_CHECK(lst->head->next->data == &unusedData_case3, L"the node next to head is not getting set correctly");
						AR_CHECK(lst->tail->data == &unusedData_case3, L"the list tail is not getting set correctly");
				}
				else 
				{
						printf("skipping AR_InsertToList as a non success error code was returned\n");
				}

				/**
				* testing AR_InsertToList
				* case 3:
				* list has >1 element, adding one element after "NULL"
				* @assumptions:
				* 1: the element next to head should be our newly created element
				* 2: the list tail should different from newly created element
				*/

				status = AR_InsertToList(lst, lst->head, &unusedData_case2);
				if(status == status) 
				{
						AR_CHECK(lst->head->next->data == &unusedData_case2, L"the node next to head is not getting set correctly");
						/* better safe than sorry, check that the tail isn't corrupted */
						AR_CHECK(lst->tail->data != &unusedData_case2, L"the list tail is not getting set correctly");
				}
				else 
				{
						printf("skipping AR_InsertToList as a non "
								"success error code was returned\n");
				}

		}
		else {
				printf("skipping AR_InsertToList as a non "
						"success error code was returned\n");
		}




		/*		 unit tests for AR_RemoveFromList */

		/**
		* case 1:
		* list has >1 element, removing head
		* @assumptions:
		* 1: list size will be decremented by one
		* 2: head will be the head->next
		* 3: "new" head's previous will be NULL
		*/

		head=lst->head;
		AR_CHECK(head, L"lst->head is NULL");
		node_next = head->next;
		llist_size = AR_GetListCount(lst);

		AR_RemoveFromList(lst, lst->head);

		AR_CHECK(AR_GetListCount(lst) ==  (llist_size-1),
				L"lst size not decremented as expected");
		AR_CHECK(lst->head == node_next,
				L"lst new head not modified properly");
		AR_CHECK(lst->head, L"lst->head is NULL");
		AR_CHECK(lst->head->prev == NULL,
				L"new head previous not set to null");

		/**
		* case 2:
		* removing non head element, with list having >=2 elements
		* @setup:
		* 1: insert another element to the list to make element >=2
		* @assumptions:
		* 1: list size will be decremented by one ; tested
		* 2: element->previous->next will be element->next
		* 3: element->next->previous will be element->previous
		*/
		AR_InsertToList(lst, lst->head, &unusedData_case3);
		llist_size = AR_GetListCount(lst);
		to_remove = lst->head->next;
		AR_CHECK(to_remove, L"to_remove is NULL");
		node_next = to_remove->next;
		node_prev = to_remove->prev;
		AR_RemoveFromList(lst, to_remove);
		AR_CHECK(node_prev->next == node_next,
				L"element previous->next is not being adjusted");
		AR_CHECK(node_next, L"next is NULL");
		AR_CHECK(node_next->prev == node_prev,
				L"element next->previous is not being adjusted");

		/**
		* case 3:
		* removing the tail with list having >=1 element
		* @assumptions
		* 1: list size will be decremented by one ;tested
		* 2: element->previous->next will be element->next ;tested
		* 3: element->next->previous will be element->previous ;tested
		* 4: list->tail will be tail->previous
		*/

		to_remove = lst->tail;
		node_prev = to_remove->prev;
		AR_RemoveFromList(lst, to_remove);
		AR_CHECK(lst->tail == node_prev,
				L"lst tail is not being adjusted when removing tail");

		/**
		* case 4:
		* removing head with list having 1 element
		* @assumptions:
		* 1: list size will be decremented by one ;tested
		* 2: list head will be null
		* 3: list tail will be null
		*/

		to_remove = lst->head;
		AR_RemoveFromList(lst, to_remove);
		AR_CHECK(lst->head == NULL,
				L"lst head is not NULL while the lst is empty");
		AR_CHECK(lst->tail == NULL,
				L"lst tail is not NULL while the lst is empty");




		AR_DestroyList(lst);
		AR_DestroyList(lst_dest);
		lst = NULL;
		lst_dest = NULL;
}

static ar_uint_64_t		hash_test(void *key, void *ctx)
{
		return (ar_uint_64_t)key;
}


ar_int_t		comp_test(void *l, void *r, void *ctx)
{
		return AR_CMP(l, r);
}

ar_bool_t		hash_visit_test(void *k, void *v, void *ctx)
{
		AR_printf(L"on hash visit func key == %qd, v == %qd\r\n", k,v);
		return true;
}


static void hash_test2()
{
		arHash_t *hash;

		hash = AR_CreateHash(3079, hash_test, comp_test, NULL, NULL, NULL, NULL, NULL);

		AR_srand(time(NULL));


		for(size_t i = 0; i < 10000000; ++i)
		{
				ar_uint_64_t k = AR_rand64() % 139;
				ar_uint_64_t v = AR_rand64() % 10000;
				arStatus_t s =  AR_SetToHash(hash, (void*)k, (void*)v);
				AR_ASSERT(s == AR_S_YES);
		}


		for(size_t i = 0; i < 10000; ++i)
		{
				ar_uint_64_t k = AR_rand64() % 139;
				void *pd;
				arStatus_t s =  AR_FindFromHash(hash, (void*)k, &pd);
				if(s == AR_S_YES)
				{
						AR_printf(L"%ls : %qu\r\n", L"hit", pd);
				}
		}

		AR_HashForEach(hash, hash_visit_test);
		AR_DestroyHash(hash);
		hash = NULL;
}




ar_uint_64_t		__wstring_hash_func(void *key, void *ctx)
{
		std::wstring *pwstr = (std::wstring*)key;
		AR_ASSERT(pwstr != NULL);
		return AR_wcshash(pwstr->c_str());
}

ar_int_t	__wstring_comp_func(void *l, void *r, void *ctx)
{
		return AR_wcscmp(    ((std::wstring*)l)->c_str(), ((std::wstring*)r)->c_str());
}

arStatus_t	__wstring_copy_key_func(void *key, void **new_key, void *ctx)
{
		if(1)//AR_rand32() % 2)
		{
				std::wstring *new_wcs = new std::wstring(((std::wstring*)key)->c_str());
				*new_key = (void*)new_wcs;
				return AR_S_YES;
		}else
		{
				return AR_E_NOMEM;
		}
}

void	__wstring_destroy_func(void *data, void *ctx)
{
		AR_ASSERT(data != NULL);
		delete (std::wstring*)data;
		data = NULL;
}




static void hash_test3()
{
		arHash_t *hash;

		hash = AR_CreateHash(13, __wstring_hash_func, __wstring_comp_func, __wstring_copy_key_func, __wstring_copy_key_func, __wstring_destroy_func, __wstring_destroy_func, L"ctx_test");

		AR_srand(time(NULL));


		for(size_t i = 0; i < 1000000; ++i)
		{
				wchar_t kt[512],vt[512];

				ar_uint_64_t k = AR_rand64() % 139;
				ar_uint_64_t v = AR_rand64() % 10000;
				
				AR_swprintf(kt, 512, L"%qu", k);
				AR_swprintf(vt, 512, L"%qu", v);

				std::wstring ks(kt), vs(vt);

				arStatus_t s =  AR_SetToHash(hash, (void*)&ks, (void*)&vs);
				if(s != AR_S_YES)
				{
						AR_error(AR_ERR_MESSAGE, L"low memory\r\n");
				}
		}


		for(size_t i = 0; i < 10000; ++i)
		{
				ar_uint_64_t k = AR_rand64() % 139;

				wchar_t kt[512];
				AR_swprintf(kt, 512, L"%qu", k);
				std::wstring ks(kt);

				std::wstring *pval = NULL;
				arStatus_t s =  AR_FindFromHash(hash, (void*)&ks, (void**)&pval);
				
				if(s == AR_S_YES)
				{
						AR_printf(L"%ls : %ls\r\n", L"hit", ((std::wstring*)pval)->c_str());
				}
		}

		AR_DestroyHash(hash);
		hash = NULL;
}


/*
arHashIter_t;
void	AR_InitHashIterator(arHash_t *hash, arHashIter_t *iter);
void	AR_UnInitHashIterator(arHashIter_t *iter);
ar_bool_t	AR_HashIteratorIsDone(const arHashIter_t *iter);
void	AR_HashIteratorNext(arHashIter_t *iter);
void*	AR_GetHashIteratorKey(const arHashIter_t *iter);
void*	AR_GetHashIteratorData(const arHashIter_t *iter);
*/
static void hash_test4()
{
		arHashIter_t iter;
		arHash_t *hash;
		
		hash = AR_CreateHash(139000, __wstring_hash_func, __wstring_comp_func, __wstring_copy_key_func, __wstring_copy_key_func, __wstring_destroy_func, __wstring_destroy_func, L"ctx_test");

		AR_srand(time(NULL));

		for(size_t i = 0; i < 1000000; ++i)
		{
				wchar_t kt[512],vt[512];

				ar_uint_64_t k = AR_rand64() % 13900;
				ar_uint_64_t v = AR_rand64() % 10000;
				
				AR_swprintf(kt, 512, L"%qu", k);
				AR_swprintf(vt, 512, L"%qu", v);

				std::wstring ks(kt), vs(vt);

				arStatus_t s =  AR_SetToHash(hash, (void*)&ks, (void*)&vs);
				if(s != AR_S_YES)
				{
						AR_error(AR_ERR_MESSAGE, L"low memory\r\n");
				}
		}

		AR_InitHashIterator(hash, &iter);
		size_t cnt = 0;
		while(!AR_HashIteratorIsDone(&iter))
		{
				const std::wstring *key = (const std::wstring*)AR_GetHashIteratorKey(&iter);
				const std::wstring *data = (const std::wstring*)AR_GetHashIteratorData(&iter);
				AR_printf(L"%ls : %ls\r\n", key->c_str(), data->c_str());
				AR_HashIteratorNext(&iter);
				cnt++;
		}

		AR_ASSERT(cnt == AR_GetHashCount(hash));
		AR_UnInitHashIterator(&iter);
		AR_DestroyHash(hash);
		hash = NULL;

}

void ds_test2()
{
		//list_test();
		hash_test2();
		hash_test3();
		hash_test4();
}



static const wchar_t*  __parse_ip(const wchar_t *s, wchar_t *ip)
{
        ar_uint_32_t num[4];
        size_t i;
        const wchar_t *p;

        AR_ASSERT(s != NULL && ip != NULL);
        
        p = s;
        for(i = 0; i < 4; ++i)
        {
                p = AR_wcstrim_space(p);
                p = AR_wtou32(p, &num[i], 10);
                if(p == NULL || num[i] > 255)
                {
                        return NULL;
                }

				if(i < 3)
				{
						p = AR_wcstrim_space(p);
						if(*p != L'.')
						{
								return NULL;
						}
						++p;
				}
        }

		AR_swprintf(ip, 16, L"%u.%u.%u.%u", num[0], num[1], num[2], num[3]);

        p = AR_wcstrim_space(p);

		switch(*p)
		{
		case L'\0':
				return p;
		case L',':
				return AR_wcstrim_space(p + 1);
		default:
				return NULL;
		}
}


void str_test10()
{
		const wchar_t *ip = L"	192		.		168		.		\r\n	001		.		1		,		202		.		106		.		1		.213,	3.3.3.3	,			3	";
		wchar_t buf[20];

		do{
				ip = __parse_ip(ip, buf);
				if(ip != NULL)
				{
						AR_printf(L"%ls\r\n", buf);
				}
		}while(ip != NULL && *ip != L'\0');

		if(ip == NULL)
		{
				AR_printf(L"bad ip address\r\n");
		}

}

void str_test11()
{
		ar_uint_t hash1 = AR_strhash("abc中国def");
		ar_uint_t hash2 = AR_strhash_n("abc中国def", AR_strlen("abc中国def"));

		AR_ASSERT(hash1 == hash2);
}








static void* oper_test_func(void *usr_ctx)
{
		std::string *pstr = (std::string*)usr_ctx;
		AR_ASSERT(usr_ctx);
		AR_error(AR_ERR_MESSAGE, L"%hs\r\n", AR_FUNC_NAME);
		AR_error(AR_ERR_MESSAGE, L"%hs\r\n", pstr->c_str());
		
		::Sleep(5000);

		std::string *result = new std::string("xxxxxx");

		return (void*)result;
}		



void	oper_test_destroy(void *result, void *usr_ctx)/*此函数在销毁operation时，如果operation为finished并且结果未被取走时执行*/
{
		AR_error(AR_ERR_MESSAGE, L"%hs\r\n", AR_FUNC_NAME);
		
		std::string *pstr = (std::string*)usr_ctx;
		std::string *presult = (std::string*)result;
		AR_ASSERT(usr_ctx);
		AR_error(AR_ERR_MESSAGE, L"%hs\r\n", pstr->c_str());

		AR_error(AR_ERR_MESSAGE, L"result == %hs\r\n", presult->c_str());

		
		delete presult;
		presult = NULL;
}






static void str_test12()
{
		wprintf(L"%S\r\n", "abcdefg");

		AR_printf(L"%ls\r\n", L"ls");
		AR_printf(L"%lS\r\n", L"lS");
		AR_printf(L"%s\r\n", L"s");
		AR_printf(L"%S\r\n", L"S");


		AR_printf(L"%hs\r\n", "hs");
		AR_printf(L"%hS\r\n", "hS");
		AR_printf(L"%s\r\n", L"s");
		AR_printf(L"%S\r\n", L"S");

		AR_printf(L"%hS : %lS\r\n", "hS", L"lS");
		AR_printf(L"%s : %S\r\n", "s", L"S");
		AR_printf(L"%hs : %ls\r\n", "ls", L"lS");
}

/****************************************************************************************************************/



/****************************************************************************************************************/

static void str_test13()
{

		const char *s = "ffffef";
		size_t l = 0;
		
		printf("wn == %d\r\n", AR_hexstr_to_data_s(s, s + strlen(s), NULL, l));

		ar_byte_t tmp[3];
		

		printf("wn == %d\r\n", AR_hexstr_to_data_s(s, s + strlen(s), tmp, 3));

		printf("%02X%02X%02X\r\n", tmp[0],tmp[1], tmp[2]);

#if(0)
		
		ar_byte_t b[] = {0xff,0xef,0x10};
		char buf[10];
		l = 10;
		
		ar_int_t wn = AR_data_to_hexstr(b, sizeof(b), buf, l);
		printf("buf == %s : l == %d\r\n", buf,wn);


		


		ar_byte_t data[16];
		ar_int_t wn = AR_data_to_hexstr(data, sizeof(data), NULL, 0);
		printf("l == %d\r\n",wn);

#endif


}


static void str_test14()
{
		/*
		printf("%lu\r\n", 3333);
		AR_scwprintf(L"%lu\r\n", 3333);
		*/
		/*
		long x = atol("3,1");
		AR_printf(L"%d\r\n", x);
		*/
		//printf("%c\r\n", 32);

		const BYTE k1[] = {48,49,2,42,1,62,55,104,219,237,75,50,190,199,4,135,186,182,7,101,83,174,85,96,200,51,57,55,95,223,79,94,45,160,47,7,252,19,237,70,41,78,136,247,102,123,2,3,1,0,1};
		const BYTE k2[] = {48,72,2,65,0,162,88,3,88,244,34,175,217,218,232,164,231,251,244,51,130,88,53,23,34,211,157,85,162,54,119,175,11,211,54,3,146,115,236,130,209,145,103,85,20,82,104,28,15,104,54,8,155,97,225,190,227,151,232,190,119,79,32,27,13,49,165,12,57,2,3,1,0,1};
		const BYTE k3[] = {48,129,137,2,129,129,0,209,13,16,148,228,73,18,11,61,206,192,22,145,121,226,190,86,114,42,144,160,39,114,31,58,90,51,128,96,35,116,209,194,152,52,75,223,113,148,73,139,177,171,201,246,4,103,124,7,30,143,181,131,234,58,81,23,28,47,181,36,219,209,126,117,197,44,229,79,200,122,203,79,50,134,124,215,145,185,46,24,214,157,211,221,156,56,182,191,249,32,93,141,125,35,190,49,211,76,201,0,79,252,201,17,202,57,247,86,75,8,221,6,42,35,92,219,149,119,133,27,83,227,1,118,162,43,21,2,3,1,0,1};


		printf("k1 == %u\r\nk2==%u\r\nk3 == %u\r\n", AR_NELEMS(k1),AR_NELEMS(k2), AR_NELEMS(k3));

}



/*str_rot13(base64_encode($url));解码：base64_decode(str_rot13($url));
例如"http://www.google.com/reader/view/"
"nUE0pQbiY3q3ql5ao29aoTHhL29gY3WyLJEypv92nJI3Yj=="
"nUE0pQbiY3q3ql5ao29aoTHhL29gY3WyLJEypv92nJI3Yj=="
*/




static void str_test15()
{
		ar_byte_t buf[1024];
		char output[1024];
		const char *url = "http://www.google.com/reader/view/";

		ar_int_t n = AR_base64_encode(buf, 1024, (const ar_byte_t*)url, strlen(url));
		buf[n] = 0;

		AR_printf(L"%hs\r\n", AR_strrot13((char*)buf, strlen((char*)buf)));

		AR_ASSERT(AR_strcmp((char*)buf, "nUE0pQbiY3q3ql5ao29aoTHhL29gY3WyLJEypv92nJI3Yj==") == 0);

		AR_strrot13((char*)buf, strlen((char*)buf));

		AR_printf(L"%u\r\n", AR_base64_decode((ar_byte_t*)output, 1024, buf, n));

		output[n] = 0;
		AR_printf(L"%hs\r\n", output);

		
		AR_ASSERT(AR_strcmp(output, url) == 0);

}



ar_uint_64_t		hash_func(void *key, void *usr_ctx)
{
        std::wstring *wcs = (std::wstring*)key;
        AR_ASSERT(key != NULL);
        return AR_wcshash(wcs->c_str());
}
        
ar_int_t			comp_func(void *l, void *r, void *usr_ctx)
{
        std::wstring *ls = (std::wstring*)l;
        std::wstring *rs = (std::wstring*)r;
        
        AR_ASSERT(ls != NULL && rs != NULL);
        return AR_wcscmp(ls->c_str(), rs->c_str());
}
        
arStatus_t		copy_wcs_func(void *data, void **pnew_data, void *usr_ctx)
{
        std::wstring *src = (std::wstring*)data;
        std::wstring **pdest = (std::wstring**)pnew_data;
        
        AR_printf(L"%ls\r\n", src->c_str());
        *pdest = AR_NEW(std::wstring);
        
        *pdest = new ((void*)*pdest) std::wstring;
        
        **pdest = *src;
        return AR_S_YES;
}




        
void destroy_wcs_func(void *data, void *usr_ctx)
{
        std::wstring *src = (std::wstring*)data;
        AR_ASSERT(data != NULL);
        src->~basic_string();
        AR_DEL(src);
}
        
        /*
        typedef struct __cache_lfu_context_tag
        {
                Cache_hash_func_t       hash_f;
                Cache_comp_func_t       comp_f;
                
                Cache_copy_func_t       copy_key_f;
                Cache_copy_func_t       copy_data_f;
                
                Cache_destroy_func_t    destroy_key_f;
                Cache_destroy_func_t    destroy_data_f;
                void    *usr_ctx;
        }cacheLFUCtx_t;
        */


void cache_test()
{

		for(int x = 0; x < 10000000; ++x)
		{
				AR_srand(time(NULL));
        cacheLFUCtx_t ctx = 
        {
                hash_func,
                comp_func,
                copy_wcs_func,
                copy_wcs_func,
                destroy_wcs_func,
                destroy_wcs_func,
                NULL
        };

        using namespace ARSpace;
        
        cacheLFU_t *lfu = Cache_CreateLFU(&ctx, 3079);
        
        for(int i = 0; i < 100000; ++i)
		{
				wchar_t buf[512];
                AR_swprintf(buf, 512, L"test:%d", i);
                std::wstring key, val;
                val = buf;
                AR_swprintf(buf, 512, L"%d", i);
                key = buf;
                
                if(Cache_InsertToLFU(lfu, &key, &val) != AR_S_YES)
                {
                        AR_ASSERT(false);
                }

                AR_swprintf(buf, 512, L"%d", AR_rand64() % 100000);
                key = buf;
                std::wstring *tmp;

				if(Cache_AccessFromLFU(lfu, (void*)&key, (void**)&tmp) == AR_S_YES)
                {
                        AR_printf(L"hit %ls\r\n", tmp->c_str());
                }

				AR_swprintf(buf, 512, L"%d", AR_rand64() % 100000);
                key = buf;

				if(Cache_DeleteFromLFU(lfu, (void*)&key) == AR_S_YES)
				{
						AR_printf(L"key '%ls' is removed\r\n", key.c_str());
				}

		}
        
        
		for(int i = 0; i < 100000; ++i)
		{
                std::wstring key;
                wchar_t buf[512];
                AR_swprintf(buf, 512, L"%d", rand() % 100000);
                key = buf;
                std::wstring *val;
				

                if(Cache_AccessFromLFU(lfu, (void*)&key, (void**)&val) == AR_S_YES)
                {
                        AR_printf(L"hit %ls\r\n", val->c_str());


                }
		}
        
		for(int i = 0; i < 100000; i++)
		{
				std::wstring key;
                wchar_t buf[512];
                AR_swprintf(buf, 512, L"%d", rand() % 100000);
                key = buf;

				if(Cache_DeleteFromLFU(lfu, (void*)&key) == AR_S_YES)
				{
						AR_printf(L"key '%ls' is removed\r\n", key.c_str());
				}
		}
        
        
        Cache_DestroyLFU(lfu);
        lfu = NULL;
		}
}


static void str_test16()
{
		{
		const wchar_t *host = L"a.b.c.d.e.f.g.baidu.com";
		const wchar_t *p;

		p = AR_reverse_wcschr(host, AR_wcslen(host), L'.');

		while(p != NULL)
		{
				AR_printf(L"%ls\r\n", p);
				p = AR_reverse_wcschr(host, AR_wcslen(host) - AR_wcslen(p), L'.');
		}

		p = AR_reverse_wcschr(host, AR_wcslen(host), L'x');
		AR_ASSERT(p == NULL);



		host = L"a..b..c..d..e..f..g..baidu..com";

		p = AR_reverse_wcsstr(host, AR_wcslen(host), L"..", 2);

		while(p != NULL)
		{
				AR_printf(L"%ls\r\n", p);
				p = AR_reverse_wcsstr(host, AR_wcslen(host) - AR_wcslen(p), L"..", 2);
		}

		p = AR_reverse_wcsstr(host, AR_wcslen(host), L"ccc", 3);
		AR_ASSERT(p == NULL);
		}
		/*********************************************************************/

		{
		const char *host = "a.b.c.d.e.f.g.baidu.com";
		const char *p;

		p = AR_reverse_strchr(host, AR_strlen(host), '.');

		while(p != NULL)
		{
				AR_printf(L"%hs\r\n", p);
				p = AR_reverse_strchr(host, AR_strlen(host) - AR_strlen(p), '.');
		}

		p = AR_reverse_strchr(host, AR_strlen(host), 'x');
		AR_ASSERT(p == NULL);

		

		host = "a..b..c..d..e..f..g..baidu..com";

		p = AR_reverse_strstr(host, AR_strlen(host), "..", 2);

		while(p != NULL)
		{
				AR_printf(L"%hs\r\n", p);
				p = AR_reverse_strstr(host, AR_strlen(host) - AR_strlen(p), "..", 2);
		}

		p = AR_reverse_strstr(host, AR_strlen(host), "ccc", 3);
		AR_ASSERT(p == NULL);
		}

}

static void str_test17()
{
		ar_byte_t buf[1024];
		char output[1024];
		const char *url = "http://weibo.com/";

		ar_int_t n = AR_base64_encode(buf, 1024, (const ar_byte_t*)url, strlen(url));
		buf[n] = 0;

		AR_printf(L"%hs\r\n", AR_strrot13((char*)buf, strlen((char*)buf)));

		output[0] = 0;
		AR_strcat(output, "c2M9MTA0fHxjbj0xMDTlhazlj7jlkI3np7B8fG9uPTEwNOWumOaWueWQjeensHx8b3U9aHR0cDovL3d3dy50YW9iYW8uY29tL3x8cHU9aHR0cDovL3d3dy4zNjAuY24vfHx0dD0xMDTmjqjojZDmloflrZd8fHR1PWh0dHA6Ly93d3cucXEuY29tLw==");

		//AR_strrot13(output, AR_strlen(output));

		n = AR_base64_decode(buf, 1024, (const ar_byte_t*)output, AR_strlen(output));
		buf[n] = 0;
		AR_printf(L"%hs\r\n", buf);
}





/****************************************************************************************/




/*********************************************************************************************/
static void str_test18()
{
		printf("------------------------\r\n");
		{
				ar_int_64_t		num;
				const char *s, *p;
				s = "-11111111111111111111111111111111";
				
				p = AR_stoi64(s, &num, 10);
				

				AR_printf(L"next == %hs\r\n", p);
				AR_printf(L"num == %Id\r\n", num);

		}
		

		{
				ar_uint_32_t		num;
				const char *s = "11111111111111111111111111111111";

				const char *p = AR_stou32(s, &num, 10);
				const char *next;

				AR_printf(L"next == %hs\r\n", p);
				printf("num == %u\r\n", num);
				printf("strtoul == %u\r\n", strtoul(s, (char**)&next, 10));
				AR_printf(L"next == %hs\r\n", next);
		}


		{
				ar_int_32_t		num;
				const char *s = "-11111111111111111111111111111111";
				
				const char *p = AR_stoi32(s, &num, 10);

				AR_printf(L"next == %hs\r\n", p);
				printf("num == %d\r\n", num);
				printf("strtol == %d\r\n", strtol(s, NULL, 10));

		}


		{
				ar_int_32_t		num;
				const char *s = "11111111111111111111111111111111";
				
				const char *p = AR_stoi32(s, &num, 10);

				AR_printf(L"next == %hs\r\n", p);
				printf("num == %d\r\n", num);
				printf("strtol == %d\r\n", strtol(s, NULL, 10));

		}

		{
				ar_int_32_t		num;
				const char *s = "0x468321xyz";
				
				const char *p = AR_stoi32(s, &num, 0);

				AR_printf(L"next == %hs\r\n", p);
				printf("num == %d\r\n", num);
				printf("strtol == %d\r\n", strtol(s, NULL, 10));


		}


		{
				ar_int_32_t		num;
				const char *s = "-0x468321xyz";
				
				const char	 *p = AR_stoi32(s, &num, 0);

				AR_printf(L"next == %hs\r\n", p);
				printf("num == %d\r\n", num);
				printf("strtol == %d\r\n", strtol(s, NULL, 10));
		}


		{
				ar_int_32_t		num;
				const char *s = "0464321xyz";
				
				const char *p = AR_stoi32(s, &num, 0);

				AR_printf(L"next == %hs\r\n", p);
				printf("num == %d\r\n", num);
				printf("strtol == %d\r\n", strtol(s, NULL, 10));
		}


				{
				ar_int_32_t		num;
				const char *s = "0464321xyz";
				
				const char *p = AR_stoi32(s, &num, 0);

				AR_printf(L"next == %hs\r\n", p);
				printf("num == %d\r\n", num);
				printf("strtol == %d\r\n", strtol(s, NULL, 10));
		}
}


static void str_test19()
{
		const char *str =    "lazy";
		const char *string = "The quick brown dog jumps over the lazy fox";
		char fmt1[] =   "         1         2         3         4         5";
		char fmt2[] =   "12345678901234567890123456789012345678901234567890";
  
		char *pdest;
		int  result;
		printf( "String to be searched:\n   %s\n", string );
		printf( "   %s\n   %s\n\n", fmt1, fmt2 );
		pdest = strstr((char*)string, str );
		result = (int)(pdest - string + 1);
		if ( pdest != NULL )
		{
				printf( "%s found at position %d\n", str, result );
		}
		else
		{
				printf( "%s not found\n", str );
		}


		pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
		result = (int)(pdest - string + 1);
		if ( pdest != NULL )
		{
				printf( "%s found at position %d\n", str, result );
		}
		else
		{
				printf( "%s not found\n", str );
		}

		str = "fox";
		pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
		result = (int)(pdest - string + 1);
		if ( pdest != NULL )
		{
				printf( "%s found at position %d\n", str, result );
		}
		else
		{
				printf( "%s not found\n", str );
		}


		/*******************************************************************************/

		str = "";
		pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
		
		if ( pdest != NULL )
		{
				printf( "found str : '%s'\n", pdest);
		}
		else
		{
				printf( "%s not found\n", str );
		}

		pdest = (char*)AR_strstr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
		
		if ( pdest != NULL )
		{
				printf( "found str : '%s'\n", pdest);
		}
		else
		{
				printf( "%s not found\n", pdest);
		}

		/*******************************************************************************/

		str = "";
		string = "";

		pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
		
		if ( pdest != NULL )
		{
				printf( "found str : '%s'\n", pdest);
		}
		else
		{
				printf( "%s not found\n", str );
		}

		pdest = (char*)AR_strstr_s(string, string + AR_strlen(string), str, str + AR_strlen(str));
		
		if ( pdest != NULL )
		{
				printf( "found str : '%s'\n", pdest);
		}
		else
		{
				printf( "%s not found\n", pdest);
		}


		/*******************************************************************************/

		str = "fox abc";
		string = "The quick brown dog jumps over the lazy fox";

		pdest = (char*)AR_strstr_s(string, string + AR_strlen(string), str, str + 3);
		
		if (pdest != NULL)
		{
				AR_ASSERT(pdest == AR_strstr(string, "fox"));
				printf( "found str : '%s'\n", pdest);
		}
		else
		{
				printf( "%s not found\n", str );
		}

		/*******************************************************************************/

		str = "FOX abc";
		string = "The quick brown dog jumps over the lazy fox";

		pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + 3);
		
		if (pdest != NULL)
		{
				AR_ASSERT(pdest == AR_strstr(string, "fox"));
				printf( "found str : '%s'\n", pdest);
		}
		else
		{
				printf( "%s not found\n", str );
		}


		/*******************************************************************************/

		str = "FOX abc";
		string = "The quick brown dog jumps over the lazy fox";

		pdest = (char*)AR_stristr_s(string, string + AR_strlen(string), str, str + 4);
		
		if (pdest != NULL)
		{
				AR_ASSERT(false);
				printf( "found str : '%s'\n", pdest);
		}
		else
		{
				printf( "%s not found\n", str );
		}



		/*******************************************************************************/
		const wchar_t *wcs = L"FOX abc";
		const wchar_t *wstr = L"The quick brown dog jumps over the lazy fox";
		const wchar_t *pwdest;

		pwdest = AR_wcsistr_s(wstr, wstr + AR_wcslen(wstr), wcs, wcs + 3);
		
		if(pwdest != NULL)
		{
				AR_ASSERT(pwdest == AR_wcsstr(wstr, L"fox"));
				printf("found str : '%ls'\n", pwdest);
		}
		else
		{
				printf( "%ls not found\n", wstr);
		}


		/*******************************************************************************/
		wcs = L"FOX abc";
		wstr = L"The quick brown dog jumps over the lazy fox";
		

		pwdest = AR_wcsistr_s(wstr, wstr + AR_wcslen(wstr), wcs, wcs + 4);
		
		if(pwdest != NULL)
		{
				AR_ASSERT(false);
				printf("found str : '%ls'\n", pwdest);
		}
		else
		{
				printf( "'%ls' not found\n", wstr);
		}


		/*******************************************************************************/
		wcs = L"FOX abc";
		wstr = L"The quick brown dog jumps over the lazy fox";
		

		pwdest = AR_wcsistr_s(wstr, wstr + AR_wcslen(wstr), wcs, wcs + 4);
		
		if(pwdest != NULL)
		{
				AR_ASSERT(false);
				printf("found str : '%ls'\n", pwdest);
		}
		else
		{
				printf( "'%ls' not found\n", wstr);
		}
}

extern void uri_test();


static ar_bool_t   is_ignore_url(const char *url)
{

        static const char *dns_top = "biz.com.edu.gov.info.int.mil.name.net.org.pro.aero.cat.coop.jobs.museum.travel.arpa.root.mobi.post.tel.asia.geo.kid.mail.sco.web.xxx.nato.example.invalid.test.bitnet.csnet.onion.uucp.xn--0zwm56d.xn--g6w251d";
		static const char *dns_nation = "ac.ad.ae.af.ag.ai.al.am.an.ao.aq.ar.as.at.au.aw.ax.az.ba.bb.bd.be.bf.bg.bh.bi.bj.bm.bn.bo.br.bs.bt.bv.bw.by.bz.ca.cc.cd.cf.cg.ch.ci.ck.cl.cm.cn.co.cr.cu.cv.cx.cy.cz.de.dj.dk.dm.do.dz.ec.ee.eg.eh.er.es.et.eu.fi.fj.fk.fm.fo.fr.ga.gb.gd.ge.gf.gg.gh.gi.gl.gm.gn.gp.gq.gr.gs.gt.gu.gw.gy.hk.hm.hn.hr.ht.hu.id.ie.il.im.in.io.iq.ir.is.it.je.jm.jo.jp.ke.kg.kh.ki.km.kn.kp.kr.kw.ky.kz.la.lb.lc.li.lk.lr.ls.lt.lu.lv.ly.ma.mc.md.me.mg.mh.mk.ml.mm.mn.mo.mp.mq.mr.ms.mt.mu.mv.mw.mx.my.mz.na.nc.ne.nf.ng.ni.nl.no.np.nr.nu.nz.om.pa.pe.pf.pg.ph.pk.pl.pm.pn.pr.ps.pt.pw.py.qa.re.ro.rs.ru.rw.sa.sb.sc.sd.se.sg.sh.si.sj.sk.sl.sm.sn.so.sr.st.su.sv.sy.sz.tc.td.tf.tg.th.tj.tk.tl.tm.tn.to.tp.tr.tt.tv.tw.tz.ua.ug.uk.um.us.uy.uz.va.vc.ve.vg.vi.vn.vu.wf.ws.ye.yt.yu.za.zm.zw.xn--fiqs8s.xn--fiqz9s.xn--j6w193g.xn--kprw13d.xn--kpry57d";
        const char *p, *b;
		ar_bool_t ignored;
        AR_ASSERT(url != NULL);

		ignored = true;
        p = url;
        
		while(*p != '\0' && ignored)
		{
				while(*p != '\0' && (AR_isspace(*p) || *p == '.'))
				{
						++p;
				}

				b = p;

				while(*p != '\0' && *p != '.')
				{
						++p;
				}

				if(p - b > 0)  /*空串，或者类似... ... .. .. .这类由space和.组成的串*/
				{

						if(AR_stristr_s(dns_top, dns_top + AR_strlen(dns_top), b, p) != NULL)
						{
								++p;
								ignored = true;
								continue;
						}

						if(AR_stristr_s(dns_nation, dns_nation + AR_strlen(dns_nation), b, p) != NULL)
						{
								++p;
								ignored = true;
								continue;
						}
						ignored = false;
				}
				
				if(*p != '\0')
				{
						++p;
				}
		}

		return ignored;
}


static void str_test20()
{
		ar_bool_t ignored = is_ignore_url(".com.cn.jp");
		AR_ASSERT(ignored);

		

		ignored = is_ignore_url("     .. .. . . . . . .. ");
		AR_ASSERT(ignored);


		ignored = is_ignore_url("     beijing.com.cn");
		AR_ASSERT(!ignored);


		ignored = is_ignore_url("bj.jxedt.com");
		AR_ASSERT(!ignored);


		ignored = is_ignore_url("bj.	jxedt.		com");
		AR_ASSERT(!ignored);


		ignored = is_ignore_url("bj.	jxedt.		com");
		AR_ASSERT(!ignored);

		ignored = is_ignore_url("weibo");
		AR_ASSERT(!ignored);
}




static void strstr_test0()
{
		{
				const char *p = "w";
				const char *s = "axyz";
				const char *matched = AR_strstr_s(s, s + AR_strlen(s), p, p + AR_strlen(p));
				printf("matched == %s\r\n", matched);
		}

		{
				const wchar_t *p = L"yz";
				const wchar_t *s = L"axyz";
				const wchar_t *matched = AR_wcsstr_s(s, s + AR_wcslen(s), p, p + AR_wcslen(p));
				printf("matched == %ls\r\n", matched);
		}


		{
				const char *p = "yz";
				const char *s = "ZXyZX";
				const char *matched = AR_stristr_s(s, s + AR_strlen(s), p, p + AR_strlen(p));
				printf("matched == %s\r\n", matched);
		}
		

		{
				const wchar_t *p = L"YZ";
				const wchar_t *s = L"axyz";
				const wchar_t *matched = AR_wcsistr_s(s, s + AR_wcslen(s), p, p + AR_wcslen(p));
				printf("matched == %ls\r\n", matched);
		}

		{
				const wchar_t *p = L"中国";
				const wchar_t *s = L"abx中国字xxx";
				const wchar_t *matched = AR_wcsistr_s(s, s + AR_wcslen(s), p, p + AR_wcslen(p));
				printf("matched == %ls\r\n", matched);
		}
}

void full_path_test()
{

		arString_t *str;
		arStatus_t status;
		str = AR_CreateString();

		status = AR_GetRealPath(L"./abc.exe", str);
		AR_ASSERT(status == AR_S_YES);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));


		status = AR_GetRealPath(L"./../", str);
		AR_ASSERT(status == AR_S_YES);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));


		status = AR_GetRealPath(L"..\\test\\a\\b\\c\\....\\", str);
		AR_ASSERT(status == AR_S_YES);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));


		status = AR_GetRealPath(L"~\\test\\a\\b\\c\\....\\", str);
		AR_ASSERT(status == AR_S_YES);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));


		status = AR_GetRealPath(L"\\", str);
		AR_ASSERT(status == AR_S_YES);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));


		status = AR_GetRealPath(L"usr", str);
		AR_ASSERT(status == AR_S_YES);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));


		status = AR_GetRealPath(L"C:\\a\\.", str);
		AR_ASSERT(status == AR_S_YES);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));


		AR_DestroyString(str);
		str = NULL;
}


void esc_wchar_hex_test()
{
		const wchar_t *s = L"一、360安全卫士安装许可协议：";

		wchar_t *escs = AR_str_to_escstr(s);


		wchar_t *src_s = AR_escstr_to_str(escs, NULL);

		AR_printf(L"%ls\r\n", escs);
		AR_printf(L"%ls\r\n", src_s);

		AR_DEL(src_s);
		AR_DEL(escs);

		src_s = NULL;
		src_s = NULL;
}

static void file_test()
{
		arFile_t *f;
		
		arStatus_t		status = AR_open_file(&f, L"C:\\Users\\liupeng\\Desktop\\urllib\\urllib.dat", L"rb");

		status = AR_seek_file(f, 100, AR_FILE_SEEK_END);

		ar_uint_64_t offset;

		status = AR_tell_file(f, &offset);




END_POINT:
		if(f)
		{
				AR_close_file(f);
				f = NULL;
		}
}


static void base64_test()
{
		static const wchar_t *src = L"中国字！！~~中国字~~~！！！中文网中文！";

		ar_int_t n = AR_base64_encode(NULL, 0, (ar_byte_t*)src, AR_wcslen(src) * sizeof(wchar_t));

		AR_printf(L"src == %d, n = %d\r\n", AR_wcslen(src), n);

		char *o = new char[n + 1];

		n = AR_base64_encode((ar_byte_t*)o, n + 1, (ar_byte_t*)src, AR_wcslen(src) * sizeof(wchar_t));
		AR_printf(L"n = %d\r\n", n);
		o[n] = 0;

		AR_printf(L"%hs\r\n", o);

		

		n = AR_base64_decode(NULL, 0, (const ar_byte_t*)o, AR_strlen(o));
		AR_printf(L"decode n = %d\r\n", n);
		
		ar_byte_t *buf = new ar_byte_t[n + 2];
		memset(buf, 0, n + 1);
		
		n = AR_base64_decode(buf, n + 2, (const ar_byte_t*)o, AR_strlen(o));
		AR_printf(L"decode n = %d\r\n", n);
		AR_printf(L"%ls\r\n", buf);
		
		AR_ASSERT(AR_wcscmp((const wchar_t*)buf, src) == 0);
}



class ZBase64
{
public:
		static std::string Encode(const unsigned char* Data,int DataByte)
		{
				using namespace std;
				//编码表
				const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
				//返回值
				string strEncode;
				unsigned char Tmp[4]={0};
				int LineLength=0;
				for(int i=0;i<(int)(DataByte / 3);i++)
				{
						Tmp[1] = *Data++;
						Tmp[2] = *Data++;
						Tmp[3] = *Data++;
						strEncode+= EncodeTable[Tmp[1] >> 2];
						strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
						strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
						strEncode+= EncodeTable[Tmp[3] & 0x3F];
						//if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
				}
				//对剩余数据进行编码
				int Mod=DataByte % 3;
				if(Mod==1)
				{
						Tmp[1] = *Data++;
						strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
						strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4)];
						strEncode+= "==";
				}
				else if(Mod==2)
				{
						Tmp[1] = *Data++;
						Tmp[2] = *Data++;
						strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
						strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
						strEncode+= EncodeTable[((Tmp[2] & 0x0F) << 2)];
						strEncode+= "=";
				}

				return strEncode;
		}

		static std::string Decode(const char* Data,int DataByte,int& OutByte)
		{
				using namespace std;
				//解码表
				const char DecodeTable[] =
				{
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						62, // '+'
						0, 0, 0,
						63, // '/'
						52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
						0, 0, 0, 0, 0, 0, 0,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
						13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
						0, 0, 0, 0, 0, 0,
						26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
						39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
				};
				//返回值
				string strDecode;
				int nValue;
				int i= 0;
				while (i < DataByte)
				{
						if (*Data != '\r' && *Data!='\n')
						{
								nValue = DecodeTable[*Data++] << 18;
								nValue += DecodeTable[*Data++] << 12;
								strDecode+=(nValue & 0x00FF0000) >> 16;
								OutByte++;
								if (*Data != '=')
								{
										nValue += DecodeTable[*Data++] << 6;
										strDecode+=(nValue & 0x0000FF00) >> 8;
										OutByte++;
										if (*Data != '=')
										{
												nValue += DecodeTable[*Data++];
												strDecode+=nValue & 0x000000FF;
												OutByte++;
										}
								}
								i += 4;
						}
						else// 回车换行,跳过
						{
								Data++;
								i++;
						}
				}
				return strDecode;
		}
};


static void base64_encode_test()
{
		
		for(size_t i = 0; i < 200000; ++i)
		{
				size_t src_len = AR_rand32() % 1234;
				if(src_len == 0)
				{
						src_len = 1024;
				}
				ar_byte_t *buf = AR_NEWARR(ar_byte_t , src_len);
				AR_ASSERT(buf != NULL);
				for(size_t k = 0; k < src_len; ++k)
				{
						buf[k] = AR_rand32() % 256;
				}

				
				static ar_byte_t b64_trans_output[10240];
				size_t b64_trans_len = 10240;
				Base64EncodeData(buf, src_len, (char*)b64_trans_output, &b64_trans_len);
				b64_trans_output[b64_trans_len] = '\0';

				
				std::string zbase64_output = ZBase64::Encode(buf, src_len);
				size_t zbase64_output_len = zbase64_output.size();
				

				static ar_byte_t encoded[10240];
				ar_int_t encoded_len = AR_base64_encode(encoded, 10240, buf, src_len);
				AR_ASSERT(encoded_len >= 0);
				encoded[encoded_len] = '\0';

				printf("base64 output len : %u\r\n", encoded_len);

				AR_ASSERT(encoded_len == zbase64_output_len);
				AR_ASSERT(AR_strcmp(zbase64_output.c_str(), (const char*)encoded) == 0);
				AR_DEL(buf);
				buf = NULL;
				//AR_ASSERT(AR_memcmp(encoded, b64_trans_output, encoded_len) == 0);
		}
		
		printf("done cmp base64 encode\r\n");

}

static void base64_decode_test()
{
		for(size_t i = 0; i < 200000; ++i)
		{
			
				size_t src_len = AR_rand32() % 12345;
				if(src_len == 0)
				{
						src_len = 1024;
				}

				if(AR_rand32() % 2 == 0)
				{
						if(src_len % 2 == 0)
						{
								src_len += 1;
						}
				}
						
				ar_byte_t *buf = AR_NEWARR(ar_byte_t , src_len);
				AR_ASSERT(buf != NULL);
				for(size_t k = 0; k < src_len; ++k)
				{
						buf[k] = AR_rand32() % 256;
				}

				std::string zbase64_output = ZBase64::Encode(buf, src_len);
				size_t zbase64_output_len = zbase64_output.size();

				printf("zbase64_output.c_str().size() == %u\r\n", zbase64_output_len);


				static ar_byte_t base64_trans_output[102400];
				size_t base64_trans_len = 102400;
				Base64DecodeData(zbase64_output.c_str(), zbase64_output.size(), (void*)base64_trans_output, &base64_trans_len);


				
				static ar_byte_t decoded[102400];
				size_t ll = AR_base64_decode(decoded, 102400, (const ar_byte_t*)zbase64_output.c_str(), zbase64_output.size());
				
				AR_ASSERT(ll == base64_trans_len);
				AR_ASSERT(ll == src_len);

				AR_ASSERT(AR_memcmp(base64_trans_output, decoded, ll) == 0);
				AR_ASSERT(AR_memcmp(decoded, buf, ll) == 0);
				AR_DEL(buf);
				buf = NULL;
		}
		
		printf("done cmp base64 decode\r\n");


}


static void base64_mutation_test()
{
		
		for(size_t i = 0; i < 200000; ++i)
		{
			
				size_t src_len = AR_rand32() % 10000;
				if(src_len == 0)
				{
						src_len = 1024;
				}

				if(AR_rand32() % 2 == 0)
				{
						if(src_len % 2 == 0)
						{
								src_len += 1;
						}
				}
						
				ar_byte_t *buf = AR_NEWARR(ar_byte_t , src_len);
				AR_ASSERT(buf != NULL);
				for(size_t k = 0; k < src_len; ++k)
				{
						buf[k] = AR_rand32() % 256;
				}

				std::string zbase64_output = ZBase64::Encode(buf, src_len);
				size_t zbase64_output_len = zbase64_output.size();

				

				std::string mutation_base64;

				for(size_t k = 0; k < zbase64_output.size(); ++k)
				{
						mutation_base64 += zbase64_output[k];

						if(AR_rand32() % 4 == 0)
						{
								static const char space[] = 
								{
										' ',
										'\r',
										'\n',
								};

								mutation_base64 += space[AR_rand32() % 3];
						}
				}

				printf("----------------------------------------------\r\n");
				printf("zbase64_output.c_str().size() == %u\r\n", zbase64_output_len);
				printf("mutation_base64 : %s\r\n", mutation_base64.c_str());
				printf("------end----------------------------------------\r\n");
				


				
				static ar_byte_t decoded[102400];
				size_t ll = AR_base64_decode(decoded, 102400, (const ar_byte_t*)mutation_base64.c_str(), mutation_base64.size());

				AR_ASSERT(ll == src_len);
				AR_ASSERT(AR_memcmp(decoded, buf, ll) == 0);
				AR_DEL(buf);
				buf = NULL;
		}
		
		printf("done base64_mutation_test\r\n");


}


static void base64_misc_test()
{

		const char *base64_str = "L5QUckBjvKPDOORIIy8wzfe5R2I9SK/H1zvnV0oLxVO1thJvD2tjoqR3OinRcFzc/p90tdw =";
		

		static ar_byte_t decoded[10240];
		size_t ll = AR_base64_decode(decoded, 10240, (const ar_byte_t*)base64_str, AR_strlen(base64_str));



}


static void base64_test1()
{

		//test encode;
		AR_srand(time(NULL));
		
		//base64_encode_test();
		//base64_decode_test();
		base64_mutation_test();
		//base64_misc_test();
}


static void base64_test2()
{
		const char *src = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";


		ar_int_t n = AR_base64_encode(NULL, 0, (ar_byte_t*)src, AR_strlen(src) * sizeof(char));

		AR_printf(L"src == %d, n = %d\r\n", AR_strlen(src), n);

		char *o = new char[n + 1];

		n = AR_base64_encode((ar_byte_t*)o, n + 1, (ar_byte_t*)src, AR_strlen(src) * sizeof(char));
		AR_printf(L"n = %d\r\n", n);
		o[n] = 0;

		AR_printf(L"%hs\r\n", o);

		

		n = AR_base64_decode(NULL, 0, (const ar_byte_t*)o, AR_strlen(o));
		AR_printf(L"decode n = %d\r\n", n);
		
		ar_byte_t *buf = new ar_byte_t[n + 2];
		memset(buf, 0, n + 1);
		
		n = AR_base64_decode(buf, n + 2, (const ar_byte_t*)o, AR_strlen(o));
		AR_printf(L"decode n = %d\r\n", n);
		AR_printf(L"%ls\r\n", buf);
		
		AR_ASSERT(AR_strcmp((const char*)buf, src) == 0);
}



/*

arStatus_t      AR_path_is_hidden(const wchar_t *path);
arStatus_t      AR_path_is_dev(const wchar_t *path);
arStatus_t      AR_path_is_dir(const wchar_t *path);

arStatus_t      AR_path_is_link(const wchar_t *path);
arStatus_t      AR_path_is_file(const wchar_t *path);


arStatus_t      AR_path_is_executable(const wchar_t *path);
arStatus_t      AR_path_is_writeable(const wchar_t *path);
arStatus_t      AR_path_is_readable(const wchar_t *path);

*/

static void path_test1()
{
		/*
		wchar_t ext[128];
		
		ext[0] = L'\0';
		_wsplitpath(L"C:\\1.txt", NULL, NULL, NULL, ext);

		AR_error(AR_ERR_MESSAGE, L"%ls\r\n", ext);
		*/

		arStatus_t status;

		status = AR_path_is_executable(L"g:\\1.exe");
		AR_ASSERT(status == AR_S_YES);

		status = AR_path_is_executable(L"g:\\1.txt");
		AR_ASSERT(status == AR_S_NO);


		status = AR_path_is_executable(L"g");
		AR_ASSERT(status == AR_S_NO);


		status = AR_path_is_file(L"g:\\1.txt");
		AR_ASSERT(AR_S_YES == status);

		status = AR_path_is_hidden(L"g:\\1.txt");
		AR_ASSERT(AR_S_YES == status);

		status = AR_path_is_readable(L"g:\\1.txt");
		AR_ASSERT(AR_S_YES == status);

		status = AR_path_is_writeable(L"g:\\1.txt");
		//AR_ASSERT(status == AR_S_NO);

		status = AR_path_is_dir(L"g:\\");
		AR_ASSERT(status == AR_S_YES);


		status = AR_path_is_existed(L"g:\\");
		AR_ASSERT(status == AR_S_YES);

		status = AR_path_is_existed(L"g:\\1.txt");
		AR_ASSERT(status == AR_S_YES);

		status = AR_path_is_existed(L"g:\\Data");
		AR_ASSERT(status == AR_S_YES);

		status = AR_path_is_existed(L"g:\\xyz");
		AR_ASSERT(status == AR_S_NO);

		status = AR_path_is_existed(L"g:\\xyz\\");
		AR_ASSERT(status == AR_S_NO);

		status = AR_path_is_existed(L"w:\\");
		AR_ASSERT(status == AR_S_NO);

		//status = AR_path_is_dev(L"C:\\");
		//AR_ASSERT(status == AR_S_NO);

		status = AR_path_set_writeable(L"g:\\1.txt", true);
		AR_ASSERT(status == AR_S_YES);

		status = AR_path_set_writeable(L"g:\\2.txt", false);
		//AR_ASSERT(status != AR_S_YES);

		status = AR_path_rename(L"g:\\3.txt", L"g:\\4.txt");
		//AR_ASSERT(status == AR_S_YES);


		status = AR_path_remove(L"g:\\4.txt");
		status = AR_path_remove(L"g:\\x");

		status = AR_path_create_file(L"g:\\x.txt");
		status = AR_path_create_file(L"g:\\x.txt");


		status = AR_path_create_dir(L"g:\\x\\");
		status = AR_path_create_dir(L"g:\\x");
		
		status = AR_path_create_dir(L"g:\\x.txt");
		

		status = AR_path_copyfile(L"g:\\1.txt", L"g:\\2.txt", true);
		status = AR_path_copyfile(L"g:\\1.txt", L"g:\\2.txt", true);
		status = AR_path_copyfile(L"g:\\1.txt", L"g:\\2.txt", false);
}


static void path_test2()
{
		arStatus_t status;
		status = AR_path_create_path(L"g:\\1\\2\\3\\4\\1.txt");
		status = AR_path_create_path(L"g:\\x\\\\\\\\\\\\\\\\\\\\2\\\\\\\\3\\\\4\\\\\\\\1.txt");
		AR_ASSERT(status == AR_S_YES);

		status = AR_path_remove_path(L"g:\\x");
		AR_ASSERT(status == AR_S_YES);

		status = AR_path_remove_path(L"g:\\1\\\\\\");
		//AR_ASSERT(status == AR_S_YES);

		status = AR_path_remove_path(L"g:\\1.txt");
		//AR_ASSERT(status == AR_S_YES);

		ar_uint_64_t fsize = 0;

		status = AR_path_get_size(L"g:\\2.txt", &fsize);
		AR_printf(L"fsize == %qu\r\n", fsize);
		
		fsize = 1024000;
		status = AR_path_set_size(L"g:\\2.txt", fsize);
		
		status = AR_path_get_size(L"g:\\2.txt", &fsize);
		AR_printf(L"fsize == %qu\r\n", fsize);


		status = AR_path_copydir(L"D:\\Tools\\Dev\\Python", L"G:\\Temp\\Python\\");


}

static void float_test2()
{
		//AR_is_nan_flt(0.0/0.0);

}

void com_test()
{
		
		//strstr_test0();
		//bsearch_test();
		//algo_test1();
		
		//str_test();
		//str_test1();
		//str_test2();
		//str_test3();
		//str_test4();
		//str_test5();
		//str_test6();
		//str_test7();
		//str_test8();
		//str_test9();
		//str_test10();
		//str_test11();
		//str_test12();
		//str_test13();
		//str_test14();
		//str_test15();
		//str_test16();
		//str_test17();
		//str_test18();
		//str_test19();
		//str_test20();

		//com_test3();
		//com_conv();
		//com_conv2();
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
		//buffer_test2();

		//com_timer_test();

		//com_str_test_vscwprintf();

		//com_str_test_cmp();
		//str_tbl_test();

		//kmp_test();
		//kmp_test2();

		//test_align3();

		//byte_filp_test();

		//float_test();
		
		float_test2();

		//com_str_test_vcprintf();
		//com_test_srpintf();

		//escstr_test1();
		//escstr_n_test1();
		//escstr_n_test0();
		//align_test();
		
		//escstr_test2();
		
		//text_test_load_save();

		//rand_test();
		
		//base64_test();
		//base64_test1();
		//base64_test2();

		//float_test_2();
		
		//env_test();

		//path_test();

		//path_iter_test();

		//thd_test();


		//ds_test2();

		//operation_test();

		//cache_test();

		//uri_test();

		//buffer_test3();

		//full_path_test();

		//esc_wchar_hex_test();

		//file_test();

		//path_test1();
		//path_test2();



}



AR_NAMESPACE_END

#endif
