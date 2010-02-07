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


#include "../Parser/grammar.h"
#include "../Parser/parser.h"
#include "../Lex/lex.h"



AR_NAMESPACE_BEGIN




struct __cfg_node_tag;
typedef struct __cfg_node_tag	cfgNode_t;

typedef struct __cfg_lexeme_tag
{
		size_t			line;
		const wchar_t	*lexeme;
		size_t			lex_val;
}cfgLexeme_t;

typedef struct __cfg_lex_name_tag
{
		size_t			line;
		const wchar_t	*name;
		const wchar_t	*regex;
}cfgName_t;

#define CFG_NAME_DEF_BEGIN		L"static const wchar_t *__g_lex_name[] = {"
#define CFG_NAME_DEF_END		L"};"
#define CFG_NAME_DEF_ITEM		L"L\"%ls = %ls\""


typedef struct __cfg_token_tag
{
		size_t			line;
		const wchar_t	*name;
		size_t			tokval;
		size_t			lex_prec;
		const wchar_t	*regex;
		bool_t			is_skip;
}cfgToken_t;


#define CFG_TERM_DEF_BEGIN	L"static struct {const wchar_t *name; size_t tokval; size_t lex_prec; const wchar_t *regex; bool_t skip; }__g_term_pattern[] =  {"
#define CFG_TERM_DEF_END	L"};"
#define CFG_TERM_DEF_ITEM_1	L"{L\"%ls\", %" AR_PLAT_INT_FMT L"d, %" AR_PLAT_INT_FMT L"d, L\"%ls\", %ls}"
#define CFG_TERM_DEF_ITEM_2	L"{%ls, %" AR_PLAT_INT_FMT L"d, %" AR_PLAT_INT_FMT L"d, L\"%ls\", %ls}"



typedef struct __cfg_prec_tag
{
		size_t	line;
		
		psrAssocType_t	assoc;
		size_t			prec_level;
		
		wchar_t			**prec_tok_set;
		size_t			*prec_tok_val;
		
		size_t			count;
		size_t			cap;
}cfgPrec_t;

#define CFG_PREC_DEF_BEGIN	L"static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {"
#define CFG_PREC_DEF_END	L"};"
#define CFG_PREC_DEF_ITEM 	L"{L\"%ls\", %" AR_PLAT_INT_FMT L"d, %" AR_PLAT_INT_FMT L"d, %ls}"


typedef struct __cfg_start_tag
{
		size_t	line;
		wchar_t	*start_rule;
}cfgStart_t;

#define CFG_START_ITEM L"#define START_RULE L\"%ls\""

typedef struct __rule_tag
{
		size_t					line;
		const wchar_t			*lhs;
		const wchar_t			*rhs;
		const wchar_t			*prec_tok;
		const wchar_t			*action_ins;
}cfgRule_t;


#define	CFG_RULE_HANDLER_DECL	L"static psrNode_t* AR_STDCALL handle_%ls(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);"
#define	CFG_RULE_HANDLER_DECL_2	L"static psrNode_t* AR_STDCALL %ls(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);"

#define	CFG_RULE_HANDLER_DEFINE	L"static psrNode_t* AR_STDCALL handle_%ls(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)\n{\n\t return NULL;\n}\n"
#define	CFG_RULE_HANDLER_DEFINE_2	L"static psrNode_t* AR_STDCALL %ls(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)\n{\n\t return NULL;\n}\n"



#define CFG_RULE_DEF_ITEM_1 	L"{L\"%ls\", L\"%ls\", %ls, %" AR_PLAT_INT_FMT L"d}"
#define CFG_RULE_DEF_ITEM_2		L"{L\"%ls\", %ls, %ls, %" AR_PLAT_INT_FMT L"d}"

#define CFG_RULE_DEF_BEGIN	L"static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {"
#define CFG_RULE_DEF_END	L"};"




typedef struct __cfg_node_list_tag
{
		cfgNode_t		**lst;
		size_t			count;
		size_t			cap;
}cfgNodeList_t;



typedef struct __cfg_config_tag
{
		
		cfgName_t		*name;
		size_t			name_cnt;

		cfgToken_t		*tok;
		size_t			tok_cnt;

		cfgPrec_t		*prec;
		size_t			prec_cnt;

		cfgRule_t		*rule;
		size_t			rule_cnt;
		
		cfgStart_t		start;

		bool_t			has_error;
}cfgConfig_t;



