

/*
header
*/
 
/*
 * MarshalDie
 * Copyright (c) 2012 by Solidus
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
include
*/
 


#include "generate_type.h"

 


/*
class_def
*/
 

std::vector<Type_t*>    			g_type_list;
std::vector<std::wstring>			g_head_code;
std::vector<std::wstring>			g_tail_code;
std::wstring					g_uni_name = L"uniType_t";

 

 
/******************************************************************************************/
 

 




typedef enum
{
	FIELD_T,
	TYPE_T,
	NAME_T,
	NUM_T,
	CODEBLOCK_T,
}astNodeType_t;

typedef struct node_tag
{
	astNodeType_t		t;
	size_t			line;
	union{
		Field_t	*field;
		Type_t		*type;
		wchar_t	name[256];
		uint_64_t	num;
		struct {
			bool_t		is_tail;
			wchar_t	*code;
		}code_block;
	};
}ast_node_t;


ast_node_t* create_anonymous_type()
{
	static uint_32_t	id = 0;
	ast_node_t	*node;
	static wchar_t	name[128];
	AR_swprintf(name, 128, L"anonymous_type_%u", id);
	id++;
	
	node = new ast_node_t;
	node->t = TYPE_T;
	node->type = new Type_t;
	node->type->name = name;
	node->type->type = CUSTOM_T;
	node->type->is_inner_type = false;
	node->type->is_anonymous_type = true;
	return node;
}


 

static const wchar_t *__g_lex_name[] = {
L"delim = [\\t\\r\\n ]+",
L"letter = [A-Z_a-z]",
L"digit = [0-9]",
L"number = 0|[1-9]{digit}*",
L"name = {letter}({letter}|{digit})*",
L"lexeme = {name}",
L"comment = /\\*([^\\*]|\\*+[^\\*/])*\\*+/",
L"comment_line = //[^\\n]*\\n",
L"skip_lexem = {delim}|{comment}|{comment_line}",
L"key_lookahead = {skip_lexem}+|\"{\""
};

#define __NAME_COUNT__ ((size_t)10)

