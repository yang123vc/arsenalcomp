/*
 * The Arsenal Library
 * Copyright (c) 2009-2012 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#include "common.h"



AR_NAMESPACE_BEGIN



/******************************************************List**************************************************************/

arList_t*       AR_CreateList(AR_list_copy_func_t copy_f, AR_list_destroy_func_t dtor_f, void *ctx)
{
        arList_t *lst;
        
        lst = AR_NEW0(arList_t);
        
        if(lst)
        {
				lst->copy_f = copy_f;
                lst->dtor_f = dtor_f;
                lst->usr_ctx = ctx;
        }
        
        return lst;
}


void            AR_DestroyList(arList_t *lst)
{
        AR_ASSERT(lst != NULL);
        AR_ClearList(lst);
        AR_DEL(lst);
        lst = NULL;
}


void AR_ClearList(arList_t *lst)
{
        arListNode_t *node, *tmp;
        AR_ASSERT(lst != NULL);
        node = lst->head;
        
        while(node)
        {
                tmp = node->next;
                
                if(lst->dtor_f)
                {
                        lst->dtor_f(node->data, lst->usr_ctx);
                }
                
                AR_DEL(node);
                node = tmp;
        }
        
        lst->count = 0;
        lst->head = lst->tail = NULL;
        
}

arStatus_t      AR_InsertToList(arList_t *lst, arListNode_t *node, void *data)
{
        arListNode_t *new_node;
        AR_ASSERT(lst != NULL);
        
        new_node = AR_NEW0(arListNode_t);
        
        if(new_node == NULL)
        {
                return AR_E_NOMEM;
        }
        
		if(lst->copy_f)
		{
				arStatus_t status;

				status = lst->copy_f(data, &new_node->data, lst->usr_ctx);
				if(status != AR_S_YES)
				{
						AR_DEL(new_node);
						return status;
				}
		}else
		{
				new_node->data = data;
		}
        
        if(lst->count == 0)
        {
                AR_ASSERT(lst->head == NULL && lst->tail == NULL);
                lst->head = lst->tail = new_node;
        }else
        {
                AR_ASSERT(lst->head != NULL && lst->tail != NULL);
                if(node == NULL)                /*如果node为空，则將￼新node插入队首*/
                {
                        new_node->prev = NULL;
                        new_node->next = lst->head;
                        lst->head->prev = new_node;
                        lst->head = new_node;
                }else
                {
                        new_node->prev = node;
                        new_node->next = node->next;
                        if(node->next != NULL)
                        {
                                node->next->prev = new_node;
                        }else
                        {
                                AR_ASSERT(node == lst->tail);
                                lst->tail = new_node;
                        }
                        node->next = new_node;
                }
        }
        
        lst->count += 1;
        return AR_S_YES;
        
}


void      AR_RemoveFromList(arList_t *lst, arListNode_t *node)
{
        AR_ASSERT(lst != NULL && node != NULL);
        
        if(node == lst->head)
        {
                lst->head = node->next;
                if(lst->head != NULL)
                {
                        lst->head->prev = NULL;
                }

        }else
        {
                node->prev->next = node->next;
                if(node->next == NULL)
                {
                        lst->tail = node->prev;
                }else
                {
                        node->next->prev = node->prev;
                }
        }
        
        if(lst->dtor_f)
        {
                lst->dtor_f(node->data, lst->usr_ctx);
        }
        
        AR_DEL(node);

        lst->count--;
        if(lst->count == 0)
        {
                lst->head = lst->tail = NULL;
        }
        
}


size_t          AR_GetListCount(const arList_t *lst)
{
        AR_ASSERT(lst != NULL);
        return lst->count;
}



arStatus_t		AR_PushListBack(arList_t *lst, void *data)
{
		AR_ASSERT(lst != NULL);
		return AR_InsertToList(lst, lst->tail, data);
}

arStatus_t		AR_PushListFront(arList_t *lst, void *data)
{
		AR_ASSERT(lst != NULL);
		return AR_InsertToList(lst, NULL, data);
}

arStatus_t		AR_PopListBack(arList_t *lst)
{
		AR_ASSERT(lst != NULL);

		if(AR_GetListCount(lst) > 0)
		{
				AR_RemoveFromList(lst, lst->tail);
				return AR_S_YES;
		}else
		{
				return AR_E_NOMORE;
		}
}

arStatus_t		AR_PopListFront(arList_t *lst)
{
		AR_ASSERT(lst != NULL);

		if(AR_GetListCount(lst) > 0)
		{
				AR_RemoveFromList(lst, lst->head);
				return AR_S_YES;
		}else
		{
				return AR_E_NOMORE;
		}
}

arStatus_t		AR_GetListFront(arList_t *lst, void **pdata)
{
		AR_ASSERT(lst != NULL && pdata != NULL);
		if(AR_GetListCount(lst) > 0)
		{
				AR_ASSERT(lst->head != NULL && lst->tail != NULL);
				*pdata = lst->head->data;
				return AR_S_YES;
		}else
		{
				return AR_E_NOTFOUND;
		}
}

