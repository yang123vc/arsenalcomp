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


#ifndef __TGU_SYNTAX_TREE_H__
#define __TGU_SYNTAX_TREE_H__


#include "tengu.h"

AR_NAMESPACE_BEGIN


/***************************************************词法信息****************************************************************/



enum{
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
};



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
		TGU_FUNCTION_T
}tguType_t;


typedef enum
{
		TGU_INIT_TABLE_EXPR,
		TGU_INIT_TABLE_FIELD
}tguTableFieldType_t;


struct __tengu_table_field_tag;
typedef struct __tengu_table_field_tag	tguTableField_t;

struct __tengu_table_field_tag
{
		tguTableFieldType_t				type;
		tguTableField_t					*next;
		union{
				tguExpr_t				*expr;
				tguTableField_t			*table;
		}filed;
};

tguTableField_t*		TGU_CreateTableField(tguTableFieldType_t t);
void					TGU_DestroyTableField(tguTableField_t *field);





typedef struct __tengu_table_initializer_tag
{
		bool_t			is_empty_table;
		tguTableField_t	*field_lst;
}tguTableInit_t;

tguTableInit_t*	TGU_CreateTableInitializer(bool_t empty);
void			TGU_DestroyTableInitializer(tguTableInit_t *initializer);


typedef enum
{
		TGU_INIT_EXPR_T,
		TGU_INIT_TABLE_T,
}tguInitializerType_t;


typedef struct  __tengu_initializer_tag
{
		tguInitializerType_t	type;
		union {
				tguTableInit_t	*table;
				tguExpr_t		*expr;
		};
}tguInitializer_t;


tguInitializer_t*		TGU_CreateInitializer(tguInitializerType_t type);
void					TGU_DestroyInitializer(tguInitializer_t *initializer);





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
		
		TGU_OP_NONE		= 0xFFFFFFFF,
}tguExprOP_t;



typedef struct __tengu_index_expression_tag
{
		tguExpr_t		*expr;
		tguExpr_t		*index_expr;
}tguIndexExpr_t;


typedef struct __tengu_binary_expression_tag
{
		tguExprOP_t		op;
		tguExpr_t		*left;
		tguExpr_t		*right;
}tguBinaryExpr_t;


typedef struct __tengu_unary_expression_tag	
{
		tguExprOP_t		op;
		tguExpr_t		*expr;
}tguUnaryExpr_t;

typedef struct __tengu_condition_expression_tag	
{
		tguExpr_t		*cond;
		tguExpr_t		*if_true;
		tguExpr_t		*if_false;
}tguCondExpr_t;


typedef struct __tengu_constant_tag
{
		tguLexInfo_t			lex_info;
		union {
				int_64_t		int_number;
				double			float_number;
				const wchar_t	*string;
		};
}tguConstant_t;




typedef struct __tengu_function_call_expression_tag
{
		tguExpr_t		*func_call;
		tguExpr_t		*arg_list;
}tguFuncCallExpr_t;



typedef enum 
{
		TGU_ET_NULL_CONST,
		TGU_ET_INT_CONST,
		TGU_ET_FLOAT_CONST,
		TGU_ET_STRING_CONST,
		TGU_ET_VAR,
		TGU_ET_INDEX,
		TGU_ET_FUNC_CALL,
		TGU_ET_BINARY,
		TGU_ET_UNARY,
		TGU_ET_CONDITIONAL,
		
		TGU_ET_UNDEF_NAME
}tguExprType_t;


struct __tengu_expression_tag 
{
		tguExprType_t			expr_type;
		bool_t					is_lvalue;
		bool_t					is_constant;
		
		tguLexInfo_t			lex_info;
		tguExpr_t				*next;
		union{
				const	wchar_t			*name;			/*TGU_ET_UNDEF_NAME*/
				tguVar_t				*var;			/*TGU_ET_VAR*/
				tguConstant_t			constant;		/*TGU_ET_NULL_CONST, TGU_ET_INT_CONST,	TGU_ET_FLOAT_CONST,	TGU_ET_STRING_CONST*/
				
