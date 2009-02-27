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
#ifndef __PARSER_IN_H__
#define __PARSER_IN_H__


#include "parser.h"





typedef struct __parser_symbol_tag psrSymb_t;

/*
所有符号如无特殊定义，则结合性都为NO_ASSOC，左结合表示在发生冲突时，优先选择同优先级的规约，而右结合与其相反，例如
expr -> id | expr '-' expr;

在 输入为 expr - expr - expr时，如果无结合性，则为冲突，如声明'-'为左结合，则优先归于前面的expr-expr，右结合反之

*/
typedef enum 
{
		PSR_ASSO_LEFT,
		PSR_ASSO_RIGHT,
		PSR_ASSO_NOASSO
}psrAssoType_t;/*终结符结合性*/

typedef struct __parser_symbol_tag
{
		wchar_t			name[PARSER_MAXNAME];
		bool_t			non_term;/*是否为非终结符*/
		size_t			priority;/*优先级，如果为<PSR_MIN_PRIOVAL则为无优先级*/
		psrAssoType_t	asso;/*结合性*/
		psrSymb_t		*next;

		union{
				size_t	val;/*对终结符来说为值，否则反之*/
				size_t	id;
		};
}psrSymb_t;


/*以下为终结符*/
extern const psrSymb_t	*PSR_EpsilonSymb;/*表示空*/
extern const psrSymb_t	*PSR_EOISymb;   /*表示输入结尾符号*/
extern const psrSymb_t	*PSR_LALRSymb; /*表示为不在所有字符值集合中的字符，计算LALR语法分析表时使用*/

/*以下都为非终结符*/
extern const psrSymb_t	*PSR_StartSymb;/*第一个符号，被转换为 %Start -> gmr->head[0]*/

psrSymb_t* PSR_CopyNewSymb(const psrSymb_t *sour);
psrSymb_t* PSR_DestroySymb(psrSymb_t *sour);
void	PSR_DestroySymbList(psrSymb_t *list);

psrSymb_t* PSR_FindSymb(psrSymb_t *start, const psrSymb_t *key);
void	PSR_InsertSymb(psrSymb_t **psymbs, const psrSymb_t *item);
bool_t PSR_InsertSymb_Unique(psrSymb_t **psymbs, const psrSymb_t *item);

psrSymb_t* PSR_CopySymbList(const psrSymb_t *sour);



int PSR_CompSymb(const psrSymb_t *left, const psrSymb_t *right);
bool_t PSR_IsNullable(const psrSymb_t *symb);

psrError_t PSR_SetSymb(psrSymb_t *symb, const wchar_t *name, bool_t is_nt, size_t prio, psrAssoType_t asso, size_t val);

/***********************for lr itemset*****************************************/

int				PSR_CompSymbList(const psrSymb_t *left, const psrSymb_t *right);
bool_t			PSR_InsertSymbByOrder_Unique(psrSymb_t **psymbs, const psrSymb_t *item);
psrSymb_t*		PSR_CopyNewSymbList(const psrSymb_t *sour);
		
size_t			PSR_CountSymbList(const psrSymb_t *lst);
const psrSymb_t* PSR_IndexOfSymbList(const psrSymb_t *lst, size_t index);

















/***********************************************************************************************************************/

typedef struct __parser_grammar_tag
{
		psrSymb_t		**head;/*非终结符组成的产生式的头集合，例如A->a|b会得到 head[1] = <A> && head[2] == <A>*/
		psrSymb_t		**body;
		
		size_t			count;
		size_t			cap;
		
		psrSymb_t		**nonterm_set;/*非终结符集合, 例如A->a|b会得到 nonterm_set[1] = <A>*/
		psrSymb_t		**first;/*first集合,first[i]为nonterm_set[i]的非终结符的first集合*/
		psrSymb_t		**follow;/*follow集合，同上*/
		size_t			non_count;

		psrSymb_t		**term_set;/*终结符集合*/
		size_t			term_count;
		size_t			term_cap;
		
		psrSymb_t		*symb_list;
}psrGrammar_t;



