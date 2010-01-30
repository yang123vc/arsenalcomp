
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


#include "parser_in.h"
#include "lalr.h"

AR_NAMESPACE_BEGIN


static	void	__build_goto(lalrState_t *start, const psrGrammar_t *grammar, lalrStateSet_t *set, const psrSymbMap_t *first_set, bool_t lr0);
static	void	__calc_lr0_closure(lalrConfigList_t *all_config, const psrGrammar_t *grammar, const psrSymbMap_t *first_set, bool_t lr0);
static	lalrState_t* __build_state(lalrConfigList_t *basis, const psrGrammar_t *grammar, lalrStateSet_t *set, const psrSymbMap_t *first_set, bool_t lr0);

static	void	__build_goto(lalrState_t *start, const psrGrammar_t *grammar, lalrStateSet_t *set, const psrSymbMap_t *first_set, bool_t lr0)
{
		lalrConfigNode_t		*node;
		AR_ASSERT(start != NULL && grammar != NULL && set != NULL && first_set != NULL);
		AR_ASSERT(start->basis != NULL && start->all_config != NULL);

		for(node = start->all_config->head; node != NULL; node = node->next)
		{
				node->config->is_completed = false;
		}


		for(node = start->all_config->head; node != NULL; node = node->next)
		{
				lalrState_t				*new_state;
				lalrConfigList_t		*goto_list;
				const psrSymb_t			*symb;
				const psrRule_t			*rule;
				lalrConfigNode_t		*inner_node;

				rule = node->config->rule;

				if(node->config->is_completed)continue;
				if(node->config->delim >= rule->body.count)continue;

				symb = rule->body.lst[node->config->delim];

				new_state = NULL;
				goto_list = PSR_CreateConfigList();

				for(inner_node = start->all_config->head; inner_node != NULL; inner_node = inner_node->next)
				{
						const psrSymb_t			*bsp;
						lalrConfig_t			*new_config;

						if(inner_node->config->is_completed)continue;
						if(inner_node->config->delim >= inner_node->config->rule->body.count)continue;

						bsp = inner_node->config->rule->body.lst[inner_node->config->delim];
						
						if(PSR_CompSymb(symb, bsp) != 0)continue;
						inner_node->config->is_completed = true;

						new_config = PSR_FindFromConfigList(goto_list, inner_node->config->rule, inner_node->config->delim + 1);

						if(new_config == NULL)
						{
								new_config = PSR_InsertToConfigListByValue(goto_list, inner_node->config->rule, inner_node->config->delim + 1);
						}
						
						if(!lr0)
						{
								PSR_InsertToConfigList(new_config->backward, inner_node->config);
						}
				}

				new_state = __build_state(goto_list, grammar, set, first_set, lr0);
				
				AR_ASSERT(new_state != NULL);
				
				PSR_InsertAction(start, new_state, symb, node->config);
		}
}


static	void	__calc_lr0_closure(lalrConfigList_t *all_config, const psrGrammar_t *grammar, const psrSymbMap_t *first_set, bool_t lr0)
{

		lalrConfigNode_t		*node;
		AR_ASSERT(all_config != NULL && grammar != NULL && first_set != NULL);

		AR_ASSERT(all_config->count > 0);
		for(node = all_config->head; node != NULL; node = node->next)
		{
				const psrRule_t *rule;
				size_t delim;
				const psrSymb_t *symb;

				rule = node->config->rule;
				delim = node->config->delim;

				if(delim >= rule->body.count)continue;

				symb = rule->body.lst[delim];

				if(symb->type == PSR_NONTERM)
				{
						size_t i;

						for(i = 0; i < grammar->count; ++i)
						{
								const psrRule_t *inner_rule;
								inner_rule = grammar->rules[i];

								if(PSR_CompSymb(symb, inner_rule->head) == 0)
								{
										size_t k;
										lalrConfig_t *new_config;

										new_config = PSR_FindFromConfigList(all_config, inner_rule, 0);
										if(new_config == NULL)
										{
												new_config = PSR_InsertToConfigListByValue(all_config, inner_rule, 0);
										}
										AR_ASSERT(new_config != NULL);

										/*lr0模式，不计算传播链以及follow set*/
										if(lr0)continue;

										for(k = delim + 1; k < rule->body.count; ++k)
										{
												const psrSymb_t *sp = rule->body.lst[k];
												AR_ASSERT(sp != NULL);

												if(sp->type == PSR_TERM)
												{
														PSR_InsertToSymbList_Unique(&new_config->follow_set, sp);
														break;
												}else
												{
														const psrMapRec_t *rec;
														size_t x;
														rec = PSR_GetSymbolFromSymbMap(first_set, sp);

														for(x = 0; x < rec->lst.count; ++x)
														{
																PSR_InsertToSymbList_Unique(&new_config->follow_set, rec->lst.lst[x]);
														}

														if(!rec->can_empty)
														{
																break;
														}
												}
										}

										if(k == rule->body.count)
										{
												PSR_InsertToConfigList(node->config->forward, new_config);
										}
								}
						}
				}

		}

}

