
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
		CFG_EMPTY_T,
		CFG_LEXEME_T,
		CFG_NAME_T,
		CFG_TOKEN_T,
		CFG_PREC_T,
		CFG_START_T,
		CFG_RULE_T,
		CFG_NODE_LIST_T,
		CFG_CONFIG_T,
		CFG_ERROR_T
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
				cfgStart_t		start;
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
		L"	lexeme		=	{name}|(\\\"([^\\\"\\n])+\\\")|('([^'\\n])+')",
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
		START,
		NAME,
		TOKEN,
		PREC,
		ASSOC,
		LEXEME,
		NUMBER,
		DOT,
		COMMA,
		COLON,
		SEMI,
		OR,
		FAKE_EOI,
		ACTION,
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
		{EOI,	L"$", false,2},
		{DELIM, L"{skip_lexem}+", true, 1},
		{ASSOC,	L"\"%\"(\"left\"|\"right\"|\"noassoc\")", false,1},

		{SKIP,	L"\"%skip\"", false,0},
		{START,	L"\"%start\"", false,0},
		{NAME,	L"\"%name\"(?={key_lookahead})", false,0},
		{TOKEN,	L"\"%token\"(?={key_lookahead})", false,0},
		{PREC,	L"\"%prec\"(?={key_lookahead})", false,0},
		/*{HANDLER, L"\"%handler\"(?={skip_lexem}+)", false, 0},*/

		{ACTION, L"\"%action\"(?={skip_lexem}+)", false, 0},

		{LEXEME,	L"{lexeme}", false,0},
		{NUMBER,	L"{number}", false,0},

		{DOT,		L"\".\"",	false,1},
		{COMMA,		L"\",\"",	false,1},

		{COLON,		L"\":\"",	false,1},
		{SEMI,		L"\";\"",	false,1},

		{OR,		L"\"|\"",	false,1}
};













typedef struct	__cfg_term_info_tag
{
		cfgLexValue_t	val;
		const wchar_t	*name;
}cfgTermInfo_t;


static const cfgTermInfo_t	__cfg_term[] =
{
		{SKIP, L"%skip"},
		{START, L"%start"},
		{NAME, L"%name"},
		{TOKEN, L"%token"},
		{PREC, L"%prec"},
		{ACTION, L"%action"},
		{ASSOC, L"assoc"},
		{LEXEME, L"lexeme"},
		{NUMBER, L"number"},

		{DOT, L"."},
		{COMMA, L","},
		{COLON, L":"},
		{SEMI, L";"},
		{OR,   L"|"},
		{FAKE_EOI, L"#"}
};

/*
typedef struct __cfg_prec_tag
{
		size_t	line;
		
		psrAssocType_t	assoc;
		size_t			prec_level;

		size_t			*prec_tok_val;
		wchar_t			**prec_tok_set;
		size_t			count;
		size_t			cap;
}cfgPrec_t;
*/

static void CFG_InitPrec(cfgPrec_t *prec)
{
		AR_ASSERT(prec != NULL);
		AR_memset(prec, 0, sizeof(*prec));
}


static void CFG_UnInitPrec(cfgPrec_t *prec)
{
		size_t i;
		AR_ASSERT(prec != NULL);
		
		for(i = 0; i < prec->count; ++i)
		{
				AR_ASSERT(prec->prec_tok_set[i] != NULL);
				AR_DEL(prec->prec_tok_set[i]);
		}

		if(prec->prec_tok_set) AR_DEL(prec->prec_tok_set);
		if(prec->prec_tok_val) AR_DEL(prec->prec_tok_val);
		AR_memset(prec, 0, sizeof(*prec));
}

static void CFG_InsertTokenToPrec(cfgPrec_t *prec, const wchar_t *prec_tok, size_t prec_tok_val)
{
		AR_ASSERT(prec != NULL && prec_tok != NULL);

		if(prec->count == prec->cap)
		{
				prec->cap = (prec->cap + 4)*2;
				prec->prec_tok_set = AR_REALLOC(wchar_t*, prec->prec_tok_set, prec->cap);
				prec->prec_tok_val = AR_REALLOC(size_t, prec->prec_tok_val, prec->cap);
		}

		prec->prec_tok_set[prec->count] = AR_wcsdup(prec_tok);
		prec->prec_tok_val[prec->count] = prec_tok_val;
		prec->count++;
}


static void CFG_CopyPrec(cfgPrec_t *dest, const cfgPrec_t *sour)
{
		size_t i;
		CFG_UnInitPrec(dest);

		dest->assoc = sour->assoc;
		dest->prec_level = sour->prec_level;
		dest->line = sour->line;
		for(i = 0; i < sour->count; ++i)
		{
				CFG_InsertTokenToPrec(dest, sour->prec_tok_set[i], sour->prec_tok_val[i]);
		}
}

