
#include "test.h"

#include "../../Arsenal/Parser/grammar.h"
#include "../../Arsenal/Parser/lr_item.h"
#include "../../Arsenal/Parser/lr_dfa.h"
#include "../../Arsenal/Parser/parser.h"
#include "../../Arsenal/Parser/parser_in.h"
#include "../../Arsenal/Parser/lr_action.h"

#include <locale.h>

#if defined(__LIB)

AR_NAMESPACE_BEGIN



/*************************GrammarTest************************/
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




#define DEF_PAT_PATH  L"..\\..\\..\\misc\\book1.gmr"

#define DEF_SOUR_PATH  L"..\\..\\..\\misc\\book1.txt"


static printNode_t* __build_syntree(lex_t *lex, parser_t *parser, const wchar_t *sour)
{
		lexMatch_t		match;
		psrToken_t		tok;
		printNode_t		*res = NULL;
		AR_ASSERT(lex != NULL && parser != NULL && sour != NULL);

		LEX_InitMatch(&match, sour);
		
		while(LEX_Match(lex, &match, &tok))
		{
				if(!PSR_AddToken(parser, &tok))
				{
						break;
				}
				if(tok.type == 0)break;
		}

		if(PSR_IsAccepted(parser))
		{
				res = (printNode_t*)PSR_GetResult(parser);
		}else
		{

				AR_printf(L"__build_syntree failed\r\n");
		}
		LEX_UnInitMatch(&match);
		PSR_Clear(parser);
		return res;
}


void parser_test()
{
		const wchar_t *pat;
		const wchar_t *sour;
		const wchar_t *p;
		
		lex_t			*lex = NULL;
		parser_t		*parser= NULL;
		psrGrammar_t	*gmr = NULL;
		printNode_t		*syntree = NULL;

		setlocale(CP_ACP, "");

		pat		= __load_txt(DEF_PAT_PATH);
		sour	= __load_txt(DEF_SOUR_PATH);

		AR_ASSERT(pat != NULL && sour != NULL);

		/*printf("%ls\r\n", pat);*/

		p = pat;
		
		lex = LEX_Create();
		gmr = PSR_CreateGrammar();
		
		p = LEX_Config(lex, p);

		AR_ASSERT(p != NULL);

		if(PSR_ConfigGrammar(gmr, p))
		{
				print_grammar(gmr);
				print_first_follow(gmr);
				system("pause");
				

				{
						ULONGLONG beg, end;
						
						beg = GetTickCount();
						parser = PSR_CreateParser(gmr, PSR_LR1, PSR_GetPrintNodeContext());
						
						end = GetTickCount();

						printf("build parser elapsed time == %I64d\r\n", end - beg);

						system("pause");

				}
				
				print_action_table(parser);
				print_conflict(parser);
				system("pause");

		}else
		{
				AR_ASSERT(false);
		}

		{
				ULONGLONG beg, end;
				
				beg = GetTickCount();

				syntree = __build_syntree(lex, parser, sour);

				end = GetTickCount();
				
				printf("build syntree elapsed time == %I64d\r\n", end - beg);
				system("pause");
		}

		if(syntree)PSR_DestroyPrintNode(syntree);


		if(parser)PSR_DestroyParser(parser);
		PSR_DestroyGrammar(gmr);
		LEX_Destroy(lex);

		if(pat)AR_DEL((void*)pat);
		if(sour)AR_DEL((void*)sour);

		pat = NULL;
		sour = NULL;
}


#if(0)





static const wchar_t *gmr_pattern1 = 
L"  %token{\r\n"
//L"'(' : 300, ')' : 301, '+' : 302, '-' : 303, '*' : 304, '/' = 305;"
L"'(' : 300, ')' : 301, '+' : 302, '*' : 304;"
L"id : 401;"
L"%}\r\n"

L"%prec{\r\n"
L"%left '+' ;"
L"%left '*' ;"
//L"%right UMINUS;"
L"%}\r\n"

L"%rules{\r\n"
L"%id = 1000"
//L"E -> E '+' E | E '-' E | E '*' E | E '/' E | '('E')' | '-' E %prec UMINUS| number |.;"
L"E -> E '+' E |  E '*' E |  '('E')' |  id ;"
L"%}\r\n"
;



static const wchar_t *gmr_pattern4 = 
L"  %token{\r\n"
L" n : 300, '+' : 301;"
L"%}\r\n"

L"%rules{\r\n"
L"%id = 1000"
L"E -> E '+' n | n;"

L"%}\r\n"
;


static const wchar_t *gmr_pattern6 = 
L"  %token{\r\n"
L"'(' : 300, ')' : 301, '+' : 302, '-' : 303, '*' : 304, '/' = 305;"
L"';' : 308;"
L"number : 400;"
L"id : 401;"
L"%}\r\n"

L"%prec{\r\n"
L"%left	';';"
L"%left '+', '-';"
L"%left '*', '/';"
L"%right UMINUS;"
L"%}\r\n"

