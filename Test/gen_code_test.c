#include "test.h"


#include "../Arsenal/Lex/lex.h"
#include "../Arsenal/Parser/parser.h"
#include "../Arsenal/Parser/grammar.h"
#include "../Arsenal/Ray/ray.h"
#include "../Arsenal/Ray/rayparser.h"

AR_NAMESPACE_BEGIN


static const wchar_t *__g_lex_name[] = {
L"delim = [ \\r\\n\\t]",
L"comment = /\\*([^\\*]|\\*+[^\\*/])*\\*+/",
L"comment_line = //[^\\n]*\\n",
L"digit = [0-9]",
L"number = {digit}+",
L"letter = [A-Z_a-z]",
L"oct = (\\\\[0-3][0-7][0-7])|(\\\\[0-7][0-7])|(\\\\[0-7])",
L"escape_seq = (\\\\(b|t|n|f|r|\\x22|\\x27|\\\\))|{oct}",
L"string_literal = \\x22({escape_seq}|[^\\x22\\n])*\\x22"
};


static struct {const wchar_t *name; size_t tokval; size_t lex_prec; const wchar_t *regex; bool_t skip; }__g_term_pattern[] =  {
{NULL, 256, 1, L"{delim}+|{comment}+|{comment_line}+", true},
{L"INTEGER", 257, 0, L"{number}", false},
{L"FLOAT", 258, 0, L"{number}", false},
{L"STRING", 259, 0, L"{string_literal}", false},
{L"ID", 260, 1, L"{letter}({letter}|{digit})*", false},
{L"=", 261, 1, L"\"=\"", false},
{L"&&", 262, 1, L"\"&&\"", false},
{L"||", 263, 1, L"\"||\"", false},
{L"==", 264, 1, L"\"==\"", false},
{L"!=", 265, 1, L"\"!=\"", false},
{L"<", 266, 1, L"\"<\"", false},
{L">", 267, 1, L"\">\"", false},
{L"<=", 268, 1, L"\"<=\"", false},
{L">=", 269, 1, L"\">=\"", false},
{L"+", 270, 1, L"\"+\"", false},
{L"-", 271, 1, L"\"-\"", false},
{L"*", 272, 1, L"\"*\"", false},
{L"/", 273, 1, L"\"/\"", false},
{L"%", 274, 1, L"\"%\"", false},
{L"!", 275, 1, L"\"!\"", false},
{L"[", 276, 1, L"\"[\"", false},
{L"]", 277, 1, L"\"]\"", false},
{L"(", 278, 1, L"\"(\"", false},
{L")", 279, 1, L"\")\"", false},
{L"{", 280, 1, L"\"{\"", false},
{L"}", 281, 1, L"\"}\"", false},
{L";", 282, 1, L"\";\"", false},
{L",", 283, 1, L"\",\"", false},
{L"int", 284, 1, L"\"int\"", false},
{L"float", 285, 1, L"\"float\"", false},
{L"bool", 286, 1, L"\"bool\"", false},
{L"char", 287, 1, L"\"char\"", false},
{L"for", 288, 1, L"\"for\"", false},
{L"if", 289, 1, L"\"if\"", false},
{L"break", 290, 1, L"\"break\"", false},
{L"do", 291, 1, L"\"do\"", false},
{L"else", 292, 1, L"\"else\"", false},
{L"while", 293, 1, L"\"while\"", false},
{L"true", 294, 1, L"\"true\"", false},
{L"false", 295, 1, L"\"false\"", false},
{L"EOI", 0, 2, L"$", false}
};


static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {
{L"+", 270, 1, },
{L"-", 271, 1, },
{L"*", 272, 2, PSR_ASSOC_RIGHT},
{L"/", 273, 2, PSR_ASSOC_RIGHT},
{L"IF_STMT_PREC", 296, 3, PSR_ASSOC_NOASSOC},
{L"IF_STMT_ELSE_STMT_PREC", 297, 4, PSR_ASSOC_NOASSOC},
{L"IF_STMT_ELSE_STMT_PREC33", 298, 5, PSR_ASSOC_NOASSOC}
};


