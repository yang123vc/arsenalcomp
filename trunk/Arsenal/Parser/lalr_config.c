
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
#include "lalr_config_aux.h"

AR_NAMESPACE_BEGIN




void PSR_Init_LALR_Config()
{
		
		__init_config_freelist();
		__init_config_list_freelist();
		__init_node_freelist();
}

void PSR_UnInit_LALR_Config()
{
		__uninit_node_freelist();
		__uninit_config_list_freelist();
		__uninit_config_freelist();
		

		
}



/**************************************lalr config list*********************************************/


lalrConfigList_t*		PSR_CreateConfigList()
{
		lalrConfigList_t *res = __create_config_list();
		AR_memset(res, 0, sizeof(lalrConfigList_t));
		return res;
}

void					PSR_DestroyConfigList(lalrConfigList_t *lst, bool_b_t destroy_config)
{
		lalrConfigNode_t		*node;

		AR_ASSERT(lst != NULL);
		
		while(lst->head)
		{
				node = lst->head;
				lst->head = lst->head->next;

				if(destroy_config)
				{
						PSR_UnInitConfig(node->config);
						__destroy_config(node->config);
				}
				__destroy_node(node);
		}

		__destroy_config_list(lst);
}

lalrConfig_t*			PSR_InsertToConfigListByValue(lalrConfigList_t *lst, const psrRule_t *rule, size_t delim)
{
		lalrConfig_t *cfg;
		AR_ASSERT(lst != NULL && rule != NULL);


		cfg = __create_config();

		PSR_InitConfig(cfg, rule, delim);

		PSR_InsertToConfigList(lst, cfg);
		
		return cfg;
}


void					PSR_InsertToConfigList(lalrConfigList_t *lst, lalrConfig_t *cfg)
{
		lalrConfigNode_t		*node;
		AR_ASSERT(lst != NULL && cfg != NULL);
		
		/*node = AR_NEW0(lalrConfigNode_t);*/
		node = __create_node();
		
		node->config = cfg;
		node->next = NULL;

		if(lst->head == NULL)
		{
				AR_ASSERT(lst->count == 0);
				AR_ASSERT(lst->tail == NULL);
				lst->head = lst->tail = node;
		}else
		{
				lst->tail->next = node;
				lst->tail = node;
		}
		
		lst->count++;
}


void					PSR_UnionConfigList(lalrConfigList_t *l, lalrConfigList_t *r)
{
		
		AR_ASSERT(l != NULL && r != NULL);

		if(r->count == 0)return;

		if(l->head == NULL)
		{
				AR_ASSERT(l->count == 0 && l->tail == NULL);
				l->head = r->head;
				l->tail = r->tail;
				l->count = r->count;

				r->head = r->tail = NULL;
				r->count = 0;
		}else
		{
				AR_ASSERT(l->tail != NULL && l->count > 0);

				l->tail->next = r->head;
				l->tail = r->tail;
				l->count += r->count;
				r->head = r->tail = NULL;
				r->count = 0;
		}
}


void					PSR_CopyConfigList(lalrConfigList_t *l, const lalrConfigList_t *r)
{
		const lalrConfigNode_t		*node;
		AR_ASSERT(l != NULL && r != NULL);
		
		for(node = r->head; node != NULL; node = node->next)
		{
				PSR_InsertToConfigList(l, node->config);
		}
}


#if(0)
int_i_t					PSR_CompConfigList(const lalrConfigList_t *l, const lalrConfigList_t *r)
{
		int_i_t cmp = 0;
		const lalrConfigNode_t		*a, *b;
		AR_ASSERT(l != NULL && r != NULL);
		if(l == r)return 0;
		
		cmp = AR_CMP(l->count, r->count);
		if(cmp != 0)return cmp;
		for(a = l->head, b = r->head; a != NULL && b != NULL; a = a->next, b = b->next)
		{
				cmp = PSR_CompConfig(a->config, b->config);
				if(cmp != 0)return cmp;
		}
		
		if(a == NULL && b == NULL)
		{
				return 0;
		}else if(a != NULL)
		{
				return 1;
		}else
		{
				return -1;
		}
}
#endif

