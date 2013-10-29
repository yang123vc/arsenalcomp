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


#if(AR_ARCH_VER == ARCH_32)

#define COMP_EXCH(_val, _new, _old)	OSAtomicCompareAndSwap32Barrier((int32_t)(_old), (int32_t)(_new), (volatile  int32_t*)(_val))

#elif(AR_ARCH_VER == ARCH_64)

#define COMP_EXCH(_val, _new, _old)	OSAtomicCompareAndSwap64Barrier((int64_t)(_old), (int64_t)(_new), (volatile  int64_t*)(_val))

#else
#error "Target ARCH  not supported"
#endif

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
#if(AR_ARCH_VER == ARCH_32)
		return (ar_int_t)OSAtomicIncrement32Barrier(dest);
#elif(AR_ARCH_VER == ARCH_64)
		return (ar_int_t)OSAtomicIncrement64Barrier(dest);
#else
#error	"Unknown Platform";
#endif

}

ar_int_t			AR_AtomicDec(volatile ar_int_t *dest)
{
		AR_ASSERT(dest != NULL);

#if(AR_ARCH_VER == ARCH_32)
		return (ar_int_t)OSAtomicDecrement32Barrier(dest);
#elif(AR_ARCH_VER == ARCH_64)
		return (ar_int_t)OSAtomicDecrement64Barrier(dest);
#else
#error	"Unknown Platform";
#endif
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
        
		if(COMP_EXCH(lock, LOCK_STATE, UNLOCK_STATE) == UNLOCK_STATE)
		{
				return true;
		}else
		{
				return false;
		}
		

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

		while(!COMP_EXCH(lock, UNLOCK_STATE, LOCK_STATE))
		{
				AR_ASSERT(false);
		}
}


/************************************************************************************************************************************end*/
void			AR_YieldThread()
{
		pthread_yield_np();
}

void			AR_Sleep(ar_uint_64_t millisecond)
{
		useconds_t m = (useconds_t)millisecond;

		usleep(m * 1000);

}
