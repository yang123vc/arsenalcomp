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
#include "lr_action.h"


AR_NAMESPACE_BEGIN





/***************************************************辅助数据结构**********************************************/



struct __parser_node_stack
{
		psrNode_t		**nodes;
		size_t			count;
		size_t			cap;
};

static AR_INLINE void Parser_InitNodeStack(psrNodeStack_t *stack)
{
		AR_memset(stack, 0, sizeof(*stack));

		stack->cap = 1024;
		stack->nodes = AR_REALLOC(psrNode_t*, stack->nodes, stack->cap);
}

static AR_INLINE void Parser_UnInitNodeStack(psrNodeStack_t *stack)
{
		AR_DEL(stack->nodes);
		AR_memset(stack, 0, sizeof(*stack));
}

static AR_INLINE void Parser_PushNodeStack(psrNodeStack_t *stack, psrNode_t *node)
{
		AR_ASSERT(stack != NULL);
		if(stack->count == stack->cap)
		{
				stack->cap = (stack->cap + 1)*2;
				stack->nodes = AR_REALLOC(psrNode_t*, stack->nodes, stack->cap);
		}
		stack->nodes[stack->count++] = node;
}

static AR_INLINE void Parser_PopNodeStack(psrNodeStack_t *stack, size_t n)
{
		AR_ASSERT(stack != NULL && n <= stack->count);
		stack->count -= n;
}

static AR_INLINE void Parser_ClearNodeStack(psrNodeStack_t *stack)
{
		AR_ASSERT(stack != NULL);
		stack->count = 0;
}

static AR_INLINE psrNode_t* Parser_TopNodeStack(psrNodeStack_t *stack)
{
		AR_ASSERT(stack != NULL && stack->count > 0);
		return stack->nodes[stack->count-1];
}





struct __parser_stack_tag
{
		size_t			 *states;
		size_t			 count;
		size_t			 cap;
};


static AR_INLINE void Parser_PushStack(psrStack_t *stack, size_t state)
{
		if(stack->count == stack->cap)
		{
				stack->cap = (stack->cap + 1)*2;
				stack->states = AR_REALLOC(size_t, stack->states, stack->cap);
		}
		stack->states[stack->count++] = state;
}


static AR_INLINE void Parser_PopStack(psrStack_t *stack, size_t n)
{
		AR_ASSERT(n <= stack->count);
		stack->count -= n;
}

static AR_INLINE size_t Parser_TopStack(const psrStack_t *stack)
{
		AR_ASSERT(stack->count > 0);
		return stack->states[stack->count-1];
}


static AR_INLINE void Parser_InitStack(psrStack_t *stack)
{
		AR_memset(stack, 0,sizeof(*stack));

		stack->cap = 1024;
		stack->states = AR_REALLOC(size_t, stack->states, stack->cap);


}

static AR_INLINE void Parser_UnInitStack(psrStack_t *stack)
{
		AR_DEL(stack->states);
		AR_memset(stack, 0,sizeof(*stack));
}


static AR_INLINE void Parser_ClearStack(psrStack_t *stack)
{
		AR_ASSERT(stack != NULL);
		stack->count = 0;
}





typedef struct __term_info_rec_tag
{
		const psrTermInfo_t	**terms;
		size_t	count;
		size_t	cap;
}psrTermInfoRec_t;

static AR_INLINE psrTermInfoRec_t* Parser_CreateTermInfoRec()
{
		return AR_NEW0(psrTermInfoRec_t);
}

static AR_INLINE void			Parser_DestroyTermInfoRec(psrTermInfoRec_t *rec)
{
		if(rec)
		{
				AR_DEL((psrTermInfo_t**)rec->terms);
				AR_DEL(rec);
		}
}

static AR_INLINE const psrTermInfo_t* Parser_FindTermFromRec(const psrTermInfoRec_t *rec, size_t val)
{
		size_t i;
		AR_ASSERT(rec != NULL);

		for(i = 0; i < rec->count; ++i)
		{
				if(rec->terms[i]->val == val)return rec->terms[i];
		}

		return NULL;
}

static AR_INLINE bool_t			Parser_InsertToTermInfoRec(psrTermInfoRec_t *rec, const psrTermInfo_t *term)
{
		AR_ASSERT(rec != NULL && term != NULL);
		if(Parser_FindTermFromRec(rec, term->val) != NULL)return false;

		if(rec->count == rec->cap)
		{
				rec->cap = (rec->cap + 4)*2;
				rec->terms = AR_REALLOC(const psrTermInfo_t*, (void*)rec->terms, rec->cap);
		}
		rec->terms[rec->count++] = term;
		return true;
}



#define TERM_BUCKET_SIZE		(256)


