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

#include "grammar.h"
#include "parser_in.h"

AR_NAMESPACE_BEGIN

/*
void			PSR_InitTermInfoList(psrTermInfoList_t	*lst);
void			PSR_UnInitTermInfoList(psrTermInfoList_t	*lst);
void			PSR_ClearTermInfoList(psrTermInfoList_t	*lst);
psrTermInfo_t*	PSR_FindTermByName(psrTermInfoList_t	*lst, const wchar_t *name);
psrTermInfo_t*	PSR_FindTermByValue(psrTermInfoList_t	*lst, size_t val);
bool_t			PSR_InsertToTermInfoList(psrTermInfoList_t	*lst, const wchar_t *name, size_t val, psrAssocType_t assoc, size_t prec, psrTermFunc_t	leaf_f);
*/


void			PSR_InitTermInfoList(psrTermInfoList_t	*lst)
{
		AR_ASSERT(lst != NULL);
		AR_memset(lst, 0, sizeof(*lst));
		

}


void			PSR_UnInitTermInfoList(psrTermInfoList_t	*lst)
{
		AR_ASSERT(lst != NULL);
		PSR_ClearTermInfoList(lst);
		if(lst->lst)AR_DEL(lst->lst);
		AR_memset(lst, 0, sizeof(*lst));
}


void			PSR_ClearTermInfoList(psrTermInfoList_t	*lst)
{
		size_t i;
		AR_ASSERT(lst != NULL);

		for(i = 0; i < lst->count; ++i)
		{
				psrTermInfo_t	*info = &lst->lst[i];

				PSR_DestroySymb(info->term);
		}
		lst->count = 0;
}





psrTermInfo_t*	PSR_FindTermByName(psrTermInfoList_t	*lst, const wchar_t *name)
{
		size_t i;
		AR_ASSERT(lst != NULL && name != NULL);
		for(i = 0; i < lst->count; ++i)
		{
				psrTermInfo_t	*info = &lst->lst[i];
				/*注意，这里的参数name可能是任何非PSR_StringTable分配的内存，
				 *因此，这里必须用AR_wcscmp比较字符串而非类似PSR_CompSymb内部的比较str指针
				*/
				if(AR_wcscmp(info->term->name, name) == 0)return info;
		}
		return NULL;
}



psrTermInfo_t*	PSR_FindTermByValue(psrTermInfoList_t	*lst, size_t val)
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


bool_t			PSR_InsertToTermInfoList(psrTermInfoList_t	*lst, const wchar_t *name, size_t val, psrAssocType_t assoc, size_t prec, psrTermFunc_t	leaf_f)
{
		psrTermInfo_t *info;
		AR_ASSERT(lst != NULL && name != NULL);

		if(lst->count == lst->cap)
		{
				lst->cap = (lst->cap + 4)*2;
				lst->lst = AR_REALLOC(psrTermInfo_t, lst->lst, lst->cap);
		}
		info = &lst->lst[lst->count++];
		
		info->term = PSR_CreateSymb(name, PSR_TERM);
		info->assoc = assoc;
		info->prec = prec;
		info->val = val;
		info->leaf_f = leaf_f;
		return true;
}

/*******************************************Rule******************************************************************/

/*


*/



