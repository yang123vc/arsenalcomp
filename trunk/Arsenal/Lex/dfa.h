#ifndef __LEX_DFA_H__
#define __LEX_DFA_H__

#include "node.h"




typedef struct	__lex_dfa_vtx_tag lexDFAVertex_t;



typedef struct __lex_dfa_arc_tag
{
		size_t			match_id;
		lexDFAVertex_t	*from;
		lexDFAVertex_t	*to;
}lexDFAArc_t;




typedef struct __lex_dfa_vtx_tag
{
		lexDFAArc_t			*in;
		size_t				in_count;
		size_t				in_cap;

		lexDFAArc_t			*out;
		size_t				out_count;
		size_t				out_cap;
		
		lexAction_t			*action;
		size_t				act_count;

		bool_t				is_final;
		lexPositionSet_t	pos_set;
}lexDFAVertex_t;



lexDFAVertex_t*	LEX_CreateDFAVertex();
void			LEX_DestroyDFAVertex(lexDFAVertex_t* vtx);/*会先调用LEX_DisConnectVertex清除一切相关连的边，之后调用AR_DEL清理vtx内存*/

void			LEX_ConnectDFAVertex(lexDFAVertex_t* from, lexDFAVertex_t* to, size_t match_id);
void			LEX_DisConnectVertex(lexDFAVertex_t *vtx, lexDFAArc_t *arc);

lexDFAVertex_t* LEX_GetNextVertex(lexDFAVertex_t *vtx, size_t match_id);


typedef struct __lex_dfa_vtx_set_tag
{
		lexDFAVertex_t	**set;
		size_t			count;
		size_t			cap;
}lexDFAVertexSet_t;

void	LEX_InitDFAVertexSet(lexDFAVertexSet_t *set);
void	LEX_UnInitDFAVertexSet(lexDFAVertexSet_t *set);
void	LEX_InsertDFAVertexSet(lexDFAVertexSet_t *set, lexDFAVertex_t *vtx);
void	LEX_CollectDFAVertex(lexDFAVertexSet_t *empty_set, lexDFAVertex_t *start_vtx);

/*******************************************************************************************************************/






lexDFAVertex_t* LEX_BuildDFA(const lexCClass_t *cclass, const lexLeafSet_t *leafset);
void			LEX_DestroyDFA(lexDFAVertex_t  *dfa);


/************************************************State_Table***************************************/

typedef struct __lex_state_table_tag
{
		lexAction_t		*actions;
		bool_t			*is_final;
		size_t			count;
		

		int				*tbl;
		size_t			row;
		size_t			col;
}lexStateTable_t;

void LEX_InitStateTable(lexStateTable_t *tbl);
void LEX_UnInitStateTable(lexStateTable_t *tbl);

void LEX_BuildStateTable(lexStateTable_t *tbl, lexDFAVertex_t *dfa, const lexCClass_t *cclass);

AR_INLINE int LEX_GetNextState(const lexStateTable_t *tbl, int curr, size_t id)
{
		AR_ASSERT(id < tbl->col && (size_t)curr < tbl->row && curr >= 0);
		return tbl->tbl[((size_t)curr) * tbl->col + id];
}


#endif
