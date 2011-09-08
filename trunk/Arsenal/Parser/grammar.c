/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#include "parser.h"
#include "parser_in.h"

AR_NAMESPACE_BEGIN




/*
void			Parser_InitTermInfoList(psrTermInfoList_t	*lst);
void			Parser_UnInitTermInfoList(psrTermInfoList_t	*lst);
void			Parser_ClearTermInfoList(psrTermInfoList_t	*lst);
psrTermInfo_t*	Parser_FindTermByName(psrTermInfoList_t	*lst, const wchar_t *name);
psrTermInfo_t*	Parser_FindTermByValue(psrTermInfoList_t	*lst, size_t val);
bool_t			Parser_InsertToTermInfoList(psrTermInfoList_t	*lst, const wchar_t *name, size_t val, psrAssocType_t assoc, size_t prec, psrTermFunc_t	leaf_f);
*/


void			Parser_InitTermInfoList(psrTermInfoList_t	*lst)
{
		AR_ASSERT(lst != NULL);
		AR_memset(lst, 0, sizeof(*lst));
		

}


void			Parser_UnInitTermInfoList(psrTermInfoList_t	*lst)
{
		AR_ASSERT(lst != NULL);
		Parser_ClearTermInfoList(lst);
		if(lst->lst)AR_DEL(lst->lst);
		AR_memset(lst, 0, sizeof(*lst));
}


void			Parser_ClearTermInfoList(psrTermInfoList_t	*lst)
{
		size_t i;
		AR_ASSERT(lst != NULL);

		for(i = 0; i < lst->count; ++i)
		{
				psrTermInfo_t	*info = &lst->lst[i];

				Parser_DestroySymb(info->term);
		}
		lst->count = 0;
}





psrTermInfo_t*	Parser_FindTermByName(psrTermInfoList_t	*lst, const wchar_t *name)
{
		size_t i;
		AR_ASSERT(lst != NULL && name != NULL);
		for(i = 0; i < lst->count; ++i)
		{
				psrTermInfo_t	*info = &lst->lst[i];
				/*注意，这里的参数name可能是任何非Parser_StringTable分配的内存，
				 *因此，这里必须用AR_wcscmp比较字符串而非类似Parser_CompSymb内部的比较str指针
				*/
				if(AR_wcscmp(info->term->name, name) == 0)return info;
		}
		return NULL;
}


psrTermInfo_t*	Parser_FindTermByValue(psrTermInfoList_t	*lst, size_t val)
{
		size_t i;
		AR_ASSERT(lst != NULL);
		for(i = 0; i < lst->count; ++i)
		{
				psrTermInfo_t	*info = &lst->lst[i];
				if(info->val == val)return info;
		}
		return NULL;
}




int_t			Parser_IndexOfTermInfoListByName(const psrTermInfoList_t	*lst, const wchar_t *name)
{

		size_t i;
		AR_ASSERT(lst != NULL && name != NULL);
		for(i = 0; i < lst->count; ++i)
		{
				psrTermInfo_t	*info = &lst->lst[i];
				/*注意，这里的参数name可能是任何非Parser_StringTable分配的内存，
				 *因此，这里必须用AR_wcscmp比较字符串而非类似Parser_CompSymb内部的比较str指针
				*/
				if(AR_wcscmp(info->term->name, name) == 0)return (int_t)i;
		}

		return -1;
}



int_t			Parser_IndexOfTermInfoListByValue(const psrTermInfoList_t	*lst, size_t val)
{

		size_t i;
		AR_ASSERT(lst != NULL);
		for(i = 0; i < lst->count; ++i)
		{
				psrTermInfo_t	*info = &lst->lst[i];
				if(info->val == val)return (int_t)i;
		}
		
		return -1;
}


psrTermInfo_t*	Parser_GetTermByIndex(psrTermInfoList_t	*lst, size_t index)
{
		AR_ASSERT(lst != NULL && index < lst->count);
		return &lst->lst[index];
}

bool_t			Parser_InsertToTermInfoList(psrTermInfoList_t	*lst, const wchar_t *name, size_t val, psrAssocType_t assoc, size_t prec, psrTermFunc_t	leaf_f)
{
		psrTermInfo_t *info;
		AR_ASSERT(lst != NULL && name != NULL && AR_wcslen(name) > 0);

		if(lst->count == lst->cap)
		{
				lst->cap = (lst->cap + 4)*2;
				lst->lst = AR_REALLOC(psrTermInfo_t, lst->lst, lst->cap);
		}
		info = &lst->lst[lst->count++];
		
		info->term = Parser_CreateSymb(name, PARSER_TERM);
		info->assoc = assoc;
		info->prec = prec;
		info->val = val;
		info->leaf_f = leaf_f;
		return true;
}

/*******************************************Rule******************************************************************/

/*


*/



