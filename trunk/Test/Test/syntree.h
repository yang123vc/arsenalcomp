#pragma once
#include "tiny_parser.h"




enum LEX_TYPE{
		INT_CONST			=  			270,
		FLOAT_CONST			=			271,
		ID					=			290,
		ASSIGN_OP			=			300,
		AND_OP				=			400,
		OR_OP				=			401,
		EQUAL_OP			=			405,
		NOTEQUAL_OP			=			406,
		LESS_OP				=			407,
		GREATER_OP			=			408,
		LE_OP				=			409,
		GE_OP				=			410,
		ADD_OP				=			413,
		MINUS_OP			=			414,
		MUL_OP				=			415,
		DIV_OP				=			416,
		MOD_OP				=			417,
		NOT_OP				=			503,
		SQUARE_OPEN			=			600,
		SQUARE_CLOSE		=			601,
		PAREN_OPEN			=			602,
		PAREN_CLOSE			=			603,
		BRACE_OPEN			=			604,
		BRACE_CLOSE			=			605,
		SEMICOLON			=			606,
		DOT					=			610,
		INT_KWORD			=			702,
		FLOAT_KWORD			=			704,
		BOOL_KWORD			=			705,
		CHAR_KWORD			=			706,
		FOR_KWORD			=			800,
		IF_KWORD			=			802,
		BREAK_KWORD			=			803,
		DO_KWORD			=			806,
		ELSE_KWORD			=			807,
		WHILE_KWORD			=			809,		
		TRUE_KWORD			=			900,
		FALSE_KWORD			=			901
};



enum GRAMMAR_TYPE
{
		PROGRAM = 1,
		BLOCK = 2,
		DECLS_DECL = 3,
		DECLS_EPSILON = 4,
		DECL = 5,
		TYPE_ARRAY = 6,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_BOOL,
		TYPE_CHAR,
		STMTS_STMT,
		STMTS_EPSILON,
		STMT_LOC,
		STMT_IF,
		STMT_IF_ELSE,
		STMT_WHILE,
		STMT_DO_WHILE,
		STMT_BREAK,
		STMT_BLOCK,
		LOC_INDEX,
		LOC_ID,
		BOOL_EXPR_OR_JOIN,
		BOOL_EXPR_JOIN,
		JOIN_AND_EQUAL,
		JOIN_EQUAL,
		EQUAL_EQ_REL,
		EQUAL_NOT_EQ_REL,
		EQUAL_REL,
		REL_EXPR_LESS_EXPR,
		REL_EXPR_LE_EXPR,
		REL_EXPR_GREAT_EXPR,
		REL_EXPR_GE_EXPR,
		REL_EXPR,
		EXPR_EXPR_ADD_TERM,
		EXPR_EXPR_MINUS_TERM,
		EXPR_TERM,
		TERM_TERM_MUL_UNARY,
		TERM_TERM_DIV_UNARY,
		TERM_TERM_MOD_UNARY,
		TERM_UNARY,
		UNARY_NOT_UNARY,
		UNARY_MINUS_UNARY,
		UNARY_FACTOR,
		FACTOR_BO_BOOL_EXPR_BC,
		FACTOR_LOC,
		FACTOR_INT_CONST,
		FACTOR_FLOAT_CONST,
		FACTOR_TRUE,
		FACTOR_FALSE
};


enum {
		TOKEN_T,
		STMT_T,
		
		
};


#if(0)
%rules{
	program 			->		block;
	block			->		"{" decls stmts "}";
	decls			->		decls decl | .;
	decl			->		type id ";";
	type			->		type "[" int_const "]" | int | float | bool | char;  
	stmts			->		stmts stmt | .;
	

	stmt			->		loc "=" bool_expr ";"
					|	if "(" bool_expr ")" stmt %prec IF_STMT_PREC
					|	if "(" bool_expr ")" stmt else stmt %prec IF_STMT_ELSE_STMT_PREC
					|	while "(" bool_expr ")" stmt
					|	do stmt while "(" bool_expr ")" ";"
					|	break ";"	
					|	block
					;

	
	loc			->	loc "[" bool_expr "]" | id;	
	bool_expr			->	bool_expr "||" join | join;
	join 			->	join "&&" equality | equality;
	equality			->	equality "==" rel | equality "!=" rel | rel;
	rel			->	expr "<" expr | expr "<=" expr | expr ">=" expr | expr ">" expr | expr;
	expr			->	expr "+" term | expr "-" term | term;
	term			->	term "*" unary | term "/" unary | term "%" unary | unary;
	unary			->	"!" unary | "-" unary | factor;
	factor			->	"(" bool_expr")" | loc | int_const | float_const | true | false ;
%}
#endif



class Token : public Node
{
protected:
		std::wstring	m_name;
		size_t			m_val;
		size_t			m_lineno;

public:
		Token(const AR_Token_t *tok) : Node(TOKEN_T)
		{
				assert(tok != NULL);
				for(size_t i = 0; i < tok->count; ++i)
				{
						m_name.push_back(tok->str[i]);
				}
				m_val = tok->value;
				m_lineno = tok->lineno;
		}

		virtual ~Token()
		{

		}
public:
		virtual const std::wstring& ToString()const
		{
				return m_name;
		}

		LEX_TYPE GetTokenValue()const
		{
				return (LEX_TYPE)m_val;
		}
};






class NodeBuilderTest : public NodeBuilder
{
public:
		virtual Node*	BuildLeaf(const AR_Token_t *tok)
		{
				assert(tok != NULL);
				return new Token(tok);
		}

		virtual Node*	BuildNode(size_t rule_id, const std::wstring &rule_name, std::vector<Node*> &nodes)
		{
				wprintf(L"name == %s\r\n", rule_name.c_str());
				return (Node*)1;
		}

		virtual void	FreeNode(Node *node)
		{

		}

		virtual ~NodeBuilderTest()
		{

		}
};
