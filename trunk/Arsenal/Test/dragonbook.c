#include "test.h"

#include "..\Parser\grammar.h"
#include "..\Parser\lr_item.h"
#include "..\Parser\lr_dfa.h"
#include "..\Parser\parser.h"
#include "..\Parser\parser_in.h"
#include "..\Parser\lr_action.h"

AR_NAMESPACE_BEGIN




typedef struct __node_tag		Node_t;


static const wchar_t *lex_pattern = 
L"%lex{\r\n"
L"	delim 			= 	[ \\r\\n\\t]\r\n"
L"	any    			= 	[^\\0]\r\n"
L"	any_but_newline	=	[^\\n\\0]\r\n"
L"	comment			= 	\\/\\*([^\\*]|\\*+[^\\*\\/])*\\*+\\/\r\n"
L"	comment_line	= 	\\/\\/[^\\n]*\\n\r\n"
L"	digit			= 	[0-9]\r\n"
L"	number  		=	{digit}+\r\n"
L"	letter			=	[A-Z_a-z]\r\n"

L"	int_const		=	0|[1-9]{number}*|0{number}|0x[0-9A-F]*\r\n"

L"	exp_part		=	(e|E)(\"+\"|\"-\")?{number}\r\n"
L"	float_const		=	{number}.{number}({exp_part}?)\r\n"
L"	identifier		=	{letter}({letter}|{digit})*\r\n"
L"	EOI				=	\\0\r\n"
	

L"	%skip 1			{delim}*\r\n"
L"	%skip 2			{comment}+\r\n"
L"	%skip 3			{comment_line}+\r\n"

L"	270				{int_const}\r\n"
L"	271				{float_const}\r\n"
	
L"	290,0			{identifier}\r\n"
	
L"	300,1			\"=\"\r\n"
	
L"	400,1			\"&&\"\r\n"
L"	401,1			\"||\"\r\n"
L"	405,1			\"==\"\r\n"
L"	406,1			\"!=\"\r\n"
L"	407,1			\"<\"\r\n"
L"	408,1			\">\"\r\n"
L"	409,1			\"<=\"\r\n"
L"	410,1			\">=\"\r\n"

L"	413,1			\"+\"\r\n"
L"	414,1			\"-\"\r\n"
L"	415,1			\"*\"\r\n"
L"	416,1			\"/\"\r\n"
L"	417,1			\"%\"\r\n"
	
L"	503,1			\"!\"\r\n"

L"	600,1			\"[\"\r\n"
L"	601,1			\"]\"\r\n"
L"	602,1			\"(\"\r\n"
L"	603,1			\")\"\r\n"
L"	604,1			\"{\"\r\n"
L"	605,1			\"}\"\r\n"
L"	606,1			\";\"\r\n"
L"	610,1			\",\"\r\n"

L"	702,1			\"int\"\r\n"
L"	704,1			\"float\"\r\n"
L"	705,1			\"bool\"\r\n"
L"	706,1			\"char\"\r\n"

L"	800,1			\"for\"\r\n"
L"	802,1			\"if\"\r\n"
L"	803,1			\"break\"\r\n"
L"	806,1			\"do\"\r\n"
L"	807,1			\"else\"\r\n"
L"	809,1			\"while\"\r\n"
	
L"	900,1			\"true\"\r\n"
L"	901,1			\"false\"\r\n"
L"	0				{EOI}\r\n"
L"}\r\n"
;

typedef enum
{

		INT_CONST		=		270,
		FLOAT_CONST		=		280,
		IDENTIFIER		=		290,
		ASSIGN			=		300,
		ANDAND			=		400,
		OROR			=		401,
		EQUAL			=		405,		
		NOT_EQUAL		=		406,
		LESS			=		407,
		GREATER			=		408,
		LESS_EQUAL		=		409,
		GREATER_EQUAL	=		410,
		ADD				=		413,
		MINUS			=		414,
		MUL				=		415,
		DIV				=		416,
		MOD				=		418,
		NOT				=		503,
		LSQB			=		600,/*'['*/
		RSQB			=		601,/*']'*/
		LPAR			=		602,/*'('*/
		RPAR			=		603,/*')'*/
		LBRACE			=		604,/*'{'*/
		RBRACE			=		605,/*'}'*/
		SEMI			=		606,/*';'*/
		COMMA			=		610,/*','*/

		INT_KW			=		702,
		FLOAT_KW		=		704,
		BOOL_KW			=		705,
		CHAR_KW			=		706,
		
		FOR_KW			=		800,
		IF_KW			=		802,
		BREAK_KW		=		803,
		DO_KW			=		806,
		ELSE_KW			=		807,
		WHILE_KW		=		809,
		TRUE_KW			=		900,
		FALSE_KW		=		901
}rayLexType_t;





typedef struct __token_tag
{
		rayLexType_t	tok_val;
		const wchar_t	*str;
		size_t			lineno;
}Token_t;




