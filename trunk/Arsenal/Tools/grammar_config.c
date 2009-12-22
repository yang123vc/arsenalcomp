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

#include "grammar_config.h"



AR_NAMESPACE_BEGIN

typedef enum 
{
		CFG_LEXEME_T,
		CFG_NAME_T,
		CFG_TOKEN_T,
		CFG_PREC_T,
		CFG_RULE_T,
		CFG_NODE_LIST_T,
		CFG_CONFIG_T
}cfgNodeType_t;

struct __cfg_node_tag
{
		union{
				cfgLexeme_t		lexeme;
				cfgName_t		name;
				cfgNodeList_t	lst;
				cfgPrec_t		prec;
				cfgToken_t		token;
				cfgRule_t		rule;
				cfgConfig_t		config;
		};

		cfgNodeType_t	type;
};


static const wchar_t *__cfg_lex_name[] = 
{
		L"	delim		= 	[\\t\\r\\n ]+",
		L"	letter		= 	[A-Z_a-z]",
		L"	digit		=	[0-9]",
		L"	number		=	0|[1-9]{digit}*",
		L"	name		=	{letter}({letter}|{digit})*",
		L"	lexeme		=	{name}|(\\\"([^\\\"])+\\\")|('([^'])+')",

		L"	comment		= 	/\\*([^\\*]|\\*+[^\\*/])*\\*+/",
		L"	comment_line	= 	//[^\\n]*\\n",
		L"  skip_lexem		= {delim}|{comment}|{comment_line}",
		L"  key_lookahead   = {skip_lexem}+|\"{\""
};


typedef enum
{
		EOI	= 0,
		DELIM ,
		SKIP = 600,
		NAME,
		TOKEN,
		PREC,
		RULES,
		HANDLER,
		ASSOC,
		LEXEME,
		NUMBER,
		DOT,
		COMMA,
		COLON,
		SEMI,
		OR,
		BLOCK_OPEN,
		BLOCK_CLOSE
}cfgLexValue_t;



typedef struct  __cfg_lex_pattern_tag
{
		size_t			val;
		const wchar_t	*regex;
		bool_t			is_skip;
		size_t			prec;
}cfgLexPattern_t;

static const cfgLexPattern_t	__cfg_pattern[] = 
{
		{EOI,	L"[\\0]", false,0},
		{DELIM, L"{skip_lexem}+", true},
		{ASSOC,	L"\"%\"(\"left\"|\"right\"|\"noassoc\")", false,1},
		
		{SKIP,	L"\"%skip\"", false,0},
		{NAME,	L"\"%name\"(?={key_lookahead})", false,0},
		{TOKEN,	L"\"%token\"(?={key_lookahead})", false,0},
		{PREC,	L"\"%prec\"(?={key_lookahead})", false,0},
		{RULES,	L"\"%rule\"(?={key_lookahead})",false,0},
		{HANDLER, L"\"%handler\"(?={skip_lexem}+)", false, 0},

		
		{LEXEME,	L"{lexeme}", false,0},
		{NUMBER,	L"{number}", false,0},

		{DOT,		L"\".\"",	false,1},
		{COMMA,		L"\",\"",	false,1},

		{COLON,		L"\":\"",	false,1},
		{SEMI,		L"\";\"",	false,1},

		{OR,		L"\"|\"",	false,1},

		{BLOCK_OPEN,	L"\"{\"",	false,1},
		{BLOCK_CLOSE,	L"\"}\"",	false,1}
};













typedef struct	__cfg_term_info_tag
{
		cfgLexValue_t	val;
		const wchar_t	*name;
}cfgTermInfo_t;


static const cfgTermInfo_t	__cfg_term[] = 
{
		{SKIP, L"%skip"},
		{NAME, L"%name"},
		{TOKEN, L"%token"},
		{PREC, L"%prec"},
		{RULES, L"%rule"},
		{HANDLER, L"%handler"},
		{ASSOC, L"assoc"},
		{LEXEME, L"lexeme"},
		{NUMBER, L"number"},

		{DOT, L"."},
		{COMMA, L","},
		{COLON, L":"},
		{SEMI, L";"},
		{OR,   L"|"},
		{BLOCK_OPEN, L"{"},
		{BLOCK_CLOSE, L"}"}
};



static void CFG_InitNodeList(cfgNodeList_t *lst)
{
		AR_ASSERT(lst != NULL);
		AR_memset(lst, 0, sizeof(*lst));
}


static void CFG_UnInitNodeList(cfgNodeList_t *lst)
{
		if(lst && lst->lst)
		{
				AR_DEL(lst->lst);
				AR_memset(lst, 0, sizeof(*lst));
		}
}

