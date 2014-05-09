/*
 * The Arsenal Library
 * Copyright (c) 2009-2012 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#include "../Parser/parser.h"
#include "../Lex/lex.h"
#include "json.h"


AR_NAMESPACE_BEGIN



/*************************************************************Dict********************************************/

typedef struct __json_dict_pair_tag
{
		wchar_t			*key;
		jsonObj_t		*val;
}jsonPair_t;

typedef struct __json_dict_tag
{
		jsonPair_t		*pairs;
		size_t			count;
		size_t			cap;
}jsonDict_t;

static void	Init_JsonDict(jsonDict_t *dict)
{
		AR_ASSERT(dict != NULL);

		AR_memset(dict, 0, sizeof(*dict));
}

static void UnInit_JsonDict(jsonDict_t *dict)
{
		size_t i;
		AR_ASSERT(dict != NULL);

		for(i = 0; i < dict->count; ++i)
		{
				jsonPair_t *pair = &dict->pairs[i];
				AR_ASSERT(pair->key != NULL);

				AR_DEL(pair->key);
				pair->key = NULL;

				if(pair->val)
				{
						Json_DestroyObject(pair->val);
						pair->val = NULL;
				}
		}

		if(dict->pairs)
		{
				AR_DEL(dict->pairs);
				dict->pairs = NULL;
		}

		AR_memset(dict, 0, sizeof(*dict));
}



static arStatus_t Compare_JsonDict(const jsonDict_t *l, const jsonDict_t *r)
{
		ar_int_t cmp;
		size_t i;
		AR_ASSERT(l != NULL && r != NULL);


		if(l == r)
		{
				return AR_S_EQ;
		}

		cmp = AR_CMP(l->count, r->count);

		if(cmp != 0)
		{
				return cmp > 0 ? AR_S_GE : AR_S_LE;
		}

		for(i = 0; i < l->count; ++i)
		{
				arStatus_t res = AR_S_EQ;
				const wchar_t *l_key, *r_key;
				const jsonObj_t *l_val, *r_val;

				l_key = l->pairs[i].key;
				r_key = r->pairs[i].key;

				l_val = l->pairs[i].val;
				r_val = r->pairs[i].val;

				AR_ASSERT(l_key != NULL && r_key != NULL);
				


				cmp = AR_wcscmp(l_key, r_key);

				if(cmp != 0)
				{
						return cmp > 0 ? AR_S_GE : AR_S_LE;
				}

				cmp = AR_CMP(l_val, r_val);

				if(cmp != 0)
				{
						return cmp > 0 ? AR_S_GE : AR_S_LE;
				}

				
				res = Json_CompareObject(l_val, r_val);

				if(res != AR_S_EQ)
				{
						return res;
				}
		}

		return AR_S_EQ;

		
}



static ar_int_t __find_item_by_key(const jsonDict_t *dict, const wchar_t *key)
{
		ar_int_t i;
		AR_ASSERT(dict != NULL && key != NULL);

		for(i = 0; i < (ar_int_t)dict->count; ++i)
		{
				const wchar_t *l = dict->pairs[i].key;

				if(AR_wcscmp(l, key) == 0)
				{
						return i;
				}
		}

		return -1;
}


static arStatus_t __increate_dict_pairs(jsonDict_t *dict)
{
		size_t			new_cap;
		jsonPair_t		*new_lst;
		AR_ASSERT(dict != NULL);

		new_cap = (dict->cap + 4) * 2;
		new_lst = AR_NEWARR(jsonPair_t, new_cap);

		if(new_lst == NULL)
		{
				return AR_E_NOMEM;
		}

		if(dict->count > 0)
		{
				AR_memcpy(new_lst, dict->pairs, dict->count * sizeof(jsonPair_t));
		}


		if(dict->pairs)
		{
				AR_DEL(dict->pairs);
				dict->pairs = NULL;
		}

		dict->cap = new_cap;
		dict->pairs = new_lst;
		return AR_S_YES;
}


static arStatus_t InsertToDict(jsonDict_t *dict, const wchar_t *key, jsonObj_t *val, ar_bool_t replace)
{
		ar_int_t idx;
		arStatus_t status;
		jsonPair_t *pair = NULL;
		AR_ASSERT(dict != NULL && key != NULL);

		status = AR_S_YES;

		idx = __find_item_by_key(dict, key);

		

		if(idx < 0)
		{
				if(dict->count == dict->cap)
				{
						status = __increate_dict_pairs(dict);

						if(status != AR_S_YES)
						{
								AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
								goto END_POINT;
						}
				}


				pair = &dict->pairs[dict->count];

				pair->key = AR_wcsdup(key);
				if(pair->key == NULL)
				{
						status = AR_E_NOMEM;
						AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
						goto END_POINT;
				}
				pair->val = val;
				dict->count++;
		}else
		{
				if(!replace)
				{
						status = AR_E_EXISTED;
						goto END_POINT;
				}

				pair = &dict->pairs[idx];

				if(pair->val)
				{
						Json_DestroyObject(pair->val);
						pair->val = val;
				}
		}
END_POINT:
		return status;
}

