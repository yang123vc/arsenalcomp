/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import <Cocoa/Cocoa.h>#import "stdheader.h"#import "GrammarCheckerTask.h"#import "InputChecker.h"#import "GrammarTextView.h"#import "TagsView.h"#import "ConsoleView.h"#import "ActionView.h"@interface GrammarDesignerDocument : NSDocument{/************************************************/		NSString		*grammarContent;		NSTimer			*grammarCheckTimer;				ARDuplexTask			*grammarCheckerTask;		GrammarCheckTask		*grammarCheckerTaskDelegate;		psrLRItemType_t	parserMode;		BOOL			lexIsSingleLine;		BOOL			lexIsIgnoreCase;				NSFont			*grammarFont;		NSFont			*tagsFont;		NSFont			*consoleFont;				NSFont			*inputFont;		NSFont			*treeViewFont;		/************************************************/		InputChecker	*inputChecker;		BOOL			inputCheckerIsHide;		/*********************IBOutlet***************************/				IBOutlet		NSWindow		*mainWindow;				IBOutlet		GrammarTextView	*grammarTextView;		IBOutlet		TagsView		*tagsView;		IBOutlet		NSTabView		*consoleTabView;		IBOutlet		ConsoleView		*outputView;		IBOutlet		ConsoleView		*findResultsView;		IBOutlet		ActionView		*actionView;		}@property (readonly) NSFont			*grammarFont;@property (readonly) NSFont			*inputFont;@property (readonly) NSFont			*tagsFont;@property (readonly) NSFont			*treeViewFont;@property (readonly) NSFont			*consoleFont;/****************************Internal************************/-(void)load_default_config;-(void)load_font_config;-(void)on_reset_timer : (NSInteger)milliseconds;-(void)reset_view_font;/****************************Initialize************************/- (id)init;-(void)dealloc;- (NSString *)windowNibName;- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError;- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError;- (void)windowControllerDidLoadNib:(NSWindowController *) aController;/****************************Timer and Callback************************/- (void)onGrammarChecker:(NSTimer*)theTimer;-(void) onGrammarCheckerCompleted : (const cfgConfig_t*)parseResult;/********************************************Notification*********************/-(void)handleApplicationChanged : (NSNotification*)note;-(void)handleParserChanged : (NSNotification*)note;-(void)handleLexerChanged : (NSNotification*)note;-(void)handleFontChanged : (NSNotification*)note;-(void)handleMainWindowWillMiniaturize : (NSNotification*)note;-(void)handleMainWindowBecomeMain : (NSNotification*)note;/****************************Action************************/-(IBAction)setDocumentLexerSingleLine : (id)sender;-(IBAction)setDocumentLexerIgnoreCase : (id)sender;-(IBAction)setDocumentParserModeSLR : (id)sender;-(IBAction)setDocumentParserModeLALR : (id)sender;-(IBAction)buildDocumentParser	:	(id)sender;-(IBAction)rebuildDocumentTags	:	(id)sender;-(IBAction)generateDocumentGrammarTemplateCode : (id)sender;-(IBAction)showInputChecker : (id)sender;/***********************************UpdateUI***********************/-(BOOL)validateUserInterfaceItem:(id < NSValidatedUserInterfaceItem >)item;/***********************************Used for other class***********************/-(BOOL)hasParser;@end