/*program	:	block */
static psrNode_t* AR_STDCALL handle_program(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*block	:	{ decls stmts } */
static psrNode_t* AR_STDCALL handle_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*decls	:	decls decl */
static psrNode_t* AR_STDCALL handle_decls(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*decl	:	type ID ; */
static psrNode_t* AR_STDCALL handle_decl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*type	:	type [ INTEGER ] */
static psrNode_t* AR_STDCALL handle_type(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*stmts	:	stmts stmt */
static psrNode_t* AR_STDCALL handle_stmts(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*stmt	:	loc = bool_expr ; */
static psrNode_t* AR_STDCALL handle_stmt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*loc	:	loc [ bool_expr ] */
static psrNode_t* AR_STDCALL handle_loc(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*bool_expr	:	bool_expr || join */
static psrNode_t* AR_STDCALL handle_bool_expr(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*join	:	join && equality */
static psrNode_t* AR_STDCALL handle_join(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*equality	:	equality == rel */
static psrNode_t* AR_STDCALL handle_equality(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*rel	:	expr < expr */
static psrNode_t* AR_STDCALL handle_rel(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*expr	:	expr + term */
static psrNode_t* AR_STDCALL handle_expr(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*term	:	term * unary */
static psrNode_t* AR_STDCALL handle_term(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*unary	:	! unary */
static psrNode_t* AR_STDCALL handle_unary(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*factor	:	( bool_expr ) */
static psrNode_t* AR_STDCALL handle_factor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}



static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"program  :  block ", NULL, handle_program, 0},
{L"block  :  { decls stmts } ", NULL, handle_block, 0},
{L"decls  :  decls decl ", NULL, handle_decls, 0},
{L"decls  :   ", L"IF_STMT_ELSE_STMT_PREC33", handle_decls, 0},
{L"decl  :  type ID ; ", NULL, handle_decl, 0},
{L"type  :  type [ INTEGER ] ", NULL, handle_type, 0},
{L"type  :  int ", NULL, handle_type, 0},
{L"type  :  float ", NULL, handle_type, 0},
{L"type  :  bool ", NULL, handle_type, 0},
{L"type  :  char ", NULL, handle_type, 0},
{L"stmts  :  stmts stmt ", NULL, handle_stmts, 0},
{L"stmts  :   ", NULL, handle_stmts, 0},
{L"stmt  :  loc = bool_expr ; ", NULL, handle_stmt, 0},
{L"stmt  :  if ( bool_expr ) stmt ", L"IF_STMT_PREC", handle_stmt, 0},
{L"stmt  :  if ( bool_expr ) stmt else stmt ", L"IF_STMT_ELSE_STMT_PREC", handle_stmt, 0},
{L"stmt  :  while ( bool_expr ) stmt ", NULL, handle_stmt, 0},
{L"stmt  :  do stmt while ( bool_expr ) ; ", NULL, handle_stmt, 0},
{L"stmt  :  break ; ", NULL, handle_stmt, 0},
{L"stmt  :  block ", NULL, handle_stmt, 0},
{L"loc  :  loc [ bool_expr ] ", NULL, handle_loc, 0},
{L"loc  :  ID ", NULL, handle_loc, 0},
{L"bool_expr  :  bool_expr || join ", NULL, handle_bool_expr, 0},
{L"bool_expr  :  join ", NULL, handle_bool_expr, 0},
{L"join  :  join && equality ", NULL, handle_join, 0},
{L"join  :  equality ", NULL, handle_join, 0},
{L"equality  :  equality == rel ", NULL, handle_equality, 0},
{L"equality  :  equality != rel ", NULL, handle_equality, 0},
{L"equality  :  rel ", NULL, handle_equality, 0},
{L"rel  :  expr < expr ", NULL, handle_rel, 0},
{L"rel  :  expr <= expr ", NULL, handle_rel, 0},
{L"rel  :  expr >= expr ", NULL, handle_rel, 0},
{L"rel  :  expr > expr ", NULL, handle_rel, 0},
{L"rel  :  expr ", NULL, handle_rel, 0},
{L"expr  :  expr + term ", NULL, handle_expr, 0},
{L"expr  :  expr - term ", NULL, handle_expr, 0},
{L"expr  :  term ", NULL, handle_expr, 0},
{L"term  :  term * unary ", NULL, handle_term, 0},
{L"term  :  term / unary ", NULL, handle_term, 0},
{L"term  :  term % unary ", NULL, handle_term, 0},
{L"term  :  unary ", NULL, handle_term, 0},
{L"unary  :  ! unary ", NULL, handle_unary, 0},
{L"unary  :  - unary ", NULL, handle_unary, 0},
{L"unary  :  factor ", NULL, handle_unary, 0},
{L"factor  :  ( bool_expr ) ", NULL, handle_factor, 0},
{L"factor  :  loc ", NULL, handle_factor, 0},
{L"factor  :  INTEGER ", NULL, handle_factor, 0},
{L"factor  :  FLOAT ", NULL, handle_factor, 0},
{L"factor  :  STRING ", NULL, NULL, 0},
{L"factor  :  true ", NULL, handle_factor, 0},
{L"factor  :  false ", NULL, handle_factor, 0}
};

#define START_RULE L"program"

lex_t*	__build_lex(const arIOCtx_t *io)									
{																				
		lex_t	*lex;															
		size_t i;																
		lex = LEX_Create(io);													
		for(i = 0; i < AR_NELEMS(__g_lex_name); ++i)							
		{																		
				if(!LEX_Insert(lex, __g_lex_name[i]))							
				{																
						LEX_Destroy(lex);										
						AR_ASSERT(false);										
						return NULL;											
				}																
		}																		
		for(i = 0; i < AR_NELEMS(__g_term_pattern); ++i)						
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

psrGrammar_t*	__build_grammar(const psrCtx_t	*psr_ctx, const arIOCtx_t *io)													
	{																																
			psrGrammar_t	*grammar;																								
			size_t i;																												
			AR_ASSERT(psr_ctx != NULL);																								
			grammar = PSR_CreateGrammar(psr_ctx, io);																				
			for(i = 0; i < AR_NELEMS(__g_term_pattern); ++i)																		
			{																														
					if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)continue;										
					if(!PSR_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PSR_ASSOC_NOASSOC,0, NULL))	
					{																												
							PSR_DestroyGrammar(grammar);																			
							grammar = NULL;																							
							AR_ASSERT(false);																						
							return NULL;																							
					}																												
			}																														
			for(i = 0; i < AR_NELEMS(__g_prec_pattern); ++i)																		
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
			for(i = 0; i < AR_NELEMS(__g_rule_pattern); ++i)																													
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




static void	AR_STDCALL ray_parser_free(psrNode_t *node, void *ctx)
{
		AR_printf(L"%ls\r\n", L"ray_parser_free");
}

static void		AR_STDCALL ray_parser_error(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)
{
		AR_printf(L"%ls\r\n", L"ray_parser_error");

}


static psrNode_t*		AR_STDCALL ray_handle_token(const psrToken_t *tok,void *ctx)
{
		return NULL;
}


static const psrCtx_t		__g_default_psrctx = 
{
		ray_parser_error,
		ray_parser_free,
		NULL
};
void code_gen_lex_test()
{

		//lex_t *lex =	__build_lex(NULL);
		
		psrGrammar_t *gmr = __build_grammar(&__g_default_psrctx, NULL);
		getchar();

		//LEX_Destroy(lex);

		PSR_DestroyGrammar(gmr);

}



AR_NAMESPACE_END
