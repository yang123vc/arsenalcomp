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

#include "ray.h"

AR_NAMESPACE_BEGIN



typedef enum
{
		TOK_DELIM_ID = 257,				/*过滤掉的空白或注释*/
		TOK_TYPE_ID = 258,				/*typedef id*/
		TOK_DONE_ID = 259,				/*遇到EOI词法分析器会将此符号add到parser中*/
		TOK_CHAR_CONSTANT = 260,		/**/
		TOK_STRING_LITERAL = 261,		/**/
		TOK_FLOAT_CONSTANT = 262,		/**/
		TOK_HEX_CONSTANT = 263,		/**/
		TOK_OCT_CONSTANT = 264,		/**/
		TOK_DEC_CONSTANT = 265,		/**/
		TOK_IDENTIFIER = 266,		/**/
		TOK_SWITCH = 267,		/**/
		TOK_FOR = 268,		/**/
		TOK_GOTO = 269,		/**/
		TOK_RETURN = 270,		/**/
		TOK_DO = 271,		/**/
		TOK_WHILE = 272,		/**/
		TOK_IF = 273,		/**/
		TOK_ELSE = 274,		/**/
		TOK_CONTINUE = 275,		/**/
		TOK_DEFAULT = 276,		/**/
		TOK_CASE = 277,		/**/
		TOK_BREAK = 278,		/**/
		TOK_CONST = 279,		/**/
		TOK_VOLATILE = 280,		/**/
		TOK_STRUCT = 281,		/**/
		TOK_UNION = 282,		/**/
		TOK_TYPEDEF = 283,		/**/
		TOK_STATIC = 284,		/**/
		TOK_SIZEOF = 285,		/**/
		TOK_VOID = 286,		/**/
		TOK_BYTE = 287,		/**/
		TOK_CHAR = 288,		/**/
		TOK_SHORT = 289,		/**/
		TOK_INT = 290,		/**/
		TOK_LONG = 291,		/**/
		TOK_SIGNED = 292,		/**/
		TOK_UNSIGNED = 293,		/**/
		TOK_FLOAT = 294,		/**/
		TOK_DOUBLE = 295,		/**/
		TOK_IMPORT = 296,		/**/
		TOK_EXPORT = 297,		/**/
		TOK_ATTRIBUTE = 298,		/**/
		TOK_RSHIFT_ASSIGN = 299,		/**/
		TOK_LSHIFT_ASSIGN = 300,		/**/
		TOK_ADD_ASSIGN = 301,		/**/
		TOK_SUB_ASSIGN = 302,		/**/
		TOK_MUL_ASSIGN = 303,		/**/
		TOK_DIV_ASSIGN = 304,		/**/
		TOK_MOD_ASSIGN = 305,		/**/
		TOK_AND_ASSIGN = 306,		/**/
		TOK_XOR_ASSIGN = 307,		/**/
		TOK_OR_ASSIGN = 308,		/**/
		TOK_RSHIFT = 309,			/**/
		TOK_LSHIFT = 310,			/**/
		TOK_INC = 311,				/**/		
		TOK_DEC = 312,				/**/
		TOK_PTR = 313,				/**/
		TOK_ANDAND = 314,			/*&&*/
		TOK_OROR = 315,				/*||*/
		TOK_LE = 316,				/*<=*/
		TOK_GE = 317,				/*>=*/
		TOK_EQ = 318,				/*==*/
		TOK_NE = 319,				/*!=*/
		TOK_LESS = 320,				/*<*/
		TOK_GREATER = 321,			/*>*/
		TOK_L_BRACES = 322,			/*{*/
		TOK_R_BRACES = 323,			/*}*/
		TOK_L_PAREN = 324,			/*(*/
		TOK_R_PAREN = 325,			/*)*/
		TOK_L_SQUARE = 326,			/*[*/
		TOK_R_SQUARE = 327,			/*]*/
		TOK_SEMICOLON = 328,		/*;*/
		TOK_COMMA = 329,			/*,*/
		TOK_COLON = 330,			/*:*/
		TOK_ASSIGN = 331,			/*=*/
		TOK_DOT = 332,				/*.*/
		TOK_AND = 333,				/*&*/
		TOK_NOT = 334,				/*!*/
		TOK_TILDE = 335,			/*~*/
		TOK_ADD = 336,				/*+*/
		TOK_SUB = 337,				/*-*/
		TOK_MUL = 338,				/* * */
		TOK_DIV = 339,				/* / */
		TOK_MOD = 340,				/*%*/
		TOK_XOR = 341,				/*^*/
		TOK_OR = 342,				/*|*/
		TOK_QUEST = 343,			/*?*/
}rayTokType_t;



