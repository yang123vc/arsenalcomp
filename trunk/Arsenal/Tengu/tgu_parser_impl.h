

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
params
*/


	typedef struct __tengu_params_tag
	{		
		const wchar_t		**names;
		size_t			count;
		size_t			cap;
		bool_t			is_variadic;
	}tguParams_t;

	static void	init_params(tguParams_t *params)
	{	
		AR_ASSERT(params != NULL);
		AR_memset(params, 0, sizeof(*params));
		params->is_variadic = false;
	}
	
	static void	clear_params(tguParams_t *params)
	{
		AR_ASSERT(params != NULL);
		params->count = 0;
		params->is_variadic = false;
	}

	static void	uninit_params(tguParams_t *params)
	{
		AR_ASSERT(params != NULL);
		clear_params(params);
		if(params->names)AR_DEL(params);
		AR_memset(params, 0, sizeof(*params));
	}

	void	insert_to_params(tguParams_t *params, const wchar_t *name)
	{
		AR_ASSERT(params != NULL && name != NULL);

		if(params->count == params->cap)
		{
			params->cap += 4;
			params->names = AR_REALLOC(const wchar_t*, (void*)params->names, params->cap);
		}
		params->names[params->count] = TGU_AllocString(name);
		params->count++;
	}


			



/*
syntax_node
*/

	typedef enum
	{
		TGU_NODE_TOKEN_T,
		TGU_NODE_STMT_T,
		TGU_NODE_EXPR_T,
		TGU_NODE_DECL_T,
		TGU_NODE_TABLE_FILED_T,
		TGU_NODE_TABLE_INIT_T,
		TGU_NODE_PARAMS_T,
	}tguSynNodeType_t;

	typedef struct __tengu_syntax_node_tag
	{
		tguSynNodeType_t		type;
		
		union{
				tguToken_t			*token;
				tguStmt_t			*stmt;
				tguExpr_t			*expr;
				tguDeclaration_t	*decl;
				tguTableField_t		*field;
				tguTableInit_t		*tbl_init;
				tguParams_t		*params;
		};
	}tguSynNode_t;


	static tguSynNode_t*	__create_synnode(tguSynNodeType_t type, void *parser_data)
	{
		
		return NULL;
	}

	static void	__delete_synnode(	tguSynNode_t* node)
	{
		AR_ASSERT(node != NULL);
	}



/*
syntax_tree_handler
*/


	static void		AR_STDCALL on_free_node(psrNode_t *node, void *ctx)
	{
		
	}

	static void		AR_STDCALL on_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
	{

	}
	
	static psrHandler_t	__g_handler = {on_error, on_free_node};



	
	tguSynNode_t*	on_lexer_node(tguParser_t *parser, const psrToken_t *tok)
	{
		return NULL;
	}




	
	static tguDeclaration_t*	make_declaration_from_expression(tguParser_t 	*parser, const tguToken_t *tok, tguExpr_t *expr)
	{
		return NULL;
	}

	static tguDeclaration_t*	make_declaration_from_table(tguParser_t 	*parser, const tguToken_t *tok, tguTableInit_t *table_init)
	{
		return NULL;
	}

	static tguDeclaration_t*	make_declaration_from_null(tguParser_t 	*parser, const tguToken_t *tok)
	{
		return NULL;
	}

	static tguDeclaration_t*	make_declaration_from_error(tguParser_t 	*parser, const tguToken_t *tok)
	{
		return NULL;
	}



	static tguTableInit_t*	make_table_init(tguParser_t 	*parser, bool_t is_empty, tguTableField_t	*field_lst)
	{
		return NULL;
	}




	static tguTableField_t*	make_table_field(tguParser_t *parser, tguTableFieldType_t type, void *expr_or_field)
	{
		return NULL;
	}




	static void insert_decl_to_block(tguBlock_t *block, tguDeclaration_t *decl)
	{
		
	}

	static void insert_stmt_to_block(tguBlock_t *block, tguStmt_t *stmt)
	{
		
	}



	static void start_block(tguParser_t *parser, const tguLexInfo_t *info)
	{
	}

	static void close_block(tguParser_t *parser, const tguLexInfo_t *info)
	{

	}



	static tguStmt_t*	make_empty_statement(tguParser_t *parser, const tguLexInfo_t *lex_info)
	{
		return NULL;
	}




	static tguStmt_t*	make_if_statement(tguParser_t *parser, tguExpr_t *cond, tguStmt_t *if_true, tguStmt_t *if_false, const tguLexInfo_t *lex_info)
	{
		return NULL;
	}




	static tguStmt_t*	make_while_statement(tguParser_t *parser, tguExpr_t *cond, tguStmt_t *loop, const tguLexInfo_t *lex_info)
	{
		return NULL;
	}

	static tguStmt_t*	make_do_while_statement(tguParser_t *parser, tguExpr_t *cond, tguStmt_t *loop, const tguLexInfo_t *lex_info)
	{
		return NULL;
	}





	static tguStmt_t*	make_jump_statement(tguParser_t *parser, tguStmtType_t type, tguExpr_t *expr, const tguLexInfo_t *lex_info)
	{
		return NULL;
	}



	static bool_t check_is_lvalue(const tguExpr_t *expr, tguParser_t *parser)
	{
		return false;
	}

	static bool_t check_is_constant(const tguExpr_t *expr, tguParser_t *parser)
	{
		return false;
	}



	static tguExpr_t*		make_assignment_expression(tguParser_t	*parser, 	tguExpr_t *addr, tguExpr_t *value, const tguLexInfo_t *lex_info)
	{
			return NULL;
	}




	static tguExpr_t*		make_condition_expression(tguParser_t	*parser, 	tguExpr_t *cond, tguExpr_t *if_true, tguExpr_t *if_false, const tguLexInfo_t *lex_info)
	{
			return NULL;
	}




	static tguExpr_t*		make_binary_expression(tguParser_t	*parser, 	tguExprOP_t op, tguExpr_t *left, tguExpr_t *right, const tguLexInfo_t *lex_info)
	{
			return NULL;
	}



	static tguExpr_t*		make_unary_expression(tguParser_t	*parser, 	tguExprOP_t op, tguExpr_t *expr, bool_t is_lvalue, const tguLexInfo_t *lex_info)
	{
			return NULL;
	}
	
	static tguExpr_t*	make_index_expression(tguParser_t *parser, tguExpr_t *expr, tguExpr_t *index_expr, const tguLexInfo_t *lex_info)
	{	
		return NULL;
	}




	static tguExpr_t*		make_identifier_expression(tguParser_t	*parser, 	tguToken_t *token)
	{
			return NULL;
	}

	static tguExpr_t*		make_constant_expression(tguParser_t	*parser, 		tguToken_t *token)
	{
			return NULL;
	}




	static tguExpr_t*		make_call_expression(tguParser_t	*parser, tguExpr_t *call_expr, tguExpr_t *args, const tguLexInfo_t *lex_info)
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

