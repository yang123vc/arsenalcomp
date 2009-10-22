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


#include "grammar.h"

#include "parser.h"
#include "../Lex/lex.h"

AR_NAMESPACE_BEGIN


/*****************************************************PrintNode*****************************************************************/

printNode_t*	PSR_CreatePrintNode(const wchar_t *name, size_t lineno)
{
		printNode_t		*res;
		res = AR_NEW0(printNode_t);
		res->name = AR_wcsdup(name);
		res->lineno = lineno;
		return res;
}



void				PSR_DestroyPrintNode(printNode_t *node)
{
		if(node)
		{
				size_t i;
				for(i = 0; i < node->count; ++i)
				{
						PSR_DestroyPrintNode(node->nodes[i]);
				}
				AR_DEL(node->name);
				AR_DEL(node->nodes);
				AR_DEL(node);
		}
}


void PSR_InsertPrintNode(printNode_t *dest, printNode_t *sour)
{
		AR_ASSERT(dest != NULL && sour != NULL);
		if(dest->count == dest->cap)
		{
				dest->cap = (dest->cap + 4)*2;
				dest->nodes = AR_REALLOC(printNode_t*, dest->nodes, dest->cap);
		}
		dest->nodes[dest->count++] = sour;


}



static psrNode_t* AR_STDCALL __def_leaf_builder(const psrToken_t *tok,void *ctx)
{
		printNode_t		*n;
		wchar_t			*buf;
		AR_ASSERT(tok != NULL);
		if(tok->count == 0)
		{
				buf = AR_wcsdup(L"%EOI");
		}else
		{
				AR_ASSERT(tok->str != NULL);
				buf = AR_wcsndup(tok->str, tok->count);
		}

		n = PSR_CreatePrintNode(buf, tok->line);
		
		AR_DEL(buf);
		return (psrNode_t*)n;
}


static psrNode_t*		AR_STDCALL __def_reduce_handler(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		printNode_t		*n;
		printNode_t		**ns = (printNode_t**)nodes;
		size_t i;
		bool_t			has_node = false;
		AR_ASSERT(name != NULL);
		
		n = NULL;
		for(i = 0; i < count; ++i)
		{
				if(ns[i] != NULL)has_node = true;
		}

		if(has_node)
		{
				n = PSR_CreatePrintNode(name, 0);
				for(i = 0; i < count; ++i)
				{
						if(ns[i])PSR_InsertPrintNode(n, ns[i]);
				}
		}
		
		if(n && n->count > 0 )n->lineno = n->nodes[0]->lineno;

		//if(n->count > 0)n->lineno = n->nodes[0]->lineno;

		//AR_printf(L"reduce %ls\r\n", name);

		return (psrNode_t*)n;
}




static void	AR_STDCALL def_free_handler(psrNode_t *node, void *ctx)
{
		PSR_DestroyPrintNode((printNode_t*)node);

}

static void	AR_STDCALL def_error_handler(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)
{
		wchar_t *buf;
		size_t i;
		if(tok->count == 0)
		{
				buf = AR_wcsdup(L"%EOI");
		}else
		{
				buf = AR_wcsndup(tok->str, tok->count);
		}

		AR_printf(L"Invalid Token \"%ls\" in (%" AR_PLAT_INT_FMT L"u : %" AR_PLAT_INT_FMT L"u)\r\n", buf, (size_t)tok->line, (size_t)tok->col);
		
		AR_DEL(buf);
		if(count > 0)
		{
				AR_printf(L"Expected Token : ");
				for(i = 0; i < count; ++i)AR_printf(L"\"%ls\"  ", expected[i]);
				AR_printf(L"\r\n");
		}
}


static psrCtx_t		__print_node_ctx = {def_free_handler, def_error_handler, NULL};

/*const psrCtx_t			*PSR_PrintNodeCtx = &__print_node_ctx;*/

const psrCtx_t*	PSR_GetPrintNodeContext()
{
		return &__print_node_ctx;
}


/*****************************************************Config*****************************************************************/
typedef enum 
{
		CFG_TOKEN_T,
		CFG_TERM_T,
		CFG_PREC_T,
		CFG_RULE_T,
		CFG_NODE_LIST_T,
		CFG_ROOT_T
}cfgNodeType_t;


typedef struct __parser_config_node_tag	cfgNode_t;

