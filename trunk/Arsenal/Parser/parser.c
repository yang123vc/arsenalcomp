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



AR_NAMESPACE_BEGIN






/***************************************************辅助数据结构**********************************************/


typedef struct __parser_node_set_tag
{
		psrNode_t		**nodes;
		size_t			count;
		size_t			cap;
}psrNodeSet_t;

void PSR_InitNodeSet(psrNodeSet_t *nodes)
{
		AR_memset(nodes, 0, sizeof(*nodes));
}

void PSR_UnInitNodeSet(psrNodeSet_t *nodes)
{
		AR_DEL(nodes->nodes);
		AR_memset(nodes, 0, sizeof(*nodes));
}

void PSR_InsertToNodeSet(psrNodeSet_t *nodes, psrNode_t *node)
{
		if(nodes->count == nodes->cap)
		{
				nodes->cap = (nodes->cap + 1)*2;
				nodes->nodes = AR_REALLOC(psrNode_t*, nodes->nodes, nodes->cap);
		}

		nodes->nodes[nodes->count++] = node;
}

void PSR_ClearNodeSet(psrNodeSet_t *set)
{
		set->count = 0;
}





typedef struct __stack_frame_tag
{
		size_t			state;
		psrNode_t		*node;
}psrStackFrame_t;

typedef struct __parser_stack_tag
{
		psrStackFrame_t	 *frame;
		size_t			 count;
		size_t			 cap;
}psrStack_t;


void PSR_PushStack(psrStack_t *stack, psrStackFrame_t *frame)
{
		if(stack->count == stack->cap)
		{
				stack->cap = (stack->cap + 1)*2;
				stack->frame = AR_REALLOC(psrStackFrame_t, stack->frame, stack->cap);
		}

		stack->frame[stack->count++] = *frame;
}

void PSR_PopStack(psrStack_t *stack, size_t n)
{
		AR_ASSERT(n <= stack->count);
		stack->count -= n;
}

psrStackFrame_t* PSR_TopStack(psrStack_t *stack)
{
		if(stack->count == 0)return NULL;
		return &(stack->frame[stack->count-1]);
}


void PSR_InitStack(psrStack_t *stack)
{
		AR_memset(stack, 0,sizeof(*stack));

}
void PSR_UnInitStack(psrStack_t *stack)
{
		AR_DEL(stack->frame);
		AR_memset(stack, 0,sizeof(*stack));
}


/****************************************************Parser*****************************************************/


void	  PSR_Clear(parser_t *parser)
{
		size_t i;
		AR_ASSERT(parser != NULL && parser->stack != NULL);
		parser->is_inerr = false;
		parser->is_accepted = false;
		for(i = 0; i < parser->stack->count; ++i)
		{
				if(parser->stack->frame[i].node != NULL)
				{
						parser->ctx.free_f(parser->stack->frame[i].node, parser->ctx.ctx);
				}
		}

		PSR_ClearNodeSet(parser->node_set);
		PSR_UnInitStack(parser->stack);
		PSR_InitStack(parser->stack);
		
}


void	  PSR_DestroyParser(parser_t *parser)
{
		if(parser != NULL)
		{
				size_t i;
				
				for(i = 0; i < parser->term_count; ++i)
				{
						PSR_DestroySymb(parser->term_set[i]);
				}
				AR_DEL((psrSymb_t**)parser->term_set);

				PSR_UnInitStack(parser->stack);
				AR_DEL(parser->stack);

				PSR_UnInitNodeSet(parser->node_set);
				AR_DEL(parser->node_set);

				PSR_DestroyActionTable(parser->tbl);
				PSR_UnInitGrammar(parser->grammar);
				AR_DEL(parser->grammar);
				AR_DEL(parser);
		}
}



parser_t* PSR_CreateParser(const psrGrammar_t *grammar, psrModeType_t type, const psrCtx_t *ctx)
{
		parser_t *parser;
		AR_ASSERT(grammar != NULL && ctx != NULL);

		parser = AR_NEW0(parser_t);
		parser->grammar = AR_NEW0(psrGrammar_t);
		PSR_InitGrammar(parser->grammar);
		PSR_CopyGrammar(parser->grammar, grammar);
		parser->stack = AR_NEW0(psrStack_t);
		PSR_InitStack(parser->stack);

		parser->node_set = AR_NEW0(psrNodeSet_t);
		PSR_InitNodeSet(parser->node_set);

		parser->ctx = *ctx;
		AR_ASSERT( parser->ctx.free_f != NULL && parser->ctx.node_f != NULL && parser->ctx.leaf_f != NULL);
		
		parser->is_inerr = false;
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

		{
				int_t i,j;
				const psrSymbList_t *lst;
				lst = PSR_GetSymbList(grammar);
				parser->term_count = 0;
				parser->term_set = AR_NEWARR0(const psrSymb_t*, lst->count);

				for(i = 0; i < (int_t)lst->count; ++i)
				{
						if(lst->lst[i]->type == PSR_TERM)
						{
								parser->term_set[parser->term_count++] = PSR_CopyNewSymb(lst->lst[i]);
						}
				}

				for(i = 0; i < (int_t)parser->term_count; ++i)
				{
						for(j = i;j > 0; --j)
						{
								if(parser->term_set[j]->val < parser->term_set[j-1]->val)
								{
										const psrSymb_t *tmp = parser->term_set[j-1];
										parser->term_set[j-1] = parser->term_set[j];
										parser->term_set[j] = tmp;
								}
						}
				}
		}
		parser->is_accepted = false;
		return parser;
}

