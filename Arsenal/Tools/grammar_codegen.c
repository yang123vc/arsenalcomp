
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





/****************************************************************生成模板代码部分***********************************************/





static const wchar_t CFG_DEF_BUILD_LEX[] =
L"static lex_t*	__build_lex()													\n"
L"{																				\n"
L"		lex_t	*lex;															\n"
L"		size_t i;																\n"
L"		lex = Lex_Create();														\n"
L"		for(i = 0; i < __NAME_COUNT__; ++i)										\n"
L"		{																		\n"
L"				if(!Lex_Insert(lex, __g_lex_name[i]))							\n"
L"				{																\n"
L"						Lex_Destroy(lex);										\n"
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
L"				if(!Lex_InsertRule(lex, __g_term_pattern[i].regex, &act))		\n"
L"				{																\n"
L"						Lex_Destroy(lex);										\n"
L"						AR_ASSERT(false);										\n"
L"						return NULL;											\n"
L"				}																\n"
L"		}																		\n"
L"		return lex;																\n"
L"}"
;

static const wchar_t CFG_DEF_BUILD_GRAMMAR[] =
L"static psrGrammar_t*	__build_grammar(const psrHandler_t	*handler)															\n"
L"{																																\n"
L"		psrGrammar_t	*grammar;																								\n"
L"		size_t i;																												\n"
L"		AR_ASSERT(handler != NULL);																								\n"
L"		grammar = Parser_CreateGrammar(handler);																				\n"
L"		for(i = 0; i < __TERM_COUNT__; ++i)																						\n"
L"		{																														\n"
L"				if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)continue;										\n"
L"				if(!Parser_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PARSER_ASSOC_NONASSOC,0, __g_term_pattern[i].leaf))	\n"
L"				{																												\n"
L"						Parser_DestroyGrammar(grammar);																			\n"
L"						grammar = NULL;																							\n"
L"						AR_ASSERT(false);																						\n"
L"						return NULL;																							\n"
L"				}																												\n"
L"		}																														\n"
L"		for(i = 0; i < __PREC_COUNT__; ++i)																						\n"
L"		{																														\n"
L"				psrTermInfo_t	*info;																							\n"
L"				info = Parser_GetTermSymbInfoByName(grammar, __g_prec_pattern[i].name);											\n"
L"				if(info == NULL)																								\n"
L"				{																												\n"
L"						if(!Parser_InsertTerm(grammar, __g_prec_pattern[i].name, __g_prec_pattern[i].tokval, __g_prec_pattern[i].assoc, __g_prec_pattern[i].prec_level, NULL))\n"
L"						{																																					\n"
L"								Parser_DestroyGrammar(grammar);																												\n"
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
L"				if(!Parser_InsertRuleByStr(grammar, __g_rule_pattern[i].rule, __g_rule_pattern[i].prec_token, __g_rule_pattern[i].handler, __g_rule_pattern[i].auto_ret))		\n"
L"				{																																							\n"
L"						Parser_DestroyGrammar(grammar);																														\n"
L"						grammar = NULL;																																		\n"
L"						AR_ASSERT(false);																																	\n"
L"						return NULL;																																		\n"
L"				}																																							\n"
L"		}																																									\n"
L"		if(!Parser_SetStartRule(grammar,START_RULE) || !Parser_CheckIsValidGrammar(grammar, NULL))																			\n"
L"		{																																									\n"
L"				Parser_DestroyGrammar(grammar);																																\n"
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



typedef struct __handler_record_tag
{
		wchar_t			*name;
		wchar_t			*handler_def;
		bool_t			has_spec_def;
}handlerRec_t;

typedef struct __handler_table_tag
{
		handlerRec_t	*tbl;
		size_t			count;
		size_t			cap;
}handlerTbl_t;

static void		InitHandlerTable(handlerTbl_t *tbl)
{
		AR_ASSERT(tbl != NULL);
		AR_memset(tbl, 0, sizeof(*tbl));
}

static void ClearHandlerTable(handlerTbl_t *tbl)
{
		size_t	i;
		AR_ASSERT(tbl != NULL);
		for(i = 0; i < tbl->count; ++i)
		{
				handlerRec_t	*rec = &tbl->tbl[i];
				if(rec->name)AR_DEL(rec->name);
				if(rec->handler_def)AR_DEL(rec->handler_def);
		}
		tbl->count = 0;
}


