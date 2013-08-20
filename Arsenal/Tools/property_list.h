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
arStatus_t      PList_ReserveString(plistString_t  *str, size_t cap);

arStatus_t      PList_CopyString(plistString_t  *dest, plistString_t  *src);


ar_int_t        PList_CompareString(const plistString_t  *l, const plistString_t  *r);
ar_int_t        PList_CompareStringWithWcs(const plistString_t  *l, const wchar_t *wcs);


arStatus_t      PList_AppendString(plistString_t  *str, const wchar_t *wcs);
arStatus_t      PList_AppendStringN(plistString_t  *str, const wchar_t *wcs, size_t n);
arStatus_t      PList_AppendCharToString(plistString_t *str, wchar_t ch);


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

ar_int_t PList_CompNumber(const plistNumber_t *l, const plistNumber_t *r);


typedef struct __plist_data_tag
{
        arBuffer_t      *buf;
}plistData_t;

arStatus_t              PList_InitData(plistData_t *data);
void                    PList_UnInitData(plistData_t *data);
ar_int_t                PList_CompData(const plistData_t *l, const plistData_t *r);
arStatus_t              PList_CopyData(plistData_t *dest, plistData_t *src);
const ar_byte_t*        PList_GetDataPointer(const plistData_t *data);
size_t                  PList_GetDataLength(const plistData_t *data);
arStatus_t              PList_SetData(plistData_t *data, const ar_byte_t *buf, size_t length);

typedef struct __plist_date_tag
{
        ar_uint_16_t    year;
        ar_uint_8_t     month;
        ar_uint_8_t     day;
        ar_uint_8_t     hour;
        ar_uint_8_t     minute;
        ar_uint_8_t     second;
}plistDate_t;


ar_int_t        PList_CompDate(const plistDate_t *l, const plistDate_t *r);


typedef struct __plist_array_tag
{
        plistElem_t     **items;
        size_t          count;
        size_t          cap;
}plistArray_t;


void            PList_InitArray(plistArray_t *arr);
void            PList_UnInitArray(plistArray_t *arr);
void            PList_ClearArray(plistArray_t *arr);
arStatus_t      PList_CopyArray(plistArray_t *dest, plistArray_t *src);
arStatus_t      PList_PushToArray(plistArray_t *arr, plistElem_t *elem);
#define         PList_GetArrayCount(_arr)       ((_arr)->count)
plistElem_t*    PList_GetArrayByIndex(plistArray_t *arr, size_t idx);
ar_bool_t       PList_RemoveArrayByIndex(plistArray_t *arr, size_t idx);
ar_int_t        PList_CompArray(const plistArray_t *l, const plistArray_t *r);

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
arStatus_t      PList_CopyDict(plistDict_t *dest, plistDict_t *src);

plistElem_t*    PList_FindValueByElem(plistDict_t *dict, const plistElem_t *key);
plistElem_t*    PList_FindValueByString(plistDict_t *dict, const wchar_t *key);
arStatus_t      PList_SetDictValueForKey(plistDict_t *dict, plistElem_t *key, plistElem_t *val);
arStatus_t      PList_RemoveValueForKey(plistDict_t *dict, plistElem_t *key);

#define         PList_GetDictCount(_d)  ((_d)->count)

const wchar_t*  PList_GetDictKeyWcsByIndex(plistDict_t *dict, size_t idx);
plistElem_t*    PList_GetDictKeyByIndex(plistDict_t *dict, size_t idx);
plistElem_t*    PList_GetDictValueByIndex(plistDict_t *dict, size_t idx);


ar_int_t        PList_CompDict(const plistDict_t *l, const plistDict_t *r);


struct  __plist_element_tag
{
        plistElemType_t         type;

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
plistElem_t*    PList_CopyNewElem(plistElem_t            *elem);
#define         PList_GetElemType(_elem)        ((_elem)->type)

ar_int_t        PList_CompElem(const plistElem_t *l, const plistElem_t *r);
ar_uint_64_t    PList_HashElem(const plistElem_t *elem);

ar_bool_t               PList_GetElemBooleanValue(const plistElem_t *elem);
void                    PList_SetElemBooleanValue(plistElem_t *elem, ar_bool_t val);

void                    PList_SetElemSignedInteger(plistElem_t *elem, ar_int_64_t num);
void                    PList_SetElemUnsignedInteger(plistElem_t *elem, ar_uint_64_t num);
void                    PList_SetElemReal(plistElem_t *elem, double num);
void                    PList_SetElemRealByType(plistElem_t *elem, plistRealType_t t, double num);
const plistNumber_t*    PList_GetElemNumber(const plistElem_t *elem);

void                    PList_SetElemGMTDate(plistElem_t *elem, ar_uint_16_t year, ar_uint_16_t mon, ar_uint_16_t day, ar_uint_16_t hour, ar_uint_16_t min, ar_uint_16_t sec);



const wchar_t*          PList_GetElemCString(const plistElem_t *elem);
arStatus_t              PList_AppendCharToElemString(plistElem_t *elem, wchar_t ch);
arStatus_t              PList_ReserveElemString(plistElem_t *elem, size_t cap);

const ar_byte_t*        PList_GetElemDataPointer(const plistElem_t *elem);
size_t                  PList_GetElemDataLength(const plistElem_t *elem);

arStatus_t              PList_SetElemData(plistElem_t *elem, const ar_byte_t *data, size_t length);


size_t                  PList_GetElemArrayCount(const plistElem_t *elem);
plistElem_t*            PList_GetElemArrayByIndex(plistElem_t *elem, size_t idx);
arStatus_t              PList_AppendToElemArray(plistElem_t *elem, plistElem_t *item);

size_t                  PList_GetElemDictCount(const plistElem_t *elem);
plistElem_t*            PList_FindElemDictValueByWcs(plistElem_t *elem, const wchar_t *key);
arStatus_t              PList_SetElemDictValueForKey(plistElem_t *elem, plistElem_t *key, plistElem_t *val);


const wchar_t*          PList_GetElemDictKeyWcsByIndex(plistElem_t *elem, size_t idx);

plistElem_t*            PList_GetElemDictKeyByIndex(plistElem_t *elem, size_t idx);
plistElem_t*            PList_GetElemDictValueByIndex(plistElem_t *elem, size_t idx);



arStatus_t              PList_LoadElemFromBinary(arBuffer_t *data, plistType_t *ptype, plistElem_t **pelem);
arStatus_t              PList_SaveElemToXML(const plistElem_t *elem, arString_t *out);
arStatus_t              PList_SaveElemToBinary(const plistElem_t *elem, arBuffer_t *out);



/***************************************************************************************/









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
