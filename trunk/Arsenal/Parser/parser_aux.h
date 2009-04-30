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

#ifndef __PARSER_AUX_H__
#define __PARSER_AUX_H__


#include "parser.h"

AR_NAMESPACE_BEGIN


/***************************************************辅助数据结构**********************************************/


typedef struct __parser_node_stack
{
		psrNode_t		**nodes;
		size_t			count;
		size_t			cap;
}psrNodeStack_t;

inline void PSR_InitNodeStack(psrNodeStack_t *stack)
{
		AR_memset(stack, 0, sizeof(*stack));
}

inline void PSR_UnInitNodeStack(psrNodeStack_t *stack)
{
		AR_DEL(stack->nodes);
		AR_memset(stack, 0, sizeof(*stack));
}

inline void PSR_PushNodeStack(psrNodeStack_t *stack, psrNode_t *node)
{
		AR_ASSERT(stack != NULL);
		if(stack->count == stack->cap)
		{
				stack->cap = (stack->cap + 1)*2;
				stack->nodes = AR_REALLOC(psrNode_t*, stack->nodes, stack->cap);
		}
		stack->nodes[stack->count++] = node;
}

inline void PSR_PopNodeStack(psrNodeStack_t *stack, size_t n)
{
		AR_ASSERT(stack != NULL && n <= stack->count);
		stack->count -= n;
}

inline void PSR_ClearNodeStack(psrNodeStack_t *stack)
{
		AR_ASSERT(stack != NULL);
		stack->count = 0;
}

inline psrNode_t* PSR_TopNodeStack(psrNodeStack_t *stack)
{
		AR_ASSERT(stack != NULL && stack->count > 0);
		return stack->nodes[stack->count-1];
}






typedef struct __parser_stack_tag
{
		size_t			 *states;
		size_t			 count;
		size_t			 cap;
}psrStack_t;


inline void PSR_PushStack(psrStack_t *stack, size_t state)
{
		if(stack->count == stack->cap)
		{
				stack->cap = (stack->cap + 1)*2;
				stack->states = AR_REALLOC(size_t, stack->states, stack->cap);
		}
		stack->states[stack->count++] = state;
}


inline void PSR_PopStack(psrStack_t *stack, size_t n)
{
		AR_ASSERT(n <= stack->count);
		stack->count -= n;
}

inline size_t PSR_TopStack(const psrStack_t *stack)
{
		AR_ASSERT(stack->count > 0);
		return stack->states[stack->count-1];
}


inline void PSR_InitStack(psrStack_t *stack)
{
		AR_memset(stack, 0,sizeof(*stack));
}

inline void PSR_UnInitStack(psrStack_t *stack)
{
		AR_DEL(stack->states);
		AR_memset(stack, 0,sizeof(*stack));
}


inline void PSR_ClearStack(psrStack_t *stack)
{
		AR_ASSERT(stack != NULL);
		stack->count = 0;
}





typedef struct __term_info_rec_tag
{
		const psrTermInfo_t	**terms;
		size_t	count;
		size_t	cap;
}psrTermInfoRec_t;

inline psrTermInfoRec_t* PSR_CreateTermInfoRec()
{
		return AR_NEW0(psrTermInfoRec_t);
}

inline void			PSR_DestroyTermInfoRec(psrTermInfoRec_t *rec)
{
		if(rec)
		{
				AR_DEL((psrTermInfo_t**)rec->terms);
				AR_DEL(rec);
		}
}

inline const psrTermInfo_t* PSR_FindTermFromRec(const psrTermInfoRec_t *rec, size_t val)
{
		size_t i;
		AR_ASSERT(rec != NULL);

		for(i = 0; i < rec->count; ++i)
		{
				if(rec->terms[i]->val == val)return rec->terms[i];
		}

		return NULL;
}

inline bool			PSR_InsertToTermInfoRec(psrTermInfoRec_t *rec, const psrTermInfo_t *term)
{
		AR_ASSERT(rec != NULL && term != NULL);
		if(PSR_FindTermFromRec(rec, term->val) != NULL)return false;

		if(rec->count == rec->cap)
		{
				rec->cap = (rec->cap + 4)*2;
				rec->terms = AR_REALLOC(const psrTermInfo_t*, rec->terms, rec->cap);
		}
		rec->terms[rec->count++] = term;
		return true;
}



#define TERM_BUCKET_SIZE		20480


typedef struct __term_table_tag
{
		psrTermInfoRec_t		*bucket[TERM_BUCKET_SIZE];
		size_t					item_count;
}psrTermInfoTbl_t;

inline psrTermInfoTbl_t* PSR_CreateTermInfoTable()
{
		return AR_NEW0(psrTermInfoTbl_t);
}

inline void PSR_DestroyTermInfoTable(psrTermInfoTbl_t *tbl)
{
		size_t i;
		
		for(i = 0; i < TERM_BUCKET_SIZE; ++i)
		{
				PSR_DestroyTermInfoRec(tbl->bucket[i]);
		}
		AR_DEL(tbl);
}

inline bool PSR_InsertToTermInfoTable(psrTermInfoTbl_t *tbl, const psrTermInfo_t *term)
{
		size_t idx;
		psrTermInfoRec_t		*rec;
		AR_ASSERT(tbl != NULL && term != NULL);

		idx = term->val % TERM_BUCKET_SIZE;

		if(tbl->bucket[idx] == NULL)
		{
				tbl->bucket[idx] = PSR_CreateTermInfoRec();
		}

		rec = tbl->bucket[idx];

		if(PSR_InsertToTermInfoRec(rec, term))
		{
				tbl->item_count++;
				return true;
		}else
		{
				return false;
		}
}

inline const psrTermInfo_t* PSR_FindTermFromInfoTable(const psrTermInfoTbl_t *tbl, size_t tokval)
{
		size_t idx;
		psrTermInfoRec_t		*rec;
		AR_ASSERT(tbl != NULL);
		idx = tokval % TERM_BUCKET_SIZE;
		
		rec = tbl->bucket[idx];

		if(rec == NULL)return NULL;
		return PSR_FindTermFromRec(rec, tokval);
}



typedef struct __expected_message_tag
{
		const wchar_t	**msg;
		size_t			count;
}psrExpectedMsg_t;




inline void PSR_InitExpectedMsg(psrExpectedMsg_t *msg, const psrSymbList_t *lst)
{
		size_t i;
		AR_ASSERT(msg != NULL && lst != NULL);

		AR_memset(msg, 0, sizeof(*msg));
		if(lst->count == 0)return;
		
		msg->count = lst->count;
		
		msg->msg = AR_NEWARR(const wchar_t*, msg->count);
		
		for(i = 0; i < msg->count; ++i)
		{
				AR_ASSERT(lst->lst[i]->type == PSR_TERM);
				msg->msg[i] = PSR_AllocString(lst->lst[i]->name);
		}
}

inline void PSR_UnInitExpectedMsg(psrExpectedMsg_t *msg)
{
		AR_ASSERT(msg != NULL);
		if(msg->msg)AR_DEL(msg->msg);
}























AR_NAMESPACE_END


#endif

