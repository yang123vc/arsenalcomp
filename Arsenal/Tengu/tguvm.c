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


void	TGU_InitVM()
{
		
}

void	TGU_UnInitVM()
{
		

}





void	TGU_Execute(tguMachine_t *vm)
{

#if(0)
		tguFunc_t		*curr_func;
		tguIns_t		*pc;
		tguStackID_t	*top;
		
		AR_ASSERT(vm != NULL);

		top = vm->stack.top;

		AR_ASSERT( TGU_STACK_SIZE(&(vm->stack)) > nargs + 1);
		AR_ASSERT((top-nargs)->stk_type == TGU_STK_VALUE && (top-nargs)->object.type == TGU_TYPE_FUNC);
		curr_func = (top-nargs)->object.val.function;
		pc = curr_func->code;
		AR_ASSERT(pc != NULL);

		for(;;)
		{
				switch(pc->op)
				{
				case OP_RETURN:
				{
						break;
				}
				case OP_CALL:
				{
						*++top->stk_type = 
						break;
				}
				case OP_PUSHNULL:
				{
						break;
				}
				case OP_POP:
				{
						break;
				}
				case OP_PUSHSTRING:
				{
						break;
				}
				case OP_PUSHNUM:
				{
						break;
				}
				case OP_GETLOCAL:
				{
						break;
				}
				case OP_GETGLOBAL:
				{
						break;
				}
				case OP_GETTABLE:
				{
						break;
				}
				case OP_CREATETABLE:
				{
						break;
				}
				case OP_SETLOCAL:
				{
						break;
				}
				case OP_SETGLOBAL:
				{
						break;
				}
				case OP_SETTABLE:
				{
						break;
				}
				case OP_SETLIST:
				{
						break;
				}
				case OP_SETMAP:
				{
						break;
				}
				case OP_ADD:
				{
						break;
				}
				case OP_SUB:
				{
						break;
				}
				case OP_MUL:
				{
						break;
				}
				case OP_DIV:
				{
						break;
				}
				case OP_MOD:
				{
						break;
				}
				case OP_UMINUS:
				{
						break;
				}
				case OP_NOT:
				{
						break;
				}
				case OP_JMPNE:
				{
						break;
				}
				case OP_JMPEQ:
				{
						break;
				}
				case OP_JMPLT:
				{
						break;
				}
				case OP_JMPLE:
				{
						break;
				}
				case OP_JMPGT:
				{
						break;
				}
				case OP_JMPGE:
				{
						break;
				}
				case OP_JMPT:
				{
						break;
				}
				case OP_JMPF:
				{
						break;
				}
				case OP_JMPONT:
				{
						break;
				}
				case OP_JMPONF:
				{
						break;
				}
				case OP_JMP:
				{
						break;
				}
				case OP_PUSHNULLJMP:
				{
						break;
				}
				default:
						AR_ASSERT(false);
				}
		}
#endif
		
}

AR_NAMESPACE_END


