

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
 

#include <vector>
#include <list>
#include <string>
#include <set>
#include <map>
#include "Arsenal.h"
using namespace ARSpace;

 


/*
class_def
*/
 

typedef enum
{
	 BOOL_T,
        BYET_T,

        INT8_T,
        UINTT8_T,
        
        INT16_T,
        UINTT16_T,
        
        INT32_T,
        UINTT32_T,
        
        INT64_T,
        UINTT64_T,

	 FLOAT_T,
        DOUBLE_T,
        
        CHAR_T,
        WCHAR_T,

        CUSTOM_T,
}FieldType_t;


const wchar_t* __g_inner_type[CUSTOM_T] = 
{
	 L"bool_t",
        L"byte_t",
        L"int_8_t",
        L"uint_8_t",
        L"int_16_t",
        L"uint_16_t",
        L"int_32_t",
        L"uint_32_t",
        L"int_64_t",
        L"uint_64_t",

	 L"float",
        L"double",

        L"char",
        L"wchar_t",
        
};


static bool_t is_inner_type(FieldType_t t)
{
        switch(t)
        {
		   case BOOL_T:
                case BYET_T:
                case INT8_T:
                case UINTT8_T:
                case INT16_T:
                case UINTT16_T:
                case INT32_T:
                case UINTT32_T:
                case INT64_T:
                case UINTT64_T:
		  case FLOAT_T:
		  case DOUBLE_T:
                case CHAR_T:
                case WCHAR_T:
                        return true;
                case CUSTOM_T:
                default:
                        return false;
        };
}




static bool_t is_inner_type_name(const wchar_t *name)
{
        for(size_t i = 0; i < CUSTOM_T; ++i)
        {
                if(AR_wcscmp(name, __g_inner_type[i]) == 0)
                {
                        return true;
                }
        }
        
        return false;
}

static const wchar_t* get_inner_type_name(FieldType_t t)
{
        AR_ASSERT(is_inner_type(t));
        return __g_inner_type[t];
}


typedef struct Type					Type_t;
typedef struct Field					Field_t;
typedef std::vector<Field_t*>    			FieldList;



typedef struct Field
{
	std::wstring		name;
	Type_t			*type;
	bool_t			is_array;
	size_t			array_size;
}Field_t;



typedef struct Type
{
        FieldType_t             type;
	 std::wstring             name;
        bool_t                  is_inner_type;

        
        
        FieldList		   fields;
}Type_t;


void insert_field_to_type(Type_t *type, Field_t *field)
{
	size_t i;

	AR_ASSERT(type != NULL && field != NULL);

	for(size_t i = 0; i < type->fields.size(); ++i)
	{
		if(type->fields[i]->name == field->name)
		{
			AR_error(AR_ERR_FATAL, L"duplicate field name '%ls'\r\n", field->name.c_str());
			AR_abort();
		}
	}

	if(is_inner_type_name(field->name.c_str()))
	{
		AR_error(AR_ERR_FATAL, L"invalid field name '%ls'\r\n", field->name.c_str());
		AR_abort();
	}
	
	type->fields.push_back(field);
	
}




std::vector<Type_t*>    			g_type_list;
std::vector<std::wstring>			g_head_code;
std::vector<std::wstring>			g_tail_code;



 

 
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
	
	return node;
}

Type_t*	find_type(const std::wstring &name)
{
		for(size_t i = 0; i < g_type_list.size(); ++i)
		{
				if(g_type_list[i]->name == name)
				{
						return g_type_list[i];
				}
		}
		return NULL;

}

void		insert_type(Type_t *type)
{
		AR_ASSERT(type != NULL);

		if(find_type(type->name) != NULL)
		{
				AR_error(AR_ERR_FATAL, L"duplicate type name '%ls'\r\n", type->name.c_str());
				AR_abort();
		}
		
		g_type_list.push_back(type);
}



