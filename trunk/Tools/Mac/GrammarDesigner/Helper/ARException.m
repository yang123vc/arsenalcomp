////  ARException.m//  ARHelper////  Created by liu peng on 9/15/11.//  Copyright 2011 none. All rights reserved.//#import "ARException.h"@implementation ARException+(id)exceptionWithReason : (NSString*)reason{		Class c = [self class];		ARException *expt = [[c alloc] initWithReason : reason];		[expt autorelease];		return expt;}-(id)init{		return [self initWithReason : @"Unknow exception"];}-(id)initWithReason : (NSString*)reason{		NSString *name = [NSString stringWithFormat : @"%@", [self class]];						self = [super initWithName: name						   reason : reason 						  userInfo:	nil				];				return self;}-(NSString*)what{		return [self reason];}@endIMPL_EXCEPTION(ARRuntimeException)IMPL_EXCEPTION(ARSystemException)IMPL_EXCEPTION(ARStringConvertException)	