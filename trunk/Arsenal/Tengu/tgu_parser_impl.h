

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





	static tguExpr_t*		make_identifier_expression(tguParser_t	*parser, 	tguToken_t *token)
	{
			return NULL;
	}

	static tguExpr_t*		make_constant_expression(tguParser_t	*parser, 		tguToken_t *token)
	{
			return NULL;
	}




	static tguExpr_t*		make_call_expression(tguParser_t	*parser, tguExpr_t *args)
	{
			return NULL;
	}


static const wchar_t *__g_lex_name[] = {
L"delim = [ \\r\\n\\t]",
L"comment = /\\*([^\\*]|\\*+[^\\*/])*\\*+/",
L"comment_line = (//[^\\r\\n]*\\r?(\\n|$))",
L"skip_lexem = {comment_line}|{delim}|{comment}",
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



/*enum{
TOK_DELIM_ID = 257,
TOK_NAME = 258,
TOK_STRING = 259,
TOK_FLOAT_NUMBER = 260,
TOK_INT_NUMBER = 261,
TOK_DO = 262,
TOK_WHILE = 263,
TOK_IF = 264,
TOK_ELSE = 265,
TOK_CONTINUE = 266,
TOK_BREAK = 267,
TOK_RETURN = 268,
TOK_NULL = 269,
TOK_TRUE = 270,
TOK_FALSE = 271,
TOK_VAR = 272,
TOK_ELLIPSIS = 273,
TOK_INC = 274,
TOK_DEC = 275,
TOK_ANDAND = 276,
TOK_OROR = 277,
TOK_LE = 278,
TOK_GE = 279,
TOK_EQ = 280,
TOK_NE = 281,
TOK_LESS = 282,
TOK_GREATER = 283,
TOK_L_BRACES = 284,
TOK_R_BRACES = 285,
TOK_L_PAREN = 286,
TOK_R_PAREN = 287,
TOK_L_SQUARE = 288,
TOK_R_SQUARE = 289,
TOK_SEMICOLON = 290,
TOK_COMMA = 291,
TOK_ASSIGN = 292,
TOK_ADD = 293,
TOK_SUB = 294,
TOK_MUL = 295,
TOK_DIV = 296,
TOK_MOD = 297,
TOK_NOT = 298,
TOK_COLON = 299,
TOK_QUEST = 300,
};*/




static struct {const wchar_t *name; size_t tokval; size_t lex_prec; const wchar_t *regex; bool_t skip; }__g_term_pattern[] =  {
{NULL, TOK_DELIM_ID,1, L"{skip_lexem}", true},
{L"NAME", TOK_NAME, 0, L"{letter}({letter}|{digit})*", false},
{L"STRING", TOK_STRING, 0, L"{string_dq}|{string_sq}", false},
{L"FLOAT_NUMBER", TOK_FLOAT_NUMBER, 2, L"{float_constant}", false},
{L"INT_NUMBER", TOK_INT_NUMBER, 2, L"{hex_constant}|{oct_constant}|{dec_constant}", false},
{L"do", TOK_DO, 1, L"\"do\"(?!{keyword_lhd})", false},
{L"while", TOK_WHILE, 1, L"\"while\"(?!{keyword_lhd})", false},
{L"if", TOK_IF, 1, L"\"if\"(?!{keyword_lhd})", false},
{L"else", TOK_ELSE, 1, L"\"else\"(?!{keyword_lhd})", false},
{L"continue", TOK_CONTINUE, 1, L"\"continue\"(?!{keyword_lhd})", false},
{L"break", TOK_BREAK, 1, L"\"break\"(?!{keyword_lhd})", false},
{L"return", TOK_RETURN, 1, L"\"return\"(?!{keyword_lhd})", false},
{L"null", TOK_NULL, 1, L"\"null\"(?!{keyword_lhd})", false},
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
{L":", TOK_COLON, 0, L"\":\"", false},
{L"?", TOK_QUEST, 0, L"\"?\"", false},
{L"EOI", 0, 2, L"$", false}
};

#define __TERM_COUNT__ ((size_t)45)

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
{L"IF_WITHOUT_ELSE", 301,7, PARSER_ASSOC_NONASSOC},
{L"else", TOK_ELSE,8, PARSER_ASSOC_NONASSOC}
};

#define __PREC_COUNT__ ((size_t)17)