static psrRetVal_t AR_STDCALL on_codeblock(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL on_number(const psrToken_t *tok,void *ctx);




enum{
TOK_CLODE_BLOCK = 258,
TOK_HEAD = 259,
TOK_TAIL = 260,
TOK_UNI = 261,
TOK_NAME = 262,
TOK_NUM = 263,
};




static struct {const wchar_t *name;
size_t tokval;
size_t lex_prec;
const wchar_t *regex;
bool_t skip;
psrTermFunc_t leaf;
}__g_term_pattern[] =  {
{NULL, 257,0, L"{skip_lexem}+", true, NULL},
{L"CODE_BLOCK", TOK_CLODE_BLOCK, 0, L"\\{:[^\\u0]*?:\\}", false, on_codeblock},
{L"HEAD", TOK_HEAD, 0, L"\"%head\"", false, NULL},
{L"TAIL", TOK_TAIL, 0, L"\"%tail\"", false, NULL},
{L"UNI", TOK_UNI, 0, L"\"%uni\"", false, NULL},
{L"LEXEME", TOK_NAME, 0, L"{lexeme}", false, on_codeblock},
{L"NUMBER", TOK_NUM, 0, L"{number}", false, on_number},
{L":", 264, 0, L"\":\"", false, NULL},
{L";", 265, 0, L"\";\"", false, NULL},
{L"{", 266, 0, L"\"{\"", false, NULL},
{L"}", 267, 0, L"\"}\"", false, NULL},
{L"[", 268, 0, L"\"[\"", false, NULL},
{L"]", 269, 0, L"\"]\"", false, NULL},
{L"CFG_LEXVAL_EOI", 0, 2, L"$", false, NULL}
};

#define __TERM_COUNT__ ((size_t)14)

static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {
NULL
};

#define __PREC_COUNT__ ((size_t)0)

/*program	:	item_list */
/*program	:	 */
static psrRetVal_t AR_STDCALL handle_program(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*item_list	:	item_list item */
/*item_list	:	item */
static psrRetVal_t AR_STDCALL handle_item_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*item	:	head_code */
/*item	:	tail_code */
/*item	:	uni_name */
/*item	:	named_type_def */
static psrRetVal_t AR_STDCALL handle_item(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*head_code	:	HEAD CODE_BLOCK ; */
/*tail_code	:	TAIL CODE_BLOCK ; */
static psrRetVal_t AR_STDCALL on_head_code_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*uni_name	:	UNI LEXEME ; */
static psrRetVal_t AR_STDCALL on_uni_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*named_type_def	:	LEXEME anonymous_type_def ; */
static psrRetVal_t AR_STDCALL on_named_type_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*anonymous_type_def	:	{ field_def_list } */
static psrRetVal_t AR_STDCALL auto_return_1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*field_def_list	:	field_def_list field_def */
static psrRetVal_t AR_STDCALL on_filed_def_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*field_def_list	:	field_def */
static psrRetVal_t AR_STDCALL on_new_field_def_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*field_def	:	field_name : LEXEME ; */
static psrRetVal_t AR_STDCALL on_named_field_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*field_def	:	field_name : LEXEME [ NUMBER ] ; */
static psrRetVal_t AR_STDCALL on_named_array_field(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*field_def	:	field_name : anonymous_type_def ; */
static psrRetVal_t AR_STDCALL on_named_nesting_field(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*field_def	:	field_name : anonymous_type_def [ NUMBER ] ; */
static psrRetVal_t AR_STDCALL on_named_nesting_field_array(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*field_name	:	LEXEME */
static psrRetVal_t AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);




static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"program  :  item_list ", NULL, handle_program, 0},
{L"program  :   ", NULL, handle_program, 0},
{L"item_list  :  item_list item ", NULL, handle_item_list, 0},
{L"item_list  :  item ", NULL, handle_item_list, 0},
{L"item  :  head_code ", NULL, handle_item, 0},
{L"item  :  tail_code ", NULL, handle_item, 0},
{L"item  :  uni_name ", NULL, handle_item, 0},
{L"item  :  named_type_def ", NULL, handle_item, 0},
{L"head_code  :  HEAD CODE_BLOCK ; ", NULL, on_head_code_def, 0},
{L"tail_code  :  TAIL CODE_BLOCK ; ", NULL, on_head_code_def, 0},
{L"uni_name  :  UNI LEXEME ; ", NULL, on_uni_name, 0},
{L"named_type_def  :  LEXEME anonymous_type_def ; ", NULL, on_named_type_def, 0},
{L"anonymous_type_def  :  { field_def_list } ", NULL, auto_return_1, 0},
{L"field_def_list  :  field_def_list field_def ", NULL, on_filed_def_list, 0},
{L"field_def_list  :  field_def ", NULL, on_new_field_def_list, 0},
{L"field_def  :  field_name : LEXEME ; ", NULL, on_named_field_name, 0},
{L"field_def  :  field_name : LEXEME [ NUMBER ] ; ", NULL, on_named_array_field, 0},
{L"field_def  :  field_name : anonymous_type_def ; ", NULL, on_named_nesting_field, 0},
{L"field_def  :  field_name : anonymous_type_def [ NUMBER ] ; ", NULL, on_named_nesting_field_array, 0},
{L"field_name  :  LEXEME ", NULL, auto_return_0, 0}
};

#define __RULE_COUNT__ ((size_t)20)
#define START_RULE L"program"


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





static psrRetVal_t AR_STDCALL on_codeblock(const psrToken_t *tok,void *ctx)
{
	 { 
						ast_node_t	*node = new ast_node_t;
						node->t = NAME_T;
						node->line = tok->line;
						if(tok->str_cnt >= 256)
						{
							wchar_t *tmp = AR_wcsndup(tok->str, tok->str_cnt);
							AR_error(AR_ERR_FATAL, L"invalid name length : '%ls' line : %Iu!", tmp, tok->line );
							AR_abort();
						}
						
						AR_wcsncpy(node->name, tok->str, tok->str_cnt);
						node->name[tok->str_cnt] = L'\0';
						psrRetVal_t ret = {AR_S_YES, (psrToken_t*)node}; 
						return ret;
				 }
}


static psrRetVal_t AR_STDCALL on_number(const psrToken_t *tok,void *ctx)
{
	 { 
						ast_node_t	*node = new ast_node_t;
						node->t = NUM_T;
						node->line = tok->line;
						if(AR_wtou64_s(tok->str, tok->str + tok->str_cnt, &node->num, 10) == NULL)
						{
							wchar_t *tmp = AR_wcsndup(tok->str, tok->str_cnt);
							AR_error(AR_ERR_FATAL, L"invalid number : '%ls' line : %Iu!", tmp, tok->line);
							AR_abort();
						}

						psrRetVal_t ret = {AR_S_YES, (psrToken_t*)node}; 
						return ret;

				 }
}


/*program	:	item_list */
/*program	:	 */
static psrRetVal_t AR_STDCALL handle_program(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*item_list	:	item_list item */
/*item_list	:	item */
static psrRetVal_t AR_STDCALL handle_item_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*item	:	head_code */
/*item	:	tail_code */
/*item	:	uni_name */
/*item	:	named_type_def */
static psrRetVal_t AR_STDCALL handle_item(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*head_code	:	HEAD CODE_BLOCK ; */
/*tail_code	:	TAIL CODE_BLOCK ; */
static psrRetVal_t AR_STDCALL on_head_code_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 	
				psrRetVal_t ret = {AR_S_YES, NULL};
				ast_node_t *n = (ast_node_t*)nodes[1];
				AR_ASSERT(n != NULL);
				if(n->code_block.code)
				{
					g_tail_code.push_back(n->code_block.code);
				}
				return ret;
			 }
}




/*uni_name	:	UNI LEXEME ; */
static psrRetVal_t AR_STDCALL on_uni_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t ret = {AR_S_YES, NULL};
				ast_node_t *n = (ast_node_t*)nodes[1];
				g_uni_name = n->name;
				return ret;
			 }
}




/*named_type_def	:	LEXEME anonymous_type_def ; */
static psrRetVal_t AR_STDCALL on_named_type_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t ret = {AR_S_YES, NULL};
				ast_node_t *type_name = (ast_node_t*)nodes[0];
				ast_node_t *type_node = (ast_node_t*)nodes[1];
				AR_ASSERT(type_name != NULL && type_name->t == NAME_T);
				AR_ASSERT(type_node != NULL && type_node->t == TYPE_T);
				type_node->type->name = type_name->name;
				type_node->type->is_anonymous_type = false;
				insert_type(type_node->type);
				return ret;

			 }
}