				tguIndexExpr_t			index_expr;		/*TGU_ET_INDEX*/
				tguCondExpr_t			cond_expr;		/*TGU_ET_CONDITIONAL*/
				tguUnaryExpr_t			unary_expr;		/*TGU_ET_UNARY*/
				tguBinaryExpr_t			binary_expr;	/*TGU_ET_BINARY*/
				tguFuncCallExpr_t		func_call_expr;	/*TGU_ET_FUNC_CALL*/
		};
};

tguExpr_t*		TGU_CreateExpr(tguExprType_t type);
void			TGU_DestroyExpr(tguExpr_t *expr);


/*********************************************语句**************************************************************/

typedef enum 
{
		TGU_STT_EMPTY,
		TGU_STT_CONTINUE,
		TGU_STT_BREAK,
		
		TGU_STT_EXPR,
		TGU_STT_COMPOUND,
		TGU_STT_IF,
		TGU_STT_WHILE,
		TGU_STT_DO,
		TGU_STT_FOR,
		TGU_STT_RETURN
}tguStmtType_t;



typedef struct __tengu_compound_statement_tag
{
		tguBlock_t		*block;
}tguCompoundStmt_t;


typedef struct __tengu_if_statement_tag		
{
		tguExpr_t		*expr;
		tguStmt_t		*true_part;
		tguStmt_t		*false_part;
}tguIFStmt_t;

typedef struct __tengu_while_statement_tag	
{
		tguExpr_t		*expr;
		tguStmt_t		*loop_part;
}tguWhileStmt_t;


typedef struct __tengu_for_statement_tag
{
		tguExpr_t		*init_expr;
		tguExpr_t		*cond_expr;
		tguExpr_t		*step_expr;
		tguStmt_t		*loop_part;
}tguForStmt_t;


typedef struct __tengu_return_statement_tag
{
		tguExpr_t		*expr;
}tguRetrunStmt_t;




struct __tengu_statement_tag
{
		tguStmtType_t			stmt_type;		/*TGU_STT_EMPTY, TGU_STT_CONTINUE,	TGU_STT_BREAK 这三种类型的语句无附加参数*/
		
		tguLexInfo_t			lex_info;
		
		union{
				tguExpr_t				*expr;			/*TGU_STT_EXPR*/
				tguCompoundStmt_t		compound_stmt;	/*TGU_STT_COMPOUND*/
				tguIFStmt_t				if_stmt;		/*TGU_STT_IF*/
				tguWhileStmt_t			while_stmt;		/*TGU_STT_WHILE, TGU_STT_DO*/
				tguForStmt_t			for_stmt;		/*TGU_STT_FOR*/
				tguRetrunStmt_t			return_stmt;	/*TGU_STT_RETURN*/

		};
};



tguStmt_t*	TGU_CreateStmt(tguStmtType_t			stmt_type);
void		TGU_DestroyStmt(tguStmt_t *stmt);



/*********************************************函数**************************************************************/




struct __tengu_function_tag;
typedef struct __tengu_function_tag		tguFunc_t;

struct __tengu_function_tag
{
		const wchar_t	*name;
		tguLexInfo_t	lex_info;

		wchar_t			**params;
		size_t			param_cnt;
		bool_t			is_variadic_param;
		
		tguBlock_t		*block;
};

tguFunc_t*		TGU_CreateFunction(const wchar_t *name, const tguBlock_t *parent_block);
void			TGU_DestroyFunction(tguFunc_t *func);
/*返回此参数位置索引*/
size_t			TGU_InsertParam(tguFunc_t *func, const wchar_t *param_name);



/****************************************************变量***************************************************/