/******************************************************************************/



typedef enum 
{
		TY_NOTYPE,

		DT_PTR_TO = 0,		/* 指针 */
		DT_FUNC_TYPE,		/*函数签名 */
		DT_ARRAY_OF,	 	/*数组*/

		TY_UNSIGNED,
		TY_SIGNED,

		TY_STRUCT,			/*结构*/
		TY_UNDEF_STRUCT,	/*未定义的结构*/
		TY_UNION,			/*联合*/
		TY_UNDEF_UNION,		/*未定义联合*/

		/* 基本类型*/
		TY_VOID,	
		TY_BYTE,
		TY_CHAR,
		TY_UINT_8,
		TY_INT_8,
		TY_UINT_16,
		TY_INT_16,
		TY_UINT_32,
		TY_INT_32,
		TY_UINT_64,
		TY_INT_64,
		TY_FP_32,
		TY_FP_64,
}rayTypeCode_t;



#define TF_VOID			0x0001
#define TF_BYTE			0x0002
#define TF_CHAR			0x0004
#define TF_SHORT		0x0008
#define TF_INT			0x0010
#define TF_LONG			0x0020
#define TF_FLOAT		0x0040
#define TF_DOUBLE		0x0080
#define TF_SIGNED		0x0100
#define TF_UNSIGNED		0x0200
#define TF_STRUCT		0x0400
#define TF_UNION		0x0800


/*********************************************************************************************************************/

struct __ray_type_tag;
typedef	struct __ray_type_tag	rayType_t;




struct	__var_tag;
typedef struct __var_tag		rayVar_t;


struct __aggregate_tag;
typedef struct __aggregate_tag		rayAggregate_t;


struct __aggregate_tag
{
		const wchar_t		*tag_name;
		bool_t				is_union;
		size_t				size;
		size_t				alignment;

		rayVar_t			*members;
		
		raySrcInfo_t		src;
};

/*********************************************************************************/


struct	__initializer_tag;
typedef struct	__initializer_tag		rayInitializer_t;


struct	__statement_tag;
typedef struct __statement_tag	rayStatement_t;

/*********************************************************************************/

typedef struct __array_tag 
{
		rayType_t	*type;
		size_t		size;
		bool_t		is_completed;
}rayArray_t;




typedef struct __func_type_tag
{
		rayType_t			*ret_type;
		
		bool_t				has_id_list;
		rayVar_t			*params;
		
		raySrcInfo_t		src;
}rayFuncType_t;




#define QU_CONST				0x0001
#define QU_VOLATILE				0x0002


typedef struct __pointer_tag
{
		rayType_t			*pointer_to;
}rayPointer_t;




struct __ray_type_tag
{
		rayTypeCode_t		code;		
		size_t				size;
		size_t				qualifiers;

		union{
				rayPointer_t		*pointer;			
				rayArray_t			*arr;				
				rayAggregate_t		*aggregate;			
				rayFuncType_t		*func_type;			
				const wchar_t		*name;
		};
};




typedef enum 
{
		CL_NOCLASS,	
		CL_LOCAL,
		CL_STATIC,	
		CL_LOCAL_STATIC,
		
		CL_ARGUMENT,
		CL_MEMBER_OF_STRUCT,
		CL_TYPEDEF
}rayStorageClass_t;


struct __initializer_tag;
typedef struct __initializer_tag		rayInitializer_t;

struct	__var_tag 
{

		const wchar_t		*name;	
		rayStorageClass_t	storage;	
		rayType_t			*type;	
		uint_64_t			addr;

		rayInitializer_t	*init;

		rayVar_t			*next;

		raySrcInfo_t		src;
};



/*********************************************block**********************************************************/

typedef struct __typedef_tag
{
		const wchar_t	*name;
		rayType_t		*type;	
		raySrcInfo_t	src;
}rayTypedef_t;

struct __block_tag;
typedef struct __block_tag		rayBlock_t;

struct __block_tag
{
		rayVar_t			**vars;		
		size_t				var_cnt;

		rayTypedef_t		**typedefs;		
		size_t				typedef_cnt;

		rayInitializer_t	**initializer;	
		size_t				init_cnt;

		rayType_t			**structs;
		size_t				struct_cnt;

		rayBlock_t			*next;
		rayBlock_t			*subblocks;
		rayBlock_t			*parent;
};


rayBlock_t*		RAY_CreateBlock(rayBlock_t		*parent);
void			RAY_DestroyBlock(rayBlock_t		*block);






