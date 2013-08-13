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

plistElem_t     *__g_boolean_true = NULL;
plistElem_t     *__g_boolean_false = NULL;

arStatus_t      PList_Init()
{

        arStatus_t status;
        
        status = AR_S_YES;
        
        __g_boolean_true = PList_CreateElem(PLIST_ELEM_BOOLEAN_T);
        __g_boolean_false = PList_CreateElem(PLIST_ELEM_BOOLEAN_T);
        
        if(__g_boolean_true == NULL || __g_boolean_false == NULL)
        {
                status = AR_E_NOMEM;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto END_POINT;
        }
        
        __g_boolean_true->boolean.val = true;
        __g_boolean_false->boolean.val = false;
        
END_POINT:
        if(status != AR_S_YES)
        {
                if(__g_boolean_true)
                {
                        PList_DestroyElem(__g_boolean_true);
                        __g_boolean_true = NULL;
                }
                
                if(__g_boolean_false)
                {
                        PList_DestroyElem(__g_boolean_false);
                        __g_boolean_false = NULL;
                }
        }
        
        return status;
}


void            PList_UnInit()
{

        PList_DestroyElem(__g_boolean_true);
        PList_DestroyElem(__g_boolean_false);
         
}



/********************************************************PList elements********************************************************/


/*****plistString_t***********/


arStatus_t      PList_InitString(plistString_t  *str)
{
        AR_ASSERT(str != NULL);
        
        AR_memset(str, 0, sizeof(*str));
        
        str->str = AR_CreateString();
        
        if(str->str == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                return AR_E_NOMEM;
        }else
        {
                return AR_S_YES;
        }
}

void            PList_UnInitString(plistString_t  *str)
{
        AR_ASSERT(str != NULL && str->str != NULL);
        
        AR_DestroyString(str->str);
        str->str = NULL;
        AR_memset(str, 0, sizeof(*str));
}


ar_int_t        PList_CompareString(const plistString_t  *l, const plistString_t  *r)
{
        AR_ASSERT(l != NULL && r != NULL);
        AR_ASSERT(l->str && r->str);
        return AR_CompString(l->str, r->str);
}

ar_int_t        PList_CompareStringWithWcs(const plistString_t  *l, const wchar_t *wcs)
{
        AR_ASSERT(l != NULL && wcs != NULL);
        return AR_CompStringWithWcs(l->str, wcs);
}


arStatus_t      PList_AppendString(plistString_t  *str, const wchar_t *wcs)
{
        AR_ASSERT(str != NULL && str->str != NULL && wcs != NULL);
        return AR_AppendString(str->str, wcs);
}

arStatus_t      PList_AppendStringN(plistString_t  *str, const wchar_t *wcs, size_t n)
{
        AR_ASSERT(str != NULL && str->str != NULL && wcs != NULL);
        return AR_AppendStringN(str->str, wcs, n);
}

ar_bool_t       PList_IsEmptyString(const plistString_t  *str)
{
        AR_ASSERT(str != NULL);
        return AR_IsEmptyString(str->str);
}

const wchar_t*  PList_GetStringCString(const plistString_t *str)
{
        AR_ASSERT(str != NULL && str->str != NULL);
        return AR_CSTR(str->str);
}

/*****plistData_t***********/

arStatus_t      PList_InitData(plistData_t *data)
{
        AR_ASSERT(data != NULL);
        
        AR_memset(data, 0, sizeof(*data));
        
        data->buf = AR_CreateBuffer(0);
        
        if(data->buf == NULL)
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                return AR_E_NOMEM;
        }else
        {
                return AR_S_YES;
        }
}

void            PList_UnInitData(plistData_t *data)
{
        AR_ASSERT(data != NULL && data->buf != NULL);
        
        AR_DestroyBuffer(data->buf);
        data->buf = NULL;
        AR_memset(data, 0, sizeof(*data));
}

/*****plistArray_t***********/

void      PList_InitArray(plistArray_t *arr)
{
        AR_ASSERT(arr != NULL);
        AR_memset(arr, 0, sizeof(*arr));
}

void            PList_UnInitArray(plistArray_t *arr)
{
        AR_ASSERT(arr != NULL);
        PList_ClearArray(arr);
        
        if(arr->items)
        {
                AR_DEL(arr->items);
                arr->items = NULL;
        }
        
        AR_memset(arr, 0, sizeof(*arr));
}

void            PList_ClearArray(plistArray_t *arr)
{
        size_t i;
        AR_ASSERT(arr != NULL);
        
        for(i = 0; i < arr->count; ++i)
        {
                PList_DestroyElem(arr->items[i]);
                arr->items[i] = NULL;
        }
        arr->count = 0;
}

arStatus_t      PList_PushToArray(plistArray_t *arr, plistElem_t *elem)
{
        AR_ASSERT(arr != NULL && elem != NULL);
        
        if(arr->count == arr->cap)
		{
				size_t          new_cap;
				plistElem_t     **new_items;
                
				new_cap = (arr->cap + 4) * 2;
				new_items = AR_NEWARR(plistElem_t*, new_cap);
                
				if(new_items == NULL)
				{
						return AR_E_NOMEM;
				}
                
				if(arr->count > 0)
				{
						AR_memcpy(new_items, arr->items, arr->count * sizeof(plistElem_t*));
				}
                
				if(arr->items)
				{
						AR_DEL(arr->items);
						arr->items = NULL;
				}
				arr->cap = new_cap;
				arr->items = new_items;
		}
        
        arr->items[arr->count++] = elem;
		return AR_S_YES;
}

plistElem_t*    PList_GetElemByIndex(plistArray_t *arr, size_t idx)
{
        AR_ASSERT(arr != NULL && idx < arr->count);
        return arr->items[idx];
}

arStatus_t      PList_RemoveElemByIndex(plistArray_t *arr, size_t idx)
{
        AR_ASSERT(arr != NULL);
		if(idx >= arr->count)
		{
				return AR_E_RANGE;
		}
        
		PList_DestroyElem(arr->items[idx]);
		arr->items[idx] = arr->items[arr->count-1];
		arr->count--;
		return AR_S_YES;
}

/*****plistDict_t***********/

void      PList_InitDict(plistDict_t *dict)
{
        AR_ASSERT(dict != NULL);
        AR_memset(dict, 0, sizeof(*dict));
        
        
        
}

void            PList_UnInitDict(plistDict_t *dict)
{
        AR_ASSERT(dict != NULL);
        PList_ClearDict(dict);
        
        if(dict->keys)
        {
                AR_DEL(dict->keys);
                dict->keys = NULL;
        }
        
        if(dict->values)
        {
                AR_DEL(dict->values);
                dict->values = NULL;
        }
        AR_memset(dict, 0, sizeof(*dict));
}


