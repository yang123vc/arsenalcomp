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



struct __arsenal_thread_tag
{
        pthread_t               thd;
        arThreadPrio_t          prio;
        void                    *data;
        arThreadFunc_t          func;
        arEvent_t               *done;
};



struct __arsenal_event_tag
{
        ar_bool_t           is_auto_reset;
        volatile ar_bool_t  state;
        pthread_mutex_t  mtx;
        pthread_cond_t   cond;
};



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





#if(OS_TYPE != OS_MAC_OS_X && OS_TYPE != OS_IOS)


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

        par.sched_priority = __map_prio(prio);

        if(pthread_setschedparam(thd->thd, SCHED_OTHER, &par) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot set thread priority!\r\n");

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


#endif

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
                AR_error(AR_ERR_WARNING, L"cannot create mutex\r\n");
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
                AR_error(AR_ERR_WARNING, L"cannot lock mutex\r\n");
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
                AR_error(AR_ERR_WARNING, L"cannot trylock mutex\r\n");
                return AR_E_SYS;
        }
}

arStatus_t		AR_UnLockMutex(arMutex_t *mtx)
{
        AR_ASSERT(mtx != NULL);
        if(pthread_mutex_unlock((pthread_mutex_t*)mtx) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot unlock mutex\r\n");
                return AR_E_SYS;
        }else
        {
                return AR_S_YES;
        }
}


/***************************************************Event*****************************************/



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
                AR_error(AR_ERR_WARNING, L"cannot create event (mutex)\r\n");
                goto FAILED_POINT;
        }else
        {
                mtx_init = true;
        }

        if(pthread_cond_init(&evt->cond, NULL) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot create event (condition)\r\n");
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




arStatus_t		AR_SetEvent(arEvent_t *evt)
{
        if(pthread_mutex_lock(&evt->mtx) != 0)
        {
                AR_error(AR_ERR_WARNING, L"cannot signal event (lock)\r\n");
                return AR_E_SYS;
        }

        evt->state = true;

        if(pthread_cond_broadcast(&evt->cond) != 0)
        {
                pthread_mutex_unlock(&evt->mtx);
                AR_error(AR_ERR_WARNING, L"cannot signal event\r\n");
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
                AR_error(AR_ERR_WARNING, L"cannot reset event\r\n");
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
                AR_error(AR_ERR_WARNING, L"wait for event failed (lock)\r\n");
                return AR_E_SYS;
        }

        while (!evt->state)
        {
                int err = pthread_cond_wait(&evt->cond, &evt->mtx);
                if(err != 0)
                {
                        pthread_mutex_unlock(&evt->mtx);
                        AR_error(AR_ERR_WARNING, L"wait for event failed : %d\r\n", err);
                        AR_ASSERT(false);/*must be logical error !!!*/
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
                AR_error(AR_ERR_WARNING, L"wait for event failed (lock)\r\n");
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
                        AR_error(AR_ERR_WARNING, L"cannot wait for event\r\n");
                        AR_ASSERT(false);//must be logical error !!!
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
		
}

void			AR_UnInitThread()
{
        Plat_UnInitThread();
}

AR_NAMESPACE_END


