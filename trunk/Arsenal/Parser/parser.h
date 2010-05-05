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

#ifndef __PARSER_H__
#define __PARSER_H__

#include "../Common/common.h"


AR_NAMESPACE_BEGIN







typedef void			psrNode_t;


typedef struct __parser_token_tag
{
		size_t			term_val;
		const wchar_t	*str;
		size_t			str_cnt;
		size_t			line;
		size_t			col;
}psrToken_t;

#define PSR_TOTERMTOK(_ltok, _psr_tok)							\
		do{														\
				(_psr_tok)->str = (_ltok)->str;					\
				(_psr_tok)->str_cnt = (_ltok)->count;			\
				(_psr_tok)->term_val = (_ltok)->value;			\
				(_psr_tok)->line = (_ltok)->line;				\
				(_psr_tok)->col = (_ltok)->col;					\
		}while(0)


#define PSR_TOLEXTOK(_psr_tok,_ltok)							\
		do{														\
				(_ltok)->str = (_psr_tok)->str;					\
				(_ltok)->count = (_psr_tok)->str_cnt;			\
				(_ltok)->value = (_psr_tok)->term_val;			\
				(_ltok)->line = (_psr_tok)->line;				\
				(_ltok)->col = (_psr_tok)->col;					\
		}while(0)


