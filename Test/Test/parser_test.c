
#include "test.h"

#include "../../Arsenal/Parser/grammar.h"
#include "../../Arsenal/Parser/lr_item.h"
#include "../../Arsenal/Parser/lr_dfa.h"
#include "../../Arsenal/Parser/parser.h"
#include "../../Arsenal/Parser/parser_in.h"
#include "../../Arsenal/Parser/lr_action.h"


#include "../../Arsenal/Tools/tools.h"

//#include "gmr_config.h"

#include <locale.h>

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


void print_grammar(const psrGrammar_t *gmr)
{
		arString_t  *str;
		AR_ASSERT(gmr);

		str = AR_CreateString();

		PSR_PrintGrammar(gmr, str);

		AR_printf(L"%ls\r\n", AR_GetStrString(str));

		PSR_ReportLeftRecursion(gmr, str);

		AR_printf(L"%ls\r\n", AR_GetStrString(str));
		

		AR_DestroyString(str);
}

void print_first_follow(const psrGrammar_t *gmr)
{
		psrSymbMap_t	first, follow;
		arString_t		*str;
		AR_ASSERT(gmr != NULL);

		PSR_InitSymbMap(&first);
		PSR_InitSymbMap(&follow);

		str = AR_CreateString();
				
		
		PSR_CalcFirstSet(gmr, &first);
		PSR_CalcFollowSet(gmr, &follow, &first);
		
		AR_AppendString(str, L"First Set:\r\n");
		PSR_PrintSymbolMap(&first, str);
		AR_AppendString(str, L"----------------------------------------\r\n");

		AR_AppendString(str, L"Follow Set:\r\n");
		PSR_PrintSymbolMap(&follow, str);
		AR_AppendString(str, L"----------------------------------------\r\n");


		PSR_UnInitSymbMap(&first);
		PSR_UnInitSymbMap(&follow);

		AR_printf(L"%ls\r\n", AR_GetStrString(str));
		AR_DestroyString(str);
		
}



void print_action_table(const parser_t *psr)
{
		arString_t  *str;
		AR_ASSERT(psr);

		str = AR_CreateString();

		PSR_PrintParserActionTable(psr, str, 20);

		AR_printf(L"%ls\r\n", AR_GetStrString(str));

		
		AR_DestroyString(str);
}

void print_conflict(const parser_t *psr)
{
		arString_t  *str;
		AR_ASSERT(psr);

		str = AR_CreateString();

		PSR_PrintParserConflict(psr, str);

		AR_printf(L"%ls\r\n", AR_GetStrString(str));

		
		AR_DestroyString(str);
}







#define DEF_PAT_PATH  L"..\\..\\..\\misc\\test.gmr"




void parser_test()
{
		const wchar_t	*gmr_txt;
		cfgConfig_t		*cfg;
		

		
		gmr_txt = __load_txt(DEF_PAT_PATH);


		AR_printf(L"%ls\r\n", gmr_txt);

		getchar();

		
		cfg = CFG_CollectGrammarConfig(gmr_txt, NULL);
		

		
		
		
		

		if(cfg)
		{
#if(1)
				size_t i;
				lex_t *tmp_lex;
				tmp_lex = LEX_Create(NULL);
				AR_printf(L"----------------------\r\n");
				for(i = 0; i < cfg->name_cnt; ++i)
				{
						AR_printf(L"%ls : %ls : %d\r\n", cfg->name[i].name, cfg->name[i].regex, cfg->name[i].line);

						if(!LEX_InsertName(tmp_lex, cfg->name[i].name, cfg->name[i].regex))
						{
								AR_abort();
						}
						
				}
				AR_printf(L"----------------------\r\n");
				for(i = 0; i < cfg->tok_cnt; ++i)
				{
						lexAction_t act;
						act.is_skip = cfg->tok[i].is_skip;
						act.priority = cfg->tok[i].lex_prec;
						act.type = i + 256;

						AR_printf(L"%ls : %ls : %d : %d\r\n", cfg->tok[i].name, cfg->tok[i].regex, cfg->tok[i].lex_prec, cfg->tok[i].tokval);

						if(!LEX_InsertRule(tmp_lex, cfg->tok[i].regex, &act))
						{
								AR_abort();
						}
				}
				AR_printf(L"----------------------\r\n");

				for(i = 0; i < cfg->rule_cnt; ++i)
				{
						AR_printf(L"%ls : %ls : %ls : %ls : %d\r\n"
								, cfg->rule[i].lhs
								, cfg->rule[i].rhs
								, cfg->rule[i].prec_tok ? cfg->rule[i].prec_tok : L" "
								, cfg->rule[i].handler_name ? cfg->rule[i].handler_name : L" "
								,cfg->rule[i].line
								);
				}

				AR_printf(L"----------------------\r\n");
		
		//		arString_t *str = NULL;
		//		CFG_ConfigToCode(cfg, str);
#endif

		}else
		{
				AR_abort();
		}


		
		if(cfg)CFG_DestroyGrammarConfig(cfg);
}






AR_NAMESPACE_END

#endif

