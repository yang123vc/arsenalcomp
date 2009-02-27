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

/********************************************************************************************/
void			PSR_DestroyGrammar(psrGrammar_t* grammar)
{
		size_t i;
		for(i = 0; i < grammar->count; ++i)
		{
				PSR_DestroySymbList(grammar->head[i]);
				PSR_DestroySymbList(grammar->body[i]);
		}


		for(i = 0; i < grammar->non_count; ++i)
		{
				PSR_DestroySymbList(grammar->nonterm_set[i]);
				PSR_DestroySymbList(grammar->first[i]);
				PSR_DestroySymbList(grammar->follow[i]);
		}

		for(i = 0; i < grammar->term_count; ++i)
		{
				PSR_DestroySymbList(grammar->term_set[i]);
		}
		
		AR_DEL(grammar->term_set);
		AR_DEL(grammar->head);
		AR_DEL(grammar->body);
		AR_DEL(grammar->nonterm_set);
		AR_DEL(grammar->first);
		AR_DEL(grammar->follow);
		AR_DEL(grammar);
}





static int __get_firstset_idx_by_symb(const psrGrammar_t *grammar,const psrSymb_t *symb)
{
		size_t i;
		AR_ASSERT(symb->non_term);
		for(i = 0; i < grammar->non_count; ++i)
		{
				if(PSR_CompSymb(grammar->nonterm_set[i], symb) == 0)
				{
						return (int)i;
				}
		}
		return -1;

}

#define __get_followset_idx_by_symb		__get_firstset_idx_by_symb


/**********************************************************************************************************/

static int __find_token_form_tokset(const psrGrammar_t *grammar, const psrSymb_t *token)
{
		size_t i;
		for(i = 0; i < grammar->term_count; ++i)
		{
				if(PSR_CompSymb(grammar->term_set[i], token) == 0)return (int)i;
		}
		return -1;
}


static int __find_token_by_name_form_tokset(const psrGrammar_t *grammar, const wchar_t *name)
{
		size_t i;
		for(i = 0; i < grammar->term_count; ++i)
		{
				if(AR_WSTRCMP(grammar->term_set[i]->name, name) == 0)return (int)i;
		}
		return -1;
}



static bool_t __insert_token_to_grammar(psrGrammar_t *grammar, const psrSymb_t *token)
{
		AR_ASSERT(!token->non_term);

		if(__find_token_form_tokset(grammar, token) != -1)return False;

		if(grammar->term_count == grammar->term_cap)
		{
				size_t i;
				grammar->term_cap += 20;
				grammar->term_set = AR_REALLOC(psrSymb_t*, grammar->term_set, grammar->term_cap);
				
				for(i = grammar->term_count; i < grammar->term_cap; ++i)
				{
						grammar->term_set[i] = NULL;
				}
		}
		grammar->term_set[grammar->term_count++] = PSR_CopyNewSymb(token);
		return True;
}

static void __init_default_tokset(psrGrammar_t *grammar)
{
		/*
				extern const psrSymb_t	*PSR_EpsilonSymb;
				extern const psrSymb_t	*PSR_EOISymb;
		*/
		__insert_token_to_grammar(grammar, PSR_EpsilonSymb);
		__insert_token_to_grammar(grammar, PSR_EOISymb);

}





const psrSymb_t* PSR_GetFirstSetBySymb(const psrGrammar_t *grammar,const psrSymb_t *symb)
{
		int i;
		if(!symb->non_term)
		{
				return grammar->term_set[__find_token_form_tokset(grammar, symb)];
		}

		i = __get_firstset_idx_by_symb(grammar, symb);

		if(i == -1)return NULL;

		return grammar->first[i];
		
}

const psrSymb_t* PSR_GetFollowSetBySymb(const psrGrammar_t *grammar,const psrSymb_t *symb)
{
		int i;
		AR_ASSERT(symb->non_term);
		i = __get_followset_idx_by_symb(grammar, symb);
		if(i == -1)return NULL;
		return grammar->follow[i];
}



