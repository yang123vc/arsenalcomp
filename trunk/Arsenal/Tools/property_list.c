//
//  property_list.c
//  Arsenal
//
//  Created by solidus on 13-8-8.
//  Copyright (c) 2013年 none. All rights reserved.
//


#include "../Lex/lex.h"
#include "property_list.h"


AR_NAMESPACE_BEGIN


/*this module is modified from apple CF-550*/

/*
typedef struct __plist_xml_parser
{
        
        arString_t      *content;
        
        const wchar_t   *begin;
        const wchar_t   *curr;
        const wchar_t   *end;
        
        arString_t      *errmsg;
}plistXMLParser_t;
*/

plistXMLParser_t*       PList_CreateXMLParser()
{
        plistXMLParser_t        *parser;
        
        parser = NULL;
        
        parser = AR_NEW(plistXMLParser_t);
        
        if(parser == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        parser->content = AR_CreateString();
        parser->errmsg = AR_CreateString();
        
        if(parser->content == NULL || parser->errmsg == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        parser->begin = NULL;
        parser->curr = NULL;
        parser->end = NULL;
        parser->has_error = false;
        
        parser->root = NULL;
        
        return parser;
        
        
INVALID_POINT:
        
        if(parser && parser->content)
        {
                AR_DestroyString(parser->content);
                parser->content = NULL;
        }
        
        if(parser && parser->errmsg)
        {
                AR_DestroyString(parser->errmsg);
                parser->errmsg = NULL;
        }
        
        if(parser)
        {
                AR_DEL(parser);
                parser = NULL;
        }
        return parser;
}

void                    PList_DestroyXMLParser(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL);
        
        AR_DestroyString(parser->content);
        parser->content = NULL;
        
        AR_DestroyString(parser->errmsg);
        parser->errmsg = NULL;
        
        AR_DEL(parser);
        parser = NULL;
}


void                    PList_ClearXMLParser(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL);
        
        AR_ClearString(parser->content);
        AR_ClearString(parser->errmsg);
        parser->begin = NULL;
        parser->curr = NULL;
        parser->end = NULL;
        parser->has_error = false;
        
        parser->root = NULL;
}


arStatus_t              PList_SetXMLParserWithWcs(plistXMLParser_t *parser, const wchar_t *xml, size_t length)
{
        arStatus_t      status;
        AR_ASSERT(parser != NULL && xml != NULL && length > 0);
        
        status = AR_S_YES;
        PList_ClearXMLParser(parser);
        
        status = AR_SetStringN(parser->content, xml, length);
        
        if(status != AR_S_YES)
        {
                PList_ClearXMLParser(parser);
                goto END_POINT;
        }
        
        parser->begin = AR_CSTR(parser->content);
        parser->curr = parser->begin;
        parser->end = parser->begin + AR_GetStringLength(parser->content);
        
END_POINT:
        return status;
}


const wchar_t*          PList_GetXMLParserErrorMessage(const plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL);
        if(parser->has_error)
        {
                return L"";
        }else
        {
                return AR_CSTR(parser->errmsg);
        }
}





static const wchar_t __g_plist_tags[13][10]=
{
        {L'p', L'l', L'i', L's', L't', L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'a', L'r', L'r', L'a', L'y', L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'd', L'i', L'c', L't', L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'k', L'e', L'y', L'\0',L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L's', L't', L'r', L'i', L'n', L'g',  L'\0', L'\0', L'\0', L'\0'},
        {L'd', L'a', L't', L'a', L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'd', L'a', L't', L'e', L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'r', L'e', L'a', L'l', L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'i', L'n', L't', L'e', L'g', L'e',  L'r',  L'\0', L'\0', L'\0'},
        {L't', L'r', L'u', L'e', L'\0',L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'f', L'a', L'l', L's', L'e', L'\0', L'\0', L'\0', L'\0', L'\0'},
        {L'D', L'O', L'C', L'T', L'Y', L'P',  L'E',  L'\0', L'\0', L'\0'},
        {L'<', L'!', L'[', L'C', L'D', L'A',  L'T',  L'A',  L'[',  L'\0'}
};







static size_t __calc_linenumber(const plistXMLParser_t *parser)
{
        const wchar_t *p;
        size_t count;
        AR_ASSERT(parser != NULL);
        
        if(parser->begin == NULL)
        {
                AR_error(AR_ERR_WARNING, L"parser not input xml content\r\n");
                return 0;
        }
        
        p = parser->begin;
        count = 0;
        
        while (p < parser->curr)
        {
                if(!Lex_IsLineTerminator(*p))
                {
                        ++p;
                }else
                {
                        count++;
                        
                        if(*p == L'\r' && *(p + 1) == '\n')
                        {
                                ++p;
                        }
                }
        }
        
        return count;
}

