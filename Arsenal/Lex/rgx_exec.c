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

static void __add_thread(rgxThreadList_t *lst,  rgxThread_t thd, rgxProg_t *prog)
{
		AR_ASSERT(lst != NULL);
		
		
		if(__already_in_list(lst, &thd))
		{
				return;
		}
		


		switch(thd.pc->opcode)
		{
		case RGX_JMP_I:
		{
				AR_ASSERT(thd.pc->right == NULL);
				__add_thread(lst, RGX_BuildThread(thd.pc->left, thd.sp, thd.line, thd.col, thd.act), prog);
				break;
		}
		case RGX_BRANCH_I:
		{
				__add_thread(lst, RGX_BuildThread(thd.pc->left, thd.sp, thd.line, thd.col, thd.act), prog);
				__add_thread(lst, RGX_BuildThread(thd.pc->right, thd.sp, thd.line, thd.col, thd.act), prog);
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


static AR_INLINE ar_bool_t  __match_on_char(const rgxIns_t *pc, wchar_t c, ar_uint_t flags)
{
		
		AR_ASSERT(pc != NULL);

		if(c == L'\0')
		{
				return false;
		}
				
		if(flags & LEX_IGNORE_CASE)
		{
				wchar_t lower = (wchar_t)AR_tolower(c);
				wchar_t upper = (wchar_t)AR_towupper(c);

				if(lower >=  pc->range.beg && lower <= pc->range.end)
				{
						return true;

				}else if(upper >=  pc->range.beg && upper <= pc->range.end)
				{
						return true;
				}else
				{
						return false;
				}
		}else
		{
				if(c >= pc->range.beg && c <= pc->range.end)
				{
						return true;
				}else
				{
						return false;
				}

		}
}




static AR_INLINE ar_bool_t  __match_on_anychar(wchar_t c, ar_uint_t flags)
{

		if(c == L'\0')
		{
				return false;
		}

		if(flags & LEX_SINGLE_LINE)/*single line 可以匹配包含换行符号在内的所有字符*/
		{
				return true;
		}

		
		if(Lex_IsLineTerminator(c))
		{
				return false;
		}else
		{
				return true;
		}
}


static AR_INLINE arStatus_t __match_on_loop(const rgxIns_t *pc, const wchar_t **start_pos, size_t *px, size_t *py, ar_uint_32_t *pact, lexMatch_t *match)
{
		size_t i;
		arStatus_t status;
		AR_ASSERT(pc != NULL && start_pos != NULL && px != NULL && py != NULL && pact != NULL && match != NULL);

		status = AR_S_YES;
		
		for(i = 0; i < pc->fix_count && status == AR_S_YES; ++i)
		{
				rgxProg_t loop;
				loop.start = (rgxIns_t*)pc + 1;
				loop.pc = loop.start;
				status = __loop(&loop, start_pos, px,py,pact, match);
		}

		return status;
}


static AR_INLINE arStatus_t __match_on_lookahead(const rgxIns_t *pc, const wchar_t *sp, lexMatch_t *match)
{
		rgxProg_t lhd;

		AR_ASSERT(pc != NULL && sp != NULL && match != NULL);

		lhd.start = (rgxIns_t*)pc + 1;
		lhd.pc = lhd.start;

		return __lookahead(&lhd, sp, match);

}


static arStatus_t  __lookahead(rgxProg_t *prog, const wchar_t *sp, lexMatch_t *match)
{
		rgxThreadList_t *curr, *next;
		rgxIns_t *pc;
		size_t i;
		const wchar_t *input_beg;
		arStatus_t status;
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

		__add_thread(curr, RGX_BuildThread(prog->start, sp,0,0, RGX_ACT_NOACTION), prog);

		for(;;)
		{
				if(curr->count == 0)
				{
						break;
				}
		

				for(i = 0; i < curr->count; ++i)
				{
						pc = curr->lst[i].pc;
						sp = curr->lst[i].sp;
						
						switch(pc->opcode)
						{
						case RGX_CHAR_I:
						{
								if(__match_on_char(pc, *sp, match->flags))
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp + 1, 0, 0, RGX_ACT_NOACTION), prog);
								}

								break;
						}
						case RGX_ANY_CHAR_I:
						{
								if(__match_on_anychar(*sp, match->flags))
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp + 1, 0, 0, RGX_ACT_NOACTION), prog);
								}
								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == input_beg)
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, 0, 0, RGX_ACT_NOACTION), prog);
								}
								/*--sp;*/
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, 0, 0, RGX_ACT_NOACTION), prog);
								}
								break;
						}
						case RGX_LINE_BEGIN_I:
						{
								if(IS_LINE_BEGIN(sp, match->input))
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, 0, 0, RGX_ACT_NOACTION), prog);
								}
								/*--sp;*/
								break;
						}
						case RGX_LINE_END_I:
						{
								if(IS_LINE_END(sp))
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, 0, 0, RGX_ACT_NOACTION), prog);
								}
								break;
						}
						case RGX_LOOP_BEG_I:
						{
								
								size_t	x = 0,y = 0;
								ar_uint_32_t act = RGX_ACT_NOACTION;
								arStatus_t is_ok = __match_on_loop(pc, &sp, &x, &y, &act, match);

								if(is_ok == AR_S_YES)/*匹配成功*/
								{
										__add_thread(next, RGX_BuildThread(pc->left, sp, 0, 0, RGX_ACT_NOACTION), prog);
								
								}else if(is_ok == AR_S_NO)/*匹配失败，则继续*/
								{

								}else					/*出现错误*/
								{
										status = is_ok;
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
								
								is_ok = __match_on_lookahead(pc, sp, match);

								if(is_ok == AR_S_YES)/*匹配搜索*/
								{
										if(pc->lookahead.negative)
										{

										}else
										{
												__add_thread(next, RGX_BuildThread(pc->left, sp, 0,0, RGX_ACT_NOACTION), prog);
										}

								}else if(is_ok == AR_S_NO)		/*匹配失败*/
								{
										if(pc->lookahead.negative)
										{
												__add_thread(next, RGX_BuildThread(pc->left, sp, 0,0, RGX_ACT_NOACTION), prog);
										}else
										{

										}
								}else							/*内部错误*/
								{
										status = is_ok;
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

								__add_thread(next, RGX_BuildThread(pc + 1, sp, 0, 0, RGX_ACT_NOACTION), prog);
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

		__add_thread(curr, RGX_BuildThread(prog->start, sp, x,y, act), prog);

		
		for(;;)
		{
				if(curr->count == 0)
				{
						break;
				}


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
								if(__match_on_char(pc, *sp, match->flags))
								{
										__check_is_newline(sp, &act, &x, &y);
										sp++;
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}
								break;
						}
						case RGX_ANY_CHAR_I:
						{
								if(__match_on_anychar(*sp, match->flags))
								{
										__check_is_newline(sp, &act, &x, &y);
										sp++;
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}

								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == match->input)
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}
								break;
						}
						case RGX_LINE_BEGIN_I:
						{
								if(IS_LINE_BEGIN(sp, match->input))
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}
								break;
						}
						case RGX_LINE_END_I:
						{
								if(IS_LINE_END(sp))
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}
								break;
						}
						case RGX_LOOP_BEG_I:
						{
								
								arStatus_t is_ok = __match_on_loop(pc, &sp, &x, &y, &act, match);

								if(is_ok == AR_S_YES)
								{
										__add_thread(next, RGX_BuildThread(pc->left, sp, x,y, act), prog);
								}else if(is_ok == AR_S_NO)
								{
										
								}else
								{
										status = is_ok;
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

								
								is_ok = __match_on_lookahead(pc, sp, match);

								if(is_ok == AR_S_YES)/*匹配成功*/
								{
										if(pc->lookahead.negative)
										{

										}else
										{
												__add_thread(next, RGX_BuildThread(pc->left, sp, x,y, act), prog);
										}

								}else if(is_ok == AR_S_NO)/*匹配失败*/
								{
										if(pc->lookahead.negative)
										{
												__add_thread(next, RGX_BuildThread(pc->left, sp, x,y, act), prog);
										}else
										{

										}
								}else			/*内部错误*/
								{
										status = is_ok;
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

								__add_thread(next, RGX_BuildThread(pc + 1, sp, x, y, act), prog);
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


		__add_thread(curr, RGX_BuildThread(prog->start, sp, x,y, act), prog);

		for(;;)
		{
				if(curr->count == 0)
				{
						break;
				}


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
								if(__match_on_char(pc, *sp, match->flags))
								{
										__check_is_newline(sp, &act, &x, &y);
										sp++;
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}
								break;
						}
						case RGX_ANY_CHAR_I:
						{

								if(__match_on_anychar(*sp, match->flags))
								{
										__check_is_newline(sp, &act, &x, &y);
										sp++;
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}

								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == match->input)
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}
								break;
						}
						case RGX_LINE_BEGIN_I:
						{
								if(IS_LINE_BEGIN(sp, match->input))
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}
								break;
						}
						case RGX_LINE_END_I:
						{
								if(IS_LINE_END(sp))
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y, act), prog);
								}
								break;
						}
						case RGX_LOOP_BEG_I:
						{

								arStatus_t is_ok = __match_on_loop(pc, &sp, &x, &y, &act, match);
								if(is_ok == AR_S_YES)/*匹配成功*/
								{
										__add_thread(next, RGX_BuildThread(pc->left, sp, x,y, act), prog);
								
								}else if(is_ok == AR_S_NO)/*匹配失败*/
								{
										
								}else	/*错误*/
								{
										status = is_ok;
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

								is_ok = __match_on_lookahead(pc, sp, match);
								
								if(is_ok == AR_S_YES)
								{
										if(pc->lookahead.negative)
										{

										}else
										{
												__add_thread(next, RGX_BuildThread(pc->left, sp, x,y, act), prog);
										}

								}else if(is_ok == AR_S_NO)
								{
										if(pc->lookahead.negative)
										{
												__add_thread(next, RGX_BuildThread(pc->left, sp, x,y, act), prog);
										}else
										{

										}
								
								}else	/*错误*/
								{
										status = is_ok;
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

								__add_thread(next, RGX_BuildThread(pc + 1, sp, x, y, act), prog);
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

		AR_ASSERT(prog != NULL && match != NULL && tok != NULL);

		AR_ASSERT(match->input != NULL && match->next != NULL);

		if(!match->is_ok)
		{
				return AR_E_LEXNOTREADY;
		}

		return __thompson(prog, match, tok);
}








AR_NAMESPACE_END




