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
#include <signal.h>
#include <sched.h>
#include <errno.h>
#include <time.h>

#if(OS_TYPE == OS_MAC_OS_X || OS_TYPE == OS_IOS)
        #include <mach/mach.h>
        #include <mach/mach_time.h>
#endif

#include "common.h"





AR_NAMESPACE_BEGIN


#if (OS_TYPE == OS_MAC_OS_X || OS_TYPE == OS_IOS)

		#include "thread_posix_apple.h"

#elif (OS_TYPE == OS_ANDROID)

        #include "thread_posix_android.h"

#else
		#include "thread_posix_posix.h"
#endif





/**********************************************************************************************************************************end*/






/***************************************************************************************************************************************/

#define RESOLUTION		1000000LL

ar_int_64_t		AR_GetTime_Microseconds()
{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return (ar_int_64_t)tv.tv_sec * (ar_int_64_t)RESOLUTION + (ar_int_64_t)tv.tv_usec;
}




/***********************************************************************************************/


/********************************************Thread**********************************************/

struct __arsenal_thread_tag
{
        pthread_t               thd;
        arThreadPrio_t          prio;
        void                    *data;
        arThreadFunc_t          func;
        arEvent_t               *done;
};


static void* __entry(void* data)
{
        arThread_t *thd;
#if defined(OS_FAMILY_UNIX)
        sigset_t sset;
        sigemptyset(&sset);
        sigaddset(&sset, SIGQUIT);
        sigaddset(&sset, SIGTERM);
        sigaddset(&sset, SIGPIPE);
        pthread_sigmask(SIG_BLOCK, &sset, 0);
#endif

        thd = (arThread_t*)data;
        thd->func(thd->data);

        AR_SetEvent(thd->done);

        return NULL;
}


arThread_t*		AR_CreateThread(arThreadFunc_t func, void *data)
{
        arThread_t *thd;
        pthread_attr_t attributes;
        AR_ASSERT(func != NULL);

        thd = AR_NEW0(arThread_t);

        if(thd == NULL)
        {
                goto FAILED_POINT;
        }

        thd->done = AR_CreateEvent(false);
        if(thd->done == NULL)
        {
                goto FAILED_POINT;
        }

        thd->prio = AR_THREAD_PREC_NORMAL;
        thd->func = func;
        thd->data = data;

        pthread_attr_init(&attributes);

        if(pthread_create(&thd->thd, &attributes, __entry, (void*)thd) != 0)
        {
                goto FAILED_POINT;
        }

        return thd;

FAILED_POINT:
        if(thd && thd->done != NULL)
        {
                AR_DestroyEvent(thd->done);
                thd->done = NULL;
        }

        if(thd)
        {
                AR_DEL(thd);
                thd = NULL;
        }
        return NULL;

}

void			AR_DestroyThread(arThread_t *thd)
{
        void *result;
        int ret = 0;
        AR_ASSERT(thd != NULL);
        AR_JoinThread(thd);

        if((ret = pthread_join(thd->thd, &result)) != 0)
        {
                AR_error(AR_ERR_WARNING, L"can not join thread : error code == %d\r\n", ret);
        }

        if((ret = pthread_detach(thd->thd)) != 0)
        {
               /*If you join after the success of this function call is actually redundant,
			   so returns 3 it's not matter, the resources occupied by thread has been released
			   */

        }

        AR_DestroyEvent(thd->done);
        thd->done = NULL;
        AR_DEL(thd);
        thd = NULL;
}

arStatus_t		AR_JoinThread(arThread_t *thd)
{
        arStatus_t status;

        AR_ASSERT(thd != NULL && thd->done != NULL);

        status = AR_WaitEvent(thd->done);

        return status;
}

arStatus_t		AR_JoinThreadWithTimeout(arThread_t *thd, ar_int_64_t milliseconds)
{
        arStatus_t status;
        AR_ASSERT(thd != NULL && thd->done != NULL);

        status = AR_WaitEventWithTimeout(thd->done, milliseconds);

        return status;
}