static arStatus_t RemoveFromDict(jsonDict_t *dict, const wchar_t *key)
{
		ar_int_t idx;
		arStatus_t status;
		jsonPair_t *pair;
		AR_ASSERT(dict != NULL);
		AR_ASSERT(key != NULL);
		status = AR_S_YES;
		idx = __find_item_by_key(dict, key);

		if(idx < 0)
		{
				status = AR_E_NOTFOUND;
				goto END_POINT;
		}

		pair = &dict->pairs[idx];

		AR_ASSERT(pair->key != NULL);
		if(pair->val)
		{
				Json_DestroyObject(pair->val);
				pair->val = NULL;
		}

		AR_memcpy(&dict->pairs[idx], &dict->pairs[dict->count-1], sizeof(jsonPair_t));
		
		dict->pairs[dict->count-1].key = NULL;
		dict->pairs[dict->count-1].val = NULL;
		dict->count -= 1;
		

END_POINT:
		return status;

}

static arStatus_t FindFromDict(jsonDict_t *dict, const wchar_t *key, jsonObj_t **pval)
{
		ar_int_t idx;
		AR_ASSERT(dict != NULL);
		AR_ASSERT(key != NULL);
		AR_ASSERT(pval != NULL);

		idx = __find_item_by_key(dict, key);

		if(idx >= 0)
		{
				*pval = dict->pairs[idx].val;
				return AR_S_YES;
		}else
		{
				return AR_E_NOTFOUND;
		}
}

/******************************************************************Array************************************************/

typedef struct __json_array_tag
{
		jsonObj_t		**lst;
		size_t			count;
		size_t			cap;
}jsonArray_t;

static void Init_JsonArray(jsonArray_t *array)
{
		AR_ASSERT(array != NULL);
		AR_memset(array, 0, sizeof(jsonArray_t));
}

static void UnInit_JsonArray(jsonArray_t *array)
{
		size_t i;
		AR_ASSERT(array != NULL);

		for(i = 0; i < array->count; ++i)
		{
				Json_DestroyObject(array->lst[i]);
		}

		if(array->lst)
		{
				AR_DEL(array->lst);
				array->lst = NULL;
		}

		AR_memset(array, 0,sizeof(jsonArray_t));
}

static arStatus_t Compare_JsonArray(const jsonArray_t *l, const jsonArray_t *r)
{
		ar_int_t cmp;
		size_t i;
		AR_ASSERT(l != NULL && r != NULL);

		if(l == r)
		{
				return AR_S_EQ;
		}

		cmp = AR_CMP(l->count, r->count);

		if(cmp != 0)
		{
				return cmp > 0 ? AR_S_GE : AR_S_LE;
		}

		for(i = 0; i < l->count; ++i)
		{
				arStatus_t res = AR_S_EQ;
				const jsonObj_t *l_item, *r_item;
				l_item = l->lst[i];
				r_item = r->lst[i];

				AR_ASSERT(l_item != NULL && r_item != NULL);

				res = Json_CompareObject(l_item, r_item);

				if(res != AR_S_EQ)
				{
						return res;
				}
		}

		return AR_S_EQ;

		
}

static arStatus_t __increate_array_list(jsonArray_t *array)
{
		size_t			new_cap;
		jsonObj_t		**new_lst;
		AR_ASSERT(array != NULL);

		new_cap = (array->cap + 4) * 2;
		new_lst = AR_NEWARR(jsonObj_t*, new_cap);

		if(new_lst == NULL)
		{
				return AR_E_NOMEM;
		}

		if(array->count > 0)
		{
				AR_memcpy(new_lst, array->lst, array->count * sizeof(jsonObj_t*));
		}


		if(array->lst)
		{
				AR_DEL(array->lst);
				array->lst = NULL;
		}

		array->cap = new_cap;
		array->lst = new_lst;
		return AR_S_YES;
}



static arStatus_t AppendToArray(jsonArray_t *array, jsonObj_t *obj)
{
		arStatus_t status;
		AR_ASSERT(array != NULL);

		status = AR_S_YES;

		if(array->count == array->cap)
		{
				status = __increate_array_list(array);
				if(status != AR_S_YES)
				{
						AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
						goto END_POINT;
				}
		}

		array->lst[array->count] = obj;
		array->count++;

END_POINT:
		return status;

}

