//
//  Operation.h
//  Cloud
//
//  Created by SolidusSnake on 12-5-11.
//  Copyright (c) 2012年 none. All rights reserved.
//

#ifndef Cloud_Operation_h
#define Cloud_Operation_h

//#include "Cloud.h"

#include "./test.h"

#define CLD_EXTERN_C_BEGIN		AR_NAMESPACE_BEGIN
#define CLD_EXTERN_C_END		AR_NAMESPACE_END


CLD_EXTERN_C_BEGIN

#define CLOUD_MAX_OPERATION_POOL_SIZE   3
#define CLOUD_OPERATION_POLL_TIMEOUT	1000

arStatus_t      Operation_Init();
void            Operation_UnInit();






typedef void*	(*operationFunc_t)(void *usr_ctx);
typedef void	(*operationDestroyResultFunc_t)(void *result, void *usr_ctx);/*此函数在销毁operation时，如果operation为finished并且结果未被取走时执行*/

struct __operation_tag;
typedef struct __operation_tag	operation_t;

operation_t*	Operation_Create(operationFunc_t func, operationDestroyResultFunc_t destroy_func, void *usr_ctx);
void			Operation_Destroy(operation_t *operation);

arStatus_t		Operation_Start(operation_t *operation);
arStatus_t		Operation_Cancel(operation_t *operation);

arStatus_t		Operation_IsReady(operation_t *operation);
arStatus_t		Operation_IsExecuting(operation_t *operation);
arStatus_t		Operation_IsCancelled(operation_t *operation);
arStatus_t		Operation_IsFinished(operation_t *operation);

arStatus_t		Operation_GetResult(operation_t *operation, void **presult);
arStatus_t		Operation_GetResultWithTimeout(operation_t *operation, void **presult, uint_64_t timeout_ms);





typedef struct __operation_pool_tag
{
        arThread_t      *thd[CLOUD_MAX_OPERATION_POOL_SIZE+1];
        size_t          thread_cnt;
        arAsyncQueue_t  async_que;
        arSpinLock_t    mutex;
}operationPool_t;


operationPool_t*		Operation_CreatePool(size_t init_thread_cnt);
void                    Operation_DestroyPool(operationPool_t *pool);
arStatus_t              Operation_IncreasePoolThread(operationPool_t *pool);
arStatus_t              Operation_PoolHasIdleThread(operationPool_t *pool);
arStatus_t				Operation_PostToPool(operationPool_t *pool, operation_t *operation);
size_t					Operation_GetPoolThreadCount(operationPool_t *pool);


CLD_EXTERN_C_END

#endif
