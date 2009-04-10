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

#include "grammar.h"
#include "parser_in.h"

AR_NAMESPACE_BEGIN




void					PSR_InitGrammar(psrGrammar_t *grammar)
{
		AR_memset(grammar, 0, sizeof(*grammar));
		PSR_InitSymbTbl(&grammar->term_tbl, 10);
		PSR_InitSymbList(&grammar->symb_list);

}

void					PSR_UnInitGrammar(psrGrammar_t *grammar)
{
		if(grammar != NULL)
		{
				PSR_ClearGrammar(grammar);
				PSR_UnInitSymbTbl(&grammar->term_tbl);
				PSR_UnInitSymbList(&grammar->symb_list);
				AR_memset(grammar, 0, sizeof(*grammar));
		}
}


static void		__destroy_rule(psrRule_t *rule)
{
		if(rule)
		{
				size_t i;
				PSR_DestroySymb(rule->head);
				for(i = 0; i < rule->body.count; ++i)
				{
						PSR_DestroySymb(rule->body.lst[i]);
				}
				PSR_UnInitSymbList(&rule->body);
		}
}

void					PSR_ClearGrammar(psrGrammar_t *grammar)
{
		size_t i;
		psrSymbTblIter_t iter;
		
		for(iter = PSR_FirstFromSymbTbl(&grammar->term_tbl); iter.curr; PSR_NextFromSymbTbl(&grammar->term_tbl, &iter))
		{
				PSR_DestroySymb(iter.curr->symb);
		}

		PSR_ClearSymbTbl(&grammar->term_tbl);
		PSR_ClearSymbList(&grammar->symb_list);

		for(i = 0; i < grammar->count; ++i)
		{
				__destroy_rule(&grammar->rules[i]);
		}
		grammar->count = 0;
}

/***************************************************************************************/

static void	__copy_rule(psrRule_t *dest, const psrRule_t *sour)
{
		size_t i;
		
		dest->head = PSR_CopyNewSymb(sour->head);
		if(sour->prec_tok)
		{
				dest->prec_tok = PSR_AllocString(sour->prec_tok);
		}else
		{
				dest->prec_tok = NULL;
		}
		dest->rule_id = sour->rule_id;
		PSR_InitSymbList(&dest->body);
		
		for(i = 0; i < sour->body.count; ++i)
		{
				PSR_InsertToSymbList(&dest->body, PSR_CopyNewSymb(sour->body.lst[i]));
		}
		
}

void PSR_CopyGrammar(psrGrammar_t *dest, const psrGrammar_t *grammar)
{
		psrSymbTblIter_t iter;
		size_t	i;
		AR_ASSERT(grammar != NULL && dest != NULL);
		
		PSR_ClearSymbTbl(&dest->term_tbl);

		for(iter = PSR_FirstFromSymbTbl(&grammar->term_tbl); iter.curr; PSR_NextFromSymbTbl(&grammar->term_tbl, &iter))
		{
				PSR_InsertToSymbTbl(&dest->term_tbl, PSR_CopyNewSymb(iter.curr->symb));
		}
		
		if(grammar->count > 0)
		{
				dest->count = dest->cap = grammar->count;
				dest->rules = AR_NEWARR0(psrRule_t, dest->count);

				for(i = 0; i < dest->count; ++i)
				{
						__copy_rule(&dest->rules[i], &grammar->rules[i]);
				}
		}
}

/***************************************************************************************/





