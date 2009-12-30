#include "rgx_thompson.h"

#if defined(__LIB)

AR_NAMESPACE_BEGIN


typedef struct __regex_thread_tag
{
		rgxIns_t		*pc;
		const wchar_t	*sp;
}thread_t;


thread_t thread(rgxIns_t *pc, const wchar_t *sp)
{
		thread_t tmp;
		AR_ASSERT(pc != NULL && sp != NULL);
		tmp.pc = pc;
		tmp.sp = sp;
		return tmp;
}


typedef struct __thd_list_tag
{
		thread_t		*lst;
		size_t			count;
		size_t			cap;
}thdList_t;

static void Thd_InitList(thdList_t *lst)
{
		AR_ASSERT(lst != NULL);
		AR_memset(lst, 0, sizeof(*lst));
}


static void Thd_UnInitList(thdList_t *lst)
{
		AR_ASSERT(lst != NULL);
		if(lst->lst)
		{
				AR_DEL(lst->lst);
		}
		AR_memset(lst, 0, sizeof(*lst));
}

static void Thd_Insert(thdList_t *lst, thread_t thd)
{
		AR_ASSERT(lst != NULL);
		AR_ASSERT(thd.pc != NULL && thd.sp != NULL);
		
		if(lst->count == lst->cap)
		{
				lst->cap = (lst->cap + 1)*2;
				lst->lst = AR_REALLOC(thread_t, lst->lst, lst->cap);
		}
		
		lst->lst[lst->count++] = thd;
}

static void Thd_Swap(thdList_t *l, thdList_t *r)
{
		thdList_t tmp;
		AR_ASSERT(l != NULL && r != NULL);
		tmp = *l;
		*l = *r;
		*r = tmp;
}

static void Thd_Clear(thdList_t *l)
{
		AR_ASSERT(l != NULL);
		l->count = 0;
}



static void add_thread(thdList_t *lst,  thread_t thd, rgxProg_t *prog)
{
		AR_ASSERT(lst != NULL);
		
		if(prog->mark == thd.pc->mark)return;
		thd.pc->mark = prog->mark;
		
		//Thd_Insert(lst, thd);

		switch(thd.pc->opcode)
		{
		case RGX_JMP_I:
		{
				AR_ASSERT(thd.pc->right == NULL);
				add_thread(lst, thread(thd.pc->left, thd.sp), prog);
				break;
		}
		case RGX_BRANCH_I:
		{
				add_thread(lst, thread(thd.pc->left, thd.sp), prog);
				add_thread(lst, thread(thd.pc->right, thd.sp), prog);
				break;
		}
		case RGX_LOOKAHEAD_BEG_I:
		case RGX_LOOKAHEAD_END_I:
		case RGX_NOP_I:
		case RGX_CHAR_I:
		case RGX_BEGIN_I:
		case RGX_END_I:
		case RGX_MATCH_I:
		{
				Thd_Insert(lst, thd);
				break;
		}
		default:
		{
				//AR_ASSERT(false);
				//AR_abort();
				break;
		}
		}
}