/*anonymous_type_def	:	{ field_def_list } */
static psrRetVal_t AR_STDCALL auto_return_1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t ret = {AR_S_YES, nodes[1]};
				return ret;

			 }
}




/*field_def_list	:	field_def_list field_def */
static psrRetVal_t AR_STDCALL on_filed_def_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t ret = {AR_S_YES, NULL};
				ast_node_t *type_node = (ast_node_t*)nodes[0];
				ast_node_t *new_field = (ast_node_t*)nodes[1];
				AR_ASSERT(type_node != NULL && new_field != NULL);
				AR_ASSERT(type_node->t == TYPE_T && new_field->t == FIELD_T);
				
				insert_field_to_type(type_node->type, new_field->field);
				ret.node = type_node;
				return ret;

			 }
}




/*field_def_list	:	field_def */
static psrRetVal_t AR_STDCALL on_new_field_def_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t ret = {AR_S_YES, NULL};
				ast_node_t *new_type = create_anonymous_type();
				ast_node_t *new_field = (ast_node_t*)nodes[0];
				AR_ASSERT(new_field != NULL);
				nodes[0] = NULL;
				insert_field_to_type(new_type->type, new_field->field);
				ret.node = (psrNode_t*)new_type;
				return ret;
			 }
}




/*field_def	:	field_name : LEXEME ; */
static psrRetVal_t AR_STDCALL on_named_field_name(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t	ret = {AR_S_YES, NULL};		/*搜索g_type_list,有返回Type节点， 否则报告错误程序终止*/
				ast_node_t *name = (ast_node_t*)nodes[0];
				ast_node_t *type_name = (ast_node_t*)nodes[2];
				AR_ASSERT(name->t == NAME_T && type_name->t == NAME_T);

				ast_node_t *field_node = new ast_node_t;
				field_node->t = FIELD_T;
				field_node->field = new Field_t;
				field_node->field->name = name->name;
				field_node->field->type = find_type(type_name->name);
				field_node->field->is_array = false;
				field_node->field->array_size = 0;
				if(field_node->field->type == NULL)
				{
					AR_error(AR_ERR_FATAL, L"invalid type name '%ls'\r\n", type_name->name);
					AR_abort();
				}
				ret.node = (psrNode_t*)field_node;
				return ret;
			 }
}




