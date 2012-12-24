//
//  EncodingConverter.m
//  GrammarDesigner
//
//  Created by Solidus on 12-7-31.
//  Copyright (c) 2012年 none. All rights reserved.
//

#include <openssl/md5.h>
#include <openssl/sha.h>

#import "EncodingConverter.h"


static void __digit_to_hex_char(ar_byte_t b, char tmp[2])
{
		ar_uint_32_t v = (ar_uint_32_t)b;
		static const char *__tbl = "0123456789ABCDEF";
        
		tmp[1] = __tbl[v % 16];
		v /= 16;
		tmp[0] = __tbl[v % 16];
}


static ar_int_t	AR_data_to_hexstr(const ar_byte_t *data, size_t l, char *out, size_t len)
{
		size_t i, si;
		AR_ASSERT(data != NULL && l > 0);
        
		if(out == NULL)
		{
				return l * 2 + 1;
		}else
		{
				if(len < l * 2 + 1)
				{
						return -1;
				}
                
				for(i = 0,si = 0; i < l; ++i)
				{
						__digit_to_hex_char(data[i], out + si);
						si += 2;
						AR_ASSERT(si < len);
				}
                
				out[si] = '\0';
				return si + 1;
		}
}



static arStatus_t  __hex_char_to_digit(char c, ar_byte_t *d)
{
		AR_ASSERT(d != NULL);
        
		if(c >= '0' && c <= '9')
		{
				*d = c - '0';
		}else if(c >= 'a' && c <= 'f')
		{
				*d = 10 + c - 'a';
		}else if(c >= 'A' && c <= 'F')
		{
				*d = 10 + c - 'A';
		}else
		{
				return AR_E_INVAL;
		}
		return AR_S_YES;
}


static ar_int_t	AR_hexstr_to_data_s(const char *b, const char *e, ar_byte_t *data, size_t len)
{
		ar_byte_t *p;
		arStatus_t status;
		size_t need_l;
		AR_ASSERT(b != NULL && e != NULL && b < e);
		
		need_l = e - b;
        
		if(need_l % 2 != 0)
		{
				return -1;
		}
        
		if(data == NULL)
		{
				return (ar_int_t)need_l / 2;
		}
        
		if(len < need_l / 2)
		{
				return -1;
		}
		
		p = data;
		while(b < e)
		{
				ar_byte_t d1,d2;
				
				status = __hex_char_to_digit(*b, &d1);
                
				if(status != AR_S_YES)
				{
						return -1;
				}
                
				b++;
				AR_ASSERT(b < e);
                
				status = __hex_char_to_digit(*b, &d2);
                
				if(status != AR_S_YES)
				{
						return -1;
				}
                
				*p++ = d1 * 16 + d2;
				++b;
		}
		
		return need_l / 2;
}



static ar_int_t           AR_hexstr_to_data(const char *s, ar_byte_t *data, size_t len)
{
        AR_ASSERT(s != NULL);
        return AR_hexstr_to_data_s(s, s + AR_strlen(s), data, len);
}





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





static NSString*	rot13_convert(NSString *input, arCodePage_t cp)
{
        		
		AR_ASSERT(input != NULL);
        
        
        if([input length] == 0)
        {
                return @"empty input !";
        }

        @try{
                WideCharWrapper *wcs_wrapper = [ARUtility convertNSStringToUTF32 : input];
                
                char *mbs = AR_wcs_to_str(cp, [wcs_wrapper string], AR_wcslen([wcs_wrapper string]));
                
                if(mbs == NULL)
                {
                        return @"convert string to mbs failed";
                }
                AR_strrot13(mbs, AR_strlen(mbs));
                wchar_t *wcs = AR_str_to_wcs(cp, mbs, AR_strlen(mbs));
                
                if(wcs == NULL)
                {
                        AR_DEL(mbs);
                        mbs = NULL;
                        return @"convert string to wcs failed";
                }
                NSString *ret = [ARUtility convertUTF32ToNSString : wcs];
                

                AR_DEL(wcs);
                AR_DEL(mbs);
                wcs = NULL;
                mbs = NULL;
                return ret;
        
        }@catch(NSException *e)
        {
                return [e reason];
        }
        
}





