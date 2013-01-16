
 
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

#include "v3_checker_impl.h"


ar_bool_t v3check(const wchar_t *expr);

void v3checker_test()
{
		wchar_t expr[1024];

		while(true)
		{
				_getws(expr);

				if(AR_wcscmp(expr, L"quit") == 0)
				{
						break;
				}

				ar_bool_t is_valid = v3check(expr);

				if(is_valid)
				{
						AR_printf(L"'%ls' is valid\r\n", expr);
				}else
				{
						AR_printf(L"'%ls' failed\r\n", expr);
				}
		}
		
} 



AR_NAMESPACE_END