void            PList_ClearDict(plistDict_t *dict)
{
        size_t i;
        AR_ASSERT(dict != NULL);
        
        for(i = 0; i < dict->count; ++i)
        {
                PList_DestroyElem(dict->keys[i]);
                dict->keys[i] = NULL;
                PList_DestroyElem(dict->values[i]);
                dict->values[i] = NULL;
        }
        dict->count = 0;
}


plistElem_t*    PList_FindValueByElem(plistDict_t *dict, const plistElem_t *key)
{
        size_t i;
        AR_ASSERT(dict != NULL && key != NULL);
        AR_ASSERT(PList_GetElemType(key) == PLIST_ELEM_STRING_T);
        
        for(i = 0; i < dict->count; ++i)
        {
                ar_int_t cmp;
                AR_ASSERT(PList_GetElemType(dict->keys[i]) == PLIST_ELEM_STRING_T);
                cmp = PList_CompareString(&dict->keys[i]->str, &key->str);
                
                if(cmp == 0)
                {
                        return dict->values[i];
                }
        }
        return NULL;
}

plistElem_t*    PList_FindValueByString(plistDict_t *dict, const wchar_t *key)
{
        size_t i;
        AR_ASSERT(dict != NULL && key != NULL);
        
        for(i = 0; i < dict->count; ++i)
        {
                ar_int_t cmp;
                AR_ASSERT(PList_GetElemType(dict->keys[i]) == PLIST_ELEM_STRING_T);
                cmp = PList_CompareStringWithWcs(&dict->keys[i]->str, key);
                
                if(cmp == 0)
                {
                        return dict->values[i];
                }
        }
        return NULL;
}

arStatus_t      PList_SetDictValueForKey(plistDict_t *dict, plistElem_t *key, plistElem_t *val)
{
        size_t i;
		AR_ASSERT(dict != NULL && key != NULL && PList_GetElemType(key) == PLIST_ELEM_STRING_T && val != NULL);
        
		for(i = 0; i < dict->count; ++i)
		{
				AR_ASSERT(dict->keys[i] != NULL && dict->values[i] != NULL);
                AR_ASSERT(PList_GetElemType(dict->keys[i]) == PLIST_ELEM_STRING_T);
                
				if(PList_CompareString(&dict->keys[i]->str, &key->str) == 0)
				{
						break;
				}
		}
        
		if(i < dict->count)
		{
                PList_DestroyElem(dict->keys[i]);
                dict->keys[i] = key;
                
                PList_DestroyElem(dict->values[i]);
                dict->values[i] = val;
               
                
				return AR_S_YES;
		}else
		{
				if(dict->count == dict->cap)
				{
						size_t			new_cap;
						plistElem_t		**new_keys, **new_values;
                        
						new_cap = (dict->cap + 4) * 2;
						new_keys = AR_NEWARR(plistElem_t*, new_cap);
						new_values = AR_NEWARR(plistElem_t*, new_cap);
                        
						if(new_keys == NULL || new_values == NULL)
						{
                                if(new_keys)
                                {
                                        AR_DEL(new_keys);
                                        new_keys = NULL;
                                }
                                if(new_values)
                                {
                                        AR_DEL(new_values);
                                        new_values = NULL;
                                }
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
								return AR_E_NOMEM;
						}
                        
						if(dict->count > 0)
						{
								AR_memcpy(new_keys, dict->keys, dict->count * sizeof(plistElem_t*));
								AR_memcpy(new_values, dict->values, dict->count * sizeof(plistElem_t*));
						}
                        
						if(dict->keys)
						{
								AR_DEL(dict->keys);
								dict->keys = NULL;
						}
                        
                        if(dict->values)
                        {
                                AR_DEL(dict->values);
                                dict->values = NULL;
                        }
                        
						dict->cap = new_cap;
                        dict->keys = new_keys;
                        dict->values = new_values;
				}
                
				dict->keys[dict->count] = key;
				dict->values[dict->count] = val;
				dict->count++;
                
#if defined(AR_DEBUG)
				for(i = 0; i < dict->count; ++i)
				{
						AR_ASSERT(dict->keys[i] != NULL && dict->values[i] != NULL);
				}
#endif
				
				return AR_S_YES;
		}
}

arStatus_t      PList_RemoveValueForKey(plistDict_t *dict, plistElem_t *key)
{
        size_t i;
		AR_ASSERT(dict != NULL && key != NULL && PList_GetElemType(key) == PLIST_ELEM_STRING_T);
        
		for(i = 0; i < dict->count; ++i)
		{
				AR_ASSERT(dict->keys[i] != NULL && dict->values[i] != NULL);
                AR_ASSERT(PList_GetElemType(dict->keys[i]) == PLIST_ELEM_STRING_T);
                
				if(PList_CompareString(&dict->keys[i]->str, &key->str) == 0)
				{
						break;
				}
		}

        
		if(i >= dict->count)
		{
                return AR_E_NOTFOUND;
		}
        
        PList_DestroyElem(dict->keys[i]);
        PList_DestroyElem(dict->values[i]);
        
        dict->keys[i] = dict->keys[dict->count-1];
        dict->values[i] = dict->values[dict->count-1];
        
		dict->count--;
		return AR_S_YES;
}




/*plistDict_t*/


#if(0)



typedef struct __plist_dict_tag
{
        plistElem_t     **keys;
        plistElem_t     **values;
        size_t          count;
        size_t          cap;
}plistDict_t;

arStatus_t      PList_InitDict(plistDict_t *dict);
void            PList_UnInitDict(plistDict_t *dict);

plistElem_t*    PList_FindValueByElem(plistDict_t *dict, const plistElem_t *key);
plistElem_t*    PList_FindValueByString(plistDict_t *dict, const wchar_t *key);
arStatus_t      PList_SetDictValueForKey(plistDict_t *dict, plistElem_t *key, plistElem_t *val);
arStatus_t      PList_RemoveValueForKey(plistDict_t *dict, plistElem_t *key);

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




#endif



/*plistElem_t*/


plistElem_t*    PList_CreateElem(plistElemType_t        t)
{
        return NULL;
        
}

void            PList_DestroyElem(plistElem_t            *elem)
{
        AR_ASSERT(elem != NULL);
}


/********************************************************XML Parser********************************************************/


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



#define PLIST_IX        0
#define ARRAY_IX        1
#define DICT_IX         2
#define KEY_IX          3
#define STRING_IX       4
#define DATA_IX     	5
#define DATE_IX     	6
#define REAL_IX     	7
#define INTEGER_IX      8
#define TRUE_IX         9
#define FALSE_IX        10
#define DOCTYPE_IX      11
#define CDSECT_IX       12

