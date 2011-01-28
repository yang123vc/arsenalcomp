

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

	typedef enum
	{
		TGU_NODE_TOKEN_T,
		TGU_NODE_STMT_T,
		TGU_NODE_EXPR_T,
		TGU_NODE_SYMB_T,
		TGU_NODE_TABLE_FILED_T,
		TGU_NODE_PARAMS_T,
		TGU_NODE_FUNC_T
	}tguSynNodeType_t;

	typedef struct __tengu_syntax_node_tag
	{
		tguSynNodeType_t		type;
		
		union{
				tguToken_t			token;
				tguStmt_t			*stmt;
				tguExpr_t			*expr;
				tguTableField_t		*field;
				tguParams_t			*params;
				tguSymb_t			*symb;
		};
	}tguSynNode_t;


	static tguSynNode_t*	__create_synnode(tguSynNodeType_t type, void *data)
	{
			tguSynNode_t *node;

			node = AR_NEW0(tguSynNode_t);
			node->type = type;

			switch(node->type)
			{
			case TGU_NODE_TOKEN_T:
				node->token = *(tguToken_t*)data;
				break;
			case TGU_NODE_STMT_T:
				node->stmt = (tguStmt_t*)data;
				break;
			case TGU_NODE_EXPR_T:
				node->expr = (tguExpr_t*)data;
				break;
			case TGU_NODE_SYMB_T:
				node->symb = (tguSymb_t*)data;
				break;
			case TGU_NODE_TABLE_FILED_T:
				node->field = (tguTableField_t*)data;
				break;
			case TGU_NODE_PARAMS_T:
				node->params = (tguParams_t*)data;
				break;
			}

			return node;
	}


	static void	__destroy_synnode(	tguSynNode_t* node)
	{
		AR_ASSERT(node != NULL);
		switch(node->type)
		{
		case TGU_NODE_TOKEN_T:
			break;
		case TGU_NODE_STMT_T:
			if(node->stmt)
			{
				TGU_DestroyStmt(node->stmt);
				node->stmt = NULL;
			}
			break;
		case TGU_NODE_EXPR_T:
			if(node->expr)
			{
				TGU_DestroyExpr(node->expr);
				node->expr = NULL;
			}
			break;
		case TGU_NODE_SYMB_T:
			if(node->symb)
			{
				TGU_DestroySymb(node->symb);
				node->symb = NULL;
			}
			break;
		case TGU_NODE_TABLE_FILED_T:
			if(node->field)
			{
				TGU_DestroyTableField(node->field);
				node->field = NULL;
			}
			break;
		case TGU_NODE_PARAMS_T:
			if(node->params)
			{	
				TGU_DestroyParams(node->params);
				node->params = NULL;
			}
			break;


		}
	}



/*
syntax_tree_handler
*/


	static void		AR_STDCALL on_free_node(psrNode_t *node, void *ctx)
	{
		AR_ASSERT(node != NULL && ctx != NULL);
		__destroy_synnode((tguSynNode_t*)node);
		
	}

	static void		AR_STDCALL on_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
	{
		
	}
	
	static const psrHandler_t	__g_handler = {on_error, on_free_node};



	
	tguSynNode_t*	on_lex_node(tguParser_t *parser, const psrToken_t *tok)
	{
		return NULL;

	}




	
	void	start_function(tguParser_t *parser, const wchar_t *func_name, const tguParams_t	*params, const tguLexInfo_t *lex_info)
	{
		
	}
	
	tguFunc_t*	close_function(tguParser_t *parser)
	{
		return NULL;
	}



	
	static void	handle_symb_from_expression(tguParser_t 	*parser, const tguToken_t *tok, tguExpr_t *expr)
	{

	}



	static tguTableInit_t*	make_table_init(tguParser_t 	*parser, bool_t is_empty, tguTableField_t	*field_lst)
	{
		return NULL;
	}




	static tguTableField_t*	make_table_field(tguParser_t *parser, tguTableFieldType_t type, void *expr_or_field)
	{
		return NULL;
	}