static psrNode_t* AR_STDCALL build_default_leaf(const psrToken_t *tok,void *ctx);




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




static struct {const wchar_t *name;
size_t tokval;
size_t lex_prec;
const wchar_t *regex;
bool_t skip;
psrTermFunc_t leaf;
}__g_term_pattern[] =  {
{NULL, TOK_DELIM_ID,1, L"{skip_lexem}", true, NULL},
{L"NAME", TOK_NAME, 0, L"{letter}({letter}|{digit})*", false, build_default_leaf},
{L"STRING", TOK_STRING, 0, L"{string_dq}|{string_sq}", false, build_default_leaf},
{L"FLOAT_NUMBER", TOK_FLOAT_NUMBER, 2, L"{float_constant}", false, build_default_leaf},
{L"INT_NUMBER", TOK_INT_NUMBER, 2, L"{hex_constant}|{oct_constant}|{dec_constant}", false, build_default_leaf},
{L"do", TOK_DO, 1, L"\"do\"(?!{keyword_lhd})", false, build_default_leaf},
{L"while", TOK_WHILE, 1, L"\"while\"(?!{keyword_lhd})", false, build_default_leaf},
{L"if", TOK_IF, 1, L"\"if\"(?!{keyword_lhd})", false, build_default_leaf},
{L"else", TOK_ELSE, 1, L"\"else\"(?!{keyword_lhd})", false, build_default_leaf},
{L"continue", TOK_CONTINUE, 1, L"\"continue\"(?!{keyword_lhd})", false, build_default_leaf},
{L"break", TOK_BREAK, 1, L"\"break\"(?!{keyword_lhd})", false, build_default_leaf},
{L"return", TOK_RETURN, 1, L"\"return\"(?!{keyword_lhd})", false, build_default_leaf},
{L"null", TOK_NULL, 1, L"\"null\"(?!{keyword_lhd})", false, build_default_leaf},
{L"true", TOK_TRUE, 1, L"\"true\"(?!{keyword_lhd})", false, build_default_leaf},
{L"false", TOK_FALSE, 1, L"\"false\"(?!{keyword_lhd})", false, build_default_leaf},
{L"var", TOK_VAR, 1, L"\"var\"(?!{keyword_lhd})", false, build_default_leaf},
{L"...", TOK_ELLIPSIS, 2, L"\"...\"", false, build_default_leaf},
{L"++", TOK_INC, 1, L"\"++\"", false, build_default_leaf},
{L"--", TOK_DEC, 1, L"\"--\"", false, build_default_leaf},
{L"&&", TOK_ANDAND, 1, L"\"&&\"", false, build_default_leaf},
{L"||", TOK_OROR, 1, L"\"||\"", false, build_default_leaf},
{L"<=", TOK_LE, 1, L"\"<=\"", false, build_default_leaf},
{L">=", TOK_GE, 1, L"\">=\"", false, build_default_leaf},
{L"==", TOK_EQ, 1, L"\"==\"", false, build_default_leaf},
{L"!=", TOK_NE, 1, L"\"!=\"", false, build_default_leaf},
{L"<", TOK_LESS, 0, L"\"<\"", false, build_default_leaf},
{L">", TOK_GREATER, 0, L"\">\"", false, build_default_leaf},
{L"{", TOK_L_BRACES, 0, L"\"{\"", false, build_default_leaf},
{L"}", TOK_R_BRACES, 0, L"\"}\"", false, build_default_leaf},
{L"(", TOK_L_PAREN, 0, L"\"(\"", false, build_default_leaf},
{L")", TOK_R_PAREN, 0, L"\")\"", false, build_default_leaf},
{L"[", TOK_L_SQUARE, 0, L"\"[\"", false, build_default_leaf},
{L"]", TOK_R_SQUARE, 0, L"\"]\"", false, build_default_leaf},
{L";", TOK_SEMICOLON, 0, L"\";\"", false, build_default_leaf},
{L",", TOK_COMMA, 0, L"\",\"", false, build_default_leaf},
{L"=", TOK_ASSIGN, 0, L"\"=\"", false, build_default_leaf},
{L"+", TOK_ADD, 0, L"\"+\"", false, build_default_leaf},
{L"-", TOK_SUB, 0, L"\"-\"", false, build_default_leaf},
{L"*", TOK_MUL, 0, L"\"*\"", false, build_default_leaf},
{L"/", TOK_DIV, 0, L"\"/\"", false, build_default_leaf},
{L"%", TOK_MOD, 0, L"\"%\"", false, build_default_leaf},
{L"!", TOK_NOT, 0, L"\"!\"", false, build_default_leaf},
{L":", TOK_COLON, 0, L"\":\"", false, build_default_leaf},
{L"?", TOK_QUEST, 0, L"\"?\"", false, build_default_leaf},
{L"EOI", 0, 2, L"$", false, NULL}
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

/*translation_unit	:	element */
/*translation_unit	:	translation_unit element */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*element	:	declaration */
/*element	:	function_defination */
static psrNode_t* AR_STDCALL handle_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_defination	:	var NAME ( params ) start_function compound_statement close_function */
static psrNode_t* AR_STDCALL on_function_defination(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist , ... */
static psrNode_t* AR_STDCALL on_namelist_ellipsis(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist */
/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
/*statement	:	empty_statement */
/*init_declarator_list	:	init_declarator */
/*filed_list	:	filed */
/*compound_element	:	statement */
/*compound_element	:	declaration */
/*expression_statement	:	expression semi */
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
/*expression_list	:	expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	... */
static psrNode_t* AR_STDCALL on_ellipsis(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namelist	:	namelist , NAME */
static psrNode_t* AR_STDCALL on_name_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namelist	:	NAME */
static psrNode_t* AR_STDCALL on_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*start_function	:	 */
static psrNode_t* AR_STDCALL on_start_function(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*close_function	:	 */
static psrNode_t* AR_STDCALL on_close_function(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration	:	var init_declarator_list semi */
static psrNode_t* AR_STDCALL auto_return_1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL on_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator	:	NAME = expression */
/*init_declarator	:	NAME = table_constructor */
/*init_declarator	:	NAME */
static psrNode_t* AR_STDCALL on_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*table_constructor	:	{ filed_list } */
/*table_constructor	:	{ } */
/*table_constructor	:	{ error } */
static psrNode_t* AR_STDCALL on_table_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*filed_list	:	filed_list , filed */
static psrNode_t* AR_STDCALL on_filed_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*filed	:	expression */
/*filed	:	table_constructor */
static psrNode_t* AR_STDCALL on_filed(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	start_block compound_element_list close_block */
/*compound_statement	:	{ } */
/*compound_statement	:	start_block error close_block */
static psrNode_t* AR_STDCALL on_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_element_list	:	compound_element_list compound_element */
/*compound_element_list	:	compound_element */
static psrNode_t* AR_STDCALL on_compound_element_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*start_block	:	{ */
static psrNode_t* AR_STDCALL on_start_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*close_block	:	} */
static psrNode_t* AR_STDCALL on_close_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*empty_statement	:	; */
static psrNode_t* AR_STDCALL on_empty_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*if_statement	:	if ( expression ) statement */
/*if_statement	:	if ( error ) statement */
static psrNode_t* AR_STDCALL on_if_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*if_else_statement	:	if ( expression ) statement else statement */
/*if_else_statement	:	if ( error ) statement else statement */
static psrNode_t* AR_STDCALL on_if_else_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*while_statement	:	while enter_loop ( expression ) statement leave_loop */
/*while_statement	:	while enter_loop ( error ) statement leave_loop */
static psrNode_t* AR_STDCALL on_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*do_while_statement	:	do enter_loop statement while ( expression ) leave_loop semi */
/*do_while_statement	:	do enter_loop statement while ( error ) leave_loop semi */
static psrNode_t* AR_STDCALL on_do_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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

/*semi	:	error */
static psrNode_t* AR_STDCALL auto_return_null(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*assignment_expression	:	unary_expression = table_constructor */
/*assignment_expression	:	unary_expression = assignment_expression */
static psrNode_t* AR_STDCALL on_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*constant_expression	:	binary_expression ? expression : constant_expression */
static psrNode_t* AR_STDCALL on_condition_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_expression	:	+ unary_expression */
/*unary_expression	:	- unary_expression */
/*unary_expression	:	! unary_expression */
/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
static psrNode_t* AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
static psrNode_t* AR_STDCALL on_post_add_minus_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression [ error ] */
static psrNode_t* AR_STDCALL on_index_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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

/*call_expression	:	postfix_expression ( expression_list ) */
/*call_expression	:	postfix_expression ( error ) */
/*call_expression	:	postfix_expression ( ) */
static psrNode_t* AR_STDCALL on_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_list	:	expression_list , expression */
static psrNode_t* AR_STDCALL on_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);




static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"program  :  translation_unit ", NULL, handle_program, 0},
{L"program  :   ", NULL, handle_program, 0},
{L"translation_unit  :  element ", NULL, handle_translation_unit, 0},
{L"translation_unit  :  translation_unit element ", NULL, handle_translation_unit, 0},
{L"element  :  declaration ", NULL, handle_element, 0},
{L"element  :  function_defination ", NULL, handle_element, 0},
{L"function_defination  :  var NAME ( params ) start_function compound_statement close_function ", NULL, on_function_defination, 0},
{L"params  :  namelist , ... ", NULL, on_namelist_ellipsis, 0},
{L"params  :  namelist ", NULL, auto_return_0, 0},
{L"params  :  ... ", NULL, on_ellipsis, 0},
{L"params  :   ", NULL, NULL, 0},
{L"namelist  :  namelist , NAME ", NULL, on_name_list, 0},
{L"namelist  :  NAME ", NULL, on_name, 0},
{L"start_function  :   ", NULL, on_start_function, 0},
{L"close_function  :   ", NULL, on_close_function, 0},
{L"statement  :  compound_statement ", NULL, auto_return_0, 0},
{L"statement  :  expression_statement ", NULL, auto_return_0, 0},
{L"statement  :  selection_statement ", NULL, auto_return_0, 0},
{L"statement  :  iteration_statement ", NULL, auto_return_0, 0},
{L"statement  :  jump_statement ", NULL, auto_return_0, 0},
{L"statement  :  empty_statement ", NULL, auto_return_0, 0},
{L"declaration  :  var init_declarator_list semi ", NULL, auto_return_1, 0},
{L"init_declarator_list  :  init_declarator ", NULL, auto_return_0, 0},
{L"init_declarator_list  :  init_declarator_list , init_declarator ", NULL, on_declarator_list, 0},
{L"init_declarator  :  NAME = expression ", NULL, on_declarator, 0},
{L"init_declarator  :  NAME = table_constructor ", NULL, on_declarator, 0},
{L"init_declarator  :  NAME ", NULL, on_declarator, 0},
{L"table_constructor  :  { filed_list } ", NULL, on_table_constructor, 0},
{L"table_constructor  :  { } ", NULL, on_table_constructor, 0},
{L"table_constructor  :  { error } ", NULL, on_table_constructor, 0},
{L"filed_list  :  filed_list , filed ", NULL, on_filed_list, 0},
{L"filed_list  :  filed ", NULL, auto_return_0, 0},
{L"filed  :  expression ", NULL, on_filed, 0},
{L"filed  :  table_constructor ", NULL, on_filed, 0},
{L"compound_statement  :  start_block compound_element_list close_block ", NULL, on_compound_statement, 0},
{L"compound_statement  :  { } ", NULL, on_compound_statement, 0},
{L"compound_statement  :  start_block error close_block ", NULL, on_compound_statement, 0},
{L"compound_element_list  :  compound_element_list compound_element ", NULL, on_compound_element_list, 0},
{L"compound_element_list  :  compound_element ", NULL, on_compound_element_list, 0},
{L"compound_element  :  statement ", NULL, auto_return_0, 0},
{L"compound_element  :  declaration ", NULL, auto_return_0, 0},
{L"start_block  :  { ", NULL, on_start_block, 0},
{L"close_block  :  } ", NULL, on_close_block, 0},
{L"empty_statement  :  ; ", NULL, on_empty_statement, 0},
{L"expression_statement  :  expression semi ", NULL, auto_return_0, 0},
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
{L"semi  :  error ", NULL, auto_return_null, 0},
{L"expression  :  assignment_expression ", NULL, auto_return_0, 0},
{L"assignment_expression  :  constant_expression ", NULL, auto_return_0, 0},
{L"assignment_expression  :  unary_expression = table_constructor ", NULL, on_assignment_expression, 0},
{L"assignment_expression  :  unary_expression = assignment_expression ", NULL, on_assignment_expression, 0},
{L"constant_expression  :  binary_expression ", NULL, auto_return_0, 0},
{L"constant_expression  :  binary_expression ? expression : constant_expression ", NULL, on_condition_expression, 0},
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
{L"expression_list  :  expression ", NULL, auto_return_0, 0},
{L"expression_list  :  expression_list , expression ", NULL, on_expression_list, 0}
};

#define __RULE_COUNT__ ((size_t)111)
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
		if(!Parser_SetFirstRule(grammar,START_RULE) || !Parser_CheckIsValidGrammar(grammar))																						
		{																																									
				Parser_DestroyGrammar(grammar);																																
				grammar = NULL;																																				
				AR_ASSERT(false);																																			
				return NULL;																																				
		}																																									
		return grammar;																																						
}




static psrNode_t* AR_STDCALL build_default_leaf(const psrToken_t *tok,void *ctx)
{
	 {

	return NULL;
}
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




/*function_defination	:	var NAME ( params ) start_function compound_statement close_function */
static psrNode_t* AR_STDCALL on_function_defination(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						return NULL;
					}
}




/*params	:	namelist , ... */
static psrNode_t* AR_STDCALL on_namelist_ellipsis(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						AR_ASSERT(nodes != NULL && count == 3);
						ns[0]->params->is_variadic = true;
						ret = ns[0];
						ns[0] = NULL;
						return ret;
					}
}




/*params	:	namelist */
/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
/*statement	:	empty_statement */
/*init_declarator_list	:	init_declarator */
/*filed_list	:	filed */
/*compound_element	:	statement */
/*compound_element	:	declaration */
/*expression_statement	:	expression semi */
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
/*expression_list	:	expression */
static psrNode_t* AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						psrNode_t *ret = nodes[0];
						ret  = nodes[0];
						nodes[0] = NULL;
						return ret;
					}
}




/*params	:	... */
static psrNode_t* AR_STDCALL on_ellipsis(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParams_t	*params;
						tguSynNode_t	*ret;
						AR_ASSERT(nodes != NULL && count == 1);

						params = AR_NEW0(tguParams_t);
						init_params(params);
						params->is_variadic = true;
						ret = __create_synnode(TGU_NODE_PARAMS_T, (void*)params);
						return ret;
					}
}




