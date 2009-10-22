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

#include "expr.h"

AR_NAMESPACE_BEGIN




bool_t			LEX_InsertToNameList(lexName_t **dest, const wchar_t *name, lexNode_t *node)
{
		
		lexName_t *prev, *curr, *tmp;
		AR_ASSERT(dest != NULL && name != NULL && node != NULL);
		AR_ASSERT(AR_wcslen(name) < AR_MAX_LEXNAME);

		prev = NULL; curr = *dest;
		
		while(curr != NULL)
		{
				int cmp = AR_wcscmp(curr->name, name);
				if(cmp == 0)
				{
						return false;
				}else if(cmp < 0)
				{
						prev = curr;
						curr = curr->next;
				}else
				{
						break;
				}
		}

		tmp = AR_NEW0(lexName_t);
		AR_wcscpy(tmp->name, name);
		tmp->node = node;

		if(prev == NULL)
		{
				AR_ASSERT(curr == *dest);
				tmp->next = *dest;
				*dest = tmp;
		}else
		{
				prev->next = tmp;
				tmp->next = curr;
		}
		return true;
}


void			LEX_DestroyNameList(lexName_t *lst)
{		

		lexName_t *curr,*tmp;

		curr = lst;
		while(curr != NULL)
		{
				tmp = curr->next;
				LEX_DestroyNode(curr->node);
				AR_DEL(curr);
				curr = tmp;
		}
}


lexNode_t*		LEX_FindFromNameList(lexName_t *lst, const wchar_t *name)
{
		lexName_t *curr;
		
		curr = lst;
		while(curr != NULL)
		{
				int cmp = AR_wcscmp(curr->name, name);
				if(cmp == 0)
				{
						return curr->node;
				}else 
				{
						curr = curr->next;
				}
		}
		return NULL;
}

/*****************************************************************************************************************/



#if(0)
const wchar_t*	__transform_char(const wchar_t *input, wchar_t *c, lexError_t *err)
{
		const wchar_t	*p; 
		AR_ASSERT(input != NULL && c != NULL && err != NULL);
		err->pos = NULL;

		p = input;
		switch(*p)
		{
		case L'\0':
				err->pos = p;
				return NULL;
		case L'{':
		case L'}':
		case L'[':
		case L']':
		case L'(':
		case L')':
		case L'\\':
		case L'?':
		case L'+':
		case L'-':
		case L'*':
		case L'/':
		case L'|':
		case L'^':
		case L'$':
		case L'"':
				*c = *p;
				return ++p;
		case L'a': 
				*c = L'\a';
				return ++p;
		case L'f': 
				*c = L'\f';
				return ++p;
		case L'n': 
				*c = L'\n';
				return ++p;
		case L'r': 
				*c = L'\r';
				return ++p;
		case L't': 
				*c = L'\t';
				return ++p;
		case L'v': 
				*c = L'\v';
				return ++p;
		case L'0':
				*c = L'\0';
				return ++p;
		case L'u':
		{
				size_t i; size_t val; wchar_t vc;
				p += 1;
				for(i = 0,val = 0; i < 4; ++i)
				{
						vc = p[i];
						val *= 16;
						
						if(vc >= L'0' && vc <= L'9')
						{
								val += (vc - L'0');
						}else if(vc >= L'A' && vc <= L'F')
						{
								val += 10;
								val += (vc - L'A');
						}else if(vc >= L'a' && vc <= L'f')
						{
								val += 10;
								val += (vc - L'a');
						}else if(vc == L'\0')
						{
								err->pos = p;
								return NULL;	
						}else
						{
								err->pos = p;
								return NULL;
						}
				}
				*c = (wchar_t)val;
				p += 4;
				return p;
		}
				break;
		case L'x':
		{
				
				size_t i; size_t val; wchar_t vc; 
				p += 1;
				val = 0;
				if(*p != L'{')
				{
						for(i = 0; i < 2; ++i)
						{
								vc = p[i];
								val *= 16;

								if(vc >= L'0' && vc <= L'9')
								{
										val += (vc - L'0');
								}else if(vc >= L'A' && vc <= L'F')
								{
										val += 10;
										val += (vc - L'A');
								}else if(vc >= L'a' && vc <= L'f')
								{
										val += 10;
										val += (vc - L'a');
								}else if(vc == L'\0')
								{
										err->pos = p;
										return NULL;	
								}else
								{
										err->pos = p;
										return NULL;
								}
						}
						p += 2;
				}else
				{
						p+=1;
						while(*p != L'}')
						{
								val *= 16;
								vc = *p++;
								if(vc >= L'0' && vc <= L'9')
								{
										val += (vc - L'0');
								}else if(vc >= L'A' && vc <= L'F')
								{
										val += 10;
										val += (vc - L'A');
								}else if(vc >= L'a' && vc <= L'f')
								{
										val += 10;
										val += (vc - L'a');
								}else if(vc == L'\0')
								{
										err->pos = p;
										return NULL;	
								}else
								{
										err->pos = p;
										return NULL;
								}
						}
						AR_ASSERT(*p == L'}');
						p++;
				}
				
				*c = (wchar_t)val;
				return p;
		}
				break;
		default:
				err->pos = p;
				return NULL;
		}
}