cfgNode_t*		CFG_CreateNode(cfgNodeType_t type);
void			CFG_DestroyNode(cfgNode_t *node);
void			CFG_InsertNode(cfgNode_t  *dest, cfgNode_t *node);


typedef struct __parser_config_node_list_tag
{
		cfgNode_t		**nodes;
		size_t								count;
		size_t								cap;
}cfgNodeList_t;



typedef enum {
		
		CFG_EOI = 0,
		TOK_DEF = 601, PREC_DEF = 602, RULE_DEF = 603,ASSOC_DECL = 604, LEXEME = 605, NUMBER = 606,
	
		/*
		"." : 607, "=" : 608,  "->" : 609, ":" : 610, "|" : 611, "," : 612, ";" : 613, "{" : 614, "}" : 615;
		*/
	
		DOT = 607, ASSIGN = 608, ARROW = 609, COLON = 610, OR = 611, COMMA = 612, SEMICOLON = 613,OPEN_BR = 614, CLOSE_BR = 615
	
}cfgTokenType_t;

typedef struct __parser_config_token_tag
{
		wchar_t			*str;
		cfgTokenType_t	type;
		size_t			lineno;
}cfgToken_t;



typedef struct __parser_config_token_value_tag
{
		wchar_t			*term;
		size_t			value;
}cfgTerm_t;


typedef struct __parser_config_prec_tag
{
		psrAssocType_t	assoc;
		wchar_t			*prec_tok;
		size_t			prec;
}cfgPrec_t;

typedef struct __parser_config_rule_tag
{
		wchar_t									*head;
		wchar_t									*prec_token;
		cfgNode_t			*nodes;
		wchar_t									*rule_str;
		
}cfgRule_t;

typedef struct __parser_config_root_tag
{
		cfgNode_t	*term_list;
		cfgNode_t	*prec_list;
		cfgNode_t	*rule_list;
}cfgRoot_t;


struct __parser_config_node_tag
{
		cfgNodeType_t	type;
		union{
				cfgNodeList_t	lst;
				cfgToken_t		tok;
				cfgTerm_t		term;
				cfgPrec_t		prec;
				cfgRule_t		rule;
				cfgRoot_t		root;
				
		};
};



/*********************************************************Implementation****************************************************************************/
cfgNode_t*		CFG_CreateNode(cfgNodeType_t type)
{
		cfgNode_t		*node;

		node = AR_NEW0(cfgNode_t);
		node->type = type;
		switch(type)
		{
		case CFG_TOKEN_T:
				break;
		case CFG_TERM_T:
				break;
		case CFG_PREC_T:
				break;
		case CFG_RULE_T:
				node->rule.nodes = CFG_CreateNode(CFG_NODE_LIST_T);
				break;
		case CFG_ROOT_T:
				break;
		default:
				AR_ASSERT(CFG_NODE_LIST_T == type);
		}
		return node;		
}

void			CFG_DestroyNode(cfgNode_t *node)
{
		switch(node->type)
		{
		case CFG_TOKEN_T:
				AR_DEL(node->tok.str);
				break;
		case CFG_TERM_T:
				AR_DEL(node->term.term);
				break;
		case CFG_PREC_T:
				AR_DEL(node->prec.prec_tok);
				break;
		case CFG_RULE_T:
				AR_DEL(node->rule.head);
				AR_DEL(node->rule.prec_token);
				AR_DEL(node->rule.rule_str);
				CFG_DestroyNode(node->rule.nodes);
				break;
		case CFG_ROOT_T:
				CFG_DestroyNode(node->root.term_list);
				CFG_DestroyNode(node->root.prec_list);
				CFG_DestroyNode(node->root.rule_list);
				break;
		default:
		{
				size_t i;
				
				AR_ASSERT(CFG_NODE_LIST_T == node->type);
				
				for(i = 0; i < node->lst.count; ++i)if(node->lst.nodes[i])CFG_DestroyNode(node->lst.nodes[i]);
				
				AR_DEL(node->lst.nodes);
		}
		}

		AR_DEL(node);

}


void			CFG_InsertNode(cfgNode_t  *dest, cfgNode_t *node)
{
		AR_ASSERT(dest->type == CFG_NODE_LIST_T && node != NULL);
		
		if(dest->lst.count == dest->lst.cap)
		{
				dest->lst.cap = (dest->lst.cap + 4)*2;
				dest->lst.nodes = AR_REALLOC(cfgNode_t*, dest->lst.nodes, dest->lst.cap);
		}

		dest->lst.nodes[dest->lst.count++] = node;
}


