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

#include "lex.h"
#include "rgx.h"



AR_NAMESPACE_BEGIN


static void __add_thread(rgxThreadList_t *lst,  rgxThread_t thd, rgxProg_t *prog)
{
		AR_ASSERT(lst != NULL);
		
		if(prog->mark == thd.pc->mark)return;
		thd.pc->mark = prog->mark;

		switch(thd.pc->opcode)
		{
		case RGX_JMP_I:
		{
				AR_ASSERT(thd.pc->right == NULL);
				__add_thread(lst, RGX_BuildThread(thd.pc->left, thd.sp, thd.line, thd.col), prog);
				break;
		}
		case RGX_BRANCH_I:
		{
				__add_thread(lst, RGX_BuildThread(thd.pc->left, thd.sp, thd.line, thd.col), prog);
				__add_thread(lst, RGX_BuildThread(thd.pc->right, thd.sp, thd.line, thd.col), prog);
				break;
		}
		case RGX_LOOKAHEAD_BEG_I:
		case RGX_LOOKAHEAD_END_I:
		case RGX_NOP_I:
		case RGX_CHAR_I:
		case RGX_BEGIN_I:
		case RGX_END_I:
		case RGX_LINE_BEGIN_I:
		case RGX_LINE_END_I:
		case RGX_ANY_CHAR_I:
		case RGX_MATCH_I:
		{
				RGX_InsertToThreadList(lst, thd);
				break;
		}
		default:
		{
				AR_ASSERT(false);
				AR_error(AR_ERR_FATAL, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
				break;
		}
		}
}



static void __clear_for_lookahead(rgxProg_t *prog)
{		
		size_t k,l;
		AR_ASSERT(prog && prog->start != NULL);
		prog->mark = 0;

		for(k = 0, l = 1; l > 0; ++k)
		{
				if(prog->start[k].opcode == RGX_LOOKAHEAD_BEG_I)
				{
						l++;
				}else if(prog->start[k].opcode == RGX_LOOKAHEAD_END_I)
				{
						l--;
				}
				prog->start[k].mark = 0;
		}

		prog->mark = 0;
}


static bool_t  __lookahead(rgxProg_t *prog, const wchar_t *sp, lexMatch_t *match)
{
		rgxThreadList_t *curr, *next;
		rgxIns_t *pc;
		size_t i;
		const wchar_t *input_beg;
		AR_ASSERT(prog != NULL && sp != NULL && match != NULL);
		input_beg = match->input;
		curr = RGX_CreateThreadList();
		next = RGX_CreateThreadList();

		__clear_for_lookahead(prog);
		
		prog->mark++;
		__add_thread(curr, RGX_BuildThread(prog->start, sp,0,0), prog);

		for(;;)
		{
				if(curr->count == 0)break;
				
				prog->mark++;
				for(i = 0; i < curr->count; ++i)
				{
						pc = curr->lst[i].pc;
						sp = curr->lst[i].sp;
						
						switch(pc->opcode)
						{
						case RGX_CHAR_I:
						{
								bool_t is_ok = false;
								if(*sp != L'\0')
								{
										if(match->flags & LEX_IGNORE_CASE)
										{
												wchar_t lower = (wchar_t)AR_tolower(*sp), upper = (wchar_t)AR_towupper(*sp);
												
												if(lower >=  pc->range.beg && lower <= pc->range.end)
												{
														is_ok = true;
												}else if(upper >=  pc->range.beg && upper <= pc->range.end)
												{
														is_ok = true;
												}
										}else
										{
												is_ok = (*sp >= pc->range.beg && *sp <= pc->range.end);
										}
								}

								if(is_ok)
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp + 1, 0, 0), prog);
								}

								break;
						}
						case RGX_ANY_CHAR_I:
						{
								bool_t is_ok = false;

								if(*sp != L'\0')
								{
										if(match->flags & LEX_SINGLE_LINE)/*single line 可以匹配包含\n在内的所有字符*/
										{
												is_ok = true;
										}else
										{
												is_ok = *sp != L'\n';/*否则*sp不可以为'\n'*/
										}
								}

								if(is_ok)
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp + 1, 0, 0), prog);
								}
								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == input_beg)
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, 0, 0), prog);
								}
								--sp;
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, 0, 0), prog);
								}
								break;
						}
						case RGX_LINE_BEGIN_I:
						{
								if(sp == input_beg || *(sp-1) == L'\n')
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, 0, 0), prog);
								}
								--sp;
								break;
						}
						case RGX_LINE_END_I:
						{
								if(*sp == L'\0' || *sp == L'\n')
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, 0, 0), prog);
								}
								break;
						}
						case RGX_LOOKAHEAD_BEG_I:
						{
								
								rgxProg_t lhd;
								
								lhd.start = pc + 1;
								lhd.pc = lhd.start;
								lhd.mark = 0;
								
								/*__clear_ins_set(lhd.start);*/


								if(__lookahead(&lhd, sp, match))
								{
										if(pc->lookahead.negative)
										{

										}else
										{
												__add_thread(next, RGX_BuildThread(pc->left, sp, 0,0), prog);
										}

								}else
								{
										if(pc->lookahead.negative)
										{
												__add_thread(next, RGX_BuildThread(pc->left, sp, 0,0), prog);
										}else
										{

										}
								}
								
								break;
						}
						case RGX_LOOKAHEAD_END_I:
						{
								/*
										这里的意义是，不论你前向预搜索什么，只要有一个走通了就OK
										例如a(?=(a|b|c|d))走通了哪个都算OK
								*/
								RGX_DestroyThreadList(curr);
								RGX_DestroyThreadList(next);
								return true;
								break;
						}
						case RGX_NOP_I:
						case RGX_MATCH_I:
						default:
						{
								AR_ASSERT(false);
								AR_error(AR_ERR_FATAL, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
								break;
						}
						}
				}
				RGX_SwapThreadList(curr, next);
				RGX_ClearThreadList(next);
		}

		RGX_DestroyThreadList(curr);
		RGX_DestroyThreadList(next);
		return false;
}




