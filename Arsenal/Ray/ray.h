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


#ifndef __ARSENAL_RAY_H__
#define __ARSENAL_RAY_H__

#include "../Common/common.h"
#include "../Lex/lex.h"
#include "../Parser/grammar.h"
#include "../Parser/parser.h"




AR_NAMESPACE_BEGIN



void	RAY_Init();
void	RAY_UnInit();









/************************************************src info******************************/
typedef struct __src_info_tag
{
		size_t	col;
		size_t	line;
}raySrcInfo_t;


/********************************************错误报告*****************************************/

typedef enum
{
		RAY_REPORT_MESSAGE,
		RAY_REPORT_ERROR,
		RAY_REPORT_WARNING
}rayReportType_t;

typedef struct __ray_report_msg_tag
{
		rayReportType_t			type;
		raySrcInfo_t			location;
		
		union{
				const	wchar_t			*message;
				psrToken_t				token;
		};
}rayReportMsg_t;


typedef void (AR_STDCALL *rayReportFunc_t)(const rayReportMsg_t *report, void *context);

typedef struct __ray_report_tag
{
		rayReportFunc_t			report_func;
		void					*ctx;
}rayReport_t;




/********************************************************************************************************************************/


struct __ray_parser_node_tag;
typedef struct __ray_parser_node_tag	rayNode_t;

struct __ray_parser_tag;
typedef struct __ray_parser_tag		rayParser_t;



struct __ray_type_tag;
typedef	struct __ray_type_tag	rayType_t;


struct	__var_tag;
typedef struct __var_tag		rayVar_t;


struct __aggregate_tag;
typedef struct __aggregate_tag		rayAggregate_t;


struct __typedef_tag;
typedef struct __typedef_tag	rayTypedef_t;

struct __block_tag;
typedef struct __block_tag		rayBlock_t;

struct	__statement_tag;
typedef struct __statement_tag	rayStatement_t;


struct	__expr_tag;
typedef	struct	__expr_tag		rayExpr_t;



struct __declarator_tag;
typedef struct __declarator_tag	rayDeclarator_t;

struct __declaration_tag;
typedef struct __declaration_tag		rayDeclaration_t;

/*******************************************************************type*********************************************************/



typedef enum{
		TOK_EOI		= 0,
		TOK_DELIM_ID = 257,
		TOK_TYPE_ID = 258,
		TOK_DONE_ID = 259,
		TOK_CHAR_CONSTANT = 260,
		TOK_STRING_LITERAL = 261,
		TOK_FLOAT_NUMBER = 262,
		TOK_INT_NUMBER = 263,
		TOK_IDENTIFIER = 264,
		TOK_SWITCH = 265,
		TOK_FOR = 266,
		TOK_GOTO = 267,
		TOK_RETURN = 268,
		TOK_DO = 269,
		TOK_WHILE = 270,
		TOK_IF = 271,
		TOK_ELSE = 272,
		TOK_CONTINUE = 273,
		TOK_DEFAULT = 274,
		TOK_CASE = 275,
		TOK_BREAK = 276,
		TOK_CONST = 277,
		TOK_VOLATILE = 278,
		TOK_STRUCT = 279,
		TOK_UNION = 280,
		TOK_TYPEDEF = 281,
		TOK_STATIC = 282,
		TOK_SIZEOF = 283,
		TOK_VOID = 284,
		TOK_BYTE = 285,
		TOK_CHAR = 286,
		TOK_SHORT = 287,
		TOK_INT = 288,
		TOK_LONG = 289,
		TOK_SIGNED = 290,
		TOK_UNSIGNED = 291,
		TOK_FLOAT = 292,
		TOK_DOUBLE = 293,
		TOK_IMPORT = 294,
		TOK_EXPORT = 295,
		TOK_ATTRIBUTE = 296,
		TOK_RSHIFT_ASSIGN = 297,
		TOK_LSHIFT_ASSIGN = 298,
		TOK_ADD_ASSIGN = 299,
		TOK_SUB_ASSIGN = 300,
		TOK_MUL_ASSIGN = 301,
		TOK_DIV_ASSIGN = 302,
		TOK_MOD_ASSIGN = 303,
		TOK_AND_ASSIGN = 304,
		TOK_XOR_ASSIGN = 305,
		TOK_OR_ASSIGN = 306,
		TOK_RSHIFT = 307,
		TOK_LSHIFT = 308,
		TOK_INC = 309,
		TOK_DEC = 310,
		TOK_PTR = 311,
		TOK_ANDAND = 312,
		TOK_OROR = 313,
		TOK_LE = 314,
		TOK_GE = 315,
		TOK_EQ = 316,
		TOK_NE = 317,
		TOK_LESS = 318,
		TOK_GREATER = 319,
		TOK_L_BRACES = 320,
		TOK_R_BRACES = 321,
		TOK_L_PAREN = 322,
		TOK_R_PAREN = 323,
		TOK_L_SQUARE = 324,
		TOK_R_SQUARE = 325,
		TOK_SEMICOLON = 326,
		TOK_COMMA = 327,
		TOK_COLON = 328,
		TOK_ASSIGN = 329,
		TOK_DOT = 330,
		TOK_AND = 331,
		TOK_NOT = 332,
		TOK_TILDE = 333,		/* ~ */
		TOK_ADD = 334,
		TOK_SUB = 335,
		TOK_MUL = 336,
		TOK_DIV = 337,
		TOK_MOD = 338,
		TOK_XOR = 339,
		TOK_OR = 340,
		TOK_QUEST = 341
}rayTokType_t;


