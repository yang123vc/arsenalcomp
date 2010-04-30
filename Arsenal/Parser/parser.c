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
#include "grammar.h"
#include "lr_action.h"
#include "parser_aux.h"


AR_NAMESPACE_BEGIN


/****************************************************Parser*****************************************************/


const parser_t* PSR_CreateParser(const psrGrammar_t *grammar, psrModeType_t type)
{
		parser_t *parser;
		size_t i;
		AR_ASSERT(grammar != NULL);

		parser = AR_NEW0(parser_t);
		
		parser->grammar = grammar;

		switch(type)
		{
		case PSR_SLR:
				parser->tbl = PSR_CreateActionTable_SLR(parser->grammar);
				break;
		default:
				parser->tbl = PSR_CreateActionTable_LALR(parser->grammar);
				break;
		}

		parser->msg_count = parser->tbl->row;
		parser->msg_set = AR_NEWARR0(psrExpectedMsg_t, parser->msg_count);

		for(i = 0; i < parser->msg_count; ++i)
		{
				PSR_InitExpectedMsg(&parser->msg_set[i], PSR_GetExpectedSymb(parser->tbl, i));
		}

		parser->term_tbl = PSR_CreateTermInfoTable();
		
		{
				const psrTermInfoList_t *tlst = &parser->grammar->term_list;
				
				for(i = 0; i < tlst->count; ++i)
				{
						PSR_InsertToTermInfoTable(parser->term_tbl, &tlst->lst[i]);
				}
		}
		
		return parser;
}



void	  PSR_DestroyParser(const parser_t *parser)
{
		parser_t		*psr = (parser_t*)parser;
		AR_ASSERT(parser != NULL);

		if(psr != NULL)
		{
				size_t i;

				for(i = 0; i < psr->msg_count; ++i)
				{
						PSR_UnInitExpectedMsg(&psr->msg_set[i]);
				}
				AR_DEL(psr->msg_set);

				PSR_DestroyTermInfoTable(psr->term_tbl);
				PSR_DestroyActionTable(psr->tbl);
				AR_DEL(psr);
		}
}


/*
typedef struct __parser_context_tag
{
		const parser_t							*parser;
		bool_t									is_repair;
		bool_t									is_accepted;
		struct __parser_stack_tag				*state_stack;
		struct __parser_node_stack				*node_stack;
}psrContext_t;
*/
psrContext_t*	PSR_CreateContext(const parser_t *parser, void *ctx)
{
		psrContext_t	*psr_ctx = NULL;
		AR_ASSERT(parser != NULL);
		psr_ctx = AR_NEW0(psrContext_t);
		psr_ctx->ctx = ctx;
		psr_ctx->parser = parser;
		psr_ctx->is_repair = false;
		psr_ctx->is_accepted = false;
		psr_ctx->state_stack = AR_NEW0(psrStack_t);
		PSR_InitStack(psr_ctx->state_stack);
		psr_ctx->node_stack = AR_NEW0(psrNodeStack_t);
		PSR_InitNodeStack(psr_ctx->node_stack);
		return psr_ctx;
}

void	  PSR_Clear(psrContext_t *parser_context)
{
		size_t i;
		const psrHandler_t *handler;
		AR_ASSERT(parser_context != NULL && parser_context->parser != NULL);

		handler = PSR_GetGrammarHandler(PSR_GetGrammar(parser_context->parser));
		AR_ASSERT(handler != NULL);
		parser_context->is_repair = false;
		parser_context->is_accepted = false;
		
		for(i = 0; i < parser_context->node_stack->count; ++i)
		{
				if(parser_context->node_stack->nodes[i])handler->free_f(parser_context->node_stack->nodes[i], parser_context->ctx);
		}

		PSR_ClearNodeStack(parser_context->node_stack);
		PSR_ClearStack(parser_context->state_stack);
}

void			PSR_DestroyContext(psrContext_t	*parser_context)
{
		AR_ASSERT(parser_context != NULL);
		PSR_Clear(parser_context);
		PSR_UnInitNodeStack(parser_context->node_stack);
		PSR_UnInitStack(parser_context->state_stack);
		AR_DEL(parser_context->node_stack);
		AR_DEL(parser_context->state_stack);
		AR_DEL(parser_context);
}


psrNode_t*	PSR_GetResult(psrContext_t *parser_context)
{
		psrNode_t *res;
		AR_ASSERT(parser_context != NULL);
		AR_ASSERT(parser_context->is_accepted);
		if(!parser_context->is_accepted)return NULL;
		AR_ASSERT(parser_context->state_stack->count == 2);
		res = PSR_TopNodeStack(parser_context->node_stack);
		
		PSR_PopStack(parser_context->state_stack, 1);
		PSR_PopNodeStack(parser_context->node_stack, 1);
		parser_context->is_accepted = false;
		return res;
}

bool_t	   PSR_IsAccepted(const psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		return parser_context->is_accepted;
}


bool_t	PSR_IsInError(const psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		return parser_context->is_repair;

}

void	PSR_ClearError(psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		parser_context->is_repair = false;
}