typedef struct __func_tag		
{
		const char			*name;		
		rayType_t			*func_type;
		rayStatement_t		*statement;
		raySrcInfo_t		src;
}rayFunc_t;


/*********************************************expression**********************************************************/


typedef enum 
{
		OP_ASSIGN, OP_MUL_ASSIGN, OP_DIV_ASSIGN, OP_MOD_ASSIGN,	OP_PLUS_ASSIGN,  OP_MINUS_ASSIGN, OP_LSHIFT_ASSIGN, OP_RSHIFT_ASSIGN, OP_BITWISE_AND_ASSIGN, OP_BITWISE_XOR_ASSIGN, OP_BITWISE_OR_ASSIGN,

		OP_LOGICAL_OR, OP_LOGICAL_AND, OP_LOGICAL_NOT,

		OP_BITWISE_OR, OP_BITWISE_XOR, OP_BITWISE_AND, OP_BITWISE_NOT,

		OP_IS_EQUAL, OP_NOT_EQUAL,

		OP_LESS, OP_GREATER, OP_LESS_OR_EQUAL, OP_GREATER_OR_EQUAL,

		OP_LSHIFT, OP_RSHIFT,

		OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_MOD,

		OP_PREINC, OP_PREDEC, OP_POSTINC, OP_POSTDEC,

		OP_ADDRESS_OF, OP_DEREF,

		OP_UNARY_PLUS, OP_UNARY_MINUS,

		OP_COMMA,

		OP_CVT_TO_BYTE,
		OP_CVT_TO_CHAR, 
		OP_CVT_TO_UINT_8,
		OP_CVT_TO_INT_8,
		OP_CVT_TO_UINT_16,
		OP_CVT_TO_INT_16,
		OP_CVT_TO_UINT_32,
		OP_CVT_TO_INT_32,
		OP_CVT_TO_UINT_64,
		OP_CVT_TO_INT_64,
		OP_CVT_TO_FP_32,
		OP_CVT_TO_FP_64,
		OP_CVT_TO_PTR, 
		OP_CVT_TO_VOID
}rayOPType_t;




struct	__expr_tag;
typedef	struct	__expr_tag		rayExpr_t;

typedef struct __expr_list_tag
{
		rayExpr_t		**expr;
		size_t			cnt;
		size_t			cap;
}rayExprList_t;


typedef struct __func_call_expr_tag
{
		rayExpr_t			*func_expr;
		rayExprList_t		*expr_list;
}rayCallExpr_t;


typedef struct __dot_expr_tag 
{
		rayExpr_t	*struct_expr;
		rayVar_t	*member;
}rayDotExpr_t;


typedef struct __scale_expr_tag
{
		rayOPType_t		op;
		rayExpr_t		*expr;
		size_t			factor;
}rayScaleExpr_t;

typedef struct __binary_expr_tag
{
		rayOPType_t		op;
		rayExpr_t		*left;
		rayExpr_t		*right;
}rayBinaryExpr_t;

typedef struct __unary_expr_tag
{
		rayOPType_t		op;
		rayExpr_t		*expr;
}rayUnaryExpr_t;


typedef struct __cond_expr_tag
{
		rayExpr_t		*cond;
		rayExpr_t		*if_true;
		rayExpr_t		*if_false;
}rayCondExpr_t;


typedef struct __size_of_expr_tag
{
		rayExpr_t		*expr;
		rayType_t		*type;
		size_t			size;
}raySizeOfExpr_t;



typedef union __constant_value_tag
{
		byte_t			byte_cons;
		wchar_t			char_cons;
		uint_8_t		uint8_cons;
		int_8_t			int8_cons;
		uint_16_t		uint16_cons;
		int_16_t		int16_cons;
		uint_32_t		uint32_cons;
		int_32_t		int32_cons;
		uint_64_t		uint64_cons;
		int_64_t		int64_cons;
		float			float_cons;
		double			double_cons;
		const wchar_t	*string_cons;
}rayConstValue_t;

typedef struct __constant_tag
{
		bool_t			is_unsigned;
		rayConstValue_t	val;
		raySrcInfo_t	src;
}rayConstant_t;



typedef enum 
{
		ET_CHAR_CONST,
		ET_INT_CONST,
		ET_FLOATING_CONST,
		ET_STRING_CONST,

		ET_VAR,
		ET_FUNCNAME,
		ET_UNDEF_VAR,
		ET_FUNC_CALL,
		ET_DOT,

		ET_BINARY,
		ET_UNARY,
		ET_PROMOTION,
		ET_CAST,
		ET_ASSIGNMENT_CONVERSION,
		ET_SCALE,
		ET_SIZEOF,
		ET_CONDITIONAL
}rayExprType_t;


