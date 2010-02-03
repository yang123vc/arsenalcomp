
/*  '"'  == \x22,  '\'' == \x27 */


static const wchar_t *__g_lex_name[] = 
{
		L"delim 		=	[ \\r\\n\\t]",
		L"comment		=	/\\*([^\\*]|\\*+[^\\*/])*\\*+/",
		L"comment_line	=	//[^\\n]*\\n",
		L"digit			=	[0-9]",
		L"number			=	{digit}+",
		L"letter			= [A-Z_a-z]",
		L"int_type_suffix	=	((u|U)?(l|L))|((u|U)(l|L)?)",
		L"octal_escape	= (\\\\[0-3][0-7][0-7])|(\\\\[0-7][0-7])|(\\\\[0-7])",
		L"hex_digit		= [0-9a-fA-F]",
		L"hex_literal 	=	0(x|X){hex_digit}+{int_type_suffix}?",
		L"dec_literal	=	(0|[1-9][0-9]*){int_type_suffix}?",
		L"oct_literal	= 0[0-7]+{int_type_suffix}?",
		L"exponet		= (e|E)(\\+|\\-)?[0-9]+",
		L"float_type_suffix	 = (f|F|d|D)",
		L"float_literal	=	([0-9]+\\.[0-9]*{exponet}?{float_type_suffix}?)",
		L"skip_lexem	= 	{delim}|{comment}|{comment_line}",
		L"escape_seq		=	(\\\\(b|t|n|f|r|\\x22|\\x27|\\\\))|{octal_escape}",
		L"string_literal	= 	(\\x22({escape_seq}|[^\\x22\\\\])*\\x22)",
		L"char_literal	=	\\x27({escape_seq}|[^\\x27\\\\])\\x27",
		L"keyword_lhd	=	[A-Z_a-z0-9]"
};



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


typedef struct __ray_lex_pattern_tag
{
		size_t			val;
		const wchar_t	*regex;
		bool_t			is_skip;
		size_t			prec;
}rayLexPattern_t;