bool					PSR_InsertTerm(psrGrammar_t *grammar, const psrSymb_t *token)
{
		AR_ASSERT(grammar != NULL && token != NULL && token->type == PSR_TERM);
		
		if(token->val < PSR_MIN_TOKENVAL)
		{
				AR_error(AR_GRAMMAR, L"Grammar Error : invalid token value %s : %d\r\n", token->name, token->val);
				return false;
		}

		if(grammar->term_tbl.item_count == 0)
		{
				PSR_InsertToSymbTbl(&grammar->term_tbl, PSR_CopyNewSymb(PSR_EpsilonSymb));
				PSR_InsertToSymbTbl(&grammar->term_tbl, PSR_CopyNewSymb(PSR_EOISymb));
				PSR_InsertToSymbTbl(&grammar->term_tbl, PSR_CopyNewSymb(PSR_ErrorSymb));
		}

		if(PSR_FindTermByName(grammar, token->name) != NULL || PSR_FindTermByValue(grammar, token->val) != NULL)return false;
		return PSR_InsertToSymbTbl(&grammar->term_tbl, PSR_CreateSymb(token->name, token->type, token->prec, token->assoc, token->val));
}


const psrSymb_t*		PSR_FindTermByName(psrGrammar_t	*grammar, const wchar_t *name)
{
		psrSymbTblIter_t iter;
		AR_ASSERT(grammar != NULL && name != NULL);

		for(iter = PSR_FirstFromSymbTbl(&grammar->term_tbl); iter.curr; PSR_NextFromSymbTbl(&grammar->term_tbl, &iter))
		{
				if(AR_wcscmp(iter.curr->symb->name, name) == 0)return iter.curr->symb;
		}
		return NULL;	
}


const psrSymb_t*		PSR_FindTermByValue(psrGrammar_t	*grammar, uint_t val)
{
		psrSymbTblIter_t iter;
		AR_ASSERT(grammar != NULL);

		for(iter = PSR_FirstFromSymbTbl(&grammar->term_tbl); iter.curr; PSR_NextFromSymbTbl(&grammar->term_tbl, &iter))
		{
				if(iter.curr->symb->val == val)return iter.curr->symb;
		}
		return NULL;
}


bool					PSR_SetTerm(psrGrammar_t *grammar, const wchar_t *name, psrAssocType_t assoc, size_t prec)
{
		const psrSymb_t *term;
		AR_ASSERT(grammar != NULL && name != NULL);
		term = PSR_FindTermByName(grammar, name);
		if(term == NULL)return false;

		((psrSymb_t*)term)->prec = prec;
		((psrSymb_t*)term)->assoc = assoc;
		return true;
}


const psrRule_t*		PSR_GetRuleByRuleID(const psrGrammar_t *grammar, size_t rule_id)
{
		AR_ASSERT(grammar != NULL && rule_id < grammar->count);
		return &grammar->rules[rule_id];
}





static void __insert_rule(psrGrammar_t *grammar, const psrSymb_t *head, const psrSymbList_t *body, const psrSymb_t *term)
{
		size_t i;
		if(grammar->count == grammar->cap)
		{
				
				grammar->cap = (grammar->cap + 4) * 2;
				grammar->rules = AR_REALLOC(psrRule_t, grammar->rules, grammar->cap);
				for(i = grammar->count; i < grammar->cap; ++i)
				{
						AR_memset(&grammar->rules[i], 0, sizeof(grammar->rules[i]));
				}
		}
		
		grammar->rules[grammar->count].head = head;
		grammar->rules[grammar->count].rule_id = grammar->count;
		PSR_InitSymbList(&grammar->rules[grammar->count].body);
		
		for(i = 0; i < body->count; ++i)
		{
				const psrSymb_t *new_symb = PSR_CreateSymb(body->lst[i]->name, body->lst[i]->type, body->lst[i]->prec, body->lst[i]->assoc, body->lst[i]->val);
				PSR_InsertToSymbList(&grammar->rules[grammar->count].body, new_symb);
		}

		if(term != NULL)
		{
				grammar->rules[grammar->count].prec_tok = PSR_AllocString(term->name);
		}else
		{
				grammar->rules[grammar->count].prec_tok = NULL;
		}
		grammar->count++;
}