#define PLIST_TAG_LENGTH        5
#define ARRAY_TAG_LENGTH        5
#define DICT_TAG_LENGTH         4
#define KEY_TAG_LENGTH			3
#define STRING_TAG_LENGTH       6
#define DATA_TAG_LENGTH         4
#define DATE_TAG_LENGTH         4
#define REAL_TAG_LENGTH         4
#define INTEGER_TAG_LENGTH      7
#define TRUE_TAG_LENGTH         4
#define FALSE_TAG_LENGTH        5
#define DOCTYPE_TAG_LENGTH      7
#define CDSECT_TAG_LENGTH       9


ar_bool_t __match_string(const wchar_t *l, const wchar_t *r, size_t n)
{
        AR_ASSERT(l != NULL && r != NULL);
        if(AR_wcsnicmp(l, r, n) == 0)
        {
                return true;
        }else
        {
                return false;
        }
}


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
        AR_FormatString(parser->errmsg, L"Unterminated comment started on line %qu", __calc_linenumber(parser));
        
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
        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing the processing instruction begun on line %qu", __calc_linenumber(parser));
}


static void __skip_xml_inline_dtd(plistXMLParser_t *parser);

        

// first character should be immediately after the "<!"
static void __skip_xml_dtd(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);

        if(parser->end - parser->curr < DOCTYPE_TAG_LENGTH || !__match_string(parser->curr, __g_plist_tags[DOCTYPE_IX], DOCTYPE_TAG_LENGTH))
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Malformed DTD on line %qu", __calc_linenumber(parser));
                return;
        }
        
        parser->curr += DOCTYPE_TAG_LENGTH;
        __skip_whitespace(parser);
        
 
        
        // Look for either the beginning of a complex DTD or the end of the DOCTYPE structure
        while (parser->curr < parser->end)
        {
                wchar_t ch = *(parser->curr);
                
                if (ch == L'[') // inline DTD
                {
                        break;
                }
                
                if (ch == L'>') // End of the DTD
                {
                        parser->curr++;
                        return;
                }
                parser->curr++;
        }
        
        if (parser->curr == parser->end)
        {
                
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing DTD");
                return;
        }
        
        __skip_xml_inline_dtd(parser);
        
        if(parser->has_error)
        {
                return;
        }
        
        __skip_whitespace(parser);
        
        
        if (parser->curr < parser->end)
        {
                if(*(parser->curr) == L'>')
                {
                        parser->curr++;
                }else
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %qu while parsing DTD", *(parser->curr), __calc_linenumber(parser));
                }
        }else
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing DTD");
        }
}

static void __skip_xml_peref(plistXMLParser_t *parser);



// First character should be just past '['
static void __skip_xml_inline_dtd(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);

        
        while (!parser->has_error && parser->curr < parser->end)
        {
                wchar_t ch;
                __skip_whitespace(parser);
                ch = *parser->curr;
                
                if (ch == L'%')
                {
                        parser->curr++;
                        __skip_xml_peref(parser);
                        
                }else if(ch == L'<')
                {
                        parser->curr++;
                        
                        if (parser->curr >= parser->end)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing inline DTD");
                                return;
                        }
                        ch = *parser->curr;
                        if (ch == L'?')
                        {
                                parser->curr++;
                                __skip_xml_processing_instruction(parser);
                        }else if(ch == L'!')
                        {
                                if (parser->curr + 2 < parser->end && (*(parser->curr+1) == '-' && *(parser->curr+2) == '-'))
                                {
                                        parser->curr += 3;
                                        __skip_xml_comment(parser);
                                }else
                                {
                                        // Skip the myriad of DTD declarations of the form "<!string" ... ">"
                                        parser->curr ++; // Past both '<' and '!'
                                        while (parser->curr < parser->end)
                                        {
                                                if (*(parser->curr) == L'>')
                                                {
                                                        break;
                                                }
                                                parser->curr++;
                                        }
                                        
                                        if (*(parser->curr) != '>')
                                        {
                                                parser->has_error = true;
                                                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing inline DTD");
                                                return;
                                        }
                                        parser->curr++;
                                }
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %d while parsing inline DTD", ch, __calc_linenumber(parser));
                                return;
                        }
                } else if(ch == L']')
                {
                        parser->curr ++;
                        return;
                }else
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %d while parsing inline DTD", ch, __calc_linenumber(parser));
                        return;
                }
        }
        
        if(!parser->has_error)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing inline DTD");
        }
}

static void __skip_xml_peref(plistXMLParser_t *parser)
{
        const wchar_t *p;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        p = parser->curr;
        
        while(p < parser->end)
        {
                if(*p == L';')
                {
                        parser->curr = p + 1;
                        return;
                }
                ++p;
        }
        
        parser->has_error = true;
        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF while parsing percent-escape sequence begun on line %qu", __calc_linenumber(parser));
}



static plistElem_t*     __parse_xml_element(plistXMLParser_t *parser);

static plistElem_t*     __get_xml_content_object(plistXMLParser_t *parser)
{
        
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        while (!parser->has_error && parser->curr < parser->end)
        {
                __skip_whitespace(parser);
                
                if(parser->curr >= parser->end)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                        return NULL;
                }
                
                if(*parser->curr != L'<')
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %qu", *parser->curr, __calc_linenumber(parser));
                        return NULL;
                }
                parser->curr++;

                if (parser->curr >= parser->end)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                        return NULL;
                }
                
                switch (*(parser->curr))
                {
                case '?':
                                // Processing instruction
                                __skip_xml_processing_instruction(parser);
                                break;
                case '!':
                                // Could be a comment
                                if(parser->curr+2 >= parser->end)
                                {
                                        parser->has_error = true;
                                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                                        return NULL;
                                }
                                
                                if(*(parser->curr+1) == '-' && *(parser->curr+2) == '-')
                                {
                                        parser->curr += 2;
                                        __skip_xml_comment(parser);
                                }else
                                {
                                        parser->has_error = true;
                                        AR_FormatString(parser->errmsg, L"Encountered unexpected item %s begun on line %qu", parser->curr, __calc_linenumber(parser));
                                        return NULL;
                                }
                                break;
                        case '/':
                                // Whoops!  Looks like we got to the end tag for the element whose content we're parsing
                                parser->curr--;// Back off to the '<'
                                return NULL;
                        default:
                                // Should be an element
                                return __parse_xml_element(parser);
                }
        }
        // Do not set the error string here; if it wasn't already set by one of the recursive parsing calls, the caller will quickly detect the failure (b/c pInfo->curr >= pInfo->end) and provide a more useful one of the form "end tag for <blah> not found"
        return NULL;
}

static plistElem_t* __parse_xml_plisttag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_arraytag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_dicttag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_datatag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_datetag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_realtag(plistXMLParser_t *parser);
static plistElem_t* __parse_xml_integertag(plistXMLParser_t *parser);