int_i_t					PSR_CompConfigList(const lalrConfigList_t *l, const lalrConfigList_t *r)
{
		int_i_t cmp = 0;
		const lalrConfigNode_t		*a, *b;
		AR_ASSERT(l != NULL && r != NULL);
		if(l == r)return 0;
		
		cmp = (int_i_t)l->count - (int_i_t)r->count;
		if(cmp != 0)return cmp;
		for(a = l->head, b = r->head; a != NULL && b != NULL; a = a->next, b = b->next)
		{
				cmp = PSR_CompConfig(a->config, b->config);
				if(cmp != 0)return cmp;
		}
		
		return (int_i_t)a - (int_i_t)b;

}


lalrConfig_t*			PSR_FindFromConfigList(lalrConfigList_t *lst, const psrRule_t *rule, size_t delim)
{
		lalrConfigNode_t		*node;
		
		AR_ASSERT(lst != NULL);
		
		node = lst->head;

		while(node)
		{
				if(node->config->rule == rule && node->config->delim == delim)
				{
						return node->config;
				}else
				{
						node = node->next;
				}
		}

		return NULL;
}



/************************************************************Sort*****************************************************/

static AR_INLINE void __merge_list(lalrConfigList_t *dest, lalrConfigList_t *a, lalrConfigList_t *b)
{
		lalrConfigNode_t *l, *r;
		lalrConfigNode_t *prev , *tmp, *head;
		AR_ASSERT(a != NULL && b != NULL && dest != NULL);

		l = a->head; r = b->head;
		prev = NULL, head = NULL;

		while(l && r)
		{
				int_i_t cmp;

				cmp = PSR_CompConfig(l->config, r->config);

				if(cmp < 0)
				{
						tmp = l;
						l = l->next;
				}else
				{
						tmp = r;
						r = r->next;
				}	
				
				tmp->next = NULL;

				if(prev == NULL)
				{
						prev = tmp;
						head = tmp;
				}else
				{
						prev->next = tmp;
						prev = prev->next;
				}
		}

		if(l)
		{
				if(prev)
				{
						prev->next = l;
				}else
				{
						head = l;
				}
		}else
		{
				if(prev)
				{
						prev->next = r;
				}else
				{
						head = r;
				}
		}
		
		AR_memset(a, 0, sizeof(*a));
		AR_memset(b, 0, sizeof(*b));

		dest->head = head;
		
		tmp = dest->head;
		while(tmp)
		{
				dest->count++;
				tmp = tmp->next;
		}
}

static AR_INLINE lalrConfigNode_t* __pop_head(lalrConfigList_t *lst)
{
		lalrConfigNode_t *res;
		AR_ASSERT(lst != NULL);

		if(lst->count == 0)return NULL;

		res = lst->head;
		
		lst->head = lst->head->next;

		if(lst->head == NULL)
		{
				lst->tail = NULL;
		}

		lst->count--;
		return res;
}

static AR_INLINE void		__insert_back(lalrConfigList_t *lst, lalrConfigNode_t *node)
{
		
		AR_ASSERT(lst != NULL && node != NULL);

		node->next = NULL;
		if(lst->head == NULL)
		{
				AR_ASSERT(lst->count == 0);
				AR_ASSERT(lst->tail == NULL);
				lst->head = lst->tail = node;
		}else
		{
				lst->tail->next = node;
				lst->tail = node;
		}
		
		lst->count++;

}


static AR_INLINE void __sort_list(lalrConfigList_t *sour_list)
{
		int_i_t i,fill;
		lalrConfigList_t lst, carry, tmp_list[64];

		AR_ASSERT(sour_list != NULL);

		AR_memset(&lst, 0, sizeof(lst));
		AR_memset(&carry, 0, sizeof(carry));
		
		AR_memset(tmp_list, 0, sizeof(tmp_list));
		
		fill = 0;
		lst = *sour_list;

		while(lst.count > 0)
		{
				lalrConfigNode_t		*tmp;
				tmp = __pop_head(&lst);
				tmp->next = NULL;
				
				__insert_back(&carry, tmp);
				
				for(i = 0; i < fill && tmp_list[i].count > 0; ++i)
				{
						__merge_list(&carry,&tmp_list[i], &carry);
				}
				if(i == fill)fill++;
				tmp_list[i] = carry;/*把前面归并排序好的放入下一个桶，carry清空*/
				
				AR_memset(&carry, 0, sizeof(carry));
		}

		for(i = 1; i < fill; ++i)
		{
				lalrConfigList_t		*dest, *l, *r;

				dest	=		&tmp_list[i];
				l		=		&tmp_list[i];
				r		=		&tmp_list[i-1];
				__merge_list(dest, l, r);
		}

		*sour_list = tmp_list[fill-1];
}