bool					PSR_InsertRule(psrGrammar_t *grammar, const psrSymb_t *head, const psrSymbList_t *body, const psrSymb_t *term)
{
		bool is_ok;
		bool is_error;
		size_t right_prec;
		psrAssocType_t right_assoc;
		size_t i;
		const psrSymb_t *right_term;
		
		
		AR_ASSERT(grammar != NULL && head != NULL && body != NULL && body->count > 0 && head->type == PSR_NONTERM);

		if(PSR_FindTermByName(grammar, head->name) != NULL)
		{
				AR_error(AR_GRAMMAR, L"Grammar Error : invalid rule head \"%s\" is a term symbol\r\n", head->name);
				return false;
		}
		
		if(term != NULL && (PSR_FindTermByValue(grammar, term->val) == NULL || PSR_FindTermByName(grammar, term->name) == NULL))
		{
				AR_error(AR_GRAMMAR, L"Grammar Error : prec term %s not found\r\n", term->name);
				return false;
		}
		
		if(grammar->count == 0)
		{
				psrSymbList_t	lst;
				PSR_InitSymbList(&lst);
				PSR_InsertToSymbList(&lst, head);
				__insert_rule(grammar, PSR_CopyNewSymb(PSR_StartSymb), &lst, NULL);
				PSR_UnInitSymbList(&lst);
		}
		
		is_ok = true;
		
		right_prec = 0;
		right_assoc = PSR_ASSOC_NOASSOC;
		right_term = NULL;
		
		is_error = false;

		for(i = 0; i < body->count; ++i)
		{
				const psrSymb_t *curr;
				curr = body->lst[i];
				if(curr->type == PSR_TERM)
				{
						const psrSymb_t *tok, *tok2;
						tok = PSR_FindTermByName(grammar, curr->name);
						tok2 = PSR_FindTermByValue(grammar, curr->val);
						if(tok == NULL || tok2 == NULL || tok != tok2)
						{
								is_ok = false;
								break;
						}

						if(PSR_CompSymb(tok, PSR_EpsilonSymb) == 0)
						{
								if(i > 0 || body->count > 1)
								{
										is_ok = false;
										AR_error(AR_GRAMMAR, L"Grammar Error: Invalid epsilon token definition<%s>!\r\n", head->name);
										break;
								}
						}
						
						((psrSymb_t*)curr)->prec = tok->prec;
						((psrSymb_t*)curr)->val = tok->val;
						((psrSymb_t*)curr)->assoc = tok->assoc;

						right_prec = curr->prec;
						right_assoc = curr->assoc;
						right_term = curr;

						if(PSR_CompSymb(curr, PSR_ErrorSymb) == 0)
						{
							/*	if(i == body->count - 1)
								{
										AR_error(AR_GRAMMAR, L"Grammar Error: error  must be followed by term or non-term token : <%s>!\r\n", head->name);
										is_ok = false;
										break;
								}
							*/
								if(!is_error)
								{
										is_error = true;
								}else
								{
										AR_error(AR_GRAMMAR, L"Grammar Error: Duplicate error definition<%s>!\r\n", head->name);
										is_ok = false;
										break;
								}
						}

				}else if(curr->type == PSR_NONTERM)
				{
						/*if(is_error)
						{
								AR_error(AR_GRAMMAR, L"Grammar Error: Invalid error definition<%s>!\r\n", head->name);
								is_ok = false;
								break;
						}
						*/
						((psrSymb_t*)curr)->prec = 0;
						((psrSymb_t*)curr)->val = 0;
						((psrSymb_t*)curr)->assoc = PSR_ASSOC_NOASSOC;
				}else
				{
						AR_ASSERT(false);
				}
		}
		
		if(is_ok)
		{
				if(term == NULL)
				{
						((psrSymb_t*)head)->prec = right_prec;
						((psrSymb_t*)head)->assoc = right_assoc;
				}else
				{
						((psrSymb_t*)head)->assoc = term->assoc;
						((psrSymb_t*)head)->prec = term->prec;
				}

				if(term == NULL)term = right_term;
				__insert_rule(grammar, PSR_CreateSymb(head->name, head->type, head->prec, head->assoc, head->val), body, term);
		}else
		{
				if(grammar->count == 1)
				{
						PSR_ClearGrammar(grammar);
				}
		}
		return is_ok;
}



