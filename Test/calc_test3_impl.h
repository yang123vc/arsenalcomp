

/*
pre_defined
*/
 

	static void		AR_STDCALL on_free_node(psrNode_t *node, void *ctx)
	{
			
	}

	static bool_t		AR_STDCALL on_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
	{
			wchar_t buf[256], tmp[128];
			AR_wcsncpy(tmp, tok->str, AR_MIN(tok->str_cnt, 128-1));
			tmp[AR_MIN(tok->str_cnt, 128-1)] = L'\0';
			AR_swprintf(buf, 512, L"Error token '%ls'", tmp);
			wprintf(L"%ls\r\n", buf);
			return true;
	}
	
	static const psrHandler_t	__g_handler = {on_error, on_free_node};

 

static const wchar_t *__g_lex_name[] = {
L"delim = [ \\r\\n\\t]",
L"comment = /\\*([^\\*]|\\*+[^\\*/])*\\*+/",
L"comment_line = //[^\\n]*\\n",
L"digit = [0-9]",
L"number = {digit}+"
};

#define __NAME_COUNT__ ((size_t)5)

static psrNode_t* AR_STDCALL default_leaf_handler(const psrToken_t *tok,void *ctx);




enum{
TOK_NUMBER = 258,
TOK_ADD = 259,
TOK_MINUS = 260,
TOK_MUL = 261,
TOK_DIV = 262,
TOK_LP = 263,
TOK_RP = 264,
TOK_MOD = 265,
};




static struct {const wchar_t *name;
size_t tokval;
size_t lex_prec;
const wchar_t *regex;
bool_t skip;
psrTermFunc_t leaf;
}__g_term_pattern[] =  {
{NULL, 257,1, L"{delim}+|{comment}+|{comment_line}+", true, NULL},
{L"NUM", TOK_NUMBER, 0, L"{number}", false, default_leaf_handler},
{L"+", TOK_ADD, 0, L"\"+\"", false, default_leaf_handler},
{L"-", TOK_MINUS, 0, L"\"-\"", false, default_leaf_handler},
{L"*", TOK_MUL, 0, L"\"*\"", false, default_leaf_handler},
{L"/", TOK_DIV, 0, L"\"/\"", false, default_leaf_handler},
{L"(", TOK_LP, 0, L"\"(\"", false, default_leaf_handler},
{L")", TOK_RP, 0, L"\")\"", false, default_leaf_handler},
{L"%", TOK_MOD, 0, L"\"%\"", false, default_leaf_handler},
{L"CFG_LEXVAL_EOI", 0, 2, L"$", false, NULL}
};

#define __TERM_COUNT__ ((size_t)10)

static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {
{L"+", TOK_ADD,1, PARSER_ASSOC_LEFT},
{L"-", TOK_MINUS,1, PARSER_ASSOC_LEFT},
{L"*", TOK_MUL,2, PARSER_ASSOC_LEFT},
{L"/", TOK_DIV,2, PARSER_ASSOC_LEFT},
{L"%", TOK_MOD,2, PARSER_ASSOC_LEFT},
{L"UMINUS", 266,3, PARSER_ASSOC_RIGHT}
};

#define __PREC_COUNT__ ((size_t)6)

/*E	:	E + E */
/*E	:	E - E */
/*E	:	E * E */
/*E	:	E / E */
/*E	:	E % E */
static psrNode_t* AR_STDCALL on_calc(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*E	:	( E ) */
static psrNode_t* AR_STDCALL auto_return_1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*E	:	- E */
static psrNode_t* AR_STDCALL on_negative_num(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);




static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"E  :  E + E ", NULL, on_calc, 0},
{L"E  :  E - E ", NULL, on_calc, 0},
{L"E  :  E * E ", NULL, on_calc, 0},
{L"E  :  E / E ", NULL, on_calc, 0},
{L"E  :  E % E ", NULL, on_calc, 0},
{L"E  :  ( E ) ", NULL, auto_return_1, 0},
{L"E  :  - E ", L"UMINUS", on_negative_num, 0},
{L"E  :  NUM ", NULL, NULL, 0}
};

#define __RULE_COUNT__ ((size_t)8)
#define START_RULE L"E"

static lex_t*	__build_lex()													
{																				
		lex_t	*lex;															
		size_t i;																
		lex = Lex_Create();														
		for(i = 0; i < __NAME_COUNT__; ++i)										
		{																		
				if(!Lex_Insert(lex, __g_lex_name[i]))							
				{																
						Lex_Destroy(lex);										
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
				if(!Lex_InsertRule(lex, __g_term_pattern[i].regex, &act))		
				{																
						Lex_Destroy(lex);										
						AR_ASSERT(false);										
						return NULL;											
				}																
		}																		
		if(!Lex_GenerateTransTable(lex))													
		{																					
				AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);		
		}																					
		return lex;																			
}