struct __tengu_var_tag
{
		const wchar_t			*name;
		tguLexInfo_t			lex_info;
		size_t					ref_cnt;
		tguInitializer_t		*initializer;

		tguVar_t				*next;
};



tguVar_t*		TGU_CreateVar(const wchar_t *name, tguInitializer_t	*initializer);
void			TGU_DestroyVar(tguVar_t	*var);
tguVar_t*		TGU_CopyVar(tguVar_t	*var);



/************************************************符号管理************************************************************************/

typedef enum 
{
		TGU_SYMB_VAR_T,
		TGU_SYMB_FUNC_T
}tguSymbType_t;


struct __tengu_symb_table_tag;
typedef struct __tengu_symb_table_tag	tguSymb_t;

struct __tengu_symb_table_tag
{
		tguSymbType_t			type;
		const	wchar_t*		name;
		tguSymb_t				*next;

		union{
				tguVar_t		*var;
				tguFunc_t		*func;
		};
};


tguSymb_t*		TGU_CreateSymb(tguSymbType_t	type, const wchar_t *name);
void			TGU_DestroySymb(tguSymb_t *symb);



#define	TENGU_SYMB_BUCKET_SIZE	33

typedef struct __tengu_symbol_table_tag
{
		tguSymb_t		*tbl[TENGU_SYMB_BUCKET_SIZE];
		size_t			item_cnt;
}tguSymbTbl_t;

tguSymbTbl_t*	TGU_CreateSymbTable();
void			TGU_DestroySymbTable(tguSymbTbl_t *tbl);
tguSymb_t*		TGU_FindSymb(tguSymbTbl_t *tbl, const wchar_t *name);

tguSymb_t*		TGU_InsertVarToSymbTable(tguSymbTbl_t *tbl, const wchar_t *name, tguVar_t		*var);
tguSymb_t*		TGU_InsertFuncToSymbTable(tguSymbTbl_t *tbl, const wchar_t *name, tguFunc_t		*func);






/*************************************************************基本块****************************************************************************/




struct __tengu_block_tag
{
		tguLexInfo_t	begin;
		tguLexInfo_t	end;

		tguSymbTbl_t	*symb_tbl;
		
		tguStmt_t		**stmts;
		size_t			stmt_cnt;
		size_t			stmt_cap;
		
		tguBlock_t		*next;/*同一级的下一个块*/
		tguBlock_t		*parent;
		
		/*
		A:
		{
				B:
				{

				};

				C:
				{

				};
				则Block A ->sub_blocks ==		
				
				B->next == B;
				C->next == NULL;

				B->parent == C->parent = A;



		}
		*/
		tguBlock_t		*sub_blocks; 
};


tguBlock_t*		TGU_CreateBlock(const tguBlock_t	*parent);
/*
TGU_DestroyBlock会将block从block->parent中的sub_blocks中剔除，之后销毁此block及其所有变量声明
*/

void			TGU_DestroyBlock(tguBlock_t	*block);

tguSymb_t*		TGU_FindFromBlockByName(tguBlock_t	*block, const wchar_t *name, bool_t current_block);

void			TGU_InsertVarToBlock(tguBlock_t	*block, tguVar_t*	var);
void			TGU_InsertStmtToBlock(tguBlock_t	*block, tguStmt_t	*stmt);

void			TGU_InsertSubBlockToBlock(tguBlock_t	*block, tguBlock_t	*sub);
bool_t			TGU_RemoveSubBlockFromBlock(tguBlock_t	*block, tguBlock_t	*sub);





/****************************************************ABS***************************************/

typedef struct __tengu_abstract_syntax_tree_tag
{
		tguBlock_t		*global_block;
		bool_t			has_error;
}tguSyntaxTree_t;


tguSyntaxTree_t*		TGU_CreateSyntaxTree(const tguBlock_t *build_in_block);
void					TGU_DestroySyntaxTree(tguSyntaxTree_t	*tree);





AR_NAMESPACE_END

#endif