/******************************************************************************/



typedef enum 
{
		TY_NOTYPE = 0,

		DT_PTR_TO ,		/* 指针 */
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
		TY_FP_SINGLE,
		TY_FP_DOUBLE,
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


typedef enum
{
		AT_UNION_T,
		AT_STRUCT_T
}rayAggrType_t;

struct __aggregate_tag
{
		rayAggrType_t		at_type;

		const wchar_t		*tag_name;
		size_t				size;
		size_t				alignment;
		rayVar_t			*members;			/*拥有*/
};


/*********************************************************************************/

typedef struct __array_tag 
{
		rayType_t	*type;						/*引用*/
		size_t		nelem;
		bool_t		is_completed;
}rayArray_t;


typedef struct __func_type_tag
{
		rayType_t			*ret_type;			/*引用*/
		bool_t				has_id_list;
		rayVar_t			*params;			/*拥有*/
}rayFuncType_t;




#define QU_CONST				0x0001
#define QU_VOLATILE				0x0002


typedef struct __pointer_tag
{
		rayType_t			*pointer_to;		/*引用*/
}rayPointer_t;



struct __ray_type_tag
{
		raySrcInfo_t		src;
		rayTypeCode_t		code;		
		size_t				size;
		size_t				qualifiers;
		
		union{
				rayPointer_t		pointer;	
				rayArray_t			arr;			
				rayAggregate_t		aggregate;		
				rayFuncType_t		func_type;		
		};

		rayType_t			*next;
};

rayType_t*		RAY_CreateBasicType(rayParser_t	*parser,		rayTypeCode_t	code);
rayType_t*		RAY_CreatePointerType(rayParser_t	*parser,	rayType_t		*base);
rayType_t*		RAY_CreateArrayType(rayParser_t	*parser,		rayType_t		*base,			rayExpr_t *expr);
rayType_t*		RAY_CreateFuncType(rayParser_t	*parser,		rayType_t		*ret_type,		rayDeclaration_t *decl);

rayType_t*		RAY_CreateAggregateType(rayParser_t	*parser,	rayTypeCode_t	code, const wchar_t *tag, bool_t is_completed, rayDeclaration_t *member, bool_t is_define);

void			RAY_DestroyType(rayParser_t	*parser, rayType_t	*type);
void			RAY_SetSrcInfo(rayParser_t	*parser, rayType_t	*type, const raySrcInfo_t *info);
void			RAY_AddQualifier(rayParser_t	*parser, rayType_t	*type, size_t	qualifier);

bool_t			RAY_IsSameType(const rayType_t	*l, const rayType_t *r);
size_t			RAY_GetTypeAlign(const rayParser_t *parser, const rayType_t *type);
size_t			RAY_GetTypeSize(const rayType_t *type);

AR_INLINE		size_t	RAY_AlignAddr(size_t addr, size_t align){ size_t		pad = align - addr % align; return pad == align ? addr : addr + pad; }


		
		


struct	__initializer_tag;
typedef struct	__initializer_tag		rayInitializer_t;

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



struct	__var_tag 
{
		raySrcInfo_t		src;
		const wchar_t		*name;
		rayStorageClass_t	storage;
		rayType_t			*type;		/*引用*/
		uint_64_t			addr;

		rayInitializer_t	*init;		/*拥有*/

		rayVar_t			*next;
};


/*********************************************block**********************************************************/



typedef enum
{
		NT_VAR_T,
		NT_TAG_T,
		NT_TYPEDEF_T
}rayNameType_t;

typedef struct __lookup_name_tag
{
		rayNameType_t			nt;
		const wchar_t			*name;

		union{
				rayVar_t		*var;
				rayTypedef_t	*type_def;
				rayType_t		*type;
		};
}rayName_t;



struct __typedef_tag
{
		raySrcInfo_t	src;
		const wchar_t	*name;
		rayType_t		*type;							/*引用*/
		
		rayTypedef_t	*next;
};

rayTypedef_t*	RAY_CreateTypedef(rayParser_t	*parser, const wchar_t	*name, rayType_t *type, const raySrcInfo_t *src);
void			RAY_DestroyTypedef(rayParser_t	*parser, rayTypedef_t	*type_def);



struct __block_tag
{
		rayVar_t			*vars;						/*拥有*/
		rayTypedef_t		*typedefs;					/*拥有*/
		rayType_t			*aggregates;				/*拥有*/
		
		rayBlock_t			*parent;
		rayBlock_t			*next;						
		rayBlock_t			*subblocks;
		
};



rayBlock_t*				RAY_CreateBlock(rayParser_t	*parser, rayBlock_t		*parent);
void					RAY_DestroyBlock(rayParser_t	*parser, rayBlock_t		*block);

void					RAY_InsertVariable(rayParser_t	*parser, rayBlock_t	*block, rayVar_t	 *var);
void					RAY_InsertTypedef(rayParser_t	*parser, rayBlock_t	*block, rayTypedef_t *type_def);
void					RAY_InsertType(rayParser_t		*parser, rayBlock_t	*block,	rayType_t	 *type);

rayVar_t*				RAY_LookupVar(rayParser_t	*parser,	rayBlock_t	*block,	const wchar_t *name, bool_t		current);
rayTypedef_t*			RAY_LookupTypedef(rayParser_t	*parser,rayBlock_t	*block, const wchar_t *name, bool_t		current);
rayType_t*				RAY_LookupType(rayParser_t	*parser,	rayBlock_t	*block, const wchar_t *name, bool_t		current);
const rayName_t*		RAY_LookupName(rayParser_t	*parser,	rayBlock_t	*block, const wchar_t *name, bool_t		current);





typedef struct __func_tag		
{
		raySrcInfo_t			src;
		const char				*name;
		rayType_t				*func_type;					/*引用*/

		bool_t					build_in;
		
		union{
				rayStatement_t	*statement;					/*拥有*/
				uint_64_t		extern_func;
		};
		
		struct __func_tag		*next;
}rayFunc_t;


typedef struct __parse_result_tag
{
		rayBlock_t		*block;
		rayFunc_t		*function;
}rayParseResult_t;


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




struct __ray_declarator_tag
{
		rayType_t				*type;
		const wchar_t			*id;
		rayInitializer_t		*init_lst;
		rayDeclarator_t			*next;
};


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



typedef struct __statement_list_tag
{
		rayStatement_t			**lst;
		size_t					count;
		size_t					cap;
}rayStatementList_t;


typedef struct __label_tag
{
		const wchar_t			*label;
		bool_t					is_used;
		bool_t					is_defined;
		struct __label_tag		*next;
}rayLabel_t;


typedef struct __labeled_stmt_tag
{
		rayLabel_t				*label;
		rayStatement_t			*stmt;
}rayLabelStmt_t;


typedef struct __case_label_stmt_tag
{
		int_64_t			val;
		rayExpr_t			*expr;				
		rayStatement_t		*stmt;				
}rayCaseLabelStmt_t;



typedef struct __compound_stmt_tag
{
		rayBlock_t				*block;			
		rayStatementList_t		*stmt_list;		
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
		
		raySrcInfo_t			src;
		rayStmtType_t			type;
		
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







/***************************************************************node************************************************/

typedef enum
{
		NODE_TOKEN_T,
		NODE_TYPE_FLAGS_T,
		NODE_SPECIFIER_T,
		NODE_QUALIFIER_T,
		NODE_TYPE_T,
		NODE_STORAGE_CLASS_T,
		NODE_DECLARATOR_T,
		NODE_DECLARATION_T,
		
		NODE_FUNCTION_T
}rayNodeType_t;

struct __ray_parser_node_tag
{
		rayNodeType_t	node_type;
		
		union{
				psrToken_t				tok;
				
				size_t					type_flags;
				size_t					type_specifier;
				size_t					qualifier;
				rayStorageClass_t		storage_class;

				rayType_t				*type;
				rayDeclarator_t			*declarator;
				rayDeclaration_t		*declaration;
				rayFunc_t				*function;
		};
};

rayNode_t*		RAY_CreateParserNode(rayNodeType_t type);
void			RAY_DestroyParserNode(rayNode_t	   *node);




/***************************************************************parser************************************************/

void			RAY_InitParser();
void			RAY_UnInitParser();

void			RAY_Report(const rayParser_t *parser, const rayReportMsg_t *msg);

rayParser_t*	RAY_CreateParser(const rayReport_t *report);
void			RAY_DestroyParser(rayParser_t		*parser);

bool_t			RAY_ParserHasError(const rayParser_t	*parser);
void			RAY_SetParserError(rayParser_t	*parser, bool_t is_error);

void			RAY_SetOuterBlock(rayParser_t	*parser, rayBlock_t		*block);
rayBlock_t*		RAY_GetOuterBlock(const rayParser_t *parser);
bool_t			RAY_IsOuterBlock(const rayParser_t *parser, const rayBlock_t *block);
rayBlock_t*		RAY_GetCurrentBlock(const rayParser_t *parser);



size_t			RAY_GetAlignment(const rayParser_t	*parser);
void			RAY_PushAlignment(rayParser_t	*parser, size_t align);
void			RAY_PopAlignment(rayParser_t	*parser);



rayParseResult_t* RAY_Parse(rayParser_t			*parser, const wchar_t *src);











/**********************************************************Compiler****************************************************************/


struct __ray_compiler_tag;
typedef struct __ray_compiler_tag		rayCompiler_t;



AR_NAMESPACE_END

#endif

