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

/******************************************************Print**************************************************************/

AR_NAMESPACE_BEGIN





/***********************************************************Init****************************************************************/


void AR_STDCALL AR_def_error(int level, const wchar_t* msg, void *ctx)
{
		printf("%d:%ls", (uint32_t)level, msg);
}


void AR_STDCALL AR_def_print(const wchar_t *msg, void *ctx)
{
		/*wprintf(L"%ls", msg);*/
		printf("%ls", msg);
}

void	AR_STDCALL AR_def_fatal(const wchar_t *msg, void *ctx)
{
		/*wprintf(L"Arsenal fatal error : %ls\r\n", msg);*/
		AR_printf(L"Arsenal fatal error : %ls\r\n", msg);
		/*
		exit(-1);
		abort();
		*/
}



static arInit_t	__ctx = { AR_def_error, AR_def_print, AR_def_fatal, NULL};

void AR_Init(const arInit_t *info)
{
		__ctx = *info;

}

void AR_UnInit()
{

}



void AR_error(int level,  const wchar_t *msg, ...)
{
		wchar_t *buf;
		int_t len;
		va_list arg_ptr;
		
		if(__ctx.error_f != NULL)
		{
				
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);

				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
		
				__ctx.error_f(level, buf, __ctx.ctx);
		
				AR_DEL(buf);
		}
}



void AR_printf(const wchar_t *msg,...)
{
		wchar_t *buf;
		int_t len;
		va_list arg_ptr;
		if(__ctx.print_f != NULL)
		{
		
				
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);

				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
		
				__ctx.print_f(buf, __ctx.ctx);
		
				AR_DEL(buf);

		}

}


void	AR_fatal(const wchar_t *msg,...)
{
		wchar_t *buf;
		int_t len;
		va_list arg_ptr;
		if(__ctx.fatal_f != NULL)
		{
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);
				
				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
				__ctx.fatal_f(buf, __ctx.ctx);
				AR_DEL(buf);
		}
		
		AR_abort();
		
		
}













AR_NAMESPACE_END

