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


ar_bool_t			AR_AtomicCompExch(volatile ar_int_t *dest, ar_int_t val, ar_int_t cmpval)
{
		AR_ASSERT(dest != NULL);
		return COMP_EXCH(dest, val, cmpval) ? true : false;
}



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
        ticks = (((now - start_mach) * (ar_int_64_t)mach_base_info.numer) / (ar_int_64_t)mach_base_info.denom) / 1000000;
		return ticks;
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

void			AR_Sleep(ar_int_64_t millisecond)
{
		useconds_t m = (useconds_t)millisecond;

		usleep(m * 1000);

}