/*namelist	:	namelist , NAME */
static psrNode_t* AR_STDCALL on_name_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						tguParams_t	*params;
						tguToken_t		*tok;
						AR_ASSERT(nodes != NULL && count == 2);
						tok = ns[1]->token;
						params = ns[0]->params;
						insert_to_params(params, tok->token);
						ret = ns[0];
						ns[0] = NULL;
						return ret;
					}
}




/*namelist	:	NAME */
static psrNode_t* AR_STDCALL on_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						tguParams_t	*params;
						AR_ASSERT(nodes != NULL && count == 1);

						params = AR_NEW0(tguParams_t);
						init_params(params);
						insert_to_params(params, ns[0]->token->token);
						ret = __create_synnode(TGU_NODE_PARAMS_T, (void*)params);
						return ret;
					}
}




/*start_function	:	 */
static psrNode_t* AR_STDCALL on_start_function(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						return NULL;
					}
}




/*close_function	:	 */
static psrNode_t* AR_STDCALL on_close_function(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						return NULL;
					}
}




/*declaration	:	var init_declarator_list semi */
static psrNode_t* AR_STDCALL auto_return_1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						psrNode_t *ret;
						ret  = nodes[1];
						nodes[1] = NULL;
						return ret;
					}
}




/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL on_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						//tguParser_t 	*parser = (tguParser_t*)ctx;
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						tguDeclaration_t *lst, *decl, *tmp;

						if(ns[0] == NULL)
						{
							ret = ns[2];
							ns[2] = NULL;
						}else if(ns[2] == NULL)
						{
							ret = ns[0];
							ns[0] = NULL;
						}else
						{
							lst = ns[0]->decl; 
							decl = ns[2]->decl;
							ns[2]->decl = NULL;

							for(tmp = lst; tmp->next != NULL; tmp = tmp->next);
							tmp->next = decl;
							ret = ns[0];
							ns[0] = NULL;
						}
						 return ret;
					}
}




