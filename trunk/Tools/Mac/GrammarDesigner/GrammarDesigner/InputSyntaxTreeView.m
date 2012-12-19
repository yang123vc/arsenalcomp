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

@implementation InputSyntaxTreeView




-(void)resetFont
{
		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
		
		NSString *name;
		NSInteger size;
		
		
		name = [defaults stringForKey : PREFERENCE_FONT_TREEVIEW_NAME];
		size = (NSInteger)[defaults doubleForKey : PREFERENCE_FONT_TREEVIEW_SIZE];
		
		[font release];
		font = [NSFont fontWithName : name
							   size : size
				];
		
		[font retain];
		
		
		NSLayoutManager *layout = [[NSLayoutManager alloc] init];
		
		[self setRowHeight: 
		 [layout defaultLineHeightForFont : font ] + 2
		 ];
		
		
		[self setFont:font];
		[self reloadData];
		
		[layout release];
}

- (void)setFont:(NSFont *)aFont;
{
		NSArray *tableColumns;
		unsigned int columnIndex;
		
		tableColumns = [self tableColumns];
		columnIndex = [tableColumns count];
		
		while (columnIndex--)
		{
				[ [ (NSTableColumn *)[tableColumns objectAtIndex:columnIndex] dataCell] 
				 setFont:aFont
				 ];
		}
}





-(void)handleFontChanged : (NSNotification*)note
{
		DLog(@"InputSyntaxTreeView::handleFontChanged for notification %@", note);
		[self resetFont];
		[self reloadData];
		
}



-(void)setHasValidParser : (BOOL)valid
{
		hasValidParser = valid;
}


-(void)awakeFromNib
{
		hasValidParser = NO;
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver : self
			   selector : @selector(handleFontChanged:)
				   name : NOTIFICATION_PREFERENCE_FONT_CHANGED
				 object : nil
		 ];
		
		
		
		[self setDataSource : self];
		[self resetFont];
		[self setDoubleAction : @selector(onDoubleClicked:)];
		
		
}


-(void)dealloc
{
		[self setSyntaxTree : nil];
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc removeObserver : self];
		
		[font release];
		font = nil;
		[super dealloc];
}



-(void)setSyntaxTree : (PrintNode*)node
{
		
		DLog(@"On InputSyntaxTreeView::setSyntaxTree");
		
		[node retain];
		[root release];
		root = node;
		[self reloadData];
}



- (NSMenu *)defaultMenu {
		
        NSMenu *theMenu = [[[NSMenu alloc] initWithTitle:@"SyntaxTreeView Context Menu"] autorelease];
		
        [theMenu insertItemWithTitle:@"Expand" action:@selector(expandTree:) keyEquivalent:@"" atIndex:0];
		[theMenu insertItemWithTitle:@"Collapse" action:@selector(collapseTree:) keyEquivalent:@"" atIndex:1];
	
		[theMenu insertItem : [NSMenuItem separatorItem]
					atIndex : 2
		 ];
		[theMenu insertItemWithTitle:@"Clear" action:@selector(clearTree:) keyEquivalent:@"" atIndex:3];
		
        return theMenu;
}



-(NSMenu *)menuForEvent:(NSEvent *)theEvent 
{
        return [self defaultMenu];  
}



-(void) expandTree :(id)sender
{
		DLog(@"On expandTree");
		
		PrintNode *node = [self itemAtRow:[self selectedRow]];
		
		if(node != nil)
		{
				
				[self expandItem : node expandChildren:YES];
				
		}
}

-(void) collapseTree :(id)sender
{
		DLog(@"On collapseTree");
		PrintNode *node = [self itemAtRow:[self selectedRow]];
		
		if(node != nil)
		{
				
				[self collapseItem : node];
				
		}
		
}


-(void) clearTree :(id)sender
{
		DLog(@"On clearTree");
		[self setSyntaxTree : nil];
		
}





-(void)onDoubleClicked : (id)sender
{
		DLog(@"On InputSyntaxTreeView::onDoubleClicked %@ sender", sender);
		
		PrintNode *node = [self itemAtRow:[self selectedRow]];
		
		if(node != nil)
		{

				DLog(@"InputSyntaxTreeView Sending Notification : %@", NOTIFICATION_SYNTAXTREEVIEW_INVOKE_RELOCATE);
				
				NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
				[nc postNotificationName :	NOTIFICATION_SYNTAXTREEVIEW_INVOKE_RELOCATE 
								  object :  self
								userInfo : [NSDictionary dictionaryWithObject : node
																	   forKey : @"UserInfo"
											]
				 ];
				
		}
		
}










-(NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item
{
		if(item == nil)
		{
				if(root == nil)
				{
						return 0;
				}else
				{
						return 1;
				}
		}else
		{
				return [item getChildCount];
		}
}



-(BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item 
{
		PrintNode *node = (PrintNode*)item;
		
		if(node == nil || node.isTerm == NO)
		{
				return YES;
		}else
		{
				return NO;
		}
}




-(id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item 
{
		PrintNode *node = (PrintNode*)item;
		
		if(node == nil)
		{
				return root;
		}else
		{
				assert(index < [node getChildCount]);
				return [node getChild : index];
		}
}


-(id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item 
{
		if(item == nil)
		{
				if(root == nil)
				{
						return @"";
				}else
				{
						return root.name;
				}
		}else
		{
				PrintNode *node = (PrintNode*)item;
				return node.name;
		}
}


@end
