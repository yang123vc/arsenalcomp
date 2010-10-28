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





AR_NAMESPACE_BEGIN


#if(AR_ARCH_VER == ARCH_32)

	#define COMP_EXCH(_old, _new, _val)	OSAtomicCompareAndSwap32((int32_t)(_old), (int32_t)(_new), (volatile  int32_t*)(_val))

#elif(AR_ARCH_VER == ARCH_64)

	#define COMP_EXCH(_old, _new, _val)	OSAtomicCompareAndSwap64((int64_t)(_old), (int64_t)(_new), (volatile  int64_t*)(_val))
#else
	#error "Target ARCH  not supported"
#endif

void			AR_InitThread()
{
	
	
}

void			AR_UnInitThread()
{
	
	
}

int_t			AR_AtomicInc(volatile int_t *dest)
{
	int_t ret;
	AR_ASSERT(dest != NULL);
#if(ARCH == ARCH_32)
		ret = (int_t)OSAtomicAdd32(1, (volatile int32_t*)dest);
#elif(ARCH == ARCH64)
		ret = (int_t)OSAtomicAdd64(1, (volatile int64_t*)dest);
#endif
	return ret;
}

int_t			AR_AtomicDec(volatile int_t *dest)
{
	int_t ret;
	AR_ASSERT(dest != NULL);
#if(ARCH == ARCH_32)
	ret = (int_t)OSAtomicAdd32(-1, (volatile int32_t*)dest);
#elif(ARCH == ARCH64)
	ret = (int_t)OSAtomicAdd64(-1, (volatile int64_t*)dest);
#endif
	return ret;
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
	
	while(COMP_EXCH(UNLOCK_STATE, LOCK_STATE, lock))
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
	
	COMP_EXCH(LOCK_STATE, UNLOCK_STATE, lock);
	
}


/************************************************************************************************************************************end*/
void			AR_YieldThread()
{
		sleep(0);
}

void			AR_Sleep(size_t millisecond)
{
	struct  timeval tv;
	tv.tv_sec = 0LL;
	tv.tv_usec = (int_64_t)millisecond * 1000LL;
	select(0,NULL,NULL,NULL,&tv);
}




/**********************************************************************************************************************************end*/

#define RESOLUTION		1000000LL

uint_64_t		AR_GetTime_Microseconds()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (uint_64_t)tv.tv_sec * (uint_64_t)RESOLUTION + (uint_64_t)tv.tv_usec;
}


AR_NAMESPACE_END


