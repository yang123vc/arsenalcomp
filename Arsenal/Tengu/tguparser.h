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

/***************************************************************´íÎó±¨¸æ**************************************************************************/


typedef enum
{
		TGU_REPORT_MESSAGE_T,
		TGU_REPORT_ERROR_T,
		TGU_REPORT_ERROR_LEX_T,
		TGU_REPORT_ERROR_SYNTAX_T,
		TGU_REPORT_WARNING_SYNTAX_T
}tguReportType_t;


typedef struct __tengu_report_info_tag
{
		tguReportType_t			type;
		
		union{

		struct					{
				const	wchar_t			*message;
		
		}								std_msg;

		struct	{
				int_t					err_level;
				const	wchar_t			*err_msg;
		
		}								error;
		
		struct			{
				const	wchar_t			*msg;
				const	lexToken_t		*tok;
		
		}								lex_error;
		
		struct			{
				const	wchar_t			*msg;
				const	tguToken_t		*tok;
		}								syntax_error;
		
		struct	{
				size_t					line;
				const wchar_t			*msg;
		}								warning;
		};
}tguReportInfo_t;


typedef void (AR_STDCALL *tguReportFunc_t)(const tguReportInfo_t *report, void *context);


typedef struct __tengu_report_tag
{
		tguReportFunc_t	report_func;
		void			*report_ctx;
}tguReport_t;


/***************************************************************·ÖÎöÆ÷**************************************************************************/


typedef struct __tengu_parser_tag
{
		tguReport_t				report;
		lexMatch_t				*match;
		psrContext_t			*parser_context;
		const tguBlock_t		*build_in;

		tguSyntaxTree_t	*result;

		tguBlock_t		*top_block;
		tguFunc_t		*current_function;
		
		
		/***************************/
		size_t			loop_level;
		bool_t			has_error;

}tguParser_t;


tguParser_t*			TGU_CreateParser(tguReport_t	*report, const tguBlock_t		*build_in_block);
void					TGU_DestroyParser(tguParser_t	*parser);
tguSyntaxTree_t*		TGU_ParseCode(tguParser_t	*parser, const wchar_t *sources_name, const wchar_t *code);




/******************************************************************************************************************************************************/


void	TGU_InitParser();
void	TGU_UnInitParser();







AR_NAMESPACE_END




#endif


