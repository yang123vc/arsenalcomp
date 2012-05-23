//
//  lfu_cache.c
//  Test
//
//  Created by Solidus on 12-5-22.
//  Copyright (c) 2012年 none. All rights reserved.
//


#include "lfu_cache.h"



AR_NAMESPACE_BEGIN


typedef struct __cahce_value_node_tag
{
		void					*key;	/*之所以保留一份key是因为清理弹出时候减少遍历*/
        void                    *data;
		size_t					index_in_heap;
		size_t					access_count;
}cacheValueNode_t;


static cacheValueNode_t* create_value_node(void *key, void *data)
{
        cacheValueNode_t *node;

        node = AR_NEW(cacheValueNode_t);
        if(node == NULL)
        {
                return NULL;
        }
		node->key = key;
        node->data = data;
		node->access_count = 0;
        node->index_in_heap = 0;
        return node;
}


static void destroy_value_node(cacheValueNode_t *node)
{
        AR_ASSERT(node != NULL);
        AR_DEL(node);
        node = NULL;
}



/*
最小二叉堆，插入需要将新元素放入lfu->min_heap[item_count], lfu->item_count++;之后，从lfu->item_coutn开始fixup,
删除将lfu->min_heap[0]与lfu->min_heap[lfu->item_count-1]交换，lfu->item_count--;之后从lfu->min_heap[0]开始fixdown,
*/

static void __swap_min_heap(cacheValueNode_t **arr, size_t l, size_t r)
{
		AR_ASSERT(arr != NULL);
		if(l != r)
		{
				cacheValueNode_t *t = arr[l];
				arr[l] = arr[r];
				arr[r] = t;

				arr[l]->index_in_heap = l;
				arr[r]->index_in_heap = r;

				AR_ASSERT(arr[l]->index_in_heap != arr[r]->index_in_heap);
		}
}

static void __heap_fixup(cacheValueNode_t **arr, size_t count, size_t idx)
{
		size_t p;
		AR_ASSERT(idx < count);
		AR_UNUSED(count);
		
		while(idx > 0)
		{
				p = (idx - 1) / 2;

				AR_ASSERT(arr[p]->access_count > 0 && arr[idx]->access_count > 0);
				if(arr[p]->access_count <= arr[idx]->access_count)/*父节点小于等于子节点，则循环终止*/
				{
						break;
				}
				
				__swap_min_heap(arr, p, idx);/*否则交换数组及其索引*/

				idx = p;
		}
}

static void __heap_fixdown(cacheValueNode_t **arr, size_t count, size_t idx)
{
		size_t c;
		AR_ASSERT(arr != NULL);
		AR_ASSERT(idx < count);
		
		while(idx < count / 2)
		{

				c = idx * 2 + 1;

				/*
				比如此堆为
						8
				       / \
				      5   4
				fixdown需要将4和8交换，保证父小于子，因此需要对比两个子节点
				*/

				if(c < count - 1 && arr[c]->access_count > arr[c+1]->access_count)	
				{
						++c;
				}

				AR_ASSERT(arr[c]->access_count > 0 && arr[idx]->access_count > 0);

				if(arr[c]->access_count >= arr[idx]->access_count)
				{
						break;
				}
				
				__swap_min_heap(arr, idx, c);/*否则交换数组及其索引*/
				idx = c;
		}
}





static void __heap_remove_at(cacheValueNode_t **arr, size_t count, size_t idx)
{
		cacheValueNode_t *tmp;
		size_t i,p;
		AR_ASSERT(arr != NULL && idx < count);
		tmp = NULL;
		__swap_min_heap(arr, idx, count - 1);
		tmp = arr[count -1];
		count--;
		
		if(count == 0 || idx == count)
		{
				return;
		}

		i = idx;

		while(i > 0)
		{
				p = (i - 1) / 2;
				
				AR_ASSERT(arr[p]->access_count > 0 && arr[i]->access_count > 0);
				if(arr[p]->access_count <= arr[i]->access_count)
				{
						break;
				}
				
				__swap_min_heap(arr, p, i);/*否则交换数组及其索引*/
				
				i = p;
		}

		if(i == idx)
		{
				__heap_fixdown(arr, count, i);
		}

}

static bool_t __heap_verify(cacheValueNode_t **arr, size_t count)
{
		size_t i,l,r;
		AR_ASSERT(arr != NULL);
		i = 0;
		while(i < count / 2)
		{
				l = (2 * i) + 1;
				r = l + 1;

				if(arr[i]->access_count > arr[l]->access_count)
				{
						return false;
				}

				if(r < count && arr[i]->access_count > arr[r]->access_count)
				{
						return false;
				}
				
				i += 1;
		}

		
		return true;
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

		/*
		for(size_t i = 0; i < lfu->items_count; ++i)
		{
				AR_printf(L"access count == %Iu\r\n", lfu->min_heap[i]->access_count);
		}
		*/

		AR_ASSERT(__heap_verify(lfu->min_heap, lfu->items_count));

#if(0)
		{

				size_t prev_access_count = 0;

				while(lfu->items_count > 0)
				{
						cacheValueNode_t *tmp = lfu->min_heap[0];
						AR_ASSERT(tmp->access_count >= prev_access_count);
						__heap_remove_at(lfu->min_heap, lfu->items_count, 0);
						lfu->items_count--;
						/*
						__swap_min_heap(lfu->min_heap, 0, lfu->items_count-1);
						if(--lfu->items_count > 0)
						{
								__heap_fixdown(lfu->min_heap, lfu->items_count, 0);
						}
						*/
				}
		}
#endif
		
		lfu->items_count = 0;
		AR_memset(lfu->min_heap, 0, sizeof(lfu->min_heap[0]) * lfu->max_items);
		
		AR_ClearHash(lfu->table);
}





