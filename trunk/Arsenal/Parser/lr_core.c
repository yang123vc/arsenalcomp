#include "lr.h"


/********************************************************LR0自动机*******************************************************/





static psrLRItem_t* __lr0_closure(const psrLRItem_t *itemset, const psrGrammar_t *grammar)
{
		psrLRItem_t *closure;
		const psrLRItem_t *item;
		bool_t changed;
		AR_ASSERT(grammar != NULL);
		
		if(itemset == NULL)return NULL;

		closure = PSR_CopyNewLRItemList(itemset);
		
		changed = True;
		while(changed)
		{
				changed = False;

		
				for(item = closure; item != NULL; item = item->next)
				{
						const psrSymb_t *head, *body;
						head = PSR_GetRuleHeadByRuleID(grammar, item->rule_id);
						body = PSR_GetRuleBodyByRuleID(grammar, item->rule_id);
						AR_ASSERT(head != NULL && body != NULL);
						
						body = PSR_IndexOfSymbList(body, item->delim);
						
						if(body == NULL)continue;/*规约状态*/
						
						if(!body->non_term)
						{
								if(PSR_IsNullable(body))
								{
										psrLRItem_t new_item;
										new_item = *item;
										new_item.delim = item->delim + 1;
										new_item.next = NULL;
										new_item.symb = NULL;

										if(PSR_InsertLRItem_Unique(&closure, &new_item))
										{
												changed = True;
										}
								}
						}else
						{
								size_t i;
								for(i = 0; i < grammar->count; ++i)
								{
										if(PSR_CompSymb(body, grammar->head[i]) == 0)
										{
												psrLRItem_t new_item;
												AR_MSET0(&new_item, sizeof(new_item));
												new_item.rule_id = grammar->head[i]->id;
												new_item.delim = 0;
												

												
												if(PSR_IsNullable(grammar->body[i]))
												{
														new_item.delim += 1;
														AR_ASSERT(grammar->body[i]->next == NULL || !PSR_IsNullable(grammar->body[i]->next));
												}

												if(PSR_InsertLRItem_Unique(&closure, &new_item))
												{
														changed = True;
												}
										}
								}
						}
				}
		}
		
		return closure;

}


static psrLRItem_t* __lr0_goto(const psrLRItem_t *itemset, const psrSymb_t *symb, const psrGrammar_t *grammar)
{
		psrLRItem_t *dest, *result;
		psrLRItem_t	 new_item;
		const psrLRItem_t *curr;
		
		dest = NULL;
		for(curr = itemset; curr != NULL; curr = curr->next)
		{
				const psrSymb_t *head, *next_symb;

				head = PSR_GetRuleHeadByRuleID(grammar, curr->rule_id);
				next_symb = PSR_GetRuleBodyByRuleID(grammar, curr->rule_id);
				
				AR_ASSERT(head != NULL && next_symb != NULL);

				next_symb = PSR_IndexOfSymbList(next_symb, curr->delim);

				if(next_symb == NULL)continue;

				if(PSR_CompSymb(next_symb, symb) == 0)
				{
						new_item = *curr;
						new_item.next = NULL;
						new_item.symb = NULL;
						new_item.delim += 1;
						PSR_InsertLRItem_Unique(&dest, &new_item);
				}
		}
		result = __lr0_closure(dest, grammar);
		PSR_DestroyLRItemList(dest);
		return result;
}

lrItemsCollection_t* LR0_CreateItemsCollection(const psrGrammar_t *grammar)
{
		lrItemsCollection_t* clt;
		psrLRItem_t first, *tmp;
		bool_t changed;

		clt = AR_NEW0(lrItemsCollection_t);
		
		AR_MSET0(&first, sizeof(first));
		first.rule_id = grammar->head[0]->id;
		first.delim = 0;
		
		tmp = __lr0_closure(&first, grammar);

		LR_InsertToItemsCollection_Unique(clt, tmp);
		
		changed = True;
		while(changed)
		{
				size_t i;
				changed = False;
				for(i = 0; i < clt->count; ++i)
				{
						const psrLRItem_t *items;
						const psrSymb_t	  *symbol;
						
						items = clt->items[i];
						
						for(symbol = grammar->symb_list; symbol != NULL; symbol = symbol->next)
						{
								psrLRItem_t *new_items; 
								
								new_items = __lr0_goto(items, symbol, grammar);
								
								if(new_items == NULL)continue;

								
								
								if(LR_InsertToItemsCollection_Unique(clt, new_items))
								{
										size_t new_state;
										changed = True;
										new_state = clt->count-1;
										LR_InsertToGotoMap(&clt->goto_map[i], symbol, new_state);
								}else
								{
										int idx;
										idx = LR_FindItemsFromCollection(clt, new_items);
										LR_InsertToGotoMap(&clt->goto_map[i], symbol, (size_t)idx);
								}
								
								PSR_DestroyLRItemList(new_items);
						}
				}
		}
		
		return clt;
}




