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


#ifndef __TGU_PARSER_H__
#define __TGU_PARSER_H__


#include "tengu.h"

AR_NAMESPACE_BEGIN

/***************************************************词法信息****************************************************************/

typedef	struct __tengu_lexinfo_tag
{
		size_t	linenum;
		size_t	col;
}tguLexInfo_t;


typedef struct __tengu_token_tag
{
		const	wchar_t	*token;
		size_t			term_val;
		tguLexInfo_t	lex_info;
}tguToken_t;


/***************************************************前向声明****************************************************************/



struct __tengu_var_tag;
typedef struct __tengu_var_tag tguVar_t;


struct __tengu_block_tag;
typedef struct __tengu_block_tag		tguBlock_t;


struct __tengu_expression_tag;
typedef struct __tengu_expression_tag	tguExpr_t;



struct __tengu_statement_tag;
typedef struct __tengu_statement_tag	tguStmt_t;






/*********************************************声明**************************************************************/

typedef enum 
{
		TGU_NULL_T,
		TGU_INTEGER_T,
		TGU_FLOAT_T,
		TGU_STRING_T,
		TGU_BOOLEAN_T,
		TGU_TABLE_T,
		TGU_FUNCTION_T,
		TGU_USERDATA_T
}tguType_t;


struct __tengu_initializer_table_tag;
typedef struct __tengu_initializer_table_tag	tguInitTable_t;

typedef enum
{
		TGU_INIT_TABLE_EXPR,
		TGU_INIT_TABLE_TABLE
}tguInitTableType_t;

struct __tengu_initializer_table_tag
{
		tguInitTableType_t		type;
		tguInitTable_t			*next;
		union{
				tguExpr_t		*expr;
				tguInitTable_t	*table;
		}filed;
		
};

struct  __tengu_initializer_tag;
typedef struct __tengu_initializer_tag		tguInitializer_t;

struct  __tengu_initializer_tag
{
		tguType_t				value_type;
		union {
				int_64_t		int_number;
				double			float_number;
				const wchar_t	*string;
				tguInitTable_t	*table;
				tguVar_t		*var;
				tguExpr_t		*expr;
		};
};


struct __tengu_declaration_tag;
typedef struct __tengu_declaration_tag	tguDeclration_t;
struct __tengu_declration_tag
{
		tguLexInfo_t			lex_info;
		tguType_t				decl_type;
		const	wchar_t			*name;
		tguInitializer_t		*initializer;
		tguDeclration_t			*next;
};


/*********************************************表达式**************************************************************/

typedef enum
{
		TGU_OP_ASSIGN, 

		TGU_OP_LOGICAL_OR,		
		TGU_OP_LOGICAL_AND, 
		TGU_OP_LOGICAL_NOT,
		
		TGU_OP_IS_EQUAL, 
		TGU_OP_NOT_EQUAL,
		TGU_OP_LESS, 
		TGU_OP_GREATER, 
		TGU_OP_LESS_OR_EQUAL, 
		TGU_OP_GREATER_OR_EQUAL,
		
		TGU_OP_PLUS, 
		TGU_OP_MINUS, 
		TGU_OP_MUL, 
		TGU_OP_DIV, 
		TGU_OP_MOD,

		TGU_OP_UNARY_PLUS, 
		TGU_OP_UNARY_MINUS,

		TGU_OP_PREINC, 
		TGU_OP_PREDEC, 
		TGU_OP_POSTINC, 
		TGU_OP_POSTDEC,
		
		/*
		TGU_OP_CVT_TO_INTEGER, 
		TGU_OP_CVT_TO_FLOAT,
		TGU_OP_CVT_TO_BOOLEAN,
		TGU_OP_CVT_TO_NULL,
		*/
}tguExprOp_t;


typedef enum 
{

		TGU_ET_NULL_CONST,
		TGU_ET_INT_CONST,
		TGU_ET_FLOAT_CONST,
		TGU_ET_STRING_CONST,
		TGU_ET_NAME,
		TGU_ET_VAR,
		TGU_ET_FUNC_CALL,
		TGU_ET_BINARY,
		TGU_ET_UNARY,
		TGU_ET_CONDITIONAL,
}tguExprType_t;





struct __tengu_binary_expression_tag;
typedef struct __tengu_binary_expression_tag	tguBinaryExpr_t;
struct __tengu_binary_expression_tag
{
		tguExprOp_t		op;
		tguExpr_t		*left;
		tguExpr_t		*right;
};


struct __tengu_unary_expression_tag;
typedef struct __tengu_unary_expression_tag		tguUnaryExpr_t;
struct __tengu_unary_expression_tag
{
		tguExprOp_t		op;
		tguExpr_t		*expr;
};