/*
block_operation
*/

	static void push_block(tguParser_t *parser)
	{
	}

	static tguBlock_t* pop_block(tguParser_t *parser)
	{
		return NULL;
	}	

	tguStmt_t*		make_compound_stmt(tguParser_t 	*parser, tguBlock_t		*block, const tguLexInfo_t *start, const tguLexInfo_t *end)
	{
		return NULL;
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




	
	static tguStmt_t*	make_for_statement(tguParser_t *parser, tguExpr_t *init, tguExpr_t *cond, tguExpr_t *step, tguStmt_t *loop, const tguLexInfo_t *lex_info)
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
			tguExpr_t 	*ret;
			tguSymb_t	*symb;
			AR_ASSERT(parser != NULL && token != NULL); 
			
			ret = NULL;
			symb = NULL;

			switch(token->term_val)
			{
			default:
				AR_ASSERT(false);
				break;
			case TOK_NULL:
				break;
			case TOK_TRUE:
				break;
			case TOK_FALSE:
				break;
			case TOK_STRING:
				break;
			case TOK_FLOAT_NUMBER:
				break;
			case TOK_INT_NUMBER:
				break;
			}

			return ret;
	}



	static tguExpr_t*		make_call_expression(tguParser_t	*parser, tguExpr_t *call_expr, tguExpr_t *args, const tguLexInfo_t *lex_info)
	{
			tguExpr_t *expr;
			AR_ASSERT(parser != NULL && lex_info != NULL);

			expr = TGU_CreateExpr(TGU_ET_FUNC_CALL);
			expr->is_lvalue = false;
			expr->is_constant = false;
			expr->lex_info = *lex_info;
			
			expr->func_call_expr.func_call = call_expr;
			expr->func_call_expr.arg_list = args;
			return expr;
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

static psrNode_t* AR_STDCALL default_leaf_handler(const psrToken_t *tok,void *ctx);




/*enum{
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
TOK_TRUE = 271,
TOK_FALSE = 272,
TOK_VAR = 273,
TOK_IMPORT = 274,
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
TOK_COLON = 301,
TOK_QUEST = 302,
TOK_DOT = 303,
};*/




static struct {const wchar_t *name;
size_t tokval;
size_t lex_prec;
const wchar_t *regex;
bool_t skip;
psrTermFunc_t leaf;
}__g_term_pattern[] =  {
{NULL, TOK_DELIM_ID,1, L"{skip_lexem}", true, NULL},
{L"NAME", TOK_NAME, 0, L"{letter}({letter}|{digit})*", false, default_leaf_handler},
{L"STRING", TOK_STRING, 0, L"{string_dq}|{string_sq}", false, default_leaf_handler},
{L"FLOAT_NUMBER", TOK_FLOAT_NUMBER, 2, L"{float_constant}", false, default_leaf_handler},
{L"INT_NUMBER", TOK_INT_NUMBER, 2, L"{hex_constant}|{oct_constant}|{dec_constant}", false, default_leaf_handler},
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
{L"import", TOK_IMPORT, 1, L"\"import\"(?!{keyword_lhd})", false, default_leaf_handler},
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
{L"EOI", 0, 2, L"$", false, NULL}
};

#define __TERM_COUNT__ ((size_t)48)

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
{L"IF_WITHOUT_ELSE", 304,7, PARSER_ASSOC_NONASSOC},
{L"else", TOK_ELSE,8, PARSER_ASSOC_NONASSOC}
};

#define __PREC_COUNT__ ((size_t)17)