psrRule_t* PSR_CreateRule(const psrSymb_t *head, const psrSymbList_t *body, const wchar_t *prec_tok, psrRuleFunc_t rule_f, size_t auto_ret, const psrTermInfoList_t *term_list, arIOCtx_t *ctx)
{
		psrRule_t		*rule;
		size_t i;
		bool_t			inerr;
		const wchar_t*  right_term;
		AR_ASSERT(head != NULL && body != NULL && term_list != NULL);
		
		AR_ASSERT(ctx != NULL);

		AR_ASSERT(auto_ret <= body->count);

		if(PSR_FindTermByName((psrTermInfoList_t*)term_list, head->name) != NULL)
		{
				AR_printf_ctx(ctx, L"Grammar Error: Duplicate Rule name <%ls>!\r\n", head->name);
				return NULL;
		}

		if(prec_tok != NULL && PSR_FindTermByName((psrTermInfoList_t*)term_list, prec_tok) == NULL)
		{
				/*AR_error(L"Grammar Error: Invalid prec token in <%ls>!\r\n", head->name);*/
				AR_printf_ctx(ctx, L"Grammar Error: Invalid prec \"%ls\" token <%ls>\r\n", prec_tok, head->name);
				return NULL;
		}
		
		right_term = NULL; 
		inerr = false;

		for(i = 0; i < body->count; ++i)
		{
				const psrSymb_t	*symb;

				symb = body->lst[i];

				if(body->lst[i]->type == PSR_TERM)
				{
						if(PSR_FindTermByName((psrTermInfoList_t*)term_list, symb->name) == NULL)return NULL;
						
						if(PSR_CompSymb(symb, PSR_ErrorSymb) == 0)
						{
								if(inerr)
								{
										/*AR_error(L"Grammar Error: Duplicate error definition in <%ls>!\r\n", head->name);*/
										AR_printf_ctx(ctx, L"Grammar Error: Duplicate error definition in <%ls>!\r\n", head->name);
										return NULL;
								}else
								{
										inerr = true;
								}
						}
						right_term = symb->name;
				}else
				{
						if(PSR_FindTermByName((psrTermInfoList_t*)term_list, symb->name) != NULL)return NULL;
				}
		}
		
		if(right_term == NULL)right_term = PSR_DefPrecSymb->name;
		
		rule = AR_NEW0(psrRule_t);
		PSR_InitSymbList(&rule->body);
		
		rule->head = PSR_CopyNewSymb(head);
		
		for(i = 0; i < body->count; ++i)
		{
				PSR_InsertToSymbList(&rule->body, PSR_CopyNewSymb(body->lst[i]));
				
		}


		rule->prec_tok = prec_tok != NULL ? AR_wcsdup(prec_tok) : AR_wcsdup(right_term);
		rule->rule_f = rule_f;
		rule->auto_ret = auto_ret;
		return rule;
}



/*****************************************************************************************************************************************/

psrRule_t* PSR_CreateRuleByStr(const wchar_t *str, const wchar_t *prec, psrRuleFunc_t rule_f, size_t auto_ret, const psrTermInfoList_t *term_list, arIOCtx_t *ctx)
{
		const psrSymb_t *head;
		psrSymbList_t	body;
		psrRule_t		*res;
		const wchar_t	*p, *beg;
		size_t			count;
		AR_ASSERT(str != NULL && term_list != NULL);

		AR_ASSERT(ctx != NULL);
		
		res = NULL;
		head = NULL; 
		PSR_InitSymbList(&body);
		p = AR_wcstrim_space(str);

		beg = p;

		if(!AR_iswalpha(*beg) && *beg != L'_')goto END_POINT;
		

		while(*p != L'\0' && (AR_iswalnum(*p) || *p == L'_'))p++;

		if(*p == L'\0' || p - beg == 0)goto END_POINT;
		
		{
				
				wchar_t *name;
				count = p - beg;
				name = AR_wcsndup(beg, count);
				head = PSR_CreateSymb(name, PSR_NONTERM);
				AR_DEL(name);
		}
		
		p = AR_wcstrim_space(p);

		if(*p != L':')goto END_POINT;
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
						

						{
								wchar_t *name;
								const psrSymb_t *tmp;
								count = p - beg;
								name = AR_wcsndup(beg, count);

								if(PSR_FindTermByName((psrTermInfoList_t*)term_list, name) == NULL)
								{
										tmp = PSR_CreateSymb(name, PSR_NONTERM);
								}else
								{
										tmp = PSR_CreateSymb(name, PSR_TERM);
								}
								PSR_InsertToSymbList(&body, tmp);
								AR_DEL(name);
						}
				}
		}
		

		res =  PSR_CreateRule(head, &body, prec, rule_f, auto_ret, term_list, ctx);

END_POINT:
		if(head)PSR_DestroySymb(head);
		
		{
				size_t i;
				
				for(i = 0; i < body.count; ++i)
				{
						PSR_DestroySymb(body.lst[i]);
				}
		}
		PSR_UnInitSymbList(&body);
		
		return res;
}


void			PSR_DestroyRule(psrRule_t *rule)
{
		size_t i;
		AR_ASSERT(rule != NULL);

		PSR_DestroySymb(rule->head);
		for(i = 0; i < rule->body.count; ++i)
		{
				PSR_DestroySymb(rule->body.lst[i]);
		}

		PSR_UnInitSymbList(&rule->body);
		if(rule->prec_tok) AR_DEL(rule->prec_tok);
		AR_DEL(rule);
}