static void CFG_InsertToNodeList(cfgNodeList_t *lst, cfgNode_t *node)
{
		AR_ASSERT(lst != NULL && node != NULL);

		
		if(lst->count == lst->cap)
		{
				lst->cap = (lst->cap + 4)*2;
				lst->lst = (cfgNode_t**)AR_realloc(lst->lst, sizeof(cfgNode_t*) * lst->cap);
		}
		lst->lst[lst->count++] = node;
}




static void CFG_InitConfig(cfgConfig_t *cfg, cfgNodeList_t *name, cfgNodeList_t *token, cfgNodeList_t *prec, cfgNodeList_t *rule)
{
		size_t i;
		AR_ASSERT(cfg != NULL && token != NULL);

		AR_memset(cfg, 0, sizeof(*cfg));

		if(name)
		{
				cfg->name_cnt = name->count;
				cfg->name = cfg->name_cnt > 0 ? AR_NEWARR0(cfgName_t, name->count) : NULL;
				
				for(i = 0; i < name->count; ++i)
				{
						cfg->name[i].line = name->lst[i]->name.line;
						cfg->name[i].name = AR_wcsdup(name->lst[i]->name.name);
						cfg->name[i].regex = AR_wcsdup(name->lst[i]->name.regex);
				}
		}


		if(token)
		{
				cfg->tok_cnt = token->count;

				cfg->tok = cfg->tok_cnt > 0 ? AR_NEWARR0(cfgToken_t, cfg->tok_cnt) : NULL;

				for(i = 0; i < cfg->tok_cnt; ++i)
				{
						cfg->tok[i] = token->lst[i]->token;
						cfg->tok[i].name = token->lst[i]->token.name != NULL ? AR_wcsdup(token->lst[i]->token.name) : NULL;
						cfg->tok[i].regex = AR_wcsdup(token->lst[i]->token.regex);
				}
		}


		if(prec)
		{
				cfg->prec_cnt = prec->count;
				cfg->prec = cfg->prec_cnt > 0 ? AR_NEWARR0(cfgPrec_t, cfg->prec_cnt) : NULL;

				for(i = 0; i < cfg->prec_cnt; ++i)
				{
						cfg->prec[i] = prec->lst[i]->prec;

						if(prec->lst[i]->prec.prec_tok)
						{
								cfg->prec[i].prec_tok = AR_wcsdup(prec->lst[i]->prec.prec_tok);
						}
				}
		}

		if(rule)
		{
				cfg->rule_cnt = rule->count;

				cfg->rule = rule->count > 0 ? AR_NEWARR0(cfgRule_t, rule->count) : NULL;

				for(i = 0; i < cfg->rule_cnt; ++i)
				{
						cfg->rule[i].line = rule->lst[i]->rule.line;

						cfg->rule[i].lhs = AR_wcsdup(rule->lst[i]->rule.lhs);
						cfg->rule[i].rhs = AR_wcsdup(rule->lst[i]->rule.rhs);

						if(rule->lst[i]->rule.prec_tok)
						{
								cfg->rule[i].prec_tok = AR_wcsdup(rule->lst[i]->rule.prec_tok);
						}

						if(rule->lst[i]->rule.handler_name)
						{
								cfg->rule[i].handler_name = AR_wcsdup(rule->lst[i]->rule.handler_name);
						}
				}

		}



}


static void CFG_UnInitConfig(cfgConfig_t *cfg)
{
		size_t i;
		AR_ASSERT(cfg != NULL);

		for(i = 0; i < cfg->name_cnt; ++i)
		{
				AR_DEL(cfg->name[i].name);
				AR_DEL(cfg->name[i].regex);
				
		}

		if(cfg->name)AR_DEL(cfg->name);


		for(i = 0; i < cfg->tok_cnt; ++i)
		{
				AR_DEL(cfg->tok[i].name);
				AR_DEL(cfg->tok[i].regex);
		}

		if(cfg->tok)AR_DEL(cfg->tok);


		for(i = 0; i < cfg->prec_cnt; ++i)
		{
				if(cfg->prec[i].prec_tok)AR_DEL(cfg->prec[i].prec_tok);
		}

		if(cfg->prec)AR_DEL(cfg->prec);

		
		for(i = 0; i < cfg->rule_cnt; ++i)
		{
				AR_DEL(cfg->rule[i].lhs);
				AR_DEL(cfg->rule[i].rhs);
				
				if(cfg->rule[i].prec_tok)AR_DEL(cfg->rule[i].prec_tok);
				if(cfg->rule[i].handler_name)AR_DEL(cfg->rule[i].handler_name);
		}

		AR_DEL(cfg->rule);

}




