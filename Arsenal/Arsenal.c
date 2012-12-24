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




#include "Arsenal.h"



AR_NAMESPACE_BEGIN


static ar_int_t __g_init_count = 0;

arStatus_t	Arsenal_Init(const arInit_t *ctx)
{

        arStatus_t      result;
		ar_uint_64_t		total_beg, total_end;
		bool_t          cm_init,lex_init, psr_init,tools_init,tgu_init;
		/*AR_ASSERT(ctx != NULL);*/
		
		result = AR_S_YES;
		cm_init = false;
		lex_init = false;
		psr_init  = false;
		tools_init = false;
		tgu_init = false;

		total_beg = AR_GetTime_Milliseconds();
		
		if(AR_AtomicInc(&__g_init_count) == 1)
		{
				result = AR_CommonInit(ctx);
				if(result != AR_S_YES)
				{
						goto END_POINT;
				}
				cm_init = true;
				
				result = Lex_Init();
				if(result != AR_S_YES)
				{
						goto END_POINT;
				}
				lex_init = true;

				result = Parser_Init();
				if(result != AR_S_YES)
				{
						goto END_POINT;
				}
				psr_init = true;
				
				result = Tools_Init();
				if(result != AR_S_YES)
				{
						goto END_POINT;
				}
				tools_init = true;

				result = TGU_Init();
				if(result != AR_S_YES)
				{
						goto END_POINT;
				}
				tgu_init = true;
		}

		total_end = AR_GetTime_Milliseconds();

		/*__g_init_count++;*/

		{
				wchar_t msg[1024];
				AR_swprintf(msg, AR_NELEMS(msg), L"Arsenal initialized consume time == %Id\r\n", total_end - total_beg);
				AR_printf(msg);
		}

		return result;
END_POINT:
		
		if(tgu_init)
		{
				TGU_UnInit();
				tgu_init = false;
		}

		if(tools_init)
		{
				Tools_UnInit();
				tools_init = false;
		}

		if(psr_init)
		{
				Parser_UnInit();
				psr_init  = false;
		}

		if(lex_init)
		{
				Lex_UnInit();
				lex_init = false;
		}
		if(cm_init)
		{
				AR_CommonUnInit();
				cm_init = false;
		}

		return result;
}





void	Arsenal_UnInit()
{

		/*if(--__g_init_count == 0)*/

		if(AR_AtomicDec(&__g_init_count) == 0)
		{
				TGU_UnInit();
				Tools_UnInit();
				Parser_UnInit();
				Lex_UnInit();
				AR_CommonUnInit();
		}
}


/*************************************************************************************************/



AR_NAMESPACE_END
