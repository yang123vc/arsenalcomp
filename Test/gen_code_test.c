#include "test.h"
#include "../Arsenal/Lex/lex.h"
#include "../Arsenal/Parser/grammar.h"
#include "../Arsenal/Parser/parser.h"
#include "../Arsenal/Parser/lr_action.h"


#if defined(__LIB)

AR_NAMESPACE_BEGIN



static const wchar_t *__g_lex_name[] = {
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
L"float_literal = ([0-9]*\\.[0-9]*{exponet}?{float_type_suffix}?)",
L"skip_lexem = {delim}|{comment}|{comment_line}",
L"escape_seq = (\\\\(b|t|n|f|r|\\x22|\\x27|\\\\))|{octal_escape}",
L"string_literal = (\\x22({escape_seq}|[^\\x22\\\\])*\\x22)",
L"char_literal = \\x27({escape_seq}|[^\\x27\\\\])\\x27",
L"keyword_lhd = [A-Z_a-z0-9]"
};


static struct {const wchar_t *name; size_t tokval; size_t lex_prec; const wchar_t *regex; ar_bool_t skip; }__g_term_pattern[] =  {
{NULL, 256, 1, L"{delim}+|{comment}+|{comment_line}+", true},
{L"CHAR_CONSTANT", 257, 0, L"{char_literal}", false},
{L"STRING_LITERAL", 258, 0, L"{string_literal}", false},
{L"FLOAT_CONSTANT", 259, 2, L"{float_literal}(?!{keyword_lhd})", false},
{L"HEX_CONSTANT", 260, 2, L"{hex_literal}(?!{keyword_lhd})", false},
{L"OCT_CONSTANT", 261, 1, L"{oct_literal}(?!{keyword_lhd})", false},
{L"DEC_CONSTANT", 262, 1, L"{dec_literal}(?!{keyword_lhd})", false},
{L"IDENTIFIER", 263, 0, L"{letter}({letter}|{digit})*", false},
{L"switch", 264, 1, L"\"switch\"(?!{keyword_lhd})", false},
{L"for", 265, 1, L"\"for\"(?!{keyword_lhd})", false},
{L"goto", 266, 1, L"\"goto\"(?!{keyword_lhd})", false},
{L"return", 267, 1, L"\"return\"(?!{keyword_lhd})", false},
{L"do", 268, 1, L"\"do\"(?!{keyword_lhd})", false},
{L"while", 269, 1, L"\"while\"(?!{keyword_lhd})", false},
{L"if", 270, 1, L"\"if\"(?!{keyword_lhd})", false},
{L"else", 271, 1, L"\"else\"(?!{keyword_lhd})", false},
{L"continue", 272, 1, L"\"continue\"(?!{keyword_lhd})", false},
{L"default", 273, 1, L"\"default\"(?!{keyword_lhd})", false},
{L"case", 274, 1, L"\"case\"(?!{keyword_lhd})", false},
{L"break", 275, 1, L"\"break\"(?!{keyword_lhd})", false},
{L"const", 276, 1, L"\"const\"(?!{keyword_lhd})", false},
{L"sizeof", 277, 1, L"\"sizeof\"(?!{keyword_lhd})", false},
{L"struct", 278, 1, L"\"struct\"(?!{keyword_lhd})", false},
{L"typedef", 279, 1, L"\"typedef\"(?!{keyword_lhd})", false},
{L"void", 280, 1, L"\"void\"(?!{keyword_lhd})", false},
{L"byte", 281, 1, L"\"byte\"(?!{keyword_lhd})", false},
{L"char", 282, 1, L"\"char\"(?!{keyword_lhd})", false},
{L"short", 283, 1, L"\"short\"(?!{keyword_lhd})", false},
{L"int", 284, 1, L"\"int\"(?!{keyword_lhd})", false},
{L"long", 285, 1, L"\"long\"(?!{keyword_lhd})", false},
{L"signed", 286, 1, L"\"signed\"(?!{keyword_lhd})", false},
{L"unsigned", 287, 1, L"\"unsigned\"(?!{keyword_lhd})", false},
{L"float", 288, 1, L"\"float\"(?!{keyword_lhd})", false},
{L"double", 289, 1, L"\"double\"(?!{keyword_lhd})", false},
{L"__attribute__", 290, 1, L"\"__attribute__\"", false},
{L"__pack__", 291, 1, L"\"__pack__\"", false},
{L">>=", 292, 2, L"\">>=\"", false},
{L"<<=", 293, 2, L"\"<<=\"", false},
{L"+=", 294, 1, L"\"+=\"", false},
{L"-=", 295, 1, L"\"-=\"", false},
{L"*=", 296, 1, L"\"*=\"", false},
{L"/=", 297, 1, L"\"/=\"", false},
{L"%=", 298, 1, L"\"%=\"", false},
{L"&=", 299, 1, L"\"&=\"", false},
{L"^=", 300, 1, L"\"^=\"", false},
{L"|=", 301, 1, L"\"|=\"", false},
{L">>", 302, 1, L"\">>\"", false},
{L"<<", 303, 1, L"\"<<\"", false},
{L"++", 304, 1, L"\"++\"", false},
{L"--", 305, 1, L"\"--\"", false},
{L"->", 306, 1, L"\"->\"", false},
{L"&&", 307, 1, L"\"&&\"", false},
{L"||", 308, 1, L"\"||\"", false},
{L"<=", 309, 1, L"\"<=\"", false},
{L">=", 310, 1, L"\">=\"", false},
{L"==", 311, 1, L"\"==\"", false},
{L"!=", 312, 1, L"\"!=\"", false},
{L";", 313, 0, L"\";\"", false},
{L"{", 314, 0, L"\"{\"", false},
{L"}", 315, 0, L"\"}\"", false},
{L",", 316, 0, L"\",\"", false},
{L":", 317, 0, L"\":\"", false},
{L"=", 318, 0, L"\"=\"", false},
{L"(", 319, 0, L"\"(\"", false},
{L")", 320, 0, L"\")\"", false},
{L"[", 321, 0, L"\"[\"", false},
{L"]", 322, 0, L"\"]\"", false},
{L".", 323, 0, L"\".\"", false},
{L"&", 324, 0, L"\"&\"", false},
{L"!", 325, 0, L"\"!\"", false},
{L"~", 326, 0, L"\"~\"", false},
{L"-", 327, 0, L"\"-\"", false},
{L"+", 328, 0, L"\"+\"", false},
{L"*", 329, 0, L"\"*\"", false},
{L"/", 330, 0, L"\"/\"", false},
{L"%", 331, 0, L"\"%\"", false},
{L"<", 332, 0, L"\"<\"", false},
{L">", 333, 0, L"\">\"", false},
{L"^", 334, 0, L"\"^\"", false},
{L"|", 335, 0, L"\"|\"", false},
{L"?", 336, 0, L"\"?\"", false},
{L"TYPE_ID", 337, 0, L"^", false},
{L"FAKE_EOI", 338, 0, L"\"$\"", false},
{L"EOI", 0, 2, L"$", false}
};