/*program	:	translation_unit */
/*program	:	 */
static psrNode_t* AR_STDCALL handle_program(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*program	:	translation_unit */
/*program	:	 */
static psrNode_t* AR_STDCALL handle_program(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*translation_unit	:	element */
/*translation_unit	:	translation_unit element */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*translation_unit	:	element */
/*translation_unit	:	translation_unit element */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*element	:	declaration */
/*element	:	function_defination */
static psrNode_t* AR_STDCALL handle_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*element	:	declaration */
/*element	:	function_defination */
static psrNode_t* AR_STDCALL handle_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_defination	:	var NAME ( params ) compound_statement */
static psrNode_t* AR_STDCALL handle_function_defination(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist , ... */
/*params	:	namelist */
/*params	:	... */
/*params	:	 */
static psrNode_t* AR_STDCALL handle_params(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist , ... */
/*params	:	namelist */
/*params	:	... */
/*params	:	 */
static psrNode_t* AR_STDCALL handle_params(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist , ... */
/*params	:	namelist */
/*params	:	... */
/*params	:	 */
static psrNode_t* AR_STDCALL handle_params(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist , ... */
/*params	:	namelist */
/*params	:	... */
/*params	:	 */
static psrNode_t* AR_STDCALL handle_params(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namelist	:	namelist , NAME */
/*namelist	:	NAME */
static psrNode_t* AR_STDCALL handle_namelist(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namelist	:	namelist , NAME */
/*namelist	:	NAME */
static psrNode_t* AR_STDCALL handle_namelist(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration	:	var init_declarator_list semi */
static psrNode_t* AR_STDCALL handle_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL handle_init_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL handle_init_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator	:	NAME = expression */
/*init_declarator	:	NAME = table_constructor */
/*init_declarator	:	NAME */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator	:	NAME = expression */
/*init_declarator	:	NAME = table_constructor */
/*init_declarator	:	NAME */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator	:	NAME = expression */
/*init_declarator	:	NAME = table_constructor */
/*init_declarator	:	NAME */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*table_constructor	:	{ filed_list } */
/*table_constructor	:	{ } */
static psrNode_t* AR_STDCALL handle_table_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*table_constructor	:	{ filed_list } */
/*table_constructor	:	{ } */
static psrNode_t* AR_STDCALL handle_table_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*filed_list	:	filed */
/*filed_list	:	filed_list , filed */
static psrNode_t* AR_STDCALL handle_filed_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*filed_list	:	filed */
/*filed_list	:	filed_list , filed */
static psrNode_t* AR_STDCALL handle_filed_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*filed	:	expression */
/*filed	:	table_constructor */
static psrNode_t* AR_STDCALL handle_filed(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*filed	:	expression */
/*filed	:	table_constructor */
static psrNode_t* AR_STDCALL handle_filed(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	start_block compound_element_list close_block */
/*compound_statement	:	{ } */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	start_block compound_element_list close_block */
/*compound_statement	:	{ } */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_element_list	:	compound_element_list compound_element */
/*compound_element_list	:	compound_element */
static psrNode_t* AR_STDCALL handle_compound_element_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_element_list	:	compound_element_list compound_element */
/*compound_element_list	:	compound_element */
static psrNode_t* AR_STDCALL handle_compound_element_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_element	:	statement */
/*compound_element	:	declaration */
static psrNode_t* AR_STDCALL handle_compound_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_element	:	statement */
/*compound_element	:	declaration */
static psrNode_t* AR_STDCALL handle_compound_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*start_block	:	{ */
static psrNode_t* AR_STDCALL handle_start_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*close_block	:	} */
static psrNode_t* AR_STDCALL handle_close_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	semi */
static psrNode_t* AR_STDCALL auto_return_null(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*if_statement	:	if ( expression ) statement */
/*if_statement	:	if ( error ) statement */
static psrNode_t* AR_STDCALL on_if_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*if_statement	:	if ( expression ) statement */
/*if_statement	:	if ( error ) statement */
static psrNode_t* AR_STDCALL on_if_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*if_else_statement	:	if ( expression ) statement else statement */
/*if_else_statement	:	if ( error ) statement else statement */
static psrNode_t* AR_STDCALL on_if_else_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*if_else_statement	:	if ( expression ) statement else statement */
/*if_else_statement	:	if ( error ) statement else statement */
static psrNode_t* AR_STDCALL on_if_else_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*while_statement	:	while enter_loop ( expression ) statement leave_loop */
/*while_statement	:	while enter_loop ( error ) statement leave_loop */
static psrNode_t* AR_STDCALL on_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*while_statement	:	while enter_loop ( expression ) statement leave_loop */
/*while_statement	:	while enter_loop ( error ) statement leave_loop */
static psrNode_t* AR_STDCALL on_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*do_while_statement	:	do enter_loop statement while ( expression ) leave_loop semi */
/*do_while_statement	:	do enter_loop statement while ( error ) leave_loop semi */
static psrNode_t* AR_STDCALL on_do_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*do_while_statement	:	do enter_loop statement while ( expression ) leave_loop semi */
/*do_while_statement	:	do enter_loop statement while ( error ) leave_loop semi */
static psrNode_t* AR_STDCALL on_do_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*enter_loop	:	 */
static psrNode_t* AR_STDCALL on_enter_loop(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*leave_loop	:	 */
static psrNode_t* AR_STDCALL on_leave_loop(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*jump_statement	:	continue semi */
static psrNode_t* AR_STDCALL on_continue_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*jump_statement	:	break semi */
static psrNode_t* AR_STDCALL on_break_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*jump_statement	:	return semi */
/*jump_statement	:	return expression semi */
static psrNode_t* AR_STDCALL on_return_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*jump_statement	:	return semi */
/*jump_statement	:	return expression semi */
static psrNode_t* AR_STDCALL on_return_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*semi	:	; */
/*semi	:	error */
static psrNode_t* AR_STDCALL on_semicolon(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*semi	:	; */
/*semi	:	error */
static psrNode_t* AR_STDCALL on_semicolon(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*assignment_expression	:	unary_expression = table_constructor */
/*assignment_expression	:	unary_expression = assignment_expression */
static psrNode_t* AR_STDCALL on_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*assignment_expression	:	unary_expression = table_constructor */
/*assignment_expression	:	unary_expression = assignment_expression */
static psrNode_t* AR_STDCALL on_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*constant_expression	:	binary_expression ? expression : constant_expression */
static psrNode_t* AR_STDCALL on_condition_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
/*unary_expression	:	+ unary_expression */
/*unary_expression	:	- unary_expression */
/*unary_expression	:	! unary_expression */
static psrNode_t* AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
/*unary_expression	:	+ unary_expression */
/*unary_expression	:	- unary_expression */
/*unary_expression	:	! unary_expression */
static psrNode_t* AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
/*unary_expression	:	+ unary_expression */
/*unary_expression	:	- unary_expression */
/*unary_expression	:	! unary_expression */
static psrNode_t* AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
/*unary_expression	:	+ unary_expression */
/*unary_expression	:	- unary_expression */
/*unary_expression	:	! unary_expression */
static psrNode_t* AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
/*unary_expression	:	+ unary_expression */
/*unary_expression	:	- unary_expression */
/*unary_expression	:	! unary_expression */
static psrNode_t* AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
static psrNode_t* AR_STDCALL on_post_add_minus_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
static psrNode_t* AR_STDCALL on_post_add_minus_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression [ error ] */
static psrNode_t* AR_STDCALL on_index_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression [ error ] */
static psrNode_t* AR_STDCALL on_index_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	( expression ) */
/*primary_expression	:	( error ) */
static psrNode_t* AR_STDCALL on_lp_rp_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	( expression ) */
/*primary_expression	:	( error ) */
static psrNode_t* AR_STDCALL on_lp_rp_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	NAME */
static psrNode_t* AR_STDCALL on_identifier_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	FLOAT_NUMBER */
/*primary_expression	:	INT_NUMBER */
/*primary_expression	:	STRING */
/*primary_expression	:	true */
/*primary_expression	:	false */
/*primary_expression	:	null */
static psrNode_t* AR_STDCALL on_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	FLOAT_NUMBER */
/*primary_expression	:	INT_NUMBER */
/*primary_expression	:	STRING */
/*primary_expression	:	true */
/*primary_expression	:	false */
/*primary_expression	:	null */
static psrNode_t* AR_STDCALL on_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	FLOAT_NUMBER */
/*primary_expression	:	INT_NUMBER */
/*primary_expression	:	STRING */
/*primary_expression	:	true */
/*primary_expression	:	false */
/*primary_expression	:	null */
static psrNode_t* AR_STDCALL on_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	FLOAT_NUMBER */
/*primary_expression	:	INT_NUMBER */
/*primary_expression	:	STRING */
/*primary_expression	:	true */
/*primary_expression	:	false */
/*primary_expression	:	null */
static psrNode_t* AR_STDCALL on_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	FLOAT_NUMBER */
/*primary_expression	:	INT_NUMBER */
/*primary_expression	:	STRING */
/*primary_expression	:	true */
/*primary_expression	:	false */
/*primary_expression	:	null */
static psrNode_t* AR_STDCALL on_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	FLOAT_NUMBER */
/*primary_expression	:	INT_NUMBER */
/*primary_expression	:	STRING */
/*primary_expression	:	true */
/*primary_expression	:	false */
/*primary_expression	:	null */
static psrNode_t* AR_STDCALL on_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*call_expression	:	postfix_expression ( expression_list ) */
/*call_expression	:	postfix_expression ( error ) */
/*call_expression	:	postfix_expression ( ) */
static psrNode_t* AR_STDCALL on_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*call_expression	:	postfix_expression ( expression_list ) */
/*call_expression	:	postfix_expression ( error ) */
/*call_expression	:	postfix_expression ( ) */
static psrNode_t* AR_STDCALL on_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*call_expression	:	postfix_expression ( expression_list ) */
/*call_expression	:	postfix_expression ( error ) */
/*call_expression	:	postfix_expression ( ) */
static psrNode_t* AR_STDCALL on_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_list	:	expression_list , expression */
/*expression_list	:	expression */
static psrNode_t* AR_STDCALL handle_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_list	:	expression_list , expression */
/*expression_list	:	expression */
static psrNode_t* AR_STDCALL handle_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);




static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"program  :  translation_unit ", NULL, handle_program, 0},
{L"program  :   ", NULL, handle_program, 0},
{L"translation_unit  :  element ", NULL, handle_translation_unit, 0},
{L"translation_unit  :  translation_unit element ", NULL, handle_translation_unit, 0},
{L"element  :  declaration ", NULL, handle_element, 0},
{L"element  :  function_defination ", NULL, handle_element, 0},
{L"function_defination  :  var NAME ( params ) compound_statement ", NULL, handle_function_defination, 0},
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
{L"declaration  :  var init_declarator_list semi ", NULL, handle_declaration, 0},
{L"init_declarator_list  :  init_declarator ", NULL, handle_init_declarator_list, 0},
{L"init_declarator_list  :  init_declarator_list , init_declarator ", NULL, handle_init_declarator_list, 0},
{L"init_declarator  :  NAME = expression ", NULL, handle_init_declarator, 0},
{L"init_declarator  :  NAME = table_constructor ", NULL, handle_init_declarator, 0},
{L"init_declarator  :  NAME ", NULL, handle_init_declarator, 0},
{L"table_constructor  :  { filed_list } ", NULL, handle_table_constructor, 0},
{L"table_constructor  :  { } ", NULL, handle_table_constructor, 0},
{L"filed_list  :  filed ", NULL, handle_filed_list, 0},
{L"filed_list  :  filed_list , filed ", NULL, handle_filed_list, 0},
{L"filed  :  expression ", NULL, handle_filed, 0},
{L"filed  :  table_constructor ", NULL, handle_filed, 0},
{L"compound_statement  :  start_block compound_element_list close_block ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { } ", NULL, handle_compound_statement, 0},
{L"compound_element_list  :  compound_element_list compound_element ", NULL, handle_compound_element_list, 0},
{L"compound_element_list  :  compound_element ", NULL, handle_compound_element_list, 0},
{L"compound_element  :  statement ", NULL, handle_compound_element, 0},
{L"compound_element  :  declaration ", NULL, handle_compound_element, 0},
{L"start_block  :  { ", NULL, handle_start_block, 0},
{L"close_block  :  } ", NULL, handle_close_block, 0},
{L"expression_statement  :  expression semi ", NULL, auto_return_0, 0},
{L"expression_statement  :  semi ", NULL, auto_return_null, 0},
{L"if_statement  :  if ( expression ) statement ", L"IF_WITHOUT_ELSE", on_if_statement, 0},
{L"if_statement  :  if ( error ) statement ", L"IF_WITHOUT_ELSE", on_if_statement, 0},
{L"if_else_statement  :  if ( expression ) statement else statement ", NULL, on_if_else_statement, 0},
{L"if_else_statement  :  if ( error ) statement else statement ", NULL, on_if_else_statement, 0},
{L"selection_statement  :  if_statement ", NULL, auto_return_0, 0},
{L"selection_statement  :  if_else_statement ", NULL, auto_return_0, 0},
{L"while_statement  :  while enter_loop ( expression ) statement leave_loop ", NULL, on_while_statement, 0},
{L"while_statement  :  while enter_loop ( error ) statement leave_loop ", NULL, on_while_statement, 0},
{L"do_while_statement  :  do enter_loop statement while ( expression ) leave_loop semi ", NULL, on_do_while_statement, 0},
{L"do_while_statement  :  do enter_loop statement while ( error ) leave_loop semi ", NULL, on_do_while_statement, 0},
{L"iteration_statement  :  while_statement ", NULL, auto_return_0, 0},
{L"iteration_statement  :  do_while_statement ", NULL, auto_return_0, 0},
{L"enter_loop  :   ", NULL, on_enter_loop, 0},
{L"leave_loop  :   ", NULL, on_leave_loop, 0},
{L"jump_statement  :  continue semi ", NULL, on_continue_statement, 0},
{L"jump_statement  :  break semi ", NULL, on_break_statement, 0},
{L"jump_statement  :  return semi ", NULL, on_return_statement, 0},
{L"jump_statement  :  return expression semi ", NULL, on_return_statement, 0},
{L"semi  :  ; ", NULL, on_semicolon, 0},
{L"semi  :  error ", NULL, on_semicolon, 0},
{L"expression  :  assignment_expression ", NULL, auto_return_0, 0},
{L"assignment_expression  :  constant_expression ", NULL, auto_return_0, 0},
{L"assignment_expression  :  unary_expression = table_constructor ", NULL, on_assignment_expression, 0},
{L"assignment_expression  :  unary_expression = assignment_expression ", NULL, on_assignment_expression, 0},
{L"constant_expression  :  binary_expression ", NULL, auto_return_0, 0},
{L"constant_expression  :  binary_expression ? expression : constant_expression ", NULL, on_condition_expression, 0},
{L"binary_expression  :  unary_expression ", NULL, auto_return_0, 0},
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
{L"binary_expression  :  binary_expression + binary_expression ", NULL, on_binary_expression, 0},
{L"unary_expression  :  postfix_expression ", NULL, auto_return_0, 0},
{L"unary_expression  :  ++ unary_expression ", NULL, on_unary_expression, 0},
{L"unary_expression  :  -- unary_expression ", NULL, on_unary_expression, 0},
{L"unary_expression  :  + unary_expression ", NULL, on_unary_expression, 0},
{L"unary_expression  :  - unary_expression ", NULL, on_unary_expression, 0},
{L"unary_expression  :  ! unary_expression ", NULL, on_unary_expression, 0},
{L"postfix_expression  :  postfix_expression ++ ", NULL, on_post_add_minus_expression, 0},
{L"postfix_expression  :  postfix_expression -- ", NULL, on_post_add_minus_expression, 0},
{L"postfix_expression  :  postfix_expression [ expression ] ", NULL, on_index_expression, 0},
{L"postfix_expression  :  postfix_expression [ error ] ", NULL, on_index_expression, 0},
{L"postfix_expression  :  call_expression ", NULL, auto_return_0, 0},
{L"postfix_expression  :  primary_expression ", NULL, auto_return_0, 0},
{L"primary_expression  :  ( expression ) ", NULL, on_lp_rp_expression, 0},
{L"primary_expression  :  ( error ) ", NULL, on_lp_rp_expression, 0},
{L"primary_expression  :  NAME ", NULL, on_identifier_expression, 0},
{L"primary_expression  :  FLOAT_NUMBER ", NULL, on_constant_expression, 0},
{L"primary_expression  :  INT_NUMBER ", NULL, on_constant_expression, 0},
{L"primary_expression  :  STRING ", NULL, on_constant_expression, 0},
{L"primary_expression  :  true ", NULL, on_constant_expression, 0},
{L"primary_expression  :  false ", NULL, on_constant_expression, 0},
{L"primary_expression  :  null ", NULL, on_constant_expression, 0},
{L"call_expression  :  postfix_expression ( expression_list ) ", NULL, on_call_expression, 0},
{L"call_expression  :  postfix_expression ( error ) ", NULL, on_call_expression, 0},
{L"call_expression  :  postfix_expression ( ) ", NULL, on_call_expression, 0},
{L"expression_list  :  expression_list , expression ", NULL, handle_expression_list, 0},
{L"expression_list  :  expression ", NULL, handle_expression_list, 0}
};

#define __RULE_COUNT__ ((size_t)106)
#define START_RULE L"program"

static lex_t*	__build_lex(const arIOCtx_t *io)								
{																				
		lex_t	*lex;															
		size_t i;																
		lex = Lex_Create(io);													
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
		return lex;																
}

static psrGrammar_t*	__build_grammar(const psrHandler_t	*handler, const arIOCtx_t *io)										
{																																
		psrGrammar_t	*grammar;																								
		size_t i;																												
		AR_ASSERT(handler != NULL);																								
		grammar = Parser_CreateGrammar(handler, io);																				
		for(i = 0; i < __TERM_COUNT__; ++i)																						
		{																														
				if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)continue;										
				if(!Parser_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PARSER_ASSOC_NONASSOC,0, NULL))	
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
		if(!Parser_SetFirstRule(grammar,START_RULE) || !Parser_CheckIsValidGrammar(grammar))																						
		{																																									
				Parser_DestroyGrammar(grammar);																																
				grammar = NULL;																																				
				AR_ASSERT(false);																																			
				return NULL;																																				
		}																																									
		return grammar;																																						
}




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




/*element	:	declaration */
/*element	:	function_defination */
static psrNode_t* AR_STDCALL handle_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*function_defination	:	var NAME ( params ) compound_statement */
static psrNode_t* AR_STDCALL handle_function_defination(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
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
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*declaration	:	var init_declarator_list semi */
static psrNode_t* AR_STDCALL handle_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL handle_init_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*init_declarator	:	NAME = expression */
/*init_declarator	:	NAME = table_constructor */
/*init_declarator	:	NAME */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
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
static psrNode_t* AR_STDCALL handle_filed(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*compound_statement	:	start_block compound_element_list close_block */
/*compound_statement	:	{ } */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*compound_element_list	:	compound_element_list compound_element */
/*compound_element_list	:	compound_element */
static psrNode_t* AR_STDCALL handle_compound_element_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*compound_element	:	statement */
/*compound_element	:	declaration */
static psrNode_t* AR_STDCALL handle_compound_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
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




/*expression_statement	:	expression semi */
/*selection_statement	:	if_statement */
/*selection_statement	:	if_else_statement */
/*iteration_statement	:	while_statement */
/*iteration_statement	:	do_while_statement */
/*expression	:	assignment_expression */
/*assignment_expression	:	constant_expression */
/*constant_expression	:	binary_expression */
/*binary_expression	:	unary_expression */
/*unary_expression	:	postfix_expression */
/*postfix_expression	:	call_expression */
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						psrNode_t *ret = nodes[0];
						tguParser_t	*parser = (tguParser_t*)ctx;
						nodes[0] = NULL;
						if(ret == NULL)
						{
							parser->has_error = true;
						}
						return ret;
					}
}




/*expression_statement	:	semi */
static psrNode_t* AR_STDCALL auto_return_null(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						return NULL;
					}
}




/*if_statement	:	if ( expression ) statement */
/*if_statement	:	if ( error ) statement */
static psrNode_t* AR_STDCALL on_if_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguSynNode_t	*ret;
						tguExpr_t		*expr;
						tguStmt_t		*if_true;
						AR_ASSERT(ns != NULL && count == 5);
						if(ns[2] == NULL)
						{
							expr = NULL;
							parser->has_error = true;
						}else
						{
							expr = ns[2]->expr;
							ns[2]->expr = NULL;
						}
							
						if(ns[4] == NULL)
						{	
							if_true = NULL;
							parser->has_error = true;
						}else
						{
							if_true = ns[4]->stmt;
							ns[4]->stmt = NULL;
						}

						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_STMT_T;
						ret->stmt = AR_NEW0(tguStmt_t);								
						ret->stmt->stmt_type = TGU_STT_IF;
						ret->stmt->lex_info = ns[0]->token->lex_info;
						ret->stmt->expr = expr;
						ret->stmt->if_stmt.true_part = if_true;
						ret->stmt->if_stmt.false_part = NULL;
						return ret;
					}
}




/*if_else_statement	:	if ( expression ) statement else statement */
/*if_else_statement	:	if ( error ) statement else statement */
static psrNode_t* AR_STDCALL on_if_else_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguSynNode_t	*ret;
						tguExpr_t		*expr;
						tguStmt_t		*if_true;
						tguStmt_t		*if_false;
						AR_ASSERT(ns != NULL && count == 5);
						if(ns[2] == NULL)
						{
							expr = NULL;
							parser->has_error = true;
						}else
						{
							expr = ns[2]->expr;
							ns[2]->expr = NULL;
						}
							
						if(ns[4] == NULL)
						{	
							if_true = NULL;
							parser->has_error = true;
						}else
						{
							if_true = ns[4]->stmt;
							ns[4]->stmt = NULL;
						}

						if(ns[6] == NULL)
						{	
							if_false = NULL;
							parser->has_error = true;
						}else
						{
							if_false = ns[6]->stmt;
							ns[6]->stmt = NULL;
						}


						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_STMT_T;
						ret->stmt = AR_NEW0(tguStmt_t);								
						ret->stmt->stmt_type = TGU_STT_IF;
						ret->stmt->lex_info = ns[0]->token->lex_info;
						ret->stmt->expr = expr;
						ret->stmt->if_stmt.true_part = if_true;
						ret->stmt->if_stmt.false_part = if_false;
						return ret;
					}
}




/*while_statement	:	while enter_loop ( expression ) statement leave_loop */
/*while_statement	:	while enter_loop ( error ) statement leave_loop */
static psrNode_t* AR_STDCALL on_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguSynNode_t	*ret;
						tguExpr_t		*expr;
						tguStmt_t		*stmt;
						AR_ASSERT(ns != NULL && count == 5);
						if(ns[3] == NULL)
						{
							expr = NULL;
							parser->has_error = true;
						}else
						{
							expr = ns[3]->expr;
							ns[3]->expr = NULL;
						}
							
						if(ns[5] == NULL)
						{	
							stmt = NULL;
							parser->has_error = true;
						}else
						{
							stmt = ns[5]->stmt;
							ns[5]->stmt = NULL;
						}

						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_STMT_T;
						ret->stmt = AR_NEW0(tguStmt_t);								
						ret->stmt->stmt_type = TGU_STT_WHILE;
						ret->stmt->lex_info = ns[0]->token->lex_info;
						ret->stmt->while_stmt.expr = expr;
						ret->stmt->while_stmt.loop_part = stmt;
						return ret;
					}
}




/*do_while_statement	:	do enter_loop statement while ( expression ) leave_loop semi */
/*do_while_statement	:	do enter_loop statement while ( error ) leave_loop semi */
static psrNode_t* AR_STDCALL on_do_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguSynNode_t	*ret;
						tguExpr_t		*expr;
						tguStmt_t		*stmt;
						AR_ASSERT(ns != NULL && count == 5);
						if(ns[2] == NULL)
						{
							stmt = NULL;
							parser->has_error = true;
						}else
						{
							stmt = ns[2]->stmt;
							ns[2]->stmt = NULL;
						}
							
						if(ns[5] == NULL)
						{	
							expr = NULL;
							parser->has_error = true;
						}else
						{
							expr = ns[5]->expr;
							ns[5]->expr = NULL;
						}

						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_STMT_T;
						ret->stmt = AR_NEW0(tguStmt_t);								
						ret->stmt->stmt_type = TGU_STT_DO;
						ret->stmt->lex_info = ns[0]->token->lex_info;
						ret->stmt->while_stmt.expr = expr;
						ret->stmt->while_stmt.loop_part = stmt;
						return ret;
					}
}




/*enter_loop	:	 */
static psrNode_t* AR_STDCALL on_enter_loop(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t	*parser = NULL;
						AR_ASSERT(ctx != NULL);
						parser = (tguParser_t*)ctx;
						parser->loop_level++;
						return NULL;
					}
}




/*leave_loop	:	 */
static psrNode_t* AR_STDCALL on_leave_loop(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t	*parser = NULL;
						AR_ASSERT(ctx != NULL);
						parser = (tguParser_t*)ctx;
						AR_ASSERT(parser->loop_level > 0);
						parser->loop_level--;
						return NULL;
					}
}




/*jump_statement	:	continue semi */
static psrNode_t* AR_STDCALL on_continue_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						AR_ASSERT(ns != NULL);
						if(parser->loop_level == 0)
						{
							tguReportInfo_t	info;
							info.type = TGU_REPORT_ERROR_SYNTAX_T;
							info.syntax_error.tok = ns[0]->token;
							info.syntax_error.msg = L"illegal continue";
							AR_ASSERT(parser->report.report_func != NULL);
							parser->report.report_func(&info, parser->report.report_ctx);
							parser->has_error = true;
						}
						
						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_STMT_T;
						ret->stmt = AR_NEW0(tguStmt_t);								
						ret->stmt->stmt_type = TGU_STT_CONTINUE;
						ret->stmt->lex_info = ns[0]->token->lex_info;
						return ret;

					}
}