L"%rules{\r\n"
L"%id = 1000;"
L"E -> E '+' E | E '-' E | E '*' E | E '/' E | '('E')' | '-' E %prec UMINUS| number | error '+';"
L"%}\r\n"
;

static const wchar_t *gmr_pattern7 = 
L"  %token{\r\n"
L"';' : 300, ',' : 301;"
L"type : 302;"
L"var : 400;"
L"%}\r\n"

L"%prec{\r\n"

L"%left '+' ;"
L"%}\r\n"

L"%rules{\r\n"
L"%id = 1000"
L"Decl -> type VarLst ';'| Z ;"
L"VarLst -> var | VarLst ','var;"
L"%}\r\n"
;



static const wchar_t *gmr_pattern = 
L"  %token{\r\n"
L"'(' : 300, ')' : 301, '+' : 302, '-' : 303, '*' : 304, '/' = 305;"
L"';' : 308;"
L"number : 400;"
L"id : 401;"
L"%}\r\n"

L"%prec{\r\n"
//L"%left '+', '-';"
//L"%left '*', '/';"
L"%right UMINUS;"
L"%}\r\n"

L"%rules{\r\n"
L"E -> E '+' E | E '-' E | E '*' E | E '/' E | '('E')' | '-' E %prec UMINUS| number ; "

L"%}\r\n"
;





static const wchar_t *gmr_pattern5 = 
L"  %token{\r\n"
L" \"(\" : 300, ')' : 301, \"++\" : 303;"
L"%}\r\n"


L"%rules{\r\n"
L"S -> \"(\" S ')' S | . |error;"

L"%}\r\n"
;



static const wchar_t *gmr_pattern3 = 
L"  %token{\r\n"
L" n : 300, id : 301, '=' : 400, '*' : 401;"
L"%}\r\n"

L"%prec{\r\n"
//L"%right	'=';"
L"%}\r\n"

L"%rules{\r\n"
L"S -> L '=' R | R;"
L"L -> '*'R | id;"
L"R -> L;"

L"%}\r\n"
;



static const wchar_t *gmr_pattern2 = 
L"  %token{\r\n"
L"'(' : 300, ')' : 301, a : 400;"
L"%}\r\n"

L"%rules{\r\n"
L"A -> '(' A ')' | a ;"
L"%}\r\n"
;




static const wchar_t *gmr_pattern10 = 
L"  %token{\r\n"
L"id : 300, n : 400, ':=' : 500;"
L"%}\r\n"

L"%rules{\r\n"
L"S -> id | V ':=' E;"
L"V-> id;"
L"E -> V | n;"
L"%}"
;

static void print_str(arString_t *str)
{
		AR_printf(L"%ls\n", AR_GetStrString(str));
		AR_ClearString(str);
}


void gmr_test()
{
		psrGrammar_t grammar;
		arString_t	*str;


		PSR_InitGrammar(&grammar);
		
		str = AR_CreateString();
		
		if(!PSR_ConfigGrammar(&grammar, gmr_pattern))
		{
				AR_ASSERT(false);
				return;
		}

		PSR_PrintGrammar(&grammar, str);
		print_str(str);

		
		
		getchar();
		
		{
				psrSymbMap_t	first, follow;

				PSR_InitSymbMap(&first);
				PSR_InitSymbMap(&follow);

				PSR_CalcFirstSet(&grammar, &first);
				PSR_PrintSymbolMap(&first, str);
				AR_AppendFormatString(str, L"-------------------------------------\r\n");
				PSR_CalcFollowSet(&grammar, &follow, &first);
				
				PSR_PrintSymbolMap(&follow, str);
				AR_AppendFormatString(str, L"-------------------------------------\r\n");
				PSR_UnInitSymbMap(&first);
				PSR_UnInitSymbMap(&follow);

				print_str(str);
		}
		
		getchar();
#if(0)

		{
				DWORD beg, end;
				size_t k;
				psrDFA_t		*dfa;
				psrDFASet_t		set;
				PSR_InitDFASet(&set);
				
				beg = GetTickCount();
				dfa = PSR_Build_LR0_DFA(&grammar);
				end = GetTickCount();

				printf("elipsed == %d\r\n", end - beg);

				PSR_CollectDFA(&set, dfa);

				for(k = 0; k < set.count; ++k)
				{
						AR_AppendFormatString(str, L"I[%d] : \r\n",k);
						PSR_PrintLRItemTable(&set.set[k]->tbl, &grammar, str);
						AR_AppendFormatString(str, L"-----------------------------\r\n");

				}
				
				print_str(str);
				PSR_UnInitDFASet(&set);
		}
#endif
		{
				DWORD beg, end;
				size_t k;
				psrDFA_t		*dfa;
				psrDFASet_t		set;
				PSR_InitDFASet(&set);
				
				beg = GetTickCount();
				dfa = PSR_Build_LALR_DFA(&grammar);
				end = GetTickCount();

				printf("elipsed == %d\r\n", end - beg);

				PSR_CollectDFA(&set, dfa);

				for(k = 0; k < set.count; ++k)
				{
						AR_AppendFormatString(str, L"I[%d] : \r\n",k);
						PSR_PrintLRItemTable(&set.set[k]->tbl, &grammar, str);
						AR_AppendFormatString(str, L"-----------------------------\r\n");

				}
				getchar();
				print_str(str);
				PSR_DestroyDFA_ALL(dfa);
				PSR_UnInitDFASet(&set);
		}
		
		PSR_UnInitGrammar(&grammar);
}



