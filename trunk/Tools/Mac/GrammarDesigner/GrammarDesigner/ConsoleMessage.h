/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "stdheader.h"typedef enum{		CONSOLE_MSG_MESSAGE_T,		CONSOLE_MSG_WARNING_T,		CONSOLE_MSG_ERROR_T,				CONSOLE_MSG_INPUT_MESSAGE_T,		CONSOLE_MSG_INPUT_WARNING_T,		CONSOLE_MSG_INPUT_ERROR_T		}consoleMessageType_t;@interface ConsoleMessage : NSObject {		consoleMessageType_t	type;		NSString				*msg;		int						line;}+(id)consoleMessageWithType : (consoleMessageType_t)t  						Msg : (NSString*)m 					   Line : (int)l;-(id)init;-(id)initWithType : (consoleMessageType_t)t  			  Msg : (NSString*)m 			 Line : (int)l;- (NSString *)description;@property (readwrite)			consoleMessageType_t	type;@property (readwrite, retain)	NSString				*msg;@property (readwrite)			int						line;@end