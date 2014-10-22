/*
 * The Arsenal Library
 * Copyright (c) 2009-2013 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#ifndef Arsenal_property_list_internal_h
#define Arsenal_property_list_internal_h

#include "property_list.h"


AR_NAMESPACE_BEGIN

/*************************************************Parser****************************************************/

/*xml plist parser*/
typedef struct __plist_xml_parser
{
        
        arString_t      *content;
        
        const wchar_t   *begin;
        const wchar_t   *curr;
        const wchar_t   *end;
        
        arString_t      *errmsg;
        ar_bool_t       has_error;
        
}plistXMLParser_t;


plistXMLParser_t*       PList_CreateXMLParser();
void                    PList_DestroyXMLParser(plistXMLParser_t *parser);
void                    PList_ClearXMLParser(plistXMLParser_t *parser);

arStatus_t              PList_SetXMLParserWithWcs(plistXMLParser_t *parser, const wchar_t *xml, size_t length);


plistElem_t*            PList_ParseXML(plistXMLParser_t *parser);

#define                 PList_XMLParserInError(_psr) ((_psr)->has_error)
const wchar_t*          PList_GetXMLParserErrorMessage(const plistXMLParser_t *parser);





/*binary plist parser*/


arStatus_t      PList_TryParseBinaryPlist(const ar_byte_t *data, size_t length, plistElem_t **pelem, arString_t *errmsg);







/**********************************Utility********************************************/
arStatus_t              PList_LoadXMLFromBinary(arBuffer_t *buf, arString_t *out);
arStatus_t              PList_LoadXMLFromFile(const wchar_t *path, arString_t *out);
arStatus_t              PList_LoadBinaryFromFile(const wchar_t *path, arBuffer_t *out);



AR_NAMESPACE_END



#endif
