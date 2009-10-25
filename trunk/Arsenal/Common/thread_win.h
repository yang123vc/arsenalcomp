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




#if(AR_COMPILER == AR_VC6 || OS_TYPE == OS_WINDOWS_CE)
		struct _RPC_ASYNC_STATE;
#endif

#include <windows.h>

#define COMP_EXCH(_dest, _exch, _comp_val)	InterlockedCompareExchange((volatile LONG*)(_dest), (LONG )(_exch), (LONG)(_comp_val))

#if(AR_ARCH_VER == ARCH_32)
		#define ATOMIC_INC(_dest) (int_t)InterlockedIncrement((volatile LONG*)(_dest))
		#define  ATOMIC_DEC(_dest) (int_t)InterlockedDecrement((volatile LONG*)(_dest))
		/*#define COMP_EXCH(_dest, _exch, _comp_val)	InterlockedCompareExchange((volatile LONG*)(_dest), (LONG )(_exch), (LONG)(_comp_val))*/
#elif(AR_ARCH_VER == ARCH_64)
		#define  ATOMIC_INC(_dest) (int_t)InterlockedIncrement64((volatile LONGLONG*)(_dest))
		#define  ATOMIC_DEC(_dest) (int_t)InterlockedDecrement64((volatile LONGLONG*)(_dest))
		/*#define COMP_EXCH(_dest, _exch, _comp_val)	InterlockedCompareExchange64((volatile LONGLONG*)(_dest), (LONGLONG )(_exch), (LONGLONG)(_comp_val))*/
#else
		#error "Target ARCH  not supported"
#endif
		
#define __Yield()		Sleep(0)












AR_NAMESPACE_BEGIN


void			AR_YiledThread()
{
		__Yield();
}
/*
uint_t			AR_CompExchange(volatile uint_t *dest, uint_t exch, uint_t compval)
{
		return (uint_t)COMP_EXCH(dest, exch, compval);
}
*/

int_t			AR_AtomicInc(volatile int_t *dest)
{
		//return (uint_t)ATOMIC_INC(dest);
		return (int_t)ATOMIC_INC(dest);
}

int_t			AR_AtomicDec(volatile int_t *dest)
{
		return ATOMIC_DEC(dest);
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

		/*while((uint_t)COMP_EXCH(lock, LOCK_STATE, UNLOCK_STATE) != UNLOCK_STATE)*/
		while(COMP_EXCH(lock, LOCK_STATE, UNLOCK_STATE) != UNLOCK_STATE)
		{
				if(++count > AR_MAXSPIN_COUNT)
				{
						AR_YiledThread();
						count = 0;
				}
		}

}


void			AR_UnLockSpinLock(arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL && *lock == LOCK_STATE);
		/*COMP_EXCH(lock, (LONG)UNLOCK_STATE, (LONG)LOCK_STATE);*/
		COMP_EXCH(lock, UNLOCK_STATE, LOCK_STATE);
}


/****************************************************************************SpinLock***********************************************end*/



AR_NAMESPACE_END