static cfgNode_t* CFG_CreateNode(cfgNodeType_t type)
{
		cfgNode_t		*node;

		node = AR_NEW0(cfgNode_t);

		node->type = type;

		if(node->type == CFG_NODE_LIST_T)
		{
				CFG_InitNodeList(&node->lst);
		}

		return node;
}


static void CFG_DestroyNode(cfgNode_t *node)
{
		if(node)
		{
				switch(node->type)
				{
				case CFG_LEXEME_T:
				{
						if(node->lexeme.lexeme)
						{
								AR_DEL(node->lexeme.lexeme);
						}
						break;
				}
				case CFG_NAME_T:
				{
						if(node->name.name)AR_DEL(node->name.name);
						if(node->name.regex)AR_DEL(node->name.regex);
						break;
				}
				case CFG_TOKEN_T:
				{
						if(node->token.name)AR_DEL(node->token.name);
						if(node->token.regex)AR_DEL(node->token.regex);
						break;
				}
				case CFG_PREC_T:
				{
						if(node->prec.prec_tok)AR_DEL(node->prec.prec_tok);
						break;
				}
				case CFG_RULE_T:
				{
						if(node->rule.lhs)AR_DEL(node->rule.lhs);
						if(node->rule.rhs)AR_DEL(node->rule.rhs);
						if(node->rule.prec_tok)AR_DEL(node->rule.prec_tok);
						if(node->rule.handler_name)AR_DEL(node->rule.handler_name);
						break;
				}
				case CFG_NODE_LIST_T:
				{
						size_t i;
						for(i = 0; i < node->lst.count; ++i)
						{
								CFG_DestroyNode(node->lst.lst[i]);
						}
						CFG_UnInitNodeList(&node->lst);
						break;
				}
				case CFG_CONFIG_T:
				{
						CFG_UnInitConfig(&node->config);
						break;
				}
				default:
				{
						AR_ASSERT(false);
						AR_abort();
				}
				}

				AR_DEL(node);
		}
}





static void	AR_STDCALL cfg_free(psrNode_t *node, void *ctx)
{
		CFG_DestroyNode((cfgNode_t*)node);

}






static psrNode_t* AR_STDCALL __build_leaf(const psrToken_t *tok,  void *ctx)
{
		cfgNode_t *node;
		AR_ASSERT(tok->str_cnt > 0);
		node = CFG_CreateNode(CFG_LEXEME_T);
		
		node->lexeme.lex_val = (cfgLexValue_t)tok->term_val;

		
		if(tok->term_val == LEXEME && (tok->str[0] == L'"' || tok->str[0] == L'\''))
		{
				node->lexeme.lexeme = AR_wcsndup(tok->str + 1, tok->str_cnt-2);
				AR_ASSERT(node->lexeme.lexeme != NULL);
		}else if(tok->str_cnt > 0)
		{
				node->lexeme.lexeme = AR_wcsndup(tok->str, tok->str_cnt);
		}
		
		node->lexeme.line = tok->line;
		return (psrNode_t*)node;
}



/*{ L"prec_decl			:		%prec lexeme",									NULL},*/
/*{ L"prec_decl			:		",												NULL},*/

static psrNode_t*		AR_STDCALL __build_prec_decl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		
		AR_ASSERT(count == 2 && nodes != NULL);
		AR_ASSERT(ns[0]->type == CFG_LEXEME_T && ns[1]->type == CFG_LEXEME_T);

		res = ns[1];
		CFG_DestroyNode(ns[0]);
		ns[0] = NULL;
		ns[1] = NULL;
		return res;
}

/*
{ L"handler_decl		:		%handler lexeme",								NULL},
{ L"handler_decl		:				",										NULL},
*/
static psrNode_t*		AR_STDCALL __handle_handler_decl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		return __build_prec_decl(nodes, count, name, ctx);
}



/*
{ L"term_list			:		term_list term",								NULL},
{ L"term_list			:		term",											NULL},
*/

static psrNode_t*		AR_STDCALL __handle_term_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		
		AR_ASSERT((count == 1 || count == 2) && nodes != NULL);

		if(count == 1)
		{
				res = CFG_CreateNode(CFG_NODE_LIST_T);

				AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T);
				
				CFG_InsertToNodeList(&res->lst, ns[0]);
				ns[0] = NULL;
		}else
		{
				AR_ASSERT(ns[0] && ns[0]->type == CFG_NODE_LIST_T && ns[1] && ns[1]->type == CFG_LEXEME_T);

				res = ns[0];
				
				CFG_InsertToNodeList(&res->lst, ns[1]);

				ns[0] = ns[1] = NULL;
		}
		
		return res;
}