/************************************************************************************************************************/
const wchar_t *lex_pattern =
L"%lex{\r\n"
L"	delim		= 	[\\t\\r\\n ]*\r\n"
L"	letter 		= 	[A-Z_a-z]\r\n"
L"	digit		=	[0-9]\r\n"
L"	number		=	0|[1-9]{digit}*\r\n"
L"	name		=	{letter}({letter}|{digit})*\r\n"
L"	lexeme		=	{name}|('[^']+')|(\\\"[^\\\"]+\\\")\r\n"
L"	comment		= 	\\/\\*([^\\*]|\\*+[^\\*\\/])*\\*+\\/\r\n"
L"	comment_line	= 	\\/\\/[^\\n]*\\n\r\n"

L"	0					[\\0]\r\n"
L"%skip	1				{delim}|{comment_line}|{comment}\r\n"
	
L"	601,1				\"%token\"\r\n"
L"	602,1				\"%prec\"\r\n"
L"	603,1				\"%rules\"\r\n"
L"	604,1				\"%\"(\"left\"|\"right\"|\"noassoc\")\r\n"

L"	605,0				{lexeme}\r\n"
L"	606,0				{number}\r\n"
	
L"	607,1				\".\"\r\n"
L"	608,1				\"=\"\r\n"
L"	609,1				\"->\"\r\n"
L"	610,1				\":\"\r\n"
L"	611,1				\"|\"\r\n"
L"	612,1				\",\"\r\n"
L"	613,1				\";\"\r\n"
L"	614,1				\"{\"\r\n"
L"	615,1				\"}\"\r\n"
L"}"
;


static lex_t *__build_lex()
{
		lex_t *lex;

		lex = LEX_Create();
		
		if(LEX_Config(lex, lex_pattern) == NULL)
		{
				AR_ASSERT(false);
				return NULL;
		}
		return lex;
}


/*
	  ";" : 613, "{" : 614, "}" : 615;
	
	SEMICOLON = 613,OPEN_BR = 614, CLOSE_BR = 615
*/

static psrNode_t* AR_STDCALL __build_leaf(const psrToken_t *tok,  void *ctx)
{
		cfgNode_t *n;
		AR_ASSERT(tok->count > 0);
		n = CFG_CreateNode(CFG_TOKEN_T);

		n->tok.type = (cfgTokenType_t)tok->type;
		if(tok->type == LEXEME && (tok->str[0] == L'"' || tok->str[0] == L'\''))
		{
				n->tok.str = AR_wcsndup(tok->str + 1, tok->count-2);
				AR_ASSERT(n->tok.str != NULL);
		}else
		{
				n->tok.str = AR_wcsndup(tok->str, tok->count);
		}
		n->tok.lineno = tok->line;
		return (psrNode_t*)n;
}

//L"token_rule : TOK_DEF { token_decl_list ; }"
static psrNode_t*		AR_STDCALL __build_token_rule(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		
		AR_ASSERT(count == 5 && nodes != NULL);
		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		res = ns[2];
		CFG_DestroyNode(ns[3]);
		CFG_DestroyNode(ns[4]);
		return (psrNode_t*)res;
}


/*token_decl_list 	:	token_decl_list , token_decl*/
static psrNode_t*		AR_STDCALL __build_token_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		
		AR_ASSERT(count == 3 && nodes != NULL);
		res = ns[0];
		AR_ASSERT(res != NULL);
		CFG_DestroyNode(ns[1]);
		CFG_InsertNode(res, ns[2]);
		return (psrNode_t*)res;
}






/*token_decl_list : token_decl*/
static psrNode_t*		AR_STDCALL __build_token_decl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		AR_ASSERT(count == 1 && nodes != NULL);
		res = CFG_CreateNode(CFG_NODE_LIST_T);
		CFG_InsertNode(res, ns[0]);
		return (psrNode_t*)res;
}



/*token_decl	:	LEXEME ":" NUMBER;*/

