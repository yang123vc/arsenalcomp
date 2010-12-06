
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
L"float_literal = ([0-9]+\\.[0-9]*{exponet}?{float_type_suffix}?)",
L"skip_lexem = {delim}|{comment}|{comment_line}",
L"escape_seq = (\\\\(b|t|n|f|r|\\x22|\\x27|\\\\))|{octal_escape}",
L"string_literal = (\\x22({escape_seq}|[^\\x22\\\\])*\\x22)",
L"char_literal = \\x27({escape_seq}|[^\\x27\\\\])\\x27",
L"keyword_lhd = [A-Z_a-z0-9]"
};

#define __NAME_COUNT__ ((size_t)20)



/*enum{
};*/




static struct {const wchar_t *name;
size_t tokval;
size_t lex_prec;
const wchar_t *regex;
bool_t skip;
psrTermFunc_t leaf;
}__g_term_pattern[] =  {
{NULL, 257,1, L"{delim}+|{comment}+|{comment_line}+", true, NULL},
{L"IDENTIFIER", 258, 0, L"{letter}({letter}|{digit})*", false, NULL},
{L"OCT_CONSTANT", 259, 0, L"{oct_literal}", false, NULL},
{L"HEX_CONSTANT", 260, 0, L"{hex_literal}", false, NULL},
{L"DEC_CONSTANT", 261, 0, L"{dec_literal}", false, NULL},
{L"FLOAT_CONSTANT", 262, 0, L"{float_literal}", false, NULL},
{L"CHAR_CONSTANT", 263, 0, L"{char_literal}", false, NULL},
{L"STRING_LITERAL", 264, 0, L"{string_literal}", false, NULL},
{L"TYPEDEF_NAME", 265, 0, L"^", false, NULL},
{L"NAMESPACE_NAME", 266, 0, L"^", false, NULL},
{L"CLASS_NAME", 267, 0, L"^", false, NULL},
{L"ENUM_NAME", 268, 0, L"^", false, NULL},
{L"TEMPLATE_NAME", 269, 0, L"^", false, NULL},
{L"ELLIPSIS", 270, 1, L"\"...\"", false, NULL},
{L"COLONCOLON", 271, 1, L"\"::\"", false, NULL},
{L"DOTSTAR", 272, 1, L"\".*\"", false, NULL},
{L"ADDEQ", 273, 1, L"\"+=\"", false, NULL},
{L"SUBEQ", 274, 1, L"\"-=\"", false, NULL},
{L"MULEQ", 275, 1, L"\"*=\"", false, NULL},
{L"DIVEQ", 276, 1, L"\"/=\"", false, NULL},
{L"MODEQ", 277, 1, L"\"%=\"", false, NULL},
{L"XOREQ", 278, 1, L"\"^=\"", false, NULL},
{L"ANDEQ", 279, 1, L"\"&=\"", false, NULL},
{L"OREQ", 280, 1, L"\"|=\"", false, NULL},
{L"SL", 281, 1, L"\"<<\"", false, NULL},
{L"SR", 282, 1, L"\">>\"", false, NULL},
{L"SREQ", 283, 1, L"\">>=\"", false, NULL},
{L"SLEQ", 284, 1, L"\"<<=\"", false, NULL},
{L"EQ", 285, 1, L"\"==\"", false, NULL},
{L"NOTEQ", 286, 1, L"\"!=\"", false, NULL},
{L"LTEQ", 287, 1, L"\"<=\"", false, NULL},
{L"GTEQ", 288, 1, L"\">=\"", false, NULL},
{L"ANDAND", 289, 1, L"\"&&\"", false, NULL},
{L"OROR", 290, 1, L"\"||\"", false, NULL},
{L"PLUSPLUS", 291, 1, L"\"++\"", false, NULL},
{L"MINUSMINUS", 292, 1, L"\"--\"", false, NULL},
{L"ARROWSTAR", 293, 2, L"\"->*\"", false, NULL},
{L"ARROW", 294, 1, L"\"->\"", false, NULL},
{L"ASM", 295, 1, L"\"__asm\"", false, NULL},
{L"AUTO", 296, 1, L"\"auto\"", false, NULL},
{L"BOOL", 297, 1, L"\"bool\"", false, NULL},
{L"BREAK", 298, 1, L"\"break\"", false, NULL},
{L"CASE", 299, 1, L"\"case\"", false, NULL},
{L"CATCH", 300, 1, L"\"catch\"", false, NULL},
{L"CHAR", 301, 1, L"\"char\"", false, NULL},
{L"CLASS", 302, 1, L"\"class\"", false, NULL},
{L"CONST", 303, 1, L"\"const\"", false, NULL},
{L"CONST_CAST", 304, 1, L"\"const_cast\"", false, NULL},
{L"CONTINUE", 305, 1, L"\"continue\"", false, NULL},
{L"DEFAULT", 306, 1, L"\"default\"", false, NULL},
{L"DELETE", 307, 1, L"\"delete\"", false, NULL},
{L"DO", 308, 1, L"\"do\"", false, NULL},
{L"DOUBLE", 309, 1, L"\"double\"", false, NULL},
{L"DYNAMIC_CAST", 310, 1, L"\"dynamic_cast\"", false, NULL},
{L"ELSE", 311, 1, L"\"else\"", false, NULL},
{L"ENUM", 312, 1, L"\"enum\"", false, NULL},
{L"EXPLICIT", 313, 1, L"\"explicit\"", false, NULL},
{L"EXPORT", 314, 1, L"\"export\"", false, NULL},
{L"EXTERN", 315, 1, L"\"extern\"", false, NULL},
{L"FLOAT", 316, 1, L"\"float\"", false, NULL},
{L"FALSE", 317, 1, L"\"false\"", false, NULL},
{L"FOR", 318, 1, L"\"for\"", false, NULL},
{L"FRIEND", 319, 1, L"\"friend\"", false, NULL},
{L"GOTO", 320, 1, L"\"goto\"", false, NULL},
{L"IF", 321, 1, L"\"if\"", false, NULL},
{L"INLINE", 322, 1, L"\"inline\"", false, NULL},
{L"INT", 323, 1, L"\"int\"", false, NULL},
{L"LONG", 324, 1, L"\"long\"", false, NULL},
{L"MUTABLE", 325, 1, L"\"mutable\"", false, NULL},
{L"NAMESPACE", 326, 1, L"\"namespace\"", false, NULL},
{L"NEW", 327, 1, L"\"new\"", false, NULL},
{L"OPERATOR", 328, 1, L"\"operator\"", false, NULL},
{L"PRIVATE", 329, 1, L"\"private\"", false, NULL},
{L"PROTECTED", 330, 1, L"\"protected\"", false, NULL},
{L"PUBLIC", 331, 1, L"\"public\"", false, NULL},
{L"REGISTER", 332, 1, L"\"register\"", false, NULL},
{L"REINTERPRET_CAST", 333, 1, L"\"reinterprect_cast\"", false, NULL},
{L"RETURN", 334, 1, L"\"return\"", false, NULL},
{L"SHORT", 335, 1, L"\"short\"", false, NULL},
{L"SIGNED", 336, 1, L"\"signed\"", false, NULL},
{L"SIZEOF", 337, 1, L"\"sizeof\"", false, NULL},
{L"STATIC", 338, 1, L"\"static\"", false, NULL},
{L"STATIC_CAST", 339, 1, L"\"static_cast\"", false, NULL},
{L"STRUCT", 340, 1, L"\"struct\"", false, NULL},
{L"SWITCH", 341, 1, L"\"switch\"", false, NULL},
{L"TEMPLATE", 342, 1, L"\"template\"", false, NULL},
{L"THIS", 343, 1, L"\"this\"", false, NULL},
{L"THROW", 344, 1, L"\"throw\"", false, NULL},
{L"TRUE", 345, 1, L"\"true\"", false, NULL},
{L"TRY", 346, 1, L"\"try\"", false, NULL},
{L"TYPEDEF", 347, 1, L"\"typedef\"", false, NULL},
{L"TYPEID", 348, 1, L"\"typeid\"", false, NULL},
{L"TYPENAME", 349, 1, L"\"typename\"", false, NULL},
{L"UNION", 350, 1, L"\"union\"", false, NULL},
{L"UNSIGNED", 351, 1, L"\"unsigned\"", false, NULL},
{L"USING", 352, 1, L"\"using\"", false, NULL},
{L"VIRTUAL", 353, 1, L"\"virtual\"", false, NULL},
{L"VOID", 354, 1, L"\"void\"", false, NULL},
{L"VOLATILE", 355, 1, L"\"volatile\"", false, NULL},
{L"WCHAR_T", 356, 1, L"\"wchar_t\"", false, NULL},
{L"WHILE", 357, 1, L"\"while\"", false, NULL},
{L";", 358, 0, L"\";\"", false, NULL},
{L"{", 359, 0, L"\"{\"", false, NULL},
{L"}", 360, 0, L"\"}\"", false, NULL},
{L",", 361, 0, L"\",\"", false, NULL},
{L":", 362, 0, L"\":\"", false, NULL},
{L"=", 363, 0, L"\"=\"", false, NULL},
{L"(", 364, 0, L"\"(\"", false, NULL},
{L")", 365, 0, L"\")\"", false, NULL},
{L"[", 366, 0, L"\"[\"", false, NULL},
{L"]", 367, 0, L"\"]\"", false, NULL},
{L".", 368, 0, L"\".\"", false, NULL},
{L"&", 369, 0, L"\"&\"", false, NULL},
{L"!", 370, 0, L"\"!\"", false, NULL},
{L"~", 371, 0, L"\"~\"", false, NULL},
{L"-", 372, 0, L"\"-\"", false, NULL},
{L"+", 373, 0, L"\"+\"", false, NULL},
{L"*", 374, 0, L"\"*\"", false, NULL},
{L"/", 375, 0, L"\"/\"", false, NULL},
{L"%", 376, 0, L"\"%\"", false, NULL},
{L"<", 377, 0, L"\"<\"", false, NULL},
{L">", 378, 0, L"\">\"", false, NULL},
{L"^", 379, 0, L"\"^\"", false, NULL},
{L"|", 380, 0, L"\"|\"", false, NULL},
{L"?", 381, 0, L"\"?\"", false, NULL},
{L"EOI", 0, 2, L"$", false, NULL}
};

#define __TERM_COUNT__ ((size_t)126)

static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {
NULL
};

#define __PREC_COUNT__ ((size_t)0)