struct	__expr_tag
{
		rayExprType_t	ex_exprtype;
		rayType_t		*ex_type;
		raySrcInfo_t	src;
		bool_t			ex_is_lvalue;
		bool_t			ex_is_constant;

		union {
				rayCallExpr_t	*call_expr;
				rayDotExpr_t	*dot_expr;
				rayBinaryExpr_t	*binary_expr;
				rayUnaryExpr_t	*unary_expr;
				rayScaleExpr_t	*scale_expr;
				rayCondExpr_t	*cond_expr;
				raySizeOfExpr_t	*sizeof_expr;
				rayVar_t		*var;
				rayConstant_t	constant;

				const wchar_t	*undef_name;
		};

};










/********************************************declaration*********************************************************/

typedef enum
{
		IT_INT_VAL,
		IT_FLOAT_VAL,

		IT_VAR_ADDR,
		IT_STRING_ADDR,	
		IT_FUNC_ADDR,

		IT_STRING_ARRAY,
		IT_EXPR,

		IT_LIST

}rayInitializerType_t;


struct __initializer_tag
{
		rayInitializerType_t	init_type;

		union{
				rayConstValue_t			val;


				struct{
						union{
								rayVar_t		*var;
								const wchar_t	*string_const;
								const wchar_t	*func_name;
						};
						size_t			offset;
				}addr;

				rayExpr_t				*expr;

				rayInitializer_t		*lst;
		};

		rayInitializer_t		*next;
};


struct __declarator_tag;
typedef struct __declarator_tag	rayDeclarator_t;

struct __ray_declarator_tag
{
		rayType_t				*type;
		const wchar_t			*id;
		rayInitializer_t		*init_lst;
		rayDeclarator_t			*next;
};


struct __declaration_tag;
typedef struct __declaration_tag		rayDeclaration_t;

struct __declaration_tag
{
		rayStorageClass_t		decl_class;
		size_t					qualifier;
		rayType_t				*base_type;
		size_t					type_flags;
		rayDeclarator_t			*declarator;

		rayDeclaration_t		*next;
};



/**********************************************stat**********************************************/

typedef struct __label_tag
{
		const wchar_t			*label;
		bool_t					is_used;
		bool_t					is_defined;
		raySrcInfo_t			src;
		struct __label_tag		*next;
}rayLabel_t;


typedef struct __labeled_stmt_tag
{
		rayLabel_t		*label;
		rayStatement_t	*stmt;
}rayLabelStmt_t;


typedef struct __case_label_stmt_tag
{
		int_64_t			val;
		rayExpr_t			*expr;
		rayStatement_t		*stmt;
}rayCaseLabelStmt_t;



typedef struct __compound_stmt_tag
{
		rayBlock_t			*block;
		rayStatement_t		*stmt;
}rayCompoundStmt_t;

typedef struct __if_stmt_tag
{
		rayExpr_t				*expr;
		rayStatement_t			*true_part;
		rayStatement_t			*false_part;
}rayIFStmt_t;

typedef struct __switch_stmt_tag
{
		rayExpr_t				*expr;
		rayStatement_t			*stmt;
		rayCaseLabelStmt_t		*case_lst;
		rayCaseLabelStmt_t		*default_lst;
		size_t					case_cnt;
}raySwitchStmt_t;

typedef struct __while_stmt_tag
{
		rayExpr_t				*expr;
		rayStatement_t			*stmt;
}rayWhileStmt_t;

typedef struct __for_stmt_tag
{
		rayExpr_t		*init;
		rayExpr_t		*test;
		rayExpr_t		*reinit;
		rayStatement_t	*stmt;
}rayForStmt_t;

typedef enum
{
		STT_LABELED,
		STT_CASE_LABELED,
		STT_EXPR,
		STT_COMPOUND,
		STT_IF,
		STT_SWITCH,
		STT_WHILE,
		STT_DO,
		STT_FOR,
		STT_GOTO,
		STT_RETURN,
		STT_CONTINUE,
		STT_BREAK
}rayStmtType_t;


struct __statement_tag {
		rayStmtType_t		type;
		raySrcInfo_t		src;

		union {
				rayExpr_t				*expr;
				rayLabelStmt_t			*label_stmt;
				rayCaseLabelStmt_t		*case_label_stmt;
				rayCompoundStmt_t		*compound_stmt;
				rayIFStmt_t				*if_stmt;
				raySwitchStmt_t			*switch_stmt;
				rayWhileStmt_t			*while_stmt;
				rayForStmt_t			*for_stmt;
				rayLabel_t				*goto_stmt;
		};
};




AR_NAMESPACE_END