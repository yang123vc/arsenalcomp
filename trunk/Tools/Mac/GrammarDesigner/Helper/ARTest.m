/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "Common.h"#import "ARThread.h"/********************************/#import "ARUtility.h"void msgbox_nsstr(NSString *msg){		NSRunAlertPanel(@"Warning", msg, @"OK", nil, nil);}static void utility_test(){		wchar_t *ws = L"中国字真难!";				NSString *s = [ARUtility convertUTF32ToNSString : ws];				msgbox_nsstr(s);}/*************************************************************************/void AR_Test(){		utility_test();		}