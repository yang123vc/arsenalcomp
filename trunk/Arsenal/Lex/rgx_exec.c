/*
 * The Arsenal Library
 * Copyright (c) 2009-2013 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#include "lex.h"
#include "rgx.h"



AR_NAMESPACE_BEGIN


#if(0)
static AR_INLINE ar_bool_t __already_in_list(const rgxThreadList_t *lst, const rgxThread_t *pthd)
{
		ar_int_t i;
		AR_ASSERT(lst != NULL && pthd != NULL);

		i = (ar_int_t)lst->count-1;
		while(i >= 0)
		{
				if(lst->lst[i].pc == pthd->pc && lst->lst[i].sp == pthd->sp)
				{
						return true;
				}
				--i;
		}

		return false;
}


#endif


#if(0)

static void __add_thread(rgxThreadList_t *lst,  rgxThread_t *thd, rgxProg_t *prog)
{
		ar_uint_t pc_mark = 0;
		AR_ASSERT(lst != NULL && thd != NULL);

		/*
		if(__already_in_list(lst, thd))
		{
				return;
		}
		*/

		AR_ASSERT(thd->pc >= prog->start && thd->pc < prog->start + prog->count);

		pc_mark = prog->marks[thd->pc - prog->start];

		if(prog->mark == pc_mark)
		{
				return;
		}

		prog->marks[thd->pc - prog->start] = prog->mark;

		switch(thd->pc->opcode)
		{
		case RGX_JMP_I:
		{
				AR_ASSERT(thd->pc->right == NULL);
				
				RGX_BuildThread(thd, thd->pc->left, thd->sp, thd->line, thd->col, thd->act);
				__add_thread(lst, thd, prog);
				break;
		}
		case RGX_BRANCH_I:
		{
				rgxThread_t tmp;
				RGX_BuildThread(&tmp, thd->pc->left, thd->sp, thd->line, thd->col, thd->act);
				__add_thread(lst, &tmp, prog);

				RGX_BuildThread(&tmp, thd->pc->right, thd->sp, thd->line, thd->col, thd->act);
				__add_thread(lst, &tmp, prog);
				break;
		}
		case RGX_NOP_I:
		case RGX_CHAR_I:
		case RGX_ANY_CHAR_I:

		case RGX_LOOP_BEG_I:
		case RGX_LOOP_END_I:
		case RGX_LOOKAHEAD_BEG_I:
		case RGX_LOOKAHEAD_END_I:
		case RGX_BEGIN_I:
		case RGX_END_I:
		case RGX_LINE_BEGIN_I:
		case RGX_LINE_END_I:

		case RGX_MATCH_I:
		{
				RGX_InsertToThreadList(lst, thd);
				break;
		}
		default:
		{
				AR_CHECK(false, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
				break;
		}
		}
}


#endif

#if(1)


