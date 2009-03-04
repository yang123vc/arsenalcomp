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

static void AR_STDCALL __def_disp_handler(const wchar_t *msg)
{
		wprintf(L"%s\n", msg);
}

static ArPrintHandler_t	disp_handler = __def_disp_handler;


ArPrintHandler_t AR_SetPrintHandler(ArPrintHandler_t hdl)
{
		ArPrintHandler_t	tmp = disp_handler;
		disp_handler = hdl;
		return tmp;
}



void AR_Print(const wchar_t *msg,...)
{
		wchar_t buf[AR_MAXTRC_LEN];
		va_list arg_ptr;
		va_start(arg_ptr, msg);
		ar_vswprintf(buf, AR_MAXTRC_LEN, msg, arg_ptr);
		va_end(arg_ptr);
		disp_handler(buf);
}


/******************************************************Error**************************************************************/


static void AR_STDCALL __def_err_handler(int level, const wchar_t *msg)
{
		AR_Print(L"%s\n", msg);
		
		if(level == AR_FATAL) abort();
}

static ArErrorHandler_t	err_handler = __def_err_handler;

ArErrorHandler_t AR_SetErrorHandler(ArErrorHandler_t hdl)
{
		ArErrorHandler_t	tmp = err_handler;
		err_handler = hdl;
		return tmp;
}


void AR_Error(int level,  const wchar_t *msg, ...)
{
		wchar_t buf[AR_MAXTRC_LEN];
		
		va_list arg_ptr;
		va_start(arg_ptr, msg);
		ar_vswprintf(buf, AR_MAXTRC_LEN, msg, arg_ptr);
		va_end(arg_ptr);
		err_handler(level, buf);

}

AR_NAMESPACE_END