/*init_declarator	:	NAME = expression */
/*init_declarator	:	NAME = table_constructor */
/*init_declarator	:	NAME */
static psrNode_t* AR_STDCALL on_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t 	*parser = (tguParser_t*)ctx;
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						tguTableInit_t  *tbl_init;
						tguExpr_t		*expr;
						tguDeclaration_t *decl;
						AR_ASSERT(nodes != NULL && parser != NULL && (count == 1 || count == 3));
							
						if(count == 1)
						{
							decl = make_declaration_from_null(parser, ns[0]->token);
						}else
						{
							if(ns[2] == NULL)
							{
								decl = make_declaration_from_error(parser, ns[0]->token);
							}else 
							{
								if(ns[2]->type == TGU_NODE_TABLE_INIT_T)
								{
										tbl_init = ns[2]->tbl_init;
										ns[2]->tbl_init = NULL;
										decl = make_declaration_from_table(parser, ns[0]->token, tbl_init);
								}else
								{
										expr = ns[2]->expr;
										ns[2]->expr = NULL;
										decl = make_declaration_from_expression(parser, ns[0]->token, expr);
								}
							}
						}

						ret = __create_synnode(TGU_NODE_DECL_T, (void*)decl);
						return ret;
					}
}




/*table_constructor	:	{ filed_list } */
/*table_constructor	:	{ } */
/*table_constructor	:	{ error } */
static psrNode_t* AR_STDCALL on_table_constructor(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t 	*parser = (tguParser_t*)ctx;
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						bool_t is_empty;
						tguTableField_t *lst;
						tguTableInit_t  *tbl_init;
						
						AR_ASSERT(parser != NULL && (count == 2 || count == 3));

						if(count == 2)
						{
							is_empty = true;
							lst = NULL;
						}else 
						{
							is_empty = false;
							lst = ns[1]->field;
							ns[1]->field = NULL;
						}

						tbl_init = make_table_init(parser, is_empty, lst);
						ret = __create_synnode(TGU_NODE_TABLE_INIT_T, tbl_init);
						return ret;
					}
}




