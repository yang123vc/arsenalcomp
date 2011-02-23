
#include <iostream>
#include <string>
#include "Arsenal.h"



using namespace ARSpace;

void AR_STDCALL report_message(const cfgReportInfo_t *report, void *context)
{
		
		AR_ASSERT(report != NULL && context == NULL);

		switch(report->type)
		{
				case CFG_REPORT_MESSAGE_T:
						AR_printf(L"Message == %ls\r\n", report->std_msg.message);
						break;
				case CFG_REPORT_ERROR_T:
						AR_printf(L"%ls == %d\r\n", report->error.err_msg,report->error.err_level);
						break;
				case CFG_REPORT_ERROR_LEX_T:
						AR_printf(L"Lex Error == %ls\r\n", report->lex_error.msg);
						break;
				case CFG_REPORT_ERROR_SYNTAX_T:
						AR_printf(L"Syntax Error == %ls\r\n", report->syntax_error.msg);
						break;
				case CFG_REPORT_WARNING_SYNTAX_T:
						AR_printf(L"Warning == %ls\r\n", report->warning.msg);						
						break;
				default:
						AR_ASSERT(false);
						
		}
}




bool	generate_parser(const std::wstring &grammar, std::wstring &out)
{

		bool_t			is_ok = true;
		cfgConfig_t		*cfg = NULL;
		arString_t		*ar_str = NULL;
		cfgReport_t		report = {report_message, NULL };

		
		cfg = CFG_CollectGrammarConfig(grammar.c_str(), &report);
		
		if(cfg == NULL) { is_ok = false; goto END_POINT; }
				
		ar_str = AR_CreateString();
		
		if(!CFG_ConfigToCode(cfg, ar_str)) { is_ok = false; goto  END_POINT;}
		
		
		out = AR_GetStrString(ar_str);
		
	
		


END_POINT:
		   if(cfg)
		   {
				   CFG_DestroyGrammarConfig(cfg);
				   cfg = NULL;
		   }
		   
		   if(ar_str)
		   {
				   AR_DestroyString(ar_str);
				   ar_str = NULL;
		   }
		   
		   return is_ok;
		   
}


#define LOAD_TXT_MAX_BUF  (1024*1024 * 5)

static const wchar_t* __load_txt(const char *path)
{
		FILE *pf;
		byte_t	*buf;
		size_t rn;
		const wchar_t *ret;
		buf = AR_NEWARR(byte_t, LOAD_TXT_MAX_BUF);
		
		AR_ASSERT(path != NULL);
		
		pf = fopen(path, "rb");
				
		if(pf == NULL)
		{
				AR_DEL(buf);
				return NULL;
		}
		
		rn = fread((void*)buf, 1, LOAD_TXT_MAX_BUF, pf);
		fclose(pf);
		
		if(rn <= 3)
		{
				AR_DEL(buf);
				return NULL;
		}
		
		buf[rn] = 0;
		
		if(buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF)
		{
				ret = AR_utf8_convto_wcs((const char*)(buf + 3));
		}else
		{
				ret = AR_utf8_convto_wcs((const char*)(buf));
		}
		
		AR_DEL(buf);
		return ret;
}

bool __save_text(const char *path, const wchar_t *txt)
{
		AR_ASSERT(path != NULL && txt != NULL);
		FILE *pf;
		char *buf;
		size_t wn;
	
		pf = fopen(path, "wb");
		
		if(pf == NULL)
		{
				return false;
		}
		
		static const char head[] = {0xEF, 0xBB, 0xBF };

		if(fwrite(head, 1, 3, pf) != 3)
		{
				fclose(pf);
				return false;
		}
		
		buf = AR_wcs_convto_utf8(txt);
		
		
		
		wn = fwrite((void*)buf, 1, strlen(buf), pf);

		fclose(pf);
		
		if(wn != strlen(buf))
		{
				AR_DEL(buf);		
				return false;
		}
		
		return true;
		
}

void test_generate_parser(const char *path)
{
		const wchar_t *tmp = __load_txt(path);
		if(tmp == NULL)
		{
				AR_printf(L"load %hs failed\r\n", path);
				return;
		}
		
		std::wstring in = tmp, out;
		AR_DEL(tmp);
		tmp = NULL;
		
		if(generate_parser(in, out))
		{
				AR_printf(L"generate %hs success\r\n", path);
				
				AR_printf(L"%ls\r\n", out.c_str());
				
				
				if(!__save_text("/Users/solidussnakeex/Desktop/tengu.c", out.c_str()))
				{
						AR_printf(L"%ls\r\n", L"save text failed\r\n");
				}else
				{
						AR_printf(L"%ls\r\n", L"save text successed\r\n");
				}
				
				
		}else
		{
				AR_printf(L"generate %hs failed\r\n", path);
		}
		
}

void AR_Test()
{
		char buf[1024];
		getcwd(buf, 1024);
		printf("%hs\r\n", buf);
		
		while(true)
		{
				scanf("%s", buf);
				if(AR_stricmp(buf, "quit") == 0)break;
				

				test_generate_parser(buf);
				
		}
		
}

void text_test_save()
{
#define OUTPUT_FILE		L"/Users/solidussnakeex/Desktop/test.txt"
#define OUTPUT_TYPE		AR_TXT_BOM_ASCII
		
		arString_t *str = AR_CreateString();
		
		if(!AR_LoadBomTextFile(L"/Users/solidussnakeex/Desktop/Arsenal/misc/txt_enc_test/utf16_be_read_test.txt",NULL, str))
		{
				AR_ASSERT(false);
		}
		
		
		if(!AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStrString(str)))
		{
				AR_ASSERT(false);
		}
		
		
		arTxtBom_t bom;
		if(!AR_LoadBomTextFile(OUTPUT_FILE, &bom, str))
		{
				AR_ASSERT(false);
		}
		
		AR_ASSERT(bom == OUTPUT_TYPE);
		
		
		AR_DestroyString(str);
}



void AR_Test2()
{

		text_test_save();
				
		
		
}






void AR_STDCALL tiny_error(int_t level, const wchar_t* msg, void *ctx)
{
		
        printf("%ls\r\n", msg);
}

void AR_STDCALL tiny_printf(const wchar_t *msg, void *ctx)
{
		
        printf("%ls\r\n", msg);
}




int  main()
{
      
		setlocale(LC_ALL, "");
		
		//printf("%s\r\n", AR_FUNC_NAME);
        arInit_t ai = {{tiny_error, tiny_printf, NULL}};
		
        //printf("%s\r\n", setlocale(LC_ALL, NULL));
		
        Arsenal_Init(&ai);
		
        AR_Test2();
		
		
		Arsenal_UnInit();
		
		
		printf("done\r\n");
		
		
		getchar();
		
		return 0;
		
}









