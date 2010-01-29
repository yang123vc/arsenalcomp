
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


#include "lalr.h"

AR_NAMESPACE_BEGIN


lalrState_t*	PSR_CreateState()
{
		lalrState_t		*state;
		state = AR_NEW0(lalrState_t);
		return state;
}

void			PSR_DestroyState(lalrState_t *state)
{
		AR_ASSERT(state != NULL);

		if(state->actions != NULL)
		{
				AR_DEL(state->actions);
		}

		if(state->basis != NULL)
		{
				PSR_DestroyConfigList(state->basis, false);
		}

		if(state->all_config != NULL)
		{

				PSR_DestroyConfigList(state->all_config, true);
		}

		AR_DEL(state);
}


static lalrAction_t* __insert_action(lalrState_t *state, void *data, const psrSymb_t *symb, lalrActionType_t act)
{
		AR_ASSERT(state != NULL && data != NULL && symb != NULL);

		if(state->count == state->cap)
		{
				state->cap = (state->cap + 4)*2;
				state->actions = AR_REALLOC(lalrAction_t, state->actions, state->cap);
		}
		state->actions[state->count].act_type = act;
		state->actions[state->count].symb = symb;
		state->actions[state->count].to = (lalrState_t*)data;
		state->count++;
		return &state->actions[state->count-1];
}

lalrAction_t*	PSR_InsertShiftAction(lalrState_t *state, lalrState_t *to, const psrSymb_t *symb)
{
		AR_ASSERT(state != NULL && to != NULL && symb != NULL);
		return __insert_action(state, (void*)to, symb, LALR_ACT_SHIFT);
		
}

lalrAction_t*	PSR_InsertReduceAction(lalrState_t *state, const psrRule_t *rule, const psrSymb_t *symb)
{
		AR_ASSERT(state != NULL && rule != NULL && symb != NULL);
		return __insert_action(state, (void*)rule, symb, LALR_ACT_REDUCE);
}

void			PSR_DestroyState_ALL(lalrState_t *state)
{
		lalrStateSet_t	set;
		size_t i;
		AR_ASSERT(state != NULL);

		PSR_InitStateSet(&set);
		PSR_CollectState(&set, state);

		for(i = 0; i < set.count; ++i)
		{
				PSR_DestroyState(set.set[i]);
		}
		
		PSR_UnInitStateSet(&set);
}



void			PSR_InitStateSet(lalrStateSet_t *set)
{
		AR_ASSERT(set != NULL);
		AR_memset(set, 0, sizeof(*set));
}

void			PSR_UnInitStateSet(lalrStateSet_t *set)
{
		AR_ASSERT(set != NULL);

		if(set->set != NULL)
		{
				AR_DEL(set->set);
		}
		AR_memset(set, 0, sizeof(*set));
}

void			PSR_InsertToStateSet(lalrStateSet_t *set, lalrState_t *state)
{
		AR_ASSERT(set != NULL && state != NULL);
		
		if(set->count == set->cap)
		{
				set->cap = (set->cap + 4) * 2;

				set->set = AR_REALLOC(lalrState_t*, set->set, set->cap);
		}
		set->set[set->count++] = state;
}

int_t			PSR_IndexOfStateSet(const lalrStateSet_t *set, const lalrState_t *state)
{
		int_t i;
		AR_ASSERT(set != NULL && state != NULL);

		for(i = 0; i < (int_t)set->count; ++i)
		{
				if(set->set[i] == state)return i;
		}
		return -1;
}

lalrState_t*	PSR_FindStateByBasis(lalrStateSet_t *set, lalrConfigList_t *basis)
{
		size_t i;
		AR_ASSERT(set != NULL && basis != NULL);

		for(i = 0; i < set->count; ++i)
		{
				if(PSR_CompConfigList(set->set[i]->basis, basis) == 0)
				{
						return set->set[i];
				}
		}
		
		return NULL;
}

void			PSR_CollectState(lalrStateSet_t *set, lalrState_t *start)
{
		size_t i;
		AR_ASSERT(set != NULL && start != NULL);
		set->count = 0;

		PSR_InsertToStateSet(set, start);

		for(i = 0; i < set->count; ++i)
		{
				size_t k;
				lalrState_t *state;

				state = set->set[i];

				for(k = 0; k < state->count; ++k)
				{
						if(state->actions[k].act_type == LALR_ACT_SHIFT)
						{
								AR_ASSERT(state->actions[k].to != NULL);
								if(PSR_IndexOfStateSet(set, state->actions[k].to) == -1)
								{
										PSR_InsertToStateSet(set, state->actions[k].to);
								}
						}
				}
		}
}



lalrState_t*   PSR_GetTransTo(lalrState_t *state, const psrSymb_t *symb)
{
		size_t i;

		AR_ASSERT(state != NULL && symb != NULL);

		for(i = 0; i < state->count; ++i)
		{
				if(state->actions[i].act_type == LALR_ACT_REDUCE)continue;

				if(PSR_CompSymb(state->actions[i].symb, symb) == 0)return state->actions[i].to;
		}
		return NULL;
}



AR_NAMESPACE_END