static plistElem_t* __get_string(plistXMLParser_t *parser);
static ar_bool_t    __check_for_closetag(plistXMLParser_t *parser, const wchar_t *tag, size_t tag_len);
static plistElem_t*     __parse_xml_element(plistXMLParser_t *parser);


static ar_bool_t __cat_from_mark_to_buf(const wchar_t *beg, const wchar_t *end, plistElem_t *str)
{
        AR_ASSERT(beg != NULL && beg >= end);
        AR_ASSERT(str != NULL && PList_GetElemType(str) == PLIST_ELEM_STRING_T);
        
        if(end - beg == 0)
        {
                return true;
        }
        
        if(PList_AppendStringN(&str->str, beg, end - beg) != AR_S_YES)
        {
                return false;
        }else
        {
                return true;
        }
}

static void __parse_xml_cdsect_pl(plistXMLParser_t *parser, plistElem_t *str);
static void __parse_xml_entity_reference_pl(plistXMLParser_t *parser, plistElem_t *str);


// String could be comprised of characters, CDSects, or references to one of the "well-known" entities ('<', '>', '&', ''', '"')
// returns a retained object in *string.
static plistElem_t* __get_string(plistXMLParser_t *parser)
{
        plistElem_t *str;
        const wchar_t *mark;
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);
        
        str = NULL;
        mark = parser->curr; // At any time in the while loop below, the characters between mark and p have not yet been added to *string
        
        str = PList_CreateElem(PLIST_ELEM_STRING_T);
        
        if(str == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        while(!parser->has_error && parser->curr < parser->end)
        {
                wchar_t ch = *parser->curr;
                
                if(ch == L'<')
                {
                        
                        if(parser->curr + 1 >= parser->end)
                        {
                                break;
                        }
                        
                        // Could be a CDSect; could be the end of the string
                        
                        if(*(parser->curr + 1) != L'!') // End of the string
                        {
                                break;
                        }
                        
                        if(!__cat_from_mark_to_buf(mark, parser->curr, str))
                        {
                                parser->has_error = true;
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                goto INVALID_POINT;
                        }
                        
                        __parse_xml_cdsect_pl(parser, str);
                        mark = parser->curr;
                        
                }else if(ch == L'&')
                {
                        if(!__cat_from_mark_to_buf(mark, parser->curr, str))
                        {
                                parser->has_error = true;
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                goto INVALID_POINT;
                        }
                        
                        __parse_xml_entity_reference_pl(parser, str);
                        mark = parser->curr;
                }else
                {
                        ++parser->curr;
                }
        }
        
        
        if(parser->has_error)
        {
                goto INVALID_POINT;
        }
        
        if(!__cat_from_mark_to_buf(mark, parser->curr, str))
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        
        return str;
        
        
INVALID_POINT:
        if(str)
        {
                PList_DestroyElem(str);
                str = NULL;
        }
        
        return NULL;
}



static void __parse_xml_cdsect_pl(plistXMLParser_t *parser, plistElem_t *str)
{
        const wchar_t *begin, *end;
        
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        AR_ASSERT(str != NULL && PList_GetElemType(str) == PLIST_ELEM_STRING_T);
        
        if(parser->end - parser->curr < CDSECT_TAG_LENGTH)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                return;
        }
        
        if(!__match_string(parser->curr, __g_plist_tags[CDSECT_IX], CDSECT_TAG_LENGTH))
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered improper CDATA opening at line %qu", __calc_linenumber(parser));
                return;
        }
        
        parser->curr += CDSECT_TAG_LENGTH;
        
        begin = parser->curr;
        end = parser->end - 2;
        
        while(parser->curr < end)
        {
                if(*parser->curr == L']' && *(parser->curr + 1) == L']' && *(parser->curr + 2) == L'>')
                {
                        if(!__cat_from_mark_to_buf(begin, parser->curr, str))
                        {
                                parser->has_error = true;
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                return;
                        }
                        
                        parser->curr += 3;
                        return;
                }
                
                parser->curr++;
        }
        
        parser->curr = begin;
        parser->has_error = true;
        AR_FormatString(parser->errmsg, L"Could not find end of CDATA started on line %qu", __calc_linenumber(parser));
        return;
}



static void __parse_xml_entity_reference_pl(plistXMLParser_t *parser, plistElem_t *str)
{
        ar_int_t len;
        wchar_t ch;
        
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        AR_ASSERT(str != NULL && PList_GetElemType(str) == PLIST_ELEM_STRING_T);
        
        len = parser->end - parser->curr;
        
        if(len < 1)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                return;
        }
        
        
        switch (*(parser->curr))
        {
                case L'l':  // "lt"
                {
                        if (len >= 3 && *(parser->curr+1) == 't' && *(parser->curr+2) == ';')
                        {
                                ch = L'<';
                                parser->curr += 3;
                                break;
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unknown ampersand-escape sequence at line %qu", __calc_linenumber(parser));
                                return;
                        }
                }
                        break;
                case L'g': // "gt"
                {
                        if (len >= 3 && *(parser->curr+1) == L't' && *(parser->curr+2) == L';')
                        {
                                ch = L'>';
                                parser->curr += 3;
                                break;
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unknown ampersand-escape sequence at line %qu", __calc_linenumber(parser));
                                return;
                        }
                }
                        break;
                case L'a': // "apos" or "amp"
                {
                        if (len < 4)    // Not enough characters for either conversion
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                                return;
                        }
                        
                        if (*(parser->curr+1) == L'm')
                        {
                                // "amp"
                                if(*(parser->curr+2) == L'p' && *(parser->curr+3) == L';')
                                {
                                        ch = L'&';
                                        parser->curr += 4;
                                        break;
                                }
                                
                        } else if (*(parser->curr+1) == L'p')
                        {
                                // "apos"
                                if (len > 4 && *(parser->curr+2) == 'o' && *(parser->curr+3) == L's' && *(parser->curr+4) == L';')
                                {
                                        ch = L'\'';
                                        parser->curr += 5;
                                        break;
                                }
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unknown ampersand-escape sequence at line %qu", __calc_linenumber(parser));
                                return;
                        }
                }
                        break;
                case L'q':  // "quote"
                {
                        if (len >= 5 && *(parser->curr+1) == L'u' && *(parser->curr+2) == L'o' && *(parser->curr+3) == L't' && *(parser->curr+4) == L';')
                        {
                                ch = L'\"';
                                parser->curr += 5;
                                break;
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unknown ampersand-escape sequence at line %qu", __calc_linenumber(parser));
                                return;
                        }
                }
                        break;
                case L'#':
                {
                        ar_uint_16_t num = 0;
                        ar_bool_t is_hex = false;
                        
                        if(len < 4)   // Not enough characters to make it all fit!  Need at least "&#d;"
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                                return;
                        }
                        
                        parser->curr++;
                        if (*(parser->curr) == 'x')
                        {
                                is_hex = true;
                                parser->curr ++;
                        }
                        
                        while (parser->curr < parser->end)
                        {
                                ch = *(parser->curr);
                                parser->curr ++;
                                
                                if(ch == ';')
                                {
                                        if(!__cat_from_mark_to_buf(&ch, &ch + 1, str))
                                        {
                                                parser->has_error = true;
                                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                                return;
                                        }
                                        return;
                                }
                                
                                if(!is_hex)
                                {
                                        num = num * 10;
                                }else
                                {
                                        num = num * 16;
                                }
                                
                                if(ch <= L'9' && ch >= L'0')
                                {
                                        num += (ch - L'0');
                                        
                                }else if(!is_hex)
                                {
                                        parser->has_error = true;
                                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c at line %qu", ch, __calc_linenumber(parser));
                                        return;
                                }else if(ch >= L'a' && ch <= L'f')
                                {
                                        num += 10 + (ch - L'a');
                                        
                                }else if(ch >= L'A' && ch <= L'F')
                                {
                                        num += 10 + (ch - L'A');
                                        
                                }else
                                {
                                        parser->has_error = true;
                                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c at line %qu", ch, __calc_linenumber(parser));
                                        return;
                                }
                        }
                        
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                        return;
                }
                default:
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unknown ampersand-escape sequence at line %qu", __calc_linenumber(parser));
                        return;
        }
        
        if(!__cat_from_mark_to_buf(&ch, &ch + 1, str))
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                return;
        }
}