/*******************************************************************/
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




static void CFG_InitConfig(cfgConfig_t *cfg, cfgNodeList_t *name, cfgNodeList_t *token, cfgNodeList_t *prec, cfgNodeList_t *rule, cfgStart_t *start_rule)
{
		size_t i;
		size_t tmp_tok_val = PSR_MIN_TOKENVAL;

		AR_ASSERT(cfg != NULL);
		
		AR_memset(cfg, 0, sizeof(*cfg));

		if(name)
		{
				//cfg->name_cnt = name->count;
				cfg->name = name->count > 0 ? AR_NEWARR0(cfgName_t, name->count) : NULL;

				for(i = 0; i < name->count; ++i)
				{
						if(name->lst[i]->type == CFG_ERROR_T)
						{
								cfg->has_error = true;
						}else
						{
								cfg->name[cfg->name_cnt].line = name->lst[i]->name.line;
								cfg->name[cfg->name_cnt].name = AR_wcsdup(name->lst[i]->name.name);
								cfg->name[cfg->name_cnt].regex = AR_wcsdup(name->lst[i]->name.regex);
								cfg->name_cnt++;
						}
				}
		}


		if(token)
		{
				size_t k;
				

				/*cfg->tok_cnt = token->count + 1;*/

				/*cfg->tok = cfg->tok_cnt > 0 ? AR_NEWARR0(cfgToken_t, cfg->tok_cnt + 1) : NULL;*/

				cfg->tok = AR_NEWARR0(cfgToken_t, token->count + 1);

				for(i = 0; i < token->count; ++i)
				{
						if(token->lst[i]->type == CFG_ERROR_T)
						{
								cfg->has_error = true;
						}else
						{
								cfg->tok[cfg->tok_cnt] = token->lst[i]->token;
								cfg->tok[cfg->tok_cnt].name = token->lst[i]->token.name != NULL ? AR_wcsdup(token->lst[i]->token.name) : NULL;
								cfg->tok[cfg->tok_cnt].regex = AR_wcsdup(token->lst[i]->token.regex);
								cfg->tok_cnt++;
						}
				}

				for(i = 0; i < cfg->tok_cnt ; ++i)
				{
						if(cfg->tok[i].tokval == 0)
						{
RE_CHECK_POINT:
								for(k = 0; k < cfg->tok_cnt; ++k)
								{
										if(cfg->tok[k].tokval == tmp_tok_val)
										{
												tmp_tok_val++;
												k = 0;
												goto RE_CHECK_POINT;
										}
								}
								cfg->tok[i].tokval = tmp_tok_val++;
						}
				}


				/*EOI*/
				cfg->tok[cfg->tok_cnt ].is_skip = false;
				cfg->tok[cfg->tok_cnt ].lex_prec = 2;
				cfg->tok[cfg->tok_cnt ].line = 0;
				cfg->tok[cfg->tok_cnt ].tokval = 0;
				cfg->tok[cfg->tok_cnt ].name = AR_wcsdup(L"EOI");
				cfg->tok[cfg->tok_cnt ].regex = AR_wcsdup(L"$");

				cfg->tok_cnt++;

		}


		if(prec)
		{
				/*cfg->prec_cnt = prec->count;*/
				cfg->prec = prec->count > 0 ? AR_NEWARR0(cfgPrec_t, prec->count) : NULL;

				for(i = 0; i < prec->count; ++i)
				{

						if(prec->lst[i]->type == CFG_ERROR_T)
						{
								cfg->has_error = true;

						}else
						{
								size_t k;
								cfgPrec_t *dest = &cfg->prec[cfg->prec_cnt];
								CFG_InitPrec(dest);
								
								CFG_CopyPrec(dest, &prec->lst[i]->prec);

								for(k = 0; k < dest->count; ++k)
								{
										size_t x;
										for(x = 0; x < cfg->tok_cnt; ++x)
										{
												if(cfg->tok[x].is_skip)continue;

												AR_ASSERT(cfg->tok[x].name != NULL);
												if(AR_wcscmp(dest->prec_tok_set[k], cfg->tok[x].name) == 0)
												{
														dest->prec_tok_val[k] = cfg->tok[x].tokval;
														break;
												}
										}

										if(dest->prec_tok_val[k] == 0)
										{
												dest->prec_tok_val[k] = tmp_tok_val++;
										}

								}

								dest->prec_level = i + 1;
								cfg->prec_cnt++;
						}
				}
		}


		if(rule)
		{
				/*cfg->rule_cnt = rule->count;*/

				cfg->rule = rule->count > 0 ? AR_NEWARR0(cfgRule_t, rule->count) : NULL;
				

				for(i = 0; i < rule->count; ++i)
				{
						if(rule->lst[i]->type == CFG_ERROR_T)
						{
								cfg->has_error = true;
						}else
						{
								cfg->rule[cfg->rule_cnt].line = rule->lst[i]->rule.line;

								cfg->rule[cfg->rule_cnt].lhs = AR_wcsdup(rule->lst[i]->rule.lhs);
								cfg->rule[cfg->rule_cnt].rhs = AR_wcsdup(rule->lst[i]->rule.rhs);

								if(rule->lst[i]->rule.prec_tok)
								{
										cfg->rule[cfg->rule_cnt].prec_tok = AR_wcsdup(rule->lst[i]->rule.prec_tok);
								}

								if(rule->lst[i]->rule.action_ins)
								{
										cfg->rule[cfg->rule_cnt].action_ins = AR_wcsdup(rule->lst[i]->rule.action_ins);
								}

								cfg->rule_cnt++;
						}
				}

		}

		
		if(start_rule != NULL)
		{
				cfg->start.line = start_rule->line;
				cfg->start.start_rule = AR_wcsdup(start_rule->start_rule);

		}else
		{
				cfg->start.start_rule = NULL;
				cfg->start.line = 0;
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
				CFG_UnInitPrec(&cfg->prec[i]);
				/*if(cfg->prec[i].prec_tok)AR_DEL(cfg->prec[i].prec_tok);*/
		}

		if(cfg->prec)AR_DEL(cfg->prec);

		if(cfg->start.start_rule)
		{
				AR_DEL(cfg->start.start_rule);
		}

		for(i = 0; i < cfg->rule_cnt; ++i)
		{
				AR_DEL(cfg->rule[i].lhs);
				AR_DEL(cfg->rule[i].rhs);

				if(cfg->rule[i].prec_tok)AR_DEL(cfg->rule[i].prec_tok);
				if(cfg->rule[i].action_ins)AR_DEL(cfg->rule[i].action_ins);
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
		}else if(node->type == CFG_PREC_T)
		{
				CFG_InitPrec(&node->prec);
		}
		return node;
}