void					PSR_UpdateGrammar(psrGrammar_t *grammar)
{
		size_t i;
		AR_ASSERT(grammar != NULL);

		for(i = 0; i < grammar->count; ++i)
		{
				psrRule_t		*rule;

				rule = &grammar->rules[i];
				if(rule->prec_tok != NULL)
				{
						const psrSymb_t *prec = PSR_FindTermByName(grammar, rule->prec_tok);
						AR_ASSERT(prec != NULL);

						((psrSymb_t*)rule->head)->prec = prec->prec;
						((psrSymb_t*)rule->head)->assoc = prec->assoc;
				}
		}

		PSR_ClearSymbList(&grammar->symb_list);
}


const psrSymbList_t* PSR_GetSymbList(const psrGrammar_t *grammar)
{
		psrSymbList_t *lst;
		AR_ASSERT(grammar != NULL);
		
		lst = (psrSymbList_t*)(&grammar->symb_list);
		if(grammar->symb_list.count == 0)
		{
				psrSymbTblIter_t iter;
				size_t i;
				for(iter = PSR_FirstFromSymbTbl(&grammar->term_tbl); iter.curr; PSR_NextFromSymbTbl(&grammar->term_tbl, &iter))
				{
						PSR_InsertToSymbList(lst, iter.curr->symb);
				}
				
				for(i = 0; i < grammar->count; ++i)
				{
						const psrSymb_t *head = grammar->rules[i].head;

						if(PSR_FindFromSymbList(lst, head) == -1)
						{
								PSR_InsertToSymbList(lst, head);
						}
				}
		}

		return lst;
}



bool			PSR_CheckIsValidGrammar(const psrGrammar_t *grammar)
{
		size_t i,j,k;
		bool result;
		const psrSymbList_t *lst;
		AR_ASSERT(grammar != NULL);
		
		if(grammar->count < 2)return false;/*Start和输入的第一个产生式一定>=2*/

		result = true;
		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule;
				rule = &grammar->rules[i];

				for(j = 0; j < rule->body.count; ++j)
				{
						const psrSymb_t *symb;
						symb = rule->body.lst[j];

						if(symb->type == PSR_NONTERM)
						{
								bool is_ok;
								is_ok = false;
								for(k = 0; !is_ok && k < grammar->count; ++k)
								{
										if(PSR_CompSymb(symb, grammar->rules[k].head) == 0)
										{
												is_ok = true;
										}
								}

								if(!is_ok)
								{
										AR_error(AR_GRAMMAR, L"Grammar Error : The rule %s not exist in this grammar\r\n", symb->name);
										result = false;
								}
						}

				}
		}

		lst = PSR_GetSymbList(grammar);

		
		for(i = 0; i < lst->count; ++i)
		{
				const psrSymb_t *symb;

				symb = lst->lst[i];

				if(symb->type == PSR_NONTERM && PSR_CompSymb(symb, PSR_StartSymb) != 0)
				{
						bool is_ok;
						is_ok = false;
						for(k = 0; !is_ok && k < grammar->count; ++k)
						{
								const psrRule_t *rule;
								
								rule = &grammar->rules[k];

								if(PSR_CompSymb(rule->head, symb) == 0)continue;/*自己引用自己不算*/
								
								for(j = 0; j < rule->body.count; ++j)
								{
										if(PSR_CompSymb(rule->body.lst[j], symb) == 0)
										{
												is_ok = true;
												break;
										}
								}
						}

						if(!is_ok)
						{
								AR_error(AR_GRAMMAR, L"Grammar Warning : The rule %s is declared but never used\r\n", symb->name);
						}
				}
		}

		return result;
}




