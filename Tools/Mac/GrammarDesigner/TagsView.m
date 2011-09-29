/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "TagsView.h"@implementation TagsItemInfo@synthesize		name;@synthesize		line;@synthesize		isRefreshed;+(id)TagsItemWithName : (NSString*)n Line : (int)l{		TagsItemInfo *info = [[TagsItemInfo alloc] initWithName : n														   Line : l							  ];						[info autorelease];		return info;							  }-(id)init{		self = [super init];		if(self)		{				name = [NSString stringWithString : @""];				line = -1;				isRefreshed = NO;		}		return self;}-(id)initWithName : (NSString*)n Line : (int)l{		assert(n != nil);		self = [super init];		if(self)		{				[n retain];				name = n;				line = l;		}		return self;}-(void)dealloc{		[name release];		name = nil;		[super dealloc];}@end@implementation TagsView-(void)resetFont{		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];				NSString *name;		NSInteger size;						name = [defaults stringForKey : PREFERENCE_FONT_TAGS_NAME];		size = (NSInteger)[defaults doubleForKey : PREFERENCE_FONT_TAGS_SIZE];				[font release];		font = [NSFont fontWithName : name							   size : size				];				[font retain];				[self setFont : font];}-(void)handleFontChanged : (NSNotification*)note{		DLog(@"TagsView::handleFontChanged for notification %@", note);		[self resetFont];		}-(void)awakeFromNib{		nameSet = [[NSMutableArray alloc] init];		termSet = [[NSMutableArray alloc] init];		precSet = [[NSMutableArray alloc] init];		ruleSet = [[NSMutableArray alloc] init];		predefSet = [[NSMutableArray alloc] init];		actionSet  = [[NSMutableArray alloc] init];				NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc addObserver : self			   selector : @selector(handleFontChanged:)				   name : NOTIFICATION_PREFERENCE_FONT_CHANGED				 object : nil		 ];				[self setDataSource : self];		[self resetFont];				[self setDoubleAction : @selector(onDoubleClicked:)];}-(void)dealloc{		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc removeObserver : self];				[nameSet release];		[termSet release];		[precSet release];		[ruleSet release];		[predefSet release];		[actionSet release];						[font release];		font = nil;		[super dealloc];}-(BOOL)isRootItem : (id)item{		if(item == nil || item == nameSet || item == termSet || item == precSet || item == ruleSet || item == predefSet || item == actionSet)		{				return YES;		}else		{				return NO;		}}-(id)getRootItem : (int)idx{		switch(idx)		{				case 0:						return nameSet;				case 1: 						return termSet;				case 2:						return precSet;				case 3:						return ruleSet;				case 4:						return predefSet;				case 5:						return actionSet;				default:						return nil;		}}-(NSString*)getRootItemName : (id)item{		NSString *ret = nil;				if(item == nameSet)		{				ret = [NSString stringWithFormat : @"Name(%d)", [nameSet count]];						}else if(item == termSet)		{				ret = [NSString stringWithFormat : @"Term(%d)", [termSet count]];		}else if(item == precSet)		{				ret = [NSString stringWithFormat : @"Prec(%d)", [precSet count]];		}else if(item == ruleSet)		{				ret = [NSString stringWithFormat : @"Rule(%d)", [ruleSet count]];		}else if(item == predefSet)		{				ret = [NSString stringWithFormat : @"PreDef(%d)", [predefSet count]];						}else if(item == actionSet)		{				ret = [NSString stringWithFormat : @"Action(%d)", [actionSet count]];		}else		{				ret = @"";		}		return ret;}-(void)onDoubleClicked : (id)sender{		DLog(@"On TagsView::onDoubleClicked %@ sender", sender);				TagsItemInfo *info = [self itemAtRow:[self selectedRow]];				if(![self isRootItem : info])		{				DLog(@"TagsView Sending Notification : %@", NOTIFICATION_TAGSVIEW_INVOKE_RELOCATE_TEXT);								NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];				[nc postNotificationName :	NOTIFICATION_TAGSVIEW_INVOKE_RELOCATE_TEXT 								  object :  self								userInfo : [NSDictionary dictionaryWithObject : [NSNumber numberWithInt : info.line]																	   forKey : @"UserInfo"											]				 ];						}}		   		   		   		   -(NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item{		return (item == nil) ? 6 : [item count];}-(BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item {		if([self isRootItem : item])		{				if(item == nil)				{						return YES;				}								if([item count] > 0)				{						return YES;				}								return NO;		}else		{				return NO;		}}-(id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item {		if(item == nil)		{				return [self getRootItem : index];		}else		{				return [item objectAtIndex : index];		}}-(id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item {		if([self isRootItem : item])		{				return [self getRootItemName :item];		}else		{				return [item name];		}}-(void)change_state : (NSMutableArray*)set {		const size_t count = [set count];				for(size_t i = 0; i < count; ++i)		{				TagsItemInfo *info = [set objectAtIndex : i];				info.isRefreshed = NO;		}}-(void)updateSet : (NSMutableArray*)set 			Name : (NSString*)name 			Line : (int)line{		size_t i;		assert(set != nil && name != nil && line >= 0);		const size_t count = [set count];				TagsItemInfo *info = nil;		for(i = 0; i < count; ++i)		{				TagsItemInfo *tmp = [set objectAtIndex : i];				assert(tmp != nil);				if([tmp.name compare : name] == NSOrderedSame)				{						info = tmp;						break;				}		}				if(info)		{				info.line = line;				info.isRefreshed = YES;		}else		{				info = [TagsItemInfo TagsItemWithName : name												 Line : line						];				info.isRefreshed = YES;				[set addObject : info];		}}-(void)removeUnusedItem : (NSMutableArray*)set{		const size_t count = [set count];				NSMutableArray *rm = [NSMutableArray arrayWithCapacity : 20];				for(size_t i = 0; i < count; ++i)		{				TagsItemInfo *info = [set objectAtIndex : i];				if(!info.isRefreshed)				{						[rm addObject : info];				}		}				[set removeObjectsInArray : rm];		}-(void)updateActionTable : (const cfgConfig_t*)cfg{		assert(cfg != NULL);		NSMutableDictionary *dict = [[NSMutableDictionary alloc]init];						for(size_t i = 0; i < cfg->tok_cnt; ++i)		{				const cfgToken_t *tok = &cfg->tok[i];				int line = -1;				BOOL has_definition = NO;				if(tok->action_name != NULL)				{						NSString *str = [ARUtility convertUTF32ToNSString : tok->action_name];						line = (int)tok->action_line;						has_definition = tok->action_ins == NULL ? NO : YES;												TagsItemInfo *val = [dict objectForKey : str];												if(val != nil)						{								if(has_definition)								{										val.line = line;								}else								{								}						}else						{								val = [TagsItemInfo TagsItemWithName : str																Line : line									   ];								[dict setObject : val 										 forKey : val.name								 ];						}				}		}						for(size_t i = 0; i < cfg->rule_cnt; ++i)		{				if(cfg->rule[i].action_name != NULL)				{						NSString *str = nil;						int		line = -1;						BOOL	has_definition = NO;												str = [ARUtility convertUTF32ToNSString : cfg->rule[i].action_name];						line = cfg->rule[i].action_line;						has_definition = cfg->rule[i].action_ins == NULL ? NO : YES;												TagsItemInfo *val = [dict objectForKey : str];												if(val != nil)						{								if(has_definition)								{										val.line = line;								}else								{								}						}else						{								val = [TagsItemInfo TagsItemWithName : str																Line : line									   ];																[dict setObject : val 										 forKey : val.name								 ];						}				}		}				TagsItemInfo *value;		NSEnumerator *enumerator = [dict objectEnumerator];				while ((value = [enumerator nextObject])) 		{				[self updateSet : actionSet						  Name  :  value.name 						   Line : value.line				 ];		}		[dict release];}-(void)update : (const cfgConfig_t*)cfg{		[self change_state : nameSet];		[self change_state : termSet];		[self change_state : precSet];		[self change_state : ruleSet];		[self change_state : predefSet];		[self change_state : actionSet];				if(cfg != nil)		{				for(size_t i = 0; i < cfg->name_cnt; ++i)				{						NSString *str = nil;						const cfgName_t *name = &cfg->name[i];						if(name->name == NULL)						{								str = [NSString stringWithFormat : @"Null Token %d", i];						}else							{										str = [ARUtility convertUTF32ToNSString : name->name];						}										[self updateSet : nameSet								  Name  :  str 								   Line : name->line						 ];				}								for(size_t i = 0; i < cfg->tok_cnt; ++i)				{						const cfgToken_t *tok = &cfg->tok[i];						if(tok->tokval == 0)continue;						NSString *str;						if(tok->name == NULL)						{								str = [NSString stringWithFormat : @"Null Token %d", i];						}else						{								str = [ARUtility convertUTF32ToNSString : tok->name];						}												[self updateSet : termSet								  Name  :  str 								   Line : tok->line						 ];										}												for(size_t i = 0; i < cfg->prec_cnt; ++i)				{						size_t k;						NSString *str;												const cfgPrec_t *prec = &cfg->prec[i];												for(k = 0; k < prec->count; ++k)						{								assert(prec->prec_tok_set[k] != NULL);								const wchar_t *tmp = prec->prec_tok_set[k];								str = [ARUtility convertUTF32ToNSString : tmp];																[self updateSet : precSet										  Name  :  str 										   Line : prec->line								 ];						}				}												for(size_t i = 0; i < cfg->rule_cnt; ++i)				{						NSString *str;						const cfgRule_t *rule = &cfg->rule[i];												assert(rule->lhs != NULL);												str = [ARUtility convertUTF32ToNSString : rule->lhs];												[self updateSet : ruleSet								   Name : str								   Line : rule->line						 ];				}								size_t	code_i = 0;				for(size_t i = 0; i < cfg->predef_cnt; ++i)				{						const cfgPreDef_t *def = &cfg->pre_def[i];						NSString *str;												if(def->name == NULL || wcslen(def->name) == 0)						{								str = [NSString stringWithFormat : @"Code %d",									   code_i									   ];								code_i++;						}else						{								str =  [ARUtility convertUTF32ToNSString : def->name];						}												[self updateSet : predefSet								   Name : str								   Line : def->line						 ];				}								[self updateActionTable : cfg];						}					   [self removeUnusedItem : nameSet ];	   [self removeUnusedItem : termSet];	   [self removeUnusedItem :  precSet];	   [self removeUnusedItem :  ruleSet];	   [self removeUnusedItem : predefSet];	   [self removeUnusedItem : actionSet];				[self reloadData];		[self expandItem : nil expandChildren:YES];		}@end