static void __free_one_least_used(cacheLFU_t *lfu)/*删除access_count 最小的一个*/
{
		arStatus_t status;
		cacheValueNode_t *val_node;
		AR_ASSERT(lfu != NULL);
		val_node = NULL;
		status = AR_S_YES;

		if(lfu->items_count == 0)
		{
				AR_ASSERT(AR_GetHashCount(lfu->table) == 0);
				return;
		}
		
		__swap_min_heap(lfu->min_heap, 0, lfu->items_count - 1);
		__heap_fixdown(lfu->min_heap, lfu->items_count-1, 0);
		
		val_node = lfu->min_heap[lfu->items_count - 1];
		status = AR_RemoveFromHash(lfu->table, val_node->key);
		AR_ASSERT(status == AR_S_YES);
		lfu->items_count--;
}



static arStatus_t     __replace_value_node(cacheLFU_t *lfu, cacheValueNode_t        *val_node, void *data)
{
        arStatus_t status;
        void *new_data;
        AR_ASSERT(lfu != NULL && val_node != NULL);
		
		status = AR_S_YES;
        new_data = NULL;

		if(lfu->copy_data_f)/*复制data*/
		{
				status = lfu->copy_data_f(data, &new_data, lfu->usr_ctx);
        
				if(status != AR_S_YES)
				{
						return status;
				}
		}else
		{
				new_data = data;
		}
        
        if(lfu->destroy_data_f)
        {
                lfu->destroy_data_f(val_node->data, lfu->usr_ctx);
                val_node->data = NULL;
        }else
		{
				val_node->data = NULL;
		}
        
		val_node->data = new_data;
        
        return status;
}


arStatus_t      Cache_InsertToLFU(cacheLFU_t *lfu, void *key, void *data)
{
        arStatus_t status;
        cacheValueNode_t        *val_node;
        void                    *new_key, *new_data;
        AR_ASSERT(lfu != NULL);
        
		status = AR_S_YES;
		new_key = NULL;
		new_data = NULL;
		val_node = NULL;

        status = AR_FindFromHash(lfu->table, key, (void**)&val_node);
        
        if(status == AR_S_YES)
        {
                return __replace_value_node(lfu, val_node, data);
        }

        while(AR_GetHashCount(lfu->table) >= lfu->max_items)
        {
				AR_ASSERT(AR_GetHashCount(lfu->table) == lfu->items_count);
                __free_one_least_used(lfu);
				AR_ASSERT(__heap_verify(lfu->min_heap, lfu->items_count));
        }
        
		status = AR_S_YES;

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
        
        val_node = create_value_node(new_key, new_data);
		
		if(val_node == NULL)
		{
                status = AR_E_NOMEM;
                goto INVALID_POINT;
        }
		
		val_node->access_count = 1;
        
		status = AR_InsertToHash(lfu->table, new_key, (void*)val_node);
        
		if(status != AR_S_YES)
        {
                goto INVALID_POINT;
        }

		AR_ASSERT(lfu->items_count == AR_GetHashCount(lfu->table) - 1);
		
		val_node->index_in_heap = lfu->items_count;
		lfu->min_heap[lfu->items_count] = val_node;
		__heap_fixup(lfu->min_heap, lfu->items_count + 1, val_node->index_in_heap);
		lfu->items_count++;
        
		AR_ASSERT(__heap_verify(lfu->min_heap, lfu->items_count));

		return status;
        

INVALID_POINT:
        AR_ASSERT(__heap_verify(lfu->min_heap, lfu->items_count));

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
		
		AR_ASSERT(val_node == lfu->min_heap[val_node->index_in_heap]);
		
		val_node->access_count++;
		__heap_fixdown(lfu->min_heap, lfu->items_count, val_node->index_in_heap);
        
		AR_ASSERT(__heap_verify(lfu->min_heap, lfu->items_count));

		return status;
}




arStatus_t      Cache_DeleteFromLFU(cacheLFU_t *lfu, void *key)
{
		arStatus_t status;
		cacheValueNode_t *val_node;
		AR_ASSERT(lfu != NULL);

		AR_ASSERT(AR_GetHashCount(lfu->table) == lfu->items_count);
		status = AR_FindFromHash(lfu->table, key, (void**)&val_node);

		if(status != AR_S_YES)
		{
				return status;
		}

		__heap_remove_at(lfu->min_heap, lfu->items_count, val_node->index_in_heap);
		lfu->items_count--;
		
		AR_ASSERT(__heap_verify(lfu->min_heap, lfu->items_count));

		status = AR_RemoveFromHash(lfu->table, key);

		AR_ASSERT(status == AR_S_YES);
		
		return status;
}














AR_NAMESPACE_END










