//
//  Operation.c
//  Cloud
//
//  Created by SolidusSnake on 12-5-11.
//  Copyright (c) 2012年 none. All rights reserved.
//

#include "Operation.h"

CLD_EXTERN_C_BEGIN

static arStatus_t Cloud_RunOperation(cldOperation_t *operation);



/*********************************************************************************************************************/



cldOperationPool_t*     Cloud_CreateOperationPool(size_t init_thread_cnt)
{
        size_t i;
        bool_t que_init, mutex_is_init;
        cldOperationPool_t *pool;
        
        
		if(init_thread_cnt > CLOUD_MAX_OPERATION_POOL_SIZE)
        {
                init_thread_cnt = CLOUD_MAX_OPERATION_POOL_SIZE;
        }
        
        
        que_init = false;
        mutex_is_init = false;
        
        pool = AR_NEW0(cldOperationPool_t);
        
        if(pool == NULL)
        {
                goto INVALID_POINT;
        }
        
        AR_InitSpinLock(&pool->mutex);
        mutex_is_init = true;
        AR_InitAsyncQueue(&pool->async_que);
        que_init = true;
        
        
        for(i = 0; i < init_thread_cnt; ++i)
        {
                if(Cloud_IncreaseOperationPoolThread(pool) != AR_S_YES)
                {
                        goto INVALID_POINT;
                }
        }
        
        
        
        
        return pool;
INVALID_POINT:
        
        if(pool != NULL && pool->thread_cnt > 0)
        {
                AR_ASSERT(que_init);
         
                for(i = 0; i < pool->thread_cnt; ++i)
                {
                        if(AR_PutToAsyncQueue(&pool->async_que, NULL) != AR_S_YES)      /*这里出现错误就无法恢复也没必要恢复了，应用程序直接挂掉！！*/
                        {
                                AR_error(AR_ERR_FATAL, L"create operation pool failed!\r\n");
                        }
                        
                        AR_ASSERT(pool->thd[i] != NULL);
                        if(AR_JoinThread(pool->thd[i]) != AR_S_YES)     /*同上*/
                        {
                                AR_error(AR_ERR_FATAL, L"create operation pool failed!\r\n");
                        }
                        
                        AR_DestroyThread(pool->thd[i]);
                        pool->thd[i] = NULL;
                }
                
                pool->thread_cnt = 0;
        }
        
        if(pool != NULL && que_init)
        {
                AR_UnInitAsyncQueue(&pool->async_que);
        }

        if(pool != NULL && mutex_is_init)
        {
                AR_UnInitSpinLock(&pool->mutex);
        }
        
        
        if(pool != NULL)
        {
                AR_DEL(pool);
                pool = NULL;
        }
        
        return NULL;
}



void    Cloud_DestroyOperationPool(cldOperationPool_t *pool)
{
        size_t i;
        AR_ASSERT(pool != NULL);

		AR_ASSERT(AR_AsyncQueueIsEmpty(&pool->async_que) == AR_S_YES);

		for(i = 0; i < pool->thread_cnt; ++i)
		{
				if(AR_PutToAsyncQueue(&pool->async_que, NULL) != AR_S_YES)      /*这里出现错误就无法恢复也没必要恢复了，应用程序直接挂掉！！*/
                {
                        AR_error(AR_ERR_FATAL, L"destroy operation pool failed!\r\n");
                }
		}

        for(i = 0; i < pool->thread_cnt; ++i)
        {
                AR_ASSERT(pool->thd[i] != NULL);
                if(AR_JoinThread(pool->thd[i]) != AR_S_YES)     /*同上*/
                {
                        AR_error(AR_ERR_FATAL, L"create operation pool failed!\r\n");
                }
                
                AR_DestroyThread(pool->thd[i]);
                pool->thd[i] = NULL;
        }

        pool->thread_cnt = 0;
        
		AR_ASSERT(AR_AsyncQueueIsEmpty(&pool->async_que) == AR_S_YES);
        AR_UnInitAsyncQueue(&pool->async_que);
        
        AR_UnInitSpinLock(&pool->mutex);

        AR_DEL(pool);
        pool = NULL;
        
}