ar_uint_64_t		AR_GetThreadId(arThread_t *thd)
{
		AR_ASSERT(thd != NULL);
		return (ar_uint_64_t)thd->thd;
}




static int __get_min_os_prio()
{
        return sched_get_priority_min(SCHED_OTHER);
}

static int __get_max_os_prio()
{
        return sched_get_priority_max(SCHED_OTHER);
}

static int __map_prio(arThreadPrio_t prio)
{
        int pmin = __get_min_os_prio();
        int pmax = __get_max_os_prio();

        switch (prio)
        {
        case AR_THREAD_PREC_LOW:
                return pmin;
        case AR_THREAD_PREC_NORMAL:
                return pmin + (pmax - pmin) / 2;
       default:
               return pmax;
        }
}


arStatus_t		AR_SetThreadPriority(arThread_t *thd, arThreadPrio_t prio)
{
        struct sched_param par;
        AR_ASSERT(thd != NULL);

        if(prio == thd->prio)
        {
                return AR_S_YES;
        }


        par.sched_priority = __map_prio(thd->prio);

        if(pthread_setschedparam(thd->thd, SCHED_OTHER, &par) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot set thread priority!");
                return AR_E_SYS;
        }else
        {
                thd->prio = prio;
                return AR_S_YES;
        }
}

arStatus_t		AR_GetThreadPriority(arThread_t *thd, arThreadPrio_t *p_prio)
{
        AR_ASSERT(thd != NULL);

        *p_prio = thd->prio;
        return AR_S_YES;
}



/********************************Mutex*****************************************/




arMutex_t*		AR_CreateMutex()
{
        pthread_mutex_t *mtx;
        pthread_mutexattr_t attr;

        mtx = AR_NEW0(pthread_mutex_t);
        if(mtx == NULL)
        {
                return NULL;
        }

        pthread_mutexattr_init(&attr);

        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

        if(pthread_mutex_init(mtx, &attr) != 0)
        {
                pthread_mutexattr_destroy(&attr);
                AR_DEL(mtx);
                mtx = NULL;
                AR_error(AR_ERR_WARNING, L"cannot create mutex");
                return NULL;
        }
        pthread_mutexattr_destroy(&attr);
        return (arMutex_t*)mtx;
}

void			AR_DestroyMutex(arMutex_t *mtx)
{
        AR_ASSERT(mtx != NULL);
        pthread_mutex_destroy((pthread_mutex_t*)mtx);

        AR_DEL(mtx);
		mtx = NULL;

}

arStatus_t		AR_LockMutex(arMutex_t *mtx)
{
        AR_ASSERT(mtx != NULL);
        if (pthread_mutex_lock((pthread_mutex_t*)mtx) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot lock mutex");
                return AR_E_SYS;
        }
        return AR_S_YES;
}

arStatus_t		AR_TryLockMutex(arMutex_t *mtx)
{
        int rc;
        AR_ASSERT(mtx != NULL);

        rc = pthread_mutex_trylock((pthread_mutex_t*)mtx);
        if(rc == 0)
        {
                return AR_S_YES;
        }else if(rc == EBUSY)
        {
                return AR_S_NO;
        }else
        {
                AR_error(AR_ERR_WARNING, L"cannot trylock mutex");
                return AR_E_SYS;
        }
}

arStatus_t		AR_UnLockMutex(arMutex_t *mtx)
{
        AR_ASSERT(mtx != NULL);
        if(pthread_mutex_unlock((pthread_mutex_t*)mtx) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot unlock mutex");
                return AR_E_SYS;
        }else
        {
                return AR_S_YES;
        }
}


/***************************************************Event*****************************************/



struct __arsenal_event_tag
{
        ar_bool_t           is_auto_reset;
        volatile ar_bool_t  state;
        pthread_mutex_t  mtx;
        pthread_cond_t   cond;

		arEvent_t		*next;
};


static arSpinLock_t		__g_event_list_lock;
static arEvent_t		*__g_event_list = NULL;
static size_t			__g_event_list_count = 0;