const psrSymb_t* PSR_GetRuleBodyByRuleID(const psrGrammar_t *grammar,size_t id)
{
		size_t i;
		for(i = 0; i < grammar->count; ++i)
		{
				if(grammar->head[i]->id == id)
				{
						return grammar->body[i];
				}
		}
		return NULL;
}

const psrSymb_t* PSR_GetRuleHeadByRuleID(const psrGrammar_t *grammar,size_t id)
{
		size_t i;
		for(i = 0; i < grammar->count; ++i)
		{
				if(grammar->head[i]->id == id)
				{
						return grammar->head[i];
				}
		}
		return NULL;
}

const psrSymb_t* PSR_GetTermSymbByValue(const psrGrammar_t *grammar,size_t value)
{
		size_t i;
		for(i = 0; i < grammar->term_count; ++i)
		{
				if(grammar->term_set[i]->val == value)
				{
						return grammar->term_set[i];
				}
		}
		return NULL;
}



static void __calc_first(psrGrammar_t* grammar)
{
		size_t i;
		bool_t changed;
		
		AR_ASSERT(grammar != NULL && grammar->count > 1 && grammar->non_count > 1);
		changed = True;
		while(changed)
		{
				changed = False;
				
				for(i = 0; i < grammar->count; ++i)/*对每一条产生式*/
				{
						const psrSymb_t  *body;/*产生式体*/
						psrSymb_t  **dest;
						bool_t need_continue;	/*是否需要继续*/
						
						body = grammar->body[i];															/*产生式head[i]的体为body[i]*/
						/*
								函数__get_firstset_idx_by_symb将得到产生式grammar->head[i]对应的first集合的位置
						*/
						dest = &grammar->first[__get_firstset_idx_by_symb(grammar, grammar->head[i])];		
						
						AR_ASSERT(body != NULL);/*不可能为空，至少会有一个Epsilon或者EOI*/
						need_continue = True;

						/*
						firstset求法为，例如A->X(0)...X(n-1);
						设First(Term) = {Term}; First(Epsilon) = {Epsilon}; i = 0;
						如果First(X(i))存在{Epsilon}，则 First(A) += (First(X(i) - {Epsilon}); 如果i == n-1，则First(A) += {Epsilon}，本次循环终止；
						如果不存在{Epsilon}，则循环终止；

						因为每次都会处理至少一个符号，所以能确保循环终止，除非无任何符号加入到First集合中(changed == False);
						*/
						while(need_continue && body != NULL)
						{
								const psrSymb_t *first;
								
								need_continue = False;
								
								first = PSR_GetFirstSetBySymb(grammar, body);/*得到*/

								while(first != NULL)
								{
										AR_ASSERT(first->non_term == False);/*first不可能为非终结符*/
										if(PSR_IsNullable(first))
										{
												need_continue = True;
										}else
										{
												if(PSR_InsertSymb_Unique(dest, first))/*如果不存在符号first，则加入到dest中，*/
												{
														changed = True;/*任何一步的改动都需要重新计算first集合*/
												}
										}
										first = first->next;
								}
								body = body->next;
						}
						
						if(need_continue && PSR_InsertSymb_Unique(dest, PSR_EpsilonSymb))/*如果最后一步的计算仍包含空，则将空加入到集合First(A(i))中*/
						{
								changed = True;
						}
				}
		}
		
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

static void __calc_follow(psrGrammar_t* grammar)
{
		size_t i;
		bool_t changed;
		
		AR_ASSERT(grammar != NULL && grammar->count > 1 && grammar->non_count > 1);
		changed = True;
		while(changed)
		{
				changed = False;
				
				for(i = 0; i < grammar->count; ++i)/*对每一条产生式*/
				{
						const psrSymb_t  *key;/*产生式体*/
						const psrSymb_t  *head_follow; /*grammar->head[i]所对应的follow集合*/
						
						key = grammar->body[i];
						head_follow = head_follow = PSR_GetFollowSetBySymb(grammar, grammar->head[i]);
						
						while(key != NULL)
						{
								const psrSymb_t *body;
								psrSymb_t		**dest;
								bool_t need_continue;	/*是否需要继续*/

								if(!key->non_term)
								{
										key = key->next;
										continue;
								}
								
								dest  = &grammar->follow[__get_followset_idx_by_symb(grammar, key)];/*函数__get_followset_idx_by_symb将求得非终结符body对应followset的位置*/
								body = key->next;
								need_continue = True;
								
								while(need_continue && body != NULL)
								{
										const psrSymb_t *first;
										
										need_continue = False;

										first = PSR_GetFirstSetBySymb(grammar, body);
										while(first != NULL)
										{
												AR_ASSERT(first->non_term == False);/*first不可能为非终结符*/
												if(PSR_IsNullable(first))
												{
														need_continue = True;
												}else
												{
													if(PSR_InsertSymb_Unique(dest, first))/*如果不存在符号follow，则加入到dest中，*/
													{
															changed = True;/*任何一步的改动都需要重新计算follow集合*/
													}
												}
												first = first->next;
										}
								}
								
								if(need_continue)
								{
										const psrSymb_t *phead;

										phead = head_follow;

										while(phead != NULL)
										{
												AR_ASSERT(!phead->non_term &&!PSR_IsNullable(phead));/*follow集合中不可能有非终结符和空*/
												
												if(PSR_InsertSymb_Unique(dest, phead))/*如果不存在符号first，则加入到dest中，*/
												{
															changed = True;/*任何一步的改动都需要重新计算first集合*/
												}
												phead = phead->next;
										}
								}
								key = key->next;
						}
				}

		}
}






static void __insert_rule_to_grammar(psrGrammar_t *grammar, const psrSymb_t *head, const psrSymb_t *body)
{

		if(grammar->count == grammar->cap)
		{
				grammar->cap += 10;
				grammar->head = AR_REALLOC(psrSymb_t*, grammar->head, grammar->cap);
				grammar->body = AR_REALLOC(psrSymb_t*, grammar->body, grammar->cap);
				
				{
						size_t i;
						for(i = grammar->count; i < grammar->cap; ++i)
						{
								grammar->head[i] = NULL;
								grammar->body[i] = NULL;
						}

				}
				
		}
		AR_ASSERT(head->non_term);
		PSR_InsertSymb(&grammar->head[grammar->count], head);

		while(body != NULL)
		{
				PSR_InsertSymb(&grammar->body[grammar->count], body);
				body = body->next;
		}
		
		grammar->count++;
}


static void __calc_grammar(psrGrammar_t *grammar)
{
		size_t i;
		psrSymb_t **dest;
		AR_ASSERT(grammar->count > 1);
		grammar->body[0] = PSR_CopyNewSymb(grammar->head[1]);/*初始化%Start为第一个被插入的产生式头*/
		

		grammar->nonterm_set = AR_NEWARR0(psrSymb_t*, grammar->count);
		grammar->first = AR_NEWARR0(psrSymb_t*, grammar->count);
		grammar->follow = AR_NEWARR0(psrSymb_t*, grammar->count);

		for(i = 0; i < grammar->count; ++i)
		{
				const psrSymb_t *nonterm = grammar->head[i];
				
				if(__get_firstset_idx_by_symb(grammar,nonterm) == -1)
				{
						grammar->nonterm_set[grammar->non_count++] = PSR_CopyNewSymb(nonterm);
				}
		}
		
		
		dest = &grammar->follow[__get_followset_idx_by_symb(grammar, PSR_StartSymb)];

		AR_ASSERT(*dest == NULL);
		PSR_InsertSymb(dest, PSR_EOISymb);
		
		__calc_first(grammar);
		
		__calc_follow(grammar);

		AR_ASSERT(grammar->symb_list == NULL);

		for(i = 0; i < grammar->non_count; ++i)
		{
				PSR_InsertSymb_Unique(&grammar->symb_list, grammar->nonterm_set[i]);
		}

		for(i = 0; i < grammar->term_count; ++i)
		{
				PSR_InsertSymb_Unique(&grammar->symb_list, grammar->term_set[i]);
		}
}


static const wchar_t *__gmr_pattern =
LEX_DEF("delim = [\\r\\n\\t ]*")
LEX_DEF("letter = [A-Z_a-z]")
LEX_DEF("digit = [0-9]")
LEX_DEF("number = 0|[1-9]{digit}*")
LEX_DEF("name  = {letter}({letter}|{digit})*")
LEX_DEF("lexeme  = ({name})|('[^']*')|(\\\"[^\\\"]\\\")")
LEX_DEF("assign   = (\":\")|(\"=\")|(\"->\")")
LEX_DEF("rule_sep = \\|")
LEX_DEF("end_char = \";\"")
LEX_DEF("sep_char = \",\"")
LEX_DEF("epsilon_char = \".\"")
LEX_DEF("tokdef_begin   = \"%token{\"")
LEX_DEF("prec_begin   = \"%prec{\"")
LEX_DEF("rules_begin   = \"%rules{\"")
LEX_DEF("end   = \"%}\"")
LEX_DEF("asso_def = \"%\"(left|right|noassoc)")
LEX_DEF("prec_def = \"%prec\"")
LEX_DEF("comment   = \\/\\*([^\\\\\\*]|\\*+[^\\*\\/])*\\*+\\/")
LEX_DEF("start_id_def = \"%id\"")
LEX_DEF("(0) {delim}")
LEX_DEF("(1) {assign}")
LEX_DEF("(2) {rule_sep}")
LEX_DEF("(3) ({end_char}|{sep_char})")
LEX_DEF("(4) {tokdef_begin}")
LEX_DEF("(5) {prec_begin}")
LEX_DEF("(6) {rules_begin}")
LEX_DEF("(7) {end}")
LEX_DEF("(8) {comment}")
LEX_DEF("(9) {lexeme}")
LEX_DEF("(10) {number}")
LEX_DEF("(11) {asso_def}")
LEX_DEF("(12) {prec_def}")
LEX_DEF("(13) {epsilon_char}")
LEX_DEF("(14) {start_id_def}")
;


typedef enum
{
		DELIM = 0,
		ASSIGN = 1,
		RULE_SEP = 2,
		END_SEP_CHAR = 3,
		TOK_DEF_BEGIN = 4,
		PREC_DEF_BEGIN =  5,
		RULES_DEF_BEGIN = 6,
		DEF_END = 7,
		COMMENT = 8,
		LEXEME = 9,
		NUMBER = 10,
		ASSOC_DEF = 11,
		PREC_DEF = 12,
		EPSILON_CHAR = 13,
		START_ID = 14
}psrGrammarTokenType_t;



typedef struct __psr_grammar_token_tag
{
		wchar_t			name[PARSER_MAXNAME];
		int				type;
		size_t			line;
		size_t			col;
}psrGrammarToken_t;


#define __GMR_WARNING(_tok, _line, _col, _code) do{AR_Error(AR_WARNING, L"Grammar Warning : %s : line %d : col %d : error code %d\n", (_tok), (_line), (_col), (_code));}while(0)
#define __GMR_ERROR(_tok, _line, _col, _code)do{AR_Error(AR_CRITICAL, L"Grammar Error :  %s : line %d : col %d : error code %d\n", (_tok), (_line), (_col), (_code));}while(0)


static bool_t  __get_token(lex_t *lex, lexMatch_t *match, psrGrammarToken_t *tok, psrGrammarTokenType_t type)
{
		lexError_t g_err;
		
		g_err = LEX_NO_ERROR;

__GMR_LOOP__:
		if(!LEX_Match(lex, match))
		{
				return False;
		}else if(match->token.type == DELIM || match->token.type == COMMENT)
		{
				goto __GMR_LOOP__;
		}else 
		{
				
				if(match->token.count > PSR_MAX_NAMELEN)
				{
						__GMR_WARNING(match->token.tok, match->token.x, match->token.y, PSR_INVALID_NAMELEN);
				}

				AR_WSTRNCPY(tok->name, match->token.tok, match->token.count);
				tok->name[match->token.count] = L'\0';
				tok->type = (int)match->token.type;
				tok->line = match->token.x;
				tok->col = match->token.y;

				return (tok->type == type ? True : False);
		}
		
}







static bool_t __handle_token_def(lex_t *lex, lexMatch_t *match, psrGrammar_t *grammar)
{		
		psrGrammarToken_t tok;
		psrError_t g_err;
		lexError_t input_err;
		psrSymb_t  symbol;
		wchar_t	   name[PARSER_MAXNAME];
		size_t	   val;
		const	wchar_t *p;

		g_err = PSR_NO_ERROR;
		input_err = LEX_NO_ERROR;
		
		
		while(__get_token(lex, match, &tok, LEXEME))
		{
				AR_WSTRCPY(name, tok.name);


				if(!__get_token(lex, match, &tok, ASSIGN))
				{
						__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_TOKEN);
						return False;
				}
				
				if(!__get_token(lex, match, &tok, NUMBER))
				{
						__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_TOKEN);
						return False;
				}

				
				p = tok.name; val = 0;
				while(*p != L'\0')
				{
						val *= 10;
						val += (*p - L'0');
						++p;
				}
				
				

				g_err = PSR_SetSymb(&symbol, name, False, PSR_MIN_PRIOVAL, PSR_ASSO_NOASSO, val);
				
				if(g_err != PSR_NO_ERROR)
				{
						__GMR_ERROR(tok.name, tok.line, tok.col, g_err);
						return False;
				}

				if(!__insert_token_to_grammar(grammar, &symbol))
				{
						__GMR_ERROR(tok.name, tok.line, tok.col, PSR_DUPLICATE_TOKEN_DEFINE);
						return False;
				}
				
				if(!__get_token(lex, match, &tok, END_SEP_CHAR))
				{
						__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_TOKEN);		
						return False;
				}
		}
		
		if(match->err != LEX_NO_ERROR)
		{
				__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_GRAMMAR_PATTERN);
				return False;
		}else if(tok.type == DEF_END)
		{
				return True;
		}else
		{
				__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_GRAMMAR_PATTERN);
				return False;
		}

		
}





