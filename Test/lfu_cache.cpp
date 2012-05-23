//
//  lfu_cache.c
//  Test
//
//  Created by Solidus on 12-5-22.
//  Copyright (c) 2012年 none. All rights reserved.
//


#include "lfu_cache.h"
#include <string>


AR_NAMESPACE_BEGIN

#if(0)
typedef struct __cahce_value_node_tag
{
        void                    *data;
        size_t					access_count;
}cacheValueNode_t;


static cacheValueNode_t* create_value_node(void *data, size_t access_count)
{
        cacheValueNode_t *node;

        node = AR_NEW(cacheValueNode_t);
        if(node == NULL)
        {
                return NULL;
        }
        node->data = data;
        node->access_count = access_count;
        return node;
}


static void destroy_value_node(cacheValueNode_t *node)
{
        AR_ASSERT(node != NULL);
        AR_DEL(node);
        node = NULL;
}


struct __cache_lfu_tag
{
        arHash_t                *table;
		
        cacheValueNode_t		**min_heap;
		size_t					items_count;
        size_t                  max_items;
        
		Cache_hash_func_t       hash_f;
        Cache_comp_func_t       comp_f;
        
        Cache_copy_func_t       copy_key_f;
        Cache_copy_func_t       copy_data_f;
        
        Cache_destroy_func_t    destroy_key_f;
        Cache_destroy_func_t    destroy_data_f;
        void                    *usr_ctx;
};



static uint_64_t	table_hash_func(void *key, void *usr_ctx)
{
        cacheLFU_t *lfu;
        AR_ASSERT(usr_ctx != NULL);
        lfu = (cacheLFU_t*)usr_ctx;
        return lfu->hash_f(key, lfu->usr_ctx);
}

static int_t			table_comp_func(void *l, void *r, void *usr_ctx)
{
        cacheLFU_t *lfu;
        AR_ASSERT(usr_ctx != NULL);
        lfu = (cacheLFU_t*)usr_ctx;
        return lfu->comp_f(l,r,lfu->usr_ctx);
        
}


static void			table_destroy_key_func(void *key, void *ctx)
{
        cacheLFU_t *lfu;

        AR_ASSERT(ctx != NULL);
        lfu = (cacheLFU_t*)ctx;
        lfu->destroy_key_f(key, lfu->usr_ctx);
}


static void			table_destroy_data_func(void *data, void *ctx)
{
        cacheLFU_t *lfu;
        cacheValueNode_t        *val_node;
        AR_ASSERT(data != NULL && ctx != NULL);
        lfu = (cacheLFU_t*)ctx;
        val_node = (cacheValueNode_t*)data;
        
		lfu->destroy_key_f(val_node->data, lfu->usr_ctx);
        destroy_value_node(val_node);
        val_node = NULL;
}


cacheLFU_t*     Cache_CreateLFU(cacheLFUCtx_t *ctx, size_t max_items_cnt)
{
        cacheLFU_t *lfu;
        AR_ASSERT(ctx != NULL && max_items_cnt > 0);
        AR_ASSERT(ctx->hash_f != NULL && ctx->comp_f != NULL);
        
        lfu = AR_NEW0(cacheLFU_t);
        
        if(lfu == NULL)
        {
                goto INVALID_POINT;
        }

		lfu->min_heap = AR_NEWARR0(cacheValueNode_t*, max_items_cnt);

		if(lfu->min_heap == NULL)
		{
				goto INVALID_POINT;
		}
		
		lfu->items_count = 0;
		lfu->max_items = max_items_cnt;
        lfu->hash_f = ctx->hash_f;
        lfu->comp_f = ctx->comp_f;
        lfu->copy_key_f = ctx->copy_key_f;
        lfu->copy_data_f = ctx->copy_data_f;
        lfu->destroy_key_f = ctx->destroy_key_f;
        lfu->destroy_data_f = ctx->destroy_data_f;
        lfu->usr_ctx = ctx->usr_ctx;


        lfu->table = AR_CreateHash(3079, table_hash_func, table_comp_func, NULL, NULL, table_destroy_key_func, table_destroy_data_func, (void*)lfu);
        
        if(lfu->table == NULL)
        {
                goto INVALID_POINT;
        }
        
        
        return lfu;
INVALID_POINT:
        
		if(lfu && lfu->min_heap)
		{
				AR_DEL(lfu->min_heap);
				lfu->min_heap = NULL;
		}

        if(lfu && lfu->table)
        {
                AR_DestroyHash(lfu->table);
                lfu->table = NULL;
        }

        if(lfu)
        {
                AR_DEL(lfu);
                lfu = NULL;
        }

        return NULL;

}

void            Cache_DestroyLFU(cacheLFU_t *lfu)
{
        AR_ASSERT(lfu != NULL);

		Cache_ClearLFU(lfu);
		
		if(lfu->table)
		{
				AR_DestroyHash(lfu->table);
				lfu->table = NULL;
		}
		
		if(lfu->min_heap)
		{
				AR_DEL(lfu->min_heap);
				lfu->min_heap = NULL;
		}
                
		AR_DEL(lfu);
		lfu = NULL;
}