void			PSR_PrintGrammar(const psrGrammar_t *grammar, arString_t *str)
{
		size_t i;
		psrSymbList_t lst;
		


		AR_AppendFormatString(str,L"Rule:\r\n");
		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule;
				rule = &grammar->rules[i];
				
				AR_AppendFormatString(str, L"id == %d  ", rule->rule_id);
				PSR_PrintSymbol(rule->head, false,str);
				AR_AppendString(str, L" -> ");
				PSR_PrintSymbolList(&rule->body, false,str);
				AR_AppendString(str, L"    ");
				PSR_PrintSymbol(rule->head, true,str);
				
				AR_AppendString(str, L"\r\n");
		}
		
		
		AR_AppendString(str, L"------------------------------------------\r\n");
		AR_AppendString(str, L"Term Set:\r\n");
		
		PSR_InitSymbList(&lst);
		
		PSR_SymbTblToSymbList(&grammar->term_tbl, &lst);

		for(i = 0; i < lst.count; ++i)
		{
				PSR_PrintSymbol(lst.lst[i], true,str);
				AR_AppendString(str, L"\r\n");
		}
		PSR_UnInitSymbList(&lst);

		AR_AppendString(str, L"------------------------------------------\r\n");
		AR_AppendString(str, L"Symbol List:\r\n");
		
		
		PSR_PrintSymbolList(PSR_GetSymbList(grammar), false,str);
		AR_AppendString(str, L"\r\n\r\n");
}



/****************************************SymbMap*********************************************************/

/*
typedef struct __parser_symbmap_record_tag
{
		const psrSymb_t			*key;
		psrSymbTbl_t			tbl;
		struct __parser_symbmap_record_tag		*next;
}psrMapRec_t;


typedef struct __parser_symbmap_tag
{
		psrMapRec_t		*bucket[1024];
		size_t			item_count;
}psrSymbMap_t;
*/



void					PSR_InitSymbMap(psrSymbMap_t *map)
{
		AR_ASSERT(map != NULL);
		AR_memset(map, 0, sizeof(*map));
}

void					PSR_UnInitSymbMap(psrSymbMap_t *map)
{
		size_t i;
		psrMapRec_t *rec, *tmp;
		
		AR_ASSERT(map != NULL);

		for(i = 0; i < MAP_BUCKET_SIZE; ++i)
		{
				rec = map->bucket[i];
				while(rec)
				{
						tmp = rec->next;
						PSR_UnInitSymbTbl(&rec->tbl);
						AR_DEL(rec);
						rec = tmp;
				}
		}
}


bool					PSR_InsertToSymbMap(psrSymbMap_t *map, const psrSymb_t *key, const psrSymb_t *val)
{
		psrMapRec_t *rec, *tmp;
		AR_ASSERT(map != NULL && key != NULL);
		
		rec = map->bucket[key->hash_code % MAP_BUCKET_SIZE];
		

		if(rec)
		{
				for(tmp = rec; tmp; tmp = tmp->next)
				{
						if(PSR_CompSymb(tmp->key, key) == 0)
						{
								break;
						}
				}

				if(tmp == NULL)
				{
						tmp = AR_NEW0(psrMapRec_t);
						tmp->key = key;
						PSR_InitSymbTbl(&tmp->tbl, 10);
						tmp->next = map->bucket[key->hash_code % MAP_BUCKET_SIZE];
						map->bucket[key->hash_code % MAP_BUCKET_SIZE] = tmp;
						map->item_count++;
				}
				rec = tmp;
		}else
		{
				rec = AR_NEW0(psrMapRec_t);
				rec->key = key;
				PSR_InitSymbTbl(&rec->tbl, 10);
				rec->next = map->bucket[key->hash_code % MAP_BUCKET_SIZE];
				map->bucket[key->hash_code % MAP_BUCKET_SIZE] = rec;
				map->item_count++;
		}
		
		if(val)
		{
				if(PSR_InsertToSymbTbl(&rec->tbl, val))
				{
						return true;
				}else
				{
						return false;
				}
		}else
		{
				return false;
		}
}


const psrMapRec_t*		PSR_GetSymbolFromSymbMap(const psrSymbMap_t *map, const psrSymb_t *key)
{
		const psrMapRec_t *rec;

		rec = map->bucket[key->hash_code % MAP_BUCKET_SIZE];

		while(rec)
		{
				if(PSR_CompSymb(key, rec->key) == 0)break;
				rec = rec->next;
		}
		return rec;
}