static void CFG_DestroyNode(cfgNode_t *node)
{
		if(node)
		{
				
				switch(node->type)
				{
				case CFG_EMPTY_T:
				{
						break;
				}
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
						CFG_UnInitPrec(&node->prec);
						break;
				}
				case CFG_START_T:
				{
						if(node->start.start_rule)
						{
								AR_DEL(node->start.start_rule);
						}
						break;
				}
				case CFG_RULE_T:
				{
						if(node->rule.lhs)AR_DEL(node->rule.lhs);
						if(node->rule.rhs)AR_DEL(node->rule.rhs);
						if(node->rule.prec_tok)AR_DEL(node->rule.prec_tok);
						if(node->rule.action_ins)AR_DEL(node->rule.action_ins);
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
				case CFG_ERROR_T:
				{
						break;
				}
				default:
				{
						AR_ASSERT(false);
						AR_error(AR_ERR_FATAL, L"%hs\r\n", AR_FUNC_NAME);
				}
				}

				AR_DEL(node);
		}
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
{ L"handler_decl		:		%action lexeme",								NULL},
{ L"handler_decl		:				",										NULL},
*/
static psrNode_t*		AR_STDCALL __handle_action_decl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
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
{ L"rhs					:		. prec_decl handler_decl",				NULL},
*/

static psrNode_t*		AR_STDCALL __handle_rhs(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		size_t i;
		AR_ASSERT(count == 3 && nodes != NULL);

		res = CFG_CreateNode(CFG_RULE_T);
		res->rule.lhs = NULL;
		res->rule.line = 0;
		res->rule.prec_tok = NULL;
		res->rule.rhs = AR_NEWARR0(wchar_t, 2048);

		//AR_ASSERT(ns[0] && ns[0]->type == CFG_NODE_LIST_T && ns[0]->lst.count > 0);
		AR_ASSERT(ns[0]);
		AR_ASSERT(ns[1] ? ns[1]->type == CFG_LEXEME_T : true);
		AR_ASSERT(ns[2] ? ns[2]->type == CFG_LEXEME_T : true);


		if(ns[0]->type == CFG_LEXEME_T && ns[0]->lexeme.lex_val == DOT)
		{
				AR_wcscat((wchar_t*)res->rule.rhs, L" ");

		}else if(ns[0]->type == CFG_NODE_LIST_T)
		{
				for(i = 0; i < ns[0]->lst.count; ++i)
				{
						cfgNode_t *tmp = ns[0]->lst.lst[i];

						AR_ASSERT(tmp->type == CFG_LEXEME_T && tmp->lexeme.lex_val == (size_t)LEXEME);

						AR_wcscat((wchar_t*)res->rule.rhs, tmp->lexeme.lexeme);
						AR_wcscat((wchar_t*)res->rule.rhs, L" ");
				}
		}else
		{
				AR_ASSERT(false);
		}


		if(ns[1])
		{
				res->rule.prec_tok = ns[1]->lexeme.lexeme;
				ns[1]->lexeme.lexeme = NULL;
		}

		if(ns[2])
		{
				res->rule.action_ins = ns[2]->lexeme.lexeme;
				ns[2]->lexeme.lexeme = NULL;
		}

		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		CFG_DestroyNode(ns[2]);
		ns[0] = ns[1] = ns[2] = NULL;

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
{ L"rule_def			: 		lexeme : rhs_list ",							__handle_rule_def},
*/

static psrNode_t*		AR_STDCALL __handle_rule_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		size_t i;
		AR_ASSERT(count == 3);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_NODE_LIST_T);


		AR_ASSERT(ns[0]->lexeme.lexeme != NULL);

		for(i = 0; i < ns[2]->lst.count; ++i)
		{
				cfgNode_t *tmp = ns[2]->lst.lst[i];

				AR_ASSERT(tmp->type == CFG_RULE_T);
				tmp->rule.lhs = AR_wcsdup(ns[0]->lexeme.lexeme);
				tmp->rule.line = ns[0]->lexeme.line;
		}
		
		res = ns[2];
		ns[2] = NULL;
		
		return res;
}