static	lalrState_t* __build_state(lalrConfigList_t *basis, const psrGrammar_t *grammar, lalrStateSet_t *set, const psrSymbMap_t *first_set, bool_t lr0)
{
		lalrState_t		*new_state;
		AR_ASSERT(basis != NULL && basis->count > 0 && grammar != NULL && set != NULL && first_set != NULL);
		
		PSR_SortConfigList(basis);
		new_state = PSR_FindStateByBasis(set, basis);

		if(new_state)
		{
				if(!lr0)
				{
						lalrConfigNode_t *l, *r;

						for(l = basis->head, r = new_state->basis->head; l != NULL && r != NULL; l = l->next, r = r->next)
						{
								PSR_UnionConfigList(r->config->backward, l->config->backward);
						}
				}
				
				PSR_DestroyConfigList(basis, true);
		}else
		{
				lalrConfigList_t *all_config;

				all_config = PSR_CreateConfigList();
				PSR_CopyConfigList(all_config, basis);

				__calc_lr0_closure(all_config, grammar, first_set, lr0);
				PSR_SortConfigList(all_config);
				
				new_state = PSR_CreateState();
				new_state->basis = basis;
				new_state->all_config = all_config;
				PSR_InsertToStateSet(set, new_state);
				
				__build_goto(new_state, grammar, set, first_set, lr0);
		}

		return new_state;
}


static void __build_slr_actions(lalrStateSet_t *set, const psrSymbMap_t *follow_set)
{
		size_t i;
		
		
		AR_ASSERT(set != NULL && set->count > 0 && follow_set != NULL);

		
		for(i = 0; i < set->count; ++i)
		{
				lalrConfigNode_t *node;
				lalrState_t *state = set->set[i];

				for(node = state->all_config->head; node != NULL; node = node->next)
				{
						if(node->config->delim == node->config->rule->body.count)
						{
								size_t x;
								const psrSymbList_t *lst = &(PSR_GetSymbolFromSymbMap(follow_set, node->config->rule->head)->lst);

								for(x = 0; x < lst->count; ++x)
								{
										
										lalrAction_t *action = PSR_InsertAction(state, NULL, lst->lst[x], node->config);

										if(PSR_CompSymb(node->config->rule->head, PSR_StartSymb) == 0)
										{
												action->act_type = LALR_ACT_ACCEPT;
										}
								}
						}
				}
		}

		
}



lalrState_t*	PSR_Create_LR0_State(const psrGrammar_t *grammar)
{
		psrSymbMap_t			first_set, follow_set;
		lalrState_t				*start;
		lalrConfigList_t		*basis;
		lalrConfig_t			*first_cfg;
		lalrStateSet_t			set;
		AR_ASSERT(grammar != NULL);
		
		
		PSR_InitSymbMap(&first_set);
		PSR_InitSymbMap(&follow_set);
		PSR_CalcFirstSet(grammar, &first_set);
		PSR_CalcFollowSet(grammar, &follow_set, &first_set);

		PSR_InitStateSet(&set);

		basis = PSR_CreateConfigList();

		first_cfg = PSR_InsertToConfigListByValue(basis, PSR_GetStartRule(grammar), 0);
		
		PSR_InsertToSymbList(&first_cfg->follow_set, PSR_EOISymb);

		start = __build_state(basis, grammar, &set, &first_set, true);

		__build_slr_actions(&set, &follow_set);

		PSR_UnInitSymbMap(&follow_set);
		PSR_UnInitSymbMap(&first_set);
		PSR_UnInitStateSet(&set);

		return start;
}