static NSString* base64_convert(NSString *input, arCodePage_t cp, ar_bool_t is_encode)
{
		
		AR_ASSERT(input != NULL);
        
		if([input length] == 0)
		{
                return @"empty input!";
		}
        @try{
                
		if(is_encode)
		{
                WideCharWrapper *wcs_wrapper = [ARUtility convertNSStringToUTF32 : input];
                char *mbs = AR_wcs_to_str(cp, [wcs_wrapper string], AR_wcslen([wcs_wrapper string]));
                
                if(mbs == NULL)
                {
                        return @"invalid charset";
                }
                size_t n = AR_base64_encode(NULL, 0, (const ar_byte_t*)mbs, AR_strlen(mbs));
                
				if(n == 0)
				{
                        AR_DEL(mbs);
                        mbs = NULL;
                        return @"invalid input!";
				}
                
				ar_byte_t *tmp = AR_NEWARR(ar_byte_t, n + 1);
                n = AR_base64_encode(tmp, n, (const ar_byte_t*)mbs, AR_strlen(mbs));
				tmp[n] = 0;
                
                wchar_t *wcs = AR_str_to_wcs(AR_CP_UTF8, (const char*)tmp, n);
                
                if(wcs == NULL)
                {
                        AR_DEL(mbs);
                        AR_DEL(tmp);
                        mbs = NULL;
                        tmp = NULL;
                        return @"invalid charset";
                }
                
                
                NSString *ret = [ARUtility convertUTF32ToNSString : wcs];
                
                AR_DEL(mbs);
                AR_DEL(tmp);
                AR_DEL(wcs);
                mbs = NULL;
                tmp = NULL;
                wcs = NULL;
                return ret;
		}else
		{
                WideCharWrapper *wcs_wrapper = [ARUtility convertNSStringToUTF32 : input];
                char *mbs = AR_wcs_to_str(AR_CP_UTF8, [wcs_wrapper string], AR_wcslen([wcs_wrapper string]));
                
                if(mbs == NULL)
                {
                        return @"invalid charset";
                }
                                
				size_t n = AR_base64_decode(NULL, 0, (const ar_byte_t*)mbs, AR_strlen(mbs));

                if(n == 0)
				{
                        AR_DEL(mbs);
                        mbs = NULL;
                        return @"invalid input!";
				}
                
				ar_byte_t *tmp = AR_NEWARR(ar_byte_t, n + 1);
                n = AR_base64_decode(tmp, n, (const ar_byte_t*)mbs, AR_strlen(mbs));
				tmp[n] = 0;
                wchar_t *wcs = AR_str_to_wcs(cp, (const char*)tmp, n);
                
                if(wcs == NULL)
                {
                        AR_DEL(mbs);
                        AR_DEL(tmp);
                        mbs = NULL;
                        tmp = NULL;
                        return @"invalid charset";
                }

                NSString *ret = [ARUtility convertUTF32ToNSString : wcs];
                
                AR_DEL(mbs);
                AR_DEL(tmp);
                AR_DEL(wcs);
                mbs = NULL;
                tmp = NULL;
                wcs = NULL;
                return ret;
		}
                                    
        }@catch(NSException *e)
        {
                return [e reason];
        }
}


