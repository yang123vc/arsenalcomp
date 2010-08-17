
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


/*********************************************************************************************************************************************/

typedef enum
{
		CFG_STR_NAME,
		CFG_STR_TOKEN,
		CFG_STR_TOKEN_VALUE,
		CFG_STR_PREC
}cfgStrType_t;

typedef struct __config_parser_data_tag
{
		cfgReport_t				*report;
		arStringTable_t			*name;
		arStringTable_t			*token;
		arStringTable_t			*token_val;
		arStringTable_t			*prec;
}cfgParserData_t;

static	AR_INLINE		void	__init_parser_data(cfgParserData_t		*psr_data, cfgReport_t	*report)
{
		AR_ASSERT(psr_data != NULL && report != NULL);

		AR_memset(psr_data, 0, sizeof(*psr_data));

		psr_data->report = report;
		psr_data->name = AR_CreateStrTable(0);
		psr_data->token = AR_CreateStrTable(0);
		psr_data->token_val = AR_CreateStrTable(0);
		psr_data->prec  = AR_CreateStrTable(0);
}

static AR_INLINE		void	__uninit_parser_data(cfgParserData_t	*psr_data)
{
		AR_ASSERT(psr_data != NULL);

		AR_DestroyStrTable(psr_data->name);
		AR_DestroyStrTable(psr_data->token);
		AR_DestroyStrTable(psr_data->token_val);
		AR_DestroyStrTable(psr_data->prec);
		
		AR_memset(psr_data, 0, sizeof(*psr_data));
}




static AR_INLINE		arStringTable_t*		__get_parser_data_tbl(cfgParserData_t	*psr_data, cfgStrType_t t)
{
		switch(t)
		{
		case CFG_STR_NAME:
				return psr_data->name;
		case CFG_STR_TOKEN:
				return psr_data->token;
		case CFG_STR_TOKEN_VALUE:
				return psr_data->token_val;
		case CFG_STR_PREC:
				return psr_data->prec;
		default:
				AR_ASSERT(false);
				return NULL;

		}
}


static AR_INLINE		void	__insert_str_to_parser_data(cfgParserData_t	*psr_data, const wchar_t *str, cfgStrType_t t)
{
		arStringTable_t	*tbl;
		AR_ASSERT(psr_data != NULL && str != NULL);
		tbl = __get_parser_data_tbl(psr_data, t);
		AR_GetString(tbl, str);
}


static AR_INLINE		bool_t	__has_str_in_parser_data(cfgParserData_t	*psr_data, const wchar_t *str, cfgStrType_t t)
{
		arStringTable_t	*tbl;
		AR_ASSERT(psr_data != NULL && str != NULL);
		tbl = __get_parser_data_tbl(psr_data, t);
		return AR_HasString(tbl, str);
}


static AR_INLINE		void	__check_name_from_parser_data(cfgParserData_t *psr_data, const wchar_t *name, size_t line, cfgStrType_t t)
{

		AR_ASSERT(psr_data != NULL && name != NULL);

		if(__has_str_in_parser_data(psr_data, name, t))
		{
				cfgReportInfo_t	msg;
				wchar_t	buf[1024];

				msg.warning.line = line;
				switch(t)
				{
				case CFG_STR_NAME:
						AR_swprintf(buf, 1024, L"Duplicate name definition %ls\r\n", name);
						break;
				case CFG_STR_TOKEN:
						AR_swprintf(buf, 1024, L"Duplicate token definition %ls\r\n", name);
						break;
				case CFG_STR_TOKEN_VALUE:
						AR_swprintf(buf, 1024, L"Duplicate token value definition %ls\r\n", name);
						break;
				case CFG_STR_PREC:
						AR_swprintf(buf, 1024, L"Duplicate prec definition %ls\r\n", name);
						break;
				default:
						AR_ASSERT(false);
				}
				
				msg.warning.msg = buf;
				msg.type = CFG_REPORT_WARNING_SYNTAX_T;
				psr_data->report->report_func(&msg, psr_data->report->report_ctx);
		}else
		{
				__insert_str_to_parser_data(psr_data, name, t);
		}

}


/*********************************************************************************************************************************************/




typedef enum
{
		CFG_EMPTY_T,
		CFG_LEXEME_T,
		CFG_NAME_T,
		CFG_TOKEN_T,
		CFG_PREC_T,
		CFG_PREDEF_T,
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
				cfgPreDef_t		predef;
		};
		
		cfgNodeType_t	type;
		cfgNode_t		*next;
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
		ACTION_INS
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
		{ASSOC,	L"\"%\"(\"left\"|\"right\"|\"nonassoc\")(?={key_lookahead})", false,1},

		{SKIP,	L"\"%skip\"(?={key_lookahead})", false,0},
		{START,	L"\"%start\"(?={key_lookahead})", false,0},
		{NAME,	L"\"%name\"(?={key_lookahead})", false,0},
		{TOKEN,	L"\"%token\"(?={key_lookahead})", false,0},
		{PREC,	L"\"%prec\"(?={key_lookahead})", false,0},

		{ACTION, L"\"%action\"(?={skip_lexem}+)", false, 0},
		{ACTION_INS, L"\\{((\\\\\\{|\\\\\\})|[^\\{\\}])*?\\}", false, 0},

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
		{ACTION_INS, L"action_ins"},
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