static void	__thread_worker(void *data)
{
        arAsyncQueue_t *q = (arAsyncQueue_t*)data;
        AR_ASSERT(q != NULL);
        
		while(true)
		{
                arStatus_t status;
                cldOperation_t *operation = NULL;
                
				status = AR_GetFromAsyncQueueWithTimeout(q, (void**)&operation, CLOUD_OPERATION_POLL_TIMEOUT);
                
				if(status == AR_S_YES)
				{
                        if(operation == NULL)
                        {
                                AR_LOG(L"Thread exit!\r\n");
                                break;
                        }else
                        {
								Cloud_RunOperation(operation);
                        }
				}else if(status == AR_E_TIMEOUT)
				{
						AR_LOG(L"Timeout\r\n");
						continue;
				}else
				{
                        AR_LOG(L"Thread terminated : %d\r\n", AR_GET_STATUS(status));
                        break;
				}
		}
}



arStatus_t              Cloud_IncreaseOperationPoolThread(cldOperationPool_t *pool)
{
        arThread_t *thd;
        AR_ASSERT(pool != NULL);
        
        AR_LockSpinLock(&pool->mutex);
        
        if(pool->thread_cnt >= CLOUD_MAX_OPERATION_POOL_SIZE)
        {
                AR_UnLockSpinLock(&pool->mutex);
                return AR_E_RANGE;
        }
        
        thd = AR_CreateThread(__thread_worker, &pool->async_que);
        
        if(thd == NULL)
        {
                AR_UnLockSpinLock(&pool->mutex);
                return AR_E_SYS;
        }
        
        pool->thd[pool->thread_cnt] = thd;
        pool->thread_cnt++;
        
        AR_UnLockSpinLock(&pool->mutex);
        return AR_S_YES;
}



arStatus_t              Cloud_OperationPoolHasIdleThread(cldOperationPool_t *pool)
{
        
        AR_ASSERT(pool != NULL);
        return AR_HasIdleThreadInAsyncQueue(&pool->async_que);
}

size_t					Cloud_GetOperationPoolThreadCount(cldOperationPool_t *pool)
{
		size_t cnt;
		AR_ASSERT(pool != NULL);

		AR_LockSpinLock(&pool->mutex);
		cnt = pool->thread_cnt;
		AR_UnLockSpinLock(&pool->mutex);
		return cnt;
}

arStatus_t				Cloud_PostToOperationPool(cldOperationPool_t *pool, cldOperation_t *operation)
{
		AR_ASSERT(pool != NULL && operation != NULL);
		
		if(Cloud_OperationPoolHasIdleThread(pool) == AR_S_NO && Cloud_GetOperationPoolThreadCount(pool) < CLOUD_MAX_OPERATION_POOL_SIZE)
		{
				if(Cloud_IncreaseOperationPoolThread(pool) != AR_S_YES)
				{
						AR_LOG(L"increase thread failed in Operation Pool!\r\n");
				}
		}
		
		return AR_PutToAsyncQueue(&pool->async_que, (void*)operation);
}


/*******************************************Operation********************************/


static cldOperationPool_t		*__g_pool = NULL;

arStatus_t      Operation_Init()
{
        __g_pool = Cloud_CreateOperationPool(0);

		if(__g_pool == NULL)
		{
				AR_LOG(L"failed to create operation pool!\r\n");
				return AR_E_SYS;
		}else
		{
				return AR_S_YES;
		}
}


void            Operation_UnInit()
{
		if(__g_pool != NULL)
		{
				Cloud_DestroyOperationPool(__g_pool);
				__g_pool = NULL;
		}
}



/*
typedef void* (*cldOperationFunc_t)(void *usr_ctx);

struct __cloud_operation_tag;
typedef struct __cloud_operation_tag	cldOperation_t;

*/

typedef enum
{
		CLOUD_OPER_READY,
		CLOUD_OPER_EXECUTING,
		CLOUD_OPER_CANCELLED,
		CLOUD_OPER_FINISHED,
}cldOperationState_t;

struct __cloud_operation_tag
{
		arSpinLock_t			mutex;
		arEvent_t				*done_event;
		
		bool_t					has_started;
		bool_t					has_result;
		cldOperationState_t		state;
		
		

		void					*usr_ctx;
		cldOperationFunc_t		func;
		cldOperationDestroyResultFunc_t	destroy_result_func;
		void					*result;
};