static arEvent_t*		__create_event(ar_bool_t is_auto_reset)
{
		arEvent_t *ret = NULL;
		ar_bool_t mtx_init = false,cond_init = false;
		arStatus_t status = AR_S_YES;

		AR_LockSpinLock(&__g_event_list_lock);

		if(__g_event_list)
		{
				ret = __g_event_list;
				__g_event_list = __g_event_list->next;
				ret->next = NULL;
				ret->is_auto_reset = is_auto_reset;
				AR_ResetEvent(ret);
				AR_ASSERT(__g_event_list_count > 0);
				__g_event_list_count--;
		}else
		{
				AR_ASSERT(__g_event_list_count == 0);
				ret = AR_NEW0(arEvent_t);
				if(ret == NULL)
				{
						status = AR_E_NOMEM;
						AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
						goto END_POINT;
				}

				ret->is_auto_reset = is_auto_reset;
				ret->state = false;

				if(pthread_mutex_init(&ret->mtx, NULL) != 0)
				{
						AR_error(AR_ERR_WARNING, L"cannot create event (mutex)");
						goto END_POINT;
				}else
				{
						mtx_init = true;
				}

				if(pthread_cond_init(&ret->cond, NULL) != 0)
				{
						AR_error(AR_ERR_WARNING, L"cannot create event (condition)");
						goto END_POINT;
				}else
				{
						cond_init = true;
				}
		}

END_POINT:
		if(status != AR_S_YES)
		{
				if(ret && mtx_init)
				{
						pthread_mutex_destroy(&ret->mtx);
				}

				if(ret && cond_init)
				{
						pthread_cond_destroy(&ret->cond);
				}

				if(ret)
				{
						AR_DEL(ret);
						ret = NULL;
				}
		}

		AR_UnLockSpinLock(&__g_event_list_lock);
		return ret;
}

static void __destroy_event(arEvent_t *evt)
{

		if(evt == NULL)
		{
				return;
		}


		AR_LockSpinLock(&__g_event_list_lock);

		if(__g_event_list_count > 20)
		{
				pthread_cond_destroy(&evt->cond);
				pthread_mutex_destroy(&evt->mtx);
				AR_DEL(evt);
				evt = NULL;
		}else
		{
				AR_ResetEvent(evt);
				evt->next = __g_event_list;
				__g_event_list = evt;
				__g_event_list_count++;
		}

		AR_UnLockSpinLock(&__g_event_list_lock);
}

static arStatus_t		__init_event_freelist()
{
		AR_InitSpinLock(&__g_event_list_lock);
		__g_event_list = NULL;
		__g_event_list_count = 0;
}


static void __uninit_event_freelist()
{
		arEvent_t *curr = NULL;
		curr = __g_event_list;
		
		while(curr)
		{
				arEvent_t *next = curr->next;
				AR_DEL(curr);
				curr = next;
		}

		AR_UnInitSpinLock(&__g_event_list_lock);
}





#if(0)
arEvent_t*		AR_CreateEvent(ar_bool_t is_auto_reset)
{
        ar_bool_t mtx_init,cond_init;
        arEvent_t *evt;
        evt = AR_NEW0(arEvent_t);
        if(evt == NULL)
        {
                return NULL;
        }

        mtx_init = false;
        cond_init = false;

        evt->is_auto_reset = is_auto_reset;
        evt->state = false;

        if(pthread_mutex_init(&evt->mtx, NULL) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot create event (mutex)");
                goto FAILED_POINT;
        }else
        {
                mtx_init = true;
        }

        if(pthread_cond_init(&evt->cond, NULL) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot create event (condition)");
                goto FAILED_POINT;
        }else
        {
                cond_init = true;
        }

        return evt;

FAILED_POINT:
        if(evt && mtx_init)
        {
                pthread_mutex_destroy(&evt->mtx);
        }

        if(evt && cond_init)
        {
                pthread_cond_destroy(&evt->cond);
        }

        if(evt)
        {
                AR_DEL(evt);
                evt = NULL;
        }

        return NULL;
}

void			AR_DestroyEvent(arEvent_t *evt)
{
        AR_ASSERT(evt != NULL);
        pthread_cond_destroy(&evt->cond);
        pthread_mutex_destroy(&evt->mtx);
        AR_DEL(evt);
        evt = NULL;
}
#else