static psrNode_t*		AR_STDCALL __build_term(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		AR_ASSERT(count == 3 && nodes != NULL);
		res = CFG_CreateNode(CFG_TERM_T);
		res->term.term = AR_wcsdup(ns[0]->tok.str);
		CFG_DestroyNode(ns[1]);
		AR_wtou(ns[2]->tok.str, &res->term.value, 10);
		return (psrNode_t*)res;
}




/*******************************Prec Rule***********************************************/

/*prec_rule	 :	PREC_DEF { prec_decl_list }*/
static psrNode_t*		AR_STDCALL __build_prec_rule(psrNode_t **nodes, size_t count,const wchar_t *name,  void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		cfgNode_t		*lst;
		size_t i,j;
		AR_ASSERT(count == 4 && nodes != NULL);

		lst = CFG_CreateNode(CFG_NODE_LIST_T);

		res = ns[2];
		
		
		for(i = 0; i < res->lst.count; ++i)
		{
				cfgNode_t *n = res->lst.nodes[i];
				AR_ASSERT(n->type == CFG_NODE_LIST_T);
				for(j = 0; j < n->lst.count; ++j)
				{
						cfgNode_t  *tok = n->lst.nodes[j];
						AR_ASSERT(tok->type == CFG_PREC_T);
						tok->prec.prec = i + 1;
				}
		}

		for(i = 0; i < res->lst.count; ++i)
		{
				cfgNode_t *n = res->lst.nodes[i];
				AR_ASSERT(n->type == CFG_NODE_LIST_T);
				for(j = 0; j < n->lst.count; ++j)
				{
						cfgNode_t  *tok = n->lst.nodes[j];
						n->lst.nodes[j] = NULL;
						CFG_InsertNode(lst, tok);
				}
		}

		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		CFG_DestroyNode(ns[2]);
		CFG_DestroyNode(ns[3]);
		return lst;
}

/*prec_rule : Epsilon NULL*/


/*prec_decl_list :	prec_decl_list  prec_decl */


static psrNode_t*		AR_STDCALL __build_prec_decl_list1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		AR_ASSERT(count == 2 && nodes != NULL);
		
		res = ns[0];
		AR_ASSERT(res != NULL);
		CFG_InsertNode(res, ns[1]);
		return res;
}


/* prec_decl_list :  prec_decl */
static psrNode_t*		AR_STDCALL __build_prec_decl_list2(psrNode_t **nodes, size_t count,const wchar_t *name,  void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		AR_ASSERT(count == 1 && nodes != NULL);
		
		res = CFG_CreateNode(CFG_NODE_LIST_T);
		CFG_InsertNode(res, ns[0]);
		return res;
}


/*prec_decl	:	ASSOC_DECL prec_token_list ; */

static psrNode_t*		AR_STDCALL __build_prec_decl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		*res;
		size_t i;
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		AR_ASSERT(count == 3 && nodes != NULL);
		res = ns[1];
		for(i = 0;i < res->lst.count; ++i)
		{
				cfgNode_t *n = res->lst.nodes[i];
				AR_ASSERT(n->type == CFG_PREC_T);

				switch(ns[0]->tok.str[1])
				{
				case L'l':
						n->prec.assoc = PSR_ASSOC_LEFT;
						break;
				case L'r':
						n->prec.assoc = PSR_ASSOC_RIGHT;
						break;
				case L'n':
						n->prec.assoc = PSR_ASSOC_NOASSOC;
						break;
				default:
						/*
								因为parser所依赖的token必须是正确的，否则此程序被认为有问题，且据地方所完成的功能来说，
						这里直接挂掉是最好的选择。
						*/
						AR_ASSERT(false);
						break;
				}
		}
		CFG_DestroyNode(ns[2]);
		return res;
}


/*prec_token_list :	prec_token_list  ","  LEXEME*/

static psrNode_t*		AR_STDCALL __build_prec_token1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res, *n;
		AR_ASSERT(count == 3 && nodes != NULL);
		res = ns[0]; AR_ASSERT(res != NULL);
		CFG_DestroyNode(ns[1]);
		
		n = CFG_CreateNode(CFG_PREC_T);
		n->prec.assoc = PSR_ASSOC_NOASSOC;
		n->prec.prec = 0;
		n->prec.prec_tok = AR_wcsdup(ns[2]->tok.str);
		
		CFG_InsertNode(res, n);
		CFG_DestroyNode(ns[2]);
		return res;
}

