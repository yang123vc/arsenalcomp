
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


#ifndef __PARSER_LALR_DFA_H__
#define __PARSER_LALR_DFA_H__

#include "grammar.h"


AR_NAMESPACE_BEGIN




typedef struct __lalr_config_tag lalrConfig_t;


/**************************************************LALR config list***************************************/
typedef struct __lalr_config_node_tag	
{
		lalrConfig_t					*config;
		struct __lalr_config_node_tag	*next;
}lalrConfigNode_t;

typedef struct __lalr_config_list_tag
{
		lalrConfigNode_t		*head;
		lalrConfigNode_t		*tail;
		size_t					count;
}lalrConfigList_t;


lalrConfigList_t*		PSR_CreateConfigList();
void					PSR_DestroyConfigList(lalrConfigList_t *lst, bool_t destroy_config);
void					PSR_InsertToConfigList(lalrConfigList_t *lst, lalrConfig_t *cfg);

void					PSR_UnionConfigList(lalrConfigList_t *l, lalrConfigList_t *r);
void					PSR_CopyConfigList(lalrConfigList_t *l, const lalrConfigList_t *r);

void					PSR_SortConfigList(lalrConfigList_t *l);
int_t					PSR_CompConfigList(const lalrConfigList_t *l, const lalrConfigList_t *r);

lalrConfig_t*			PSR_FindFromConfigList(lalrConfigList_t *lst, const psrRule_t *rule, size_t delim);
lalrConfig_t*			PSR_InsertToConfigListByValue(lalrConfigList_t *lst, const psrRule_t *rule, size_t delim);

/******************************************************************************************************/


/**************************************************LALR config***************************************/

struct __lalr_config_tag
{
		const psrRule_t			*rule;
		size_t					delim;

/******************************************/
		psrSymbList_t			follow_set;
		lalrConfigList_t		*forward;
		lalrConfigList_t		*backward;
/******************************************/
		bool_t					is_completed;
};

void	PSR_InitConfig(lalrConfig_t *config, const psrRule_t *rule, size_t delim);
void	PSR_UnInitConfig(lalrConfig_t *config);
int_t	PSR_CompConfig(const lalrConfig_t *l, const lalrConfig_t *r);



void PSR_PrintConfig(const lalrConfig_t *config, const psrGrammar_t *gmr, arString_t *str);
void PSR_PrintConfigList(const lalrConfigList_t *lst, const psrGrammar_t *gmr, arString_t *str);


void PSR_Init_LALR_Config();
void PSR_UnInit_LALR_Config();

/**************************************************LALR State***************************************/
typedef struct	__lalr_action_tag			lalrAction_t;
typedef struct	__lalr_state_tag			lalrState_t;

typedef enum 
{
		LALR_ACT_SHIFT,
		LALR_ACT_REDUCE,
		LALR_ACT_ACCEPT
}lalrActionType_t;


struct __lalr_action_tag
{
		lalrActionType_t				act_type;
		const psrSymb_t					*symb;
		lalrState_t						*to;
		const lalrConfig_t				*config;
};


struct __lalr_state_tag
{
		lalrConfigList_t		*basis;
		lalrConfigList_t		*all_config;

		lalrAction_t			*actions;
		size_t					count;
		size_t					cap;
};


lalrState_t*	PSR_CreateState();
void			PSR_DestroyState(lalrState_t *state);

lalrAction_t*	PSR_InsertAction(lalrState_t *state, lalrState_t *to, const psrSymb_t *symb, const lalrConfig_t *config);


void			PSR_DestroyState_ALL(lalrState_t *state);




typedef struct __lalr_state_set_tag
{
		lalrState_t		**set;
		size_t			count;
		size_t			cap;
}lalrStateSet_t;

void			PSR_InitStateSet(lalrStateSet_t *set);
void			PSR_UnInitStateSet(lalrStateSet_t *set);
void			PSR_InsertToStateSet(lalrStateSet_t *set, lalrState_t *state);

void			PSR_CollectState(lalrStateSet_t *empty_set, lalrState_t *start);
lalrState_t*	PSR_FindStateByBasis(lalrStateSet_t *set, lalrConfigList_t *basis);
int_t			PSR_IndexOfStateSet(const lalrStateSet_t *set, const lalrState_t *state);



lalrState_t*   PSR_GetTransTo(lalrState_t *state, const psrSymb_t *symb);


/*******************************************************************************************************/

lalrState_t*	PSR_Create_LR0_State(const psrGrammar_t *grammar);
lalrState_t*	PSR_Create_LALR_State(const psrGrammar_t *grammar);


AR_NAMESPACE_END

#endif