static void __skip_whitespace(plistXMLParser_t *parser)
{
        
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);
        
        parser->curr = AR_wcstrim_space(parser->curr);
        
}

// parser should be just past "<!--"
static void __skip_xml_comment(plistXMLParser_t *parser)
{
        const wchar_t *p, *end;

        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);
        p = parser->curr;
        end = parser->end - 3;   // "-->" Need at least 3 characters to compare against
        
        while(p < end)
        {
                if(*p == L'-' && *(p + 1) == L'-' && *(p + 2) == L'>')
                {
                        parser->curr = p + 3;
                        return;
                }else
                {
                        ++p;
                }
        }
        
        parser->has_error = true;
        AR_FormatString(parser->errmsg, L"Unterminated comment started on line %u", (ar_uint_32_t)__calc_linenumber(parser));
        
}

// parser should be set to the first character after "<?"
static void __skip_xml_processing_instruction(plistXMLParser_t *parser)
{
        const wchar_t *beg, *end;
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);
        
        beg = parser->curr;
        end = parser->end - 2;  // Looking for "?>" so we need at least 2 characters
        
        while(parser->curr < end)
        {
                if(* (parser->curr) == L'?' && *(parser->curr+1) == L'>')
                {
                        parser->curr += 2;
                        return;
                }else
                {
                        parser->curr++;
                }
        }
        
        parser->curr = beg;
        
        parser->has_error = true;
        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing the processing instruction begun on line %d", (ar_uint_32_t)__calc_linenumber(parser));
}





arStatus_t              PList_ParseXML(plistXMLParser_t *parser, plistElem_t **result)
{
        arStatus_t status;
        AR_ASSERT(parser != NULL && result != NULL);
        
        status = AR_S_YES;
        
        
END_POINT:
        return status;
}









/**********************************************Utility*****************************************************/


static arCodePage_t __detect_xml_codepage(const char *xml, size_t length)
{
        arCodePage_t cp;
        const char *idx, *end;
        char quote;
        AR_ASSERT(xml != NULL && length > 0);
        
        cp = AR_CP_MAX;
        
        
        // Scan for the <?xml.... ?> opening
        if(length < 5 || AR_strncmp(xml, "<?xml", 5) != 0)
        {
                return AR_CP_UTF8;
        }
        
        idx = xml + 5;
        end = xml + length;
        
        // Found "<?xml"; now we scan for "encoding"
        while (idx < end)
        {
                char ch = *idx;
                const char *scan;
                
                if(ch == '?' || ch == '>')
                {
                        return AR_CP_UTF8;
                }
                
                idx++;
                scan = idx;
                
                if (ch == 'e' && *scan++ == 'n' && *scan++ == 'c' && *scan++ == 'o' && *scan++ == 'd' && *scan++ == 'i' && *scan++ == 'n' && *scan++ == 'g' && *scan++ == '=')
                {
                        idx = scan;
                        break;
                }
        }
        
        if (idx >= end)
        {
                return AR_CP_UTF8;
        }
        
        quote = *idx;
        if (quote != '\'' && quote != '\"')
        {
                return AR_CP_UTF8;
                
        }else
        {
                const char *base = idx+1;
                size_t len;
                
                idx++;
                
                while(idx < end && *idx != quote)
                {
                        idx++;
                }
                
                if (idx >= end)
                {
                        return AR_CP_UTF8;
                }
                
                len = idx - base;
                
                
                
                if(len == 7)
                {
                        if(AR_strnicmp(base, "GB10830", 7) == 0)
                        {
                                return AR_CP_GB18030;
                        }else
                        {
                                return AR_CP_MAX;
                        }
                }
                

                if(len == 6)
                {
                        if(AR_strnicmp(base, "GB2312", 6) == 0)
                        {
                                return AR_CP_GB2312;
                        }else
                        {
                                return AR_CP_MAX;
                        }
                }
                
                if(len == 5)
                {
                        if(AR_strnicmp(base, "UTF-8", 5) == 0)
                        {
                                return AR_CP_UTF8;
                        }else
                        {
                                return AR_CP_MAX;
                        }
                        
                }
                
                if(len == 4)
                {
                        if(AR_strnicmp(base, "BIG5", 4) == 0)
                        {
                                return AR_CP_BIG5;
                        }else
                        {
                                return AR_CP_MAX;
                        }
                }
                
                if(len == 3)
                {
                        if(AR_strnicmp(base, "GBK", 3) == 0)
                        {
                                return AR_CP_GB18030;
                        }else
                        {
                                return AR_CP_MAX;
                        }
                }
                
                                               
                return AR_CP_MAX;
        }
        
}