/*jump_statement	:	break semi */
static psrNode_t* AR_STDCALL on_break_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						AR_ASSERT(ns != NULL);
						if(parser->loop_level == 0)
						{
							tguReportInfo_t	info;
							info.type = TGU_REPORT_ERROR_SYNTAX_T;
							info.syntax_error.tok = ns[0]->token;
							info.syntax_error.msg = L"illegal break";
							AR_ASSERT(parser->report.report_func != NULL);
							parser->report.report_func(&info, parser->report.report_ctx);
							parser->has_error = true;
						}
						
						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_STMT_T;
						ret->stmt = AR_NEW0(tguStmt_t);								
						ret->stmt->stmt_type = TGU_STT_BREAK;
						ret->stmt->lex_info = ns[0]->token->lex_info;
						return ret;
						
					}
}




/*jump_statement	:	return semi */
/*jump_statement	:	return expression semi */
static psrNode_t* AR_STDCALL on_return_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguExpr_t		*expr;
						AR_ASSERT(nodes != NULL && count == 2 || count == 3);
						AR_ASSERT(parser->current_function != NULL);

						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_STMT_T;
						ret->stmt = AR_NEW0(tguStmt_t);								
						ret->stmt->stmt_type = TGU_STT_RETURN;
						
						if(ns[1] == NULL)
						{
							expr = NULL;
						}else
						{
							AR_ASSERT(ns[1]->type == TGU_NODE_EXPR_T);
							expr = ns[1]->expr;
							ns[1]->expr = NULL;
						}
						ret->stmt->return_stmt.expr = expr;
						ret->stmt->lex_info = ns[0]->token->lex_info;
						return ret;
					}
}




