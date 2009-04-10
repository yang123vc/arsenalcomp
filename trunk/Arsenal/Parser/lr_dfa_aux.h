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
#ifndef __LR_DFA_AUX_H__
#define __LR_DFA_AUX_H__

#include "symbol.h"
#include "grammar.h"

AR_NAMESPACE_BEGIN







typedef struct __follow_record_tag
{
		bool									has_epsilon;
		bool									is_init;
		psrSymbList_t							lst;
}psrFollowRec_t;

inline void PSR_InitFollowRec(psrFollowRec_t *rec)
{
		AR_memset(rec, 0, sizeof(*rec));
		PSR_InitSymbList(&rec->lst);
		rec->is_init = true;
}

inline void PSR_UnInitFollowRec(psrFollowRec_t *rec)
{
		PSR_UnInitSymbList(&rec->lst);
		rec->is_init = false;
}


typedef struct __first_table_tag
{
		psrFollowRec_t	*tbl;
		size_t			row;
		size_t			col;
}psrFollow_t;


inline bool  __calc_first(const psrSymbList_t *body, size_t start_idx, const psrSymbMap_t *first_set, psrSymbList_t *out)
{
		size_t i;
		bool has_epsilon;
		
		AR_ASSERT(body != NULL &&  first_set != NULL && out != NULL);
		PSR_ClearSymbList(out);
		
		has_epsilon = true;

		for(i = start_idx; i < body->count; ++i)
		{
				const psrSymb_t *curr;
				const psrSymbTbl_t *tbl;
				psrSymbTblIter_t iter;
				curr = body->lst[i];
				
				tbl = &(PSR_GetSymbolFromSymbMap(first_set, curr)->tbl);
				
				if(tbl->item_count == 0)continue;
				
				has_epsilon = false;
				for(iter = PSR_FirstFromSymbTbl(tbl); iter.curr; PSR_NextFromSymbTbl(tbl, &iter))
				{
						if(PSR_CompSymb(iter.curr->symb, PSR_EpsilonSymb) == 0)
						{
								has_epsilon = true;
						}else
						{
								if(PSR_FindFromSymbList(out, iter.curr->symb) == -1)
								{
										PSR_InsertToSymbList(out, iter.curr->symb);
								}
						}
				}
				
				if(!has_epsilon)
				{
						return false;
				}
		}
		return true;
}


inline void PSR_InitFollowTbl(psrFollow_t *tbl, const psrGrammar_t *grammar)
{
		psrSymbMap_t	first_set;
		size_t i,j;
		AR_ASSERT(grammar != NULL && tbl != NULL && grammar->count > 1);

		
		PSR_InitSymbMap(&first_set);
		PSR_CalcFirstSet(grammar, &first_set);

		
		AR_memset(tbl, 0, sizeof(*tbl));
		
		tbl->row = grammar->count;
		tbl->col = 0;

		for(i = 0; i < grammar->count; ++i)
		{
				if(AR_CMP(tbl->col, grammar->rules[i].body.count) < 0)
				{
						tbl->col = grammar->rules[i].body.count;
				}
		}
		tbl->col++;
		tbl->tbl = AR_NEWARR0(psrFollowRec_t, tbl->row * tbl->col * sizeof(psrFollowRec_t));
		
		for(i = 0; i < tbl->row; ++i)
		{
				const psrRule_t *rule = &grammar->rules[i];
				
				for(j = 0; j <= rule->body.count; ++j)
				{
						psrFollowRec_t *first_rec = &tbl->tbl[AR_TBL_IDX_R(i, j, tbl->col)];
						PSR_InitFollowRec(first_rec);
						/*计算的都是符号body[j]后面跟随的符号的*/
						first_rec->has_epsilon = __calc_first(&rule->body, j + 1, &first_set, &first_rec->lst);
				}
		}

		PSR_UnInitSymbMap(&first_set);
}

inline void PSR_UnInitFollowTbl(psrFollow_t *tbl)
{
		size_t i,j;
		for(i = 0; i < tbl->row; ++i)
		{
				for(j = 0; j < tbl->col; ++j)
				{
						psrFollowRec_t *first_rec = &tbl->tbl[AR_TBL_IDX_R(0, i, tbl->col)];
						if(first_rec->is_init)PSR_UnInitFollowRec(first_rec);
				}
		}

		AR_DEL(tbl->tbl);
}


inline  const psrFollowRec_t* PSR_GetFollowRecord(const psrFollow_t *tbl, size_t rule_id, size_t symb_idx)
{
		const psrFollowRec_t *rec;
		rec = &tbl->tbl[AR_TBL_IDX_R(rule_id, symb_idx, tbl->col)];
		AR_ASSERT(rec->is_init);
		return rec;
}

/******************************************************************************************************************************/



typedef struct __lr1_closure_record_tag
{
		psrLRItem_t				key;
		psrLRItemTbl_t			value;

		struct __lr1_closure_record_tag	*next;
}psrLR1ClosureRec_t;

#define LR1_CLOSURE_BUCKET_SIZE			10240

typedef struct lr1_closure_table_tag
{
		psrLR1ClosureRec_t		*bucket[LR1_CLOSURE_BUCKET_SIZE];
		
}psrLR1ClosureTbl_t;

inline void PSR_InitLR1ClosureTbl(psrLR1ClosureTbl_t *tbl)
{
		AR_memset(tbl, 0, sizeof(*tbl));
}

inline void PSR_UnInitLR1ClosureTbl(psrLR1ClosureTbl_t	*tbl)
{
		size_t i;
		AR_ASSERT(tbl != NULL);

		for(i = 0; i < LR1_CLOSURE_BUCKET_SIZE; ++i)
		{
				psrLR1ClosureRec_t *rec, *tmp;
				rec = tbl->bucket[i];
				while(rec)
				{
						tmp = rec->next;
						PSR_UnInitLRItem(&rec->key);
						PSR_UnInitLRItemTbl(&rec->value);
						AR_DEL(rec);
						rec = tmp;
				}
		}

		AR_memset(tbl, 0, sizeof(*tbl));
}

extern void PSR_Calc_LR1_Closure(psrLRItemTbl_t *closure, const psrGrammar_t *grammar, const psrFollow_t *tbl);

inline const psrLRItemTbl_t* PSR_GetLR1Closure(psrLR1ClosureTbl_t	*tbl, const psrLRItem_t *item, const psrGrammar_t *grammar, const psrFollow_t *follow_tbl)
{
		psrLR1ClosureRec_t		*rec;
		size_t	idx;
		AR_ASSERT(tbl != NULL && item != NULL && item->symb != NULL && grammar != NULL);

		idx = item->hash_code % LR1_CLOSURE_BUCKET_SIZE;
		
		if(tbl->bucket[idx])
		{
				psrLR1ClosureRec_t		*tmp;
				tmp = tbl->bucket[idx];
				
				while(tmp)
				{
						if(PSR_CompLRItem(&tmp->key, item) == 0)
						{
								return &tmp->value;
						}
				}
		}

		rec = AR_NEW0(psrLR1ClosureRec_t);
		PSR_InitLRItem(&rec->key, item->rule_id, item->delim, item->symb);
		PSR_InitLRItemTbl(&rec->value);
		PSR_InsertToLRItemTbl(&rec->value, &rec->key);
		PSR_Calc_LR1_Closure(&rec->value, grammar, follow_tbl);

		rec->next = tbl->bucket[idx];
		tbl->bucket[idx] = rec;

		return &rec->value;
}





AR_NAMESPACE_END


#endif