void      Cache_ClearLFU(cacheLFU_t *lfu)
{
        AR_ASSERT(lfu != NULL);

		lfu->items_count = 0;
		AR_memset(lfu->min_heap, 0, sizeof(lfu->min_heap[0]) * lfu->max_items);
		AR_ClearHash(lfu->table);
}



static void __free_one_least_used(cacheLFU_t *lfu)
{
        
}


static arStatus_t     __replace_value_node(cacheLFU_t *lfu, cacheValueNode_t        *val_node, void *data)
{
        arStatus_t status;
        void *new_data;
        AR_ASSERT(lfu != NULL && val_node != NULL);
		
		status = AR_S_YES;
        new_data = NULL;

        status = lfu->copy_data_f(data, &new_data, lfu->usr_ctx);
        
        if(status != AR_S_YES)
        {
                return status;
        }
        
        if(lfu->destroy_data_f)
        {
                lfu->destroy_data_f(val_node->data, lfu->usr_ctx);
                val_node->data = NULL;
        }

        val_node->data = data;
        
        return status;
}

arStatus_t      Cache_InsertToLFU(cacheLFU_t *lfu, void *key, void *data)
{
        arStatus_t status;
        cacheValueNode_t        *val_node;
        void                    *new_key, *new_data;
        AR_ASSERT(lfu != NULL);
        
        status = AR_FindFromHash(lfu->table, key, (void**)&val_node);
        
        if(status == AR_S_YES)
        {
                return __replace_value_node(lfu, val_node, data);
        }

        while(AR_GetHashCount(lfu->table) >= lfu->max_items)
        {
                __free_one_least_used(lfu);
        }

        if(lfu->copy_key_f)
        {
                status = lfu->copy_key_f(key, &new_key, lfu->usr_ctx);
        }else
        {
                new_key = key;
        }

        if(status != AR_S_YES)
        {
                goto INVALID_POINT;
        }

        if(lfu->copy_data_f)
        {
                status = lfu->copy_data_f(data, &new_data, lfu->usr_ctx);
        }else
        {
                new_data = data;
        }
        
        if(status != AR_S_YES)
        {
                goto INVALID_POINT;
        }
        
        val_node = create_value_node(new_data, NULL);
        
        AR_printf(L"%ls\r\n", ((std::wstring*)new_data)->c_str());
        
        if(val_node == NULL)
        {
                status = AR_E_NOMEM;
                goto INVALID_POINT;
        }
        
        status = AR_InsertToHash(lfu->table, new_key, (void*)val_node);

        if(status != AR_S_YES)
        {
                goto INVALID_POINT;
        }
        
        
        if(lfu->head ==  NULL || lfu->head->access_count != 1)
        {
                new_freq = create_freq_node(1);

                if(new_freq == NULL)
                {
                        status = AR_E_NOMEM;
                        goto INVALID_POINT;
                }
                
                val_node->parent = new_freq;
                
                status = pushback_key_to_freq_node(new_freq,new_key);
                
                if(status != AR_S_YES)
                {
                        goto INVALID_POINT;
                }
                
                if(lfu->head == NULL)
                {
                        AR_ASSERT(lfu->head == NULL && lfu->tail == NULL);
                        new_freq->prev = NULL;
                        new_freq->next = NULL;
                        
                        lfu->head = new_freq;
                        lfu->tail = new_freq;
                }else
                {
                        AR_ASSERT(lfu->head != NULL && lfu->tail != NULL);
                        
                        new_freq->prev = NULL;
                        new_freq->next = lfu->head->next;
                        if(lfu->head->next)
                        {
                                lfu->head->next->prev = new_freq;
                        }
                        
                        lfu->head = new_freq;
                }
        }else
        {
                status = pushback_key_to_freq_node(lfu->head,new_key);
                
                if(status != AR_S_YES)
                {
                        goto INVALID_POINT;
                }

				val_node->parent = lfu->head;
        }

        return status;
        

INVALID_POINT:
        
        if(new_freq)
        {
                destroy_freq_node(new_freq);
                new_freq = NULL;
        }
        
        if(new_key && lfu->destroy_key_f)
        {
                lfu->destroy_key_f(new_key, lfu->usr_ctx);
                new_key = NULL;
        }
        if(new_data && lfu->destroy_data_f)
        {
                lfu->destroy_data_f(new_data, lfu->usr_ctx);
                new_data = NULL;
        }
        
        if(val_node)
        {
                destroy_value_node(val_node);
                val_node = NULL;
        }
        
        return status;
}



arStatus_t      Cache_AccessFromLFU(cacheLFU_t *lfu, void *key, void **pdata)
{
        arStatus_t status;
        cacheValueNode_t *val_node;
        
        AR_ASSERT(lfu != NULL && pdata != NULL);
        
        status = AR_FindFromHash(lfu->table, key, (void**)&val_node);
        
        if(status != AR_S_YES)
        {
                return status;
        }

		AR_ASSERT(val_node != NULL);
		*pdata = val_node->data;

        
INVALID_POINT:
                
        return status;
        
}

arStatus_t      Cache_DeleteFromLFU(cacheLFU_t *lfu, void *key);










#endif






AR_NAMESPACE_END










