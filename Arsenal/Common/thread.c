/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
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


#if defined(OS_FAMILY_WINDOWS)

		#include "thread_win.h"

#elif defined(OS_FAMILY_UNIX)

        #include <sys/select.h>
        #include <sys/time.h>

        #if(OS_TYPE == OS_MAC_OS_X || OS_TYPE == OS_IOS)
                #include <libkern/OSAtomic.h>
        #endif


        #include "thread_posix.h"
#else

		#error "Target OS system not supported"

#endif

AR_NAMESPACE_BEGIN

ar_uint_64_t		AR_GetTime_Milliseconds()
{
		return AR_GetTime_Microseconds() / AR_BIGNUM_U64(1000);
}
















/******************************************AsyncQueue_t****************************/

struct __async_data_node_tag
{
		void							*data;
		struct __async_data_node_tag	*next;
};

struct async_wait_info_tag
{
		arEvent_t						*event;
		void							*data;
};

struct __async_wait_node_tag 
{
		asyncWaitInfo_t					*info;
		struct __async_wait_node_tag	*next;
};


/**************aux****************************/
static void __init_data_queue(arAsyncQueue_t *queue)
{
		AR_ASSERT(queue != NULL);
		queue->data_head = queue->data_tail = NULL;
		queue->data_cnt = 0;
}

static void* __pop_data(arAsyncQueue_t *queue)
{
		void *ret;
		asyncDataNode_t *node;

		AR_ASSERT(queue && queue->data_cnt > 0 && queue->data_head != NULL && queue->data_tail != NULL);

		node = queue->data_head; 
		queue->data_head = queue->data_head->next;
		queue->data_cnt--;

		if(queue->data_head == NULL)
		{
				queue->data_head = queue->data_tail = NULL;
				AR_ASSERT(queue->data_cnt == 0);
		}

		ret = node->data;
		AR_DEL(node);
		return ret;
}

static arStatus_t		__push_data(arAsyncQueue_t *queue, void *data)
{
		asyncDataNode_t *node;
		AR_ASSERT(queue != NULL);
		node = AR_NEW(asyncDataNode_t);
		if(node == NULL)
		{
				return AR_E_NOMEM;
		}

		node->data = data;
		node->next = NULL;
		if(queue->data_cnt == 0)
		{
				AR_ASSERT(queue->data_head == NULL && queue->data_tail == NULL);
				queue->data_head = queue->data_tail = node;
				
		}else
		{
				AR_ASSERT(queue->data_head != NULL && queue->data_tail != NULL);
				queue->data_tail->next = node;
				queue->data_tail = node;
		}
		queue->data_cnt++;
		return AR_S_YES;
}


static arStatus_t		__push_data_front(arAsyncQueue_t *queue, void *data)
{
		asyncDataNode_t *node;
		AR_ASSERT(queue != NULL);
		node = AR_NEW(asyncDataNode_t);
		if(node == NULL)
		{
				return AR_E_NOMEM;
		}

		node->data = data;
		node->next = NULL;

		if(queue->data_cnt == 0)
		{
				AR_ASSERT(queue->data_head == NULL && queue->data_tail == NULL);
				queue->data_head = queue->data_tail = node;
				
		}else
		{
				AR_ASSERT(queue->data_head != NULL && queue->data_tail != NULL);
				node->next = queue->data_head;
				queue->data_head = node;
		}
		queue->data_cnt++;
		return AR_S_YES;
}



static void __uninit_data_queue(arAsyncQueue_t *queue)
{
		while(queue->data_cnt)
		{
				__pop_data(queue);
		}
}







static void __init_wait_queue(arAsyncQueue_t *queue)
{
		AR_ASSERT(queue != NULL);
		queue->wait_head = queue->wait_tail = NULL;
		queue->wait_cnt = 0;
}

static asyncWaitInfo_t* __top_wait(arAsyncQueue_t *queue)
{
		asyncWaitNode_t *node;
		AR_ASSERT(queue && queue->wait_cnt > 0 && queue->wait_head != NULL && queue->wait_tail != NULL);

		node = queue->wait_head;
		return node->info;
}



