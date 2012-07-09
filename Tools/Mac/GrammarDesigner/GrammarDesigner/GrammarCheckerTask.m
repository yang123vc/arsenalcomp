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

#import "stdheader.h"
#import "GrammarDesignerDocument.h"
#import "GrammarCheckerTask.h"




static void AR_STDCALL report_tag_func(const cfgReportInfo_t *report, void *context)
{
		switch(report->type)
		{
				case CFG_REPORT_MESSAGE_T:
                        AR_error(AR_ERR_MESSAGE, L"%ls\r\n", report->std_msg.message);
						break;
				case CFG_REPORT_ERROR_T:
						AR_error(AR_ERR_MESSAGE, L"%ls : %d\r\n", report->error.err_msg, report->error.err_level);
						break;
				case CFG_REPORT_ERROR_LEX_T:
						AR_error(AR_ERR_MESSAGE, L"lex error %ls\r\n", report->lex_error.msg);
						break;
				case CFG_REPORT_ERROR_SYNTAX_T:
						AR_error(AR_ERR_MESSAGE, L"syntax error %ls\r\n", report->syntax_error.msg);
						break;
				case CFG_REPORT_WARNING_SYNTAX_T:
						AR_error(AR_ERR_MESSAGE, L"warning %ls\r\n", report->warning.msg);
						
						break;
				default:
						AR_ASSERT(false);
		}
}

static cfgReport_t	__g_report = {report_tag_func, NULL};





@implementation GrammarCheckTaskMessage

@synthesize		parseResult;
@synthesize		sourcesCode;

-(id)init
{
		assert(NO);
		return nil;
}

-(id)initWithSources : (NSString*)src
{
		self = [super initWithName : nil];
		if(self)
		{
				[src retain];
				sourcesCode = src;
				parseResult = NULL;
		}
		return self;
}


-(void)dealloc
{
		[sourcesCode release];
		sourcesCode = nil;
		
		if(parseResult)
		{
				CFG_DestroyGrammarConfig(parseResult);
				parseResult = NULL;
		}
		
		[super dealloc];
}


-(void)run
{
		if([sourcesCode length] > 0)
		{
				WideCharWrapper *tmp = [ARUtility convertNSStringToUTF32 : sourcesCode];
				
				
				parseResult = CFG_CollectGrammarConfig([tmp string], &__g_report);
		}else
		{
				parseResult = NULL;
		}
}

@end



/******************************************************************/




@implementation GrammarCheckTask

-(id)init
{
		assert(false);
		return nil;
}

-(id)initWithDocument : (id)pdoc
{
		assert(pdoc != nil);
		self = [super init];
		if(self)
		{
				[pdoc retain];
				grammarDocument = pdoc;
		}
		
		return self;
		
}

-(void)dealloc
{
		if(grammarDocument != nil)
		{
				[grammarDocument release];
				grammarDocument = nil;
		}
		
		[super dealloc];
}

-(void)onWorkerBegin
{
		DLog(@"GrammarCheckTask::onWorkerBegin");
}

-(void)onWorkerEnd
{
		DLog(@"GrammarCheckTask::onWorkerEnd");
}

-(void)handleWorkerThreadMessage : (ARDuplexTaskMessage*)msg forTask : (ARDuplexTask*)task
{
		assert(msg != nil && task != nil);
		GrammarCheckTaskMessage *gmrMsg = (GrammarCheckTaskMessage*)msg;
		DLog(@"GrammarCheckTask::handleWorkerThreadMessage");
		[gmrMsg run];
		[task PostMsgToMain : msg];
		
}

-(void)handleMainThreadMessage : (ARDuplexTaskMessage*)msg forTask : (ARDuplexTask*)task
{
		GrammarCheckTaskMessage *gmrMsg = (GrammarCheckTaskMessage*)msg;
		DLog(@"GrammarCheckTask::handleMainThreadMessage");
		if(grammarDocument != nil)
		{
				[grammarDocument onGrammarCheckerCompleted : gmrMsg.parseResult];
		}
		[msg release];
}




@end