/*
firstset求法为，例如A->X(0)...X(n-1);
设First(Term) = {Term}; First(Epsilon) = {Epsilon}; i = 0;
如果First(X(i))存在{Epsilon}，则 First(A) += (First(X(i) - {Epsilon}); 如果i == n-1，则First(A) += {Epsilon}，本次循环终止；
如果不存在{Epsilon}，则循环终止；

因为每次都会处理至少一个符号，所以能确保循环终止，除非无任何符号加入到First集合中(changed == False);
*/
void					PSR_CalcFirstSet(const psrGrammar_t *grammar, psrSymbMap_t *first_set)
{
		size_t i;
		bool changed;
		const psrSymbList_t		*lst;
		const psrSymb_t			*key;
		AR_ASSERT(grammar != NULL && first_set != NULL && grammar->count > 1);
		
		lst = PSR_GetSymbList(grammar);

		for(i = 0; i < lst->count; ++i)
		{
				key = lst->lst[i];
				if(key->type == PSR_NONTERM)
				{
						PSR_InsertToSymbMap(first_set, key, NULL);
				}else
				{
						PSR_InsertToSymbMap(first_set, key, key);
				}
		}

		changed = true;
		while(changed)
		{
				changed = false;
				
				for(i = 0; i < grammar->count; ++i)/*对每一条产生式*/
				{
						const psrSymb_t *body;/*产生式体*/
						const psrRule_t *rule;
						size_t	n_body;
						bool need_continue;	/*是否需要继续*/

						/*产生式head[i]的体为body[i]*/
						rule = &grammar->rules[i];
						n_body = 0;
						body = PSR_IndexOfSymbList(&rule->body, n_body);
						AR_ASSERT(body != NULL);/*不可能为空，至少会有一个Epsilon或者EOI*/
						need_continue = true;
						
						while(need_continue && body != NULL)
						{
								const psrMapRec_t *first;
								psrSymbTblIter_t   f_iter;
								first = PSR_GetSymbolFromSymbMap(first_set, body);/*得到*/
								AR_ASSERT(first != NULL);
								need_continue = false;
								
								for(f_iter = PSR_FirstFromSymbTbl(&first->tbl); f_iter.curr; PSR_NextFromSymbTbl(&first->tbl, &f_iter))
								{
										const psrSymb_t *f_symb;
										f_symb = f_iter.curr->symb;
										AR_ASSERT(f_symb->type == PSR_TERM);/*first不可能为非终结符*/
										if(PSR_CompSymb(f_symb, PSR_EpsilonSymb) == 0)
										{
												need_continue = true;
										}else
										{
												if(PSR_InsertToSymbMap(first_set, rule->head, f_symb))/*如果不存在符号first，则加入到first中*/
												{
														changed = true;/*任何一步的改动都需要重新计算first集合*/
												}
										}
								}
								
								body = PSR_IndexOfSymbList(&rule->body, ++n_body);
						}
						
						if(need_continue && PSR_InsertToSymbMap(first_set, rule->head, PSR_EpsilonSymb))/*如果最后一步的计算仍包含空，则将空加入到集合First(A(i))中*/
						{
								changed = true;
						}
				}
		}



}


/*
FollowSet的求法为: 例如A->X(0)...X(n-1);
设i = 0->n-1, Follow(NULL)= {Epsilon};
如果X(i)为非终结符,则要对X(j)->X(n)循环检测,设j = i+1 && j < n;
如果First(X(j))不包含{Epsilon},则Follow(X(i)) += First(X(j)，循环结束，否则
j < n-1:Follow(X(i)) += (First(X(j)) - {Epsilon})，循环继续;
j == n-1:Follow(X(i)) += (First(X(j)) - {Epsilon})，循环结束，并将Follow(X(i) += Follow(A);
注意，当i == n-1时，Follow(X(i)) += Follow(A);,因为Follow(NULL)= {Epsilon};
*/


