#include "test.h"
#include "../../Arsenal/Parser/grammar.h"
#include "../../Arsenal/Parser/parser.h"
#include "../../Arsenal/Parser/lr_action.h"

#if defined(__LIB)

AR_NAMESPACE_BEGIN




typedef enum
{
		EOI = 0,
		CLP = 354,
		RP = 355,
		ADD = 356,
		MINUS = 357,
		MUL = 358,
		DIV = 359,
		MOD = 360,
		NUMBER = 361
}TokenType_t;

typedef struct __token_tag
{
		TokenType_t		type;
		const wchar_t	*str;
}Token_t;


typedef struct __node_tag
{
		Token_t			op;
		struct __node_tag	*chd[2];
}Node_t;



static Node_t* CreateNode(const Token_t *op, Node_t *l, Node_t *r)
{
		Node_t *node;

		node =AR_NEW0(Node_t);
		node->op = *op;
		node->chd[0] = l;
		node->chd[1] = r;
		return node;
}

static  psrNode_t*		AR_STDCALL create_leaf(const psrToken_t *tok,void *ctx)
{
		Token_t	op;
		op.type = (TokenType_t)tok->type;
		op.str = AR_wcsndup(tok->str, tok->count);
		return (psrNode_t*)CreateNode(&op, NULL, NULL);
}



/*
E : E + E
E : E - E
E : E * E
E : E / E
E : E % E
*/

static  psrNode_t*		AR_STDCALL create_node(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		Node_t *op_node;
		
		op_node = (Node_t*)nodes[1];
		switch(op_node->op.type)
		{
		case ADD:
		case MINUS:
		case MUL:
		case DIV:
		case MOD:
				return (psrNode_t*)CreateNode(&op_node->op, (Node_t*)nodes[0], (Node_t*)nodes[2]);
		default:
				AR_ASSERT(false);
				return NULL;
				break;
		}
}

/*
E : number
E : ( E )
*/

static  psrNode_t*		AR_STDCALL create_node1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		AR_ASSERT(count == 1 || count == 3);
		if(count == 1)
		{
				return nodes[0];
		}else
		{
				return nodes[1];
		}
}

/*
		E : - E
*/


static  psrNode_t*		AR_STDCALL create_node2(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		AR_ASSERT(count == 2);
		return (psrNode_t*)CreateNode(&((Node_t*)nodes[0])->op, NULL, (Node_t*)nodes[1]);
}


static  void			AR_STDCALL free_node(psrNode_t *node, void *ctx)
{
		printf("on_free\r\n");

}

static  void			AR_STDCALL on_error(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)

{
		printf("on_error\r\n");

}

/*
{L"(", 354, 0, PSR_ASSOC_NOASSOC},
		{L")", 355, 0, PSR_ASSOC_NOASSOC},
		{L"+", 356, 0, PSR_ASSOC_NOASSOC},
		{L"-", 357, 0, PSR_ASSOC_NOASSOC},
		{L"*", 358, 0, PSR_ASSOC_NOASSOC},
		{L"/", 359, 0, PSR_ASSOC_NOASSOC},
		{L"%", 360, 0, PSR_ASSOC_NOASSOC},
		{L"number", 361, 0, PSR_ASSOC_NOASSOC},
		{L"UMINUS", 0xFFFF, 0, PSR_ASSOC_NOASSOC}
*/

static lex_t* __build_lex()
{
		lex_t *lex;

		lex = LEX_Create();

		LEX_Insert(lex, L"delim 			= 	[ \\r\\n\\t]");
		LEX_Insert(lex, L"digit 			= 	[0-9]");
		LEX_Insert(lex, L"number 			= 	{digit}+");
		LEX_Insert(lex, L"EOI 			= 	\\0");

		LEX_Insert(lex, L"0				{EOI}");
		LEX_Insert(lex, L"%skip 1		{delim}");
		LEX_Insert(lex, L"354,1			\"(\"");
		LEX_Insert(lex, L"355,1			\")\"");
		LEX_Insert(lex, L"356,1			\"+\"");
		LEX_Insert(lex, L"357,1			\"-\"");
		LEX_Insert(lex, L"358,1			\"*\"");
		LEX_Insert(lex, L"359,1			\"/\"");
		LEX_Insert(lex, L"360,1			\"%\"");
		LEX_Insert(lex, L"361,1		¡¡¡¡{¡¡ number}");
		
		
		if(LEX_GenerateTransTable(lex))
		{
				return lex;
		}else
		{
				AR_ASSERT(0);
				return NULL;
		}
		
}

