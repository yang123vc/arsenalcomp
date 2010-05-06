#include "test.h"


#include "../Arsenal/Ray/ray.h"
#include "../Arsenal/Ray/rayparser.h"






#if defined(__LIB)

AR_NAMESPACE_BEGIN


#define LOAD_TXT_MAX_BUF  (1024*1024 * 5)
static const wchar_t* __load_txt(const wchar_t *path)
{
		FILE *pf;
		byte_t	*buf;
		size_t rn;
		const wchar_t *ret;
		buf = AR_NEWARR(byte_t, LOAD_TXT_MAX_BUF);

		AR_ASSERT(path != NULL);
		
		//pf = fopen("..\\..\\..\\misc\\book1.gmr", "r");

		pf = _wfopen(path, L"r");

		AR_ASSERT(pf != NULL);
		rn = fread((void*)buf, 1, LOAD_TXT_MAX_BUF, pf);
		fclose(pf);
		AR_ASSERT(rn > 3);
		//printf("read size == %d\r\n", rn);
		buf[rn] = 0;

		AR_ASSERT(buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF);

		//printf("%s\r\n", buf + 3);
		

		ret = AR_utf8_convto_wcs((const char*)(buf + 3));

		AR_DEL(buf);
		return ret;
}


#define DEF_PAT_PATH  L"..\\..\\..\\misc\\test.gmr"

#define DEF_SOUR_PATH  L"..\\..\\..\\misc\\input.txt"



#if(0)
static void parse_code(lex_t *lex, const wchar_t *sources)
{
		lexMatch_t match;
		lexToken_t tok;
		size_t	i;
		wchar_t buf[1024];
		
		AR_ASSERT(lex != NULL && sources != NULL);
		
		LEX_GenerateTransTable(lex);

		LEX_InitMatch(&match, sources);

		{
				
				size_t tok_cnt = 0;
				uint_64_t beg, end;

				beg = AR_GetTime_Microseconds();
				//DWORD beg, end;
				//beg = GetTickCount();


				
				while(LEX_Match(lex, &match, &tok))
				{
						/*
						AR_wcsncpy(buf, tok.str, tok.count);
						buf[tok.count] = L'\0';

						AR_printf(L"%ls : type == %d : count == %d : line = %d\r\n", buf, tok.value, tok.count, tok.line);

						getchar();

						*/
						
						tok_cnt++;
						
						if(tok.value == 0)break;
				}

				if(match.is_ok)
				{
						AR_printf(L"lex parse done for %d token\r\n", tok_cnt);
				}else
				{
						size_t n = AR_wcslen(match.next);
						AR_printf(L"lex parse failed : %ls\r\n", AR_wcsndup(match.next, n > 10 ? 10 : n));
				}

				end = AR_GetTime_Microseconds();

				AR_printf(L"elapsed == %I64d\r\n", end - beg);
		}

		LEX_UnInitMatch(&match);


}




#endif


void AR_STDCALL ray_report_func(const rayReportMsg_t *report, void *context)
{
		printf("%ls\r\n", report->message);
}


static rayReport_t		__def_report = 
{
		ray_report_func,
		NULL
};


void align_test(rayParser_t		*parser)
{
		AR_printf(L"Default alignment == %d\r\n", RAY_GetAlignment(parser));
		
		RAY_PushAlignment(parser, 16);
		AR_printf(L"After push alignment == %d\r\n", RAY_GetAlignment(parser));

		RAY_PopAlignment(parser);
		AR_printf(L"After pop alignment == %d\r\n", RAY_GetAlignment(parser));
}

void ray_test()
{
		
		rayParser_t		*parser;
		uint_64_t beg, end;

		
		beg = AR_GetTime_Milliseconds();

		parser = RAY_CreateParser(&__def_report);

		end = AR_GetTime_Milliseconds();
		
		AR_printf(L"build ray parser elapsed == %I64d\r\n", end - beg);
		
		align_test(parser);
		getchar();
		
		const wchar_t *src = __load_txt(DEF_SOUR_PATH);
		AR_ASSERT(src != NULL);
		
		RAY_Parse(parser, src);
		getchar();
		AR_printf(L"%ls\r\n", L"----------------------------------------------\r\n");
		RAY_Parse(parser, src);
		getchar();

		RAY_DestroyParser(parser);

		AR_DEL(src);
}


AR_NAMESPACE_END


#endif