static void CFG_InitConfig(cfgConfig_t *cfg, cfgNodeList_t *name, cfgNodeList_t *token, cfgNodeList_t *prec, cfgNodeList_t *rule, cfgStart_t *start_rule, cfgNodeList_t *pre_def)
{
		size_t i;
		size_t tmp_tok_val = PSR_MIN_TOKENVAL + 1;

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

								if(token->lst[i]->token.code_name == NULL)
								{
										cfg->tok[cfg->tok_cnt].code_name = NULL;
								}else
								{
										cfg->tok[cfg->tok_cnt].code_name = AR_wcsdup(token->lst[i]->token.code_name);
										cfg->tok[cfg->tok_cnt].is_assigned_code_name = true;
								}

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


				for(i = 0; i < cfg->tok_cnt ; ++i)
				{
						if(cfg->tok[i].code_name == NULL)
						{
								cfg->tok[i].code_name = AR_vtow(L"%d", (uint_32_t)cfg->tok[i].tokval);
						}
				}


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

								if(rule->lst[i]->rule.action_name)
								{
										cfg->rule[cfg->rule_cnt].action_name = AR_wcsdup(rule->lst[i]->rule.action_name);
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


		if(pre_def)
		{
				cfg->pre_def = pre_def->count > 0 ? AR_NEWARR0(cfgPreDef_t, pre_def->count) : NULL;
				cfg->predef_cnt = pre_def->count;

				for(i = 0; i < pre_def->count; ++i)
				{
						AR_ASSERT(pre_def->lst[i]->type == CFG_PREDEF_T);
						cfg->pre_def[i].code = AR_wcsdup(pre_def->lst[i]->predef.code);
						cfg->pre_def[i].line = pre_def->lst[i]->predef.line;
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
				AR_DEL(cfg->tok[i].code_name);
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
				if(cfg->rule[i].action_name)AR_DEL(cfg->rule[i].action_name);
				if(cfg->rule[i].action_ins)AR_DEL(cfg->rule[i].action_ins);
		}

		AR_DEL(cfg->rule);


		for(i = 0; i < cfg->predef_cnt; ++i)
		{
				if(cfg->pre_def[i].code)AR_DEL(cfg->pre_def[i].code);
		}

		if(cfg->pre_def)AR_DEL(cfg->pre_def);

}

#if defined(AR_DEBUG)

		#define CFG_INIT_CFG_NODE_COUNT			0
#else
		#define CFG_INIT_CFG_NODE_COUNT			1000
#endif

static cfgNode_t		*__g_node_list = NULL;
static arSpinLock_t		__g_list_lock;

static cfgNode_t*		__get_cfg_node()
{
		cfgNode_t *res = NULL;

		if(__g_node_list == NULL)
		{
				res = AR_NEW(cfgNode_t);
		}else
		{
				AR_LockSpinLock(&__g_list_lock);

				if(__g_node_list == NULL)
				{
						res = AR_NEW(cfgNode_t);
				}else
				{
						res = __g_node_list;
						__g_node_list = __g_node_list->next;
				}

				AR_UnLockSpinLock(&__g_list_lock);
		}

		AR_memset(res, 0, sizeof(*res));
		return res;
}

static void		__put_cfg_node(cfgNode_t *node)
{
		AR_ASSERT(node != NULL);

		AR_LockSpinLock(&__g_list_lock);
		node->next = __g_node_list;
		__g_node_list = node;

		AR_UnLockSpinLock(&__g_list_lock);

}


static void __init_cfg_node_list()
{
		size_t i;
		AR_InitSpinLock(&__g_list_lock);
		__g_node_list	= NULL;

		for(i = 0; i < CFG_INIT_CFG_NODE_COUNT; ++i)
		{
				__put_cfg_node(AR_NEW0(cfgNode_t));
		}
}

static void __uninit_cfg_node_list()
{
		size_t count = 0;
		AR_UnInitSpinLock(&__g_list_lock);

		while(__g_node_list)
		{
				cfgNode_t		*tmp = __g_node_list->next;
				AR_DEL(__g_node_list);
				__g_node_list = tmp;
				count++;
		}


		{
				wchar_t buf[1024];
				AR_swprintf(buf, 1024, L"Total consume cfgNode_t == %u", count);
				AR_printf(L"%ls\r\n", buf);
		}

}




static cfgNode_t* CFG_CreateNode(cfgNodeType_t type)
{
		cfgNode_t		*node;

		node = __get_cfg_node();

		node->type = type;
		node->next = NULL;

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
						if(node->token.code_name)AR_DEL(node->token.code_name);
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
						if(node->rule.action_name)
						{
								AR_DEL(node->rule.action_name);
						}

						if(node->rule.action_ins)
						{
								AR_DEL(node->rule.action_ins);
						}

						if(node->rule.lhs)AR_DEL(node->rule.lhs);
						if(node->rule.rhs)AR_DEL(node->rule.rhs);
						if(node->rule.prec_tok)AR_DEL(node->rule.prec_tok);

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
				case CFG_PREDEF_T:
				{
						if(node->predef.code)AR_DEL(node->predef.code);
						break;
				}
				default:
				{
						AR_ASSERT(false);
						AR_error(AR_ERR_FATAL, L"%hs\r\n", AR_FUNC_NAME);
				}
				}

				/*
				AR_DEL(node);
				*/
				__put_cfg_node(node);

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
				AR_ASSERT(tok->str[tok->str_cnt-1] == L'"' || tok->str[tok->str_cnt-1] == L'\'');

				node->lexeme.lexeme = AR_wcsndup(tok->str + 1, tok->str_cnt-2);
				AR_ASSERT(node->lexeme.lexeme != NULL);
		}else if(tok->term_val == ACTION_INS)
		{
				wchar_t *buf, *d;
				const wchar_t *s, *e;
				AR_ASSERT(tok->str_cnt > 0);
				buf = AR_NEWARR(wchar_t,tok->str_cnt + 1);
				d = buf; s = tok->str; e = tok->str + tok->str_cnt;

				while(s < e)
				{
						if(*s == L'\\' && (*(s+1) == L'{' || *(s+1) == L'}'))
						{
								++s;
						}
						*d++ = *s++;
				}
				
				*d = L'\0';
				node->lexeme.lexeme = buf;
		}else if(tok->str_cnt > 0)
		{
				node->lexeme.lexeme = AR_wcsndup(tok->str, tok->str_cnt);
		}

		node->lexeme.line = tok->line;
		return (psrNode_t*)node;
}