/*prec_token_list :	 LEXEME;*/
static psrNode_t*		AR_STDCALL __build_prec_token2(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res, *n;

		AR_ASSERT(count == 1 && nodes != NULL);
		res = CFG_CreateNode(CFG_NODE_LIST_T);
		n = CFG_CreateNode(CFG_PREC_T);
		n->prec.assoc = PSR_ASSOC_NOASSOC;
		n->prec.prec = 0;
		n->prec.prec_tok = AR_wcsdup(ns[0]->tok.str);
		CFG_InsertNode(res, n);
		CFG_DestroyNode(ns[0]);
		return res;
}


/*************************************************************************rule***************************************************************/


/*rules_rule :	RULE_DEF  { rule_decl_list  }*/
static psrNode_t*		AR_STDCALL __handle_rules(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		size_t i,j;
		arString_t		*str;
		AR_ASSERT(count == 4 && nodes != NULL);
		
		str = AR_CreateString();

		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		res = ns[2];
		CFG_DestroyNode(ns[3]);

		for(i = 0; i < res->lst.count; ++i)
		{
				
				cfgNode_t *rule = res->lst.nodes[i];
				
				AR_AppendString(str, rule->rule.head);
				AR_AppendString(str, L" : ");
				
				for(j = 0; j < rule->rule.nodes->lst.count; ++j)
				{
						cfgNode_t *lxm = rule->rule.nodes->lst.nodes[j];
						AR_ASSERT(lxm->type == CFG_TOKEN_T);
						AR_ASSERT(lxm->tok.type == LEXEME || lxm->tok.type == DOT);

						if(lxm->tok.type == LEXEME)
						{
								AR_AppendString(str, lxm->tok.str);
								AR_AppendString(str, L" ");
						}else
						{
								AR_AppendString(str, L" ");/*epsilon*/
						}
				}

				rule->rule.rule_str = AR_wcsdup(AR_GetStrString(str));
				AR_ClearString(str);
		}

		AR_DestroyString(str);
		return res;
}



/*rule_decl_list	:	rule_decl_list   rule_decl */

static psrNode_t*		AR_STDCALL __handle_rule_decl_list1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		size_t i;
		AR_ASSERT(count == 2 && nodes != NULL);
		res = ns[0];

		for(i = 0; i < ns[1]->lst.count; ++i)
		{
				CFG_InsertNode(res, ns[1]->lst.nodes[i]);
				ns[1]->lst.nodes[i] = NULL;
		}
		CFG_DestroyNode(ns[1]);
		return res;
}



/*rule_decl_list	: rule_decl */
static psrNode_t*		AR_STDCALL __handle_rule_decl_list2(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		AR_ASSERT(count == 1 && nodes != NULL);
		return ns[0];
}


/*rule_decl		:	LEXEME -> rule_list ;*/
static psrNode_t*		AR_STDCALL __handle_rule_decl(psrNode_t **nodes, size_t count,const wchar_t *name,  void *ctx)
{

		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*head, *rule_list;
		size_t i;
		AR_ASSERT(count == 4 && nodes != NULL);
		head = ns[0];
		CFG_DestroyNode(ns[1]);
		rule_list = ns[2];
		CFG_DestroyNode(ns[3]);

		for(i = 0; i < rule_list->lst.count; ++i)
		{
				cfgNode_t *rule = rule_list->lst.nodes[i];
				AR_ASSERT(rule->type == CFG_RULE_T);
				rule->rule.head = AR_wcsdup(head->tok.str);
		}

		return rule_list;
}

/*rule_list		:	rule_list  | rule rule_prec */

static psrNode_t*		AR_STDCALL __handle_rule_list1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res, *n, *prec;
		AR_ASSERT(count == 4 && nodes != NULL);
		res = ns[0];
		CFG_DestroyNode(ns[1]);
		
		n = ns[2];
		prec = ns[3];
		
		if(prec != NULL)
		{
				n->rule.prec_token = AR_wcsdup(prec->tok.str);
				CFG_DestroyNode(prec);
		}
		
		CFG_InsertNode(res, n);
		return res;
}