/*
{ L"rhs					:		term_list prec_decl handler_decl",				NULL},
{ L"rhs					:		.",												NULL},
*/

static psrNode_t*		AR_STDCALL __handle_rhs(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		
		AR_ASSERT((count == 1 || count == 3) && nodes != NULL);
		
		res = CFG_CreateNode(CFG_RULE_T);
		res->rule.lhs = NULL;
		res->rule.line = 0;
		res->rule.prec_tok = NULL;
		res->rule.rhs = AR_NEWARR0(wchar_t, 2048);
		
		if(count == 3)
		{
				size_t i;

				AR_ASSERT(ns[0] && ns[0]->type == CFG_NODE_LIST_T && ns[0]->lst.count > 0);
				AR_ASSERT(ns[1] ? ns[1]->type == CFG_LEXEME_T : true);

				AR_ASSERT(ns[2] ? ns[2]->type == CFG_LEXEME_T : true);
				

				
				for(i = 0; i < ns[0]->lst.count; ++i)
				{
						cfgNode_t *tmp = ns[0]->lst.lst[i];

						AR_ASSERT(tmp->type == CFG_LEXEME_T && tmp->lexeme.lex_val == (size_t)LEXEME);

						AR_wcscat((wchar_t*)res->rule.rhs, tmp->lexeme.lexeme);
						AR_wcscat((wchar_t*)res->rule.rhs, L" ");
				}

				if(ns[1])
				{
						res->rule.prec_tok = ns[1]->lexeme.lexeme;
						ns[1]->lexeme.lexeme = NULL;
				}

				if(ns[2])
				{
						res->rule.handler_name = ns[2]->lexeme.lexeme;
						ns[2]->lexeme.lexeme = NULL;
				}

				CFG_DestroyNode(ns[0]);
				CFG_DestroyNode(ns[1]);
				CFG_DestroyNode(ns[2]);
				ns[0] = ns[1] = ns[2] = NULL;
		}else
		{
				AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[0]->lexeme.lex_val == DOT);
				AR_wcscat((wchar_t*)res->rule.rhs, L" ");

				CFG_DestroyNode(ns[0]);
				ns[0] = NULL;

		}

		return res;

}




/*
{ L"rhs_list			: 		rhs_list  | rhs ",								NULL},
{ L"rhs_list			: 		rhs ",											NULL},
*/

static psrNode_t*		AR_STDCALL __handle_rhs_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		
		AR_ASSERT((count == 1 || count == 3) && nodes != NULL);

		if(count == 3)
		{
				AR_ASSERT(ns[0] && ns[0]->type == CFG_NODE_LIST_T && ns[1]->type == CFG_LEXEME_T && ns[1]->lexeme.lex_val == (size_t)OR && ns[2] && ns[2]->type == CFG_RULE_T);

				res = ns[0];

				CFG_InsertToNodeList(&res->lst, ns[2]);

				CFG_DestroyNode(ns[1]);

				ns[0] = ns[1] = ns[2] = NULL;
		}else
		{
				AR_ASSERT(ns[0]->type == CFG_RULE_T);
				res = CFG_CreateNode(CFG_NODE_LIST_T);
				CFG_InsertToNodeList(&res->lst, ns[0]);
				ns[0] = NULL;
		}

		return res;


}


/*
{ L"rule_def			: 		lexeme : rhs_list ;",							NULL},
*/

static psrNode_t*		AR_STDCALL __handle_rule_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		size_t i;
		AR_ASSERT(count == 4);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_NODE_LIST_T && ns[3] && ns[3]->type == CFG_LEXEME_T);

		
		CFG_DestroyNode(ns[1]);
		CFG_DestroyNode(ns[3]);
		
		AR_ASSERT(ns[0]->lexeme.lexeme != NULL);

		for(i = 0; i < ns[2]->lst.count; ++i)
		{
				cfgNode_t *tmp = ns[2]->lst.lst[i];

				AR_ASSERT(tmp->type == CFG_RULE_T);
				tmp->rule.lhs = AR_wcsdup(ns[0]->lexeme.lexeme);
				tmp->rule.line = ns[0]->lexeme.line;
		}
		
		CFG_DestroyNode(ns[0]);

		res = ns[2];

		ns[0] = ns[1] = ns[2] = ns[3] = NULL;

		return res;
}



/*
{ L"rule_def_list		:		rule_def_list rule_def ",						NULL},
{ L"rule_def_list		:		rule_def",										NULL},
*/