size_t			PSR_GetNodeCount(const psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		AR_ASSERT(parser_context->node_stack->count == parser_context->state_stack->count);
		return parser_context->node_stack->count;
}

psrNode_t*		PSR_IndexOfNodeStack(psrContext_t *parser_context, size_t index)
{
		AR_ASSERT(parser_context != NULL && index < parser_context->node_stack->count);
		AR_ASSERT(parser_context->node_stack->count == parser_context->state_stack->count);

		if(index < parser_context->node_stack->count)
		{
				return parser_context->node_stack->nodes[index];
		}else
		{
				return NULL;
		}
}




static void __handle_shift(psrContext_t *parser_context, size_t shift_to, const psrToken_t *tok, const psrTermInfo_t *term)
{
		psrNode_t			*new_node;
		
		AR_ASSERT(parser_context != NULL && tok != NULL && term != NULL && term->leaf_f != NULL);

		PSR_PushStack(parser_context->state_stack, shift_to);

		if(term->leaf_f)
		{
				new_node = term->leaf_f(tok, parser_context->ctx);
		}else
		{
				new_node = NULL;
		}
		PSR_PushNodeStack(parser_context->node_stack, new_node);

/*
		将parser状态从error转回正常状态只有在当前状态上存在移入，例如：
		A -> error ";";
		A -> error;
		等

		第一条将在移入";"时候清除error状态，
		第二条将在error规约为A后的下一次正确输入时清除，例如
		B -> A ";";
		这里假如A有任何错误，将在移入':'后清除错误状态；
*/
		if(parser_context->is_repair)
		{
				parser_context->is_repair = false;
		}
		
}


static void __handle_reduce(psrContext_t *parser_context, const psrAction_t *action)
{
		psrNode_t		**nodes;
		psrNode_t		*new_node;
		size_t			next_state;
		const	psrRule_t *rule;
		
		AR_ASSERT(parser_context != NULL && action != NULL && action->type == PSR_REDUCE);

		rule = PSR_GetRuleOfGrammar(PSR_GetGrammar(parser_context->parser), action->rule_num);
		

		if(action->reduce_count > 0)
		{
				AR_ASSERT(parser_context->node_stack->count >= action->reduce_count);
				nodes = &parser_context->node_stack->nodes[parser_context->node_stack->count - action->reduce_count];
		}else
		{
				nodes = NULL;
		}

/*
		有时候例如A->.这类空产生式也会存在handler，例如语义钩子等，因此这里只根据是否有注册进来的函数来决定规约是否为NULL
*/
		if(rule->rule_f != NULL)
		{
				new_node = rule->rule_f(nodes, action->reduce_count, rule->head->name, parser_context->ctx);
		}else
		{

/****************************************Experiment****************************************************/
				if(rule->auto_ret < action->reduce_count)
				{
						new_node = nodes[rule->auto_ret];
						nodes[rule->auto_ret] = NULL;

				}else
/****************************************************************************************************/
				{
						new_node = NULL;
				}
		}

		if(action->reduce_count > 0)
		{
				PSR_PopStack(parser_context->state_stack, action->reduce_count);

				{
						size_t i;
						const psrHandler_t	*handler = PSR_GetGrammarHandler(PSR_GetGrammar(parser_context->parser));
						AR_ASSERT(handler != NULL);
						for(i = 0; i < action->reduce_count; ++i)
						{
								if(nodes[i] != NULL)
								{
										handler->free_f(nodes[i], parser_context->ctx);
										nodes[i] = NULL;
								}
						}
				}

				PSR_PopNodeStack(parser_context->node_stack, action->reduce_count);
		}
		
		next_state = PSR_GetState(parser_context->parser->tbl, PSR_TopStack(parser_context->state_stack), rule->head);
		PSR_PushStack(parser_context->state_stack, next_state);
		PSR_PushNodeStack(parser_context->node_stack, new_node);
}




static void __on_error(psrContext_t *parser_context, const psrToken_t		*tok)
{
		size_t			top_state;
		const psrHandler_t		*handler;
		
		AR_ASSERT(parser_context != NULL && tok != NULL);
		
		handler = PSR_GetGrammarHandler(PSR_GetGrammar(parser_context->parser));
		

		AR_ASSERT(handler != NULL);
		
		top_state = PSR_TopStack(parser_context->state_stack);
		
		if(handler->error_f == NULL)
		{
				
		}else
		{
				handler->error_f(tok, parser_context->parser->msg_set[top_state].msg, parser_context->parser->msg_set[top_state].count, parser_context->ctx);
		}
}




typedef enum 
{
		ERR_RECOVERY_CONTINUE,
		ERR_RECOVERY_DISCARD,
		ERR_RECOVERY_FAILED
}errRecovery_t;


