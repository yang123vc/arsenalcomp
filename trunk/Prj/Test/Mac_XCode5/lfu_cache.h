//
//  lfu_cache.h
//  Test
//
//  Created by Solidus on 12-5-22.
//  Copyright (c) 2012年 none. All rights reserved.
//

#ifndef Test_lfu_cache_h
#define Test_lfu_cache_h

#include "Arsenal.h"



AR_NAMESPACE_BEGIN

struct __cache_lfu_tag;
typedef struct __cache_lfu_tag  cacheLFU_t;






typedef ar_uint_64_t		(*Cache_hash_func_t)(void *key, void *usr_ctx);
typedef ar_int_t			(*Cache_comp_func_t)(void *l, void *r, void *usr_ctx);

typedef arStatus_t		(*Cache_copy_func_t)(void *data, void **pnew_data, void *usr_ctx);
typedef void			(*Cache_destroy_func_t)(void *data, void *usr_ctx);

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


cacheLFU_t*     Cache_CreateLFU(cacheLFUCtx_t *ctx, size_t max_items_cnt);
void            Cache_DestroyLFU(cacheLFU_t *lfu);

arStatus_t      Cache_InsertToLFU(cacheLFU_t *lfu, void *key, void *data);
arStatus_t      Cache_AccessFromLFU(cacheLFU_t *lfu, void *key, void **pdata);
arStatus_t      Cache_DeleteFromLFU(cacheLFU_t *lfu, void *key);
arStatus_t      Cache_ClearLFU(cacheLFU_t *lfu);



AR_NAMESPACE_END

#endif
