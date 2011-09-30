
#include <iostream>
#include <string>
#include "Arsenal.h"
#include "TestBuildParser.h"


using namespace ARSpace;



void AR_Test1()
{
		//wprintf(L"Invalid Token \"%ls\" in (%lld : %lld)\r\n\r\n", L"d", (size_t)1,(size_t)2);
		/*
		wchar_t buf[1024];

		wchar_t *d = AR_wcsndup(L"d", 1);
		
		AR_swprintf(buf, 1024, L"Invalid Token %ls in (%Id : %Id)\r\n\r\n", d, (size_t)1,  (size_t)2);
		
		AR_DEL(d);
		
		wprintf(L"%ls", buf);
		 */
		
		
		{
		arString_t *s = AR_CreateString();

		wchar_t *buf = AR_wcsndup(L"d", 1);
		
		AR_AppendFormatString(s
							  , L"Invalid Token %ls in (%Id : %Id)\r\n\r\n"
							  , buf
							  , (size_t)0
							  , (size_t)0
							  );
		
		AR_printf(L"%ls\r\n", AR_GetStringCString(s));
		
		AR_DEL(buf);
		buf = NULL;
		AR_DestroyString(s);
		s = NULL;
		}
		
		getchar();
					 
}


#define INPUT_FILE		L"/Users/solidussnakeex/Desktop/Tengu2.gmr"
#define OUTPUT_FILE		L"/Users/solidussnakeex/Desktop/output.txt"

#define OUTPUT_TYPE		AR_TXT_BOM_UTF_8

void text_test_save()
{


		arTxtBom_t		bom;
		arString_t *str = AR_CreateString();
		
		
		if(!AR_LoadBomTextFile(INPUT_FILE, &bom, str))
		{
				AR_ASSERT(false);
		}
		
		
		if(!AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStringCString(str)))
		{
				AR_ASSERT(false);
		}
		
		
		if(!AR_LoadBomTextFile(OUTPUT_FILE, &bom, str))
		{
				AR_ASSERT(false);
		}
		
		AR_ASSERT(bom == OUTPUT_TYPE);
		
		wprintf(L"%ls\r\n", AR_GetStringCString(str));
				
		AR_DestroyString(str);
		
		

}



void AR_Test2()
{

		text_test_save();
				
		
		
}




void AR_Test3()
{
		uint_64_t beg,end;
		for(size_t i = 0; i < 10; ++i)
		{
				
				beg = AR_GetTime_Milliseconds();
				AR_Sleep(999);
				end = AR_GetTime_Milliseconds();
				
				AR_printf(L"elapsed == %qu\r\n", end - beg);
				
		}
}


void AR_Test4()
{
		arSpinLock_t lock;
		
		AR_InitSpinLock(&lock);
		
		AR_LockSpinLock(&lock);
		AR_LockSpinLock(&lock);
		
		AR_UnLockSpinLock(&lock);
		
		AR_UnInitSpinLock(&lock);
}