/*{ L"prec_def			:  		assoc lexeme ",								__handle_prec_def},*/
static psrNode_t*		AR_STDCALL __handle_prec_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		wchar_t			c;
		size_t			i;
		AR_ASSERT(count == 2);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T &&  ns[0]->lexeme.lex_val == ASSOC && ns[1] && ns[1]->type == CFG_NODE_LIST_T);


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
				AR_ASSERT(false);
				AR_error(AR_ERR_FATAL, L"Arsenal Tools Internal Error In : %hs\r\n", AR_FUNC_NAME);
		}

		for(i = 0; i < ns[1]->lst.count; ++i)
		{
				const cfgNode_t *node = ns[1]->lst.lst[i];
				AR_ASSERT(node->type == CFG_LEXEME_T);
				CFG_InsertTokenToPrec(&res->prec, node->lexeme.lexeme, 0);
		}
		return res;

}






/*
{ L"token_def			:  		%token %skip token_val_prec : lexeme token_val_prec ",	__handle_token_def},
{ L"token_def			:  		%token lexeme token_val_prec : lexeme token_val_prec ",	__handle_token_def},
*/


static psrNode_t*		AR_STDCALL __handle_token_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 6);

		/*AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_LEXEME_T && ns[4] && ns[4]->type == CFG_LEXEME_T);*/

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T 
				&& ns[1] && ns[1]->type == CFG_LEXEME_T 
				&& ns[3] && ns[3]->type == CFG_LEXEME_T
				&& ns[4] && ns[4]->type == CFG_LEXEME_T
				);

		AR_ASSERT((ns[2] == NULL) || (ns[2]->type == CFG_LEXEME_T));
		AR_ASSERT((ns[5] == NULL) || (ns[5]->type == CFG_LEXEME_T));



		res = CFG_CreateNode(CFG_TOKEN_T);

		res->token.line = ns[1]->lexeme.line;

		if(ns[1]->lexeme.lex_val == SKIP)
		{
				res->token.is_skip = true;
				res->token.name = NULL;
		}else
		{
				res->token.is_skip = false;

				res->token.name = ns[1]->lexeme.lexeme;
				ns[1]->lexeme.lexeme = NULL;
		}

		res->token.regex = ns[4]->lexeme.lexeme;
		ns[4]->lexeme.lexeme = NULL;

		res->token.lex_prec = 0;

		if(ns[2])
		{
				AR_wtou(ns[2]->lexeme.lexeme, &res->token.tokval, 10);
		}else
		{
				res->token.tokval = 0;
		}

		if(ns[5])
		{
				AR_wtou(ns[5]->lexeme.lexeme, &res->token.lex_prec, 10);
		}

		return res;

}





/*
{ L"name_def 			:		%name lexeme : lexeme  ",								__handle_name_def},
*/

