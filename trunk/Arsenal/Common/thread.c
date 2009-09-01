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

#if defined(OS_FAMILY_WINDOWS)

#if(AR_COMPILER == AR_VC6)
struct _RPC_ASYNC_STATE;
#endif

#include <windows.h>

#if(AR_COMPILER == AR_VC6)
		#define COMP_EXCH(_dest, _exch, _comp_val)	InterlockedCompareExchange((PVOID*)(_dest), (PVOID )(_exch), (PVOID)(_comp_val))
#else
		#define COMP_EXCH(_dest, _exch, _comp_val)	InterlockedCompareExchangePointer((volatile PVOID*)(_dest), (PVOID )(_exch), (PVOID)(_comp_val))
#endif

#define __Yield()		Sleep(0)

#elif defined(OS_FAMILY_UNIX)

#else 

#error UnKnow Platform

#endif






AR_NAMESPACE_BEGIN


void			AR_YiledThread()
{
		__Yield();
}

uint_t			AR_CompExchange(volatile uint_t *dest, uint_t exch, uint_t compval)
{
		return (uint_t)COMP_EXCH(dest, exch, compval);
}




/****************************************************************************SpinLock***********************************************/

#define LOCK_STATE			1
#define UNLOCK_STATE		0


void			AR_InitSpinLock(volatile arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL);
		*lock = UNLOCK_STATE;
}

void			AR_UnInitSpinLock(volatile arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL);
		*lock = UNLOCK_STATE;
}

void			AR_LockSpinLock(volatile arSpinLock_t *lock)
{
		size_t count;
		AR_ASSERT(lock != NULL);
		count = 0;

		/*while((uint_t)COMP_EXCH(lock, LOCK_STATE, UNLOCK_STATE) != UNLOCK_STATE)*/
		while(AR_CompExchange(lock, LOCK_STATE, UNLOCK_STATE) != UNLOCK_STATE)
		{
				if(++count > AR_MAXSPIN_COUNT)
				{
						AR_YiledThread();
						count = 0;
				}
		}

}


void			AR_UnLockSpinLock(volatile arSpinLock_t *lock)
{
		AR_ASSERT(lock != NULL && *lock == LOCK_STATE);
		/*COMP_EXCH(lock, (LONG)UNLOCK_STATE, (LONG)LOCK_STATE);*/
		AR_CompExchange(lock, UNLOCK_STATE, LOCK_STATE);
}


/****************************************************************************SpinLock***********************************************end*/



AR_NAMESPACE_END