psrRule_t* Parser_CreateRule(const psrSymb_t *head, const psrSymbList_t *body, const wchar_t *prec_tok, psrRuleFunc_t rule_f, size_t auto_ret, const psrTermInfoList_t *term_list, arString_t *err_msg)
{
		psrRule_t		*rule;
		size_t i;
		bool_t			inerr;
		const wchar_t*  right_term;
		AR_ASSERT(head != NULL && body != NULL && term_list != NULL);


		AR_ASSERT(auto_ret <= body->count);

		if(Parser_FindTermByName((psrTermInfoList_t*)term_list, head->name) != NULL)
		{
				if(err_msg)
				{
						AR_FormatString(err_msg,L"Grammar Error: Duplicate Rule name <%ls>!\r\n", head->name);
				}
				return NULL;
		}

		if(prec_tok != NULL && Parser_FindTermByName((psrTermInfoList_t*)term_list, prec_tok) == NULL)
		{
				/*AR_error(L"Grammar Error: Invalid prec token in <%ls>!\r\n", head->name);*/
				if(err_msg)
				{
						AR_FormatString(err_msg, L"Grammar Error: Invalid prec \"%ls\" token <%ls>\r\n", prec_tok, head->name);
				}
				return NULL;
		}
		
		right_term = NULL; 
		inerr = false;

		for(i = 0; i < body->count; ++i)
		{
				const psrSymb_t	*symb;

				symb = body->lst[i];

				if(body->lst[i]->type == PARSER_TERM)
				{
						if(Parser_FindTermByName((psrTermInfoList_t*)term_list, symb->name) == NULL)return NULL;
						
						if(Parser_CompSymb(symb, PARSER_ErrorSymb) == 0)
						{
								if(inerr)
								{
										if(err_msg)
										{
												AR_FormatString(err_msg, L"Grammar Error: Duplicate error definition in <%ls>!\r\n", head->name);
										}
										return NULL;
								}else
								{
										inerr = true;
								}
						}
						right_term = symb->name;
				}else
				{
						if(Parser_FindTermByName((psrTermInfoList_t*)term_list, symb->name) != NULL)return NULL;
				}
		}
		
		if(right_term == NULL)right_term = PARSER_DefPrecSymb->name;
		
		rule = AR_NEW0(psrRule_t);

		Parser_InitSymbList(&rule->body);
		
		rule->head = Parser_CopyNewSymb(head);
		
		for(i = 0; i < body->count; ++i)
		{
				Parser_InsertToSymbList(&rule->body, Parser_CopyNewSymb(body->lst[i]));
				
		}

		/*rule->prec_tok = prec_tok != NULL ? AR_wcsdup(prec_tok) : AR_wcsdup(right_term);*/
		
		rule->prec_tok = prec_tok != NULL ? Parser_AllocString(prec_tok) : Parser_AllocString(right_term);

		rule->rule_f = rule_f;
		rule->auto_ret = auto_ret;
		return rule;
}



/*****************************************************************************************************************************************/


psrRule_t* Parser_CreateRuleByStr(const wchar_t *str, const wchar_t *prec, psrRuleFunc_t rule_f, size_t auto_ret, const psrTermInfoList_t *term_list, arString_t *err_msg)
{
		const psrSymb_t *head;
		psrSymbList_t	body;
		psrRule_t		*res;
		const wchar_t	*p, *beg;
		size_t			count;
		AR_ASSERT(str != NULL && term_list != NULL);


		
		res = NULL;
		head = NULL; 
		Parser_InitSymbList(&body);
		p = AR_wcstrim_space(str);

		beg = p;

		if(!AR_iswalpha(*beg) && *beg != L'_')
		{
				goto END_POINT;
		}
		

		while(*p != L'\0' && (AR_iswalnum(*p) || *p == L'_'))p++;

		if(*p == L'\0' || p - beg == 0)
		{
				goto END_POINT;
		}else
		{
				
				wchar_t *name;
				count = p - beg;
				name = AR_wcsndup(beg, count);
				head = Parser_CreateSymb(name, PARSER_NONTERM);
				AR_DEL(name);
		}

		
		p = AR_wcstrim_space(p);

		if(*p != L':')
		{
				goto END_POINT;
		}

		++p;

		while(*p != L'\0')
		{
				if(AR_iswspace(*p))
				{
						++p;
				}else
				{
						beg = p;
						while(*p != L'\0' && !AR_iswspace(*p))
						{
								++p; 
						}
						
						if(*beg != L'\0')
						{
								wchar_t *name;
								const psrSymb_t *tmp;
								count = p - beg;
								name = AR_wcsndup(beg, count);

								if(Parser_FindTermByName((psrTermInfoList_t*)term_list, name) == NULL)
								{
										tmp = Parser_CreateSymb(name, PARSER_NONTERM);
								}else
								{
										tmp = Parser_CreateSymb(name, PARSER_TERM);
								}
								Parser_InsertToSymbList(&body, tmp);
								AR_DEL(name);
						}
				}
		}
		

		res =  Parser_CreateRule(head, &body, prec, rule_f, auto_ret, term_list, err_msg);

END_POINT:
		if(head)Parser_DestroySymb(head);
		
		{
				size_t i;
				
				for(i = 0; i < body.count; ++i)
				{
						Parser_DestroySymb(body.lst[i]);
				}
		}
		Parser_UnInitSymbList(&body);
		
		return res;
}




void			Parser_DestroyRule(psrRule_t *rule)
{
		size_t i;
		AR_ASSERT(rule != NULL);

		Parser_DestroySymb(rule->head);
		for(i = 0; i < rule->body.count; ++i)
		{
				Parser_DestroySymb(rule->body.lst[i]);
		}

		Parser_UnInitSymbList(&rule->body);
		/*if(rule->prec_tok) AR_DEL(rule->prec_tok);*/
		if(rule->prec_tok) rule->prec_tok = NULL;
		AR_DEL(rule);
}


/*
psrRule_t*		Parser_CopyNewRule(const psrRule_t *rule)
{
		psrRule_t *dest;
		size_t i;
		AR_ASSERT(rule != NULL);
		dest = AR_NEW0(psrRule_t);

		dest->head = Parser_CopyNewSymb(rule->head);
		Parser_InitSymbList(&dest->body);

		for(i = 0; i < rule->body.count; ++i)
		{
				Parser_InsertToSymbList(&dest->body, Parser_CopyNewSymb(rule->body.lst[i]));
		}

		dest->prec_tok = rule->prec_tok;
		dest->rule_f = rule->rule_f;
		return dest;

}

bool_t			Parser_IsEmptyRule(const psrRule_t *rule)
{
		AR_ASSERT(rule != NULL);

		return rule->body.count == 0;
}
*/

/******************************************Grammar************************************************************/

/******************************************************************************************************************/

static void __insert_rule(psrGrammar_t *grammar, psrRule_t *rule)
{
		if(grammar->count == grammar->cap)
		{
				grammar->cap = (grammar->cap + 4)*2;
				grammar->rules = AR_REALLOC(psrRule_t*, grammar->rules, grammar->cap);
		}
		grammar->rules[grammar->count++] = rule;
}

