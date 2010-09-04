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




/*******************************************辅助模块********************************************/


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

/*******************************************辅助模块结束****************************************************************/

/***************************************************前向声明************************************************/

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


struct __parser_stack_tag;
typedef struct __parser_stack_tag		psrStack_t;

struct __parser_node_stack;
typedef struct __parser_node_stack		psrNodeStack_t;


struct __parser_tag;
typedef struct __parser_tag				parser_t;



struct __parser_context_tag;
typedef struct __parser_context_tag			psrContext_t;


/***************************************************前向声明结束************************************************/





/****************************************************************Symbol***********************************************************************/


typedef enum
{
		PSR_TERM,
		PSR_NONTERM
}psrSymbType_t;




struct __parser_symbol_tag
{
		psrSymbType_t	type;
		const wchar_t	*name;
		uint_t			hash_code;
		size_t			ref_count;
};


const psrSymb_t*		PSR_CreateSymb(const wchar_t *name, psrSymbType_t t);
const psrSymb_t*		PSR_CopyNewSymb(const psrSymb_t *sour);
void					PSR_DestroySymb(const psrSymb_t *symb);
int_t					PSR_CompSymb(const psrSymb_t *l, const psrSymb_t *r);


/***************************************symbol_list***************************************************/

typedef struct __parser_symbol_list_tag
{
		const psrSymb_t **lst;
		size_t			count;
		size_t			cap;	
}psrSymbList_t;

void				PSR_InitSymbList(psrSymbList_t *symb_lst);
void				PSR_UnInitSymbList(psrSymbList_t *symb_lst);
void				PSR_ClearSymbList(psrSymbList_t *symb_lst);

void				PSR_CopySymbList(psrSymbList_t *dest, const psrSymbList_t *sour);

void				PSR_InsertToSymbList(psrSymbList_t *symb_lst, const psrSymb_t *symb);

const psrSymb_t*	PSR_IndexOfSymbList(const psrSymbList_t *symb_lst, size_t idx);
int_t				PSR_FindFromSymbList(const psrSymbList_t *symb_lst, const psrSymb_t* symb);

bool_t				PSR_RemoveFromSymbListByIndex(psrSymbList_t *symb_lst, size_t index);

int_t				PSR_BSearchFromSymbList(const psrSymbList_t *symb_lst, const psrSymb_t* symb);
void				PSR_SortSymbList(psrSymbList_t *symb_lst);



bool_t				PSR_InsertToSymbList_Unique(psrSymbList_t *symb_lst, const psrSymb_t *symb);



/********************************************************************************************************/
void			PSR_PrintSymbol(const psrSymb_t *symb,	arString_t *str);
void			PSR_PrintSymbolList(const psrSymbList_t *lst, arString_t *str);



/**************************************Symbol Map**********************************************************/



typedef struct __parser_symbmap_record_tag
{
		const psrSymb_t			*key;
		psrSymbList_t			lst;
		bool_t					can_empty;
		struct __parser_symbmap_record_tag		*next;
}psrMapRec_t;


#define MAP_BUCKET_SIZE (153)

typedef struct __parser_symbmap_tag
{
		psrMapRec_t		*bucket[MAP_BUCKET_SIZE];
		size_t			item_count;
}psrSymbMap_t;


void					PSR_InitSymbMap(psrSymbMap_t *map);
void					PSR_UnInitSymbMap(psrSymbMap_t *map);

bool_t					PSR_InsertToSymbMap(psrSymbMap_t *map, const psrSymb_t *key, const psrSymb_t *val);
bool_t					PSR_SetSymbEpsilon(psrSymbMap_t *map, const psrSymb_t *key, bool_t is_epsilon);

psrMapRec_t*			PSR_GetSymbolFromSymbMap(const psrSymbMap_t *map, const psrSymb_t *key);

void					PSR_PrintSymbolMap(const psrSymbMap_t *map, arString_t *str);



/****************************************************************Symbol结束***********************************************************************/






