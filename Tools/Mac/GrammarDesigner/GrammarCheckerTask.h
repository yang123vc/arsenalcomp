/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "stdheader.h"/***********************************GrammarCheckWorker**********************************************/@interface GrammarCheckTaskMessage : ARDuplexTaskMessage{		NSString				*sourcesCode;		const cfgConfig_t		*parseResult;}@property		(readwrite, retain) NSString				*sourcesCode;@property		(readwrite, assign) const cfgConfig_t		*parseResult;-(id)initWithSources : (NSString*)src;-(void)dealloc;-(void)run;@end@interface GrammarCheckTask : NSObject<ARDuplexTaskDelegate>@end