static bool_t __handle_token_prio(lex_t *lex, lexMatch_t *match, psrGrammar_t *grammar)
{
		psrGrammarToken_t tok;
		psrError_t g_err;
		wchar_t	   name[PARSER_MAXNAME];
		size_t curr_prio;
		g_err = PSR_NO_ERROR;
		
		curr_prio = PSR_MIN_PRIOVAL + 1;
		
		while(__get_token(lex, match, &tok, ASSOC_DEF))
		{
				psrAssoType_t assoc;
				
				switch(tok.name[1])
				{
				case L'l':
						assoc = PSR_ASSO_LEFT;
						break;
				case L'r':
						assoc = PSR_ASSO_RIGHT;
						break;
				default:
						/*noassoc*/
						assoc = PSR_ASSO_NOASSO;
						break;
				}
				
				while(__get_token(lex, match, &tok, LEXEME))
				{
						int idx;
						
						if(curr_prio > PSR_MAX_PRIOVAL)
						{
								__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_PRIO);
								return False;
						}

						AR_WSTRCPY(name, tok.name);
						
						idx = __find_token_by_name_form_tokset(grammar, name);
						
						
						if(idx == -1)
						{
								psrSymb_t symb;		
								size_t prio_term = PSR_MIN_PRIOTERMVAL;
								do{
										prio_term++;
										PSR_SetSymb(&symb, name, False, curr_prio, assoc, prio_term);

								}while(__find_token_form_tokset(grammar, &symb) != -1);

								__insert_token_to_grammar(grammar, &symb);
						}else
						{
								grammar->term_set[idx]->asso = assoc;
								grammar->term_set[idx]->priority = curr_prio;
						}
				}		
				if(match->err != LEX_NO_ERROR)break;
				if(tok.type != END_SEP_CHAR)
				{
						__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_PRIO);
						return False;
				}
				curr_prio += 1;
		}


		if(match->err != LEX_NO_ERROR)
		{
				__GMR_ERROR(match->next, match->x, match->y, PSR_INVALID_GRAMMAR_PATTERN);
				return False;
		}else if(tok.type == DEF_END)
		{
				return True;
		}else
		{
				__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_PRIO);
				return False;
		}
}