static ar_bool_t    __check_for_closetag(plistXMLParser_t *parser, const wchar_t *tag, size_t tag_len)
{
        
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        AR_ASSERT(tag != NULL && tag_len > 0);
        
		if((size_t)(parser->end - parser->curr) < (tag_len + 3))
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                }
                
                return false;
        }
        
        if(*parser->curr != L'<' || *(++parser->curr) != L'/')
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %qu", *(parser->curr), __calc_linenumber(parser));
                }
                return false;
        }
        
        parser->curr++;
        
        if(!__match_string(parser->curr, tag, tag_len))
        {
                
                if(!parser->has_error)
                {
                        wchar_t buf[128];
                        AR_ASSERT(tag_len < 128);
                        AR_wcsncpy(buf, tag, tag_len);
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Close tag on line %qu does not match open tag %ls", __calc_linenumber(parser), buf);
                }
                return false;
        }

        parser->curr += tag_len;
        
        __skip_whitespace(parser);
        
        if(parser->curr == parser->end)
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                }
                
                return false;
        }
        
        
        if(*parser->curr != L'>')
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered unexpected character %c on line %qu", *(parser->curr), __calc_linenumber(parser));
                }
                return false;
        }
        
        parser->curr++;
        return true;
}



// pInfo should be set to the first content character of the <plist>
static plistElem_t* __parse_xml_plisttag(plistXMLParser_t *parser)
{
        plistElem_t *result, *tmp;
        const wchar_t *save;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        result = NULL;
        tmp = NULL;
        
        result = __get_xml_content_object(parser);
        
        if(!result)
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered empty plist tag");
                }
                return NULL;
        }
        
        save = parser->curr;
        
        tmp = __get_xml_content_object(parser);
        
        if(tmp)
        {
                PList_DestroyElem(result);
                result = NULL;
                PList_DestroyElem(tmp);
                tmp = NULL;
                parser->curr = save;
                
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected element at line %qu (plist can only include one object)",  __calc_linenumber(parser));
                return NULL;
        }
        
        if(parser->has_error)
        {
                // Parse failed catastrophically
                PList_DestroyElem(result);
                result = NULL;

                return NULL;
        }
        
        
        if(__check_for_closetag(parser, __g_plist_tags[PLIST_IX], PLIST_TAG_LENGTH))
        {
                return result;
        }else
        {
                PList_DestroyElem(result);
                result = NULL;
                return NULL;
        }
}


static plistElem_t* __parse_xml_arraytag(plistXMLParser_t *parser)
{
        plistElem_t     *array, *tmp;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        array = PList_CreateElem(PLIST_ELEM_ARRAY_T);
        tmp = NULL;
        if(array == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        tmp = __get_xml_content_object(parser);
        while(tmp)
        {
                if(PList_PushToArray(&array->array, tmp) != AR_S_YES)
                {
                        PList_DestroyElem(tmp);
                        tmp = NULL;
                        parser->has_error = true;
                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                        goto INVALID_POINT;
                }
                tmp = __get_xml_content_object(parser);
        }
        
        if(parser->has_error)
        {
                goto INVALID_POINT;
        }
        
        
        if(!__check_for_closetag(parser, __g_plist_tags[ARRAY_IX], ARRAY_TAG_LENGTH))
        {
                goto INVALID_POINT;
        }
        
        return array;
INVALID_POINT:
        if(array)
        {
                PList_DestroyElem(array);
                array = NULL;
        }
        return NULL;
}

static plistElem_t* __parse_xml_dicttag(plistXMLParser_t *parser)
{
        plistElem_t     *dict, *k, *v;
        const wchar_t   *base;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        dict = NULL;
        k = NULL;
        v = NULL;
        
        dict = PList_CreateElem(PLIST_ELEM_DICT_T);

        if(dict == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        base = parser->curr;
        
        k = __get_xml_content_object(parser);
        
        while(k)
        {
                if(PList_GetElemType(k) != PLIST_ELEM_STRING_T)
                {
                        PList_DestroyElem(k);
                        k = NULL;
                        
                        if(!parser->has_error)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Found non-string inside <dict> at line %qu)",  __calc_linenumber(parser));
                        }
                        
                        goto INVALID_POINT;
                }
                
                v = __get_xml_content_object(parser);
                
                if(v == NULL)
                {
                        PList_DestroyElem(k);
                        k = NULL;
                        
                        if(!parser->has_error)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Value missing for key inside <dict> at line %qu)",  __calc_linenumber(parser));
                        }
                        
                        goto INVALID_POINT;
                }
                
                if(PList_SetDictValueForKey(&dict->dict, k, v) != AR_S_YES)
                {
                        parser->has_error = true;
                        parser->curr = base;
                        AR_FormatString(parser->errmsg, L"Failed to set for key %ls inside <dict> at line %qu)", PList_GetStringCString(&k->str), __calc_linenumber(parser));
                        
                        
                        PList_DestroyElem(k);
                        k = NULL;
                        PList_DestroyElem(v);
                        v = NULL;
                        
                        goto INVALID_POINT;
                }
                
                k = NULL;
                v = NULL;
                
                base = parser->curr;
                k = __get_xml_content_object(parser);
        }
        
        if(parser->has_error)
        {
                goto INVALID_POINT;
        }
        
        
        if(!__check_for_closetag(parser, __g_plist_tags[DICT_IX], DICT_TAG_LENGTH))
        {
                goto INVALID_POINT;
        }
        
        
        return dict;
        
