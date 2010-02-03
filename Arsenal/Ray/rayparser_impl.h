


typedef enum
{
		EOI		= 0x00,
		DELIM = 1000,	
		AUTO ,			/*auto*/
		EXTERN,			/*extern*/
		REGISTER,		/*register*/
		VOLATILE,		/*volatile*/
		STATIC,			/*static*/
		SWITCH,			/*switch*/
		FOR,			/*for*/
		GOTO,			/*goto*/
		RETURN,			/*return*/
		DO,				/*do*/
		WHILE,			/*while*/
		IF,				/*if*/
		ELSE,			/*else*/
		ENUM,			/*enum*/
		CONTINUE,		/*continue*/
		DEFAULT,		/*default*/
		CASE,			/*case*/
		BREAK,			/*break*/
		CONSTANT,		/*const*/
		SIZEOF,			/*size*/
		STRUCT,			/*struct*/
		UNION,			/*union*/
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
		
		ELLIPSIS,		/*...*/
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
		RIGHT_OP,		/*>>*/		
		LEFT_OP,		/*<<*/		
		INC_OP,			/*++*/
		DEC_OP,			/*--*/

		PTR_OP,			/*->*/
		AND_OP,			/*&&*/
		OR_OP,			/*||*/		
		LE_OP,			/*<=*/
		GE_OP,			/*>=*/		
		EQ_OP,			/*==*/
		NE_OP,			/*!=*/			
		
		SEMICOLON,		/*;*/
		L_BRACES,		/*{*/
		R_BRACES,		/*}*/
		L_SQUARE,		/*[*/
		R_SQUARE,		/*]*/
		L_PAREN,		/*(*/
		R_PAREN,		/*)*/
		COMMA,			/*,*/
		COLON,			/*:*/
		ASSING,			/*=*/
		DOT,			/*.*/
		
		AND,			/*&*/
		NOT,			/*!*/
		NEGA,			/*~*/
		ADD,			/*+*/
		SUB,			/*-*/
		MUL,			/***/
		DIV,			/* / */
		MOD,			/* % */
		LE,				/*<*/
		GE,				/*>*/
		XOR,			/*^*/
		OR,				/*|*/
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
		SPEC_ACTION						/*特殊优先级*/
}rayTokValue_t;



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