static NSString*	hex_convert(NSString *input, arCodePage_t cp, ar_bool_t is_encode)
{
        
		AR_ASSERT(input != NULL);
        
		if([input length] == 0)
		{
                return @"empty input!";
		}
        @try{
                
                if(is_encode)
                {
                        WideCharWrapper *wcs_wrapper = [ARUtility convertNSStringToUTF32 : input];
                        char *mbs = AR_wcs_to_str(cp, [wcs_wrapper string], AR_wcslen([wcs_wrapper string]));
                        
                        if(mbs == NULL)
                        {
                                return @"invalid charset";
                        }
                        
                        size_t l = AR_strlen(mbs);
                        char *tmp = AR_NEWARR(char, l * 2 + 1);
                        AR_data_to_hexstr((const ar_byte_t*)mbs, l, tmp, l * 2 + 1);
                        
                        wchar_t *wcs = AR_str_to_wcs(AR_CP_UTF8, (const char*)tmp, AR_strlen(tmp));
                        
                        if(wcs == NULL)
                        {
                                AR_DEL(mbs);
                                AR_DEL(tmp);
                                mbs = NULL;
                                tmp = NULL;
                                return @"invalid charset";
                        }
                        
                        
                        NSString *ret = [ARUtility convertUTF32ToNSString : wcs];
                        
                        AR_DEL(mbs);
                        AR_DEL(tmp);
                        AR_DEL(wcs);
                        mbs = NULL;
                        tmp = NULL;
                        wcs = NULL;
                        return ret;
                }else
                {
                        WideCharWrapper *wcs_wrapper = [ARUtility convertNSStringToUTF32 : input];
                        char *mbs = AR_wcs_to_str(cp, [wcs_wrapper string], AR_wcslen([wcs_wrapper string]));
                        
                        if(mbs == NULL)
                        {
                                return @"invalid charset";
                        }
                        
                        size_t l = AR_strlen(mbs);
                        char *tmp = AR_NEWARR(char, l * 2 + 1);
                        
                        ar_int_t n = AR_hexstr_to_data(mbs, (ar_byte_t*)tmp, l * 2 + 1);
                        if(n <= -1)
                        {
                                AR_DEL(tmp);
                                AR_DEL(mbs);
                                return @"invalid input!";
                        }
                        tmp[n] = 0;
                        
                        wchar_t *wcs = AR_str_to_wcs(AR_CP_UTF8, (const char*)tmp, AR_strlen(tmp));
                        
                        if(wcs == NULL)
                        {
                                AR_DEL(mbs);
                                AR_DEL(tmp);
                                mbs = NULL;
                                tmp = NULL;
                                return @"invalid charset";
                        }
                        
                        
                        NSString *ret = [ARUtility convertUTF32ToNSString : wcs];
                        
                        AR_DEL(mbs);
                        AR_DEL(tmp);
                        AR_DEL(wcs);
                        mbs = NULL;
                        tmp = NULL;
                        wcs = NULL;
                        return ret;
                }
                
        }@catch(NSException *e)
        {
                return [e reason];
        }
		
}






static NSString* cstr_convert(NSString *input, ar_bool_t is_encode)
{
        
		AR_ASSERT(input != NULL);
        
		if([input length] == 0)
		{
                return @"empty input!";
		}
        
        NSString *ret = @"";
        
        @try{
                
                WideCharWrapper *wcs_wrapper = [ARUtility convertNSStringToUTF32 : input];
                
                if(is_encode)
                {
                        ar_int_t len = AR_str_to_escstr_buf(NULL, 0, [wcs_wrapper string]);
                        
                        if(len == 0)
                        {
                                ret = @"string to cstring convert failed";
                                return ret;
                        }
                        
                        wchar_t *code = AR_NEWARR(wchar_t, len + 1);
                        len = AR_str_to_escstr_buf(code, len, [wcs_wrapper string]);

                        ret = [ARUtility convertUTF32ToNSString : code];
                        
                        AR_DEL(code);
                        code = NULL;
                        
                }else
                {
                        
                        arEscStrErr_t	err;
                        ar_int_t len;
                        
                        len = AR_escstr_to_str_buf(NULL, 0, [wcs_wrapper string], &err);
                        
                        if(len == 0)
                        {
                                ret = @"zero length string";
                                return ret;
                        }
                        
                        NSString *info = NULL;
                        
                        if(err.type == AR_ESCSTR_ERR_VALUE)
                        {
                                info = [NSString stringWithFormat : @"Invalid value %d : too big for wide character\r\n",
                                        (int)err.value
                                        ];
                        }else if(err.type == AR_ESCSTR_ERR_CHAR)
                        {
                                
                                info = [NSString stringWithFormat : @"Invalid char \\x%x : unrecognized character escape sequence\r\n",
                                        (unsigned int)*err.pos
                                        ];
                                
                        }else if(err.type == AR_ESCSTR_ERR_BUFFER)
                        {
                                AR_ASSERT(false);
                                info = @"error";
                        }
                        
                        if(info != nil)
                        {
                                ret = info;
                                return ret;
                        }
                        
                        
                        wchar_t *str = AR_NEWARR(wchar_t, len + 1);

                        len = AR_escstr_to_str_buf(str, len, [wcs_wrapper string], &err);
                        ret = [ARUtility convertUTF32ToNSString : str];
                        AR_DEL(str);
                        str = NULL;

                        if(ret == nil)
                        {
                                ret = @"convert to nsstring failed"; 
                        }
                        
                        return ret;
                }
        }@catch(NSException *e)
        {
                ret = [e reason];
        }
        
        return ret;
}

