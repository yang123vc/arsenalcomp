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



const tguVMObject_t	__g_true_object = {TGU_VM_TYPE_BOOL, {true} };
const tguVMObject_t	__g_false_object = {TGU_VM_TYPE_BOOL, {false} };
const tguVMObject_t	__g_null_object = { TGU_VM_TYPE_NULL, {false }};






void	TGU_InitVM()
{

}

void	TGU_UnInitVM()
{


}



arStatus_t TGU_FormatVMError(tguMachine_t *vm, const wchar_t *fmt,...)
{
		arStatus_t status;
		va_list arg_ptr;

		AR_va_start(arg_ptr, fmt);
		status = AR_VFormatString(vm->last_error, fmt, arg_ptr);
		AR_va_end(arg_ptr);
		return status;
}



#if(0)
static void __set_run_env(tguMachine_t *vm,  tguVMFunc_t *func)
{
		AR_ASSERT(vm != NULL && func != NULL);
		vm->curr_func = func;
		vm->pc = func->code;
		vm->fp = vm->stack.start;
}


static arStatus_t	__run_vm(tguMachine_t *vm)
{
		arStatus_t status;
		tguStackID_t	*top;
		AR_ASSERT(vm != NULL);

		status = AR_S_YES;

		top = vm->stack.top;

		for(;;)
		{
				tguVMIns_t *ins = vm->pc++;

				switch(ins->op)
				{
				case TGU_VM_OP_PUSH:			/*根据arg1决定压入什么, arg2意思取决于arg1*/
				{
						switch(ins->arg1)
						{
						case TGU_VM_TYPE_NULL:
								*top++ = __g_null_object;
								break;
						case TGU_VM_TYPE_BOOL:
						{
								if(ins->arg2)
								{
										*top++ = __g_true_object;
								}else
								{
										*top++ = __g_false_object;
								}
						}
								break;
						case TGU_VM_TYPE_INTEGER:
						{
								top->type = TGU_VM_TYPE_INTEGER;
								top->val.integer_num = TGU_GetIntegerFromConstantPool(&vm->const_pool, (size_t)ins->arg2);
								top++;
						}
								break;
						case TGU_VM_TYPE_FLOAT:
						{
								top->type = TGU_VM_TYPE_FLOAT;
								top->val.float_num = TGU_GetFloatFromConstantPool(&vm->const_pool, (size_t)ins->arg2);
								top++;
						}
								break;
						case TGU_VM_TYPE_STRING:
						{
								top->type = TGU_VM_TYPE_STRING;
								top->val.str = TGU_GetStringFromConstantPool(&vm->const_pool, (size_t)ins->arg2);
								top++;
						}
								break;
						case TGU_VM_TYPE_LIST:
						{
								top->type = TGU_VM_TYPE_LIST;
								top->val.list = TGU_CreateVMList(vm, 0);
								if(top->val.list == NULL)
								{
										status = AR_E_NOMEM;
										goto END_POINT; 
								}
						}
								break;
						case TGU_VM_TYPE_TABLE:
						{
								top->type = TGU_VM_TYPE_TABLE;
								top->val.table = TGU_CreateVMTable(vm, 0);
								if(top->val.table == NULL)
								{
										status = AR_E_NOMEM;
										goto END_POINT; 
								}
						}
								break;
						case TGU_VM_TYPE_BINARY:
						{
								top->type = TGU_VM_TYPE_BINARY;
								top->val.binary = TGU_CreateVMBinary(vm);
								if(top->val.binary == NULL)
								{
										status = AR_E_NOMEM;
										goto END_POINT; 
								}
						}
								break;
						case TGU_VM_TYPE_USERDATA:
						case TGU_VM_TYPE_FUNC:
						case TGU_VM_TYPE_C_FUNC:
						default:
								TGU_FormatVMError(vm, L"Invalid push type : %u\r\n", (ar_uint_32_t)ins->arg1);
								status = AR_E_TGUINVOPCODE;
								goto END_POINT;
								break;
						}
				}
						break;
				case TGU_VM_OP_POP:					/*arg2为调整数量*/
				{
						top -= ins->arg2;
				}
						break;
				case TGU_VM_OP_GET_LOCAL:
				{
						*top++ = *(vm->fp + ins->arg2);
				}
						break;
				case TGU_VM_OP_SET_LOCAL:
				{
						*(vm->fp + ins->arg2) = *(--top);
				}
						break;
				case TGU_VM_OP_GET_MODULE:
				{
						tguVMObject_t *obj;
						arStatus_t ret;
						const tguVMString_t *module, *var;
						module = TGU_GetStringFromConstantPool(&vm->const_pool, ins->arg1);
						var = TGU_GetStringFromConstantPool(&vm->const_pool, ins->arg2);
						
						obj = NULL;
						ret = AR_S_NO;
						if(module != NULL && var != NULL)
						{
								ret = TGU_GetModuleGlobalFromVM(vm, module, var, &obj);
						}else
						{
								ret = AR_E_TGUINVOPCODE;
								TGU_FormatVMError(vm, L"tengu vm internal error\r\n");
						}
						
						if(ret != AR_S_YES)
						{
								status = ret;
								goto END_POINT;
						}

						*top++ = *obj;

				}
						break;
				case TGU_VM_OP_SET_MODULE:
				{
						arStatus_t ret;
						const tguVMString_t *module, *var;
						module = TGU_GetStringFromConstantPool(&vm->const_pool, ins->arg1);
						var = TGU_GetStringFromConstantPool(&vm->const_pool, ins->arg2);
						
						ret = AR_S_NO;
						if(module != NULL && var != NULL)
						{
								ret = TGU_SetModuleGlobalToVM(vm, module, var, top - 1);
						}else
						{
								ret = AR_E_TGUINVOPCODE;
								TGU_FormatVMError(vm, L"tengu vm internal error\r\n");
						}
						
						if(ret != AR_S_YES)
						{
								status = ret;
								goto END_POINT;
						}

						top--;
				}
						break;
				case TGU_VM_OP_IMPORT_MODULE:
				{
						arStatus_t ret;

						tguVMObject_t	*val;
						const tguVMString_t *module_name, *module_path;
						module_name = NULL;
						module_path = NULL;
						ret = AR_S_YES;
						
						if(ins->arg1 == 1)
						{
								val = (top-1);

								if(val->type != TGU_VM_TYPE_STRING)
								{
										ret = AR_E_TGUINVOPCODE;
										goto ERROR_IMPORT_POINT;
								}

								module_name = val->val.str;
								top -= 1;
						}else
						{
								AR_ASSERT(ins->arg1 == 2);

								val = (top-2);
								if(val->type != TGU_VM_TYPE_STRING)
								{
										ret = AR_E_TGUINVOPCODE;
										goto ERROR_IMPORT_POINT;
								}

								module_name = val->val.str;

								val = (top-1);
								if(val->type != TGU_VM_TYPE_STRING)
								{
										ret = AR_E_TGUINVOPCODE;
										goto ERROR_IMPORT_POINT;
								}
								module_path = val->val.str;

								top -= 2;
						}
						
						ret = TGU_ImportModule(vm, module_path, module_name);
						
						if(ret != AR_S_YES)/*模块载入失败，返回null*/
						{
								*top++ = __g_null_object;
								ret = AR_S_YES;
						}else
						{
								top->type = TGU_VM_TYPE_MODULE;
								top->val.module = TGU_FindModuleFromVM(vm, module_name);
								top++;

								AR_ASSERT(top->val.module != NULL);
								
						}

ERROR_IMPORT_POINT:
						if(ret != AR_S_YES)
						{
								ret = AR_E_TGUINVOPCODE;
								TGU_FormatVMError(vm, L"tengu vm internal error\r\n");
								
								status = ret;
								goto END_POINT;
						}

				}
						break;
				case TGU_VM_OP_INIT_DATA:
				{
						ar_int_t n = (ar_int_t)ins->arg2;

						switch(ins->arg1)
						{
						case TGU_VM_TYPE_LIST:
						{
								tguVMList_t *list;
								if((top - n - 1)->type != TGU_VM_TYPE_LIST)
								{
										TGU_FormatVMError(vm, L"Object does not match target type\r\n");
										status = AR_E_TGUINVOPRAND;
										goto END_POINT;
								}

								list = (top - n - 1)->val.list;
								while(n > 0)
								{
										status = TGU_AppendVMList(vm, list, top - n);
										if(status != AR_S_YES)
										{
												goto END_POINT;
										}
										n--;
								}
								top -= n;
						}
								break;
						case TGU_VM_TYPE_TABLE:
						{
								tguVMTable_t *table;
								if((top - n * 2 - 1)->type != TGU_VM_TYPE_TABLE)
								{
										TGU_FormatVMError(vm, L"Object does not match target type\r\n");
										status = AR_E_TGUINVOPRAND;
										goto END_POINT;
								}

								table = (top - n * 2 - 1)->val.table;

								while(n > 0)
								{
										tguVMObject_t *obj = TGU_SetVMTable(vm, table, top - n * 2);

										*obj = *(top - n * 2 + 1);
										if(obj == NULL)
										{
												status = AR_E_NOMEM;
												goto END_POINT;
										}

										
										n--;
								}

								top -= 2 * n;

						}
								break;
						default:
								TGU_FormatVMError(vm, L"Invalid init type : %u\r\n", (ar_uint_32_t)ins->arg1);
								status = AR_E_TGUINVOPCODE;
								goto END_POINT;
								break;
						}
				}
						break;
				case TGU_VM_OP_EQ:
				{
						arStatus_t ret;
						top -= 2;
						ret = TGU_ObjectIsEqual(vm, top, top + 1);
						if(ret == AR_S_YES)
						{
								*top++ = __g_true_object;
						}else if(ret == AR_S_NO)
						{
								*top++ = __g_false_object;
						}else
						{
								status = ret;
								goto END_POINT;
						}
				}
						break;
				case TGU_VM_OP_LT:
				{
						arStatus_t ret;
						top -= 2;
						ret = TGU_ObjectIsLessThan(vm, top, top + 1);
						if(ret == AR_S_YES)
						{
								*top++ = __g_true_object;
						}else if(ret == AR_S_NO)
						{
								*top++ = __g_false_object;
						}else
						{
								status = ret;
								goto END_POINT;
						}
				}
						break;
				case TGU_VM_OP_LE:
				{
						arStatus_t ret;
						top -= 2;
						ret = TGU_ObjectIsLessThan(vm, top + 1, top);/*a <= b equal !(b < a)*/
						if(ret == AR_S_YES)
						{
								*top++ = __g_false_object;
						}else if(ret == AR_S_NO)
						{
								*top++ = __g_true_object;
						}else
						{
								status = ret;
								goto END_POINT;
						}
				}
						break;
				case TGU_VM_OP_GT:
				{
						arStatus_t ret;
						top -= 2;
						ret = TGU_ObjectIsLessThan(vm, top + 1, top); /*a > b equal b < a*/
						
						if(ret == AR_S_YES)
						{
								*top++ = __g_true_object;
						}else if(ret == AR_S_NO)
						{
								*top++ = __g_false_object;
						}else
						{
								status = ret;
								goto END_POINT;
						}
				}
						break;
				case TGU_VM_OP_GE:
				{
						arStatus_t ret;
						top -= 2;
						ret = TGU_ObjectIsLessThan(vm, top, top + 1); /*a >= b equal !(a < b)*/
						
						if(ret == AR_S_YES)
						{
								*top++ = __g_false_object;
						}else if(ret == AR_S_NO)
						{
								*top++ = __g_true_object;
						}else
						{
								status = ret;
								goto END_POINT;
						}
				}
						break;
				case TGU_VM_OP_ADD:
				{
						arStatus_t ret;
						top -= 2;
						ret = TGU_AddObjects(vm, top, top + 1, top + 2);
						
						if(ret != AR_S_YES)
						{
								status = ret;
								goto END_POINT;
						}
						*top = *(top + 2);
						top++;
				}
						break;
				case TGU_VM_OP_SUB:
				{
						arStatus_t ret;
						top -= 2;
						ret = TGU_SubObjects(vm, top, top + 1, top + 2);
						
						if(ret != AR_S_YES)
						{
								status = ret;
								goto END_POINT;
						}
						*top = *(top + 2);
						top++;
				}
						break;
				case TGU_VM_OP_MUL:
				{
						arStatus_t ret;
						top -= 2;
						ret = TGU_MulObjects(vm, top, top + 1, top + 2);
						
						if(ret != AR_S_YES)
						{
								status = ret;
								goto END_POINT;
						}
						*top = *(top + 2);
						top++;
				}
						break;
				case TGU_VM_OP_DIV:
				{
						arStatus_t ret;
						top -= 2;
						ret = TGU_DivObjects(vm, top, top + 1, top + 2);
						
						if(ret != AR_S_YES)
						{
								status = ret;
								goto END_POINT;
						}
						*top = *(top + 2);
						top++;
				}
						break;
				case TGU_VM_OP_MOD:
				{
						arStatus_t ret;
						top -= 2;
						ret = TGU_ModObjects(vm, top, top + 1, top + 2);
						
						if(ret != AR_S_YES)
						{
								status = ret;
								goto END_POINT;
						}
						*top = *(top + 2);
						top++;
				}
						break;
				case TGU_VM_OP_UMINUS:
				{
						arStatus_t ret;
						
						ret = TGU_UminusObject(vm, top - 1);

						if(ret != AR_S_YES)
						{
								status = ret;
								goto END_POINT;
						}
				}
						break;
				case TGU_VM_OP_NOT:
				{
						arStatus_t ret;
						
						ret = TGU_NotObject(vm, top - 1);

						if(ret != AR_S_YES)
						{
								status = ret;
								goto END_POINT;
						}
				}
						break;
				case TGU_VM_OP_JMP:
				{
						vm->pc += (ar_int_t)ins->arg2;
				}
						break;
				case TGU_VM_OP_JMP_TRUE:
				{
						arStatus_t ret;
						ret = TGU_ObjectIsTrue(vm, top);

						if(ret == AR_S_YES)
						{
								vm->pc += (ar_int_t)ins->arg2;
						}
				}
						break;
				case TGU_VM_OP_JMP_FALSE:
				{
						arStatus_t ret;
						ret = TGU_ObjectIsTrue(vm, top);

						if(ret == AR_S_NO)
						{
								vm->pc += (ar_int_t)ins->arg2;
						}
				}
						break;
				
				case TGU_VM_OP_DOT:
				{
						tguVMObject_t *tbl, *key, *result;
						tbl = top - 2;
						key = top - 1;
						top -= 2;

						if((status = TGU_DotObject(vm, tbl, key, &result)) != AR_S_YES)
						{
								goto END_POINT;
						}
						*top++ = *result;
				}
						break;
				case TGU_VM_OP_INDEX:
				{
						tguVMObject_t *tbl, *index, *result;
						tbl = top - 2;
						index = top - 1;
						top -= 2;

						if((status = TGU_DotObject(vm, tbl, index, &result)) != AR_S_YES)
						{
								goto END_POINT;
						}
						*top++ = *result;
				}
						break;
				case TGU_VM_OP_CALL:
				{

				}
						break;
				case TGU_VM_OP_RETURN:
				{

				}
						break;
				case TGU_VM_OP_NOP:
				{

				}
						break;
				default:
						status = AR_E_INVAL;
						goto END_POINT;
						break;
				}
		}

END_POINT:
		return status;
}