static void __build_propagation_links(lalrStateSet_t *set)
{
		size_t i;
		bool_t changed;
		
		AR_ASSERT(set != NULL);
		
		for(i = 0; i < set->count; ++i)
		{
				lalrConfigNode_t *node;
				lalrState_t *state = set->set[i];

				for(node = state->all_config->head; node != NULL; node = node->next)
				{
						lalrConfigNode_t		*bp;
						lalrConfig_t			*config;
						config = node->config;
						for(bp = node->config->backward->head; bp != NULL; bp = bp->next)
						{
								lalrConfig_t			*other = bp->config;
								PSR_InsertToConfigList(other->forward, config);
						}
				}
		}


		for(i = 0; i < set->count; ++i)
		{
				lalrConfigNode_t *node;
				lalrState_t *state = set->set[i];

				for(node = state->all_config->head; node != NULL; node = node->next)
				{
						node->config->is_completed = false;
				}
		}
		

		do{
				changed = false;

				for(i = 0; i < set->count; ++i)
				{
						lalrConfigNode_t *node;
						lalrState_t *state = set->set[i];

						for(node = state->all_config->head; node != NULL; node = node->next)
						{
								lalrConfigNode_t *fp;
								
								if(node->config->is_completed)continue;

								for(fp = node->config->forward->head; fp != NULL; fp = fp->next)
								{
										size_t x;
										lalrConfig_t *next_config = fp->config;
										
										for(x = 0; x < node->config->follow_set.count; ++x)
										{
												if(PSR_InsertToSymbList_Unique(&next_config->follow_set, node->config->follow_set.lst[x]))
												{
														changed = true;
														next_config->is_completed = false;
												}
										}
								}

								node->config->is_completed = true;
						}
				}

		}while(changed);


}


static void __build_actions(lalrStateSet_t *set)
{
		size_t i;
		AR_ASSERT(set != NULL && set->count > 0);

		for(i = 0; i < set->count; ++i)
		{
				lalrConfigNode_t *node;
				lalrState_t *state = set->set[i];

				for(node = state->all_config->head; node != NULL; node = node->next)
				{
						if(node->config->delim == node->config->rule->body.count)
						{
								size_t x;
								for(x = 0; x < node->config->follow_set.count; ++x)
								{
										lalrAction_t *action = PSR_InsertAction(state, NULL, node->config->follow_set.lst[x], node->config);

										if(PSR_CompSymb(node->config->rule->head, PSR_StartSymb) == 0)
										{
												action->act_type = LALR_ACT_ACCEPT;
										}
								}
						}
				}
		}
}


lalrState_t*	PSR_Create_LALR_State(const psrGrammar_t *grammar)
{

		psrSymbMap_t			first_set;
		lalrState_t				*start;
		lalrConfigList_t		*basis;
		lalrConfig_t			*first_cfg;
		lalrStateSet_t			set;
		AR_ASSERT(grammar != NULL);
		
		
		PSR_InitSymbMap(&first_set);
		PSR_CalcFirstSet(grammar, &first_set);
		PSR_InitStateSet(&set);

		basis = PSR_CreateConfigList();

		first_cfg = PSR_InsertToConfigListByValue(basis, PSR_GetStartRule(grammar), 0);
		
		PSR_InsertToSymbList(&first_cfg->follow_set, PSR_EOISymb);

		start = __build_state(basis, grammar, &set, &first_set, false);

		__build_propagation_links(&set);
		__build_actions(&set);
		PSR_UnInitSymbMap(&first_set);
		PSR_UnInitStateSet(&set);

		return start;
}


AR_NAMESPACE_END