__int64 beg, end;
void action_tbl_test()
{
		arString_t		*str;
		psrActionTable_t *tbl;
		psrGrammar_t	grammar, grammar_a;

		PSR_InitGrammar(&grammar);
		PSR_InitGrammar(&grammar_a);

		if(!PSR_ConfigGrammar(&grammar_a, gmr_pattern))
		{
				AR_ASSERT(false);
				abort();
		}
		PSR_CopyGrammar(&grammar, &grammar_a);

		str = AR_CreateString();
		PSR_PrintGrammar(&grammar,str);
		print_str(str);

		//getchar();

		beg = GetTime();

		//tbl = PSR_CreateActionTable_SLR(&grammar);
		//tbl = PSR_CreateActionTable_LR1(&grammar);
		tbl = PSR_CreateActionTable_LALR(&grammar);
		
		end = GetTime();
		
		printf("elipsed == %d\r\n", end - beg);
		getchar();
	
		PSR_PrintActionTable(tbl, &grammar,18,str);
		print_str(str);
		getchar();

		PSR_ReportConflict(tbl, &grammar,str);
		print_str(str);
		getchar();


		PSR_DestroyActionTable(tbl);

		PSR_UnInitGrammar(&grammar_a);
		PSR_UnInitGrammar(&grammar);
		getchar();
}





psrNode_t* AR_STDCALL new_leaf(const psrToken_t *tok, void *ctx)
{
		AR_printf(tok->str);
		printf("\n");
		return NULL;
}

psrNode_t* AR_STDCALL new_node(size_t rule_id, const wchar_t *rule_name, psrNode_t **nodes, size_t count, void *ctx) /*通过子节点和产生式ID创建父节点*/
{
		size_t i;
		AR_printf(L"name == %ls : id == %d : count == %d\n", rule_name, rule_id, count);
		for(i = 0; i < count; ++i)
		{
				AR_ASSERT(nodes[i] == NULL);
		}
		return NULL;
}


void AR_STDCALL handle_error(size_t rule_id, const wchar_t *rule_name, psrNode_t **nodes, size_t count, void *ctx) /*通过子节点和产生式ID创建父节点*/
{
		AR_printf(L"error rule : name == %ls : id == %d : count == %d\n", rule_name, rule_id, count);
}


void	   AR_STDCALL del_node(psrNode_t *node, void *ctx)
{
		
}

void parser_test()
{
		parser_t *parser;
		psrCtx_t	ctx;
		arString_t		*str;
		psrGrammar_t grammar;
		psrNode_t *node;
		ctx.ctx = NULL;
		ctx.free_f = del_node;
		ctx.leaf_f = new_leaf;
		ctx.node_f = new_node;


		str = AR_CreateString();

		PSR_InitGrammar(&grammar);
		
		beg = GetTickCount();
		if(!PSR_ConfigGrammar(&grammar, gmr_pattern5))
		{
				AR_ASSERT(false);
				abort();
		}
		
		parser = PSR_CreateParser(&grammar, PSR_SLR, &ctx);

		end = GetTickCount();
		printf("elapsed == %d\n", end - beg);
		getchar();
		PSR_UnInitGrammar(&grammar);
		
		PSR_PrintGrammar(parser->grammar,str);
		print_str(str);
		
		PSR_PrintActionTable(parser->tbl, parser->grammar,18,str);
		print_str(str);
		PSR_ReportConflict(parser->tbl, parser->grammar,str);
		print_str(str);

		{
				psrToken_t tok;
				const wchar_t *input = L"((()))()";
				wchar_t buf[1024];
				psrError_t err;
				size_t i;
				for(i = 0; i < AR_wcslen(input); ++i)
				{
						wchar_t c = input[i];
						buf[0] = c;
						buf[1] = L'\0';
						tok.type = (c == L'(' ? 300 : 301);
						tok.str = buf;
						tok.line = 0;
						tok.col = i;
						
						err = PSR_AddToken(parser, &tok);

						printf("err = %d\n", err);
						getchar();
						if(err != PSR_ERR_OK && err != PSR_ERR_ACCEPT)
						{
								printf("error\r\n");
								return;
						}
				}
				tok.type = PSR_EOI_TOKVAL;
				buf[0] = L'\0';
				tok.str = buf;
				err = PSR_AddToken(parser, &tok);
				
				printf("err = %d\n", err);

				node = PSR_GetResult(parser);
				getchar();
		}

		getchar();
		PSR_DestroyParser(parser);
}


#endif


AR_NAMESPACE_END


#endif