arEvent_t*		AR_CreateEvent(ar_bool_t is_auto_reset)
{
		__create_event(is_auto_reset);
}

void			AR_DestroyEvent(arEvent_t *evt)
{
		AR_ASSERT(evt != NULL);
		__destroy_event(evt);
}

#endif


arStatus_t		AR_SetEvent(arEvent_t *evt)
{
        if(pthread_mutex_lock(&evt->mtx) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot signal event (lock)");
                return AR_E_SYS;
        }

        evt->state = true;

        if(pthread_cond_broadcast(&evt->cond) != 0)
        {
                pthread_mutex_unlock(&evt->mtx);
                AR_error(AR_ERR_WARNING, L"cannot signal event");
                return AR_E_SYS;
        }

        pthread_mutex_unlock(&evt->mtx);

        return AR_S_YES;
}

arStatus_t		AR_ResetEvent(arEvent_t *evt)
{
        AR_ASSERT(evt != NULL);

        if(pthread_mutex_lock(&evt->mtx) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot reset event");
                return AR_E_SYS;
        }
        evt->state = false;
        pthread_mutex_unlock(&evt->mtx);
        return AR_S_YES;
}

arStatus_t		AR_WaitEvent(arEvent_t *evt)
{
        AR_ASSERT(evt != NULL);
        if(pthread_mutex_lock(&evt->mtx) != 0)
        {
                AR_error(AR_ERR_WARNING, L"wait for event failed (lock)");
                return AR_E_SYS;
        }

        while (!evt->state)
        {
                if(pthread_cond_wait(&evt->cond, &evt->mtx) != 0)
                {
                        pthread_mutex_unlock(&evt->mtx);
                        AR_error(AR_ERR_WARNING, L"wait for event failed");
                        return AR_E_SYS;
                }
        }

        if(evt->is_auto_reset)
        {
                evt->state = false;
        }

        pthread_mutex_unlock(&evt->mtx);

        return AR_S_YES;
}

arStatus_t		AR_TryWaitEvent(arEvent_t *evt)
{
        AR_ASSERT(evt != NULL);
        return AR_WaitEventWithTimeout(evt, 0);
}

arStatus_t		AR_WaitEventWithTimeout(arEvent_t *evt, ar_int_64_t milliseconds)
{
        int rc;
        struct timespec abstime;
        struct timeval tv;

        AR_ASSERT(evt != NULL);

        rc = 0;

        gettimeofday(&tv, NULL);
        abstime.tv_sec  = tv.tv_sec + (ar_int_t)milliseconds / 1000;
        abstime.tv_nsec = tv.tv_usec*1000 + ((ar_int_t)milliseconds % 1000)*1000000;
        if (abstime.tv_nsec >= 1000000000)
        {
                abstime.tv_nsec -= 1000000000;
                abstime.tv_sec++;
        }


        if(pthread_mutex_lock(&evt->mtx) != 0)
        {
                AR_error(AR_ERR_WARNING, L"wait for event failed (lock)");
                return AR_E_SYS;
        }

        while(!evt->state)
        {
                rc = pthread_cond_timedwait(&evt->cond, &evt->mtx, &abstime);

                if(rc == ETIMEDOUT)
                {
                        break;
                }else if(rc == 0)
                {
                        continue;
                }else
                {
                        pthread_mutex_unlock(&evt->mtx);
                        AR_error(AR_ERR_WARNING, L"cannot wait for event");
                        return AR_E_SYS;
                }
        }

        if (rc == 0 && evt->is_auto_reset)
        {
                evt->state = false;
        }

        pthread_mutex_unlock(&evt->mtx);
        return rc == 0 ? AR_S_YES : AR_E_TIMEOUT;
}


/************************************************************************************************************/


void		AR_InitThread()
{
		Plat_InitThread();
		__init_event_freelist();
}

void			AR_UnInitThread()
{
		__uninit_event_freelist();
        Plat_UnInitThread();
		
}

AR_NAMESPACE_END


