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



/*
typedef uint_64_t		(*Cache_hash_func_t)(void *key);
typedef int_t			(*Cache_comp_func_t)(void *l, void *r);

typedef arStatus_t		(*Cache_copy_func_t)(void *data, void **pnew_data, void *ctx);
typedef void			(*Cache_destroy_func_t)(void *data, void *ctx);


typedef struct __cache_lfu_context_tag
{
        Cache_hash_func_t       hash_f;
        Cache_comp_func_t       comp_f;
        
        Cache_copy_func_t       copy_key_f;
        Cache_copy_func_t       copy_data_f;
        
        Cache_destroy_func_t    destroy_key_f;
        Cache_destroy_func_t    destroy_data_f;
        void    *usr_ctx;
}cacheLFUCtx_t;
 */


struct __cache_key_node_tag;
typedef struct __cache_key_node_tag    cacheKeyNode_t;
struct __cache_key_node_tag
{
        void            *key;
        cacheKeyNode_t  *next;
};

static cacheKeyNode_t* create_keynode()
{
        return AR_NEW0(cacheKeyNode_t);
}

static void destroy_keynode(cacheKeyNode_t *node)
{
        AR_ASSERT(node);
        AR_DEL(node);
        node = NULL;
}


struct __cache_freq_node_tag;
typedef struct __cache_freq_node_tag    cacheFreqNode_t;


struct __cache_freq_node_tag
{
        size_t  access_count;
        
        
        cacheFreqNode_t *prev;
        cacheFreqNode_t *next;
        
        cacheKeyNode_t *key_head;
        cacheKeyNode_t *key_tail;
        size_t          key_cnt;
};

static cacheFreqNode_t*       create_freq_node(size_t access_cnt)
{
        cacheFreqNode_t *node; 
        AR_ASSERT(access_cnt > 0);
        
        node = AR_NEW0(cacheFreqNode_t);
        
        if(node == NULL)
        {
                return NULL;
        }
        
        node->access_count = access_cnt;
        return node;
}

static void destroy_freq_node(cacheFreqNode_t *node)
{
        cacheKeyNode_t *curr,*tmp;
        AR_ASSERT(node != NULL);
        
        curr = node->key_head;
        while(curr)
        {
                tmp = curr->next;
                AR_DEL(curr);
                curr = tmp;
        }
        AR_DEL(node);
        node = NULL;
}



static arStatus_t pushback_key_to_freq_node(cacheFreqNode_t *node, void *key)
{
        cacheKeyNode_t  *key_node;
        AR_ASSERT(node != NULL);
        
        key_node = create_keynode();
        if(key_node == NULL)
        {
                return AR_E_NOMEM;
        }
        
        
        key_node->key = key;

        if(node->key_cnt == 0)
        {
                AR_ASSERT(node->key_head == NULL && node->key_tail == NULL);
                node->key_head = node->key_tail = key_node;
        }else
        {
                AR_ASSERT(node->key_head != NULL && node->key_tail != NULL);
                node->key_tail->next = key_node;
                node->key_tail = key_node;
                key_node->next = NULL;
        }
        node->key_cnt++;
        return AR_S_YES;
}

static arStatus_t popfron_from_freq_node(cacheFreqNode_t *node, void **pkey)
{
        cacheKeyNode_t *tmp;
        AR_ASSERT(node != NULL && pkey != NULL);

        if(node->key_cnt == 0)
        {
                AR_ASSERT(node->key_head == NULL && node->key_tail == NULL);
                return AR_E_NOMORE;
        }
        
        tmp = node->key_head;
        
        node->key_head = tmp->next;
        
        if(tmp == node->key_tail)
        {
                AR_ASSERT(node->key_head == NULL);
                AR_ASSERT(node->key_cnt == 1);
                node->key_head = node->key_tail = NULL;
        }
        
        node->key_cnt--;
        
        *pkey = tmp->key;
        
        destroy_keynode(tmp);
        node = NULL;
        
        return AR_S_YES;
}

static arStatus_t remove_from_freq_node(cacheFreqNode_t *node, void *key)
{
        cacheKeyNode_t *curr, *prev;
        AR_ASSERT(node != NULL);
        
        curr = node->key_head;
        prev = NULL;
        
        while(curr)
        {
                if(curr->key == key)
                {
                        break;
                }
                prev = curr;
                curr = curr->next;
        }
        
        if(curr == NULL)
        {
                return AR_E_NOTFOUND;
        }
        
        if(prev == NULL)
        {
                AR_ASSERT(curr == node->key_head);
                node->key_head = curr->next;

                if(curr == node->key_tail)
                {
                        AR_ASSERT(node->key_cnt == 1);
                        node->key_head = node->key_tail = NULL;
                }
                
        }else
        {
                prev->next = curr->next;
                
                if(curr == node->key_tail)
                {
                        node->key_tail = prev;
                }
        }
        
        --node->key_cnt;
        
        return AR_S_YES;
        
}