/*program	:	translation_unit */
/*program	:	 */
static psrNode_t* AR_STDCALL on_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*translation_unit	:	element */
/*translation_unit	:	translation_unit element */
/*element	:	declaration */
/*element	:	function_defination */
/*element	:	statement */
/*declaration	:	var init_declarator_list semi */
/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
/*compound_element_list	:	compound_element_list compound_element */
/*compound_element_list	:	compound_element */
/*compound_element	:	declaration */
static psrNode_t* AR_STDCALL auto_return_null(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_signature	:	var NAME ( params ) */
static psrNode_t* AR_STDCALL on_function_signature(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_defination	:	function_signature compound_statement */
static psrNode_t* AR_STDCALL on_function_defination(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist , ... */
static psrNode_t* AR_STDCALL on_namelist_ellipsis(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	namelist */
/*filed_list	:	filed */
/*filed	:	expression */
/*filed	:	table_constructor */
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
/*iteration_statement	:	for_statement */
/*for_expression	:	expression_list */
/*for_expression	:	 */
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

/*import_statement	:	import STRING ; */
/*import_statement	:	import error ; */
static psrNode_t* AR_STDCALL on_import_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	start_block compound_element_list } */
static psrNode_t* AR_STDCALL on_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	start_block error } */
static psrNode_t* AR_STDCALL on_compound_error_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	{ } */
static psrNode_t* AR_STDCALL on_empty_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*start_block	:	{ */
static psrNode_t* AR_STDCALL on_start_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_element	:	statement */
static psrNode_t* AR_STDCALL on_compound_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*empty_statement	:	; */
static psrNode_t* AR_STDCALL on_empty_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression semi */
static psrNode_t* AR_STDCALL on_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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

/*for_statement	:	for ( for_expression ; for_expression ; for_expression ) enter_loop statement leave_loop */
/*for_statement	:	for ( error ) enter_loop statement leave_loop */
static psrNode_t* AR_STDCALL on_for_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
static psrNode_t* AR_STDCALL on_semi_error(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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

/*postfix_expression	:	postfix_expression . NAME */
static psrNode_t* AR_STDCALL on_access_name_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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
{L"program  :  translation_unit ", NULL, on_translation_unit, 0},
{L"program  :   ", NULL, on_translation_unit, 0},
{L"translation_unit  :  element ", NULL, auto_return_null, 0},
{L"translation_unit  :  translation_unit element ", NULL, auto_return_null, 0},
{L"element  :  declaration ", NULL, auto_return_null, 0},
{L"element  :  function_defination ", NULL, auto_return_null, 0},
{L"element  :  statement ", NULL, auto_return_null, 0},
{L"function_signature  :  var NAME ( params ) ", NULL, on_function_signature, 0},
{L"function_defination  :  function_signature compound_statement ", NULL, on_function_defination, 0},
{L"params  :  namelist , ... ", NULL, on_namelist_ellipsis, 0},
{L"params  :  namelist ", NULL, auto_return_0, 0},
{L"params  :  ... ", NULL, on_ellipsis, 0},
{L"params  :   ", NULL, NULL, 0},
{L"namelist  :  namelist , NAME ", NULL, on_name_list, 0},
{L"namelist  :  NAME ", NULL, on_name, 0},
{L"declaration  :  var init_declarator_list semi ", NULL, auto_return_null, 0},
{L"init_declarator_list  :  init_declarator ", NULL, auto_return_null, 0},
{L"init_declarator_list  :  init_declarator_list , init_declarator ", NULL, auto_return_null, 0},
{L"init_declarator  :  NAME = expression ", NULL, on_declarator, 0},
{L"init_declarator  :  NAME = table_constructor ", NULL, on_declarator, 0},
{L"init_declarator  :  NAME ", NULL, on_declarator, 0},
{L"table_constructor  :  { filed_list } ", NULL, on_table_constructor, 0},
{L"table_constructor  :  { } ", NULL, on_table_constructor, 0},
{L"table_constructor  :  { error } ", NULL, on_table_constructor, 0},
{L"filed_list  :  filed_list , filed ", NULL, on_filed_list, 0},
{L"filed_list  :  filed ", NULL, auto_return_0, 0},
{L"filed  :  expression ", NULL, auto_return_0, 0},
{L"filed  :  table_constructor ", NULL, auto_return_0, 0},
{L"statement  :  compound_statement ", NULL, auto_return_0, 0},
{L"statement  :  expression_statement ", NULL, auto_return_0, 0},
{L"statement  :  selection_statement ", NULL, auto_return_0, 0},
{L"statement  :  iteration_statement ", NULL, auto_return_0, 0},
{L"statement  :  jump_statement ", NULL, auto_return_0, 0},
{L"statement  :  empty_statement ", NULL, auto_return_0, 0},
{L"statement  :  import_statement ", NULL, auto_return_0, 0},
{L"import_statement  :  import STRING ; ", NULL, on_import_statement, 0},
{L"import_statement  :  import error ; ", NULL, on_import_statement, 0},
{L"compound_statement  :  start_block compound_element_list } ", NULL, on_compound_statement, 0},
{L"compound_statement  :  start_block error } ", NULL, on_compound_error_statement, 0},
{L"compound_statement  :  { } ", NULL, on_empty_compound_statement, 0},
{L"start_block  :  { ", NULL, on_start_block, 0},
{L"compound_element_list  :  compound_element_list compound_element ", NULL, auto_return_null, 0},
{L"compound_element_list  :  compound_element ", NULL, auto_return_null, 0},
{L"compound_element  :  statement ", NULL, on_compound_element, 0},
{L"compound_element  :  declaration ", NULL, auto_return_null, 0},
{L"empty_statement  :  ; ", NULL, on_empty_statement, 0},
{L"expression_statement  :  expression semi ", NULL, on_expression_statement, 0},
{L"selection_statement  :  if_statement ", NULL, auto_return_0, 0},
{L"selection_statement  :  if_else_statement ", NULL, auto_return_0, 0},
{L"if_statement  :  if ( expression ) statement ", L"IF_WITHOUT_ELSE", on_if_statement, 0},
{L"if_statement  :  if ( error ) statement ", L"IF_WITHOUT_ELSE", on_if_statement, 0},
{L"if_else_statement  :  if ( expression ) statement else statement ", NULL, on_if_else_statement, 0},
{L"if_else_statement  :  if ( error ) statement else statement ", NULL, on_if_else_statement, 0},
{L"iteration_statement  :  while_statement ", NULL, auto_return_0, 0},
{L"iteration_statement  :  do_while_statement ", NULL, auto_return_0, 0},
{L"iteration_statement  :  for_statement ", NULL, auto_return_0, 0},
{L"while_statement  :  while enter_loop ( expression ) statement leave_loop ", NULL, on_while_statement, 0},
{L"while_statement  :  while enter_loop ( error ) statement leave_loop ", NULL, on_while_statement, 0},
{L"do_while_statement  :  do enter_loop statement while ( expression ) leave_loop semi ", NULL, on_do_while_statement, 0},
{L"do_while_statement  :  do enter_loop statement while ( error ) leave_loop semi ", NULL, on_do_while_statement, 0},
{L"for_statement  :  for ( for_expression ; for_expression ; for_expression ) enter_loop statement leave_loop ", NULL, on_for_statement, 0},
{L"for_statement  :  for ( error ) enter_loop statement leave_loop ", NULL, on_for_statement, 0},
{L"for_expression  :  expression_list ", NULL, auto_return_0, 0},
{L"for_expression  :   ", NULL, auto_return_0, 0},
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
{L"postfix_expression  :  postfix_expression . NAME ", NULL, on_access_name_expression, 0},
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

#define __RULE_COUNT__ ((size_t)119)
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
		if(!Parser_SetStartRule(grammar,START_RULE) || !Parser_CheckIsValidGrammar(grammar))																						
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
	 return NULL;
}


/*program	:	translation_unit */
/*program	:	 */
static psrNode_t* AR_STDCALL on_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
					tguParser_t 	*parser = (tguParser_t*)ctx;
					tguBlock_t	*result;
					AR_ASSERT(count == 0 || count == 1);
					result = parser->abs_tree;
					parser->abs_tree = NULL;
					parser->top_block = NULL;
					parser->current_function = NULL;
					AR_ASSERT(result != NULL);
					return (psrNode_t*)result;

				}
}




/*translation_unit	:	element */
/*translation_unit	:	translation_unit element */
/*element	:	declaration */
/*element	:	function_defination */
/*element	:	statement */
/*declaration	:	var init_declarator_list semi */
/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
/*compound_element_list	:	compound_element_list compound_element */
/*compound_element_list	:	compound_element */
/*compound_element	:	declaration */
static psrNode_t* AR_STDCALL auto_return_null(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						return NULL;
					}
}




