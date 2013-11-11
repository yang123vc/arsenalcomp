/*
 * The Arsenal Library
 * Copyright (c) 2009-2013 by Solidus
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



const wchar_t*	AR_Version()
{
		return L"0.4.11.1151";
}






/***********************************************************Init****************************************************************/


static void AR_STDCALL __def_error(ar_int_t level, const wchar_t* msg, void *ctx)
{
		AR_UNUSED(level);
		AR_UNUSED(ctx);

        printf("%ls", msg);
}


static void AR_STDCALL __def_print(const wchar_t *msg, void *ctx)
{
		/*AR_UNUSED(msg);*/
		AR_UNUSED(ctx);

		printf("%ls", msg);
}



static arInit_t	__g_ctx = 
{
		{__def_error, __def_print,  NULL},
		{NULL, NULL}
};

arStatus_t AR_CommonInit(const arInit_t *info)
{
		
		if(info)
		{
				__g_ctx = *info;
		}

		AR_InitThread();
		AR_InitMemory();
		return AR_S_YES;
}

arStatus_t AR_CommonUnInit()
{
		AR_UnInitMemory();
		AR_UnInitThread();
		return AR_S_YES;
}

const arBacktrace_t*	AR_GetBacktrace()
{
		return &__g_ctx.backtrace;
}


arStatus_t AR_error(ar_int_t level, const wchar_t *msg, ...)
{
        arStatus_t status;
		va_list arg_ptr;
        wchar_t *buf;
        AR_ASSERT(msg != NULL);
        status = AR_S_YES;
        buf = NULL;
		
		if(__g_ctx.global_io_ctx.on_error != NULL)
		{
                
				ar_int_t len;
                
                AR_va_start(arg_ptr, msg);
				len = AR_vscwprintf(msg, arg_ptr);
				AR_va_end(arg_ptr);
                
				if(len <= 0)
				{
                        status = AR_E_INVAL;
                        goto END_POINT;
				}
                
				buf = (wchar_t*)malloc(sizeof(wchar_t) * (len + 1));	/*this function may be called in AR_UninitMemory,so we just use native CRT function */
                
				if(buf == NULL)
				{
                        status = AR_E_NOMEM;
                        goto END_POINT;
				}
                
				AR_va_start(arg_ptr, msg);
				len = AR_vswprintf_nonalloc(buf, len + 1, msg, arg_ptr);
				AR_va_end(arg_ptr);
				
				if(len <= 0)
				{
                        status = AR_E_INVAL;
                        goto END_POINT;
				}
                
				__g_ctx.global_io_ctx.on_error(level, buf, __g_ctx.global_io_ctx.ctx);
		}
        
END_POINT:
        
        if(buf)
        {
                free(buf);
                buf = NULL;
        }
        
		if(level <= AR_ERR_FATAL)
		{
				AR_abort();
		}
		return status;
        
}






arStatus_t	AR_debug_print(const wchar_t *msg, ...)
{
        
        arStatus_t status;
		va_list arg_ptr;
        wchar_t *buf;
        AR_ASSERT(msg != NULL);
        status = AR_S_YES;
        buf = NULL;
        

		if(__g_ctx.global_io_ctx.on_error != NULL)
		{
                ar_int_t len;
                
                AR_va_start(arg_ptr, msg);
				len = AR_vscwprintf(msg, arg_ptr);
				AR_va_end(arg_ptr);
                
				if(len <= 0)
				{
                        status = AR_E_INVAL;
                        goto END_POINT;
				}
                
				buf = (wchar_t*)malloc(sizeof(wchar_t) * (len + 1)); /*this function may be called in AR_UninitMemory,so we just use native CRT function */
                
				if(buf == NULL)
				{
                        status = AR_E_NOMEM;
                        goto END_POINT;
				}
                
				AR_va_start(arg_ptr, msg);
				len = AR_vswprintf_nonalloc(buf, len + 1, msg, arg_ptr);
				AR_va_end(arg_ptr);
				
				if(len <= 0)
				{
                        status = AR_E_INVAL;
                        goto END_POINT;
				}
				
                __g_ctx.global_io_ctx.on_error(AR_ERR_DEBUG, buf, __g_ctx.global_io_ctx.ctx);
                
		}
        
END_POINT:
        if(buf)
        {
                free(buf);
                buf = NULL;
        }
        
		return status;
        
        
}




