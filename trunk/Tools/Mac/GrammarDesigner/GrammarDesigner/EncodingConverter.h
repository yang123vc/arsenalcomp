//
//  EncodingConverter.h
//  GrammarDesigner
//
//  Created by Solidus on 12-7-31.
//  Copyright (c) 2012年 none. All rights reserved.
//

#import "stdheader.h"

@interface EncodingConverter : NSWindowController
{
        NSFont                  *default_font;
        IBOutlet NSTextView     *input;
        IBOutlet NSTextView     *output;
        
        IBOutlet NSMatrix       *charset;
        IBOutlet NSMatrix       *type;
        IBOutlet NSMatrix       *action;
}


-(void)show : (id)sender;
-(IBAction)convert : (id)sender;

@end