static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {
{L"||", 308, 1, PSR_ASSOC_LEFT},
{L"&&", 307, 2, PSR_ASSOC_LEFT},
{L"|", 335, 3, PSR_ASSOC_LEFT},
{L"^", 334, 4, PSR_ASSOC_LEFT},
{L"&", 324, 5, PSR_ASSOC_LEFT},
{L"==", 311, 6, PSR_ASSOC_LEFT},
{L"!=", 312, 6, PSR_ASSOC_LEFT},
{L"<", 332, 7, PSR_ASSOC_LEFT},
{L"<=", 309, 7, PSR_ASSOC_LEFT},
{L">", 333, 7, PSR_ASSOC_LEFT},
{L">=", 310, 7, PSR_ASSOC_LEFT},
{L"<<", 303, 8, PSR_ASSOC_LEFT},
{L">>", 302, 8, PSR_ASSOC_LEFT},
{L"+", 328, 9, PSR_ASSOC_LEFT},
{L"-", 327, 9, PSR_ASSOC_LEFT},
{L"*", 329, 10, PSR_ASSOC_LEFT},
{L"/", 330, 10, PSR_ASSOC_LEFT},
{L"%", 331, 10, PSR_ASSOC_LEFT},
{L"IF_STMT_PREC", 339, 11, PSR_ASSOC_NONASSOC},
{L"IF_STMT_ELSE_STMT_PREC", 340, 12, PSR_ASSOC_NONASSOC},
{L"SPEC_ACTION", 341, 13, PSR_ASSOC_NONASSOC},
{L"ERROR_SHIFT", 342, 14, PSR_ASSOC_RIGHT}
};