static asyncWaitInfo_t* __pop_wait(arAsyncQueue_t *queue)
{
		
		asyncWaitNode_t *node;
		asyncWaitInfo_t *ret;
		AR_ASSERT(queue && queue->wait_cnt > 0 && queue->wait_head != NULL && queue->wait_tail != NULL);
		node = queue->wait_head; 
		queue->wait_head = queue->wait_head->next;
		queue->wait_cnt--;

		if(queue->wait_head == NULL)
		{
				queue->wait_head = queue->wait_tail = NULL;
				AR_ASSERT(queue->wait_cnt == 0);
		}
		ret = node->info;
		AR_DEL(node);
		return ret;
}



static arStatus_t		__push_wait(arAsyncQueue_t *queue, asyncWaitInfo_t *info)
{
		asyncWaitNode_t *node;
		AR_ASSERT(queue != NULL);
		AR_ASSERT(info != NULL && info->event != NULL);
		node = AR_NEW(asyncWaitNode_t);
		
		if(node == NULL)
		{
				return AR_E_NOMEM;
		}


		node->info = info;
		node->next = NULL;
		node->info->data = NULL;

		if(queue->wait_cnt == 0)
		{
				AR_ASSERT(queue->wait_head == NULL && queue->wait_tail == NULL);
				queue->wait_head = queue->wait_tail = node;
		}else
		{
				AR_ASSERT(queue->wait_head != NULL && queue->wait_tail != NULL);
				queue->wait_tail->next = node;
				queue->wait_tail = node;
		}

		queue->wait_cnt++;

		return AR_S_YES;
}


static void __uninit_wait_queue(arAsyncQueue_t *queue)
{
		while(queue->wait_cnt)
		{
				asyncWaitInfo_t *info;
				AR_ASSERT(false);/*不可能执行到此*/
				info = __pop_wait(queue);
				AR_ASSERT(info != NULL);
		}
}



static ar_bool_t __remove_wait_node(arAsyncQueue_t *queue, asyncWaitInfo_t *info)
{
		asyncWaitNode_t *curr, *prev;
		AR_ASSERT(queue != NULL && info != NULL);

		prev = NULL;
		for(curr = queue->wait_head; curr != NULL && curr->info != info; curr = curr->next)
		{
				prev = curr;
		}

		if(curr == NULL)
		{
				return false;
		}else
		{
				AR_ASSERT(curr->info == info && curr->info->event == info->event);
				
				if(prev == NULL)
				{
						AR_ASSERT(curr == queue->wait_head);
						queue->wait_head = queue->wait_head->next;
						queue->wait_cnt--;
						
						if(queue->wait_head == NULL)
						{
								AR_ASSERT(queue->wait_cnt == 0);
								queue->wait_head = queue->wait_tail = NULL;
						}
				}else
				{
						prev->next = curr->next;
						queue->wait_cnt--;

						if(curr == queue->wait_tail)
						{
								queue->wait_tail = prev;
						}
				}
				AR_DEL(curr);

				return true;
		}
}



/**************aux***************************end*/



void	AR_InitAsyncQueue(arAsyncQueue_t *queue)
{
		AR_ASSERT(queue != NULL);
		AR_memset(queue, 0, sizeof(*queue));
		__init_data_queue(queue);
		__init_wait_queue(queue);
		AR_InitSpinLock(&queue->mutex);
}



void	AR_UnInitAsyncQueue(arAsyncQueue_t *queue)
{
		AR_ASSERT(queue != NULL);
		
		AR_ASSERT(queue->wait_cnt == 0);
		__uninit_wait_queue(queue);
		__uninit_data_queue(queue);

		AR_UnInitSpinLock(&queue->mutex);
}