psrRule_t*		PSR_CopyNewRule(const psrRule_t *rule)
{
		psrRule_t *dest;
		size_t i;
		AR_ASSERT(rule != NULL);
		dest = AR_NEW0(psrRule_t);

		dest->head = PSR_CopyNewSymb(rule->head);
		PSR_InitSymbList(&dest->body);

		for(i = 0; i < rule->body.count; ++i)
		{
				PSR_InsertToSymbList(&dest->body, PSR_CopyNewSymb(rule->body.lst[i]));
		}

		dest->prec_tok = rule->prec_tok;
		dest->rule_f = rule->rule_f;
		return dest;

}

bool_t			PSR_IsEmptyRule(const psrRule_t *rule)
{
		AR_ASSERT(rule != NULL);

		return rule->body.count == 0;
}


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

		PSR_InsertToTermInfoList(&grammar->term_list, PSR_EOISymb->name,PSR_EOI_TOKVAL, PSR_ASSOC_NONASSOC, 0, NULL);
		PSR_InsertToTermInfoList(&grammar->term_list, PSR_ErrorSymb->name, PSR_ERROR_TOKVAL, PSR_ASSOC_NONASSOC, 0, NULL);
		PSR_InsertToTermInfoList(&grammar->term_list, PSR_DefPrecSymb->name, PSR_DEFPREC_TOKVAL, PSR_ASSOC_NONASSOC, 0, NULL);

		{
				psrRule_t *start;
				psrSymbList_t	body;
				PSR_InitSymbList(&body);
				start = PSR_CreateRule(PSR_StartSymb, &body, NULL, NULL, 0, &grammar->term_list, &grammar->io_ctx);
				AR_ASSERT(start != NULL);
				PSR_UnInitSymbList(&body);
				__insert_rule(grammar, start);
		}
}

/******************************************************************************************************************/

void					PSR_ResetGrammarIOContext(psrGrammar_t *gmr, const arIOCtx_t *io_ctx)
{
		AR_ASSERT(gmr != NULL && io_ctx != NULL);

		gmr->io_ctx = io_ctx != NULL ? *io_ctx : *AR_global_ioctx();
}

void					PSR_ResetGrammarParseContext(psrGrammar_t *gmr, const psrCtx_t *psr_ctx)
{
		AR_ASSERT(gmr != NULL && psr_ctx != NULL);
		gmr->psr_ctx = *psr_ctx;
}

psrGrammar_t*			PSR_CreateGrammar(const psrCtx_t *ctx, const arIOCtx_t *io_ctx)
{
		psrGrammar_t* gmr;

		AR_ASSERT(ctx != NULL && ctx->free_f != NULL);
		gmr = AR_NEW0(psrGrammar_t);
		gmr->psr_ctx = *ctx;
		gmr->io_ctx = io_ctx != NULL ? *io_ctx : *AR_global_ioctx();

		PSR_InitTermInfoList(&gmr->term_list);
		PSR_InitSymbList(&gmr->symb_list);
		
		__init_grammar_component_unit(gmr);
		
		return gmr;
}

static void __clear_grammar(psrGrammar_t *grammar)
{
		size_t i;
		AR_ASSERT(grammar != NULL);
		
		for(i = 0; i < grammar->count; ++i)
		{
				PSR_DestroyRule(grammar->rules[i]);
				grammar->rules[i] = NULL;
		}
		
		grammar->count = 0;
		PSR_ClearTermInfoList(&grammar->term_list);
		PSR_ClearSymbList(&grammar->symb_list);
}


void	PSR_ClearGrammar(psrGrammar_t *grammar)
{
		
		AR_ASSERT(grammar != NULL);
		__clear_grammar(grammar);
		__init_grammar_component_unit(grammar);
}

/*
void					PSR_ClearGrammar(psrGrammar_t *grammar)
{
		size_t i;
		for(i = 0; i < grammar->count; ++i)
		{
				PSR_DestroyRule(grammar->rules[i]);
				grammar->rules[i] = NULL;
		}
		
		grammar->count = 0;
		PSR_ClearTermInfoList(&grammar->term_list);
		PSR_ClearSymbList(&grammar->symb_list);
		__init_grammar_component_unit(grammar);
}
*/