static bool_t __handle_rules(lex_t *lex, lexMatch_t *match, psrGrammar_t *grammar)
{
		psrGrammarToken_t tok;
		psrError_t g_err;
		const wchar_t *p;
		size_t rule_start_id;
		psrSymb_t *body;
		g_err = PSR_NO_ERROR;
		body = NULL;

		if(__get_token(lex, match, &tok, START_ID))
		{
				if(!__get_token(lex, match, &tok, ASSIGN))
				{
						__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_NONTERM_ID);
						return False;
				}
				if(!__get_token(lex, match, &tok, NUMBER))
				{
						__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_NONTERM_ID);
						return False;
				}
				
				p = tok.name;
				rule_start_id = 0;

				while(*p != L'\0')
				{
						rule_start_id *= 10;
						rule_start_id += (*p - L'0');
						p++;
				}

				if(rule_start_id < PSR_MIN_NONTERM_ID)
				{
						__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_NONTERM_ID);
						return False;
				}
				
				if(!__get_token(lex, match, &tok, LEXEME))
				{
						__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_RULE);
						return False;
				}
		}else
		{
				rule_start_id = PSR_MIN_NONTERM_ID;
		}


		while(tok.type == LEXEME && iswalpha(tok.name[0]))
		{
				bool_t handle_rule;
				
				psrSymb_t head,tmp;
				
				PSR_SetSymb(&head, tok.name, True, PSR_MIN_PRIOVAL, PSR_ASSO_NOASSO, PSR_MIN_NONTERM_ID);
				
				head.id = PSR_MIN_NONTERM_ID -1;
				
				if(!__get_token(lex, match, &tok, ASSIGN))goto CHECK_POINT;
				
				handle_rule = True;
				
				

				while(handle_rule)
				{
						bool_t changed_prio;
						bool_t handle_body = True;
						
						changed_prio = False;
						body = NULL;

						while(handle_body)
						{
								__get_token(lex, match, &tok, LEXEME);
								if(match->err != LEX_NO_ERROR)goto CHECK_POINT;
								
								
								switch(tok.type)
								{
								case LEXEME:
								{
										int idx = __find_token_by_name_form_tokset(grammar, tok.name);
										
										if(idx == -1)
										{
												if(!iswalpha(tok.name[0]))goto CHECK_POINT;
												PSR_SetSymb(&tmp, tok.name, True, PSR_MIN_PRIOVAL, PSR_ASSO_NOASSO, PSR_MIN_NONTERM_ID);
												tmp.id = PSR_NONTERM_ID_INRULE;
												PSR_InsertSymb(&body, &tmp);
										}else
										{
												const psrSymb_t *term = grammar->term_set[idx];
												PSR_InsertSymb(&body, term);
										}
										
								}
										break;
								case EPSILON_CHAR:
										/*if(body != NULL)
										{
												PSR_InsertSymb(&body, PSR_EpsilonSymb);
										}*/
										break;
								case PREC_DEF:
								{
										int idx;
										
										if(!__get_token(lex, match, &tok, LEXEME))goto CHECK_POINT;
										idx = __find_token_by_name_form_tokset(grammar, tok.name);
										if(idx == -1)goto CHECK_POINT;
										head.priority = grammar->term_set[idx]->priority;
										changed_prio = True;
										break;
								}
								case END_SEP_CHAR:
								case RULE_SEP:
								{
										if(body == NULL)PSR_InsertSymb(&body, PSR_EpsilonSymb);
										handle_body = False;
										
										if(tok.type == END_SEP_CHAR)
										{
												handle_rule = False;
										}
								}		
										break;
								default:
										goto CHECK_POINT;
										break;

								}
						}
						
						if(!changed_prio && body != NULL)
						{
								const psrSymb_t *right_term, *curr;
								
								right_term = NULL;
								for(curr = body; curr; curr = curr->next)
								{
										if(!curr->non_term)
										{
												right_term = curr;
										}
								}

								if(right_term != NULL)
								{
										head.priority = right_term->priority;
								}
						}

						head.id = rule_start_id++;
						__insert_rule_to_grammar(grammar, &head, body);
						AR_ASSERT(body != NULL);
				}

				__get_token(lex, match, &tok, LEXEME);
		}
		