static NSString* url_convert(NSString *input, arCodePage_t cp, ar_bool_t is_encode)
{
		arURI_t *uri;
		arString_t *str;
        NSString *ret;
		AR_ASSERT(input != NULL);
        
		if([input length] == 0)
		{
                return @"empty input!";
		}
        str = AR_CreateString();
        uri = AR_CreateURI(cp);
        ret = @"";
        
        
        @try{
                WideCharWrapper *wcs_wrapper = [ARUtility convertNSStringToUTF32 : input];
                
                if(is_encode)
                {
                        if(AR_EncodeURLString(cp, [wcs_wrapper string], str) != AR_S_YES)
                        {
                                ret = @"input is not valid url!";
                                goto END_POINT;
                        }
                        
                        ret = [ARUtility convertUTF32ToNSString : AR_GetStringCString(str)];
                }else
                {
                        if(AR_DecodeURLString(cp, [wcs_wrapper string], str) != AR_S_YES)
                        {
                                ret = @"input is not valid url!";
                                goto END_POINT;
                        }
                        
                        ret = [ARUtility convertUTF32ToNSString : AR_GetStringCString(str)];
                }
        }@catch(NSException *e)
        {
                ret = [e reason];
        }
        
END_POINT:
        if(uri)
        {
                AR_DestroyURI(uri);
                uri = NULL;
        }
        
        if(str)
        {
                AR_DestroyString(str); 
                str = NULL;
        }
        return ret;
}




static NSString* md5_convert(NSString *input, arCodePage_t cp)
{
		AR_ASSERT(input != NULL);
        
		if([input length] == 0)
		{
                return @"empty input!";
		}
        
        NSString *ret = @"";
        
        @try{
                
                WideCharWrapper *wcs_wrapper = [ARUtility convertNSStringToUTF32 : input];
                char *mbs = AR_wcs_to_str(cp, [wcs_wrapper string], AR_wcslen([wcs_wrapper string]));
                if(mbs == NULL)
                {
                        ret = @"invalid charset";
                        return ret;
                }
                
                ar_byte_t md5[16];
                AR_memset(md5, 0, 16);
                MD5((const unsigned char*)mbs, AR_strlen(mbs), (unsigned char*)md5);
                AR_DEL(mbs);
                mbs = NULL;
                
                char buf[33];
                AR_data_to_hexstr(md5, 16, buf, 33);
                buf[32] = '\0';
                
                wchar_t *wcs = AR_str_to_wcs(AR_CP_UTF8, buf, 32);
                ret = [ARUtility convertUTF32ToNSString : wcs];
                
                return ret;

        }@catch(NSException *e)
        {
                ret = [e reason];
        }
        
        return ret;
}





