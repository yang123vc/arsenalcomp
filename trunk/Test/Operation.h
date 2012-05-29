//
//  Operation.h
//  Cloud
//
//  Created by SolidusSnake on 12-5-11.
//  Copyright (c) 2012年 none. All rights reserved.
//

#ifndef Cloud_Operation_h
#define Cloud_Operation_h

#include "test.h"

#define CLD_EXTERN_C_BEGIN
#define CLD_EXTERN_C_END


CLD_EXTERN_C_BEGIN

using namespace ARSpace;

#define CLOUD_MAX_OPERATION_POOL_SIZE   4
#define CLOUD_OPERATION_POLL_TIMEOUT	1000

arStatus_t      Operation_Init();
void            Operation_UnInit();



typedef void*	(*cldOperationFunc_t)(void *usr_ctx);
typedef void	(*cldOperationDestroyResultFunc_t)(void *result, void *usr_ctx);/*此函数在销毁operation时，如果operation为finished并且结果未被取走时执行*/

struct __cloud_operation_tag;
typedef struct __cloud_operation_tag	cldOperation_t;

cldOperation_t*	Cloud_CreateOperation(cldOperationFunc_t func, cldOperationDestroyResultFunc_t destroy_func, void *usr_ctx);
void			Cloud_DestroyOperation(cldOperation_t *operation);

arStatus_t		Cloud_StartOperation(cldOperation_t *operation);
arStatus_t		Cloud_CancelOperation(cldOperation_t *operation);

arStatus_t		Cloud_OperationIsReady(cldOperation_t *operation);
arStatus_t		Cloud_OperationIsExecuting(cldOperation_t *operation);
arStatus_t		Cloud_OperationIsCancelled(cldOperation_t *operation);
arStatus_t		Cloud_OperationIsFinished(cldOperation_t *operation);

arStatus_t		Cloud_GetOperationResult(cldOperation_t *operation, void **presult);
arStatus_t		Cloud_GetOperationResultWithTimeout(cldOperation_t *operation, void **presult, uint_64_t timeout_ms);



/*********************************************************test*********************************/



typedef struct __cloud_operation_pool_tag
{
        arThread_t      *thd[CLOUD_MAX_OPERATION_POOL_SIZE+1];
        size_t          thread_cnt;
        arAsyncQueue_t  async_que;
        arSpinLock_t    mutex;
}cldOperationPool_t;


cldOperationPool_t*     Cloud_CreateOperationPool(size_t init_thread_cnt);
void                    Cloud_DestroyOperationPool(cldOperationPool_t *pool);
arStatus_t              Cloud_IncreaseOperationPoolThread(cldOperationPool_t *pool);
arStatus_t              Cloud_OperationPoolHasIdleThread(cldOperationPool_t *pool);
arStatus_t				Cloud_PostToOperationPool(cldOperationPool_t *pool, cldOperation_t *operation);
size_t					Cloud_GetOperationPoolThreadCount(cldOperationPool_t *pool);

CLD_EXTERN_C_END

#endif