CHECK_POINT:
		if(match->err != LEX_NO_ERROR)
		{
				__GMR_ERROR(match->next, match->x, match->y, PSR_INVALID_GRAMMAR_PATTERN);
				goto INVALID_POINT;
		}

		if(tok.type != DEF_END)
		{
				__GMR_ERROR(tok.name, tok.line, tok.col, PSR_INVALID_RULE);
				goto INVALID_POINT;
		}
		
		if(body != NULL)PSR_DestroySymbList(body);
		return True;
		
INVALID_POINT:
		if(body != NULL)PSR_DestroySymbList(body);
		return False;

				
}


static bool_t __check_grammar_is_valid(const psrGrammar_t *gmr)
{
		size_t i,j;
		bool_t is_ok;
		is_ok = True;
		for(i = 0; i < gmr->count; ++i)
		{
				const psrSymb_t *curr = gmr->body[i];
				for(curr; curr != NULL; curr = curr->next)
				{
						if(curr->non_term)
						{
								bool_t ok;
								
								ok = False;
								for(j = 0;j < gmr->count; ++j)
								{
										if(PSR_CompSymb(gmr->head[j], curr) == 0)
										{
												ok = True;
												break;
										}
								}
								
								if(!ok)
								{
										AR_Error(AR_CRITICAL, L"Grammar Error: Rule <%s> not defined body\n", curr->name);
										is_ok = False;
								}
						}
				}
		}

		
		for(i = 0; i < gmr->count; ++i)
		{
				for(j = 0; j < gmr->count; ++j)
				{
						if(i != j && PSR_CompSymbList(gmr->body[i], gmr->body[j]) == 0)
						{
								AR_Error(AR_WARNING, L"Grammar Warning: Duplicate rule definition<%s:%d> and <%s:%d>!\n", gmr->head[i]->name,gmr->head[i]->id, gmr->head[j]->name, gmr->head[j]->id);

								if(PSR_CompSymb(gmr->head[i], gmr->head[j]) == 0)
								{

								}
						}
				}
		}
		return is_ok;
}