/*******************************************************************Grammar*******************************************************************/


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

/****************************************************************************************************************************************/

struct __parser_grammar_tag
{
		psrRule_t				**rules;
		size_t					count;
		size_t					cap;
		
		psrTermInfoList_t		term_list;
		psrSymbList_t			symb_list;

		psrHandler_t			psr_handler;
		
		arIOCtx_t				io_ctx;
};


psrGrammar_t*			PSR_CreateGrammar(const psrHandler_t *ctx, const arIOCtx_t *io_ctx);
void					PSR_DestroyGrammar(psrGrammar_t *grammar);
void					PSR_ClearGrammar(psrGrammar_t *grammar);

void					PSR_ResetGrammarIOContext(psrGrammar_t *grammar, const arIOCtx_t *io_ctx);
void					PSR_ResetGrammarParseHandler(psrGrammar_t *grammar, const psrHandler_t *io_ctx);


const psrHandler_t*		PSR_GetGrammarHandler(const psrGrammar_t *grammar);



int_t					PSR_IndexOfGrammar(const psrGrammar_t *grammar, const psrRule_t *rule);

#define					PSR_GetRuleOfGrammar(_gmr, _idx) ((_gmr)->rules[(_idx)])

const psrSymbList_t*	PSR_GetSymbList(const psrGrammar_t *grammar);


bool_t					PSR_CheckIsValidGrammar(const psrGrammar_t *grammar);




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


#if(0)
bool_t					PSR_ReportLeftFactor(const psrGrammar_t *grammar, arString_t *output);
bool_t					PSR_ReportLeftRecursion(const psrGrammar_t *grammar, arString_t *output);
void					PSR_PrintGrammar(const psrGrammar_t *grammar, arString_t *str);
#endif


/*******************************************************************Grammar结束*******************************************************************/








/***************************************************************Parser************************************************************************/

struct __parser_tag
{
		const psrGrammar_t			*grammar;
		const psrActionTable_t		*tbl;
		psrTermInfoTbl_t			*term_tbl;
		psrExpectedMsg_t			*msg_set;
		size_t						msg_count;

		size_t						ref_cnt;
};



struct __parser_context_tag
{
		const parser_t				*parser;
		bool_t						is_repair;
		bool_t						is_accepted;
		psrStack_t					*state_stack;
		psrNodeStack_t				*node_stack;
		void						*ctx;
};








const parser_t* PSR_CreateParser(const psrGrammar_t *grammar, psrModeType_t type);

void			PSR_DestroyParser(const parser_t *parser);

const	psrGrammar_t*	PSR_GetGrammar(const parser_t *parser);

/*#define			PSR_GetGrammar(_psr)	((const psrGrammar_t*)((_psr)->grammar))*/





/*****************************************Context*****************************************/

psrContext_t*	PSR_CreateContext(const parser_t *parser, void *ctx);
void			PSR_DestroyContext(psrContext_t	*ctx);

void			PSR_Clear(psrContext_t *parser);

psrNode_t*		PSR_GetResult(psrContext_t *parser);/*在状态为accepted之后才可以调用*/

bool_t			PSR_IsAccepted(const psrContext_t *parser);

bool_t			PSR_IsInError(const psrContext_t *parser);

void			PSR_ClearError(psrContext_t *parser);

size_t			PSR_GetNodeCount(const psrContext_t *parser);

psrNode_t*		PSR_IndexOfNodeStack(psrContext_t *parser, size_t index);

/*
		由于采用了一个增广的文法，所以当EOI被增加到stack中时，只可能出现错误或者成为接受状态，EOI永远不会被SHIFT到parser中
*/
bool_t			PSR_AddToken(psrContext_t *parser_context, const psrToken_t *tok);


/***************************************************************Parser结束************************************************************************/





/**************************************************************************报告*******************************************************************/

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
		wchar_t			*lookahead;
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



/**************************************************************************报告结束*******************************************************************/

AR_NAMESPACE_END




#endif