static psrNode_t* AR_STDCALL handle_enter_scope(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*enter_scope	:	 */
static psrNode_t* AR_STDCALL handle_exit_scope(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*exit_scope	:	 */
static psrNode_t* AR_STDCALL handle_init_func_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_func_def	:	 */
static psrNode_t* AR_STDCALL handle_uninit_func_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*uninit_func_def	:	 */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*translation_unit	:	external_declaration */
/*translation_unit	:	translation_unit external_declaration */
static psrNode_t* AR_STDCALL handle_external_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*external_declaration	:	function_definition */
/*external_declaration	:	declaration */
static psrNode_t* AR_STDCALL handle_function_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_definition	:	declaration_specifiers declarator init_func_def compound_statement uninit_func_def */
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
/*type_specifier	:	signed short */
/*type_specifier	:	unsigned short */
/*type_specifier	:	signed int */
/*type_specifier	:	unsigned int */
/*type_specifier	:	signed long */
/*type_specifier	:	unsigned long */
/*type_specifier	:	struct_specifier */
/*type_specifier	:	TYPE_ID */
static psrNode_t* AR_STDCALL handle_struct_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*struct_specifier	:	struct IDENTIFIER { enter_scope struct_declaration_list exit_scope } */
/*struct_specifier	:	struct IDENTIFIER */
/*struct_specifier	:	struct IDENTIFIER { enter_scope error exit_scope } */
/*struct_specifier	:	struct error */
static psrNode_t* AR_STDCALL handle_struct_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*struct_declaration	:	type_specifier struct_declarator_list ; */
static psrNode_t* AR_STDCALL handle_struct_declaration_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*struct_declaration_list	:	struct_declaration */
/*struct_declaration_list	:	struct_declaration_list struct_declaration */
static psrNode_t* AR_STDCALL handle_struct_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*struct_declarator_list	:	declarator */
/*struct_declarator_list	:	struct_declarator_list , declarator */
static psrNode_t* AR_STDCALL handle_set_pack(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*set_pack	:	__pack__ ( ) */
/*set_pack	:	__pack__ ( DEC_CONSTANT ) */
/*set_pack	:	__pack__ error */
static psrNode_t* AR_STDCALL handle_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration	:	declaration_specifiers init_declarator_list ; */
/*declaration	:	declaration_specifiers ; */
/*declaration	:	declaration_specifiers error */
/*declaration	:	set_pack */
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
static psrNode_t* AR_STDCALL handle_labeled_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*labeled_statement	:	case constant_expression : statement */
/*labeled_statement	:	default : statement */
/*labeled_statement	:	IDENTIFIER : statement */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	{ } */
/*compound_statement	:	{ statement_list } */
/*compound_statement	:	{ enter_scope declaration_list exit_scope } */
/*compound_statement	:	{ enter_scope declaration_list statement_list exit_scope } */
/*compound_statement	:	{ enter_scope declaration_list error exit_scope } */
/*compound_statement	:	{ error } */
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
{L"enter_scope  :   ", L"SPEC_ACTION", handle_enter_scope, 0},
{L"exit_scope  :   ", L"SPEC_ACTION", handle_exit_scope, 0},
{L"init_func_def  :   ", L"SPEC_ACTION", handle_init_func_def, 0},
{L"uninit_func_def  :   ", L"SPEC_ACTION", handle_uninit_func_def, 0},
{L"translation_unit  :  external_declaration ", NULL, handle_translation_unit, 0},
{L"translation_unit  :  translation_unit external_declaration ", NULL, handle_translation_unit, 0},
{L"external_declaration  :  function_definition ", NULL, handle_external_declaration, 0},
{L"external_declaration  :  declaration ", NULL, handle_external_declaration, 0},
{L"function_definition  :  declaration_specifiers declarator init_func_def compound_statement uninit_func_def ", NULL, handle_function_definition, 0},
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
{L"type_specifier  :  signed short ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  unsigned short ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  signed int ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  unsigned int ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  signed long ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  unsigned long ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  struct_specifier ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  TYPE_ID ", NULL, handle_type_specifier, 0},
{L"struct_specifier  :  struct IDENTIFIER { enter_scope struct_declaration_list exit_scope } ", NULL, handle_struct_specifier, 0},
{L"struct_specifier  :  struct IDENTIFIER ", NULL, handle_struct_specifier, 0},
{L"struct_specifier  :  struct IDENTIFIER { enter_scope error exit_scope } ", NULL, handle_struct_specifier, 0},
{L"struct_specifier  :  struct error ", NULL, handle_struct_specifier, 0},
{L"struct_declaration  :  type_specifier struct_declarator_list ; ", NULL, handle_struct_declaration, 0},
{L"struct_declaration_list  :  struct_declaration ", NULL, handle_struct_declaration_list, 0},
{L"struct_declaration_list  :  struct_declaration_list struct_declaration ", NULL, handle_struct_declaration_list, 0},
{L"struct_declarator_list  :  declarator ", NULL, handle_struct_declarator_list, 0},
{L"struct_declarator_list  :  struct_declarator_list , declarator ", NULL, handle_struct_declarator_list, 0},
{L"set_pack  :  __pack__ ( ) ", NULL, handle_set_pack, 0},
{L"set_pack  :  __pack__ ( DEC_CONSTANT ) ", NULL, handle_set_pack, 0},
{L"set_pack  :  __pack__ error ", NULL, handle_set_pack, 0},
{L"declaration  :  declaration_specifiers init_declarator_list ; ", NULL, handle_declaration, 0},
{L"declaration  :  declaration_specifiers ; ", NULL, handle_declaration, 0},
{L"declaration  :  declaration_specifiers error ", NULL, handle_declaration, 0},
{L"declaration  :  set_pack ", NULL, handle_declaration, 0},
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
{L"labeled_statement  :  case constant_expression : statement ", NULL, handle_labeled_statement, 0},
{L"labeled_statement  :  default : statement ", NULL, handle_labeled_statement, 0},
{L"labeled_statement  :  IDENTIFIER : statement ", NULL, handle_labeled_statement, 0},
{L"compound_statement  :  { } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { statement_list } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { enter_scope declaration_list exit_scope } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { enter_scope declaration_list statement_list exit_scope } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { enter_scope declaration_list error exit_scope } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { error } ", L"ERROR_SHIFT", handle_compound_statement, 0},
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

#define START_RULE L"translation_unit"

static lex_t*	__build_lex(const arIOCtx_t *io)								
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

static psrGrammar_t*	__build_grammar(const psrCtx_t	*psr_ctx, const arIOCtx_t *io)											
{																																
		psrGrammar_t	*grammar;																								
		size_t i;																												
		AR_ASSERT(psr_ctx != NULL);																								
		grammar = PSR_CreateGrammar(psr_ctx, io);																				
		for(i = 0; i < AR_NELEMS(__g_term_pattern); ++i)																		
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




/*enter_scope	:	 */
static psrNode_t* AR_STDCALL handle_enter_scope(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*exit_scope	:	 */
static psrNode_t* AR_STDCALL handle_exit_scope(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*init_func_def	:	 */
static psrNode_t* AR_STDCALL handle_init_func_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*uninit_func_def	:	 */
static psrNode_t* AR_STDCALL handle_uninit_func_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
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


/*function_definition	:	declaration_specifiers declarator init_func_def compound_statement uninit_func_def */
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
/*type_specifier	:	signed short */
/*type_specifier	:	unsigned short */
/*type_specifier	:	signed int */
/*type_specifier	:	unsigned int */
/*type_specifier	:	signed long */
/*type_specifier	:	unsigned long */
/*type_specifier	:	struct_specifier */
/*type_specifier	:	TYPE_ID */
static psrNode_t* AR_STDCALL handle_type_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*struct_specifier	:	struct IDENTIFIER { enter_scope struct_declaration_list exit_scope } */
/*struct_specifier	:	struct IDENTIFIER */
/*struct_specifier	:	struct IDENTIFIER { enter_scope error exit_scope } */
/*struct_specifier	:	struct error */
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


/*set_pack	:	__pack__ ( ) */
/*set_pack	:	__pack__ ( DEC_CONSTANT ) */
/*set_pack	:	__pack__ error */
static psrNode_t* AR_STDCALL handle_set_pack(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}


/*declaration	:	declaration_specifiers init_declarator_list ; */
/*declaration	:	declaration_specifiers ; */
/*declaration	:	declaration_specifiers error */
/*declaration	:	set_pack */
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
/*compound_statement	:	{ enter_scope declaration_list exit_scope } */
/*compound_statement	:	{ enter_scope declaration_list statement_list exit_scope } */
/*compound_statement	:	{ enter_scope declaration_list error exit_scope } */
/*compound_statement	:	{ error } */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
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

#endif