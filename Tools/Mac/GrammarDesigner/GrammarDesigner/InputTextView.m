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


#import "InputChecker.h"
#import "GrammarDesignerDocument.h"


@implementation InputTextView



-(void)disableLineWrap
{
		const float LargeNumberForText = 1.0e7;
		
		NSScrollView *scrollView = [self enclosingScrollView];
		[scrollView setHasVerticalScroller:YES];
		[scrollView setHasHorizontalScroller:YES];
		[scrollView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
		
		NSTextContainer *textContainer = [self textContainer];
		[textContainer setContainerSize:NSMakeSize(LargeNumberForText, LargeNumberForText)];
		[textContainer setWidthTracksTextView:NO];
		[textContainer setHeightTracksTextView:NO];
		
		[self setMaxSize:NSMakeSize(LargeNumberForText, LargeNumberForText)];
		[self setHorizontallyResizable:YES];
		[self setVerticallyResizable:YES];
		[self setAutoresizingMask:NSViewNotSizable];
}





-(void)resetFont
{
		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
		
		NSString *name;
		NSInteger size;
		
		
		name = [defaults stringForKey : PREFERENCE_FONT_INPUT_NAME];
		size = (NSInteger)[defaults doubleForKey : PREFERENCE_FONT_INPUT_SIZE];
		
		[font release];
		font = [NSFont fontWithName : name
							   size : size
				];
		
		[font retain];
		
		[self setFont : font];
}


-(void)handleFontChanged : (NSNotification*)note
{
		DLog(@"InputTextView::handleFontChanged for notification %@", note);
		[self resetFont];
		
}


-(void)setHasValidParser : (BOOL)valid
{
		hasValidParser = valid;
}


-(void)awakeFromNib
{

		line_lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 0;
		act.value = 1;
		
		if(Lex_InsertRule(line_lex, L".*(\\r\\n|\\n\\r|\\r|\\n)?", &act) != AR_S_YES)
		{
				assert(0);
		}
		
		act.priority = 1;
		act.value = 0;
		
		if(Lex_InsertRule(line_lex, L"$", &act)  != AR_S_YES)
		{
				assert(0);
		}
		
		line_match = Lex_CreateMatch(line_lex);

		
		
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver : self
			   selector : @selector(handleFontChanged:)
				   name : NOTIFICATION_PREFERENCE_FONT_CHANGED
				 object : nil
		 ];

		[self resetFont];
		[self disableLineWrap];
		
		
		hasValidParser = NO;
		
		
}


-(void)dealloc
{
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc removeObserver : self];

		[font release];
		font = nil;
		
		Lex_DestroyMatch(line_match);
		line_match = NULL;
		Lex_Destroy(line_lex);
		line_lex = NULL;

		
		[super dealloc];
}


-(void)selecteLine : (size_t)line
{
		int location = -1;
		int length = -1;
		DLog(@"GrammarTextView::selectLine");
		
		NSString *txt = [self string];
		
		/*if(txt == nil || [txt length] == 0)
		 {
		 return;
		 }
		 */
		
		WideCharWrapper *wcs_txt = [ARUtility convertNSStringToUTF32 : txt];
		
		
		
		Lex_ResetInput(line_match, [wcs_txt string]);
		
		lexToken_t tok;
		arStatus_t is_ok = AR_S_NO;
		
		
		
		while(YES)
		{
				is_ok = Lex_Match(line_match, &tok);
				
				if(is_ok != AR_S_YES)
				{
						break;
				}
				
				if(line == tok.line)
				{
						break;
				}
				
				if(tok.value == 0)
				{
						break;
				}
		}
		
		if(is_ok == AR_S_YES && tok.value != 0 && tok.count > 0)
		{
				location = (int)tok.index;
				length = (int)tok.count;
		}
		
		assert(is_ok == AR_S_YES);
		
		
		
		DLog(@"select range == (%d,%d)", location, length);
		
		if(location != -1 && length != -1)
		{
				[self setSelectedRange : NSMakeRange(location, length)];
				[self scrollRangeToVisible : NSMakeRange(location, length)];
		}
}


-(void) parseInput :(id)sender
{

		NSString *s = [self string];
		
		if(s != nil && [s length] > 0)
		{
				DLog(@"InputTextView Sending NOTIFICATION_INPUTVIEW_INVOKE_PARSEINPUT");
		
				NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
				[nc postNotificationName :	NOTIFICATION_INPUTVIEW_INVOKE_PARSEINPUT 
								  object :  self
								userInfo : [NSDictionary dictionaryWithObject : s 
																	   forKey : @"UserInfo"
											]
				 ];
		}
}

