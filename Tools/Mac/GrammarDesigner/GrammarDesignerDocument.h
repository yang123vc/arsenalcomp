/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import <Cocoa/Cocoa.h>#import "stdheader.h"@interface GrammarDesignerDocument : NSDocument{		NSString		*grammarContent;}-(void)reloadPreferences;-(IBAction)setDocumentLexerSingleLine : (id)sender;-(IBAction)setDocumentLexerIgnoreCase : (id)sender;-(IBAction)setDocumentParserModeSLR : (id)sender;-(IBAction)setDocumentParserModeLALR : (id)sender;-(IBAction)buildDocumentParser	:	(id)sender;-(IBAction)parseDocumentInput	:	(id)sender;-(IBAction)rebuildDocumentTags	:	(id)sender;-(IBAction)generateDocumentGrammarTemplateCode : (id)sender;@end