void					PSR_CalcFollowSet(const psrGrammar_t *grammar, psrSymbMap_t *follow_set, const psrSymbMap_t *first_set)
{
		size_t i;
		bool changed;
		const psrSymbList_t *lst;
		//const psrSymb_t *key, *body, *head, *head_follow, *first;
		AR_ASSERT(grammar != NULL && grammar->count > 1 && follow_set != NULL && first_set != NULL);

		changed = true;
		lst = PSR_GetSymbList(grammar);
		for(i = 0; i < lst->count; i++)
		{
				const psrSymb_t *key = lst->lst[i];
				if(key->type == PSR_NONTERM)
				{
						PSR_InsertToSymbMap(follow_set, key, NULL);
				}
		}

		PSR_InsertToSymbMap(follow_set, PSR_StartSymb, PSR_EOISymb);

		

		while(changed)
		{
				changed = false;
				
				for(i = 0; i < grammar->count; ++i)/*对每一条产生式*/
				{
						const psrRule_t *rule;
						const psrMapRec_t *head_follow;
						size_t nbody;
						rule = &grammar->rules[i];
						
						head_follow = PSR_GetSymbolFromSymbMap(follow_set,rule->head);/*grammar->head[i]所对应的follow集合*/
						
						for(nbody = 0; nbody < rule->body.count; ++nbody)
						{
								bool need_continue;
								const psrSymb_t *key, *next;
								size_t next_idx;
								key = rule->body.lst[nbody];
								
								if(key->type == PSR_TERM)continue;

								next_idx = nbody + 1;
								next = PSR_IndexOfSymbList(&rule->body, next_idx);
								need_continue = true;

								while(need_continue && next)
								{
										const psrMapRec_t *first_rec;
										psrSymbTblIter_t iter;
										first_rec = PSR_GetSymbolFromSymbMap(first_set, next);

										need_continue = false;
										for(iter = PSR_FirstFromSymbTbl(&first_rec->tbl); iter.curr; PSR_NextFromSymbTbl(&first_rec->tbl, &iter))
										{
												const psrSymb_t *f_symb;
												f_symb = iter.curr->symb;

												AR_ASSERT(f_symb->type == PSR_TERM);				/*first不可能为非终结符*/
												if(PSR_CompSymb(f_symb, PSR_EpsilonSymb) == 0)
												{
														need_continue = true;
												}else
												{
														if(PSR_InsertToSymbMap(follow_set, key, f_symb))/*如果不存在符号follow，则加入到follow_set中，*/
														{
																changed = true;							/*任何一步的改动都需要重新计算follow集合*/
														}
												}
										}

										next = PSR_IndexOfSymbList(&rule->body, ++next_idx);
										
								}
								

								if(need_continue)
								{
										psrSymbTblIter_t iter;
										for(iter = PSR_FirstFromSymbTbl(&head_follow->tbl); iter.curr; PSR_NextFromSymbTbl(&head_follow->tbl, &iter))
										{
												if(PSR_InsertToSymbMap(follow_set, key, iter.curr->symb))/*如果不存在符号follow，则加入到follow_set中，*/
												{
														changed = true;							/*任何一步的改动都需要重新计算follow集合*/
												}
										}
								}
						}
				}

		}
}




void			PSR_PrintSymbolMap(const psrSymbMap_t *map, arString_t *str)
{
		size_t i;

		for(i = 0; i < MAP_BUCKET_SIZE; ++i)
		{
				const psrMapRec_t *rec;
				rec = map->bucket[i];
				while(rec)
				{
						psrSymbList_t lst;
						PSR_InitSymbList(&lst);

						PSR_SymbTblToSymbList(&rec->tbl, &lst);
						PSR_PrintSymbol(rec->key, false,str);
						AR_AppendFormatString(str, L" : ");
						PSR_PrintSymbolList(&lst, false,str);
						AR_AppendFormatString(str, L"\r\n");
						PSR_UnInitSymbList(&lst);
						rec = rec->next;
				}
		}
}






AR_NAMESPACE_END




