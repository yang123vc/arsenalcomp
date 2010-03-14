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



#ifndef __PARSER_GRAMMAR_H__
#define __PARSER_GRAMMAR_H__

#include "symbol.h"

AR_NAMESPACE_BEGIN



typedef struct __terminal_info_tag
{
		const	psrSymb_t		*term;
		size_t					prec;
		psrAssocType_t			assoc;
		size_t					val;
		psrTermFunc_t			leaf_f;	/*当有合法的token移入时，将调用对应的leaf_f*/
}psrTermInfo_t;



typedef struct __term_info_list_tag
{
		psrTermInfo_t		*lst;
		size_t				count;
		size_t				cap;
}psrTermInfoList_t;


void			PSR_InitTermInfoList(psrTermInfoList_t	*lst);
void			PSR_UnInitTermInfoList(psrTermInfoList_t	*lst);
void			PSR_ClearTermInfoList(psrTermInfoList_t	*lst);

bool_t			PSR_InsertToTermInfoList(psrTermInfoList_t	*lst, const wchar_t *name, size_t val, psrAssocType_t assoc, size_t prec, psrTermFunc_t	leaf_f);
psrTermInfo_t*	PSR_FindTermByValue(psrTermInfoList_t	*lst, size_t val);
psrTermInfo_t*	PSR_FindTermByName(psrTermInfoList_t	*lst, const wchar_t *name);




/***********************************************************Rule*********************************************************************/




typedef struct __parser_rule_tag
{
		const psrSymb_t			*head;
		psrSymbList_t			body;
		const	wchar_t			*prec_tok;
		psrRuleFunc_t			rule_f;
		size_t					auto_ret;

}psrRule_t;

psrRule_t*		PSR_CreateRule(const psrSymb_t *head, const psrSymbList_t *body, const wchar_t *prec_tok, psrRuleFunc_t rule_f, size_t auto_ret, const psrTermInfoList_t *term_list, arIOCtx_t *ctx);
psrRule_t*		PSR_CreateRuleByStr(const wchar_t *str, const wchar_t *prec, psrRuleFunc_t rule_f, size_t auto_ret, const psrTermInfoList_t *term_list, arIOCtx_t *ctx);
void			PSR_DestroyRule(psrRule_t *rule);
psrRule_t*		PSR_CopyNewRule(const psrRule_t *rule);
bool_t			PSR_IsEmptyRule(const psrRule_t *rule);

/****************************************************************************************************************************************/


typedef struct __parser_grammar_tag
{
		psrRule_t				**rules;
		size_t					count;
		size_t					cap;
		
		psrTermInfoList_t		term_list;
		psrSymbList_t			symb_list;

		psrCtx_t				psr_ctx;
		arIOCtx_t				io_ctx;
}psrGrammar_t;

psrGrammar_t*			PSR_CreateGrammar(const psrCtx_t *ctx, const arIOCtx_t *io_ctx);
void					PSR_DestroyGrammar(psrGrammar_t *grammar);
void					PSR_ClearGrammar(psrGrammar_t *grammar);

void					PSR_ResetGrammarIOContext(psrGrammar_t *grammar, const arIOCtx_t *io_ctx);
void					PSR_ResetGrammarParseContext(psrGrammar_t *grammar, const psrCtx_t *io_ctx);


const psrCtx_t*			PSR_GetGrammarContext(const psrGrammar_t *grammar);
const arIOCtx_t*		PSR_GetGrammarIOContext(const psrGrammar_t *grammar);

ar_int_t					PSR_IndexOfGrammar(const psrGrammar_t *grammar, const psrRule_t *rule);

#define					PSR_GetRuleOfGrammar(_gmr, _idx) ((_gmr)->rules[(_idx)])

const psrSymbList_t*	PSR_GetSymbList(const psrGrammar_t *grammar);


bool_t					PSR_CheckIsValidGrammar(const psrGrammar_t *grammar);
bool_t					PSR_ReportLeftRecursion(const psrGrammar_t *grammar, arString_t *output);
void					PSR_PrintGrammar(const psrGrammar_t *grammar, arString_t *str);


const psrRule_t*		PSR_GetStartRule(const psrGrammar_t *grammar);
bool_t					PSR_SetFirstRule(psrGrammar_t *grammar, const wchar_t *rule_name);

const psrTermInfo_t*	PSR_GetRulePrecAssocInfo(const psrGrammar_t *grammar, const psrRule_t *rule);

psrTermInfo_t*			PSR_GetTermSymbInfo(const psrGrammar_t	*grammar, const psrSymb_t *term);
psrTermInfo_t*			PSR_GetTermSymbInfoByName(const psrGrammar_t	*grammar, const wchar_t *name);
psrTermInfo_t*			PSR_GetTermSymbInfoByValue(const psrGrammar_t	*grammar, size_t val);


bool_t					PSR_InsertTerm(psrGrammar_t *grammar, const wchar_t *name, size_t val, psrAssocType_t assoc, size_t prec, psrTermFunc_t	leaf_f);
bool_t					PSR_InsertRule(psrGrammar_t *grammar, psrRule_t *rule);
bool_t					PSR_InsertRuleByPartStr(psrGrammar_t *grammar, const psrSymb_t *head, const psrSymbList_t *body, const wchar_t *prec_tok, psrRuleFunc_t rule_f, size_t auto_ret);
bool_t					PSR_InsertRuleByStr(psrGrammar_t *grammar, const wchar_t *str, const wchar_t *prec, psrRuleFunc_t rule_f, size_t auto_ret);




void					PSR_CalcFirstSet(const psrGrammar_t *grammar, psrSymbMap_t *first_set);
void					PSR_CalcFollowSet(const psrGrammar_t *grammar, psrSymbMap_t *follow_set, const psrSymbMap_t *first_set);






















AR_NAMESPACE_END


#endif


