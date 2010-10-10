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


#ifndef __TGU_PARSER_H__
#define __TGU_PARSER_H__


#include "tengu.h"

AR_NAMESPACE_BEGIN


typedef enum 
{
		TGU_NULL_T,
		TGU_INTEGER_T,
		TGU_FLOAT_T,
		TGU_STRING_T,
		TGU_BOOLEAN_T,
		TGU_TABLE_T,
		TGU_FUNCTION_T,
		TGU_USERDATA_T
}tguType_t;


typedef enum
{
		TGU_OP_ASSIGN, 

		TGU_OP_LOGICAL_OR,		
		TGU_OP_LOGICAL_AND, 
		TGU_OP_LOGICAL_NOT,
		
		TGU_OP_IS_EQUAL, 
		TGU_OP_NOT_EQUAL,
		TGU_OP_LESS, 
		TGU_OP_GREATER, 
		TGU_OP_LESS_OR_EQUAL, 
		TGU_OP_GREATER_OR_EQUAL,
		
		TGU_OP_PLUS, 
		TGU_OP_MINUS, 
		TGU_OP_MUL, 
		TGU_OP_DIV, 
		TGU_OP_MOD,

		TGU_OP_UNARY_PLUS, 
		TGU_OP_UNARY_MINUS,

		TGU_OP_PREINC, 
		TGU_OP_PREDEC, 
		TGU_OP_POSTINC, 
		TGU_OP_POSTDEC,

		TGU_OP_CVT_TO_INTEGER, 
		TGU_OP_CVT_TO_FLOAT,
		TGU_OP_CVT_TO_BOOLEAN,
		TGU_OP_CVT_TO_NULL,
}tguOp_t;


typedef enum 
{
		TGU_ET_NULL_CONST,
		TGU_ET_INT_CONST,
		TGU_ET_FLOAT_CONST,
		TGU_ET_STRING_CONST,
		TGU_ET_VAR,
		TGU_ET_FUNCNAME,
		TGU_ET_FUNC_CALL,
		TGU_ET_BINARY,
		TGU_ET_UNARY,
		TGU_ET_CONDITIONAL,
		TGU_ET_ASSIGNMENT_CONVERSION,
		TGU_ET_UNDEF_VAR,
	
}tguExprType_t;



void	TGU_InitParser();
void	TGU_UnInitParser();







AR_NAMESPACE_END




#endif