#define CFG_DEF_BUILD_LEX														\
L"static lex_t*	__build_lex(const arIOCtx_t *io)								\n\
{																				\n\
		lex_t	*lex;															\n\
		size_t i;																\n\
		lex = LEX_Create(io);													\n\
		for(i = 0; i < AR_NELEMS(__g_lex_name); ++i)							\n\
		{																		\n\
				if(!LEX_Insert(lex, __g_lex_name[i]))							\n\
				{																\n\
						LEX_Destroy(lex);										\n\
						AR_ASSERT(false);										\n\
						return NULL;											\n\
				}																\n\
		}																		\n\
		for(i = 0; i < AR_NELEMS(__g_term_pattern); ++i)						\n\
		{																		\n\
				lexAction_t		act;											\n\
				act.is_skip		=		__g_term_pattern[i].skip;				\n\
				act.priority	=		__g_term_pattern[i].lex_prec;			\n\
				act.value		=		__g_term_pattern[i].tokval;				\n\
				if(!LEX_InsertRule(lex, __g_term_pattern[i].regex, &act))		\n\
				{																\n\
						LEX_Destroy(lex);										\n\
						AR_ASSERT(false);										\n\
						return NULL;											\n\
				}																\n\
		}																		\n\
		return lex;																\n\
}"



#define CFG_DEF_BUILD_GRAMMAR																										\
L"static psrGrammar_t*	__build_grammar(const psrCtx_t	*psr_ctx, const arIOCtx_t *io)											\n	\
{																																\n	\
		psrGrammar_t	*grammar;																								\n	\
		size_t i;																												\n	\
		AR_ASSERT(psr_ctx != NULL);																								\n	\
		grammar = PSR_CreateGrammar(psr_ctx, io);																				\n	\
		for(i = 0; i < AR_NELEMS(__g_term_pattern); ++i)																		\n	\
		{																														\n	\
				if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)continue;										\n	\
				if(!PSR_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PSR_ASSOC_NOASSOC,0, NULL))	\n	\
				{																												\n	\
						PSR_DestroyGrammar(grammar);																			\n	\
						grammar = NULL;																							\n	\
						AR_ASSERT(false);																						\n	\
						return NULL;																							\n	\
				}																												\n	\
		}																														\n	\
		for(i = 0; i < AR_NELEMS(__g_prec_pattern); ++i)																		\n	\
		{																														\n	\
				psrTermInfo_t	*info;																							\n	\
				info = PSR_GetTermSymbInfoByName(grammar, __g_prec_pattern[i].name);											\n	\
				if(info == NULL)																								\n	\
				{																												\n	\
						if(!PSR_InsertTerm(grammar, __g_prec_pattern[i].name, __g_prec_pattern[i].tokval, __g_prec_pattern[i].assoc, __g_prec_pattern[i].prec_level, NULL))\n	\
						{																																					\n	\
								PSR_DestroyGrammar(grammar);																												\n	\
								grammar = NULL;																																\n	\
								AR_ASSERT(false);																															\n	\
								return NULL;																																\n	\
						}																																					\n	\
				}else																																						\n	\
				{																																							\n	\
						info->assoc = __g_prec_pattern[i].assoc;																											\n	\
						info->prec = __g_prec_pattern[i].prec_level;																										\n	\
				}																																							\n	\
		}																																									\n	\
		for(i = 0; i < AR_NELEMS(__g_rule_pattern); ++i)																													\n	\
		{																																									\n	\
				if(!PSR_InsertRuleByStr(grammar, __g_rule_pattern[i].rule, __g_rule_pattern[i].prec_token, __g_rule_pattern[i].handler, __g_rule_pattern[i].auto_ret))		\n	\
				{																																							\n	\
						PSR_DestroyGrammar(grammar);																														\n	\
						grammar = NULL;																																		\n	\
						AR_ASSERT(false);																																	\n	\
						return NULL;																																		\n	\
				}																																							\n	\
		}																																									\n	\
		if(!PSR_SetFirstRule(grammar,START_RULE) || !PSR_CheckIsValidGrammar(grammar))																						\n	\
		{																																									\n	\
				PSR_DestroyGrammar(grammar);																																\n	\
				grammar = NULL;																																				\n	\
				AR_ASSERT(false);																																			\n	\
				return NULL;																																				\n	\
		}																																									\n	\
		return grammar;																																						\n	\
}"








typedef enum
{
		CFG_REPORT_MESSAGE_T,
		CFG_REPORT_ERROR_T,
		CFG_REPORT_ERR_LEX_T,
		CFG_REPORT_ERR_SYNTAX_T
}cfgReportType_t;

typedef struct __cfg_report_info_tag
{
		cfgReportType_t			type;
		const	psrToken_t		*tok;
		const	wchar_t			*message;
		int_t					err_level;
}cfgReportInfo_t;


typedef void (AR_STDCALL *cfgReportFunc_t)(const cfgReportInfo_t *report, void *context);

typedef struct __cfg_report_tag
{
		cfgReportFunc_t	report_func;
		void			*report_ctx;
}cfgReport_t;



cfgConfig_t*	CFG_CollectGrammarConfig(const wchar_t *gmr_txt, cfgReport_t	*report);

void			CFG_DestroyGrammarConfig(cfgConfig_t *cfg);

bool_t			CFG_ConfigToCode(const cfgConfig_t *cfg, arString_t	*code);




void			CFG_Init();
void			CFG_UnInit();

AR_NAMESPACE_END


