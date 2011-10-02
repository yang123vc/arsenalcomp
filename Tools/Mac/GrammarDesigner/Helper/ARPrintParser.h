/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "ArsenalOC.h"@interface PrintNode : NSObject{		NSString				*name;		size_t					index;		size_t					length;		size_t					line;		size_t					col;				BOOL					isTerm;		NSMutableArray			*nodes;}@property (readwrite, retain) NSString* name;@property (readwrite, assign) size_t	index;@property (readwrite, assign) size_t	length;@property (readwrite, assign) size_t	line;@property (readwrite, assign) size_t	col;@property (readwrite, assign) BOOL		isTerm;-(id)init;-(void)dealloc;-(size_t)		getChildCount;-(PrintNode*)	getChild : (size_t)idx;-(void)			appendChild : (PrintNode*)node;@endtypedef enum{		PRINT_NODE_MSG_T,		PRINT_NODE_WARNING_T,		PRINT_NODE_ERROR_T}printNodeParserMsg_t;@protocol PrintNodeParserDelegate	-(void)onBuildParserMsg : (printNodeParserMsg_t)t Msg : (NSString*)msg Line : (int)line;-(void)onParseMsg		: (printNodeParserMsg_t)t Msg : (NSString*)msg Line : (int)line;-(void)	setParser		: (const parser_t*)parser;-(const parser_t*)parser;@end@interface PrintNodeParser		:		NSObject{		psrLRItemType_t	parserMode;		BOOL			lexIsSingleLine;		BOOL			lexIsIgnoreCase;						lex_t			*lex;		psrGrammar_t	*grammar;		const parser_t	*parser;		 				NSObject<PrintNodeParserDelegate>	*delegate;}@property	(readwrite)			psrLRItemType_t							parserMode;@property	(readwrite)			BOOL									lexIsSingleLine;@property	(readwrite)			BOOL									lexIsIgnoreCase;@property	(readwrite, assign)	NSObject<PrintNodeParserDelegate>		*delegate;		-(id)init;-(void)dealloc;-(void)clear;-(BOOL)generateFromGrammar : (NSString *)grammarInput;-(PrintNode*)	parse : (NSString*)src;-(BOOL)generate : (const cfgConfig_t *)cfg;-(BOOL)isValidParser;-(const psrStatusView_t*)createParserView;-(void)destroyParserView : (const psrStatusView_t*)view;-(const psrConflictView_t*) createParserConflictView;-(void)destroyParserConflictView : (const psrConflictView_t*)view;@end