static bool_t  __lookahead(rgxProg_t *prog, const wchar_t *sp, const wchar_t *input_beg)
{
		thdList_t curr, next;
		rgxIns_t *pc;
		size_t i;
		AR_ASSERT(prog != NULL && sp != NULL && input_beg != NULL);

		Thd_InitList(&curr);
		Thd_InitList(&next);

		prog->mark = 0;

		prog->mark++;
		add_thread(&curr, thread(prog->start, sp), prog);

		for(;;)
		{
				if(curr.count == 0)break;
				
				prog->mark++;
				for(i = 0; i < curr.count; ++i)
				{
						pc = curr.lst[i].pc;
						sp = curr.lst[i].sp;
						
						switch(pc->opcode)
						{
						case RGX_CHAR_I:
						{
								if(*sp >= pc->range.beg && *sp <= pc->range.end)
								{
										add_thread(&next, thread(pc + 1, *sp == L'\0' ? sp : sp + 1), prog);
								}
								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == input_beg)
								{
										add_thread(&next, thread(pc + 1, sp), prog);
								}
								--sp;
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										add_thread(&next, thread(pc + 1, sp), prog);
								}
								break;
						}
						case RGX_LOOKAHEAD_BEG_I:
						{
								
								rgxProg_t lhd;
								size_t k,l;
								lhd.start = pc + 1;
								lhd.pc = lhd.start;
								lhd.mark = 0;

								for(k = 0, l = 1; l > 0; ++k)
								{
										if(lhd.start[k].opcode == RGX_LOOKAHEAD_BEG_I)
										{
												l++;
										}else if(lhd.start[k].opcode == RGX_LOOKAHEAD_END_I)
										{
												l--;
										}
										lhd.start[k].mark = 0;
								}



								if(__lookahead(&lhd, sp, input_beg))
								{
										if(pc->lookahead.negative)
										{

										}else
										{
												add_thread(&next, thread(pc->left, sp), prog);
										}

								}else
								{
										if(pc->lookahead.negative)
										{
												add_thread(&next, thread(pc->left, sp), prog);
										}else
										{

										}
								}
										//pc->lookahead.has_run = true;
								
								break;
						}
						case RGX_LOOKAHEAD_END_I:
						{
								Thd_UnInitList(&curr);
								Thd_UnInitList(&next);
								return true;
								break;
						}
						case RGX_MATCH_I:
						{
								AR_ASSERT(false);
								AR_abort();
								break;
						}
						}
				}
				
				Thd_Swap(&curr, &next);
				Thd_Clear(&next);
		}

		Thd_UnInitList(&curr);
		Thd_UnInitList(&next);
		return false;
}


int_t __thompson(rgxProg_t *prog, const wchar_t *sp, const wchar_t *input_beg)
{
		thdList_t curr, next;
		int_t matched, final;

		rgxIns_t *pc;
		size_t i;
		AR_ASSERT(prog != NULL && sp != NULL && input_beg != NULL);

		Thd_InitList(&curr);
		Thd_InitList(&next);


		prog->mark = 0;
		matched = -1;
		final = -1;
		
		prog->mark++;
		
		add_thread(&curr, thread(prog->start, sp), prog);

		for(;;)
		{
				if(curr.count == 0)break;
				prog->mark++;
				
				for(i = 0; i < curr.count; ++i)
				{
						pc = curr.lst[i].pc;
						sp = curr.lst[i].sp;
						switch(pc->opcode)
						{
						case RGX_CHAR_I:
						{
								if(*sp >= pc->range.beg && *sp <= pc->range.end)
								{
										add_thread(&next, thread(pc + 1, *sp == L'\0' ? sp : sp + 1), prog);
								}
								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == input_beg)
								{
										add_thread(&next, thread(pc + 1, sp), prog);
								}
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										add_thread(&next, thread(pc + 1, sp), prog);
								}
								break;
						}
						case RGX_LOOKAHEAD_BEG_I:
						{
								rgxProg_t lhd;
								size_t k,l;
								lhd.start = pc + 1;
								lhd.pc = lhd.start;
								lhd.mark = 0;

								for(k = 0, l = 1; l > 0; ++k)
								{
										if(lhd.start[k].opcode == RGX_LOOKAHEAD_BEG_I)
										{
												l++;
										}else if(lhd.start[k].opcode == RGX_LOOKAHEAD_END_I)
										{
												l--;
										}
										lhd.start[k].mark = 0;
								}



								if(__lookahead(&lhd, sp, input_beg))
								{
										if(pc->lookahead.negative)
										{

										}else
										{
												add_thread(&next, thread(pc->left, sp), prog);
										}

								}else
								{
										if(pc->lookahead.negative)
										{
												add_thread(&next, thread(pc->left, sp), prog);
										}else
										{

										}
								}
								break;
						}
						case RGX_LOOKAHEAD_END_I:
						{
								AR_ASSERT(false);
								AR_abort();
								break;
						}
						case RGX_MATCH_I:
						{

								matched = sp - input_beg;
								final = pc->final;
								goto BREAK_POINT;
								break;
						}
						}
				}
BREAK_POINT:
				Thd_Swap(&curr, &next);
				Thd_Clear(&next);
		}
		
		Thd_UnInitList(&curr);
		Thd_UnInitList(&next);
		AR_printf(L"final type == %d\r\n", final);
		return matched;
}

#if(0)

#endif






AR_NAMESPACE_END


#endif

