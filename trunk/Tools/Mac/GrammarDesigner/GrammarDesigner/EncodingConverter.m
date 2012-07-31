//
//  EncodingConverter.m
//  GrammarDesigner
//
//  Created by Solidus on 12-7-31.
//  Copyright (c) 2012年 none. All rights reserved.
//

#import "EncodingConverter.h"



static void disableLineWrap(NSTextView		*txtView)
{
		static const float LargeNumberForText = 1.0e7;
		
		NSScrollView *scrollView = [txtView enclosingScrollView];
		[scrollView setHasVerticalScroller:YES];
		[scrollView setHasHorizontalScroller:YES];
		[scrollView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
		
		NSTextContainer *textContainer = [txtView textContainer];
		[textContainer setContainerSize:NSMakeSize(LargeNumberForText, LargeNumberForText)];
		[textContainer setWidthTracksTextView:NO];
		[textContainer setHeightTracksTextView:NO];
		
		[txtView setMaxSize:NSMakeSize(LargeNumberForText, LargeNumberForText)];
		[txtView setHorizontallyResizable:YES];
		[txtView setVerticallyResizable:YES];
		[txtView setAutoresizingMask:NSViewNotSizable];
}

/*********************************************************************************/


@interface EncodingConverter ()

@end

@implementation EncodingConverter

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}




/**********************************************/

-(id)init
{
        self = [super initWithWindowNibName : @"EncodingConverter"];
        
        if(self)
        {
                default_font = [NSFont fontWithName : @"Menlo"
											   size : 18
								];
				[default_font retain];
        }
        
        return self;
}


-(void)dealloc
{
		[super dealloc];
}


-(void)awakeFromNib
{
        DLog(@"On EncodingConverter::awakeFromNib");
		[input setFont : default_font];
		[output setFont : default_font];
		
		disableLineWrap(input);
		disableLineWrap(output);
		
}







-(void)show : (id)sender
{
        [self showWindow : sender];
}


-(void)convert : (id)sender
{
        DLog(@"On EncodingConverter::convert");
}



@end