arStatus_t		TGU_ExecuteVM(tguMachine_t *vm, const tguVMString_t *module_name)
{
		arStatus_t status;

		tguVMObject_t	*module;
		tguVMObject_t	*val;

		tguVMFunc_t		*function;

		AR_ASSERT(vm != NULL && module_name != NULL);

		status = AR_S_YES;
		module = NULL;
		val = NULL;
		function = NULL;

		module = TGU_GetVMTableByString(vm, vm->global_table, (tguVMString_t*)module_name);

		if(module == NULL || module->type != TGU_VM_TYPE_MODULE)
		{
				TGU_FormatVMError(vm, L"Invalid module name : %ls\r\n", module_name->str);
				status = AR_E_TGUINVCALL;
				goto RET_POINT;
		}

		val = TGU_GetVMTableByCStr(vm, module->val.module->table, TGU_DEFAULT_FUNCTION_NAME);

		if(val == NULL || val->type != TGU_VM_TYPE_FUNC)
		{
				TGU_FormatVMError(vm, L"Invalid function name : %ls\r\n", TGU_DEFAULT_FUNCTION_NAME);
				status = AR_E_TGUINVCALL;
				goto RET_POINT;
		}

		function = val->val.function;
		
		__set_run_env(vm, function);

		status = __run_vm(vm);

RET_POINT:
		return status;

}

#endif





AR_NAMESPACE_END