static arStatus_t SetArrayByIndex(jsonArray_t *array, size_t idx, jsonObj_t *obj)
{
		AR_ASSERT(array != NULL && obj != NULL);

		if(idx >= array->count)
		{
				return AR_E_INVAL;
		}else
		{
				if(array->lst[idx])
				{
						Json_DestroyObject(array->lst[idx]);
						array->lst[idx] = NULL;
				}
				array->lst[idx] = obj;
				return AR_S_YES;
		}
}

static arStatus_t GetArrayByIndex(jsonArray_t *array, size_t idx, jsonObj_t **pobj)
{
		AR_ASSERT(array != NULL && pobj != NULL);

		if(idx >= array->count)
		{
				return AR_E_INVAL;
		}else
		{
				*pobj = array->lst[idx];
				return AR_S_YES;
		}
}




struct __json_object_tag
{
		jsonType_t		type;

		union{
				jsonDict_t		dict;
				jsonArray_t		array;

				wchar_t			*str;
				struct{
						ar_bool_t is_signed;
						union{
								ar_uint_64_t	un;
								ar_int_64_t		n;
						};
				}integer;
				double			float_num;

				ar_bool_t		boolean;
		};
};




jsonObj_t*		Json_CreateObject(jsonType_t t)
{
		jsonObj_t		*obj;

		obj = AR_NEW0(jsonObj_t);
		if(obj)
		{
				obj->type = t;
		}else
        {
                AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
                return NULL;
        }

		switch(obj->type)
		{
		case JSON_TYPE_DICT_T:
				Init_JsonDict(&obj->dict);
				break;
		case JSON_TYPE_ARRAY_T:
				Init_JsonArray(&obj->array);
				break;
		case JSON_TYPE_STRING_T:
				obj->str = AR_wcsdup(L"");
				break;
		case JSON_TYPE_INT_T:
				obj->integer.n = 0;
				break;
		case JSON_TYPE_FLOAT_T:
				obj->float_num = 0.0;
				break;
		case JSON_TYPE_BOOL_T:
				obj->boolean = true;
				break;
		case JSON_TYPE_NULL_T:
				break;
		default:
				AR_ASSERT(false);
				break;
		}
		return obj;
}

void			Json_DestroyObject(jsonObj_t *obj)
{
		AR_ASSERT(obj);

		switch(obj->type)
		{
		case JSON_TYPE_DICT_T:
				UnInit_JsonDict(&obj->dict);
				break;
		case JSON_TYPE_ARRAY_T:
				UnInit_JsonArray(&obj->array);
				break;
		case JSON_TYPE_STRING_T:
				if(obj->str)
				{
						AR_DEL(obj->str);
						obj->str = NULL;
				}
				break;
		case JSON_TYPE_INT_T:
				break;
		case JSON_TYPE_FLOAT_T:
				break;
		case JSON_TYPE_BOOL_T:
				break;
		case JSON_TYPE_NULL_T:
				break;
		default:
				AR_ASSERT(false);
				break;
		}

		AR_DEL(obj);
		obj = NULL;
}

arStatus_t		Json_CompareObject(const jsonObj_t *l, const jsonObj_t *r)
{
		ar_int_t cmp;
		AR_ASSERT(l != NULL && r != NULL);

		if(l->type != r->type)
		{
				cmp = (ar_int_t)l->type - (ar_int_t)r->type;
				return cmp > 0 ? AR_S_GE : AR_S_LE;
		}

		switch(l->type)
		{
		case JSON_TYPE_DICT_T:
				return Compare_JsonDict(&l->dict, &r->dict);
		case JSON_TYPE_ARRAY_T:
				return Compare_JsonArray(&l->array, &r->array);
		case JSON_TYPE_STRING_T:
		{
				cmp = AR_wcscmp(l->str, r->str);
				
				if(cmp == 0)
				{
						return AR_S_EQ;
				}else
				{
						return cmp > 0 ? AR_S_GE : AR_S_LE;
				}
		}
				break;
		case JSON_TYPE_INT_T:
				cmp = AR_CMP(l->integer.un, r->integer.un);
				if(cmp == 0)
				{
						return AR_S_EQ;
				}else
				{
						return cmp > 0 ? AR_S_GE : AR_S_LE;
				}
				break;
		case JSON_TYPE_FLOAT_T:
				if(AR_DBL_EQ(l->float_num, r->float_num))
				{
						return AR_S_EQ;
				}else
				{
						return AR_DBL_GE(l->float_num, r->float_num) ? AR_S_GE : AR_S_LE;
				}

				break;
		case JSON_TYPE_BOOL_T:
				cmp = AR_CMP(l->boolean, r->boolean);
				if(cmp == 0)
				{
						return AR_S_EQ;
				}else
				{
						return cmp > 0 ? AR_S_GE : AR_S_LE;
				}
				break;
		case JSON_TYPE_NULL_T:
				return AR_S_EQ;
		default:
				AR_ASSERT(false);
				break;
		}

		return AR_S_EQ;
}

