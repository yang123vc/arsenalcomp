#include "test.h"

#include "../../Arsenal/Regex/rgx_in.h"
#include "../../Arsenal/Lex/lex.h"




#if defined(__LIB)

AR_NAMESPACE_BEGIN
#if(0)
static const wchar_t *sp_stk[1024];
static size_t	sp_top = 0;

typedef struct __thread_tag
{
		const rgxIns_t	*pc;
		const wchar_t	*sp;

}thread_t;
#define MAX_THD_STK 10000
static int_t	__backtrack(const rgxIns_t *pc, const wchar_t *sp, const rgxCClass_t *cclass, const wchar_t *input_beg)
{
		thread_t		*thd;
		size_t			nthd;
		AR_ASSERT(pc != NULL && sp != NULL && cclass != NULL);

		thd = AR_NEWARR0(thread_t, MAX_THD_STK);
		nthd = 0;

		thd[nthd].pc = pc;
		thd[nthd].sp = sp;
		nthd++;

		while(nthd > 0)
		{
				bool_t is_ok;
				nthd--;
				pc = thd[nthd].pc;
				sp = thd[nthd].sp;

				is_ok = true;
				while(is_ok)
				{
						switch(pc->opcode)
						{
						case RGX_CHAR_I:
								{
										int_t id = RGX_FindIndexFromCClass(cclass, *sp);
										if(id != pc->data)is_ok = false;
										if(*sp != L'\0')sp++;
										pc++;

										break;
								}
						case RGX_BEGIN_I:
								{
										if(sp != input_beg)is_ok = false;
										pc++;
										break;
								}
						case RGX_END_I:
								{
										if(*sp != L'\0')is_ok = false;
										pc++;
										break;
								}
						case RGX_MATCH_I:
								{
										//AR_printf(L"%ls\r\n", AR_wcsndup(rec, (sp - input_beg + 1)));
										return sp - input_beg;
										break;
								}
						case RGX_JMP_I:
								{
										AR_ASSERT(pc->right == NULL);
										pc = pc->left;
										break;
								}
						case RGX_BRANCH_I:
								{
										if(nthd >= MAX_THD_STK)
										{
												AR_ASSERT(false);
												AR_abort();
										}
										thd[nthd].pc = pc->right;
										thd[nthd].sp = sp;
										nthd++;
										pc = pc->left;
										break;
								}
						case RGX_LOOKAHEAD_BEG_I:
								{
										sp_stk[sp_top++] = sp;
										pc++;
										break;
								}
						case RGX_LOOKAHEAD_END_I:
								{
										AR_ASSERT(sp_top > 0);
										sp = sp_stk[--sp_top];
										pc++;
										break;
								}
						default:
								{
										AR_ASSERT(false);
										AR_abort();
										break;
								}
						}
				}
		}

		return -1;
}



static int_t			__run(const rgxIns_t *pc, const wchar_t *sp, const rgxCClass_t *cclass, const wchar_t *input_beg)
{
		
		AR_ASSERT(pc != NULL && sp != NULL && cclass != NULL);

		
		for(;;)
		{
				switch(pc->opcode)
				{
				case RGX_CHAR_I:
				{
						int_t id = RGX_FindIndexFromCClass(cclass, *sp);
						if(id != pc->data)return -1;
						if(*sp != L'\0')sp++;
						pc++;

						break;
				}
				case RGX_BEGIN_I:
				{
						if(sp != input_beg)return -1;
						pc++;
						break;
				}
				case RGX_END_I:
				{
						if(*sp != L'\0')return -1;
						pc++;
						break;
				}
				case RGX_MATCH_I:
				{
						//AR_printf(L"%ls\r\n", AR_wcsndup(rec, (sp - input_beg + 1)));
						return sp - input_beg;
						break;
				}
				case RGX_JMP_I:
				{
						AR_ASSERT(pc->right == NULL);
						pc = pc->left;
						break;
				}
				case RGX_BRANCH_I:
				{
						int_t cnt = __run(pc->left, sp, cclass, input_beg);
						if(cnt >= 0)return cnt;
						AR_ASSERT(pc->right != NULL);
						pc = pc->right;
						break;
				}
				case RGX_LOOKAHEAD_BEG_I:
				{
						sp_stk[sp_top++] = sp;
						pc++;
						break;
				}
				case RGX_LOOKAHEAD_END_I:
				{
						AR_ASSERT(sp_top > 0);
						sp = sp_stk[--sp_top];
						pc++;
						break;
				}
				default:
				{
						AR_ASSERT(false);
						AR_abort();
						break;
				}
				}
		}

		return false;
}