static psrGrammar_t *__g_gmr = NULL;
static  parser_t* __build_parser()
{
		psrGrammar_t	*gmr;

		gmr = PSR_CreateGrammar();

		PSR_InsertTerm(gmr, L"(", CLP, PSR_ASSOC_NOASSOC, 0, create_leaf);
		PSR_InsertTerm(gmr, L")", RP, PSR_ASSOC_NOASSOC, 0, create_leaf);
		PSR_InsertTerm(gmr, L"+", ADD, PSR_ASSOC_LEFT, 1, create_leaf);
		PSR_InsertTerm(gmr, L"-", MINUS, PSR_ASSOC_LEFT, 1, create_leaf);
		PSR_InsertTerm(gmr, L"*", MUL, PSR_ASSOC_LEFT, 2, create_leaf);
		PSR_InsertTerm(gmr, L"/", DIV, PSR_ASSOC_LEFT, 2, create_leaf);
		PSR_InsertTerm(gmr, L"%", MOD, PSR_ASSOC_LEFT, 2, create_leaf);
		PSR_InsertTerm(gmr, L"number", NUMBER, PSR_ASSOC_NOASSOC, 0, create_leaf);
		PSR_InsertTerm(gmr, L"UMINUS", 0xFFFF, PSR_ASSOC_RIGHT, 3, create_leaf);
		
		PSR_InsertRuleByStr(gmr, L"E : E + E", NULL, create_node);
		PSR_InsertRuleByStr(gmr, L"E : E - E", NULL, create_node);
		PSR_InsertRuleByStr(gmr, L"E : E * E", NULL, create_node);
		PSR_InsertRuleByStr(gmr, L"E : E / E", NULL, create_node);
		PSR_InsertRuleByStr(gmr, L"E : E % E", NULL, create_node);

		PSR_InsertRuleByStr(gmr, L"E : number", NULL, create_node1);
		PSR_InsertRuleByStr(gmr, L"E : ( E )", NULL, create_node1);
		PSR_InsertRuleByStr(gmr, L"E : - E", L"UMINUS", create_node2);
		
		{
				arString_t		*str;

				str = AR_CreateString();
				PSR_PrintGrammar(gmr, str);
				AR_printf(L"%ls\r\n", AR_GetStrString(str));

				__g_gmr = gmr;
				getchar();
		}



		{
				psrCtx_t ctx = {free_node, on_error, NULL};
				return PSR_CreateParser(gmr, PSR_LALR, &ctx);
		}
}

int post_order(Node_t *node)
{
		int l,r,v;
		if(node == NULL)return 0;
		l = post_order(node->chd[0]);
		r = post_order(node->chd[1]);
		
		AR_printf(L"%ls : %d", node->op.str, node->op.type);
		switch(node->op.type)
		{
		case ADD:
				return l + r;
		case MINUS:
				return l - r;
		case MUL:
				return l * r;
		case DIV:
				return l / r;
		case MOD:
				return l % r;
		case NUMBER:
		{
				AR_wtoi32(node->op.str, &v, 10);
				return v;
		}
		default:
				AR_ASSERT(node != NULL);
				return 0;
		}
}




void calc_test()
{
		parser_t *psr;
		lex_t	*lex;
		lexMatch_t		match;
		lexToken_t		tok;
		lex = __build_lex();
		AR_ASSERT(lex);
		psr = __build_parser();

		LEX_InitMatch(&match, L"10 + (5 + 4) / 3");

		while(LEX_Match(lex, &match, &tok))
		{
				AR_printf(L"token == %ls : val = %d\r\n", AR_wcsndup(tok.str, tok.count), tok.type);
				AR_ASSERT(PSR_AddToken(psr, &tok));
				if(tok.type == 0)break;
		}
		
		getchar();

		{
				Node_t *res;
				int v;
				res = (Node_t*)PSR_GetResult(psr);
				v = post_order(res);
				printf("\r\n");
				printf("num == %d\r\n", v);

		}
		
		PSR_DestroyParser(psr);
		PSR_DestroyGrammar(__g_gmr);
}











AR_NAMESPACE_END

#endif