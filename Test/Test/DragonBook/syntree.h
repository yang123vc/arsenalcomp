#include "comn.h"

AR_NAMESPACE_BEGIN




/**************************************************Token*******************************************************/

typedef enum
{
		EOI				=		0,
		INT_CONST		=		270,
		FLOAT_CONST		=		271,
		ID				=		290,
		ASSIGN_OP		=		300,
		
		AND_OP			=		400,
		OR_OP			=		401,
		EQUAL_OP		=		405,
		NOT_EQUAL_OP	=		406,
		LESS_OP			=		407,
		GREATER_OP		=		408,
		LESS_EQ_OP		=		409,
		GREATER_EQ_OP	=		410,
		ADD_OP			=		413,
		MINUS_OP		=		414,
		MUL_OP			=		415,
		DIV_OP			=		416,
		MOD_OP			=		417,
		NOT_OP			=		503,
		
		L_SQ			=		600,
		R_SQ			=		601,
		L_P				=		602,
		R_P				=		603,
		L_ANGLE			=		604,
		R_ANGLE			=		605,
		SEMICOLON		=		606,
		COMMA			=		610,
		
		FOR				=		800,
		IF				=		802,
		BREAK			=		803,
		DO				=		806,
		ELSE			=		807,
		WHILE			=		809,
		TRUE_KW			=		900,
		FALSE_KW		=		901
}TokenValue_t;



typedef struct __syn_token_tag
{
		size_t			line;
		wchar_t			*name;
		TokenValue_t	value;
}synToken_t;


void SYN_InitToken(synToken_t *stok, const lexToken_t *tok)
{
		AR_ASSERT(stok != NULL && tok != NULL);
		stok->line = tok->line;
		stok->name = AR_wcsndup(tok->str, tok->count);
		stok->value = (TokenValue_t)tok->type;
}

void SYN_UnInitToken(synToken_t *stok)
{
		AR_ASSERT(stok != NULL);
		AR_DEL(stok->name);
		AR_memset(stok, 0, sizeof(*stok));
}




typedef struct __syn_node_tag synNode_t;


typedef enum
{
		SYN_TOKEN_T,

		SYN_CONSTANT_T,

		SYN_ARRAY_T,
		
		SYN_UNARY_EXPR_T,

		SYN_BINARY_EXPR_T,

		SYN_ARRAY_ACCESS_T,
		
		SYN_STMT_T,
		
		SYN_TYPE_T,
		
		SYN_DECL_T
}synNodeType_t;

synNode_t*		SYN_CreateNode(synNodeType_t t);
void			SYN_DestroyNode(synNode_t *node);



typedef struct __syn_node_array_tag
{
		synNode_t		**nodes;
		size_t			count;
		size_t			cap;
}synArray_t;

static void SYN_InitArray(synArray_t *arr)
{
		AR_ASSERT(arr != NULL);
		AR_memset(arr, 0, sizeof(*arr));
}

static void SYN_UnInitArray(synArray_t *arr)
{
		size_t i;
		AR_ASSERT(arr != NULL);
		for(i = 0; i < arr->count; ++i)SYN_DestroyNode(arr->nodes[i]);
		AR_DEL(arr->nodes);
		AR_memset(arr, 0, sizeof(*arr));
}

static void SYN_InsertToArray(synArray_t *arr, synNode_t *node)
{
		AR_ASSERT(arr != NULL && node != NULL);
		if(arr->count == arr->cap)
		{
				arr->cap = (arr->cap + 4)*2;
				arr->nodes = AR_REALLOC(synNode_t*,  arr->nodes, arr->cap);
		}
		arr->nodes[arr->count++] = node;
}




/*******************************************decl***********************************************************/

typedef enum
{
		SYN_TYPE_BASIC_T,
		SYN_TYPE_ARRAY_T
}synTypeType_t;

typedef struct __syn_type_tag
{
		synTypeType_t	type_type;
		synNode_t		*base;
		size_t			array_len;
}synType_t;


typedef struct __syn_decl_tag
{
		synNode_t		*type;
		synNode_t		*id;
}synDecl_t;




/**********************************************constant************************************************/

typedef enum 
{
		SYN_BOOLEN_T,
		SYN_FLOAT_T,
		SYN_INTEGER_T
}synConstType_t;

typedef struct __syn_constant_tag
{
		size_t			line;
		synConstType_t	const_type;
		
		union{
				bool	boolen;
				int64_t	num;
				double	fnum;
		};
}synConstant_t;



/**********************************************expr************************************************/

typedef struct __syn_unary_expr_tag
{
		size_t			line;
		TokenValue_t	op;
		synNode_t		*node;
}synUnExpr_t;

typedef struct __syn_binary_expr_tag
{
		size_t			line;
		TokenValue_t	op;

		synNode_t		*left;
		synNode_t		*right;
}synBinExpr_t;


typedef struct __syn_array_access_tag
{
		size_t			line;

		synNode_t		*id_expr;
		synNode_t		*index_expr;
}synAccess_t;

/**********************************************stmt************************************************/

typedef enum
{
		SYN_STMT_ASSIGN_T,
		SYN_STMT_IF_T,
		SYN_STMT_IF_ELSE_T,
		SYN_STMT_WHILE_T,
		SYN_STMT_DO_WHILE_T,
		SYN_STMT_BREAK_T,
		SYN_STMT_BLOCK_T
}synStmtType_t;

#define MAX_STMT_NODE	4

typedef struct __syn_stmt_tag
{
		synStmtType_t	stmt_type;

		size_t			line;

		synNode_t*		stmt[MAX_STMT_NODE];
}synStmt_t;






AR_NAMESPACE_END