static errRecovery_t __error_recovery(psrContext_t *parser_context, const psrToken_t *tok)
{		
		const psrHandler_t	*handler;
		
		AR_ASSERT(parser_context != NULL && parser_context->parser != NULL);
		
		handler = PSR_GetGrammarHandler(PSR_GetGrammar(parser_context->parser));
		AR_ASSERT(handler != NULL);

		if(!parser_context->is_repair)
		{
				bool_t found = false;
				
				__on_error(parser_context, tok);

				while(parser_context->state_stack->count > 0 && !found)
				{
						size_t top;
						const psrAction_t *action;

						top = PSR_TopStack(parser_context->state_stack);
						action = PSR_GetAction(parser_context->parser->tbl, top, PSR_ErrorSymb);
						
						AR_ASSERT(action != NULL);
						
/*
		应该找到一个在error上有shift动作的状态，之后移入NULL替代终结符error，如果存在规约，则需要先行规约(当然，规约后就是移入error了),如果error不是合法的输入，
		则向上pop stack和相关节点
*/
						if(action->type == PSR_REDUCE)
						{
								__handle_reduce(parser_context, action);
						}else if(action->type == PSR_ERROR)
						{
								psrNode_t *top_node;
								
								top_node = PSR_TopNodeStack(parser_context->node_stack);
								if(top_node != NULL)handler->free_f(top_node, parser_context->ctx);
								PSR_PopNodeStack(parser_context->node_stack, 1);
								PSR_PopStack(parser_context->state_stack, 1);
								
						
						}else if(action->type == PSR_SHIFT)
						{
								PSR_PushStack(parser_context->state_stack, action->shift_to);
								PSR_PushNodeStack(parser_context->node_stack, NULL);
								found = true;
						}else /*PSR_ACCEPT*/
						{
								/*只有EOI符号才可能导致一个accept动作*/
								AR_ASSERT(false);
								AR_error(AR_ERR_FATAL, L"%ls\r\n", L"Parser : Internal Error");
						}
				}
				
				if(found)
				{
						parser_context->is_repair = true;
						return ERR_RECOVERY_CONTINUE;
				}else
				{
						return ERR_RECOVERY_FAILED;
				}

		}else
		{

/*
		在repair模式，任何不合法的输入都会丢弃，并且不显示任何错误信息，当输入为EOI时，则调用error_f报告已达EOI;
*/
				const psrSymb_t *symb;
				symb = PSR_FindTermFromInfoTable(parser_context->parser->term_tbl, tok->term_val)->term;
				AR_ASSERT(symb != NULL);
				if(PSR_CompSymb(PSR_EOISymb, symb) == 0)
				{
						__on_error(parser_context, tok);

						return ERR_RECOVERY_FAILED;
				}else
				{
						return ERR_RECOVERY_DISCARD;
				}
		}
}


bool_t		PSR_AddToken(psrContext_t *parser_context, const psrToken_t *tok)
{

		bool_t					is_done;
		const psrTermInfo_t		*term;

		AR_ASSERT(parser_context != NULL && !parser_context->is_accepted && tok != NULL);

		
		
		/*先压入扩展的产生式Start，只有其可以接受EOI*/
		
		if(parser_context->state_stack->count == 0)
		{
				PSR_PushStack(parser_context->state_stack, 0);
				PSR_PushNodeStack(parser_context->node_stack, NULL);
		}
		
		term = PSR_FindTermFromInfoTable(parser_context->parser->term_tbl, tok->term_val);

		
		/*
				移入进来的token必须在term_set中找到对应的symbol，
				否则这里应该在调用PSR_AddToken之前由词法分析器检测出错误来
		*/
		
		if(term == NULL)
		{
				AR_ASSERT(false);
				return false;
		}
		
		is_done = false;
		
		while(!is_done)
		{
				const psrAction_t		*action;
				size_t					top;
				top = PSR_TopStack(parser_context->state_stack);
				action = PSR_GetAction(parser_context->parser->tbl, top, term->term);
				AR_ASSERT(action != NULL);
				
				switch(action->type)
				{
				case PSR_SHIFT:
				{
						__handle_shift(parser_context, action->shift_to, tok, term);
						is_done = true;
				}
						break;
				case PSR_REDUCE:
				{
						__handle_reduce(parser_context, action);
				}
						break;
				case PSR_ACCEPT:
				{
						/*
								%start : first_rule EOI
								这是第一条rule %start，且不存在一个规约到%start的动作
						*/
						AR_ASSERT(PSR_CompSymb(term->term, PSR_EOISymb) == 0);
						AR_ASSERT(action->reduce_count == 1);
						AR_ASSERT(parser_context->state_stack->count == 2);
						
						if(parser_context->is_repair)parser_context->is_repair = false;/*这里相当于shift了EOI*/
						
						is_done = true;
						parser_context->is_accepted = true;
				}
						break;
				case PSR_ERROR:
				{
						errRecovery_t stat = __error_recovery(parser_context, tok);
						if(stat == ERR_RECOVERY_FAILED)
						{
								return false;
						}else if(stat == ERR_RECOVERY_DISCARD)
						{
								is_done = true;
						}else /*ERR_RECOVERY_CONTINUE*/
						{
								
						}
				}
						break;
				default:
				{
						AR_ASSERT(false);
						AR_error(AR_ERR_FATAL, L"%ls\r\n", L"parser internal error\r\n");
				}
				}
		}

		return true;
}


AR_NAMESPACE_END