/***********************************************LR(1)自动机****************************************************/
/*这个函数求follow_body的first集合，如果follow_body的每个符号都包含空，则将lookahead加入到first集合中，否则就返回follow_body的first集合*/
static psrSymb_t*  __lr1_calc_first(const psrSymb_t *follow_body, const psrSymb_t *lookahead, const psrGrammar_t *gmr)
{
		const psrSymb_t *curr;
		psrSymb_t		*first;
		bool_t has_epsilon;
		AR_ASSERT(lookahead != NULL && gmr != NULL);
		AR_ASSERT(!lookahead->non_term);
		
		first = NULL;
		
		has_epsilon = True;
		for(curr = follow_body; curr; curr = curr->next)
		{
				const psrSymb_t *fset = PSR_GetFirstSetBySymb(gmr, curr);
				has_epsilon = False;
				for(fset; fset; fset = fset->next)
				{
						if(PSR_CompSymb(fset, PSR_EpsilonSymb) == 0)
						{
								has_epsilon = True;
						}else
						{
								PSR_InsertSymb_Unique(&first, fset);
						}
				}
				
				if(!has_epsilon)
				{
						return first;
				}
		}
		
		if(has_epsilon)
		{
				PSR_InsertSymb_Unique(&first, lookahead);
		}
		return first;
}


static psrLRItem_t* __lr1_closure(const psrLRItem_t *itemset, const psrGrammar_t *grammar)
{
		psrLRItem_t *closure;
		const psrLRItem_t *item;
		bool_t changed;
		AR_ASSERT(grammar != NULL);
		
		if(itemset == NULL)return NULL;

		closure = PSR_CopyNewLRItemList(itemset);
		
		changed = True;
		while(changed)
		{
				changed = False;

		
				for(item = closure; item != NULL; item = item->next)
				{
						
						const psrSymb_t *head, *body;
						head = PSR_GetRuleHeadByRuleID(grammar, item->rule_id);
						body = PSR_GetRuleBodyByRuleID(grammar, item->rule_id);
						AR_ASSERT(head != NULL && body != NULL);
						
						body = PSR_IndexOfSymbList(body, item->delim);
						
						if(body == NULL)continue;/*规约状态*/

						AR_ASSERT(item->symb != NULL && !PSR_IsNullable(item->symb));

						if(!body->non_term)
						{
								/*
										如果当前项为(A->.%Epsilon，a)
										当前lookahead符号为什么，都对
										(A->%Epsilon., a)成立，因为这里是求闭包，所以务必要将
										空转移删除掉，因此delim直接+=1，而lookahead符号照常拷贝
								*/

								if(PSR_IsNullable(body))
								{
										psrLRItem_t new_item;
										new_item = *item;
										new_item.delim += 1;
										new_item.symb = PSR_CopyNewSymb(item->symb);
										/*
										    这里可能将new_item插到当前item的前面去，不过这无关紧要，因为每次成功的插入都会重新计算所有项
										*/
										if(PSR_InsertLRItem_Unique(&closure, &new_item))
										{
												changed = True;
										}
										PSR_DestroySymb(new_item.symb);
								}
						}else
						{
						
								size_t i;
								for(i = 0; i < grammar->count; ++i)
								{
										if(PSR_CompSymb(body, grammar->head[i]) == 0)
										{

												psrSymb_t *firstset;
												const psrSymb_t *curr_first;

												firstset = __lr1_calc_first(body->next, item->symb, grammar);

												AR_ASSERT(firstset != NULL);

												for(curr_first = firstset; curr_first; curr_first = curr_first->next)
												{

														psrLRItem_t new_item;
														AR_MSET0(&new_item, sizeof(new_item));
														new_item.rule_id = grammar->head[i]->id;
														new_item.delim = 0;
														new_item.symb = PSR_CopyNewSymb(curr_first);
														new_item.next = NULL;

														if(PSR_IsNullable(grammar->body[i]))
														{
																/*同上注释*/
																new_item.delim += 1;
																AR_ASSERT(grammar->body[i]->next == NULL || !PSR_IsNullable(grammar->body[i]->next));
														}

														if(PSR_InsertLRItem_Unique(&closure, &new_item))
														{
																changed = True;
														}
														PSR_DestroySymb(new_item.symb);
												}
												PSR_DestroySymbList(firstset);
										}
								}
						}
				}
		}

		return closure;

}


