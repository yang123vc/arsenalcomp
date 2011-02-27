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



void	TGU_InitVMBuildinData(tguVMBuildinData_t *data)
{
		AR_ASSERT(data != NULL);
		data->build_in = TGU_CreateSymbTable();

}

void	TGU_UnInitVMBuildinData(tguVMBuildinData_t *data)
{
		AR_ASSERT(data != NULL);

		if(data->build_in)
		{
				TGU_DestroySymbTable(data->build_in);
				data->build_in = NULL;
		}
}







void	TGU_InitVM()
{
		
}

void	TGU_UnInitVM()
{
		

}





void	TGU_Execute(tguMachine_t *vm)
{
		AR_UNUSED(vm);
}

AR_NAMESPACE_END