/***************************************************************************************************************************/


void					PSR_SortConfigList(lalrConfigList_t *l)
{
		__sort_list(l);

}


/***************************************lalr config******************************************************/


void	PSR_InitConfig(lalrConfig_t *config, const psrRule_t *rule, size_t delim)
{
		AR_ASSERT(config != NULL && rule != NULL);

		AR_memset(config, 0, sizeof(*config));
		config->rule = rule;
		config->delim = delim;
		
		config->forward = PSR_CreateConfigList();
		config->backward = PSR_CreateConfigList();
		PSR_InitSymbList(&config->follow_set);
		config->is_completed = false;
}


void	PSR_UnInitConfig(lalrConfig_t *config)
{
		AR_ASSERT(config != NULL);

		PSR_DestroyConfigList(config->forward, false);
		PSR_DestroyConfigList(config->backward, false);
		PSR_UnInitSymbList(&config->follow_set);

		AR_memset(config, 0, sizeof(*config));
}

#if(0)

int_i_t	PSR_CompConfig(const lalrConfig_t *l, const lalrConfig_t *r)
{
		int_i_t cmp;
		AR_ASSERT(l != NULL && r != NULL);

		if(l == r)return 0;
		
		cmp = AR_CMP(l->rule, r->rule);

		if(cmp != 0)return cmp;

		cmp = AR_CMP(l->delim, r->delim);
		
		return cmp;
}
#endif


int_i_t	PSR_CompConfig(const lalrConfig_t *l, const lalrConfig_t *r)
{
		int_i_t cmp;
		AR_ASSERT(l != NULL && r != NULL);

		if(l == r)return 0;
		
		cmp = (int_i_t)l->rule - (int_i_t)r->rule;
		if(cmp != 0)return cmp;
		
		
		cmp = (int_i_t)l->delim - (int_i_t)r->delim;
		return cmp;
}


void PSR_PrintConfig(const lalrConfig_t *config, const psrGrammar_t *gmr, arString_t *str)
{
		size_t i;
		const psrRule_t *rule;
		AR_ASSERT(config != NULL && gmr != NULL && str != NULL);

		rule = config->rule;

		AR_AppendString(str, L"[ ");
		
		AR_AppendFormatString(str, L"<%ls> -> ", rule->head->name);
		
		i = 0;
		
		for(i = 0; i < config->delim; ++i)
		{
				const psrSymb_t	*curr;
				curr = PSR_IndexOfSymbList(&rule->body, i);
				PSR_PrintSymbol(curr, str);
		}
		
		AR_AppendString(str, L". ");

		for(i; i < rule->body.count; ++i)
		{
				const psrSymb_t	*curr;
				curr = PSR_IndexOfSymbList(&rule->body, i);
				PSR_PrintSymbol(curr, str);
		}

		if(config->follow_set.count > 0)
		{
				AR_AppendString(str, L" : ");
				PSR_PrintSymbolList(&config->follow_set, str);
		}

		AR_AppendString(str, L" ]");


}

void PSR_PrintConfigList(const lalrConfigList_t *lst, const psrGrammar_t *gmr, arString_t *str)
{
		lalrConfigNode_t *node;

		AR_ASSERT(lst != NULL && gmr != NULL && str != NULL);

		for(node = lst->head; node != NULL; node = node->next)
		{
				PSR_PrintConfig(node->config, gmr, str);
				AR_AppendString(str, L"\r\n");
		}
		AR_AppendString(str, L"\r\n");
}

/************************************************************************************************************************/




AR_NAMESPACE_END