typedef struct __thd_list_tag
{
		rgxIns_t		**lst;
		size_t			count;
		size_t			cap;
}thdList_t;

void Thd_InitList(thdList_t *lst)
{
		AR_ASSERT(lst != NULL);
		AR_memset(lst, 0, sizeof(*lst));
}

void Thd_Insert(thdList_t *lst, const rgxIns_t *ins)
{
		AR_ASSERT(lst != NULL && ins != NULL);
		
		if(lst->count == lst->cap)
		{
				lst->cap = (lst->cap + 1)*2;
				lst->lst = AR_REALLOC(rgxIns_t*, lst->lst, lst->cap);
		}
		
		lst->lst[lst->count++] = (rgxIns_t*)ins;
}

void Thd_Swap(thdList_t *l, thdList_t *r)
{
		thdList_t tmp;
		AR_ASSERT(l != NULL && r != NULL);
		tmp = *l;
		*l = *r;
		*r = *l;
}

void Thd_Clear(thdList_t *l)
{
		AR_ASSERT(l != NULL);
		l->count = 0;
}

size_t  __g_mark = 0;

void add_thread(thdList_t *lst, const rgxIns_t *pc, const wchar_t **sp)
{
		rgxIns_t *tmp;
		AR_ASSERT(lst != NULL && pc != NULL);
		
		if(pc->mark == __g_mark)return;

		tmp = (rgxIns_t*)pc;
		tmp->mark = __g_mark;
		Thd_Insert(lst, tmp);


		switch(tmp->opcode)
		{
		case RGX_JMP_I:
		{
				AR_ASSERT(tmp->right == NULL);
				add_thread(lst, tmp->left, sp);
				break;
		}
		case RGX_BRANCH_I:
		{
				add_thread(lst, tmp->left, sp);
				add_thread(lst, tmp->right, sp);
				break;
		}
		case RGX_LOOKAHEAD_BEG_I:
		{
				sp_stk[sp_top++] = *sp;
				add_thread(lst, pc + 1, sp);
				break;
		}
		case RGX_LOOKAHEAD_END_I:
		{
				AR_ASSERT(sp_top > 0);
				*sp = sp_stk[--sp_top];
				add_thread(lst, pc + 1, sp);
				break;
		}
		}
}




static int_t			thompsonvm(const rgxIns_t *pc, const wchar_t *sp, const rgxCClass_t *cclass, const wchar_t *input_beg)
{
		size_t i;
		int_t matched;
		thdList_t curr, next;
		
		Thd_InitList(&curr);
		Thd_InitList(&next);

		__g_mark = 0;
		matched = -1;
		
		__g_mark++;
		add_thread(&curr, pc, &sp);

		for(;;)
		{
				if(curr.count == 0)break;

				__g_mark++;
				for(i = 0; i < curr.count; ++i)
				{
						pc = curr.lst[i];
						switch(pc->opcode)
						{
						case RGX_CHAR_I:
						{
								int_t id = RGX_FindIndexFromCClass(cclass, *sp);
								if(id == pc->data)
								{
										add_thread(&next, pc + 1, &sp);
										if(*sp != L'\0')sp++;
								}
								break;
						}
						case RGX_BEGIN_I:
						{
								if(sp == input_beg)
								{
										add_thread(&next, pc + 1, &sp);
								}
								break;
						}
						case RGX_END_I:
						{
								if(*sp == L'\0')
								{
										add_thread(&next, pc + 1, &sp);
								}
								break;
						}
						case RGX_MATCH_I:
						{
								matched = sp - input_beg;
								goto BREAK_POINT;
								break;
						}
						}
				}
BREAK_POINT:
				Thd_Swap(&curr, &next);
				Thd_Clear(&next);
				//if(*sp == L'\0')break;
		}
		
		return matched;

}





bool_t			RGX_Run(const rgxIns_t *pc, const rgxCClass_t *cclass, lexMatch_t *match)
{
		sp_top = 0;
		int_t cnt = thompsonvm(pc, match->next, cclass, match->input);
		//int_t cnt = __backtrack(pc, match->next, cclass, match->input);
		
		if(cnt > 0)
		{
				AR_printf(L"%ls\r\n", AR_wcsndup(match->next, (size_t)cnt));
				return true;
		}else
		{
				return false;
		}
}