INVALID_POINT:
        if(dict)
        {
                PList_DestroyElem(dict);
                dict = NULL;
        }
        return NULL;

}


static plistElem_t* __parse_xml_datatag(plistXMLParser_t *parser)
{
        plistElem_t *data;
        const wchar_t *mark;
        char *base64_str;
        size_t i, base64_str_len;
        ar_int_t l;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        data = NULL;
        mark = parser->curr;
        data = PList_CreateElem(PLIST_ELEM_DATA_T);
        base64_str = NULL;
        base64_str_len = 0;
        
        if(data == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        mark = parser->curr;
        
        while(parser->curr < parser->end)
        {
                wchar_t ch = (char)*parser->curr;  /*valid base64 string always ascii*/
                if(ch == L'<')
                {
                        break;
                }else
                {
                        parser->curr++;
                }
        }
        
        if(parser->curr >= parser->end)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
                goto INVALID_POINT;
        }
        
        base64_str_len = parser->curr - mark;
        
        if(base64_str_len > 0)
        {
        
                base64_str = AR_NEWARR(char, base64_str_len);
        
                if(base64_str == NULL)
                {
                        parser->has_error = true;
                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                        goto INVALID_POINT;
                }
                
                for(i = 0; i < (size_t)(parser->curr - mark); ++i)
                {
                        base64_str[i] = (char)mark[i];
                }
        
                //ar_int_t AR_base64_decode(ar_byte_t  *out, size_t olen, const ar_byte_t *input, size_t ilen)

                l = AR_base64_decode(NULL, 0, (const ar_byte_t*)base64_str, base64_str_len);
                
                if(l > 0)
                {
                        ar_int_t real_l;
                        ar_byte_t *tmp = AR_AllocFromBuffer(data->data.buf, (size_t)l);
                        
                        if(tmp == NULL)
                        {
                                parser->has_error = true;
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                goto INVALID_POINT;
                        }
                        
                        real_l = AR_base64_decode(tmp, l, (const ar_byte_t*)base64_str, base64_str_len);
                        AR_ASSERT(real_l < l);
                        AR_EraseBufferBack(data->data.buf, l - real_l);
                }
        }
        
        
        if(!__check_for_closetag(parser, __g_plist_tags[DATA_IX], DATA_TAG_LENGTH))
        {
                goto INVALID_POINT;
        }
        
        if(base64_str)
        {
                AR_DEL(base64_str);
                base64_str = NULL;
        }

        return data;
        
        
        
INVALID_POINT:
        
        if(data)
        {
                PList_DestroyElem(data);
                data = NULL;
        }
        
        if(base64_str)
        {
                AR_DEL(base64_str);
                base64_str = NULL;
        }
        
        return NULL;

}



static ar_bool_t __read_2_digit_number(plistXMLParser_t *parser, ar_uint_32_t *num)
{
        wchar_t ch1, ch2;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        

        if(parser->curr + 2 >= parser->end)
        {
                return false;
        }
        
        ch1 = *parser->curr;
        ch2 = *(parser->curr + 1);
        parser->curr += 2;
        
        if(!AR_iswdigit(ch1) || !AR_isdigit(ch2))
        {
                return false;
        }
        *num = (ch1 - L'0') * 10 + (ch2 - L'0');
        return true;
}


// YYYY '-' MM '-' DD 'T' hh ':' mm ':' ss 'Z'
static plistElem_t* __parse_xml_datetag(plistXMLParser_t *parser)
{
        ar_uint_32_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;

        ar_bool_t bad_form = false;
        
        AR_ASSERT(parser != NULL && parser->curr != NULL);

        while(parser->curr < parser->end && AR_iswdigit(*parser->curr))
        {
                year = 10 * year + (*parser->curr) - L'0';
                parser->curr++;
        }
        
        if (parser->curr >= parser->end || *parser->curr != L'-')
        {
                bad_form = true;
        }else
        {
                parser->curr++;
        }

        
        if(!bad_form && __read_2_digit_number(parser, &month) && parser->curr < parser->end && *parser->curr == L'-')
        {
                parser->curr++;
        }else
        {
                bad_form = true;
        }
        
        
        if(!bad_form && __read_2_digit_number(parser, &day) && parser->curr < parser->end && *parser->curr == L'T')
        {
                parser->curr++;
        }else
        {
                bad_form = true;
        }
        
        
        if(!bad_form && __read_2_digit_number(parser, &hour) && parser->curr < parser->end && *parser->curr == L':')
        {
                parser->curr++;
        }else
        {
                bad_form = true;
        }
        
        
        
        if(!bad_form && __read_2_digit_number(parser, &minute) && parser->curr < parser->end && *parser->curr == L':')
        {
                parser->curr++;
        }else
        {
                bad_form = true;
        }
        
        
        if(!bad_form && __read_2_digit_number(parser, &second) && parser->curr < parser->end && *parser->curr == L'Z')
        {
                parser->curr++;
        }else
        {
                bad_form = true;
        }
        
        if(bad_form)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Could not interpret <date> at line %qu)", __calc_linenumber(parser));
                return NULL;
        }
        
        
        if(!__check_for_closetag(parser, __g_plist_tags[DATE_IX], DATE_TAG_LENGTH))
        {
                return NULL;
        }

        
        {
                plistElem_t *date = PList_CreateElem(PLIST_ELEM_DATE_T);
                
                if(date == NULL)
                {
                        parser->has_error = true;
                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                        return NULL;
                }

                date->date.year = (ar_uint_16_t)year;
                date->date.month = (ar_uint_8_t)month;
                date->date.day = (ar_uint_8_t)day;
                date->date.hour = (ar_uint_8_t)hour;
                date->date.minute = (ar_uint_8_t)minute;
                date->date.second = (ar_uint_8_t)second;
                return date;
        }
}