static void __add_thread(rgxThreadList_t *lst,  rgxThread_t *thd, rgxProg_t *prog)
{
		#define __ADD_THREAD_MAX_LEVEL 32
		ar_uint_t pc_mark = 0;
		
		rgxThread_t		*ss = NULL;
		ar_int_t		ss_idx = -1;

		AR_ASSERT(thd != NULL);
		AR_ASSERT(lst != NULL);
		AR_ASSERT(prog != NULL);

		AR_ASSERT(thd->pc >= prog->start && thd->pc < prog->start + prog->count);
		
		

		pc_mark = prog->marks[thd->pc - prog->start];

		if(prog->mark == pc_mark)
		{
				return;
		}

		prog->marks[thd->pc - prog->start] = prog->mark;


		ss = prog->thd_stk;
		ss[++ss_idx] = *thd;

		while(ss_idx >= 0)
		{
				rgxThread_t tmp = ss[ss_idx--];
				
				switch(tmp.pc->opcode)
				{
				case RGX_JMP_I:
				{
						AR_ASSERT(tmp.pc->right == NULL);
						++ss_idx;
						RGX_BuildThread(&ss[ss_idx], tmp.pc->left, tmp.sp, tmp.line, tmp.col, tmp.act);
						break;
				}
				case RGX_BRANCH_I:
				{
						++ss_idx;
						RGX_BuildThread(&ss[ss_idx], tmp.pc->right, tmp.sp, tmp.line, tmp.col, tmp.act);
						++ss_idx;
						RGX_BuildThread(&ss[ss_idx], tmp.pc->left, tmp.sp, tmp.line, tmp.col, tmp.act);
						break;
				}
				case RGX_NOP_I:
				case RGX_CHAR_I:
				case RGX_ANY_CHAR_I:

				case RGX_LOOP_BEG_I:
				case RGX_LOOP_END_I:
				case RGX_LOOKAHEAD_BEG_I:
				case RGX_LOOKAHEAD_END_I:
				case RGX_BEGIN_I:
				case RGX_END_I:
				case RGX_LINE_BEGIN_I:
				case RGX_LINE_END_I:

				case RGX_MATCH_I:
				{
						RGX_InsertToThreadList(lst, &tmp);
						break;
				}
				default:
				{
						AR_CHECK(false, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
						break;
				}
				}

		}

#undef __ADD_THREAD_MAX_LEVEL
		
}


#endif

#define IS_LINE_BEGIN(_sp, _input)		((_sp) == (_input) || (Lex_IsLineTerminator((_sp)[-1])))
#define IS_LINE_END(_sp)				(*(_sp) == L'\0' || (Lex_IsLineTerminator(*(_sp))))



static AR_INLINE void __check_is_newline(const wchar_t *sp, ar_uint_32_t *pact, size_t *px, size_t *py)
{
		AR_ASSERT(sp != NULL && pact != NULL && px != NULL && py != NULL);

		if(Lex_IsLineTerminator(*sp))
		{																				
				if(*pact & RGX_ACT_INCLINE)
				{																		
						*py = 0;															
						*px += 1;
						*pact &= ~RGX_ACT_INCLINE;
				}else
				{
						wchar_t next_c = *(sp + 1);
						if(*sp == AR_LEX_CR && next_c == AR_LEX_LF) /*CR followed by LF*/
						{
								*py += 1;
								*pact |= RGX_ACT_INCLINE;
						}else
						{
								*py = 0;
								*px += 1;
						}
				}
		}else																			
		{
				*py += 1;
		}

}

/*
#undef IS_NEW_LINE
#undef LF
#undef VT
#undef FF
#undef CR
#undef NEL
#undef LS
#undef PS
*/

static arStatus_t  __loop(rgxProg_t *prog, const wchar_t **start_pos, size_t *px, size_t *py, ar_uint_32_t *pact, lexMatch_t *match);
static arStatus_t  __lookahead(rgxProg_t *prog, const wchar_t *sp, lexMatch_t *match);

#define __match_on_char(_ret, _pc, _c, _flags)			\
		do{												\
				AR_ASSERT((_pc) != NULL);				\
				if((_c) == L'\0')						\
				{										\
						(_ret) = false;					\
						break;							\
				}										\
														\
				if((_flags) & LEX_IGNORE_CASE)											\
				{																		\
						wchar_t	lower = (wchar_t)AR_tolower((_c));						\
						wchar_t upper = (wchar_t)AR_towupper((_c));						\
						if(lower >=  (_pc)->range.beg && lower <= (_pc)->range.end)			\
						{																\
								(_ret) = true;											\
								break;													\
						}else if(upper >=  (_pc)->range.beg && upper <= (_pc)->range.end)		\
						{																\
								(_ret) = true;											\
								break;													\
						}else															\
						{																\
								(_ret) = true;											\
								break;													\
						}																\
				}else																	\
				{																		\
						if((_c) >= (_pc)->range.beg && (_c) <= (_pc)->range.end)		\
						{																\
								(_ret) = true;											\
								break;													\
						}else															\
						{																\
								(_ret) = false;											\
								break;													\
						}																\
				}																		\
		}while(0)																				




#define __match_on_anychar(_ret, _c, _flags)													\
		do{																						\
				if((_c) == L'\0')																\
				{																				\
						(_ret) = false;															\
						break;																	\
				}																				\
																								\
				if((_flags) & LEX_SINGLE_LINE)													\
				{																				\
						(_ret) = true;															\
						break;																	\
				}																				\
				if(Lex_IsLineTerminator((_c)))													\
				{																				\
						(_ret) = false;															\
						break;																	\
				}else																			\
				{																				\
						(_ret) = true;															\
						break;																	\
				}																				\
		}while(0)																				







static AR_INLINE arStatus_t __match_on_loop(rgxProg_t *prog, rgxIns_t *pc, const wchar_t **start_pos, size_t *px, size_t *py, ar_uint_32_t *pact, lexMatch_t *match)
{
		size_t i;
		arStatus_t status;
		AR_ASSERT(prog != NULL);

		AR_ASSERT(pc != NULL && start_pos != NULL && px != NULL && py != NULL && pact != NULL && match != NULL);
		AR_ASSERT(pc >= prog->start && pc < prog->start + prog->count);

		status = AR_S_YES;
		
		for(i = 0; i < pc->fix_count && status == AR_S_YES; ++i)
		{
				rgxProg_t loop = *prog;
				loop.pc = pc + 1;
				status = __loop(&loop, start_pos, px,py,pact, match);
				prog->mark = loop.mark;
		}

		return status;
}


static AR_INLINE arStatus_t __match_on_lookahead(rgxProg_t *prog, rgxIns_t *pc, const wchar_t *sp, lexMatch_t *match)
{
		rgxProg_t lhd;
		arStatus_t status;
		AR_ASSERT(prog != NULL && pc != NULL && sp != NULL && match != NULL);

		AR_ASSERT(pc >= prog->start && pc < prog->start + prog->count);

		lhd = *prog;
		lhd.pc = pc + 1;

		status = __lookahead(&lhd, sp, match);
		prog->mark = lhd.mark;
		return status;
}



static arStatus_t  __lookahead(rgxProg_t *prog, const wchar_t *sp, lexMatch_t *match)
{
		rgxThreadList_t *curr, *next;
		rgxIns_t *pc;
		size_t i;
		const wchar_t *input_beg;
		arStatus_t status;
		rgxThread_t thd;
		AR_ASSERT(prog != NULL && sp != NULL && match != NULL);
		
		status = AR_S_NO;
		curr = RGX_CreateThreadList();
		next = RGX_CreateThreadList();

		if(curr == NULL || next == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}


		input_beg = match->input;
		
		prog->mark++;
		RGX_BuildThread(&thd, prog->pc, sp,0,0, RGX_ACT_NOACTION);
		__add_thread(curr, &thd, prog);

		for(;;)
		{
				if(curr->count == 0)
				{
						break;
				}
		
				prog->mark++;

				for(i = 0; i < curr->count; ++i)
				{
						pc = curr->lst[i].pc;
						sp = curr->lst[i].sp;
						
						switch(pc->opcode)
						{
						case RGX_CHAR_I:
						{
								ar_bool_t is_ok = false;

								__match_on_char(is_ok, pc, *sp, match->flags);


								if(is_ok)
								{
										RGX_BuildThread(&thd, pc + 1, sp + 1, 0, 0, RGX_ACT_NOACTION);
										__add_thread(next, &thd, prog);
								}

								break;
						}
						case RGX_ANY_CHAR_I:
						{
								ar_bool_t is_ok = false;

								__match_on_anychar(is_ok, *sp, match->flags);
								

								if(is_ok)
								{
										RGX_BuildThread(&thd,pc + 1, sp + 1, 0, 0, RGX_ACT_NOACTION);
										__add_thread(next, &thd, prog);
								}

								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == input_beg)
								{
										RGX_BuildThread(&thd, pc + 1, sp, 0, 0, RGX_ACT_NOACTION);
										__add_thread(next, &thd, prog);
								}
								/*--sp;*/
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										RGX_BuildThread(&thd, pc + 1, sp, 0, 0, RGX_ACT_NOACTION);
										__add_thread(next, &thd, prog);
								}
								break;
						}
						case RGX_LINE_BEGIN_I:
						{
								if(IS_LINE_BEGIN(sp, match->input))
								{
										RGX_BuildThread(&thd, pc + 1, sp, 0, 0, RGX_ACT_NOACTION);
										__add_thread(next, &thd, prog);
								}
								/*--sp;*/
								break;
						}
						case RGX_LINE_END_I:
						{
								if(IS_LINE_END(sp))
								{
										RGX_BuildThread(&thd, pc + 1, sp, 0, 0, RGX_ACT_NOACTION);
										__add_thread(next, &thd, prog);
								}
								break;
						}
						case RGX_LOOP_BEG_I:
						{
								
								size_t	x = 0,y = 0;
								ar_uint_32_t act = RGX_ACT_NOACTION;
								arStatus_t is_ok = __match_on_loop(prog, pc, &sp, &x, &y, &act, match);

								if(is_ok == AR_S_YES)/*匹配成功*/
								{
										RGX_BuildThread(&thd, pc->left, sp, 0, 0, RGX_ACT_NOACTION);
										__add_thread(next, &thd, prog);
								
								}else if(is_ok == AR_S_NO)/*匹配失败，则继续*/
								{

								}else					/*出现错误*/
								{

										status = is_ok;
										AR_ASSERT(false);
										goto END_POINT;
								}
								

								break;
						}
						case RGX_LOOP_END_I:
						{

								AR_CHECK(false, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
						}
								break;
						case RGX_LOOKAHEAD_BEG_I:
						{
								arStatus_t is_ok;
								
								is_ok = __match_on_lookahead(prog, pc, sp, match);

								if(is_ok == AR_S_YES)/*匹配搜索*/
								{
										if(pc->lookahead.negative)
										{

										}else
										{
												RGX_BuildThread(&thd, pc->left, sp, 0,0, RGX_ACT_NOACTION);
												__add_thread(next, &thd, prog);
										}

								}else if(is_ok == AR_S_NO)		/*匹配失败*/
								{
										if(pc->lookahead.negative)
										{
												RGX_BuildThread(&thd, pc->left, sp, 0,0, RGX_ACT_NOACTION);
												__add_thread(next, &thd, prog);
										}else
										{

										}
								}else							/*内部错误*/
								{
										status = is_ok;
										AR_ASSERT(false);
										goto END_POINT;
								}
								
								break;
						}
						case RGX_LOOKAHEAD_END_I:
						{
								/*
										不论你前向预搜索什么，只要有一个走通了就OK
										例如a(?=(a|b|c|d))走通了哪个都算OK
								*/
								status = AR_S_YES;
								goto END_POINT;
								break;
						}
						case RGX_NOP_I:
						{
								RGX_BuildThread(&thd, pc + 1, sp, 0, 0, RGX_ACT_NOACTION);
								__add_thread(next,&thd, prog);
						}
								break;

						case RGX_MATCH_I:
						default:
						{
								AR_CHECK(false, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
								break;
						}
						}
				}
				RGX_SwapThreadList(curr, next);
				RGX_ClearThreadList(next);
		}

END_POINT:
		if(curr)
		{
				RGX_DestroyThreadList(curr);
				curr = NULL;
		}

		if(next)
		{
				RGX_DestroyThreadList(next);
				next = NULL;
		}
		return status;
}






static arStatus_t  __loop(rgxProg_t *prog, const wchar_t **start_pos, size_t *px, size_t *py, ar_uint_32_t *pact, lexMatch_t *match)
{
		arStatus_t status;
		rgxThreadList_t *curr, *next;
		rgxIns_t				*pc;
		rgxThread_t				thd;
		const wchar_t	*sp, *final_next;
		ar_uint_32_t		act, final_act;
		size_t i,x,y, final_row, final_col;
		
		
		AR_ASSERT(prog != NULL && start_pos != NULL && *start_pos != NULL && px != NULL && py != NULL && pact != NULL && match != NULL);
		
		status = AR_S_NO;

		

		sp = *start_pos;
		x = *px;
		y = *py;
		act = *pact;
		final_row = x; final_col = y; final_next = NULL; final_act = RGX_ACT_NOACTION;


		curr = RGX_CreateThreadList();
		next = RGX_CreateThreadList();

		if(curr == NULL || next == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}

		RGX_ClearThreadList(curr);
		RGX_ClearThreadList(next);

		prog->mark++;

		RGX_BuildThread(&thd, prog->pc, sp, x,y, act);
		__add_thread(curr, &thd, prog);

		
		for(;;)
		{
				if(curr->count == 0)
				{
						break;
				}

				prog->mark++;

				for(i = 0; i < curr->count; ++i)
				{
						pc = curr->lst[i].pc;
						sp = curr->lst[i].sp;
						x = curr->lst[i].line;
						y = curr->lst[i].col;
						act = curr->lst[i].act;

						switch(pc->opcode)
						{
						case RGX_CHAR_I:
						{
								ar_bool_t is_ok = false;

								__match_on_char(is_ok, pc, *sp, match->flags);

								if(is_ok)
								{
										__check_is_newline(sp, &act, &x, &y);
										sp++;

										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}

								break;
						}
						case RGX_ANY_CHAR_I:
						{
								ar_bool_t is_ok = false;
								
								__match_on_anychar(is_ok, *sp, match->flags);
								

								if(is_ok)
								{
										__check_is_newline(sp, &act, &x, &y);
										sp++;
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}

								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == match->input)
								{
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}
								break;
						}
						case RGX_LINE_BEGIN_I:
						{
								if(IS_LINE_BEGIN(sp, match->input))
								{
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}
								break;
						}
						case RGX_LINE_END_I:
						{
								if(IS_LINE_END(sp))
								{
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}
								break;
						}
						case RGX_LOOP_BEG_I:
						{
								
								arStatus_t is_ok = __match_on_loop(prog, pc, &sp, &x, &y, &act, match);

								if(is_ok == AR_S_YES)
								{
										RGX_BuildThread(&thd, pc->left, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}else if(is_ok == AR_S_NO)
								{
										
								}else
								{
										status = is_ok;
										AR_ASSERT(false);
										goto END_POINT;
								}
								

								break;
						}
						case RGX_LOOP_END_I:
						{
								final_row = x;
								final_col = y;
								final_next = sp;
								final_act = act;
								status = AR_S_YES;
								goto BREAK_POINT;/*这一步决定了优先级为left most*/
						}
								break;
						case RGX_LOOKAHEAD_BEG_I:
						{
								
								arStatus_t is_ok;

								
								is_ok = __match_on_lookahead(prog, pc, sp, match);

								if(is_ok == AR_S_YES)/*匹配成功*/
								{
										if(pc->lookahead.negative)
										{

										}else
										{
												RGX_BuildThread(&thd, pc->left, sp, x,y, act);
												__add_thread(next, &thd, prog);
										}

								}else if(is_ok == AR_S_NO)/*匹配失败*/
								{
										if(pc->lookahead.negative)
										{
												RGX_BuildThread(&thd, pc->left, sp, x,y, act);
												__add_thread(next, &thd, prog);
										}else
										{

										}
								}else			/*内部错误*/
								{
										status = is_ok;
										AR_ASSERT(false);
										goto END_POINT;
								}

								break;
						}
						case RGX_LOOKAHEAD_END_I:
						{
								AR_ASSERT(false);
								AR_CHECK(false, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
								break;
						}
						case RGX_NOP_I:
						{
								RGX_BuildThread(&thd, pc + 1, sp, x, y, act);
								__add_thread(next, &thd, prog);
						}
								break;
						case RGX_MATCH_I:
						default:
						{
								AR_CHECK(false, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
								break;
						}
						}
				}
		BREAK_POINT:
				RGX_SwapThreadList(curr, next);
				RGX_ClearThreadList(next);
		}
		
END_POINT:
		if(curr)
		{
				RGX_DestroyThreadList(curr);
				curr = NULL;
		}

		if(next)
		{
				RGX_DestroyThreadList(next);
				next = NULL;
		}

		if(status == AR_S_YES)
		{
				*py = final_col;
				*px = final_row;
				*pact = final_act;
				AR_ASSERT(final_next != NULL);
				*start_pos = final_next;
		}

		return status;
}




static arStatus_t __thompson(rgxProg_t *prog, lexMatch_t *match, lexToken_t *tok)
{
		arStatus_t status;
		rgxThreadList_t	*curr, *next;
		rgxThread_t		thd;
		rgxIns_t		*pc;
		const wchar_t	*sp, *final_next;

		size_t i,x,y, final_row, final_col;
		ar_uint_32_t		act, final_act;
		

		AR_ASSERT(prog != NULL && match->next != NULL && match->input != NULL);
		
		status = AR_S_NO;

		AR_memset(tok, 0, sizeof(*tok));


		if(prog->curr == NULL) 
		{
				prog->curr = RGX_CreateThreadList();
		}


		if(prog->next == NULL) 
		{
				prog->next = RGX_CreateThreadList();
		}


		if(prog->next == NULL || prog->curr == NULL)
		{
				if(prog->next)
				{
						RGX_DestroyThreadList(prog->next);
						prog->next = NULL;
				}

				if(prog->curr)
				{
						RGX_DestroyThreadList(prog->curr);
						prog->curr = NULL;
				}

				return AR_E_NOMEM;
		}


		curr = prog->curr;
		next = prog->next;
		
		RGX_ClearThreadList(curr);
		RGX_ClearThreadList(next);

		sp = match->next;
		x = match->line;
		y = match->col;
		act = match->next_action;

		final_row = 0; final_col = 0; final_next = NULL; final_act = RGX_ACT_NOACTION;

		prog->mark++;
		RGX_BuildThread(&thd, prog->start, sp, x,y, act);
		__add_thread(curr, &thd, prog);

		for(;;)
		{
				if(curr->count == 0)
				{
						break;
				}

				prog->mark++;

				for(i = 0; i < curr->count; ++i)
				{
						pc = curr->lst[i].pc;
						sp = curr->lst[i].sp;
						x = curr->lst[i].line;
						y = curr->lst[i].col;
						act = curr->lst[i].act;

						switch(pc->opcode)
						{
						case RGX_CHAR_I:
						{
								ar_bool_t is_ok = false;

								__match_on_char(is_ok, pc, *sp, match->flags);

								if(is_ok)
								{
										__check_is_newline(sp, &act, &x, &y);

										sp++;
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}

								break;
						}
						case RGX_ANY_CHAR_I:
						{

								ar_bool_t is_ok = false;
								
								__match_on_anychar(is_ok, *sp, match->flags);
								

								if(is_ok)
								{
										__check_is_newline(sp, &act, &x, &y);
										sp++;
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}

								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == match->input)
								{
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}
								break;
						}
						case RGX_LINE_BEGIN_I:
						{
								if(IS_LINE_BEGIN(sp, match->input))
								{
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}
								break;
						}
						case RGX_LINE_END_I:
						{
								if(IS_LINE_END(sp))
								{
										RGX_BuildThread(&thd, pc + 1, sp, x,y, act);
										__add_thread(next, &thd, prog);
								}
								break;
						}
						case RGX_LOOP_BEG_I:
						{

								arStatus_t is_ok = __match_on_loop(prog, pc, &sp, &x, &y, &act, match);
								if(is_ok == AR_S_YES)/*匹配成功*/
								{
										RGX_BuildThread(&thd, pc->left, sp, x,y, act);
										__add_thread(next, &thd, prog);
								
								}else if(is_ok == AR_S_NO)/*匹配失败*/
								{
										
								}else	/*错误*/
								{
										status = is_ok;
										AR_ASSERT(false);
										goto END_POINT;
								}
								

								break;
						}
						case RGX_LOOP_END_I:
						{
								AR_CHECK(false, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
						}
								break;
						case RGX_LOOKAHEAD_BEG_I:
						{
								
								arStatus_t is_ok;

								is_ok = __match_on_lookahead(prog, pc, sp, match);
								
								if(is_ok == AR_S_YES)
								{
										if(pc->lookahead.negative)
										{

										}else
										{
												RGX_BuildThread(&thd, pc->left, sp, x,y, act);
												__add_thread(next, &thd, prog);
										}

								}else if(is_ok == AR_S_NO)
								{
										if(pc->lookahead.negative)
										{
												RGX_BuildThread(&thd, pc->left, sp, x,y, act);
												__add_thread(next, &thd, prog);
										}else
										{

										}
								
								}else	/*错误*/
								{
										status = is_ok;
										AR_ASSERT(false);
										goto END_POINT;
								}

								break;
						}
						case RGX_LOOKAHEAD_END_I:
						{
								AR_ASSERT(false);
								AR_CHECK(false, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
								break;
						}
						case RGX_NOP_I:
						{
								RGX_BuildThread(&thd, pc + 1, sp, x, y, act);
								__add_thread(next, &thd, prog);
						}
								break;
						case RGX_MATCH_I:
						{

#if(0)			/*POSIX标准*/
								if(status == AR_S_NO)
								{
										tok->str = match->next;
										tok->index = match->next - match->input;
										tok->count = sp - match->next;
										tok->line = match->line;
										tok->col = match->col;
										tok->value = (size_t)pc->final;

										final_row = x;
										final_col = y;
										final_next = sp;
										final_act = act;
										status = AR_S_YES;
								}else
								{
										if((size_t)(sp - match->next) > tok->count)
										{
												tok->str = match->next;
												tok->index = match->next - match->input;
												tok->count = sp - match->next;
												tok->line = match->line;
												tok->col = match->col;
												tok->value = (size_t)pc->final;

												final_row = x;
												final_col = y;
												final_next = sp;
												final_act = act;
										}

								}
#endif

								tok->str = match->next;
								tok->index = match->next - match->input;
								tok->count = sp - match->next;
								tok->line = match->line;
								tok->col = match->col;
								tok->value = (size_t)pc->final;

								final_row = x;
								final_col = y;
								final_next = sp;
								final_act = act;
								status = AR_S_YES;

								goto BREAK_POINT;/*这一步决定了优先级为left most*/

								break;
						}
						default:
						{
								AR_CHECK(false, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
								break;
						}
						}
				}
		BREAK_POINT:
				RGX_SwapThreadList(curr, next);
				RGX_ClearThreadList(next);
		}

END_POINT:
		if(status == AR_S_YES)
		{
				match->col = final_col;
				match->line = final_row;
				match->next = final_next;
				match->next_action = final_act;
		}
		return status;
}







arStatus_t RGX_Match(rgxProg_t *prog, lexMatch_t *match, lexToken_t *tok)
{
		arStatus_t status;
		AR_ASSERT(prog != NULL && match != NULL && tok != NULL);

		AR_ASSERT(match->input != NULL && match->next != NULL);

		if(!match->is_ok)
		{
				return AR_E_LEXNOTREADY;
		}

		prog->mark = 0;


        AR_memset(prog->marks, 0, prog->count * sizeof(prog->marks[0]));
		status = __thompson(prog, match, tok);
		
		return status;
}








AR_NAMESPACE_END