void					PSR_DestroyGrammar(psrGrammar_t *grammar)
{

		AR_ASSERT(grammar != NULL);
		/*PSR_ClearGrammar(grammar);*/
		__clear_grammar(grammar);
		
		PSR_UnInitTermInfoList(&grammar->term_list);
		PSR_UnInitSymbList(&grammar->symb_list);
		AR_DEL(grammar->rules);
		AR_DEL(grammar);
}


/*

psrGrammar_t*			PSR_CopyNewGrammar(const psrGrammar_t* grammar)
{
		psrGrammar_t	*gmr;
		size_t i;
		AR_ASSERT(grammar != NULL);

		gmr = AR_NEW0(psrGrammar_t);

		gmr->user = grammar->user;

		gmr->count = gmr->cap = grammar->count;

		gmr->rules = AR_NEWARR0(psrRule_t*, gmr->cap);

		for(i = 0; i < gmr->cap; ++i)
		{
				gmr->rules[i] = PSR_CopyNewRule(grammar->rules[i]);
		}

		PSR_InitSymbList(&gmr->symb_list);
		PSR_InitTermInfoList(&gmr->term_list);

		for(i = 0; i < grammar->symb_list.count; ++i)
		{
				PSR_InsertToSymbList(&gmr->symb_list, grammar->symb_list.lst[i]);
		}
	
		for(i = 0; i < grammar->term_list.count; ++i)
		{
				const psrTermInfo_t *term = &grammar->term_list.lst[i];
				PSR_InsertToTermInfoList(&gmr->term_list, term->term->name, term->val, term->assoc, term->prec, term->leaf_f);
		}

		return gmr;
}
*/

const psrCtx_t*			PSR_GetGrammarContext(const psrGrammar_t *grammar)
{
		AR_ASSERT(grammar != NULL);
		return &grammar->psr_ctx;
}

const arIOCtx_t*		PSR_GetGrammarIOContext(const psrGrammar_t *grammar)
{
		AR_ASSERT(grammar != NULL);
		return &grammar->io_ctx;
}


bool_t					PSR_InsertTerm(psrGrammar_t *grammar, const wchar_t *name, size_t val, psrAssocType_t assoc, size_t prec, psrTermFunc_t	leaf_f)
{
		AR_ASSERT(grammar != NULL && name != NULL);
		
		if(val < PSR_MIN_TOKENVAL)
		{
				/*AR_error(AR_GRAMMAR, L"Grammar Error : invalid token value %d\r\n", val);*/
				/*AR_error(L"Grammar Error : invalid token value %" AR_PLAT_INT_FMT L"d\r\n", (size_t)val);*/
				AR_printf_ctx(&grammar->io_ctx, L"Grammar Error : invalid token value %" AR_PLAT_INT_FMT L"d\r\n", (size_t)val);
				

				return false;
		}
		

		if(PSR_FindTermByName(&grammar->term_list, name) != NULL)
		{
				/*AR_error( L"Grammar Error : duplicate name : %ls definition\r\n", name);*/
				AR_printf_ctx(&grammar->io_ctx, L"Grammar Error : duplicate name : \"%ls\" definition\r\n", name);
				return false;
		}

		if(PSR_FindTermByValue(&grammar->term_list, val) != NULL)
		{
				/*AR_error(AR_GRAMMAR, L"Grammar Error : duplicate token value : %d definition\r\n", val);*/
				/*AR_error(L"Grammar Error : duplicate token value : %" AR_PLAT_INT_FMT L"d definition\r\n", (size_t)val);*/
				AR_printf_ctx(&grammar->io_ctx, L"Grammar Error : duplicate token value : %" AR_PLAT_INT_FMT L"d definition\r\n", (size_t)val);
				return false;
		}
		
		return PSR_InsertToTermInfoList(&grammar->term_list, name, val, assoc, prec, leaf_f);
}

bool_t					PSR_InsertRule(psrGrammar_t *grammar, psrRule_t *rule)
{
		size_t i;
		AR_ASSERT(grammar != NULL && rule != NULL);
		
		for(i = 0; i < grammar->count; ++i)
		{
				if(grammar->rules[i] == rule)return false;
		}

		if(PSR_CompSymb(rule->head, PSR_StartSymb) == 0)/*rule-head不可为%Start保留符号*/
		{
				/*AR_error(L"Grammar Error : Non-term token %ls was reserved\r\n", rule->head->name);*/
				AR_printf_ctx(&grammar->io_ctx, L"Grammar Error : Non-term token %ls was reserved\r\n", rule->head->name);
				return false;
		}
		
		if(grammar->count == 1)
		{
				AR_ASSERT(grammar->rules[0]->body.count == 0);
				/*
				PSR_DestroySymb(grammar->rules[0]->body.lst[0]);
				grammar->rules[0]->body.lst[0] = PSR_CopyNewSymb(rule->head);
				*/
				PSR_InsertToSymbList(&grammar->rules[0]->body, PSR_CopyNewSymb(rule->head));
		}
		__insert_rule(grammar, rule);
		
		return true;
}