static const rayLexPattern_t	__g_lex_pattern[] = 
{
		{AUTO,			L"\"auto\"(?!{keyword_lhd})",	false,	1},
		{EXTERN,		L"\"extern\"(?!{keyword_lhd})",	false,	1},
		{REGISTER,		L"\"register\"(?!{keyword_lhd})",	false,	1},
		{VOLATILE,		L"\"volatile\"(?!{keyword_lhd})",	false,	1},
		{STATIC,		L"\"static\"(?!{keyword_lhd})",	false,	1},
		{SWITCH,		L"\"switch\"(?!{keyword_lhd})",	false,	1},
		{FOR,			L"\"for\"(?!{keyword_lhd})",	false,	1},
		{GOTO,			L"\"goto\"(?!{keyword_lhd})",	false,	1},
		{DO,			L"\"do\"(?!{keyword_lhd})",	false,	1},
		{WHILE,			L"\"while\"(?!{keyword_lhd})",	false,	1},
		{IF,			L"\"if\"(?!{keyword_lhd})",	false,	1},
		{ELSE,			L"\"else\"(?!{keyword_lhd})",	false,	1},
		{ENUM,			L"\"enum\"(?!{keyword_lhd})",	false,	1},
		{CONTINUE,		L"\"continue\"(?!{keyword_lhd})",	false,	1},
		{DEFAULT,		L"\"default\"(?!{keyword_lhd})",	false,	1},
		{CASE,			L"\"case\"(?!{keyword_lhd})",	false,	1},
		{BREAK,			L"\"break\"(?!{keyword_lhd})",	false,	1},
		{CONSTANT,		L"\"const\"(?!{keyword_lhd})",	false,	1},
		{SIZEOF,		L"\"sizeof\"(?!{keyword_lhd})",	false,	1},
		{STRUCT,		L"\"struct\"(?!{keyword_lhd})",	false,	1},
		{UNION,			L"\"union\"(?!{keyword_lhd})",	false,	1},
		{TYPEDEF,		L"\"typedef\"(?!{keyword_lhd})",	false,	1},
		{ATTRIBUTE,		L"\"attribute\"(?!{keyword_lhd})",	false,	1},

		
		{VOID_TYPE,		L"\"void\"(?!{keyword_lhd})",	false,	1},
		{SIGNED,		L"\"signed\"(?!{keyword_lhd})",	false,	1},
		{UNSIGNED,		L"\"unsigned\"(?!{keyword_lhd})",	false,	1},
		{BYTE_TYPE,		L"\"byte\"(?!{keyword_lhd})",	false,	1},
		{CHAR_TYPE,		L"\"char\"(?!{keyword_lhd})",	false,	1},
		{SHORT_TYPE,	L"\"short\"(?!{keyword_lhd})",	false,	1},
		{INT_TYPE,		L"\"int\"(?!{keyword_lhd})",	false,	1},
		{LONG_TYPE,		L"\"long\"(?!{keyword_lhd})",	false,	1},
		{FLOAT_TYPE,	L"\"float\"(?!{keyword_lhd})",	false,	1},
		{DOUBLE_TYPE,	L"\"double\"(?!{keyword_lhd})",	false,	1},
		

		{AND,			L"\"&\"",	false,	0},
		{NOT,			L"\"!\"",	false,	0},
		{NEGA,			L"\"~\"",	false,	0},
		{SUB,			L"\"-\"",	false,	0},
		{ADD,			L"\"+\"",	false,	0},
		{MUL,			L"\"*\"",	false,	0},
		{DIV,			L"\"/\"",	false,	0},
		{MOD,			L"\"%\"",	false,	0},
		{LE,			L"\"<\"",	false,	0},
		{GE,			L"\">\"",	false,	0},
		{XOR,			L"\"^\"",	false,	0},
		{OR,			L"\"|\"",	false,	0},
		{QUEST,			L"\"?\"",	false,	0},
		{SEMICOLON,		L"\";\"",	false,	0},
		{L_BRACES,		L"\"{\"",	false,	0},
		{R_BRACES,		L"\"}\"",	false,	0},
		{L_SQUARE,		L"\"[\"",	false,	0},
		{R_SQUARE,		L"\"]\"",	false,	0},
		{L_PAREN,		L"\"(\"",	false,	0},
		{R_PAREN,		L"\")\"",	false,	0},
		{COMMA,			L"\",\"",	false,	0},
		{COLON,			L"\":\"",	false,	0},
		{ASSING,		L"\"=\"",	false,	0},
		{DOT,			L"\".\"",	false,	0},

		
		{RIGHT_OP,		L"\">>\"",	false,	1},
		{LEFT_OP,		L"\"<<\"",	false,	1},
		{INC_OP,		L"\"++\"",	false,	1},
		{DEC_OP,		L"\"--\"",	false,	1},

		{PTR_OP,		L"\"->\"",	false,	1},
		{AND_OP,		L"\"&&\"",	false,	1},
		{OR_OP,			L"\"||\"",	false,	1},
		{LE_OP,			L"\"<=\"",	false,	1},
		{GE_OP,			L"\">=\"",	false,	1},
		{EQ_OP,			L"\"==\"",	false,	1},
		{NE_OP,			L"\"!=\"",	false,	1},



		{ELLIPSIS,		L"\"...\"",	false,	2},
		{RIGHT_ASSIGN,	L"\">>=\"",	false,	2},
		{LEFT_ASSIGN,	L"\"<<=\"",	false,	2},
		{ADD_ASSIGN,	L"\"+=\"",	false,	2},
		{SUB_ASSIGN,	L"\"-=\"",	false,	2},
		{MUL_ASSIGN,	L"\"*=\"",	false,	2},
		{DIV_ASSIGN,	L"\"/=\"",	false,	2},
		{MOD_ASSIGN,	L"\"%=\"",	false,	2},
		{AND_ASSIGN,	L"\"&=\"",	false,	2},
		{XOR_ASSIGN,	L"\"^=\"",	false,	2},
		{OR_ASSIGN,		L"\"|=\"",	false,	2},

		{STRING_LITERAL,	L"{string_literal}",	false,	0},
		{OCT_CONSTANT,		L"{oct_literal}",	false,	0},
		{HEX_CONSTANT,		L"{hex_literal}",	false,	0},
		{DEC_CONSTANT,		L"{dec_literal}",	false,	0},
		{CHAR_CONSTANT,		L"{char_literal}",	false,	0},
		{FLOAT_CONSTANT,	L"{float_literal}",	false,	0},
		{IDENTIFIER,		L"{letter}({letter}|{digit})*",	false,	0},
		
		{DELIM,				L"{delim}+|{comment}+|{comment_line}+",	true,	1},

		{EOI,				L"$",	false,	0}

};


