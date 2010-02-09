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




#include "rayparser_impl.h"


AR_NAMESPACE_BEGIN

#if(0)
ELLIPSIS,		/*...*/
AUTO ,			/*auto*/
EXTERN,			/*extern*/
REGISTER,		/*register*/
VOLATILE,		/*volatile*/
STATIC,			/*static*/
ENUM,			/*enum*/
UNION,			/*union*/

		{L"auto", AUTO, 1, L"\"auto\"(?!{keyword_lhd})", false},
		{L"extern", EXTERN, 1, L"\"extern\"(?!{keyword_lhd})", false},
		{L"register", REGISTER, 1, L"\"register\"(?!{keyword_lhd})", false},
		{L"volatile", VOLATILE, 1, L"\"volatile\"(?!{keyword_lhd})", false},
		{L"static", STATIC, 1, L"\"static\"(?!{keyword_lhd})", false},
		{L"union", UNION, 1, L"\"union\"(?!{keyword_lhd})", false},
		{L"enum", ENUM, 1, L"\"enum\"(?!{keyword_lhd})", false},
		{L"...", ELLIPSIS, 1, L"\"...\"", false},

#endif


		
typedef enum
{
		EOI		=		0x00,
		DELIM	=		0x1000,	
		
		SWITCH,			/*switch*/
		FOR,			/*for*/
		GOTO,			/*goto*/
		RETURN,			/*return*/
		DO,				/*do*/
		WHILE,			/*while*/
		IF,				/*if*/
		ELSE,			/*else*/

		CONTINUE,		/*continue*/
		DEFAULT,		/*default*/
		CASE,			/*case*/
		BREAK,			/*break*/
		CONSTANT,		/*const*/
		SIZEOF,			/*size*/
		STRUCT,			/*struct*/
		
		TYPEDEF,		/*typedef*/
		
		ATTRIBUTE,		/*attribute*/

		SIGNED,			/*signed*/
		UNSIGNED,		/*unsigned*/
		VOID_TYPE,		/*void*/
		BYTE_TYPE,		/*byte*/
		SHORT_TYPE,		/*short*/
		INT_TYPE,		/*int*/
		LONG_TYPE,		/*long*/
		CHAR_TYPE,		/*char*/
		FLOAT_TYPE,		/*float*/
		DOUBLE_TYPE,	/*double*/
		
		RIGHT_ASSIGN,	/*>>=*/
		LEFT_ASSIGN,	/*<<=*/
		ADD_ASSIGN,		/*+=*/
		SUB_ASSIGN,		/*-=*/
		MUL_ASSIGN,		/**=*/
		DIV_ASSIGN,		/*/=*/
		MOD_ASSIGN,		/*%=*/
		AND_ASSIGN,		/*&=*/
		XOR_ASSIGN,		/*^=*/
		OR_ASSIGN,		/*|=*/
		INC_OP,			/*++*/
		DEC_OP,			/*--*/
		PTR_OP,			/*->*/
		AND_OP,			/*&&*/
		OR_OP,			/*||*/

		RIGHT_OP,			/*>>*/
		LEFT_OP,			/*<<*/		
		
		LE_OP,				/*<=*/
		GE_OP,				/*>=*/		
		EQ_OP,				/*==*/
		NE_OP,				/*!=*/			
		LESS_OP,			/*<*/
		GREATER_OP,		/*>*/

		AND,			/*&*/
		XOR,			/*^*/
		OR,				/*|*/

		NOT,			/*!*/
		NEGA,			/* ~ */
		ADD,			/* + */
		SUB,			/* - */
		MUL,			/* * */
		DIV,			/* / */
		MOD,			/* % */
		
		SEMICOLON,		/*;*/
		L_BRACES,		/*{*/
		R_BRACES,		/*}*/
		L_SQUARE,		/*[*/
		R_SQUARE,		/*]*/
		L_PAREN,		/*(*/
		R_PAREN,		/*)*/
		COMMA,			/*,*/
		COLON,			/*:*/
		ASSIGN,			/*=*/
		DOT,			/*.*/
		QUEST,			/*?*/

		STRING_LITERAL,	/*STRING_LITERAL*/
		OCT_CONSTANT,	/*OCT_CONSTANT*/
		HEX_CONSTANT,	/*HEX_CONSTANT*/
		DEC_CONSTANT,	/*DEC_CONSTANT*/
		FLOAT_CONSTANT,	/*FLOAT_CONSTANT*/
		CHAR_CONSTANT,	/*CHAR_CONSTANT*/
		IDENTIFIER,		/*IDENTIFIER*/

		TYPE_ID,		/*占位符，提取出的lexeme需要到符号表检查是否为一个类型*/
		FAKE_EOI,		/*占位符, 伪终结符*/
		
		/*以下为优先级符号*/
		IF_STMT_PREC,					/*if(stmt)...*/
		IF_STMT_ELSE_STMT_PREC,			/*if(stmt)else stmt....*/
		SPEC_ACTION,					/*特殊优先级*/
		ERROR_SHIFT
}rayTokValue_t;