jsonType_t		Json_GetObjectType(const jsonObj_t *obj)
{
		AR_ASSERT(obj != NULL);
		return obj->type;
}

#define CHECK_MATCHED_TYPE(_obj, _t)			\
		do{										\
				if((_obj)->type != (_t))		\
				{								\
						return AR_E_NOTMATCHED;	\
				}								\
		}while(0)		

arStatus_t		Json_SetIntToObject(jsonObj_t *obj, ar_int_64_t num)
{
		AR_ASSERT(obj != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_INT_T);

		obj->integer.is_signed = true;
		obj->integer.n = num;
		return AR_S_YES;
}

arStatus_t		Json_GetIntFromObject(jsonObj_t *obj, ar_int_64_t *num)
{
		AR_ASSERT(obj != NULL && num != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_INT_T);
		*num = obj->integer.n;
		return AR_S_YES;
}

arStatus_t		Json_SetUIntToObject(jsonObj_t *obj, ar_uint_64_t num)
{
		AR_ASSERT(obj != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_INT_T);

		obj->integer.is_signed = false;
		obj->integer.un = num;
		return AR_S_YES;
}

arStatus_t		Json_GetUIntFromObject(jsonObj_t *obj, ar_uint_64_t *num)
{
		AR_ASSERT(obj != NULL && num != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_INT_T);

		*num = obj->integer.un;
		return AR_S_YES;
}

arStatus_t		Json_SetFloatToObject(jsonObj_t *obj, double num)
{
		AR_ASSERT(obj != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_FLOAT_T);
		obj->float_num = num;
		return AR_S_YES;
}

arStatus_t		Json_GetFloatFromObject(jsonObj_t *obj, double *num)
{
		AR_ASSERT(obj != NULL && num != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_FLOAT_T);
		*num = obj->float_num;
		return AR_S_YES;
}

arStatus_t		Json_SetStringToObject(jsonObj_t *obj, const wchar_t *str)
{
		wchar_t *new_str;
		AR_ASSERT(obj != NULL && str != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_STRING_T);

		new_str = AR_wcsdup(str);

		if(new_str == NULL)
		{
				AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
				return AR_E_NOMEM;
		}

		if(obj->str)
		{
				AR_DEL(obj->str);
				obj->str = NULL;
		}

		obj->str = new_str;
		return AR_S_YES;

}

arStatus_t		Json_GetStringFromObject(jsonObj_t *obj, arString_t *str)
{
		AR_ASSERT(obj != NULL && str != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_STRING_T);
		return AR_SetString(str, obj->str);
}

const wchar_t*	Json_GetWcsFromObject(jsonObj_t *obj)
{
		AR_ASSERT(obj != NULL);

		if(obj->type != JSON_TYPE_STRING_T)
		{
				return NULL;
		}

		return obj->str;

}


arStatus_t		Json_SetBooleanToObject(jsonObj_t *obj, ar_bool_t b)
{
		AR_ASSERT(obj != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_BOOL_T);
		obj->boolean = b;
		return AR_S_YES;
}

arStatus_t		Json_GetBooleanFromObject(jsonObj_t *obj, ar_bool_t *b)
{
		AR_ASSERT(obj != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_BOOL_T);
		*b = obj->boolean;
		return AR_S_YES;
}


arStatus_t		Json_GetArrayObjectCount(const jsonObj_t *obj, size_t *count)
{
		AR_ASSERT(obj != NULL && count != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_ARRAY_T);
		*count = obj->array.count;
		return AR_S_YES;
}


arStatus_t		Json_SetArrayObjectByIndex(jsonObj_t *obj, size_t idx, jsonObj_t *item)
{
		AR_ASSERT(obj != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_ARRAY_T);
		return SetArrayByIndex(&obj->array, idx, item);
}

arStatus_t		Json_GetArrayObjectByIndex(jsonObj_t *obj, size_t idx, jsonObj_t **item)
{
		AR_ASSERT(obj != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_ARRAY_T);
		return GetArrayByIndex(&obj->array, idx, item);
}

arStatus_t		Json_AppendToArrayObject(jsonObj_t *obj, jsonObj_t *item)
{
		AR_ASSERT(obj != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_ARRAY_T);
		return AppendToArray(&obj->array, item);
}


arStatus_t		Json_GetDictObjectCount(const jsonObj_t *obj, size_t *count)
{
		AR_ASSERT(obj != NULL && count != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_DICT_T);
		*count = obj->dict.count;
		return AR_S_YES;
}

