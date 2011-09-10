
 
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


#include "../Arsenal/Common/common.h"
#include "../Arsenal/Lex/lex.h"
#include "../Arsenal/Parser/parser.h"
#include "../Arsenal/Parser/lr_action.h"

AR_NAMESPACE_BEGIN

#include "calc_test3_impl.h"



void calc3_test()
{
		wchar_t buf[1024];
		int x;
		while(true)
		{
				_getws(buf);
				if(AR_wcscmp(buf, L"quit") == 0)break;
				x = calc(buf);
				AR_printf(L"result value == %d\r\n", x);
		}
		
} 

AR_NAMESPACE_END