static void __init_grammar_component_unit(psrGrammar_t *grammar)
{
		AR_ASSERT(grammar != NULL);

		Parser_InsertToTermInfoList(&grammar->term_list, PARSER_EOISymb->name,PARSER_EOI_TOKVAL, PARSER_ASSOC_NONASSOC, 0, NULL);
		Parser_InsertToTermInfoList(&grammar->term_list, PARSER_ErrorSymb->name, PARSER_ERROR_TOKVAL, PARSER_ASSOC_NONASSOC, 0, NULL);
		Parser_InsertToTermInfoList(&grammar->term_list, PARSER_DefPrecSymb->name, PARSER_DEFPREC_TOKVAL, PARSER_ASSOC_NONASSOC, 0, NULL);

		{
				psrRule_t *start;
				psrSymbList_t	body;
				Parser_InitSymbList(&body);
				start = Parser_CreateRule(PARSER_StartSymb, &body, NULL, NULL, 0, &grammar->term_list, grammar->last_err_msg);
				AR_ASSERT(start != NULL);
				Parser_UnInitSymbList(&body);
				__insert_rule(grammar, start);
		}
}

/******************************************************************************************************************/

void					Parser_ResetGrammarParseHandler(psrGrammar_t *gmr, const psrHandler_t *handler)
{
		AR_ASSERT(gmr != NULL && handler != NULL);
		gmr->psr_handler = *handler;
}

psrGrammar_t*			Parser_CreateGrammar(const psrHandler_t *handler)
{
		psrGrammar_t* gmr;

		AR_ASSERT(handler != NULL && handler->free_f != NULL);
		gmr = AR_NEW0(psrGrammar_t);
	

		gmr->psr_handler = *handler;

		Parser_InitTermInfoList(&gmr->term_list);
		Parser_InitSymbList(&gmr->symb_list);
		
		__init_grammar_component_unit(gmr);

		gmr->last_err_msg = AR_CreateString();
		
		return gmr;
}

static void __clear_grammar(psrGrammar_t *grammar)
{
		size_t i;
		AR_ASSERT(grammar != NULL);
		
		for(i = 0; i < grammar->count; ++i)
		{
				Parser_DestroyRule(grammar->rules[i]);
				grammar->rules[i] = NULL;
		}
		
		grammar->count = 0;
		Parser_ClearTermInfoList(&grammar->term_list);
		Parser_ClearSymbList(&grammar->symb_list);
}


void	Parser_ClearGrammar(psrGrammar_t *grammar)
{
		
		AR_ASSERT(grammar != NULL);
		__clear_grammar(grammar);
		__init_grammar_component_unit(grammar);
}


const wchar_t*			Parser_GetGrammarLastError(const psrGrammar_t *grammar)
{
		AR_ASSERT(grammar != NULL);
		return AR_GetStringCString(grammar->last_err_msg);
}

void					Parser_ClearGrammarLastError(psrGrammar_t *grammar)
{
		AR_ASSERT(grammar != NULL);
		AR_ClearString(grammar->last_err_msg);
}


void					Parser_DestroyGrammar(psrGrammar_t *grammar)
{

		AR_ASSERT(grammar != NULL);

		AR_DestroyString(grammar->last_err_msg);

		/*Parser_ClearGrammar(grammar);*/
		__clear_grammar(grammar);
		
		Parser_UnInitTermInfoList(&grammar->term_list);
		Parser_UnInitSymbList(&grammar->symb_list);
		AR_DEL(grammar->rules);
		AR_DEL(grammar);
}



const psrHandler_t*		Parser_GetGrammarHandler(const psrGrammar_t *grammar)
{
		AR_ASSERT(grammar != NULL);
		return &grammar->psr_handler;
}

/*
const arIOCtx_t*		Parser_GetGrammarIOContext(const psrGrammar_t *grammar)
{
		AR_ASSERT(grammar != NULL);
		return &grammar->io_ctx;
}
*/


bool_t					Parser_InsertTerm(psrGrammar_t *grammar, const wchar_t *name, size_t val, psrAssocType_t assoc, size_t prec, psrTermFunc_t	leaf_f)
{
		AR_ASSERT(grammar != NULL && name != NULL && AR_wcslen(name) > 0);
		
		if(val < PARSER_MIN_TOKENVAL)
		{
				/*AR_error(AR_GRAMMAR, L"Grammar Error : invalid token value %d\r\n", val);*/
				/*AR_error(L"Grammar Error : invalid token value %" AR_PLAT_INT_FMT L"d\r\n", (size_t)val);*/
				AR_FormatString(grammar->last_err_msg, L"Grammar Error : invalid token value %Id\r\n", (size_t)val);
				return false;
		}
		
		if(AR_wcslen(name) == 0)
		{
				AR_FormatString(grammar->last_err_msg, L"Grammar Error : invalid token name '%ls'\r\n", name);
				return false;
		}

		
		if(Parser_FindTermByName(&grammar->term_list, name) != NULL)
		{
				AR_FormatString(grammar->last_err_msg, L"Grammar Error : duplicate name : \"%ls\" definition\r\n", name);
				return false;
		}

		if(Parser_FindTermByValue(&grammar->term_list, val) != NULL)
		{
				/*AR_error(AR_GRAMMAR, L"Grammar Error : duplicate token value : %d definition\r\n", val);*/
				/*AR_error(L"Grammar Error : duplicate token value : %" AR_PLAT_INT_FMT L"d definition\r\n", (size_t)val);*/
				AR_FormatString(grammar->last_err_msg, L"Grammar Error : duplicate token value : %Id definition\r\n", (size_t)val);
				return false;
		}


		if(Parser_GetSymbFromGrammarByName(grammar, name) != NULL)
		{
				AR_FormatString(grammar->last_err_msg, L"Grammar Error : duplicate symbol name : \"%ls\" definition\r\n", name);
				return false;
		}
		
		return Parser_InsertToTermInfoList(&grammar->term_list, name, val, assoc, prec, leaf_f);
}