/*rule_list		:	rule rule_prec */
static psrNode_t*		AR_STDCALL __handle_rule_list2(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res, *lst;
		AR_ASSERT(count == 2 && nodes != NULL);
		res = ns[0];
		if(ns[1] != NULL)
		{
				AR_ASSERT(ns[1]->type == CFG_TOKEN_T);
				res->rule.prec_token = AR_wcsdup(ns[1]->tok.str);
				CFG_DestroyNode(ns[1]);
		}
		
		
		lst = CFG_CreateNode(CFG_NODE_LIST_T);
		CFG_InsertNode(lst, res);
		return lst;
}


/*rule		:	rule LEXEME */
static psrNode_t*		AR_STDCALL __handle_rule_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		AR_ASSERT(count == 2 && nodes != NULL);
		res = ns[0];
		CFG_InsertNode(res->rule.nodes, ns[1]);
		return res;
}

/*rule		:   LEXEME*/
/*rule		:	.*/
static psrNode_t*		AR_STDCALL __handle_rule_lexeme(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		AR_ASSERT(count == 1&& nodes != NULL);
		res = CFG_CreateNode(CFG_RULE_T);
		CFG_InsertNode(res->rule.nodes, ns[0]);

		return res;
}

	
/*rule_prec	:	PREC_DEF LEXEME*/
static psrNode_t*		AR_STDCALL __handle_rule_prec(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		AR_ASSERT(count == 2 && nodes != NULL);
		CFG_DestroyNode(ns[0]);
		return ns[1];
}

/*rule_prec	:	. */






/*************************************************************************root***************************************************************/
/*root_rule	: 	token_rule  prec_rule rules_rule*/

static psrNode_t*		AR_STDCALL __build_root(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		AR_ASSERT(count == 3 && nodes != NULL);
		res = CFG_CreateNode(CFG_ROOT_T);
		res->root.term_list = ns[0];
		res->root.prec_list = ns[1];
		res->root.rule_list = ns[2];
		return res;
}

