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

#import "NSString_Convert.h"


@implementation NSString (Convert)


+(id)stringWithBytes:(const void *)bytes length:(NSUInteger)length encoding:(NSStringEncoding)encoding
{
        NSString *ret = [ [NSString alloc] initWithBytes : bytes
                                                  length : length
                                                encoding : encoding
                         ];
        

        AR_ASSERT(ret != NULL);
        
        [ret autorelease];
        return ret;
}


+(id) stringWithWideString : (const wchar_t*)widestring
{
        NSStringEncoding enc;
        size_t len;
        assert(widestring != NULL);
        
        len = wcslen(widestring);
        if(len == 0)
        {
                return @"";
        }
        
#if defined(ARCH_LITTLE_ENDIAN)
        enc = NSUTF32LittleEndianStringEncoding;
#else
        enc = NSUTF32BigEndianStringEncoding;
#endif
        
        //printf("%ls\r\n", widestring);
        
        return [NSString stringWithBytes : (const void*)widestring
                                  length : len *sizeof(wchar_t)
                                encoding : enc
                ];
}



-(const wchar_t*)cWideString
{
        NSStringEncoding enc;

#if defined(ARCH_LITTLE_ENDIAN)
        enc = NSUTF32LittleEndianStringEncoding;
#else
        enc = NSUTF32BigEndianStringEncoding;
#endif

        return (const wchar_t*)[self cStringUsingEncoding : enc];

}


@end
