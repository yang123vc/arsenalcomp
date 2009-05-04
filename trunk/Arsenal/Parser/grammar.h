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




/***********************************************************Rule*********************************************************************/




typedef struct __parser_rule_tag
{
		const psrSymb_t			*head;
		psrSymbList_t			body;
		const	wchar_t			*prec_tok;
		psrRuleFunc_t			rule_f;/**/
}psrRule_t;


/****************************************************************************************************************************************/



typedef struct __parser_grammar_tag
{
		psrRule_t				**rules;
		size_t					count;
		size_t					cap;
		
		psrTermInfoList_t		term_list;
		psrSymbList_t			symb_list;
}psrGrammar_t;

psrGrammar_t*			PSR_CreateGrammar();
void					PSR_DestroyGrammar(psrGrammar_t *grammar);
void					PSR_ClearGrammar(psrGrammar_t *grammar);

int_t					PSR_IndexOfGrammar(const psrGrammar_t *grammar, const psrRule_t *rule);

const psrSymbList_t*	PSR_GetSymbList(const psrGrammar_t *grammar);

bool					PSR_CheckIsValidGrammar(const psrGrammar_t *grammar);

void					PSR_PrintGrammar(const psrGrammar_t *grammar, arString_t *str);

const psrRule_t*		PSR_GetStartRule(const psrGrammar_t *grammar);

const psrTermInfo_t*	PSR_GetRulePrecAssocInfo(const psrGrammar_t *grammar, const psrRule_t *rule);

psrTermInfo_t*			PSR_GetTermSymbInfo(const psrGrammar_t	*grammar, const psrSymb_t *term);
psrTermInfo_t*			PSR_GetTermSymbInfoByName(const psrGrammar_t	*grammar, const wchar_t *name);
psrTermInfo_t*			PSR_GetTermSymbInfoByValue(const psrGrammar_t	*grammar, size_t val);


bool					PSR_InsertTerm(psrGrammar_t *grammar, const wchar_t *name, size_t val, psrAssocType_t assoc, size_t prec, psrTermFunc_t	leaf_f);
bool					PSR_InsertRule(psrGrammar_t *grammar, psrRule_t *rule);
bool					PSR_InsertRule2(psrGrammar_t *grammar, const psrSymb_t *head, const psrSymbList_t *body, const wchar_t *prec_tok, psrRuleFunc_t rule_f);
bool					PSR_InsertRule3(psrGrammar_t *grammar, const wchar_t *str, const wchar_t *prec, psrRuleFunc_t rule_f);




typedef struct __parser_symbmap_record_tag
{
		const psrSymb_t			*key;
		psrSymbList_t			lst;
		struct __parser_symbmap_record_tag		*next;
}psrMapRec_t;


#define MAP_BUCKET_SIZE 1024

typedef struct __parser_symbmap_tag
{
		psrMapRec_t		*bucket[MAP_BUCKET_SIZE];
		size_t			item_count;
}psrSymbMap_t;




void					PSR_InitSymbMap(psrSymbMap_t *map);
void					PSR_UnInitSymbMap(psrSymbMap_t *map);

bool					PSR_InsertToSymbMap(psrSymbMap_t *map, const psrSymb_t *key, const psrSymb_t *val);
const psrMapRec_t*		PSR_GetSymbolFromSymbMap(const psrSymbMap_t *map, const psrSymb_t *key);
void					PSR_PrintSymbolMap(const psrSymbMap_t *map, arString_t *str);



void					PSR_CalcFirstSet(const psrGrammar_t *grammar, psrSymbMap_t *first_set);
void					PSR_CalcFollowSet(const psrGrammar_t *grammar, psrSymbMap_t *follow_set, const psrSymbMap_t *first_set);













/*****************************************************PrintNode****************************************************************/


typedef struct __print_node_tag printNode_t;

typedef struct __print_node_tag
{
		wchar_t			*name;
		printNode_t		**nodes;
		size_t			count;
		size_t			cap;
}printNode_t;

extern const psrCtx_t	*PSR_PrintNodeCtx;

printNode_t*	PSR_CreatePrintNode(const wchar_t *name);
void			PSR_DestroyPrintNode(printNode_t *node);
void			PSR_InsertPrintNode(printNode_t *dest, printNode_t *sour);



bool			PSR_ConfigGrammar(psrGrammar_t *gmr, const wchar_t *pattern);





AR_NAMESPACE_END


#endif