cldOperation_t*	Cloud_CreateOperation(cldOperationFunc_t func, cldOperationDestroyResultFunc_t destroy_func, void *usr_ctx)
{
		cldOperation_t *oper;
		AR_ASSERT(func != NULL);

		oper = AR_NEW0(cldOperation_t);
		if(oper == NULL)
		{
				return NULL;
		}
		
		oper->done_event = AR_CreateEvent(false);
		if(oper->done_event == NULL)
		{
				AR_DEL(oper);
				oper = NULL;
				return NULL;
		}

		AR_InitSpinLock(&oper->mutex);

		oper->state = CLOUD_OPER_READY;
		oper->has_started = false;
		oper->has_result = false;

		oper->func = func;
		oper->destroy_result_func = destroy_func;
		oper->usr_ctx = usr_ctx;
		oper->result = NULL;

		return oper;

}

void			Cloud_DestroyOperation(cldOperation_t *operation)
{
		void *result;
		bool_t need_cancel;
		AR_ASSERT(operation != NULL);
		result = NULL;
		need_cancel = false;

		AR_LockSpinLock(&operation->mutex);
		
		if(operation->has_started)
		{
				need_cancel = true;
		}

		AR_UnLockSpinLock(&operation->mutex);

		if(need_cancel)
		{
				Cloud_CancelOperation(operation);
		}


		if(need_cancel)
		{
				if(AR_WaitEvent(operation->done_event) != AR_S_YES)
				{
						AR_error(AR_ERR_FATAL, L"destroy operation object failed\r\n");
				}

				if(operation->has_result && operation->destroy_result_func && Cloud_GetOperationResult(operation, &result) == AR_S_YES)
				{
						operation->destroy_result_func(result, operation->usr_ctx);
				}
		}else
		{
				AR_ASSERT(operation->result == NULL);
		}

		

		AR_ASSERT(!operation->has_result);
		AR_ASSERT(operation->result == NULL);
		
		AR_DestroyEvent(operation->done_event);
		operation->done_event = NULL;

		AR_UnInitSpinLock(&operation->mutex);

		AR_ASSERT(need_cancel ? (operation->state == CLOUD_OPER_FINISHED || operation->state == CLOUD_OPER_CANCELLED) : true);

		AR_DEL(operation);
		operation = NULL;
}





arStatus_t		Cloud_StartOperation(cldOperation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);
		AR_ASSERT(__g_pool != NULL);

		if(Cloud_OperationIsReady(oper) == AR_S_YES)
		{
				AR_LockSpinLock(&oper->mutex);
				status = Cloud_PostToOperationPool(__g_pool, oper);

				if(status == AR_S_YES)
				{
						oper->has_started = true;
				}

				AR_UnLockSpinLock(&oper->mutex);

		}else
		{
				status = AR_E_INVAL;
		}

		return status;
}



arStatus_t		Cloud_CancelOperation(cldOperation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);

		AR_LockSpinLock(&oper->mutex);

		if(oper->state == CLOUD_OPER_READY)
		{
				oper->state = CLOUD_OPER_CANCELLED;
				status = AR_S_YES;
		}else
		{
				status = AR_S_NO;
		}
		AR_UnLockSpinLock(&oper->mutex);

		return status;

}


arStatus_t		Cloud_OperationIsReady(cldOperation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);

		AR_LockSpinLock(&oper->mutex);

		status = oper->state == CLOUD_OPER_READY ? AR_S_YES : AR_S_NO;

		AR_UnLockSpinLock(&oper->mutex);

		return status;
}


arStatus_t		Cloud_OperationIsExecuting(cldOperation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);

		AR_LockSpinLock(&oper->mutex);

		status = oper->state == CLOUD_OPER_EXECUTING ? AR_S_YES : AR_S_NO;

		AR_UnLockSpinLock(&oper->mutex);

		return status;
}

arStatus_t		Cloud_OperationIsCancelled(cldOperation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);

		AR_LockSpinLock(&oper->mutex);

		status = oper->state == CLOUD_OPER_CANCELLED ? AR_S_YES : AR_S_NO;

		AR_UnLockSpinLock(&oper->mutex);

		return status;
}