arStatus_t	AR_GetFromAsyncQueueWithTimeout(arAsyncQueue_t *queue, void **pdata, ar_uint_64_t	millisecond)
{
		arStatus_t res;
		AR_ASSERT(queue != NULL && pdata != NULL);
		AR_LockSpinLock(&queue->mutex);

		res = AR_S_YES;

		if(queue->data_cnt > 0)
		{
				*pdata = __pop_data(queue);
				AR_UnLockSpinLock(&queue->mutex);
				res = AR_S_YES;
		}else
		{

				arEvent_t		*event;
				asyncWaitInfo_t	info;

				event = NULL;
				event = AR_CreateEvent(false);
				if(event == NULL)
				{
						res = AR_E_SYS;
						AR_UnLockSpinLock(&queue->mutex);
						goto END_POINT;
				}

				
				info.event = event;
				info.data = NULL;

				res = __push_wait(queue, &info);

				if(res != AR_S_YES)
				{
						AR_UnLockSpinLock(&queue->mutex);
						goto END_POINT;
				}

				AR_UnLockSpinLock(&queue->mutex);
				
				res = AR_WaitEventWithTimeout(event, millisecond);
				
				if(res == AR_S_YES)
				{
						*pdata = info.data;
						res = AR_S_YES;
				}else if(res == AR_E_TIMEOUT)
				{
						AR_LockSpinLock(&queue->mutex);
						/*
						如果__remove_wait_node不成功，则证明在在本函数AR_WaitEventTimeout超时之后，
						AR_LockSpinLock(&queue->mutex);之前，有AR_PutToAsyncQueue获得锁且将info.data
						赋值
						*/
						if(__remove_wait_node(queue, &info))
						{
								res = AR_E_TIMEOUT;
						}else
						{
								res = AR_S_YES;
								*pdata = info.data;
						}
						AR_UnLockSpinLock(&queue->mutex);
				}else
				{
						/*如果不幸执行到此，最大的可能应该是setevent失败了，那么也许应该终止，这里不考虑此情况，继续试图删除掉已注册的waitinfo*/
						
						AR_LockSpinLock(&queue->mutex);

						__remove_wait_node(queue, &info);

						AR_UnLockSpinLock(&queue->mutex);

						goto END_POINT;
				}

END_POINT:
				if(event != NULL)
				{
						AR_DestroyEvent(event);
						event = NULL;
				}


		}

		return res;
}



arStatus_t	AR_GetFromAsyncQueue(arAsyncQueue_t *queue, void **pdata)
{
		arStatus_t status;
		AR_ASSERT(queue != NULL && pdata != NULL);
		status = AR_S_YES;

		AR_LockSpinLock(&queue->mutex);
		
		if(queue->data_cnt > 0)
		{
				*pdata = __pop_data(queue);
				AR_UnLockSpinLock(&queue->mutex);
				return status;
		}else
		{
				arEvent_t		*event;
				asyncWaitInfo_t	info;

				event = NULL;

				event = AR_CreateEvent(false);

				if(event == NULL)
				{
						status = AR_E_SYS;
						AR_UnLockSpinLock(&queue->mutex);
						goto END_POINT;
				}
				
				info.event = event;
				info.data = NULL;

				status = __push_wait(queue, &info);
				
				if(status != AR_S_YES)
				{
						AR_UnLockSpinLock(&queue->mutex);
						goto END_POINT;
				}

				AR_UnLockSpinLock(&queue->mutex);
				
				status = AR_WaitEvent(event);
				
				if(status != AR_S_YES)
				{
						/*不幸执行到此，可能出现任何问题，依然尝试性的清理一下info*/
						AR_LockSpinLock(&queue->mutex);

						__remove_wait_node(queue, &info);

						AR_UnLockSpinLock(&queue->mutex);

						goto END_POINT;
				}

				*pdata = info.data;

END_POINT:
				if(event != NULL)
				{
						AR_DestroyEvent(event);
						event = NULL;
				}

				return status;

		}

}


#if(0)