bool_t					Parser_InsertRule(psrGrammar_t *grammar, psrRule_t *rule)
{
		size_t i;
		AR_ASSERT(grammar != NULL && rule != NULL);
		
		for(i = 0; i < grammar->count; ++i)
		{
				if(grammar->rules[i] == rule)return false;
		}

		if(Parser_CompSymb(rule->head, PARSER_StartSymb) == 0)/*rule-head不可为%Start保留符号*/
		{
				/*AR_error(L"Grammar Error : Non-term token %ls was reserved\r\n", rule->head->name);*/
				AR_FormatString(grammar->last_err_msg, L"Grammar Error : Non-term token %ls was reserved\r\n", rule->head->name);
				return false;
		}


		for(i = 0; i < rule->body.count; ++i)
		{
				const psrSymb_t *l = rule->body.lst[i];
				const psrSymb_t *r = Parser_GetSymbFromGrammarByName(grammar, l->name);

				if(r != NULL && Parser_CompSymb(l, r) != 0)
				{
						AR_FormatString(grammar->last_err_msg, L"Grammar Error : duplicate symbol name : \"%ls\" definition\r\n", l->name);
						return false;
				}
		}

		

		
		

		
		if(grammar->count == 1)
		{
				AR_ASSERT(grammar->rules[0]->body.count == 0);
				/*
				Parser_DestroySymb(grammar->rules[0]->body.lst[0]);
				grammar->rules[0]->body.lst[0] = Parser_CopyNewSymb(rule->head);
				*/
				Parser_InsertToSymbList(&grammar->rules[0]->body, Parser_CopyNewSymb(rule->head));
		}
		__insert_rule(grammar, rule);
		
		return true;
}

bool_t					Parser_InsertRuleBySymbList(psrGrammar_t *grammar, const psrSymb_t *head, const psrSymbList_t *body, const wchar_t *prec_tok, psrRuleFunc_t rule_f, size_t auto_ret)
{
		psrRule_t		*rule;
		AR_ASSERT(grammar != NULL && head != NULL && body != NULL);

		rule = Parser_CreateRule(head, body, prec_tok, rule_f, auto_ret, &grammar->term_list, grammar->last_err_msg);
		
		if(rule == NULL)return false;

		return Parser_InsertRule(grammar, rule);
}


bool_t					Parser_InsertRuleByStr(psrGrammar_t *grammar, const wchar_t *str, const wchar_t *prec, psrRuleFunc_t rule_f, size_t auto_ret)
{

		psrRule_t		*rule;
		AR_ASSERT(grammar != NULL && str != NULL);
		rule = Parser_CreateRuleByStr(str, prec, rule_f, auto_ret, &grammar->term_list, grammar->last_err_msg);
		if(rule == NULL)return false;
		return Parser_InsertRule(grammar, rule);
}







int_t					Parser_IndexOfGrammar(const psrGrammar_t *grammar, const psrRule_t *rule)
{
		size_t i;
		for(i = 0; i < grammar->count; ++i)
		{
				if(grammar->rules[i] == rule)return (int_t)i;
		}

		return -1;
}


const psrTermInfoList_t*		Parser_GetTermList(const psrGrammar_t *grammar)
{
		AR_ASSERT(grammar != NULL);
		return &grammar->term_list;
}


void							Parser_ResetTermSpecID(const psrGrammar_t *grammar)
{
		size_t i,k;
		const psrTermInfoList_t *term_lst;
		AR_ASSERT(grammar != NULL);

		term_lst = Parser_GetTermList(grammar);

		for(i = 0; i < term_lst->count; ++i)
		{
				psrTermInfo_t *term;

				term = &term_lst->lst[i];
				((psrSymb_t*)term->term)->spec_id = i;
		}



		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule = grammar->rules[i];
				
				for(k = 0; k < rule->body.count; ++k)
				{
						const psrSymb_t *symb = rule->body.lst[k];

						if(symb->type == PARSER_TERM && !Parser_IsBuildInSymbol(symb))
						{
								((psrSymb_t*)symb)->spec_id = Parser_IndexOfTermInfoListByName(term_lst, symb->name);
								AR_ASSERT(symb->spec_id != -1);
						}
				}
		}
}


int_t							Parser_GetTermSpecID(const psrGrammar_t *grammar, const psrSymb_t *symb)
{
		int_t idx;
		const psrTermInfoList_t *term_lst;
		AR_ASSERT(grammar != NULL && symb != NULL);

		term_lst = Parser_GetTermList(grammar);


		if(Parser_IsBuildInSymbol(symb))
		{
				idx = Parser_IndexOfTermInfoListByName(term_lst, symb->name);
		}else
		{
				idx = (int_t)symb->spec_id;
		}
		AR_ASSERT(idx != -1);
		return idx;

}

const psrSymbList_t* Parser_GetSymbList(const psrGrammar_t *grammar)
{
		psrSymbList_t *lst;
		AR_ASSERT(grammar != NULL);
		
		lst = (psrSymbList_t*)(&grammar->symb_list);

	/*	if(grammar->symb_list.count == 0)*/

		Parser_ClearSymbList(lst);
		
		{
				size_t i,k;
				/*先插入所有终结符*/
				for(i = 0; i < grammar->term_list.count; ++i)Parser_InsertToSymbList(lst, grammar->term_list.lst[i].term);
				
				/*插入每一条产生式的每一个符号，终结符或非终结符*/
				for(i = 0; i < grammar->count; ++i)
				{
						const psrRule_t	*rule = grammar->rules[i];
						
						if(Parser_FindFromSymbList(lst, rule->head) == -1)
						{
								Parser_InsertToSymbList(lst, rule->head);
						}
						
						for(k = 0; k < rule->body.count; ++k)
						{
								if(Parser_FindFromSymbList(lst, rule->body.lst[k]) == -1)
								{
										Parser_InsertToSymbList(lst, rule->body.lst[k]);
								}
						}
				}
		}

		return lst;
}