bool_t					PSR_InsertRuleByPartStr(psrGrammar_t *grammar, const psrSymb_t *head, const psrSymbList_t *body, const wchar_t *prec_tok, psrRuleFunc_t rule_f, size_t auto_ret)
{
		psrRule_t		*rule;
		AR_ASSERT(grammar != NULL && head != NULL && body != NULL);

		rule = PSR_CreateRule(head, body, prec_tok, rule_f, auto_ret, &grammar->term_list, &grammar->io_ctx);
		
		if(rule == NULL)return false;

		return PSR_InsertRule(grammar, rule);
}


bool_t					PSR_InsertRuleByStr(psrGrammar_t *grammar, const wchar_t *str, const wchar_t *prec, psrRuleFunc_t rule_f, size_t auto_ret)
{

		psrRule_t		*rule;
		AR_ASSERT(grammar != NULL && str != NULL);
		rule = PSR_CreateRuleByStr(str, prec, rule_f, auto_ret, &grammar->term_list, &grammar->io_ctx);
		if(rule == NULL)return false;
		return PSR_InsertRule(grammar, rule);
}







int_t					PSR_IndexOfGrammar(const psrGrammar_t *grammar, const psrRule_t *rule)
{
		size_t i;
		for(i = 0; i < grammar->count; ++i)
		{
				if(grammar->rules[i] == rule)return (int_t)i;
		}

		return -1;
}

const psrSymbList_t* PSR_GetSymbList(const psrGrammar_t *grammar)
{
		psrSymbList_t *lst;
		AR_ASSERT(grammar != NULL);
		
		lst = (psrSymbList_t*)(&grammar->symb_list);

		if(grammar->symb_list.count == 0)
		{
				size_t i;

				for(i = 0; i < grammar->term_list.count; ++i)PSR_InsertToSymbList(lst, grammar->term_list.lst[i].term);
				
				
				
				for(i = 0; i < grammar->count; ++i)
				{
						const psrSymb_t *head = grammar->rules[i]->head;

						if(PSR_FindFromSymbList(lst, head) == -1)
						{
								PSR_InsertToSymbList(lst, head);
						}
				}
		}

		return lst;
}





/*效率成为问题时我才会考虑改这没什么用的东西！*/
static bool_t __detect_left_recursion(const psrGrammar_t *grammar, const psrSymb_t *head, psrSymbList_t *lst, arString_t *output)
{
		size_t i;
		bool_t is_recu = false;
		AR_ASSERT(grammar != NULL && head != NULL && lst != NULL);

		/*AR_ASSERT(lst->count > 0);*/

		AR_ASSERT(PSR_FindFromSymbList(lst, head) == -1);
		
		PSR_InsertToSymbList(lst, head);
		
		
		
		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule;
				rule = grammar->rules[i];
				
				if(PSR_CompSymb(rule->head, head) == 0)
				{
						const psrSymb_t *symb;

						size_t x = 0;
						
						/*while(x < rule->body.count && PSR_CompSymb(rule->body.lst[x], PSR_EpsilonSymb) == 0)x++;*/
						
						if(x < rule->body.count && rule->body.lst[x]->type == PSR_NONTERM)
						{
								symb = rule->body.lst[x];

								if(PSR_CompSymb(lst->lst[0], symb) == 0)
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

								}else if(PSR_FindFromSymbList(lst, symb) != -1)
								{
										continue;
								}else
								{
										__detect_left_recursion(grammar, symb, lst,output);
								}
						}
				}
		}
		
		PSR_RemoveFromSymbListByIndex(lst, lst->count-1);
		return is_recu;
		
}