/*这个其实和lr0没啥子区别*/
static psrLRItem_t* __lr1_goto(const psrLRItem_t *itemset, const psrSymb_t *symb, const psrGrammar_t *grammar)
{
		
		psrLRItem_t *dest, *result;
		psrLRItem_t	 new_item;
		const psrLRItem_t *curr;
		
		dest = NULL;
		for(curr = itemset; curr != NULL; curr = curr->next)
		{
				const psrSymb_t *head, *next_symb;

				head = PSR_GetRuleHeadByRuleID(grammar, curr->rule_id);
				next_symb = PSR_GetRuleBodyByRuleID(grammar, curr->rule_id);
				
				AR_ASSERT(head != NULL && next_symb != NULL);

				next_symb = PSR_IndexOfSymbList(next_symb, curr->delim);

				if(next_symb == NULL)continue;

				if(PSR_CompSymb(next_symb, symb) == 0)
				{
						new_item = *curr;
						new_item.next = NULL;
						new_item.symb = PSR_CopyNewSymb(curr->symb);
						new_item.delim += 1;
						PSR_InsertLRItem_Unique(&dest, &new_item);
						PSR_DestroySymb(new_item.symb);
				}
		}
		result = __lr1_closure(dest, grammar);
		PSR_DestroyLRItemList(dest);
		return result;
}



lrItemsCollection_t* LR1_CreateItemsCollection(const psrGrammar_t *grammar)
{
		lrItemsCollection_t* clt;
		psrLRItem_t first, *tmp;
		bool_t changed;

		clt = AR_NEW0(lrItemsCollection_t);
		
		AR_MSET0(&first, sizeof(first));
		first.rule_id = grammar->head[0]->id;
		first.delim = 0;
		first.symb = (psrSymb_t*)PSR_EOISymb;
		
		tmp = __lr1_closure(&first, grammar);

		LR_InsertToItemsCollection_Unique(clt, tmp);
		
		changed = True;
		while(changed)
		{
				size_t i;
				changed = False;
				for(i = 0; i < clt->count; ++i)
				{
						const psrLRItem_t *items;
						const psrSymb_t	  *symbol;
						
						items = clt->items[i];
						
						for(symbol = grammar->symb_list; symbol != NULL; symbol = symbol->next)
						{
								psrLRItem_t *new_items; 
								new_items = __lr1_goto(items, symbol, grammar);
								if(new_items == NULL)continue;

								if(LR_InsertToItemsCollection_Unique(clt, new_items))
								{
										size_t new_state;
										changed = True;
										new_state = clt->count-1;
										LR_InsertToGotoMap(&clt->goto_map[i], symbol, new_state);
								}else
								{
										int idx;
										idx = LR_FindItemsFromCollection(clt, new_items);
										LR_InsertToGotoMap(&clt->goto_map[i], symbol, (size_t)idx);

								}
								PSR_DestroyLRItemList(new_items);
						}
				}
		}
		
		return clt;
}




/************************************************LALR自动机***************************************************************/
/*
bool_t __lalr_insert_symb_to_lalritem(psrLRItem_t *itemset, const psrLRItem_t *key, const psrSymb_t *symb)
{
		psrLRItem_t *curr;
		for(curr = itemset; curr; curr = curr->next)
		{
				if(curr->rule_id == key->rule_id && curr->delim == key->delim)
				{
						return PSR_InsertSymb_Unique(&curr->symb, symb);
				}
		}
		AR_ASSERT(0);
		return False;
}
*/
/*输入一个项集以及这个项集在input上的转移*/
static bool_t __insert_to_lalr_collection(size_t state, const psrSymb_t *input, lrItemsCollection_t *clt, const psrLRItem_t *key, const psrSymb_t *symb)
{
		int idx;
		psrLRItem_t *item;
		idx = LR_FindStateFromGotoMap(&clt->goto_map[state], input);
		AR_ASSERT(idx != -1);
		
		/*idx为项集state在input上的转移*/
		for(item = clt->items[idx]; item; item = item->next)
		{
				if(item->rule_id == key->rule_id && item->delim == key->delim)
				{
						/*如果某个item与key的LR0项相关信息相同，则将lookahead符号增加到此项中
						*/
						break;
				}
		}
		
		if(item != NULL)
		{
				return PSR_InsertSymb_Unique(&item->symb, symb);
		}else
		{
				AR_ASSERT(0);
				return False;
		}
}