/*
{ L"action_decl			:		%action lexeme action_ins",				__handle_action_decl,0},
{ L"action_decl			:		%action lexeme",						__handle_action_decl},
{ L"action_decl			:				",										NULL},
*/
static psrNode_t*		AR_STDCALL __handle_action_decl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;		
		
		AR_ASSERT(count == 2 || count == 3);
		AR_ASSERT(ns[0]->type == CFG_LEXEME_T && ns[1]->type == CFG_LEXEME_T);
		
		res = CFG_CreateNode(CFG_NODE_LIST_T);
		
		CFG_InsertToNodeList(&res->lst, ns[1]);
		ns[1] = NULL;
		
		if(count == 3)
		{
				AR_ASSERT(ns[2] && ns[2]->type == CFG_LEXEME_T);
				CFG_InsertToNodeList(&res->lst, ns[2]);
				ns[2] = NULL;
		}
		return res;
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
{ L"rhs					:		term_list prec_decl action_decl",				__handle_rhs,0},
{ L"rhs					:		. prec_decl action_decl",						__handle_rhs,0},
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
		AR_ASSERT(ns[2] ? ns[2]->type == CFG_NODE_LIST_T : true);


		if(ns[0]->type == CFG_LEXEME_T && ns[0]->lexeme.lex_val == DOT)
		{
				AR_wcscat((wchar_t*)res->rule.rhs, L" ");

				res->rule.line = ns[0]->lexeme.line;

		}else if(ns[0]->type == CFG_NODE_LIST_T)
		{
				for(i = 0; i < ns[0]->lst.count; ++i)
				{
						cfgNode_t *tmp = ns[0]->lst.lst[i];

						AR_ASSERT(tmp->type == CFG_LEXEME_T && tmp->lexeme.lex_val == (size_t)LEXEME);

						AR_wcscat((wchar_t*)res->rule.rhs, tmp->lexeme.lexeme);
						AR_wcscat((wchar_t*)res->rule.rhs, L" ");

						if(i == 0)
						{
								res->rule.line = tmp->lexeme.line;
						}
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
				AR_ASSERT(ns[2]->lst.count == 1 || ns[2]->lst.count == 2);

				res->rule.action_name = ns[2]->lst.lst[0]->lexeme.lexeme;
				ns[2]->lst.lst[0]->lexeme.lexeme = NULL;
				
				if(ns[2]->lst.count == 2)
				{
						res->rule.action_ins = ns[2]->lst.lst[1]->lexeme.lexeme;
						ns[2]->lst.lst[1]->lexeme.lexeme = NULL;
				}
		}

/*
		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		CFG_DestroyNode(ns[2]);
		ns[0] = ns[1] = ns[2] = NULL;
*/
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
				res->prec.assoc	= PSR_ASSOC_NONASSOC;

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
				
				__check_name_from_parser_data((cfgParserData_t*)ctx, node->lexeme.lexeme, node->lexeme.line, CFG_STR_PREC);

		}
		return res;

}






/*
{ L"token_def			:  		%token %skip token_val_prec : lexeme token_val_prec action_decl",	__handle_token_def},
{ L"token_def			:  		%token lexeme token_val_prec : lexeme token_val_prec action_decl",	__handle_token_def},
*/


static psrNode_t*		AR_STDCALL __handle_token_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 7);

		/*AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_LEXEME_T && ns[4] && ns[4]->type == CFG_LEXEME_T);*/

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T
				&& ns[1] && ns[1]->type == CFG_LEXEME_T
				&& ns[3] && ns[3]->type == CFG_LEXEME_T
				&& ns[4] && ns[4]->type == CFG_LEXEME_T
				);

		AR_ASSERT((ns[2] == NULL) || (ns[2]->type == CFG_LEXEME_T));
		AR_ASSERT((ns[5] == NULL) || (ns[5]->type == CFG_LEXEME_T));

		AR_ASSERT((ns[6] == NULL) || (ns[6]->type == CFG_NODE_LIST_T));



		res = CFG_CreateNode(CFG_TOKEN_T);

		res->token.line = ns[1]->lexeme.line;

		if(ns[1]->lexeme.lex_val == SKIP)
		{
				res->token.is_skip = true;
				res->token.name = NULL;
		}else
		{
				__check_name_from_parser_data((cfgParserData_t*)ctx, ns[1]->lexeme.lexeme, ns[1]->lexeme.line, CFG_STR_TOKEN);

				res->token.is_skip = false;
				res->token.name = ns[1]->lexeme.lexeme;
				ns[1]->lexeme.lexeme = NULL;
		}

		res->token.regex = ns[4]->lexeme.lexeme;
		ns[4]->lexeme.lexeme = NULL;

		res->token.lex_prec = 0;

		if(ns[2])
		{
				AR_wtou(ns[2]->lexeme.lexeme, (uint_t*)&res->token.tokval, 10);
		}else
		{
				res->token.tokval = 0;
		}

		if(ns[5])
		{
				AR_wtou(ns[5]->lexeme.lexeme, (uint_t*)&res->token.lex_prec, 10);
		}


		if(ns[6])
		{
				AR_ASSERT(ns[6]->lst.count > 0);
				res->token.code_name = AR_wcsdup(ns[6]->lst.lst[0]->lexeme.lexeme);

				__check_name_from_parser_data((cfgParserData_t*)ctx, ns[6]->lst.lst[0]->lexeme.lexeme, ns[6]->lst.lst[0]->lexeme.line, CFG_STR_TOKEN_VALUE);
		}

		

		return res;

}





/*
{ L"name_def 			:		%name lexeme : lexeme  ",								__handle_name_def},
*/

static psrNode_t*		AR_STDCALL __handle_name_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgParserData_t			*psr_data;
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

		psr_data = (cfgParserData_t*)ctx;

		__check_name_from_parser_data(psr_data, res->name.name, res->name.line, CFG_STR_NAME);
		
		return res;
}


