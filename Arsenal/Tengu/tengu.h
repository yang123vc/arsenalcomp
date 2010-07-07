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


#ifndef __ARSENAL_TENGU_H__
#define __ARSENAL_TENGU_H__

#include "../Common/common.h"
#include "../Lex/lex.h"
#include "../Parser/parser.h"




AR_NAMESPACE_BEGIN



void	TGU_Init();
void	TGU_UnInit();





struct	__tengu_machine_tag;
typedef struct	__tengu_machine_tag		tguMachine_t;

typedef	int_t (AR_STDCALL *	tguExternFunc_t) (tguMachine_t	*vm);


AR_NAMESPACE_END

#endif