static plistElem_t* __parse_xml_realtag(plistXMLParser_t *parser)
{
        plistElem_t *str, *num;
        const wchar_t *wcs;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        str = NULL;
        num = NULL;
        wcs = NULL;
        
        str = __get_string(parser);
        
        if(str == NULL)
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered empty <real> on line %qu)", __calc_linenumber(parser));
                }
                goto INVALID_POINT;
        }
        
        num = PList_CreateElem(PLIST_ELEM_NUMBER_T);

        if(num == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        
        wcs = PList_GetStringCString(&str->str);
        num->number.type = PLIST_NUMBER_REAL_T;
        
        if(AR_wcsicmp(wcs, L"nan") == 0)
        {
                num->number.real.t = PLIST_REAL_NAN_T;
        }else if(AR_wcsicmp(wcs, L"+infinity") == 0)
        {
                num->number.real.t = PLIST_REAL_POSITIVE_INF_T;
                
        }else if(AR_wcsicmp(wcs, L"-infinity") == 0)
        {
                num->number.real.t = PLIST_REAL_NEGATIVE_INF_T;
                
        }else if(AR_wcsicmp(wcs, L"infinity") == 0)
        {
                num->number.real.t = PLIST_REAL_INF_T;
                
        }else if(AR_wcsicmp(wcs, L"inf") == 0)
        {
                num->number.real.t = PLIST_REAL_INF_T;
                
        }else if(AR_wcsicmp(wcs, L"-inf") == 0)
        {
                num->number.real.t = PLIST_REAL_NEGATIVE_INF_T;
                
        }else if(AR_wcsicmp(wcs, L"+inf") == 0)
        {
                num->number.real.t = PLIST_REAL_POSITIVE_INF_T;
        }else
        {
                num->number.real.t = PLIST_REAL_NORMAL_T;

                if(AR_wtod(wcs, &num->number.real.num) == NULL)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered misformatted real %ls on line %qu)", wcs, __calc_linenumber(parser));
                        goto INVALID_POINT;
                }
                
                if(AR_is_nan_dbl(num->number.real.num))
                {
                        num->number.real.t = PLIST_REAL_NAN_T;
                }
                
                if(AR_is_inf_dbl(num->number.real.num))
                {
                        num->number.real.t = PLIST_REAL_INF_T;
                }
        }
        
        if(!__check_for_closetag(parser, __g_plist_tags[REAL_IX], REAL_TAG_LENGTH))
        {
                goto INVALID_POINT;
        }
        
        if(str)
        {
                PList_DestroyElem(str);
                str = NULL;
        }
        
        return num;
INVALID_POINT:
        
        if(num)
        {
                PList_DestroyElem(num);
                num = NULL;
        }
        
        if(str)
        {
                PList_DestroyElem(str);
                str = NULL;
        }
        
        return NULL;
}



static plistElem_t* __parse_xml_integertag(plistXMLParser_t *parser)
{
        
        plistElem_t *str, *num;
        const wchar_t *wcs;
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        str = NULL;
        num = NULL;
        wcs = NULL;
        
        str = __get_string(parser);
        
        if(str == NULL)
        {
                if(!parser->has_error)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Encountered empty <integer> on line %qu)", __calc_linenumber(parser));
                }
                goto INVALID_POINT;
        }
        
        num = PList_CreateElem(PLIST_ELEM_NUMBER_T);
        
        if(num == NULL)
        {
                parser->has_error = true;
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                goto INVALID_POINT;
        }
        
        wcs = PList_GetStringCString(&str->str);
        num->number.type = PLIST_NUMBER_REAL_T;
        
        if(AR_wtou64(wcs, &num->number.integer.unsigned_num, 0) == NULL)
        {
                if(AR_wtoi64(wcs, &num->number.integer.signed_num, 0) == NULL)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Invalid <integer> %ls on line %qu)", wcs, __calc_linenumber(parser));
                        goto INVALID_POINT;
                        
                }else
                {
                        num->number.integer.is_signed = true;
                }
        }else
        {
                num->number.integer.is_signed = false;
        }
        
        
        if(str)
        {
                PList_DestroyElem(str);
                str = NULL;
        }
        
        return num;
INVALID_POINT:
        
        if(num)
        {
                PList_DestroyElem(num);
                num = NULL;
        }
        
        if(str)
        {
                PList_DestroyElem(str);
                str = NULL;
        }
        
        return NULL;
   
}