static psrNode_t*		AR_STDCALL __handle_rule_def_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		AR_ASSERT(count == 2 || count == 1);

		res = ns[0];
		
		if(count == 1)
		{
				AR_ASSERT(ns[0] && ns[0]->type == CFG_NODE_LIST_T);

				ns[0] = NULL;
				
		}else
		{
				size_t i;
				AR_ASSERT(ns[0] && ns[0]->type == CFG_NODE_LIST_T && ns[1] && ns[1]->type == CFG_NODE_LIST_T);
				
				for(i = 0; i < ns[1]->lst.count; ++i)
				{
						cfgNode_t *tmp = ns[1]->lst.lst[i];
						AR_ASSERT(tmp && tmp->type == CFG_RULE_T);
						CFG_InsertToNodeList(&res->lst, tmp);
						ns[1]->lst.lst[i] = NULL;
				}

				CFG_DestroyNode(ns[1]);
				ns[0] = ns[1] = NULL;
		}

		return res;

}


/*
{ L"rule_block			:		%rule { rule_def_list  }",						NULL}
*/
static psrNode_t*		AR_STDCALL __handle_rule_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		AR_ASSERT(count == 4);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_NODE_LIST_T && ns[3] && ns[3]->type == CFG_LEXEME_T);

		
		
		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		res = ns[2];
		ns[2] = NULL;
		CFG_DestroyNode(ns[3]);

		ns[0] = ns[1] = ns[2] = ns[3] = NULL;
		return res;
}





/*{ L"prec_def			:  		assoc lexeme ;",								NULL},*/
static psrNode_t*		AR_STDCALL __handle_prec_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		wchar_t			c;
		AR_ASSERT(count == 3);
		
		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T &&  ns[0]->lexeme.lex_val == ASSOC && ns[1] && ns[1]->type == CFG_LEXEME_T);


		
		res = CFG_CreateNode(CFG_PREC_T);
		res->prec.line = ns[0]->lexeme.line;

		c = ns[0]->lexeme.lexeme[1];
		
		if(c == L'n')
		{
				res->prec.assoc	= PSR_ASSOC_NOASSOC;

		}else if(c == L'l')
		{
				res->prec.assoc	= PSR_ASSOC_LEFT;

		}else if(c == L'r')
		{
				res->prec.assoc	= PSR_ASSOC_RIGHT;
		}else
		{
				AR_abort();
		}
				
		
		res->prec.prec_tok = ns[1]->lexeme.lexeme;
		ns[1]->lexeme.lexeme = NULL;

		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);

		ns[0] = ns[1] = NULL;

		return res;

}


/*
{ L"prec_def_list		:		prec_def_list prec_def",						NULL},
{ L"prec_def_list		:		",												NULL},
*/

static psrNode_t*		AR_STDCALL __handle_prec_def_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 2 && nodes);

		AR_ASSERT((ns[0] == NULL) || (ns[0] && ns[0]->type == CFG_NODE_LIST_T));
		AR_ASSERT(ns[1] && ns[1]->type == CFG_PREC_T);

		if(ns[0] == NULL)
		{
				res = CFG_CreateNode(CFG_NODE_LIST_T);
		}else
		{
				res = ns[0];
				ns[0] = NULL;
		}

		CFG_InsertToNodeList(&res->lst, ns[1]);
		ns[1] = NULL;
		
		return res;
}


/*
{ L"prec_block 			:		%prec { prec_def_list  }",						NULL},
{ L"prec_block 			:		",												NULL},
*/


static psrNode_t*		AR_STDCALL __handle_prec_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 4 && nodes != NULL);
		
		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[3] && ns[3]->type == CFG_LEXEME_T);
		AR_ASSERT((ns[2] == NULL) || (ns[2]->type == CFG_NODE_LIST_T));

		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		res = ns[2];
		ns[2] = NULL;
		
		CFG_DestroyNode(ns[2]);

		ns[0] = ns[1] = ns[2] = ns[3] = NULL;

		return res;
}


/*
{ L"token_val_prec 		:		,	number",									NULL},
{ L"token_val_prec 		:		",												NULL},
*/


static psrNode_t*		AR_STDCALL __handle_token_val_prec(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 2 && nodes != NULL);
		
		AR_ASSERT(ns[0]->type == CFG_LEXEME_T && ns[0]->lexeme.lex_val == (size_t)COMMA);
		AR_ASSERT(ns[1]->type == CFG_LEXEME_T && ns[1]->lexeme.lex_val == (size_t)NUMBER);
		
		CFG_DestroyNode(ns[0]);
		res = ns[1];

		ns[0] = ns[1] = NULL;

		return res;

}