struct __term_table_tag
{
		psrTermInfoRec_t		*bucket[TERM_BUCKET_SIZE];
		size_t					item_count;
};

static AR_INLINE psrTermInfoTbl_t* Parser_CreateTermInfoTable()
{
		return AR_NEW0(psrTermInfoTbl_t);
}

static AR_INLINE void Parser_DestroyTermInfoTable(psrTermInfoTbl_t *tbl)
{
		size_t i;
		AR_ASSERT(tbl != NULL);
		
		for(i = 0; i < TERM_BUCKET_SIZE; ++i)
		{
				Parser_DestroyTermInfoRec(tbl->bucket[i]);
		}
		AR_DEL(tbl);
}

static AR_INLINE bool_t Parser_InsertToTermInfoTable(psrTermInfoTbl_t *tbl, const psrTermInfo_t *term)
{
		size_t idx;
		psrTermInfoRec_t		*rec;
		AR_ASSERT(tbl != NULL && term != NULL);

		idx = term->val % TERM_BUCKET_SIZE;

		if(tbl->bucket[idx] == NULL)
		{
				tbl->bucket[idx] = Parser_CreateTermInfoRec();
		}

		rec = tbl->bucket[idx];

		if(Parser_InsertToTermInfoRec(rec, term))
		{
				tbl->item_count++;
				return true;
		}else
		{
				return false;
		}
}

static AR_INLINE const psrTermInfo_t* Parser_FindTermFromInfoTable(const psrTermInfoTbl_t *tbl, size_t tokval)
{
		size_t idx;
		psrTermInfoRec_t		*rec;
		AR_ASSERT(tbl != NULL);
		idx = tokval % TERM_BUCKET_SIZE;
		
		rec = tbl->bucket[idx];

		if(rec == NULL)
		{
				return NULL;
		}else if(rec->count == 1)
		{
				return rec->terms[0];
		}else
		{
				return Parser_FindTermFromRec(rec, tokval);
		}
}




struct __expected_message_tag
{
		size_t			*msg;
		size_t			count;
};




static AR_INLINE void Parser_InitExpectedMsg(psrExpectedMsg_t *msg, const psrSymbList_t *lst, const psrGrammar_t *grammar)
{
		size_t i;
		AR_ASSERT(msg != NULL && lst != NULL && grammar != NULL);

		AR_memset(msg, 0, sizeof(*msg));
		if(lst->count == 0)return;
		
		msg->count = lst->count;
		
		msg->msg = AR_NEWARR(size_t,	msg->count);
		
		for(i = 0; i < msg->count; ++i)
		{
				const psrTermInfo_t		*term_info; 
				AR_ASSERT(lst->lst[i]->type == PARSER_TERM);
				term_info = Parser_GetTermSymbInfo(grammar, lst->lst[i]);
				AR_ASSERT(term_info != NULL);
				msg->msg[i] = term_info->val;
		}
}

static AR_INLINE void Parser_UnInitExpectedMsg(psrExpectedMsg_t *msg)
{
		AR_ASSERT(msg != NULL);
		if(msg->msg)AR_DEL((void*)msg->msg);
}


/****************************************************Parser*****************************************************/


const parser_t* Parser_CreateParser(const psrGrammar_t *grammar, psrModeType_t type)
{
		parser_t *parser;
		size_t i;
		AR_ASSERT(grammar != NULL);

		parser = AR_NEW0(parser_t);
		parser->ref_cnt = 1;

		parser->grammar = grammar;
		
		switch(type)
		{
		case PARSER_SLR:
				parser->tbl = Parser_CreateActionTable_SLR(parser->grammar);
				break;
		default:
				parser->tbl = Parser_CreateActionTable_LALR(parser->grammar);
				break;
		}

		parser->msg_count = parser->tbl->row;
		parser->msg_set = AR_NEWARR0(psrExpectedMsg_t, parser->msg_count);

		for(i = 0; i < parser->msg_count; ++i)
		{
				Parser_InitExpectedMsg(&parser->msg_set[i], Parser_GetExpectedSymb(parser->tbl, i), parser->grammar);
		}

		parser->term_tbl = Parser_CreateTermInfoTable();
		
		{
				const psrTermInfoList_t *tlst = &parser->grammar->term_list;
				
				for(i = 0; i < tlst->count; ++i)
				{
						Parser_InsertToTermInfoTable(parser->term_tbl, &tlst->lst[i]);
				}
		}
		
		return parser;
}

const	psrGrammar_t*	Parser_GetGrammar(const parser_t *parser)
{
		AR_ASSERT(parser != NULL && parser->grammar != NULL);
		return parser->grammar;
}