/*filed_list	:	filed_list , filed */
static psrNode_t* AR_STDCALL on_filed_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						//tguParser_t 	*parser = (tguParser_t*)ctx;
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						tguTableField_t *lst, *field, *tmp;

						if(ns[0] == NULL)
						{
							ret = ns[2];
							ns[2] = NULL;
						}else if(ns[2] == NULL)
						{
							ret = ns[0];
							ns[0] = NULL;
						}else
						{
							lst = ns[0]->field; 
							field= ns[2]->field;
							ns[2]->field= NULL;

							for(tmp = lst; tmp->next != NULL; tmp = tmp->next);
							tmp->next = field;
							ret = ns[0];
							ns[0] = NULL;
						}
						 return ret;
					}
}




/*filed	:	expression */
/*filed	:	table_constructor */
static psrNode_t* AR_STDCALL on_filed(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t 	*parser = (tguParser_t*)ctx;
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						tguTableField_t *field;
						AR_ASSERT(ns != NULL && count == 1 && parser != NULL);

						if(ns[0] == NULL)
						{
							parser->has_error = true;
							field = NULL;
							return NULL;
						}else if(ns[0]->type == TGU_NODE_EXPR_T)
						{
							 field = make_table_field(parser, TGU_INIT_TABLE_EXPR, (void*)ns[0]->expr);
							 ns[0]->expr = NULL;
						}else if(ns[0]->type == TGU_NODE_TABLE_FILED_T)
						{
							field = make_table_field(parser, TGU_INIT_TABLE_FIELD, (void*)ns[0]->field);
							ns[0]->field = NULL;
						}else
						{
							field = NULL;
							AR_ASSERT(false);
							return NULL;
						}

						ret = __create_synnode(TGU_NODE_TABLE_FILED_T, field);
						return ret;
					}
}