/*
{ L"token_def			:  		%skip token_val_prec : lexeme token_val_prec ;",		NULL},
{ L"token_def			:  		lexeme token_val_prec : lexeme token_val_prec ;",		NULL},
*/


static psrNode_t*		AR_STDCALL __handle_token_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 6);
		
		/*AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_LEXEME_T && ns[4] && ns[4]->type == CFG_LEXEME_T);*/

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_LEXEME_T && ns[3] && ns[3]->type == CFG_LEXEME_T);
		
		AR_ASSERT((ns[1] == NULL) || (ns[1]->type == CFG_LEXEME_T));
		AR_ASSERT((ns[4] == NULL) || (ns[4]->type == CFG_LEXEME_T));
		


		res = CFG_CreateNode(CFG_TOKEN_T);

		res->token.line = ns[0]->lexeme.line;

		if(ns[0]->lexeme.lex_val == SKIP)
		{
				res->token.is_skip = true;
				res->token.name = NULL;
		}else
		{
				res->token.is_skip = false;
				
				res->token.name = ns[0]->lexeme.lexeme;
				ns[0]->lexeme.lexeme = NULL;
		}

		res->token.regex = ns[3]->lexeme.lexeme;
		ns[3]->lexeme.lexeme = NULL;
		
		res->token.lex_prec = 0;
		
		if(ns[1])
		{
				AR_wtou(ns[1]->lexeme.lexeme, &res->token.tokval, 10);
		}else
		{
				res->token.tokval = 0;
		}

		if(ns[4])
		{
				AR_wtou(ns[4]->lexeme.lexeme, &res->token.lex_prec, 10);
		}

		

		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		CFG_DestroyNode(ns[2]);
		CFG_DestroyNode(ns[3]);
		CFG_DestroyNode(ns[4]);

		ns[0] = ns[1] = ns[2] = ns[3] = ns[4] = NULL;

		return res;

}


/*
{ L"token_def_list 		:		token_def_list  token_def",						NULL},
{ L"token_def_list 		:		token_def ",									NULL},
*/

static psrNode_t*		AR_STDCALL __handle_token_def_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 1 || count == 2);

		if(count == 2)
		{
				AR_ASSERT(ns[0] && ns[0]->type == CFG_NODE_LIST_T && ns[1] && ns[1]->type == CFG_TOKEN_T);

				res = ns[0];

				CFG_InsertToNodeList(&res->lst, ns[1]);
				ns[0] = ns[1] = NULL;
		}else
		{
				AR_ASSERT(ns[0] && ns[0]->type == CFG_TOKEN_T);
				res = CFG_CreateNode(CFG_NODE_LIST_T);
				CFG_InsertToNodeList(&res->lst, ns[0]);
				ns[0] = NULL;
		}
		
		return res;

}


/*
{ L"token_block 		:		%token { token_def_list }",						NULL},
*/

static psrNode_t*		AR_STDCALL __handle_token_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 4);
		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_NODE_LIST_T && ns[3] && ns[3]->type == CFG_LEXEME_T);
		
		

		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		res = ns[2];
		ns[2] = NULL;
		CFG_DestroyNode(ns[3]);

		ns[0] = ns[1] = ns[2] = ns[3] = NULL;
		return res;
}




/*
{ L"name_def 			:		lexeme : lexeme ; ",							NULL},
*/

static psrNode_t*		AR_STDCALL __handle_name_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 4);
		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_LEXEME_T && ns[3] && ns[3]->type == CFG_LEXEME_T);
		
		res = CFG_CreateNode(CFG_NAME_T);

		res->name.line = ns[0]->lexeme.line;
		res->name.name = ns[0]->lexeme.lexeme;

		ns[0]->lexeme.lexeme = NULL;
		
		res->name.regex = ns[2]->lexeme.lexeme;
		ns[2]->lexeme.lexeme = NULL;

		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		CFG_DestroyNode(ns[2]);
		CFG_DestroyNode(ns[3]);
		ns[0] = ns[1] = ns[2] = ns[3] = NULL;
		return res;
}




/*
{ L"name_def_list 		:		name_def_list  name_def",						NULL},
{ L"name_def_list 		:		",												NULL},
*/

static psrNode_t*		AR_STDCALL __handle_name_def_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 2);
		AR_ASSERT((ns[0] == NULL || (ns[0] && ns[0]->type == CFG_NODE_LIST_T)) && ns[1] && ns[1]->type == CFG_NAME_T);
		
		if(ns[0] == NULL)
		{
				res = CFG_CreateNode(CFG_NODE_LIST_T);
		}else
		{
				res =ns[0];
				ns[0] = NULL;
		}
		
		CFG_InsertToNodeList(&res->lst, ns[1]);
		ns[1] = NULL;
		
		return res;
}





