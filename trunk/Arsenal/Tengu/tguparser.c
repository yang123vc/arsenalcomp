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

#include "tguparser.h"


AR_NAMESPACE_BEGIN

/*******************************************decl****************************************/

struct __parser_node_tag;
typedef struct __parser_node_tag		parserNode_t;


/********************************************************************************************/

typedef enum
{
		TOKEN_T,
}parserNodeType_t;

struct __parser_node_tag
{
		parserNodeType_t		type;

};

static parserNode_t*	TGU_CreateParserNode(parserNodeType_t	type);
static void				TGU_DestroyParserNode(parserNode_t		*node);



/**************************************************************************************************/





enum{
TOK_DELIM_ID = 257,
TOK_NAME = 258,
TOK_STRING = 259,
TOK_FLOAT_NUMBER = 260,
TOK_INT_NUMBER = 261,
TOK_FOR = 262,
TOK_DO = 263,
TOK_WHILE = 264,
TOK_IF = 265,
TOK_ELSE = 266,
TOK_CONTINUE = 267,
TOK_BREAK = 268,
TOK_RETURN = 269,
TOK_NULL = 270,
TOK_IN = 271,
TOK_TRUE = 272,
TOK_FALSE = 273,
TOK_VAR = 274,
TOK_ELLIPSIS = 275,
TOK_INC = 276,
TOK_DEC = 277,
TOK_ANDAND = 278,
TOK_OROR = 279,
TOK_LE = 280,
TOK_GE = 281,
TOK_EQ = 282,
TOK_NE = 283,
TOK_LESS = 284,
TOK_GREATER = 285,
TOK_L_BRACES = 286,
TOK_R_BRACES = 287,
TOK_L_PAREN = 288,
TOK_R_PAREN = 289,
TOK_L_SQUARE = 290,
TOK_R_SQUARE = 291,
TOK_SEMICOLON = 292,
TOK_COMMA = 293,
TOK_ASSIGN = 294,
TOK_ADD = 295,
TOK_SUB = 296,
TOK_MUL = 297,
TOK_DIV = 298,
TOK_MOD = 299,
TOK_NOT = 300,
TOK_DOT = 301,
TOK_COLON = 302,
TOK_QUEST = 303,
};


/*处理词法输入*/
static psrNode_t* AR_STDCALL __build_leaf(const psrToken_t *tok,  void *ctx)
{
		
		
		AR_ASSERT(tok != NULL);

		
		
		return	NULL;
}





static const wchar_t *__g_lex_name[] = {
L"delim = [ \\r\\n\\t]",
L"comment = /\\*([^\\*]|\\*+[^\\*/])*\\*+/",
L"comment_line = //[^\\n]*\\n",
L"skip_lexem = {delim}|{comment}|{comment_line}",
L"digit = [0-9]",
L"number = {digit}+",
L"letter = [A-Z_a-z]",
L"hex_digit = [0-9a-fA-F]",
L"hex_literal = 0(x|X){hex_digit}+",
L"oct_literal = 0[0-7]+",
L"dec_literal = (0|[1-9][0-9]*)",
L"exponet = (e|E)(\\+|\\-)?[0-9]+",
L"float_literal = (((([0-9]\\.[0-9]*)|(\\.[0-9]+)){exponet}?)|([0-9]+{exponet}))",
L"escape_seq = (\\\\(\\x22|\\x27))",
L"string_dq = (\\x22({escape_seq}|[^\\x22])*\\x22)",
L"string_sq = \\x27({escape_seq}|[^\\x27])*\\x27",
L"keyword_lhd = [A-Z_a-z0-9]",
L"float_constant = {float_literal}(?!{keyword_lhd})",
L"hex_constant = {hex_literal}(?!{keyword_lhd})",
L"oct_constant = {oct_literal}(?!{keyword_lhd})",
L"dec_constant = {dec_literal}(?!{keyword_lhd})"
};