static const wchar_t *__g_lex_name[] = 
{
		L"delim = [ \\r\\n\\t]",
		L"comment = /\\*([^\\*]|\\*+[^\\*/])*\\*+/",
		L"comment_line = //[^\\n]*\\n",
		L"digit = [0-9]",
		L"number = {digit}+",
		L"letter = [A-Z_a-z]",
		L"int_type_suffix = ((u|U)?(l|L))|((u|U)(l|L)?)",
		L"octal_escape = (\\\\[0-3][0-7][0-7])|(\\\\[0-7][0-7])|(\\\\[0-7])",
		L"hex_digit = [0-9a-fA-F]",
		L"hex_literal = 0(x|X){hex_digit}+{int_type_suffix}?",
		L"dec_literal = (0|[1-9][0-9]*){int_type_suffix}?",
		L"oct_literal = 0[0-7]+{int_type_suffix}?",
		L"exponet = (e|E)(\\+|\\-)?[0-9]+",
		L"float_type_suffix = (f|F|d|D)",
		L"float_literal = ([0-9]+\\.[0-9]*{exponet}?{float_type_suffix}?)",
		L"skip_lexem = {delim}|{comment}|{comment_line}",
		L"escape_seq = (\\\\(b|t|n|f|r|\\x22|\\x27|\\\\))|{octal_escape}",
		L"string_literal = (\\x22({escape_seq}|[^\\x22\\\\])*\\x22)",
		L"char_literal = \\x27({escape_seq}|[^\\x27\\\\])\\x27",
		L"keyword_lhd = [A-Z_a-z0-9]"
};

		
static struct {
		const wchar_t *name; 
		size_t tokval; 
		size_t lex_prec; 
		const wchar_t *regex; 
		bool_t skip; 
}__g_term_pattern[] = 
{
		{NULL, DELIM, 1, L"{delim}+|{comment}+|{comment_line}+", true},
		{L"EOI", EOI, 2, L"$", false},
		{L"STRING_LITERAL", STRING_LITERAL, 0, L"{string_literal}", false},
		{L"OCT_CONSTANT", OCT_CONSTANT, 0, L"{oct_literal}", false},
		{L"HEX_CONSTANT", HEX_CONSTANT, 0, L"{hex_literal}", false},
		{L"DEC_CONSTANT", DEC_CONSTANT, 0, L"{dec_literal}", false},
		{L"FLOAT_CONSTANT", FLOAT_CONSTANT, 0, L"{float_literal}", false},
		{L"CHAR_CONSTANT", CHAR_CONSTANT, 0, L"{char_literal}", false},
		{L"IDENTIFIER", IDENTIFIER, 0, L"{letter}({letter}|{digit})*", false},

		{L"switch", SWITCH, 1, L"\"switch\"(?!{keyword_lhd})", false},
		{L"for", FOR, 1, L"\"for\"(?!{keyword_lhd})", false},
		{L"goto", GOTO, 1, L"\"goto\"(?!{keyword_lhd})", false},
		{L"return", RETURN, 1, L"\"return\"(?!{keyword_lhd})", false},
		{L"do", DO, 1, L"\"do\"(?!{keyword_lhd})", false},
		{L"while", WHILE, 1, L"\"while\"(?!{keyword_lhd})", false},
		{L"if", IF, 1, L"\"if\"(?!{keyword_lhd})", false},
		{L"else", ELSE, 1, L"\"else\"(?!{keyword_lhd})", false},
		
		{L"continue", CONTINUE, 1, L"\"continue\"(?!{keyword_lhd})", false},
		{L"default", DEFAULT, 1, L"\"default\"(?!{keyword_lhd})", false},
		{L"case", CASE, 1, L"\"case\"(?!{keyword_lhd})", false},
		{L"break", BREAK, 1, L"\"break\"(?!{keyword_lhd})", false},
		{L"const", CONSTANT, 1, L"\"const\"(?!{keyword_lhd})", false},
		{L"sizeof", SIZEOF, 1, L"\"sizeof\"(?!{keyword_lhd})", false},
		{L"struct", STRUCT, 1, L"\"struct\"(?!{keyword_lhd})", false},
		
		{L"typedef", TYPEDEF, 1, L"\"typedef\"(?!{keyword_lhd})", false},
		{L"void", VOID_TYPE, 1, L"\"void\"(?!{keyword_lhd})", false},
		{L"signed", SIGNED, 1, L"\"signed\"(?!{keyword_lhd})", false},
		{L"unsigned", UNSIGNED, 1, L"\"unsigned\"(?!{keyword_lhd})", false},
		{L"byte", BYTE_TYPE, 1, L"\"byte\"(?!{keyword_lhd})", false},
		{L"char", CHAR_TYPE, 1, L"\"char\"(?!{keyword_lhd})", false},
		{L"short", SHORT_TYPE, 1, L"\"short\"(?!{keyword_lhd})", false},
		{L"int", INT_TYPE, 1, L"\"int\"(?!{keyword_lhd})", false},
		{L"long", LONG_TYPE, 1, L"\"long\"(?!{keyword_lhd})", false},
		{L"float", FLOAT_TYPE, 1, L"\"float\"(?!{keyword_lhd})", false},
		{L"double", DOUBLE_TYPE, 1, L"\"double\"(?!{keyword_lhd})", false},
		{L"__attribute__", ATTRIBUTE, 1, L"\"__attribute__\"", false},

		{L">>=", RIGHT_ASSIGN, 2, L"\">>=\"", false},
		{L"<<=", LEFT_ASSIGN, 2, L"\"<<=\"", false},
		{L"+=", ADD_ASSIGN, 1, L"\"+=\"", false},
		{L"-=", SUB_ASSIGN, 1, L"\"-=\"", false},
		{L"*=", MUL_ASSIGN, 1, L"\"*=\"", false},
		{L"/=", DIV_ASSIGN, 1, L"\"/=\"", false},
		{L"%=", MOD_ASSIGN, 1, L"\"%=\"", false},
		{L"&=", AND_ASSIGN, 1, L"\"&=\"", false},
		{L"^=", XOR_ASSIGN, 1, L"\"^=\"", false},
		{L"|=", OR_ASSIGN, 1, L"\"|=\"", false},
		{L">>", RIGHT_OP, 1, L"\">>\"", false},
		{L"<<", LEFT_OP, 1, L"\"<<\"", false},
		{L"++", INC_OP, 1, L"\"++\"", false},
		{L"--", DEC_OP, 1, L"\"--\"", false},
		{L"->", PTR_OP, 1, L"\"->\"", false},
		{L"&&", AND_OP, 1, L"\"&&\"", false},
		{L"||", OR_OP, 1, L"\"||\"", false},
		{L"<=", LE_OP, 1, L"\"<=\"", false},
		{L">=", GE_OP, 1, L"\">=\"", false},
		{L"==", EQ_OP, 1, L"\"==\"", false},
		{L"!=", NE_OP, 1, L"\"!=\"", false},
		{L";", SEMICOLON, 0, L"\";\"", false},
		{L"{", L_BRACES, 0, L"\"{\"", false},
		{L"}", R_BRACES, 0, L"\"}\"", false},
		{L",", COMMA, 0, L"\",\"", false},
		{L":", COLON, 0, L"\":\"", false},
		{L"=", ASSIGN, 0, L"\"=\"", false},
		{L"(", L_PAREN, 0, L"\"(\"", false},
		{L")", R_PAREN, 0, L"\")\"", false},
		{L"[", L_SQUARE, 0, L"\"[\"", false},
		{L"]", R_SQUARE, 0, L"\"]\"", false},
		{L".", DOT, 0, L"\".\"", false},
		{L"&", AND, 0, L"\"&\"", false},
		{L"!", NOT, 0, L"\"!\"", false},
		{L"~", NEGA, 0, L"\"~\"", false},
		{L"-", ADD, 0, L"\"-\"", false},
		{L"+", SUB, 0, L"\"+\"", false},
		{L"*", MUL, 0, L"\"*\"", false},
		{L"/", DIV, 0, L"\"/\"", false},
		{L"%", MOD, 0, L"\"%\"", false},
		{L"<", LESS_OP, 0, L"\"<\"", false},
		{L">", GREATER_OP, 0, L"\">\"", false},
		{L"^", XOR, 0, L"\"^\"", false},
		{L"|", OR, 0, L"\"|\"", false},
		{L"?", QUEST, 0, L"\"?\"", false},
		{L"TYPE_ID", TYPE_ID, 0, L"^", false},
		{L"FAKE_EOI", FAKE_EOI, 0, L"^", false}
};