#endif



const wchar_t*	__transform_char(const wchar_t *input, wchar_t *c, lexError_t *err)
{
		const wchar_t	*p; 
		AR_ASSERT(input != NULL && c != NULL && err != NULL);
		err->pos = NULL;

		p = input;
		switch(*p)
		{
		case L'\0':
				err->pos = p;
				return NULL;
		case L'{':
		case L'}':
		case L'[':
		case L']':
		case L'(':
		case L')':
		case L'\\':
		case L'?':
		case L'+':
		case L'-':
		case L'*':
		case L'/':
		case L'|':
		case L'^':
		case L'$':
		case L'"':
				*c = *p;
				return ++p;
		case L'a': 
				*c = L'\a';
				return ++p;
		case L'f': 
				*c = L'\f';
				return ++p;
		case L'n': 
				*c = L'\n';
				return ++p;
		case L'r': 
				*c = L'\r';
				return ++p;
		case L't': 
				*c = L'\t';
				return ++p;
		case L'v': 
				*c = L'\v';
				return ++p;
		case L'0':
				*c = L'\0';
				return ++p;
		case L'u':
		{
				const wchar_t *ret; uint32_t val;
				p += 1;
				ret = AR_wtou32_s(p,p+4, &val, 16);
				if(ret == NULL)
				{
						err->pos = p;
						return NULL;
				}else
				{
						*c = (wchar_t)val;
						p = ret;
				}
				return p;
		}
				break;
		case L'x':
		{
				const wchar_t *ret;  uint32_t val;

				p += 1;
				
				if(*p != L'{')
				{
						ret = AR_wtou32_s(p, p + 2, &val, 16);
						if(ret == NULL)
						{
								err->pos = p;
								return NULL;
						}else
						{
								*c = (wchar_t)val;
								p = ret;
						}
				}else
				{
						const wchar_t *end;
						p+=1;
						end = p;
						
						while(*end != L'\0' && *end != L'}')end++;
						
						ret = AR_wtou32_s(p, end, &val, 16);

						if(ret)
						{
								ret = AR_wcstrim_space(ret);
								if(*ret != L'}')ret = NULL;
						}

						if(ret == NULL)
						{
								err->pos = p;
								return NULL;
						}else
						{
								*c = (wchar_t)val;
								p = ret;
						}

						AR_ASSERT(*p == L'}');
						p++;
				}
				return p;
		}
				break;
		default:
				err->pos = p;
				return NULL;
		}
}

lexResult_t	__handle_quote(const wchar_t *input)
{
		const wchar_t *p; lexResult_t		g_res; 
		AR_ASSERT(input != NULL);
		p = input;
		g_res.next = NULL; g_res.node = LEX_CreateNode(LEX_CAT);

		while(*p != L'"')
		{
				wchar_t c;
				lexCharRange_t range;
				lexNode_t		*tmp = NULL;

				if(*p == L'\0')
				{
						g_res.err.pos = p;
						goto INVALID_POINT;
				}else if(*p == L'\\')
				{
						c = *(p + 1);

						if(c == L'\0')
						{
								g_res.err.pos = p;
								goto INVALID_POINT;
						}else if(c == L'"')
						{
								p++;
						}else
						{


						}
				}else
				{
						c = *p;
				}

				range.beg = range.end = c;
				tmp = LEX_CreateNode(LEX_CHARSET);
				LEX_InsertRangeToCharSet(&tmp->charset, &range);
				LEX_InsertNodeToCat(&g_res.node->cat, tmp);
				++p;
		}
		AR_ASSERT(*p == L'"');
		
		if(g_res.node->cat.left == NULL)
		{
				g_res.err.pos = p;
				goto INVALID_POINT;
		}

		g_res.next = ++p;
		return g_res;
INVALID_POINT:
		g_res.next = NULL;
		if(g_res.node != NULL)
		{
				LEX_DestroyNode(g_res.node);
				g_res.node = NULL;
		}
		return g_res;
}




