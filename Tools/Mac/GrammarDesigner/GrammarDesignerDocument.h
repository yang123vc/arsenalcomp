/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import <Cocoa/Cocoa.h>#import "stdheader.h"#import "GrammarCheckerTask.h"#import "InputChecker.h"#import "GrammarTextView.h"#import "TagsView.h"#import "OutputView.h"#import "FindResultsView.h"#import "ActionView.h"@interface GrammarDesignerDocument : NSDocument{/************************************************/		NSString		*grammarContent;		NSTimer			*grammarCheckTimer;				ARDuplexTask			*grammarCheckerTask;		GrammarCheckTask		*grammarCheckerTaskDelegate;		psrLRItemType_t	parserMode;		BOOL			lexIsSingleLine;		BOOL			lexIsIgnoreCase;/************************************************/		InputChecker	*inputChecker;		BOOL			inputCheckerIsHide;		/*********************IBOutlet***************************/				IBOutlet		NSWindow		*mainWindow;				IBOutlet		GrammarTextView	*grammarTextView;		IBOutlet		TagsView		*tagsView;		IBOutlet		NSTabView		*consoleTabView;		IBOutlet		OutputView		*outputView;		IBOutlet		FindResultsView	*findResultsView;		IBOutlet		ActionView		*actionView;		}/****************************Internal************************/-(void)load_default_config;-(void)on_reset_timer : (NSInteger)milliseconds;/****************************Initialize************************/- (id)init;-(void)dealloc;- (NSString *)windowNibName;- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError;- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError;- (void)windowControllerDidLoadNib:(NSWindowController *) aController;/****************************Timer and Callback************************/- (void)onGrammarChecker:(NSTimer*)theTimer;-(void) onGrammarCheckerCompleted : (const cfgConfig_t*)parseResult;/********************************************Notification*********************/-(void)handleApplicationChanged : (NSNotification*)note;-(void)handleParserChanged : (NSNotification*)note;-(void)handleLexerChanged : (NSNotification*)note;-(void)handleFontChanged : (NSNotification*)note;-(void)handleMainWindowWillMiniaturize : (NSNotification*)note;-(void)handleMainWindowBecomeMain : (NSNotification*)note;-(void)handleBuildParser : (NSNotification*)note;-(void)handleFindAllReferences : (NSNotification*)note;-(void)handleShowActionTable : (NSNotification*)note;-(void)handleShowConflicts : (NSNotification*)note;-(void)handleShowFirstFollowSet : (NSNotification*)note;-(void)handleShowLeftRecursive : (NSNotification*)note;-(void)handleShowLeftFactor : (NSNotification*)note;-(void)handleParseInput : (NSNotification*)note;/****************************Action************************/-(IBAction)setDocumentLexerSingleLine : (id)sender;-(IBAction)setDocumentLexerIgnoreCase : (id)sender;-(IBAction)setDocumentParserModeSLR : (id)sender;-(IBAction)setDocumentParserModeLALR : (id)sender;-(IBAction)buildDocumentParser	:	(id)sender;-(IBAction)rebuildDocumentTags	:	(id)sender;-(IBAction)generateDocumentGrammarTemplateCode : (id)sender;-(IBAction)showInputChecker : (id)sender;/***********************************UpdateUI***********************/-(BOOL)validateUserInterfaceItem:(id < NSValidatedUserInterfaceItem >)item;/***********************************Used for other class***********************/-(BOOL)hasParser;@end