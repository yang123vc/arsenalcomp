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


#ifndef __RAY_PARSER_H__
#define __RAY_PARSER_H__


#include "ray.h"
#include "raysymbol.h"



AR_NAMESPACE_BEGIN



typedef enum
{
		EOI		=		0x00,
		DELIM	=		0x1000,	
		
		SWITCH,			/*switch*/
		FOR,			/*for*/
		GOTO,			/*goto*/
		RETURN,			/*return*/
		DO,				/*do*/
		WHILE,			/*while*/
		IF,				/*if*/
		ELSE,			/*else*/

		CONTINUE,		/*continue*/
		DEFAULT,		/*default*/
		CASE,			/*case*/
		BREAK,			/*break*/
		CONSTANT,		/*const*/
		SIZEOF,			/*size*/
		STRUCT,			/*struct*/
		
		TYPEDEF,		/*typedef*/
		
		ATTRIBUTE,		/*attribute*/

		SIGNED,			/*signed*/
		UNSIGNED,		/*unsigned*/
		VOID_TYPE,		/*void*/
		BYTE_TYPE,		/*byte*/
		SHORT_TYPE,		/*short*/
		INT_TYPE,		/*int*/
		LONG_TYPE,		/*long*/
		CHAR_TYPE,		/*char*/
		FLOAT_TYPE,		/*float*/
		DOUBLE_TYPE,	/*double*/
		
		RIGHT_ASSIGN,	/*>>=*/
		LEFT_ASSIGN,	/*<<=*/
		ADD_ASSIGN,		/*+=*/
		SUB_ASSIGN,		/*-=*/
		MUL_ASSIGN,		/**=*/
		DIV_ASSIGN,		/*/=*/
		MOD_ASSIGN,		/*%=*/
		AND_ASSIGN,		/*&=*/
		XOR_ASSIGN,		/*^=*/
		OR_ASSIGN,		/*|=*/
		INC_OP,			/*++*/
		DEC_OP,			/*--*/
		PTR_OP,			/*->*/
		AND_OP,			/*&&*/
		OR_OP,			/*||*/

		RIGHT_OP,			/*>>*/
		LEFT_OP,			/*<<*/		
		
		LE_OP,				/*<=*/
		GE_OP,				/*>=*/		
		EQ_OP,				/*==*/
		NE_OP,				/*!=*/			
		LESS_OP,			/*<*/
		GREATER_OP,		/*>*/

		AND,			/*&*/
		XOR,			/*^*/
		OR,				/*|*/

		NOT,			/*!*/
		NEGA,			/* ~ */
		ADD,			/* + */
		SUB,			/* - */
		MUL,			/* * */
		DIV,			/* / */
		MOD,			/* % */
		
		SEMICOLON,		/*;*/
		L_BRACES,		/*{*/
		R_BRACES,		/*}*/
		L_SQUARE,		/*[*/
		R_SQUARE,		/*]*/
		L_PAREN,		/*(*/
		R_PAREN,		/*)*/
		COMMA,			/*,*/
		COLON,			/*:*/
		ASSIGN,			/*=*/
		DOT,			/*.*/
		QUEST,			/*?*/

		STRING_LITERAL,	/*STRING_LITERAL*/
		OCT_CONSTANT,	/*OCT_CONSTANT*/
		HEX_CONSTANT,	/*HEX_CONSTANT*/
		DEC_CONSTANT,	/*DEC_CONSTANT*/
		FLOAT_CONSTANT,	/*FLOAT_CONSTANT*/
		CHAR_CONSTANT,	/*CHAR_CONSTANT*/
		IDENTIFIER,		/*IDENTIFIER*/

		TYPE_ID,		/*占位符，提取出的lexeme需要到符号表检查是否为一个类型*/
		FAKE_EOI,		/*占位符, 伪终结符*/
		
		/*以下为优先级符号*/
		IF_STMT_PREC,					/*if(stmt)...*/
		IF_STMT_ELSE_STMT_PREC,			/*if(stmt)else stmt....*/
		SPEC_ACTION,					/*特殊优先级*/
		ERROR_SHIFT
}rayTokValue_t;





typedef enum
{
		NODE_SPECIFIER_T,
		NODE_QUALIFIER_T,
		NODE_TYPE_T,
		NODE_DECLARATOR_T

}rayNodeType_t;

typedef __ray_parser_node_tag
{
		rayNodeType_t	node_type;
		
		union{
				rayType_t		*type;

		};
}rayNode_t;




AR_NAMESPACE_END





#endif