arStatus_t	AR_PutToAsyncQueue(arAsyncQueue_t *queue, void *data)
{
		arStatus_t	status;
		AR_ASSERT(queue != NULL);

		status = AR_S_YES;

		AR_LockSpinLock(&queue->mutex);
		
		if(queue->wait_cnt > 0)
		{
				asyncWaitInfo_t *info = __pop_wait(queue);
				AR_ASSERT(info != NULL && info->event != NULL);
				info->data = data;
				
				status = AR_SetEvent(info->event);
				if(status != AR_S_YES)
				{
						AR_error(AR_ERR_FATAL, L"%ls\r\n", L"Set event failed!");
				}
		}else
		{
				status = __push_data(queue, data);
		}
		
		AR_UnLockSpinLock(&queue->mutex);

		return status;
}
#endif


arStatus_t	AR_PutToAsyncQueue(arAsyncQueue_t *queue, void *data)
{
		arStatus_t	status;
		AR_ASSERT(queue != NULL);

		status = AR_S_YES;

		AR_LockSpinLock(&queue->mutex);
		
		if(queue->wait_cnt > 0)
		{
				asyncWaitInfo_t *info = __top_wait(queue);
				AR_ASSERT(info != NULL && info->event != NULL);
				info->data = data;
				status = AR_SetEvent(info->event);

				if(status != AR_S_YES)
				{
						AR_error(AR_ERR_WARNING, L"%ls\r\n", L"failed to put data!");
				}else
				{
						__pop_wait(queue);
				}
		}else
		{
				status = __push_data(queue, data);
		}
		
		AR_UnLockSpinLock(&queue->mutex);

		return status;
}

arStatus_t	AR_PutToAsyncQueueFront(arAsyncQueue_t *queue, void *data)
{
		arStatus_t	status;
		AR_ASSERT(queue != NULL);

		status = AR_S_YES;

		AR_LockSpinLock(&queue->mutex);
		
		if(queue->wait_cnt > 0)
		{
				asyncWaitInfo_t *info = __top_wait(queue);
				AR_ASSERT(info != NULL && info->event != NULL);
				info->data = data;
				status = AR_SetEvent(info->event);

				if(status != AR_S_YES)
				{
						AR_error(AR_ERR_WARNING, L"%ls\r\n", L"failed to put data!");
				}else
				{
						__pop_wait(queue);
				}
		}else
		{
				status = __push_data_front(queue, data);
		}
		
		AR_UnLockSpinLock(&queue->mutex);

		return status;
}



arStatus_t	AR_HasIdleThreadInAsyncQueue(const arAsyncQueue_t *queue)
{
		arStatus_t ret;
		arAsyncQueue_t *que;
		AR_ASSERT(queue != NULL);
		que = (arAsyncQueue_t*)queue;
		AR_LockSpinLock(&que->mutex);

		ret = que->wait_cnt == 0 ? AR_S_NO : AR_S_YES;

		AR_UnLockSpinLock(&que->mutex);
		return ret;

}



arStatus_t	AR_AsyncQueueIsEmpty(const arAsyncQueue_t *queue)
{
		arStatus_t ret;
		arAsyncQueue_t *que;
		AR_ASSERT(queue != NULL);
		que = (arAsyncQueue_t*)queue;
		AR_LockSpinLock(&que->mutex);

		ret = que->data_cnt == 0 ? AR_S_YES : AR_S_NO;

		AR_UnLockSpinLock(&que->mutex);
		return ret;
}


size_t		AR_GetAsyncQueueWaitCount(const arAsyncQueue_t *queue)
{
		AR_ASSERT(queue != NULL);
		return queue->wait_cnt;

}

size_t		AR_GetAsyncQueueDataCount(const arAsyncQueue_t *queue)
{
		AR_ASSERT(queue != NULL);
		
		return queue->data_cnt;
}


void	AR_ClearAsyncQueue(arAsyncQueue_t *queue)
{
		AR_ASSERT(queue != NULL);
		AR_LockSpinLock(&queue->mutex);
		
		while(queue->data_cnt)
		{
				__pop_data(queue);
		}
		
		AR_UnLockSpinLock(&queue->mutex);
}






AR_NAMESPACE_END
