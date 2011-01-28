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
#include "tgusyntree.h"

AR_NAMESPACE_BEGIN

/****************************************************************相关全局资源*******************************************************************************/


void	TGU_InitParser();
void	TGU_UnInitParser();



/***************************************************************分析器**************************************************************************/


typedef struct __tengu_parser_tag
{
		tguReport_t				report;
		lexMatch_t				*match;
		psrContext_t			*parser_context;
		const tguBlock_t		*build_in;
		
		tguBlock_t				*abs_tree;					/*分析结果*/

		
		bool_t					has_error;
		tguBlock_t				*top_block;				/*当前块*/
		tguFunc_t				*current_function;		/*当前函数*/
		
		/***************************/
		size_t					loop_level;
}tguParser_t;


tguParser_t*			TGU_CreateParser(tguReport_t	*report, const tguBlock_t		*build_in_block);
void					TGU_DestroyParser(tguParser_t	*parser);
tguBlock_t*				TGU_ParseCode(tguParser_t	*parser, const wchar_t *code);










AR_NAMESPACE_END




#endif