psrNode_t*		AR_STDCALL ray_handle_token(const psrToken_t *tok,void *ctx);

typedef struct	__ray_term_info_tag
{
		rayTokValue_t	val;
		const wchar_t	*name;
		psrAssocType_t	assoc;
		size_t			prec;
		psrTermFunc_t	leaf_f;
}rayTermInfo_t;


static const rayTermInfo_t	__ray_term[] =
{
		{AUTO,			L"auto",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{EXTERN,		L"extern",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{REGISTER,		L"register" ,	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{VOLATILE,		L"volatile",	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{STATIC,		L"static",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{SWITCH,		L"switch",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{FOR,			L"for",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{GOTO,			L"goto",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{RETURN,		L"return",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{DO,			L"do",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{WHILE,			L"while",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{IF,			L"if",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{ELSE,			L"else",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{ENUM,			L"enum",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{CONTINUE,		L"continue",	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{DEFAULT,		L"default",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{CASE,			L"case",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{BREAK,			L"break",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{CONSTANT,		L"const",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{SIZEOF,		L"sizeof",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{STRUCT,		L"struct",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{UNION,			L"union",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{TYPEDEF,		L"typedef",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{ATTRIBUTE,		L"attribute",	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		
		
		{SIGNED,		L"signed",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{UNSIGNED,		L"unsigned",	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{VOID_TYPE,		L"void",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{BYTE_TYPE,		L"byte",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{SHORT_TYPE,	L"short",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{INT_TYPE,		L"int",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{LONG_TYPE,		L"long",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{CHAR_TYPE,		L"char",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{FLOAT_TYPE,	L"float",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{DOUBLE_TYPE,	L"double",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},

		{STRING_LITERAL,L"STRING_LITERAL",	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{OCT_CONSTANT,	L"OCT_CONSTANT",	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{HEX_CONSTANT,	L"HEX_CONSTANT",	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{DEC_CONSTANT,	L"DEC_CONSTANT",	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{FLOAT_CONSTANT,L"FLOAT_CONSTANT",	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{CHAR_CONSTANT,	L"CHAR_CONSTANT",	PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{IDENTIFIER,	L"IDENTIFIER",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},

		{TYPE_ID,		L"TYPE_ID",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{FAKE_EOI,		L"FAKE_EOI",		PSR_ASSOC_NOASSOC, 0, ray_handle_token},

		
		{ELLIPSIS,		L"...",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{INC_OP,		L"++",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{DEC_OP,		L"--",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{PTR_OP,		L"->",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		
		{SEMICOLON,		L";",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{L_BRACES,		L"{",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{R_BRACES,		L"}",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{L_SQUARE,		L"[",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{R_SQUARE,		L"]",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{L_PAREN,		L"(",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{R_PAREN,		L")",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{COMMA,			L",",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{COLON,			L":",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{DOT,			L".",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		
		

		

		{ASSING,		L"=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{RIGHT_ASSIGN,	L">>=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{LEFT_ASSIGN,	L"<<=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{ADD_ASSIGN,	L"+=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{SUB_ASSIGN,	L"-=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{MUL_ASSIGN,	L"*=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{DIV_ASSIGN,	L"/=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{MOD_ASSIGN,	L"%=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{AND_ASSIGN,	L"&=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{XOR_ASSIGN,	L"^=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{OR_ASSIGN,		L"|=",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},

		{NOT,			L"!",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{NEGA,			L"~",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},
		{QUEST,			L"?",			PSR_ASSOC_NOASSOC, 0, ray_handle_token},

		
/*
%left		"||"				;
%left		"&&"				;
%left		"|"				;
%left		"^"				;
%left		"&"				;
%left		"=="	"!="			;
%left		"<"	"<="	">"	">="	;
%left		"<<"	">>"			;
%left		"+"	"-"			;
%left		"*"	"/"	"%"		;

*/

		{OR_OP,			L"||",			PSR_ASSOC_LEFT, 1, ray_handle_token},

		{AND_OP,		L"&&",			PSR_ASSOC_LEFT, 2, ray_handle_token},

		{OR,			L"|",			PSR_ASSOC_LEFT, 3, ray_handle_token},

		{XOR,			L"^",			PSR_ASSOC_LEFT, 4, ray_handle_token},

		{AND,			L"&",			PSR_ASSOC_LEFT, 5, ray_handle_token},

		{EQ_OP,			L"==",			PSR_ASSOC_LEFT, 6, ray_handle_token},
		{NE_OP,			L"!=",			PSR_ASSOC_LEFT, 6, ray_handle_token},

		{LE,			L"<",			PSR_ASSOC_LEFT, 7, ray_handle_token},
		{GE,			L">",			PSR_ASSOC_LEFT, 7, ray_handle_token},
		{LE_OP,			L"<=",			PSR_ASSOC_LEFT, 7, ray_handle_token},
		{GE_OP,			L">=",			PSR_ASSOC_LEFT, 7, ray_handle_token},

		{RIGHT_OP,		L">>",			PSR_ASSOC_LEFT, 8, ray_handle_token},
		{LEFT_OP,		L"<<",			PSR_ASSOC_LEFT, 8, ray_handle_token},

		{ADD,			L"+",			PSR_ASSOC_LEFT, 9, ray_handle_token},
		{SUB,			L"-",			PSR_ASSOC_LEFT, 9, ray_handle_token},

		{MUL,			L"*",			PSR_ASSOC_LEFT, 10, ray_handle_token},
		{DIV,			L"/",			PSR_ASSOC_LEFT, 10, ray_handle_token},
		{MOD,			L"%",			PSR_ASSOC_LEFT, 10, ray_handle_token},

		{IF_STMT_PREC,				L"IF_STMT_PREC",			PSR_ASSOC_NOASSOC, 101, NULL},
		{IF_STMT_ELSE_STMT_PREC,	L"IF_STMT_ELSE_STMT_PREC",	PSR_ASSOC_NOASSOC, 102, NULL},
		{SPEC_ACTION,				L"SPEC_ACTION",				PSR_ASSOC_NOASSOC, 103, NULL}
};


typedef struct __ray_rule_tag
{
		const wchar_t	*rule;
		const wchar_t	*prec_token;
		psrRuleFunc_t	handler;
		size_t			auto_ret;
}rayRule_t;

static const rayRule_t	__ray_rule[] = 
{
		{ L"translation_unit : external_declaration", NULL, NULL, 0},
		{ L"translation_unit : translation_unit external_declaration", NULL, NULL, 0},

/*函数定义或声明*/
		{ L"external_declaration : function_definition", NULL, NULL, 0},
		{ L"external_declaration : declaration", NULL, NULL, 0},
		{ L"function_definition : declaration_specifiers declarator compound_statement", NULL, NULL, 0},

/*声明*/

		{ L"declaration : declaration_specifiers ;", NULL, NULL, 0},
		{ L"declaration : declaration_specifiers init_declarator_list ;", NULL, NULL, 0},
		{ L"declaration : declaration_specifiers  error ;", NULL, NULL, 0},
		{ L"declaration : error ;", NULL, NULL, 0},


		{ L"declaration_list : declaration", NULL, NULL, 0},
		{ L"declaration_list : declaration_list declaration", NULL, NULL, 0},


		{ L"storage_class_specifier : typedef", NULL, NULL, 0},
		{ L"storage_class_specifier : extern", NULL, NULL, 0},

		{ L"storage_class_specifier : static", NULL, NULL, 0},
		{ L"storage_class_specifier : auto", NULL, NULL, 0},
		{ L"storage_class_specifier : register", NULL, NULL, 0},
		

		{ L"declaration_specifiers : storage_class_specifier", NULL, NULL, 0},
		{ L"declaration_specifiers : storage_class_specifier declaration_specifiers", NULL, NULL, 0},
		{ L"declaration_specifiers : type_specifier", NULL, NULL, 0},
		{ L"declaration_specifiers : type_qualifier", NULL, NULL, 0},
		{ L"declaration_specifiers : type_qualifier declaration_specifiers", NULL, NULL, 0},
		{ L"declaration_specifiers : type_specifier declaration_specifiers", NULL, NULL, 0},


		{ L"init_declarator_list : init_declarator", NULL, NULL, 0},
		{ L"init_declarator_list : init_declarator_list , init_declarator", NULL, NULL, 0},


		{ L"init_declarator : declarator", NULL, NULL, 0},
		{ L"init_declarator : declarator = initializer", NULL, NULL, 0},

		
/*基本类型*/
		{ L"type_specifier : void", NULL, NULL, 0},
		{ L"type_specifier : byte", NULL, NULL, 0},
		{ L"type_specifier : char", NULL, NULL, 0},
		{ L"type_specifier : short", NULL, NULL, 0},
		{ L"type_specifier : int", NULL, NULL, 0},
		{ L"type_specifier : long", NULL, NULL, 0},
		{ L"type_specifier : float", NULL, NULL, 0},
		{ L"type_specifier : double", NULL, NULL, 0},
		{ L"type_specifier : signed", NULL, NULL, 0},
		{ L"type_specifier : unsigned", NULL, NULL, 0},
		{ L"type_specifier : struct_or_union_specifier", NULL, NULL, 0},
		{ L"type_specifier : enum_specifier", NULL, NULL, 0},
		{ L"type_specifier : TYPE_ID", NULL, NULL, 0},


		

/*结构声明*/

		{ L"struct_or_union_specifier	:	struct_or_union IDENTIFIER { struct_declaration_list }", NULL, NULL, 0},
		{ L"struct_or_union_specifier	:	struct_or_union { struct_declaration_list }", NULL, NULL, 0},
		{ L"struct_or_union_specifier	:	struct_or_union { error }", NULL, NULL, 0},
		{ L"struct_or_union_specifier	:	struct_or_union error ;", NULL, NULL, 0},


		{ L"struct_or_union		:	struct", NULL, NULL, 0},
		{ L"struct_or_union		:	union", NULL, NULL, 0},
		
		{ L"struct_declaration		:	specifier_qualifier_list struct_declarator_list ;", NULL, NULL, 0},
		{ L"struct_declaration		:	specifier_qualifier_list  ;", NULL, NULL, 0},
		
		{ L"struct_declaration_list		:	struct_declaration", NULL, NULL, 0},
		{ L"struct_declaration_list		:	struct_declaration_list struct_declaration", NULL, NULL, 0},

		{ L"specifier_qualifier_list		:	type_specifier specifier_qualifier_list", NULL, NULL, 0},
		{ L"specifier_qualifier_list		:	type_specifier", NULL, NULL, 0},
		{ L"specifier_qualifier_list		:	type_qualifier specifier_qualifier_list", NULL, NULL, 0},
		{ L"specifier_qualifier_list		:	type_qualifier", NULL, NULL, 0},
		
		{ L"struct_declarator_list		:	struct_declarator", NULL, NULL, 0},
		{ L"struct_declarator_list		:	struct_declarator_list , struct_declarator", NULL, NULL, 0},
		
		{ L"struct_declarator			:	declarator", NULL, NULL, 0},
		/*
		去掉了位域
		{ L"struct_declarator		:	: constant_expression", NULL, NULL, 0},
		{ L"struct_declarator		:	declarator : constant_expression", NULL, NULL, 0},
		*/
		


/*
	枚举声明
*/
		{ L"enum_specifier		:	enum { enumerator_list }", NULL, NULL, 0},
		{ L"enum_specifier		:	enum IDENTIFIER { enumerator_list }", NULL, NULL, 0},
		{ L"enum_specifier		:	enum IDENTIFIER", NULL, NULL, 0},

		
		{ L"enumerator_list		:	enumerator", NULL, NULL, 0},
		{ L"enumerator_list		:	enumerator_list , enumerator", NULL, NULL, 0},


		{ L"enumerator			:	IDENTIFIER", NULL, NULL, 0},
		{ L"enumerator			:	IDENTIFIER = constant_expression", NULL, NULL, 0},

		{ L"type_qualifier			:	const", NULL, NULL, 0},
		{ L"type_qualifier			:	volatile", NULL, NULL, 0},

/*声明修饰符*/		
		{ L"attribute_modifier	:	attribute ( STRING_LITERAL )",  NULL, NULL, 0},
		{ L"attribute_modifier	:	",  L"SPEC_ACTION", NULL, 0},


/*所有声明的变量，函数指针，类型，等等都会被规约为declarator或declarator_list*/

		{ L"declarator			:	attribute_modifier pointer direct_declarator",  NULL, NULL, 0},
		{ L"declarator			:	attribute_modifier direct_declarator",  L"SPEC_ACTION", NULL, 0},


		{ L"direct_declarator		:		IDENTIFIER",  NULL, NULL, 0},
		{ L"direct_declarator		:		( declarator )",  NULL, NULL, 0},
		{ L"direct_declarator		:		direct_declarator [ constant_expression ]",  NULL, NULL, 0},
		{ L"direct_declarator		:		direct_declarator [ ]",  NULL, NULL, 0},
		{ L"direct_declarator		:		direct_declarator ( parameter_type_list )",  NULL, NULL, 0},
		{ L"direct_declarator		:		direct_declarator ( )",  NULL, NULL, 0},
		{ L"direct_declarator		:		direct_declarator ( error )",  NULL, NULL, 0},
		{ L"direct_declarator		:		direct_declarator [ error ]",  NULL, NULL, 0},
		

		{ L"pointer				:	*",  NULL, NULL, 0},
		{ L"pointer				:	* type_qualifier_list",  NULL, NULL, 0},
		{ L"pointer				:	* type_qualifier_list pointer",  NULL, NULL, 0},
		{ L"pointer				:	* pointer",  NULL, NULL, 0},


		{ L"type_qualifier_list	:	type_qualifier",  NULL, NULL, 0},
		{ L"type_qualifier_list	:	type_qualifier_list type_qualifier",  NULL, NULL, 0},

		{ L"parameter_type_list	:	parameter_list",		NULL, NULL, 0},
		{ L"parameter_type_list :	parameter_list , ...",  NULL, NULL, 0},

		{ L"parameter_list		:	parameter_declaration",  NULL, NULL, 0},
		{ L"parameter_list		:	parameter_list , parameter_declaration",  NULL, NULL, 0},

		{ L"parameter_declaration	:	declaration_specifiers declarator",  NULL, NULL, 0},
		{ L"parameter_declaration	:	declaration_specifiers abstract_declarator",  NULL, NULL, 0},
		{ L"parameter_declaration	:	declaration_specifiers",  NULL, NULL, 0},

		{ L"type_name			:	specifier_qualifier_list",  NULL, NULL, 0},
		{ L"type_name			:	specifier_qualifier_list abstract_declarator",  NULL, NULL, 0},

		{ L"abstract_declarator	:	pointer",  NULL, NULL, 0},
		{ L"abstract_declarator :	direct_abstract_declarator",  NULL, NULL, 0},
		{ L"abstract_declarator	:	pointer direct_abstract_declarator",  NULL, NULL, 0},

		{ L"direct_abstract_declarator	:	( abstract_declarator )",  NULL, NULL, 0},
		{ L"direct_abstract_declarator	:	[ ]",  NULL, NULL, 0},
		{ L"direct_abstract_declarator	:	[ constant_expression ]",  NULL, NULL, 0},
		{ L"direct_abstract_declarator	:	direct_abstract_declarator [ ]",  NULL, NULL, 0},
		{ L"direct_abstract_declarator	:	direct_abstract_declarator [ constant_expression ]",  NULL, NULL, 0},
		{ L"direct_abstract_declarator	:	( )",  NULL, NULL, 0},
		{ L"direct_abstract_declarator	:	( parameter_type_list )",  NULL, NULL, 0},
		{ L"direct_abstract_declarator	:	direct_abstract_declarator ( )",  NULL, NULL, 0},
		{ L"direct_abstract_declarator	:	direct_abstract_declarator ( parameter_type_list )",  NULL, NULL, 0},
		{ L"direct_abstract_declarator	:	( error )",  NULL, NULL, 0},
		{ L"direct_abstract_declarator	:	[	error ]",  NULL, NULL, 0},


/*
声明初始化
*/
		{ L"initializer			:	assignment_expression",  NULL, NULL, 0},
		{ L"initializer			:	{ initializer_list }",  NULL, NULL, 0},
		{ L"initializer			:	{ initializer_list , }",  NULL, NULL, 0},
		{ L"initializer			:	{ error }",  NULL, NULL, 0},

		{ L"initializer_list	:	initializer",  NULL, NULL, 0},
		{ L"initializer_list	:	initializer_list , initializer",  NULL, NULL, 0},

/*语句*/
		{ L"statement			:	labeled_statement", NULL, NULL, 0},
		{ L"statement			:	compound_statement", NULL, NULL, 0},
		{ L"statement			:	expression_statement", NULL, NULL, 0},
		{ L"statement			:	selection_statement", NULL, NULL, 0},
		{ L"statement			:	iteration_statement", NULL, NULL, 0},
		{ L"statement			:	jump_statement", NULL, NULL, 0},
		
		{L"labeled_statement	:	case constant_expression : statement", NULL, NULL, 0},
		{L"labeled_statement	:	default : statement", NULL, NULL, 0},
		{L"labeled_statement	:	IDENTIFIER : statement", NULL, NULL, 0},

		{L"push_symtbl			:	", L"SPEC_ACTION", NULL, 0},
		{L"pop_symtbl			:	", L"SPEC_ACTION", NULL, 0},

		{L"compound_statement	:	{ }", NULL, NULL, 0},
		{L"compound_statement	:	{ statement_list }", NULL, NULL, 0},
		{L"compound_statement	:	{ push_symtbl	declaration_list pop_symtbl	}", NULL, NULL, 0},
		{L"compound_statement	:	{ push_symtbl	declaration_list statement_list pop_symtbl	}					", NULL, NULL, 0},
		{L"compound_statement	:	{ error  }", NULL, NULL, 0},
		{L"compound_statement	:	{ push_symtbl declaration_list error pop_symtbl }", NULL, NULL, 0},
		{L"compound_statement	:	{ push_symtbl declaration_list statement_list error pop_symtbl }", NULL, NULL, 0},
		
		{L"statement_list		:	statement", NULL, NULL, 0},
		{L"statement_list		:	statement_list statement", NULL, NULL, 0},

		{L"expression_statement	:	;", NULL, NULL, 0},
		{L"expression_statement	:	expression ;", NULL, NULL, 0},

		{L"selection_statement		:	if ( expression ) statement", L"IF_STMT_PREC", NULL,0},
		{L"selection_statement		:	if ( expression ) statement else statement", L"IF_STMT_ELSE_STMT_PREC", NULL,0},
		{L"selection_statement		:	switch ( expression ) statement", NULL, NULL,0},

		{L"iteration_statement		:	while ( expression ) statement", NULL, NULL,0},
		{L"iteration_statement		:	do statement while ( expression ) ;", NULL, NULL,0},
		{L"iteration_statement		:	for ( expression_statement expression_statement ) statement", NULL, NULL,0},
		{L"iteration_statement		:	for ( expression_statement expression_statement expression ) statement", NULL, NULL,0},


		{L"jump_statement		:	goto IDENTIFIER ;", NULL, NULL,0},
		{L"jump_statement		:	continue ;", NULL, NULL,0},
		{L"jump_statement		:	break ;", NULL, NULL,0},
		{L"jump_statement		:	return ;", NULL, NULL,0},
		{L"jump_statement		:	return expression ;", NULL, NULL,0},





		

		/*表达式*/
		{L"expression			:	assignment_expression", NULL, NULL,0},
		{L"expression			:	expression , assignment_expression", NULL, NULL,0},


		{L"primary_expression	:	IDENTIFIER", NULL, NULL,0},
		{L"primary_expression	:	constant", NULL, NULL,0},
		{L"primary_expression	:	( expression )", NULL, NULL,0},

		{L"postfix_expression	:	primary_expression", NULL, NULL,0},
		{L"postfix_expression	:	postfix_expression [ expression ]", NULL, NULL,0},
		{L"postfix_expression	:	postfix_expression ( )", NULL, NULL,0},
		{L"postfix_expression	:	postfix_expression ( argument_expression_list )", NULL, NULL,0},
		{L"postfix_expression	:	postfix_expression . IDENTIFIER", NULL, NULL,0},
		{L"postfix_expression	:	postfix_expression -> IDENTIFIER", NULL, NULL,0},
		{L"postfix_expression	:	postfix_expression ++", NULL, NULL,0},
		{L"postfix_expression	:	postfix_expression --", NULL, NULL,0},


		
		{L"argument_expression_list	:	assignment_expression", NULL, NULL, 0},
		{L"argument_expression_list	:	argument_expression_list , assignment_expression", NULL, NULL, 0},
					

		{L"unary_expression	:	postfix_expression", NULL, NULL, 0},
		{L"unary_expression	:	++ unary_expression", NULL, NULL, 0},
		{L"unary_expression	:	-- unary_expression", NULL, NULL, 0},
		{L"unary_expression	:	unary_operator cast_expression", NULL, NULL, 0},
		{L"unary_expression	:	sizeof unary_expression", NULL, NULL, 0},
		{L"unary_expression	:	sizeof ( type_name )", NULL, NULL, 0},
				

		{L"unary_operator	:	&", NULL, NULL, 0},
		{L"unary_operator	:	*", NULL, NULL, 0},
		{L"unary_operator	:	+", NULL, NULL, 0},
		{L"unary_operator	:	-", NULL, NULL, 0},
		{L"unary_operator	:	~", NULL, NULL, 0},
		{L"unary_operator	:	!", NULL, NULL, 0},


		{L"cast_expression		:	unary_expression", NULL, NULL, 0},
		{L"cast_expression		:	( type_name ) cast_expression", NULL, NULL, 0},



		{L"binary_expression	:	binary_expression	+	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	-	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	*	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	/	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	%	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	<<	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	>>	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	<	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	<=	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	>	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	>=	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	==	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	!=	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	&	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	^	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	|	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	&&	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	binary_expression	||	binary_expression", NULL, NULL, 0},
		{L"binary_expression	:	cast_expression", NULL, NULL, 0},




		{L"constant_expression		:	binary_expression", NULL, NULL, 0},
		{L"constant_expression		:	binary_expression	?	expression	:	constant_expression", NULL, NULL, 0},


		{L"assignment_expression		:	constant_expression", NULL, NULL, 0},
		{L"assignment_expression		:	unary_expression assignment_operator assignment_expression", NULL, NULL, 0},
					

		{L"assignment_operator		:		=", NULL, NULL, 0},
		{L"assignment_operator		:		*=", NULL, NULL, 0},
		{L"assignment_operator		:		/=", NULL, NULL, 0},
		{L"assignment_operator		:		%=", NULL, NULL, 0},
		{L"assignment_operator		:		+=", NULL, NULL, 0},
		{L"assignment_operator		:		-=", NULL, NULL, 0},
		{L"assignment_operator		:		<<=", NULL, NULL, 0},
		{L"assignment_operator		:		>>=", NULL, NULL, 0},
		{L"assignment_operator		:		&=", NULL, NULL, 0},
		{L"assignment_operator		:		^=", NULL, NULL, 0},
		{L"assignment_operator		:		|=", NULL, NULL, 0},






/*常量*/
		{ L"constant : OCT_CONSTANT", NULL, NULL, 0},
		{ L"constant : HEX_CONSTANT", NULL, NULL, 0},
		{ L"constant : DEC_CONSTANT", NULL, NULL, 0},
		{ L"constant : FLOAT_CONSTANT", NULL, NULL, 0},
		{ L"constant : CHAR_CONSTANT", NULL, NULL, 0},
		{ L"constant : STRING_LITERAL", NULL, NULL, 0}
};



#if(0)







#endif