static const wchar_t *gmr_pattern = 
L"%token{\r\n"
L"	int_const 		: 	270,"
L"	float_const 	: 	271,"
L"	id  			: 	290,"
L"	\"=\"			:	300,"
L"	\"&&\"			:	400,"
L"	\"||\"			:	401,"
L"	\"==\"			:	405,"	
L"	\"!=\"			:	406,"
L"	\"<\"			:	407,"
L"	\">\"			:	408,"
L"	\"<=\"			:	409,"
L"	\">=\"			:	410,"
L"	\"+\"			:	413,"
L"	\"-\"			:	414,"
L"	\"*\"			:	415,"
L"	\"/\"			:	416,"
L"	\"%\"			:	417,"
L"	\"!\"			:	503,"
	
L"	\"[\"			:	600,"
L"	\"]\"			:	601,"
L"	\"(\"			:	602,"
L"	\")\"			:	603,"

L"	\"{\"			:	604,"
L"	\"}\"			:	605,"
L"	\";\"			:	606,"
L"	\",\"			:	610,"

L"	int				:	702,	"
L"	float			:	704,"
L"	bool			:	705,"
L"	char			:	706,"

L"	for				:	800,"
L"	if				:	802,"	
L"	break			:	803,"
L"	do				:	806,"
L"	else			:	807,"
L"	while			:	809,"

L"	true			:	900,"
L"	false			:	901"
L"	;"
L"%}"

L"%prec{"
L"%noassoc 	IF_STMT_PREC;"
L"%noassoc	IF_STMT_ELSE_STMT_PREC;"

L"%}"

L"%rules{"
L"	program 			->		block;"
L"	block			->		\"{\" decls stmts \"}\";"
L"	decls			->		decls decl | .;"
L"	decl			->		type id \";\";"
L"	type			->		type \"[\" int_const \"]\" | int | float | bool | char ; "
L"	stmts			->		stmts stmt | .;"
	

L"	stmt			->		loc \"=\" bool_expr \";\""
L"					|	if \"(\" bool_expr \")\" stmt %prec IF_STMT_PREC"
L"					|	if \"(\" bool_expr \")\" stmt else stmt %prec IF_STMT_ELSE_STMT_PREC"
L"					|	while \"(\" bool_expr \")\" stmt"
L"					|	do stmt while \"(\" bool_expr \")\" \";\""
L"					|	break \";\"	"
L"					|	block;"
					

	
L"	loc			->	loc \"[\" bool_expr \"]\" | id;	"
L"	bool_expr	->	bool_expr \"||\" join | join;"
L"	join 		->	join \"&&\" equality | equality;"
L"	equality	->	equality \"==\" rel | equality \"!=\" rel | rel;"
L"	rel			->	expr \"<\" expr | expr \"<=\" expr | expr \">=\" expr | expr \">\" expr | expr;"
L"	expr		->	expr \"+\" term | expr \"-\" term | term;"
L"	term		->	term \"*\" unary | term \"/\" unary | term \"%\" unary | unary;"
L"	unary		->	\"!\" unary | \"-\" unary | factor;"
L"	factor		->	\"(\" bool_expr\")\" | loc | int_const | float_const | true | false ;"
L"%}"
;

typedef struct __op_tag
{
		Token_t	token;
		Node_t	*l;
		Node_t	*r;
}Op_t;

typedef struct __if_stmt_tag
{
		Node_t	*expr;
		Node_t	*stmt;
}IfStmt_t;



typedef enum
{
		PROGRAM		=		1,
		
		BLOCK,

		DECLS,	DECLS_EPSILON,
		
		DECL,	
		
		TYPE_ARRAY, TYPE_INT, TYPE_FLOAT, TYPE_BOOL,TYPE_CHAR,
		
		STMTS, STMTS_EPSILON, 

		ASSIGN_STMT, IF_STMT, IF_STMT_ELSE_STMT, WHILE_STMT, DO_STMT, BREAK_STMT, BLOCK_STMT,
		

		LOC_INDEX, ID_RULE,
		
		BOOL_OROR_JOIN, JOIN_RULE,
		
		JOIN_ANDAND_EQUAL, EQUAL_RULE,
		
		EQUAL_EQ_REL, EQUAL_NOT_REL, REL_RULE,
		
		REL_EXPR_LE_EXPR, REL_EXPR_LEQ_EXPR, REL_EXPR_GEQ_EXPR, REL_EXPR_GE_EXPR, EXPR_RULE,
		
		EXPR_ADD_EXPR, EXPR_MINUS_EXPR, TERM_RULE,
		
		TERM_MUL_UNARY, TERM_DIV_UNARY, TERM_MOD_UNARY,  UNARY_RULE,
		
		UNARY_NOT_UNARY, UNARY_MINUS_UNARY, FACTOR_RULE,
		
		LP_BOOL_EXPR_RP, LOC_RULE, 
		
		INT_CONST_RULE, 
		
		FLOAT_CONST_RULE, 
		
		TRUE_RULE, 
		
		FALSE_RULE
}RayRuleType_t;
















AR_NAMESPACE_END