#include "parser_in.h"
#include "..\Lex\lex.h"













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








parser_t* PSR_CreateParser(const wchar_t *lex, const wchar_t *grammar, psrLeaf_func leaf_func, psrNode_func node_func, psrDestroy_func destroy_func, psrEngineType_t type)
{
		parser_t *parser;
		AR_ASSERT(lex != NULL && grammar != NULL && leaf_func != NULL && node_func != NULL && destroy_func != NULL);

		parser = AR_NEW0(parser_t);
		parser->leaf_func = leaf_func;
		parser->node_func = node_func;
		parser->destroy_func = destroy_func;
		parser->lex = LEX_CreateLex();
		
		if(LEX_Build(parser->lex, lex)  != LEX_NO_ERROR)
		{
				LEX_Destroy(parser->lex);
				AR_DEL(parser);
				return NULL;
		}

		parser->grammar = PSR_BuildGrammar(grammar);
		if(parser->grammar == NULL)
		{
				
				LEX_Destroy(parser->lex);
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










static bool_t __get_token(lex_t *lex, lexMatch_t *match, psrToken_t *tok)
{
		while(True)
		{
				if(!LEX_Match(lex, match))
				{
						if(match->err == LEX_RUN_EOI)
						{
								tok->lex_type = PSR_EOISymb->val;
								tok->line = match->x;
								tok->col = match->y;
								AR_WSTRCPY(tok->value, PSR_EOISymb->name);
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
										AR_WSTRNCPY(tok->value, match->token.tok, match->token.count);
										tok->value[match->token.count] = L'\0';
										tok->lex_type = match->token.type;
										tok->line = match->token.x;
										tok->col = match->token.y;
										return True;
								}
						}
				}
		}
}


const psrSymb_t* __calc_lookahead(parser_t *parser, lexMatch_t *match, psrToken_t *tok)
{
		const psrSymb_t *res;
		if(!__get_token(parser->lex, match, tok))return NULL;

		res = PSR_GetTermSymbByValue(parser->grammar, tok->lex_type);

		if(res == NULL)
		{
				AR_Error(AR_CRITICAL, L"Lex Error: %s : (%d,%d), %d\r\n", tok->value, tok->line, tok->col, tok->lex_type);
				return NULL;
		}
		return res;

}

psrNode_t* PSR_Parse(parser_t *parser, const wchar_t *sources)
{
		lexMatch_t		match;
		psrStack_t		stack;
		psrToken_t		token;
		psrNode_t		*root;
		psrStackFrame_t frame;
		psrNodeSet_t	node_set;
		const psrSymb_t			*lookahead;
		const psrAction_t		*action;
		psrStackFrame_t			*top;
		

		LEX_InitMatch(&match, sources);
		PSR_InitNodeSet(&node_set);
		PSR_InitStack(&stack);
		frame.node = NULL;
		frame.state = 0;
		PSR_PushStack(&stack, &frame);
		
		lookahead = __calc_lookahead(parser, &match, &token);
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
						frame.node = parser->leaf_func(&token);
						frame.state = action->shift_to;
						PSR_PushStack(&stack, &frame);
						lookahead = __calc_lookahead(parser, &match, &token);
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
										frame.node = parser->node_func(action->reduce_id, &node_set);
										PSR_PopStack(&stack, action->reduce_count);
								}else
								{
										frame.node = NULL;
								}
						}
						
						head = PSR_GetRuleHeadByRuleID(parser->grammar, action->reduce_id);
						AR_ASSERT(head != NULL);
						frame.state = PSR_GetState(parser->tbl, PSR_TopStack(&stack)->state, head);
						PSR_PushStack(&stack, &frame);
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
						AR_Error(AR_CRITICAL, L"Syntax Error : %s : (%d, %d)\r\n", token.value, token.line, token.col);
						goto INVALID_POINT;
				}
				}
		}
		
		AR_ASSERT(root != NULL);
		PSR_UnInitNodeSet(&node_set);
		PSR_UnInitStack(&stack);
		LEX_UnInitMatch(&match);
		return root;
INVALID_POINT:
		{
				size_t i;
				for(i = 0; i < stack.count; ++i)
				{
						if(stack.frame[i].node != NULL)
						{
								parser->destroy_func(stack.frame[i].node);
						}
				}
		}
		PSR_UnInitStack(&stack);
		LEX_UnInitMatch(&match);
		PSR_UnInitNodeSet(&node_set);
		return NULL;
}