const wchar_t* __get_charset(const wchar_t *input, wchar_t *c, lexError_t *err)
{
		switch(*input)
		{
		case L'\0':
				err->pos = input;
				return NULL;
		case L'\a': 
		case L'\f': 
		case L'\n': 
		case L'\r': 
		case L'\t': 
		case L'\v': 
//				AR_ASSERT(0);
				err->pos = input;
				return NULL;
		case L'{':
		case L'}':
		case L'[':
		case L']':
		case L'(':
		case L')':
		case L'?':
		case L'+':
		case L'-':
		case L'*':
		case L'/':
		case L'|':
		case L'^':
		case L'$':
		case L'"':
				err->pos = input;
				return NULL;
		case L'\\':
				return __transform_char(input + 1, c, err);
		default:
				*c = *input;
				return ++input;
		}
}





lexResult_t	__handle_cset_range(const wchar_t *input)
{
		const wchar_t *p; 
		lexResult_t		g_res; 
		lexCharSet_t	cset;

		AR_ASSERT(input != NULL);
		p = input;
		g_res.next = NULL; g_res.node = NULL;
		
		LEX_InitCharSet(&cset);
		
		if(*p == L'^') {cset.is_neg = true; p++;}
		
		while(*p != L']')
		{
				lexCharRange_t	range;	lexError_t err;
				p = __get_charset(p, &range.beg, &err);
				if(p == NULL)
				{
						g_res.err = err;
						goto INVALID_POINT;
				}else
				{
						if(*p == L'\0')
						{
								g_res.err.pos = p;
								goto INVALID_POINT;
						}

						if(*p == L'-')
						{
								p = __get_charset(p + 1, &range.end, &err);
								if(p == NULL)
								{
										g_res.err = err;
										goto INVALID_POINT;
								}
						}else
						{
								range.end = range.beg;
						}
				}
				LEX_InsertRangeToCharSet(&cset, &range);
		}

		AR_ASSERT(*p == L']');
		g_res.next = ++p;
		g_res.node = LEX_CreateNode(LEX_CHARSET);
		g_res.node->charset = cset;
		cset.range = NULL;
		LEX_UnInitCharSet(&cset);

		if(g_res.node->charset.is_neg)
		{
				LEX_ReverseNegativeCharSet(&g_res.node->charset);
		}

		return g_res;
INVALID_POINT:
		LEX_UnInitCharSet(&cset);
		g_res.next = NULL;
		if(g_res.node != NULL) 
		{
				LEX_DestroyNode(g_res.node);
				g_res.node = NULL;
		}
		return g_res;
}


lexResult_t	__handle_charset(const wchar_t *input)
{
		const wchar_t *p; 
		lexResult_t		g_res; 
		AR_ASSERT(input != NULL);
		p = input;
		g_res.next = NULL; g_res.node = NULL;

		if(*p == L'[')
		{
				return __handle_cset_range(p+1);
		}else
		{
				lexCharRange_t range;
				p = __get_charset(p, &range.beg, &g_res.err);
				if(p == NULL)
				{
						return g_res;
				}else
				{
						range.end = range.beg;
						g_res.node = LEX_CreateNode(LEX_CHARSET);
						LEX_InsertRangeToCharSet(&(g_res.node->charset), &range);
						g_res.next = p;
						return g_res;
				}
		}
}