#define __NAME_COUNT__ ((size_t)21)






static struct {const wchar_t *name; size_t tokval; size_t lex_prec; const wchar_t *regex; bool_t skip; }__g_term_pattern[] =  {
{NULL, TOK_DELIM_ID,1, L"{skip_lexem}+", true},
{L"NAME", TOK_NAME, 0, L"{letter}({letter}|{digit})*", false},
{L"STRING", TOK_STRING, 0, L"{string_dq}|{string_sq}", false},
{L"FLOAT_NUMBER", TOK_FLOAT_NUMBER, 2, L"{float_constant}", false},
{L"INT_NUMBER", TOK_INT_NUMBER, 2, L"{hex_constant}|{oct_constant}|{dec_constant}", false},
{L"for", TOK_FOR, 1, L"\"for\"(?!{keyword_lhd})", false},
{L"do", TOK_DO, 1, L"\"do\"(?!{keyword_lhd})", false},
{L"while", TOK_WHILE, 1, L"\"while\"(?!{keyword_lhd})", false},
{L"if", TOK_IF, 1, L"\"if\"(?!{keyword_lhd})", false},
{L"else", TOK_ELSE, 1, L"\"else\"(?!{keyword_lhd})", false},
{L"continue", TOK_CONTINUE, 1, L"\"continue\"(?!{keyword_lhd})", false},
{L"break", TOK_BREAK, 1, L"\"break\"(?!{keyword_lhd})", false},
{L"return", TOK_RETURN, 1, L"\"return\"(?!{keyword_lhd})", false},
{L"null", TOK_NULL, 1, L"\"null\"(?!{keyword_lhd})", false},
{L"in", TOK_IN, 1, L"\"in\"(?!{keyword_lhd})", false},
{L"true", TOK_TRUE, 1, L"\"true\"(?!{keyword_lhd})", false},
{L"false", TOK_FALSE, 1, L"\"false\"(?!{keyword_lhd})", false},
{L"var", TOK_VAR, 1, L"\"var\"(?!{keyword_lhd})", false},
{L"...", TOK_ELLIPSIS, 2, L"\"...\"", false},
{L"++", TOK_INC, 1, L"\"++\"", false},
{L"--", TOK_DEC, 1, L"\"--\"", false},
{L"&&", TOK_ANDAND, 1, L"\"&&\"", false},
{L"||", TOK_OROR, 1, L"\"||\"", false},
{L"<=", TOK_LE, 1, L"\"<=\"", false},
{L">=", TOK_GE, 1, L"\">=\"", false},
{L"==", TOK_EQ, 1, L"\"==\"", false},
{L"!=", TOK_NE, 1, L"\"!=\"", false},
{L"<", TOK_LESS, 0, L"\"<\"", false},
{L">", TOK_GREATER, 0, L"\">\"", false},
{L"{", TOK_L_BRACES, 0, L"\"{\"", false},
{L"}", TOK_R_BRACES, 0, L"\"}\"", false},
{L"(", TOK_L_PAREN, 0, L"\"(\"", false},
{L")", TOK_R_PAREN, 0, L"\")\"", false},
{L"[", TOK_L_SQUARE, 0, L"\"[\"", false},
{L"]", TOK_R_SQUARE, 0, L"\"]\"", false},
{L";", TOK_SEMICOLON, 0, L"\";\"", false},
{L",", TOK_COMMA, 0, L"\",\"", false},
{L"=", TOK_ASSIGN, 0, L"\"=\"", false},
{L"+", TOK_ADD, 0, L"\"+\"", false},
{L"-", TOK_SUB, 0, L"\"-\"", false},
{L"*", TOK_MUL, 0, L"\"*\"", false},
{L"/", TOK_DIV, 0, L"\"/\"", false},
{L"%", TOK_MOD, 0, L"\"%\"", false},
{L"!", TOK_NOT, 0, L"\"!\"", false},
{L".", TOK_DOT, 0, L"\".\"", false},
{L":", TOK_COLON, 0, L"\":\"", false},
{L"?", TOK_QUEST, 0, L"\"?\"", false},
{L"EOI", 0, 2, L"$", false}
};