-(void) openFile :(id)sender
{
		DLog(@"On openFile");
		
		NSOpenPanel *panel = [NSOpenPanel openPanel];
		
		[panel setCanChooseDirectories:NO];
		[panel setCanChooseFiles:YES];
		[panel setDirectory:NSHomeDirectory()];

		if ([panel runModal] == NSOKButton) 
		{
				NSString *path = [[[panel URLs] objectAtIndex:0] path];
				
				const wchar_t *wcs_path = [[ARUtility convertNSStringToUTF32 : path] string];
				
				arString_t *txt = AR_CreateString(1024);
				if(AR_LoadBomTextFile(wcs_path, NULL, txt) != AR_S_YES)
				{
						NSRunAlertPanel(@"Warning", @"Open \"%@\" failed!", @"OK", nil,nil, path);
				}else
				{
						NSString *content = [ARUtility convertUTF32ToNSString : AR_GetStringCString(txt)];
						[self setString : content];
				}
				AR_DestroyString(txt);
				txt = NULL;
				
		}
}


-(void) saveFile :(id)sender
{
		DLog(@"On saveFile");
		
		if([[self string] length] == 0)
		{
				return;
		}
		
		NSSavePanel *panel = [NSSavePanel savePanel];
		[panel setDirectory:NSHomeDirectory()]; 
		if ([panel runModal] == NSOKButton) 
		{
				NSString *path = [[panel URL] path];
				DLog(@"%@", path);
				
				const wchar_t *wcs_path = [[ARUtility convertNSStringToUTF32 : path] string];
				const wchar_t *wcs_val = [[ARUtility convertNSStringToUTF32 : [self string]] string];
				
				if(AR_SaveBomTextFile(wcs_path, AR_TXT_BOM_UTF_8, wcs_val) != AR_S_YES)
				{
						NSRunAlertPanel(@"Warning", @"Save to \"%@\" failed!", @"OK", nil,nil, path);
				}
		}
		
}



- (NSMenu *)defaultMenu {
		
        NSMenu *theMenu = [[[NSMenu alloc] initWithTitle:@"Model browser context menu"] autorelease];
		
        [theMenu insertItemWithTitle:@"Parse" action:@selector(parseInput:) keyEquivalent:@"" atIndex:0];
		
		[theMenu insertItem : [NSMenuItem separatorItem]
					atIndex : 1
		 ];
		
	
		

		[theMenu insertItemWithTitle:@"Cut" action:@selector(cut:) keyEquivalent:@"" atIndex:2];
		[theMenu insertItemWithTitle:@"Copy" action:@selector(copy:) keyEquivalent:@"" atIndex:3];
		[theMenu insertItemWithTitle:@"Paste" action:@selector(paste:) keyEquivalent:@"" atIndex:4];
		

		[theMenu insertItemWithTitle:@"Select All" action:@selector(selectAll:) keyEquivalent:@"" atIndex:5];
		
		[theMenu insertItem : [NSMenuItem separatorItem]
					atIndex : 6
		 ];
		
		
		
		
        NSString* openItem = [NSString stringWithFormat: @"Open"];
        [theMenu insertItemWithTitle: openItem action:@selector(openFile:) keyEquivalent:@"" atIndex:7];
		
		NSString* saveItem = [NSString stringWithFormat: @"Save"];
        [theMenu insertItemWithTitle: saveItem action:@selector(saveFile:) keyEquivalent:@"" atIndex:8];
		
        return theMenu;
}


-(NSMenu *)menuForEvent:(NSEvent *)theEvent 
{
        return [self defaultMenu];  

}








-(BOOL)validateUserInterfaceItem:(id < NSValidatedUserInterfaceItem >)item
{
				
		BOOL ret = YES;
		assert(item != nil);
				
		NSMenuItem *menuItem = (NSMenuItem*)item;
		
		NSString *s = [self string];
		
				
		if([menuItem action] == @selector(parseInput:)) 
		{
				if([s length] == 0)
				{
						ret = NO;
				}
						
		}else if([menuItem action] == @selector(openFile:))
		{
				
				
		}else if([menuItem action] == @selector(saveFile:))
		{
				if([s length] == 0)
				{
						ret = NO;
				}
		}else
		{
				ret = [super validateUserInterfaceItem : item];
		}
		return ret;
}





@end