static psrNode_t*		AR_STDCALL __handle_name_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 4);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T	
				&& ns[1] && ns[1]->type == CFG_LEXEME_T 
				&& ns[2] && ns[2]->type == CFG_LEXEME_T 
				&& ns[3] && ns[3]->type == CFG_LEXEME_T
				);

		res = CFG_CreateNode(CFG_NAME_T);

		res->name.line = ns[1]->lexeme.line;
		res->name.name = ns[1]->lexeme.lexeme;
		ns[1]->lexeme.lexeme = NULL;

		res->name.regex = ns[3]->lexeme.lexeme;
		ns[3]->lexeme.lexeme = NULL;

		return res;
}


/*
{ L"item				:		name_def ;",	__handle_item},
{ L"item				:		token_def ;",	__handle_item},
{ L"item				:		prec_def ;",	__handle_item},
{ L"item				:		rule_def ;",	__handle_item},
{ L"item				:		error ; ",	__handle_item},
{ L"item				:		; ",	__handle_item},
*/

static psrNode_t*		AR_STDCALL __handle_item(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res = NULL;

		AR_ASSERT(count == 1 || count == 2);

		if(count == 2)
		{
				if(ns[0] == NULL)
				{
						res = CFG_CreateNode(CFG_ERROR_T);
				}else
				{
						res = ns[0];
						ns[0] = NULL;
				}

		}else
		{
				res = CFG_CreateNode(CFG_EMPTY_T);
		}
		return (psrNode_t*)res;
}

/*
{ L"item_list			:		item_list item",	__handle_item_list},
{ L"item_list			:		",	NULL},
*/

static psrNode_t*		AR_STDCALL __handle_item_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 2);
		
		AR_ASSERT((ns[0] == NULL) || (ns[0] && ns[0]->type == CFG_NODE_LIST_T));
		AR_ASSERT(ns[1]);

		res = ns[0];
		if(res == NULL)res = CFG_CreateNode(CFG_NODE_LIST_T);
		ns[0] = NULL;
				
		if(ns[1]->type == CFG_NODE_LIST_T)
		{
				size_t i;
				for(i = 0; i < ns[1]->lst.count; ++i)
				{
						CFG_InsertToNodeList(&res->lst, ns[1]->lst.lst[i]);
						ns[1]->lst.lst[i] = NULL;
				}

		}else
		{
				CFG_InsertToNodeList(&res->lst, ns[1]);
				ns[1] = NULL;
		}
		
		
		return (psrNode_t*)res;
}

/*
{ L"start_def			:  		%start lexeme",							__handle_start_def,0},
*/

static psrNode_t*		AR_STDCALL __handle_start_def(psrNode_t **nodes, size_t count, const wchar_t * name_tmp, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 2);
		
		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[0]->lexeme.lex_val == START);
		AR_ASSERT(ns[1] && ns[1]->type == CFG_LEXEME_T);


		res = CFG_CreateNode(CFG_START_T);
		res->start.line = ns[1]->lexeme.line;
		res->start.start_rule = AR_wcsdup(ns[1]->lexeme.lexeme);
		return res;
		
}

/*
{ L"program				:		item_list #",			__handle_program, 0},
{ L"program				:		item_list error #",		__handle_program, 0},
*/
static psrNode_t*		AR_STDCALL __handle_program(psrNode_t **nodes, size_t count, const wchar_t * name_tmp, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res = NULL;
		size_t	i;
		bool_t			has_err = false;
		cfgNodeList_t name, token, prec, rule, error, empty;
		cfgStart_t		*start_rule = NULL;

		AR_ASSERT(count == 2 || count == 3);
		AR_ASSERT((ns[0] == NULL) || (ns[0] && ns[0]->type == CFG_NODE_LIST_T));
		
		if(count == 2)
		{
				AR_ASSERT(ns[1]->type == CFG_LEXEME_T && ns[1]->lexeme.lex_val == FAKE_EOI);
		}else
		{
				AR_ASSERT(ns[1] == NULL && ns[2]->type == CFG_LEXEME_T && ns[2]->lexeme.lex_val == FAKE_EOI);
				has_err = true;
		}

		CFG_InitNodeList(&name);
		CFG_InitNodeList(&token);
		CFG_InitNodeList(&prec);
		CFG_InitNodeList(&rule);
		CFG_InitNodeList(&error);
		CFG_InitNodeList(&empty);
		

		for(i = 0; ns[0] && i < ns[0]->lst.count; ++i)
		{
				cfgNode_t *node = ns[0]->lst.lst[i];
				AR_ASSERT(node);
				
				switch(node->type)
				{
				case CFG_EMPTY_T:
				{
						CFG_InsertToNodeList(&empty, node);
						break;
				}
				case CFG_NAME_T:
				{
						CFG_InsertToNodeList(&name, node);
						break;
				}
				case CFG_TOKEN_T:
				{
						CFG_InsertToNodeList(&token, node);
						break;
				}
				case CFG_PREC_T:
				{
						CFG_InsertToNodeList(&prec, node);
						break;
				}
				case CFG_START_T:
				{
						start_rule = &node->start;
						break;
				}
				case CFG_RULE_T:
				{
						CFG_InsertToNodeList(&rule, node);
						break;
				}
				case CFG_ERROR_T:
				{
						CFG_InsertToNodeList(&error, node);
						break;
				}
				case CFG_NODE_LIST_T:
				case CFG_CONFIG_T:
				case CFG_LEXEME_T:
				default:
				{
						AR_ASSERT(false);
						AR_error(AR_ERR_FATAL, L"Arsenal Tools Internal Error In : %hs\r\n", AR_FUNC_NAME);
				}
				}
		}

		res = CFG_CreateNode(CFG_CONFIG_T);
		CFG_InitConfig(&res->config, &name, &token, &prec, &rule, start_rule);

		if(error.count > 0 || has_err)
		{
				res->config.has_error = true;
		}

		CFG_UnInitNodeList(&name);
		CFG_UnInitNodeList(&token);
		CFG_UnInitNodeList(&prec);
		CFG_UnInitNodeList(&rule);
		CFG_UnInitNodeList(&error);
		CFG_UnInitNodeList(&empty);
		return (psrNode_t*)res;
}