typedef psrNode_t*		(AR_STDCALL *psrTermFunc_t)(const psrToken_t *tok,void *ctx);
typedef psrNode_t*		(AR_STDCALL *psrRuleFunc_t)(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

typedef void			(AR_STDCALL *psrFreeFunc_t)(psrNode_t *node, void *ctx);

typedef void			(AR_STDCALL *psrErrorFunc_t)(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx);



typedef struct __parser_handler_tag
{
		psrErrorFunc_t	error_f;
		psrFreeFunc_t	free_f;
}psrHandler_t;


/**********************************必须在所有其它PSR族函数之前调用，否则行为未定义***************************/

void	PSR_Init();
void	PSR_UnInit();



#define PSR_EOI_TOKVAL			0/*end of input符号的词法值*/
#define PSR_EPSILON_TOKVAL		1
#define PSR_LALR_TOKVAL			2
#define PSR_ERROR_TOKVAL		3
#define PSR_DEFPREC_TOKVAL		4

#define PSR_MIN_TOKENVAL		256		/*最小终结符value*/


/*
所有符号如无特殊定义，则结合性都为NO_ASSOC，左结合表示在发生冲突时，优先选择同优先级的规约，而右结合与其相反，例如
expr -> id | expr '-' expr;

在 输入为 expr - expr - expr时，如果无结合性，则为冲突，如声明'-'为左结合，则优先归于前面的expr-expr，右结合反之

*/

typedef enum 
{
		PSR_ASSOC_NONASSOC,
		PSR_ASSOC_LEFT,
		PSR_ASSOC_RIGHT
}psrAssocType_t;/*终结符结合性*/


typedef enum
{
		PSR_LR0,
		PSR_SLR = PSR_LR0,
		PSR_LALR
}psrLRItemType_t;

typedef psrLRItemType_t psrModeType_t;



struct __parser_symbol_tag;
typedef struct __parser_symbol_tag		psrSymb_t;

struct __parser_grammar_tag;
typedef struct __parser_grammar_tag		psrGrammar_t;


struct __parser_action_tag;
typedef struct __parser_action_tag		psrActionTable_t;

struct __term_table_tag;
typedef struct __term_table_tag			psrTermInfoTbl_t;

struct __expected_message_tag;
typedef	struct __expected_message_tag	psrExpectedMsg_t;




typedef struct __parser_tag
{
		const psrGrammar_t						*grammar;
		const psrActionTable_t					*tbl;
		psrTermInfoTbl_t						*term_tbl;
		psrExpectedMsg_t						*msg_set;
		size_t									msg_count;

		size_t									ref_cnt;
}parser_t;



struct __parser_stack_tag;
typedef struct __parser_stack_tag		psrStack_t;

struct __parser_node_stack;
typedef struct __parser_node_stack		psrNodeStack_t;

typedef struct __parser_context_tag
{
		const parser_t							*parser;
		bool_t									is_repair;
		bool_t									is_accepted;
		psrStack_t								*state_stack;
		psrNodeStack_t							*node_stack;
		void									*ctx;
}psrContext_t;


const parser_t* PSR_CreateParser(const psrGrammar_t *grammar, psrModeType_t type);

void			PSR_DestroyParser(const parser_t *parser);

const	psrGrammar_t*	PSR_GetGrammar(const parser_t *parser);

/*#define			PSR_GetGrammar(_psr)	((const psrGrammar_t*)((_psr)->grammar))*/


psrContext_t*	PSR_CreateContext(const parser_t *parser, void *ctx);
void			PSR_DestroyContext(psrContext_t	*ctx);


/*****************************************Context*****************************************/

/*
		由于采用了一个增广的文法，所以当EOI被增加到stack中时，只可能出现错误或者成为接受状态，EOI永远不会被SHIFT到parser中
*/

void		PSR_Clear(psrContext_t *parser);

psrNode_t*	PSR_GetResult(psrContext_t *parser);/*在状态为accepted之后才可以调用*/

bool_t		PSR_IsAccepted(const psrContext_t *parser);

bool_t		PSR_IsInError(const psrContext_t *parser);

void		PSR_ClearError(psrContext_t *parser);

size_t		PSR_GetNodeCount(const psrContext_t *parser);

psrNode_t*	PSR_IndexOfNodeStack(psrContext_t *parser, size_t index);

bool_t		PSR_AddToken(psrContext_t *parser_context, const psrToken_t *tok);




/***************************************Print************************************/

void	PSR_PrintParserConflict(const parser_t *parser, arString_t *out);
size_t	PSR_CountParserConflict(const parser_t *parser);
void	PSR_PrintParserActionTable(const parser_t *parser, arString_t *out, size_t width);

/*action table view*/

typedef struct __parser_action_item_view_tag
{
		wchar_t			**item;
		size_t			item_cnt;

		wchar_t	**action_tbl;
		size_t			row;
		size_t			col;
}psrActionView_t;

const psrActionView_t*	PSR_CreateParserActionView(const parser_t *parser);
void					PSR_DestroyParserActionView(const psrActionView_t *view);

#define	PSR_IndexActionViewItem(_v, _n)			((_v)->item[(_n)])
#define PSR_IndexActionViewAction(_v, _x,_y)	((_v)->action_tbl[AR_TBL_IDX_R((_x), (_y), (_v)->col)])

/*conflict*/
typedef struct __parser_conflict_item_tag
{
		wchar_t			*name;
		wchar_t			**items;
		size_t			count;
}psrConflictItem_t;

typedef struct __parser_conflict_view_tag
{
		psrConflictItem_t		**conflict;
		size_t					count;
		size_t					cap;
}psrConflictView_t;


const	psrConflictView_t*		PSR_CreateParserConflictView(const parser_t *parser);
void							PSR_DestroyParserConflictView(const psrConflictView_t *view);


/*first follow view*/
typedef struct __sym_tbl_view_tag
{
		wchar_t	**name;
		wchar_t	**name_set;
		size_t	count;
		size_t	cap;
}psrSymbolMapView_t;




typedef struct __view_info_tag
{
		psrSymbolMapView_t		first_set;
		psrSymbolMapView_t		follow_set;

		psrSymbolMapView_t		left_recursion;
		psrSymbolMapView_t		left_factor;
}psrStatusView_t;

const psrStatusView_t*			PSR_CreateParserStatusView(const parser_t *parser);
void							PSR_DestroyParserStatusView(const psrStatusView_t *view);





AR_NAMESPACE_END




#endif