arStatus_t		Json_SetDictObject(jsonObj_t *obj, const wchar_t *key, jsonObj_t *val, ar_bool_t replace_exist)
{
		AR_ASSERT(obj != NULL && key != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_DICT_T);
		return InsertToDict(&obj->dict, key, val, replace_exist);
}

arStatus_t		Json_GetDictObject(jsonObj_t *obj, const wchar_t *key, jsonObj_t **val)
{
		AR_ASSERT(obj != NULL && key != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_DICT_T);
		return FindFromDict(&obj->dict, key, val);
}

arStatus_t		Json_RemoveDictObject(jsonObj_t *obj, const wchar_t *key)
{
		AR_ASSERT(obj != NULL && key != NULL);
		CHECK_MATCHED_TYPE(obj, JSON_TYPE_DICT_T);
		return RemoveFromDict(&obj->dict, key);
}





/***************************************************************************************************/

jsonObj_t* parse_json_escape_string(const wchar_t *lexer_str, size_t count)
{
		jsonObj_t *obj;
		wchar_t *str ;
		const wchar_t *p, *e;

		wchar_t *pstr;

		AR_ASSERT(lexer_str != NULL);

		obj = NULL;
		str = AR_NEWARR(wchar_t, count + 1);

		if(str == NULL)
		{
				AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
				return NULL;
		}

		p = lexer_str;
		e = p + count;
		pstr = str;

		while(*p && p < e)
		{
				if(*p == L'\\')
				{
						++p;
						switch(*p)
						{
						case L'\\':
								*pstr++ = *p++;
								break;
						case L'b':
								*pstr++ = L'\b';
								++p;
								break;
						case L'f':
								*pstr++ = L'\f';
								++p;
								break;
						case L'n':
								*pstr++ = L'\n';
								++p;
								break;
						case L'r':
								*pstr++ = L'\r';
								++p;
								break;
						case L't':
								*pstr++ = L'\t';
								++p;
								break;
						case L'"':
								*pstr++ = L'"';
								++p;
								break;
						case L'\'':
								*pstr++ = L'\'';
								++p;
								break;
						case L'u':
						{
								wchar_t c = 0;
								size_t i;
								++p;

								for(i = 0; i < 4 && (p + i) < e && p[i] != L'\0'; ++i)
								{
										c *= 16;

										if(p[i] >= L'0' && p[i] <= L'9')
										{
												c += (p[i] - L'0');
										}else if(p[i] >= L'a' && p[i] <= L'f')
										{
												c += (10 + p[i] - L'a');

										}else if(p[i] >= L'A' && p[i] <= L'F')
										{
												c += (10 + p[i] - L'A');
										}else
										{
												AR_error(AR_ERR_WARNING, L"invalid unicode val : %ls\r\n", p);
												AR_DEL(str);
												str = NULL;
												return NULL;
										}
								}

								p += i;

								*pstr++ = c;
						}
								break;
						default:
								AR_error(AR_ERR_WARNING, L"invalid escape char : %c\r\n", *p);
								break;
						}
				}else
				{
						*pstr++ = *p++;
				}
		}
		*pstr = L'\0';

		obj = Json_CreateObject(JSON_TYPE_STRING_T);

		if(obj == NULL)
		{
				AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
				AR_DEL(str);
				str = NULL;
				return NULL;
		}

		if(Json_SetStringToObject(obj, str) != AR_S_YES)
		{
				Json_DestroyObject(obj);
				obj = NULL;
				AR_DEL(str);
				str = NULL;
				return NULL;
		}
		
		AR_DEL(str);
		str = NULL;

		return obj;

}


jsonObj_t* parse_json_float_string(const wchar_t *lexer_str, size_t count)
{
		double flt;
		AR_ASSERT(lexer_str != NULL);

		if(AR_wtod_s(lexer_str, lexer_str + count, &flt) == NULL)
		{
				AR_error(AR_ERR_WARNING, L"invalid float string : %ls\r\n", lexer_str);
				return NULL;
		}else
		{
				jsonObj_t *obj = Json_CreateObject(JSON_TYPE_FLOAT_T);
				if(obj == NULL)
				{
						AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
						return NULL;
				}

				Json_SetFloatToObject(obj, flt);
				return obj;
		}
}



jsonObj_t* parse_json_integer_string(const wchar_t *lexer_str, size_t count)
{
		jsonObj_t *obj;
		AR_ASSERT(lexer_str != NULL);

		obj = Json_CreateObject(JSON_TYPE_INT_T);

		if(obj == NULL)
		{
				AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
				return NULL;
		}

		if(lexer_str[0] == L'-')
		{
				obj->integer.is_signed = true;
				if(AR_wtoi64_s(lexer_str, lexer_str + count, &obj->integer.n, 0) == NULL)
				{
						Json_DestroyObject(obj);
						obj = NULL;
						return NULL;

				}
		}else
		{
				obj->integer.is_signed = false;
				if(AR_wtou64_s(lexer_str, lexer_str + count, &obj->integer.un, 0) == NULL)
				{
						Json_DestroyObject(obj);
						obj = NULL;
						return NULL;
				}
		}

		return obj;
}







