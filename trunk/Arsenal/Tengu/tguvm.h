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


#ifndef __ARSENAL_TGUVM_H__
#define __ARSENAL_TGUVM_H__

#include "tengu.h"
#include "tgusyntree.h"



AR_NAMESPACE_BEGIN


void	TGU_InitVM();
void	TGU_UnInitVM();


/*************************************************************************************************************************************/
struct	__insctruction_tag;
typedef struct	__insctruction_tag		tguVMIns_t;



/******************************************************Object***********************************************************************/




typedef int_64_t								tguVMInteger_t;

typedef	double									tguVMFloat_t;

typedef	bool_t									tguVMBoolean_t;


struct __tengu_vm_string_tag;
typedef struct __tengu_vm_string_tag			tguVMString_t;

struct __tengu_vm_binary_tag;
typedef struct __tengu_vm_binary_tag			tguVMBinary_t;

struct __tengu_vm_userdata_tag;
typedef struct __tengu_vm_userdata_tag			tguVMUserData_t;



struct __tengu_vm_list_tag;
typedef struct __tengu_vm_list_tag				tguVMList_t;


struct __tengu_vm_table_tag;
typedef struct __tengu_vm_table_tag				tguVMTable_t;



struct __tengu_vm_function_tag;
typedef	struct __tengu_vm_function_tag			tguVMFunc_t;

struct __tengu_vm_module_tag;
typedef struct __tengu_vm_module_tag			tguVMModule_t;



typedef union __tengu_value_tag
{
		tguVMInteger_t		integer_num;
		tguVMFloat_t		float_num;
		tguVMBoolean_t		boolean;

		/*GC Object*/
		tguVMString_t		*str;
		tguVMUserData_t		*userdata;
		tguVMBinary_t		*binary;

		tguVMList_t			*list;
		tguVMTable_t		*table;
		
		tguVMFunc_t			*function;
		
		tguVMModule_t		*module;
		
		tguCFunction_t		c_function;
}tguVMValue_t;




enum
{
		TGU_VM_TYPE_NULL,
		TGU_VM_TYPE_BOOL,
		TGU_VM_TYPE_INTEGER,
		TGU_VM_TYPE_FLOAT,

		TGU_VM_TYPE_STR,
		TGU_VM_TYPE_TABLE,
		TGU_VM_TYPE_LIST,
		TGU_VM_TYPE_BINARY,
		TGU_VM_TYPE_FUNC,
		TGU_VM_TYPE_USERDATA,

		TGU_VM_TYPE_MODULE,

		TGU_VM_TYPE_C_FUNC
	
};


typedef struct __tgu_object_tag
{
		tguVMValue_t	val;
		uint_8_t		type;
}tguVMObject_t;


/*****************************************************Object definition***********************************************************************/

typedef enum 
{
		TGU_MEM_UNMARK,
		TGU_MEM_MARKED,
		TGU_MEM_FIXED
}tguMemMark_t;


struct __tengu_vm_string_tag
{
		tguMemMark_t			mark;
		tguVMString_t			*gc_next;
		tguVMString_t			*hash_next;


		wchar_t					*str;
		size_t					n;
		uint_t					hash;
};


tguVMString_t*	TGU_CreateVMString(tguMachine_t *vm, const wchar_t *str);
tguVMString_t*	TGU_CreateVMStringN(tguMachine_t *vm, const wchar_t *str, size_t n);
tguVMString_t*	TGU_CreateFixedString(tguMachine_t *vm, const wchar_t *str);




struct __tengu_vm_userdata_tag
{
		tguMemMark_t			mark;
		tguVMUserData_t			*next;


		void					*data;
		int_t					tag;

		
};


tguVMUserData_t*	TGU_CreateVMUserData(tguMachine_t *vm, void *user_data, int_t tag);



struct __tengu_vm_binary_tag
{
		tguMemMark_t			mark;
		tguVMBinary_t	*next;
		
		arBuffer_t		*buf;
};


tguVMBinary_t*	TGU_CreateVMBinary(tguMachine_t *vm);





struct __tengu_vm_list_tag
{
		tguMemMark_t		mark;
		tguVMList_t			*next;


		tguVMObject_t	**lst;
		size_t			cnt;
		size_t			cap;
		
};

tguVMList_t*	TGU_CreateVMList(tguMachine_t *vm, size_t nsize);
bool_t			TGU_SetVMList(tguVMList_t *lst, size_t idx, tguVMObject_t *obj);
tguVMObject_t*	TGU_GetVMList(tguVMList_t *lst, size_t idx);
size_t			TGU_AppendVMList(tguVMList_t *lst, tguVMObject_t *obj);
bool_t			TGU_RemoveVMList(tguVMList_t *lst, size_t idx);
int_t			TGU_FindVMList(tguVMList_t	 *lst, tguVMObject_t *obj);