#define __TERM_COUNT__ ((size_t)48)

static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {
{L"?", TOK_QUEST,1, PSR_ASSOC_RIGHT},
{L":", TOK_COLON,1, PSR_ASSOC_RIGHT},
{L"||", TOK_OROR,2, PSR_ASSOC_LEFT},
{L"&&", TOK_ANDAND,3, PSR_ASSOC_LEFT},
{L"==", TOK_EQ,4, PSR_ASSOC_LEFT},
{L"!=", TOK_NE,4, PSR_ASSOC_LEFT},
{L"<", TOK_LESS,4, PSR_ASSOC_LEFT},
{L"<=", TOK_LE,4, PSR_ASSOC_LEFT},
{L">", TOK_GREATER,4, PSR_ASSOC_LEFT},
{L">=", TOK_GE,4, PSR_ASSOC_LEFT},
{L"+", TOK_ADD,5, PSR_ASSOC_LEFT},
{L"-", TOK_SUB,5, PSR_ASSOC_LEFT},
{L"*", TOK_MUL,6, PSR_ASSOC_LEFT},
{L"/", TOK_DIV,6, PSR_ASSOC_LEFT},
{L"%", TOK_MOD,6, PSR_ASSOC_LEFT},
{L"PREC_UNARY", 304,7, PSR_ASSOC_RIGHT},
{L"IF_WITHOUT_ELSE", 305,8, PSR_ASSOC_NONASSOC},
{L"else", TOK_ELSE,9, PSR_ASSOC_NONASSOC}
};

#define __PREC_COUNT__ ((size_t)18)