const psrSymb_t*		Parser_GetSymbFromGrammarByName(const psrGrammar_t *grammar, const wchar_t *name)
{
		
		size_t i,k;
		AR_ASSERT(grammar != NULL && name != NULL);
		
		
		for(i = 0; i < grammar->term_list.count; ++i)
		{
				const psrSymb_t *term = grammar->term_list.lst[i].term;
				if(AR_wcscmp(term->name, name) == 0)
				{
						return term;
				}
		}

		for(i = 0; i < grammar->count; ++i)
		{
				
				const psrRule_t	*rule = grammar->rules[i];
				const psrSymb_t *symb = rule->head;

				if(AR_wcscmp(symb->name, name) == 0)
				{
						return symb;
				}

				for(k = 0; k < rule->body.count; ++k)
				{
						symb = rule->body.lst[k];
						if(AR_wcscmp(symb->name, name) == 0)
						{
								return symb;
						}
				}
		}
		return NULL;
}





bool_t			Parser_CheckIsValidGrammar(const psrGrammar_t *grammar, arIOCtx_t *io_ctx)
{
		size_t i,j,k;
		bool_t result;
		const psrSymbList_t *lst;
		bool_t	*mark_tbl; 
		AR_ASSERT(grammar != NULL);
		
		if(grammar->count < 2)return false;/*Start和输入的第一个产生式一定>=2*/

		lst = Parser_GetSymbList(grammar);
		mark_tbl = AR_NEWARR0(bool_t, lst->count * lst->count);

		result = true;
		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule;
				rule = grammar->rules[i];

				for(j = 0; j < rule->body.count; ++j)
				{
						const psrSymb_t *symb;
						symb = rule->body.lst[j];

						if(symb->type == PARSER_NONTERM)
						{
								bool_t is_ok;
								is_ok = false;
								for(k = 0; !is_ok && k < grammar->count; ++k)
								{
										if(Parser_CompSymb(symb, grammar->rules[k]->head) == 0)
										{
												is_ok = true;
										}
								}

								if(!is_ok)
								{
										size_t h,s;
										h = (size_t)Parser_FindFromSymbList(lst, rule->head);
										s = (size_t)Parser_FindFromSymbList(lst, symb);

										if(!mark_tbl[AR_TBL_IDX_R(h,s,lst->count)])
										{
												if(io_ctx)
												{
														AR_printf_ctx(io_ctx, L"Grammar Error : The rule <%ls> not exist in this grammar <%ls>\r\n", symb->name, rule->head->name);
												}

												mark_tbl[AR_TBL_IDX_R(h,s,lst->count)] = true;
										}
										result = false;
								}
						}

				}
		}


		AR_DEL(mark_tbl);
		
		for(i = 0; i < lst->count; ++i)
		{
				const psrSymb_t *symb;
				bool_t is_ok;
				symb = lst->lst[i];
				is_ok = false;
				
				if(Parser_IsBuildInSymbol(symb) ||	symb->type == PARSER_TERM)continue;

				for(k = 0; !is_ok && k < grammar->count; ++k)
				{
						const psrRule_t *rule;
						rule = grammar->rules[k];

						if(Parser_CompSymb(rule->head, symb) == 0)continue;/*自己引用自己不算*/
						
						for(j = 0; j < rule->body.count; ++j)
						{
								if(Parser_CompSymb(rule->body.lst[j], symb) == 0)
								{
										is_ok = true;
										break;
								}
						}
				}

				if(!is_ok)
				{
						if(io_ctx)
						{
								AR_printf_ctx((arIOCtx_t*)io_ctx,L"Grammar Warning : The rule <%ls> is declared but never used\r\n", symb->name);
						}
				}
		}

		return result;
}

const psrRule_t*		Parser_GetStartRule(const psrGrammar_t *grammar)
{
		AR_ASSERT(grammar != NULL && grammar->count > 0);
		return grammar->rules[0];
}


bool_t					Parser_SetStartRule(psrGrammar_t *grammar, const wchar_t *rule_name)
{
		psrRule_t *start = grammar->rules[0];
		const psrSymb_t *lhs = NULL;
		size_t i;
		AR_ASSERT(grammar != NULL && grammar->count > 0 && rule_name != NULL);

		if(Parser_GetTermSymbInfoByName(grammar, rule_name) != NULL)
		{
				return false;
		}

		if(AR_wcscmp(PARSER_StartSymb->name, rule_name) == 0)return false;


		for(i = 0; i < grammar->count; ++i)
		{
				AR_ASSERT(rule_name != NULL && grammar->rules[i]->head->name);

				if(AR_wcscmp(rule_name, grammar->rules[i]->head->name) == 0)
				{
						lhs = Parser_CopyNewSymb(grammar->rules[i]->head);
						break;
				}
		}

		if(lhs == NULL)return false;

		AR_ASSERT(grammar->count > 1);
		AR_ASSERT(start != NULL && start->body.count > 0);

		Parser_DestroySymb(start->body.lst[0]);
		start->body.lst[0] = lhs;
		
		return true;
}

const psrTermInfo_t* Parser_GetRulePrecAssocInfo(const psrGrammar_t *grammar, const psrRule_t *rule)
{
		AR_ASSERT(grammar != NULL && rule != NULL && rule->prec_tok != NULL);
		return Parser_FindTermByName((psrTermInfoList_t*)&grammar->term_list, rule->prec_tok);

}

psrTermInfo_t*	Parser_GetTermSymbInfo(const psrGrammar_t	*grammar, const psrSymb_t *term)
{
		AR_ASSERT(grammar != NULL && term != NULL && term->type == PARSER_TERM);
		return  Parser_FindTermByName((psrTermInfoList_t*)&grammar->term_list, term->name);
		
}