/*
{ L"name_block			:		%name { name_def_list }",						NULL},
{ L"name_block			:			",											NULL},
*/

static psrNode_t*		AR_STDCALL __handle_name_block(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 4);
		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T &&  ns[1] && ns[1]->type == CFG_LEXEME_T && ns[3] && ns[3]->type == CFG_LEXEME_T);

		AR_ASSERT((ns[2] == NULL) || (ns[2]->type == CFG_NODE_LIST_T));
		
		res = ns[2];
		
		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		res = ns[2];
		CFG_DestroyNode(ns[3]);

		ns[0] = ns[1] = ns[2] = ns[3] = NULL;

		
		return res;
}


/*{ L"program				:		name_block token_block prec_block rule_block",	NULL},*/

static psrNode_t*		AR_STDCALL __handle_program(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 4);
		AR_ASSERT((ns[0] == NULL) || (ns[0] && ns[0]->type == CFG_NODE_LIST_T));
		AR_ASSERT(ns[1] && ns[1]->type == CFG_NODE_LIST_T);
		AR_ASSERT((ns[2] == NULL) || (ns[2] && ns[2]->type == CFG_NODE_LIST_T));
		AR_ASSERT((ns[3] == NULL) || (ns[3] && ns[3]->type == CFG_NODE_LIST_T));

		
		res = CFG_CreateNode(CFG_CONFIG_T);

		CFG_InitConfig(	&res->config
						, ns[0] ?  &ns[0]->lst : NULL
						, ns[1] ?  &ns[1]->lst : NULL
						, ns[2] ?  &ns[2]->lst : NULL
						, ns[3] ?  &ns[3]->lst : NULL
						);

		return (psrNode_t*)res;
}










typedef struct __cfg_rule_tag
{
		const wchar_t *rule;
		psrRuleFunc_t handler;
}cfgRuleDef_t;

static const cfgRuleDef_t	__cfg_rule[] = 
{

		{ L"program				:		name_block token_block prec_block rule_block",	__handle_program},
		
		{ L"name_block			:		%name { name_def_list }",						__handle_name_block},
		{ L"name_block			:			",											NULL},
		{ L"name_def_list 		:		name_def_list  name_def",						__handle_name_def_list},
		{ L"name_def_list 		:		",												NULL},
		{ L"name_def 			:		lexeme : lexeme ; ",							__handle_name_def},

		{ L"token_block 		:		%token { token_def_list }",						__handle_token_block},
		{ L"token_def_list 		:		token_def_list  token_def",						__handle_token_def_list},
		{ L"token_def_list 		:		token_def ",									__handle_token_def_list},
		
		{ L"token_def			:  		%skip token_val_prec : lexeme token_val_prec ;",				__handle_token_def},
		{ L"token_def			:  		lexeme token_val_prec : lexeme token_val_prec ;",				__handle_token_def},

		{ L"token_val_prec 		:		, number",										__handle_token_val_prec},
		{ L"token_val_prec 		:		",												NULL},
		

		{ L"prec_block 			:		%prec { prec_def_list  }",						__handle_prec_block},
		{ L"prec_block 			:		",												NULL},
		{ L"prec_def_list		:		prec_def_list prec_def",						__handle_prec_def_list},
		{ L"prec_def_list		:		",												NULL},
		{ L"prec_def			:  		assoc lexeme ;",								__handle_prec_def},

		{ L"rule_block			:		%rule { rule_def_list  }",						__handle_rule_block},
		{ L"rule_block			:		",												NULL},
		{ L"rule_def_list		:		rule_def_list rule_def ",						__handle_rule_def_list},
		{ L"rule_def_list		:		rule_def",										__handle_rule_def_list},
		
		
		{ L"rule_def			: 		lexeme : rhs_list ;",							__handle_rule_def},
		{ L"rhs_list			: 		rhs_list  | rhs ",								__handle_rhs_list},
		{ L"rhs_list			: 		rhs ",											__handle_rhs_list},
		{ L"rhs					:		term_list prec_decl handler_decl",				__handle_rhs},
		{ L"rhs					:		.",												__handle_rhs},
		{ L"term_list			:		term_list lexeme",								__handle_term_list},
		{ L"term_list			:		lexeme",										__handle_term_list},
		{ L"prec_decl			:		%prec lexeme",									__build_prec_decl},
		{ L"prec_decl			:		",												NULL},
		{ L"handler_decl		:		%handler lexeme",								__handle_handler_decl},
		{ L"handler_decl		:				",										NULL}
};