typedef struct __cfg_rule_tag
{
		const wchar_t *rule;
		psrRuleFunc_t handler;
		size_t			auto_ret;
}cfgRuleDef_t;



static const cfgRuleDef_t	__cfg_rule[] =
{

		
		{ L"item_list			:		item_list item",	__handle_item_list, 0},
		{ L"item_list			:		",	NULL, 0},
		
		{ L"item				:		start_def ;",	__handle_item, 0},
		{ L"item				:		name_def ;",	__handle_item, 0},
		{ L"item				:		token_def ;",	__handle_item,0},
		{ L"item				:		prec_def ;",	__handle_item,0},
		{ L"item				:		rule_def ;",	__handle_item,0},
		{ L"item				:		error ; ",	__handle_item,0},
		{ L"item				:		; ",	__handle_item,0},

		{ L"name_def 			:		%name lexeme : lexeme ",								__handle_name_def,0},
		
		{ L"token_def			:  		%token %skip token_val_prec : lexeme token_val_prec ",	__handle_token_def,0},
		{ L"token_def			:  		%token lexeme token_val_prec : lexeme token_val_prec ",	__handle_token_def,0},

		{ L"token_val_prec 		:		, number",												NULL,1},
		{ L"token_val_prec 		:		",														NULL,0},
		

		{ L"prec_def			:  		assoc term_list ",								__handle_prec_def,0},

		
		{ L"rule_def			: 		lexeme : rhs_list ",							__handle_rule_def,0},
		

		{ L"rhs_list			: 		rhs_list  | rhs ",								__handle_rhs_list,0},
		{ L"rhs_list			: 		rhs ",											__handle_rhs_list,0},
		{ L"rhs					:		term_list prec_decl action_decl",				__handle_rhs,0},
		{ L"rhs					:		. prec_decl action_decl",						__handle_rhs,0},

		{ L"term_list			:		term_list lexeme",								__handle_term_list,0},
		{ L"term_list			:		lexeme",										__handle_term_list,0},




		{ L"prec_decl			:		%prec lexeme",									NULL,1},

		{ L"prec_decl			:		",												NULL,0},
		{ L"action_decl			:		%action lexeme",								__handle_action_decl,0},
		{ L"action_decl			:		",										NULL,0},

		{ L"start_def			:  		%start lexeme",							__handle_start_def,0},
		

		{ L"program				:		item_list #",			__handle_program, 0},
		{ L"program				:		item_list error #",		__handle_program, 0}

};


static lex_t* __build_lex(arIOCtx_t		*io)
{
		lex_t *lex;
		size_t	i;

		lex = LEX_Create(io);

		AR_ASSERT(lex != NULL);

		for(i = 0; i < AR_NELEMS(__cfg_lex_name); ++i)
		{
				if(!LEX_Insert(lex, __cfg_lex_name[i]))
				{
						AR_ASSERT(false);
						AR_error(AR_ERR_FATAL, L"%hs\r\n", AR_FUNC_NAME);

				}
		}

		for(i = 0; i < AR_NELEMS(__cfg_pattern); ++i)
		{
				lexAction_t action;
				action.is_skip = __cfg_pattern[i].is_skip;
				action.priority = __cfg_pattern[i].prec;
				action.value = (size_t)__cfg_pattern[i].val;

				if(!LEX_InsertRule(lex, __cfg_pattern[i].regex, &action))
				{
						AR_ASSERT(false);
						AR_error(AR_ERR_FATAL, L"%hs\r\n", AR_FUNC_NAME);

				}
		}


		if(!LEX_GenerateTransTable(lex))
		{
				AR_ASSERT(false);
				AR_error(AR_ERR_FATAL, L"%hs\r\n", AR_FUNC_NAME);
		}

		return lex;
}