void	  Parser_DestroyParser(const parser_t *parser)
{
		parser_t		*psr = (parser_t*)parser;
		AR_ASSERT(parser != NULL);
		AR_ASSERT(psr->ref_cnt == 1);

		if(psr != NULL)
		{
				size_t i;

				for(i = 0; i < psr->msg_count; ++i)
				{
						Parser_UnInitExpectedMsg(&psr->msg_set[i]);
				}
				AR_DEL(psr->msg_set);

				Parser_DestroyTermInfoTable(psr->term_tbl);
				Parser_DestroyActionTable(psr->tbl);
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
psrContext_t*	Parser_CreateContext(const parser_t *parser, void *ctx)
{
		psrContext_t	*psr_ctx = NULL;
		AR_ASSERT(parser != NULL);
		AR_ASSERT(parser->ref_cnt > 0);

		psr_ctx = AR_NEW0(psrContext_t);
		psr_ctx->ctx = ctx;
		psr_ctx->parser = parser;
		psr_ctx->is_repair = false;
		psr_ctx->is_accepted = false;
		psr_ctx->state_stack = AR_NEW0(psrStack_t);
		Parser_InitStack(psr_ctx->state_stack);
		psr_ctx->node_stack = AR_NEW0(psrNodeStack_t);
		Parser_InitNodeStack(psr_ctx->node_stack);

		AR_AtomicInc((int_t*)&(((parser_t*)parser)->ref_cnt));
		return psr_ctx;
}

void	  Parser_Clear(psrContext_t *parser_context)
{
		size_t i;
		const psrHandler_t *handler;
		AR_ASSERT(parser_context != NULL && parser_context->parser != NULL);

		handler = Parser_GetGrammarHandler(Parser_GetGrammar(parser_context->parser));
		AR_ASSERT(handler != NULL);
		parser_context->is_repair = false;
		parser_context->is_accepted = false;
		
		for(i = 0; i < parser_context->node_stack->count; ++i)
		{
				if(parser_context->node_stack->nodes[i])handler->free_f(parser_context->node_stack->nodes[i], parser_context->ctx);
		}

		Parser_ClearNodeStack(parser_context->node_stack);
		Parser_ClearStack(parser_context->state_stack);
}

void			Parser_DestroyContext(psrContext_t	*parser_context)
{
		AR_ASSERT(parser_context != NULL);
		AR_ASSERT(parser_context->parser->ref_cnt > 1);
		AR_AtomicDec((int_t*)&(((parser_t*)(parser_context->parser))->ref_cnt));

		Parser_Clear(parser_context);
		Parser_UnInitNodeStack(parser_context->node_stack);
		Parser_UnInitStack(parser_context->state_stack);
		AR_DEL(parser_context->node_stack);
		AR_DEL(parser_context->state_stack);
		AR_DEL(parser_context);

}


psrNode_t*	Parser_GetResult(psrContext_t *parser_context)
{
		psrNode_t *res;
		AR_ASSERT(parser_context != NULL);
		AR_ASSERT(parser_context->is_accepted);
		if(!parser_context->is_accepted)return NULL;
		AR_ASSERT(parser_context->state_stack->count == 2);
		res = Parser_TopNodeStack(parser_context->node_stack);
		
		Parser_PopStack(parser_context->state_stack, 1);
		Parser_PopNodeStack(parser_context->node_stack, 1);
		parser_context->is_accepted = false;
		return res;
}

bool_t	   Parser_IsAccepted(const psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		return parser_context->is_accepted;
}


bool_t	Parser_IsRecovering(const psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		return parser_context->is_repair;

}

void	Parser_RecoverDone(psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		parser_context->is_repair = false;
}

size_t			Parser_GetNodeCount(const psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		AR_ASSERT(parser_context->node_stack->count == parser_context->state_stack->count);
		return parser_context->node_stack->count;
}

psrNode_t*		Parser_IndexOfNodeStack(psrContext_t *parser_context, size_t index)
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

		Parser_PushStack(parser_context->state_stack, shift_to);

		if(term->leaf_f)
		{
				new_node = term->leaf_f(tok, parser_context->ctx);
		}else
		{
				new_node = NULL;
		}
		Parser_PushNodeStack(parser_context->node_stack, new_node);

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
		
		AR_ASSERT(parser_context != NULL && action != NULL && action->type == PARSER_REDUCE);

		rule = Parser_GetRuleOfGrammar(Parser_GetGrammar(parser_context->parser), action->rule_num);
		

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
				Parser_PopStack(parser_context->state_stack, action->reduce_count);

				{
						size_t i;
						const psrHandler_t	*handler = Parser_GetGrammarHandler(Parser_GetGrammar(parser_context->parser));
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

				Parser_PopNodeStack(parser_context->node_stack, action->reduce_count);
		}
		
		next_state = Parser_GetState(parser_context->parser->tbl, Parser_TopStack(parser_context->state_stack), rule->head);
		Parser_PushStack(parser_context->state_stack, next_state);
		Parser_PushNodeStack(parser_context->node_stack, new_node);
}




static void __on_error(psrContext_t *parser_context, const psrToken_t		*tok)
{
		size_t			top_state;
		const psrHandler_t		*handler;
		
		AR_ASSERT(parser_context != NULL && tok != NULL);
		
		handler = Parser_GetGrammarHandler(Parser_GetGrammar(parser_context->parser));
		

		AR_ASSERT(handler != NULL);
		
		top_state = Parser_TopStack(parser_context->state_stack);
		
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
		
		handler = Parser_GetGrammarHandler(Parser_GetGrammar(parser_context->parser));
		AR_ASSERT(handler != NULL);

		if(!parser_context->is_repair)
		{
				bool_t found = false;
				
				__on_error(parser_context, tok);

				while(parser_context->state_stack->count > 0 && !found)
				{
						size_t top;
						const psrAction_t *action;

						top = Parser_TopStack(parser_context->state_stack);
						action = Parser_GetAction(parser_context->parser->tbl, top, PARSER_ErrorSymb);
						
						AR_ASSERT(action != NULL);
						
/*
		应该找到一个在error上有shift动作的状态，之后移入NULL替代终结符error，如果存在规约，则需要先行规约(当然，规约后就是移入error了),如果error不是合法的输入，
		则向上pop stack和相关节点
*/
						if(action->type == PARSER_REDUCE)
						{
								__handle_reduce(parser_context, action);
						}else if(action->type == PARSER_ERROR)
						{
								psrNode_t *top_node;
								
								top_node = Parser_TopNodeStack(parser_context->node_stack);
								if(top_node != NULL)handler->free_f(top_node, parser_context->ctx);
								Parser_PopNodeStack(parser_context->node_stack, 1);
								Parser_PopStack(parser_context->state_stack, 1);
								
						
						}else if(action->type == PARSER_SHIFT)
						{
								Parser_PushStack(parser_context->state_stack, action->shift_to);
								Parser_PushNodeStack(parser_context->node_stack, NULL);
								found = true;
						}else /*PARSER_ACCEPT*/
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
				symb = Parser_FindTermFromInfoTable(parser_context->parser->term_tbl, tok->term_val)->term;
				AR_ASSERT(symb != NULL);
				if(Parser_CompSymb(PARSER_EOISymb, symb) == 0)
				{
						__on_error(parser_context, tok);

						return ERR_RECOVERY_FAILED;
				}else
				{
						return ERR_RECOVERY_DISCARD;
				}
		}
}


bool_t		Parser_AddToken(psrContext_t *parser_context, const psrToken_t *tok)
{

		bool_t					is_done;
		const psrTermInfo_t		*term;

		AR_ASSERT(parser_context != NULL && !parser_context->is_accepted && tok != NULL);

		
		
		/*先压入扩展的产生式Start，只有其可以接受EOI*/
		
		if(parser_context->state_stack->count == 0)
		{
				Parser_PushStack(parser_context->state_stack, 0);
				Parser_PushNodeStack(parser_context->node_stack, NULL);
		}
		
		term = Parser_FindTermFromInfoTable(parser_context->parser->term_tbl, tok->term_val);

		
		/*
				移入进来的token必须在term_set中找到对应的symbol，
				否则这里应该在调用Parser_AddToken之前由词法分析器检测出错误来
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
				top = Parser_TopStack(parser_context->state_stack);
				action = Parser_GetAction(parser_context->parser->tbl, top, term->term);
				AR_ASSERT(action != NULL);
				
				switch(action->type)
				{
				case PARSER_SHIFT:
				{
						__handle_shift(parser_context, action->shift_to, tok, term);
						is_done = true;
				}
						break;
				case PARSER_REDUCE:
				{
						__handle_reduce(parser_context, action);
				}
						break;
				case PARSER_ACCEPT:
				{
						/*
								%start : first_rule EOI
								这是第一条rule %start，且不存在一个规约到%start的动作
						*/
						AR_ASSERT(Parser_CompSymb(term->term, PARSER_EOISymb) == 0);
						AR_ASSERT(action->reduce_count == 1);
						AR_ASSERT(parser_context->state_stack->count == 2);
						
						if(parser_context->is_repair)parser_context->is_repair = false;/*这里相当于shift了EOI*/
						
						is_done = true;
						parser_context->is_accepted = true;
				}
						break;
				case PARSER_ERROR:
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
						AR_error(AR_ERR_FATAL, L"%ls\r\n", L"Arsenal parser internal error");
				}
				}
		}

		return true;
}


AR_NAMESPACE_END