bool_t					PSR_ReportLeftRecursion(const psrGrammar_t *grammar, arString_t *output)
{
		size_t i;
		psrSymbList_t	lst;
		bool_t			ret = false;
		AR_ASSERT(grammar != NULL);
		if(output)AR_ClearString(output);
		PSR_InitSymbList(&lst);
		
		for(i = 0; i < grammar->symb_list.count; ++i)
		{
				const psrSymb_t *symb;
				
				symb = grammar->symb_list.lst[i];

				PSR_ClearSymbList(&lst);
				
				if(symb->type == PSR_NONTERM)
				{
						if(__detect_left_recursion(grammar, symb, &lst,output))
						{
								ret = true;
						}
				}
		}

		PSR_UnInitSymbList(&lst);
		return ret;
}




bool_t			PSR_CheckIsValidGrammar(const psrGrammar_t *grammar)
{
		size_t i,j,k;
		bool_t result;
		const psrSymbList_t *lst;
		AR_ASSERT(grammar != NULL);
		
		if(grammar->count < 2)return false;/*Start和输入的第一个产生式一定>=2*/

		result = true;
		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule;
				rule = grammar->rules[i];

				for(j = 0; j < rule->body.count; ++j)
				{
						const psrSymb_t *symb;
						symb = rule->body.lst[j];

						if(symb->type == PSR_NONTERM)
						{
								bool_t is_ok;
								is_ok = false;
								for(k = 0; !is_ok && k < grammar->count; ++k)
								{
										if(PSR_CompSymb(symb, grammar->rules[k]->head) == 0)
										{
												is_ok = true;
										}
								}

								if(!is_ok)
								{
										/*AR_error(L"Grammar Error : The rule <%ls> not exist in this grammar\r\n", symb->name);*/
										AR_printf_ctx((arIOCtx_t*)&grammar->io_ctx, L"Grammar Error : The rule <%ls> not exist in this grammar <%ls>\r\n", symb->name, rule->head->name);
										result = false;
								}
						}

				}
		}

		lst = PSR_GetSymbList(grammar);

		
		for(i = 0; i < lst->count; ++i)
		{
				const psrSymb_t *symb;

				symb = lst->lst[i];

				if(symb->type == PSR_NONTERM && PSR_CompSymb(symb, PSR_StartSymb) != 0)
				{
						bool_t is_ok;
						is_ok = false;
						for(k = 0; !is_ok && k < grammar->count; ++k)
						{
								const psrRule_t *rule;
								
								rule = grammar->rules[k];

								if(PSR_CompSymb(rule->head, symb) == 0)continue;/*自己引用自己不算*/
								
								for(j = 0; j < rule->body.count; ++j)
								{
										if(PSR_CompSymb(rule->body.lst[j], symb) == 0)
										{
												is_ok = true;
												break;
										}
								}
						}

						if(!is_ok)
						{
								/*AR_error(L"Grammar Warning : The rule %ls is declared but never used\r\n", symb->name);*/

								AR_printf_ctx((arIOCtx_t*)&grammar->io_ctx,L"Grammar Warning : The rule <%ls> is declared but never used\r\n", symb->name);
						}
				}
		}

		return result;
}

const psrRule_t*		PSR_GetStartRule(const psrGrammar_t *grammar)
{
		return grammar->rules[0];
}


bool_t					PSR_SetFirstRule(psrGrammar_t *grammar, const wchar_t *rule_name)
{
		psrRule_t *start = grammar->rules[0];
		const psrSymb_t *lhs = NULL;
		size_t i;
		AR_ASSERT(grammar != NULL && grammar->count > 0 && rule_name != NULL);

		if(PSR_GetTermSymbInfoByName(grammar, rule_name) != NULL)
		{
				return false;
		}

		if(AR_wcscmp(PSR_StartSymb->name, rule_name) == 0)return false;


		for(i = 0; i < grammar->count; ++i)
		{
				if(AR_wcscmp(rule_name, grammar->rules[i]->head->name) == 0)
				{
						lhs = PSR_CopyNewSymb(grammar->rules[i]->head);
						break;
				}
		}

		if(lhs == NULL)return false;

		AR_ASSERT(grammar->count > 1);
		AR_ASSERT(start != NULL && start->body.count > 0);

		PSR_DestroySymb(start->body.lst[0]);
		start->body.lst[0] = lhs;
		
		return true;
}