static psrGrammar_t*	__build_grammar(psrCtx_t *psr_ctx, arIOCtx_t *io)
{
		psrGrammar_t	*gmr;
		size_t i;
		AR_ASSERT(psr_ctx != NULL);
		
		gmr = PSR_CreateGrammar(psr_ctx, io);
		
		if(gmr == NULL)return NULL;

		for(i = 0; i < AR_NELEMS(__cfg_term); ++i)
		{
				if(!PSR_InsertTerm(gmr, __cfg_term[i].name, (size_t)__cfg_term[i].val, PSR_ASSOC_NOASSOC, 0, __build_leaf))
				{
						AR_ASSERT(false);
						AR_error(AR_ERR_FATAL, L"%hs\r\n", AR_FUNC_NAME);
				}
		}

		for(i = 0; i < AR_NELEMS(__cfg_rule); ++i)
		{
				if(!PSR_InsertRuleByStr(gmr, __cfg_rule[i].rule, NULL, __cfg_rule[i].handler, __cfg_rule[i].auto_ret))
				{
						AR_ASSERT(false);
						AR_error(AR_ERR_FATAL, L"%hs\r\n", AR_FUNC_NAME);
				}

		}

		if(!PSR_SetFirstRule(gmr, L"program"))
		{
				AR_ASSERT(false);
				AR_error(AR_ERR_FATAL, L"%hs\r\n", AR_FUNC_NAME);
				return NULL;
		}
		
		if(!PSR_CheckIsValidGrammar(gmr))
		{
				AR_ASSERT(false);
				AR_error(AR_ERR_FATAL, L"Arsenal Internal Error : %hs\r\n", AR_FUNC_NAME);
		}
		
		return gmr;
}

static parser_t*		__build_parser(const psrGrammar_t *gmr)
{
		parser_t *parser;
		AR_ASSERT(gmr && PSR_CheckIsValidGrammar(gmr));
				
		parser = PSR_CreateParser(gmr, PSR_SLR);
		AR_ASSERT(parser && PSR_CountParserConflict(parser) == 0);
		return parser;
}


/*
typedef void	(AR_STDCALL *AR_error_func)(int_t level, const wchar_t *msg, void *ctx);
typedef void	(AR_STDCALL *AR_print_func)(const wchar_t *msg, void *ctx);
*/



static void	AR_STDCALL cfg_on_error(int_t level, const wchar_t *msg, void *ctx)
{
		cfgReport_t				*report = NULL;
		cfgReportInfo_t			info;
		AR_ASSERT(msg != NULL && ctx != NULL);

		report = (cfgReport_t*)ctx;

		AR_memset(&info, 0, sizeof(info));

		info.type = CFG_REPORT_ERROR_T;
		info.tok = NULL;
		info.message = msg;
		info.err_level = level;
		report->report_func(&info, report->report_ctx);
}


void	AR_STDCALL cfg_on_print(const wchar_t *msg, void *ctx)
{
		cfgReport_t				*report = NULL;
		cfgReportInfo_t			info;
		AR_ASSERT(msg != NULL && ctx != NULL);

		report = (cfgReport_t*)ctx;

		AR_memset(&info, 0, sizeof(info));

		info.type = CFG_REPORT_MESSAGE_T;
		info.tok = NULL;
		info.message = msg;
		info.err_level = 0;
		report->report_func(&info, report->report_ctx);
}



static void	AR_STDCALL cfg_free(psrNode_t *node, void *ctx)
{
		CFG_DestroyNode((cfgNode_t*)node);
}