void rgx_test_1(const wchar_t *expr, const wchar_t *input)
{
		rgxNameSet_t	name_set;
		rgxResult_t		result;
		rgxNode_t		*final,*cat;
		rgxCClass_t		cclass;
		rgxProg_t		prog;
		
		AR_ASSERT(expr != NULL);

		RGX_InitNameSet(&name_set);
		
		//result = RGX_ParseExpr(L"(?=abc)(([a-z]*)+)?(?=.(?=\"/\"(?=[^c][\\r\\n])))+", &name_set);

		result = RGX_ParseExpr(expr, &name_set);

		if(result.node == NULL)
		{
				AR_printf(L"error == %ls\r\n", result.err.pos);
				return;
		}

		if(result.node)
		{
				final = RGX_CreateNode(RGX_FINAL_T);
				cat	  = RGX_CreateNode(RGX_CAT_T);

				if(result.node == NULL)AR_printf(L"%ls\r\n", result.err.pos);
				cat->left = result.node;
				cat->right = final;
				result.node = cat;
		}

//		getchar();

		if(result.node)
		{

				
				size_t i;

				RGX_InitCClass(&cclass);
				RGX_CalcCharClass(result.node, &cclass);

				for(i = 0; i < cclass.count; ++i)
				{
						printf("id%d : %d-%d\r\n", i,cclass.cclass[i].beg, cclass.cclass[i].end);
				}

				RGX_SplitCharSetToCClassID(result.node, &cclass);
				//RGX_UnInitCClass(&cclass);
				
				RGX_CorrectTree(result.node);

		}

		if(result.node)
		{
				arString_t *str = AR_CreateString();
				arString_t *str2 = AR_CreateString();

				RGX_ToString(result.node, str);
				RGX_ToString(RGX_CopyNode(result.node), str2);

				
				AR_printf(L"%ls\r\n", AR_GetStrString(str));
				AR_printf(L"%ls\r\n", AR_GetStrString(str2));

				AR_DestroyString(str);
		}

		RGX_InitProg(&prog);

		if(result.node)
		{
				
				arString_t *str;

				
				
				RGX_Compile(&prog, result.node);
				
				str = AR_CreateString();
				
				RGX_PringProg(&prog, str);

				AR_StrPrint(str);
				
				AR_DestroyString(str);

		}
		
		if(result.node)
		{
				getchar();
				lexMatch_t match;
				
				LEX_InitMatch(&match, input);

				
				AR_printf(L"RGX_Run result == %d\r\n", RGX_Run(prog.start, &cclass, &match));

				LEX_UnInitMatch(&match);
				RGX_UnInitCClass(&cclass);
				RGX_DestroyNode(result.node);
		}

		RGX_UnInitProg(&prog);
}
#endif




void rgx_test_1(const wchar_t *expr, const wchar_t *input)
{
		rgxNameSet_t	name_set;
		rgxResult_t		result;
		rgxNode_t		*final,*cat;
		rgxProg_t		prog;
		
		AR_ASSERT(expr != NULL);

		RGX_InitNameSet(&name_set);
		
		//result = RGX_ParseExpr(L"(?=abc)(([a-z]*)+)?(?=.(?=\"/\"(?=[^c][\\r\\n])))+", &name_set);

		result = RGX_ParseExpr(expr, &name_set);

		if(result.node == NULL)
		{
				AR_printf(L"error == %ls\r\n", result.err.pos);
				return;
		}

		if(result.node)
		{
				final = RGX_CreateNode(RGX_FINAL_T);
				cat	  = RGX_CreateNode(RGX_CAT_T);

				if(result.node == NULL)AR_printf(L"%ls\r\n", result.err.pos);
				cat->left = result.node;
				cat->right = final;
				result.node = cat;
		}

//		getchar();

		if(result.node)
		{
				RGX_CorrectTree(result.node);
		}

		if(result.node)
		{
				arString_t *str = AR_CreateString();
				arString_t *str2 = AR_CreateString();

				RGX_ToString(result.node, str);
				RGX_ToString(RGX_CopyNode(result.node), str2);

				
				AR_printf(L"%ls\r\n", AR_GetStrString(str));
				AR_printf(L"%ls\r\n", AR_GetStrString(str2));

				AR_DestroyString(str);
		}

}


void rgx_test()
{
		while(true)
		{
				wchar_t buf[1024], input[1024];
				_getws(buf);
				_getws(input);
				if(AR_wcscmp(buf, L"quit") == 0)break;
				rgx_test_1(buf, input);
		}

}


AR_NAMESPACE_END



#endif