/*compound_statement	:	start_block compound_element_list close_block */
/*compound_statement	:	{ } */
/*compound_statement	:	start_block error close_block */
static psrNode_t* AR_STDCALL on_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						/*
						tguParser_t 	*parser = (tguParser_t*)ctx;
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						AR_ASSERT(nodes != NULL && (count == 2 || count == 3));

						if(count == 3)return NULL;
						AR_ASSERT(ns[1] == NULL);
						*/
						return NULL;
					}
}




/*compound_element_list	:	compound_element_list compound_element */
/*compound_element_list	:	compound_element */
static psrNode_t* AR_STDCALL on_compound_element_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t 	*parser = (tguParser_t*)ctx;
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNodeType_t type;
						tguDeclaration_t *decl;
						tguStmt_t		*stmt;
						AR_ASSERT(nodes != NULL && (count == 2 || count == 1));
						if(count == 2) return NULL;
						type = ns[0]->type;
						if(ns[0]->type == TGU_NODE_DECL_T)
						{
							decl = ns[0]->decl;
							ns[0]->decl = NULL;
							if(decl) 
							{
								insert_decl_to_block(parser->top_block, decl);
							}else 
							{
								parser->has_error = true;
							}
						}else if(ns[0]->type == TGU_NODE_STMT_T)
						{
							stmt = ns[0]->stmt;
							ns[0]->stmt = NULL;
							if(stmt)
							{
								insert_stmt_to_block(parser->top_block, stmt);
							}else 
							{
								parser->has_error = true;
							}
						}else {
							AR_ASSERT(false);
						}
						return NULL;
					}
}




/*start_block	:	{ */
static psrNode_t* AR_STDCALL on_start_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
							tguParser_t 	*parser = (tguParser_t*)ctx;
							tguSynNode_t	**ns = (tguSynNode_t**)nodes;
							start_block(parser, &ns[0]->token->lex_info);
							return NULL;
					}
}




/*close_block	:	} */
static psrNode_t* AR_STDCALL on_close_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
							tguParser_t 	*parser = (tguParser_t*)ctx;
							tguSynNode_t	**ns = (tguSynNode_t**)nodes;
							close_block(parser, &ns[0]->token->lex_info);
							return NULL;
					}
}