/*function_signature	:	var NAME ( params ) */
static psrNode_t* AR_STDCALL on_function_signature(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t 	*parser = (tguParser_t*)ctx;
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguToken_t		name;
						tguLexInfo_t		lex_info;
						tguParams_t		*params;
						AR_ASSERT(ns[0] != NULL && ns[1] != NULL);

						lex_info = ns[0]->token.lex_info;
						name = ns[1]->token;
						if(ns[3] == NULL)
						{
							params = NULL;
							parser->has_error = true;
						}else
						{
							params = ns[3]->params;
							AR_ASSERT(params != NULL);
						}
						
						start_function(parser, name.token, params, &lex_info);
						return NULL;
				}
}




/*function_defination	:	function_signature compound_statement */
static psrNode_t* AR_STDCALL on_function_defination(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t 	*parser = (tguParser_t*)ctx;
						close_function(parser);
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
/*filed_list	:	filed */
/*filed	:	expression */
/*filed	:	table_constructor */
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
/*iteration_statement	:	for_statement */
/*for_expression	:	expression_list */
/*for_expression	:	 */
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

						params = TGU_CreateParams();
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
						tguToken_t	tok;
						AR_ASSERT(nodes != NULL && count == 2);
						tok = ns[1]->token;
						params = ns[0]->params;
						TGU_InsertToParams(params, &tok);
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
						tguToken_t	tok;
						AR_ASSERT(nodes != NULL && count == 1);
						tok = ns[0]->token;
						params = TGU_CreateParams();
						TGU_InsertToParams(params, &tok);
						ret = __create_synnode(TGU_NODE_PARAMS_T, (void*)params);
						return ret;
				}
}