static psrNode_t*		AR_STDCALL ray_handle_token(const psrToken_t *tok,void *ctx);



static struct {
		const wchar_t *name; 
		size_t tokval; 
		size_t prec_level; 
		psrAssocType_t	assoc;
}__g_prec_pattern[] = 
{
		{L"||", OR_OP,			1, PSR_ASSOC_LEFT},
		{L"&&", AND_OP,			2, PSR_ASSOC_LEFT},
		{L"|", OR,				3, PSR_ASSOC_LEFT},
		{L"^", XOR,				4, PSR_ASSOC_LEFT},
		{L"&", AND,				5, PSR_ASSOC_LEFT},
		{L"==", EQ_OP,			6, PSR_ASSOC_LEFT},
		{L"!=", NE_OP,			6, PSR_ASSOC_LEFT},
		{L"<", LESS_OP,			7, PSR_ASSOC_LEFT},
		{L"<=", LE_OP,			7, PSR_ASSOC_LEFT},
		{L">", GREATER_OP,		7, PSR_ASSOC_LEFT},
		{L">=", GE_OP,			7, PSR_ASSOC_LEFT},
		{L"<<", LEFT_OP,		8, PSR_ASSOC_LEFT},
		{L">>", RIGHT_OP,		8, PSR_ASSOC_LEFT},
		{L"+", ADD,				9, PSR_ASSOC_LEFT},
		{L"-", SUB,				9, PSR_ASSOC_LEFT},
		{L"*", MUL,				10, PSR_ASSOC_LEFT},
		{L"/", DIV,				10, PSR_ASSOC_LEFT},
		{L"%", MOD,				10, PSR_ASSOC_LEFT},
		
		{L"IF_STMT_PREC", IF_STMT_PREC,						11, PSR_ASSOC_NOASSOC},
		{L"IF_STMT_ELSE_STMT_PREC", IF_STMT_ELSE_STMT_PREC, 12, PSR_ASSOC_NOASSOC},
		{L"SPEC_ACTION", SPEC_ACTION,						13, PSR_ASSOC_NOASSOC},
		{L"ERROR_SHIFT", ERROR_SHIFT, 14, PSR_ASSOC_RIGHT}
};



