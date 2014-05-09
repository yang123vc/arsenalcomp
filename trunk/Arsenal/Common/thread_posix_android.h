/*
 * The Arsenal Library
 * Copyright (c) 2009-2013 by Solidus
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express
 * or implied warranty.
 *
 */

#ifndef Arsenal_thread_posix_android_h
#define Arsenal_thread_posix_android_h


static void __init_ticks();
static void __uninit_ticks();

void			AR_InitThread()
{
        sigset_t sset;
        sigemptyset(&sset);
        sigaddset(&sset, SIGPIPE);
        pthread_sigmask(SIG_BLOCK, &sset, 0);
		__init_ticks();
        
}

void			AR_UnInitThread()
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


/****************************************************************************Ticks***********************************************/

static struct timespec __g_start_ts;
static void __init_ticks()
{
		clock_gettime(CLOCK_MONOTONIC, &__g_start_ts);
}

static void __uninit_ticks()
{

}

ar_int_64_t		AR_GetTime_TickCount()
{
		struct timespec now;
		ar_int_64_t ticks;
        clock_gettime(CLOCK_MONOTONIC, &now);
        ticks = (ar_int_64_t)((now.tv_sec - __g_start_ts.tv_sec) * 1000 + (now.tv_nsec - __g_start_ts.tv_nsec) / 1000000);
		return ticks;
}

/************************************************************************************************************************************end*/
void			AR_YieldThread()
{
        usleep(0);
}

void			AR_Sleep(ar_int_64_t millisecond)
{
		useconds_t m = (useconds_t)millisecond;
        
		usleep(m * 1000);
        
}


#endif