static plistElem_t*     __parse_xml_element(plistXMLParser_t *parser)
{
        const wchar_t *marker;
        ar_int_t        marker_length;
        ar_bool_t       is_empty;
        ar_int_t       marker_idx;
        AR_ASSERT(parser != NULL);
        AR_ASSERT(parser->curr != NULL);
        
        marker = parser->curr;
        marker_length = -1;
        is_empty = false;
        marker_idx = -1;
        
        while(parser->curr < parser->end)
        {
                wchar_t ch = *parser->curr;
                
                if(AR_iswspace(ch))
                {
                        if(marker_length == -1)
                        {
                                marker_length = parser->curr - marker;
                        }
                }else if(ch == L'>')
                {
                        break;
                }
                
                parser->curr++;
        }
        
        if(parser->curr >= parser->end)
        {
                return NULL;
        }
        
        if( *(parser->curr - 1) == L'/')
        {
                is_empty = true;
        }
        
        if(marker_length == -1)
        {
                marker_length = parser->curr - (is_empty ? 1 : 0) - marker;
        }
        
        parser->curr++; // Advance past '>'
        
        
        if(marker_length == 0)
        {
                parser->curr = marker;
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Malformed tag on line %qu", __calc_linenumber(parser));
                return NULL;
                
        }
        
        switch (*marker)
        {
                case L'A':
                case L'a':   // Array
                        if(marker_length == ARRAY_TAG_LENGTH && __match_string(marker, __g_plist_tags[ARRAY_IX], ARRAY_TAG_LENGTH))
                        {
                                marker_idx = ARRAY_IX;
                        }
                        break;
                case L'D': // Dictionary, data, or date; Fortunately, they all have the same marker length....
                case L'd':
                        if (marker_length != DICT_TAG_LENGTH)
                        {
                                break;
                        }
                        
                        if(__match_string(marker, __g_plist_tags[DICT_IX], DICT_TAG_LENGTH))
                        {
                                marker_idx = DICT_IX;
                        }else if(__match_string(marker, __g_plist_tags[DATA_IX], DATA_TAG_LENGTH))
                        {
                                marker_idx = DATA_IX;
                        }else if(__match_string(marker, __g_plist_tags[DATE_IX], DATE_TAG_LENGTH))
                        {
                                marker_idx = DATE_IX;
                        }
                        
                        break;
                        
                case L'I':
                case L'i': // integer
                        if(marker_length == INTEGER_TAG_LENGTH && __match_string(marker, __g_plist_tags[INTEGER_IX], INTEGER_TAG_LENGTH))
                        {
                                marker_idx = INTEGER_IX;
                        }
                        break;
                        
                case L'K':
                case L'k': // Key of a dictionary
                        if(marker_length == KEY_TAG_LENGTH && __match_string(marker, __g_plist_tags[KEY_IX], KEY_TAG_LENGTH))
                        {
                                marker_idx = KEY_IX;
                        }
                        break;
                        
                case L'P':
                case L'p': // Plist
                        
                        if(marker_length == PLIST_TAG_LENGTH && __match_string(marker, __g_plist_tags[PLIST_IX], PLIST_TAG_LENGTH))
                        {
                                marker_idx = PLIST_IX;
                        }
                        break;
                case L'R':
                case L'r': // real
                        
                        if(marker_length == REAL_TAG_LENGTH && __match_string(marker, __g_plist_tags[REAL_IX], REAL_TAG_LENGTH))
                        {
                                marker_idx = REAL_IX;
                        }
                        break;
                        
                case L'S':
                case L's': // String
                        
                        if(marker_length == STRING_TAG_LENGTH && __match_string(marker, __g_plist_tags[STRING_IX], STRING_TAG_LENGTH))
                        {
                                marker_idx = STRING_IX;
                        }
                        break;
                        
                case L'T':
                case L't': // true (boolean)
                        if(marker_length == TRUE_TAG_LENGTH && __match_string(marker, __g_plist_tags[TRUE_IX], TRUE_TAG_LENGTH))
                        {
                                marker_idx = TRUE_IX;
                        }
                        break;
                case L'F':
                case L'f': // false (boolean)
                        
                        if(marker_length == FALSE_TAG_LENGTH && __match_string(marker, __g_plist_tags[FALSE_IX], FALSE_TAG_LENGTH))
                        {
                                marker_idx = FALSE_IX;
                        }
                        break;
        }
        
        
        
        
        switch (marker_idx)
        {
                case PLIST_IX:
                        if (is_empty)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered empty plist tag", __calc_linenumber(parser));
                                return NULL;
                        }
                        return __parse_xml_plisttag(parser);
                case ARRAY_IX:
                        
                        if (is_empty)
                        {
                                plistElem_t *arr = PList_CreateElem(PLIST_ELEM_ARRAY_T);
                                
                                if(arr == NULL)
                                {
                                        parser->has_error = true;
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        return NULL;
                                }else
                                {
                                        return arr;
                                }
                        }else
                        {
                                return __parse_xml_arraytag(parser);
                        }
                case DICT_IX:
                        if (is_empty)
                        {
                                plistElem_t *dict = PList_CreateElem(PLIST_ELEM_DICT_T);
                                
                                if(dict == NULL)
                                {
                                        parser->has_error = true;
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        return NULL;
                                }else
                                {
                                        return dict;
                                }
                        }else
                        {
                                return __parse_xml_dicttag(parser);
                        }
                case KEY_IX:
                case STRING_IX:
                {
                        size_t tag_len = 0;
                        plistElem_t *str = NULL;
                        
                        tag_len = marker_idx == KEY_IX ? KEY_TAG_LENGTH : STRING_TAG_LENGTH;
                        
                        if(is_empty)
                        {
                                str = PList_CreateElem(PLIST_ELEM_STRING_T);
                                if(str == NULL)
                                {
                                        parser->has_error = true;
                                        AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                                        return NULL;
                                }else
                                {
                                        return str;
                                }
                        }else
                        {
                                str = __get_string(parser);
                                
                                if(str == NULL) // getString will already have set the error string
                                {
                                        return NULL;
                                }
                                
                                if(!__check_for_closetag(parser, __g_plist_tags[marker_idx], tag_len))
                                {
                                        PList_DestroyElem(str);
                                        str = NULL;
                                        return NULL;
                                }
                                
                                return str;
                        }
                }
                case DATA_IX:
                        if(is_empty)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered empty <data> on line %qu", __calc_linenumber(parser));
                                return NULL;
                        }else
                        {
                                return __parse_xml_datatag(parser);
                        }
                case DATE_IX:
                        if (is_empty)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered empty <date> on line %qu", __calc_linenumber(parser));
                                return NULL;
                        } else {
                                return __parse_xml_datetag(parser);
                        }
                case TRUE_IX:
                        if (!is_empty)
                        {
                                if(!__check_for_closetag(parser, __g_plist_tags[TRUE_IX], TRUE_TAG_LENGTH))
                                {
                                        return NULL;
                                }
                        }else
                        {
                                return __g_boolean_true;
                        }
                case FALSE_IX:
                        if (!is_empty)
                        {
                                if(!__check_for_closetag(parser, __g_plist_tags[FALSE_IX], FALSE_TAG_LENGTH))
                                {
                                        return NULL;
                                }
                        }else
                        {
                                return __g_boolean_false;
                        }
                        
                case REAL_IX:
                        if(is_empty)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered empty <real> on line %qu", __calc_linenumber(parser));
                                return NULL;
                        }else
                        {
                                return __parse_xml_realtag(parser);
                        }
                case INTEGER_IX:
                        if(is_empty)
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered empty <integer> on line %qu", __calc_linenumber(parser));
                                return NULL;
                        }else
                        {
                                return __parse_xml_integertag(parser);
                        }
                default:
                {
                        const wchar_t *marker_str = AR_wcsndup(marker, marker_length);
                        
                        parser->has_error = true;
                        parser->curr = marker;
                        
                        if(marker_str == NULL)
                        {
                                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                        }else
                        {
                                parser->has_error = true;
                                AR_FormatString(parser->errmsg, L"Encountered unknown tag %ls on line %qu", marker_str, __calc_linenumber(parser));
                                AR_DEL(marker_str);
                        }
                        return NULL;
                }
        }
        
        
        
        
}

static plistElem_t*     __parse_property_list(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        
        while(!parser->has_error && parser->curr < parser->end)
        {
                wchar_t ch;
                __skip_whitespace(parser);
                
                if(parser->curr + 1 >= parser->end)
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"No XML content found");
                        return NULL;
                }
                
                if(*parser->curr != L'<')
                {
                        parser->has_error = true;
                        AR_FormatString(parser->errmsg, L"Unexpected character %c at line %qu", *parser->curr, __calc_linenumber(parser));
                        return NULL;
                }
                
                parser->curr++;
                ch = *parser->curr;
                
                if(ch == L'!')
                {
                        /*comment or DTD*/
                        parser->curr++;
                        if(parser->curr + 1 < parser->end && *parser->curr == L'-' && *(parser->curr + 1) == L'-')
                        {
                                /*Comment*/
                                parser->curr += 2;
                                __skip_xml_comment(parser);
                        }else
                        {
                                __skip_xml_dtd(parser);
                        }
                }else if(ch == L'?')
                {
                        parser->curr++;
                        __skip_xml_processing_instruction(parser);
                }else
                {
                        return __parse_xml_element(parser);
                }
        }
        
        if(!parser->has_error)
        {
                parser->has_error = true;
                AR_FormatString(parser->errmsg, L"Encountered unexpected EOF");
        }
        return NULL;
}



plistElem_t*            PList_ParseXML(plistXMLParser_t *parser)
{
        AR_ASSERT(parser != NULL && parser->curr != NULL);
        
        return __parse_property_list(parser);
        
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
        
        
		buf = AR_CreateBuffer(0);
        
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