static psrGrammar_t* __build_grammar()
{
		psrGrammar_t *gmr;
		
		gmr = PSR_CreateGrammar();
		
		PSR_InsertTerm(gmr, L"TOK_DEF", TOK_DEF, PSR_ASSOC_NOASSOC, 0, __build_leaf);
		PSR_InsertTerm(gmr, L"PREC_DEF", PREC_DEF, PSR_ASSOC_NOASSOC, 0, __build_leaf);

		PSR_InsertTerm(gmr, L"RULE_DEF", RULE_DEF, PSR_ASSOC_NOASSOC, 0, __build_leaf);
		PSR_InsertTerm(gmr, L"LEXEME", LEXEME, PSR_ASSOC_NOASSOC, 0, __build_leaf);

		PSR_InsertTerm(gmr, L"NUMBER", NUMBER, PSR_ASSOC_NOASSOC, 0, __build_leaf);
		PSR_InsertTerm(gmr, L"ASSOC_DECL", ASSOC_DECL, PSR_ASSOC_NOASSOC, 0, __build_leaf);


		PSR_InsertTerm(gmr, L".", DOT, PSR_ASSOC_NOASSOC, 0, __build_leaf);
		PSR_InsertTerm(gmr, L"=", ASSIGN, PSR_ASSOC_NOASSOC, 0, __build_leaf);

		PSR_InsertTerm(gmr, L"->", ARROW, PSR_ASSOC_NOASSOC, 0, __build_leaf);
		PSR_InsertTerm(gmr, L":", COLON, PSR_ASSOC_NOASSOC, 0, __build_leaf);

		PSR_InsertTerm(gmr, L"|", OR, PSR_ASSOC_NOASSOC, 0, __build_leaf);
		PSR_InsertTerm(gmr, L",", COMMA, PSR_ASSOC_NOASSOC, 0, __build_leaf);

		PSR_InsertTerm(gmr, L";", SEMICOLON, PSR_ASSOC_NOASSOC, 0, __build_leaf);
		PSR_InsertTerm(gmr, L"{", OPEN_BR, PSR_ASSOC_NOASSOC, 0, __build_leaf);
		PSR_InsertTerm(gmr, L"}", CLOSE_BR, PSR_ASSOC_NOASSOC, 0, __build_leaf);
		
		
		/**************************************************root******************************************************/

		PSR_InsertRuleByStr(gmr, L"root_rule	: 	token_rule  prec_rule rules_rule", NULL, __build_root);
		
		/**************************************************root*****************************************************end*/

		PSR_InsertRuleByStr(gmr, L"token_rule		: TOK_DEF { token_decl_list ; }", NULL, __build_token_rule);
		PSR_InsertRuleByStr(gmr, L"token_decl_list 	: token_decl_list , token_decl", NULL, __build_token_list);
		PSR_InsertRuleByStr(gmr, L"token_decl_list	: token_decl", NULL, __build_token_decl);
		PSR_InsertRuleByStr(gmr, L"token_decl		: LEXEME : NUMBER", NULL, __build_term);


		PSR_InsertRuleByStr(gmr, L"prec_rule	 :	PREC_DEF { prec_decl_list }", NULL, __build_prec_rule);
		PSR_InsertRuleByStr(gmr, L"prec_rule	 :	", NULL, NULL);

		
		PSR_InsertRuleByStr(gmr, L"prec_decl_list :	prec_decl_list  prec_decl", NULL, __build_prec_decl_list1);
		PSR_InsertRuleByStr(gmr, L"prec_decl_list :  prec_decl", NULL, __build_prec_decl_list2);
		
		PSR_InsertRuleByStr(gmr, L"prec_decl	:	ASSOC_DECL prec_token_list ;", NULL, __build_prec_decl);
		
		
		PSR_InsertRuleByStr(gmr, L"prec_token_list :	prec_token_list  ,  LEXEME", NULL, __build_prec_token1);
		
		PSR_InsertRuleByStr(gmr, L"prec_token_list :	LEXEME", NULL, __build_prec_token2);

		/*******************************rules*************************************/


		PSR_InsertRuleByStr(gmr, L"rules_rule :	RULE_DEF  { rule_decl_list  }", NULL, __handle_rules);
		PSR_InsertRuleByStr(gmr, L"rule_decl_list	:	rule_decl_list   rule_decl", NULL, __handle_rule_decl_list1);
		PSR_InsertRuleByStr(gmr, L"rule_decl_list	: rule_decl", NULL, __handle_rule_decl_list2);
		
		PSR_InsertRuleByStr(gmr, L"rule_decl		:	LEXEME -> rule_list ;", NULL, __handle_rule_decl);
		
		PSR_InsertRuleByStr(gmr, L"rule_list		:	rule_list  | rule rule_prec", NULL, __handle_rule_list1);
		PSR_InsertRuleByStr(gmr, L"rule_list		:	rule rule_prec", NULL, __handle_rule_list2);
		
		PSR_InsertRuleByStr(gmr, L"rule		:	rule LEXEME", NULL, __handle_rule_list);

		PSR_InsertRuleByStr(gmr, L"rule		:   LEXEME", NULL, __handle_rule_lexeme);
		PSR_InsertRuleByStr(gmr, L"rule		:   .", NULL, __handle_rule_lexeme);
		

			
		PSR_InsertRuleByStr(gmr, L"rule_prec	:	PREC_DEF LEXEME", NULL, __handle_rule_prec);

		PSR_InsertRuleByStr(gmr, L"rule_prec	: ", NULL, NULL);

		AR_ASSERT(PSR_CheckIsValidGrammar(gmr));
		return gmr;
}







static void	AR_STDCALL cfg_free(psrNode_t *node, void *ctx)
{
		CFG_DestroyNode((cfgNode_t*)node);

}

static void	AR_STDCALL cfg_on_error(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)
{
		wchar_t *buf;
		size_t i;
		if(tok->count == 0)
		{
				buf = AR_wcsdup(L"%EOI");
		}else
		{
				buf = AR_wcsndup(tok->str, tok->count);
		}

		
		AR_printf(L"Invalid Token \"%ls\" in (%" AR_PLAT_INT_FMT L"d : %" AR_PLAT_INT_FMT L"d)\r\n", buf, tok->line, tok->col);
		

		AR_DEL(buf);

		for(i = 0; i < count; ++i)AR_printf(L"Expected token \"%ls\"", expected[i]);
		AR_printf(L"\r\n");
}




