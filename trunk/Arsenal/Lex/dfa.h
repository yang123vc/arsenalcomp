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

#ifndef __LEX_DFA_H__
#define __LEX_DFA_H__

#include "node.h"


AR_NAMESPACE_BEGIN

typedef struct __lex_dfa_tag		lexDFA_t;

typedef struct __lex_dfa_arc_tag
{
		int_t			match_id;
		lexDFA_t		*start;
		lexDFA_t		*to;
		
}lexDFAArc_t;


typedef struct __lex_dfa_tag
{
		lexDFAArc_t		*out;
		size_t			out_count;
		size_t			out_cap;

		lexDFAArc_t		*in;
		size_t			in_count;
		size_t			in_cap;
		
		lexAction_t		action;
		bool			is_final;
		lexNodeList_t	*pos_lst;
}lexDFA_t;



lexDFA_t*		LEX_CreateDFA();
void			LEX_DestroyDFA(lexDFA_t* vtx);/*会先调用LEX_DisConnectVertex清除一切相关连的边，之后调用AR_DEL清理vtx内存*/

void			LEX_ConnectDFA(lexDFA_t *from, lexDFA_t *to, int_t match_id);
void			LEX_DisConnect(lexDFA_t *vtx, lexDFAArc_t *arc);
void			LEX_DestroyDFA_ALL(lexDFA_t *start);

/***************************************************lexDFASet_t****************************************/


typedef struct __lex_dfa_set_tag
{
		lexDFA_t	**set;
		size_t		count;
		size_t		cap;
}lexDFASet_t;

void	LEX_InitDFASet(lexDFASet_t *set);
void	LEX_UnInitDFASet(lexDFASet_t *set);
void	LEX_InsertToDFASet(lexDFASet_t *set, lexDFA_t *vtx);
void	LEX_CollectDFA(lexDFASet_t *empty_set, lexDFA_t *start);



/*****************************************StateTable_t*********************************************************/

lexDFA_t* LEX_BuildDFA(lexCClass_t *cclass, lexLeafSet_t *leafset);


typedef struct __lex_state_table_tag
{
		lexAction_t		*actions;
		bool			*is_final;
		size_t			count;
		
		int_t			*tbl;
		size_t			row;
		size_t			col;
}lexStateTable_t;


void LEX_InitStateTable(lexStateTable_t *tbl);
void LEX_UnInitStateTable(lexStateTable_t *tbl);
void LEX_BuildStateTable(lexStateTable_t *tbl, lexDFA_t *dfa, const lexCClass_t *cclass);

inline int_t LEX_GetNextState(const lexStateTable_t *tbl, int_t curr, size_t id)
{
		AR_ASSERT(id < tbl->col && (size_t)curr < tbl->row && curr >= 0);
		return tbl->tbl[AR_TBL_IDX_R((size_t)curr, id, tbl->col)];
}


AR_NAMESPACE_END

#endif