static bool_t __thompson(rgxProg_t *prog, lexMatch_t *match, lexToken_t *tok)
{
		rgxThreadList_t	*curr, *next;
		bool_t			matched;
		rgxIns_t		*pc;
		const wchar_t	*sp, *fianl_next;
		size_t i,x,y, final_row, final_col;
		

		AR_ASSERT(prog != NULL && match->next != NULL && match->input != NULL);
		

		AR_memset(tok, 0, sizeof(*tok));

		for(i = 0; i < prog->count; ++i)
		{
				prog->start[i].mark = 0;
		}
		prog->mark = 0;
		
		if(prog->curr == NULL) prog->curr = RGX_CreateThreadList();
		if(prog->next == NULL) prog->next = RGX_CreateThreadList();


		curr = prog->curr;
		next = prog->next;
		
		RGX_ClearThreadList(curr);
		RGX_ClearThreadList(next);

		matched = false;
		sp = match->next;
		x = match->line;
		y = match->col;

		final_row = 0; final_col = 0; fianl_next = NULL;

		prog->mark++;
		__add_thread(curr, RGX_BuildThread(prog->start, sp, x,y), prog);

		for(;;)
		{
				if(curr->count == 0)break;
				prog->mark++;
				
				for(i = 0; i < curr->count; ++i)
				{
						pc = curr->lst[i].pc;
						sp = curr->lst[i].sp;
						x = curr->lst[i].line;
						y = curr->lst[i].col;

						switch(pc->opcode)
						{
						case RGX_CHAR_I:
						{
								bool_t is_ok = false;
								if(*sp != L'\0')
								{
										if(match->flags & LEX_IGNORE_CASE)
										{
												wchar_t lower = (wchar_t)AR_tolower(*sp), upper = (wchar_t)AR_towupper(*sp);
												
												if(lower >=  pc->range.beg && lower <= pc->range.end)
												{
														is_ok = true;
												}else if(upper >=  pc->range.beg && upper <= pc->range.end)
												{
														is_ok = true;
												}
										}else
										{
												is_ok = (*sp >= pc->range.beg && *sp <= pc->range.end);
										}
								}

								if(is_ok)
								{
										if(*sp == L'\n')
										{
												y = 0;
												x++;
										}else
										{
												y++;
										}

										sp++;
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y), prog);
								}
								break;
						}
						case RGX_ANY_CHAR_I:
						{
								bool_t is_ok = false;

								if(*sp != L'\0')
								{
										if(match->flags & LEX_SINGLE_LINE)/*single line 可以匹配包含\n在内的所有字符*/
										{
												is_ok = true;
										}else
										{
												is_ok = *sp != L'\n';/*否则*sp不可以为'\n'*/
										}
								}

								if(is_ok)
								{
										if(*sp == L'\n')
										{
												y = 0;
												x++;
										}else
										{
												y++;
										}
										sp++;
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y), prog);
								}

								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == match->input)
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y), prog);
								}
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y), prog);
								}
								break;
						}
						case RGX_LINE_BEGIN_I:
						{
								if(sp == match->input || *(sp-1) == L'\n')
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y), prog);
								}
								break;
						}
						case RGX_LINE_END_I:
						{
								if(*sp == L'\0' || *sp == L'\n') 
								{
										__add_thread(next, RGX_BuildThread(pc + 1, sp, x,y), prog);
								}
								break;
						}
						case RGX_LOOKAHEAD_BEG_I:
						{
								
								rgxProg_t lhd;
								
								lhd.start = pc + 1;
								lhd.pc = lhd.start;
								lhd.mark = 0;

								/*__clear_ins_set(lhd.start);*/


								if(__lookahead(&lhd, sp, match))
								{
										if(pc->lookahead.negative)
										{

										}else
										{
												__add_thread(next, RGX_BuildThread(pc->left, sp, x,y), prog);
										}

								}else
								{
										if(pc->lookahead.negative)
										{
												__add_thread(next, RGX_BuildThread(pc->left, sp, x,y), prog);
										}else
										{

										}
								}
								break;
						}
						case RGX_LOOKAHEAD_END_I:
						{
								AR_ASSERT(false);
								AR_error(AR_ERR_FATAL, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
								break;
						}
						case RGX_MATCH_I:
						{
								tok->str = match->next;
								tok->count = sp - match->next;
								tok->line = match->line;
								tok->col = match->col;
								tok->value = (size_t)pc->final;

								final_row = x;
								final_col = y;
								fianl_next = sp;

								matched = true;
								goto BREAK_POINT;/*这一步决定了优先级为left most*/
								break;
						}
						case RGX_NOP_I:
						default:
						{
								AR_ASSERT(false);
								AR_error(AR_ERR_FATAL, L"Arsenal : regex exec error %hs\r\n", AR_FUNC_NAME);
								break;
						}
						}
				}
		BREAK_POINT:
				RGX_SwapThreadList(curr, next);
				RGX_ClearThreadList(next);
		}
		
		/*RGX_DestroyThreadList(curr);*/
		/*RGX_DestroyThreadList(next);*/
		if(matched)
		{
				match->col = final_col;
				match->line = final_row;
				match->next = fianl_next;
		}
		return matched;
}






bool_t RGX_Match(rgxProg_t *prog, lexMatch_t *match, lexToken_t *tok)
{
		AR_ASSERT(prog != NULL && match != NULL && tok != NULL);

		AR_ASSERT(match->input != NULL && match->next != NULL);

		if(!match->is_ok)return match->is_ok;

		return __thompson(prog, match, tok);
}








AR_NAMESPACE_END