static psrNode_t* AR_STDCALL handle_program(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*program	:	translation_unit */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*translation_unit	:	external_declaration */
/*translation_unit	:	translation_unit external_declaration */
static psrNode_t* AR_STDCALL handle_external_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*external_declaration	:	function_definition */
/*external_declaration	:	declaration */
static psrNode_t* AR_STDCALL handle_function_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_definition	:	declaration_specifiers declarator compound_statement */
static psrNode_t* AR_STDCALL handle_type_qualifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_qualifier	:	const */
static psrNode_t* AR_STDCALL handle_type_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_specifier	:	void */
/*type_specifier	:	byte */
/*type_specifier	:	char */
/*type_specifier	:	short */
/*type_specifier	:	int */
/*type_specifier	:	long */
/*type_specifier	:	float */
/*type_specifier	:	double */
/*type_specifier	:	signed */
/*type_specifier	:	unsigned */
/*type_specifier	:	struct_specifier */
/*type_specifier	:	TYPE_ID */
static psrNode_t* AR_STDCALL handle_struct_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*struct_specifier	:	struct IDENTIFIER { struct_declaration_list } */
/*struct_specifier	:	struct IDENTIFIER { error } */
static psrNode_t* AR_STDCALL handle_struct_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*struct_declaration	:	type_specifier struct_declarator_list ; */
static psrNode_t* AR_STDCALL handle_struct_declaration_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*struct_declaration_list	:	struct_declaration */
/*struct_declaration_list	:	struct_declaration_list struct_declaration */
static psrNode_t* AR_STDCALL handle_struct_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*struct_declarator_list	:	declarator */
/*struct_declarator_list	:	struct_declarator_list , declarator */
static psrNode_t* AR_STDCALL handle_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration	:	declaration_specifiers init_declarator_list ; */
/*declaration	:	declaration_specifiers ; */
/*declaration	:	declaration_specifiers error ; */
/*declaration	:	declaration_specifiers error */
static psrNode_t* AR_STDCALL handle_declaration_specifiers(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration_specifiers	:	typedef type_qualifier type_specifier */
/*declaration_specifiers	:	typedef type_specifier */
/*declaration_specifiers	:	type_qualifier type_specifier */
/*declaration_specifiers	:	type_specifier */
static psrNode_t* AR_STDCALL handle_declaration_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration_list	:	declaration */
/*declaration_list	:	declaration_list declaration */
static psrNode_t* AR_STDCALL handle_init_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator	:	declarator */
/*init_declarator	:	declarator = initializer */
static psrNode_t* AR_STDCALL handle_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*initializer	:	assignment_expression */
/*initializer	:	{ initializer_list } */
static psrNode_t* AR_STDCALL handle_initializer_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*initializer_list	:	initializer */
/*initializer_list	:	initializer_list , initializer */
static psrNode_t* AR_STDCALL handle_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declarator	:	pointer direct_declarator */
/*declarator	:	direct_declarator */
static psrNode_t* AR_STDCALL handle_direct_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*direct_declarator	:	IDENTIFIER */
/*direct_declarator	:	IDENTIFIER [ constant_expression ] */
/*direct_declarator	:	IDENTIFIER [ ] */
/*direct_declarator	:	IDENTIFIER ( parameter_list ) */
/*direct_declarator	:	IDENTIFIER ( ) */
static psrNode_t* AR_STDCALL handle_pointer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*pointer	:	* */
/*pointer	:	* type_qualifier */
/*pointer	:	* type_qualifier pointer */
/*pointer	:	* pointer */
static psrNode_t* AR_STDCALL handle_parameter_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*parameter_list	:	parameter_declaration */
/*parameter_list	:	parameter_list , parameter_declaration */
static psrNode_t* AR_STDCALL handle_parameter_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*parameter_declaration	:	declaration_specifiers declarator */
/*parameter_declaration	:	declaration_specifiers pointer */
/*parameter_declaration	:	declaration_specifiers */
static psrNode_t* AR_STDCALL handle_type_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_name	:	specifier_qualifier_list */
/*type_name	:	specifier_qualifier_list pointer */
static psrNode_t* AR_STDCALL handle_specifier_qualifier_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*specifier_qualifier_list	:	type_qualifier type_specifier */
/*specifier_qualifier_list	:	type_specifier */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement	:	labeled_statement */
/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
/*statement	:	error */
static psrNode_t* AR_STDCALL handle_labeled_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*labeled_statement	:	case constant_expression : statement */
/*labeled_statement	:	default : statement */
/*labeled_statement	:	IDENTIFIER : statement */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	{ } */
/*compound_statement	:	{ statement_list } */
/*compound_statement	:	{ push_symtbl declaration_list pop_symtbl } */
/*compound_statement	:	{ push_symtbl declaration_list statement_list pop_symtbl } */
/*compound_statement	:	{ push_symtbl declaration_list error pop_symtbl } */
/*compound_statement	:	{ error } */
static psrNode_t* AR_STDCALL handle_push_symtbl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*push_symtbl	:	 */
static psrNode_t* AR_STDCALL handle_pop_symtbl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*pop_symtbl	:	 */
static psrNode_t* AR_STDCALL handle_statement_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement_list	:	statement */
/*statement_list	:	statement_list statement */
static psrNode_t* AR_STDCALL handle_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	; */
/*expression_statement	:	expression ; */
static psrNode_t* AR_STDCALL handle_selection_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*selection_statement	:	if ( expression ) statement */
/*selection_statement	:	if ( expression ) statement else statement */
/*selection_statement	:	switch ( expression ) statement */
static psrNode_t* AR_STDCALL handle_iteration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*iteration_statement	:	while ( expression ) statement */
/*iteration_statement	:	do statement while ( expression ) ; */
/*iteration_statement	:	for ( expression_statement expression_statement ) statement */
/*iteration_statement	:	for ( expression_statement expression_statement expression ) statement */
static psrNode_t* AR_STDCALL handle_jump_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*jump_statement	:	goto IDENTIFIER ; */
/*jump_statement	:	continue ; */
/*jump_statement	:	break ; */
/*jump_statement	:	return ; */
/*jump_statement	:	return expression ; */
static psrNode_t* AR_STDCALL handle_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression	:	assignment_expression */
/*expression	:	expression , assignment_expression */
static psrNode_t* AR_STDCALL handle_primary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	IDENTIFIER */
/*primary_expression	:	constant */
/*primary_expression	:	( expression ) */
static psrNode_t* AR_STDCALL handle_postfix_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	primary_expression */
/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression ( ) */
/*postfix_expression	:	postfix_expression ( argument_expression_list ) */
/*postfix_expression	:	postfix_expression . IDENTIFIER */
/*postfix_expression	:	postfix_expression -> IDENTIFIER */
/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
static psrNode_t* AR_STDCALL handle_argument_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*argument_expression_list	:	assignment_expression */
/*argument_expression_list	:	argument_expression_list , assignment_expression */
static psrNode_t* AR_STDCALL handle_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_expression	:	postfix_expression */
/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
/*unary_expression	:	unary_operator cast_expression */
/*unary_expression	:	sizeof unary_expression */
/*unary_expression	:	sizeof ( type_name ) */
static psrNode_t* AR_STDCALL handle_unary_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_operator	:	& */
/*unary_operator	:	* */
/*unary_operator	:	+ */
/*unary_operator	:	- */
/*unary_operator	:	~ */
/*unary_operator	:	! */
static psrNode_t* AR_STDCALL handle_cast_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*cast_expression	:	unary_expression */
/*cast_expression	:	( type_name ) cast_expression */
static psrNode_t* AR_STDCALL handle_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*binary_expression	:	binary_expression + binary_expression */
/*binary_expression	:	binary_expression - binary_expression */
/*binary_expression	:	binary_expression * binary_expression */
/*binary_expression	:	binary_expression / binary_expression */
/*binary_expression	:	binary_expression % binary_expression */
/*binary_expression	:	binary_expression << binary_expression */
/*binary_expression	:	binary_expression >> binary_expression */
/*binary_expression	:	binary_expression < binary_expression */
/*binary_expression	:	binary_expression <= binary_expression */
/*binary_expression	:	binary_expression > binary_expression */
/*binary_expression	:	binary_expression >= binary_expression */
/*binary_expression	:	binary_expression == binary_expression */
/*binary_expression	:	binary_expression != binary_expression */
/*binary_expression	:	binary_expression & binary_expression */
/*binary_expression	:	binary_expression ^ binary_expression */
/*binary_expression	:	binary_expression | binary_expression */
/*binary_expression	:	binary_expression && binary_expression */
/*binary_expression	:	binary_expression || binary_expression */
/*binary_expression	:	cast_expression */
static psrNode_t* AR_STDCALL handle_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*constant_expression	:	binary_expression */
/*constant_expression	:	binary_expression ? expression : constant_expression */
static psrNode_t* AR_STDCALL handle_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*assignment_expression	:	constant_expression */
/*assignment_expression	:	unary_expression assignment_operator assignment_expression */
static psrNode_t* AR_STDCALL handle_assignment_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*assignment_operator	:	= */
/*assignment_operator	:	*= */
/*assignment_operator	:	/= */
/*assignment_operator	:	%= */
/*assignment_operator	:	+= */
/*assignment_operator	:	-= */
/*assignment_operator	:	<<= */
/*assignment_operator	:	>>= */
/*assignment_operator	:	&= */
/*assignment_operator	:	^= */
/*assignment_operator	:	|= */
static psrNode_t* AR_STDCALL handle_constant(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*constant	:	OCT_CONSTANT */
/*constant	:	HEX_CONSTANT */
/*constant	:	DEC_CONSTANT */
/*constant	:	FLOAT_CONSTANT */
/*constant	:	CHAR_CONSTANT */
/*constant	:	STRING_LITERAL */



static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"program  :  translation_unit ", NULL, handle_program, 0},
{L"translation_unit  :  external_declaration ", NULL, handle_translation_unit, 0},
{L"translation_unit  :  translation_unit external_declaration ", NULL, handle_translation_unit, 0},
{L"external_declaration  :  function_definition ", NULL, handle_external_declaration, 0},
{L"external_declaration  :  declaration ", NULL, handle_external_declaration, 0},
{L"function_definition  :  declaration_specifiers declarator compound_statement ", NULL, handle_function_definition, 0},
{L"type_qualifier  :  const ", NULL, handle_type_qualifier, 0},
{L"type_specifier  :  void ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  byte ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  char ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  short ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  int ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  long ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  float ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  double ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  signed ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  unsigned ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  struct_specifier ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  TYPE_ID ", NULL, handle_type_specifier, 0},
{L"struct_specifier  :  struct IDENTIFIER { struct_declaration_list } ", NULL, handle_struct_specifier, 0},
{L"struct_specifier  :  struct IDENTIFIER { error } ", NULL, handle_struct_specifier, 0},
{L"struct_declaration  :  type_specifier struct_declarator_list ; ", NULL, handle_struct_declaration, 0},
{L"struct_declaration_list  :  struct_declaration ", NULL, handle_struct_declaration_list, 0},
{L"struct_declaration_list  :  struct_declaration_list struct_declaration ", NULL, handle_struct_declaration_list, 0},
{L"struct_declarator_list  :  declarator ", NULL, handle_struct_declarator_list, 0},
{L"struct_declarator_list  :  struct_declarator_list , declarator ", NULL, handle_struct_declarator_list, 0},
{L"declaration  :  declaration_specifiers init_declarator_list ; ", NULL, handle_declaration, 0},
{L"declaration  :  declaration_specifiers ; ", NULL, handle_declaration, 0},
{L"declaration  :  declaration_specifiers error ; ", L"ERROR_SHIFT", handle_declaration, 0},
{L"declaration  :  declaration_specifiers error ", NULL, handle_declaration, 0},
{L"declaration_specifiers  :  typedef type_qualifier type_specifier ", NULL, handle_declaration_specifiers, 0},
{L"declaration_specifiers  :  typedef type_specifier ", NULL, handle_declaration_specifiers, 0},
{L"declaration_specifiers  :  type_qualifier type_specifier ", NULL, handle_declaration_specifiers, 0},
{L"declaration_specifiers  :  type_specifier ", NULL, handle_declaration_specifiers, 0},
{L"declaration_list  :  declaration ", NULL, handle_declaration_list, 0},
{L"declaration_list  :  declaration_list declaration ", NULL, handle_declaration_list, 0},
{L"init_declarator_list  :  init_declarator ", NULL, handle_init_declarator_list, 0},
{L"init_declarator_list  :  init_declarator_list , init_declarator ", NULL, handle_init_declarator_list, 0},
{L"init_declarator  :  declarator ", NULL, handle_init_declarator, 0},
{L"init_declarator  :  declarator = initializer ", NULL, handle_init_declarator, 0},
{L"initializer  :  assignment_expression ", NULL, handle_initializer, 0},
{L"initializer  :  { initializer_list } ", NULL, handle_initializer, 0},
{L"initializer_list  :  initializer ", NULL, handle_initializer_list, 0},
{L"initializer_list  :  initializer_list , initializer ", NULL, handle_initializer_list, 0},
{L"declarator  :  pointer direct_declarator ", NULL, handle_declarator, 0},
{L"declarator  :  direct_declarator ", NULL, handle_declarator, 0},
{L"direct_declarator  :  IDENTIFIER ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  IDENTIFIER [ constant_expression ] ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  IDENTIFIER [ ] ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  IDENTIFIER ( parameter_list ) ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  IDENTIFIER ( ) ", NULL, handle_direct_declarator, 0},
{L"pointer  :  * ", NULL, handle_pointer, 0},
{L"pointer  :  * type_qualifier ", NULL, handle_pointer, 0},
{L"pointer  :  * type_qualifier pointer ", NULL, handle_pointer, 0},
{L"pointer  :  * pointer ", NULL, handle_pointer, 0},
{L"parameter_list  :  parameter_declaration ", NULL, handle_parameter_list, 0},
{L"parameter_list  :  parameter_list , parameter_declaration ", NULL, handle_parameter_list, 0},
{L"parameter_declaration  :  declaration_specifiers declarator ", NULL, handle_parameter_declaration, 0},
{L"parameter_declaration  :  declaration_specifiers pointer ", NULL, handle_parameter_declaration, 0},
{L"parameter_declaration  :  declaration_specifiers ", NULL, handle_parameter_declaration, 0},
{L"type_name  :  specifier_qualifier_list ", NULL, handle_type_name, 0},
{L"type_name  :  specifier_qualifier_list pointer ", NULL, handle_type_name, 0},
{L"specifier_qualifier_list  :  type_qualifier type_specifier ", NULL, handle_specifier_qualifier_list, 0},
{L"specifier_qualifier_list  :  type_specifier ", NULL, handle_specifier_qualifier_list, 0},
{L"statement  :  labeled_statement ", NULL, handle_statement, 0},
{L"statement  :  compound_statement ", NULL, handle_statement, 0},
{L"statement  :  expression_statement ", NULL, handle_statement, 0},
{L"statement  :  selection_statement ", NULL, handle_statement, 0},
{L"statement  :  iteration_statement ", NULL, handle_statement, 0},
{L"statement  :  jump_statement ", NULL, handle_statement, 0},
{L"statement  :  error ", NULL, handle_statement, 0},
{L"labeled_statement  :  case constant_expression : statement ", NULL, handle_labeled_statement, 0},
{L"labeled_statement  :  default : statement ", NULL, handle_labeled_statement, 0},
{L"labeled_statement  :  IDENTIFIER : statement ", NULL, handle_labeled_statement, 0},
{L"compound_statement  :  { } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { statement_list } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { push_symtbl declaration_list pop_symtbl } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { push_symtbl declaration_list statement_list pop_symtbl } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { push_symtbl declaration_list error pop_symtbl } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { error } ", L"ERROR_SHIFT", handle_compound_statement, 0},
{L"push_symtbl  :   ", L"SPEC_ACTION", handle_push_symtbl, 0},
{L"pop_symtbl  :   ", L"SPEC_ACTION", handle_pop_symtbl, 0},
{L"statement_list  :  statement ", NULL, handle_statement_list, 0},
{L"statement_list  :  statement_list statement ", NULL, handle_statement_list, 0},
{L"expression_statement  :  ; ", NULL, handle_expression_statement, 0},
{L"expression_statement  :  expression ; ", NULL, handle_expression_statement, 0},
{L"selection_statement  :  if ( expression ) statement ", L"IF_STMT_PREC", handle_selection_statement, 0},
{L"selection_statement  :  if ( expression ) statement else statement ", L"IF_STMT_ELSE_STMT_PREC", handle_selection_statement, 0},
{L"selection_statement  :  switch ( expression ) statement ", NULL, handle_selection_statement, 0},
{L"iteration_statement  :  while ( expression ) statement ", NULL, handle_iteration_statement, 0},
{L"iteration_statement  :  do statement while ( expression ) ; ", NULL, handle_iteration_statement, 0},
{L"iteration_statement  :  for ( expression_statement expression_statement ) statement ", NULL, handle_iteration_statement, 0},
{L"iteration_statement  :  for ( expression_statement expression_statement expression ) statement ", NULL, handle_iteration_statement, 0},
{L"jump_statement  :  goto IDENTIFIER ; ", NULL, handle_jump_statement, 0},
{L"jump_statement  :  continue ; ", NULL, handle_jump_statement, 0},
{L"jump_statement  :  break ; ", NULL, handle_jump_statement, 0},
{L"jump_statement  :  return ; ", NULL, handle_jump_statement, 0},
{L"jump_statement  :  return expression ; ", NULL, handle_jump_statement, 0},
{L"expression  :  assignment_expression ", NULL, handle_expression, 0},
{L"expression  :  expression , assignment_expression ", NULL, handle_expression, 0},
{L"primary_expression  :  IDENTIFIER ", NULL, handle_primary_expression, 0},
{L"primary_expression  :  constant ", NULL, handle_primary_expression, 0},
{L"primary_expression  :  ( expression ) ", NULL, handle_primary_expression, 0},
{L"postfix_expression  :  primary_expression ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression [ expression ] ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression ( ) ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression ( argument_expression_list ) ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression . IDENTIFIER ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression -> IDENTIFIER ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression ++ ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression -- ", NULL, handle_postfix_expression, 0},
{L"argument_expression_list  :  assignment_expression ", NULL, handle_argument_expression_list, 0},
{L"argument_expression_list  :  argument_expression_list , assignment_expression ", NULL, handle_argument_expression_list, 0},
{L"unary_expression  :  postfix_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  ++ unary_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  -- unary_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  unary_operator cast_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  sizeof unary_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  sizeof ( type_name ) ", NULL, handle_unary_expression, 0},
{L"unary_operator  :  & ", NULL, handle_unary_operator, 0},
{L"unary_operator  :  * ", NULL, handle_unary_operator, 0},
{L"unary_operator  :  + ", NULL, handle_unary_operator, 0},
{L"unary_operator  :  - ", NULL, handle_unary_operator, 0},
{L"unary_operator  :  ~ ", NULL, handle_unary_operator, 0},
{L"unary_operator  :  ! ", NULL, handle_unary_operator, 0},
{L"cast_expression  :  unary_expression ", NULL, handle_cast_expression, 0},
{L"cast_expression  :  ( type_name ) cast_expression ", NULL, handle_cast_expression, 0},
{L"binary_expression  :  binary_expression + binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression - binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression * binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression / binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression % binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression << binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression >> binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression < binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression <= binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression > binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression >= binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression == binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression != binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression & binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression ^ binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression | binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression && binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  binary_expression || binary_expression ", NULL, handle_binary_expression, 0},
{L"binary_expression  :  cast_expression ", NULL, handle_binary_expression, 0},
{L"constant_expression  :  binary_expression ", NULL, handle_constant_expression, 0},
{L"constant_expression  :  binary_expression ? expression : constant_expression ", NULL, handle_constant_expression, 0},
{L"assignment_expression  :  constant_expression ", NULL, handle_assignment_expression, 0},
{L"assignment_expression  :  unary_expression assignment_operator assignment_expression ", NULL, handle_assignment_expression, 0},
{L"assignment_operator  :  = ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  *= ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  /= ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  %= ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  += ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  -= ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  <<= ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  >>= ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  &= ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  ^= ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  |= ", NULL, handle_assignment_operator, 0},
{L"constant  :  OCT_CONSTANT ", NULL, handle_constant, 0},
{L"constant  :  HEX_CONSTANT ", NULL, handle_constant, 0},
{L"constant  :  DEC_CONSTANT ", NULL, handle_constant, 0},
{L"constant  :  FLOAT_CONSTANT ", NULL, handle_constant, 0},
{L"constant  :  CHAR_CONSTANT ", NULL, handle_constant, 0},
{L"constant  :  STRING_LITERAL ", NULL, handle_constant, 0}
};

#define START_RULE L"program"









lex_t*	__ray_build_lex_impl(const arIOCtx_t *io)								
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

psrGrammar_t*	__ray_build_grammar_impl(const psrCtx_t	*psr_ctx, const arIOCtx_t *io)											
	{																																
			psrGrammar_t	*grammar;																								
			size_t i;																												
			AR_ASSERT(psr_ctx != NULL);																								
			grammar = PSR_CreateGrammar(psr_ctx, io);																				
			for(i = 0; i < AR_NELEMS(__g_term_pattern); ++i)																		
			{																														
					if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)continue;										
					if(!PSR_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PSR_ASSOC_NOASSOC,0, ray_handle_token))	
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









/**********************************************semantic action**********************************************************/



/*token*/
static psrNode_t*		AR_STDCALL ray_handle_token(const psrToken_t *tok,void *ctx)
{
		return NULL;
}





/*program	:	translation_unit */
static psrNode_t* AR_STDCALL handle_program(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*translation_unit	:	external_declaration */
/*translation_unit	:	translation_unit external_declaration */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*external_declaration	:	function_definition */
/*external_declaration	:	declaration */
static psrNode_t* AR_STDCALL handle_external_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*function_definition	:	declaration_specifiers declarator compound_statement */
static psrNode_t* AR_STDCALL handle_function_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*type_qualifier	:	const */
static psrNode_t* AR_STDCALL handle_type_qualifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*type_specifier	:	void */
/*type_specifier	:	byte */
/*type_specifier	:	char */
/*type_specifier	:	short */
/*type_specifier	:	int */
/*type_specifier	:	long */
/*type_specifier	:	float */
/*type_specifier	:	double */
/*type_specifier	:	signed */
/*type_specifier	:	unsigned */
/*type_specifier	:	struct_specifier */
/*type_specifier	:	TYPE_ID */
static psrNode_t* AR_STDCALL handle_type_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*struct_specifier	:	struct IDENTIFIER { struct_declaration_list } */
/*struct_specifier	:	struct IDENTIFIER { error } */
static psrNode_t* AR_STDCALL handle_struct_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*struct_declaration	:	type_specifier struct_declarator_list ; */
static psrNode_t* AR_STDCALL handle_struct_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*struct_declaration_list	:	struct_declaration */
/*struct_declaration_list	:	struct_declaration_list struct_declaration */
static psrNode_t* AR_STDCALL handle_struct_declaration_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*struct_declarator_list	:	declarator */
/*struct_declarator_list	:	struct_declarator_list , declarator */
static psrNode_t* AR_STDCALL handle_struct_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*declaration	:	declaration_specifiers init_declarator_list ; */
/*declaration	:	declaration_specifiers ; */
/*declaration	:	declaration_specifiers error ; */
/*declaration	:	declaration_specifiers error */
static psrNode_t* AR_STDCALL handle_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*declaration_specifiers	:	typedef type_qualifier type_specifier */
/*declaration_specifiers	:	typedef type_specifier */
/*declaration_specifiers	:	type_qualifier type_specifier */
/*declaration_specifiers	:	type_specifier */
static psrNode_t* AR_STDCALL handle_declaration_specifiers(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*declaration_list	:	declaration */
/*declaration_list	:	declaration_list declaration */
static psrNode_t* AR_STDCALL handle_declaration_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL handle_init_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*init_declarator	:	declarator */
/*init_declarator	:	declarator = initializer */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*initializer	:	assignment_expression */
/*initializer	:	{ initializer_list } */
static psrNode_t* AR_STDCALL handle_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*initializer_list	:	initializer */
/*initializer_list	:	initializer_list , initializer */
static psrNode_t* AR_STDCALL handle_initializer_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*declarator	:	pointer direct_declarator */
/*declarator	:	direct_declarator */
static psrNode_t* AR_STDCALL handle_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*direct_declarator	:	IDENTIFIER */
/*direct_declarator	:	IDENTIFIER [ constant_expression ] */
/*direct_declarator	:	IDENTIFIER [ ] */
/*direct_declarator	:	IDENTIFIER ( parameter_list ) */
/*direct_declarator	:	IDENTIFIER ( ) */
static psrNode_t* AR_STDCALL handle_direct_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*pointer	:	* */
/*pointer	:	* type_qualifier */
/*pointer	:	* type_qualifier pointer */
/*pointer	:	* pointer */
static psrNode_t* AR_STDCALL handle_pointer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*parameter_list	:	parameter_declaration */
/*parameter_list	:	parameter_list , parameter_declaration */
static psrNode_t* AR_STDCALL handle_parameter_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*parameter_declaration	:	declaration_specifiers declarator */
/*parameter_declaration	:	declaration_specifiers pointer */
/*parameter_declaration	:	declaration_specifiers */
static psrNode_t* AR_STDCALL handle_parameter_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*type_name	:	specifier_qualifier_list */
/*type_name	:	specifier_qualifier_list pointer */
static psrNode_t* AR_STDCALL handle_type_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*specifier_qualifier_list	:	type_qualifier type_specifier */
/*specifier_qualifier_list	:	type_specifier */
static psrNode_t* AR_STDCALL handle_specifier_qualifier_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*statement	:	labeled_statement */
/*statement	:	compound_statement */
/*statement	:	expression_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
/*statement	:	error */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*labeled_statement	:	case constant_expression : statement */
/*labeled_statement	:	default : statement */
/*labeled_statement	:	IDENTIFIER : statement */
static psrNode_t* AR_STDCALL handle_labeled_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*compound_statement	:	{ } */
/*compound_statement	:	{ statement_list } */
/*compound_statement	:	{ push_symtbl declaration_list pop_symtbl } */
/*compound_statement	:	{ push_symtbl declaration_list statement_list pop_symtbl } */
/*compound_statement	:	{ push_symtbl declaration_list error pop_symtbl } */
/*compound_statement	:	{ error } */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*push_symtbl	:	 */
static psrNode_t* AR_STDCALL handle_push_symtbl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*pop_symtbl	:	 */
static psrNode_t* AR_STDCALL handle_pop_symtbl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*statement_list	:	statement */
/*statement_list	:	statement_list statement */
static psrNode_t* AR_STDCALL handle_statement_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*expression_statement	:	; */
/*expression_statement	:	expression ; */
static psrNode_t* AR_STDCALL handle_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*selection_statement	:	if ( expression ) statement */
/*selection_statement	:	if ( expression ) statement else statement */
/*selection_statement	:	switch ( expression ) statement */
static psrNode_t* AR_STDCALL handle_selection_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*iteration_statement	:	while ( expression ) statement */
/*iteration_statement	:	do statement while ( expression ) ; */
/*iteration_statement	:	for ( expression_statement expression_statement ) statement */
/*iteration_statement	:	for ( expression_statement expression_statement expression ) statement */
static psrNode_t* AR_STDCALL handle_iteration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*jump_statement	:	goto IDENTIFIER ; */
/*jump_statement	:	continue ; */
/*jump_statement	:	break ; */
/*jump_statement	:	return ; */
/*jump_statement	:	return expression ; */
static psrNode_t* AR_STDCALL handle_jump_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*expression	:	assignment_expression */
/*expression	:	expression , assignment_expression */
static psrNode_t* AR_STDCALL handle_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*primary_expression	:	IDENTIFIER */
/*primary_expression	:	constant */
/*primary_expression	:	( expression ) */
static psrNode_t* AR_STDCALL handle_primary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*postfix_expression	:	primary_expression */
/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression ( ) */
/*postfix_expression	:	postfix_expression ( argument_expression_list ) */
/*postfix_expression	:	postfix_expression . IDENTIFIER */
/*postfix_expression	:	postfix_expression -> IDENTIFIER */
/*postfix_expression	:	postfix_expression ++ */
/*postfix_expression	:	postfix_expression -- */
static psrNode_t* AR_STDCALL handle_postfix_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*argument_expression_list	:	assignment_expression */
/*argument_expression_list	:	argument_expression_list , assignment_expression */
static psrNode_t* AR_STDCALL handle_argument_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*unary_expression	:	postfix_expression */
/*unary_expression	:	++ unary_expression */
/*unary_expression	:	-- unary_expression */
/*unary_expression	:	unary_operator cast_expression */
/*unary_expression	:	sizeof unary_expression */
/*unary_expression	:	sizeof ( type_name ) */
static psrNode_t* AR_STDCALL handle_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*unary_operator	:	& */
/*unary_operator	:	* */
/*unary_operator	:	+ */
/*unary_operator	:	- */
/*unary_operator	:	~ */
/*unary_operator	:	! */
static psrNode_t* AR_STDCALL handle_unary_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*cast_expression	:	unary_expression */
/*cast_expression	:	( type_name ) cast_expression */
static psrNode_t* AR_STDCALL handle_cast_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*binary_expression	:	binary_expression + binary_expression */
/*binary_expression	:	binary_expression - binary_expression */
/*binary_expression	:	binary_expression * binary_expression */
/*binary_expression	:	binary_expression / binary_expression */
/*binary_expression	:	binary_expression % binary_expression */
/*binary_expression	:	binary_expression << binary_expression */
/*binary_expression	:	binary_expression >> binary_expression */
/*binary_expression	:	binary_expression < binary_expression */
/*binary_expression	:	binary_expression <= binary_expression */
/*binary_expression	:	binary_expression > binary_expression */
/*binary_expression	:	binary_expression >= binary_expression */
/*binary_expression	:	binary_expression == binary_expression */
/*binary_expression	:	binary_expression != binary_expression */
/*binary_expression	:	binary_expression & binary_expression */
/*binary_expression	:	binary_expression ^ binary_expression */
/*binary_expression	:	binary_expression | binary_expression */
/*binary_expression	:	binary_expression && binary_expression */
/*binary_expression	:	binary_expression || binary_expression */
/*binary_expression	:	cast_expression */
static psrNode_t* AR_STDCALL handle_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*constant_expression	:	binary_expression */
/*constant_expression	:	binary_expression ? expression : constant_expression */
static psrNode_t* AR_STDCALL handle_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*assignment_expression	:	constant_expression */
/*assignment_expression	:	unary_expression assignment_operator assignment_expression */
static psrNode_t* AR_STDCALL handle_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*assignment_operator	:	= */
/*assignment_operator	:	*= */
/*assignment_operator	:	/= */
/*assignment_operator	:	%= */
/*assignment_operator	:	+= */
/*assignment_operator	:	-= */
/*assignment_operator	:	<<= */
/*assignment_operator	:	>>= */
/*assignment_operator	:	&= */
/*assignment_operator	:	^= */
/*assignment_operator	:	|= */
static psrNode_t* AR_STDCALL handle_assignment_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*constant	:	OCT_CONSTANT */
/*constant	:	HEX_CONSTANT */
/*constant	:	DEC_CONSTANT */
/*constant	:	FLOAT_CONSTANT */
/*constant	:	CHAR_CONSTANT */
/*constant	:	STRING_LITERAL */
static psrNode_t* AR_STDCALL handle_constant(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




AR_NAMESPACE_END