psrTermInfo_t*			Parser_GetTermSymbInfoByName(const psrGrammar_t	*grammar, const wchar_t *name)
{
		AR_ASSERT(grammar != NULL && name != NULL);

		return  Parser_FindTermByName((psrTermInfoList_t*)&grammar->term_list, name);

}

psrTermInfo_t*			Parser_GetTermSymbInfoByValue(const psrGrammar_t	*grammar, size_t val)
{
		AR_ASSERT(grammar != NULL);
		return Parser_FindTermByValue((psrTermInfoList_t*)&grammar->term_list, val);
}





/************************************************************************************************************************************/



/*
firstset求法为，例如A->X(0)...X(n-1);
设First(Term) = {Term}; First(Epsilon) = {Epsilon}; i = 0;
如果First(X(i))存在{Epsilon}，则 First(A) += (First(X(i) - {Epsilon}); 如果i == n-1，则First(A) += {Epsilon}，本次循环终止；
如果不存在{Epsilon}，则循环终止；

因为每次都会处理至少一个符号，所以能确保循环终止，除非无任何符号加入到First集合中(changed == False);
*/
void					Parser_CalcFirstSet(const psrGrammar_t *grammar, psrSymbMap_t *first_set)
{
		size_t i;
		bool_t changed;
		const psrSymbList_t		*lst;
		const psrSymb_t			*key;
		AR_ASSERT(grammar != NULL && first_set != NULL && grammar->count > 1);
		
		lst = Parser_GetSymbList(grammar);

		for(i = 0; i < lst->count; ++i)
		{
				key = lst->lst[i];
				if(key->type == PARSER_NONTERM)
				{
						Parser_InsertToSymbMap(first_set, key, NULL);
				}else
				{
						Parser_InsertToSymbMap(first_set, key, key);
				}
		}
		
		/*changed = false; */
		
		/*首先计算哪个非终结符可以推导出空串Epsilon*/
		do{
				changed = false;

				for(i = 0; i < grammar->count; ++i)
				{
						psrMapRec_t *rec = NULL;
						size_t k = 0;
						psrRule_t *rule = grammar->rules[i];
						rec= Parser_GetSymbolFromSymbMap(first_set, rule->head);

						if(rec->can_empty)continue;
						
						for(k = 0; k < rule->body.count; ++k)
						{
								const psrSymb_t *symb = NULL;
								const psrMapRec_t *tmp_rec = NULL;
								symb = rule->body.lst[k];
								if(symb->type == PARSER_TERM)break;
								tmp_rec= Parser_GetSymbolFromSymbMap(first_set, symb);
								if(!tmp_rec->can_empty)break;
						}

						if(k == rule->body.count)/*如果产生式体中无任何符号或所有符号均可以推导出空串，则此产生式头被认为可推导出空*/
						{
								rec->can_empty = true;
								changed = true;
						}
				}

		}while(changed);


		do{
				const psrSymb_t *s1, *s2;
				changed = false;

				for(i = 0; i < grammar->count; ++i)
				{
						const psrRule_t *rule;
						psrMapRec_t *rec = NULL;
						size_t k;
						rule = grammar->rules[i];

						s1 = rule->head;
						rec= Parser_GetSymbolFromSymbMap(first_set, s1);


						for(k = 0; k < rule->body.count; ++k)
						{
								s2 = rule->body.lst[k];/*产生式体中第k个符号*/
								
								if(s2->type == PARSER_TERM)
								{
										/*当前产生式体中存在终结符，则将其加入后终止循环*/
										if(Parser_InsertToSymbList_Unique(&rec->lst, s2))
										{
												changed = true;
										}
										break;
								}else if(Parser_CompSymb(s1, s2) == 0)/*非终结符，且与产生式头相同的符号*/
								{
										if(!rec->can_empty)break;/*如果当前产生式头不为空，则终止循环*/
								}
								else
								{
										/*非终结符，且与产生式头不同*/
										const psrMapRec_t *rec2 = NULL;
										size_t x;
										rec2 = Parser_GetSymbolFromSymbMap(first_set, s2);
										/*将当前非终结符号的所有first符号加入到当前产生式头的first-set中*/
										for(x = 0; x < rec2->lst.count; ++x)
										{
												if(Parser_InsertToSymbList_Unique(&rec->lst, rec2->lst.lst[x]))
												{
														changed = true;
												}
										}
										/*如果当前符号不可导出空串则终止循环*/
										if(!rec2->can_empty)break;
								}

						}
				}
		}while(changed);

}



/*
FollowSet的求法为: 例如A->X(0)...X(n-1);
设i = 0->n-1, Follow(NULL)= {Epsilon};
如果X(i)为非终结符,则要对X(j)->X(n)循环检测,设j = i+1 && j < n;
如果First(X(j))不包含{Epsilon},则Follow(X(i)) += First(X(j)，循环结束，否则
j < n-1:Follow(X(i)) += (First(X(j)) - {Epsilon})，循环继续;
j == n-1:Follow(X(i)) += (First(X(j)) - {Epsilon})，循环结束，并将Follow(X(i) += Follow(A);
注意，当i == n-1时，Follow(X(i)) += Follow(A);,因为Follow(NULL)= {Epsilon};
*/