lexNode_t*	__handle_loopcount(lexNode_t *expr, size_t min, size_t max)
{
		bool_t is_infinite;
		lexNode_t *cat, *branch, *loop;
		size_t i;
		AR_ASSERT(expr != NULL && min <= max && max > 0);

		is_infinite = (max == AR_SIZE_MAX ? true : false);
RECHECK:
		if(min < max)
		{
				if(min == 0)
				{
						if(is_infinite)
						{
								min = max; 
								goto RECHECK;
						}else
						{
								branch = LEX_CreateNode(LEX_BRANCH);
								
								LEX_InsertNodeToBranch(&branch->branch, LEX_CopyNode(expr));
								LEX_InsertNodeToBranch(&branch->branch, EPSILON_NODE());
								cat = LEX_CreateNode(LEX_CAT);
								for(i = 0; i < max; ++i)LEX_InsertNodeToCat(&cat->cat, LEX_CopyNode(branch));
								LEX_DestroyNode(branch);
								return cat;
						}
				}else
				{
						cat = LEX_CreateNode(LEX_CAT);
						for(i = 0; i < min; ++i)LEX_InsertNodeToCat(&cat->cat, LEX_CopyNode(expr));

						if(is_infinite)
						{
								loop = LEX_CreateNode(LEX_LOOP);
								loop->loop = LEX_CopyNode(expr);
								LEX_InsertNodeToCat(&cat->cat, loop);
						}else
						{
								branch = LEX_CreateNode(LEX_BRANCH);
								LEX_InsertNodeToBranch(&branch->branch, LEX_CopyNode(expr));
								LEX_InsertNodeToBranch(&branch->branch, EPSILON_NODE());
								for(; i < max; i++)LEX_InsertNodeToCat(&cat->cat, LEX_CopyNode(branch));

								LEX_DestroyNode(branch);
						}
						LEX_DestroyNode(expr);
						return cat;
				}


		}else/* if(min == max)*/
		{
				if(is_infinite)
				{
						loop = LEX_CreateNode(LEX_LOOP);
						loop->loop = expr;
						return loop;
				}else /*if(max > 0)*/
				{
						cat = LEX_CreateNode(LEX_CAT);
						for(i = 0; i < min; ++i) LEX_InsertNodeToCat(&cat->cat, LEX_CopyNode(expr));
						LEX_DestroyNode(expr);
						return cat;
				}
		}
}



lexResult_t	__handle_postfix(lexNode_t *expr, const wchar_t *input)
{
		const wchar_t *p; 
		lexResult_t		g_res; 
		AR_ASSERT(input != NULL);
		p = input;
		g_res.next = NULL; g_res.node = NULL;
		
		switch(*p)
		{
		case '*':
		{
				g_res.node = __handle_loopcount(expr, 0, AR_SIZE_MAX);
				g_res.next = ++p;
		}
				break;
		case '+':
		{
				g_res.node = __handle_loopcount(expr, 1, AR_SIZE_MAX);
				g_res.next = ++p;
		}
				break;
		case '?':
		{
				g_res.node = __handle_loopcount(expr, 0,1);
				g_res.next = ++p;
		}
				break;
		case '{':
		{
				const wchar_t *beg = p + 1;
				uint_t min,max;
				
				/*beg = AR_wcstrim(beg,L" \t");*/
				beg = AR_wcstrim_space(beg);

				beg = AR_wtou(beg, &min, 10);

				if(beg == NULL) 
				{
						g_res.err.pos = NULL;
						g_res.next = p;
						g_res.node = expr;
						return g_res;
				}
				
				/*beg = AR_wcstrim(beg,L" \t");*/
				beg = AR_wcstrim_space(beg);

				if(*beg == L',')
				{
						beg++;
						/*beg = AR_wcstrim(beg,L" \t");*/
						beg = AR_wcstrim_space(beg);
						
						if(*beg == L'}')
						{
								max = AR_SIZE_MAX;
						}else
						{
								beg = AR_wtou(beg, &max, 10);
								if(beg == NULL) { g_res.err.pos = p; goto INVALID_POINT;}
								/*beg = AR_wcstrim(beg,L" \t");*/
								beg = AR_wcstrim_space(beg);
								if(*beg != L'}'){ g_res.err.pos = p; goto INVALID_POINT;}
						}
				}else if(*beg == L'}')
				{
						max = min;

				}else
				{
						{ g_res.err.pos = p; goto INVALID_POINT;}
				}
				
				if(min > max || max == 0){ g_res.err.pos = p; goto INVALID_POINT;}

				g_res.next = ++beg;
				g_res.node = __handle_loopcount(expr, min,max);
		}
				break;
		default:
				g_res.err.pos = p;
				g_res.next = input;
				g_res.node = expr;
				return g_res;
		}

		return __handle_postfix(g_res.node, g_res.next);
INVALID_POINT:
		LEX_DestroyNode(expr);
		g_res.next = NULL;
		g_res.node = NULL;
		return g_res;

}



lexResult_t __handle_expr(const wchar_t *input, wchar_t tc, lexName_t *lst);