struct __tengu_condition_expression_tag;
typedef struct __tengu_condition_expression_tag	tguCondExpr_t;
struct __tengu_condition_expression_tag
{
		tguExpr_t		*cond;
		tguExpr_t		*if_true;
		tguExpr_t		*if_false;
};


struct __tengu_constant_tag;
typedef struct __tengu_constant_tag		tguConstant_t;
struct __tengu_constant_tag
{
		tguLexInfo_t			lex_info;
		union {
				int_64_t		int_number;
				double			float_number;
				const wchar_t	*string;
		};
};


struct __tengu_function_call_expression_tag;
typedef struct __tengu_function_call_expression_tag		tguFuncCallExpr_t;
struct __tengu_function_call_expression_tag
{
		tguExpr_t		*func_call;
		tguExpr_t		*arg_list;
};


struct __tengu_expression_tag 
{
		tguExprType_t			expr_type;
		tguLexInfo_t			lex_info;
		bool_t					is_lvalue;
		bool_t					is_constant;
		tguExpr_t				*next;
		union{
				tguVar_t				*var;
				const	wchar_t			*name;
				tguConstant_t			constant;
				tguCondExpr_t			cond_expr;
				tguUnaryExpr_t			unary_expr;
				tguBinaryExpr_t			binary_expr;
				tguFuncCallExpr_t		func_call_expr;
		};
};


/*********************************************语句**************************************************************/

typedef enum 
{
		TGU_STT_EXPR,
		TGU_STT_COMPOUND,
		TGU_STT_IF,
		TGU_STT_WHILE,
		TGU_STT_DO,
		TGU_STT_CONTINUE,
		TGU_STT_BREAK,
		TGU_STT_RETURN
}tguStmtType_t;



struct __tengu_compound_statement_tag;
typedef struct __tengu_compound_statement_tag	tguCompoundStmt_t;
struct __tengu_compound_statement_tag
{
		tguBlock_t		*block;
		tguStmt_t		*statement_list;
};

struct __tengu_if_statement_tag;
typedef struct __tengu_if_statement_tag		tguIFStmt_t;
struct __tengu_if_statement_tag
{
		tguExpr_t		*expr;
		tguStmt_t		*true_part;
		tguStmt_t		*false_part;
};

struct __tengu_while_statement_tag;
typedef struct __tengu_while_statement_tag		tguWhileStmt_t;
struct __tengu_while_statement_tag
{
		tguExpr_t		*expr;
		tguStmt_t		*loop_part;
};

struct __tengu_return_statement_tag;
typedef struct __tengu_return_statement_tag		tguRetrunStmt_t;
struct __tengu_return_statement_tag
{
		tguExpr_t		*expr;
};


struct __tengu_statement_tag
{
		tguStmtType_t			stmt_type;
		tguLexInfo_t			lex_info;
		tguStmt_t				*next;
		union{
				tguExpr_t				*expr;
				
				tguCompoundStmt_t		compound_stmt;
				tguIFStmt_t				if_stmt;
				tguWhileStmt_t			while_stmt;
				tguRetrunStmt_t			return_stmt;

		};
};



/*********************************************函数**************************************************************/

struct	__tengu_function_tag;
typedef struct __tengu_function_tag		tguFunc_t;

struct __tengu_function_tag
{
		tguLexInfo_t	lex_info;
		const wchar_t	*name;

		tguVar_t		*params;
		size_t			param_cnt;
		bool_t			is_variadic_param;
		
		tguBlock_t		*block;
		tguStmt_t		*statement;
		
		tguFunc_t		*next;
};



/************************************************符号管理************************************************************************/

struct __tengu_var_tag
{
		tguLexInfo_t			lex_info;
		tguType_t				type;
		const wchar_t			*name;
		tguInitializer_t		*initializer;
		tguVar_t				*next;
};


struct __tengu_block_tag
{
		tguVar_t		*vars;
		tguBlock_t		*next;/*同一级的下一个块*/
		tguBlock_t		*sub_blocks;
		tguBlock_t		*parent;
};

/***************************************************************分析器**************************************************************************/


typedef enum
{
		TGU_NODE_TOKEN_T,
}tguSynNodeType_t;



typedef struct __tengu_syntax_node_tag
{
		tguSynNodeType_t		type;
		
		union{
				tguToken_t		token;
		};
}tguSynNode_t;

typedef struct __tengu_parser_tag
{
		tguBlock_t		*global_block;
		tguBlock_t		*top_block;
		
		tguFunc_t		*function_list;

}tguParser_t;



void	TGU_InitParser();
void	TGU_UnInitParser();







AR_NAMESPACE_END




#endif