static void		UnInitHandlerTable(handlerTbl_t		*tbl)
{
		AR_ASSERT(tbl != NULL);
		ClearHandlerTable(tbl);
		if(tbl->tbl)AR_DEL(tbl->tbl);
		AR_memset(tbl, 0, sizeof(*tbl));
}

static handlerRec_t*	FindFromHandlerTable(handlerTbl_t		*tbl, const wchar_t *name)
{
		size_t i;
		AR_ASSERT(tbl != NULL && name != NULL);
		for(i = 0; i < tbl->count; ++i)
		{
				handlerRec_t	*rec = &tbl->tbl[i];
				if(AR_wcscmp(rec->name, name) == 0)return rec;
		}
		return NULL;
}

static void			InsertToHandlerTable(handlerTbl_t		*tbl, const wchar_t *name, const wchar_t *handler_def, bool_t has_spec_def)
{
		handlerRec_t *rec;
		AR_ASSERT(tbl != NULL && name != NULL);
		rec = FindFromHandlerTable(tbl,name);

		if(rec == NULL)
		{
				if(tbl->count == tbl->cap)
				{
						tbl->cap = (tbl->cap + 4)*2;
						tbl->tbl = AR_REALLOC(handlerRec_t, tbl->tbl, tbl->cap);
				}
				tbl->tbl[tbl->count].name = AR_wcsdup(name);
				tbl->tbl[tbl->count].handler_def = handler_def == NULL ? NULL : AR_wcsdup(handler_def);
				tbl->tbl[tbl->count].has_spec_def = has_spec_def;
				tbl->count++;
		}else
		{
				if(rec->handler_def)
				{
						AR_DEL(rec->handler_def);
						rec->handler_def = NULL;
				}
				
				rec->handler_def = handler_def == NULL ? NULL : AR_wcsdup(handler_def);
				rec->has_spec_def = has_spec_def;
		}
		
}





