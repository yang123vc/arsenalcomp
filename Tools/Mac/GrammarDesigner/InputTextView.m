////  InputTextView.m//  GrammarDesigner////  Created by liu peng on 9/26/11.//  Copyright 2011 none. All rights reserved.//#import "InputChecker.h"#import "GrammarDesignerDocument.h"@implementation InputTextView-(void)disableLineWrap{		const float LargeNumberForText = 1.0e7;				NSScrollView *scrollView = [self enclosingScrollView];		[scrollView setHasVerticalScroller:YES];		[scrollView setHasHorizontalScroller:YES];		[scrollView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];				NSTextContainer *textContainer = [self textContainer];		[textContainer setContainerSize:NSMakeSize(LargeNumberForText, LargeNumberForText)];		[textContainer setWidthTracksTextView:NO];		[textContainer setHeightTracksTextView:NO];				[self setMaxSize:NSMakeSize(LargeNumberForText, LargeNumberForText)];		[self setHorizontallyResizable:YES];		[self setVerticallyResizable:YES];		[self setAutoresizingMask:NSViewNotSizable];}-(void)resetFont{		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];				NSString *name;		NSInteger size;						name = [defaults stringForKey : PREFERENCE_FONT_INPUT_NAME];		size = (NSInteger)[defaults doubleForKey : PREFERENCE_FONT_INPUT_SIZE];				[font release];		font = [NSFont fontWithName : name							   size : size				];				[font retain];				[self setFont : font];}-(void)handleFontChanged : (NSNotification*)note{		DLog(@"InputTextView::handleFontChanged for notification %@", note);		[self resetFont];		}-(void)awakeFromNib{		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc addObserver : self			   selector : @selector(handleFontChanged:)				   name : NOTIFICATION_PREFERENCE_FONT_CHANGED				 object : nil		 ];		[self resetFont];		[self disableLineWrap];						}-(void)dealloc{		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc removeObserver : self];		[font release];		font = nil;		[super dealloc];}-(void)setOwner :(InputChecker*)o{		assert(o != nil);		owner = o;}-(void) parseInput :(id)sender{		DLog(@"On parseInput");		}-(void) openFile :(id)sender{		DLog(@"On openFile");}-(void) saveFile :(id)sender{		DLog(@"On saveFile");}- (NSMenu *)defaultMenu {		        NSMenu *theMenu = [[[NSMenu alloc] initWithTitle:@"Model browser context menu"] autorelease];		        [theMenu insertItemWithTitle:@"Parse" action:@selector(parseInput:) keyEquivalent:@"" atIndex:0];				[theMenu insertItem : [NSMenuItem separatorItem]					atIndex : 1		 ];		        NSString* openItem = [NSString stringWithFormat: @"Open"];        [theMenu insertItemWithTitle: openItem action:@selector(openFile:) keyEquivalent:@"" atIndex:2];				NSString* saveItem = [NSString stringWithFormat: @"Save"];        [theMenu insertItemWithTitle: saveItem action:@selector(saveFile:) keyEquivalent:@"" atIndex:3];		        return theMenu;}-(NSMenu *)menuForEvent:(NSEvent *)theEvent {        return [self defaultMenu];  }-(BOOL)validateUserInterfaceItem:(id < NSValidatedUserInterfaceItem >)item{						BOOL ret = YES;		assert(item != nil);						NSMenuItem *menuItem = (NSMenuItem*)item;				NSString *s = [self string];								if([menuItem action] == @selector(parseInput:)) 		{				if([s length] == 0 || ![owner.ownerDocument hasParser] )				{						ret = NO;				}								}else if([menuItem action] == @selector(openFile:))		{										}else if([menuItem action] == @selector(saveFile:))		{				if([s length] == 0)				{						ret = NO;				}		}		return ret;}@end