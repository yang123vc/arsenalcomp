//
//  Operation.c
//  Cloud
//
//  Created by SolidusSnake on 12-5-11.
//  Copyright (c) 2012年 none. All rights reserved.
//

#if defined(__LIB)

#include "operation.h"


CLD_EXTERN_C_BEGIN

static arStatus_t Operation_Run(operation_t *operation);



/*********************************************************************************************************************/



operationPool_t*     Operation_CreatePool(size_t init_thread_cnt)
{
        size_t i;
        bool_t que_init, mutex_is_init;
        operationPool_t *pool;
        
        
		if(init_thread_cnt > CLOUD_MAX_OPERATION_POOL_SIZE)
        {
                init_thread_cnt = CLOUD_MAX_OPERATION_POOL_SIZE;
        }
        
        
        que_init = false;
        mutex_is_init = false;
        
        pool = AR_NEW0(operationPool_t);
        
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
                if(Operation_IncreasePoolThread(pool) != AR_S_YES)
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



void    Operation_DestroyPool(operationPool_t *pool)
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
                operation_t *operation = NULL;
                
				status = AR_GetFromAsyncQueueWithTimeout(q, (void**)&operation, CLOUD_OPERATION_POLL_TIMEOUT);
                
				if(status == AR_S_YES)
				{
                        if(operation == NULL)
                        {
                                AR_error(AR_ERR_MESSAGE,L"Thread exit!\r\n");
                                break;
                        }else
                        {
								Operation_Run(operation);
                        }
				}else if(status == AR_E_TIMEOUT)
				{
						AR_error(AR_ERR_MESSAGE,L"Timeout\r\n");
						continue;
				}else
				{
                        AR_error(AR_ERR_WARNING,L"Thread terminated : %d\r\n", AR_GET_STATUS(status));
                        break;
				}
		}
}



arStatus_t              Operation_IncreasePoolThread(operationPool_t *pool)
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



arStatus_t              Operation_PoolHasIdleThread(operationPool_t *pool)
{
        
        AR_ASSERT(pool != NULL);
        return AR_HasIdleThreadInAsyncQueue(&pool->async_que);
}

size_t					Operation_GetPoolThreadCount(operationPool_t *pool)
{
		size_t cnt;
		AR_ASSERT(pool != NULL);
        
		AR_LockSpinLock(&pool->mutex);
		cnt = pool->thread_cnt;
		AR_UnLockSpinLock(&pool->mutex);
		return cnt;
}

arStatus_t				Operation_PostToPool(operationPool_t *pool, operation_t *operation)
{
		AR_ASSERT(pool != NULL && operation != NULL);
		
		if(Operation_PoolHasIdleThread(pool) == AR_S_NO && Operation_GetPoolThreadCount(pool) < CLOUD_MAX_OPERATION_POOL_SIZE)
		{
				if(Operation_IncreasePoolThread(pool) != AR_S_YES)
				{
						AR_error(AR_ERR_WARNING, L"increase thread failed in Operation Pool!\r\n");
				}
		}
		
		return AR_PutToAsyncQueue(&pool->async_que, (void*)operation);
}


/*******************************************Operation********************************/


static operationPool_t		*__g_pool = NULL;

arStatus_t      Operation_Init()
{
        __g_pool = Operation_CreatePool(0);
        
		if(__g_pool == NULL)
		{
				AR_error(AR_ERR_WARNING,L"failed to create operation pool!\r\n");
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
				Operation_DestroyPool(__g_pool);
				__g_pool = NULL;
		}
}


typedef enum
{
		CLOUD_OPER_READY,
		CLOUD_OPER_EXECUTING,
		CLOUD_OPER_CANCELLED,
		CLOUD_OPER_FINISHED,
}cldOperationState_t;

struct __operation_tag
{
		arSpinLock_t			mutex;
		arEvent_t				*done_event;
		
		bool_t					has_started;
		bool_t					has_result;
		cldOperationState_t		state;
		
		
        
		void					*usr_ctx;
		operationFunc_t		func;
		operationDestroyResultFunc_t	destroy_result_func;
		void					*result;
};

operation_t*	Operation_Create(operationFunc_t func, operationDestroyResultFunc_t destroy_func, void *usr_ctx)
{
		operation_t *oper;
		AR_ASSERT(func != NULL);
        
		oper = AR_NEW0(operation_t);
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

void			Operation_Destroy(operation_t *operation)
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
				Operation_Cancel(operation);
		}
        
        
		if(need_cancel)
		{
				if(AR_WaitEvent(operation->done_event) != AR_S_YES)
				{
						AR_error(AR_ERR_FATAL, L"destroy operation object failed\r\n");
				}
                
                AR_LockSpinLock(&operation->mutex);
                
                if(operation->has_result && operation->destroy_result_func != NULL)
                {
                        AR_UnLockSpinLock(&operation->mutex);
                        
                        if(Operation_GetResult(operation, &result) == AR_S_YES)
                        {
                                AR_LockSpinLock(&operation->mutex);
                                operation->destroy_result_func(result, operation->usr_ctx);
                                AR_UnLockSpinLock(&operation->mutex);
                        }
                }else
                {
                        AR_UnLockSpinLock(&operation->mutex);
                }
		}else
		{
				AR_ASSERT(operation->result == NULL);
		}
        
		
