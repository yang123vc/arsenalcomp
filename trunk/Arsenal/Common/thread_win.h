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


#if(WINVER < 0x0400)
		#define __VOLATILE__
#else
		#if(OS_TYPE == OS_WINDOWS_CE)
				#define __VOLATILE__	
		#else
				#define __VOLATILE__	volatile
		#endif
#endif



#if(AR_ARCH_VER == ARCH_32)
		
		#define ATOMIC_INC(_dest) (int_t)InterlockedIncrement((__VOLATILE__ LONG*)(_dest))
		#define ATOMIC_DEC(_dest) (int_t)InterlockedDecrement((__VOLATILE__ LONG*)(_dest))
		#define COMP_EXCH(_dest, _exch, _comp_val)	InterlockedCompareExchange((__VOLATILE__ LONG*)(_dest), (LONG )(_exch), (LONG)(_comp_val))

#elif(AR_ARCH_VER == ARCH_64)
		
		#define ATOMIC_INC(_dest) (int_t)InterlockedIncrement64((__VOLATILE__ LONGLONG*)(_dest))
		#define ATOMIC_DEC(_dest) (int_t)InterlockedDecrement64((__VOLATILE__ LONGLONG*)(_dest))
		#define COMP_EXCH(_dest, _exch, _comp_val)	InterlockedCompareExchange64((__VOLATILE__ LONGLONG*)(_dest), (LONGLONG )(_exch), (LONGLONG)(_comp_val))
#else
		#error "Target ARCH  not supported"
#endif
		
#define __Yield()		Sleep(0)







AR_NAMESPACE_BEGIN



void			AR_InitThread()
{


}

void			AR_UnInitThread()
{


}

void			AR_YieldThread()
{
		__Yield();
}

void			AR_Sleep(size_t millisecond)
{
		Sleep((DWORD)millisecond);
}

int_t			AR_AtomicInc(volatile int_t *dest)
{
		return (int_t)ATOMIC_INC(dest);
}

int_t			AR_AtomicDec(volatile int_t *dest)
{
		return (int_t)ATOMIC_DEC(dest);
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

		while(COMP_EXCH(lock, LOCK_STATE, UNLOCK_STATE) != UNLOCK_STATE)
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

		COMP_EXCH(lock, UNLOCK_STATE, LOCK_STATE);
}


static AR_INLINE uint_64_t __get_time_microseconds()
{
		FILETIME ft;
		ULARGE_INTEGER epoch; /* UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME*/
		ULARGE_INTEGER ts;

#if(OS_TYPE == OS_WINDOWS_CE)
		{
				SYSTEMTIME	st;
				GetSystemTime(&st);
				SystemTimeToFileTime(&st, &ft);
		}
#else
		GetSystemTimeAsFileTime(&ft);
#endif

		/*此代码来自一个个地方，我不知道这做了什么，也不想知道*/
		epoch.LowPart  = 0xD53E8000;
		epoch.HighPart = 0x019DB1DE;
		ts.LowPart  = ft.dwLowDateTime;
		ts.HighPart = ft.dwHighDateTime;
		ts.QuadPart -= epoch.QuadPart;
		return ts.QuadPart/10;
}


uint_64_t		AR_GetTime_Microseconds()
{
		return __get_time_microseconds();
}





/****************************************************************************SpinLock***********************************************end*/





arMutex_t*		AR_CreateMutex()
{
		CRITICAL_SECTION *mtx;

		mtx = AR_NEW(CRITICAL_SECTION);
		
		if(mtx == NULL)
		{
				return NULL;
		}

		InitializeCriticalSectionAndSpinCount(mtx, 100000);
		return (arMutex_t*)mtx;
}

void			AR_DestroyMutex(arMutex_t *mtx)
{
		AR_ASSERT(mtx != NULL);
		DeleteCriticalSection((CRITICAL_SECTION*)mtx);
		AR_DEL(mtx);
		mtx = NULL;
}

arStatus_t		AR_LockMutex(arMutex_t *mtx)
{
		AR_ASSERT(mtx != NULL);
		EnterCriticalSection((CRITICAL_SECTION*)mtx);
		return AR_S_YES;
}


arStatus_t		AR_TryLockMutex(arMutex_t *mtx)
{
		AR_ASSERT(mtx != NULL);
		
		return TryEnterCriticalSection((CRITICAL_SECTION*)mtx) ? AR_S_YES : AR_S_NO;
}


arStatus_t		AR_UnlockMutex(arMutex_t *mtx)
{
		AR_ASSERT(mtx != NULL);
		LeaveCriticalSection((CRITICAL_SECTION*)mtx);
		return AR_S_YES;
}



arEvent_t*		AR_CreateEvent(bool_t is_auto_reset)
{
		HANDLE evt;
		evt = CreateEvent(NULL, is_auto_reset ? FALSE : TRUE, FALSE, NULL);
		return (arEvent_t*)evt;
}


void			AR_DestroyEvent(arEvent_t *evt)
{
		AR_ASSERT(evt != NULL);
		CloseHandle(evt);
		evt = NULL;
}

arStatus_t		AR_SetEvent(arEvent_t *evt)
{
		AR_ASSERT(evt != NULL);
		return SetEvent((HANDLE)evt) ? AR_S_YES : AR_E_SIGEVT;
}

arStatus_t		AR_WaitEvent(arEvent_t *evt)
{
		AR_ASSERT(evt != NULL);
		switch(WaitForSingleObject((HANDLE)evt, INFINITE))
		{
		case WAIT_OBJECT_0:
				return AR_S_YES;
		default:
				return AR_E_WAITEVT;
		}
}

arStatus_t		AR_WaitEventWithTimeout(arEvent_t *evt, size_t milliseconds)
{
		AR_ASSERT(evt != NULL);
		switch(WaitForSingleObject((HANDLE)evt, (DWORD)milliseconds))
		{
		case WAIT_TIMEOUT:
				return AR_S_NO;
		case WAIT_OBJECT_0:
				return AR_S_YES;
		default:
				return AR_E_WAITEVT;
		}
}


arStatus_t		AR_TryWaitEvent(arEvent_t *evt)
{
		return AR_WaitEventWithTimeout(evt, 0);
}

arStatus_t		AR_ResetEvent(arEvent_t *evt)
{
		return ResetEvent((HANDLE)evt) ? AR_S_YES : AR_E_SIGEVT;
}


AR_NAMESPACE_END