psrGrammar_t*	PSR_BuildGrammar(const wchar_t *input);

void			PSR_DestroyGrammar(psrGrammar_t* grammar);

const psrSymb_t* PSR_GetFirstSetBySymb(const psrGrammar_t *grammar,const psrSymb_t *symb);

const psrSymb_t* PSR_GetFollowSetBySymb(const psrGrammar_t *grammar,const psrSymb_t *symb);

const psrSymb_t* PSR_GetRuleHeadByRuleID(const psrGrammar_t *grammar,size_t id);
const psrSymb_t* PSR_GetRuleBodyByRuleID(const psrGrammar_t *grammar,size_t id);
const psrSymb_t* PSR_GetTermSymbByValue(const psrGrammar_t *grammar,size_t value);

/***************************************LR Item && ItemSet****************************************************************/

typedef struct __parser_lr_item_tag psrLRItem_t;


typedef struct __parser_lr_item_tag
{
		size_t			rule_id;
		size_t			delim;
		
		psrSymb_t		*symb;/*将在规范LR1或者LALR1中使用,规范LR1中永远只有一个，LALR中有N个，N > 1，其他时候永远为NULL*/
		
		psrLRItem_t		*next;
}psrLRItem_t;


int				PSR_CompLRItem(const psrLRItem_t *left, const psrLRItem_t *right);
bool_t			PSR_InsertLRItem_Unique(psrLRItem_t **dest, const psrLRItem_t *sour);
psrLRItem_t*	PSR_CopyNewLRItem(const psrLRItem_t *sour);

psrLRItem_t*	PSR_DestroyLRItem(psrLRItem_t *item);
void			PSR_DestroyLRItemList(psrLRItem_t *lst);

psrLRItem_t*	PSR_FindLRItem(psrLRItem_t *sour, const psrLRItem_t *key);
psrLRItem_t*	PSR_CopyNewLRItemList(const psrLRItem_t *sour);
int				PSR_CompLRItemList(const psrLRItem_t *left, const psrLRItem_t *right);

/*比较left和right，如果返回True则表明left中的所有元素都存在于right中,此函数调用PSR_CompLRItem*/
bool_t			PSR_IsIncludesLRItemList(const psrLRItem_t *left, const psrLRItem_t *right);

/************************************************语法分析表*********************************************************************/

typedef enum
{
		PSR_SHIFT,
		PSR_REDUCE,
		PSR_ACCEPT,
		PSR_ERROR
}psrActionType_t;

typedef struct __action_record_tag psrAction_t;

typedef struct __action_record_tag
{
		psrActionType_t			type;
		size_t					priority;
		psrAction_t				*next;
		

		union{
				size_t			shift_to;
				size_t			reduce_id;
		};
		
		psrLRItem_t				item;/*从哪项得到的*/
		size_t					reduce_count;/*规约时用到，产生式如果为%Epsilon，则count为0*/
}psrAction_t;




typedef struct __parser_action_tag
{
		psrSymb_t		*term_set;
		size_t			term_count;
		psrAction_t		**actions;

		size_t			row;
		size_t			col;


		psrSymb_t		*non_term_set;
		size_t			nonterm_count;

		int				*goto_tbl;
		size_t			goto_row;
		size_t			goto_col;
		
}psrActionTable_t;

void					PSR_DestroyActionTable(psrActionTable_t *tbl);
const psrAction_t*		PSR_GetAction(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb);
int						PSR_GetState(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb);



psrActionTable_t* PSR_BuildActionTable_SLR(const psrGrammar_t *grammar);
psrActionTable_t* PSR_BuildActionTable_LR1(const psrGrammar_t *grammar);
psrActionTable_t* PSR_BuildActionTable_LALR(const psrGrammar_t *grammar);




/***********************************************Parser*********************/

typedef struct __parser_tag 
{
		psrActionTable_t		*tbl;
		psrGrammar_t			*grammar;
		lex_t					*lex;

		psrLeaf_func			leaf_func;
		psrNode_func			node_func;
		psrDestroy_func			destroy_func;
}parser_t;















#endif


