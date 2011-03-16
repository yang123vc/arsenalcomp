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

#include "tguvm.h"


AR_NAMESPACE_BEGIN


/************************************************************Buildin****************************************************/









void	TGU_InitVM()
{
		
}

void	TGU_UnInitVM()
{
		

}



#define __FORMAT_ERRMSG(_vm, _msg)		do{		AR_FormatString((_vm)->last_error, L"%ls", (_msg)); }while(0)

bool_t	TGU_Execute(tguMachine_t *vm)
{
		bool_t is_ok = false;
		AR_ASSERT(vm != NULL);

		if(vm->pc == NULL)
		{
				is_ok = false;
				__FORMAT_ERRMSG(vm, "Invlaid Tengu VM!\r\n");
				goto RET_POINT;
		}


RET_POINT:
		return is_ok;
}

AR_NAMESPACE_END