arStatus_t		AR_GetListBack(arList_t *lst, void **pdata)
{
		AR_ASSERT(lst != NULL && pdata != NULL);
		if(AR_GetListCount(lst) > 0)
		{
				AR_ASSERT(lst->head != NULL && lst->tail != NULL);
				*pdata = lst->tail->data;
				return AR_S_YES;
		}else
		{
				return AR_E_NOTFOUND;
		}
}

/******************************************************Hash**************************************************************/
/*

typedef struct __arsenal_hash_node_tag
{
		void	*key;
		void	*val;
}arHashNode_t;

typedef struct __arsenal_hash_tag
{
		arHashNode_t	**bucket;
		size_t			bucket_size;
		size_t			item_count;

		AR_hash_hash_func_t		hash_f;
		AR_hash_comp_func_t		comp_f;

		AR_hash_destroy_func_t	dtor_f;
		void					*usr_ctx;
}arHash_t;
*/

arHash_t*		AR_CreateHash(size_t bucket_size, AR_hash_hash_func_t hash_f, AR_hash_comp_func_t comp_f, AR_hash_copy_func_t copykey_f, AR_hash_copy_func_t copyval_f, AR_hash_destroy_func_t destroy_key_f, AR_hash_destroy_func_t destroy_val_f, void *usr_ctx)
{
		arHash_t *hash;
		AR_ASSERT(bucket_size > 0 && hash_f != NULL && comp_f != NULL);
		hash = AR_NEW0(arHash_t);
		
		if(hash == NULL)
		{
				goto FAILED_POINT;
		}

		hash->bucket = AR_NEWARR0(arHashNode_t*, bucket_size);

		if(hash->bucket == NULL)
		{
				goto FAILED_POINT;
		}

		hash->bucket_size = bucket_size;
		hash->comp_f = comp_f;
		hash->hash_f = hash_f;
		hash->copykey_f = copykey_f;
		hash->copyval_f = copyval_f;
		hash->destroy_key_f = destroy_key_f;
		hash->destroy_val_f = destroy_val_f;
		hash->usr_ctx = usr_ctx;

		return hash;

FAILED_POINT:
		if(hash && hash->bucket != NULL)
		{
				AR_DEL(hash->bucket);
				hash->bucket = NULL;
		}

		if(hash)
		{
				AR_DEL(hash);
				hash = NULL;
		}

		return NULL;

}

void			AR_DestroyHash(arHash_t *hash)
{
		AR_ASSERT(hash != NULL);
		AR_ClearHash(hash);
		if(hash->bucket)
		{
				AR_DEL(hash->bucket);
				hash->bucket = NULL;
		}

		AR_DEL(hash);
		hash = NULL;
}


void			AR_ClearHash(arHash_t *hash)
{
        arHashNode_t *node, *tmp;
        size_t i;
        AR_ASSERT(hash != NULL);
        
        for(i = 0; i < hash->bucket_size; ++i)
        {
                node = hash->bucket[i];
				
				while(node)
				{
						if(hash->destroy_key_f)
						{
								hash->destroy_key_f(node->key, hash->usr_ctx);
						}
						if(hash->destroy_val_f)
						{
								hash->destroy_val_f(node->val, hash->usr_ctx);
						}

						tmp = node->next;
						AR_DEL(node);
						node = tmp;
				}

                hash->bucket[i] = NULL;
        }

        hash->item_count = 0;
}



arStatus_t		AR_FindFromHash(arHash_t *hash, void *key, void **pval)
{
        
        arHashNode_t *node;
        uint_64_t hash_code;
        AR_ASSERT(hash != NULL && pval != NULL);
        
        hash_code = hash->hash_f(key, hash->usr_ctx);
        
        node = hash->bucket[hash_code % hash->bucket_size];
        
        
		while(node)
		{
				if(hash->comp_f(node->key, key, hash->usr_ctx) == 0)
                {
                        *pval = node->val;
                        return AR_S_YES;
                }

				node = node->next;
		}
        
		return AR_E_NOTFOUND;
}

size_t			AR_GetHashCount(arHash_t *hash)
{
        AR_ASSERT(hash != NULL);
        return (size_t)hash->item_count;
}

void			AR_HashForEach(arHash_t *hash, AR_hash_visit_func_t visit)
{
		arHashNode_t *node;
		size_t i;
		AR_ASSERT(hash != NULL && visit != NULL);
		for(i = 0; i < hash->bucket_size; ++i)
		{
				node = hash->bucket[i];

				while(node)
				{
						if(!visit(node->key, node->val, hash->usr_ctx))
						{
								return;
						}
						node = node->next;
				}
		}
}


