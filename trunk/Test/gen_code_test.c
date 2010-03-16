#include "test.h"
#include "../Arsenal/Lex/lex.h"
#include "../Arsenal/Parser/grammar.h"
#include "../Arsenal/Parser/parser.h"
#include "../Arsenal/Parser/lr_action.h"


#if defined(__LIB)

AR_NAMESPACE_BEGIN



static const wchar_t *__g_lex_name[] = {
NULL
};

#define __NAME_COUNT__ ((size_t)0)

static struct {const wchar_t *name; size_t tokval; size_t lex_prec; const wchar_t *regex; bool_t skip; }__g_term_pattern[] =  {
{NULL, 256, 3, L"\\B[ \\t\\r]*;[^\\r\\n]*(\\r)*\\n", true},
{NULL, 257, 1, L";[^\\r\\n]*", true},
{NULL, 258, 1, L"[ \\t\\r]+", true},
{NULL, 259, 2, L"\\B[ \\t\\r]*\\n", true},
{L"LINE_END", 260, 2, L"(\\r)*\\n", false},
{L"string", 261, 0, L"[^ ;\\r\\n\\[\\]][^;\\r\\n\\[\\]]*", false},
{L"=", 262, 2, L"=", false},
{L"[", 263, 2, L"\\[", false},
{L"]", 264, 2, L"\\]", false},
{L"EOI", 0, 2, L"$", false}
};

#define __TERM_COUNT__ ((size_t)10)

static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {
NULL
};

#define __PREC_COUNT__ ((size_t)0)

static psrNode_t* AR_STDCALL handle_ini_file(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*ini_file	:	section_list */
static psrNode_t* AR_STDCALL handle_section_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*section_list	:	section_list section */
/*section_list	:	section */
static psrNode_t* AR_STDCALL handle_section(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*section	:	section_header line_list */
/*section	:	section_header */
static psrNode_t* AR_STDCALL handle_section_header(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*section_header	:	[ string ] LINE_END */
/*section_header	:	[ error ] LINE_END */
static psrNode_t* AR_STDCALL handle_line_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*line_list	:	line_list line */
/*line_list	:	line */
static psrNode_t* AR_STDCALL handle_line(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*line	:	string LINE_END */
/*line	:	error LINE_END */



static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"ini_file  :  section_list ", NULL, handle_ini_file, 0},
{L"section_list  :  section_list section ", NULL, handle_section_list, 0},
{L"section_list  :  section ", NULL, handle_section_list, 0},
{L"section  :  section_header line_list ", NULL, handle_section, 0},
{L"section  :  section_header ", NULL, handle_section, 0},
{L"section_header  :  [ string ] LINE_END ", NULL, handle_section_header, 0},
{L"section_header  :  [ error ] LINE_END ", NULL, handle_section_header, 0},
{L"line_list  :  line_list line ", NULL, handle_line_list, 0},
{L"line_list  :  line ", NULL, handle_line_list, 0},
{L"line  :  string LINE_END ", NULL, handle_line, 0},
{L"line  :  error LINE_END ", NULL, handle_line, 0}
};

#define __RULE_COUNT__ ((size_t)11)
#define START_RULE L"ini_file"

static lex_t*	__build_lex(const arIOCtx_t *io)								
{																				
		lex_t	*lex;															
		size_t i;																
		lex = LEX_Create(io);													
		for(i = 0; i < __NAME_COUNT__; ++i)										
		{																		
				if(!LEX_Insert(lex, __g_lex_name[i]))							
				{																
						LEX_Destroy(lex);										
						AR_ASSERT(false);										
						return NULL;											
				}																
		}																		
		for(i = 0; i < __TERM_COUNT__; ++i)										
		{																		
				lexAction_t		act;											
				act.is_skip		=		__g_term_pattern[i].skip;				
				act.priority	=		__g_term_pattern[i].lex_prec;			
				act.value		=		__g_term_pattern[i].tokval;				
				if(!LEX_InsertRule(lex, __g_term_pattern[i].regex, &act))		
				{																
						LEX_Destroy(lex);										
						AR_ASSERT(false);										
						return NULL;											
				}																
		}																		
		return lex;																
}

static psrGrammar_t*	__build_grammar(const psrCtx_t	*psr_ctx, const arIOCtx_t *io)											
{																																
		psrGrammar_t	*grammar;																								
		size_t i;																												
		AR_ASSERT(psr_ctx != NULL);																								
		grammar = PSR_CreateGrammar(psr_ctx, io);																				
		for(i = 0; i < __TERM_COUNT__; ++i)																						
		{																														
				if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)continue;										
				if(!PSR_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PSR_ASSOC_NONASSOC,0, NULL))	
				{																												
						PSR_DestroyGrammar(grammar);																			
						grammar = NULL;																							
						AR_ASSERT(false);																						
						return NULL;																							
				}																												
		}																														
		for(i = 0; i < __PREC_COUNT__; ++i)																						
		{																														
				psrTermInfo_t	*info;																							
				info = PSR_GetTermSymbInfoByName(grammar, __g_prec_pattern[i].name);											
				if(info == NULL)																								
				{																												
						if(!PSR_InsertTerm(grammar, __g_prec_pattern[i].name, __g_prec_pattern[i].tokval, __g_prec_pattern[i].assoc, __g_prec_pattern[i].prec_level, NULL))
						{																																					
								PSR_DestroyGrammar(grammar);																												
								grammar = NULL;																																
								AR_ASSERT(false);																															
								return NULL;																																
						}																																					
				}else																																						
				{																																							
						info->assoc = __g_prec_pattern[i].assoc;																											
						info->prec = __g_prec_pattern[i].prec_level;																										
				}																																							
		}																																									
		for(i = 0; i < __RULE_COUNT__; ++i)																													
		{																																									
				if(!PSR_InsertRuleByStr(grammar, __g_rule_pattern[i].rule, __g_rule_pattern[i].prec_token, __g_rule_pattern[i].handler, __g_rule_pattern[i].auto_ret))		
				{																																							
						PSR_DestroyGrammar(grammar);																														
						grammar = NULL;																																		
						AR_ASSERT(false);																																	
						return NULL;																																		
				}																																							
		}																																									
		if(!PSR_SetFirstRule(grammar,START_RULE) || !PSR_CheckIsValidGrammar(grammar))																						
		{																																									
				PSR_DestroyGrammar(grammar);																																
				grammar = NULL;																																				
				AR_ASSERT(false);																																			
				return NULL;																																				
		}																																									
		return grammar;																																						
}




