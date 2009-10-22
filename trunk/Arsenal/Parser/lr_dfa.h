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
#ifndef __PSR_DFA_H__
#define __PSR_DFA_H__

#include "parser.h"
#include "lr_item.h"

AR_NAMESPACE_BEGIN



/**************************************************************************************************/


/*typedef struct __parser_lr_dfa_tag		psrDFA_t;*/

typedef struct __parser_lr_dfa_arc_tag
{
		const psrSymb_t					*symb;
		struct __parser_lr_dfa_tag		*to;
}psrDFAArc_t;

typedef struct __parser_lr_dfa_tag		
{
		psrDFAArc_t				*out;
		size_t					out_count;
		size_t					out_cap;
		psrLRItemTbl_t			tbl;
}psrDFA_t;


psrDFA_t*	PSR_CreateDFA();
void		PSR_DestroyDFA(psrDFA_t *dfa);
void		PSR_DestroyDFA_ALL(psrDFA_t *start);
void		PSR_ConnectDFA(psrDFA_t *start, psrDFA_t *to, const psrSymb_t *symb);
psrDFA_t*   PSR_GetTransTo(psrDFA_t *state, const psrSymb_t *symb);


typedef struct __parser_dfa_set_tag
{
		psrDFA_t	**set;
		size_t		count;
		size_t		cap;
}psrDFASet_t;

void	  PSR_InitDFASet(psrDFASet_t *set);
void	  PSR_UnInitDFASet(psrDFASet_t *set);
void	  PSR_InsertToDFASet(psrDFASet_t *set, psrDFA_t *dfa);
void	  PSR_CollectDFA(psrDFASet_t *empty_set, psrDFA_t *start);
psrDFA_t* PSR_FindDFAByItems(psrDFASet_t *set, const psrLRItemTbl_t *tbl);
int_t	  PSR_IndexOfDFASet(const psrDFASet_t *set, const psrDFA_t *dfa);



/*********************************************************************************************/

psrDFA_t*		PSR_Build_LR0_DFA(const psrGrammar_t *grammar);
psrDFA_t*		PSR_Build_LR1_DFA(const psrGrammar_t *grammar);
psrDFA_t*		PSR_Build_LALR_DFA(const psrGrammar_t *grammar);

















AR_NAMESPACE_END



#endif