const psrSymb_t* __find_term_symb(const parser_t *parser, size_t val)
{
		int_t l,r,m,cmp;
		l = 0; r = parser->term_count-1;
		
		while(l <= r)
		{
				m = (l + r)/2;
				cmp = AR_CMP(parser->term_set[m]->val, val);

				if(cmp < 0)
				{
						l = m + 1;
				}else if(cmp == 0)
				{
						return parser->term_set[m];
				}else
				{
						r = m -1;
				}
		}

		return NULL;
}


psrNode_t* PSR_GetResult(parser_t *parser)/*在状态为accepted之后才可以调用*/
{
		psrNode_t *res;
		psrStackFrame_t *frame;
		AR_ASSERT(parser != NULL && parser->is_accepted);
		if(!parser->is_accepted)return NULL;
		AR_ASSERT(parser->stack->count == 2);
		frame = PSR_TopStack(parser->stack);
		res = frame->node;
		PSR_PopStack(parser->stack, 1);
		parser->is_accepted = false;
		AR_ASSERT(parser->stack->count == 1);
		return res;
}


psrError_t PSR_AddToken(parser_t *parser, const psrToken_t *tok)
{
		const psrSymb_t *lookahead;
		const psrStackFrame_t *top;
		const psrAction_t	  *action;
		
		psrError_t		err;
		bool			is_done;
		

		AR_ASSERT(parser != NULL && !parser->is_accepted && tok != NULL);
		
		
		err = PSR_ERR_OK;
		
		if(parser->stack->count == 0)
		{
				psrStackFrame_t frame = {0, NULL};
				PSR_PushStack(parser->stack, &frame);
		}
		
		is_done = false;
		
		while(!is_done)
		{
				psrStackFrame_t frame;

				//lookahead = PSR_FindTermByValue(parser->grammar, tok->tokval);
//				lookahead = __find_term_symb(parser, tok->tokval);
				lookahead = __find_term_symb(parser, tok->type);
				if(lookahead == NULL)
				{
						top = PSR_TopStack(parser->stack);
						action = (const psrAction_t*)PSR_ErrorAction;
				}else
				{
						top = PSR_TopStack(parser->stack);
						action = PSR_GetAction(parser->tbl, top->state, lookahead);
				}
				
				
				switch(action->type)
				{
				case PSR_SHIFT:
				{

						AR_ASSERT(action->shift_to < parser->tbl->row);
						frame.node = parser->ctx.leaf_f(tok,parser->ctx.ctx);
						frame.state = action->shift_to;
						PSR_PushStack(parser->stack, &frame);
						is_done = true;


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
						if(parser->is_inerr)parser->is_inerr = false;
				}
						break;
				case PSR_REDUCE:
				{
						size_t i;
						psrNode_t **nodes;
						PSR_ClearNodeSet(parser->node_set);

						for(i = 0; i < action->reduce_count; ++i)
						{
								psrNode_t *node;
								node = parser->stack->frame[parser->stack->count - action->reduce_count + i].node;
								PSR_InsertToNodeSet(parser->node_set, node);
						}
						
						if(parser->node_set->count > 0)
						{
								nodes = parser->node_set->count > 0 ? parser->node_set->nodes : NULL;
								frame.node = parser->ctx.node_f(action->rule_id, action->rule_name, nodes, parser->node_set->count, parser->ctx.ctx);
						}else
						{
								/*
										如果所在产生式所有符号都为空，则直接压入空
								*/
								frame.node = NULL;
						}
						
						PSR_PopStack(parser->stack, action->reduce_count);
						
						{
								size_t state;
								int_t next;
								state = PSR_TopStack(parser->stack)->state;
								next = PSR_GetState(parser->tbl, state, PSR_GetRuleByRuleID(parser->grammar, action->rule_id)->head);
								AR_ASSERT(next != -1);
								frame.state = (size_t)next;
								PSR_PushStack(parser->stack, &frame);
						}
						/*AR_ASSERT(!parser->is_inerr);*/
						/*if(parser->is_inerr)parser->is_inerr = false;*/
				}
						break;
				case PSR_ACCEPT:
				{
						AR_ASSERT(action->reduce_count == 1);
						err = PSR_ERR_ACCEPT;
						is_done = true;
						parser->is_accepted = true;
						/*这里相当于shift了EOI*/
						parser->is_inerr = false;
				}
						break;
				default:/*PSR_ERROR*/
				{
						if(!parser->is_inerr)
						{
								bool is_ok;
								is_ok = false;
								
								parser->ctx.error_f(tok, parser->ctx.ctx);
								
								while(parser->stack->count > 0 && !is_ok)
								{
										top = PSR_TopStack(parser->stack);
										action = PSR_GetAction(parser->tbl, top->state, PSR_ErrorSymb);
										AR_ASSERT(action != NULL);
										/*
										应该找到一个在error上有shift动作的状态
										*/
										if(action->type == PSR_SHIFT)
										{
												frame.node = NULL;
												frame.state = action->shift_to;
												PSR_PushStack(parser->stack, &frame);
												is_ok = true;
										}else
										{
												if(top->node != NULL)parser->ctx.free_f(top->node, parser->ctx.ctx);
												PSR_PopStack(parser->stack, 1);
										}
								}

								if(!is_ok)
								{
										err = PSR_ERR_INVALID_SYNTAX;
										is_done = true;
								}
								
								parser->is_inerr = true;
						}else
						{
/*
		在error状态中，任何不合法的输入都会丢弃，并且不显示任何错误信息;

*/
								if(lookahead != NULL)
								{
										err = (PSR_CompSymb(PSR_EOISymb, lookahead) == 0 ? PSR_ERR_INVALID_SYNTAX : PSR_ERR_OK);
								}else
								{
										err = PSR_ERR_OK;
								}
								is_done = true;
						}
				}
						break;
				}
		}

		PSR_ClearNodeSet(parser->node_set);
		return err;
}





AR_NAMESPACE_END

