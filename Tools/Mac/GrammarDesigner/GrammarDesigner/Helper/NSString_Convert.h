/*
 * Copyright (c) 2013 by Solidus
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

#ifndef __NSString_Convert_h
#define _NSString_Convert_h



#import "ArsenalOC.h"

@interface NSString (Convert)

-(const wchar_t*)cWideString;

+(id)stringWithBytes:(const void *)bytes length:(NSUInteger)length encoding:(NSStringEncoding)encoding;

+(id) stringWithWideString : (const wchar_t*)widestring;

-(const wchar_t*)cWideString;




@end



#endif