lexResult_t __handle_factor(const wchar_t *input,lexName_t *lst)
{
		const wchar_t *p; 
		lexResult_t		g_res; 
		AR_ASSERT(input != NULL);
		p = input;
		g_res.err.pos = NULL; g_res.next = NULL; g_res.node = NULL;
		
		switch(*p)
		{
		case L'"':
				g_res = __handle_quote(p + 1);
				break;
		case L'(':
				g_res = __handle_expr(p + 1, L')', lst);
				break;
		case L'{':
		{
				const wchar_t *beg;
				size_t i, count;
				p += 1;
				/*p = AR_wcstrim(p, L" \t");*/
				p = AR_wcstrim_space(p);
				/*if((*p >= L'A' && *p <= L'Z') || (*p >= L'a' && *p <= L'z') || *p == L'_')*/
				if(AR_iswalnum(*p) || *p == L'_')
				{
						beg = p; count = 0;
						
						/*while((*p >= L'A' && *p <= L'Z') || (*p >= L'a' && *p <= L'z') || *p == L'_' || (*p >= L'0' && *p <= L'9'))*/
						while(AR_iswalnum(*p) || *p == L'_')
						{
								p++;
								count++;
						}

						/*p = AR_wcstrim(p, L" \t");*/
						p = AR_wcstrim_space(p);

						if(*p != L'}' || count >= AR_MAX_LEXNAME)
						{
								AR_ASSERT(0);
								g_res.err.pos = p;
								g_res.next = NULL;
								g_res.node = NULL;
								return g_res;
						}else
						{
								lexNode_t *new_node; wchar_t name[AR_MAX_LEXNAME];
								
								for(i = 0; i < count; ++i) name[i] = beg[i];
								name[count] = L'\0';
								new_node = LEX_FindFromNameList(lst, name);
								
								if(new_node == NULL)
								{
										/*AR_ASSERT(0);*/
										g_res.err.pos = p - count - 1;
										g_res.next = NULL;
										g_res.node = NULL;
										return g_res;
								}else
								{
										g_res.node = LEX_CopyNode(new_node);
										g_res.next = ++p;
										g_res.err.pos = NULL;
								}
						}

				}else
				{
						AR_ASSERT(0);
						g_res.err.pos = p;
						g_res.next = NULL;
						g_res.node = NULL;
						return g_res;
				}
		}
				break;
		default:
				g_res = __handle_charset(p);
		}

		if(g_res.next != NULL)
		{
				return __handle_postfix(g_res.node, g_res.next);
		}else
		{
				return g_res;
		}


}



lexResult_t __handle_expr(const wchar_t *input, wchar_t tc, lexName_t *lst)
{

		lexNode_t		*branch, *cat;
		const wchar_t *p; lexResult_t g_res; 
		
		AR_ASSERT(input != NULL);

		branch = NULL; cat = NULL;
		
		p = input; g_res.err.pos = NULL; g_res.next = NULL; g_res.node = NULL;

		while(*p != L'\0' && *p != tc)
		{
				lexResult_t tmp = __handle_factor(p, lst);
				if(tmp.next == NULL)
				{
						g_res.err = tmp.err;
						goto INVALID_POINT;
				}
				
				p = tmp.next;
				
				if(*p == L'|')
				{
						if(branch == NULL) branch = LEX_CreateNode(LEX_BRANCH);
						
						if(cat == NULL)
						{
								LEX_InsertNodeToBranch(&branch->branch, tmp.node);
						}else
						{
								LEX_InsertNodeToCat(&cat->cat, tmp.node);
								LEX_InsertNodeToBranch(&branch->branch, cat);
								cat = NULL;
						}
						p++;
				}else
				{
						if(cat == NULL)cat =  LEX_CreateNode(LEX_CAT);
						LEX_InsertNodeToCat(&cat->cat, tmp.node);
				}
		}
		
		if(*p != tc){ g_res.err.pos = p; goto INVALID_POINT;}
		if(cat == NULL && branch == NULL) { g_res.err.pos = p; goto INVALID_POINT;}
		
		
		if(cat != NULL && branch != NULL)
		{
				LEX_InsertNodeToBranch(&branch->branch, cat);
				cat = NULL;
		}
		
		g_res.next = (*p == L'\0' ? p : ++p);
		g_res.node = (branch != NULL ? branch : cat);
		
		return g_res;

INVALID_POINT:
		if(cat != NULL)LEX_DestroyNode(cat);
		if(branch != NULL) LEX_DestroyNode(branch);
		if(g_res.node != NULL) LEX_DestroyNode(g_res.node);

		g_res.node = NULL; g_res.next = NULL;
		return g_res;
}



lexResult_t		LEX_CompileExpr(const wchar_t *expr, lexName_t *name_set)
{
		return __handle_expr(expr, L'\0', name_set);

}


AR_NAMESPACE_END