static struct {const wchar_t *name; size_t tokval; size_t lex_prec; const wchar_t *regex; bool_t skip; }__g_term_pattern[] =  {
{NULL, 256, 1, L"{delim}+|{comment}+|{comment_line}+", true},
{L"STRING_LITERAL", 257, 0, L"{string_literal}", false},
{L"OCT_CONSTANT", 258, 0, L"{oct_literal}", false},
{L"HEX_CONSTANT", 259, 0, L"{hex_literal}", false},
{L"DEC_CONSTANT", 260, 0, L"{dec_literal}", false},
{L"FLOAT_CONSTANT", 261, 0, L"{float_literal}", false},
{L"CHAR_CONSTANT", 262, 0, L"{char_literal}", false},
{L"IDENTIFIER", 263, 0, L"{letter}({letter}|{digit})*", false},
{L"auto", 264, 1, L"\"auto\"(?!{keyword_lhd})", false},
{L"extern", 265, 1, L"\"extern\"(?!{keyword_lhd})", false},
{L"register", 266, 1, L"\"register\"(?!{keyword_lhd})", false},
{L"volatile", 267, 1, L"\"volatile\"(?!{keyword_lhd})", false},
{L"static", 268, 1, L"\"static\"(?!{keyword_lhd})", false},
{L"switch", 269, 1, L"\"switch\"(?!{keyword_lhd})", false},
{L"for", 270, 1, L"\"for\"(?!{keyword_lhd})", false},
{L"goto", 271, 1, L"\"goto\"(?!{keyword_lhd})", false},
{L"return", 272, 1, L"\"return\"(?!{keyword_lhd})", false},
{L"do", 273, 1, L"\"do\"(?!{keyword_lhd})", false},
{L"while", 274, 1, L"\"while\"(?!{keyword_lhd})", false},
{L"if", 275, 1, L"\"if\"(?!{keyword_lhd})", false},
{L"else", 276, 1, L"\"else\"(?!{keyword_lhd})", false},
{L"enum", 277, 1, L"\"enum\"(?!{keyword_lhd})", false},
{L"continue", 278, 1, L"\"continue\"(?!{keyword_lhd})", false},
{L"default", 279, 1, L"\"default\"(?!{keyword_lhd})", false},
{L"case", 280, 1, L"\"case\"(?!{keyword_lhd})", false},
{L"break", 281, 1, L"\"break\"(?!{keyword_lhd})", false},
{L"const", 282, 1, L"\"const\"(?!{keyword_lhd})", false},
{L"sizeof", 283, 1, L"\"sizeof\"(?!{keyword_lhd})", false},
{L"struct", 284, 1, L"\"struct\"(?!{keyword_lhd})", false},
{L"union", 285, 1, L"\"union\"(?!{keyword_lhd})", false},
{L"typedef", 286, 1, L"\"typedef\"(?!{keyword_lhd})", false},
{L"void", 287, 1, L"\"void\"(?!{keyword_lhd})", false},
{L"signed", 288, 1, L"\"signed\"(?!{keyword_lhd})", false},
{L"unsigned", 289, 1, L"\"unsigned\"(?!{keyword_lhd})", false},
{L"byte", 290, 1, L"\"byte\"(?!{keyword_lhd})", false},
{L"char", 291, 1, L"\"char\"(?!{keyword_lhd})", false},
{L"short", 292, 1, L"\"short\"(?!{keyword_lhd})", false},
{L"int", 293, 1, L"\"int\"(?!{keyword_lhd})", false},
{L"long", 294, 1, L"\"long\"(?!{keyword_lhd})", false},
{L"float", 295, 1, L"\"float\"(?!{keyword_lhd})", false},
{L"double", 296, 1, L"\"double\"(?!{keyword_lhd})", false},
{L"attribute", 297, 1, L"\"attribute\"", false},
{L"...", 298, 1, L"\"...\"", false},
{L">>=", 299, 2, L"\">>=\"", false},
{L"<<=", 300, 2, L"\"<<=\"", false},
{L"+=", 301, 1, L"\"+=\"", false},
{L"-=", 302, 1, L"\"-=\"", false},
{L"*=", 303, 1, L"\"*=\"", false},
{L"/=", 304, 1, L"\"/=\"", false},
{L"%=", 305, 1, L"\"%=\"", false},
{L"&=", 306, 1, L"\"&=\"", false},
{L"^=", 307, 1, L"\"^=\"", false},
{L"|=", 308, 1, L"\"|=\"", false},
{L">>", 309, 1, L"\">>\"", false},
{L"<<", 310, 1, L"\"<<\"", false},
{L"++", 311, 1, L"\"++\"", false},
{L"--", 312, 1, L"\"--\"", false},
{L"->", 313, 1, L"\"->\"", false},
{L"&&", 314, 1, L"\"&&\"", false},
{L"||", 315, 1, L"\"||\"", false},
{L"<=", 316, 1, L"\"<=\"", false},
{L">=", 317, 1, L"\">=\"", false},
{L"==", 318, 1, L"\"==\"", false},
{L"!=", 319, 1, L"\"!=\"", false},
{L";", 320, 0, L"\";\"", false},
{L"{", 321, 0, L"\"{\"", false},
{L"}", 322, 0, L"\"}\"", false},
{L",", 323, 0, L"\",\"", false},
{L":", 324, 0, L"\":\"", false},
{L"=", 325, 0, L"\"=\"", false},
{L"(", 326, 0, L"\"(\"", false},
{L")", 327, 0, L"\")\"", false},
{L"[", 328, 0, L"\"[\"", false},
{L"]", 329, 0, L"\"]\"", false},
{L".", 330, 0, L"\".\"", false},
{L"&", 331, 0, L"\"&\"", false},
{L"!", 332, 0, L"\"!\"", false},
{L"~", 333, 0, L"\"~\"", false},
{L"-", 334, 0, L"\"-\"", false},
{L"+", 335, 0, L"\"+\"", false},
{L"*", 336, 0, L"\"*\"", false},
{L"/", 337, 0, L"\"/\"", false},
{L"%", 338, 0, L"\"%\"", false},
{L"<", 339, 0, L"\"<\"", false},
{L">", 340, 0, L"\">\"", false},
{L"^", 341, 0, L"\"^\"", false},
{L"|", 342, 0, L"\"|\"", false},
{L"?", 343, 0, L"\"?\"", false},
{L"TYPE_ID", 344, 0, L"^", false},
{L"FAKE_EOI", 345, 0, L"^", false},
{L"EOI", 0, 2, L"$", false}
};