typedef struct __tengu_vm_table_node_tag
{
		tguVMObject_t *key;
		tguVMObject_t *val;
		
		struct __tengu_vm_table_node_tag		*next;
}tguVMTableNode_t;


struct __tengu_vm_table_tag
{
		tguMemMark_t	mark;
		tguVMTable_t	*next;

		tguVMTableNode_t		**bucket;
		size_t					bucket_cnt;
		size_t					item_cnt;
};


tguVMTable_t*	TGU_CreateVMTable(tguMachine_t *vm, size_t estimate_cnt);
void			TGU_DestroyVMTable(tguVMTable_t *tbl);

tguVMObject_t*	TGU_GetTable(tguVMTable_t *tbl, tguVMObject_t *key);
tguVMObject_t*	TGU_SetTable(tguVMTable_t *tbl, tguVMObject_t *key);
bool_t			TGU_RemoveTable(tguVMTable_t *tbl, tguVMObject_t *key);







/***********************************************************************************************************************************/




typedef struct __tengu_vm_gc_pool_tag
{
		tguVMString_t		*str_root;
		tguVMFunc_t			*function_root;
		tguVMBinary_t		*binary_root;
		tguVMUserData_t		*userdata_root;
		tguVMTable_t		*table_root;
}tguVMGCPool_t;


typedef struct __tengu_vm_constant_pool_tag
{
		tguVMInteger_t		*integer_set;
		size_t				int_cnt;
		size_t				int_cap;
		
		tguVMFloat_t		*float_set;
		size_t				float_cnt;
		size_t				float_cap;

		const wchar_t		**str_set;
		size_t				str_cnt;
		size_t				str_cap;
}tguVMConstPool_t;



/*************************************************************************************************************************************/


/******************************************************OpCode***********************************************************************/

typedef enum 
{
		TGU_VM_OP_PUSH				,			/*根据arg1决定压入什么*/
		TGU_VM_OP_POP				,			/*无参数*/
		TGU_VM_OP_ADJUST			,			

		TGU_VM_OP_GET				,			/*根据栈上参数决定是否失败*/
		TGU_VM_OP_SET				,			/*同上*/


		TGU_VM_OP_EQ				,
		TGU_VM_OP_LT				,
		TGU_VM_OP_LE				,
		TGU_VM_OP_GT				,
		TGU_VM_OP_GE				,
		TGU_VM_OP_ADD				,
		TGU_VM_OP_SUB				,
		TGU_VM_OP_MUL				,
		TGU_VM_OP_DIV				,
		TGU_VM_OP_MOD				,
		
		TGU_VM_OP_UMINUS			,
		TGU_VM_OP_NOT				,
		
		TGU_VM_OP_JMP				,
		TGU_VM_OP_TEST				,
		TGU_VM_OP_TESTTEST			,

		TGU_VM_OP_CALL				,
		TGU_VM_OP_RETURN			,
		
		TGU_VM_OP_NOP
}tguVMOpCode_t;




struct	__insctruction_tag
{
		uint_8_t		op;
		uint_8_t		arg1;
		int_16_t		arg2;
};


#define TGU_OP_MIN_ARG2			AR_INT16_MIN
#define TGU_OP_MAX_ARG2			AR_INT16_MAX





typedef tguVMObject_t		tguStackID_t;


typedef struct __call_info_tag
{
		tguVMIns_t		*pc;		
		tguStackID_t	*fp;
		tguStackID_t	*sp;
}tguCallInfo_t;

#define	TGU_MAX_CALL			512


typedef struct __tengu_call_stack_tag
{
		tguStackID_t	*start;
		tguStackID_t	*last;
		tguStackID_t	*sp;	/*last used slot*/
}tguCallStack_t;

#define	TGU_STACK_SIZE(_stk)	((size_t) ( (_stk)->top - (_stk)->start + 1) )

#define	TGU_MAX_STACK_SIZE		8192










struct	__tengu_machine_tag
{
/***************************Run************************/
		tguVMIns_t		*pc;
		tguStackID_t	*fp;
		tguStackID_t	*sp;

		tguCallStack_t	stack;
		tguCallInfo_t	ci[TGU_MAX_CALL];
		size_t			call_cnt;

		
		tguVMGCPool_t			gc_pool;
		tguVMConstPool_t		const_pool;
/************************Value***********************/
		tguVMTable_t		*global_table;

		arString_t			*last_error;
};




bool_t	TGU_Execute(tguMachine_t *vm);



AR_NAMESPACE_END

#endif
