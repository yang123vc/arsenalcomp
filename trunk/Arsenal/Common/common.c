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


void AR_STDCALL AR_def_error(int_t level, const wchar_t* msg, void *ctx)
{
		printf("%ls", msg);
}


void AR_STDCALL AR_def_print(const wchar_t *msg, void *ctx)
{
		/*wprintf(L"%ls", msg);*/
		printf("%ls", msg);
}

#if(0)
void	AR_STDCALL AR_def_fatal(const wchar_t *msg, void *ctx)
{
		/*printf("Arsenal fatal error : %ls\r\n", msg);*/
		AR_printf(L"Arsenal fatal error : %ls\r\n", msg);
		/*
		exit(-1);
		abort();
		*/
}
#endif


static arInit_t	__g_ctx = {AR_def_error, AR_def_print,  NULL};

void AR_Init(const arInit_t *info)
{
		__g_ctx = *info;

}

void AR_UnInit()
{

}


void*	AR_global_ioctx()
{
		return __g_ctx.global_ctx;
}



void AR_error(int_t level, const wchar_t *msg, ...)
{
		wchar_t *buf;
		int_t len;
		va_list arg_ptr;
		
		if(__g_ctx.on_error != NULL)
		{
				
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);

				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
		
				__g_ctx.on_error(level, buf, __g_ctx.global_ctx);
				
				AR_DEL(buf);
		}


		if(level == AR_ERR_FATAL)
		{
				AR_abort();
		}

}




void AR_printf(const wchar_t *msg,...)
{
		wchar_t *buf;
		int_t len;
		va_list arg_ptr;
		if(__g_ctx.on_print != NULL)
		{
		
				
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);

				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
		
				__g_ctx.on_print(buf, __g_ctx.global_ctx);
				
				AR_DEL(buf);
		}
}


/*
void	AR_fatal(const wchar_t *msg,...)
{
		wchar_t *buf;
		int_t len;
		va_list arg_ptr;
		if(__g_ctx.on_fatal != NULL)
		{
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);
				
				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
				
				__g_ctx.on_fatal(buf, __g_ctx.global_ctx);

				AR_DEL(buf);
		}
		
		AR_abort();
}

*/


/***************************************************ctx**********************************/

void	AR_printf_ctx(void *ctx, const wchar_t *msg,...)
{
		wchar_t *buf;
		int_t len;
		va_list arg_ptr;
		if(__g_ctx.on_print)
		{
		
				
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);

				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
				__g_ctx.on_print(buf, ctx);
				AR_DEL(buf);
		}
}


void	AR_error_ctx(void *ctx, int_t level, const wchar_t *msg, ...)
{		
		wchar_t *buf;
		int_t len;
		va_list arg_ptr;
		
		if(__g_ctx.on_error)
		{
				
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);

				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
				__g_ctx.on_error(level, buf, ctx);
				AR_DEL(buf);
		}

}


/*
void	AR_fatal_ctx(void *ctx, const wchar_t *msg,...)
{

		wchar_t *buf;
		int_t len;
		va_list arg_ptr;
		if(__g_ctx.on_fatal)
		{
				va_start(arg_ptr, msg);
				
				len = AR_vscwprintf(msg, arg_ptr);
				if(len <= 0)return;
				
				buf = AR_NEWARR0(wchar_t, len + 1);
				
				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				va_end(arg_ptr);
				
				__g_ctx.on_fatal(buf, ctx);

				AR_DEL(buf);
		}
		
		AR_abort();

}
*/






AR_NAMESPACE_END