/*ini_file	:	section_list */
static psrNode_t* AR_STDCALL handle_ini_file(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*section_list	:	section_list section */
/*section_list	:	section */
static psrNode_t* AR_STDCALL handle_section_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*section	:	section_header line_list */
/*section	:	section_header */
static psrNode_t* AR_STDCALL handle_section(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*section_header	:	[ string ] LINE_END */
/*section_header	:	[ error ] LINE_END */
static psrNode_t* AR_STDCALL handle_section_header(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*line_list	:	line_list line */
/*line_list	:	line */
static psrNode_t* AR_STDCALL handle_line_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*line	:	string LINE_END */
/*line	:	error LINE_END */
static psrNode_t* AR_STDCALL handle_line(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}











/*********************************************************************************/

static lex_t*	test_BuildLexer(const arIOCtx_t *io)
{
		return __build_lex(io);
}



static void	AR_STDCALL test_parser_free(psrNode_t *node, void *ctx)
{
		AR_printf(L"%ls\r\n", L"ray_parser_free");
}

static void		AR_STDCALL test_parser_error(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)
{
		AR_printf(L"%ls\r\n", L"ray_parser_error");

}


static const psrCtx_t		__g_default_psrctx = 
{
		test_parser_error,
		test_parser_free,
		NULL
};




static psrGrammar_t*	test_BuildGrammar(const arIOCtx_t *io)
{
		return __build_grammar(&__g_default_psrctx, io);
		
}





void gen_code_test()
{
		psrGrammar_t	*gmr;
		lex_t *lex = test_BuildLexer(NULL);
		gmr = test_BuildGrammar(NULL);

		LEX_Destroy(lex);
		PSR_DestroyGrammar(gmr);
		
}



AR_NAMESPACE_END

#endif