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



#include "tools.h"


AR_NAMESPACE_BEGIN

arStatus_t	Tools_Init()
{
        arStatus_t status;
        ar_bool_t cfg_is_init = false, plist_is_init = false;
        
        status = AR_S_YES;
        
		CFG_Init();
        cfg_is_init = true;
        
        status = PList_Init();
        if(status != AR_S_YES)
        {
                
        }
        plist_is_init = true;
        
        
END_POINT:
        
        if(status == AR_S_YES)
        {
                return status;
        }
        
        if(plist_is_init)
        {
                PList_UnInit();
                plist_is_init = false;
        }
        
        if(cfg_is_init)
        {
                CFG_UnInit();
                cfg_is_init = false;
        }
        
        return status;

}

void	Tools_UnInit()
{
        PList_UnInit();
		CFG_UnInit();
}



AR_NAMESPACE_END

