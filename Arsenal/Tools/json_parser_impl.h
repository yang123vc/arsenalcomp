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


/*
syntax_tree_handler
*/


    static void        AR_STDCALL on_free_node(psrNode_t *node, void *ctx)
    {
			AR_UNUSED(ctx);
			AR_ASSERT(node != NULL);
			Json_DestroyObject((jsonObj_t*)node);
			node = NULL;

    }

    static arStatus_t        AR_STDCALL on_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
    {


			wchar_t *str;
			AR_ASSERT(tok != NULL);

			AR_UNUSED(expected);
			AR_UNUSED(count);
			AR_UNUSED(ctx);

			str = NULL;

			if(tok->str_cnt > 0)
			{
				str = AR_wcsndup(tok->str, tok->str_cnt);
			}

			if(str)
			{
					AR_error(AR_ERR_WARNING, L"invalid token : '%ls', (%u, %u)\r\n", str, (ar_uint_32_t)tok->line, (ar_uint_32_t)tok->col);
					AR_DEL(str);
					str = NULL;
			}

		return AR_S_YES;
    }

    static const psrHandler_t    __g_handler = {on_error, on_free_node};


static const wchar_t *__g_lex_name[] = {
L"delim = [\\x{000A}\\x{000B}\\x{000C}\\x{000D}\\x{0085}\\x{2028}\\x{2029}\\x{0020}\\f\\n\\r\\t\\v\\x{0009}\\x{0020}\\x{00A0}\\x{1680}\\x{180E}\\x{2000}-\\x{200A}\\x{202F}\\x{205F}\\x{3000}]",
L"comment = /\\*([^*]|\\*+[^*/])*\\*+/",
L"comment_line = (//[^\\x{000A}\\x{000B}\\x{000C}\\x{000D}\\x{0085}\\x{2028}\\x{2029}]*(\\x{000A}|\\x{000B}|\\x{000C}|\\x{000D}|\\x{0085}|\\x{2028}|\\x{2029}|$))",
L"skip_lexem = {delim}|{comment_line}|{comment}",
L"digit = [0-9]",
L"number = {digit}+",
L"hex_digit = [0-9a-fA-F]",
L"hex_literal = 0(x|X){hex_digit}+",
L"oct_literal = 0[0-7]+",
L"dec_literal = (0|[1-9][0-9]*)",
L"exponet = (e|E)(\\+|\\-)?[0-9]+",
L"float_literal = (((([0-9]\\.[0-9]*)){exponet}?)|([0-9]+{exponet}))",
/*L"escape_seq = (\\\\\\x22|\\\\\\\\)",*/
/*L"string_dq = (\\x22({escape_seq}|[^\\x22])*\\x22)",*/
L"string_dq = (\\x22([^\\x22\\\\]|\\\\.)*\\x22)",
L"float_constant = {float_literal}",
L"hex_constant = {hex_literal}",
L"oct_constant = {oct_literal}",
L"dec_constant = {dec_literal}"
};

#define __NAME_COUNT__ ((size_t)17)

