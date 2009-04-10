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


typedef struct __parser_rule_tag
{
		size_t					rule_id;
		const psrSymb_t			*head;
		psrSymbList_t			body;
		const	wchar_t			*prec_tok;
}psrRule_t;


typedef struct __parser_grammar_tag
{
		psrRule_t		*rules;
		size_t			count;
		size_t			cap;
		
		psrSymbTbl_t	term_tbl;
		psrSymbList_t	symb_list;
}psrGrammar_t;




void					PSR_InitGrammar(psrGrammar_t *grammar);
void					PSR_UnInitGrammar(psrGrammar_t *grammar);
void					PSR_ClearGrammar(psrGrammar_t *grammar);
void					PSR_CopyGrammar(psrGrammar_t *dest, const psrGrammar_t *grammar);

bool					PSR_InsertTerm(psrGrammar_t *grammar, const psrSymb_t *token);
const psrSymb_t*		PSR_FindTermByName(psrGrammar_t	*grammar, const wchar_t *name);
const psrSymb_t*		PSR_FindTermByValue(psrGrammar_t	*grammar, uint_t val);
bool					PSR_SetTerm(psrGrammar_t		*grammar, const wchar_t *name, psrAssocType_t assoc, size_t prec);


bool					PSR_InsertRule(psrGrammar_t *grammar, const psrSymb_t *head, const psrSymbList_t *lst, const psrSymb_t *term);
const psrRule_t*		PSR_GetRuleByRuleID(const psrGrammar_t *grammar, size_t rule_id);


void					PSR_UpdateGrammar(psrGrammar_t *grammar);
bool					PSR_ConfigGrammar(psrGrammar_t *grammar, const wchar_t *pattern);

const psrSymbList_t*	PSR_GetSymbList(const psrGrammar_t *grammar);

bool					PSR_CheckIsValidGrammar(const psrGrammar_t *grammar);

void					PSR_PrintGrammar(const psrGrammar_t *grammar, arString_t *str);




typedef struct __parser_symbmap_record_tag
{
		const psrSymb_t			*key;
		psrSymbTbl_t			tbl;
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
void					PSR_CalcFollowSet(const psrGrammar_t *grammar, psrSymbMap_t *follow_set, const psrSymbMap_t *first_set);
void					PSR_CalcFirstSet(const psrGrammar_t *grammar, psrSymbMap_t *first_set);
const psrMapRec_t*		PSR_GetSymbolFromSymbMap(const psrSymbMap_t *map, const psrSymb_t *key);


void					PSR_PrintSymbolMap(const psrSymbMap_t *map, arString_t *str);




AR_NAMESPACE_END


#endif