static psrGrammar_t*	__build_grammar(const psrHandler_t	*handler)															
{																																
		psrGrammar_t	*grammar;																								
		size_t i;																												
		AR_ASSERT(handler != NULL);																								
		grammar = Parser_CreateGrammar(handler);																				
		for(i = 0; i < __TERM_COUNT__; ++i)																						
		{																														
				if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)continue;										
				if(!Parser_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PARSER_ASSOC_NONASSOC,0, __g_term_pattern[i].leaf))	
				{																												
						Parser_DestroyGrammar(grammar);																			
						grammar = NULL;																							
						AR_ASSERT(false);																						
						return NULL;																							
				}																												
		}																														
		for(i = 0; i < __PREC_COUNT__; ++i)																						
		{																														
				psrTermInfo_t	*info;																							
				info = Parser_GetTermSymbInfoByName(grammar, __g_prec_pattern[i].name);											
				if(info == NULL)																								
				{																												
						if(!Parser_InsertTerm(grammar, __g_prec_pattern[i].name, __g_prec_pattern[i].tokval, __g_prec_pattern[i].assoc, __g_prec_pattern[i].prec_level, NULL))
						{																																					
								Parser_DestroyGrammar(grammar);																												
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
				if(!Parser_InsertRuleByStr(grammar, __g_rule_pattern[i].rule, __g_rule_pattern[i].prec_token, __g_rule_pattern[i].handler, __g_rule_pattern[i].auto_ret))		
				{																																							
						Parser_DestroyGrammar(grammar);																														
						grammar = NULL;																																		
						AR_ASSERT(false);																																	
						return NULL;																																		
				}																																							
		}																																									
		if(!Parser_SetStartRule(grammar,START_RULE) || !Parser_CheckIsValidGrammar(grammar, NULL))																			
		{																																									
				Parser_DestroyGrammar(grammar);																																
				grammar = NULL;																																				
				AR_ASSERT(false);																																			
				return NULL;																																				
		}																																									
		return grammar;																																						
}



static psrNode_t* AR_STDCALL default_leaf_handler(const psrToken_t *tok,void *ctx)
{
	 { 
				AR_ASSERT(tok != NULL && tok->str_cnt > 0);

				switch(tok->term_val)
				{
				case TOK_NUMBER:
				{
					int_t num;
					AR_wtoi_s(tok->str, tok->str + tok->str_cnt, &num, 10);
					return (psrNode_t*)num;
				}
					break;
				case TOK_ADD:
				case TOK_MINUS:
				case TOK_MUL:
				case TOK_DIV:
				case TOK_MOD:
				case TOK_LP:
				case TOK_RP:
					return (psrNode_t*)tok->term_val;
					break;
				default:
					AR_ASSERT(false);
					return NULL;
				}
			 }
}


/*E	:	E + E */
/*E	:	E - E */
/*E	:	E * E */
/*E	:	E / E */
/*E	:	E % E */
static psrNode_t* AR_STDCALL on_calc(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 { 
			int_t l, r;
			size_t op;
			AR_ASSERT(nodes != NULL && count == 3);
			l = (int_t)nodes[0];
			op = (size_t)nodes[1];
			r = (int_t)nodes[2];
			switch(op)
			{
			case TOK_ADD:
				return (psrNode_t*)(l + r);
			case TOK_MINUS:
				return (psrNode_t*)(l - r);
			case TOK_MUL:
				return (psrNode_t*)(l * r);
			case TOK_DIV:
			case TOK_MOD:
			{
				if(r == 0)
				{
					*(bool_t*)ctx = false;
					return (psrNode_t*)0;
				}else
				{
					return (psrNode_t*) (op == TOK_MOD ? (l % r) : (l / r));
				}
			}
			default:
				AR_ASSERT(false);
				return (psrNode_t*)0;
			}
		 }
}




/*E	:	( E ) */
static psrNode_t* AR_STDCALL auto_return_1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 { 
				return nodes[1];

		 }
}




/*E	:	- E */
static psrNode_t* AR_STDCALL on_negative_num(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 { 
			int_t n;
			AR_ASSERT(nodes != NULL && count == 2);
			n = (int_t)nodes[1];
			return (psrNode_t*)-n;
		 }
}






/*
Calc
*/
 


static int calc(const wchar_t *input)
{
		lex_t *lex;
		lexMatch_t *match;
		lexToken_t		tok;
		psrGrammar_t	*grammar;
		const parser_t		*parser;
		psrContext_t *ctx;
		bool_t is_ok;
		int_t v = 0;
		lex = __build_lex();
		grammar = __build_grammar(&__g_handler);
		parser = Parser_CreateParser(grammar, PARSER_LALR);
		ctx = Parser_CreateContext(parser, (void*)&is_ok);


		match = Lex_CreateMatch(lex);

		Lex_ResetInput(match, input);
		
		is_ok = true;
		while(is_ok)
		{
				is_ok = Lex_Match(match, &tok);

				if(!is_ok)
				{
						AR_printf(L"Invalid Token == %ls\r\n", Lex_GetNextInput(match));
						continue;
				}else
				{
						psrToken_t psr_tok;

						PARSER_TOTERMTOK(&tok, &psr_tok);
						
						if(!Parser_AddToken(ctx, &psr_tok))
						{
								is_ok = false;
						}

						
						if(is_ok)
						{
								if(psr_tok.term_val == 0)break;		

						}else
						{
								continue;
						}

						
				}
		}
		
		
		
		if(is_ok)
		{
				v = (int)Parser_GetResult(ctx);
		}else
		{
				printf("error expr\r\n");
		}

		Lex_DestroyMatch(match);
		Lex_Destroy(lex);
		Parser_DestroyContext(ctx);
		Parser_DestroyParser(parser);
		Parser_DestroyGrammar(grammar);
		
		return v;
}

 