arStatus_t            PList_LoadXMLFromBinary(arBuffer_t *buf, arString_t *out)
{
        arStatus_t      status;
        arTxtBom_t      bom;

        
        AR_ASSERT(buf != NULL && out != NULL);
        AR_ASSERT(AR_GetBufferAvailable(buf) > 0);
        
        status = AR_S_YES;
        
        bom = AR_TXT_BOM_NONE;
        
        status = AR_DetectTextBom(AR_GetBufferData(buf), AR_GetBufferAvailable(buf), &bom, NULL);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        
        if(bom != AR_TXT_BOM_NONE)      //unicode
        {
                AR_ClearString(out);
                status = AR_LoadBomTextFromBinary(buf, NULL, out);
                
                if(status != AR_S_YES)
                {
                        AR_error(AR_ERR_WARNING, L"bad text bom : %d\r\n", bom);
                        goto END_POINT;
                }
        }else
        {
                arCodePage_t    cp = AR_CP_MAX;
                wchar_t         *str = NULL;
                const char *xml = (const char*)AR_GetBufferData(buf);
                size_t length =   AR_GetBufferAvailable(buf);
                
                cp = __detect_xml_codepage(xml, length);
                
                if(cp == AR_CP_MAX)
                {
                        status = AR_E_BADENCCONV;
                        goto END_POINT;
                }
                
                str = AR_str_to_wcs(cp, xml, length);
                
                if(str == NULL)
                {
                        status = AR_E_BADENCCONV;
                        AR_error(AR_ERR_WARNING, L"invalid code page : %u\r\n", (ar_uint_32_t)cp);
                        goto END_POINT;
                }
                
                status = AR_SetString(out, str);
                
                if(str)
                {
                        AR_DEL(str);
                        str = NULL;
                }
        }
        
END_POINT:
        
        return status;
        
}



arStatus_t              PList_LoadXMLFromFile(const wchar_t *path, arString_t *out)
{
        arStatus_t		ret;
		arFile_t		*file = NULL;
		arBuffer_t		*buf;
		
		AR_ASSERT(path != NULL && out != NULL);
        
		ret = AR_S_YES;
		buf = NULL;
		file = NULL;
		
        
		ret = AR_open_file(&file, path, L"rb");
        
		if(ret != AR_S_YES)
		{
				AR_error(AR_ERR_WARNING, L"__AR_open_file failed for %ls in function '%hs'\r\n", path, AR_FUNC_NAME);
				goto FAILED_POINT;
		}
        
        
		buf = AR_CreateBuffer(1024);
        
		if(buf == NULL)
		{
				ret = AR_E_NOMEM;
				goto FAILED_POINT;
		}
        
		{
				size_t	rn;
				ar_byte_t	tmp[256];
				
				do{
						ret = AR_read_file(file, tmp, 256, &rn);
						/*rn = fread((void*)tmp, 1, sizeof(tmp), file);*/
						if(rn > 0)
						{
								ret = AR_InsertToBuffer(buf, tmp, rn);
								if(ret == AR_E_NOMEM)
								{
										goto FAILED_POINT;
								}
						}
                        //}while(!feof(file) && !ferror(file));
				}while(AR_eof_file(file) != AR_S_YES && AR_error_file(file) != AR_S_YES);
                
				if(AR_error_file(file) == AR_S_YES)
				{
						ret = AR_E_FILE;
						AR_error(AR_ERR_WARNING, L"fread failed for %ls in function '%hs'\r\n", path, AR_FUNC_NAME);
						goto FAILED_POINT;
				}else
				{
						tmp[0] = '\0';
						
						ret = AR_InsertToBuffer(buf, tmp, 1);
						if(ret != AR_S_YES)
						{
								goto FAILED_POINT;
						}
				}
		}
        
		ret = PList_LoadXMLFromBinary(buf, out);
        
FAILED_POINT:
		if(file)
		{
				AR_close_file(file);
				file = NULL;
		}
        
		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}
        
		return ret;
}




AR_NAMESPACE_END