static bool_t __parse_grammar_pattern(const wchar_t *input, psrGrammar_t *gmr)
{
		
		lex_t *lex;
		lexMatch_t match;
		psrGrammarToken_t tok;
		if(input == NULL)return False;

		lex = LEX_CreateLex();
		LEX_Build(lex, __gmr_pattern);
		LEX_InitMatch(&match, input);
		
		
		if(!__get_token(lex, &match, &tok, TOK_DEF_BEGIN))
		{
				if(match.err != LEX_NO_ERROR)
				{
						__GMR_ERROR(match.next, match.x, match.y, PSR_INVALID_GRAMMAR_PATTERN);
						
				}else
				{
						__GMR_ERROR(tok.name,tok.line, tok.col, PSR_INVALID_GRAMMAR_PATTERN);
				}
				goto INVALID_POINT;
		}
		
		if(!__handle_token_def(lex, &match, gmr))goto INVALID_POINT;


		__get_token(lex, &match, &tok, PREC_DEF_BEGIN);
		if(tok.type == PREC_DEF_BEGIN)
		{
				if(!__handle_token_prio(lex, &match, gmr))
				{
						goto INVALID_POINT;
				}
				__get_token(lex, &match, &tok, RULES_DEF_BEGIN);
		}
		
		

		if(tok.type != RULES_DEF_BEGIN)
		{
				__GMR_ERROR(match.next, match.x, match.y, PSR_INVALID_GRAMMAR_PATTERN);
				goto INVALID_POINT;
		}

		if(!__handle_rules(lex, &match, gmr))
		{
				goto INVALID_POINT;
		}
		LEX_Destroy(lex);
		return __check_grammar_is_valid(gmr);

INVALID_POINT:
		LEX_Destroy(lex);
		return False;
}







psrGrammar_t*	PSR_BuildGrammar(const wchar_t *input)
{
		psrGrammar_t *res;
		res = AR_NEW0(psrGrammar_t);
		__init_default_tokset(res);
		__insert_rule_to_grammar(res, PSR_StartSymb,NULL);
		
		if(!__parse_grammar_pattern(input, res))
		{
				PSR_DestroyGrammar(res);
				return NULL;
		}else
		{
				__calc_grammar(res);
				return res;
		}
}



