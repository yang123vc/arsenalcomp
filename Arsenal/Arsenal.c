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


static bool_t	__g_is_initialized = false;




bool_t	Arsenal_Init(const arInit_t *ctx)
{

        bool_t	result;
		uint_64_t		total_beg, total_end;
		/*AR_ASSERT(ctx != NULL);*/
		AR_ASSERT(!__g_is_initialized);
		result = true;
		total_beg = AR_GetTime_Milliseconds();
		
		if(!__g_is_initialized)
		{
				AR_Init(ctx);
				Lex_Init();
				Parser_Init();
				Tools_Init();
				TGU_Init();
				__g_is_initialized = true;
				result = true;
		}else
		{
				result = false;
		}

		total_end = AR_GetTime_Milliseconds();



		if(result)
		{
				wchar_t msg[1024];
				AR_swprintf(msg, AR_NELEMS(msg), L"Arsenal initialized consume time == %Id\r\n", total_end - total_beg);
				AR_printf(msg);
		}else
		{
				
		}

		return result;
}





bool_t	Arsenal_UnInit()
{
		AR_ASSERT(__g_is_initialized);
		if(__g_is_initialized)
		{
				TGU_UnInit();
				Tools_UnInit();
				Parser_UnInit();
				Lex_UnInit();
				AR_UnInit();
				__g_is_initialized = false;
				return true;
		}else
		{
				return false;
		}
}


/*************************************************************************************************/



AR_NAMESPACE_END