/*field_def	:	field_name : LEXEME [ NUMBER ] ; */
static psrRetVal_t AR_STDCALL on_named_array_field(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t	ret = {AR_S_YES, NULL};		/*搜索g_type_list,有返回Type节点， 否则报告错误程序终止*/
				ast_node_t *name = (ast_node_t*)nodes[0];
				ast_node_t *type_name = (ast_node_t*)nodes[2];
				ast_node_t *array_size = (ast_node_t*)nodes[4];
				AR_ASSERT(name != NULL && type_name != NULL && array_size != NULL);
				AR_ASSERT(name->t == NAME_T && type_name->t == NAME_T);
				
				ast_node_t *field_node = new ast_node_t;
				field_node->t = FIELD_T;
				field_node->field = new Field_t;
				field_node->field->name = name->name;
				field_node->field->type = find_type(type_name->name);

				if(field_node->field->type == NULL)
				{
					AR_error(AR_ERR_FATAL, L"invalid type name '%ls'\r\n", type_name->name);
					AR_abort();
				}


				field_node->field->is_array = true;
				field_node->field->array_size = array_size->num;

				if(field_node->field->array_size == 0)
				{
					AR_error(AR_ERR_FATAL, L"invalid array size '%ls' : %u\r\n", type_name->name, field_node->field->array_size);
					AR_abort();
				}

				ret.node = (psrNode_t*)field_node;
				return ret;

			 }
}




/*field_def	:	field_name : anonymous_type_def ; */
static psrRetVal_t AR_STDCALL on_named_nesting_field(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t	ret = {AR_S_YES, NULL};		/*搜索g_type_list,有返回Type节点， 否则报告错误程序终止*/
				ast_node_t *name = (ast_node_t*)nodes[0];
				ast_node_t *type = (ast_node_t*)nodes[2];
				AR_ASSERT(name != NULL && type != NULL);
				AR_ASSERT(name->t == NAME_T && type->t == TYPE_T);

				insert_type(type->type);

				ast_node_t *field_node = new ast_node_t;
				field_node->t = FIELD_T;
				field_node->field = new Field_t;
				field_node->field->name = name->name;
				field_node->field->type = type->type;
				field_node->field->is_array = false;
				field_node->field->array_size = 0;
				type->type = NULL;
				ret.node = (psrNode_t*)field_node;
				return ret;
			 }
}




