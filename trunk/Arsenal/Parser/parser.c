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
#include "parser_in.h"
#include "..\Lex\lex.h"


AR_NAMESPACE_BEGIN


typedef struct __parser_node_set_tag
{
		psrNode_t		**nodes;
		size_t			count;
		size_t			cap;
}psrNodeSet_t;

void PSR_InitNodeSet(psrNodeSet_t *nodes)
{
		AR_MSET0(nodes, sizeof(*nodes));
}

void PSR_UnInitNodeSet(psrNodeSet_t *nodes)
{
		AR_DEL(nodes->nodes);
		AR_MSET0(nodes, sizeof(*nodes));
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

void PSR_InitStack(psrStack_t *stack)
{
		AR_MSET0(stack, sizeof(*stack));
}
void PSR_UnInitStack(psrStack_t *stack)
{
		AR_DEL(stack->frame);
		AR_MSET0(stack, sizeof(*stack));
}

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


/****************************************************Parser*****************************************************/


void	  PSR_DestroyParser(parser_t *parser)
{
		if(parser != NULL)
		{
				PSR_DestroyActionTable(parser->tbl);
				PSR_DestroyGrammar(parser->grammar);
				AR_DEL(parser);
		}
}



parser_t* PSR_CreateParser(const wchar_t *grammar, psrEngineType_t type)
{
		parser_t *parser;
		AR_ASSERT(grammar != NULL);

		parser = AR_NEW0(parser_t);
		
		parser->grammar = PSR_BuildGrammar(grammar);
		if(parser->grammar == NULL)
		{
				AR_DEL(parser);
				return NULL;
		}
		
		switch(type)
		{
		case PSR_SLR:
				parser->tbl = PSR_BuildActionTable_SLR(parser->grammar);
				break;
		case PSR_LR1:
				parser->tbl = PSR_BuildActionTable_LR1(parser->grammar);
				break;
		default:
				parser->tbl = PSR_BuildActionTable_LALR(parser->grammar);
				break;
		}
		return parser;
}









#if(0)
static bool_t __get_token(lex_t *lex, lexMatch_t *match, psrToken_t *tok)
{
		while(True)
		{
				if(!LEX_Match(lex, match))
				{
						if(match->err == LEX_RUN_EOI)
						{
								/*
								tok->lex_type = PSR_EOISymb->val;
								tok->line = match->x;
								tok->col = match->y;
								AR_WSTRCPY(tok->value, PSR_EOISymb->name);
								*/
								tok->type = PSR_EOISymb->val;
								tok->tok = PSR_EOISymb->name;
								tok->count = AR_WSTRLEN(PSR_EOISymb->name);
								tok->x = match->x;
								tok->y = match->y;
								return True;
						}else
						{
								AR_Error(AR_CRITICAL, L"Lex Error: %s : (%d,%d), %d\r\n", match->next, match->x, match->y, match->err);
								return False;
						}
				}else
				{
						if(match->token.type == PSR_IGNORE_TOKEN_TYPE)/*如果是需要忽略的词法单元，则忽略掉*/
						{
								continue;
						}else
						{
								if(match->token.count > PSR_MAX_TOKENLEN)
								{
										AR_Error(AR_CRITICAL, L"Lex Error: %s : (%d,%d)\r\n", match->token.tok, match->token.x, match->token.y);
										return False;
								}else
								{
										*tok = match->token;
										/*
										AR_WSTRNCPY(tok->value, match->token.tok, match->token.count);
										tok->value[match->token.count] = L'\0';
										tok->lex_type = match->token.type;
										tok->line = match->token.x;
										tok->col = match->token.y;
										*/
										return True;
								}
						}
				}
		}
}
#endif


const psrSymb_t* __calc_lookahead(parser_t *parser, psrToken_t *tok, psrCtx_t *ctx)
{
		const psrSymb_t *res;
		
		if(!ctx->token_f(tok, ctx->ctx))return NULL;

		res = PSR_GetTermSymbByValue(parser->grammar, tok->type);

		if(res == NULL)
		{
				wchar_t buf[PSR_MAX_TOKENLEN];
				AR_WSTRNCPY(buf, tok->tok, tok->count);
				buf[tok->count] = L'\0';
				AR_Error(AR_CRITICAL, L"Syntax Error: %s : (%d,%d), %d\r\n", buf, tok->x, tok->y, tok->type);
				return NULL;
		}
		return res;

}


#if(0)
返回True表示可以继续，否则表示已经结束分析




static bool_t __handle_error(parser_t *parser, psrStack_t *stack, const psrSymb_t *lookahead, psrCtx_t *ctx)
{
		psrToken_t				token;
		psrNode_t				*root;
		psrStackFrame_t			frame;
		psrStack_t				err_stack;
		psrNodeSet_t			node_set;
		const psrAction_t		*action;
		psrStackFrame_t			*top;
		bool_t					is_ok;
		action == NULL;

		is_ok = False;
		while(stack->count != 0 && !is_ok)
		{
				top = &stack->frame[stack->count-1];
				action = PSR_GetAction(parser->tbl, top->state, PSR_ErrorSymb);
				AR_ASSERT(action != NULL);
				if(action->type != PSR_ERROR)
				{
						AR_ASSERT(action->type == PSR_SHIFT);
						frame.node = NULL;
						frame.state = action->shift_to;
						PSR_InitStack(&err_stack);
						PSR_PushStack(&err_stack, &frame);
						is_ok = True;
				}else
				{
						if(top->node != NULL)ctx->destroy_f(top->node);
						PSR_PopStack(stack, 1);
				}
		}

		if(!is_ok)return False;

		top = &err_stack.frame[err_stack.count-1];

		while(err_stack.count > 0)
		{
				action = PSR_GetAction(parser->tbl, top->state, lookahead);
				AR_ASSERT(action != NULL);
				
				switch(action->type)
				{
				case PSR_SHIFT:
				{
						frame.node = NULL;
						frame.state = action->shift_to;
						AR_ASSERT(action->shift_to < parser->tbl->row);
						PSR_PushStack(&err_stack, &frame);
						
						lookahead = __calc_lookahead(parser, &token, ctx);
						if(lookahead == NULL) return False;
						
						break;
				}
				case PSR_REDUCE:
				{

						break;
				}
				case PSR_ERROR:
				{
						lookahead = __calc_lookahead(parser, &token, ctx);
						if(PSR_CompSymb(lookahead, PSR_EOISymb) == 0)return False;
						break;
				}
				default:/*PSR_ACCEPT*/
						AR_ASSERT(0);
						return False;
				}
		}
}
#endif





psrNode_t* PSR_Parse(parser_t *parser, psrCtx_t *ctx)
{
		psrStack_t				stack;
		psrToken_t				token;
		psrNode_t				*root;
		psrStackFrame_t			frame;
		psrNodeSet_t			node_set;
		const psrSymb_t			*lookahead;
		const psrAction_t		*action;
		psrStackFrame_t			*top;
		bool_t					err_mode;
		AR_ASSERT(parser != NULL && ctx != NULL);

		;

		PSR_InitNodeSet(&node_set);
		PSR_InitStack(&stack);
		frame.node = NULL;
		frame.state = 0;
		PSR_PushStack(&stack, &frame);
		err_mode = False;
		lookahead = __calc_lookahead(parser, &token, ctx);
		if(lookahead == NULL)goto INVALID_POINT;
		root = NULL;
		
		while(root == NULL)
		{
				top = PSR_TopStack(&stack);
				AR_ASSERT(top != NULL);

				action = PSR_GetAction(parser->tbl, top->state, lookahead);
				AR_ASSERT(action != NULL);

				switch(action->type)
				{
				case PSR_SHIFT:
				{
						frame.node = ctx->leaf_f(&token,ctx->ctx);
						frame.state = action->shift_to;
						AR_ASSERT(action->shift_to < parser->tbl->row);
						PSR_PushStack(&stack, &frame);
						lookahead = __calc_lookahead(parser, &token, ctx);
						if(lookahead == NULL)
						{
								goto INVALID_POINT;
						}
				}		
						break;
				case PSR_REDUCE:
				{
						size_t i;
						const psrSymb_t *head;
						
						PSR_ClearNodeSet(&node_set);
						head = PSR_GetRuleHeadByRuleID(parser->grammar, action->reduce_id);
						AR_ASSERT(head != NULL);

						if(action->reduce_count == 0)/*如果为0则表明此次规约为产生式空*/
						{
								frame.node = NULL;
						}else
						{
								for(i = 0; i < action->reduce_count; ++i)
								{
										if(stack.frame[stack.count - action->reduce_count + i].node != NULL)
										{
												PSR_InsertToNodeSet(&node_set, stack.frame[stack.count - action->reduce_count + i].node);
										}
								}
								if(node_set.count > 0)
								{
										frame.node = ctx->node_f(action->reduce_id, head->name, node_set.nodes, node_set.count, ctx->ctx);
								}else
								{
										frame.node = NULL;
								}
						}
						
						PSR_PopStack(&stack, action->reduce_count);
						AR_ASSERT(PSR_TopStack(&stack)->state < parser->tbl->row);
						
						{
								size_t state;
								int next;
								state = PSR_TopStack(&stack)->state;
								next = PSR_GetState(parser->tbl, state, head);
								AR_ASSERT(next != -1);
								/*
								if(next == -1)
								{
										wchar_t buf[PSR_MAX_TOKENLEN];
										AR_WSTRNCPY(buf, token.tok, token.count);
										buf[token.count] = L'\0';
										AR_Error(AR_CRITICAL, L"Syntax Error : %s : (%d, %d)\r\n", buf, token.x, token.y);
										
										AR_ASSERT(0);

										goto INVALID_POINT;
								}else*/
								frame.state = (size_t)next;
								PSR_PushStack(&stack, &frame);
								
						}

						err_mode = False;
				}
						break;
				case PSR_ACCEPT:
				{
						AR_ASSERT(action->item.delim == 1);
						root = PSR_TopStack(&stack)->node;
				}
						break;	
				default:/*PSR_ERROR*/
				{
						/*
						wchar_t buf[PSR_MAX_TOKENLEN];
						AR_WSTRNCPY(buf, token.tok, token.count);
						buf[token.count] = L'\0';
						AR_Error(AR_CRITICAL, L"Syntax Error : %s : (%d, %d)\r\n", buf, token.x, token.y);
						goto INVALID_POINT;
						*/

						if(!err_mode)
						{
								bool_t is_ok;
								is_ok = False;

								while(stack.count != 0 && !is_ok)
								{
										top = &stack.frame[stack.count-1];
										action = PSR_GetAction(parser->tbl, top->state, PSR_ErrorSymb);
										AR_ASSERT(action != NULL);
										if(action->type != PSR_ERROR)
										{
												AR_ASSERT(action->type == PSR_SHIFT);
												frame.node = NULL;
												frame.state = action->shift_to;
												PSR_PushStack(&stack, &frame);
												is_ok = True;
										}else
										{
												if(top->node != NULL)ctx->destroy_f(top->node,ctx->ctx);
												PSR_PopStack(&stack, 1);
										}
								}

								//if(!is_ok)
								{
								wchar_t buf[PSR_MAX_TOKENLEN];
								AR_WSTRNCPY(buf, token.tok, token.count);
								buf[token.count] = L'\0';
								AR_Error(AR_CRITICAL, L"Syntax Error : %s : (%d, %d)\r\n", buf, token.x, token.y);
								}
								if(!is_ok)goto INVALID_POINT;
								err_mode = True;
						
						}else
						{
								if(PSR_CompSymb(lookahead, PSR_EOISymb) == 0)
								{
										goto INVALID_POINT;
								}else
								{
										lookahead = __calc_lookahead(parser, &token, ctx);
								}
						}
				}
				}
		}
		
		AR_ASSERT(root != NULL);
		PSR_UnInitNodeSet(&node_set);
		PSR_UnInitStack(&stack);
		return root;
INVALID_POINT:
		{
				size_t i;
				for(i = 0; i < stack.count; ++i)
				{
						if(stack.frame[i].node != NULL)
						{
								ctx->destroy_f(stack.frame[i].node,ctx->ctx);
						}
				}
		}
		PSR_UnInitStack(&stack);
		PSR_UnInitNodeSet(&node_set);
		return NULL;
}


AR_NAMESPACE_END


#if(0)
#endif




