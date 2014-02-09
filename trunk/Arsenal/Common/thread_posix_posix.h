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


ar_bool_t			AR_AtomicCompExch(volatile ar_int_t *dest, ar_int_t val, ar_int_t cmpval)
{
		AR_ASSERT(dest != NULL);
		return __sync_bool_compare_and_swap(dest, cmpval, val) ? true : false;
}

/****************************************************************************SpinLock***********************************************/



void			AR_YieldThread()
{
		pthread_yield();
}

void			AR_Sleep(ar_int_64_t millisecond)
{
		long m = (long)millisecond;

		struct timeval tv;
		tv.tv_sec = m / 1000;
		tv.tv_usec = (m % 1000) * 1000;

		int ret = select(0,NULL,NULL,NULL,&tv);
		if(ret == -1)
		{
				AR_error(AR_ERR_WARNING, L"AR_Sleep failed\r\n");
		}

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




ar_bool_t AR_TryLockSpinLock(arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL);
		return pthread_spin_trylock(lock) == 0 ? true : false;
		

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