/*
我感觉得简单的记录下这玩意儿的逻辑，首先，他的功能是计算从lr0项集state,在symb上的转移到底生成的哪些符号，以及如果新状态是否继承了state下的lookahead符号，
注意，因为lr1的闭包详细记录了每一个产生式(rule_id)所跟随的符号，lalr只是将他们汇集到一起（在这里，我产生式的意思是A->a|b;为两个产生式）；


这个算法的细节是，用一个不再终极符中的符号PSR_LALRSymb,求LR1项{rule_id, delim, PSR_LALRSymb}的LR1闭包，如果LALRSymb在输出的闭包项，且此闭包的delim + 1为state在symb上的LR0转移
则将发出项的lalr lookahead符号列表copy到转移后的项的lookahead列表里，否则，证明在这个项的这个转移上存在自发生成的符号，则将此符号插入到转移后的项的lookahead列表内

*/

static bool_t calc_symbol_transfer(size_t state, lrItemsCollection_t *clt, const psrSymb_t *symb,  const psrGrammar_t *grammar)
{
		psrLRItem_t *item;
		bool_t res;
		res = False;
		/*遍历state中所有项*/
		for(item = clt->items[state]; item; item = item->next)
		{
				psrLRItem_t tmp;
				psrLRItem_t *closure;
				const psrLRItem_t *curr;
				tmp = *item;
				tmp.next = NULL; 
				tmp.symb = (psrSymb_t*)PSR_LALRSymb;
				closure = __lr1_closure(&tmp, grammar);/*对每个项求lr1闭包*/

				for(curr = closure; curr; curr = curr->next)
				{
						const psrSymb_t *body;
						body = PSR_GetRuleBodyByRuleID(grammar,curr->rule_id);
						body = PSR_IndexOfSymbList(body, curr->delim);

						if(body == NULL)continue;/*无转移了*/


						if(PSR_CompSymb(body, symb) == 0)/*如果得到的闭包中的某个项存在在symb上的转移，则测试其有无自发生成的符号*/
						{
								
								psrLRItem_t new_item;
								new_item = *curr;
								new_item.delim += 1;
								new_item.next = NULL;
								new_item.symb = NULL;
								
								if(PSR_CompSymb(curr->symb, PSR_LALRSymb) != 0)/*如果是自发的终结符，则插入到对应的项集的lookahead列表中*/
								{
										if(__insert_to_lalr_collection(state, symb, clt, &new_item, curr->symb))
										{
												res = True;
										}
								}else/*否则，要将所有起始项（item)中的lookahead列表中的符号copy到从state在symb的转移中的项集的相应项中*/
								{
										const psrSymb_t *symb_tmp;
										
										for(symb_tmp = item->symb; symb_tmp; symb_tmp = symb_tmp->next)
										{
												if(__insert_to_lalr_collection(state, symb, clt, &new_item, symb_tmp))
												{
														res = True;
												}
										}
								}
						}
				}
				PSR_DestroyLRItemList(closure);
		}
		return res;
}





/*重复过程calc_symbol_transfer， 直到没有新项符号加入*/
lrItemsCollection_t* LALR_CreateItemsCollection(const psrGrammar_t *grammar)
{
		lrItemsCollection_t *result;
		size_t state;
		bool_t changed;
		psrLRItem_t *item;
		
		result = LR0_CreateItemsCollection(grammar);
		
		LR_ClearNonKernelItems(result);
		
		
		
		
		for(item = result->items[0]; item; item = item->next)
		{
				if(item->rule_id == 0 && item->next == 0)/* %Start->xxx中的lookahead(End of input)必须提前加入进去*/ 
				{
						item->symb = PSR_CopyNewSymb(PSR_EOISymb);
						break;
				}
		}

		

		changed = True;

		while(changed)/*不断的计算并加入自发生成的符号，直到没有可加入的*/
		{
				changed = False;
#if(0)
				for(state = 0; state < result->count; ++state)
				{
						const psrSymb_t *symb;
						for(symb = grammar->symb_list; symb; symb = symb->next)
						{
								if(calc_symbol_transfer(state, result, symb, grammar))
								{
										changed = True;
								}
						}

				}
#endif
				/*上面那个版本每个循环中都得针对所有符号计算一次lr1闭包，好像他妈的挺浪费的，这玩意儿得以后得好好优化*/
				for(state = 0; state < result->count; ++state)
				{		
						size_t i;
						for(i = 0; i < result->goto_map[state].count; i++)
						{
								if(calc_symbol_transfer(state, result, &result->goto_map[state].symb[i], grammar))
								{
										changed = True;
								}
						}
				}
		}


		
		

		return result;
}

