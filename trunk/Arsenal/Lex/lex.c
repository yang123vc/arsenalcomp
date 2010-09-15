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

#include "rgx.h"
#include "lex.h"

AR_NAMESPACE_BEGIN

void	Lex_Init()
{
		RGX_InitNode();
		RGX_InitMisc();
}

void	Lex_UnInit()
{
		RGX_UnInitNode();
		RGX_UnInitMisc();
}

/*
typedef struct __lex_rule_set
{
		lexAction_t					*action;
		struct __rgx_node_tag		**nodes;
		size_t						count;
		size_t						cap;
}lexRuleSet_t;
*/

static void		__init_rule_set(lexRuleSet_t *set)
{
		AR_ASSERT(set != NULL);
		AR_memset(set, 0, sizeof(*set));
}

static void		__clear_rule_set(lexRuleSet_t *set)
{
		size_t i;
		AR_ASSERT(set != NULL);

		for(i = 0; i < set->count; ++i)
		{
				RGX_DestroyNode(set->nodes[i]);
				set->nodes[i] = NULL;
		}
		
		set->count = 0;

}

static void		__uninit_rule_set(lexRuleSet_t *set)
{
		size_t i;
		AR_ASSERT(set != NULL);
		for(i = 0; i < set->count; ++i)
		{
				RGX_DestroyNode(set->nodes[i]);
				set->nodes[i] = NULL;
		}

		AR_DEL(set->nodes);
		AR_DEL(set->action);
		AR_memset(set, 0, sizeof(*set));
}

static void		__insert_to_rule_set(lexRuleSet_t *set, rgxNode_t *node, const lexAction_t *action)
{
		AR_ASSERT(set != NULL && node != NULL && action != NULL);

		if(set->count == set->cap)
		{
				set->cap = (set->cap + 1)*2;
				set->action = AR_REALLOC(lexAction_t, set->action, set->cap);
				set->nodes = AR_REALLOC(rgxNode_t*, set->nodes, set->cap);
		}
		
		set->action[set->count] = *action;
		set->nodes[set->count] = node;
		set->count++;
}


static bool_t		__remove_from_rule_set(lexRuleSet_t *set, size_t value)
{
		size_t i;
		AR_ASSERT(set != NULL);

		for(i = 0; i < set->count; ++i)
		{
				if(set->action[i].value == value)
				{
						set->action[i] = set->action[set->count-1];
						set->nodes[i] = set->nodes[set->count-1];
						set->count--;
						return true;
				}
		}
		return false;
}


/**********************************lex**************************************************/

bool_t	Lex_InsertName(lex_t *lex, const wchar_t *name, const wchar_t *expr)
{
		rgxResult_t res;
		AR_ASSERT(name != NULL && AR_wcslen(name) > 0 && expr != NULL);

		if(RGX_FindFromNameSet(lex->name_tbl, name) != NULL)
		{
				/*AR_error( L"Lex Rule Error : Duplicate name defination %ls: %ls\r\n", name, expr);*/
				AR_printf_ctx(&lex->io_ctx, L"Lex Name Error : Duplicate name defination %ls: %ls\r\n", name, expr);
				return false;
		}

		res = RGX_ParseExpr(expr, lex->name_tbl);

		if(res.node == NULL)
		{
				/*AR_error(L"Lex Rule Error : %ls: %ls\r\n", name, res.err.pos);*/
				AR_printf_ctx(&lex->io_ctx, L"Lex Name Error : %ls: %ls\r\n", name, res.err.pos);
				return false;
		}


		/*RGX_CorrectTree(res.node);*/

		return RGX_InsertToNameSet(lex->name_tbl, name, res.node);
}


bool_t	Lex_RemoveByName(lex_t *lex, const wchar_t *name)
{
		AR_ASSERT(lex != NULL && name != NULL);

		return RGX_RemoveFromNameSet(lex->name_tbl, name);
}