static lex_t* __build_lex(void *io)
{
		lex_t *lex;
		int_t	i;

		lex = LEX_Create(io);

		AR_ASSERT(lex != NULL);
		
		for(i = 0; i < AR_NELEMS(__cfg_lex_name); ++i)
		{
				if(!LEX_Insert(lex, __cfg_lex_name[i]))
				{
						AR_ASSERT(false);
						AR_abort();

				}
		}

		for(i = 0; i < AR_NELEMS(__cfg_pattern); ++i)
		{
				lexAction_t action;
				action.is_skip = __cfg_pattern[i].is_skip;
				action.priority = __cfg_pattern[i].prec;
				action.type = (size_t)__cfg_pattern[i].val;

				if(!LEX_InsertRule(lex, __cfg_pattern[i].regex, &action))
				{
						AR_ASSERT(false);
						AR_abort();

				}
		}

		
		if(!LEX_GenerateTransTable(lex))
		{
				AR_ASSERT(false);
				AR_abort();
		}
		
		return lex;
}




static parser_t*	__build_parser(void *io)
{
		psrGrammar_t	*gmr;
		parser_t		*parser;
		size_t i;
		
		gmr = PSR_CreateGrammar(io);

		for(i = 0; i < AR_NELEMS(__cfg_term); ++i)
		{
				if(!PSR_InsertTerm(gmr, __cfg_term[i].name, (size_t)__cfg_term[i].val, PSR_ASSOC_NOASSOC, 0, __build_leaf))
				{
						AR_abort();
				}
		}

		for(i = 0; i < AR_NELEMS(__cfg_rule); ++i)
		{
				if(!PSR_InsertRuleByStr(gmr, __cfg_rule[i].rule, NULL, __cfg_rule[i].handler))
				{
						AR_abort();
				}

		}


		if(gmr == NULL || !PSR_CheckIsValidGrammar(gmr))
		{
				AR_printf(L"Internal Error : %ls\r\n", AR_WSTR(__FUNCSIG__));
				AR_abort();
		}

		{

				psrCtx_t		psr_ctx = {cfg_free,  NULL};
				parser = PSR_CreateParser(gmr, PSR_LALR, &psr_ctx);
		}

		return parser;
}

static void		__destroy_parser(parser_t *parser)
{
		AR_ASSERT(parser != NULL);

		PSR_DestroyGrammar((psrGrammar_t*)parser->grammar);
		PSR_DestroyParser(parser);
}


cfgConfig_t*	CFG_CollectGrammarConfig(const wchar_t *gmr_txt, void *io)
{
		
		bool_t is_ok;
		lex_t *lex;
		lexMatch_t match;
		lexToken_t		tok;
		psrToken_t		term;
		parser_t		*parser;
		cfgNode_t		*result = NULL;
		AR_ASSERT(gmr_txt != NULL);
		LEX_InitMatch(&match, gmr_txt);

		lex = __build_lex(io);
		


		parser = __build_parser(io);
		
		is_ok = true;
		
		while(is_ok)
		{
				is_ok = LEX_Match(lex, &match, &tok);

				if(!is_ok)
				{
						const wchar_t *tok = NULL;
						size_t n = AR_wcslen(match.next);
						tok = AR_wcsndup(match.next, n > 10 ? 10 : n);
						AR_printf(L"Invalid input %ls...(%"AR_PLAT_INT_FMT L"d : %"AR_PLAT_INT_FMT L"d)\r\n", tok, match.line, match.col);

						continue;
				}
				/*
				AR_printf(L"tok->name %ls : (%d : %d)\r\n", AR_wcsndup(tok.str, tok.count), tok.line, tok.col);
				getchar();
				*/
				
				PSR_TOTERMTOK(&tok, &term);

				is_ok = PSR_AddToken(parser, &term);
				
				if(tok.type == EOI)break;

				//AR_printf(L"%ls : %d\r\n", AR_wcsndup(tok.str, tok.count), tok.type);
		}
		
		if(is_ok)
		{
				result = (cfgNode_t*)PSR_GetResult(parser);
				
				AR_ASSERT(result->type == CFG_CONFIG_T);
		}


		__destroy_parser(parser);
		LEX_UnInitMatch(&match);

		
		LEX_Destroy(lex);
		
		return (cfgConfig_t*)result;

}

void			CFG_DestroyGrammarConfig(cfgConfig_t *cfg)
{
		CFG_DestroyNode((cfgNode_t*)cfg);

}








AR_NAMESPACE_END