void init_inner_type()
{
	for(size_t i = 0; i < CUSTOM_T; ++i)
	{
		Type_t *type = new Type_t;
		type->name = get_inner_type_name((FieldType_t)i);
		type->type = (FieldType_t)i;
		type->is_inner_type = true;
		insert_type(type);
	}
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
TOK_NAME = 261,
TOK_NUM = 262,
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
{L"LEXEME", TOK_NAME, 0, L"{lexeme}", false, on_codeblock},
{L"NUMBER", TOK_NUM, 0, L"{number}", false, on_number},
{L":", 263, 0, L"\":\"", false, NULL},
{L";", 264, 0, L"\";\"", false, NULL},
{L"{", 265, 0, L"\"{\"", false, NULL},
{L"}", 266, 0, L"\"}\"", false, NULL},
{L"[", 267, 0, L"\"[\"", false, NULL},
{L"]", 268, 0, L"\"]\"", false, NULL},
{L"CFG_LEXVAL_EOI", 0, 2, L"$", false, NULL}
};

#define __TERM_COUNT__ ((size_t)13)

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
/*item	:	named_type_def */
static psrRetVal_t AR_STDCALL handle_item(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*head_code	:	HEAD CODE_BLOCK */
/*tail_code	:	TAIL CODE_BLOCK */
static psrRetVal_t AR_STDCALL on_head_code_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

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

/*field_name	:	LEXEME */
static psrRetVal_t AR_STDCALL auto_return_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);




static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"program  :  item_list ", NULL, handle_program, 0},
{L"program  :   ", NULL, handle_program, 0},
{L"item_list  :  item_list item ", NULL, handle_item_list, 0},
{L"item_list  :  item ", NULL, handle_item_list, 0},
{L"item  :  head_code ", NULL, handle_item, 0},
{L"item  :  tail_code ", NULL, handle_item, 0},
{L"item  :  named_type_def ", NULL, handle_item, 0},
{L"head_code  :  HEAD CODE_BLOCK ", NULL, on_head_code_def, 0},
{L"tail_code  :  TAIL CODE_BLOCK ", NULL, on_head_code_def, 0},
{L"named_type_def  :  LEXEME anonymous_type_def ; ", NULL, on_named_type_def, 0},
{L"anonymous_type_def  :  { field_def_list } ", NULL, auto_return_1, 0},
{L"field_def_list  :  field_def_list field_def ", NULL, on_filed_def_list, 0},
{L"field_def_list  :  field_def ", NULL, on_new_field_def_list, 0},
{L"field_def  :  field_name : LEXEME ; ", NULL, on_named_field_name, 0},
{L"field_def  :  field_name : LEXEME [ NUMBER ] ; ", NULL, on_named_array_field, 0},
{L"field_def  :  field_name : anonymous_type_def ; ", NULL, on_named_nesting_field, 0},
{L"field_name  :  LEXEME ", NULL, auto_return_0, 0}
};

#define __RULE_COUNT__ ((size_t)17)
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
/*item	:	named_type_def */
static psrRetVal_t AR_STDCALL handle_item(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		psrRetVal_t ret = {AR_S_YES, NULL};
		return ret;

}




/*head_code	:	HEAD CODE_BLOCK */
/*tail_code	:	TAIL CODE_BLOCK */
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
		AR_ASSERT(ctx != NULL && tok != NULL);
		
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





void generate_type_list(const std::wstring &input)
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