static void		AR_STDCALL cfg_error(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx)
{
		cfgReport_t				*report = NULL;
		cfgReportInfo_t			info;

		arString_t		*str;
		wchar_t			*buf;
		size_t			i;

/*
		if(tok->term_val == FAKE_EOI)
		{
				return;
		}
*/

		if(ctx == NULL)return;

		AR_ASSERT(tok != NULL && ctx != NULL && expected != NULL && count > 0);
		
		report = (cfgReport_t*)ctx;
		AR_memset(&info, 0, sizeof(info));

		/******************************************************************************************/
		buf = tok->str_cnt == 0 ? AR_wcsdup(L"%EOI") : AR_wcsndup(tok->str, tok->str_cnt);
		str = AR_CreateString();
		
		AR_AppendFormatString(str
						, L"Invalid Token \"%ls\" in (%" AR_PLAT_INT_FMT L"d : %" AR_PLAT_INT_FMT L"d)\r\n\r\n"
						, buf
						, tok->line
						, tok->col
						);
		AR_DEL(buf);
		AR_AppendFormatString(str, L"Expected Term Type:\r\n");

		for(i = 0; i < count; ++i)
		{
				AR_AppendFormatString(str, L"\"%ls\" ", expected[i]);
		}
		AR_AppendFormatString(str, L"\r\n\r\n");
		/******************************************************************************************/
		info.message = AR_GetStrString(str);
		info.tok = tok;
		info.type = CFG_REPORT_ERR_SYNTAX_T;
		report->report_func(&info, report->report_ctx);
		AR_DestroyString(str);
}





cfgConfig_t*	CFG_CollectGrammarConfig(const wchar_t *gmr_txt, cfgReport_t *report)
{

		bool_t is_ok, has_error;
		lex_t *lex;
		lexMatch_t match;
		lexToken_t		tok;
		psrToken_t		term;
		psrGrammar_t	*gmr;
		parser_t		*parser;
		cfgNode_t		*result = NULL;

		arIOCtx_t	io_ctx ;
		psrCtx_t	psr_ctx;
		
		AR_ASSERT(gmr_txt != NULL && report != NULL);

		io_ctx.on_error = cfg_on_error;
		io_ctx.on_print = cfg_on_print;
		io_ctx.ctx = (void*)report;

		psr_ctx.error_f = cfg_error;
		psr_ctx.free_f = cfg_free;
		psr_ctx.ctx = (void*)report;
		

		
		

		lex = __build_lex(&io_ctx);

		gmr = __build_grammar(&psr_ctx, &io_ctx);
		
		parser = __build_parser(gmr);

		LEX_InitMatch(&match, gmr_txt);

		is_ok = true;
		has_error = false;
		

		while(is_ok)
		{
				is_ok = LEX_Match(lex, &match, &tok);

				if(!is_ok)
				{
						cfgReportInfo_t	info;
						psrToken_t		tmp_tok;
						arString_t		*str;
						size_t n;
						const wchar_t *tok = NULL;

						AR_memset(&info, 0, sizeof(info));
						n = AR_wcslen(match.next);
						tok = AR_wcsndup(match.next, n > 5 ? 5 : n);
						
						str = AR_CreateString();

						AR_AppendFormatString(str, L"Invalid Token %ls...(%"AR_PLAT_INT_FMT L"d : %"AR_PLAT_INT_FMT L"d)\r\n", tok, match.line, match.col);
						if(tok)AR_DEL(tok);

						info.type = CFG_REPORT_ERR_LEX_T;
						info.message = AR_GetStrString(str);


						tmp_tok.term_val = 0;
						tmp_tok.str_cnt = 0;
						tmp_tok.str =  match.input;
						tmp_tok.line = match.line;
						tmp_tok.col = match.col;
						info.tok = &tmp_tok;
						info.err_level = 0;
						
						report->report_func(&info, report->report_ctx);

						AR_DestroyString(str);
						
						AR_ASSERT(*match.next != L'\0');
						LEX_Skip(&match);
						LEX_ClearError(&match);
						is_ok = true;
						has_error = true;
						continue;
				}

				PSR_TOTERMTOK(&tok, &term);

				/*
						构造一个简单空语句，以便不会在 abc EOI这种情况下，无法分析出子树
				*/
				if(term.term_val == EOI)
				{
						psrToken_t end;
						end.col = term.col;
						end.line = term.line;
						end.str = L"#";
						end.str_cnt = 1;
						end.term_val = FAKE_EOI;

						if(!PSR_AddToken(parser, &end))
						{
								AR_error(AR_ERR_FATAL, L"%hs\r\n", AR_FUNC_NAME);
						}
				}

				is_ok = PSR_AddToken(parser, &term);

				
				if(tok.value == EOI)break;
		}

		if(is_ok)
		{
				result = (cfgNode_t*)PSR_GetResult(parser);
				AR_ASSERT(result->type == CFG_CONFIG_T);

				if(result && !result->config.has_error)
				{
						result->config.has_error = has_error;
				}
		}


		PSR_DestroyParser(parser);
		PSR_DestroyGrammar(gmr);
		LEX_UnInitMatch(&match);

		
		LEX_Destroy(lex);

		

		return (cfgConfig_t*)result;

}

void			CFG_DestroyGrammarConfig(cfgConfig_t *cfg)
{
		CFG_DestroyNode((cfgNode_t*)cfg);

}

AR_NAMESPACE_END


