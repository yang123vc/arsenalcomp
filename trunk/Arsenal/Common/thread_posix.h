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


#include <unistd.h>
#include <pthread.h>




AR_NAMESPACE_BEGIN




int_t			AR_AtomicInc(volatile int_t *dest)
{
		//return (uint_t)ATOMIC_INC(dest);
		*dest += 1;
		return *dest;
}

int_t			AR_AtomicDec(volatile int_t *dest)
{
		*dest -= 1;
		return *dest;
}


/****************************************************************************SpinLock***********************************************/


void			AR_YiledThread()
{
		pthread_yield();
}


void			AR_InitSpinLock(arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL);
		pthread_spin_init((pthread_spinlock_t*)lock,0);
		
}

void			AR_UnInitSpinLock(arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL);
		pthread_spin_destroy((pthread_spinlock_t*)lock);
}

void			AR_LockSpinLock(arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL);
		pthread_spin_lock((pthread_spinlock_t*)lock);
}


void			AR_UnLockSpinLock(arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL);
		/*COMP_EXCH(lock, (LONG)UNLOCK_STATE, (LONG)LOCK_STATE);*/
		pthread_spin_unlock((pthread_spinlock_t*)lock);
}


/****************************************************************************SpinLock***********************************************end*/



AR_NAMESPACE_END


