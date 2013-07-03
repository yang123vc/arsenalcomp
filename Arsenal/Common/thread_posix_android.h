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


void			AR_InitThread()
{
        sigset_t sset;
        sigemptyset(&sset);
        sigaddset(&sset, SIGPIPE);
        pthread_sigmask(SIG_BLOCK, &sset, 0);
        
}

void			AR_UnInitThread()
{
        
        
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

void			AR_LockSpinLock(arSpinLock_t *lock)
{
        
		size_t count;
		AR_ASSERT(lock != NULL);
		count = 0;
        
		while(!__sync_bool_compare_and_swap(lock, UNLOCK_STATE, LOCK_STATE))
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


/************************************************************************************************************************************end*/
void			AR_YieldThread()
{
        usleep(0);
}

void			AR_Sleep(ar_uint_64_t millisecond)
{
		useconds_t m = (useconds_t)millisecond;
        
		usleep(m * 1000);
        
}


#endif