/*        
        AR_ASSERT(!operation->has_result);
		AR_ASSERT(operation->result == NULL);
*/
		AR_DestroyEvent(operation->done_event);
		operation->done_event = NULL;
        
		AR_UnInitSpinLock(&operation->mutex);
        
		AR_ASSERT(need_cancel ? (operation->state == CLOUD_OPER_FINISHED || operation->state == CLOUD_OPER_CANCELLED) : true);
        
		AR_DEL(operation);
		operation = NULL;
}


static arStatus_t Operation_Run(operation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);

		AR_error(AR_ERR_MESSAGE,L"enter function %hs\r\n", AR_FUNC_NAME);
        
		status = AR_S_YES;
		
		AR_LockSpinLock(&oper->mutex);
		
		if(oper->state != CLOUD_OPER_READY)
		{
				status = AR_S_NO;
                oper->has_result = false;
		}else
		{
				oper->state = CLOUD_OPER_EXECUTING;
		}
        
		if(status != AR_S_YES)
		{
				AR_ASSERT(oper->state == CLOUD_OPER_CANCELLED);
                AR_UnLockSpinLock(&oper->mutex);
                
				if(AR_SetEvent(oper->done_event) != AR_S_YES)
				{
						AR_error(AR_ERR_FATAL, L"run operation failed, app terminated\r\n");
				}
                
				return status;
		}else
        {
                AR_UnLockSpinLock(&oper->mutex);
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
        }
        
		return AR_S_YES;
}




arStatus_t		Operation_Start(operation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);
		AR_ASSERT(__g_pool != NULL);
        
		if(Operation_IsReady(oper) == AR_S_YES)
		{
				AR_LockSpinLock(&oper->mutex);
				status = Operation_PostToPool(__g_pool, oper);
                
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



arStatus_t		Operation_Cancel(operation_t *oper)
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


arStatus_t		Operation_IsReady(operation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);
        
		AR_LockSpinLock(&oper->mutex);
        
		status = oper->state == CLOUD_OPER_READY ? AR_S_YES : AR_S_NO;
        
		AR_UnLockSpinLock(&oper->mutex);
        
		return status;
}


arStatus_t		Operation_IsExecuting(operation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);
        
		AR_LockSpinLock(&oper->mutex);
        
		status = oper->state == CLOUD_OPER_EXECUTING ? AR_S_YES : AR_S_NO;
        
		AR_UnLockSpinLock(&oper->mutex);
        
		return status;
}

arStatus_t		Operation_IsCancelled(operation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);
        
		AR_LockSpinLock(&oper->mutex);
        
		status = oper->state == CLOUD_OPER_CANCELLED ? AR_S_YES : AR_S_NO;
        
		AR_UnLockSpinLock(&oper->mutex);
        
		return status;
}

arStatus_t		Operation_IsFinished(operation_t *oper)
{
		arStatus_t status;
		AR_ASSERT(oper != NULL);
        
		AR_LockSpinLock(&oper->mutex);
        
		status = oper->state == CLOUD_OPER_FINISHED ? AR_S_YES : AR_S_NO;
        
		AR_UnLockSpinLock(&oper->mutex);
        
		return status;
}





arStatus_t		Operation_GetResult(operation_t *oper, void **presult)
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
                        if(oper->has_result)
                        {
                                *presult = oper->result;
                                oper->result = NULL;
                                oper->has_result = false;
                                status = AR_S_YES;
                        }else
                        {
                                status = AR_E_NOTFOUND;
                        }
                        break;
                default:
                        AR_ASSERT(false);
                        AR_error(AR_ERR_FATAL, L"internal error\r\n");
		}
		
		AR_UnLockSpinLock(&oper->mutex);
        
		return status;
}


arStatus_t		Operation_GetResultWithTimeout(operation_t *oper, void **presult, uint_64_t timeout_ms)
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
                        if(oper->has_result)
                        {
                                *presult = oper->result;
                                oper->result = NULL;
                                oper->has_result = false;
                                status = AR_S_YES;
                        }else
                        {
                                status = AR_E_EMPTY;
                        }
                        break;
                default:
                        AR_ASSERT(false);
                        AR_error(AR_ERR_FATAL, L"internal error\r\n");
		}
		
		AR_UnLockSpinLock(&oper->mutex);
        
		return status;
}







CLD_EXTERN_C_END

#endif


