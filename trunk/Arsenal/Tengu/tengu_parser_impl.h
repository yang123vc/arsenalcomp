
 
/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */
 



/*
syntax_node
*/
 

 


/*
syntax_tree_handler
*/
 

    static void        AR_STDCALL on_free_node(psrNode_t *node, void *ctx)
    {
		AR_ASSERT(node != NULL && ctx != NULL);
		
        
    }

    static arStatus_t        AR_STDCALL on_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
    {
/*
            tguParser_t    *parser;
            size_t        i;
            arString_t    *str;
            wchar_t        *tok_str;
            AR_ASSERT(tok != NULL && ctx != NULL);

            parser = (tguParser_t*)ctx;
            str = AR_CreateString();

            if(tok->term_val == 0)
            {
                    tok_str = AR_wcsdup(L"EOI");
            }else
            {
                    tok_str = AR_wcsndup(tok->str, tok->str_cnt);
            }
        
            AR_AppendFormatString(str, L"Invalid token : '%ls', expected ", tok_str);
            
            for(i = 0; i < count; ++i)
            {
                    AR_AppendFormatString(str, L"'%ls'", TGU_TokenValToString(expected[i]));
            }
            
            TGU_ReportError(&parser->report, AR_GetStringCString(str), tok->line);
        
            AR_DestroyString(str);
            str = NULL;
            AR_DEL(tok_str);
            tok_str = NULL;
*/
            return AR_S_YES;
    }
    
    static const psrHandler_t    __g_handler = {on_error, on_free_node};
 


/*
token_operation
*/
 
    
   
 


/*
handle_function
*/
 
    
 


/*
handle_init_declarator
*/
 
    
 

 
    
 

 
          

 

 
          

 

 
 

 
 
 

 
    
 

 
 

 

 

 
 

 

 


/*
handle_constant
*/
 
   
 


/*
handle_identifier
*/
 
    


 

 

 

static const wchar_t *__g_lex_name[] = {
L"delim = [\\x{000A}\\x{000B}\\x{000C}\\x{000D}\\x{0085}\\x{2028}\\x{2029}\\x{0020}\\f\\n\\r\\t\\v\\x{0009}\\x{0020}\\x{00A0}\\x{1680}\\x{180E}\\x{2000}-\\x{200A}\\x{202F}\\x{205F}\\x{3000}]",
L"comment = /\\*([^\\*]|\\*+[^\\*/])*\\*+/",
L"comment_line = (//[^\\x{000A}\\x{000B}\\x{000C}\\x{000D}\\x{0085}\\x{2028}\\x{2029}]*(\\x{000A}|\\x{000B}|\\x{000C}|\\x{000D}|\\x{0085}|\\x{2028}|\\x{2029}|$))",
L"skip_lexem = {delim}|{comment_line}|{comment}",
L"digit = [0-9]",
L"number = {digit}+",
L"letter = [A-Z_a-z\\x{0800}-\\x{4E00}\\x{4E00}-\\x{9FA5}\\x{3130}-\\x{318F}\\x{AC00}-\\x{D7AF}]",
L"hex_digit = [0-9a-fA-F]",
L"hex_literal = 0(x|X){hex_digit}+",
L"oct_literal = 0[0-7]+",
L"dec_literal = (0|[1-9][0-9]*)",
L"exponet = (e|E)(\\+|\\-)?[0-9]+",
L"float_literal = (((([0-9]\\.[0-9]*)){exponet}?)|([0-9]+{exponet}))",
L"escape_seq = (\\\\(\\x22|\\x27))",
L"string_dq = (\\x22({escape_seq}|[^\\x22])*\\x22)",
L"string_sq = \\x27({escape_seq}|[^\\x27])*\\x27",
L"keyword_lhd = {letter}|[0-9]",
L"float_constant = {float_literal}(?!{keyword_lhd})",
L"hex_constant = {hex_literal}(?!{keyword_lhd})",
L"oct_constant = {oct_literal}(?!{keyword_lhd})",
L"dec_constant = {dec_literal}(?!{keyword_lhd})"
};

#define __NAME_COUNT__ ((size_t)21)

