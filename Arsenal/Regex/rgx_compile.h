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

#ifndef __REGEX_COMPILE_H__
#define __REGEX_COMPILE_H__

#include "rgx_in.h"


AR_NAMESPACE_BEGIN

typedef enum
{
		RGX_CHAR_I,
		RGX_BEGIN_I,
		RGX_END_I,
		RGX_LOOKAHEAD_BEG_I,
		RGX_LOOKAHEAD_END_I,
		RGX_JMP_I,
		RGX_BRANCH_I,
		RGX_MATCH_I
}rgxInsType_t;

static const wchar_t *RGX_INS_NAME[] = 
{
		L"char",
		L"match_begin",
		L"match_end",
		L"lookahead_begin",
		L"lookahead_end",
		L"jmp",
		L"branch",
		L"match"
};

typedef struct __regex_instruction_tag	rgxIns_t;

struct __regex_instruction_tag
{
		rgxInsType_t	opcode;
		int_t			data;
		
		rgxIns_t		*left;
		rgxIns_t		*right;
		int_t			mark;
};



typedef struct __regex_program_tag		rgxProg_t;


struct __regex_program_tag
{
		rgxIns_t		*start;
		size_t			count;
		
		rgxIns_t		*pc;
};


void			RGX_InitProg(rgxProg_t *prog);
void			RGX_UnInitProg(rgxProg_t *prog);
void			RGX_Compile(rgxProg_t *prog, const rgxNode_t *tree);
void			RGX_PringProg(const rgxProg_t *prog, arString_t *str);




AR_NAMESPACE_END


#endif