/*program	:	translation_unit */
/*program	:	 */
static psrNode_t* AR_STDCALL handle_program(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*translation_unit	:	element */
/*translation_unit	:	translation_unit element */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*element	:	statement */
/*element	:	function_defination */
static psrNode_t* AR_STDCALL handle_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_defination	:	var dotname ( params ) compound_statement */
static psrNode_t* AR_STDCALL handle_function_defination(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*dotname	:	NAME */
/*dotname	:	dotname . NAME */
static psrNode_t* AR_STDCALL handle_dotname(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist , ... */
/*params	:	namelist */
/*params	:	... */
/*params	:	 */
static psrNode_t* AR_STDCALL handle_params(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namelist	:	namelist , NAME */
/*namelist	:	NAME */
static psrNode_t* AR_STDCALL handle_namelist(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
/*statement	:	assignment_statement */
/*statement	:	declaration_statement */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration_statement	:	var init_declarator_list semi */
static psrNode_t* AR_STDCALL handle_declaration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL handle_init_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator	:	NAME */
/*init_declarator	:	NAME = initializer */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*initializer	:	expression */
/*initializer	:	table_constructor */
static psrNode_t* AR_STDCALL handle_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*assignment_statement	:	variable = expression semi */
/*assignment_statement	:	variable = table_constructor semi */
static psrNode_t* AR_STDCALL handle_assignment_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	start_block statement_list close_block */
/*compound_statement	:	{ } */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*start_block	:	{ */
static psrNode_t* AR_STDCALL handle_start_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*close_block	:	} */
static psrNode_t* AR_STDCALL handle_close_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement_list	:	statement_list statement */
/*statement_list	:	statement */
static psrNode_t* AR_STDCALL handle_statement_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	semi */
/*expression_statement	:	expression semi */
static psrNode_t* AR_STDCALL handle_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*selection_statement	:	if ( expression ) statement */
/*selection_statement	:	if ( expression ) statement else statement */
static psrNode_t* AR_STDCALL handle_selection_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*iteration_statement	:	while ( expression ) statement */
/*iteration_statement	:	do statement while ( expression ) semi */
/*iteration_statement	:	for ( for_expression ) statement */
static psrNode_t* AR_STDCALL handle_iteration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*for_expression	:	assignment_statement expression_statement expression */
/*for_expression	:	assignment_statement expression_statement */
/*for_expression	:	expression_statement expression_statement expression */
/*for_expression	:	expression_statement expression_statement */
/*for_expression	:	NAME in expression */
static psrNode_t* AR_STDCALL handle_for_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*jump_statement	:	continue semi */
/*jump_statement	:	break semi */
/*jump_statement	:	return semi */
/*jump_statement	:	return expression semi */
static psrNode_t* AR_STDCALL handle_jump_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*semi	:	; */
/*semi	:	error */
static psrNode_t* AR_STDCALL handle_semi(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression	:	expression ? expression : expression */
/*expression	:	expression + expression */
/*expression	:	expression - expression */
/*expression	:	expression * expression */
/*expression	:	expression / expression */
/*expression	:	expression % expression */
/*expression	:	expression < expression */
/*expression	:	expression <= expression */
/*expression	:	expression > expression */
/*expression	:	expression >= expression */
/*expression	:	expression == expression */
/*expression	:	expression != expression */
/*expression	:	expression && expression */
/*expression	:	expression || expression */
/*expression	:	++ postfix_expression */
/*expression	:	-- postfix_expression */
/*expression	:	+ postfix_expression */
/*expression	:	- postfix_expression */
/*expression	:	! postfix_expression */
/*expression	:	postfix_expression */
/*expression	:	FLOAT_NUMBER */
/*expression	:	INT_NUMBER */
/*expression	:	STRING */
/*expression	:	true */
/*expression	:	false */
/*expression	:	null */
static psrNode_t* AR_STDCALL handle_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
/*postfix_expression	:	( expression ) */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	variable */
static psrNode_t* AR_STDCALL handle_postfix_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*call_expression	:	postfix_expression ( expression_list ) */
/*call_expression	:	postfix_expression ( ) */
static psrNode_t* AR_STDCALL handle_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_list	:	expression_list , expression */
/*expression_list	:	expression */
static psrNode_t* AR_STDCALL handle_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*variable	:	NAME */
/*variable	:	variable [ expression ] */
/*variable	:	variable . NAME */
static psrNode_t* AR_STDCALL handle_variable(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*table_constructor	:	{ filed_list } */
/*table_constructor	:	{ } */
static psrNode_t* AR_STDCALL handle_table_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*filed_list	:	filed */
/*filed_list	:	filed_list , filed */
static psrNode_t* AR_STDCALL handle_filed_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*filed	:	expression */
/*filed	:	table_constructor */
/*filed	:	[ expression ] = expression */
/*filed	:	NAME = expression */
/*filed	:	NAME = table_constructor */
static psrNode_t* AR_STDCALL handle_filed(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);




static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"program  :  translation_unit ", NULL, handle_program, 0},
{L"program  :   ", NULL, handle_program, 0},
{L"translation_unit  :  element ", NULL, handle_translation_unit, 0},
{L"translation_unit  :  translation_unit element ", NULL, handle_translation_unit, 0},
{L"element  :  statement ", NULL, handle_element, 0},
{L"element  :  function_defination ", NULL, handle_element, 0},
{L"function_defination  :  var dotname ( params ) compound_statement ", NULL, handle_function_defination, 0},
{L"dotname  :  NAME ", NULL, handle_dotname, 0},
{L"dotname  :  dotname . NAME ", NULL, handle_dotname, 0},
{L"params  :  namelist , ... ", NULL, handle_params, 0},
{L"params  :  namelist ", NULL, handle_params, 0},
{L"params  :  ... ", NULL, handle_params, 0},
{L"params  :   ", NULL, handle_params, 0},
{L"namelist  :  namelist , NAME ", NULL, handle_namelist, 0},
{L"namelist  :  NAME ", NULL, handle_namelist, 0},
{L"statement  :  compound_statement ", NULL, handle_statement, 0},
{L"statement  :  expression_statement ", NULL, handle_statement, 0},
{L"statement  :  selection_statement ", NULL, handle_statement, 0},
{L"statement  :  iteration_statement ", NULL, handle_statement, 0},
{L"statement  :  jump_statement ", NULL, handle_statement, 0},
{L"statement  :  assignment_statement ", NULL, handle_statement, 0},
{L"statement  :  declaration_statement ", NULL, handle_statement, 0},
{L"declaration_statement  :  var init_declarator_list semi ", NULL, handle_declaration_statement, 0},
{L"init_declarator_list  :  init_declarator ", NULL, handle_init_declarator_list, 0},
{L"init_declarator_list  :  init_declarator_list , init_declarator ", NULL, handle_init_declarator_list, 0},
{L"init_declarator  :  NAME ", NULL, handle_init_declarator, 0},
{L"init_declarator  :  NAME = initializer ", NULL, handle_init_declarator, 0},
{L"initializer  :  expression ", NULL, handle_initializer, 0},
{L"initializer  :  table_constructor ", NULL, handle_initializer, 0},
{L"assignment_statement  :  variable = expression semi ", NULL, handle_assignment_statement, 0},
{L"assignment_statement  :  variable = table_constructor semi ", NULL, handle_assignment_statement, 0},
{L"compound_statement  :  start_block statement_list close_block ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { } ", NULL, handle_compound_statement, 0},
{L"start_block  :  { ", NULL, handle_start_block, 0},
{L"close_block  :  } ", NULL, handle_close_block, 0},
{L"statement_list  :  statement_list statement ", NULL, handle_statement_list, 0},
{L"statement_list  :  statement ", NULL, handle_statement_list, 0},
{L"expression_statement  :  semi ", NULL, handle_expression_statement, 0},
{L"expression_statement  :  expression semi ", NULL, handle_expression_statement, 0},
{L"selection_statement  :  if ( expression ) statement ", L"IF_WITHOUT_ELSE", handle_selection_statement, 0},
{L"selection_statement  :  if ( expression ) statement else statement ", NULL, handle_selection_statement, 0},
{L"iteration_statement  :  while ( expression ) statement ", NULL, handle_iteration_statement, 0},
{L"iteration_statement  :  do statement while ( expression ) semi ", NULL, handle_iteration_statement, 0},
{L"iteration_statement  :  for ( for_expression ) statement ", NULL, handle_iteration_statement, 0},
{L"for_expression  :  assignment_statement expression_statement expression ", NULL, handle_for_expression, 0},
{L"for_expression  :  assignment_statement expression_statement ", NULL, handle_for_expression, 0},
{L"for_expression  :  expression_statement expression_statement expression ", NULL, handle_for_expression, 0},
{L"for_expression  :  expression_statement expression_statement ", NULL, handle_for_expression, 0},
{L"for_expression  :  NAME in expression ", NULL, handle_for_expression, 0},
{L"jump_statement  :  continue semi ", NULL, handle_jump_statement, 0},
{L"jump_statement  :  break semi ", NULL, handle_jump_statement, 0},
{L"jump_statement  :  return semi ", NULL, handle_jump_statement, 0},
{L"jump_statement  :  return expression semi ", NULL, handle_jump_statement, 0},
{L"semi  :  ; ", NULL, handle_semi, 0},
{L"semi  :  error ", NULL, handle_semi, 0},
{L"expression  :  expression ? expression : expression ", NULL, handle_expression, 0},
{L"expression  :  expression + expression ", NULL, handle_expression, 0},
{L"expression  :  expression - expression ", NULL, handle_expression, 0},
{L"expression  :  expression * expression ", NULL, handle_expression, 0},
{L"expression  :  expression / expression ", NULL, handle_expression, 0},
{L"expression  :  expression % expression ", NULL, handle_expression, 0},
{L"expression  :  expression < expression ", NULL, handle_expression, 0},
{L"expression  :  expression <= expression ", NULL, handle_expression, 0},
{L"expression  :  expression > expression ", NULL, handle_expression, 0},
{L"expression  :  expression >= expression ", NULL, handle_expression, 0},
{L"expression  :  expression == expression ", NULL, handle_expression, 0},
{L"expression  :  expression != expression ", NULL, handle_expression, 0},
{L"expression  :  expression && expression ", NULL, handle_expression, 0},
{L"expression  :  expression || expression ", NULL, handle_expression, 0},
{L"expression  :  ++ postfix_expression ", L"PREC_UNARY", handle_expression, 0},
{L"expression  :  -- postfix_expression ", L"PREC_UNARY", handle_expression, 0},
{L"expression  :  + postfix_expression ", L"PREC_UNARY", handle_expression, 0},
{L"expression  :  - postfix_expression ", L"PREC_UNARY", handle_expression, 0},
{L"expression  :  ! postfix_expression ", L"PREC_UNARY", handle_expression, 0},
{L"expression  :  postfix_expression ", NULL, handle_expression, 0},
{L"expression  :  FLOAT_NUMBER ", NULL, handle_expression, 0},
{L"expression  :  INT_NUMBER ", NULL, handle_expression, 0},
{L"expression  :  STRING ", NULL, handle_expression, 0},
{L"expression  :  true ", NULL, handle_expression, 0},
{L"expression  :  false ", NULL, handle_expression, 0},
{L"expression  :  null ", NULL, handle_expression, 0},
{L"postfix_expression  :  postfix_expression ++ ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression -- ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  ( expression ) ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  call_expression ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  variable ", NULL, handle_postfix_expression, 0},
{L"call_expression  :  postfix_expression ( expression_list ) ", NULL, handle_call_expression, 0},
{L"call_expression  :  postfix_expression ( ) ", NULL, handle_call_expression, 0},
{L"expression_list  :  expression_list , expression ", NULL, handle_expression_list, 0},
{L"expression_list  :  expression ", NULL, handle_expression_list, 0},
{L"variable  :  NAME ", NULL, handle_variable, 0},
{L"variable  :  variable [ expression ] ", NULL, handle_variable, 0},
{L"variable  :  variable . NAME ", NULL, handle_variable, 0},
{L"table_constructor  :  { filed_list } ", NULL, handle_table_constructor, 0},
{L"table_constructor  :  { } ", NULL, handle_table_constructor, 0},
{L"filed_list  :  filed ", NULL, handle_filed_list, 0},
{L"filed_list  :  filed_list , filed ", NULL, handle_filed_list, 0},
{L"filed  :  expression ", NULL, handle_filed, 0},
{L"filed  :  table_constructor ", NULL, handle_filed, 0},
{L"filed  :  [ expression ] = expression ", NULL, handle_filed, 0},
{L"filed  :  NAME = expression ", NULL, handle_filed, 0},
{L"filed  :  NAME = table_constructor ", NULL, handle_filed, 0}
};

#define __RULE_COUNT__ ((size_t)102)
#define START_RULE L"program"







/*program	:	translation_unit */
/*program	:	 */
static psrNode_t* AR_STDCALL handle_program(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*translation_unit	:	element */
/*translation_unit	:	translation_unit element */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*element	:	statement */
/*element	:	function_defination */
static psrNode_t* AR_STDCALL handle_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*function_defination	:	var dotname ( params ) compound_statement */
static psrNode_t* AR_STDCALL handle_function_defination(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*dotname	:	NAME */
/*dotname	:	dotname . NAME */
static psrNode_t* AR_STDCALL handle_dotname(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*params	:	namelist , ... */
/*params	:	namelist */
/*params	:	... */
/*params	:	 */
static psrNode_t* AR_STDCALL handle_params(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*namelist	:	namelist , NAME */
/*namelist	:	NAME */
static psrNode_t* AR_STDCALL handle_namelist(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
/*statement	:	assignment_statement */
/*statement	:	declaration_statement */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*declaration_statement	:	var init_declarator_list semi */
static psrNode_t* AR_STDCALL handle_declaration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL handle_init_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*init_declarator	:	NAME */
/*init_declarator	:	NAME = initializer */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*initializer	:	expression */
/*initializer	:	table_constructor */
static psrNode_t* AR_STDCALL handle_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*assignment_statement	:	variable = expression semi */
/*assignment_statement	:	variable = table_constructor semi */
static psrNode_t* AR_STDCALL handle_assignment_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*compound_statement	:	start_block statement_list close_block */
/*compound_statement	:	{ } */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*start_block	:	{ */
static psrNode_t* AR_STDCALL handle_start_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*close_block	:	} */
static psrNode_t* AR_STDCALL handle_close_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*statement_list	:	statement_list statement */
/*statement_list	:	statement */
static psrNode_t* AR_STDCALL handle_statement_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*expression_statement	:	semi */
/*expression_statement	:	expression semi */
static psrNode_t* AR_STDCALL handle_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*selection_statement	:	if ( expression ) statement */
/*selection_statement	:	if ( expression ) statement else statement */
static psrNode_t* AR_STDCALL handle_selection_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*iteration_statement	:	while ( expression ) statement */
/*iteration_statement	:	do statement while ( expression ) semi */
/*iteration_statement	:	for ( for_expression ) statement */
static psrNode_t* AR_STDCALL handle_iteration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*for_expression	:	assignment_statement expression_statement expression */
/*for_expression	:	assignment_statement expression_statement */
/*for_expression	:	expression_statement expression_statement expression */
/*for_expression	:	expression_statement expression_statement */
/*for_expression	:	NAME in expression */
static psrNode_t* AR_STDCALL handle_for_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*jump_statement	:	continue semi */
/*jump_statement	:	break semi */
/*jump_statement	:	return semi */
/*jump_statement	:	return expression semi */
static psrNode_t* AR_STDCALL handle_jump_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*semi	:	; */
/*semi	:	error */
static psrNode_t* AR_STDCALL handle_semi(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*expression	:	expression ? expression : expression */
/*expression	:	expression + expression */
/*expression	:	expression - expression */
/*expression	:	expression * expression */
/*expression	:	expression / expression */
/*expression	:	expression % expression */
/*expression	:	expression < expression */
/*expression	:	expression <= expression */
/*expression	:	expression > expression */
/*expression	:	expression >= expression */
/*expression	:	expression == expression */
/*expression	:	expression != expression */
/*expression	:	expression && expression */
/*expression	:	expression || expression */
/*expression	:	++ postfix_expression */
/*expression	:	-- postfix_expression */
/*expression	:	+ postfix_expression */
/*expression	:	- postfix_expression */
/*expression	:	! postfix_expression */
/*expression	:	postfix_expression */
/*expression	:	FLOAT_NUMBER */
/*expression	:	INT_NUMBER */
/*expression	:	STRING */
/*expression	:	true */
/*expression	:	false */
/*expression	:	null */
static psrNode_t* AR_STDCALL handle_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
/*postfix_expression	:	( expression ) */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	variable */
static psrNode_t* AR_STDCALL handle_postfix_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*call_expression	:	postfix_expression ( expression_list ) */
/*call_expression	:	postfix_expression ( ) */
static psrNode_t* AR_STDCALL handle_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*expression_list	:	expression_list , expression */
/*expression_list	:	expression */
static psrNode_t* AR_STDCALL handle_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*variable	:	NAME */
/*variable	:	variable [ expression ] */
/*variable	:	variable . NAME */
static psrNode_t* AR_STDCALL handle_variable(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*table_constructor	:	{ filed_list } */
/*table_constructor	:	{ } */
static psrNode_t* AR_STDCALL handle_table_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*filed_list	:	filed */
/*filed_list	:	filed_list , filed */
static psrNode_t* AR_STDCALL handle_filed_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*filed	:	expression */
/*filed	:	table_constructor */
/*filed	:	[ expression ] = expression */
/*filed	:	NAME = expression */
/*filed	:	NAME = table_constructor */
static psrNode_t* AR_STDCALL handle_filed(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}















/***************************************************************************************************/

static void		AR_STDCALL handle_free_node(psrNode_t *node, void *ctx)
{
		AR_ASSERT(node != NULL);
		
}

static void		AR_STDCALL handle_on_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
{
		AR_ASSERT(ctx != NULL && tok != NULL);
		
		AR_ASSERT(false);
}

static const psrHandler_t		__g_handler = 
{
		handle_on_error,
		handle_free_node
};


/**********************************************************************************************************************************/


static const lex_t*	__build_lexer()
{																				
		lex_t			*lex;
		size_t			i;
		lex = LEX_Create(NULL);
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

		if(!LEX_GenerateTransTable(lex))
		{
				AR_ASSERT(false);
		}
		return lex;
}





static const psrGrammar_t*	__build_grammar()											
{		
		psrGrammar_t	*grammar;																								
		size_t i;
		
		grammar = PSR_CreateGrammar(&__g_handler, NULL);
		for(i = 0; i < __TERM_COUNT__; ++i)
		{																														
				if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)continue;
				if(!PSR_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PSR_ASSOC_NONASSOC,0, __build_leaf))	
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


/************************************************************parser core***********************************/

static			arSpinLock_t	__g_lock;
static	const	lex_t			*__g_lex	 = NULL;
static	const	psrGrammar_t	*__g_grammar = NULL;
static	const	parser_t		*__g_parser = NULL;

static	void	__parser_core_init()
{
		AR_InitSpinLock(&__g_lock);
		__g_lex     = __build_lexer();
		__g_grammar	= __build_grammar();
		__g_parser	= PSR_CreateParser(__g_grammar, PSR_LALR);
}



static	void	__parser_core_uninit()
{
		PSR_DestroyParser(__g_parser);
		PSR_DestroyGrammar((psrGrammar_t*)__g_grammar);
		LEX_Destroy((lex_t*)__g_lex);
		__g_lex	= NULL;
		__g_grammar = NULL;
		__g_parser = NULL;
		AR_UnInitSpinLock(&__g_lock);
}



static lexMatch_t*		__build_match()
{
		lexMatch_t		*match;
		AR_ASSERT(__g_lex != NULL);
		
		AR_LockSpinLock(&__g_lock);
		match = LEX_CreateMatch(__g_lex, NULL);
		AR_UnLockSpinLock(&__g_lock);
		return match;
}

static void			__release_match(lexMatch_t	*match)
{
		AR_ASSERT(match != NULL);
		LEX_DestroyMatch(match);
		
}



static psrContext_t*		__build_parser_context(void *ctx)
{
		psrContext_t	*parser_context;
		AR_ASSERT(__g_parser != NULL && __g_grammar != NULL);
		AR_LockSpinLock(&__g_lock);
		parser_context = PSR_CreateContext(__g_parser, ctx);
		AR_UnLockSpinLock(&__g_lock);
		return parser_context;
}


static void				__release_parser_context(psrContext_t		*parser_context)
{
		AR_ASSERT(__g_parser != NULL && __g_grammar != NULL);
		AR_ASSERT(parser_context != NULL);
		AR_LockSpinLock(&__g_lock);
		PSR_DestroyContext(parser_context);
		AR_UnLockSpinLock(&__g_lock);
}





void	TGU_InitParser()
{
		__parser_core_init();
}

void	TGU_UnInitParser()
{
		__parser_core_uninit();
}


AR_NAMESPACE_END