const psrTermInfo_t* PSR_GetRulePrecAssocInfo(const psrGrammar_t *grammar, const psrRule_t *rule)
{
		AR_ASSERT(grammar != NULL && rule != NULL && rule->prec_tok != NULL);
		return PSR_FindTermByName((psrTermInfoList_t*)&grammar->term_list, rule->prec_tok);

}

psrTermInfo_t*	PSR_GetTermSymbInfo(const psrGrammar_t	*grammar, const psrSymb_t *term)
{
		AR_ASSERT(grammar != NULL && term != NULL && term->type == PSR_TERM);
		return  PSR_FindTermByName((psrTermInfoList_t*)&grammar->term_list, term->name);
		
}

psrTermInfo_t*			PSR_GetTermSymbInfoByName(const psrGrammar_t	*grammar, const wchar_t *name)
{
		AR_ASSERT(grammar != NULL && name != NULL);

		return  PSR_FindTermByName((psrTermInfoList_t*)&grammar->term_list, name);

}

psrTermInfo_t*			PSR_GetTermSymbInfoByValue(const psrGrammar_t	*grammar, size_t val)
{
		AR_ASSERT(grammar != NULL);
		return PSR_FindTermByValue((psrTermInfoList_t*)&grammar->term_list, val);
}



void			PSR_PrintGrammar(const psrGrammar_t *grammar, arString_t *str)
{
		size_t i;
		

		AR_AppendFormatString(str,L"Rule:\r\n");

		for(i = 0; i < grammar->count; ++i)
		{
				const psrRule_t *rule;
				rule = grammar->rules[i];
				
				PSR_PrintSymbol(rule->head, str);
				AR_AppendString(str, L" -> ");
				PSR_PrintSymbolList(&rule->body, str);
				AR_AppendString(str, L"    ");
				PSR_PrintSymbol(rule->head, str);
				
				{
						const psrTermInfo_t *info;
						AR_ASSERT(rule->prec_tok != NULL);
						info = PSR_FindTermByName((psrTermInfoList_t*)&grammar->term_list, rule->prec_tok);
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
				PSR_PrintSymbol(grammar->term_list.lst[i].term, str);
				
				v = (size_t)PSR_GetTermSymbInfoByName(grammar,grammar->term_list.lst[i].term->name)->val;
				AR_AppendFormatString(str, L": %" AR_PLAT_INT_FMT L"d\r\n",  v);
				/*
				AR_AppendString(str, L"\r\n");
				*/
		}
		

		AR_AppendString(str, L"------------------------------------------\r\n");
		AR_AppendString(str, L"Symbol List:\r\n");
		
		
		PSR_PrintSymbolList(PSR_GetSymbList(grammar), str);
		AR_AppendString(str, L"\r\n\r\n");
}




/************************************************************************************************************************************/



/*
firstset求法为，例如A->X(0)...X(n-1);
设First(Term) = {Term}; First(Epsilon) = {Epsilon}; i = 0;
如果First(X(i))存在{Epsilon}，则 First(A) += (First(X(i) - {Epsilon}); 如果i == n-1，则First(A) += {Epsilon}，本次循环终止；
如果不存在{Epsilon}，则循环终止；

因为每次都会处理至少一个符号，所以能确保循环终止，除非无任何符号加入到First集合中(changed == False);
*/
void					PSR_CalcFirstSet(const psrGrammar_t *grammar, psrSymbMap_t *first_set)
{
		size_t i;
		bool_t changed;
		const psrSymbList_t		*lst;
		const psrSymb_t			*key;
		AR_ASSERT(grammar != NULL && first_set != NULL && grammar->count > 1);
		
		lst = PSR_GetSymbList(grammar);

		for(i = 0; i < lst->count; ++i)
		{
				key = lst->lst[i];
				if(key->type == PSR_NONTERM)
				{
						PSR_InsertToSymbMap(first_set, key, NULL);
				}else
				{
						PSR_InsertToSymbMap(first_set, key, key);
				}
		}

		changed = false;
		do{
				changed = false;

				for(i = 0; i < grammar->count; ++i)
				{
						psrMapRec_t *rec = NULL;
						size_t k = 0;
						psrRule_t *rule = grammar->rules[i];
						rec= PSR_GetSymbolFromSymbMap(first_set, rule->head);

						if(rec->can_empty)continue;
						
						
						for(k = 0; k < rule->body.count; ++k)
						{
								const psrSymb_t *symb = NULL;
								const psrMapRec_t *tmp_rec = NULL;
								symb = rule->body.lst[k];
								if(symb->type == PSR_TERM)break;
								tmp_rec= PSR_GetSymbolFromSymbMap(first_set, symb);
								if(!tmp_rec->can_empty)break;
						}

						if(k == rule->body.count)
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
						rec= PSR_GetSymbolFromSymbMap(first_set, s1);


						for(k = 0; k < rule->body.count; ++k)
						{
								s2 = rule->body.lst[k];
								
								if(s2->type == PSR_TERM)
								{
										if(PSR_InsertToSymbList_Unique(&rec->lst, s2))
										{
												changed = true;
										}
										break;
								}else if(PSR_CompSymb(s1, s2) == 0)
								{
										if(!rec->can_empty)break;
								}
								else
								{
										const psrMapRec_t *rec2 = NULL;
										size_t x;
										rec2 = PSR_GetSymbolFromSymbMap(first_set, s2);

										for(x = 0; x < rec2->lst.count; ++x)
										{
												if(PSR_InsertToSymbList_Unique(&rec->lst, rec2->lst.lst[x]))
												{
														changed = true;
												}
										}

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


void					PSR_CalcFollowSet(const psrGrammar_t *grammar, psrSymbMap_t *follow_set, const psrSymbMap_t *first_set)
{
		size_t i;
		bool_t changed;
		const psrSymbList_t *lst;
		psrMapRec_t *rec1 = NULL, *rec2 = NULL;

		AR_ASSERT(grammar != NULL && grammar->count > 1 && follow_set != NULL && first_set != NULL);

		changed = true;
		lst = PSR_GetSymbList(grammar);
		for(i = 0; i < lst->count; i++)
		{
				const psrSymb_t *key = lst->lst[i];
				if(key->type == PSR_NONTERM)
				{
						PSR_InsertToSymbMap(follow_set, key, NULL);

						rec1 = PSR_GetSymbolFromSymbMap(first_set, key);
						rec2 = PSR_GetSymbolFromSymbMap(follow_set, key);
						rec2->can_empty = rec1->can_empty;
				}
		}

		PSR_InsertToSymbMap(follow_set, PSR_StartSymb, PSR_EOISymb);
		
		do{
				changed = false;
				
				for(i = 0; i < grammar->count; ++i)/*对每一条产生式*/
				{
						const psrRule_t *rule;
						const psrMapRec_t *head_follow;
						size_t k;
						rule = grammar->rules[i];

						head_follow = PSR_GetSymbolFromSymbMap(follow_set,rule->head);/*grammar->head[i]所对应的follow集合*/
						
						for(k = 0; k < rule->body.count; ++k)
						{
								const psrSymb_t *key;
								size_t next_idx;
								key = rule->body.lst[k];

								if(key->type == PSR_TERM)continue;

								next_idx = k + 1;
								
								for(next_idx = k + 1; next_idx < rule->body.count; ++next_idx)
								{
										const psrSymb_t *next = NULL;
										const psrMapRec_t *first_rec;
										psrMapRec_t *rec_tmp = NULL;
										size_t x;
										
										next = PSR_IndexOfSymbList(&rule->body, next_idx);
										first_rec = PSR_GetSymbolFromSymbMap(first_set, next);
										rec_tmp  = PSR_GetSymbolFromSymbMap(follow_set, key);
										for(x = 0; first_rec && x < first_rec->lst.count; ++x)
										{
												const psrSymb_t *f_symb;
												f_symb = first_rec->lst.lst[x];
												AR_ASSERT(f_symb->type == PSR_TERM);				/*first不可能为非终结符*/

												if(PSR_InsertToSymbList_Unique(&rec_tmp->lst, f_symb))
												{
														changed = true;	/*任何一步的改动都需要重新计算follow集合*/
												}
										}
										if(!first_rec->can_empty)break;
								}

								if(next_idx == rule->body.count)
								{
										
										size_t x;
										psrMapRec_t *rec_tmp = PSR_GetSymbolFromSymbMap(follow_set, key);

										for(x = 0; x < head_follow->lst.count; ++x)
										{
												if(PSR_InsertToSymbList_Unique(&rec_tmp->lst,head_follow->lst.lst[x]))
												{
														changed = true;	/*任何一步的改动都需要重新计算follow集合*/
												}
										}
								}
						}
				}
		}while(changed);
}


AR_NAMESPACE_END