arStatus_t AR_printf(const wchar_t *msg,...)
{
		wchar_t *buf;
		ar_int_t len;
		va_list arg_ptr;

		if(__g_ctx.global_io_ctx.on_print != NULL)
		{
		
				
				AR_va_start(arg_ptr, msg);
				len = AR_vscwprintf(msg, arg_ptr);
				AR_va_end(arg_ptr);

				if(len <= 0)
				{
						return AR_E_INVAL;
				}

				buf = AR_NEWARR0(wchar_t, len + 1);

				if(buf == NULL)
				{
						return AR_E_NOMEM;
				}

				AR_va_start(arg_ptr, msg);
				len = AR_vswprintf(buf, len + 1, msg, arg_ptr);
				AR_va_end(arg_ptr);
				
				if(len <= 0)
				{
						AR_DEL(buf);
						return AR_E_INVAL;
				}

				__g_ctx.global_io_ctx.on_print(buf, __g_ctx.global_io_ctx.ctx);
				
				AR_DEL(buf);
		}

		return AR_S_YES;
}


void	AR_check(ar_bool_t cond, const wchar_t *fmt, ...)
{
		wchar_t buf[1024];
		ar_int_t len;
		va_list arg_ptr;
		AR_ASSERT(fmt);
		
		if(!cond)
		{
				AR_va_start(arg_ptr, fmt);
				len = AR_vswprintf_nonalloc(buf, 1024, fmt, arg_ptr);
				AR_va_end(arg_ptr);
				
				if(len < 0)
				{
						buf[0] = L'\0';
				}
				
				AR_error(AR_ERR_FATAL, buf);

		}
}





/***************************************************ctx**********************************/



#if(0)
arIOCtx_t*	AR_global_ioctx()
{
		return &__g_ctx.global_io_ctx;
}


arStatus_t	AR_printf_ctx(arIOCtx_t *ctx, const wchar_t *msg,...)
{
		wchar_t *buf;
		ar_int_t len;
		va_list arg_ptr;
		
		if(ctx && ctx->on_print)
		{
				AR_va_start(arg_ptr, msg);
				len = AR_vscwprintf(msg, arg_ptr);
				AR_va_end(arg_ptr);

				if(len <= 0)
				{
						return AR_E_INVAL;
				}

				buf = AR_NEWARR0(wchar_t, len + 1);

				if(buf == NULL)
				{
						return AR_E_NOMEM;
				}
				
				AR_va_start(arg_ptr, msg);
				AR_vswprintf(buf, len + 1, msg, arg_ptr);
				AR_va_end(arg_ptr);

				ctx->on_print(buf, ctx->ctx);
				
				if(buf)
				{
						AR_DEL(buf);
						buf = NULL;
				}
		}

		return AR_S_YES;
}


arStatus_t	AR_error_ctx(arIOCtx_t *ctx, ar_int_t level, const wchar_t *msg, ...)
{		
		wchar_t buf[1024];
		va_list arg_ptr;
		
		if(ctx && ctx->on_error)
		{
				AR_va_start(arg_ptr, msg);
				
				if(AR_vswprintf(buf, 1024, msg, arg_ptr) <= 0)
				{
						buf[0] = L'\0';
				}
				AR_va_end(arg_ptr);

				ctx->on_error(level, buf, ctx->ctx);
		}

		
		if(level <= AR_ERR_FATAL)
		{
				AR_abort();
		}


		return AR_S_YES;
}
#endif





AR_NAMESPACE_END