arStatus_t		Cloud_OperationIsFinished(cldOperation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);

		AR_LockSpinLock(&oper->mutex);

		status = oper->state == CLOUD_OPER_FINISHED ? AR_S_YES : AR_S_NO;

		AR_UnLockSpinLock(&oper->mutex);

		return status;
}




static arStatus_t Cloud_RunOperation(cldOperation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);

		AR_LOG(L"on Cloud_RunOperation\r\n");
		
		status = AR_S_YES;
		
		AR_LockSpinLock(&oper->mutex);
		
		if(oper->state != CLOUD_OPER_READY)
		{
				status = AR_S_NO;
		}else
		{
				oper->state = CLOUD_OPER_EXECUTING;
		}

		AR_UnLockSpinLock(&oper->mutex);

		if(status != AR_S_YES)
		{
				AR_ASSERT(oper->state == CLOUD_OPER_CANCELLED);

				if(AR_SetEvent(oper->done_event) != AR_S_YES)
				{
						AR_error(AR_ERR_FATAL, L"run operation failed, app terminated\r\n");
				}

				return status;
		}
		
		oper->result = oper->func(oper->usr_ctx);

		AR_LockSpinLock(&oper->mutex);
		AR_ASSERT(oper->state == CLOUD_OPER_EXECUTING);
		oper->state = CLOUD_OPER_FINISHED;
		oper->has_result = true;
		AR_UnLockSpinLock(&oper->mutex);

		if(AR_SetEvent(oper->done_event) != AR_S_YES)
		{
				AR_error(AR_ERR_FATAL, L"run operation failed, app terminated\r\n");
		}

		return AR_S_YES;
}



arStatus_t		Cloud_GetOperationResult(cldOperation_t *oper, void **presult)
{
		arStatus_t status;
		
		AR_ASSERT(oper != NULL && presult != NULL);

		status = AR_S_YES;

		AR_LockSpinLock(&oper->mutex);

		if(oper->state == CLOUD_OPER_FINISHED && !oper->has_result)
		{
				status = AR_E_NOTFOUND;
		}

		AR_UnLockSpinLock(&oper->mutex);

		if(status != AR_S_YES)
		{
				return status;
		}


		status = AR_WaitEvent(oper->done_event);

		if(status != AR_S_YES)
		{
				return status;
		}

		AR_LockSpinLock(&oper->mutex);

		switch(oper->state)
		{
		case CLOUD_OPER_CANCELLED:
				status = AR_E_CANCELLED;
				break;
		case CLOUD_OPER_FINISHED:
				AR_ASSERT(oper->has_result);
				*presult = oper->result;
				oper->result = NULL;
				oper->has_result = false;
				status = AR_S_YES;
				break;
		default:
				AR_ASSERT(false);
				AR_error(AR_ERR_FATAL, L"internal error\r\n");
		}
		
		AR_UnLockSpinLock(&oper->mutex);

		return status;
}


arStatus_t		Cloud_GetOperationResultWithTimeout(cldOperation_t *oper, void **presult, uint_64_t timeout_ms)
{

		arStatus_t status;
		AR_ASSERT(oper != NULL && presult != NULL);

		status = AR_S_YES;

		AR_LockSpinLock(&oper->mutex);

		if(oper->state == CLOUD_OPER_FINISHED && !oper->has_result)
		{
				status = AR_E_NOTFOUND;
		}

		AR_UnLockSpinLock(&oper->mutex);

		if(status != AR_S_YES)
		{
				return status;
		}
		
		status = AR_WaitEventWithTimeout(oper->done_event, timeout_ms);

		if(status != AR_S_YES)
		{
				return status;
		}

		AR_LockSpinLock(&oper->mutex);

		switch(oper->state)
		{
		case CLOUD_OPER_CANCELLED:
				status = AR_E_CANCELLED;
				break;
		case CLOUD_OPER_FINISHED:
				AR_ASSERT(oper->has_result);
				*presult = oper->result;
				oper->result = NULL;
				oper->has_result = false;
				status = AR_S_YES;
				break;
		default:
				AR_ASSERT(false);
				AR_error(AR_ERR_FATAL, L"internal error\r\n");
		}
		
		AR_UnLockSpinLock(&oper->mutex);

		return status;
}








CLD_EXTERN_C_END