/*semi	:	; */
/*semi	:	error */
static psrNode_t* AR_STDCALL on_semicolon(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguParser_t	*parser = (tguParser_t*)ctx;
						AR_ASSERT(count == 1 && parser != NULL);
						if(ns[0] == NULL)
						{
							parser->has_error = true;
						}

						return NULL;
					}
}




/*assignment_expression	:	unary_expression = table_constructor */
/*assignment_expression	:	unary_expression = assignment_expression */
static psrNode_t* AR_STDCALL on_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
							return NULL;
					}
}




/*constant_expression	:	binary_expression ? expression : constant_expression */
static psrNode_t* AR_STDCALL on_condition_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




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
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						return NULL;
					}
}




/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
/*unary_expression	:	+ unary_expression */
/*unary_expression	:	- unary_expression */
/*unary_expression	:	! unary_expression */
static psrNode_t* AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
					return NULL;
				}
}




/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
static psrNode_t* AR_STDCALL on_post_add_minus_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
					return NULL;
				}
}




/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression [ error ] */
static psrNode_t* AR_STDCALL on_index_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
					return NULL;
				}
}




/*primary_expression	:	( expression ) */
/*primary_expression	:	( error ) */
static psrNode_t* AR_STDCALL on_lp_rp_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
					tguParser_t	*parser = (tguParser_t*)ctx;
					psrNode_t 	*ret;
					if(nodes[1] == NULL)
					{
						parser->has_error = true;
						ret = NULL;
					}else
					{
						ret = nodes[1];
						nodes[1] = NULL;
					}
					return ret;
				}
}




