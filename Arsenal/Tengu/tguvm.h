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





typedef	double							tguFloat_t;
typedef	int_64_t						tguInt_t;

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
		tguFloat_t		float_num;
		tguInt_t		int_num;
		tguBoolean_t	boolean;
		tguString_t		*str;
		tguTable_t		*table;
		tguFunc_t		*function;
		tguUserData_t	*userdata;
}tguValue_t;


enum
{
		TGU_TYPE_NIL,
		TGU_TYPE_FLOAT,
		TGU_TYPE_INT,
		TGU_TYPE_BOOL,
		TGU_TYPE_STR,
		TGU_TYPE_TABLE,
		TGU_TYPE_FUNC,
		TGU_TYPE_USERDATA
};


typedef struct __tgu_object_tag
{
		uint_8_t		type;
		tguValue_t		val;
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
		/*----------------------------------------------------------------------
		name		args	stack before	stack after	side effects
		------------------------------------------------------------------------*/
		OP_RETURN,	/*	U	v_n-v_x(at u)	(return)	returns v_x-v_n	*/
		OP_CALL,		/*		A B	v_n-v_1 f(at a)	r_b-r_1		f(v1,...,v_n)	*/

		OP_PUSHNULL,	/*		U	-		nil_1-nil_u			*/
		OP_PUSHSTRING,	/* K	-		KSTR[k]				*/
		OP_PUSHNUM,		/* N	-		KNUM[n]				*/
		OP_POP,			/*	U	a_u-a_1		-				*/

		OP_GETLOCAL,/*	L	-		LOC[l]				*/
		OP_GETGLOBAL,/*	K	-		VAR[KSTR[k]]			*/

		OP_GETTABLE,/*	-	i t		t[i]				*/

		OP_CREATETABLE,/* U	-		newarray(size = u)		*/

		OP_SETLOCAL,/*	L	x		-		LOC[l]=x	*/
		OP_SETGLOBAL,/*	K	x		-		VAR[KSTR[k]]=x	*/
		OP_SETTABLE,/*	A B	v a_a-a_1 i t	(pops b values)	t[i]=v		*/

		OP_SETLIST,/*	A B	v_b-v_1 t	t		t[i+a*FPF]=v_i	*/
		OP_SETMAP,/*	U	v_u k_u - v_1 k_1 t	t	t[k_i]=v_i	*/

		OP_ADD,/*	-	y x		x+y				*/
		OP_SUB,/*	-	y x		x-y				*/
		OP_MUL,/*	-	y x		x*y				*/
		OP_DIV,/*	-	y x		x/y				*/
		OP_MOD,/*	-	y x		x%y				convert to int*/
		OP_UMINUS,/*	-	x		-x				*/
		OP_NOT,	 /*	-	x		(x==nil)? 1 : nil		*/

		OP_JMPNE,/*	J	y x		-		(x!=y)? PC+=s	*/
		OP_JMPEQ,/*	J	y x		-		(x==y)? PC+=s	*/
		OP_JMPLT,/*	J	y x		-		(x<y)? PC+=s	*/
		OP_JMPLE,/*	J	y x		-		(x<y)? PC+=s	*/
		OP_JMPGT,/*	J	y x		-		(x>y)? PC+=s	*/
		OP_JMPGE,/*	J	y x		-		(x>=y)? PC+=s	*/

		OP_JMPT,/*	J	x		-		(x!=null)? PC+=s	*/
		OP_JMPF,/*	J	x		-		(x==null)? PC+=s	*/
		OP_JMPONT,/*	J	x			(x!=null)? x : -	(x~=nil)? PC+=s	*/
		OP_JMPONF,/*	J	x			(x==null)? x : -	(x==nil)? PC+=s	*/
		OP_JMP,/*	J	-		-		PC+=s		*/

		OP_PUSHNULLJMP/* -	-			nil		PC++;		*/

}tguOpCode;




struct	__insctruction_tag
{
		int_8_t			op;
		int_16_t		arg1;
		int_16_t		arg2;
};


typedef tguObject_t		tguStackID_t;

typedef struct __call_info_tag
{
		tguIns_t		*save_pc;		
		tguStackID_t	*base;
		tguStackID_t	*top;
}tguCallInfo_t;

#define	TGU_MAX_CALL			40

typedef struct __tengu_call_stack_tag
{
		tguStackID_t	*start;
		tguStackID_t	*last;
		tguStackID_t	*top;	/*last used slot*/
}tguCallStack_t;

#define	TGU_STACK_SIZE(_stk)	((size_t) ( (_stk)->top - (_stk)->start + 1) )

#define	TGU_MAX_STACK_SIZE		4096



struct	__tengu_machine_tag
{
/***************************Run************************/
		tguIns_t		*pc;		
		tguStackID_t	*base;
		tguStackID_t	*top;

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
