/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#include "common.h"


static void AR_STDCALL __def_err_handler(int level, const wchar_t *msg)
{
		wprintf(L"%s\n", msg);
		
		if(level == AR_FATAL) abort();
}

static ar_errhandler_t	err_handler = __def_err_handler;

ar_errhandler_t ar_set_errhdl(ar_errhandler_t hdl)
{
		ar_errhandler_t	tmp = err_handler;
		err_handler = hdl;
		return tmp;
}

void ar_error(int level,  const wchar_t *msg, ...)
{
		wchar_t buf[AR_MAXTRC_LEN];
		
		va_list arg_ptr;
		va_start(arg_ptr, msg);
		ar_vswprintf(buf, AR_MAXTRC_LEN, msg, arg_ptr);
		va_end(arg_ptr);
		err_handler(level, buf);

}



static void AR_STDCALL __def_disp_handler(const wchar_t *msg)
{
		wprintf(L"%s\n", msg);
}

static ar_disphandler_t	disp_handler = __def_disp_handler;


ar_disphandler_t ar_set_disphdl(ar_disphandler_t hdl)
{
		ar_disphandler_t	tmp = disp_handler;
		disp_handler = hdl;
		return tmp;
}



void ar_printf(const wchar_t *msg,...)
{
		wchar_t buf[AR_MAXTRC_LEN];
		va_list arg_ptr;
		va_start(arg_ptr, msg);
		ar_vswprintf(buf, AR_MAXTRC_LEN, msg, arg_ptr);
		va_end(arg_ptr);
		disp_handler(buf);
}


/******************************************************String**************************************************************/