/*typedef_name	:	TYPEDEF_NAME */
static psrNode_t* AR_STDCALL handle_typedef_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namespace_name	:	original_namespace_name */
/*namespace_name	:	namespace_alias */
static psrNode_t* AR_STDCALL handle_namespace_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*original_namespace_name	:	NAMESPACE_NAME */
static psrNode_t* AR_STDCALL handle_original_namespace_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namespace_alias	:	NAMESPACE_NAME */
static psrNode_t* AR_STDCALL handle_namespace_alias(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*class_name	:	CLASS_NAME */
/*class_name	:	template_id */
static psrNode_t* AR_STDCALL handle_class_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*enum_name	:	ENUM_NAME */
static psrNode_t* AR_STDCALL handle_enum_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*template_name	:	TEMPLATE_NAME */
static psrNode_t* AR_STDCALL handle_template_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*identifier	:	IDENTIFIER */
static psrNode_t* AR_STDCALL handle_identifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*literal	:	integer_literal */
/*literal	:	character_literal */
/*literal	:	floating_literal */
/*literal	:	string_literal */
/*literal	:	boolean_literal */
static psrNode_t* AR_STDCALL handle_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*integer_literal	:	OCT_CONSTANT */
/*integer_literal	:	HEX_CONSTANT */
/*integer_literal	:	DEC_CONSTANT */
static psrNode_t* AR_STDCALL handle_integer_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*character_literal	:	CHAR_CONSTANT */
static psrNode_t* AR_STDCALL handle_character_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*floating_literal	:	FLOAT_CONSTANT */
static psrNode_t* AR_STDCALL handle_floating_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*string_literal	:	STRING_LITERAL */
static psrNode_t* AR_STDCALL handle_string_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*boolean_literal	:	TRUE */
/*boolean_literal	:	FALSE */
static psrNode_t* AR_STDCALL handle_boolean_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*translation_unit	:	declaration_seq_opt */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*primary_expression	:	literal */
/*primary_expression	:	THIS */
/*primary_expression	:	COLONCOLON identifier */
/*primary_expression	:	COLONCOLON operator_function_id */
/*primary_expression	:	COLONCOLON qualified_id */
/*primary_expression	:	( expression ) */
/*primary_expression	:	id_expression */
static psrNode_t* AR_STDCALL handle_primary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*id_expression	:	unqualified_id */
/*id_expression	:	qualified_id */
static psrNode_t* AR_STDCALL handle_id_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unqualified_id	:	identifier */
/*unqualified_id	:	operator_function_id */
/*unqualified_id	:	conversion_function_id */
/*unqualified_id	:	~ class_name */
/*unqualified_id	:	template_id */
static psrNode_t* AR_STDCALL handle_unqualified_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*qualified_id	:	nested_name_specifier TEMPLATE_opt unqualified_id */
static psrNode_t* AR_STDCALL handle_qualified_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*nested_name_specifier	:	class_or_namespace_name COLONCOLON nested_name_specifier_opt */
static psrNode_t* AR_STDCALL handle_nested_name_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*class_or_namespace_name	:	class_name */
/*class_or_namespace_name	:	namespace_name */
static psrNode_t* AR_STDCALL handle_class_or_namespace_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*postfix_expression	:	primary_expression */
/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression ( expression_list_opt ) */
/*postfix_expression	:	simple_type_specifier ( expression_list_opt ) */
/*postfix_expression	:	postfix_expression . TEMPLATE_opt COLONCOLON_opt id_expression */
/*postfix_expression	:	postfix_expression ARROW TEMPLATE_opt COLONCOLON_opt id_expression */
/*postfix_expression	:	postfix_expression . pseudo_destructor_name */
/*postfix_expression	:	postfix_expression ARROW pseudo_destructor_name */
/*postfix_expression	:	postfix_expression PLUSPLUS */
/*postfix_expression	:	postfix_expression MINUSMINUS */
/*postfix_expression	:	DYNAMIC_CAST < type_id > ( expression ) */
/*postfix_expression	:	STATIC_CAST < type_id > ( expression ) */
/*postfix_expression	:	REINTERPRET_CAST < type_id > ( expression ) */
/*postfix_expression	:	CONST_CAST < type_id > ( expression ) */
/*postfix_expression	:	TYPEID ( expression ) */
/*postfix_expression	:	TYPEID ( type_id ) */
static psrNode_t* AR_STDCALL handle_postfix_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_list	:	assignment_expression */
/*expression_list	:	expression_list , assignment_expression */
static psrNode_t* AR_STDCALL handle_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*pseudo_destructor_name	:	COLONCOLON_opt nested_name_specifier_opt type_name COLONCOLON ~ type_name */
/*pseudo_destructor_name	:	COLONCOLON_opt nested_name_specifier_opt ~ type_name */
static psrNode_t* AR_STDCALL handle_pseudo_destructor_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_expression	:	postfix_expression */
/*unary_expression	:	PLUSPLUS cast_expression */
/*unary_expression	:	MINUSMINUS cast_expression */
/*unary_expression	:	unary_operator cast_expression */
/*unary_expression	:	SIZEOF unary_expression */
/*unary_expression	:	SIZEOF ( type_id ) */
/*unary_expression	:	new_expression */
/*unary_expression	:	delete_expression */
static psrNode_t* AR_STDCALL handle_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_operator	:	* */
/*unary_operator	:	& */
/*unary_operator	:	+ */
/*unary_operator	:	- */
/*unary_operator	:	! */
/*unary_operator	:	~ */
static psrNode_t* AR_STDCALL handle_unary_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*new_expression	:	COLONCOLON_opt NEW new_placement_opt new_type_id new_initializer_opt */
/*new_expression	:	COLONCOLON_opt NEW new_placement_opt ( type_id ) new_initializer_opt */
static psrNode_t* AR_STDCALL handle_new_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*new_placement	:	( expression_list ) */
static psrNode_t* AR_STDCALL handle_new_placement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*new_type_id	:	type_specifier_seq new_declarator_opt */
static psrNode_t* AR_STDCALL handle_new_type_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*new_declarator	:	ptr_operator new_declarator_opt */
/*new_declarator	:	direct_new_declarator */
static psrNode_t* AR_STDCALL handle_new_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*direct_new_declarator	:	[ expression ] */
/*direct_new_declarator	:	direct_new_declarator [ constant_expression ] */
static psrNode_t* AR_STDCALL handle_direct_new_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*new_initializer	:	( expression_list_opt ) */
static psrNode_t* AR_STDCALL handle_new_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*delete_expression	:	COLONCOLON_opt DELETE cast_expression */
/*delete_expression	:	COLONCOLON_opt DELETE [ ] cast_expression */
static psrNode_t* AR_STDCALL handle_delete_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*cast_expression	:	unary_expression */
/*cast_expression	:	( type_id ) cast_expression */
static psrNode_t* AR_STDCALL handle_cast_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*pm_expression	:	cast_expression */
/*pm_expression	:	pm_expression DOTSTAR cast_expression */
/*pm_expression	:	pm_expression ARROWSTAR cast_expression */
static psrNode_t* AR_STDCALL handle_pm_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*multiplicative_expression	:	pm_expression */
/*multiplicative_expression	:	multiplicative_expression * pm_expression */
/*multiplicative_expression	:	multiplicative_expression / pm_expression */
/*multiplicative_expression	:	multiplicative_expression % pm_expression */
static psrNode_t* AR_STDCALL handle_multiplicative_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*additive_expression	:	multiplicative_expression */
/*additive_expression	:	additive_expression + multiplicative_expression */
/*additive_expression	:	additive_expression - multiplicative_expression */
static psrNode_t* AR_STDCALL handle_additive_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*shift_expression	:	additive_expression */
/*shift_expression	:	shift_expression SL additive_expression */
/*shift_expression	:	shift_expression SR additive_expression */
static psrNode_t* AR_STDCALL handle_shift_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*relational_expression	:	shift_expression */
/*relational_expression	:	relational_expression < shift_expression */
/*relational_expression	:	relational_expression > shift_expression */
/*relational_expression	:	relational_expression LTEQ shift_expression */
/*relational_expression	:	relational_expression GTEQ shift_expression */
static psrNode_t* AR_STDCALL handle_relational_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*equality_expression	:	relational_expression */
/*equality_expression	:	equality_expression EQ relational_expression */
/*equality_expression	:	equality_expression NOTEQ relational_expression */
static psrNode_t* AR_STDCALL handle_equality_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*and_expression	:	equality_expression */
/*and_expression	:	and_expression & equality_expression */
static psrNode_t* AR_STDCALL handle_and_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*exclusive_or_expression	:	and_expression */
/*exclusive_or_expression	:	exclusive_or_expression ^ and_expression */
static psrNode_t* AR_STDCALL handle_exclusive_or_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*inclusive_or_expression	:	exclusive_or_expression */
/*inclusive_or_expression	:	inclusive_or_expression | exclusive_or_expression */
static psrNode_t* AR_STDCALL handle_inclusive_or_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*logical_and_expression	:	inclusive_or_expression */
/*logical_and_expression	:	logical_and_expression ANDAND inclusive_or_expression */
static psrNode_t* AR_STDCALL handle_logical_and_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*logical_or_expression	:	logical_and_expression */
/*logical_or_expression	:	logical_or_expression OROR logical_and_expression */
static psrNode_t* AR_STDCALL handle_logical_or_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*conditional_expression	:	logical_or_expression */
/*conditional_expression	:	logical_or_expression ? expression : assignment_expression */
static psrNode_t* AR_STDCALL handle_conditional_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*assignment_expression	:	conditional_expression */
/*assignment_expression	:	logical_or_expression assignment_operator assignment_expression */
/*assignment_expression	:	throw_expression */
static psrNode_t* AR_STDCALL handle_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*assignment_operator	:	= */
/*assignment_operator	:	MULEQ */
/*assignment_operator	:	DIVEQ */
/*assignment_operator	:	MODEQ */
/*assignment_operator	:	ADDEQ */
/*assignment_operator	:	SUBEQ */
/*assignment_operator	:	SREQ */
/*assignment_operator	:	SLEQ */
/*assignment_operator	:	ANDEQ */
/*assignment_operator	:	XOREQ */
/*assignment_operator	:	OREQ */
static psrNode_t* AR_STDCALL handle_assignment_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression	:	assignment_expression */
/*expression	:	expression , assignment_expression */
static psrNode_t* AR_STDCALL handle_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*constant_expression	:	conditional_expression */
static psrNode_t* AR_STDCALL handle_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement	:	labeled_statement */
/*statement	:	expression_statement */
/*statement	:	compound_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
/*statement	:	declaration_statement */
/*statement	:	try_block */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*labeled_statement	:	identifier : statement */
/*labeled_statement	:	CASE constant_expression : statement */
/*labeled_statement	:	DEFAULT : statement */
static psrNode_t* AR_STDCALL handle_labeled_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_statement	:	expression_opt ; */
static psrNode_t* AR_STDCALL handle_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*compound_statement	:	{ statement_seq_opt } */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement_seq	:	statement */
/*statement_seq	:	statement_seq statement */
static psrNode_t* AR_STDCALL handle_statement_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*selection_statement	:	IF ( condition ) statement */
/*selection_statement	:	IF ( condition ) statement ELSE statement */
/*selection_statement	:	SWITCH ( condition ) statement */
static psrNode_t* AR_STDCALL handle_selection_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*condition	:	expression */
/*condition	:	type_specifier_seq declarator = assignment_expression */
static psrNode_t* AR_STDCALL handle_condition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*iteration_statement	:	WHILE ( condition ) statement */
/*iteration_statement	:	DO statement WHILE ( expression ) ; */
/*iteration_statement	:	FOR ( for_init_statement condition_opt ; expression_opt ) statement */
static psrNode_t* AR_STDCALL handle_iteration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*for_init_statement	:	expression_statement */
/*for_init_statement	:	simple_declaration */
static psrNode_t* AR_STDCALL handle_for_init_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*jump_statement	:	BREAK ; */
/*jump_statement	:	CONTINUE ; */
/*jump_statement	:	RETURN expression_opt ; */
/*jump_statement	:	GOTO identifier ; */
static psrNode_t* AR_STDCALL handle_jump_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration_statement	:	block_declaration */
static psrNode_t* AR_STDCALL handle_declaration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration_seq	:	declaration */
/*declaration_seq	:	declaration_seq declaration */
static psrNode_t* AR_STDCALL handle_declaration_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration	:	block_declaration */
/*declaration	:	function_definition */
/*declaration	:	template_declaration */
/*declaration	:	explicit_instantiation */
/*declaration	:	explicit_specialization */
/*declaration	:	linkage_specification */
/*declaration	:	namespace_definition */
static psrNode_t* AR_STDCALL handle_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*block_declaration	:	simple_declaration */
/*block_declaration	:	asm_definition */
/*block_declaration	:	namespace_alias_definition */
/*block_declaration	:	using_declaration */
/*block_declaration	:	using_directive */
static psrNode_t* AR_STDCALL handle_block_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*simple_declaration	:	decl_specifier_seq_opt init_declarator_list_opt ; */
static psrNode_t* AR_STDCALL handle_simple_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*decl_specifier	:	storage_class_specifier */
/*decl_specifier	:	type_specifier */
/*decl_specifier	:	function_specifier */
/*decl_specifier	:	FRIEND */
/*decl_specifier	:	TYPEDEF */
static psrNode_t* AR_STDCALL handle_decl_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*decl_specifier_seq	:	decl_specifier_seq_opt decl_specifier */
static psrNode_t* AR_STDCALL handle_decl_specifier_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*storage_class_specifier	:	AUTO */
/*storage_class_specifier	:	REGISTER */
/*storage_class_specifier	:	STATIC */
/*storage_class_specifier	:	EXTERN */
/*storage_class_specifier	:	MUTABLE */
static psrNode_t* AR_STDCALL handle_storage_class_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_specifier	:	INLINE */
/*function_specifier	:	VIRTUAL */
/*function_specifier	:	EXPLICIT */
static psrNode_t* AR_STDCALL handle_function_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_specifier	:	simple_type_specifier */
/*type_specifier	:	class_specifier */
/*type_specifier	:	enum_specifier */
/*type_specifier	:	elaborated_type_specifier */
/*type_specifier	:	cv_qualifier */
static psrNode_t* AR_STDCALL handle_type_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*simple_type_specifier	:	COLONCOLON_opt nested_name_specifier_opt type_name */
/*simple_type_specifier	:	CHAR */
/*simple_type_specifier	:	WCHAR_T */
/*simple_type_specifier	:	BOOL */
/*simple_type_specifier	:	SHORT */
/*simple_type_specifier	:	INT */
/*simple_type_specifier	:	LONG */
/*simple_type_specifier	:	SIGNED */
/*simple_type_specifier	:	UNSIGNED */
/*simple_type_specifier	:	FLOAT */
/*simple_type_specifier	:	DOUBLE */
/*simple_type_specifier	:	VOID */
static psrNode_t* AR_STDCALL handle_simple_type_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_name	:	class_name */
/*type_name	:	enum_name */
/*type_name	:	typedef_name */
static psrNode_t* AR_STDCALL handle_type_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*elaborated_type_specifier	:	class_key COLONCOLON_opt nested_name_specifier_opt identifier */
/*elaborated_type_specifier	:	ENUM COLONCOLON_opt nested_name_specifier_opt identifier */
/*elaborated_type_specifier	:	TYPENAME COLONCOLON_opt nested_name_specifier identifier */
/*elaborated_type_specifier	:	TYPENAME COLONCOLON_opt nested_name_specifier identifier < template_argument_list > */
static psrNode_t* AR_STDCALL handle_elaborated_type_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*enum_specifier	:	ENUM identifier_opt { enumerator_list_opt } */
static psrNode_t* AR_STDCALL handle_enum_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*enumerator_list	:	enumerator_definition */
/*enumerator_list	:	enumerator_list , enumerator_definition */
static psrNode_t* AR_STDCALL handle_enumerator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*enumerator_definition	:	enumerator */
/*enumerator_definition	:	enumerator = constant_expression */
static psrNode_t* AR_STDCALL handle_enumerator_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*enumerator	:	identifier */
static psrNode_t* AR_STDCALL handle_enumerator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namespace_definition	:	named_namespace_definition */
/*namespace_definition	:	unnamed_namespace_definition */
static psrNode_t* AR_STDCALL handle_namespace_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*named_namespace_definition	:	original_namespace_definition */
/*named_namespace_definition	:	extension_namespace_definition */
static psrNode_t* AR_STDCALL handle_named_namespace_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*original_namespace_definition	:	NAMESPACE identifier { namespace_body } */
static psrNode_t* AR_STDCALL handle_original_namespace_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*extension_namespace_definition	:	NAMESPACE original_namespace_name { namespace_body } */
static psrNode_t* AR_STDCALL handle_extension_namespace_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unnamed_namespace_definition	:	NAMESPACE { namespace_body } */
static psrNode_t* AR_STDCALL handle_unnamed_namespace_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namespace_body	:	declaration_seq_opt */
static psrNode_t* AR_STDCALL handle_namespace_body(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*namespace_alias_definition	:	NAMESPACE identifier = qualified_namespace_specifier ; */
static psrNode_t* AR_STDCALL handle_namespace_alias_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*qualified_namespace_specifier	:	COLONCOLON_opt nested_name_specifier_opt namespace_name */
static psrNode_t* AR_STDCALL handle_qualified_namespace_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*using_declaration	:	USING TYPENAME_opt COLONCOLON_opt nested_name_specifier unqualified_id ; */
/*using_declaration	:	USING COLONCOLON unqualified_id ; */
static psrNode_t* AR_STDCALL handle_using_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*using_directive	:	USING NAMESPACE COLONCOLON_opt nested_name_specifier_opt namespace_name ; */
static psrNode_t* AR_STDCALL handle_using_directive(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*asm_definition	:	ASM ( string_literal ) ; */
static psrNode_t* AR_STDCALL handle_asm_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*linkage_specification	:	EXTERN string_literal { declaration_seq_opt } */
/*linkage_specification	:	EXTERN string_literal declaration */
static psrNode_t* AR_STDCALL handle_linkage_specification(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL handle_init_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator	:	declarator initializer_opt */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declarator	:	direct_declarator */
/*declarator	:	ptr_operator declarator */
static psrNode_t* AR_STDCALL handle_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*direct_declarator	:	declarator_id */
/*direct_declarator	:	direct_declarator ( parameter_declaration_clause ) cv_qualifier_seq_opt exception_specification_opt */
/*direct_declarator	:	direct_declarator [ constant_expression_opt ] */
/*direct_declarator	:	( declarator ) */
static psrNode_t* AR_STDCALL handle_direct_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*ptr_operator	:	* cv_qualifier_seq_opt */
/*ptr_operator	:	& */
/*ptr_operator	:	COLONCOLON_opt nested_name_specifier * cv_qualifier_seq_opt */
static psrNode_t* AR_STDCALL handle_ptr_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*cv_qualifier_seq	:	cv_qualifier cv_qualifier_seq_opt */
static psrNode_t* AR_STDCALL handle_cv_qualifier_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*cv_qualifier	:	CONST */
/*cv_qualifier	:	VOLATILE */
static psrNode_t* AR_STDCALL handle_cv_qualifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declarator_id	:	COLONCOLON_opt id_expression */
/*declarator_id	:	COLONCOLON_opt nested_name_specifier_opt type_name */
static psrNode_t* AR_STDCALL handle_declarator_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_id	:	type_specifier_seq abstract_declarator_opt */
static psrNode_t* AR_STDCALL handle_type_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_specifier_seq	:	type_specifier type_specifier_seq_opt */
static psrNode_t* AR_STDCALL handle_type_specifier_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*abstract_declarator	:	ptr_operator abstract_declarator_opt */
/*abstract_declarator	:	direct_abstract_declarator */
static psrNode_t* AR_STDCALL handle_abstract_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*direct_abstract_declarator	:	direct_abstract_declarator_opt ( parameter_declaration_clause ) cv_qualifier_seq_opt exception_specification_opt */
/*direct_abstract_declarator	:	direct_abstract_declarator_opt [ constant_expression_opt ] */
/*direct_abstract_declarator	:	( abstract_declarator ) */
static psrNode_t* AR_STDCALL handle_direct_abstract_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*parameter_declaration_clause	:	parameter_declaration_list_opt ELLIPSIS_opt */
/*parameter_declaration_clause	:	parameter_declaration_list , ELLIPSIS */
static psrNode_t* AR_STDCALL handle_parameter_declaration_clause(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*parameter_declaration_list	:	parameter_declaration */
/*parameter_declaration_list	:	parameter_declaration_list , parameter_declaration */
static psrNode_t* AR_STDCALL handle_parameter_declaration_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*parameter_declaration	:	decl_specifier_seq declarator */
/*parameter_declaration	:	decl_specifier_seq declarator = assignment_expression */
/*parameter_declaration	:	decl_specifier_seq abstract_declarator_opt */
/*parameter_declaration	:	decl_specifier_seq abstract_declarator_opt = assignment_expression */
static psrNode_t* AR_STDCALL handle_parameter_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_definition	:	decl_specifier_seq_opt declarator ctor_initializer_opt function_body */
/*function_definition	:	decl_specifier_seq_opt declarator function_try_block */
static psrNode_t* AR_STDCALL handle_function_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_body	:	compound_statement */
static psrNode_t* AR_STDCALL handle_function_body(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*initializer	:	= initializer_clause */
/*initializer	:	( expression_list ) */
static psrNode_t* AR_STDCALL handle_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*initializer_clause	:	assignment_expression */
/*initializer_clause	:	{ initializer_list COMMA_opt } */
/*initializer_clause	:	{ } */
static psrNode_t* AR_STDCALL handle_initializer_clause(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*initializer_list	:	initializer_clause */
/*initializer_list	:	initializer_list , initializer_clause */
static psrNode_t* AR_STDCALL handle_initializer_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*class_specifier	:	class_head { member_specification_opt } */
static psrNode_t* AR_STDCALL handle_class_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*class_head	:	class_key identifier_opt base_clause_opt */
/*class_head	:	class_key nested_name_specifier identifier base_clause_opt */
static psrNode_t* AR_STDCALL handle_class_head(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*class_key	:	CLASS */
/*class_key	:	STRUCT */
/*class_key	:	UNION */
static psrNode_t* AR_STDCALL handle_class_key(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*member_specification	:	member_declaration member_specification_opt */
/*member_specification	:	access_specifier : member_specification_opt */
static psrNode_t* AR_STDCALL handle_member_specification(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*member_declaration	:	decl_specifier_seq_opt member_declarator_list_opt ; */
/*member_declaration	:	function_definition SEMICOLON_opt */
/*member_declaration	:	qualified_id ; */
/*member_declaration	:	using_declaration */
/*member_declaration	:	template_declaration */
static psrNode_t* AR_STDCALL handle_member_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*member_declarator_list	:	member_declarator */
/*member_declarator_list	:	member_declarator_list , member_declarator */
static psrNode_t* AR_STDCALL handle_member_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*member_declarator	:	declarator pure_specifier_opt */
/*member_declarator	:	declarator constant_initializer_opt */
/*member_declarator	:	identifier_opt : constant_expression */
static psrNode_t* AR_STDCALL handle_member_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*pure_specifier	:	= integer_literal */
static psrNode_t* AR_STDCALL handle_pure_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*constant_initializer	:	= constant_expression */
static psrNode_t* AR_STDCALL handle_constant_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*base_clause	:	: base_specifier_list */
static psrNode_t* AR_STDCALL handle_base_clause(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*base_specifier_list	:	base_specifier */
/*base_specifier_list	:	base_specifier_list , base_specifier */
static psrNode_t* AR_STDCALL handle_base_specifier_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*base_specifier	:	COLONCOLON_opt nested_name_specifier_opt class_name */
/*base_specifier	:	VIRTUAL access_specifier_opt COLONCOLON_opt nested_name_specifier_opt class_name */
/*base_specifier	:	access_specifier VIRTUAL_opt COLONCOLON_opt nested_name_specifier_opt class_name */
static psrNode_t* AR_STDCALL handle_base_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*access_specifier	:	PRIVATE */
/*access_specifier	:	PROTECTED */
/*access_specifier	:	PUBLIC */
static psrNode_t* AR_STDCALL handle_access_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*conversion_function_id	:	OPERATOR conversion_type_id */
static psrNode_t* AR_STDCALL handle_conversion_function_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*conversion_type_id	:	type_specifier_seq conversion_declarator_opt */
static psrNode_t* AR_STDCALL handle_conversion_type_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*conversion_declarator	:	ptr_operator conversion_declarator_opt */
static psrNode_t* AR_STDCALL handle_conversion_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*ctor_initializer	:	: mem_initializer_list */
static psrNode_t* AR_STDCALL handle_ctor_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*mem_initializer_list	:	mem_initializer */
/*mem_initializer_list	:	mem_initializer , mem_initializer_list */
static psrNode_t* AR_STDCALL handle_mem_initializer_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*mem_initializer	:	mem_initializer_id ( expression_list_opt ) */
static psrNode_t* AR_STDCALL handle_mem_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*mem_initializer_id	:	COLONCOLON_opt nested_name_specifier_opt class_name */
/*mem_initializer_id	:	identifier */
static psrNode_t* AR_STDCALL handle_mem_initializer_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*operator_function_id	:	OPERATOR operator */
static psrNode_t* AR_STDCALL handle_operator_function_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*operator	:	NEW */
/*operator	:	DELETE */
/*operator	:	NEW [ ] */
/*operator	:	DELETE [ ] */
/*operator	:	+ */
/*operator	:	* */
/*operator	:	/ */
/*operator	:	% */
/*operator	:	^ */
/*operator	:	& */
/*operator	:	| */
/*operator	:	~ */
/*operator	:	! */
/*operator	:	= */
/*operator	:	< */
/*operator	:	> */
/*operator	:	ADDEQ */
/*operator	:	SUBEQ */
/*operator	:	MULEQ */
/*operator	:	DIVEQ */
/*operator	:	MODEQ */
/*operator	:	XOREQ */
/*operator	:	ANDEQ */
/*operator	:	OREQ */
/*operator	:	SL */
/*operator	:	SR */
/*operator	:	SREQ */
/*operator	:	SLEQ */
/*operator	:	EQ */
/*operator	:	NOTEQ */
/*operator	:	LTEQ */
/*operator	:	GTEQ */
/*operator	:	ANDAND */
/*operator	:	OROR */
/*operator	:	PLUSPLUS */
/*operator	:	MINUSMINUS */
/*operator	:	, */
/*operator	:	ARROWSTAR */
/*operator	:	ARROW */
/*operator	:	( ) */
/*operator	:	[ ] */
static psrNode_t* AR_STDCALL handle_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*template_declaration	:	EXPORT_opt TEMPLATE < template_parameter_list > declaration */
static psrNode_t* AR_STDCALL handle_template_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*template_parameter_list	:	template_parameter */
/*template_parameter_list	:	template_parameter_list , template_parameter */
static psrNode_t* AR_STDCALL handle_template_parameter_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*template_parameter	:	type_parameter */
/*template_parameter	:	parameter_declaration */
static psrNode_t* AR_STDCALL handle_template_parameter(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_parameter	:	CLASS identifier_opt */
/*type_parameter	:	CLASS identifier_opt = type_id */
/*type_parameter	:	TYPENAME identifier_opt */
/*type_parameter	:	TYPENAME identifier_opt = type_id */
/*type_parameter	:	TEMPLATE < template_parameter_list > CLASS identifier_opt */
/*type_parameter	:	TEMPLATE < template_parameter_list > CLASS identifier_opt = template_name */
static psrNode_t* AR_STDCALL handle_type_parameter(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*template_id	:	template_name < template_argument_list > */
static psrNode_t* AR_STDCALL handle_template_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*template_argument_list	:	template_argument */
/*template_argument_list	:	template_argument_list , template_argument */
static psrNode_t* AR_STDCALL handle_template_argument_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*template_argument	:	assignment_expression */
/*template_argument	:	type_id */
/*template_argument	:	template_name */
static psrNode_t* AR_STDCALL handle_template_argument(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*explicit_instantiation	:	TEMPLATE declaration */
static psrNode_t* AR_STDCALL handle_explicit_instantiation(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*explicit_specialization	:	TEMPLATE < > declaration */
static psrNode_t* AR_STDCALL handle_explicit_specialization(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*try_block	:	TRY compound_statement handler_seq */
static psrNode_t* AR_STDCALL handle_try_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*function_try_block	:	TRY ctor_initializer_opt function_body handler_seq */
static psrNode_t* AR_STDCALL handle_function_try_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*handler_seq	:	handler handler_seq_opt */
static psrNode_t* AR_STDCALL handle_handler_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*handler	:	CATCH ( exception_declaration ) compound_statement */
static psrNode_t* AR_STDCALL handle_handler(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*exception_declaration	:	type_specifier_seq declarator */
/*exception_declaration	:	type_specifier_seq abstract_declarator */
/*exception_declaration	:	type_specifier_seq */
/*exception_declaration	:	ELLIPSIS */
static psrNode_t* AR_STDCALL handle_exception_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*throw_expression	:	THROW assignment_expression_opt */
static psrNode_t* AR_STDCALL handle_throw_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*exception_specification	:	THROW ( type_id_list_opt ) */
static psrNode_t* AR_STDCALL handle_exception_specification(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_id_list	:	type_id */
/*type_id_list	:	type_id_list , type_id */
static psrNode_t* AR_STDCALL handle_type_id_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*declaration_seq_opt	:	declaration_seq */
static psrNode_t* AR_STDCALL handle_declaration_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*TEMPLATE_opt	:	TEMPLATE */
static psrNode_t* AR_STDCALL handle_TEMPLATE_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*nested_name_specifier_opt	:	nested_name_specifier */
static psrNode_t* AR_STDCALL handle_nested_name_specifier_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_list_opt	:	expression_list */
static psrNode_t* AR_STDCALL handle_expression_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*COLONCOLON_opt	:	COLONCOLON */
static psrNode_t* AR_STDCALL handle_COLONCOLON_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*new_placement_opt	:	new_placement */
static psrNode_t* AR_STDCALL handle_new_placement_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*new_initializer_opt	:	new_initializer */
static psrNode_t* AR_STDCALL handle_new_initializer_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*new_declarator_opt	:	new_declarator */
static psrNode_t* AR_STDCALL handle_new_declarator_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expression_opt	:	expression */
static psrNode_t* AR_STDCALL handle_expression_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*statement_seq_opt	:	statement_seq */
static psrNode_t* AR_STDCALL handle_statement_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*condition_opt	:	condition */
static psrNode_t* AR_STDCALL handle_condition_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*decl_specifier_seq_opt	:	decl_specifier_seq */
static psrNode_t* AR_STDCALL handle_decl_specifier_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*init_declarator_list_opt	:	init_declarator_list */
static psrNode_t* AR_STDCALL handle_init_declarator_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*identifier_opt	:	identifier */
static psrNode_t* AR_STDCALL handle_identifier_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*enumerator_list_opt	:	enumerator_list */
static psrNode_t* AR_STDCALL handle_enumerator_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*TYPENAME_opt	:	TYPENAME */
static psrNode_t* AR_STDCALL handle_TYPENAME_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*initializer_opt	:	initializer */
static psrNode_t* AR_STDCALL handle_initializer_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*cv_qualifier_seq_opt	:	cv_qualifier_seq */
static psrNode_t* AR_STDCALL handle_cv_qualifier_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*exception_specification_opt	:	exception_specification */
static psrNode_t* AR_STDCALL handle_exception_specification_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*constant_expression_opt	:	constant_expression */
static psrNode_t* AR_STDCALL handle_constant_expression_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*abstract_declarator_opt	:	abstract_declarator */
static psrNode_t* AR_STDCALL handle_abstract_declarator_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_specifier_seq_opt	:	type_specifier_seq */
static psrNode_t* AR_STDCALL handle_type_specifier_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*direct_abstract_declarator_opt	:	direct_abstract_declarator */
static psrNode_t* AR_STDCALL handle_direct_abstract_declarator_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*parameter_declaration_list_opt	:	parameter_declaration_list */
static psrNode_t* AR_STDCALL handle_parameter_declaration_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*ELLIPSIS_opt	:	ELLIPSIS */
static psrNode_t* AR_STDCALL handle_ELLIPSIS_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*ctor_initializer_opt	:	ctor_initializer */
static psrNode_t* AR_STDCALL handle_ctor_initializer_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*COMMA_opt	:	, */
static psrNode_t* AR_STDCALL handle_COMMA_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*member_specification_opt	:	member_specification */
static psrNode_t* AR_STDCALL handle_member_specification_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*base_clause_opt	:	base_clause */
static psrNode_t* AR_STDCALL handle_base_clause_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*member_declarator_list_opt	:	member_declarator_list */
static psrNode_t* AR_STDCALL handle_member_declarator_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*SEMICOLON_opt	:	; */
static psrNode_t* AR_STDCALL handle_SEMICOLON_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*pure_specifier_opt	:	pure_specifier */
static psrNode_t* AR_STDCALL handle_pure_specifier_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*constant_initializer_opt	:	constant_initializer */
static psrNode_t* AR_STDCALL handle_constant_initializer_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*access_specifier_opt	:	access_specifier */
static psrNode_t* AR_STDCALL handle_access_specifier_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*VIRTUAL_opt	:	VIRTUAL */
static psrNode_t* AR_STDCALL handle_VIRTUAL_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*conversion_declarator_opt	:	conversion_declarator */
static psrNode_t* AR_STDCALL handle_conversion_declarator_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*EXPORT_opt	:	EXPORT */
static psrNode_t* AR_STDCALL handle_EXPORT_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*handler_seq_opt	:	handler_seq */
static psrNode_t* AR_STDCALL handle_handler_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*assignment_expression_opt	:	assignment_expression */
static psrNode_t* AR_STDCALL handle_assignment_expression_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*type_id_list_opt	:	type_id_list */
static psrNode_t* AR_STDCALL handle_type_id_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);




static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"typedef_name  :  TYPEDEF_NAME ", NULL, handle_typedef_name, 0},
{L"namespace_name  :  original_namespace_name ", NULL, handle_namespace_name, 0},
{L"namespace_name  :  namespace_alias ", NULL, handle_namespace_name, 0},
{L"original_namespace_name  :  NAMESPACE_NAME ", NULL, handle_original_namespace_name, 0},
{L"namespace_alias  :  NAMESPACE_NAME ", NULL, handle_namespace_alias, 0},
{L"class_name  :  CLASS_NAME ", NULL, handle_class_name, 0},
{L"class_name  :  template_id ", NULL, handle_class_name, 0},
{L"enum_name  :  ENUM_NAME ", NULL, handle_enum_name, 0},
{L"template_name  :  TEMPLATE_NAME ", NULL, handle_template_name, 0},
{L"identifier  :  IDENTIFIER ", NULL, handle_identifier, 0},
{L"literal  :  integer_literal ", NULL, handle_literal, 0},
{L"literal  :  character_literal ", NULL, handle_literal, 0},
{L"literal  :  floating_literal ", NULL, handle_literal, 0},
{L"literal  :  string_literal ", NULL, handle_literal, 0},
{L"literal  :  boolean_literal ", NULL, handle_literal, 0},
{L"integer_literal  :  OCT_CONSTANT ", NULL, handle_integer_literal, 0},
{L"integer_literal  :  HEX_CONSTANT ", NULL, handle_integer_literal, 0},
{L"integer_literal  :  DEC_CONSTANT ", NULL, handle_integer_literal, 0},
{L"character_literal  :  CHAR_CONSTANT ", NULL, handle_character_literal, 0},
{L"floating_literal  :  FLOAT_CONSTANT ", NULL, handle_floating_literal, 0},
{L"string_literal  :  STRING_LITERAL ", NULL, handle_string_literal, 0},
{L"boolean_literal  :  TRUE ", NULL, handle_boolean_literal, 0},
{L"boolean_literal  :  FALSE ", NULL, handle_boolean_literal, 0},
{L"translation_unit  :  declaration_seq_opt ", NULL, handle_translation_unit, 0},
{L"primary_expression  :  literal ", NULL, handle_primary_expression, 0},
{L"primary_expression  :  THIS ", NULL, handle_primary_expression, 0},
{L"primary_expression  :  COLONCOLON identifier ", NULL, handle_primary_expression, 0},
{L"primary_expression  :  COLONCOLON operator_function_id ", NULL, handle_primary_expression, 0},
{L"primary_expression  :  COLONCOLON qualified_id ", NULL, handle_primary_expression, 0},
{L"primary_expression  :  ( expression ) ", NULL, handle_primary_expression, 0},
{L"primary_expression  :  id_expression ", NULL, handle_primary_expression, 0},
{L"id_expression  :  unqualified_id ", NULL, handle_id_expression, 0},
{L"id_expression  :  qualified_id ", NULL, handle_id_expression, 0},
{L"unqualified_id  :  identifier ", NULL, handle_unqualified_id, 0},
{L"unqualified_id  :  operator_function_id ", NULL, handle_unqualified_id, 0},
{L"unqualified_id  :  conversion_function_id ", NULL, handle_unqualified_id, 0},
{L"unqualified_id  :  ~ class_name ", NULL, handle_unqualified_id, 0},
{L"unqualified_id  :  template_id ", NULL, handle_unqualified_id, 0},
{L"qualified_id  :  nested_name_specifier TEMPLATE_opt unqualified_id ", NULL, handle_qualified_id, 0},
{L"nested_name_specifier  :  class_or_namespace_name COLONCOLON nested_name_specifier_opt ", NULL, handle_nested_name_specifier, 0},
{L"class_or_namespace_name  :  class_name ", NULL, handle_class_or_namespace_name, 0},
{L"class_or_namespace_name  :  namespace_name ", NULL, handle_class_or_namespace_name, 0},
{L"postfix_expression  :  primary_expression ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression [ expression ] ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression ( expression_list_opt ) ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  simple_type_specifier ( expression_list_opt ) ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression . TEMPLATE_opt COLONCOLON_opt id_expression ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression ARROW TEMPLATE_opt COLONCOLON_opt id_expression ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression . pseudo_destructor_name ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression ARROW pseudo_destructor_name ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression PLUSPLUS ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  postfix_expression MINUSMINUS ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  DYNAMIC_CAST < type_id > ( expression ) ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  STATIC_CAST < type_id > ( expression ) ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  REINTERPRET_CAST < type_id > ( expression ) ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  CONST_CAST < type_id > ( expression ) ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  TYPEID ( expression ) ", NULL, handle_postfix_expression, 0},
{L"postfix_expression  :  TYPEID ( type_id ) ", NULL, handle_postfix_expression, 0},
{L"expression_list  :  assignment_expression ", NULL, handle_expression_list, 0},
{L"expression_list  :  expression_list , assignment_expression ", NULL, handle_expression_list, 0},
{L"pseudo_destructor_name  :  COLONCOLON_opt nested_name_specifier_opt type_name COLONCOLON ~ type_name ", NULL, handle_pseudo_destructor_name, 0},
{L"pseudo_destructor_name  :  COLONCOLON_opt nested_name_specifier_opt ~ type_name ", NULL, handle_pseudo_destructor_name, 0},
{L"unary_expression  :  postfix_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  PLUSPLUS cast_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  MINUSMINUS cast_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  unary_operator cast_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  SIZEOF unary_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  SIZEOF ( type_id ) ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  new_expression ", NULL, handle_unary_expression, 0},
{L"unary_expression  :  delete_expression ", NULL, handle_unary_expression, 0},
{L"unary_operator  :  * ", NULL, handle_unary_operator, 0},
{L"unary_operator  :  & ", NULL, handle_unary_operator, 0},
{L"unary_operator  :  + ", NULL, handle_unary_operator, 0},
{L"unary_operator  :  - ", NULL, handle_unary_operator, 0},
{L"unary_operator  :  ! ", NULL, handle_unary_operator, 0},
{L"unary_operator  :  ~ ", NULL, handle_unary_operator, 0},
{L"new_expression  :  COLONCOLON_opt NEW new_placement_opt new_type_id new_initializer_opt ", NULL, handle_new_expression, 0},
{L"new_expression  :  COLONCOLON_opt NEW new_placement_opt ( type_id ) new_initializer_opt ", NULL, handle_new_expression, 0},
{L"new_placement  :  ( expression_list ) ", NULL, handle_new_placement, 0},
{L"new_type_id  :  type_specifier_seq new_declarator_opt ", NULL, handle_new_type_id, 0},
{L"new_declarator  :  ptr_operator new_declarator_opt ", NULL, handle_new_declarator, 0},
{L"new_declarator  :  direct_new_declarator ", NULL, handle_new_declarator, 0},
{L"direct_new_declarator  :  [ expression ] ", NULL, handle_direct_new_declarator, 0},
{L"direct_new_declarator  :  direct_new_declarator [ constant_expression ] ", NULL, handle_direct_new_declarator, 0},
{L"new_initializer  :  ( expression_list_opt ) ", NULL, handle_new_initializer, 0},
{L"delete_expression  :  COLONCOLON_opt DELETE cast_expression ", NULL, handle_delete_expression, 0},
{L"delete_expression  :  COLONCOLON_opt DELETE [ ] cast_expression ", NULL, handle_delete_expression, 0},
{L"cast_expression  :  unary_expression ", NULL, handle_cast_expression, 0},
{L"cast_expression  :  ( type_id ) cast_expression ", NULL, handle_cast_expression, 0},
{L"pm_expression  :  cast_expression ", NULL, handle_pm_expression, 0},
{L"pm_expression  :  pm_expression DOTSTAR cast_expression ", NULL, handle_pm_expression, 0},
{L"pm_expression  :  pm_expression ARROWSTAR cast_expression ", NULL, handle_pm_expression, 0},
{L"multiplicative_expression  :  pm_expression ", NULL, handle_multiplicative_expression, 0},
{L"multiplicative_expression  :  multiplicative_expression * pm_expression ", NULL, handle_multiplicative_expression, 0},
{L"multiplicative_expression  :  multiplicative_expression / pm_expression ", NULL, handle_multiplicative_expression, 0},
{L"multiplicative_expression  :  multiplicative_expression % pm_expression ", NULL, handle_multiplicative_expression, 0},
{L"additive_expression  :  multiplicative_expression ", NULL, handle_additive_expression, 0},
{L"additive_expression  :  additive_expression + multiplicative_expression ", NULL, handle_additive_expression, 0},
{L"additive_expression  :  additive_expression - multiplicative_expression ", NULL, handle_additive_expression, 0},
{L"shift_expression  :  additive_expression ", NULL, handle_shift_expression, 0},
{L"shift_expression  :  shift_expression SL additive_expression ", NULL, handle_shift_expression, 0},
{L"shift_expression  :  shift_expression SR additive_expression ", NULL, handle_shift_expression, 0},
{L"relational_expression  :  shift_expression ", NULL, handle_relational_expression, 0},
{L"relational_expression  :  relational_expression < shift_expression ", NULL, handle_relational_expression, 0},
{L"relational_expression  :  relational_expression > shift_expression ", NULL, handle_relational_expression, 0},
{L"relational_expression  :  relational_expression LTEQ shift_expression ", NULL, handle_relational_expression, 0},
{L"relational_expression  :  relational_expression GTEQ shift_expression ", NULL, handle_relational_expression, 0},
{L"equality_expression  :  relational_expression ", NULL, handle_equality_expression, 0},
{L"equality_expression  :  equality_expression EQ relational_expression ", NULL, handle_equality_expression, 0},
{L"equality_expression  :  equality_expression NOTEQ relational_expression ", NULL, handle_equality_expression, 0},
{L"and_expression  :  equality_expression ", NULL, handle_and_expression, 0},
{L"and_expression  :  and_expression & equality_expression ", NULL, handle_and_expression, 0},
{L"exclusive_or_expression  :  and_expression ", NULL, handle_exclusive_or_expression, 0},
{L"exclusive_or_expression  :  exclusive_or_expression ^ and_expression ", NULL, handle_exclusive_or_expression, 0},
{L"inclusive_or_expression  :  exclusive_or_expression ", NULL, handle_inclusive_or_expression, 0},
{L"inclusive_or_expression  :  inclusive_or_expression | exclusive_or_expression ", NULL, handle_inclusive_or_expression, 0},
{L"logical_and_expression  :  inclusive_or_expression ", NULL, handle_logical_and_expression, 0},
{L"logical_and_expression  :  logical_and_expression ANDAND inclusive_or_expression ", NULL, handle_logical_and_expression, 0},
{L"logical_or_expression  :  logical_and_expression ", NULL, handle_logical_or_expression, 0},
{L"logical_or_expression  :  logical_or_expression OROR logical_and_expression ", NULL, handle_logical_or_expression, 0},
{L"conditional_expression  :  logical_or_expression ", NULL, handle_conditional_expression, 0},
{L"conditional_expression  :  logical_or_expression ? expression : assignment_expression ", NULL, handle_conditional_expression, 0},
{L"assignment_expression  :  conditional_expression ", NULL, handle_assignment_expression, 0},
{L"assignment_expression  :  logical_or_expression assignment_operator assignment_expression ", NULL, handle_assignment_expression, 0},
{L"assignment_expression  :  throw_expression ", NULL, handle_assignment_expression, 0},
{L"assignment_operator  :  = ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  MULEQ ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  DIVEQ ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  MODEQ ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  ADDEQ ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  SUBEQ ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  SREQ ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  SLEQ ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  ANDEQ ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  XOREQ ", NULL, handle_assignment_operator, 0},
{L"assignment_operator  :  OREQ ", NULL, handle_assignment_operator, 0},
{L"expression  :  assignment_expression ", NULL, handle_expression, 0},
{L"expression  :  expression , assignment_expression ", NULL, handle_expression, 0},
{L"constant_expression  :  conditional_expression ", NULL, handle_constant_expression, 0},
{L"statement  :  labeled_statement ", NULL, handle_statement, 0},
{L"statement  :  expression_statement ", NULL, handle_statement, 0},
{L"statement  :  compound_statement ", NULL, handle_statement, 0},
{L"statement  :  selection_statement ", NULL, handle_statement, 0},
{L"statement  :  iteration_statement ", NULL, handle_statement, 0},
{L"statement  :  jump_statement ", NULL, handle_statement, 0},
{L"statement  :  declaration_statement ", NULL, handle_statement, 0},
{L"statement  :  try_block ", NULL, handle_statement, 0},
{L"labeled_statement  :  identifier : statement ", NULL, handle_labeled_statement, 0},
{L"labeled_statement  :  CASE constant_expression : statement ", NULL, handle_labeled_statement, 0},
{L"labeled_statement  :  DEFAULT : statement ", NULL, handle_labeled_statement, 0},
{L"expression_statement  :  expression_opt ; ", NULL, handle_expression_statement, 0},
{L"compound_statement  :  { statement_seq_opt } ", NULL, handle_compound_statement, 0},
{L"statement_seq  :  statement ", NULL, handle_statement_seq, 0},
{L"statement_seq  :  statement_seq statement ", NULL, handle_statement_seq, 0},
{L"selection_statement  :  IF ( condition ) statement ", NULL, handle_selection_statement, 0},
{L"selection_statement  :  IF ( condition ) statement ELSE statement ", NULL, handle_selection_statement, 0},
{L"selection_statement  :  SWITCH ( condition ) statement ", NULL, handle_selection_statement, 0},
{L"condition  :  expression ", NULL, handle_condition, 0},
{L"condition  :  type_specifier_seq declarator = assignment_expression ", NULL, handle_condition, 0},
{L"iteration_statement  :  WHILE ( condition ) statement ", NULL, handle_iteration_statement, 0},
{L"iteration_statement  :  DO statement WHILE ( expression ) ; ", NULL, handle_iteration_statement, 0},
{L"iteration_statement  :  FOR ( for_init_statement condition_opt ; expression_opt ) statement ", NULL, handle_iteration_statement, 0},
{L"for_init_statement  :  expression_statement ", NULL, handle_for_init_statement, 0},
{L"for_init_statement  :  simple_declaration ", NULL, handle_for_init_statement, 0},
{L"jump_statement  :  BREAK ; ", NULL, handle_jump_statement, 0},
{L"jump_statement  :  CONTINUE ; ", NULL, handle_jump_statement, 0},
{L"jump_statement  :  RETURN expression_opt ; ", NULL, handle_jump_statement, 0},
{L"jump_statement  :  GOTO identifier ; ", NULL, handle_jump_statement, 0},
{L"declaration_statement  :  block_declaration ", NULL, handle_declaration_statement, 0},
{L"declaration_seq  :  declaration ", NULL, handle_declaration_seq, 0},
{L"declaration_seq  :  declaration_seq declaration ", NULL, handle_declaration_seq, 0},
{L"declaration  :  block_declaration ", NULL, handle_declaration, 0},
{L"declaration  :  function_definition ", NULL, handle_declaration, 0},
{L"declaration  :  template_declaration ", NULL, handle_declaration, 0},
{L"declaration  :  explicit_instantiation ", NULL, handle_declaration, 0},
{L"declaration  :  explicit_specialization ", NULL, handle_declaration, 0},
{L"declaration  :  linkage_specification ", NULL, handle_declaration, 0},
{L"declaration  :  namespace_definition ", NULL, handle_declaration, 0},
{L"block_declaration  :  simple_declaration ", NULL, handle_block_declaration, 0},
{L"block_declaration  :  asm_definition ", NULL, handle_block_declaration, 0},
{L"block_declaration  :  namespace_alias_definition ", NULL, handle_block_declaration, 0},
{L"block_declaration  :  using_declaration ", NULL, handle_block_declaration, 0},
{L"block_declaration  :  using_directive ", NULL, handle_block_declaration, 0},
{L"simple_declaration  :  decl_specifier_seq_opt init_declarator_list_opt ; ", NULL, handle_simple_declaration, 0},
{L"decl_specifier  :  storage_class_specifier ", NULL, handle_decl_specifier, 0},
{L"decl_specifier  :  type_specifier ", NULL, handle_decl_specifier, 0},
{L"decl_specifier  :  function_specifier ", NULL, handle_decl_specifier, 0},
{L"decl_specifier  :  FRIEND ", NULL, handle_decl_specifier, 0},
{L"decl_specifier  :  TYPEDEF ", NULL, handle_decl_specifier, 0},
{L"decl_specifier_seq  :  decl_specifier_seq_opt decl_specifier ", NULL, handle_decl_specifier_seq, 0},
{L"storage_class_specifier  :  AUTO ", NULL, handle_storage_class_specifier, 0},
{L"storage_class_specifier  :  REGISTER ", NULL, handle_storage_class_specifier, 0},
{L"storage_class_specifier  :  STATIC ", NULL, handle_storage_class_specifier, 0},
{L"storage_class_specifier  :  EXTERN ", NULL, handle_storage_class_specifier, 0},
{L"storage_class_specifier  :  MUTABLE ", NULL, handle_storage_class_specifier, 0},
{L"function_specifier  :  INLINE ", NULL, handle_function_specifier, 0},
{L"function_specifier  :  VIRTUAL ", NULL, handle_function_specifier, 0},
{L"function_specifier  :  EXPLICIT ", NULL, handle_function_specifier, 0},
{L"type_specifier  :  simple_type_specifier ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  class_specifier ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  enum_specifier ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  elaborated_type_specifier ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  cv_qualifier ", NULL, handle_type_specifier, 0},
{L"simple_type_specifier  :  COLONCOLON_opt nested_name_specifier_opt type_name ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  CHAR ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  WCHAR_T ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  BOOL ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  SHORT ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  INT ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  LONG ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  SIGNED ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  UNSIGNED ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  FLOAT ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  DOUBLE ", NULL, handle_simple_type_specifier, 0},
{L"simple_type_specifier  :  VOID ", NULL, handle_simple_type_specifier, 0},
{L"type_name  :  class_name ", NULL, handle_type_name, 0},
{L"type_name  :  enum_name ", NULL, handle_type_name, 0},
{L"type_name  :  typedef_name ", NULL, handle_type_name, 0},
{L"elaborated_type_specifier  :  class_key COLONCOLON_opt nested_name_specifier_opt identifier ", NULL, handle_elaborated_type_specifier, 0},
{L"elaborated_type_specifier  :  ENUM COLONCOLON_opt nested_name_specifier_opt identifier ", NULL, handle_elaborated_type_specifier, 0},
{L"elaborated_type_specifier  :  TYPENAME COLONCOLON_opt nested_name_specifier identifier ", NULL, handle_elaborated_type_specifier, 0},
{L"elaborated_type_specifier  :  TYPENAME COLONCOLON_opt nested_name_specifier identifier < template_argument_list > ", NULL, handle_elaborated_type_specifier, 0},
{L"enum_specifier  :  ENUM identifier_opt { enumerator_list_opt } ", NULL, handle_enum_specifier, 0},
{L"enumerator_list  :  enumerator_definition ", NULL, handle_enumerator_list, 0},
{L"enumerator_list  :  enumerator_list , enumerator_definition ", NULL, handle_enumerator_list, 0},
{L"enumerator_definition  :  enumerator ", NULL, handle_enumerator_definition, 0},
{L"enumerator_definition  :  enumerator = constant_expression ", NULL, handle_enumerator_definition, 0},
{L"enumerator  :  identifier ", NULL, handle_enumerator, 0},
{L"namespace_definition  :  named_namespace_definition ", NULL, handle_namespace_definition, 0},
{L"namespace_definition  :  unnamed_namespace_definition ", NULL, handle_namespace_definition, 0},
{L"named_namespace_definition  :  original_namespace_definition ", NULL, handle_named_namespace_definition, 0},
{L"named_namespace_definition  :  extension_namespace_definition ", NULL, handle_named_namespace_definition, 0},
{L"original_namespace_definition  :  NAMESPACE identifier { namespace_body } ", NULL, handle_original_namespace_definition, 0},
{L"extension_namespace_definition  :  NAMESPACE original_namespace_name { namespace_body } ", NULL, handle_extension_namespace_definition, 0},
{L"unnamed_namespace_definition  :  NAMESPACE { namespace_body } ", NULL, handle_unnamed_namespace_definition, 0},
{L"namespace_body  :  declaration_seq_opt ", NULL, handle_namespace_body, 0},
{L"namespace_alias_definition  :  NAMESPACE identifier = qualified_namespace_specifier ; ", NULL, handle_namespace_alias_definition, 0},
{L"qualified_namespace_specifier  :  COLONCOLON_opt nested_name_specifier_opt namespace_name ", NULL, handle_qualified_namespace_specifier, 0},
{L"using_declaration  :  USING TYPENAME_opt COLONCOLON_opt nested_name_specifier unqualified_id ; ", NULL, handle_using_declaration, 0},
{L"using_declaration  :  USING COLONCOLON unqualified_id ; ", NULL, handle_using_declaration, 0},
{L"using_directive  :  USING NAMESPACE COLONCOLON_opt nested_name_specifier_opt namespace_name ; ", NULL, handle_using_directive, 0},
{L"asm_definition  :  ASM ( string_literal ) ; ", NULL, handle_asm_definition, 0},
{L"linkage_specification  :  EXTERN string_literal { declaration_seq_opt } ", NULL, handle_linkage_specification, 0},
{L"linkage_specification  :  EXTERN string_literal declaration ", NULL, handle_linkage_specification, 0},
{L"init_declarator_list  :  init_declarator ", NULL, handle_init_declarator_list, 0},
{L"init_declarator_list  :  init_declarator_list , init_declarator ", NULL, handle_init_declarator_list, 0},
{L"init_declarator  :  declarator initializer_opt ", NULL, handle_init_declarator, 0},
{L"declarator  :  direct_declarator ", NULL, handle_declarator, 0},
{L"declarator  :  ptr_operator declarator ", NULL, handle_declarator, 0},
{L"direct_declarator  :  declarator_id ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  direct_declarator ( parameter_declaration_clause ) cv_qualifier_seq_opt exception_specification_opt ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  direct_declarator [ constant_expression_opt ] ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  ( declarator ) ", NULL, handle_direct_declarator, 0},
{L"ptr_operator  :  * cv_qualifier_seq_opt ", NULL, handle_ptr_operator, 0},
{L"ptr_operator  :  & ", NULL, handle_ptr_operator, 0},
{L"ptr_operator  :  COLONCOLON_opt nested_name_specifier * cv_qualifier_seq_opt ", NULL, handle_ptr_operator, 0},
{L"cv_qualifier_seq  :  cv_qualifier cv_qualifier_seq_opt ", NULL, handle_cv_qualifier_seq, 0},
{L"cv_qualifier  :  CONST ", NULL, handle_cv_qualifier, 0},
{L"cv_qualifier  :  VOLATILE ", NULL, handle_cv_qualifier, 0},
{L"declarator_id  :  COLONCOLON_opt id_expression ", NULL, handle_declarator_id, 0},
{L"declarator_id  :  COLONCOLON_opt nested_name_specifier_opt type_name ", NULL, handle_declarator_id, 0},
{L"type_id  :  type_specifier_seq abstract_declarator_opt ", NULL, handle_type_id, 0},
{L"type_specifier_seq  :  type_specifier type_specifier_seq_opt ", NULL, handle_type_specifier_seq, 0},
{L"abstract_declarator  :  ptr_operator abstract_declarator_opt ", NULL, handle_abstract_declarator, 0},
{L"abstract_declarator  :  direct_abstract_declarator ", NULL, handle_abstract_declarator, 0},
{L"direct_abstract_declarator  :  direct_abstract_declarator_opt ( parameter_declaration_clause ) cv_qualifier_seq_opt exception_specification_opt ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  direct_abstract_declarator_opt [ constant_expression_opt ] ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  ( abstract_declarator ) ", NULL, handle_direct_abstract_declarator, 0},
{L"parameter_declaration_clause  :  parameter_declaration_list_opt ELLIPSIS_opt ", NULL, handle_parameter_declaration_clause, 0},
{L"parameter_declaration_clause  :  parameter_declaration_list , ELLIPSIS ", NULL, handle_parameter_declaration_clause, 0},
{L"parameter_declaration_list  :  parameter_declaration ", NULL, handle_parameter_declaration_list, 0},
{L"parameter_declaration_list  :  parameter_declaration_list , parameter_declaration ", NULL, handle_parameter_declaration_list, 0},
{L"parameter_declaration  :  decl_specifier_seq declarator ", NULL, handle_parameter_declaration, 0},
{L"parameter_declaration  :  decl_specifier_seq declarator = assignment_expression ", NULL, handle_parameter_declaration, 0},
{L"parameter_declaration  :  decl_specifier_seq abstract_declarator_opt ", NULL, handle_parameter_declaration, 0},
{L"parameter_declaration  :  decl_specifier_seq abstract_declarator_opt = assignment_expression ", NULL, handle_parameter_declaration, 0},
{L"function_definition  :  decl_specifier_seq_opt declarator ctor_initializer_opt function_body ", NULL, handle_function_definition, 0},
{L"function_definition  :  decl_specifier_seq_opt declarator function_try_block ", NULL, handle_function_definition, 0},
{L"function_body  :  compound_statement ", NULL, handle_function_body, 0},
{L"initializer  :  = initializer_clause ", NULL, handle_initializer, 0},
{L"initializer  :  ( expression_list ) ", NULL, handle_initializer, 0},
{L"initializer_clause  :  assignment_expression ", NULL, handle_initializer_clause, 0},
{L"initializer_clause  :  { initializer_list COMMA_opt } ", NULL, handle_initializer_clause, 0},
{L"initializer_clause  :  { } ", NULL, handle_initializer_clause, 0},
{L"initializer_list  :  initializer_clause ", NULL, handle_initializer_list, 0},
{L"initializer_list  :  initializer_list , initializer_clause ", NULL, handle_initializer_list, 0},
{L"class_specifier  :  class_head { member_specification_opt } ", NULL, handle_class_specifier, 0},
{L"class_head  :  class_key identifier_opt base_clause_opt ", NULL, handle_class_head, 0},
{L"class_head  :  class_key nested_name_specifier identifier base_clause_opt ", NULL, handle_class_head, 0},
{L"class_key  :  CLASS ", NULL, handle_class_key, 0},
{L"class_key  :  STRUCT ", NULL, handle_class_key, 0},
{L"class_key  :  UNION ", NULL, handle_class_key, 0},
{L"member_specification  :  member_declaration member_specification_opt ", NULL, handle_member_specification, 0},
{L"member_specification  :  access_specifier : member_specification_opt ", NULL, handle_member_specification, 0},
{L"member_declaration  :  decl_specifier_seq_opt member_declarator_list_opt ; ", NULL, handle_member_declaration, 0},
{L"member_declaration  :  function_definition SEMICOLON_opt ", NULL, handle_member_declaration, 0},
{L"member_declaration  :  qualified_id ; ", NULL, handle_member_declaration, 0},
{L"member_declaration  :  using_declaration ", NULL, handle_member_declaration, 0},
{L"member_declaration  :  template_declaration ", NULL, handle_member_declaration, 0},
{L"member_declarator_list  :  member_declarator ", NULL, handle_member_declarator_list, 0},
{L"member_declarator_list  :  member_declarator_list , member_declarator ", NULL, handle_member_declarator_list, 0},
{L"member_declarator  :  declarator pure_specifier_opt ", NULL, handle_member_declarator, 0},
{L"member_declarator  :  declarator constant_initializer_opt ", NULL, handle_member_declarator, 0},
{L"member_declarator  :  identifier_opt : constant_expression ", NULL, handle_member_declarator, 0},
{L"pure_specifier  :  = integer_literal ", NULL, handle_pure_specifier, 0},
{L"constant_initializer  :  = constant_expression ", NULL, handle_constant_initializer, 0},
{L"base_clause  :  : base_specifier_list ", NULL, handle_base_clause, 0},
{L"base_specifier_list  :  base_specifier ", NULL, handle_base_specifier_list, 0},
{L"base_specifier_list  :  base_specifier_list , base_specifier ", NULL, handle_base_specifier_list, 0},
{L"base_specifier  :  COLONCOLON_opt nested_name_specifier_opt class_name ", NULL, handle_base_specifier, 0},
{L"base_specifier  :  VIRTUAL access_specifier_opt COLONCOLON_opt nested_name_specifier_opt class_name ", NULL, handle_base_specifier, 0},
{L"base_specifier  :  access_specifier VIRTUAL_opt COLONCOLON_opt nested_name_specifier_opt class_name ", NULL, handle_base_specifier, 0},
{L"access_specifier  :  PRIVATE ", NULL, handle_access_specifier, 0},
{L"access_specifier  :  PROTECTED ", NULL, handle_access_specifier, 0},
{L"access_specifier  :  PUBLIC ", NULL, handle_access_specifier, 0},
{L"conversion_function_id  :  OPERATOR conversion_type_id ", NULL, handle_conversion_function_id, 0},
{L"conversion_type_id  :  type_specifier_seq conversion_declarator_opt ", NULL, handle_conversion_type_id, 0},
{L"conversion_declarator  :  ptr_operator conversion_declarator_opt ", NULL, handle_conversion_declarator, 0},
{L"ctor_initializer  :  : mem_initializer_list ", NULL, handle_ctor_initializer, 0},
{L"mem_initializer_list  :  mem_initializer ", NULL, handle_mem_initializer_list, 0},
{L"mem_initializer_list  :  mem_initializer , mem_initializer_list ", NULL, handle_mem_initializer_list, 0},
{L"mem_initializer  :  mem_initializer_id ( expression_list_opt ) ", NULL, handle_mem_initializer, 0},
{L"mem_initializer_id  :  COLONCOLON_opt nested_name_specifier_opt class_name ", NULL, handle_mem_initializer_id, 0},
{L"mem_initializer_id  :  identifier ", NULL, handle_mem_initializer_id, 0},
{L"operator_function_id  :  OPERATOR operator ", NULL, handle_operator_function_id, 0},
{L"operator  :  NEW ", NULL, handle_operator, 0},
{L"operator  :  DELETE ", NULL, handle_operator, 0},
{L"operator  :  NEW [ ] ", NULL, handle_operator, 0},
{L"operator  :  DELETE [ ] ", NULL, handle_operator, 0},
{L"operator  :  + ", NULL, handle_operator, 0},
{L"operator  :  * ", NULL, handle_operator, 0},
{L"operator  :  / ", NULL, handle_operator, 0},
{L"operator  :  % ", NULL, handle_operator, 0},
{L"operator  :  ^ ", NULL, handle_operator, 0},
{L"operator  :  & ", NULL, handle_operator, 0},
{L"operator  :  | ", NULL, handle_operator, 0},
{L"operator  :  ~ ", NULL, handle_operator, 0},
{L"operator  :  ! ", NULL, handle_operator, 0},
{L"operator  :  = ", NULL, handle_operator, 0},
{L"operator  :  < ", NULL, handle_operator, 0},
{L"operator  :  > ", NULL, handle_operator, 0},
{L"operator  :  ADDEQ ", NULL, handle_operator, 0},
{L"operator  :  SUBEQ ", NULL, handle_operator, 0},
{L"operator  :  MULEQ ", NULL, handle_operator, 0},
{L"operator  :  DIVEQ ", NULL, handle_operator, 0},
{L"operator  :  MODEQ ", NULL, handle_operator, 0},
{L"operator  :  XOREQ ", NULL, handle_operator, 0},
{L"operator  :  ANDEQ ", NULL, handle_operator, 0},
{L"operator  :  OREQ ", NULL, handle_operator, 0},
{L"operator  :  SL ", NULL, handle_operator, 0},
{L"operator  :  SR ", NULL, handle_operator, 0},
{L"operator  :  SREQ ", NULL, handle_operator, 0},
{L"operator  :  SLEQ ", NULL, handle_operator, 0},
{L"operator  :  EQ ", NULL, handle_operator, 0},
{L"operator  :  NOTEQ ", NULL, handle_operator, 0},
{L"operator  :  LTEQ ", NULL, handle_operator, 0},
{L"operator  :  GTEQ ", NULL, handle_operator, 0},
{L"operator  :  ANDAND ", NULL, handle_operator, 0},
{L"operator  :  OROR ", NULL, handle_operator, 0},
{L"operator  :  PLUSPLUS ", NULL, handle_operator, 0},
{L"operator  :  MINUSMINUS ", NULL, handle_operator, 0},
{L"operator  :  , ", NULL, handle_operator, 0},
{L"operator  :  ARROWSTAR ", NULL, handle_operator, 0},
{L"operator  :  ARROW ", NULL, handle_operator, 0},
{L"operator  :  ( ) ", NULL, handle_operator, 0},
{L"operator  :  [ ] ", NULL, handle_operator, 0},
{L"template_declaration  :  EXPORT_opt TEMPLATE < template_parameter_list > declaration ", NULL, handle_template_declaration, 0},
{L"template_parameter_list  :  template_parameter ", NULL, handle_template_parameter_list, 0},
{L"template_parameter_list  :  template_parameter_list , template_parameter ", NULL, handle_template_parameter_list, 0},
{L"template_parameter  :  type_parameter ", NULL, handle_template_parameter, 0},
{L"template_parameter  :  parameter_declaration ", NULL, handle_template_parameter, 0},
{L"type_parameter  :  CLASS identifier_opt ", NULL, handle_type_parameter, 0},
{L"type_parameter  :  CLASS identifier_opt = type_id ", NULL, handle_type_parameter, 0},
{L"type_parameter  :  TYPENAME identifier_opt ", NULL, handle_type_parameter, 0},
{L"type_parameter  :  TYPENAME identifier_opt = type_id ", NULL, handle_type_parameter, 0},
{L"type_parameter  :  TEMPLATE < template_parameter_list > CLASS identifier_opt ", NULL, handle_type_parameter, 0},
{L"type_parameter  :  TEMPLATE < template_parameter_list > CLASS identifier_opt = template_name ", NULL, handle_type_parameter, 0},
{L"template_id  :  template_name < template_argument_list > ", NULL, handle_template_id, 0},
{L"template_argument_list  :  template_argument ", NULL, handle_template_argument_list, 0},
{L"template_argument_list  :  template_argument_list , template_argument ", NULL, handle_template_argument_list, 0},
{L"template_argument  :  assignment_expression ", NULL, handle_template_argument, 0},
{L"template_argument  :  type_id ", NULL, handle_template_argument, 0},
{L"template_argument  :  template_name ", NULL, handle_template_argument, 0},
{L"explicit_instantiation  :  TEMPLATE declaration ", NULL, handle_explicit_instantiation, 0},
{L"explicit_specialization  :  TEMPLATE < > declaration ", NULL, handle_explicit_specialization, 0},
{L"try_block  :  TRY compound_statement handler_seq ", NULL, handle_try_block, 0},
{L"function_try_block  :  TRY ctor_initializer_opt function_body handler_seq ", NULL, handle_function_try_block, 0},
{L"handler_seq  :  handler handler_seq_opt ", NULL, handle_handler_seq, 0},
{L"handler  :  CATCH ( exception_declaration ) compound_statement ", NULL, handle_handler, 0},
{L"exception_declaration  :  type_specifier_seq declarator ", NULL, handle_exception_declaration, 0},
{L"exception_declaration  :  type_specifier_seq abstract_declarator ", NULL, handle_exception_declaration, 0},
{L"exception_declaration  :  type_specifier_seq ", NULL, handle_exception_declaration, 0},
{L"exception_declaration  :  ELLIPSIS ", NULL, handle_exception_declaration, 0},
{L"throw_expression  :  THROW assignment_expression_opt ", NULL, handle_throw_expression, 0},
{L"exception_specification  :  THROW ( type_id_list_opt ) ", NULL, handle_exception_specification, 0},
{L"type_id_list  :  type_id ", NULL, handle_type_id_list, 0},
{L"type_id_list  :  type_id_list , type_id ", NULL, handle_type_id_list, 0},
{L"declaration_seq_opt  :  declaration_seq ", NULL, handle_declaration_seq_opt, 0},
{L"TEMPLATE_opt  :  TEMPLATE ", NULL, handle_TEMPLATE_opt, 0},
{L"nested_name_specifier_opt  :  nested_name_specifier ", NULL, handle_nested_name_specifier_opt, 0},
{L"expression_list_opt  :  expression_list ", NULL, handle_expression_list_opt, 0},
{L"COLONCOLON_opt  :  COLONCOLON ", NULL, handle_COLONCOLON_opt, 0},
{L"new_placement_opt  :  new_placement ", NULL, handle_new_placement_opt, 0},
{L"new_initializer_opt  :  new_initializer ", NULL, handle_new_initializer_opt, 0},
{L"new_declarator_opt  :  new_declarator ", NULL, handle_new_declarator_opt, 0},
{L"expression_opt  :  expression ", NULL, handle_expression_opt, 0},
{L"statement_seq_opt  :  statement_seq ", NULL, handle_statement_seq_opt, 0},
{L"condition_opt  :  condition ", NULL, handle_condition_opt, 0},
{L"decl_specifier_seq_opt  :  decl_specifier_seq ", NULL, handle_decl_specifier_seq_opt, 0},
{L"init_declarator_list_opt  :  init_declarator_list ", NULL, handle_init_declarator_list_opt, 0},
{L"identifier_opt  :  identifier ", NULL, handle_identifier_opt, 0},
{L"enumerator_list_opt  :  enumerator_list ", NULL, handle_enumerator_list_opt, 0},
{L"TYPENAME_opt  :  TYPENAME ", NULL, handle_TYPENAME_opt, 0},
{L"initializer_opt  :  initializer ", NULL, handle_initializer_opt, 0},
{L"cv_qualifier_seq_opt  :  cv_qualifier_seq ", NULL, handle_cv_qualifier_seq_opt, 0},
{L"exception_specification_opt  :  exception_specification ", NULL, handle_exception_specification_opt, 0},
{L"constant_expression_opt  :  constant_expression ", NULL, handle_constant_expression_opt, 0},
{L"abstract_declarator_opt  :  abstract_declarator ", NULL, handle_abstract_declarator_opt, 0},
{L"type_specifier_seq_opt  :  type_specifier_seq ", NULL, handle_type_specifier_seq_opt, 0},
{L"direct_abstract_declarator_opt  :  direct_abstract_declarator ", NULL, handle_direct_abstract_declarator_opt, 0},
{L"parameter_declaration_list_opt  :  parameter_declaration_list ", NULL, handle_parameter_declaration_list_opt, 0},
{L"ELLIPSIS_opt  :  ELLIPSIS ", NULL, handle_ELLIPSIS_opt, 0},
{L"ctor_initializer_opt  :  ctor_initializer ", NULL, handle_ctor_initializer_opt, 0},
{L"COMMA_opt  :  , ", NULL, handle_COMMA_opt, 0},
{L"member_specification_opt  :  member_specification ", NULL, handle_member_specification_opt, 0},
{L"base_clause_opt  :  base_clause ", NULL, handle_base_clause_opt, 0},
{L"member_declarator_list_opt  :  member_declarator_list ", NULL, handle_member_declarator_list_opt, 0},
{L"SEMICOLON_opt  :  ; ", NULL, handle_SEMICOLON_opt, 0},
{L"pure_specifier_opt  :  pure_specifier ", NULL, handle_pure_specifier_opt, 0},
{L"constant_initializer_opt  :  constant_initializer ", NULL, handle_constant_initializer_opt, 0},
{L"access_specifier_opt  :  access_specifier ", NULL, handle_access_specifier_opt, 0},
{L"VIRTUAL_opt  :  VIRTUAL ", NULL, handle_VIRTUAL_opt, 0},
{L"conversion_declarator_opt  :  conversion_declarator ", NULL, handle_conversion_declarator_opt, 0},
{L"EXPORT_opt  :  EXPORT ", NULL, handle_EXPORT_opt, 0},
{L"handler_seq_opt  :  handler_seq ", NULL, handle_handler_seq_opt, 0},
{L"assignment_expression_opt  :  assignment_expression ", NULL, handle_assignment_expression_opt, 0},
{L"type_id_list_opt  :  type_id_list ", NULL, handle_type_id_list_opt, 0}
};

#define __RULE_COUNT__ ((size_t)437)
#define START_RULE L"translation_unit"

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




/*typedef_name	:	TYPEDEF_NAME */
static psrNode_t* AR_STDCALL handle_typedef_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*namespace_name	:	original_namespace_name */
/*namespace_name	:	namespace_alias */
static psrNode_t* AR_STDCALL handle_namespace_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*original_namespace_name	:	NAMESPACE_NAME */
static psrNode_t* AR_STDCALL handle_original_namespace_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*namespace_alias	:	NAMESPACE_NAME */
static psrNode_t* AR_STDCALL handle_namespace_alias(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*class_name	:	CLASS_NAME */
/*class_name	:	template_id */
static psrNode_t* AR_STDCALL handle_class_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*enum_name	:	ENUM_NAME */
static psrNode_t* AR_STDCALL handle_enum_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*template_name	:	TEMPLATE_NAME */
static psrNode_t* AR_STDCALL handle_template_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*identifier	:	IDENTIFIER */
static psrNode_t* AR_STDCALL handle_identifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*literal	:	integer_literal */
/*literal	:	character_literal */
/*literal	:	floating_literal */
/*literal	:	string_literal */
/*literal	:	boolean_literal */
static psrNode_t* AR_STDCALL handle_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*integer_literal	:	OCT_CONSTANT */
/*integer_literal	:	HEX_CONSTANT */
/*integer_literal	:	DEC_CONSTANT */
static psrNode_t* AR_STDCALL handle_integer_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*character_literal	:	CHAR_CONSTANT */
static psrNode_t* AR_STDCALL handle_character_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*floating_literal	:	FLOAT_CONSTANT */
static psrNode_t* AR_STDCALL handle_floating_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*string_literal	:	STRING_LITERAL */
static psrNode_t* AR_STDCALL handle_string_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*boolean_literal	:	TRUE */
/*boolean_literal	:	FALSE */
static psrNode_t* AR_STDCALL handle_boolean_literal(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*translation_unit	:	declaration_seq_opt */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*primary_expression	:	literal */
/*primary_expression	:	THIS */
/*primary_expression	:	COLONCOLON identifier */
/*primary_expression	:	COLONCOLON operator_function_id */
/*primary_expression	:	COLONCOLON qualified_id */
/*primary_expression	:	( expression ) */
/*primary_expression	:	id_expression */
static psrNode_t* AR_STDCALL handle_primary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*id_expression	:	unqualified_id */
/*id_expression	:	qualified_id */
static psrNode_t* AR_STDCALL handle_id_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*unqualified_id	:	identifier */
/*unqualified_id	:	operator_function_id */
/*unqualified_id	:	conversion_function_id */
/*unqualified_id	:	~ class_name */
/*unqualified_id	:	template_id */
static psrNode_t* AR_STDCALL handle_unqualified_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*qualified_id	:	nested_name_specifier TEMPLATE_opt unqualified_id */
static psrNode_t* AR_STDCALL handle_qualified_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*nested_name_specifier	:	class_or_namespace_name COLONCOLON nested_name_specifier_opt */
static psrNode_t* AR_STDCALL handle_nested_name_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*class_or_namespace_name	:	class_name */
/*class_or_namespace_name	:	namespace_name */
static psrNode_t* AR_STDCALL handle_class_or_namespace_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*postfix_expression	:	primary_expression */
/*postfix_expression	:	postfix_expression [ expression ] */
/*postfix_expression	:	postfix_expression ( expression_list_opt ) */
/*postfix_expression	:	simple_type_specifier ( expression_list_opt ) */
/*postfix_expression	:	postfix_expression . TEMPLATE_opt COLONCOLON_opt id_expression */
/*postfix_expression	:	postfix_expression ARROW TEMPLATE_opt COLONCOLON_opt id_expression */
/*postfix_expression	:	postfix_expression . pseudo_destructor_name */
/*postfix_expression	:	postfix_expression ARROW pseudo_destructor_name */
/*postfix_expression	:	postfix_expression PLUSPLUS */
/*postfix_expression	:	postfix_expression MINUSMINUS */
/*postfix_expression	:	DYNAMIC_CAST < type_id > ( expression ) */
/*postfix_expression	:	STATIC_CAST < type_id > ( expression ) */
/*postfix_expression	:	REINTERPRET_CAST < type_id > ( expression ) */
/*postfix_expression	:	CONST_CAST < type_id > ( expression ) */
/*postfix_expression	:	TYPEID ( expression ) */
/*postfix_expression	:	TYPEID ( type_id ) */
static psrNode_t* AR_STDCALL handle_postfix_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*expression_list	:	assignment_expression */
/*expression_list	:	expression_list , assignment_expression */
static psrNode_t* AR_STDCALL handle_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*pseudo_destructor_name	:	COLONCOLON_opt nested_name_specifier_opt type_name COLONCOLON ~ type_name */
/*pseudo_destructor_name	:	COLONCOLON_opt nested_name_specifier_opt ~ type_name */
static psrNode_t* AR_STDCALL handle_pseudo_destructor_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*unary_expression	:	postfix_expression */
/*unary_expression	:	PLUSPLUS cast_expression */
/*unary_expression	:	MINUSMINUS cast_expression */
/*unary_expression	:	unary_operator cast_expression */
/*unary_expression	:	SIZEOF unary_expression */
/*unary_expression	:	SIZEOF ( type_id ) */
/*unary_expression	:	new_expression */
/*unary_expression	:	delete_expression */
static psrNode_t* AR_STDCALL handle_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*unary_operator	:	* */
/*unary_operator	:	& */
/*unary_operator	:	+ */
/*unary_operator	:	- */
/*unary_operator	:	! */
/*unary_operator	:	~ */
static psrNode_t* AR_STDCALL handle_unary_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*new_expression	:	COLONCOLON_opt NEW new_placement_opt new_type_id new_initializer_opt */
/*new_expression	:	COLONCOLON_opt NEW new_placement_opt ( type_id ) new_initializer_opt */
static psrNode_t* AR_STDCALL handle_new_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*new_placement	:	( expression_list ) */
static psrNode_t* AR_STDCALL handle_new_placement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*new_type_id	:	type_specifier_seq new_declarator_opt */
static psrNode_t* AR_STDCALL handle_new_type_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*new_declarator	:	ptr_operator new_declarator_opt */
/*new_declarator	:	direct_new_declarator */
static psrNode_t* AR_STDCALL handle_new_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*direct_new_declarator	:	[ expression ] */
/*direct_new_declarator	:	direct_new_declarator [ constant_expression ] */
static psrNode_t* AR_STDCALL handle_direct_new_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*new_initializer	:	( expression_list_opt ) */
static psrNode_t* AR_STDCALL handle_new_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*delete_expression	:	COLONCOLON_opt DELETE cast_expression */
/*delete_expression	:	COLONCOLON_opt DELETE [ ] cast_expression */
static psrNode_t* AR_STDCALL handle_delete_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*cast_expression	:	unary_expression */
/*cast_expression	:	( type_id ) cast_expression */
static psrNode_t* AR_STDCALL handle_cast_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*pm_expression	:	cast_expression */
/*pm_expression	:	pm_expression DOTSTAR cast_expression */
/*pm_expression	:	pm_expression ARROWSTAR cast_expression */
static psrNode_t* AR_STDCALL handle_pm_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*multiplicative_expression	:	pm_expression */
/*multiplicative_expression	:	multiplicative_expression * pm_expression */
/*multiplicative_expression	:	multiplicative_expression / pm_expression */
/*multiplicative_expression	:	multiplicative_expression % pm_expression */
static psrNode_t* AR_STDCALL handle_multiplicative_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*additive_expression	:	multiplicative_expression */
/*additive_expression	:	additive_expression + multiplicative_expression */
/*additive_expression	:	additive_expression - multiplicative_expression */
static psrNode_t* AR_STDCALL handle_additive_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*shift_expression	:	additive_expression */
/*shift_expression	:	shift_expression SL additive_expression */
/*shift_expression	:	shift_expression SR additive_expression */
static psrNode_t* AR_STDCALL handle_shift_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*relational_expression	:	shift_expression */
/*relational_expression	:	relational_expression < shift_expression */
/*relational_expression	:	relational_expression > shift_expression */
/*relational_expression	:	relational_expression LTEQ shift_expression */
/*relational_expression	:	relational_expression GTEQ shift_expression */
static psrNode_t* AR_STDCALL handle_relational_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*equality_expression	:	relational_expression */
/*equality_expression	:	equality_expression EQ relational_expression */
/*equality_expression	:	equality_expression NOTEQ relational_expression */
static psrNode_t* AR_STDCALL handle_equality_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*and_expression	:	equality_expression */
/*and_expression	:	and_expression & equality_expression */
static psrNode_t* AR_STDCALL handle_and_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*exclusive_or_expression	:	and_expression */
/*exclusive_or_expression	:	exclusive_or_expression ^ and_expression */
static psrNode_t* AR_STDCALL handle_exclusive_or_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*inclusive_or_expression	:	exclusive_or_expression */
/*inclusive_or_expression	:	inclusive_or_expression | exclusive_or_expression */
static psrNode_t* AR_STDCALL handle_inclusive_or_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*logical_and_expression	:	inclusive_or_expression */
/*logical_and_expression	:	logical_and_expression ANDAND inclusive_or_expression */
static psrNode_t* AR_STDCALL handle_logical_and_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*logical_or_expression	:	logical_and_expression */
/*logical_or_expression	:	logical_or_expression OROR logical_and_expression */
static psrNode_t* AR_STDCALL handle_logical_or_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*conditional_expression	:	logical_or_expression */
/*conditional_expression	:	logical_or_expression ? expression : assignment_expression */
static psrNode_t* AR_STDCALL handle_conditional_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*assignment_expression	:	conditional_expression */
/*assignment_expression	:	logical_or_expression assignment_operator assignment_expression */
/*assignment_expression	:	throw_expression */
static psrNode_t* AR_STDCALL handle_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*assignment_operator	:	= */
/*assignment_operator	:	MULEQ */
/*assignment_operator	:	DIVEQ */
/*assignment_operator	:	MODEQ */
/*assignment_operator	:	ADDEQ */
/*assignment_operator	:	SUBEQ */
/*assignment_operator	:	SREQ */
/*assignment_operator	:	SLEQ */
/*assignment_operator	:	ANDEQ */
/*assignment_operator	:	XOREQ */
/*assignment_operator	:	OREQ */
static psrNode_t* AR_STDCALL handle_assignment_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*expression	:	assignment_expression */
/*expression	:	expression , assignment_expression */
static psrNode_t* AR_STDCALL handle_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*constant_expression	:	conditional_expression */
static psrNode_t* AR_STDCALL handle_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*statement	:	labeled_statement */
/*statement	:	expression_statement */
/*statement	:	compound_statement */
/*statement	:	selection_statement */
/*statement	:	iteration_statement */
/*statement	:	jump_statement */
/*statement	:	declaration_statement */
/*statement	:	try_block */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*labeled_statement	:	identifier : statement */
/*labeled_statement	:	CASE constant_expression : statement */
/*labeled_statement	:	DEFAULT : statement */
static psrNode_t* AR_STDCALL handle_labeled_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*expression_statement	:	expression_opt ; */
static psrNode_t* AR_STDCALL handle_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*compound_statement	:	{ statement_seq_opt } */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*statement_seq	:	statement */
/*statement_seq	:	statement_seq statement */
static psrNode_t* AR_STDCALL handle_statement_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*selection_statement	:	IF ( condition ) statement */
/*selection_statement	:	IF ( condition ) statement ELSE statement */
/*selection_statement	:	SWITCH ( condition ) statement */
static psrNode_t* AR_STDCALL handle_selection_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*condition	:	expression */
/*condition	:	type_specifier_seq declarator = assignment_expression */
static psrNode_t* AR_STDCALL handle_condition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*iteration_statement	:	WHILE ( condition ) statement */
/*iteration_statement	:	DO statement WHILE ( expression ) ; */
/*iteration_statement	:	FOR ( for_init_statement condition_opt ; expression_opt ) statement */
static psrNode_t* AR_STDCALL handle_iteration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*for_init_statement	:	expression_statement */
/*for_init_statement	:	simple_declaration */
static psrNode_t* AR_STDCALL handle_for_init_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*jump_statement	:	BREAK ; */
/*jump_statement	:	CONTINUE ; */
/*jump_statement	:	RETURN expression_opt ; */
/*jump_statement	:	GOTO identifier ; */
static psrNode_t* AR_STDCALL handle_jump_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*declaration_statement	:	block_declaration */
static psrNode_t* AR_STDCALL handle_declaration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*declaration_seq	:	declaration */
/*declaration_seq	:	declaration_seq declaration */
static psrNode_t* AR_STDCALL handle_declaration_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*declaration	:	block_declaration */
/*declaration	:	function_definition */
/*declaration	:	template_declaration */
/*declaration	:	explicit_instantiation */
/*declaration	:	explicit_specialization */
/*declaration	:	linkage_specification */
/*declaration	:	namespace_definition */
static psrNode_t* AR_STDCALL handle_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*block_declaration	:	simple_declaration */
/*block_declaration	:	asm_definition */
/*block_declaration	:	namespace_alias_definition */
/*block_declaration	:	using_declaration */
/*block_declaration	:	using_directive */
static psrNode_t* AR_STDCALL handle_block_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*simple_declaration	:	decl_specifier_seq_opt init_declarator_list_opt ; */
static psrNode_t* AR_STDCALL handle_simple_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*decl_specifier	:	storage_class_specifier */
/*decl_specifier	:	type_specifier */
/*decl_specifier	:	function_specifier */
/*decl_specifier	:	FRIEND */
/*decl_specifier	:	TYPEDEF */
static psrNode_t* AR_STDCALL handle_decl_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*decl_specifier_seq	:	decl_specifier_seq_opt decl_specifier */
static psrNode_t* AR_STDCALL handle_decl_specifier_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*storage_class_specifier	:	AUTO */
/*storage_class_specifier	:	REGISTER */
/*storage_class_specifier	:	STATIC */
/*storage_class_specifier	:	EXTERN */
/*storage_class_specifier	:	MUTABLE */
static psrNode_t* AR_STDCALL handle_storage_class_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*function_specifier	:	INLINE */
/*function_specifier	:	VIRTUAL */
/*function_specifier	:	EXPLICIT */
static psrNode_t* AR_STDCALL handle_function_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*type_specifier	:	simple_type_specifier */
/*type_specifier	:	class_specifier */
/*type_specifier	:	enum_specifier */
/*type_specifier	:	elaborated_type_specifier */
/*type_specifier	:	cv_qualifier */
static psrNode_t* AR_STDCALL handle_type_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*simple_type_specifier	:	COLONCOLON_opt nested_name_specifier_opt type_name */
/*simple_type_specifier	:	CHAR */
/*simple_type_specifier	:	WCHAR_T */
/*simple_type_specifier	:	BOOL */
/*simple_type_specifier	:	SHORT */
/*simple_type_specifier	:	INT */
/*simple_type_specifier	:	LONG */
/*simple_type_specifier	:	SIGNED */
/*simple_type_specifier	:	UNSIGNED */
/*simple_type_specifier	:	FLOAT */
/*simple_type_specifier	:	DOUBLE */
/*simple_type_specifier	:	VOID */
static psrNode_t* AR_STDCALL handle_simple_type_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*type_name	:	class_name */
/*type_name	:	enum_name */
/*type_name	:	typedef_name */
static psrNode_t* AR_STDCALL handle_type_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*elaborated_type_specifier	:	class_key COLONCOLON_opt nested_name_specifier_opt identifier */
/*elaborated_type_specifier	:	ENUM COLONCOLON_opt nested_name_specifier_opt identifier */
/*elaborated_type_specifier	:	TYPENAME COLONCOLON_opt nested_name_specifier identifier */
/*elaborated_type_specifier	:	TYPENAME COLONCOLON_opt nested_name_specifier identifier < template_argument_list > */
static psrNode_t* AR_STDCALL handle_elaborated_type_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*enum_specifier	:	ENUM identifier_opt { enumerator_list_opt } */
static psrNode_t* AR_STDCALL handle_enum_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*enumerator_list	:	enumerator_definition */
/*enumerator_list	:	enumerator_list , enumerator_definition */
static psrNode_t* AR_STDCALL handle_enumerator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*enumerator_definition	:	enumerator */
/*enumerator_definition	:	enumerator = constant_expression */
static psrNode_t* AR_STDCALL handle_enumerator_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*enumerator	:	identifier */
static psrNode_t* AR_STDCALL handle_enumerator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*namespace_definition	:	named_namespace_definition */
/*namespace_definition	:	unnamed_namespace_definition */
static psrNode_t* AR_STDCALL handle_namespace_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*named_namespace_definition	:	original_namespace_definition */
/*named_namespace_definition	:	extension_namespace_definition */
static psrNode_t* AR_STDCALL handle_named_namespace_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*original_namespace_definition	:	NAMESPACE identifier { namespace_body } */
static psrNode_t* AR_STDCALL handle_original_namespace_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*extension_namespace_definition	:	NAMESPACE original_namespace_name { namespace_body } */
static psrNode_t* AR_STDCALL handle_extension_namespace_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*unnamed_namespace_definition	:	NAMESPACE { namespace_body } */
static psrNode_t* AR_STDCALL handle_unnamed_namespace_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*namespace_body	:	declaration_seq_opt */
static psrNode_t* AR_STDCALL handle_namespace_body(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*namespace_alias_definition	:	NAMESPACE identifier = qualified_namespace_specifier ; */
static psrNode_t* AR_STDCALL handle_namespace_alias_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*qualified_namespace_specifier	:	COLONCOLON_opt nested_name_specifier_opt namespace_name */
static psrNode_t* AR_STDCALL handle_qualified_namespace_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*using_declaration	:	USING TYPENAME_opt COLONCOLON_opt nested_name_specifier unqualified_id ; */
/*using_declaration	:	USING COLONCOLON unqualified_id ; */
static psrNode_t* AR_STDCALL handle_using_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*using_directive	:	USING NAMESPACE COLONCOLON_opt nested_name_specifier_opt namespace_name ; */
static psrNode_t* AR_STDCALL handle_using_directive(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*asm_definition	:	ASM ( string_literal ) ; */
static psrNode_t* AR_STDCALL handle_asm_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*linkage_specification	:	EXTERN string_literal { declaration_seq_opt } */
/*linkage_specification	:	EXTERN string_literal declaration */
static psrNode_t* AR_STDCALL handle_linkage_specification(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*init_declarator_list	:	init_declarator */
/*init_declarator_list	:	init_declarator_list , init_declarator */
static psrNode_t* AR_STDCALL handle_init_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*init_declarator	:	declarator initializer_opt */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*declarator	:	direct_declarator */
/*declarator	:	ptr_operator declarator */
static psrNode_t* AR_STDCALL handle_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*direct_declarator	:	declarator_id */
/*direct_declarator	:	direct_declarator ( parameter_declaration_clause ) cv_qualifier_seq_opt exception_specification_opt */
/*direct_declarator	:	direct_declarator [ constant_expression_opt ] */
/*direct_declarator	:	( declarator ) */
static psrNode_t* AR_STDCALL handle_direct_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*ptr_operator	:	* cv_qualifier_seq_opt */
/*ptr_operator	:	& */
/*ptr_operator	:	COLONCOLON_opt nested_name_specifier * cv_qualifier_seq_opt */
static psrNode_t* AR_STDCALL handle_ptr_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*cv_qualifier_seq	:	cv_qualifier cv_qualifier_seq_opt */
static psrNode_t* AR_STDCALL handle_cv_qualifier_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*cv_qualifier	:	CONST */
/*cv_qualifier	:	VOLATILE */
static psrNode_t* AR_STDCALL handle_cv_qualifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*declarator_id	:	COLONCOLON_opt id_expression */
/*declarator_id	:	COLONCOLON_opt nested_name_specifier_opt type_name */
static psrNode_t* AR_STDCALL handle_declarator_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*type_id	:	type_specifier_seq abstract_declarator_opt */
static psrNode_t* AR_STDCALL handle_type_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*type_specifier_seq	:	type_specifier type_specifier_seq_opt */
static psrNode_t* AR_STDCALL handle_type_specifier_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*abstract_declarator	:	ptr_operator abstract_declarator_opt */
/*abstract_declarator	:	direct_abstract_declarator */
static psrNode_t* AR_STDCALL handle_abstract_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*direct_abstract_declarator	:	direct_abstract_declarator_opt ( parameter_declaration_clause ) cv_qualifier_seq_opt exception_specification_opt */
/*direct_abstract_declarator	:	direct_abstract_declarator_opt [ constant_expression_opt ] */
/*direct_abstract_declarator	:	( abstract_declarator ) */
static psrNode_t* AR_STDCALL handle_direct_abstract_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*parameter_declaration_clause	:	parameter_declaration_list_opt ELLIPSIS_opt */
/*parameter_declaration_clause	:	parameter_declaration_list , ELLIPSIS */
static psrNode_t* AR_STDCALL handle_parameter_declaration_clause(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*parameter_declaration_list	:	parameter_declaration */
/*parameter_declaration_list	:	parameter_declaration_list , parameter_declaration */
static psrNode_t* AR_STDCALL handle_parameter_declaration_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*parameter_declaration	:	decl_specifier_seq declarator */
/*parameter_declaration	:	decl_specifier_seq declarator = assignment_expression */
/*parameter_declaration	:	decl_specifier_seq abstract_declarator_opt */
/*parameter_declaration	:	decl_specifier_seq abstract_declarator_opt = assignment_expression */
static psrNode_t* AR_STDCALL handle_parameter_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*function_definition	:	decl_specifier_seq_opt declarator ctor_initializer_opt function_body */
/*function_definition	:	decl_specifier_seq_opt declarator function_try_block */
static psrNode_t* AR_STDCALL handle_function_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*function_body	:	compound_statement */
static psrNode_t* AR_STDCALL handle_function_body(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*initializer	:	= initializer_clause */
/*initializer	:	( expression_list ) */
static psrNode_t* AR_STDCALL handle_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*initializer_clause	:	assignment_expression */
/*initializer_clause	:	{ initializer_list COMMA_opt } */
/*initializer_clause	:	{ } */
static psrNode_t* AR_STDCALL handle_initializer_clause(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*initializer_list	:	initializer_clause */
/*initializer_list	:	initializer_list , initializer_clause */
static psrNode_t* AR_STDCALL handle_initializer_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*class_specifier	:	class_head { member_specification_opt } */
static psrNode_t* AR_STDCALL handle_class_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*class_head	:	class_key identifier_opt base_clause_opt */
/*class_head	:	class_key nested_name_specifier identifier base_clause_opt */
static psrNode_t* AR_STDCALL handle_class_head(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*class_key	:	CLASS */
/*class_key	:	STRUCT */
/*class_key	:	UNION */
static psrNode_t* AR_STDCALL handle_class_key(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*member_specification	:	member_declaration member_specification_opt */
/*member_specification	:	access_specifier : member_specification_opt */
static psrNode_t* AR_STDCALL handle_member_specification(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*member_declaration	:	decl_specifier_seq_opt member_declarator_list_opt ; */
/*member_declaration	:	function_definition SEMICOLON_opt */
/*member_declaration	:	qualified_id ; */
/*member_declaration	:	using_declaration */
/*member_declaration	:	template_declaration */
static psrNode_t* AR_STDCALL handle_member_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*member_declarator_list	:	member_declarator */
/*member_declarator_list	:	member_declarator_list , member_declarator */
static psrNode_t* AR_STDCALL handle_member_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*member_declarator	:	declarator pure_specifier_opt */
/*member_declarator	:	declarator constant_initializer_opt */
/*member_declarator	:	identifier_opt : constant_expression */
static psrNode_t* AR_STDCALL handle_member_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*pure_specifier	:	= integer_literal */
static psrNode_t* AR_STDCALL handle_pure_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*constant_initializer	:	= constant_expression */
static psrNode_t* AR_STDCALL handle_constant_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*base_clause	:	: base_specifier_list */
static psrNode_t* AR_STDCALL handle_base_clause(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*base_specifier_list	:	base_specifier */
/*base_specifier_list	:	base_specifier_list , base_specifier */
static psrNode_t* AR_STDCALL handle_base_specifier_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*base_specifier	:	COLONCOLON_opt nested_name_specifier_opt class_name */
/*base_specifier	:	VIRTUAL access_specifier_opt COLONCOLON_opt nested_name_specifier_opt class_name */
/*base_specifier	:	access_specifier VIRTUAL_opt COLONCOLON_opt nested_name_specifier_opt class_name */
static psrNode_t* AR_STDCALL handle_base_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*access_specifier	:	PRIVATE */
/*access_specifier	:	PROTECTED */
/*access_specifier	:	PUBLIC */
static psrNode_t* AR_STDCALL handle_access_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*conversion_function_id	:	OPERATOR conversion_type_id */
static psrNode_t* AR_STDCALL handle_conversion_function_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*conversion_type_id	:	type_specifier_seq conversion_declarator_opt */
static psrNode_t* AR_STDCALL handle_conversion_type_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*conversion_declarator	:	ptr_operator conversion_declarator_opt */
static psrNode_t* AR_STDCALL handle_conversion_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*ctor_initializer	:	: mem_initializer_list */
static psrNode_t* AR_STDCALL handle_ctor_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*mem_initializer_list	:	mem_initializer */
/*mem_initializer_list	:	mem_initializer , mem_initializer_list */
static psrNode_t* AR_STDCALL handle_mem_initializer_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*mem_initializer	:	mem_initializer_id ( expression_list_opt ) */
static psrNode_t* AR_STDCALL handle_mem_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*mem_initializer_id	:	COLONCOLON_opt nested_name_specifier_opt class_name */
/*mem_initializer_id	:	identifier */
static psrNode_t* AR_STDCALL handle_mem_initializer_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*operator_function_id	:	OPERATOR operator */
static psrNode_t* AR_STDCALL handle_operator_function_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*operator	:	NEW */
/*operator	:	DELETE */
/*operator	:	NEW [ ] */
/*operator	:	DELETE [ ] */
/*operator	:	+ */
/*operator	:	* */
/*operator	:	/ */
/*operator	:	% */
/*operator	:	^ */
/*operator	:	& */
/*operator	:	| */
/*operator	:	~ */
/*operator	:	! */
/*operator	:	= */
/*operator	:	< */
/*operator	:	> */
/*operator	:	ADDEQ */
/*operator	:	SUBEQ */
/*operator	:	MULEQ */
/*operator	:	DIVEQ */
/*operator	:	MODEQ */
/*operator	:	XOREQ */
/*operator	:	ANDEQ */
/*operator	:	OREQ */
/*operator	:	SL */
/*operator	:	SR */
/*operator	:	SREQ */
/*operator	:	SLEQ */
/*operator	:	EQ */
/*operator	:	NOTEQ */
/*operator	:	LTEQ */
/*operator	:	GTEQ */
/*operator	:	ANDAND */
/*operator	:	OROR */
/*operator	:	PLUSPLUS */
/*operator	:	MINUSMINUS */
/*operator	:	, */
/*operator	:	ARROWSTAR */
/*operator	:	ARROW */
/*operator	:	( ) */
/*operator	:	[ ] */
static psrNode_t* AR_STDCALL handle_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*template_declaration	:	EXPORT_opt TEMPLATE < template_parameter_list > declaration */
static psrNode_t* AR_STDCALL handle_template_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*template_parameter_list	:	template_parameter */
/*template_parameter_list	:	template_parameter_list , template_parameter */
static psrNode_t* AR_STDCALL handle_template_parameter_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*template_parameter	:	type_parameter */
/*template_parameter	:	parameter_declaration */
static psrNode_t* AR_STDCALL handle_template_parameter(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*type_parameter	:	CLASS identifier_opt */
/*type_parameter	:	CLASS identifier_opt = type_id */
/*type_parameter	:	TYPENAME identifier_opt */
/*type_parameter	:	TYPENAME identifier_opt = type_id */
/*type_parameter	:	TEMPLATE < template_parameter_list > CLASS identifier_opt */
/*type_parameter	:	TEMPLATE < template_parameter_list > CLASS identifier_opt = template_name */
static psrNode_t* AR_STDCALL handle_type_parameter(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*template_id	:	template_name < template_argument_list > */
static psrNode_t* AR_STDCALL handle_template_id(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*template_argument_list	:	template_argument */
/*template_argument_list	:	template_argument_list , template_argument */
static psrNode_t* AR_STDCALL handle_template_argument_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*template_argument	:	assignment_expression */
/*template_argument	:	type_id */
/*template_argument	:	template_name */
static psrNode_t* AR_STDCALL handle_template_argument(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*explicit_instantiation	:	TEMPLATE declaration */
static psrNode_t* AR_STDCALL handle_explicit_instantiation(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*explicit_specialization	:	TEMPLATE < > declaration */
static psrNode_t* AR_STDCALL handle_explicit_specialization(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*try_block	:	TRY compound_statement handler_seq */
static psrNode_t* AR_STDCALL handle_try_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*function_try_block	:	TRY ctor_initializer_opt function_body handler_seq */
static psrNode_t* AR_STDCALL handle_function_try_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*handler_seq	:	handler handler_seq_opt */
static psrNode_t* AR_STDCALL handle_handler_seq(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*handler	:	CATCH ( exception_declaration ) compound_statement */
static psrNode_t* AR_STDCALL handle_handler(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*exception_declaration	:	type_specifier_seq declarator */
/*exception_declaration	:	type_specifier_seq abstract_declarator */
/*exception_declaration	:	type_specifier_seq */
/*exception_declaration	:	ELLIPSIS */
static psrNode_t* AR_STDCALL handle_exception_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*throw_expression	:	THROW assignment_expression_opt */
static psrNode_t* AR_STDCALL handle_throw_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*exception_specification	:	THROW ( type_id_list_opt ) */
static psrNode_t* AR_STDCALL handle_exception_specification(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*type_id_list	:	type_id */
/*type_id_list	:	type_id_list , type_id */
static psrNode_t* AR_STDCALL handle_type_id_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*declaration_seq_opt	:	declaration_seq */
static psrNode_t* AR_STDCALL handle_declaration_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*TEMPLATE_opt	:	TEMPLATE */
static psrNode_t* AR_STDCALL handle_TEMPLATE_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*nested_name_specifier_opt	:	nested_name_specifier */
static psrNode_t* AR_STDCALL handle_nested_name_specifier_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*expression_list_opt	:	expression_list */
static psrNode_t* AR_STDCALL handle_expression_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*COLONCOLON_opt	:	COLONCOLON */
static psrNode_t* AR_STDCALL handle_COLONCOLON_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*new_placement_opt	:	new_placement */
static psrNode_t* AR_STDCALL handle_new_placement_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*new_initializer_opt	:	new_initializer */
static psrNode_t* AR_STDCALL handle_new_initializer_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*new_declarator_opt	:	new_declarator */
static psrNode_t* AR_STDCALL handle_new_declarator_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*expression_opt	:	expression */
static psrNode_t* AR_STDCALL handle_expression_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*statement_seq_opt	:	statement_seq */
static psrNode_t* AR_STDCALL handle_statement_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*condition_opt	:	condition */
static psrNode_t* AR_STDCALL handle_condition_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*decl_specifier_seq_opt	:	decl_specifier_seq */
static psrNode_t* AR_STDCALL handle_decl_specifier_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*init_declarator_list_opt	:	init_declarator_list */
static psrNode_t* AR_STDCALL handle_init_declarator_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*identifier_opt	:	identifier */
static psrNode_t* AR_STDCALL handle_identifier_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*enumerator_list_opt	:	enumerator_list */
static psrNode_t* AR_STDCALL handle_enumerator_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*TYPENAME_opt	:	TYPENAME */
static psrNode_t* AR_STDCALL handle_TYPENAME_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*initializer_opt	:	initializer */
static psrNode_t* AR_STDCALL handle_initializer_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*cv_qualifier_seq_opt	:	cv_qualifier_seq */
static psrNode_t* AR_STDCALL handle_cv_qualifier_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*exception_specification_opt	:	exception_specification */
static psrNode_t* AR_STDCALL handle_exception_specification_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*constant_expression_opt	:	constant_expression */
static psrNode_t* AR_STDCALL handle_constant_expression_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*abstract_declarator_opt	:	abstract_declarator */
static psrNode_t* AR_STDCALL handle_abstract_declarator_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*type_specifier_seq_opt	:	type_specifier_seq */
static psrNode_t* AR_STDCALL handle_type_specifier_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*direct_abstract_declarator_opt	:	direct_abstract_declarator */
static psrNode_t* AR_STDCALL handle_direct_abstract_declarator_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*parameter_declaration_list_opt	:	parameter_declaration_list */
static psrNode_t* AR_STDCALL handle_parameter_declaration_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*ELLIPSIS_opt	:	ELLIPSIS */
static psrNode_t* AR_STDCALL handle_ELLIPSIS_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*ctor_initializer_opt	:	ctor_initializer */
static psrNode_t* AR_STDCALL handle_ctor_initializer_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*COMMA_opt	:	, */
static psrNode_t* AR_STDCALL handle_COMMA_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*member_specification_opt	:	member_specification */
static psrNode_t* AR_STDCALL handle_member_specification_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*base_clause_opt	:	base_clause */
static psrNode_t* AR_STDCALL handle_base_clause_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*member_declarator_list_opt	:	member_declarator_list */
static psrNode_t* AR_STDCALL handle_member_declarator_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*SEMICOLON_opt	:	; */
static psrNode_t* AR_STDCALL handle_SEMICOLON_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*pure_specifier_opt	:	pure_specifier */
static psrNode_t* AR_STDCALL handle_pure_specifier_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*constant_initializer_opt	:	constant_initializer */
static psrNode_t* AR_STDCALL handle_constant_initializer_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*access_specifier_opt	:	access_specifier */
static psrNode_t* AR_STDCALL handle_access_specifier_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*VIRTUAL_opt	:	VIRTUAL */
static psrNode_t* AR_STDCALL handle_VIRTUAL_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*conversion_declarator_opt	:	conversion_declarator */
static psrNode_t* AR_STDCALL handle_conversion_declarator_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*EXPORT_opt	:	EXPORT */
static psrNode_t* AR_STDCALL handle_EXPORT_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*handler_seq_opt	:	handler_seq */
static psrNode_t* AR_STDCALL handle_handler_seq_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*assignment_expression_opt	:	assignment_expression */
static psrNode_t* AR_STDCALL handle_assignment_expression_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




/*type_id_list_opt	:	type_id_list */
static psrNode_t* AR_STDCALL handle_type_id_list_opt(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
	 return NULL;
}