/*empty_statement	:	; */
static psrNode_t* AR_STDCALL on_empty_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguSynNode_t 	*ret;
						tguStmt_t		*stmt;
						AR_ASSERT(ns != NULL && parser != NULL && count == 1);

						stmt = make_empty_statement(parser, &ns[0]->token->lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
						return ret;
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
						tguStmt_t		*if_true, *stmt;
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

						stmt = make_if_statement(parser, expr, if_true, NULL, &ns[0]->token->lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
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
						tguStmt_t		*stmt;
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


						stmt = make_if_statement(parser, expr, if_true, if_false,  &ns[0]->token->lex_info);	
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
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

						stmt = make_while_statement(parser, expr, stmt, &ns[0]->token->lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
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

						stmt =make_do_while_statement(parser, expr, stmt, &ns[0]->token->lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
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
						tguStmt_t		*stmt;
						tguParser_t	*parser = (tguParser_t*)ctx;
						stmt = make_jump_statement(parser, TGU_STT_CONTINUE, 	NULL, &ns[0]->token->lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
						return ret;

					}
}




/*jump_statement	:	break semi */
static psrNode_t* AR_STDCALL on_break_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguStmt_t		*stmt;
						tguParser_t	*parser = (tguParser_t*)ctx;
						ret = AR_NEW(tguSynNode_t);
						ret->type = TGU_NODE_STMT_T;
						stmt = make_jump_statement(parser, TGU_STT_BREAK, 	NULL, &ns[0]->token->lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
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
						tguExpr_t		*expr;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguStmt_t		*stmt;
						AR_ASSERT(nodes != NULL && count == 2 || count == 3);
						AR_ASSERT(parser->current_function != NULL);

						if(ns[1] == NULL)
						{
							expr = NULL;
						}else
						{
							AR_ASSERT(ns[1]->type == TGU_NODE_EXPR_T);
							expr = ns[1]->expr;
							ns[1]->expr = NULL;
						}

						stmt = make_jump_statement(parser, TGU_STT_RETURN, 	expr, &ns[0]->token->lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
						return ret;
					}
}




/*semi	:	error */
static psrNode_t* AR_STDCALL auto_return_null(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*assignment_expression	:	unary_expression = table_constructor */
/*assignment_expression	:	unary_expression = assignment_expression */
static psrNode_t* AR_STDCALL on_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguExpr_t		*addr, *value, *expr;
						const tguLexInfo_t *lex_info; 
						AR_ASSERT(ns != NULL && count == 3 && parser != NULL);

						if(ns[0] == NULL)
						{
							parser->has_error = true;
							addr = NULL;	
						}else
						{
							addr = ns[0]->expr;
							ns[0]->expr = NULL;
						}
						

						if(ns[2] == NULL)
						{
							parser->has_error = true;
							value = NULL;	
						}else
						{
							value = ns[2]->expr;
							ns[2]->expr = NULL;
						}

						lex_info = addr != NULL ? &addr->lex_info : &ns[1]->token->lex_info;
						expr = make_assignment_expression(parser, addr, value, lex_info);
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);

						return ret;

					}
}




/*constant_expression	:	binary_expression ? expression : constant_expression */
static psrNode_t* AR_STDCALL on_condition_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguExpr_t		*cond, *if_true, *if_false, *expr;
						const tguLexInfo_t *lex_info;
						AR_ASSERT(ns != NULL && count == 3 && parser != NULL);

						if(ns[0] == NULL)
						{
							parser->has_error = true;
							cond= NULL;	
						}else
						{
							cond = ns[0]->expr;
							ns[0]->expr = NULL;
						}

						

						if(ns[2] == NULL)
						{
							parser->has_error = true;
							if_true = NULL;	
						}else
						{
							if_true= ns[2]->expr;
							ns[2]->expr = NULL;
						}
						
						if(ns[4] == NULL)
						{
							parser->has_error = true;
							if_false = NULL;	
						}else
						{
							if_false= ns[4]->expr;
							ns[4]->expr = NULL;
						}

						lex_info = cond != NULL ? &cond->lex_info : &ns[1]->token->lex_info;
						expr = make_condition_expression(parser, cond, if_true, if_false, lex_info);
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);
						return ret;


					}
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
static psrNode_t* AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguExpr_t		*left, *right, *expr;
						tguToken_t		*tok;
						tguExprOP_t	op;
						const tguLexInfo_t *lex_info;
						AR_ASSERT(ns != NULL && count == 3 && parser != NULL);

						if(ns[0] == NULL)
						{
							parser->has_error = true;
							left = NULL;	
						}else
						{
							left = ns[0]->expr;
							ns[0]->expr = NULL;
						}

						AR_ASSERT(ns[1] != NULL);
						tok = ns[1]->token;	

						if(ns[2] == NULL)
						{
							parser->has_error = true;
							right = NULL;	
						}else
						{
							right = ns[2]->expr;
							ns[2]->expr = NULL;
						}

						switch(tok->term_val)
						{
						case TOK_INC:
							op = TGU_OP_PLUS;
							break;
						case TOK_SUB :
							op = TGU_OP_MINUS;
							break;
						case TOK_MUL:
							op = TGU_OP_MUL;
							break;
						case TOK_DIV:
							op = TGU_OP_DIV;
							break;
						case TOK_MOD:
							op = TGU_OP_MOD;
							break;
						case TOK_LESS:
							op = TGU_OP_LESS;
							break;
						case TOK_LE:
							op = TGU_OP_LESS_OR_EQUAL;
							break;
						case TOK_GREATER:
							op = TGU_OP_GREATER;
							break;
						case TOK_GE:
							op = TGU_OP_GREATER_OR_EQUAL;
							break;
						case TOK_EQ:
							op = TGU_OP_IS_EQUAL;
							break;
						case TOK_NE:
							op = TGU_OP_NOT_EQUAL;
							break;
						case TOK_ANDAND:
							op = TGU_OP_LOGICAL_AND;
							break;
						case TOK_OROR:
							op = TGU_OP_LOGICAL_OR;
							break;
						default:
							op = TGU_OP_NONE;/*op如果在此不赋值会导致一个warning*/
							AR_ASSERT(false);
							break;
						}
						lex_info  = left != NULL ? &left->lex_info : &tok->lex_info;
						expr = make_binary_expression(parser, op, left, right, lex_info);
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);
						return ret;


					}
}




