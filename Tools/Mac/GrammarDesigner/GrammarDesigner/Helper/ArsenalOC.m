/*
 * Copyright (c) 2011 by Solidus
 * This file is part of Arsenal library tools source code.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#include <execinfo.h>

#import "ArsenalOC.h"
#import "ARUtility.h"
#import "NSString_Convert.h"


static void	AR_STDCALL __default_error_func(ar_int_t level, const wchar_t *msg, void *ctx)
{
		assert(ctx != NULL && msg != NULL);
		NSObject<ArsenalContext> *context = (NSObject<ArsenalContext>*)ctx;


		[context onError : level 
					 msg : [NSString stringWithWideString : msg]
		 ];
		
}


static void	AR_STDCALL __default_print_func(const wchar_t *msg, void *ctx)
{
		assert(ctx != NULL && msg != NULL);
		
		NSObject<ArsenalContext> *context = (NSObject<ArsenalContext>*)ctx;
		
		[context onPrint : [NSString stringWithWideString : msg]];
}


static ArsenalOC *__g_instance = nil;


//typedef size_t	(AR_STDCALL *AR_backtrace_func_t)(void **callstack, size_t callstack_cnt);
//typedef size_t	(AR_STDCALL *AR_backtrace_symbol_t)(void **callstack, size_t callstack_cnt, char *str, size_t len);



static size_t AR_STDCALL __build_backtrace(void **call_stack, size_t call_stack_cnt)
{
        size_t i, nstack;
        
        AR_ASSERT(call_stack != NULL && call_stack_cnt > 0);
        
        nstack = backtrace(call_stack, (int)call_stack_cnt);
        
        for(i = nstack; i < call_stack_cnt; ++i)
        {
                call_stack[i] = NULL;
        }
        
        return nstack;
}

static size_t AR_STDCALL __build_backtrace_string(void **call_stack, size_t call_stack_cnt, char *str, size_t l)
{
        char **strings;
        char *p;
        size_t remain;
        size_t i, n;
        
        AR_ASSERT(call_stack != NULL);
        AR_ASSERT(str != NULL && l > 0);
        
        if(call_stack_cnt == 0)
        {
                str[0] = '\0';
                return 0;
        }
        
        strings = backtrace_symbols(call_stack, (int)call_stack_cnt);
        if(strings == NULL)
        {
                str[0] = '\0';
                return 0;
        }
        
        remain = l - 1;
        p = str;
        
        for (i = 1; i < call_stack_cnt && remain > 0; i++)
        {
                if(AR_SPRINTF(p, remain, "%s\r\n", strings[i]) <= 0)
                {
                        break;
                }
                
                n = AR_strlen(p);
                
                if(n == 0)
                {
                        break;
                }
                
                p += n;
                remain -= n;
        }
        
        *p = '\0';
        
        free(strings);
        return p - str;
}





@implementation ArsenalOC


-(id)init
{
		assert(NO);
}

-(id)initWithContext : (NSObject<ArsenalContext>*)ctx
{
		assert(ctx != nil);
		
		self = [super init];
		
		if(self)
		{
				context = ctx;
				[context retain];
				
				arInit_t init;
				init.global_io_ctx.on_error = __default_error_func;
				init.global_io_ctx.on_print = __default_print_func;
				init.global_io_ctx.ctx = (void*)context;
                init.backtrace.gen_backtrace = __build_backtrace;
                init.backtrace.gen_backtrace_sym = __build_backtrace_string;
                
				Arsenal_Init(&init);
		}
		return self;
}

-(void)dealloc
{
		Arsenal_UnInit();
		[super dealloc];
}


+(void) initializeArsenalOC : (NSObject<ArsenalContext>*)ctx
{
		@synchronized(self)
		{
				if(__g_instance == nil)
				{
						__g_instance = [[self alloc] initWithContext : ctx];
				}
		}
}


+(void)			uninitializeArsenalOC
{
		
		@synchronized(self)
		{
				
				if(__g_instance != nil)
				{
						[__g_instance release];
						__g_instance = nil;
				}
		}
}






@end