#if(0)
 
 -(BOOL)generateFromGrammar : (NSString *)grammarInput;
 {
 [self clear];
 
 if([grammarInput length] == 0)
 {
 return NO;
 }
 
  
 }
 
 
 
 -(BOOL)generate : (const cfgConfig_t *)cfg
 {
 
 BOOL has_error = NO;
 
 
 lex_t	*lexer = Lex_Create();
 psrGrammar_t	*gmr = Parser_CreateGrammar(&__def_handler);
 
 for(size_t i = 0; i < cfg->name_cnt; ++i)
 {
 const cfgName_t		*name = &cfg->name[i];
 
 if(!Lex_InsertName(lexer, name->name, name->regex))
 {
 NSString *msg = [NSString stringWithFormat : @"Name Error : \"%@ : %@\"",
 [ARUtility convertUTF32ToNSString : name->name],
 [ARUtility convertUTF32ToNSString : name->regex]
 ];
 
 [ self.delegate onBuildParserMsg : PRINT_NODE_ERROR_T
 Msg : msg
 Line : name->line
 ];
 has_error = YES;
 }
 }
 
 
 for(size_t i = 0; i < cfg->tok_cnt; ++i)
 {
 const cfgToken_t		*tok = &cfg->tok[i];
 
 lexAction_t	action;
 action.is_skip = tok->is_skip;
 action.priority = tok->lex_prec;
 action.value = tok->tokval;
 if(!Lex_InsertRule(lexer, tok->regex, &action))
 {
 NSString *msg = [NSString stringWithFormat : @"Token Error : \"%@ : %@\"",
 [ARUtility convertUTF32ToNSString : tok->name],
 [ARUtility convertUTF32ToNSString : tok->regex]
 ];
 
 [ delegate onBuildParserMsg : PRINT_NODE_ERROR_T
 Msg : msg
 Line : tok->line
 ];
 has_error = YES;
 }
 
 if(tok->is_skip || tok->tokval == 0)
 {
 continue;
 }
 
 if(!Parser_InsertTerm(gmr, tok->name, tok->tokval, PARSER_ASSOC_NONASSOC, 0, build_leaf))
 {
 
 NSString *msg = [NSString stringWithFormat : @"Term Error : \"%@ : %@\"",
 [ARUtility convertUTF32ToNSString : tok->name],
 [ARUtility convertUTF32ToNSString : tok->regex]
 ];
 
 [ delegate onBuildParserMsg : PRINT_NODE_ERROR_T
 Msg : msg
 Line : tok->line
 ];
 has_error = YES;
 }
 }
 
 
 for(size_t i = 0; i < cfg->prec_cnt; ++i)
 {
 const cfgPrec_t		*prec = &cfg->prec[i];
 
 for(size_t k = 0; k < prec->count; ++k)
 {
 psrTermInfo_t *info = Parser_GetTermSymbInfoByName(gmr,prec->prec_tok_set[k]);
 
 if(info == NULL)
 {
 if(!Parser_InsertTerm(gmr, prec->prec_tok_set[k], prec->prec_tok_val[k], prec->assoc, prec->prec_level, NULL))
 {
 NSString *msg = [NSString stringWithFormat : @"Prec Error : \"%@\"",
 [ARUtility convertUTF32ToNSString : prec->prec_tok_set[k]]
 ];
 
 [ delegate onBuildParserMsg : PRINT_NODE_ERROR_T
 Msg : msg
 Line : prec->line
 ];
 
 has_error = YES;
 }
 }else
 {
 info->assoc = prec->assoc;
 info->prec = prec->prec_level;
 }
 }
 
 }
 
 
 
 for(size_t i = 0; i < cfg->rule_cnt; ++i)
 {
 const cfgRule_t		*rule = &cfg->rule[i];
 
 NSString *str = [NSString stringWithFormat : @"%@ : %@",
 [ARUtility convertUTF32ToNSString : rule->lhs],
 [ARUtility convertUTF32ToNSString : rule->rhs]
 ];
 
 if(!Parser_InsertRuleByStr(gmr, [ARUtility convertNSStringToUTF32 : str], rule->prec_tok,  build_rule, 0))
 {
 NSString *msg = [NSString stringWithFormat : @"Rule Error : \"%@\"", str ];
 [ delegate onBuildParserMsg : PRINT_NODE_ERROR_T
 Msg : msg
 Line : rule->line
 ];
 
 has_error = YES;
 }
 }
 
 
 
 
 if(cfg->start.start_rule != NULL)
 {
 BOOL has_start_rule = NO;
 
 for(size_t i = 0; i < cfg->rule_cnt; ++i)
 {
 if(AR_wcscmp(cfg->start.start_rule, cfg->rule[i].lhs) == 0)
 {
 has_start_rule = YES;
 break;
 }
 }
 
 
 if(!has_start_rule || !Parser_SetStartRule(gmr, cfg->start.start_rule))
 {
 
 NSString *msg = [NSString stringWithFormat : @"Start Rule Error : \"%@\"", 
 [ARUtility convertUTF32ToNSString : cfg->start.start_rule]
 
 ];
 [ delegate onBuildParserMsg : PRINT_NODE_ERROR_T
 Msg : msg
 Line : cfg->start.line
 ];
 has_error = YES;
 }
 }
 
 
 /*********************************************检查并报告语法错误*******************************************************/
{
		
		arIOCtx_t	io_context = 
		{
				__report_io_error_func,
				__report_io_print_func,
				(void*)delegate
				
		};
		
		if(!Parser_CheckIsValidGrammar(gmr, &io_context))
		{
				has_error = YES;
		}
}
/******************************************************************************************************/


if(has_error)
{
		Lex_Destroy(lexer);
		lexer = NULL;
		Parser_DestroyGrammar(gmr);
		gmr = NULL;
		return NO;
}else
{
		
		uint_64_t beg, end;
		
		beg = AR_GetTime_Milliseconds();
		
		lex = lexer;
		Lex_GenerateTransTable(lex);
		grammar	= gmr;
		
		parser = Parser_CreateParser(grammar, parserMode);
		
		
		end = AR_GetTime_Milliseconds();
		
		
		NSString *str = [NSString stringWithFormat : @"Build Parser Tick count %qu",
						 end - beg
						 ];
		
		[ delegate onBuildParserMsg : PRINT_NODE_MSG_T
								Msg : str
							   Line : -1
		 ];
		
		
		size_t conflict = Parser_CountParserConflict(parser);
		
		if(conflict > 0)
		{
				str = [NSString stringWithFormat : @"The grammar has %u conflicts",
					   (uint_32_t)conflict
					   ];
				
				[ delegate onBuildParserMsg : PRINT_NODE_MSG_T
										Msg : str
									   Line : -1
				 ];
		}
		
		
		return YES;
}
}

#endif










void AR_STDCALL tiny_error(int_t level, const wchar_t* msg, void *ctx)
{
		
        wprintf(L"%ls\r\n", msg);
}

void AR_STDCALL tiny_printf(const wchar_t *msg, void *ctx)
{
		
        wprintf(L"%ls\r\n", msg);
}







int  main()
{
      
		setlocale(LC_ALL, "");
		
		
				
		//printf("%s\r\n", AR_FUNC_NAME);
        arInit_t ai = {{tiny_error, tiny_printf, NULL}};
		
        //printf("%s\r\n", setlocale(LC_ALL, NULL));
		
        Arsenal_Init(&ai);
		
		//AR_Test1();
        //AR_Test2();
		//AR_Test3();
		//AR_Test4();
		AR_GenerateParsertest();
		
		
		Arsenal_UnInit();
		
		
		printf("done\r\n");
		
		
		//getchar();
		
		return 0;
		
}