void					Parser_CalcFollowSet(const psrGrammar_t *grammar, psrSymbMap_t *follow_set, const psrSymbMap_t *first_set)
{
		size_t i;
		bool_t changed;
		const psrSymbList_t *lst;
		psrMapRec_t *rec1 = NULL, *rec2 = NULL;

		AR_ASSERT(grammar != NULL && grammar->count > 1 && follow_set != NULL && first_set != NULL);

		changed = true;
		lst = Parser_GetSymbList(grammar);
		for(i = 0; i < lst->count; i++)
		{
				const psrSymb_t *key = lst->lst[i];
				if(key->type == PARSER_NONTERM)
				{
						Parser_InsertToSymbMap(follow_set, key, NULL);

						rec1 = Parser_GetSymbolFromSymbMap(first_set, key);
						rec2 = Parser_GetSymbolFromSymbMap(follow_set, key);
						rec2->can_empty = rec1->can_empty;
				}
		}

		Parser_InsertToSymbMap(follow_set, PARSER_StartSymb, PARSER_EOISymb);
		
		do{
				changed = false;
				
				for(i = 0; i < grammar->count; ++i)/*对每一条产生式*/
				{
						const psrRule_t *rule;
						const psrMapRec_t *head_follow;
						size_t k;
						rule = grammar->rules[i];

						head_follow = Parser_GetSymbolFromSymbMap(follow_set,rule->head);/*grammar->head[i]所对应的follow集合*/
						
						for(k = 0; k < rule->body.count; ++k)
						{
								const psrSymb_t *key;
								size_t next_idx;
								key = rule->body.lst[k];
								/*终结符无follow-set*/
								if(key->type == PARSER_TERM)continue;

								next_idx = k + 1;
								
								for(next_idx = k + 1; next_idx < rule->body.count; ++next_idx)
								{
										const psrSymb_t *next = NULL;
										const psrMapRec_t *first_rec;
										psrMapRec_t *rec_tmp = NULL;
										size_t x;
										
										next = Parser_IndexOfSymbList(&rule->body, next_idx);/*next为key之后的符号*/
										first_rec = Parser_GetSymbolFromSymbMap(first_set, next);/*next的first-set*/
										rec_tmp  = Parser_GetSymbolFromSymbMap(follow_set, key);/*key的follow-set*/
										/*将key之后的符号next的first-set加入到key的follow-set中*/
										for(x = 0; first_rec && x < first_rec->lst.count; ++x)
										{
												const psrSymb_t *f_symb;
												f_symb = first_rec->lst.lst[x];
												AR_ASSERT(f_symb->type == PARSER_TERM);				/*first不可能为非终结符*/

												if(Parser_InsertToSymbList_Unique(&rec_tmp->lst, f_symb))
												{
														changed = true;	/*任何一步的改动都需要重新计算follow集合*/
												}
										}
										/*如果next不可导出空串，则循环终止*/
										if(!first_rec->can_empty)break;
								}
								
								if(next_idx == rule->body.count)/*如果key为当前产生式最后一个符号或key之后的所有符号都可导出空串*/
								{
										/*则将产生式头的follow-set加入到符号key的follow-set中*/
										size_t x;
										psrMapRec_t *rec_tmp = Parser_GetSymbolFromSymbMap(follow_set, key);
										for(x = 0; x < head_follow->lst.count; ++x)
										{
												if(Parser_InsertToSymbList_Unique(&rec_tmp->lst,head_follow->lst.lst[x]))
												{
														changed = true;	/*任何一步的改动都需要重新计算follow集合*/
												}
										}
								}
						}
				}
		}while(changed);
}




















#if(0)
/**********************************************Print*******************************************/


/*效率成为问题时我才会考虑改这没什么用的东西！*/
static bool_t __detect_left_recursion(const psrGrammar_t *grammar, const psrSymb_t *head, psrSymbList_t *lst, arString_t *output)
{
		size_t i;
		bool_t is_recu = false;
		AR_ASSERT(grammar != NULL && head != NULL && lst != NULL);

		/*AR_ASSERT(lst->count > 0);*/

		AR_ASSERT(Parser_FindFromSymbList(lst, head) == -1);
		
		Parser_InsertToSymbList(lst, head);
		
		
		
		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule;
				rule = grammar->rules[i];
				
				if(Parser_CompSymb(rule->head, head) == 0)
				{
						const psrSymb_t *symb;

						size_t x = 0;
						
						/*while(x < rule->body.count && Parser_CompSymb(rule->body.lst[x], Parser_EpsilonSymb) == 0)x++;*/
						
						if(x < rule->body.count && rule->body.lst[x]->type == PARSER_NONTERM)
						{
								symb = rule->body.lst[x];

								if(Parser_CompSymb(lst->lst[0], symb) == 0)
								{
										is_recu = true;
										if(output)
										{
												size_t cnt;
												AR_AppendFormatString(output, L"Path : ");

												for(cnt = 0; cnt < lst->count; ++cnt)
												{
														AR_AppendFormatString(output, L"<%ls> -> ", lst->lst[cnt]->name);
												}
												AR_AppendFormatString(output, L"<%ls> ", lst->lst[0]->name);
												AR_AppendFormatString(output, L"\r\n");
										}

								}else if(Parser_FindFromSymbList(lst, symb) != -1)
								{
										continue;
								}else
								{
										__detect_left_recursion(grammar, symb, lst,output);
								}
						}
				}
		}
		
		Parser_RemoveFromSymbListByIndex(lst, lst->count-1);
		return is_recu;
		
}


bool_t					Parser_ReportLeftRecursion(const psrGrammar_t *grammar, arString_t *output)
{
		size_t i;
		psrSymbList_t	lst;
		bool_t			ret = false;
		AR_ASSERT(grammar != NULL);
		if(output)AR_ClearString(output);
		Parser_InitSymbList(&lst);
		
		for(i = 0; i < grammar->symb_list.count; ++i)
		{
				const psrSymb_t *symb;
				
				symb = grammar->symb_list.lst[i];

				Parser_ClearSymbList(&lst);
				
				if(symb->type == PARSER_NONTERM)
				{
						if(__detect_left_recursion(grammar, symb, &lst,output))
						{
								ret = true;
						}
				}
		}

		Parser_UnInitSymbList(&lst);
		return ret;
}





static size_t __calc_leftfactor(const psrRule_t *l, const psrRule_t *r)
{
		size_t cnt = 0;
		AR_ASSERT(l != NULL && r != NULL);

		while(cnt < l->body.count && cnt < r->body.count && Parser_CompSymb(l->body.lst[cnt], r->body.lst[cnt]) == 0)
		{
				cnt++;
		}
		return cnt;
}



