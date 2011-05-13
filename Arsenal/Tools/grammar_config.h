
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



#include "../Parser/parser.h"
#include "../Lex/lex.h"



AR_NAMESPACE_BEGIN



void			CFG_Init();
void			CFG_UnInit();


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

		const wchar_t	*code_name;
		bool_t			is_assigned_code_name;

		const	wchar_t	*action_name;
		const	wchar_t	*action_ins;
		size_t			action_line;
}cfgToken_t;


#define CFG_TERM_DEF_BEGIN	L"static struct {const wchar_t *name;\r\nsize_t tokval;\r\nsize_t lex_prec;\r\nconst wchar_t *regex;\r\nbool_t skip;\r\npsrTermFunc_t leaf;\r\n}__g_term_pattern[] =  {"
#define CFG_TERM_DEF_END	L"};"

#define CFG_TERM_DEF_ITEM_1	L"{L\"%ls\", %ls, " L"%" AR_PLAT_INT_FMT L"d, L\"%ls\", %ls, %ls}"
#define CFG_TERM_DEF_ITEM_2	L"{%ls, %ls," L"%" AR_PLAT_INT_FMT L"d, L\"%ls\", %ls, %ls}"




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

#define CFG_PREC_DEF_ITEM 	L"{L\"%ls\", %ls," L"%" AR_PLAT_INT_FMT L"d, %ls}"



typedef struct __cfg_start_tag
{
		size_t	line;
		wchar_t	*start_rule;
}cfgStart_t;

#define CFG_START_ITEM L"#define START_RULE L\"%ls\""


typedef struct __cfg_predef_tag
{
		size_t	line;
		wchar_t	*name;
		wchar_t	*code;
}cfgPreDef_t;


typedef struct __rule_tag
{
		size_t					line;
		const wchar_t			*lhs;
		const wchar_t			*rhs;
		const wchar_t			*prec_tok;
		
		size_t					action_line;
		const wchar_t			*action_name;
		const wchar_t			*action_ins;
}cfgRule_t;


#define	CFG_TOKEN_HANDLER_DECL			L"static psrNode_t* AR_STDCALL %ls(const psrToken_t *tok,void *ctx);"
#define	CFG_TOKEN_HANDLER_DEFINE		L"static psrNode_t* AR_STDCALL %ls(const psrToken_t *tok,void *ctx)\n{\n\t %ls\n}\n"
#define	CFG_TOKEN_HANDLER_DEFINE_2		L"static psrNode_t* AR_STDCALL %ls(const psrToken_t *tok,void *ctx)\n{\n\t return NULL;\n}\n"


#define	CFG_RULE_HANDLER_DECL	L"static psrNode_t* AR_STDCALL handle_%ls(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);"
#define	CFG_RULE_HANDLER_DECL_2	L"static psrNode_t* AR_STDCALL %ls(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);"

#define	CFG_RULE_HANDLER_DEFINE	L"static psrNode_t* AR_STDCALL handle_%ls(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)\n{\n\t return NULL;\n}\n"
#define	CFG_RULE_HANDLER_DEFINE_2	L"static psrNode_t* AR_STDCALL %ls(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)\n{\n\t return NULL;\n}\n"
#define	CFG_RULE_HANDLER_DEFINE_3	L"static psrNode_t* AR_STDCALL %ls(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)\n{\n\t %ls\n}\n"



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

		cfgPreDef_t		*pre_def;
		size_t			predef_cnt;
		
		cfgStart_t		start;

		bool_t			has_error;
}cfgConfig_t;



typedef enum
{
		CFG_REPORT_MESSAGE_T,
		CFG_REPORT_ERROR_T,
		CFG_REPORT_ERROR_LEX_T,
		CFG_REPORT_ERROR_SYNTAX_T,
		CFG_REPORT_WARNING_SYNTAX_T
}cfgReportType_t;



typedef struct __cfg_report_info_tag
{
		cfgReportType_t			type;
		
		union{
		
		struct					{
				const	wchar_t			*message;
		
		}								std_msg;
		
		struct			{
				const	wchar_t			*msg;
				const	psrToken_t		*tok;
		
		}								lex_error;
		
		struct			{
				const	wchar_t			*msg;
				const	psrToken_t		*tok;
		
		}								syntax_error;

		struct	{
				int_t					err_level;
				const	wchar_t			*err_msg;
		
		}								error;

		struct	{
				size_t					line;
				const wchar_t			*msg;
		}								warning;
		};
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
















AR_NAMESPACE_END