static psrRetVal_t AR_STDCALL on_string_leaf_handler(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL default_float_leaf_handler(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL default_integer_leaf_handler(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL default_misc_leaf_handler(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL default_leaf_handler(const psrToken_t *tok,void *ctx);




enum{
TOK_DELIM_ID = 257,
TOK_STRING = 258,
TOK_FLOAT_NUMBER = 259,
TOK_INT_NUMBER = 260,
TOK_NULL = 261,
TOK_TRUE = 262,
TOK_FALSE = 263,
TOK_L_BRACES = 264,
TOK_R_BRACES = 265,
TOK_L_SQUARE = 266,
TOK_R_SQUARE = 267,
TOK_COMMA = 268,
TOK_COLON = 269,
};




static struct {const wchar_t *name;
size_t tokval;
size_t lex_prec;
const wchar_t *regex;
ar_bool_t skip;
psrTermFunc_t leaf;
}__g_term_pattern[] =  {
{NULL, TOK_DELIM_ID,1, L"{skip_lexem}", true, NULL},
{L"STRING", TOK_STRING, 0, L"{string_dq}", false, on_string_leaf_handler},
{L"FLOAT_NUMBER", TOK_FLOAT_NUMBER, 2, L"\\-?{float_constant}", false, default_float_leaf_handler},
{L"INT_NUMBER", TOK_INT_NUMBER, 2, L"\\-?({hex_constant}|{oct_constant}|{dec_constant})", false, default_integer_leaf_handler},
{L"null", TOK_NULL, 1, L"\"null\"", false, default_misc_leaf_handler},
{L"true", TOK_TRUE, 1, L"\"true\"", false, default_misc_leaf_handler},
{L"false", TOK_FALSE, 1, L"\"false\"", false, default_misc_leaf_handler},
{L"{", TOK_L_BRACES, 0, L"\"{\"", false, default_leaf_handler},
{L"}", TOK_R_BRACES, 0, L"\"}\"", false, default_leaf_handler},
{L"[", TOK_L_SQUARE, 0, L"\"[\"", false, default_leaf_handler},
{L"]", TOK_R_SQUARE, 0, L"\"]\"", false, default_leaf_handler},
{L",", TOK_COMMA, 0, L"\",\"", false, default_leaf_handler},
{L":", TOK_COLON, 0, L"\":\"", false, default_leaf_handler},
{L"CFG_LEXVAL_EOI", 0, 2, L"$", false, NULL}
};

#define __TERM_COUNT__ ((size_t)14)

static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =
{
    {
        NULL, 0, 0, PARSER_ASSOC_NONASSOC
    }

};

#define __PREC_COUNT__ ((size_t)0)

/*json	:	array */
/*json	:	object */
/*value	:	STRING */
/*value	:	INT_NUMBER */
/*value	:	FLOAT_NUMBER */
/*value	:	object */
/*value	:	array */
/*value	:	true */
/*value	:	false */
/*value	:	null */
static psrRetVal_t AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*optional_dot	:	, */
/*optional_dot	:	 */
static psrRetVal_t AR_STDCALL handle_optional_dot(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*object	:	{ } */
static psrRetVal_t AR_STDCALL on_object(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*object	:	{ members optional_dot } */
/*array	:	[ elements optional_dot ] */
static psrRetVal_t AR_STDCALL auto_return_1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*object	:	{ error } */
/*array	:	[ error ] */
static psrRetVal_t AR_STDCALL on_error_type(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*members	:	members , STRING : value */
static psrRetVal_t AR_STDCALL on_new_member(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*members	:	STRING : value */
static psrRetVal_t AR_STDCALL on_first_member(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*array	:	[ ] */
static psrRetVal_t AR_STDCALL on_array(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*elements	:	elements , value */
static psrRetVal_t AR_STDCALL on_new_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*elements	:	value */
static psrRetVal_t AR_STDCALL on_first_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);




static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"json  :  array ", NULL, auto_return_0, 0},
{L"json  :  object ", NULL, auto_return_0, 0},
{L"optional_dot  :  , ", NULL, handle_optional_dot, 0},
{L"optional_dot  :   ", NULL, handle_optional_dot, 0},
{L"object  :  { } ", NULL, on_object, 0},
{L"object  :  { members optional_dot } ", NULL, auto_return_1, 0},
{L"object  :  { error } ", NULL, on_error_type, 0},
{L"members  :  members , STRING : value ", NULL, on_new_member, 0},
{L"members  :  STRING : value ", NULL, on_first_member, 0},
{L"array  :  [ ] ", NULL, on_array, 0},
{L"array  :  [ elements optional_dot ] ", NULL, auto_return_1, 0},
{L"array  :  [ error ] ", NULL, on_error_type, 0},
{L"elements  :  elements , value ", NULL, on_new_element, 0},
{L"elements  :  value ", NULL, on_first_element, 0},
{L"value  :  STRING ", NULL, auto_return_0, 0},
{L"value  :  INT_NUMBER ", NULL, auto_return_0, 0},
{L"value  :  FLOAT_NUMBER ", NULL, auto_return_0, 0},
{L"value  :  object ", NULL, auto_return_0, 0},
{L"value  :  array ", NULL, auto_return_0, 0},
{L"value  :  true ", NULL, auto_return_0, 0},
{L"value  :  false ", NULL, auto_return_0, 0},
{L"value  :  null ", NULL, auto_return_0, 0}
};

#define __RULE_COUNT__ ((size_t)22)
#define START_RULE L"json"


static lex_t*	__build_lex()
{
		lex_t	*lex;
		size_t i;
		lex = Lex_Create();
		if(lex == NULL)
		{
				return NULL;
		}

		for(i = 0; i < __NAME_COUNT__; ++i)
		{
				arStatus_t status;
				status = Lex_Insert(lex, __g_lex_name[i]);
				if(status != AR_S_YES)
				{
					AR_error(AR_ERR_WARNING, L"failed to build lexer : name '%ls'!\r\n", __g_lex_name);
					Lex_Destroy(lex);
					AR_ASSERT(false);
					return NULL;
				}
		}

		for(i = 0; i < __TERM_COUNT__; ++i)
		{
				lexAction_t		act;
				arStatus_t status;
				act.is_skip		=		__g_term_pattern[i].skip;
				act.priority	=		__g_term_pattern[i].lex_prec;
				act.value		=		__g_term_pattern[i].tokval;


				status = Lex_InsertRule(lex, __g_term_pattern[i].regex, &act);
				if(status != AR_S_YES)
				{
					AR_error(AR_ERR_WARNING, L"failed to build lexer : regex '%ls'!\r\n", __g_term_pattern[i].regex);
					Lex_Destroy(lex);
					AR_ASSERT(false);
					return NULL;
				}
		}


		if(Lex_GenerateTransTable(lex) != AR_S_YES)
		{
				AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
		}
		return lex;
}


static psrGrammar_t*	__build_grammar(const psrHandler_t	*handler)
{
		psrGrammar_t	*grammar;
		size_t i;
		AR_ASSERT(handler != NULL);
		grammar = Parser_CreateGrammar(handler);
		if(grammar == NULL)
		{
				return NULL;
		}

		for(i = 0; i < __TERM_COUNT__; ++i)
		{
				arStatus_t status;
				if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)
				{
						continue;
				}

				status = Parser_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PARSER_ASSOC_NONASSOC,0, __g_term_pattern[i].leaf);

				if(status != AR_S_YES)
				{
						AR_error(AR_ERR_WARNING, L"failed to build grammar : term '%ls'!\r\n", __g_term_pattern[i].name);
						Parser_DestroyGrammar(grammar);
						grammar = NULL;
						AR_ASSERT(false);
						return NULL;
				}
		}

		for(i = 0; i < __PREC_COUNT__; ++i)
		{
				psrTermInfo_t	*info;
				info = Parser_GetTermSymbInfoByName(grammar, __g_prec_pattern[i].name);
				if(info == NULL)
				{
						arStatus_t status;
						status = Parser_InsertTerm(grammar, __g_prec_pattern[i].name, __g_prec_pattern[i].tokval, __g_prec_pattern[i].assoc, __g_prec_pattern[i].prec_level, NULL);

						if(status != AR_S_YES)
						{
								AR_error(AR_ERR_WARNING, L"failed to build grammar : prec '%ls'!\r\n", __g_term_pattern[i].name);
								Parser_DestroyGrammar(grammar);
								grammar = NULL;
								AR_ASSERT(false);
								return NULL;

						}
				}else
				{
						info->assoc = __g_prec_pattern[i].assoc;
						info->prec = __g_prec_pattern[i].prec_level;
				}
		}

		for(i = 0; i < __RULE_COUNT__; ++i)
		{
				arStatus_t status;
				status = Parser_InsertRuleByStr(grammar, __g_rule_pattern[i].rule, __g_rule_pattern[i].prec_token, __g_rule_pattern[i].handler, __g_rule_pattern[i].auto_ret);

				if(status != AR_S_YES)
				{
						AR_error(AR_ERR_WARNING, L"failed to build grammar : rule '%ls'!\r\n", __g_rule_pattern[i].rule);
						Parser_DestroyGrammar(grammar);
						grammar = NULL;
						AR_ASSERT(false);
						return NULL;
				}
		}


		if(Parser_SetStartRule(grammar,START_RULE) != AR_S_YES || Parser_CheckIsValidGrammar(grammar) != AR_S_YES)
		{
				AR_error(AR_ERR_WARNING, L"failed to build grammar!\r\n");
				Parser_DestroyGrammar(grammar);
				grammar = NULL;
				AR_ASSERT(false);
				return NULL;
		}

		return grammar;
}





static psrRetVal_t AR_STDCALL on_string_leaf_handler(const psrToken_t *tok,void *ctx)
{
	 {

		jsonObj_t *str_node;
		psrRetVal_t ret_val;

		AR_UNUSED(ctx);

		AR_ASSERT(tok != NULL && tok->term_val == TOK_STRING);
		str_node = parse_json_escape_string(tok->str + 1, tok->str_cnt - 2);

		if(str_node == NULL)
		{
			ret_val.status = AR_E_INVAL;
			ret_val.node = NULL;
		}else
		{
			ret_val.status = AR_S_YES;
			ret_val.node = (psrNode_t*)str_node;
		}
		return ret_val;
 }
}


static psrRetVal_t AR_STDCALL default_float_leaf_handler(const psrToken_t *tok,void *ctx)
{
	 {

		jsonObj_t *flt_node;
		psrRetVal_t ret_val;

		AR_UNUSED(ctx);

		AR_ASSERT(tok != NULL && tok->term_val == TOK_FLOAT_NUMBER);
		flt_node = parse_json_float_string(tok->str, tok->str_cnt);

		if(flt_node == NULL)
		{
			ret_val.status = AR_E_INVAL;
			ret_val.node = NULL;
		}else
		{
			ret_val.status = AR_S_YES;
			ret_val.node = (psrNode_t*)flt_node;
		}
		return ret_val;

 }
}


static psrRetVal_t AR_STDCALL default_integer_leaf_handler(const psrToken_t *tok,void *ctx)
{
	 {

		jsonObj_t *int_node;
		psrRetVal_t ret_val;

		AR_UNUSED(ctx);

		AR_ASSERT(tok != NULL && tok->term_val == TOK_INT_NUMBER);
		int_node = parse_json_integer_string(tok->str, tok->str_cnt);

		if(int_node == NULL)
		{
			ret_val.status = AR_E_INVAL;
			ret_val.node = NULL;
		}else
		{
			ret_val.status = AR_S_YES;
			ret_val.node = (psrNode_t*)int_node;
		}
		return  ret_val;


 }
}



static psrRetVal_t AR_STDCALL default_misc_leaf_handler(const psrToken_t *tok,void *ctx)
{
	 {

		jsonObj_t *node;
		psrRetVal_t ret_val;
		AR_UNUSED(ctx);
		AR_ASSERT(tok != NULL && (tok->term_val == TOK_NULL || tok->term_val == TOK_TRUE || tok->term_val == TOK_FALSE));


		if(tok->term_val == TOK_NULL)
		{
			node = Json_CreateObject(JSON_TYPE_NULL_T);
		}else if(tok->term_val == TOK_TRUE)
		{
			node = Json_CreateObject(JSON_TYPE_BOOL_T);

			if(node)
			{
				Json_SetBooleanToObject(node, true);
			}

		}else if(tok->term_val == TOK_FALSE)
		{

			node = Json_CreateObject(JSON_TYPE_BOOL_T);

			if(node)
			{
				Json_SetBooleanToObject(node, false);
			}

		}else
		{
			AR_ASSERT(false);
			ret_val.status = AR_E_INVAL;
			node = NULL;
		}

		if(node == NULL)
		{
			ret_val.status = AR_E_NOMEM;
			ret_val.node = NULL;
		}else
		{
			ret_val.status = AR_S_YES;
			ret_val.node = (psrNode_t*)node;
		}

		return ret_val;


 }
}


static psrRetVal_t AR_STDCALL default_leaf_handler(const psrToken_t *tok,void *ctx)
{
	 {

		psrRetVal_t ret_val;

		AR_UNUSED(ctx);
		AR_UNUSED(tok);

		ret_val.status = AR_S_YES;
		ret_val.node = NULL;
		return ret_val;
 }
}


/*json	:	array */
/*json	:	object */
/*value	:	STRING */
/*value	:	INT_NUMBER */
/*value	:	FLOAT_NUMBER */
/*value	:	object */
/*value	:	array */
/*value	:	true */
/*value	:	false */
/*value	:	null */
static psrRetVal_t AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{
		  psrRetVal_t ret;

		  AR_UNUSED(ctx);
		  AR_UNUSED(name);
		  AR_UNUSED(count);

		  ret.status = AR_S_YES;
		  ret.node = nodes[0];


		  nodes[0] = NULL;
          return ret;
        }
}




/*optional_dot	:	, */
/*optional_dot	:	 */
static psrRetVal_t AR_STDCALL handle_optional_dot(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{

		psrRetVal_t ret = {AR_S_YES, NULL};
		AR_UNUSED(ctx);
		AR_UNUSED(name);
		AR_UNUSED(count);
		AR_UNUSED(nodes);
		return ret;

}




/*object	:	{ } */
static psrRetVal_t AR_STDCALL on_object(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{

		jsonObj_t *node;
		psrRetVal_t ret;

		AR_UNUSED(ctx);
		AR_UNUSED(name);
		AR_UNUSED(count);
		AR_UNUSED(nodes);

		node = Json_CreateObject(JSON_TYPE_DICT_T);
		if(node == NULL)
		{
			AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
			ret.status = AR_E_NOMEM;
			ret.node = NULL;
		}else
		{
			ret.status = AR_S_YES;
			ret.node = (psrNode_t*)node;
		}
		return ret;
	 }
}




/*object	:	{ members optional_dot } */
/*array	:	[ elements optional_dot ] */
static psrRetVal_t AR_STDCALL auto_return_1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{

		 psrRetVal_t ret;

		 AR_UNUSED(ctx);
		AR_UNUSED(name);
		AR_UNUSED(count);
		ret.status = AR_S_YES;
		ret.node = nodes[1];

		 nodes[1] = NULL;
               return ret;
	 }
}




/*object	:	{ error } */
/*array	:	[ error ] */
static psrRetVal_t AR_STDCALL on_error_type(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{
		psrRetVal_t ret;

		AR_UNUSED(ctx);
		AR_UNUSED(name);
		AR_UNUSED(count);
		AR_UNUSED(nodes);

		ret.status = AR_E_INVAL;
		ret.node = NULL;
		return ret;
	 }
}





/*members	:	members , STRING : value */
static psrRetVal_t AR_STDCALL on_new_member(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{
		psrRetVal_t ret;
		jsonObj_t  *dict, *key, *val;
		const wchar_t *key_wcs = NULL;
		jsonObj_t **ns = (jsonObj_t**)nodes;

		AR_UNUSED(ctx);
		AR_UNUSED(name);
		AR_UNUSED(count);

		dict = ns[0];
		key = ns[2];
		val = ns[4];
		AR_ASSERT(dict != NULL && key != NULL && val != NULL);
		key_wcs = Json_GetWcsFromObject(key);

		if(key_wcs == NULL)
		{
			ret.status = AR_E_INVAL;
			ret.node = NULL;
			return ret;
		}

		ret.status = Json_SetDictObject(dict, key_wcs, val, true);
		if(ret.status != AR_S_YES)
		{
			ret.node = NULL;
			return ret;
		}

		ns[0] = NULL;
		ns[4] = NULL;

		ret.status = AR_S_YES;
		ret.node = (psrNode_t*)dict;
		return ret;
	  }
}




/*members	:	STRING : value */
static psrRetVal_t AR_STDCALL on_first_member(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{

		jsonObj_t  *dict, *key, *val;
		psrRetVal_t ret;
		const wchar_t *key_wcs = NULL;
		jsonObj_t **ns = (jsonObj_t**)nodes;

		AR_UNUSED(ctx);
		AR_UNUSED(name);
		AR_UNUSED(count);

		key = ns[0];
		val = ns[2];
		AR_ASSERT(key != NULL && val != NULL);
		key_wcs = Json_GetWcsFromObject(key);


		dict = Json_CreateObject(JSON_TYPE_DICT_T);
		if(dict == NULL)
		{
			AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
			ret.status = AR_E_NOMEM;
			ret.node = NULL;
			return ret;
		}

		ret.status = Json_SetDictObject(dict, key_wcs, val, true);
		if(ret.status != AR_S_YES)
		{
			Json_DestroyObject(dict);
			dict = NULL;
			ret.node = NULL;
			return ret;
		}

		ns[2] = NULL;

		ret.status = AR_S_YES;
		ret.node = (psrNode_t*)dict;
		return ret;
	 }
}




/*array	:	[ ] */
static psrRetVal_t AR_STDCALL on_array(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{

		jsonObj_t *node;
		psrRetVal_t ret;

		AR_UNUSED(ctx);
		AR_UNUSED(name);
		AR_UNUSED(count);
		AR_UNUSED(nodes);

		node = Json_CreateObject(JSON_TYPE_ARRAY_T);
		if(node == NULL)
		{
			AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
			ret.status = AR_E_NOMEM;
			ret.node = NULL;
		}else
		{
			ret.status = AR_S_YES;
			ret.node = (psrNode_t*)node;
		}
		return ret;

	 }
}




/*elements	:	elements , value */
static psrRetVal_t AR_STDCALL on_new_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{
			psrRetVal_t ret;
			jsonObj_t  *array, *val;
			jsonObj_t **ns = (jsonObj_t**)nodes;

			AR_UNUSED(ctx);
			AR_UNUSED(name);
			AR_UNUSED(count);

			array= ns[0];
			val = ns[2];

			AR_ASSERT(array != NULL && val != NULL);

			ret.status = Json_AppendToArrayObject(array, val);
			if(ret.status != AR_S_YES)
			{
				ret.node = NULL;
				return ret;
			}
			ns[0] = NULL;
			ns[2] = NULL;
			ret.status = AR_S_YES;
			ret.node = (psrNode_t*)array;
			return ret;

		 }
}




/*elements	:	value */
static psrRetVal_t AR_STDCALL on_first_element(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{
			jsonObj_t  *array, *item;
			psrRetVal_t ret;

			AR_UNUSED(ctx);
			AR_UNUSED(name);
			AR_UNUSED(count);

			item = (jsonObj_t*)nodes[0];

			array = Json_CreateObject(JSON_TYPE_ARRAY_T);
			if(array == NULL)
			{
				AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
				ret.status = AR_E_NOMEM;
				ret.node = NULL;
				return ret;
			}

			ret.status = Json_AppendToArrayObject(array, item);
			if(ret.status != AR_S_YES)
			{
				Json_DestroyObject(array);
				array = NULL;
				ret.node = NULL;
				return ret;
			}

			ret.status = AR_S_YES;
			ret.node = (psrNode_t*)array;
			nodes[0] = NULL;
			return ret;
		 }
}







