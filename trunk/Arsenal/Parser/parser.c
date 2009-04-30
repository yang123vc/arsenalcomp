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

#include <windows.h>

#include "parser.h"
#include "grammar.h"
#include "lr_action.h"
#include "parser_aux.h"


AR_NAMESPACE_BEGIN








/****************************************************Parser*****************************************************/



parser_t* PSR_CreateParser(const psrGrammar_t *grammar, psrModeType_t type, const psrCtx_t *ctx)
{
		parser_t *parser;
		size_t i;
		AR_ASSERT(grammar != NULL && ctx != NULL);
		
		AR_ASSERT(ctx->error_f != NULL && ctx->free_f != NULL);

		parser = AR_NEW0(parser_t);
		parser->grammar = (psrGrammar_t*)grammar;
		
		parser->state_stack = AR_NEW0(psrStack_t);
		PSR_InitStack(parser->state_stack);
		
		parser->node_stack = AR_NEW0(psrNodeStack_t);
		PSR_InitNodeStack(parser->node_stack);
		
		parser->user = *ctx;
		parser->is_repair = false;
		parser->is_accepted = false;

		switch(type)
		{
		case PSR_SLR:
				parser->tbl = PSR_CreateActionTable_SLR(parser->grammar);
				break;
		case PSR_LR1:
				parser->tbl = PSR_CreateActionTable_LR1(parser->grammar);
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



void	  PSR_Clear(parser_t *parser)
{
		size_t i;
		AR_ASSERT(parser != NULL);

		parser->is_repair = false;
		parser->is_accepted = false;
		
		for(i = 0; i < parser->node_stack->count; ++i)
		{
				if(parser->node_stack->nodes[i])parser->user.free_f(parser->node_stack->nodes[i], parser->user.ctx);
		}

		PSR_ClearNodeStack(parser->node_stack);
		PSR_ClearStack(parser->state_stack);
}



void	  PSR_DestroyParser(parser_t *parser)
{
		if(parser != NULL)
		{
				size_t i;
				PSR_Clear(parser);

				for(i = 0; i < parser->msg_count; ++i)
				{
						PSR_UnInitExpectedMsg(&parser->msg_set[i]);
				}
				AR_DEL(parser->msg_set);

				PSR_DestroyTermInfoTable(parser->term_tbl);
				PSR_DestroyActionTable(parser->tbl);
				PSR_DestroyGrammar(parser->grammar);
				
				PSR_UnInitNodeStack(parser->node_stack);
				AR_DEL(parser->node_stack);
				
				PSR_UnInitStack(parser->state_stack);
				AR_DEL(parser->state_stack);
				AR_DEL(parser);
		}
}


static void __handle_reduce(parser_t *parser, const psrAction_t *action)
{
		psrNode_t **nodes;
		psrNode_t *new_node;
		size_t	  next_state;
		AR_ASSERT(parser != NULL && action != NULL && action->type == PSR_REDUCE);
		
		/*
		if(action->reduce_count > 0)
		{
				nodes = &parser->node_stack->nodes[parser->node_stack->count - action->reduce_count];
				new_node = action->rule->rule_f(nodes, action->reduce_count, parser->user.ctx);

		}else
		{
				new_node = NULL;	
		}
		*/

		nodes = &parser->node_stack->nodes[parser->node_stack->count - action->reduce_count];

		if(action->rule->rule_f != NULL)
		{
				new_node = action->rule->rule_f(nodes, action->reduce_count, action->rule->head->name, parser->user.ctx);
		}else
		{
				new_node = NULL;
		}

		if(action->reduce_count > 0)
		{
				PSR_PopStack(parser->state_stack, action->reduce_count);
				PSR_PopNodeStack(parser->node_stack, action->reduce_count);
		}
		
		next_state = PSR_GetState(parser->tbl, PSR_TopStack(parser->state_stack), action->rule->head);
		PSR_PushStack(parser->state_stack, next_state);
		PSR_PushNodeStack(parser->node_stack, new_node);
}



static void __handle_shift(parser_t *parser, size_t shift_to, const psrToken_t *tok, const psrTermInfo_t *term)
{
		psrNode_t		*new_node;
		
		AR_ASSERT(parser != NULL && tok != NULL && term != NULL && term->leaf_f != NULL);

		PSR_PushStack(parser->state_stack, shift_to);
		new_node = term->leaf_f(tok, parser->user.ctx);
		PSR_PushNodeStack(parser->node_stack, new_node);

/*
		将parser状态从error转回正常状态只有在当前状态上存在移入，例如：
		A -> error ";";
		A -> error;
		等

		第一条将在移入";"时候清除error状态，
		第二条将在error规约为A后的下一次正确输入时清楚，例如
		B -> A ";";
		这里假如A有任何错误，将在移入':'后清除规约状态；
*/
		if(parser->is_repair)parser->is_repair = false;
}

static bool __error_recovery(parser_t *parser, const psrToken_t *tok)
{		
		
		AR_ASSERT(parser != NULL);
		
		if(!parser->is_repair)
		{
				bool found;
				
				
				{
				size_t top_state;
				top_state = PSR_TopStack(parser->state_stack);
				AR_ASSERT(top_state < parser->msg_count);
				parser->user.error_f(tok, parser->msg_set[top_state].msg, parser->msg_set[top_state].count, parser->user.ctx);
				}

				found = false;

				while(parser->state_stack->count > 0 && !found)
				{
						size_t top;
						const psrAction_t *action;

						top = PSR_TopStack(parser->state_stack);
						action = PSR_GetAction(parser->tbl, top, PSR_ErrorSymb);
						
						AR_ASSERT(action != NULL);
						
/*
		应该找到一个在error上有shift动作的状态，之后移入NULL替代终结符error，如果存在规约，则需要先行规约(当然，规约后就是移入error了),如果error不是合法的输入，
		则向上pop stack和相关节点
*/
						if(action->type == PSR_REDUCE)
						{
								__handle_reduce(parser, action);
						}else if(action->type == PSR_ERROR)
						{
								psrNode_t *top_node;
								
								top_node = PSR_TopNodeStack(parser->node_stack);
								if(top_node)parser->user.free_f(top_node, parser->user.ctx);
								PSR_PopStack(parser->state_stack, 1);
								PSR_PopNodeStack(parser->node_stack, 1);
						
						}else if(action->type == PSR_SHIFT)
						{
								PSR_PushStack(parser->state_stack, action->shift_to);
								PSR_PushNodeStack(parser->node_stack, NULL);
								found = true;
						}else
						{
								/*只有EOI符号才可能导致一个accept动作*/
								AR_ASSERT(false);
						}
				}
				parser->is_repair = true;
				return found;
		}else
		{

/*
		在repair模式，任何不合法的输入都会丢弃，并且不显示任何错误信息，当输入为EOI时，则调用error_f报告已达EOI;
*/
				const psrSymb_t *symb;
				symb = PSR_FindTermFromInfoTable(parser->term_tbl, tok->type)->term;

				if(PSR_CompSymb(PSR_EOISymb, symb) == 0)
				{
						size_t top_state;
						top_state = PSR_TopStack(parser->state_stack);
						AR_ASSERT(top_state < parser->msg_count);
						parser->user.error_f(tok, parser->msg_set[top_state].msg, parser->msg_set[top_state].count, parser->user.ctx);
						return false;
				}else
				{
						return true;
				}
		}
}

bool PSR_AddToken(parser_t *parser, const psrToken_t *tok)
{

		bool					is_done;
		
		const psrTermInfo_t		*term;

		AR_ASSERT(parser != NULL && !parser->is_accepted && tok != NULL);
		
		/*先压入扩展的产生式Start，只有其可以接受EOI*/
		
		if(parser->state_stack->count == 0)
		{
				PSR_PushStack(parser->state_stack, 0);
				PSR_PushNodeStack(parser->node_stack, NULL);
		}
		
		term = PSR_FindTermFromInfoTable(parser->term_tbl, tok->type);
		
		/*
				移入进来的token必须在term_set中找到对应的symbol，
				否则这里应该在调用PSR_AddToken之前由词法分析器检测出错误来
		*/
		
		if(term == NULL)
		{
				AR_ASSERT(false);
				AR_error(AR_PARSER, L"invalid input token\r\n");
				return false;
		}
		
		is_done = false;
		
		while(!is_done)
		{
				const psrAction_t		*action;
				size_t					top;
				top = PSR_TopStack(parser->state_stack);
				action = PSR_GetAction(parser->tbl, top, term->term);
				AR_ASSERT(action != NULL);
				
				switch(action->type)
				{
				case PSR_SHIFT:
						__handle_shift(parser, action->shift_to, tok, term);
						is_done = true;
						break;
				case PSR_REDUCE:
						__handle_reduce(parser, action);
						break;
				case PSR_ACCEPT:
						AR_ASSERT(PSR_CompSymb(term->term, PSR_EOISymb) == 0);
						AR_ASSERT(action->reduce_count == 1);
						AR_ASSERT(parser->state_stack->count == 2);
						/*这里相当于shift了EOI*/
						if(parser->is_repair)parser->is_repair = false;
						
						is_done = true;
						parser->is_accepted = true;
						
						
						break;
				case PSR_ERROR:
						if(!__error_recovery(parser, tok))return false;
						break;
				default:
						AR_ASSERT(false);
				}
		}

		return true;
}


psrNode_t* PSR_GetResult(parser_t *parser)/*在状态为accepted之后才可以调用*/
{
		psrNode_t *res;
		AR_ASSERT(parser != NULL && parser->is_accepted);
		if(!parser->is_accepted)return NULL;
		AR_ASSERT(parser->state_stack->count == 2);
		res = PSR_TopNodeStack(parser->node_stack);
		
		PSR_PopStack(parser->state_stack, 1);
		PSR_PopNodeStack(parser->node_stack, 1);
		parser->is_accepted = false;
		return res;
}

bool	   PSR_IsAccepted(const parser_t *parser)
{
		AR_ASSERT(parser != NULL);
		return parser->is_accepted;
}

AR_NAMESPACE_END