/*init_declarator	:	NAME = expression */
/*init_declarator	:	NAME = table_constructor */
/*init_declarator	:	NAME */
static psrNode_t* AR_STDCALL on_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t 		*parser = (tguParser_t*)ctx;
						tguSynNode_t		**ns = (tguSynNode_t**)nodes;
						tguExpr_t		*expr;
						AR_ASSERT(nodes != NULL && parser != NULL && (count == 1 || count == 3));

						if(count == 1)
						{
							/*声明了一个null类型的变量*/
							handle_symb_from_expression(parser, &ns[0]->token, NULL);
						}else
						{
							if(ns[2] == NULL)
							{
								/*expression 或者 table_constructor出现错误*/
								parser->has_error = true;
								handle_symb_from_expression(parser, &ns[0]->token, NULL);
							}else 
							{
								expr = ns[2]->expr;
								ns[2]->expr = NULL;
								AR_ASSERT(expr != NULL);
								handle_symb_from_expression(parser, &ns[0]->token, expr);
							}
						}

						return NULL;
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
						tguExpr_t	*expr;
						AR_ASSERT(parser != NULL && ns != NULL);
						AR_ASSERT(count == 2 || count == 3);
						
						expr = TGU_CreateExpr(TGU_ET_TABLE_INIT);

						if(count == 2)
						{
							/*创建了个空的表结构*/
							expr->table_init.field_lst = NULL;
						}else
						{
							if(ns[1] == NULL)
							{
								parser->has_error = true;
								expr->table_init.field_lst = NULL;
							}else
							{
								expr->table_init.field_lst = ns[1]->field;
								ns[1]->field = NULL;
								AR_ASSERT(expr->table_init.field_lst != NULL);
							}
						}
						
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);
						return ret;

					}
}




/*filed_list	:	filed_list , filed */
static psrNode_t* AR_STDCALL on_filed_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						/*tguParser_t 	*parser = (tguParser_t*)ctx;*/
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						tguTableField_t *lst, *field, *tmp;

						lst = ns[0]->field; 
						field= ns[2]->field;
						ns[2]->field= NULL;
						AR_ASSERT(lst != NULL);
						
						for(tmp = lst; tmp->next != NULL; tmp = tmp->next);
						tmp->next = field;
						
						ret = ns[0];
						ns[0] = NULL;
						return ret;
					}
}




/*import_statement	:	import STRING ; */
/*import_statement	:	import error ; */
static psrNode_t* AR_STDCALL on_import_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
					tguParser_t 	*parser = (tguParser_t*)ctx;
					tguSynNode_t	**ns = (tguSynNode_t**)nodes;
					AR_ASSERT(parser != NULL);
					TGU_ReportWarning(&parser->report, L"", ns[0]->token.lex_info.linenum);
					return NULL;
				}
}




/*compound_statement	:	start_block compound_element_list } */
static psrNode_t* AR_STDCALL on_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
					tguParser_t 	*parser = (tguParser_t*)ctx;
					tguSynNode_t	**ns = (tguSynNode_t**)nodes;
					tguSynNode_t	*ret;
					tguBlock_t		*cb = NULL;
					tguStmt_t *stmt;
						
					AR_ASSERT(nodes != NULL && (count == 2));
					cb = pop_block(parser);
						
					stmt = make_compound_stmt(parser, cb, &ns[0]->token.lex_info,&ns[1]->token.lex_info);
					ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
					return ret;
				}
}




/*compound_statement	:	start_block error } */
static psrNode_t* AR_STDCALL on_compound_error_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t 	*parser = (tguParser_t*)ctx;
						/*tguSynNode_t	**ns = (tguSynNode_t**)nodes;*/
						tguBlock_t		*cb = NULL;
						AR_ASSERT(nodes != NULL && (count == 2));
						AR_ASSERT(parser != NULL);
						parser->has_error = true;
						cb = pop_block(parser);
						TGU_DestroyBlock(cb);
						return NULL;
				}
}




/*compound_statement	:	{ } */
static psrNode_t* AR_STDCALL on_empty_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguSynNode_t	*ret;
						tguStmt_t *stmt;
						tguParser_t	*parser = (tguParser_t*)ctx;
						AR_ASSERT(ns != NULL && count == 1);
						AR_ASSERT(parser != NULL);
						stmt = make_compound_stmt(parser, NULL, &ns[0]->token.lex_info,&ns[1]->token.lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
						return ret;
						
				}
}