/*primary_expression	:	NAME */
static psrNode_t* AR_STDCALL on_identifier_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguExpr_t		*expr;
						AR_ASSERT(parser != NULL && ns != NULL && count == 1);
						expr = make_identifier_expression(parser, ns[0]->token);
						
						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_EXPR_T;
						ret->expr = expr;
						return ret;

				}
}




/*primary_expression	:	FLOAT_NUMBER */
/*primary_expression	:	INT_NUMBER */
/*primary_expression	:	STRING */
/*primary_expression	:	true */
/*primary_expression	:	false */
/*primary_expression	:	null */
static psrNode_t* AR_STDCALL on_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguExpr_t		*expr;
						AR_ASSERT(parser != NULL && ns != NULL && count == 1);
						expr = make_constant_expression(parser, ns[0]->token);
						
						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_EXPR_T;
						ret->expr = expr;
						return ret;
				}
}




/*call_expression	:	postfix_expression ( expression_list ) */
/*call_expression	:	postfix_expression ( error ) */
/*call_expression	:	postfix_expression ( ) */
static psrNode_t* AR_STDCALL on_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguExpr_t		*expr, *args;
						if(count == 4)
						{
							if(ns[2] == NULL)
							{
								args = NULL;
								parser->has_error = true;
							}else
							{
								args = ns[2]->expr;
								ns[2]->expr = NULL;
							}
						}else
						{
							args = NULL;
						}
						AR_ASSERT(parser != NULL && ns != NULL && count == 1);
						
						expr = make_call_expression(parser, args);
						
						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_EXPR_T;
						ret->expr = expr;
						return ret;
				}
}




/*expression_list	:	expression_list , expression */
/*expression_list	:	expression */
static psrNode_t* AR_STDCALL handle_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