static psrRetVal_t AR_STDCALL default_leaf_handler(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL on_string_leaf_handler(const psrToken_t *tok,void *ctx);




enum{
TOK_DELIM_ID = 257,
TOK_NAME = 258,
TOK_STRING = 259,
TOK_FLOAT_NUMBER = 260,
TOK_INT_NUMBER = 261,
TOK_IMPORT = 262,
TOK_FROM = 263,
TOK_FOR = 264,
TOK_DO = 265,
TOK_WHILE = 266,
TOK_IF = 267,
TOK_ELSE = 268,
TOK_CONTINUE = 269,
TOK_BREAK = 270,
TOK_RETURN = 271,
TOK_NULL = 272,
TOK_TRUE = 273,
TOK_FALSE = 274,
TOK_VAR = 275,
TOK_IN = 276,
TOK_LIST = 277,
TOK_TABLE = 278,
TOK_ELLIPSIS = 279,
TOK_INC = 280,
TOK_DEC = 281,
TOK_ANDAND = 282,
TOK_OROR = 283,
TOK_LE = 284,
TOK_GE = 285,
TOK_EQ = 286,
TOK_NE = 287,
TOK_LESS = 288,
TOK_GREATER = 289,
TOK_L_BRACES = 290,
TOK_R_BRACES = 291,
TOK_L_PAREN = 292,
TOK_R_PAREN = 293,
TOK_L_SQUARE = 294,
TOK_R_SQUARE = 295,
TOK_SEMICOLON = 296,
TOK_COMMA = 297,
TOK_ASSIGN = 298,
TOK_ADD = 299,
TOK_SUB = 300,
TOK_MUL = 301,
TOK_DIV = 302,
TOK_MOD = 303,
TOK_NOT = 304,
TOK_COLON = 305,
TOK_QUEST = 306,
TOK_DOT = 307,
};




static struct {const wchar_t *name;
size_t tokval;
size_t lex_prec;
const wchar_t *regex;
bool_t skip;
psrTermFunc_t leaf;
}__g_term_pattern[] =  {
{NULL, TOK_DELIM_ID,1, L"{skip_lexem}", true, NULL},
{L"NAME", TOK_NAME, 0, L"{letter}({letter}|{digit})*", false, default_leaf_handler},
{L"STRING", TOK_STRING, 0, L"{string_dq}|{string_sq}", false, on_string_leaf_handler},
{L"FLOAT_NUMBER", TOK_FLOAT_NUMBER, 2, L"{float_constant}", false, default_leaf_handler},
{L"INT_NUMBER", TOK_INT_NUMBER, 2, L"{hex_constant}|{oct_constant}|{dec_constant}", false, default_leaf_handler},
{L"#import", TOK_IMPORT, 1, L"\"#import\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"from", TOK_FROM, 1, L"\"from\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"for", TOK_FOR, 1, L"\"for\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"do", TOK_DO, 1, L"\"do\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"while", TOK_WHILE, 1, L"\"while\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"if", TOK_IF, 1, L"\"if\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"else", TOK_ELSE, 1, L"\"else\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"continue", TOK_CONTINUE, 1, L"\"continue\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"break", TOK_BREAK, 1, L"\"break\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"return", TOK_RETURN, 1, L"\"return\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"null", TOK_NULL, 1, L"\"null\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"true", TOK_TRUE, 1, L"\"true\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"false", TOK_FALSE, 1, L"\"false\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"var", TOK_VAR, 1, L"\"var\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"in", TOK_IN, 1, L"\"in\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"list", TOK_LIST, 1, L"\"list\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"table", TOK_TABLE, 1, L"\"table\"(?!{keyword_lhd})", false, default_leaf_handler},
{L"...", TOK_ELLIPSIS, 2, L"\"...\"", false, default_leaf_handler},
{L"++", TOK_INC, 1, L"\"++\"", false, default_leaf_handler},
{L"--", TOK_DEC, 1, L"\"--\"", false, default_leaf_handler},
{L"&&", TOK_ANDAND, 1, L"\"&&\"", false, default_leaf_handler},
{L"||", TOK_OROR, 1, L"\"||\"", false, default_leaf_handler},
{L"<=", TOK_LE, 1, L"\"<=\"", false, default_leaf_handler},
{L">=", TOK_GE, 1, L"\">=\"", false, default_leaf_handler},
{L"==", TOK_EQ, 1, L"\"==\"", false, default_leaf_handler},
{L"!=", TOK_NE, 1, L"\"!=\"", false, default_leaf_handler},
{L"<", TOK_LESS, 0, L"\"<\"", false, default_leaf_handler},
{L">", TOK_GREATER, 0, L"\">\"", false, default_leaf_handler},
{L"{", TOK_L_BRACES, 0, L"\"{\"", false, default_leaf_handler},
{L"}", TOK_R_BRACES, 0, L"\"}\"", false, default_leaf_handler},
{L"(", TOK_L_PAREN, 0, L"\"(\"", false, default_leaf_handler},
{L")", TOK_R_PAREN, 0, L"\")\"", false, default_leaf_handler},
{L"[", TOK_L_SQUARE, 0, L"\"[\"", false, default_leaf_handler},
{L"]", TOK_R_SQUARE, 0, L"\"]\"", false, default_leaf_handler},
{L";", TOK_SEMICOLON, 0, L"\";\"", false, default_leaf_handler},
{L",", TOK_COMMA, 0, L"\",\"", false, default_leaf_handler},
{L"=", TOK_ASSIGN, 0, L"\"=\"", false, default_leaf_handler},
{L"+", TOK_ADD, 0, L"\"+\"", false, default_leaf_handler},
{L"-", TOK_SUB, 0, L"\"-\"", false, default_leaf_handler},
{L"*", TOK_MUL, 0, L"\"*\"", false, default_leaf_handler},
{L"/", TOK_DIV, 0, L"\"/\"", false, default_leaf_handler},
{L"%", TOK_MOD, 0, L"\"%\"", false, default_leaf_handler},
{L"!", TOK_NOT, 0, L"\"!\"", false, default_leaf_handler},
{L":", TOK_COLON, 0, L"\":\"", false, default_leaf_handler},
{L"?", TOK_QUEST, 0, L"\"?\"", false, default_leaf_handler},
{L".", TOK_DOT, 0, L"\".\"", false, default_leaf_handler},
{L"CFG_LEXVAL_EOI", 0, 2, L"$", false, NULL}
};

#define __TERM_COUNT__ ((size_t)52)

static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {
{L"?", TOK_QUEST,1, PARSER_ASSOC_RIGHT},
{L":", TOK_COLON,1, PARSER_ASSOC_RIGHT},
{L"||", TOK_OROR,2, PARSER_ASSOC_LEFT},
{L"&&", TOK_ANDAND,3, PARSER_ASSOC_LEFT},
{L"==", TOK_EQ,4, PARSER_ASSOC_LEFT},
{L"!=", TOK_NE,4, PARSER_ASSOC_LEFT},
{L"<", TOK_LESS,4, PARSER_ASSOC_LEFT},
{L"<=", TOK_LE,4, PARSER_ASSOC_LEFT},
{L">", TOK_GREATER,4, PARSER_ASSOC_LEFT},
{L">=", TOK_GE,4, PARSER_ASSOC_LEFT},
{L"+", TOK_ADD,5, PARSER_ASSOC_LEFT},
{L"-", TOK_SUB,5, PARSER_ASSOC_LEFT},
{L"*", TOK_MUL,6, PARSER_ASSOC_LEFT},
{L"/", TOK_DIV,6, PARSER_ASSOC_LEFT},
{L"%", TOK_MOD,6, PARSER_ASSOC_LEFT},
{L"IF_WITHOUT_ELSE", 308,7, PARSER_ASSOC_NONASSOC},
{L"else", TOK_ELSE,8, PARSER_ASSOC_NONASSOC}
};

#define __PREC_COUNT__ ((size_t)17)

/*module	:	element_list */
/*module	:	 */
static psrRetVal_t AR_STDCALL on_module(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*element_list	:	element */
/*element_list	:	element_list element */
/*element	:	declaration */
/*element	:	function_defination */
/*declaration	:	var init_declarator_list semi */
/*declaration	:	var error ; */
/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
/*compound_element	:	declaration */
/*expression_statement	:	error ; */
static psrRetVal_t AR_STDCALL auto_return_null(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*element	:	statement */
static psrRetVal_t AR_STDCALL handle_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_signature	:	var NAME ( params ) */
static psrRetVal_t AR_STDCALL on_function_signature(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_defination	:	function_signature compound_statement */
static psrRetVal_t AR_STDCALL on_function_defination(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist , ... */
static psrRetVal_t AR_STDCALL on_namelist_ellipsis(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist */
/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
/*statement	:	empty_statement */
/*statement	:	import_statement */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*semi	:	; */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
/*primary_expression	:	constant */
/*primary_expression	:	aggregate_constructor */
/*expression_list	:	expression */
static psrRetVal_t AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	... */
static psrRetVal_t AR_STDCALL on_ellipsis(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namelist	:	namelist , NAME */
static psrRetVal_t AR_STDCALL on_name_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namelist	:	NAME */
static psrRetVal_t AR_STDCALL on_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator	:	NAME = expression */
/*init_declarator	:	NAME */
static psrRetVal_t AR_STDCALL on_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*aggregate_constructor	:	table_constructor */
/*aggregate_constructor	:	list_constructor */
static psrRetVal_t AR_STDCALL handle_aggregate_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*list_constructor	:	list { list_field_list optional_dot } */
/*list_constructor	:	list { } */
/*list_constructor	:	list { error } */
static psrRetVal_t AR_STDCALL handle_list_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*list_field_list	:	list_field_list , expression */
/*list_field_list	:	expression */
static psrRetVal_t AR_STDCALL handle_list_field_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*table_constructor	:	table { table_field_list optional_dot } */
/*table_constructor	:	table { } */
/*table_constructor	:	table { error } */
static psrRetVal_t AR_STDCALL on_table_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*table_field_list	:	table_field_list , table_field */
/*table_field_list	:	table_field */
static psrRetVal_t AR_STDCALL handle_table_field_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*table_field	:	expression : expression */
static psrRetVal_t AR_STDCALL handle_table_field(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*optional_dot	:	, */
/*optional_dot	:	 */
static psrRetVal_t AR_STDCALL handle_optional_dot(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	start_block compound_element_list } */
static psrRetVal_t AR_STDCALL on_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	start_block } */
static psrRetVal_t AR_STDCALL on_empty_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	start_block error } */
static psrRetVal_t AR_STDCALL on_compound_error_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*start_block	:	{ */
static psrRetVal_t AR_STDCALL on_start_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_element_list	:	compound_element_list compound_element */
/*compound_element_list	:	compound_element */
static psrRetVal_t AR_STDCALL handle_compound_element_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_element	:	statement */
static psrRetVal_t AR_STDCALL on_compound_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*import_statement	:	#import NAME ; */
/*import_statement	:	#import NAME from STRING ; */
/*import_statement	:	#import error ; */
static psrRetVal_t AR_STDCALL on_import_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*empty_statement	:	; */
static psrRetVal_t AR_STDCALL on_empty_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
static psrRetVal_t AR_STDCALL on_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*if_statement	:	if ( expression ) statement */
/*if_statement	:	if ( error ) statement */
static psrRetVal_t AR_STDCALL on_if_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*if_else_statement	:	if ( expression ) statement else statement */
/*if_else_statement	:	if ( error ) statement else statement */
static psrRetVal_t AR_STDCALL on_if_else_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*while_statement	:	while enter_loop ( expression ) statement leave_loop */
/*while_statement	:	while enter_loop ( error ) statement leave_loop */
static psrRetVal_t AR_STDCALL on_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*do_while_statement	:	do enter_loop statement while ( expression ) leave_loop semi */
/*do_while_statement	:	do enter_loop statement while ( error ) leave_loop semi */
static psrRetVal_t AR_STDCALL on_do_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*enter_loop	:	 */
static psrRetVal_t AR_STDCALL on_enter_loop(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*leave_loop	:	 */
static psrRetVal_t AR_STDCALL on_leave_loop(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*jump_statement	:	continue semi */
static psrRetVal_t AR_STDCALL on_continue_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*jump_statement	:	break semi */
static psrRetVal_t AR_STDCALL on_break_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*jump_statement	:	return semi */
/*jump_statement	:	return expression semi */
static psrRetVal_t AR_STDCALL on_return_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*semi	:	error */
static psrRetVal_t AR_STDCALL on_semi_error(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*assignment_expression	:	unary_expression = assignment_expression */
static psrRetVal_t AR_STDCALL on_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*constant_expression	:	binary_expression ? expression : expression */
static psrRetVal_t AR_STDCALL on_condition_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*binary_expression	:	binary_expression + binary_expression */
/*binary_expression	:	binary_expression - binary_expression */
/*binary_expression	:	binary_expression * binary_expression */
/*binary_expression	:	binary_expression / binary_expression */
/*binary_expression	:	binary_expression % binary_expression */
/*binary_expression	:	binary_expression < binary_expression */
/*binary_expression	:	binary_expression <= binary_expression */
/*binary_expression	:	binary_expression > binary_expression */
/*binary_expression	:	binary_expression >= binary_expression */
/*binary_expression	:	binary_expression == binary_expression */
/*binary_expression	:	binary_expression != binary_expression */
/*binary_expression	:	binary_expression && binary_expression */
/*binary_expression	:	binary_expression || binary_expression */
static psrRetVal_t AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_expression	:	+ unary_expression */
/*unary_expression	:	- unary_expression */
/*unary_expression	:	! unary_expression */
/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
static psrRetVal_t AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
static psrRetVal_t AR_STDCALL on_post_add_minus_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression [ error ] */
static psrRetVal_t AR_STDCALL on_index_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	postfix_expression . NAME */
static psrRetVal_t AR_STDCALL on_access_name_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	( expression ) */
/*primary_expression	:	( error ) */
static psrRetVal_t AR_STDCALL on_lp_rp_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	NAME */
static psrRetVal_t AR_STDCALL on_identifier_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*constant	:	FLOAT_NUMBER */
/*constant	:	INT_NUMBER */
/*constant	:	STRING */
/*constant	:	true */
/*constant	:	false */
/*constant	:	null */
static psrRetVal_t AR_STDCALL on_constant(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*call_expression	:	postfix_expression ( expression_list ) */
/*call_expression	:	postfix_expression ( error ) */
/*call_expression	:	postfix_expression ( ) */
static psrRetVal_t AR_STDCALL on_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_list	:	expression_list , expression */
static psrRetVal_t AR_STDCALL on_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);




static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"module  :  element_list ", NULL, on_module, 0},
{L"module  :   ", NULL, on_module, 0},
{L"element_list  :  element ", NULL, auto_return_null, 0},
{L"element_list  :  element_list element ", NULL, auto_return_null, 0},
{L"element  :  declaration ", NULL, auto_return_null, 0},
{L"element  :  function_defination ", NULL, auto_return_null, 0},
{L"element  :  statement ", NULL, handle_element, 0},
{L"function_signature  :  var NAME ( params ) ", NULL, on_function_signature, 0},
{L"function_defination  :  function_signature compound_statement ", NULL, on_function_defination, 0},
{L"params  :  namelist , ... ", NULL, on_namelist_ellipsis, 0},
{L"params  :  namelist ", NULL, auto_return_0, 0},
{L"params  :  ... ", NULL, on_ellipsis, 0},
{L"params  :   ", NULL, NULL, 0},
{L"namelist  :  namelist , NAME ", NULL, on_name_list, 0},
{L"namelist  :  NAME ", NULL, on_name, 0},
{L"declaration  :  var init_declarator_list semi ", NULL, auto_return_null, 0},
{L"declaration  :  var error ; ", NULL, auto_return_null, 0},
{L"init_declarator_list  :  init_declarator ", NULL, auto_return_null, 0},
{L"init_declarator_list  :  init_declarator_list , init_declarator ", NULL, auto_return_null, 0},
{L"init_declarator  :  NAME = expression ", NULL, on_declarator, 0},
{L"init_declarator  :  NAME ", NULL, on_declarator, 0},
{L"aggregate_constructor  :  table_constructor ", NULL, handle_aggregate_constructor, 0},
{L"aggregate_constructor  :  list_constructor ", NULL, handle_aggregate_constructor, 0},
{L"list_constructor  :  list { list_field_list optional_dot } ", NULL, handle_list_constructor, 0},
{L"list_constructor  :  list { } ", NULL, handle_list_constructor, 0},
{L"list_constructor  :  list { error } ", NULL, handle_list_constructor, 0},
{L"list_field_list  :  list_field_list , expression ", NULL, handle_list_field_list, 0},
{L"list_field_list  :  expression ", NULL, handle_list_field_list, 0},
{L"table_constructor  :  table { table_field_list optional_dot } ", NULL, on_table_constructor, 0},
{L"table_constructor  :  table { } ", NULL, on_table_constructor, 0},
{L"table_constructor  :  table { error } ", NULL, on_table_constructor, 0},
{L"table_field_list  :  table_field_list , table_field ", NULL, handle_table_field_list, 0},
{L"table_field_list  :  table_field ", NULL, handle_table_field_list, 0},
{L"table_field  :  expression : expression ", NULL, handle_table_field, 0},
{L"optional_dot  :  , ", NULL, handle_optional_dot, 0},
{L"optional_dot  :   ", NULL, handle_optional_dot, 0},
{L"statement  :  compound_statement ", NULL, auto_return_0, 0},
{L"statement  :  expression_statement ", NULL, auto_return_0, 0},
{L"statement  :  selection_statement ", NULL, auto_return_0, 0},
{L"statement  :  iteration_statement ", NULL, auto_return_0, 0},
{L"statement  :  jump_statement ", NULL, auto_return_0, 0},
{L"statement  :  empty_statement ", NULL, auto_return_0, 0},
{L"statement  :  import_statement ", NULL, auto_return_0, 0},
{L"compound_statement  :  start_block compound_element_list } ", NULL, on_compound_statement, 0},
{L"compound_statement  :  start_block } ", NULL, on_empty_compound_statement, 0},
{L"compound_statement  :  start_block error } ", NULL, on_compound_error_statement, 0},
{L"start_block  :  { ", NULL, on_start_block, 0},
{L"compound_element_list  :  compound_element_list compound_element ", NULL, handle_compound_element_list, 0},
{L"compound_element_list  :  compound_element ", NULL, handle_compound_element_list, 0},
{L"compound_element  :  statement ", NULL, on_compound_element, 0},
{L"compound_element  :  declaration ", NULL, auto_return_null, 0},
{L"import_statement  :  #import NAME ; ", NULL, on_import_statement, 0},
{L"import_statement  :  #import NAME from STRING ; ", NULL, on_import_statement, 0},
{L"import_statement  :  #import error ; ", NULL, on_import_statement, 0},
{L"empty_statement  :  ; ", NULL, on_empty_statement, 0},
{L"expression_statement  :  expression semi ", NULL, on_expression_statement, 0},
{L"expression_statement  :  error ; ", NULL, auto_return_null, 0},
{L"selection_statement  :  if_statement ", NULL, auto_return_0, 0},
{L"selection_statement  :  if_else_statement ", NULL, auto_return_0, 0},
{L"if_statement  :  if ( expression ) statement ", L"IF_WITHOUT_ELSE", on_if_statement, 0},
{L"if_statement  :  if ( error ) statement ", L"IF_WITHOUT_ELSE", on_if_statement, 0},
{L"if_else_statement  :  if ( expression ) statement else statement ", NULL, on_if_else_statement, 0},
{L"if_else_statement  :  if ( error ) statement else statement ", NULL, on_if_else_statement, 0},
{L"iteration_statement  :  while_statement ", NULL, auto_return_0, 0},
{L"iteration_statement  :  do_while_statement ", NULL, auto_return_0, 0},
{L"while_statement  :  while enter_loop ( expression ) statement leave_loop ", NULL, on_while_statement, 0},
{L"while_statement  :  while enter_loop ( error ) statement leave_loop ", NULL, on_while_statement, 0},
{L"do_while_statement  :  do enter_loop statement while ( expression ) leave_loop semi ", NULL, on_do_while_statement, 0},
{L"do_while_statement  :  do enter_loop statement while ( error ) leave_loop semi ", NULL, on_do_while_statement, 0},
{L"enter_loop  :   ", NULL, on_enter_loop, 0},
{L"leave_loop  :   ", NULL, on_leave_loop, 0},
{L"jump_statement  :  continue semi ", NULL, on_continue_statement, 0},
{L"jump_statement  :  break semi ", NULL, on_break_statement, 0},
{L"jump_statement  :  return semi ", NULL, on_return_statement, 0},
{L"jump_statement  :  return expression semi ", NULL, on_return_statement, 0},
{L"semi  :  ; ", NULL, auto_return_0, 0},
{L"semi  :  error ", NULL, on_semi_error, 0},
{L"expression  :  assignment_expression ", NULL, auto_return_0, 0},
{L"assignment_expression  :  constant_expression ", NULL, auto_return_0, 0},
{L"assignment_expression  :  unary_expression = assignment_expression ", NULL, on_assignment_expression, 0},
{L"constant_expression  :  binary_expression ", NULL, auto_return_0, 0},
{L"constant_expression  :  binary_expression ? expression : expression ", NULL, on_condition_expression, 0},
{L"binary_expression  :  unary_expression ", NULL, auto_return_0, 0},
{L"binary_expression  :  binary_expression + binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression - binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression * binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression / binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression % binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression < binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression <= binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression > binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression >= binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression == binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression != binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression && binary_expression ", NULL, on_binary_expression, 0},
{L"binary_expression  :  binary_expression || binary_expression ", NULL, on_binary_expression, 0},
{L"unary_expression  :  + unary_expression ", NULL, on_unary_expression, 0},
{L"unary_expression  :  - unary_expression ", NULL, on_unary_expression, 0},
{L"unary_expression  :  ! unary_expression ", NULL, on_unary_expression, 0},
{L"unary_expression  :  ++ unary_expression ", NULL, on_unary_expression, 0},
{L"unary_expression  :  -- unary_expression ", NULL, on_unary_expression, 0},
{L"unary_expression  :  postfix_expression ", NULL, auto_return_0, 0},
{L"postfix_expression  :  postfix_expression ++ ", NULL, on_post_add_minus_expression, 0},
{L"postfix_expression  :  postfix_expression -- ", NULL, on_post_add_minus_expression, 0},
{L"postfix_expression  :  postfix_expression [ expression ] ", NULL, on_index_expression, 0},
{L"postfix_expression  :  postfix_expression [ error ] ", NULL, on_index_expression, 0},
{L"postfix_expression  :  postfix_expression . NAME ", NULL, on_access_name_expression, 0},
{L"postfix_expression  :  call_expression ", NULL, auto_return_0, 0},
{L"postfix_expression  :  primary_expression ", NULL, auto_return_0, 0},
{L"primary_expression  :  ( expression ) ", NULL, on_lp_rp_expression, 0},
{L"primary_expression  :  ( error ) ", NULL, on_lp_rp_expression, 0},
{L"primary_expression  :  NAME ", NULL, on_identifier_expression, 0},
{L"primary_expression  :  constant ", NULL, auto_return_0, 0},
{L"primary_expression  :  aggregate_constructor ", NULL, auto_return_0, 0},
{L"constant  :  FLOAT_NUMBER ", NULL, on_constant, 0},
{L"constant  :  INT_NUMBER ", NULL, on_constant, 0},
{L"constant  :  STRING ", NULL, on_constant, 0},
{L"constant  :  true ", NULL, on_constant, 0},
{L"constant  :  false ", NULL, on_constant, 0},
{L"constant  :  null ", NULL, on_constant, 0},
{L"call_expression  :  postfix_expression ( expression_list ) ", NULL, on_call_expression, 0},
{L"call_expression  :  postfix_expression ( error ) ", NULL, on_call_expression, 0},
{L"call_expression  :  postfix_expression ( ) ", NULL, on_call_expression, 0},
{L"expression_list  :  expression ", NULL, auto_return_0, 0},
{L"expression_list  :  expression_list , expression ", NULL, on_expression_list, 0}
};

#define __RULE_COUNT__ ((size_t)125)
#define START_RULE L"module"


static lex_t*	__build_lex()													
{																				
		lex_t	*lex;															
		size_t i;																
		lex = Lex_Create();														
		if(lex == NULL)
		{
				return NULL;
		}

		for(i = 0; i < __NAME_COUNT__; ++i)										
		{
				arStatus_t status;
				status = Lex_Insert(lex, __g_lex_name[i]);
				if(status == AR_S_YES)							
				{
				}else if(status == AR_S_NO)
				{
						AR_error(AR_ERR_WARNING, L"failed to build lexer : name '%ls'!\r\n", __g_lex_name);
						Lex_Destroy(lex);										
						AR_ASSERT(false);										
						return NULL;											
				}else
				{																
						AR_error(AR_ERR_FATAL, L"failed to build lexer : name '%ls'!\r\n", __g_lex_name);
						return NULL;
				}																
		}

		for(i = 0; i < __TERM_COUNT__; ++i)										
		{																		
				lexAction_t		act;											
				act.is_skip		=		__g_term_pattern[i].skip;				
				act.priority	=		__g_term_pattern[i].lex_prec;			
				act.value		=		__g_term_pattern[i].tokval;		

				arStatus_t status;
				status = Lex_InsertRule(lex, __g_term_pattern[i].regex, &act);
				if(status == AR_S_YES)							
				{
				}else if(status == AR_S_NO)
				{
						AR_error(AR_ERR_WARNING, L"failed to build lexer : regex '%ls'!\r\n", __g_term_pattern[i].regex);
						Lex_Destroy(lex);										
						AR_ASSERT(false);										
						return NULL;											
				}else
				{																
						AR_error(AR_ERR_FATAL, L"failed to build lexer : regex '%ls'!\r\n", __g_term_pattern[i].regex);
						return NULL;
				}							
		}


		if(Lex_GenerateTransTable(lex) != AR_S_YES)													
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
		if(grammar == NULL)
		{
				return NULL;
		}

		for(i = 0; i < __TERM_COUNT__; ++i)																						
		{		
				arStatus_t status;
				if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)
				{
						continue;										
				}

				status = Parser_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PARSER_ASSOC_NONASSOC,0, __g_term_pattern[i].leaf);

				if(status == AR_S_YES)
				{
				}else if(status == AR_S_NO)	
				{
						AR_error(AR_ERR_WARNING, L"failed to build grammar : term '%ls'!\r\n", __g_term_pattern[i].name);
						Parser_DestroyGrammar(grammar);																			
						grammar = NULL;																							
						AR_ASSERT(false);																						
						return NULL;																							
				}else
				{
						AR_error(AR_ERR_FATAL, L"failed to build grammar : term '%ls'!\r\n", __g_term_pattern[i].name);
						return NULL;																					
				}
		}					

		for(i = 0; i < __PREC_COUNT__; ++i)																						
		{																														
				psrTermInfo_t	*info;																							
				info = Parser_GetTermSymbInfoByName(grammar, __g_prec_pattern[i].name);											
				if(info == NULL)																								
				{						
						arStatus_t status;
						status = Parser_InsertTerm(grammar, __g_prec_pattern[i].name, __g_prec_pattern[i].tokval, __g_prec_pattern[i].assoc, __g_prec_pattern[i].prec_level, NULL);

						if(status == AR_S_YES)
						{

						}else if(status == AR_S_NO)
						{
								AR_error(AR_ERR_WARNING, L"failed to build grammar : prec '%ls'!\r\n", __g_term_pattern[i].name);
								Parser_DestroyGrammar(grammar);																												
								grammar = NULL;																																
								AR_ASSERT(false);																															
								return NULL;																																
						}else
						{
								AR_error(AR_ERR_WARNING, L"failed to build grammar : prec '%ls'!\r\n", __g_term_pattern[i].name);
						}
				}else																																						
				{																																							
						info->assoc = __g_prec_pattern[i].assoc;																											
						info->prec = __g_prec_pattern[i].prec_level;																										
				}																																							
		}		

		for(i = 0; i < __RULE_COUNT__; ++i)																													
		{				
				arStatus_t status;
				status = Parser_InsertRuleByStr(grammar, __g_rule_pattern[i].rule, __g_rule_pattern[i].prec_token, __g_rule_pattern[i].handler, __g_rule_pattern[i].auto_ret);
				
				if(status == AR_S_YES)		
				{
				}else if(status == AR_S_NO)
				{		
						AR_error(AR_ERR_WARNING, L"failed to build grammar : rule '%ls'!\r\n", __g_rule_pattern[i].rule);
						Parser_DestroyGrammar(grammar);																														
						grammar = NULL;																																		
						AR_ASSERT(false);																																	
						return NULL;																																		
				}else
				{
						AR_error(AR_ERR_FATAL, L"failed to build grammar : rule '%ls'!\r\n", __g_rule_pattern[i].rule);
				}
		}				


		if(Parser_SetStartRule(grammar,START_RULE) != AR_S_YES || Parser_CheckIsValidGrammar(grammar, NULL) != AR_S_YES)
		{
				AR_error(AR_ERR_WARNING, L"failed to build grammar!\r\n");
				Parser_DestroyGrammar(grammar);
				grammar = NULL;
				AR_ASSERT(false);
				return NULL;																																				
		}

		return grammar;																																						
}





static psrRetVal_t AR_STDCALL default_leaf_handler(const psrToken_t *tok,void *ctx)
{
	 psrRetVal_t ret = {AR_S_YES, NULL}; return ret;
}


static psrRetVal_t AR_STDCALL on_string_leaf_handler(const psrToken_t *tok,void *ctx)
{
	 psrRetVal_t ret = {AR_S_YES, NULL}; return ret;
}


/*module	:	element_list */
/*module	:	 */
static psrRetVal_t AR_STDCALL on_module(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*element_list	:	element */
/*element_list	:	element_list element */
/*element	:	declaration */
/*element	:	function_defination */
/*declaration	:	var init_declarator_list semi */
/*declaration	:	var error ; */
/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
/*compound_element	:	declaration */
/*expression_statement	:	error ; */
static psrRetVal_t AR_STDCALL auto_return_null(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*element	:	statement */
static psrRetVal_t AR_STDCALL handle_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*function_signature	:	var NAME ( params ) */
static psrRetVal_t AR_STDCALL on_function_signature(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*function_defination	:	function_signature compound_statement */
static psrRetVal_t AR_STDCALL on_function_defination(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*params	:	namelist , ... */
static psrRetVal_t AR_STDCALL on_namelist_ellipsis(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*params	:	namelist */
/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
/*statement	:	empty_statement */
/*statement	:	import_statement */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*semi	:	; */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
/*primary_expression	:	constant */
/*primary_expression	:	aggregate_constructor */
/*expression_list	:	expression */
static psrRetVal_t AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*params	:	... */
static psrRetVal_t AR_STDCALL on_ellipsis(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*namelist	:	namelist , NAME */
static psrRetVal_t AR_STDCALL on_name_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*namelist	:	NAME */
static psrRetVal_t AR_STDCALL on_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*init_declarator	:	NAME = expression */
/*init_declarator	:	NAME */
static psrRetVal_t AR_STDCALL on_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*aggregate_constructor	:	table_constructor */
/*aggregate_constructor	:	list_constructor */
static psrRetVal_t AR_STDCALL handle_aggregate_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*list_constructor	:	list { list_field_list optional_dot } */
/*list_constructor	:	list { } */
/*list_constructor	:	list { error } */
static psrRetVal_t AR_STDCALL handle_list_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*list_field_list	:	list_field_list , expression */
/*list_field_list	:	expression */
static psrRetVal_t AR_STDCALL handle_list_field_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*table_constructor	:	table { table_field_list optional_dot } */
/*table_constructor	:	table { } */
/*table_constructor	:	table { error } */
static psrRetVal_t AR_STDCALL on_table_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*table_field_list	:	table_field_list , table_field */
/*table_field_list	:	table_field */
static psrRetVal_t AR_STDCALL handle_table_field_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*table_field	:	expression : expression */
static psrRetVal_t AR_STDCALL handle_table_field(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*optional_dot	:	, */
/*optional_dot	:	 */
static psrRetVal_t AR_STDCALL handle_optional_dot(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*compound_statement	:	start_block compound_element_list } */
static psrRetVal_t AR_STDCALL on_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*compound_statement	:	start_block } */
static psrRetVal_t AR_STDCALL on_empty_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*compound_statement	:	start_block error } */
static psrRetVal_t AR_STDCALL on_compound_error_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*start_block	:	{ */
static psrRetVal_t AR_STDCALL on_start_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*compound_element_list	:	compound_element_list compound_element */
/*compound_element_list	:	compound_element */
static psrRetVal_t AR_STDCALL handle_compound_element_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*compound_element	:	statement */
static psrRetVal_t AR_STDCALL on_compound_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*import_statement	:	#import NAME ; */
/*import_statement	:	#import NAME from STRING ; */
/*import_statement	:	#import error ; */
static psrRetVal_t AR_STDCALL on_import_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*empty_statement	:	; */
static psrRetVal_t AR_STDCALL on_empty_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*expression_statement	:	expression semi */
static psrRetVal_t AR_STDCALL on_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*if_statement	:	if ( expression ) statement */
/*if_statement	:	if ( error ) statement */
static psrRetVal_t AR_STDCALL on_if_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*if_else_statement	:	if ( expression ) statement else statement */
/*if_else_statement	:	if ( error ) statement else statement */
static psrRetVal_t AR_STDCALL on_if_else_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*while_statement	:	while enter_loop ( expression ) statement leave_loop */
/*while_statement	:	while enter_loop ( error ) statement leave_loop */
static psrRetVal_t AR_STDCALL on_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*do_while_statement	:	do enter_loop statement while ( expression ) leave_loop semi */
/*do_while_statement	:	do enter_loop statement while ( error ) leave_loop semi */
static psrRetVal_t AR_STDCALL on_do_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*enter_loop	:	 */
static psrRetVal_t AR_STDCALL on_enter_loop(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*leave_loop	:	 */
static psrRetVal_t AR_STDCALL on_leave_loop(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*jump_statement	:	continue semi */
static psrRetVal_t AR_STDCALL on_continue_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*jump_statement	:	break semi */
static psrRetVal_t AR_STDCALL on_break_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*jump_statement	:	return semi */
/*jump_statement	:	return expression semi */
static psrRetVal_t AR_STDCALL on_return_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*semi	:	error */
static psrRetVal_t AR_STDCALL on_semi_error(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*assignment_expression	:	unary_expression = assignment_expression */
static psrRetVal_t AR_STDCALL on_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*constant_expression	:	binary_expression ? expression : expression */
static psrRetVal_t AR_STDCALL on_condition_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*binary_expression	:	binary_expression + binary_expression */
/*binary_expression	:	binary_expression - binary_expression */
/*binary_expression	:	binary_expression * binary_expression */
/*binary_expression	:	binary_expression / binary_expression */
/*binary_expression	:	binary_expression % binary_expression */
/*binary_expression	:	binary_expression < binary_expression */
/*binary_expression	:	binary_expression <= binary_expression */
/*binary_expression	:	binary_expression > binary_expression */
/*binary_expression	:	binary_expression >= binary_expression */
/*binary_expression	:	binary_expression == binary_expression */
/*binary_expression	:	binary_expression != binary_expression */
/*binary_expression	:	binary_expression && binary_expression */
/*binary_expression	:	binary_expression || binary_expression */
static psrRetVal_t AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*unary_expression	:	+ unary_expression */
/*unary_expression	:	- unary_expression */
/*unary_expression	:	! unary_expression */
/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
static psrRetVal_t AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
static psrRetVal_t AR_STDCALL on_post_add_minus_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression [ error ] */
static psrRetVal_t AR_STDCALL on_index_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*postfix_expression	:	postfix_expression . NAME */
static psrRetVal_t AR_STDCALL on_access_name_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*primary_expression	:	( expression ) */
/*primary_expression	:	( error ) */
static psrRetVal_t AR_STDCALL on_lp_rp_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*primary_expression	:	NAME */
static psrRetVal_t AR_STDCALL on_identifier_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*constant	:	FLOAT_NUMBER */
/*constant	:	INT_NUMBER */
/*constant	:	STRING */
/*constant	:	true */
/*constant	:	false */
/*constant	:	null */
static psrRetVal_t AR_STDCALL on_constant(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*call_expression	:	postfix_expression ( expression_list ) */
/*call_expression	:	postfix_expression ( error ) */
/*call_expression	:	postfix_expression ( ) */
static psrRetVal_t AR_STDCALL on_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*expression_list	:	expression_list , expression */
static psrRetVal_t AR_STDCALL on_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}