bool_t			CFG_ConfigToCode(const cfgConfig_t *cfg, arString_t	*code)
{
		size_t i;
		arString_t		*handler_define = NULL;
		AR_ASSERT(cfg != NULL && code != NULL);

		if(cfg->has_error)return false;

		handler_define	= AR_CreateString();

		if(cfg->predef_cnt > 0)
		{
				for(i = 0; i < cfg->predef_cnt; ++i)
				{
						if(cfg->pre_def[i].flags & CFG_CODE_ON_BOTTOM_F)
						{
								continue;
						}
						AR_AppendString(code, L"\r\n");


						if(cfg->pre_def[i].name && AR_wcslen(cfg->pre_def[i].name) > 0)
						{
								AR_AppendFormatString(code, L"%ls", L"\r\n/*\r\n");
								AR_AppendFormatString(code, L"%ls", cfg->pre_def[i].name);
								AR_AppendFormatString(code, L"%ls", L"\r\n*/\r\n");
						}


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


		
/*********************************************************************************************************************/
		
		if(cfg->tok_cnt > 0)	/*声明终结符处理函数*/
		{
				handlerTbl_t	token_handler_tbl;

				InitHandlerTable(&token_handler_tbl);
				
				for(i = 0; i < cfg->tok_cnt; ++i)
				{
						const cfgToken_t *tok = &cfg->tok[i];
						if(tok->name && tok->action_name)
						{
								handlerRec_t	*rec;
								rec = FindFromHandlerTable(&token_handler_tbl, tok->action_name);
								
								if(rec == NULL)
								{
										InsertToHandlerTable(&token_handler_tbl, tok->action_name, tok->action_ins, tok->action_ins == NULL ? false : true);
								}else
								{
										if(rec->has_spec_def)
										{
												if(tok->action_ins)
												{
														InsertToHandlerTable(&token_handler_tbl, tok->action_name, tok->action_ins, true);
												}else
												{

												}
										}else
										{
												InsertToHandlerTable(&token_handler_tbl, tok->action_name, tok->action_ins, tok->action_ins == NULL ? false : true);
										}
								}
						}
				}

				for(i = 0; i < token_handler_tbl.count; ++i)
				{
						const handlerRec_t *rec = &token_handler_tbl.tbl[i];
						
						AR_AppendString(code, L"\r\n");
						AR_AppendFormatString(code, CFG_TOKEN_HANDLER_DECL, rec->name); 
						AR_AppendString(code, L"\r\n\r\n");
						
						if(rec->has_spec_def)
						{
								AR_AppendFormatString(handler_define, CFG_TOKEN_HANDLER_DEFINE, rec->name, rec->handler_def);
						}else
						{
								AR_AppendFormatString(handler_define, CFG_TOKEN_HANDLER_DEFINE_2, rec->name);
						}

						AR_AppendString(handler_define, L"\r\n\r\n");
				}

				UnInitHandlerTable(&token_handler_tbl);
		}		
/*********************************************************************************************************************/


		if(cfg->tok_cnt > 0)/*为存在名字的词法值生成枚举*/
		{
				arString_t		*enum_str;
				enum_str = AR_CreateString();
				AR_AppendString(enum_str, L"\r\n\r\n\r\n/*enum{\r\n");
				for(i = 0; i < cfg->tok_cnt; ++i)
				{
						/*if(cfg->tok[i].code_name == NULL)continue;*/
						AR_ASSERT(cfg->tok[i].code_name != NULL);

						if(cfg->tok[i].is_assigned_code_name)
						{
								AR_AppendFormatString(enum_str, L"%s = %Id,\r\n", cfg->tok[i].code_name, cfg->tok[i].tokval);
						}
				}
				AR_AppendString(enum_str, L"};*/\r\n\r\n\r\n\r\n");
				AR_AppendString(code, AR_GetStringCString(enum_str));
				AR_DestroyString(enum_str);
		}

		if(cfg->tok_cnt > 0)
		{
				AR_AppendString(code, L"\n");
				AR_AppendString(code, CFG_TERM_DEF_BEGIN);
				AR_AppendString(code, L"\n");

				

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
								const wchar_t *leaf_name = cfg->tok[i].action_name == NULL ? L"NULL" : cfg->tok[i].action_name;
								AR_AppendFormatString(code, CFG_TERM_DEF_ITEM_1, name, cfg->tok[i].code_name, cfg->tok[i].lex_prec, regex, cfg->tok[i].is_skip ? L"true" : L"false", leaf_name);
						}else
						{
								const wchar_t *leaf_name = cfg->tok[i].action_name == NULL ? L"NULL" : cfg->tok[i].action_name;
								AR_AppendFormatString(code, CFG_TERM_DEF_ITEM_2, L"NULL", cfg->tok[i].code_name, cfg->tok[i].lex_prec, regex, cfg->tok[i].is_skip ? L"true" : L"false", leaf_name);
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


/************************************************生成优先级相关代码***************************************************************/
		if(cfg->prec_cnt > 0)
		{
				AR_AppendString(code, L"\n");
				AR_AppendString(code, CFG_PREC_DEF_BEGIN);
				AR_AppendString(code, L"\n");

				

				for(i = 0; i < cfg->prec_cnt; ++i)
				{
						size_t k;
						size_t prec = cfg->prec[i].prec_level;
						const wchar_t *assoc;

						switch(cfg->prec[i].assoc)
						{
						case PARSER_ASSOC_NONASSOC:
								assoc = L"PARSER_ASSOC_NONASSOC";
								break;
						case PARSER_ASSOC_LEFT:
								assoc = L"PARSER_ASSOC_LEFT";
								break;
						default:/*case PARSER_ASSOC_RIGHT:*/
								assoc = L"PARSER_ASSOC_RIGHT";
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
										code_name = AR_vtow(L"%Id", tok_val);
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

/**********************************************************************************************************************************************************/
		if(cfg->rule_cnt > 0)
		{
				handlerTbl_t	handler_tbl;
				
				InitHandlerTable(&handler_tbl);

				AR_AppendString(code, L"\n");

				for(i = 0; i < cfg->rule_cnt; ++i)
				{
						size_t tmp_len;
						wchar_t *handler, *handler_def, *tmp;
						bool_t	has_spec_def = false;
/***************************************************生成handler和handler_def*******************************************************/
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
								if(AR_swprintf(tmp, tmp_len, CFG_RULE_HANDLER_DECL_2, cfg->rule[i].action_name) < 0)
								{
										AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
								}
								handler = AR_wcsdup(tmp);

								if(cfg->rule[i].action_ins == NULL)
								{
										has_spec_def = false;
										if(AR_swprintf(tmp, tmp_len, CFG_RULE_HANDLER_DEFINE_2, cfg->rule[i].action_name) < 0)
										{
												AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
										}
										handler_def = AR_wcsdup(tmp);
								}else
								{
										has_spec_def = true;
										
										if(AR_swprintf(tmp, tmp_len, CFG_RULE_HANDLER_DEFINE_3, cfg->rule[i].action_name, cfg->rule[i].action_ins) < 0)
										{
												AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
										}

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
								if(AR_swprintf(tmp, tmp_len, CFG_RULE_HANDLER_DECL, cfg->rule[i].lhs) < 0)
								{
										AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
								}
								handler = AR_wcsdup(tmp);

								if(AR_swprintf(tmp, tmp_len, CFG_RULE_HANDLER_DEFINE, cfg->rule[i].lhs) < 0)
								{
										AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
								}
								handler_def = AR_wcsdup(tmp);

								if(tmp)
								{
										AR_DEL(tmp);
										tmp = NULL;
								}

								has_spec_def = false;
						}
/****************************************************************************************************************************/
			
						{
								size_t k;
								handlerRec_t	*rec;
								arString_t		*comment = AR_CreateString();
								arString_t		*define  = AR_CreateString();

/****************************************************生成handler对应的注释*****************************************************************/
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
/****************************************************************************************************************************/
								
								AR_AppendString(define, AR_GetStringCString(comment));
								AR_AppendString(define, handler_def);
								AR_AppendString(define, L"\r\n\n");

								rec = FindFromHandlerTable(&handler_tbl, handler);
								if(rec == NULL)
								{
										AR_AppendString(code, AR_GetStringCString(comment));
										AR_AppendString(code, handler);
										AR_AppendString(code, L"\r\n\n");
										
										InsertToHandlerTable(&handler_tbl, handler, AR_GetStringCString(define), has_spec_def);
								}else
								{
										if(rec->has_spec_def)
										{
												if(has_spec_def)
												{
														InsertToHandlerTable(&handler_tbl, handler, AR_GetStringCString(define), has_spec_def);
												}else
												{

												}
										}else
										{
												InsertToHandlerTable(&handler_tbl, handler, AR_GetStringCString(define), has_spec_def);
										}
								}

								AR_DestroyString(define);
								AR_DestroyString(comment);
						}
						
						if(handler)AR_DEL(handler);
						if(handler_def)AR_DEL(handler_def);
				}

				for(i = 0; i < handler_tbl.count; ++i)
				{
						const handlerRec_t		*rec = &handler_tbl.tbl[i];
						AR_AppendString(handler_define, rec->handler_def);
						AR_AppendString(handler_define, L"\r\n\r\n");
				}
				AR_AppendString(code, L"\r\n");
				AR_AppendString(code, L"\r\n");

				UnInitHandlerTable(&handler_tbl);
		}

/**********************************************************************************************************************************************************/


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
								if(AR_swprintf(tmp, 2048, L"handle_%ls", cfg->rule[i].lhs) < 0)
								{
										AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
								}
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
				AR_AppendString(code, L"\r\n\r\n\r\n");
				AR_AppendString(code, AR_GetStringCString(handler_define));
				AR_DestroyString(handler_define);
		}




		if(cfg->predef_cnt > 0)
		{
				for(i = 0; i < cfg->predef_cnt; ++i)
				{
						if(cfg->pre_def[i].flags & CFG_CODE_ON_BOTTOM_F)
						{
								
								AR_AppendString(code, L"\r\n");


								if(cfg->pre_def[i].name && AR_wcslen(cfg->pre_def[i].name) > 0)
								{
										AR_AppendFormatString(code, L"%ls", L"\r\n/*\r\n");
										AR_AppendFormatString(code, L"%ls", cfg->pre_def[i].name);
										AR_AppendFormatString(code, L"%ls", L"\r\n*/\r\n");
								}


								AR_AppendString(code,  cfg->pre_def[i].code);
								AR_AppendString(code, L"\r\n");
						}
				}
		}

		AR_AppendString(code, L"\r\n\r\n\r\n");


		return true;
}


/*************************************************************************************************************************************************/





AR_NAMESPACE_END