/*start_block	:	{ */
static psrNode_t* AR_STDCALL on_start_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
							tguParser_t 	*parser = (tguParser_t*)ctx;
							tguSynNode_t	**ns = (tguSynNode_t**)nodes;
							tguSynNode_t	*ret;
							AR_ASSERT(ns != NULL && count == 1);
							ret = ns[0]; 
							ns[0] = NULL;
							push_block(parser);
							return ret;
				}
}




/*compound_element	:	statement */
static psrNode_t* AR_STDCALL on_compound_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = (tguSynNode_t**)nodes;
						tguParser_t	*parser = (tguParser_t*)ctx;
						AR_ASSERT(ns != NULL && count == 1 && ns[0]->type == TGU_NODE_STMT_T);
						if(ns[0] == NULL)
						{
								parser->has_error = true;
								return NULL;
						}
						

						AR_ASSERT(ns[0]->stmt);
						TGU_InsertStmtToBlock(parser->top_block, ns[0]->stmt);
						ns[0]->stmt = NULL;
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

						stmt = make_empty_statement(parser, &ns[0]->token.lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
						return ret;
					}
}




/*expression_statement	:	expression semi */
static psrNode_t* AR_STDCALL on_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
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
							AR_ASSERT(expr != NULL);
						}
							
						if(ns[4] == NULL)
						{	
							if_true = NULL;
							parser->has_error = true;
						}else
						{
							if_true = ns[4]->stmt;
							ns[4]->stmt = NULL;
							AR_ASSERT(if_true != NULL);
						}

						stmt = make_if_statement(parser, expr, if_true, NULL, &ns[0]->token.lex_info);
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
						AR_ASSERT(ns != NULL && count == 7);
						if(ns[2] == NULL)
						{
							expr = NULL;
							parser->has_error = true;
						}else
						{
							expr = ns[2]->expr;
							ns[2]->expr = NULL;
							AR_ASSERT(expr != NULL);
						}
							
						if(ns[4] == NULL)
						{	
							if_true = NULL;
							parser->has_error = true;
						}else
						{
							if_true = ns[4]->stmt;
							ns[4]->stmt = NULL;
							AR_ASSERT(if_true != NULL);
						}

						if(ns[6] == NULL)
						{	
							if_false = NULL;
							parser->has_error = true;
						}else
						{
							if_false = ns[6]->stmt;
							ns[6]->stmt = NULL;
							AR_ASSERT(if_false != NULL);
						}


						stmt = make_if_statement(parser, expr, if_true, if_false,  &ns[0]->token.lex_info);	
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
						AR_ASSERT(ns != NULL && count == 7);
						if(ns[3] == NULL)
						{
							expr = NULL;
							parser->has_error = true;
						}else
						{
							expr = ns[3]->expr;
							ns[3]->expr = NULL;
							AR_ASSERT(expr != NULL);
						}
							
						if(ns[5] == NULL)
						{	
							stmt = NULL;
							parser->has_error = true;
						}else
						{
							stmt = ns[5]->stmt;
							ns[5]->stmt = NULL;
							AR_ASSERT(stmt != NULL);
						}

						stmt = make_while_statement(parser, expr, stmt, &ns[0]->token.lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
						return ret;
					}
}




/*do_while_statement	:	do enter_loop statement while ( expression ) leave_loop semi */
/*do_while_statement	:	do enter_loop statement while ( error ) leave_loop semi */
static psrNode_t* AR_STDCALL on_do_while_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t	**ns = 	(tguSynNode_t**)nodes;
						tguParser_t	*parser =	(tguParser_t*)ctx;
						tguSynNode_t	*ret;
						tguExpr_t		*expr;
						tguStmt_t		*stmt;
						AR_ASSERT(ns != NULL && count == 9);
						if(ns[2] == NULL)
						{
							stmt = NULL;
							parser->has_error = true;
						}else
						{
							stmt = ns[2]->stmt;
							ns[2]->stmt = NULL;
							AR_ASSERT(stmt != NULL);
						}
							
						if(ns[5] == NULL)
						{	
							expr = NULL;
							parser->has_error = true;
						}else
						{
							expr = ns[5]->expr;
							ns[5]->expr = NULL;
							AR_ASSERT(expr != NULL);
						}

						stmt = make_do_while_statement(parser, expr, stmt, &ns[0]->token.lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
						return ret;
					}
}