static bool_t	__report_rule_left_factor(const psrSymb_t *lhs, const psrRule_t **rules, size_t n, arString_t *output)
{
		size_t i,k;
		size_t	*bk;
		size_t			max,cnt;
		bool_t			has_left_factor = false;
		arString_t		*tmp;
		AR_ASSERT(lhs != NULL && rules != NULL);

		if(n < 2)return false;
		tmp = AR_CreateString();
		bk = AR_NEWARR0(size_t, n);

RECHECK_POINT:
		
		max = 0;
		cnt = 0;

		for(i = 0; i < n; ++i)
		{
				if(rules[i] == NULL)continue;
				AR_ASSERT(Parser_CompSymb(lhs, rules[i]->head) == 0);
				for(k = 0; k < n; ++k)
				{
						if(rules[k] == NULL)continue;

						if(k != i)
						{
								size_t x = __calc_leftfactor(rules[i], rules[k]);
								
								if(x > max)
								{
										max = x;
										AR_memset(bk, 0, sizeof(size_t)*n);
										cnt = 0;
										bk[cnt++] = i;
										bk[cnt++] = k;
								}else if(x > 0 && x == max && __calc_leftfactor(rules[bk[0]], rules[i]) == x)
								{
										size_t j;
										bool_t need_i = true, need_k = true;
										for(j = 0; j < n; ++j)
										{
												if(bk[j] == i)need_i = false;
												if(bk[j] == k)need_k = false;
										}
										
										if(need_i)bk[cnt++] = i;
										if(need_k)bk[cnt++] = k;
								}else
								{
								}
						}
				}
		}

		if(cnt == 0)
		{
				goto RETURN_POINT;
		}else
		{
				has_left_factor = true;
		}
		
		
		for(i = 0; i < cnt ; ++i)
		{
				if(output)
				{
						AR_AppendString(tmp, lhs->name);
						AR_AppendString(tmp, L"\t:\t");
						Parser_PrintSymbolList(&rules[bk[i]]->body, tmp);
						AR_AppendFormatString(tmp, L"\t:\t%d", max);
						AR_AppendString(tmp, L"\r\n");
				}
				rules[bk[i]] = NULL;
		}
		
		if(cnt > 0)
		{
				if(output)
				{
						AR_AppendString(tmp, L"\r\n");
				}
				goto RECHECK_POINT;
		}

RETURN_POINT:
		AR_DEL(bk);
		
		if(has_left_factor)
		{
				AR_AppendFormatString(output, L"%ls:\r\n",lhs->name);
				AR_AppendString(output, AR_GetStringCString(tmp));
		}
		AR_DestroyString(tmp);
		return has_left_factor;
}



bool_t					Parser_ReportLeftFactor(const psrGrammar_t *grammar, arString_t *output)
{
		const psrRule_t	**rules;
		bool_t has_left_factor;
		size_t	cnt;
		size_t	i,k;
		AR_ASSERT(grammar != NULL);
		
		cnt = 0;
		has_left_factor = false;
		rules = AR_NEWARR0(const psrRule_t*, grammar->symb_list.count);
		
		if(output)AR_ClearString(output);

		for(i = 0; i < grammar->symb_list.count; ++i)
		{
				const psrSymb_t *lhs = grammar->symb_list.lst[i];
				if(lhs->type == PARSER_TERM)continue;
				
				AR_memset((void*)rules, 0, sizeof(const psrRule_t*) * grammar->symb_list.count);
				
				cnt = 0;
				
				for(k = 0; k < grammar->count; ++k)
				{
						if(Parser_CompSymb(lhs, grammar->rules[k]->head) == 0)
						{
								rules[cnt++] = grammar->rules[k];
						}
				}
		
				if(__report_rule_left_factor(lhs, rules, cnt,output))
				{
						has_left_factor = true;
				}
		}

		AR_DEL(rules);
		return has_left_factor;

}



void			Parser_PrintGrammar(const psrGrammar_t *grammar, arString_t *str)
{
		size_t i;
		

		AR_AppendFormatString(str,L"Rule:\r\n");

		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule;
				rule = grammar->rules[i];
				
				Parser_PrintSymbol(rule->head, str);
				AR_AppendString(str, L" -> ");
				Parser_PrintSymbolList(&rule->body, str);
				AR_AppendString(str, L"    ");
				Parser_PrintSymbol(rule->head, str);
				
				{
						const psrTermInfo_t *info;
						AR_ASSERT(rule->prec_tok != NULL);
						info = Parser_FindTermByName((psrTermInfoList_t*)&grammar->term_list, rule->prec_tok);
						AR_ASSERT(info != NULL);
						
						/*AR_AppendFormatString(str, L" prec = %d assoc = %d prec_tok = %ls", info->prec, info->assoc, info->term->name);*/
						AR_AppendFormatString(str, L" prec = %" AR_INT_FMT64 L"d assoc = %" AR_INT_FMT64 L"d prec_tok = %ls", (uint_64_t)info->prec, (uint_64_t)info->assoc, info->term->name);
				}
				
				AR_AppendString(str, L"\r\n");
		}
		
		
		AR_AppendString(str, L"------------------------------------------\r\n");
		AR_AppendString(str, L"Term Set:\r\n");
		
		

		for(i = 0; i < grammar->term_list.count; ++i)
		{
				size_t v;
				Parser_PrintSymbol(grammar->term_list.lst[i].term, str);
				
				v = (size_t)Parser_GetTermSymbInfoByName(grammar,grammar->term_list.lst[i].term->name)->val;
				AR_AppendFormatString(str, L": %" AR_PLAT_INT_FMT L"d\r\n",  v);
				/*
				AR_AppendString(str, L"\r\n");
				*/
		}
		

		AR_AppendString(str, L"------------------------------------------\r\n");
		AR_AppendString(str, L"Symbol List:\r\n");
		
		
		Parser_PrintSymbolList(Parser_GetSymbList(grammar), str);
		AR_AppendString(str, L"\r\n\r\n");
}
#endif



AR_NAMESPACE_END












