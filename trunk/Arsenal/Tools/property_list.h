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

#ifndef __ARSENAL_PROPERTY_LIST_H__
#define __ARSENAL_PROPERTY_LIST_H__



#include "../Common/common.h"

AR_NAMESPACE_BEGIN


arStatus_t      PList_Init();
void            PList_UnInit();

typedef enum
{
        PLIST_XML_1_T,
        PLIST_BINARY_1_T,
}plistType_t;




typedef enum
{
        
        PLIST_ELEM_STRING_T,
        PLIST_ELEM_BOOLEAN_T,
        PLIST_ELEM_NUMBER_T,
        PLIST_ELEM_DATA_T,
        PLIST_ELEM_DATE_T,
        PLIST_ELEM_ARRAY_T,
        PLIST_ELEM_DICT_T,
        
        PLIST_ELEM_MAX_T,
}plistElemType_t;


struct  __plist_element_tag;
typedef struct __plist_element_tag      plistElem_t;



typedef struct __plist_string_tag
{
        arString_t      *str;
}plistString_t;


arStatus_t      PList_InitString(plistString_t  *str);
void            PList_UnInitString(plistString_t  *str);

ar_int_t        PList_CompareString(const plistString_t  *l, const plistString_t  *r);
ar_int_t        PList_CompareStringWithWcs(const plistString_t  *l, const wchar_t *wcs);


arStatus_t      PList_AppendString(plistString_t  *str, const wchar_t *wcs);
arStatus_t      PList_AppendStringN(plistString_t  *str, const wchar_t *wcs, size_t n);

ar_bool_t       PList_IsEmptyString(const plistString_t  *str);
const wchar_t*  PList_GetStringCString(const plistString_t *str);
size_t          PList_GetStringLength(const plistString_t *str);

typedef struct __plist_boolean_tag
{
        ar_bool_t       val;
}plistBoolean_t;


typedef enum
{
        PLIST_NUMBER_INTEGER_T,
        PLIST_NUMBER_REAL_T,
}plistNumberType_t;


typedef enum
{
		PLIST_REAL_NORMAL_T,
		PLIST_REAL_NAN_T,
		PLIST_REAL_INF_T,
		PLIST_REAL_POSITIVE_INF_T,
		PLIST_REAL_NEGATIVE_INF_T,
}plistRealType_t;

typedef struct __plist_number_tag
{
        plistNumberType_t       type;

        union
        {
                struct {
                        ar_bool_t       is_signed;
                        union{
                                ar_uint_64_t    unsigned_num;
                                ar_int_64_t     signed_num;
                        };
                }               integer;
                
				struct {
						plistRealType_t			t;
						double					num;
				}		real;
        };
        
}plistNumber_t;



typedef struct __plist_data_tag
{
        arBuffer_t      *buf;
}plistData_t;

arStatus_t              PList_InitData(plistData_t *data);
void                    PList_UnInitData(plistData_t *data);
const ar_byte_t*        PList_GetDataPointer(const plistData_t *data);
size_t                  PList_GetDataLength(const plistData_t *data);


typedef struct __plist_date_tag
{
        ar_uint_16_t    year;
        ar_uint_8_t     month;
        ar_uint_8_t     day;
        ar_uint_8_t     hour;
        ar_uint_8_t     minute;
        ar_uint_8_t     second;
}plistDate_t;



typedef struct __plist_array_tag
{
        plistElem_t     **items;
        size_t          count;
        size_t          cap;
}plistArray_t;


void            PList_InitArray(plistArray_t *arr);
void            PList_UnInitArray(plistArray_t *arr);
void            PList_ClearArray(plistArray_t *arr);
arStatus_t      PList_PushToArray(plistArray_t *arr, plistElem_t *elem);
#define         PList_GetArrayCount(_arr)       ((_arr)->count)
plistElem_t*    PList_GetArrayByIndex(plistArray_t *arr, size_t idx);
ar_bool_t       PList_RemoveArrayByIndex(plistArray_t *arr, size_t idx);

typedef struct __plist_dict_tag
{
        plistElem_t     **keys;
        plistElem_t     **values;
        size_t          count;
        size_t          cap;
}plistDict_t;

void            PList_InitDict(plistDict_t *dict);
void            PList_UnInitDict(plistDict_t *dict);
void            PList_ClearDict(plistDict_t *dict);

plistElem_t*    PList_FindValueByElem(plistDict_t *dict, const plistElem_t *key);
plistElem_t*    PList_FindValueByString(plistDict_t *dict, const wchar_t *key);
arStatus_t      PList_SetDictValueForKey(plistDict_t *dict, plistElem_t *key, plistElem_t *val);
arStatus_t      PList_RemoveValueForKey(plistDict_t *dict, plistElem_t *key);

#define         PList_GetDictCount(_d)  ((_d)->count)
const wchar_t*  PList_GetDictKeyWcsByIndex(plistDict_t *dict, size_t idx);
plistElem_t*    PList_GetDictValueByIndex(plistDict_t *dict, size_t idx);


struct  __plist_element_tag
{
        plistElemType_t type;

        union
        {
                plistString_t           str;
                plistBoolean_t          boolean;
                plistNumber_t           number;
                plistData_t             data;
                plistDate_t             date;
                plistArray_t            array;
                plistDict_t             dict;
        };
};



plistElem_t*    PList_CreateElem(plistElemType_t        t);
void            PList_DestroyElem(plistElem_t            *elem);
#define         PList_GetElemType(_elem)        ((_elem)->type)

const wchar_t*          PList_GetElemCString(const plistElem_t *elem);

const ar_byte_t*        PList_GetElemDataPointer(const plistElem_t *elem);
size_t                  PList_GetElemDataLength(const plistElem_t *elem);

const plistNumber_t*    PList_GetElemNumber(const plistElem_t *elem);

size_t                  PList_GetElemArrayCount(const plistElem_t *elem);
plistElem_t*            PList_GetElemArrayByIndex(plistElem_t *elem, size_t idx);

size_t                  PList_GetElemDictCount(const plistElem_t *elem);
plistElem_t*            PList_FindElemDictValueByWcs(plistElem_t *elem, const wchar_t *key);
const wchar_t*          PList_GetElemDictKeyWcsByIndex(plistElem_t *elem, size_t idx);
plistElem_t*            PList_GetElemDictValueByIndex(plistElem_t *elem, size_t idx);


arStatus_t              PList_SaveElemToXML(const plistElem_t *elem, arString_t *out);


/***************************************************************************************/

arStatus_t      PList_LoadElemFromBinary(arBuffer_t *data, plistType_t *ptype, plistElem_t **pelem);
arStatus_t      PList_SaveElemToBinary(const plistElem_t *elem, arBuffer_t *data, plistType_t type);






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



/**********************************Utility********************************************/
arStatus_t              PList_LoadXMLFromBinary(arBuffer_t *buf, arString_t *out);
arStatus_t              PList_LoadXMLFromFile(const wchar_t *path, arString_t *out);




AR_NAMESPACE_END



#endif