/*
{ L"item				:		name_def ;",	__handle_item},
{ L"item				:		token_def ;",	__handle_item},
{ L"item				:		prec_def ;",	__handle_item},
{ L"item				:		rule_def ;",	__handle_item},
{ L"item				:		error ; ",	__handle_item},
{ L"item				:		; ",	__handle_item},
{ L"item				:		pre_code_decl ;",	 __handle_item,0},

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
		if(res == NULL)
		{
				res = CFG_CreateNode(CFG_NODE_LIST_T);
		}
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
{ L"pre_code_decl		:		action_ins",				__handle_pre_def,0},
*/
static psrNode_t*		AR_STDCALL __handle_pre_def(psrNode_t **nodes, size_t count, const wchar_t * name_tmp, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		size_t			len;
		AR_ASSERT(count == 1);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[0]->lexeme.lex_val == ACTION_INS);


		res = CFG_CreateNode(CFG_PREDEF_T);
		res->predef.line = ns[0]->lexeme.line;
		
		len = AR_wcslen(ns[0]->lexeme.lexeme);
		AR_ASSERT(len >= 2);
		
		if(len >= 2)
		{
				res->predef.code = AR_wcsndup(ns[0]->lexeme.lexeme+1, len - 2);
		}else
		{
				res->predef.code = AR_wcsdup(L"");
		}

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
		cfgNodeList_t name, token, prec, rule, error, empty, predef;
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
		CFG_InitNodeList(&predef);
		 

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
				case CFG_PREDEF_T:
				{
						CFG_InsertToNodeList(&predef, node);
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
		CFG_InitConfig(&res->config, &name, &token, &prec, &rule, start_rule, &predef);

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
		CFG_UnInitNodeList(&predef);
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
		
		{ L"item				:		pre_code_decl ;",	 __handle_item,0},
		{ L"item				:		start_def ;",	__handle_item, 0},
		{ L"item				:		name_def ;",	__handle_item, 0},
		{ L"item				:		token_def ;",	__handle_item,0},
		{ L"item				:		prec_def ;",	__handle_item,0},
		{ L"item				:		rule_def ;",	__handle_item,0},
		{ L"item				:		error ; ",	__handle_item,0},
		{ L"item				:		; ",	__handle_item,0},

		{ L"name_def 			:		%name lexeme : lexeme ",								__handle_name_def,0},

		{ L"token_def			:  		%token %skip token_val_prec : lexeme token_val_prec action_decl",	__handle_token_def,0},
		{ L"token_def			:  		%token lexeme token_val_prec : lexeme token_val_prec action_decl",	__handle_token_def,0},

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
		
		{ L"action_decl			:		%action lexeme action_ins",				__handle_action_decl,0},
		{ L"action_decl			:		%action lexeme",						__handle_action_decl,0},
		{ L"action_decl			:		",										NULL,0},

		{ L"start_def			:  		%start lexeme",							__handle_start_def,0},

		
		{ L"pre_code_decl		:		action_ins",				__handle_pre_def,0},


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


static psrGrammar_t*	__build_grammar(psrHandler_t *handler, arIOCtx_t *io)
{
		psrGrammar_t	*gmr;
		size_t i;
		AR_ASSERT(handler != NULL);

		gmr = PSR_CreateGrammar(handler, io);

		if(gmr == NULL)return NULL;

		for(i = 0; i < AR_NELEMS(__cfg_term); ++i)
		{
				if(!PSR_InsertTerm(gmr, __cfg_term[i].name, (size_t)__cfg_term[i].val, PSR_ASSOC_NONASSOC, 0, __build_leaf))
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



static const parser_t*		__build_parser(const psrGrammar_t *gmr)
{
		const parser_t *parser;
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

		report = ((cfgParserData_t*)ctx)->report;

		AR_memset(&info, 0, sizeof(info));

		info.type = CFG_REPORT_ERROR_T;
		info.error.err_level = level;
		info.error.err_msg = msg;
		report->report_func(&info, report->report_ctx);
}


void	AR_STDCALL cfg_on_print(const wchar_t *msg, void *ctx)
{
		cfgReport_t				*report = NULL;
		cfgReportInfo_t			info;
		AR_ASSERT(msg != NULL && ctx != NULL);

		report = ((cfgParserData_t*)ctx)->report;

		AR_memset(&info, 0, sizeof(info));

		info.type = CFG_REPORT_MESSAGE_T;
		info.std_msg.message = msg;
		report->report_func(&info, report->report_ctx);
}


static void	AR_STDCALL cfg_free(psrNode_t *node, void *ctx)
{
		CFG_DestroyNode((cfgNode_t*)node);
}


static void		AR_STDCALL cfg_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
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

		AR_ASSERT(tok != NULL && ctx != NULL && expected != NULL && count > 0);

		report = ((cfgParserData_t*)ctx)->report;
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
		AR_AppendFormatString(str, L"Expected Term :\r\n");

		for(i = 0; i < count; ++i)
		{
				const wchar_t *term = NULL;
				size_t x;
				for(x = 0; x < sizeof(__cfg_term) / sizeof(__cfg_term[0]); ++x)
				{
						if((size_t)(__cfg_term[x].val) == expected[i])
						{
								term = __cfg_term[x].name;
						}
				}

				AR_ASSERT(term != NULL);
				AR_AppendFormatString(str, L"\"%ls\" ", term);
		}
		AR_AppendFormatString(str, L"\r\n\r\n");
		/******************************************************************************************/
		info.syntax_error.msg = AR_GetStrString(str);
		info.syntax_error.tok = tok;
		info.type = CFG_REPORT_ERR_SYNTAX_T;
		report->report_func(&info, report->report_ctx);
		AR_DestroyString(str);
}



static void AR_STDCALL __default_report_func(const cfgReportInfo_t *report, void *context)
{
				
}


static cfgReport_t		__g_def_report = 
{
		__default_report_func,
		NULL
};


static arIOCtx_t	__def_io_ctx = 
{		
		cfg_on_error,
		cfg_on_print,
		NULL
};

static psrHandler_t		__def_handler_ctx = 
{
		cfg_error,
		cfg_free,
};


static arSpinLock_t				__g_lock;
static lex_t					*__g_lex;
static psrGrammar_t				*__g_grammar = NULL;
static const parser_t			*__g_parser = NULL;


static void __init_parser_tag()
{
		psrHandler_t	psr_handler;
		AR_InitSpinLock(&__g_lock);
		
		psr_handler.error_f = cfg_error;
		psr_handler.free_f = cfg_free;
		__g_lex		= __build_lex(NULL);
		__g_grammar = __build_grammar(&psr_handler, NULL);
		__g_parser	= __build_parser(__g_grammar);
}


static void __uninit_parser_tag()
{
		PSR_DestroyParser(__g_parser);
		PSR_DestroyGrammar(__g_grammar);
		LEX_Destroy(__g_lex);
		__g_lex = NULL;
		__g_parser = NULL;
		__g_grammar = NULL;
		AR_UnInitSpinLock(&__g_lock);
		
}

static lexMatch_t*		__create_lex_match(const arIOCtx_t		*io_ctx)
{
		lexMatch_t		*match;

		AR_LockSpinLock(&__g_lock);
		match = LEX_CreateMatch(__g_lex, io_ctx);
		AR_UnLockSpinLock(&__g_lock);

		return match;
}

static void				__destroy_lex_match(lexMatch_t *match)
{
		AR_ASSERT(match != NULL);
		LEX_DestroyMatch(match);
}

static psrContext_t*	__create_parser_context(void *ctx)
{
		psrContext_t	*parser_context = NULL;
		AR_LockSpinLock(&__g_lock);
		parser_context = PSR_CreateContext(__g_parser, ctx);
		AR_UnLockSpinLock(&__g_lock);
		return parser_context;
}

static void				__destroy_parser_context(psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		PSR_DestroyContext(parser_context);
}




cfgConfig_t*	CFG_CollectGrammarConfig(const wchar_t *gmr_txt, cfgReport_t *report)
{

		bool_t is_ok, has_error;
		
		psrContext_t	*parser_context;
		cfgParserData_t	psr_data;
		lexMatch_t		*match;

		lexToken_t		tok;
		psrToken_t		term;
		
		cfgNode_t		*result = NULL;

		arIOCtx_t		io_ctx ;
		

		AR_ASSERT(gmr_txt != NULL);

		if(report == NULL)report = &__g_def_report;

		io_ctx.on_error = cfg_on_error;
		io_ctx.on_print = cfg_on_print;
		io_ctx.ctx		= (void*)report;

		
		
		match		   = __create_lex_match(&io_ctx);

		__init_parser_data(&psr_data, report);
		parser_context = __create_parser_context((void*)&psr_data);
		
		LEX_ResetInput(match, gmr_txt);

		is_ok = true;
		has_error = false;


		while(is_ok)
		{
				is_ok = LEX_Match(match, &tok);

				if(!is_ok)
				{
						cfgReportInfo_t	info;
						psrToken_t		tmp_tok;
						arString_t		*str;
						size_t n;
						size_t	line, col;
						const wchar_t *tok = NULL;

						AR_memset(&info, 0, sizeof(info));
						n = AR_wcslen(LEX_GetNextInput(match));
						tok = AR_wcsndup(LEX_GetNextInput(match), n > 5 ? 5 : n);

						str = AR_CreateString();
						
						LEX_MatchGetCoordinate(match, &line, &col);
						AR_AppendFormatString(str, L"Invalid Token %ls...(%"AR_PLAT_INT_FMT L"d : %"AR_PLAT_INT_FMT L"d)\r\n", tok, line, col);

						if(tok)AR_DEL(tok);

						info.type = CFG_REPORT_ERR_LEX_T;
						info.lex_error.msg = AR_GetStrString(str);

						tmp_tok.term_val = 0;
						tmp_tok.str_cnt = 0;
						tmp_tok.str =  LEX_GetNextInput(match);

						LEX_MatchGetCoordinate(match, &line, &col);
						tmp_tok.line = line;
						tmp_tok.col = col;

						info.lex_error.tok = &tmp_tok;
						
						report->report_func(&info, report->report_ctx);
						
						AR_DestroyString(str);

						AR_ASSERT(*LEX_GetNextInput(match) != L'\0');
						LEX_Skip(match);
						LEX_ClearError(match);
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

						if(!PSR_AddToken(parser_context, &end))
						{
								AR_error(AR_ERR_FATAL, L"%hs\r\n", AR_FUNC_NAME);
						}
				}

				is_ok = PSR_AddToken(parser_context, &term);


				if(tok.value == EOI)break;
		}

		if(is_ok)
		{
				result = (cfgNode_t*)PSR_GetResult(parser_context);
				AR_ASSERT(result->type == CFG_CONFIG_T);

				if(result && !result->config.has_error)
				{
						result->config.has_error = has_error;
				}
		}

		
		__destroy_parser_context(parser_context);
		__destroy_lex_match(match);
		__uninit_parser_data(&psr_data);
		
		return (cfgConfig_t*)result;

}

void			CFG_DestroyGrammarConfig(cfgConfig_t *cfg)
{
		CFG_DestroyNode((cfgNode_t*)cfg);

}









static const wchar_t CFG_DEF_BUILD_LEX[] =
L"static lex_t*	__build_lex(const arIOCtx_t *io)								\n"
L"{																				\n"
L"		lex_t	*lex;															\n"
L"		size_t i;																\n"
L"		lex = LEX_Create(io);													\n"
L"		for(i = 0; i < __NAME_COUNT__; ++i)										\n"
L"		{																		\n"
L"				if(!LEX_Insert(lex, __g_lex_name[i]))							\n"
L"				{																\n"
L"						LEX_Destroy(lex);										\n"
L"						AR_ASSERT(false);										\n"
L"						return NULL;											\n"
L"				}																\n"
L"		}																		\n"
L"		for(i = 0; i < __TERM_COUNT__; ++i)										\n"
L"		{																		\n"
L"				lexAction_t		act;											\n"
L"				act.is_skip		=		__g_term_pattern[i].skip;				\n"
L"				act.priority	=		__g_term_pattern[i].lex_prec;			\n"
L"				act.value		=		__g_term_pattern[i].tokval;				\n"
L"				if(!LEX_InsertRule(lex, __g_term_pattern[i].regex, &act))		\n"
L"				{																\n"
L"						LEX_Destroy(lex);										\n"
L"						AR_ASSERT(false);										\n"
L"						return NULL;											\n"
L"				}																\n"
L"		}																		\n"
L"		return lex;																\n"
L"}"
;

static const wchar_t CFG_DEF_BUILD_GRAMMAR[] =
L"static psrGrammar_t*	__build_grammar(const psrHandler_t	*handler, const arIOCtx_t *io)										\n"
L"{																																\n"
L"		psrGrammar_t	*grammar;																								\n"
L"		size_t i;																												\n"
L"		AR_ASSERT(handler != NULL);																								\n"
L"		grammar = PSR_CreateGrammar(handler, io);																				\n"
L"		for(i = 0; i < __TERM_COUNT__; ++i)																						\n"
L"		{																														\n"
L"				if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)continue;										\n"
L"				if(!PSR_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PSR_ASSOC_NONASSOC,0, NULL))	\n"
L"				{																												\n"
L"						PSR_DestroyGrammar(grammar);																			\n"
L"						grammar = NULL;																							\n"
L"						AR_ASSERT(false);																						\n"
L"						return NULL;																							\n"
L"				}																												\n"
L"		}																														\n"
L"		for(i = 0; i < __PREC_COUNT__; ++i)																						\n"
L"		{																														\n"
L"				psrTermInfo_t	*info;																							\n"
L"				info = PSR_GetTermSymbInfoByName(grammar, __g_prec_pattern[i].name);											\n"
L"				if(info == NULL)																								\n"
L"				{																												\n"
L"						if(!PSR_InsertTerm(grammar, __g_prec_pattern[i].name, __g_prec_pattern[i].tokval, __g_prec_pattern[i].assoc, __g_prec_pattern[i].prec_level, NULL))\n"
L"						{																																					\n"
L"								PSR_DestroyGrammar(grammar);																												\n"
L"								grammar = NULL;																																\n"
L"								AR_ASSERT(false);																															\n"
L"								return NULL;																																\n"
L"						}																																					\n"
L"				}else																																						\n"
L"				{																																							\n"
L"						info->assoc = __g_prec_pattern[i].assoc;																											\n"
L"						info->prec = __g_prec_pattern[i].prec_level;																										\n"
L"				}																																							\n"
L"		}																																									\n"
L"		for(i = 0; i < __RULE_COUNT__; ++i)																													\n"
L"		{																																									\n"
L"				if(!PSR_InsertRuleByStr(grammar, __g_rule_pattern[i].rule, __g_rule_pattern[i].prec_token, __g_rule_pattern[i].handler, __g_rule_pattern[i].auto_ret))		\n"
L"				{																																							\n"
L"						PSR_DestroyGrammar(grammar);																														\n"
L"						grammar = NULL;																																		\n"
L"						AR_ASSERT(false);																																	\n"
L"						return NULL;																																		\n"
L"				}																																							\n"
L"		}																																									\n"
L"		if(!PSR_SetFirstRule(grammar,START_RULE) || !PSR_CheckIsValidGrammar(grammar))																						\n"
L"		{																																									\n"
L"				PSR_DestroyGrammar(grammar);																																\n"
L"				grammar = NULL;																																				\n"
L"				AR_ASSERT(false);																																			\n"
L"				return NULL;																																				\n"
L"		}																																									\n"
L"		return grammar;																																						\n"
L"}"
;

/*
__NAME_COUNT__
__TERM_COUNT__

__PREC_COUNT__
__RULE_COUNT__
*/

#define CFG_CNT_DEF		L"#define %ls ((size_t)%u)\r\n"



bool_t			CFG_ConfigToCode(const cfgConfig_t *cfg, arString_t	*code)
{
		size_t i;
		arString_t		*handler_define = NULL;
		AR_ASSERT(cfg != NULL && code != NULL);


		if(cfg->has_error)return false;

		if(cfg->predef_cnt > 0)
		{
				for(i = 0; i < cfg->predef_cnt; ++i)
				{
						AR_AppendString(code, L"\r\n");
						AR_AppendString(code,  cfg->pre_def[i].code);
						AR_AppendString(code, L"\r\n");
				}
		}

		if(cfg->name_cnt > 0)
		{
				AR_AppendString(code, L"\n");
				AR_AppendString(code, CFG_NAME_DEF_BEGIN);
				AR_AppendString(code, L"\n");

				for(i = 0; i < cfg->name_cnt; ++i)
				{
						/*#define CFG_NAME_DEF_ITEM		L"L\"%ls = %ls\""*/
						wchar_t *name , *regex;
						name =  AR_str_to_escstr(cfg->name[i].name);
						regex = AR_str_to_escstr(cfg->name[i].regex);



						AR_AppendFormatString(code, CFG_NAME_DEF_ITEM, name, regex);
						if(i < cfg->name_cnt - 1)AR_AppendString(code, L",");
						AR_AppendString(code, L"\n");

						if(name)AR_DEL(name);
						if(regex) AR_DEL(regex);
				}

				AR_AppendString(code, CFG_NAME_DEF_END);
				AR_AppendString(code, L"\n");
				AR_AppendString(code, L"\n");
		}else
		{
				AR_AppendString(code, L"\n");
				AR_AppendString(code, CFG_NAME_DEF_BEGIN);
				AR_AppendString(code, L"\n");
				AR_AppendString(code, L"NULL\r\n");
				AR_AppendString(code, CFG_NAME_DEF_END);
				AR_AppendString(code, L"\n");
				AR_AppendString(code, L"\n");
		}

		AR_AppendFormatString(code,  CFG_CNT_DEF, L"__NAME_COUNT__", cfg->name_cnt);

		if(cfg->tok_cnt > 0)
		{
				arString_t		*enum_str;
				enum_str = AR_CreateString();
				AR_AppendString(enum_str, L"\r\n\r\n/*\r\nenum{\r\n");
				for(i = 0; i < cfg->tok_cnt; ++i)
				{
						/*if(cfg->tok[i].code_name == NULL)continue;*/
						AR_ASSERT(cfg->tok[i].code_name != NULL);

						if(cfg->tok[i].is_assigned_code_name)
						{
								AR_AppendFormatString(enum_str, L"%s = %d,\r\n", cfg->tok[i].code_name, (uint_32_t)cfg->tok[i].tokval);
						}
				}
				AR_AppendString(enum_str, L"};\r\n*/\r\n\r\n\r\n");
				AR_AppendString(code, AR_GetStrString(enum_str));
				AR_DestroyString(enum_str);
		}

		if(cfg->tok_cnt > 0)
		{
				AR_AppendString(code, L"\n");
				AR_AppendString(code, CFG_TERM_DEF_BEGIN);
				AR_AppendString(code, L"\n");

				//#define CFG_TERM_DEF_ITEM	L"{/*name*/L\"%ls\", /*token value*/%d, /*lex_prec*/%d, /*regex*/L\"%ls\", /*is_skip*/L\"%ls\"}"

				for(i = 0; i < cfg->tok_cnt; ++i)
				{
						wchar_t *name , *regex;

						name =  AR_str_to_escstr(cfg->tok[i].name);
						regex = AR_str_to_escstr(cfg->tok[i].regex);

						/*
						AR_printf(L"regex == %ls\r\n", regex);

						getchar();
						*/

						if(name != NULL)
						{
								AR_AppendFormatString(code, CFG_TERM_DEF_ITEM_1, name, cfg->tok[i].code_name, cfg->tok[i].lex_prec, regex, cfg->tok[i].is_skip ? L"true" : L"false");
						}else
						{
								AR_AppendFormatString(code, CFG_TERM_DEF_ITEM_2, L"NULL", cfg->tok[i].code_name, cfg->tok[i].lex_prec, regex, cfg->tok[i].is_skip ? L"true" : L"false");
						}
						if(i < cfg->tok_cnt - 1)AR_AppendString(code, L",");
						AR_AppendString(code, L"\n");

						if(name)AR_DEL(name);
						if(regex) AR_DEL(regex);
				}


				AR_AppendString(code, CFG_TERM_DEF_END);
				AR_AppendString(code, L"\n");
				AR_AppendString(code, L"\n");
		}

		AR_AppendFormatString(code,  CFG_CNT_DEF, L"__TERM_COUNT__", cfg->tok_cnt);

		if(cfg->prec_cnt > 0)
		{
				AR_AppendString(code, L"\n");
				AR_AppendString(code, CFG_PREC_DEF_BEGIN);
				AR_AppendString(code, L"\n");

				//#define CFG_TERM_DEF_ITEM	L"{/*name*/L\"%ls\", /*token value*/%d, /*lex_prec*/%d, /*regex*/L\"%ls\", /*is_skip*/L\"%ls\"}"

				for(i = 0; i < cfg->prec_cnt; ++i)
				{
						size_t k;
						size_t prec = cfg->prec[i].prec_level;
						const wchar_t *assoc;

						switch(cfg->prec[i].assoc)
						{
						case PSR_ASSOC_NONASSOC:
								assoc = L"PSR_ASSOC_NONASSOC";
								break;
						case PSR_ASSOC_LEFT:
								assoc = L"PSR_ASSOC_LEFT";
								break;
						default:/*case PSR_ASSOC_RIGHT:*/
								assoc = L"PSR_ASSOC_RIGHT";
								break;
						}

						for(k = 0; k < cfg->prec[i].count; ++k)
						{
								wchar_t *name = NULL;
								const wchar_t	*code_name = NULL;
								size_t tok_val;
								size_t z = 0;
								name  =  AR_str_to_escstr(cfg->prec[i].prec_tok_set[k]);
								tok_val = cfg->prec[i].prec_tok_val[k];

								for(z = 0; z < cfg->tok_cnt; ++z)
								{
										if(tok_val == cfg->tok[z].tokval)
										{
												AR_ASSERT(cfg->tok[z].code_name != NULL);
												
												code_name = cfg->tok[z].code_name;
												break;
										}
								}
								
								if(code_name == NULL)
								{
										/*一些后期定义的prec token不会在cfg->tok中，因此也没有code_name*/
										code_name = AR_vtow(L"%d", (uint_32_t)tok_val);
								}


								/*#define CFG_PREC_DEF_BEGIN	L"struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {"*/
								/*#define CFG_PREC_DEF_ITEM 	L"{L\"%ls\", (size_t)%ls," L"%" AR_PLAT_INT_FMT L"d, %ls}"*/
								
								AR_AppendFormatString(code, CFG_PREC_DEF_ITEM, name, code_name, prec, assoc);
								if(name)AR_DEL(name);
								
								if(z == cfg->tok_cnt)
								{
										AR_DEL(code_name);
								}

								if(i == cfg->prec_cnt - 1 && k == cfg->prec[i].count - 1)
								{
								}else
								{
										AR_AppendString(code, L",");
								}

								AR_AppendString(code, L"\n");
						}

				}

				AR_AppendString(code, CFG_PREC_DEF_END);
				AR_AppendString(code, L"\n");
				AR_AppendString(code, L"\n");
		}else
		{
				AR_AppendString(code, L"\n");
				AR_AppendString(code, CFG_PREC_DEF_BEGIN);
				AR_AppendString(code, L"\n");
				AR_AppendString(code, L"NULL\r\n");
				AR_AppendString(code, CFG_PREC_DEF_END);
				AR_AppendString(code, L"\n");
				AR_AppendString(code, L"\n");
		}
		
		{
				size_t	prec_cnt = 0;
				size_t i;
				for(i = 0; i < cfg->prec_cnt; ++i)prec_cnt += cfg->prec[i].count;
				
				AR_AppendFormatString(code,  CFG_CNT_DEF, L"__PREC_COUNT__", prec_cnt);
		}

		if(cfg->rule_cnt > 0)
		{
				arStringTable_t	*handler_mark;
				handler_define	= AR_CreateString();
				handler_mark = 		AR_CreateStrTable(cfg->rule_cnt);


				AR_AppendString(code, L"\n");

				for(i = 0; i < cfg->rule_cnt; ++i)
				{
						size_t tmp_len;
						wchar_t *handler, *handler_def, *tmp;
								
						if(cfg->rule[i].action_name)
						{
								if(AR_wcscmp(cfg->rule[i].action_name, L"NULL") == 0)
								{
										continue;
								}
								
								tmp_len = AR_wcslen(cfg->rule[i].action_name);

								if(cfg->rule[i].action_ins)
								{
										tmp_len += AR_wcslen(cfg->rule[i].action_ins);
								}
								
								tmp_len += 4096;

								tmp = AR_NEWARR0(wchar_t, tmp_len + 1);
								AR_swprintf(tmp, tmp_len, CFG_RULE_HANDLER_DECL_2, cfg->rule[i].action_name);
								handler = AR_wcsdup(tmp);

								if(cfg->rule[i].action_ins == NULL)
								{
										AR_swprintf(tmp, tmp_len, CFG_RULE_HANDLER_DEFINE_2, cfg->rule[i].action_name);
										handler_def = AR_wcsdup(tmp);
								}else
								{
										AR_swprintf(tmp, tmp_len, CFG_RULE_HANDLER_DEFINE_3, cfg->rule[i].action_name, cfg->rule[i].action_ins);
										handler_def = AR_wcsdup(tmp);
								}

								if(tmp)
								{
										AR_DEL(tmp);
										tmp = NULL;
								}
						}else
						{
								tmp_len = 20480;
								tmp = AR_NEWARR0(wchar_t, tmp_len + 1);
								AR_swprintf(tmp, tmp_len, CFG_RULE_HANDLER_DECL, cfg->rule[i].lhs);
								handler = AR_wcsdup(tmp);

								AR_swprintf(tmp, tmp_len, CFG_RULE_HANDLER_DEFINE, cfg->rule[i].lhs);
								handler_def = AR_wcsdup(tmp);

								if(tmp)
								{
										AR_DEL(tmp);
										tmp = NULL;
								}
						}

						/*if(AR_wcsstr(AR_GetStrString(code), handler) == NULL)*/

						if(!AR_HasString(handler_mark, handler))
						{
								size_t k;
								arString_t *comment = AR_CreateString();

								for(k = 0; k < cfg->rule_cnt; ++k)
								{
										bool_t insert_comment = false;

										if(cfg->rule[k].action_name == NULL && cfg->rule[i].action_name == NULL)
										{
												insert_comment	= AR_wcscmp(cfg->rule[k].lhs, cfg->rule[i].lhs) == 0 ? true : false;
										}else if(cfg->rule[k].action_name != NULL && cfg->rule[i].action_name != NULL)
										{
												insert_comment = AR_wcscmp(cfg->rule[k].action_name, cfg->rule[i].action_name) == 0 ? true : false;
										}

										if(insert_comment)
										{
												AR_AppendString(comment, L"/*");
												AR_AppendString(comment, cfg->rule[k].lhs);
												AR_AppendString(comment, L"\t:\t");
												AR_AppendString(comment, cfg->rule[k].rhs);
												AR_AppendString(comment, L"*/");
												AR_AppendString(comment, L"\r\n");
										}
								}

								AR_AppendString(handler_define, AR_GetStrString(comment));
								AR_AppendString(handler_define, handler_def);
								AR_AppendString(handler_define, L"\r\n\n");

								AR_AppendString(code, AR_GetStrString(comment));
								AR_AppendString(code, handler);
								AR_AppendString(code, L"\r\n\n");
								
								AR_DestroyString(comment);

								AR_GetString(handler_mark, handler);
						}

						//AR_AppendString(code, L"\r\n");

						if(handler)AR_DEL(handler);
						if(handler_def)AR_DEL(handler_def);
				}

				AR_AppendString(code, L"\n");
				AR_AppendString(code, L"\n");

				AR_DestroyStrTable(handler_mark);
		}




		if(cfg->rule_cnt > 0)
		{

				AR_AppendString(code, L"\n");
				AR_AppendString(code, CFG_RULE_DEF_BEGIN);
				AR_AppendString(code, L"\n");



				for(i = 0; i < cfg->rule_cnt; ++i)
				{
						wchar_t *rule , *prec, *handler;
						wchar_t *tmp;
						tmp = AR_NEWARR0(wchar_t, 2048 + 1);
						AR_wcscat(tmp, cfg->rule[i].lhs);
						AR_wcscat(tmp, L"  :  ");
						AR_wcscat(tmp, cfg->rule[i].rhs);

						rule =  AR_str_to_escstr(tmp);

						if(tmp)
						{
								AR_DEL(tmp);
								tmp = NULL;
						}

						prec = AR_str_to_escstr(cfg->rule[i].prec_tok);

						if(cfg->rule[i].action_name)
						{
								handler = AR_str_to_escstr(cfg->rule[i].action_name);
						}else
						{
								tmp = AR_NEWARR0(wchar_t, 2048 + 1);
								AR_swprintf(tmp, 2048, L"handle_%ls", cfg->rule[i].lhs);
								handler = AR_str_to_escstr(tmp);
								if(tmp)
								{
										AR_DEL(tmp);
										tmp = NULL;
								}
						}

/*
#define CFG_RULE_DEF_ITEM 	L"{L\"%ls\", \"%ls\", \"%ls\", %d}"
#define CFG_RULE_DEF_BEGIN	L"struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {"
*/
						if(prec == NULL)
						{
								AR_AppendFormatString(code, CFG_RULE_DEF_ITEM_2, rule, L"NULL", handler, 0);
						}else
						{
								AR_AppendFormatString(code, CFG_RULE_DEF_ITEM_1, rule, prec, handler, 0);
						}
						if(i < cfg->rule_cnt - 1)AR_AppendString(code, L",");
						AR_AppendString(code, L"\n");

						if(rule)AR_DEL(rule);
						if(prec)AR_DEL(prec);
						if(handler)AR_DEL(handler);
				}


				AR_AppendString(code, CFG_RULE_DEF_END);
				AR_AppendString(code, L"\n");
				AR_AppendString(code, L"\n");

				/*
				#define CFG_RULE_DEF_ITEM 	L"{L\"%ls\", \"%ls\", \"%ls\", %d}"
				#define CFG_RULE_DEF_BEGIN	L"struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {"
				*/
		}

		AR_AppendFormatString(code,  CFG_CNT_DEF, L"__RULE_COUNT__", cfg->rule_cnt);


		if(cfg->start.start_rule != NULL)
		{
				AR_AppendFormatString(code, CFG_START_ITEM, cfg->start.start_rule);
		}else if(cfg->rule_cnt > 0)
		{
				AR_AppendFormatString(code, CFG_START_ITEM, cfg->rule[0].lhs);
		}else
		{
				AR_AppendFormatString(code, CFG_START_ITEM, L"");
		}

		AR_AppendString(code, L"\n");

		AR_AppendString(code, L"\n");
		AR_AppendString(code, CFG_DEF_BUILD_LEX);
		AR_AppendString(code, L"\n");


		AR_AppendString(code, L"\n");
		AR_AppendString(code, CFG_DEF_BUILD_GRAMMAR);
		AR_AppendString(code, L"\n");

		if(handler_define)
		{
				AR_AppendString(code, L"\n\n\n\n");
				AR_AppendString(code, AR_GetStrString(handler_define));
				AR_DestroyString(handler_define);
		}

		return true;
}


/*************************************************************************************************************************************************/



void			CFG_Init()
{
		__init_cfg_node_list();
		__init_parser_tag();
}

void			CFG_UnInit()
{
		__uninit_parser_tag();
		__uninit_cfg_node_list();
}




AR_NAMESPACE_END