typedef struct __cahce_value_node_tag
{
        void                    *data;
        cacheFreqNode_t         *parent;
}cacheValueNode_t;

static cacheValueNode_t* create_value_node(void *data, cacheFreqNode_t *parent)
{
        cacheValueNode_t *node;

        node = AR_NEW(cacheValueNode_t);
        if(node == NULL)
        {
                return NULL;
        }
        node->data = data;
        node->parent = parent;
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
        
        cacheFreqNode_t         *head;
        cacheFreqNode_t         *tail;
        
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
        val_node = (cacheValueNode_t*)val_node;
        
        AR_printf(L"data == %ls\r\n", ((std::wstring*)(val_node->data))->c_str());
        
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
                
}

arStatus_t      Cache_ClearLFU(cacheLFU_t *lfu)
{
        AR_ASSERT(lfu != NULL);
        return AR_E_FAIL;
}


static void __free_one_least_used(cacheLFU_t *lfu)
{
        void *key;
        
        AR_ASSERT(lfu != NULL);
        
        if(lfu->head == NULL)
        {
                AR_ASSERT(lfu->tail == NULL);
                AR_ASSERT(AR_GetHashCount(lfu->table) == 0);
                return;
        }
        
        if(popfron_from_freq_node(lfu->head, &key) != AR_S_YES)
        {
                AR_ASSERT(false);
        }
        
        {
                std::wstring *key_wcs = (std::wstring*)key;
                AR_printf(L"%ls\r\n", key_wcs->c_str());
        }
        if(AR_RemoveFromHash(lfu->table, key) != AR_S_YES)
        {
                AR_ASSERT(false);
        }
        
        if(lfu->head->key_cnt == 0)
        {
                cacheFreqNode_t *tmp;
                AR_ASSERT(lfu->head->key_head == NULL && lfu->head->key_tail == NULL);
                tmp = lfu->head;
                lfu->head = lfu->head->next;

                if(lfu->head == NULL)
                {
                        lfu->head = lfu->tail = NULL;
                        AR_ASSERT(AR_GetHashCount(lfu->table) == 0);
                }else
                {
                        if(lfu->head->next)
                        {
                                lfu->head->next->prev = NULL;
                        }
                }
                destroy_freq_node(tmp);
                tmp = NULL;
        }
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
        
        cacheFreqNode_t         *new_freq;
        cacheValueNode_t        *val_node;
        void                    *new_key, *new_data;
        AR_ASSERT(lfu != NULL);
        
        val_node = NULL;
        new_freq = NULL;
        new_key = NULL;
        new_data = NULL;
        
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
        cacheFreqNode_t *freq, *next_freq;
        AR_ASSERT(lfu != NULL && pdata != NULL);
        
        status = AR_FindFromHash(lfu->table, key, (void**)&val_node);
        
        if(status != AR_S_YES)
        {
                return status;
        }
        
        
        AR_ASSERT(val_node != NULL);
        freq = val_node->parent;
        AR_ASSERT(freq != NULL);
        next_freq = freq->next;
        
        if(next_freq == NULL || next_freq->access_count != freq->access_count + 1)
        {
                next_freq = create_freq_node(freq->access_count + 1);

                if(next_freq == NULL)
                {
                        status = AR_E_NOMEM;
                        goto INVALID_POINT;
                }
                        
                status = pushback_key_to_freq_node(next_freq, key);

                if(status != AR_S_YES)
                {
                        destroy_freq_node(next_freq);
                        next_freq = NULL;
                        goto INVALID_POINT;
                }

                next_freq->prev = freq;
                next_freq->next = freq->next;
                
                freq->next = next_freq;
                
                if(next_freq->next)
                {
                        next_freq->next->prev = next_freq;
                }
        }else
        {
                status = pushback_key_to_freq_node(next_freq, key);
                if(status != AR_S_YES)
                {
                        goto INVALID_POINT;
                }
        }
        
        val_node->parent = next_freq;
        
        if(remove_from_freq_node(freq, key) != AR_S_YES)
        {
                AR_ASSERT(false);
        }
        
        if(freq->key_cnt == 0)
        {
                if(freq == lfu->head)
                {
                        lfu->head = freq->next;
                }
                
                if(freq == lfu->tail)
                {
                        lfu->tail = freq->prev;
                }
                
                if(freq->prev)
                {
                        freq->prev->next = freq->next;
                }
                
                if(freq->next)
                {
                        freq->next->prev = freq->prev;
                }
                
                destroy_freq_node(freq);
        }
        
        return AR_S_YES;
        
INVALID_POINT:
                
        return status;
        
}

arStatus_t      Cache_DeleteFromLFU(cacheLFU_t *lfu, void *key);

















AR_NAMESPACE_END










