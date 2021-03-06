﻿/*
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




static void __init_ticks();
static void __uninit_ticks();


void	Plat_InitThread()
{
        sigset_t sset;
        sigemptyset(&sset);
        sigaddset(&sset, SIGPIPE);
        pthread_sigmask(SIG_BLOCK, &sset, 0);


		__init_ticks();

}

void	Plat_UnInitThread()
{
		__uninit_ticks();

}


ar_int_t			AR_AtomicInc(volatile ar_int_t *dest)
{
        AR_ASSERT(dest != NULL);
        return __sync_add_and_fetch(dest, 1);
}


ar_int_t			AR_AtomicDec(volatile ar_int_t *dest)
{
        AR_ASSERT(dest != NULL);
        return __sync_sub_and_fetch(dest, 1);
}

ar_bool_t			AR_AtomicCompExch(volatile ar_int_t *dest, ar_int_t val, ar_int_t cmpval)
{
        AR_ASSERT(dest != NULL);
        return __sync_bool_compare_and_swap(dest, cmpval, val) ? true : false;
}




/****************************************************************************SpinLock***********************************************/



#if(0)

void			AR_InitSpinLock(arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL);
		*lock = OS_SPINLOCK_INIT;
}

void			AR_UnInitSpinLock(arSpinLock_t *lock)
{
        AR_ASSERT(lock != NULL);
		*lock = OS_SPINLOCK_INIT;
}



ar_bool_t AR_TryLockSpinLock(arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL);
        
        return OSSpinLockTry(lock) ?  true : false;

}


void			AR_LockSpinLock(arSpinLock_t *lock)
{

        AR_ASSERT(lock != NULL);

        OSSpinLockLock(lock);
		
}




void			AR_UnLockSpinLock(arSpinLock_t *lock)
{
        AR_ASSERT(lock != NULL && *lock == -1);
        OSSpinLockUnlock(lock);
		
}

#else


#define LOCK_STATE			1
#define UNLOCK_STATE		0


void			AR_InitSpinLock(arSpinLock_t *lock)
{
        AR_ASSERT(lock != NULL);
        *lock = UNLOCK_STATE;
}

void			AR_UnInitSpinLock(arSpinLock_t *lock)
{
        AR_ASSERT(lock != NULL && *lock == UNLOCK_STATE);
        *lock = UNLOCK_STATE;
}




ar_bool_t AR_TryLockSpinLock(arSpinLock_t *lock)
{
        AR_ASSERT(lock != NULL);
        return __sync_bool_compare_and_swap(lock, UNLOCK_STATE, LOCK_STATE) ? true : false;
        
        
}

void			AR_LockSpinLock(arSpinLock_t *lock)
{
        
        size_t count;
        AR_ASSERT(lock != NULL);
        count = 0;
        
        while(!AR_TryLockSpinLock(lock))
        {
                if(++count > AR_MAXSPIN_COUNT)
                {
                        AR_YieldThread();
                        count = 0;
                }
        }
}



void			AR_UnLockSpinLock(arSpinLock_t *lock)
{
        AR_ASSERT(lock != NULL && *lock == LOCK_STATE);
        
        while(!__sync_bool_compare_and_swap(lock, LOCK_STATE, UNLOCK_STATE))
        {
                AR_ASSERT(false);
        }
}



#endif




/****************************************************************************Ticks***********************************************/


static ar_int_64_t __g_start_mach = 0;
static mach_timebase_info_data_t __g_mach_base_info;

static void __init_ticks()
{
        mach_timebase_info(&__g_mach_base_info);
        __g_start_mach = (ar_int_64_t)mach_absolute_time();
        
}

static void __uninit_ticks()
{
        __g_start_mach = 0;
        AR_memset(&__g_mach_base_info, 0, sizeof(__g_mach_base_info));
}


ar_int_64_t		AR_GetTime_TickCount()
{
        ar_int_64_t now;
        ar_int_64_t ticks;
        
        ticks = 0;
        
        now = (ar_int_64_t)mach_absolute_time();
        ticks = (((now - __g_start_mach) * (ar_int_64_t)__g_mach_base_info.numer) / (ar_int_64_t)__g_mach_base_info.denom) / 1000000;
        return ticks;
}


/*******************************************************Priority***********************************************************************/



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
        
        if(pthread_setschedparam(thd->thd, SCHED_FIFO, &par) != 0)
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




/************************************************************************************************************************************end*/
void			AR_YieldThread()
{
		pthread_yield_np();
}


void			AR_Sleep(ar_int_64_t millisecond)
{
        
#if(0)
		useconds_t m = (useconds_t)millisecond;

		usleep(m * 1000);
#else
        
        if(millisecond == 0)
        {
                AR_YieldThread();
        }else
        {
                uint64_t now = mach_absolute_time();
                uint64_t interval =  + (millisecond * (ar_int_64_t)__g_mach_base_info.denom * 1000000 / __g_mach_base_info.numer);
                
                mach_wait_until(now + interval);
        }
#endif

}


