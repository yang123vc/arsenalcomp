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
/*typedef lexToken_t		psrToken_t;*/


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




typedef psrNode_t*		(AR_STDCALL *psrTermFunc_t)(const psrToken_t *tok,void *ctx);
typedef psrNode_t*		(AR_STDCALL *psrRuleFunc_t)(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

typedef void			(AR_STDCALL *psrFreeFunc_t)(psrNode_t *node, void *ctx);

typedef void			(AR_STDCALL *psrErrorFunc_t)(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx);




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






typedef struct __parser_context_tag
{
		psrErrorFunc_t	error_f;
		psrFreeFunc_t	free_f;
		void			*ctx;
}psrCtx_t;



typedef struct __parser_tag
{
		const struct __parser_grammar_tag		*grammar;
		struct __parser_action_tag				*tbl;
		struct __parser_stack_tag				*state_stack;
		struct __parser_node_stack				*node_stack;
		struct __term_table_tag					*term_tbl;
		struct __expected_message_tag			*msg_set;
		bool_t									is_repair;
		bool_t									is_accepted;
		size_t									msg_count;
}parser_t;


parser_t* PSR_CreateParser(const struct __parser_grammar_tag *grammar, psrModeType_t type);

void	  PSR_DestroyParser(parser_t *parser);

void	  PSR_Clear(parser_t *parser);



/*
		由于采用了一个增广的文法，所以当EOI被增加到stack中时，只可能出现错误或者成为接受状态，EOI永远不会被SHIFT到parser中
*/

bool_t	PSR_AddToken(parser_t *parser, const psrToken_t *tok);

psrNode_t* PSR_GetResult(parser_t *parser);/*在状态为accepted之后才可以调用*/

bool_t	PSR_IsAccepted(const parser_t *parser);

bool_t	PSR_IsInError(const parser_t *parser);

void	PSR_ClearError(parser_t *parser);

size_t			PSR_GetNodeCount(const parser_t *parser);
psrNode_t*		PSR_IndexOfNodeStack(parser_t *parser, size_t index);


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