static NSString* sha1_convert(NSString *input, arCodePage_t cp)
{
		AR_ASSERT(input != NULL);
        
		if([input length] == 0)
		{
                return @"empty input!";
		}
        
        NSString *ret = @"";
        
        @try{
                
                WideCharWrapper *wcs_wrapper = [ARUtility convertNSStringToUTF32 : input];
                char *mbs = AR_wcs_to_str(cp, [wcs_wrapper string], AR_wcslen([wcs_wrapper string]));
                
                if(mbs == NULL)
                {
                        ret = @"invalid charset";
                        return ret;
                }
                
                ar_byte_t hash[20];
                AR_memset(hash, 0, 20);
                
                SHA1((const unsigned char*)mbs, AR_strlen(mbs), (unsigned char*)hash);
                AR_DEL(mbs);
                mbs = NULL;
                
                char buf[41];
                AR_data_to_hexstr(hash, 20, buf, 41);
                buf[40] = '\0';
                
                wchar_t *wcs = AR_str_to_wcs(AR_CP_UTF8, buf, 40);
                ret = [ARUtility convertUTF32ToNSString : wcs];
                
                return ret;
                
        }@catch(NSException *e)
        {
                ret = [e reason];
        }
        
        return ret;
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


typedef enum
{
        CHARSET_CURRENT = 0,
        CHARSET_UTF8   = 1,
        CHARSET_GB2312  = 2,
        CHARSET_BIG5    = 3
}charsetType_t;

typedef enum
{
        INPUT_BASE64 = 0,
        INPUT_ROT13 = 1,
        INPUT_HEX = 2,
        INPUT_URL = 3,
        INPUT_MD5 = 4,
        INPUT_SHA1= 5,
        INPUT_CSTR = 6
}inputType_t;

typedef enum
{
        ACT_ENCODE = 0,
        ACT_DECODE = 1
}actionType_t;

-(void)convert : (id)sender
{
        arCodePage_t    cp;
        charsetType_t   cset;
        inputType_t     input_type;
        actionType_t    act_type;
        NSButtonCell    *cell;
        NSString        *input_text;
        DLog(@"On EncodingConverter::convert");

        cell = [charset selectedCell];
        AR_ASSERT(cell != nil);
        cset = (charsetType_t)[cell tag];
        
        if(cset == 0)
        {
                cp = AR_CP_ACP;
                
        }else if(cset == 1)
        {
                cp = AR_CP_UTF8;
        }else if(cset == 2)
        {
                cp = AR_CP_GB2312;
        }else
        {
                cp = AR_CP_BIG5;
        }

        
        
        cell = [type selectedCell];
        AR_ASSERT(cell != nil);
        input_type = (inputType_t)[cell tag];
        
        cell = [action selectedCell];
        AR_ASSERT(cell != nil);
        act_type = (actionType_t)[cell tag];
        
        //NSRunAlertPanel(@"Warning", @"cset = %d, input = %d, action = %d", @"OK", nil,nil, cset, input_type, act_type);
        
        input_text = [input string];
        
        if([input_text length] == 0)
        {
                return;
        }
        
        switch(input_type)
        {
                case INPUT_BASE64:
                {
                        NSString *ret = base64_convert(input_text, cp, act_type == ACT_ENCODE ? true : false);
                        
                        if(ret != nil)
                        {
                                [output setString : ret];
                        }
                        
                }
                        break;
                case INPUT_ROT13:
                {
                        NSString *ret = rot13_convert(input_text, cp);
                        
                        if(ret != nil)
                        {
                                [output setString : ret];
                        }
                }
                        break;
                case INPUT_HEX:
                {
                        NSString *ret = hex_convert(input_text, cp, act_type == ACT_ENCODE ? true : false);
                        
                        if(ret != nil)
                        {
                                [output setString : ret];
                        }
                }
                        break;
                case INPUT_URL:
                {
                        NSString *ret = url_convert(input_text, cp, act_type == ACT_ENCODE ? true : false);
                        
                        if(ret != nil)
                        {
                                [output setString : ret];
                        }
                }
                        break;
                case INPUT_CSTR:
                {
                        NSString *ret = cstr_convert(input_text, act_type == ACT_ENCODE ? true : false);
                        
                        if(ret != nil)
                        {
                                [output setString : ret];
                        }
                }
                        break;
                        
                case INPUT_MD5:
                {
                        NSString *ret = md5_convert(input_text, cp);
                        
                        if(ret != nil)
                        {
                                [output setString : ret];
                        }
                }
                        
                        break;
                case INPUT_SHA1:
                {
                        NSString *ret = sha1_convert(input_text, cp);
                        
                        if(ret != nil)
                        {
                                [output setString : ret];
                        }
                }
                        break;
                
                default:
                        AR_ASSERT(false);
        }
        
        
}



@end