static const std::wstring& generate_for_inner_type(Type_t *type)
{
		static const std::wstring ret = L"\r\n#include \"Arsenal.h\"\r\n\r\n\r\n#if defined(__cplusplus)\r\nusing namespace ARSpace;\r\n#endif\r\n\r\n/************************************************Byte*****************************************************/\r\n\r\n#define __CHECK_ARG(_obj, _fn)\tdo{ if(SN_GetObjectType((_obj)) != SN_DICT_T || AR_wcslen((_fn)) == 0) {ar_status = AR_E_INVAL; goto END_POINT; }}while(0)\r\n\r\n\r\nstatic arStatus_t\t\t__put_BYTE_T_to_dict(snObject_t *obj, const wchar_t *field_name, byte_t v)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\r\n\t\t__CHECK_ARG(obj,field_name);\r\n\t\t\r\n\t\tar_status = SN_InsertToDictObjectByWcsUInt(obj, field_name, (uint_64_t)v);\r\n\t\t\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__put_BYTE_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, const byte_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*data_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\tdata_obj = NULL;\r\n\t\t\r\n\t\t__CHECK_ARG(obj,field_name);\r\n\r\n\t\tdata_obj = SN_CreateObject(SN_STRING_T);\r\n\r\n\t\tif(data_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_SetStringObjectByData(data_obj, arr, arr_size);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, data_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tif(data_obj)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(data_obj);\r\n\t\t\t\t\t\tdata_obj = NULL;\r\n\t\t\t\t}\r\n\t\t}\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_BYTE_T_from_dict(snObject_t *obj, const wchar_t *field_name, byte_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*int_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\tint_obj = NULL;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tint_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\r\n\t\tif(int_obj == NULL || SN_GetObjectType(int_obj) != SN_INT_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\t\t\r\n\t\t*pv = (uint_8_t)SN_GetUIntObject(int_obj);\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\nstatic arStatus_t\t\t__get_BYTE_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, byte_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*data_obj;\r\n\t\tint_t\t\t\tret;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tdata_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\t\t\r\n\t\tif(data_obj == NULL || SN_GetObjectType(data_obj) != SN_STRING_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tret = SN_GetDataFromStringObject(data_obj, arr, arr_size);\r\n\r\n\t\tif(ret < 0 || (size_t)ret != arr_size)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_FAIL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n/**********************************************************bool_t***************************************************/\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_BOOL_T_to_dict(snObject_t *obj, const wchar_t *field_name, bool_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\treturn __put_BYTE_T_to_dict(obj, field_name, (byte_t)v);\r\n}\r\n\r\n\r\n/*这里可以优化，但是，何必呢~~~~*/\r\n\r\nstatic arStatus_t\t\t__put_BOOL_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, bool_t *arr, size_t arr_size)\r\n{\r\n\t\treturn __put_BYTE_T_array_to_dict(obj, field_name, (byte_t*)arr, arr_size);\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_BOOL_T_from_dict(snObject_t *obj, const wchar_t *field_name, bool_t *pv)\r\n{\r\n\t\tbyte_t v;\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\tar_status = __get_BYTE_T_from_dict(obj, field_name, &v);\r\n\t\tif(ar_status == AR_S_YES)\r\n\t\t{\r\n\t\t\t\t*pv = (bool_t)v ? true : false;\r\n\t\t}\r\n\t\treturn ar_status;\r\n\r\n}\r\n\r\n\r\n/*...，同上*/\r\nstatic arStatus_t\t\t__get_BOOL_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, bool_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)arr, arr_size);\r\n\r\n\t\tif(ar_status == AR_S_YES)\r\n\t\t{\r\n\t\t\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tarr[i] = arr[i] ? true : false;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n/**********************************************************char***************************************************/\r\n\r\nstatic arStatus_t\t\t__put_CHAR_T_to_dict(snObject_t *obj, const wchar_t *field_name, char v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\treturn __put_BYTE_T_to_dict(obj, field_name, (byte_t)v);\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__put_CHAR_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, const char *v_str)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t *str_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && v_str != NULL);\r\n\t\t\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\tstr_obj = NULL;\r\n\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tstr_obj = SN_CreateObject(SN_STRING_T);\r\n\r\n\t\tif(str_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_SetStringObjectByStr(str_obj, v_str);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, str_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tif(str_obj)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(str_obj);\r\n\t\t\t\t\t\tstr_obj = NULL;\r\n\t\t\t\t}\r\n\t\t}\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_CHAR_T_from_dict(snObject_t *obj, const wchar_t *field_name, char *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tbyte_t\t\t\tv;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tar_status = __get_BYTE_T_from_dict(obj, field_name, &v);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = (char)v;\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_CHAR_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, char *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*str_obj;\r\n\t\tint_t\t\t\tret;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tstr_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\t\t\r\n\t\tif(str_obj == NULL || SN_GetObjectType(str_obj) != SN_STRING_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\t\t\r\n\t\tret = SN_GetStrFromStringObject(str_obj, arr, arr_size);\r\n\r\n\t\tif(ret < 0)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_FAIL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tarr[ret] = '\\0';\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n/**********************************************************wchar_t***************************************************/\r\n\r\n\r\nstatic arStatus_t\t\t__put_WCHAR_T_to_dict(snObject_t *obj, const wchar_t *field_name, wchar_t v)\r\n{\r\n\t\tuint_32_t t;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\r\n\t\tt = (uint_32_t)v;\r\n\t\tt = AR_LTON_32(t);\r\n\t\treturn __put_BYTE_T_array_to_dict(obj, field_name, (const byte_t*)&t, sizeof(uint_32_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_WCHAR_T_from_dict(snObject_t *obj, const wchar_t *field_name, wchar_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tuint_32_t\t\tt;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)&t, sizeof(uint_32_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\tt = AR_NTOL_32(t);\r\n\t\t*pv = (wchar_t)t;\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_WCHAR_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, const wchar_t *v_str)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t *str_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && v_str != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\tstr_obj = NULL;\r\n\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tstr_obj = SN_CreateObject(SN_STRING_T);\r\n\r\n\t\tif(str_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_SetStringObjectByWcs(str_obj, v_str);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, str_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tif(str_obj)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(str_obj);\r\n\t\t\t\t\t\tstr_obj = NULL;\r\n\t\t\t\t}\r\n\t\t}\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_WCHAR_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, wchar_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*str_obj;\r\n\t\tint_t\t\t\tret;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tstr_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\t\t\r\n\t\tif(str_obj == NULL || SN_GetObjectType(str_obj) != SN_STRING_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tret = SN_GetWcsFromStringObject(str_obj, arr, arr_size);\r\n\r\n\t\tif(ret < 0)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_FAIL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tarr[ret] = L'\\0';\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n/************************************************Integer 8*****************************************************/\r\n\r\nstatic arStatus_t\t\t__put_INT8_T_to_dict(snObject_t *obj, const wchar_t *field_name, int_8_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\treturn __put_BYTE_T_to_dict(obj, field_name, (byte_t)v);\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_INT8_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, int_8_t *arr, size_t arr_size)\r\n{\r\n\t\treturn __put_BYTE_T_array_to_dict(obj, field_name, (byte_t*)arr, arr_size);\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_INT8_T_from_dict(snObject_t *obj, const wchar_t *field_name, int_8_t *pv)\r\n{\r\n\t\tbyte_t v;\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\tar_status = __get_BYTE_T_from_dict(obj, field_name, &v);\r\n\t\tif(ar_status == AR_S_YES)\r\n\t\t{\r\n\t\t\t\t*pv = (int_8_t)v;\r\n\t\t}\r\n\t\treturn ar_status;\r\n\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_INT8_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, int_8_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)arr, arr_size);\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_UINT8_T_to_dict(snObject_t *obj, const wchar_t *field_name, uint_8_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\treturn __put_BYTE_T_to_dict(obj, field_name, (byte_t)v);\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_UINT8_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, uint_8_t *arr, size_t arr_size)\r\n{\r\n\t\treturn __put_BYTE_T_array_to_dict(obj, field_name, (byte_t*)arr, arr_size);\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_UINT8_T_from_dict(snObject_t *obj, const wchar_t *field_name, uint_8_t *pv)\r\n{\r\n\t\tbyte_t v;\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\tar_status = __get_BYTE_T_from_dict(obj, field_name, &v);\r\n\t\tif(ar_status == AR_S_YES)\r\n\t\t{\r\n\t\t\t\t*pv = (uint_8_t)v;\r\n\t\t}\r\n\t\treturn ar_status;\r\n\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_UINT8_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, uint_8_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)arr, arr_size);\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n/************************************************Integer 16*****************************************************/\r\n\r\nstatic arStatus_t\t\t__put_INT16_T_to_dict(snObject_t *obj, const wchar_t *field_name, int_16_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_16(v);\r\n\t\treturn __put_BYTE_T_array_to_dict(obj, field_name, (const byte_t*)&v, sizeof(int_16_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_INT16_T_from_dict(snObject_t *obj, const wchar_t *field_name, int_16_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)pv, sizeof(int_16_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_16(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_INT16_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, int_16_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tint_16_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(int_16_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_16(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_BYTE_T_array_to_dict(obj, field_name, (byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_INT16_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, int_16_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_16(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_UINT16_T_to_dict(snObject_t *obj, const wchar_t *field_name, uint_16_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_U16(v);\r\n\t\treturn __put_BYTE_T_array_to_dict(obj, field_name, (const byte_t*)&v, sizeof(uint_16_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_UINT16_T_from_dict(snObject_t *obj, const wchar_t *field_name, uint_16_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)pv, sizeof(uint_16_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_U16(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_UINT16_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, uint_16_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tuint_16_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(uint_16_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_U16(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_BYTE_T_array_to_dict(obj, field_name, (byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_UINT16_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, uint_16_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_U16(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n/************************************************Integer 32*****************************************************/\r\n\r\nstatic arStatus_t\t\t__put_INT32_T_to_dict(snObject_t *obj, const wchar_t *field_name, int_32_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_32(v);\r\n\t\treturn __put_BYTE_T_array_to_dict(obj, field_name, (const byte_t*)&v, sizeof(int_32_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_INT32_T_from_dict(snObject_t *obj, const wchar_t *field_name, int_32_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)pv, sizeof(int_32_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_32(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_INT32_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, int_32_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tint_32_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(int_32_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_32(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_BYTE_T_array_to_dict(obj, field_name, (byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_INT32_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, int_32_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_32(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_UINT32_T_to_dict(snObject_t *obj, const wchar_t *field_name, uint_32_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_U32(v);\r\n\t\treturn __put_BYTE_T_array_to_dict(obj, field_name, (const byte_t*)&v, sizeof(uint_32_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_UINT32_T_from_dict(snObject_t *obj, const wchar_t *field_name, uint_32_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)pv, sizeof(uint_32_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_U32(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_UINT32_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, uint_32_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tuint_32_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(uint_32_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_U32(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_BYTE_T_array_to_dict(obj, field_name, (byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_UINT32_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, uint_32_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_U32(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n/************************************************Integer 64*****************************************************/\r\n\r\nstatic arStatus_t\t\t__put_INT64_T_to_dict(snObject_t *obj, const wchar_t *field_name, int_64_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_64(v);\r\n\t\treturn __put_BYTE_T_array_to_dict(obj, field_name, (const byte_t*)&v, sizeof(int_64_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_INT64_T_from_dict(snObject_t *obj, const wchar_t *field_name, int_64_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)pv, sizeof(int_64_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_64(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_INT64_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, int_64_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tint_64_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(int_64_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_64(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_BYTE_T_array_to_dict(obj, field_name, (byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_INT64_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, int_64_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_64(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_UINT64_T_to_dict(snObject_t *obj, const wchar_t *field_name, uint_64_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_U64(v);\r\n\t\treturn __put_BYTE_T_array_to_dict(obj, field_name, (const byte_t*)&v, sizeof(uint_64_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_UINT64_T_from_dict(snObject_t *obj, const wchar_t *field_name, uint_64_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)pv, sizeof(uint_64_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_U64(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_UINT64_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, uint_64_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tuint_64_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(uint_64_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_U64(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_BYTE_T_array_to_dict(obj, field_name, (byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_UINT64_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, uint_64_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\t\tar_status = __get_BYTE_T_array_from_dict(obj, field_name, (byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_U64(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n/******************************************************float****************************************************************/\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_FLOAT_T_to_dict(snObject_t *obj, const wchar_t *field_name, float v)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsFloat(obj, field_name, (double)v);\r\n\r\nEND_POINT:\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\nstatic arStatus_t\t\t__get_FLOAT_T_from_dict(snObject_t *obj, const wchar_t *field_name, float *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*flt_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tflt_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\r\n\t\tif(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\t*pv = (float)SN_GetFloatObject(flt_obj);\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_FLOAT_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, float *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*float_arr_obj;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tAR_ASSERT(arr_size > 0);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\tfloat_arr_obj = NULL;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\r\n\t\tfloat_arr_obj = SN_CreateObject(SN_LIST_T);\r\n\t\tif(float_arr_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tar_status = SN_InsertToListObjectByFloat(float_arr_obj, (double)arr[i]);\r\n\t\t\t\t\r\n\t\t\t\tif(ar_status != AR_S_YES)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tgoto END_POINT;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, float_arr_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tif(float_arr_obj != NULL)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(float_arr_obj);\r\n\t\t\t\t\t\tfloat_arr_obj = NULL;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n\r\n}\r\n\r\nstatic arStatus_t\t\t__get_FLOAT_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, float *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*arr_list;\r\n\t\tsize_t\t\t\ti;\r\n\t\tsize_t\t\t\tlist_cnt;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tarr_list = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\t\t\r\n\t\tif(arr_list == NULL || SN_GetObjectType(arr_list) != SN_LIST_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tlist_cnt = SN_GetListObjectCount(arr_list);\r\n\r\n\t\tif(list_cnt != arr_size)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < SN_GetListObjectCount(arr_list); ++i)\r\n\t\t{\r\n\t\t\t\tsnObject_t *flt_obj = SN_GetFromListObject(arr_list, i);\r\n\t\t\t\tif(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\t\t\tgoto END_POINT;\r\n\t\t\t\t}\r\n\r\n\t\t\t\tarr[i] = (float)SN_GetFloatObject(flt_obj);\r\n\t\t}\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_DOUBLE_T_to_dict(snObject_t *obj, const wchar_t *field_name, double v)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsFloat(obj, field_name, v);\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_DOUBLE_T_array_to_dict(snObject_t *obj, const wchar_t *field_name, double *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*float_arr_obj;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\tfloat_arr_obj = NULL;\r\n\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tfloat_arr_obj = SN_CreateObject(SN_LIST_T);\r\n\t\tif(float_arr_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tar_status = SN_InsertToListObjectByFloat(float_arr_obj, arr[i]);\r\n\t\t\t\t\r\n\t\t\t\tif(ar_status != AR_S_YES)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tgoto END_POINT;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, float_arr_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tif(float_arr_obj != NULL)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(float_arr_obj);\r\n\t\t\t\t\t\tfloat_arr_obj = NULL;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_DOUBLE_T_from_dict(snObject_t *obj, const wchar_t *field_name, double *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*flt_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tflt_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\r\n\t\t\r\n\t\tif(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\t*pv = SN_GetFloatObject(flt_obj);\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_DOUBLE_T_array_from_dict(snObject_t *obj, const wchar_t *field_name, double *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*arr_list;\r\n\t\tsize_t\t\t\ti;\r\n\t\tsize_t\t\t\tlist_cnt;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\t\t\r\n\r\n\t\tar_status = AR_S_YES;\r\n\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tarr_list = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\t\t\r\n\t\tif(SN_GetObjectType(arr_list) != SN_LIST_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tlist_cnt = SN_GetListObjectCount(arr_list);\r\n\r\n\t\tif(list_cnt != arr_size)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < SN_GetListObjectCount(arr_list); ++i)\r\n\t\t{\r\n\t\t\t\tsnObject_t *flt_obj = SN_GetFromListObject(arr_list, i);\r\n\t\t\t\tif(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\t\t\tgoto END_POINT;\r\n\t\t\t\t}\r\n\r\n\t\t\t\tarr[i] = SN_GetFloatObject(flt_obj);\r\n\t\t}\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\n";
		return ret;
}