static arStatus_t __remove_key_by_hashcode(arHash_t *hash, void *key, uint_64_t hash_code)
{
		arStatus_t status;
        arHashNode_t *prev, *node;
		AR_ASSERT(hash != NULL);
		status = AR_S_YES;
		node = hash->bucket[hash_code % (uint_64_t)hash->bucket_size];
		
		prev = NULL;
		while(node)
		{
				if(hash->comp_f(node->key, key, hash->usr_ctx) == 0)
                {
						if(prev == NULL)
						{
								hash->bucket[hash_code % (uint_64_t)hash->bucket_size] = node->next;
								node->next = NULL;
						}else
						{
								prev->next = node->next;
								node->next = NULL;
						}

						if(hash->destroy_key_f)
						{
								hash->destroy_key_f(node->key, hash->usr_ctx);
								node->key = NULL;
						}

						if(hash->destroy_val_f)
						{
								hash->destroy_val_f(node->val, hash->usr_ctx);
								node->val = NULL;
						}
                        AR_DEL(node);
                        
						AR_ASSERT(hash->item_count > 0);
                        hash->item_count--;
                        return AR_S_YES;
                }

				prev = node;
				node = node->next;
		}

        return AR_E_NOTFOUND;

}



arStatus_t		AR_RemoveFromHash(arHash_t *hash, void *key)
{
        uint_64_t hash_code;
        AR_ASSERT(hash != NULL);
        
        hash_code = hash->hash_f(key, hash->usr_ctx);
        return __remove_key_by_hashcode(hash, key, hash_code);

}



arStatus_t		AR_InsertToHash(arHash_t *hash, void *key, void *val)
{
        arStatus_t status;
        uint_64_t hash_code;
        arHashNode_t *new_node;
		bool_t key_init, val_init;
        AR_ASSERT(hash != NULL);
        
        status = AR_S_YES;
        key_init = false;
		val_init = false;

		hash_code = hash->hash_f(key, hash->usr_ctx);

		new_node = AR_NEW0(arHashNode_t);
        if(new_node == NULL)
        {
                status = AR_E_NOMEM;
                goto INVALID_POINT;
        }
        
		if(hash->copykey_f != NULL)
		{
				status = hash->copykey_f(key, &new_node->key, hash->usr_ctx);

				if(status != AR_S_YES)
				{
						goto INVALID_POINT;
				}
		}else
		{
				new_node->key = key;
		}

		key_init = true;

		if(hash->copyval_f != NULL)
		{
				status = hash->copyval_f(val, &new_node->val, hash->usr_ctx);
				
				if(status != AR_S_YES)
				{
						goto INVALID_POINT;
				}
		}else
		{
				new_node->val = val;
		}

        val_init = true;

		__remove_key_by_hashcode(hash, key, hash_code);
       
		new_node->next = hash->bucket[hash_code % (uint_64_t)hash->bucket_size];
        hash->bucket[hash_code % (uint_64_t)hash->bucket_size] = new_node;
        
        hash->item_count++;
        return AR_S_YES;
        

INVALID_POINT:
        if(new_node != NULL)
        {
				if(key_init && hash->destroy_key_f)
				{
						hash->destroy_key_f(new_node->key, hash->usr_ctx);
						new_node->key = NULL;
						key_init = false;
				}

				if(val_init && hash->destroy_val_f)
				{
						hash->destroy_val_f(new_node->val, hash->usr_ctx);
						new_node->val = NULL;
						val_init = false;
				}

                AR_DEL(new_node);
                new_node = NULL;
        }
        
        return status;
}

/*
typedef struct __arsenal_hash_iterator_tag
{
		arHash_t		*hash;
		size_t			bucket_idx;
		arHashNode_t	*curr;
}arHashIter_t;
*/

void	AR_InitHashIterator(arHash_t *hash, arHashIter_t *iter)
{
		AR_ASSERT(hash != NULL && iter != NULL);
		iter->hash = hash;
		iter->bucket_idx = 0;
		iter->curr = hash->bucket[0];
		if(iter->curr == NULL)
		{
				AR_HashIteratorNext(iter);
		}
}

void	AR_UnInitHashIterator(arHashIter_t *iter)
{
		AR_ASSERT(iter != NULL);
		AR_memset(iter, 0, sizeof(*iter));
}

bool_t	AR_HashIteratorIsDone(const arHashIter_t *iter)
{
		AR_ASSERT(iter != NULL);
		return iter->curr == NULL ? true : false;
}


void	AR_HashIteratorNext(arHashIter_t *iter)
{
		AR_ASSERT(iter != NULL && iter->hash != NULL);

		if(iter->curr != NULL)
		{
				iter->curr = iter->curr->next;
		}
		
		while(iter->curr == NULL)
		{
				if(iter->bucket_idx < iter->hash->bucket_size - 1)
				{
						iter->bucket_idx++;
						iter->curr = iter->hash->bucket[iter->bucket_idx];
				}else
				{
						break;
				}

		}
}

void*	AR_GetHashIteratorKey(const arHashIter_t *iter)
{
		AR_ASSERT(iter != NULL);
		AR_ASSERT(!AR_HashIteratorIsDone(iter));
		return iter->curr->key;
}

void*	AR_GetHashIteratorData(const arHashIter_t *iter)
{
		AR_ASSERT(iter != NULL);
		AR_ASSERT(!AR_HashIteratorIsDone(iter));
		return iter->curr->val;
}


AR_NAMESPACE_END