static cfgNode_t* __build_root_node(const wchar_t *pattern)
{
		bool_t is_done;
		cfgNode_t *root;
		lex_t *lex;
		lexMatch_t		match;
		lexToken_t		tok;
		
		psrGrammar_t	*gmr;
		parser_t		*parser;
		psrCtx_t		ctx = {cfg_free,  cfg_on_error, NULL};
		AR_ASSERT(pattern != NULL);

		LEX_InitMatch(&match, pattern);

		lex = __build_lex();
		gmr = __build_grammar();
		AR_ASSERT(gmr != NULL && lex != NULL);
		parser = PSR_CreateParser(gmr, PSR_LALR, &ctx);
		AR_ASSERT(parser != NULL); 
		
		is_done = false;
		root = NULL;
		while(!is_done)
		{
				if(!LEX_Match(lex, &match, &tok))
				{
						
						/*AR_printf(L"Invalid Token : %ls in (%d : %d)\r\n",match.next, match.line, match.col);*/
						AR_printf(L"Invalid Token \"%ls\" in (%" AR_PLAT_INT_FMT L"d : %" AR_PLAT_INT_FMT L"d)\r\n", match.next, (size_t)match.line, (size_t)match.col);
						break;
				}

				if(!PSR_AddToken(parser, &tok))
				{
						break;		
				}
				if(tok.type == CFG_EOI)is_done = true;
		}
		
		if(is_done)
		{
				root = (cfgNode_t*)PSR_GetResult(parser);
				AR_ASSERT(root != NULL && root->type == CFG_ROOT_T);
		}
		
		LEX_UnInitMatch(&match);
		LEX_Destroy(lex);
		PSR_DestroyParser(parser);
		PSR_DestroyGrammar(gmr);
		return root;
}




/*************************************Config*****************************************************************************/







static bool_t __config_grammar(psrGrammar_t *gmr, cfgNode_t *root)
{
		size_t i;
		bool_t is_ok;
		AR_ASSERT(gmr != NULL && root != NULL && root->type == CFG_ROOT_T && root->root.term_list != NULL && root->root.rule_list != NULL);
		PSR_ClearGrammar(gmr);
		is_ok = true;

		for(i = 0; is_ok && i < root->root.term_list->lst.count; ++i)
		{
				const cfgNode_t	*term = root->root.term_list->lst.nodes[i];
				AR_ASSERT(term->type == CFG_TERM_T);
				is_ok = PSR_InsertTerm(gmr, term->term.term, term->term.value, PSR_ASSOC_NOASSOC, 0, __def_leaf_builder);
		}
		
		/*if(!is_ok)goto END_POINT;*/
		
		for(i = 0; is_ok && i < root->root.prec_list->lst.count; i++)
		{
				psrTermInfo_t	*tinfo = NULL;
				const cfgNode_t	*prec = root->root.prec_list->lst.nodes[i];
				AR_ASSERT(prec != NULL && prec->type == CFG_PREC_T);
				tinfo = PSR_GetTermSymbInfoByName(gmr, prec->prec.prec_tok);
				
				if(tinfo == NULL)
				{
						size_t val = 0xFFFF;

						while((tinfo = PSR_GetTermSymbInfoByValue(gmr, val)) != NULL)val++;
						
						PSR_InsertTerm(gmr, prec->prec.prec_tok, val, prec->prec.assoc, prec->prec.prec, NULL);
						tinfo = PSR_GetTermSymbInfoByName(gmr, prec->prec.prec_tok);
						AR_ASSERT(tinfo != NULL);
				}else
				{
						tinfo->assoc = prec->prec.assoc;
						tinfo->prec = prec->prec.prec;
				}
		}
		
		for(i = 0; is_ok && i < root->root.rule_list->lst.count; ++i)
		{
				const cfgNode_t	*rule = root->root.rule_list->lst.nodes[i];
				AR_ASSERT(rule && rule->type == CFG_RULE_T);
				is_ok = PSR_InsertRuleByStr(gmr, rule->rule.rule_str, rule->rule.prec_token, __def_reduce_handler);
		}

/*END_POINT:*/
		if(!is_ok)
		{
				PSR_ClearGrammar(gmr);
				return false;
		}else
		{
				return PSR_CheckIsValidGrammar(gmr);
		}
}




/******************************************************Test***************************************************************/


bool_t PSR_ConfigGrammar(psrGrammar_t *gmr, const wchar_t *pattern)
{
		cfgNode_t *root;
		bool_t is_ok;
		AR_ASSERT(gmr != NULL && pattern != NULL);
		PSR_ClearGrammar(gmr);
		root = __build_root_node(pattern);
		if(!root)return false;

		is_ok = __config_grammar(gmr, root);
		CFG_DestroyNode(root);
		return is_ok;
}























AR_NAMESPACE_END