/*for_statement	:	for ( for_expression ; for_expression ; for_expression ) enter_loop statement leave_loop */
/*for_statement	:	for ( error ) enter_loop statement leave_loop */
static psrNode_t* AR_STDCALL on_for_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t		**ns = (tguSynNode_t**)nodes;
						tguParser_t		*parser = (tguParser_t*)ctx;
						tguSynNode_t		*ret;
						tguExpr_t		*init, *cond, *step;
						tguStmt_t		*loop;
						tguStmt_t		*stmt;

						AR_ASSERT(ns != NULL && count == 11);

						if(ns[2] == NULL)
						{
							init = NULL;
						}else
						{
							init = ns[2]->expr;
							ns[2]->expr = NULL;
							AR_ASSERT(init != NULL);
						}

						if(ns[4] == NULL)
						{
							cond = NULL;
						}else
						{
							cond = ns[4]->expr;
							ns[4]->expr = NULL;
							AR_ASSERT(cond != NULL);
						}
						

						if(ns[6] == NULL)
						{
							step = NULL;
						}else
						{
							step = ns[6]->expr;
							ns[6]->expr = NULL;
							AR_ASSERT(step != NULL);
						}

						if(ns[9] == NULL)
						{
							loop = NULL;
							parser->has_error = true;
						}else
						{
							loop = ns[9]->stmt;
							ns[9]->stmt = NULL;
							AR_ASSERT(loop != NULL);
						}

						stmt = make_for_statement(parser, init, cond, step, loop, &ns[0]->token.lex_info);
						
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
						stmt = make_jump_statement(parser, TGU_STT_CONTINUE, 	NULL, &ns[0]->token.lex_info);
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
						stmt = make_jump_statement(parser, TGU_STT_BREAK, 	NULL, &ns[0]->token.lex_info);
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
						AR_ASSERT(nodes != NULL && (count == 2 || count == 3));
						AR_ASSERT(parser->current_function != NULL);
						
						if(count == 2)
						{
							expr = NULL;
						}else
						{
							if(ns[1] == NULL)
							{
								expr = NULL;
								parser->has_error = true;
							}else
							{
								AR_ASSERT(ns[1]->type == TGU_NODE_EXPR_T);
								expr = ns[1]->expr;
								ns[1]->expr = NULL;
								AR_ASSERT(expr != NULL);
							}
						}

						stmt = make_jump_statement(parser, TGU_STT_RETURN, 	expr, &ns[0]->token.lex_info);
						ret = __create_synnode(TGU_NODE_STMT_T, (void*)stmt);
						return ret;
					}
}




/*semi	:	error */
static psrNode_t* AR_STDCALL on_semi_error(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguParser_t	*parser = (tguParser_t*)ctx;
						AR_ASSERT(parser != NULL);
						parser->has_error = true;
						return NULL;
					}
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
							AR_ASSERT(addr != NULL);
						}
						

						if(ns[2] == NULL)
						{
							parser->has_error = true;
							value = NULL;	
						}else
						{
							value = ns[2]->expr;
							ns[2]->expr = NULL;
							AR_ASSERT(value != NULL);
						}

						lex_info = addr != NULL ? &addr->lex_info : &ns[1]->token.lex_info;
						AR_ASSERT(lex_info != NULL);
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
							AR_ASSERT(cond != NULL);
						}

						

						if(ns[2] == NULL)
						{
							parser->has_error = true;
							if_true = NULL;	
						}else
						{
							if_true= ns[2]->expr;
							ns[2]->expr = NULL;
							AR_ASSERT(if_true != NULL);
						}
						
						if(ns[4] == NULL)
						{
							parser->has_error = true;
							if_false = NULL;	
						}else
						{
							if_false= ns[4]->expr;
							ns[4]->expr = NULL;
							AR_ASSERT(if_false != NULL);
						}

						lex_info = cond != NULL ? &cond->lex_info : &ns[1]->token.lex_info;
						AR_ASSERT(lex_info != NULL);
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

						tguExprOP_t	op;
						const tguLexInfo_t *lex_info;
						tguToken_t	tok;
						AR_ASSERT(ns != NULL && count == 3 && parser != NULL);

						if(ns[0] == NULL)
						{
							parser->has_error = true;
							left = NULL;	
						}else
						{
							left = ns[0]->expr;
							ns[0]->expr = NULL;
							AR_ASSERT(left != NULL);
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
							AR_ASSERT(right != NULL);
						}

						switch(tok.term_val)
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

						lex_info  = left != NULL ? &left->lex_info : &tok.lex_info;
						AR_ASSERT(lex_info != NULL);
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

						tguExprOP_t		op;
						tguToken_t		tok;
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
							AR_ASSERT(expr != NULL);
						}

						AR_ASSERT(ns[0] != NULL);
						tok = ns[0]->token;	
						
						switch(tok.term_val)
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
						lex_info = expr != NULL ? &expr->lex_info : &tok.lex_info;
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
						tguSynNode_t		**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 		*ret;
						tguParser_t		*parser = (tguParser_t*)ctx;
						tguExpr_t		*expr;
						tguToken_t		tok;
						tguExprOP_t		op;
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
							AR_ASSERT(expr != NULL);
						}

						AR_ASSERT(ns[1] != NULL );
						tok = ns[1]->token;	
							
						switch(tok.term_val)	
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
						
						lex_info = expr != NULL ? &expr->lex_info : &tok.lex_info;
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
							AR_ASSERT(expr != NULL);
						}

						if(ns[2] == NULL)
						{
							index_expr = NULL;
							parser->has_error = true;
						}else
						{
							index_expr = ns[2]->expr;
							ns[2]->expr = NULL;
							AR_ASSERT(index_expr != NULL);
						}

						lex_info = expr != NULL ? &expr->lex_info : &ns[1]->token.lex_info;
						expr = make_index_expression(parser, expr, index_expr, lex_info);
						ret = __create_synnode(TGU_NODE_EXPR_T, (void*)expr);
						return ret;
				}
}




