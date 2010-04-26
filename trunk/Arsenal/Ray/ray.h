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


#ifndef __ARSENAL_RAY_H__
#define __ARSENAL_RAY_H__

#include "../Common/common.h"
#include "../Lex/lex.h"
#include "../Parser/grammar.h"
#include "../Parser/parser.h"
AR_NAMESPACE_BEGIN



void	RAY_Init();
void	RAY_UnInit();


struct __ray_parser_tag;
typedef struct __ray_parser_tag		rayParser_t;


struct __ray_compiler_tag;
typedef struct __ray_compiler_tag		rayCompiler_t;




/************************************************src info******************************/
typedef struct __src_info_tag
{
		size_t	col;
		size_t	line;
}raySrcInfo_t;


/********************************************´íÎó±¨¸æ*****************************************/

typedef enum
{
		RAY_REPORT_MESSAGE,
		RAY_REPORT_ERROR,
		RAY_REPORT_WARNING
}rayReportType_t;

typedef struct __ray_report_msg_tag
{
		rayReportType_t			type;
		
		const	wchar_t			*message;
		raySrcInfo_t			src;
}rayReportMsg_t;


typedef void (AR_STDCALL *rayReportFunc_t)(const rayReportMsg_t *report, void *context);

typedef struct __ray_report_tag
{
		rayReportFunc_t			report_func;
		void					*ctx;
}rayReport_t;





AR_NAMESPACE_END

#endif

