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




AR_NAMESPACE_BEGIN


void	TGU_InitVM();
void	TGU_UnInitVM();


/*************************************************************************************************************************************/
struct	__insctruction_tag;
typedef struct	__insctruction_tag		tguIns_t;



/******************************************************Object***********************************************************************/




typedef int_64_t						tguInteger_t;

typedef	double							tguFloat_t;

typedef	bool_t							tguBoolean_t;

struct __tengu_string_tag;
typedef struct __tengu_string_tag		tguString_t;

struct __tengu_table_tag;
typedef struct __tengu_table_tag		tguTable_t;

struct __tengu_userdata_tag;
typedef struct __tengu_userdata_tag		tguUserData_t;

struct __tengu_function_tag;
typedef	struct __tengu_function_tag		tguFunc_t;




typedef union __tengu_value_tag
{
		tguInteger_t	integer_num;
		tguFloat_t		float_num;
		tguBoolean_t	boolean;
		tguString_t		*str;
		tguTable_t		*table;
		tguFunc_t		*function;
		tguUserData_t	*userdata;
}tguValue_t;


enum
{
		TGU_VM_TYPE_NULL,
		TGU_VM_TYPE_BOOL,
		TGU_VM_TYPE_INTEGER,
		TGU_VM_TYPE_FLOAT,
		TGU_VM_TYPE_STR,
		TGU_VM_TYPE_TABLE,
		TGU_VM_TYPE_FUNC,
		TGU_VM_TYPE_USERDATA
};


typedef struct __tgu_object_tag
{
		tguValue_t		val;
		uint_8_t		type;
}tguObject_t;


/***********************************************************************************************************************************/
struct __tengu_function_tag
{
		tguIns_t		*code;
		size_t			code_size;
};

/******************************************************OpCode***********************************************************************/




typedef enum 
{
		TGU_VM_OP_PUSH_NULL			,		
		TGU_VM_OP_PUSH_BOOL			,
		TGU_VM_OP_PUSH_INTEGER		,
		TGU_VM_OP_PUSH_FLOAT		,
		TGU_VM_OP_PUSH_STRING		,
		TGU_VM_OP_PUSH_FUNCTION		,
		
		TGU_VM_OP_POP				,
		
		TGU_VM_OP_ADJUST			,
		
		TGU_VM_OP_GET_TABLE			,
		TGU_VM_OP_SET_TABLE			,
		
		TGU_VM_OP_SET_LIST			,	
		TGU_VM_OP_NEW_TABLE			,

		TGU_VM_OP_EQ					,
		TGU_VM_OP_LT					,
		TGU_VM_OP_LE					,
		TGU_VM_OP_GT					,
		TGU_VM_OP_GE					,
		TGU_VM_OP_ADD					,
		TGU_VM_OP_SUB					,
		TGU_VM_OP_MUL					,
		TGU_VM_OP_DIV					,
		TGU_VM_OP_MOD					,
		
		TGU_VM_OP_UMINUS				,
		TGU_VM_OP_NOT					,
		
		TGU_VM_OP_JMP					,
		TGU_VM_OP_TEST					,
		TGU_VM_OP_TESTTEST				,

		TGU_VM_OP_CALL					,
		TGU_VM_OP_RETURN				
}tguVMOpCode_t;




struct	__insctruction_tag
{
		int_8_t			op;
		int_16_t		arg1;
		int_16_t		arg2;
};


typedef tguObject_t		tguStackID_t;

typedef struct __call_info_tag
{
		tguIns_t		*pc;		
		tguStackID_t	*fp;
		tguStackID_t	*sp;
}tguCallInfo_t;

#define	TGU_MAX_CALL			256

typedef struct __tengu_call_stack_tag
{
		tguStackID_t	*start;
		tguStackID_t	*last;
		tguStackID_t	*sp;	/*last used slot*/
}tguCallStack_t;

#define	TGU_STACK_SIZE(_stk)	((size_t) ( (_stk)->top - (_stk)->start + 1) )

#define	TGU_MAX_STACK_SIZE		4096



struct	__tengu_machine_tag
{
/***************************Run************************/
		tguIns_t		*pc;
		tguStackID_t	*fp;
		tguStackID_t	*sp;

		tguCallStack_t	stack;
		tguCallInfo_t	ci[TGU_MAX_CALL];
		size_t			call_cnt;

/**************************POOL**********************/		
		tguString_t		*str_root;
		tguTable_t		*table_root;
		tguFunc_t		*function_root;
		tguUserData_t	*userdata_root;
/************************Value***********************/
		tguTable_t		*global_table;
};


void	TGU_Execute(tguMachine_t *vm);



AR_NAMESPACE_END

#endif