/*postfix_expression	:	postfix_expression . NAME */
static psrNode_t* AR_STDCALL on_access_name_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 {
						tguSynNode_t		**ns = (tguSynNode_t**)nodes;
						tguSynNode_t		*ret;
						tguParser_t		*parser = (tguParser_t*)ctx;
						tguExpr_t		*expr, *index_expr;
						tguToken_t		tmp;
						wchar_t		*buf;
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
							AR_ASSERT(expr != NULL);
						}

						tmp = ns[2]->token;
						tmp.term_val = TOK_STRING;
						buf = AR_NEWARR(wchar_t, AR_wcslen(ns[2]->token.token) + 2 + 1);
						buf[0] = L'\0';
						AR_wcscat(buf, L"\"");
						AR_wcscat(buf, ns[2]->token.token);
						AR_wcscat(buf, L"\"");
						tmp.token = buf;
						index_expr = make_constant_expression(parser, &tmp);
						AR_ASSERT(index_expr != NULL);
						tmp.token = NULL;
						AR_DEL(buf);
						buf = NULL;

						lex_info = expr != NULL ? &expr->lex_info : &index_expr->lex_info;
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
						tguSynNode_t		**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 		*ret;
						tguParser_t		*parser = (tguParser_t*)ctx;
						tguExpr_t		*expr;
						AR_ASSERT(parser != NULL && ns != NULL && count == 1);
						expr = make_identifier_expression(parser, &ns[0]->token);
						
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
						expr = make_constant_expression(parser, &ns[0]->token);
						
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
						tguSynNode_t		**ns = (tguSynNode_t**)nodes;
						tguSynNode_t 		*ret;
						tguParser_t	*parser = (tguParser_t*)ctx;
						tguExpr_t		*expr, *call_expr, *args;
						const tguLexInfo_t	*lex_info;

						/*如果postfix_expression 为空，则表明此call表达式错误*/
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
							if(ns[2] == NULL)/*对应"(" error ")"*/
							{
								args = NULL;
								parser->has_error = true;
							}else
							{
								args = ns[2]->expr;
								ns[2]->expr = NULL;
								AR_ASSERT(args != NULL);
							}
						}else
						{
						
							AR_ASSERT(count == 3);
							args = NULL;
						}
						
						lex_info = call_expr != NULL ? &call_expr->lex_info : &ns[1]->token.lex_info;
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
						if(ns[2] == NULL) return ns[0];
						AR_ASSERT(ns[0]->expr != NULL  && ns[2]->expr != NULL);

						for(lst = ns[0]->expr; lst->next != NULL; lst = lst->next);
						
						lst->next = ns[2]->expr;
						ns[2]->expr = NULL;
						ret = ns[0];
						ns[0] = NULL;
						return ret;
				}
}




