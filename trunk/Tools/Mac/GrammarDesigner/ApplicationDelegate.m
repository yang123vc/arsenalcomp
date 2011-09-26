/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "ApplicationDelegate.h"#import "Preferences.h"#import "StringConverter.h"#import "GrammarDesignerDocument.h"/***********************************DefaultARContext**********************************************/@interface DefaultARContext		:		NSObject<ArsenalContext>-(void)onPrint	:		(NSString*) msg;-(void)onError	:		(NSInteger)level  msg : (NSString*)msg;@end@implementation DefaultARContext-(void)onPrint	:		(NSString*) msg{		assert(msg);		DLog(@"%@", msg);}-(void)onError	:		(NSInteger)level  msg : (NSString*)msg{		assert(msg);		DLog(@"Level == %d : msg == %@", level, msg);		}@end/***********************************DefaultARContext*********************************************End*//*******************************************ApplicationDelegate**************************************/@implementation ApplicationDelegate+(void)initialize{		[Preferences setDefaultPreferences];}-(id)init{		self = [super init];		if(self)		{				preferences = nil;																					}				return (self);}-(void)dealloc{		[super dealloc];}- (BOOL)applicationShouldOpenUntitledFile:(NSApplication *)sender{		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];		BOOL isOpenEmptyDocument = [defaults boolForKey : PREFERENCE_APPLICATION_IS_OPEN_EMPTY_DOCUMENT];		return isOpenEmptyDocument;		}- (void)applicationWillFinishLaunching:(NSNotification *)aNotification{				DLog(@"%@", aNotification);		DefaultARContext *ctx = [[DefaultARContext alloc] init];		[ArsenalOC initializeArsenalOC : ctx];		[ctx release];				/********************************************End*/		}- (void)applicationDidFinishLaunching:(NSNotification *)aNotification{		DLog(@"%@", aNotification);}- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{				return NSTerminateNow;}- (void)applicationWillTerminate:(NSNotification *)aNotification{		DLog(@"%@", aNotification);						[ArsenalOC uninitializeArsenalOC];}/***************************************************************************************//**************************************Action*************************************/-(IBAction)showPreferencePanel : (id)sender{		if(preferences == nil)		{				preferences = [[Preferences alloc] init];		}				[preferences show : self];}-(IBAction)showStringConverter : (id)sender{		 if(stringConverter == nil)		 {				 stringConverter = [[StringConverter alloc] init];		 }				[stringConverter show : self];}/**************************************TestAction*************************************/-(IBAction)onTest : (id)sender{		DLog(@"On ApplicationDelegate::OnTest");		@try{				AR_Test();						}@catch(NSException *e)		{				NSLog(@"Name == %@ reason == %@", [e name], [e reason]);		}}@end