static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {
{L"||", 315, 1, PSR_ASSOC_LEFT},
{L"&&", 314, 2, PSR_ASSOC_LEFT},
{L"|", 342, 3, PSR_ASSOC_LEFT},
{L"^", 341, 4, PSR_ASSOC_LEFT},
{L"&", 331, 5, PSR_ASSOC_LEFT},
{L"==", 318, 6, PSR_ASSOC_LEFT},
{L"!=", 319, 6, PSR_ASSOC_LEFT},
{L"<", 339, 7, PSR_ASSOC_LEFT},
{L"<=", 316, 7, PSR_ASSOC_LEFT},
{L">", 340, 7, PSR_ASSOC_LEFT},
{L">=", 317, 7, PSR_ASSOC_LEFT},
{L"<<", 310, 8, PSR_ASSOC_LEFT},
{L">>", 309, 8, PSR_ASSOC_LEFT},
{L"+", 335, 9, PSR_ASSOC_LEFT},
{L"-", 334, 9, PSR_ASSOC_LEFT},
{L"*", 336, 10, PSR_ASSOC_LEFT},
{L"/", 337, 10, PSR_ASSOC_LEFT},
{L"%", 338, 10, PSR_ASSOC_LEFT},
{L"IF_STMT_PREC", 346, 11, PSR_ASSOC_NOASSOC},
{L"IF_STMT_ELSE_STMT_PREC", 347, 12, PSR_ASSOC_NOASSOC},
{L"SPEC_ACTION", 348, 13, PSR_ASSOC_NOASSOC}
};