/************************************************************parser***************************************************************/



#include "json_parser_impl.h"

static arSpinLock_t			__g_lock;
static lex_t				*__g_lex = NULL;
static psrGrammar_t			*__g_grammar = NULL;
static const parser_t		*__g_parser = NULL;


static lexMatch_t*		__create_lex_match()
{
		lexMatch_t		*match;

		AR_LockSpinLock(&__g_lock);

		if(__g_lex == NULL)
		{
				__g_lex = __build_lex();
				if(__g_lex == NULL)
				{
						AR_error(AR_ERR_FATAL, L"grammar config : failed to initialize lexer\r\n");
				}
		}


		match = Lex_CreateMatch(__g_lex);
		AR_UnLockSpinLock(&__g_lock);

		return match;
}

static void				__destroy_lex_match(lexMatch_t *match)
{
		AR_ASSERT(match != NULL);
		Lex_DestroyMatch(match);
}



static const parser_t*		__build_parser(const psrGrammar_t *gmr)
{
		const parser_t *parser;
		AR_ASSERT(gmr && Parser_CheckIsValidGrammar(gmr) == AR_S_YES);

		parser = Parser_CreateParser(gmr, PARSER_SLR);
		AR_ASSERT(parser && Parser_CountParserConflict(parser) == 0);
		return parser;
}




static psrContext_t*	__create_parser_context(void *ctx)
{
		psrContext_t	*parser_context = NULL;
		AR_LockSpinLock(&__g_lock);


		if(__g_grammar == NULL)
		{
				psrHandler_t	psr_handler;
				psr_handler.error_f = on_error;
				psr_handler.free_f = on_free_node;

				__g_grammar = __build_grammar(&psr_handler);

				if(__g_grammar == NULL)
				{
						AR_error(AR_ERR_FATAL, L"grammar config : failed to initialize grammar\r\n");
				}

		}

		if(__g_parser == NULL)
		{
				__g_parser = __build_parser(__g_grammar);
				if(__g_parser == NULL)
				{
						AR_error(AR_ERR_FATAL, L"grammar config : failed to initialize parser\r\n");
				}
		}

		parser_context = Parser_CreateContext(__g_parser, ctx);
		AR_UnLockSpinLock(&__g_lock);
		return parser_context;
}


static void				__destroy_parser_context(psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		Parser_DestroyContext(parser_context);
}




/***************************************************************************************************************************************/




arStatus_t		Json_LoadObjectFromString(const wchar_t *content, jsonObj_t **obj)
{
		arStatus_t status = AR_S_YES;
		

		psrContext_t	*parser_context;
		
		lexMatch_t		*match;

		lexToken_t		tok;
		psrToken_t		term;

		AR_ASSERT(content != NULL && obj != NULL);

		status = AR_S_YES;
		match = NULL;
		parser_context = NULL;



		match		   = __create_lex_match();
		if(match == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}

		
		
		parser_context = __create_parser_context(NULL);

		if(parser_context == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}
		
		Lex_ResetInput(match, content);


		while(status == AR_S_YES)
		{
				status = Lex_Match(match, &tok);

				if(status == AR_S_YES)
				{
						PARSER_TOTERMTOK(&tok, &term);

						status = Parser_AddToken(parser_context, &term);

						if(tok.value == 0)
						{
								break;
						}

						if(status != AR_S_YES)
						{
								
								goto END_POINT;
						}
						
				}else if(status == AR_S_NO)
				{
						size_t n;
						size_t	line, col;
						arString_t		*str;
						wchar_t			*tok = NULL;

						str = NULL;
						tok = NULL;

						n = AR_wcslen(Lex_GetNextInput(match));

						if(n > 20)
						{
								n = 20;
						}

						tok = AR_NEWARR(wchar_t, n + 1);
						if(tok)
						{
								AR_wcsncpy(tok, Lex_GetNextInput(match), n);
								tok[n] = L'\0';
						}

						str = AR_CreateString();

						if(tok == NULL || str == NULL)
						{
								status = AR_E_NOMEM;
								if(tok)
								{
										AR_DEL(tok);
										tok = NULL;
								}

								if(str)
								{
										AR_DestroyString(str);
										str = NULL;
								}

								goto END_POINT;
						}
						
						Lex_MatchGetCoordinate(match, NULL, &line, &col);

						if(AR_AppendFormatString(str, L"Invalid Token %ls...(%Id : %Id)\r\n", tok, line, col) != AR_S_YES)
						{
								AR_DEL(tok);
								tok = NULL;
								AR_DestroyString(str);
								str = NULL;

								status = AR_E_NOMEM;
								goto END_POINT;
						}


						AR_error(AR_ERR_WARNING, L"%ls\r\n", AR_CSTR(str));
						AR_DEL(tok);
						tok = NULL;

						AR_DestroyString(str);
						str = NULL;

						status = AR_E_INVAL;
						goto END_POINT;
				}else /*各种错误，例如AR_E_NOMEM*/
				{
						goto END_POINT;
				}

				
		}		

		if(status == AR_S_YES)
		{
				*obj = (jsonObj_t*)Parser_GetResult(parser_context);
				AR_ASSERT(Json_GetObjectType(*obj) == JSON_TYPE_ARRAY_T || Json_GetObjectType(*obj) == JSON_TYPE_DICT_T);

		}

END_POINT:
		if(parser_context)
		{
				__destroy_parser_context(parser_context);
				parser_context = NULL;
		}

		if(match)
		{
				__destroy_lex_match(match);
				match = NULL;
		}

		return status;
}


