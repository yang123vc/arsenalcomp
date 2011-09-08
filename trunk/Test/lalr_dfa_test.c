#include "../Arsenal/Lex/lex.h"
#include "../Arsenal/Parser/lalr.h"


#include <time.h>

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
		op.type = (TokenType_t)tok->term_val;
		op.str = AR_wcsndup(tok->str, tok->str_cnt);
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

/*
static  void			AR_STDCALL on_error(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)

{
		printf("on_error\r\n");

}
*/

/*
{L"(", 354, 0, PARSER_ASSOC_NONASSOC},
		{L")", 355, 0, PARSER_ASSOC_NONASSOC},
		{L"+", 356, 0, PARSER_ASSOC_NONASSOC},
		{L"-", 357, 0, PARSER_ASSOC_NONASSOC},
		{L"*", 358, 0, PARSER_ASSOC_NONASSOC},
		{L"/", 359, 0, PARSER_ASSOC_NONASSOC},
		{L"%", 360, 0, PARSER_ASSOC_NONASSOC},
		{L"number", 361, 0, PARSER_ASSOC_NONASSOC},
		{L"UMINUS", 0xFFFF, 0, PARSER_ASSOC_NONASSOC}
*/

static lex_t* __build_lex()
{
		lex_t *lex;

		lex = Lex_Create();

		Lex_Insert(lex, L"delim 			= 	[ \\r\\n\\t]");
		Lex_Insert(lex, L"digit 			= 	[0-9]");
		Lex_Insert(lex, L"number 			= 	{digit}+");
		Lex_Insert(lex, L"EOI 			= 	$");

		Lex_Insert(lex, L"0				{EOI}");
		Lex_Insert(lex, L"%skip 1		{delim}");
		Lex_Insert(lex, L"354,1			\"(\"");
		Lex_Insert(lex, L"355,1			\")\"");
		Lex_Insert(lex, L"356,1			\"+\"");
		Lex_Insert(lex, L"357,1			\"-\"");
		Lex_Insert(lex, L"358,1			\"*\"");
		Lex_Insert(lex, L"359,1			\"/\"");
		Lex_Insert(lex, L"360,1			\"%\"");
		Lex_Insert(lex, L"361,1		¡¡¡¡{¡¡ number}");
		
		
		if(Lex_GenerateTransTable(lex))
		{
				return lex;
		}else
		{
				AR_ASSERT(0);
				return NULL;
		}
		
}

static  psrGrammar_t* __build_grammar()
{
		psrGrammar_t	*gmr;
		psrHandler_t ctx = {NULL, free_node};

		gmr = Parser_CreateGrammar(&ctx);

		Parser_InsertTerm(gmr, L"(", CLP, PARSER_ASSOC_NONASSOC, 0, create_leaf);
		Parser_InsertTerm(gmr, L")", RP, PARSER_ASSOC_NONASSOC, 0, create_leaf);
		Parser_InsertTerm(gmr, L"+", ADD, PARSER_ASSOC_LEFT, 1, create_leaf);
		Parser_InsertTerm(gmr, L"-", MINUS, PARSER_ASSOC_LEFT, 1, create_leaf);
		Parser_InsertTerm(gmr, L"*", MUL, PARSER_ASSOC_LEFT, 2, create_leaf);
		Parser_InsertTerm(gmr, L"/", DIV, PARSER_ASSOC_LEFT, 2, create_leaf);
		Parser_InsertTerm(gmr, L"%", MOD, PARSER_ASSOC_LEFT, 2, create_leaf);
		Parser_InsertTerm(gmr, L"number", NUMBER, PARSER_ASSOC_NONASSOC, 0, create_leaf);
		Parser_InsertTerm(gmr, L"UMINUS", 0xFFFF, PARSER_ASSOC_RIGHT, 3, create_leaf);
		

		

		
		Parser_InsertRuleByStr(gmr, L"E : E + E", NULL, create_node, 0);/*	
		Parser_InsertRuleByStr(gmr, L"E : E - E", NULL, create_node, 0);
		Parser_InsertRuleByStr(gmr, L"E : E * E", NULL, create_node, 0);
		Parser_InsertRuleByStr(gmr, L"E : E / E", NULL, create_node, 0);
		Parser_InsertRuleByStr(gmr, L"E : E % E", NULL, create_node, 0);
		*/

		Parser_InsertRuleByStr(gmr, L"E : ( E )", NULL, create_node1, 0);
		Parser_InsertRuleByStr(gmr, L"E : - E", L"UMINUS", create_node2, 0);
		Parser_InsertRuleByStr(gmr, L"E : number", NULL, create_node1, 0);


		//Parser_InsertRuleByStr(gmr, L"E : X number", NULL, create_node1, 0);
		//Parser_InsertRuleByStr(gmr, L"E : Y number", NULL, create_node1, 0);

		
		/*
		Parser_InsertRuleByStr(gmr, L"E : ", NULL, NULL, 0);
		Parser_InsertRuleByStr(gmr, L"X : ", NULL, NULL, 0);
		Parser_InsertRuleByStr(gmr, L"Y : ", NULL, NULL, 0);
		*/
		{
				arString_t		*str;

				str = AR_CreateString();
//				Parser_PrintGrammar(gmr, str);
//				AR_printf(L"%ls\r\n", AR_GetStringCString(str));

				AR_DestroyString(str);
				getchar();
		}

		return gmr;
}




