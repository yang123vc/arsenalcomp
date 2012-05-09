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


arList_t*       AR_CreateList(AR_list_destroy_func_t dtor, void *ctx)
{
        arList_t *lst;
        
        lst = AR_NEW0(arList_t);
        
        if(lst)
        {
                lst->dtor = dtor;
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
                
                if(lst->dtor)
                {
                        lst->dtor(node->data, lst->usr_ctx);
                }
                
                AR_DEL(node);
                node = tmp;
        }
        
        lst->count = 0;
        lst->head = lst->tail = NULL;
        
}


arStatus_t      AR_InsertToListByNode(arList_t *lst, arListNode_t *node, void *data)
{
        arListNode_t *new_node;
        AR_ASSERT(lst != NULL);
        
        new_node = AR_NEW0(arListNode_t);
        
        if(new_node == NULL)
        {
                return AR_E_NOMEM;
        }
        
        new_node->data = data;
        
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
        
        if(lst->dtor)
        {
                lst->dtor(node->data, lst->usr_ctx);
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

		hash->bucket = AR_NEWARR0(arList_t*, bucket_size);

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
        arList_t *lst;
        arListNode_t *node;
        size_t i;
        AR_ASSERT(hash != NULL);
        
        for(i = 0; i < hash->bucket_size; ++i)
        {
                lst = hash->bucket[i];
                
                if(lst == NULL)
                {
                        continue;
                }
                
                for(node = lst->head; node != NULL; node = node->next)
                {
                        arHashNode_t *hn = (arHashNode_t*)node->data;
                        AR_ASSERT(hn != NULL);
						if(hash->destroy_key_f)
						{
								hash->destroy_key_f(hn->key, hash->usr_ctx);
						}
						if(hash->destroy_val_f)
						{
								hash->destroy_val_f(hn->val, hash->usr_ctx);
						}

                        AR_DEL(hn);
                }
                
                AR_DestroyList(lst);
                hash->bucket[i] = NULL;
        }

        hash->item_count = 0;
}



arStatus_t		AR_FindFromHash(arHash_t *hash, void *key, void **pval)
{
        arList_t *lst;
        arListNode_t *node;
        uint_64_t hash_code;
        AR_ASSERT(hash != NULL && pval != NULL);
        
        hash_code = hash->hash_f(key);
        
        lst = hash->bucket[hash_code % hash->bucket_size];
        
        if(lst == NULL)
        {
                return AR_E_NOTFOUND;
        }
        
        for(node = lst->head; node != NULL; node = node->next)
        {
                arHashNode_t *hn;
                hn = (arHashNode_t*)node->data;
                AR_ASSERT(hn != NULL);
                
                if(hash->comp_f(hn->key, key) == 0)
                {
                        *pval = hn->val;
                        return AR_S_YES;
                }
        }
        
        return AR_E_NOTFOUND;
}

size_t			AR_GetHashCount(arHash_t *hash)
{
        AR_ASSERT(hash != NULL);
        return (size_t)hash->item_count;
}


arStatus_t		AR_InsertToHash(arHash_t *hash, void *key, void *val)
{
        arStatus_t status;
        arList_t *lst;
        uint_64_t hash_code;
        arHashNode_t *new_node;
		bool_t key_init, val_init;
        AR_ASSERT(hash != NULL);
        
        status = AR_S_YES;
        key_init = false;
		val_init = false;

        AR_RemoveFromHash(hash, key);
        
        
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
        
        hash_code = hash->hash_f(key);
        
        if(hash->bucket[hash_code % (uint_64_t)hash->bucket_size] == NULL)
        {
                hash->bucket[hash_code % hash->bucket_size] = AR_CreateList(NULL, NULL);
                
                if(hash->bucket[hash_code % hash->bucket_size] == NULL)
                {
                        status = AR_E_NOMEM;
                        goto INVALID_POINT;
                }
        }

        lst = hash->bucket[hash_code % hash->bucket_size];
        status = AR_InsertToListByNode(lst, NULL, (void*)new_node);
        
        if(status != AR_S_YES)
        {
                goto INVALID_POINT;
        }
        
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

arStatus_t		AR_RemoveFromHash(arHash_t *hash, void *key)
{
        arStatus_t status;
        arList_t *lst;
        arListNode_t *node;
        uint_64_t hash_code;

        AR_ASSERT(hash != NULL);
        
        status = AR_S_YES;
        
        
        hash_code = hash->hash_f(key);
        
        lst = hash->bucket[hash_code % (uint_64_t)hash->bucket_size];

        if(lst == NULL)
        {
                return AR_E_NOTFOUND;
        }

        for(node = lst->head; node != NULL; node = node->next)
        {
                arHashNode_t *hn;
                hn = (arHashNode_t*)node->data;
                AR_ASSERT(hn != NULL);
                
                if(hash->comp_f(hn->key, key) == 0)
                {
						if(hash->destroy_key_f)
						{
								hash->destroy_key_f(hn->key, hash->usr_ctx);
								hn->key = NULL;
						}

						if(hash->destroy_val_f)
						{
								hash->destroy_val_f(hn->val, hash->usr_ctx);
								hn->val = NULL;
						}

                        AR_DEL(hn);
                        hn = NULL;
                        AR_RemoveFromList(lst, node);
                        AR_ASSERT(hash->item_count > 0);
                        hash->item_count--;
                        return AR_S_YES;
                }
        }

        return AR_E_NOTFOUND;

}



AR_NAMESPACE_END