arStatus_t		Json_LoadObjectFromFile(const wchar_t *path, jsonObj_t **obj)
{
		arStatus_t status;
		arString_t *str;
		AR_ASSERT(path != NULL && obj != NULL);

		status = AR_S_YES;
		str = AR_CreateString();
		if(str == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}
		
		
		status = AR_LoadBomTextFile(path, NULL, str);


		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

		
		status = Json_LoadObjectFromString(AR_CSTR(str), obj);

		
		
		

END_POINT:
		if(str)
		{
				AR_DestroyString(str);
				str = NULL;
		}
		return status;

}

/******************************************************************generator*************************************/
static arStatus_t __json_object_to_string(const jsonObj_t *obj, arString_t *str, size_t indent);


static arStatus_t __json_string_to_string(const wchar_t *wcs, arString_t *str, size_t indent)
{
		const wchar_t *p;
		arStatus_t status;
		AR_ASSERT(wcs != NULL && str != NULL);

		AR_UNUSED(indent);

		status = AR_S_YES;
		p = wcs;

		status = AR_AppendString(str, L"\"");
		
		while(*p != L'\0' && status == AR_S_YES)
		{
				switch(*p)
				{
				case L'\\':
						status = AR_AppendString(str, L"\\\\");
						break;
				case L'\b':
						status = AR_AppendString(str, L"\\b");
						break;
				case L'\f':
						status = AR_AppendString(str, L"\\f");
						break;
				case L'\n':
						status = AR_AppendString(str, L"\\n");
						break;
				case L'\r':
						status = AR_AppendString(str, L"\\r");
						break;
				case L'\t':
						status = AR_AppendString(str, L"\\t");
						break;
				case L'"':
						status = AR_AppendString(str, L"\\\"");
						break;
				case L'\'':
						status = AR_AppendString(str, L"\\'");
						break;
				default:
				{
						if( *p < 128 && AR_isprint((char)*p))
						{
								status = AR_AppendCharToString(str, *p);
						}else
						{
								status = AR_AppendFormatString(str, L"\\u%0.4X", (ar_uint_32_t)*p);
						}
				}
						break;
				}

				++p;
		}

		if(status == AR_S_YES)
		{
				status = AR_AppendString(str, L"\"");
		}

		return status;
}


static arStatus_t __json_float_to_string(double flt, arString_t *str, size_t indent)
{
		AR_ASSERT(str != NULL);
		AR_UNUSED(indent);
		return AR_AppendFormatString(str, L" %g", flt);
}


static arStatus_t __json_uint64_to_string(ar_uint_64_t un, arString_t *str, size_t indent)
{
		AR_ASSERT(str != NULL);
		AR_UNUSED(indent);
		return AR_AppendFormatString(str, L" %qu", un);
}

static arStatus_t __json_int64_to_string(ar_int_64_t n, arString_t *str, size_t indent)
{
		AR_ASSERT(str != NULL);
		AR_UNUSED(indent);
		return AR_AppendFormatString(str, L" %qd", n);

}

static arStatus_t __json_boolean_to_string(ar_bool_t b, arString_t *str, size_t indent)
{
		AR_ASSERT(str != NULL);
		AR_UNUSED(indent);
		return AR_AppendString(str, b ? L"true" : L"false");
}

static arStatus_t __json_null_to_string(arString_t *str, size_t indent)
{
		AR_ASSERT(str != NULL);
		AR_UNUSED(indent);

		return AR_AppendString(str, L"null");
}