static  psrGrammar_t* __build_grammar1()
{
		psrGrammar_t	*gmr;
		psrHandler_t ctx = {NULL, free_node};

		gmr = Parser_CreateGrammar(&ctx);

		Parser_InsertTerm(gmr, L"(", CLP, PARSER_ASSOC_NONASSOC, 0, create_leaf);
		Parser_InsertTerm(gmr, L")", RP, PARSER_ASSOC_NONASSOC, 0, create_leaf);
		Parser_InsertTerm(gmr, L"a", NUMBER, PARSER_ASSOC_NONASSOC, 1, create_leaf);

		

		
		Parser_InsertRuleByStr(gmr, L"A : ( A )", NULL, create_node, 0);
		Parser_InsertRuleByStr(gmr, L"A : a", NULL, create_node, 0);
		return gmr;
}



void lalr_test1()
{
		lex_t	*lex;
		lexMatch_t		match;
		lexToken_t		tok;
		psrGrammar_t	*gmr;

		psrSymbMap_t	first, follow;
		size_t i,k;
		bool_t is_ok;
		arString_t		*str;

		//lex = __build_lex();
		//AR_ASSERT(lex);
		
		//gmr = __build_grammar();

		gmr = __build_grammar1();

		str = AR_CreateString();
		
		Parser_InitSymbMap(&first);
		Parser_InitSymbMap(&follow);

		Parser_CalcFirstSet(gmr, &first);
		Parser_CalcFollowSet(gmr, &follow, &first);

		Parser_PrintSymbolMap(&first, str);
		AR_printf(L"First Set:\r\n%ls\r\n", AR_GetStringCString(str));

		AR_ClearString(str);
		Parser_PrintSymbolMap(&follow, str);
		AR_printf(L"Follow Set:\r\n%ls\r\n", AR_GetStringCString(str));
		
		Parser_CheckIsValidGrammar(gmr, NULL);
/*
		AR_ClearString(str);
		Parser_ReportLeftRecursion(gmr, str);
		AR_printf(L"Left Recursion:\r\n%ls\r\n", AR_GetStringCString(str));
*/

		
		
		lalrState_t		*start = Parser_Create_LALR_State(gmr);
		
		
		lalrStateSet_t		set;

		Parser_InitStateSet(&set);
		Parser_CollectState(&set, start);

		for(i = 0; i < set.count; ++i)
		{
				AR_ClearString(str);
				Parser_PrintConfigList(set.set[i]->all_config, gmr, str);
				AR_printf(L"[%d]:\r\n%ls\r\n", i,AR_GetStringCString(str));
		}
		
		Parser_DestroyState_ALL(start);
		Parser_UnInitStateSet(&set);
		

		Parser_DestroyGrammar(gmr);

		Parser_UnInitSymbMap(&first);
		Parser_UnInitSymbMap(&follow);

		if(str)AR_DestroyString(str);
}






void lalr_test()
{
		lalr_test1();
}

AR_NAMESPACE_END


#endif