/*unary_expression	:	+ unary_expression */
/*unary_expression	:	- unary_expression */
/*unary_expression	:	! unary_expression */
/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
static psrNode_t* AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;

						tguExpr_t		*expr;
						tguToken_t		*tok;
						tguExprOP_t	op;
						const tguLexInfo_t	*lex_info;
						AR_ASSERT(ns != NULL && count == 2 && parser != NULL);

						if(ns[1] == NULL)
						{
							parser->has_error = true;
							expr = NULL;	
						}else
						{
							expr = ns[1]->expr;
							ns[1]->expr = NULL;
						}

						AR_ASSERT(ns[0] != NULL && ns[0]->token != NULL);
						tok = ns[0]->token;	
						
						switch(tok->term_val)
						{
						case TOK_INC:
							op = TGU_OP_PREINC;
							break;
						case TOK_DEC:
							op = TGU_OP_PREDEC;
							break;
						case TOK_ADD:
							op = TGU_OP_UNARY_PLUS;
							break;
						case TOK_SUB:
							op = TGU_OP_UNARY_MINUS;
							break;
						case TOK_NOT:
							op = TGU_OP_LOGICAL_NOT;
							break;
						default:
							op = TGU_OP_NONE;
							AR_ASSERT(false);
							break;
						}
						lex_info = expr != NULL ? &expr->lex_info : &ns[0]->token->lex_info;
						expr = make_unary_expression(parser, op, expr, false, lex_info);
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);
						return ret;

				}
}




/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
static psrNode_t* AR_STDCALL on_post_add_minus_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguExpr_t		*expr;
						tguToken_t		*tok;
						tguExprOP_t	op;
						const tguLexInfo_t	*lex_info; 
						AR_ASSERT(ns != NULL && count == 2 && parser != NULL);

						if(ns[0] == NULL)
						{
							parser->has_error = true;
							expr = NULL;	
						}else
						{
							expr = ns[0]->expr;
							ns[0]->expr = NULL;
						}

						AR_ASSERT(ns[1] != NULL && ns[1]->token != NULL);
						tok = ns[1]->token;	
							
						switch(tok->term_val)	
						{
						case TOK_INC:
							op = TGU_OP_POSTINC;
							break;
						case TOK_DEC:
							op = TGU_OP_POSTDEC;
							break;
						default:
							op = TGU_OP_NONE;
							AR_ASSERT(false);
							break;
						}
						
						lex_info = expr != NULL ? &expr->lex_info : &tok->lex_info;
						expr = make_unary_expression(parser, op, expr, false, lex_info);
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);
						return ret;
				}
}




/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression [ error ] */
static psrNode_t* AR_STDCALL on_index_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
					
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 	*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguExpr_t		*expr, *index_expr;
						const tguLexInfo_t	*lex_info;
						AR_ASSERT(ns != NULL && count == 4 && parser != NULL);

						if(ns[0] == NULL)
						{
							parser->has_error = true;
							expr = NULL;
						}else
						{
							expr = ns[0]->expr;
							ns[0]->expr = NULL;
						}

						
						
						if(ns[2] == NULL)
						{
							index_expr = NULL;
							parser->has_error = true;
						}else
						{
							index_expr = ns[2]->expr;
							ns[2]->expr = NULL;
						}
						lex_info = expr == NULL ? &expr->lex_info : &ns[1]->token->lex_info;
						expr = make_index_expression(parser, expr, index_expr, lex_info);
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);
						return ret;
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
						
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);
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
						
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);
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
						tguExpr_t		*expr, *call_expr, *args;
						const tguLexInfo_t	*lex_info;
						if(ns[0] == NULL)
						{
							parser->has_error = true;
							call_expr = NULL;
						}else
						{
							call_expr= ns[0]->expr;
							ns[0]->expr = NULL;
						}


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
						
						lex_info = call_expr != NULL ? &call_expr->lex_info : &ns[1]->token->lex_info;
						expr = make_call_expression(parser, call_expr, args, lex_info );
						
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);
						return ret;
				}
}




/*expression_list	:	expression_list , expression */
static psrNode_t* AR_STDCALL on_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						tguExpr_t		*lst;
						AR_ASSERT(ns != NULL && count == 3);

						if(ns[0] == NULL) return ns[2];
						if(ns[2] == NULL) return ns[2];
						/*expression_list中有任何错误，不应该反映在此处*/
						AR_ASSERT(ns[0]->expr != NULL  && ns[2]->expr != NULL);

						for(lst = ns[0]->expr; lst->next != NULL; lst = lst->next);
						
						lst->next = ns[2]->expr;
						ns[2]->expr = NULL;
						ret = ns[0];
						ns[0] = NULL;
						return ret;
				}
}