static void handle_types()
{
		for(size_t i = 0; i < g_type_list.size(); ++i)
		{
				Type_t *type = g_type_list[i];
				AR_ASSERT(type != NULL);
				AR_printf(L"type name : %ls\r\n", type->name.c_str());
				for(size_t k = 0; k < type->fields.size(); ++k)
				{
						Field_t *field = type->fields[k];
						if(field->is_array)
						{
								AR_printf(L"%ls    %ls[%Iu] ;\r\n", field->type->name.c_str(), field->name.c_str(), field->array_size);
						}else
						{
								AR_printf(L"%ls    %ls ;\r\n", field->type->name.c_str(), field->name.c_str());
						}

				}

		}
}


#define INPUT_PATH		L"D:\\Code\\Solidus\\Compiler\\Arsenal\\misc\\marshal_die_input.txt"

extern "C" void marshal_die_main()
{
		init_inner_type();
		arString_t *input = AR_CreateString();

		if(AR_LoadBomTextFile(INPUT_PATH, NULL, input) != AR_S_YES)
		{
				AR_error(AR_ERR_FATAL, L"Failed to load '%ls'\r\n", INPUT_PATH);
				AR_abort();
		}


		generate_type_list(AR_GetStringCString(input));

		handle_types();

		AR_DestroyString(input);

}




 