/*field_def	:	field_name : anonymous_type_def [ NUMBER ] ; */
static psrRetVal_t AR_STDCALL on_named_nesting_field_array(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t	ret = {AR_S_YES, NULL};		/*搜索g_type_list,有返回Type节点， 否则报告错误程序终止*/
				ast_node_t *name = (ast_node_t*)nodes[0];
				ast_node_t *type = (ast_node_t*)nodes[2];
				ast_node_t *array_size = (ast_node_t*)nodes[4];

				AR_ASSERT(name != NULL && type != NULL);
				AR_ASSERT(name->t == NAME_T && type->t == TYPE_T);

				insert_type(type->type);

				ast_node_t *field_node = new ast_node_t;
				field_node->t = FIELD_T;
				field_node->field = new Field_t;
				field_node->field->name = name->name;
				field_node->field->type = type->type;
				field_node->field->is_array = true;
				field_node->field->array_size = array_size->num;
				
				if(field_node->field->array_size == 0)
				{
					AR_error(AR_ERR_FATAL, L"invalid array size 'anonymous type' : %u\r\n", field_node->field->array_size);
					AR_abort();
				}


				type->type = NULL;
				ret.node = (psrNode_t*)field_node;
				return ret;
			 }
}




/*field_name	:	LEXEME */
static psrRetVal_t AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t ret = {AR_S_YES, nodes[0]};
		              nodes[0] = NULL;
		              return ret;
	              }
}






/*
main
*/
 




static arStatus_t		AR_STDCALL handle_on_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
{
		AR_ASSERT(tok != NULL);
		
		wchar_t msg[32];
		if(tok->str_cnt > 0)
		{
				AR_wcsncpy(msg, tok->str, AR_MIN(tok->str_cnt, 32));
		}else
		{
				AR_wcscpy(msg, L"EOI");
		}

		AR_error(AR_ERR_FATAL, L"invalid token : '%ls'\r\n", msg);
		AR_abort();
		return AR_S_NO;
}

static void		AR_STDCALL free_node(psrNode_t *node, void *ctx)
{

}

static const psrHandler_t		__g_handler = 
{
		handle_on_error,
		free_node
};





extern "C" void generate_type_list(const std::wstring &input)
{
		
		
		lex_t *lex = __build_lex();
		psrGrammar_t	*gmr = __build_grammar(&__g_handler);
		const parser_t		*parser = Parser_CreateParser(gmr, PARSER_LALR);
		psrContext_t *parser_ctx = Parser_CreateContext(parser, NULL);






		ARSpace::lexMatch_t *match = ARSpace::Lex_CreateMatch(lex);
		ARSpace::Lex_ResetInput(match, input.c_str());
		
		ARSpace::Lex_MatchClearFlags(match);
		
		ARSpace::lexToken_t		token;

		memset(&token, 0, sizeof(token));
		
		arStatus_t status = AR_S_YES;

		while(status == AR_S_YES)
		{

				status = ARSpace::Lex_Match(match, &token);

				if(status == AR_S_YES)
				{
						ARSpace::psrToken_t		psr_tok;

						PARSER_TOTERMTOK(&token, &psr_tok);

						status = ARSpace::Parser_AddToken(parser_ctx, &psr_tok);
						
						if(token.value == 0)
						{
								break;
						}

				}else if(status == AR_S_NO)
				{
						size_t len = wcslen(ARSpace::Lex_GetNextInput(match));
						if(len > 20) len = 20;

						size_t line;
						ARSpace::Lex_MatchGetCoordinate(match, NULL, &line, NULL);

						
						wchar_t msg[1024];
						AR_wcsncpy(msg, ARSpace::Lex_GetNextInput(match),  (int)len);
						AR_error(AR_ERR_FATAL, L"Input Error : %ls line : %Iu", msg, line);
						AR_abort();

				}else
				{
						AR_error(AR_ERR_FATAL, L"inner error !\r\n");
						AR_abort();
				}

		}

		Parser_DestroyContext(parser_ctx);
		parser_ctx = NULL;

		Parser_DestroyParser(parser);
		parser = NULL;

		Parser_DestroyGrammar(gmr);
		gmr = NULL;

		Lex_Destroy(lex);
		lex = NULL;

		Lex_DestroyMatch(match);
		match = NULL;

}





 