/*translation_unit	:	external_declaration */
static psrNode_t* AR_STDCALL handle_translation_unit(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*external_declaration	:	function_definition */
static psrNode_t* AR_STDCALL handle_external_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*function_definition	:	declaration_specifiers declarator compound_statement */
static psrNode_t* AR_STDCALL handle_function_definition(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*declaration	:	declaration_specifiers ; */
static psrNode_t* AR_STDCALL handle_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*declaration_list	:	declaration */
static psrNode_t* AR_STDCALL handle_declaration_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*storage_class_specifier	:	typedef */
static psrNode_t* AR_STDCALL handle_storage_class_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*declaration_specifiers	:	storage_class_specifier */
static psrNode_t* AR_STDCALL handle_declaration_specifiers(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*init_declarator_list	:	init_declarator */
static psrNode_t* AR_STDCALL handle_init_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*init_declarator	:	declarator */
static psrNode_t* AR_STDCALL handle_init_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*type_specifier	:	void */
static psrNode_t* AR_STDCALL handle_type_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*struct_or_union_specifier	:	struct_or_union IDENTIFIER { struct_declaration_list } */
static psrNode_t* AR_STDCALL handle_struct_or_union_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*struct_or_union	:	struct */
static psrNode_t* AR_STDCALL handle_struct_or_union(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*struct_declaration	:	specifier_qualifier_list struct_declarator_list ; */
static psrNode_t* AR_STDCALL handle_struct_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*struct_declaration_list	:	struct_declaration */
static psrNode_t* AR_STDCALL handle_struct_declaration_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*specifier_qualifier_list	:	type_specifier specifier_qualifier_list */
static psrNode_t* AR_STDCALL handle_specifier_qualifier_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*struct_declarator_list	:	struct_declarator */
static psrNode_t* AR_STDCALL handle_struct_declarator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*struct_declarator	:	declarator */
static psrNode_t* AR_STDCALL handle_struct_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*enum_specifier	:	enum { enumerator_list } */
static psrNode_t* AR_STDCALL handle_enum_specifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*enumerator_list	:	enumerator */
static psrNode_t* AR_STDCALL handle_enumerator_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*enumerator	:	IDENTIFIER */
static psrNode_t* AR_STDCALL handle_enumerator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*type_qualifier	:	const */
static psrNode_t* AR_STDCALL handle_type_qualifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*attribute_modifier	:	attribute ( STRING_LITERAL ) */
static psrNode_t* AR_STDCALL handle_attribute_modifier(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*declarator	:	attribute_modifier pointer direct_declarator */
static psrNode_t* AR_STDCALL handle_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*direct_declarator	:	IDENTIFIER */
static psrNode_t* AR_STDCALL handle_direct_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*pointer	:	* */
static psrNode_t* AR_STDCALL handle_pointer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*type_qualifier_list	:	type_qualifier */
static psrNode_t* AR_STDCALL handle_type_qualifier_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*parameter_type_list	:	parameter_list */
static psrNode_t* AR_STDCALL handle_parameter_type_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*parameter_list	:	parameter_declaration */
static psrNode_t* AR_STDCALL handle_parameter_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*parameter_declaration	:	declaration_specifiers declarator */
static psrNode_t* AR_STDCALL handle_parameter_declaration(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*type_name	:	specifier_qualifier_list */
static psrNode_t* AR_STDCALL handle_type_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*abstract_declarator	:	pointer */
static psrNode_t* AR_STDCALL handle_abstract_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*direct_abstract_declarator	:	( abstract_declarator ) */
static psrNode_t* AR_STDCALL handle_direct_abstract_declarator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*initializer	:	assignment_expression */
static psrNode_t* AR_STDCALL handle_initializer(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*initializer_list	:	initializer */
static psrNode_t* AR_STDCALL handle_initializer_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*statement	:	labeled_statement */
static psrNode_t* AR_STDCALL handle_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*labeled_statement	:	case constant_expression : statement */
static psrNode_t* AR_STDCALL handle_labeled_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*push_symtbl	:	 */
static psrNode_t* AR_STDCALL handle_push_symtbl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*pop_symtbl	:	 */
static psrNode_t* AR_STDCALL handle_pop_symtbl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*compound_statement	:	{ } */
static psrNode_t* AR_STDCALL handle_compound_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*statement_list	:	statement */
static psrNode_t* AR_STDCALL handle_statement_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*expression_statement	:	; */
static psrNode_t* AR_STDCALL handle_expression_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*selection_statement	:	if ( expression ) statement */
static psrNode_t* AR_STDCALL handle_selection_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*iteration_statement	:	while ( expression ) statement */
static psrNode_t* AR_STDCALL handle_iteration_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*jump_statement	:	goto IDENTIFIER ; */
static psrNode_t* AR_STDCALL handle_jump_statement(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*expression	:	assignment_expression */
static psrNode_t* AR_STDCALL handle_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*primary_expression	:	IDENTIFIER */
static psrNode_t* AR_STDCALL handle_primary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*postfix_expression	:	primary_expression */
static psrNode_t* AR_STDCALL handle_postfix_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*argument_expression_list	:	assignment_expression */
static psrNode_t* AR_STDCALL handle_argument_expression_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*unary_expression	:	postfix_expression */
static psrNode_t* AR_STDCALL handle_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*unary_operator	:	& */
static psrNode_t* AR_STDCALL handle_unary_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*cast_expression	:	unary_expression */
static psrNode_t* AR_STDCALL handle_cast_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*binary_expression	:	binary_expression + binary_expression */
static psrNode_t* AR_STDCALL handle_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*constant_expression	:	binary_expression */
static psrNode_t* AR_STDCALL handle_constant_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*assignment_expression	:	constant_expression */
static psrNode_t* AR_STDCALL handle_assignment_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*assignment_operator	:	= */
static psrNode_t* AR_STDCALL handle_assignment_operator(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}
/*constant	:	OCT_CONSTANT */
static psrNode_t* AR_STDCALL handle_constant(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){return NULL;}





static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"translation_unit  :  external_declaration ", NULL, handle_translation_unit, 0},
{L"translation_unit  :  translation_unit external_declaration ", NULL, handle_translation_unit, 0},
{L"external_declaration  :  function_definition ", NULL, handle_external_declaration, 0},
{L"external_declaration  :  declaration ", NULL, handle_external_declaration, 0},
{L"function_definition  :  declaration_specifiers declarator compound_statement ", NULL, handle_function_definition, 0},
{L"declaration  :  declaration_specifiers ; ", NULL, handle_declaration, 0},
{L"declaration  :  declaration_specifiers init_declarator_list ; ", NULL, handle_declaration, 0},
{L"declaration  :  declaration_specifiers error ; ", NULL, handle_declaration, 0},
{L"declaration  :  error ; ", NULL, handle_declaration, 0},
{L"declaration_list  :  declaration ", NULL, handle_declaration_list, 0},
{L"declaration_list  :  declaration_list declaration ", NULL, handle_declaration_list, 0},
{L"storage_class_specifier  :  typedef ", NULL, handle_storage_class_specifier, 0},
{L"storage_class_specifier  :  extern ", NULL, handle_storage_class_specifier, 0},
{L"storage_class_specifier  :  static ", NULL, handle_storage_class_specifier, 0},
{L"storage_class_specifier  :  auto ", NULL, handle_storage_class_specifier, 0},
{L"storage_class_specifier  :  register ", NULL, handle_storage_class_specifier, 0},
{L"declaration_specifiers  :  storage_class_specifier ", NULL, handle_declaration_specifiers, 0},
{L"declaration_specifiers  :  storage_class_specifier declaration_specifiers ", NULL, handle_declaration_specifiers, 0},
{L"declaration_specifiers  :  type_specifier ", NULL, handle_declaration_specifiers, 0},
{L"declaration_specifiers  :  type_qualifier ", NULL, handle_declaration_specifiers, 0},
{L"declaration_specifiers  :  type_qualifier declaration_specifiers ", NULL, handle_declaration_specifiers, 0},
{L"declaration_specifiers  :  type_specifier declaration_specifiers ", NULL, handle_declaration_specifiers, 0},
{L"init_declarator_list  :  init_declarator ", NULL, handle_init_declarator_list, 0},
{L"init_declarator_list  :  init_declarator_list , init_declarator ", NULL, handle_init_declarator_list, 0},
{L"init_declarator  :  declarator ", NULL, handle_init_declarator, 0},
{L"init_declarator  :  declarator = initializer ", NULL, handle_init_declarator, 0},
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
{L"type_specifier  :  struct_or_union_specifier ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  enum_specifier ", NULL, handle_type_specifier, 0},
{L"type_specifier  :  TYPE_ID ", NULL, handle_type_specifier, 0},
{L"struct_or_union_specifier  :  struct_or_union IDENTIFIER { struct_declaration_list } ", NULL, handle_struct_or_union_specifier, 0},
{L"struct_or_union_specifier  :  struct_or_union { struct_declaration_list } ", NULL, handle_struct_or_union_specifier, 0},
{L"struct_or_union_specifier  :  struct_or_union IDENTIFIER ", NULL, handle_struct_or_union_specifier, 0},
{L"struct_or_union_specifier  :  struct_or_union { error } ", NULL, handle_struct_or_union_specifier, 0},
{L"struct_or_union_specifier  :  struct_or_union error ; ", NULL, handle_struct_or_union_specifier, 0},
{L"struct_or_union  :  struct ", NULL, handle_struct_or_union, 0},
{L"struct_or_union  :  union ", NULL, handle_struct_or_union, 0},
{L"struct_declaration  :  specifier_qualifier_list struct_declarator_list ; ", NULL, handle_struct_declaration, 0},
{L"struct_declaration  :  specifier_qualifier_list ; ", NULL, handle_struct_declaration, 0},
{L"struct_declaration_list  :  struct_declaration ", NULL, handle_struct_declaration_list, 0},
{L"struct_declaration_list  :  struct_declaration_list struct_declaration ", NULL, handle_struct_declaration_list, 0},
{L"specifier_qualifier_list  :  type_specifier specifier_qualifier_list ", NULL, handle_specifier_qualifier_list, 0},
{L"specifier_qualifier_list  :  type_specifier ", NULL, handle_specifier_qualifier_list, 0},
{L"specifier_qualifier_list  :  type_qualifier specifier_qualifier_list ", NULL, handle_specifier_qualifier_list, 0},
{L"specifier_qualifier_list  :  type_qualifier ", NULL, handle_specifier_qualifier_list, 0},
{L"struct_declarator_list  :  struct_declarator ", NULL, handle_struct_declarator_list, 0},
{L"struct_declarator_list  :  struct_declarator_list , struct_declarator ", NULL, handle_struct_declarator_list, 0},
{L"struct_declarator  :  declarator ", NULL, handle_struct_declarator, 0},
{L"enum_specifier  :  enum { enumerator_list } ", NULL, handle_enum_specifier, 0},
{L"enum_specifier  :  enum IDENTIFIER { enumerator_list } ", NULL, handle_enum_specifier, 0},
{L"enum_specifier  :  enum IDENTIFIER ", NULL, handle_enum_specifier, 0},
{L"enumerator_list  :  enumerator ", NULL, handle_enumerator_list, 0},
{L"enumerator_list  :  enumerator_list , enumerator ", NULL, handle_enumerator_list, 0},
{L"enumerator  :  IDENTIFIER ", NULL, handle_enumerator, 0},
{L"enumerator  :  IDENTIFIER = constant_expression ", NULL, handle_enumerator, 0},
{L"type_qualifier  :  const ", NULL, handle_type_qualifier, 0},
{L"type_qualifier  :  volatile ", NULL, handle_type_qualifier, 0},
{L"attribute_modifier  :  attribute ( STRING_LITERAL ) ", NULL, handle_attribute_modifier, 0},
{L"attribute_modifier  :   ", L"SPEC_ACTION", handle_attribute_modifier, 0},
{L"declarator  :  attribute_modifier pointer direct_declarator ", NULL, handle_declarator, 0},
{L"declarator  :  attribute_modifier direct_declarator ", NULL, handle_declarator, 0},
{L"direct_declarator  :  IDENTIFIER ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  ( declarator ) ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  direct_declarator [ constant_expression ] ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  direct_declarator [ ] ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  direct_declarator ( parameter_type_list ) ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  direct_declarator ( ) ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  direct_declarator ( error ) ", NULL, handle_direct_declarator, 0},
{L"direct_declarator  :  direct_declarator [ error ] ", NULL, handle_direct_declarator, 0},
{L"pointer  :  * ", NULL, handle_pointer, 0},
{L"pointer  :  * type_qualifier_list ", NULL, handle_pointer, 0},
{L"pointer  :  * type_qualifier_list pointer ", NULL, handle_pointer, 0},
{L"pointer  :  * pointer ", NULL, handle_pointer, 0},
{L"type_qualifier_list  :  type_qualifier ", NULL, handle_type_qualifier_list, 0},
{L"type_qualifier_list  :  type_qualifier_list type_qualifier ", NULL, handle_type_qualifier_list, 0},
{L"parameter_type_list  :  parameter_list ", NULL, handle_parameter_type_list, 0},
{L"parameter_type_list  :  parameter_list , ... ", NULL, handle_parameter_type_list, 0},
{L"parameter_list  :  parameter_declaration ", NULL, handle_parameter_list, 0},
{L"parameter_list  :  parameter_list , parameter_declaration ", NULL, handle_parameter_list, 0},
{L"parameter_declaration  :  declaration_specifiers declarator ", NULL, handle_parameter_declaration, 0},
{L"parameter_declaration  :  declaration_specifiers abstract_declarator ", NULL, handle_parameter_declaration, 0},
{L"parameter_declaration  :  declaration_specifiers ", NULL, handle_parameter_declaration, 0},
{L"type_name  :  specifier_qualifier_list ", NULL, handle_type_name, 0},
{L"type_name  :  specifier_qualifier_list abstract_declarator ", NULL, handle_type_name, 0},
{L"abstract_declarator  :  pointer ", NULL, handle_abstract_declarator, 0},
{L"abstract_declarator  :  direct_abstract_declarator ", NULL, handle_abstract_declarator, 0},
{L"abstract_declarator  :  pointer direct_abstract_declarator ", NULL, handle_abstract_declarator, 0},
{L"direct_abstract_declarator  :  ( abstract_declarator ) ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  [ ] ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  [ constant_expression ] ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  direct_abstract_declarator [ ] ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  direct_abstract_declarator [ constant_expression ] ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  ( ) ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  ( parameter_type_list ) ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  direct_abstract_declarator ( ) ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  direct_abstract_declarator ( parameter_type_list ) ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  ( error ) ", NULL, handle_direct_abstract_declarator, 0},
{L"direct_abstract_declarator  :  [ error ] ", NULL, handle_direct_abstract_declarator, 0},
{L"initializer  :  assignment_expression ", NULL, handle_initializer, 0},
{L"initializer  :  { initializer_list } ", NULL, handle_initializer, 0},
{L"initializer  :  { initializer_list , } ", NULL, handle_initializer, 0},
{L"initializer  :  { error } ", NULL, handle_initializer, 0},
{L"initializer_list  :  initializer ", NULL, handle_initializer_list, 0},
{L"initializer_list  :  initializer_list , initializer ", NULL, handle_initializer_list, 0},
{L"statement  :  labeled_statement ", NULL, handle_statement, 0},
{L"statement  :  compound_statement ", NULL, handle_statement, 0},
{L"statement  :  expression_statement ", NULL, handle_statement, 0},
{L"statement  :  selection_statement ", NULL, handle_statement, 0},
{L"statement  :  iteration_statement ", NULL, handle_statement, 0},
{L"statement  :  jump_statement ", NULL, handle_statement, 0},
{L"labeled_statement  :  case constant_expression : statement ", NULL, handle_labeled_statement, 0},
{L"labeled_statement  :  default : statement ", NULL, handle_labeled_statement, 0},
{L"labeled_statement  :  IDENTIFIER : statement ", NULL, handle_labeled_statement, 0},
{L"push_symtbl  :   ", L"SPEC_ACTION", handle_push_symtbl, 0},
{L"pop_symtbl  :   ", L"SPEC_ACTION", handle_pop_symtbl, 0},
{L"compound_statement  :  { } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { statement_list } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { push_symtbl declaration_list pop_symtbl } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { push_symtbl declaration_list statement_list pop_symtbl } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { error } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { push_symtbl declaration_list error pop_symtbl } ", NULL, handle_compound_statement, 0},
{L"compound_statement  :  { push_symtbl declaration_list statement_list error pop_symtbl } ", NULL, handle_compound_statement, 0},
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
