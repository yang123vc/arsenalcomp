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


#import <Cocoa/Cocoa.h>
#import "stdheader.h"


@class Preferences;
@class StringConverter;
@class GrammarCheckWorker;
@class EncodingConverter;

@interface ApplicationDelegate : NSObject<NSApplicationDelegate> 
{
		Preferences             *preferences;
		StringConverter         *stringConverter;
        EncodingConverter       *encodingConverter;
}


-(IBAction)showPreferencePanel : (id)sender;
-(IBAction)showStringConverter : (id)sender;
-(IBAction)showEncodingConverter : (id)sender;


/**************Test*********************/

-(IBAction)onTest : (id)sender;


@end