static arStatus_t __json_dict_to_string(const jsonDict_t *dict, arString_t *str, size_t indent)
{
		size_t i,k;
		AR_ASSERT(dict != NULL && str != NULL);

		AR_AppendString(str, L"\r\n");
		for(i = 0; i < indent; ++i)
		{
				AR_AppendString(str, L" ");
		}
		AR_AppendString(str, L"{\r\n");

		

		for(i = 0; i < dict->count; ++i)
		{
				const wchar_t *key = dict->pairs[i].key;
				const jsonObj_t *val = dict->pairs[i].val;
				AR_ASSERT(key != NULL);

				AR_AppendString(str, L"\r\n");
				for(k = 0; k < indent + 8; ++k)
				{
						AR_AppendString(str, L" ");
				}

				__json_string_to_string(key, str, indent);

				AR_AppendString(str, L" : ");

				if(val == NULL)
				{
						AR_AppendString(str, L"null");
				}else
				{
						__json_object_to_string(val, str, indent + 8);
				}

				if(i < dict->count - 1)
				{
						AR_AppendString(str, L",");
				}
		}

		if(dict->count > 0)
		{
				AR_AppendString(str, L"\r\n");
				for(i = 0; i < indent; ++i)
				{
						AR_AppendString(str, L" ");
				}
		}

		AR_AppendString(str, L"}");

		return AR_S_YES;
}


static arStatus_t __json_array_to_string(const jsonArray_t *arr, arString_t *str, size_t indent)
{
		size_t i;
		arStatus_t status;
		AR_ASSERT(arr != NULL && str != NULL);

		status = AR_S_YES;

		status = AR_AppendString(str, L"[ ");
		if(status != AR_S_YES)
		{
				return status;
		}

		for(i = 0; i < arr->count && status == AR_S_YES; ++i)
		{
				const jsonObj_t *obj = arr->lst[i];
				AR_ASSERT(obj != NULL);
				status = __json_object_to_string(obj, str, indent);
				if(status == AR_S_YES && i < arr->count - 1)
				{
						status = AR_AppendString(str, L", ");
				}
		}

		if(status != AR_S_YES)
		{
				return status;
		}

		status = AR_AppendString(str, L" ]");
		
		return status;

}



static arStatus_t __json_object_to_string(const jsonObj_t *obj, arString_t *str, size_t indent)
{
		arStatus_t status;
		AR_ASSERT(obj != NULL && str != NULL);

		status = AR_S_YES;

		switch(obj->type)
		{
		case JSON_TYPE_DICT_T:
				return __json_dict_to_string(&obj->dict, str, indent);
		case JSON_TYPE_ARRAY_T:
				return __json_array_to_string(&obj->array, str, indent);
		case JSON_TYPE_STRING_T:
				return __json_string_to_string(obj->str, str, indent);
		case JSON_TYPE_INT_T:
				if(obj->integer.is_signed)
				{
						return __json_int64_to_string(obj->integer.n, str, indent);
				}else
				{
						return __json_uint64_to_string(obj->integer.n, str, indent);
				}

		case JSON_TYPE_FLOAT_T:
				return __json_float_to_string(obj->float_num, str, indent);
		case JSON_TYPE_BOOL_T:
				return __json_boolean_to_string(obj->boolean, str, indent);
		case JSON_TYPE_NULL_T:
				return __json_null_to_string(str, indent);
		default:
				AR_ASSERT(false);
				return AR_E_INVAL;
				break;
		}
}

arStatus_t		Json_SaveObjectToString(const jsonObj_t *obj, arString_t *str)
{
		AR_ASSERT(obj != NULL && str != NULL);
		AR_ClearString(str);
		return __json_object_to_string(obj, str, 0);
}


arStatus_t		Json_SaveObjectToFile(const jsonObj_t *obj, const wchar_t *path)
{
		arString_t *str;
		arStatus_t status;
		AR_ASSERT(obj != NULL && path != NULL);

		status = AR_S_YES;
		str = AR_CreateString();

		if(str == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}

		status = Json_SaveObjectToString(obj, str);

		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

		
		status = AR_SaveBomTextFile(path, AR_TXT_BOM_UTF_8, AR_CSTR(str));


END_POINT:
		if(str)
		{
				AR_DestroyString(str);
				str = NULL;
		}
		return status;
}





arStatus_t		Json_Init()
{
		AR_InitSpinLock(&__g_lock);
		return AR_S_YES;
}


void			Json_UnInit()
{

		if(__g_grammar)
		{
				Parser_DestroyGrammar(__g_grammar);
				__g_grammar = NULL;
		}

		if(__g_parser)
		{
				Parser_DestroyParser(__g_parser);
				__g_parser = NULL;
		}

		if(__g_lex)
		{
				Lex_Destroy(__g_lex);
				__g_lex = NULL;
		}

		AR_InitSpinLock(&__g_lock);
		
}





AR_NAMESPACE_END