bool_t	Lex_Insert(lex_t *lex, const wchar_t *input)
{
		const wchar_t *p;

		AR_ASSERT(lex != NULL && input != NULL);
		p = AR_wcstrim_space(input);

		if(AR_iswdigit(*p) || *p == L'%')/*action*/
		{
				lexAction_t		act;
				bool_t			is_skip;

				is_skip = false;
				if(*p == L'%')
				{
						if(AR_wcsstr(p, L"%skip") == NULL)return false;

						p = AR_wcstrim_space(p + AR_wcslen(L"%skip"));

						is_skip = true;
				}

				act.priority = act.value = 0;
				p = AR_wtou(p, (uint_t*)&act.value, 10);
				if(p == NULL)return false;

				p = AR_wcstrim_space(p);

				if(*p == L',')
				{
						p = AR_wtou(++p, (uint_t*)&act.priority, 10);
						if(p == NULL)return false;
				}

				p = AR_wcstrim_space(p);

				act.is_skip = is_skip;
				return Lex_InsertRule(lex, p, &act);

		}else if(AR_iswalpha(*p) || *p == L'_')
		{
				wchar_t name[AR_RGX_MAXNAME + 1];
				size_t i;
				i = 0;
				while(AR_iswalnum(*p) || *p == L'_')
				{
						name[i++] = *p++;
				}
				name[i] = L'\0';
				p = AR_wcstrim_space(p);

				if(*p != L'=')return false;
				p = AR_wcstrim_space(++p);
				return Lex_InsertName(lex, name, p);

		}else
		{
				/*AR_error(L"Lex Rule Error : Invalid Input %ls\r\n", p);*/

				AR_printf_ctx(&lex->io_ctx, L"Lex Rule Error : Invalid Input %ls\r\n", p);
				return false;
		}


}






void	Lex_ResetIOContext(lex_t *lex, const arIOCtx_t *io)
{
		AR_ASSERT(lex != NULL && io != NULL);
		lex->io_ctx = io == NULL ? *AR_global_ioctx() : *io;


}





bool_t	Lex_RemoveByValue(lex_t *lex, size_t value)
{
		AR_ASSERT(lex != NULL);
		
		return __remove_from_rule_set(&lex->rule_set, value);
}





bool_t	Lex_InsertRule(lex_t *lex, const wchar_t *rule, const lexAction_t *action)
{
		rgxResult_t res;
		rgxNode_t	*cat, *final;
		
		AR_ASSERT(lex != NULL && rule != NULL && action != NULL);
		AR_ASSERT(AR_wcslen(rule) > 0);

		res = RGX_ParseExpr(rule, lex->name_tbl);

		if(res.node == NULL)
		{
				/*AR_error(AR_LEX, L"Lex Rule Error : %d : %ls\n", action->type, res.err.pos);*/
				/*AR_error(L"Lex Rule Error : %" AR_PLAT_INT_FMT L"d : %ls\n", (size_t)action->type, (size_t)res.err.pos);*/

				AR_printf_ctx(&lex->io_ctx, L"Lex Rule Error : %" AR_PLAT_INT_FMT L"d : %ls\n", (size_t)action->value, res.err.pos);
				return false;
		}

		cat = RGX_CreateNode(RGX_CAT_T);
		final = RGX_CreateNode(RGX_FINAL_T);
		final->final_val = (int_t)action->value;

		cat->left = res.node;
		cat->right = final;

		/*RGX_CorrectTree(cat);

		prog = AR_NEW(rgxProg_t);

		RGX_InitProg(prog);

		RGX_Compile(prog, cat);

		Lex_InserToProgSet(lex->prog_set, prog, action);
		RGX_DestroyNode(cat);
		*/

		__insert_to_rule_set(&lex->rule_set, cat, action);
		return true;
}



bool_t	Lex_GenerateTransTable(lex_t *lex)
{
		AR_ASSERT(lex != NULL);
		
		/*
		Lex_SortProgSet(lex->prog_set);
		*/
		return (bool_t)(lex->rule_set.count > 0);
}





lex_t*	Lex_Create(const arIOCtx_t *io)
{
		lex_t *res;

		res = AR_NEW0(lex_t);

		res->name_tbl = AR_NEW(rgxNameSet_t);
		RGX_InitNameSet(res->name_tbl);
		__init_rule_set(&res->rule_set);
		res->io_ctx = io == NULL ? *AR_global_ioctx() : *io;

		return res;
}



void	Lex_Clear(lex_t *lex)
{
		AR_ASSERT(lex != NULL);
		RGX_ClearNameSet(lex->name_tbl);
		__clear_rule_set(&lex->rule_set);
}





void	Lex_Destroy(lex_t *lex)
{
		AR_ASSERT(lex != NULL);
		
		Lex_Clear(lex);
		RGX_UnInitNameSet(lex->name_tbl);
		__uninit_rule_set(&lex->rule_set);
		AR_DEL(lex->name_tbl);
		AR_DEL(lex);
}






AR_NAMESPACE_END
