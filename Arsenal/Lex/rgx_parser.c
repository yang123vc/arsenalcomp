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


#ifndef __REGEX_PARSER_H__
#define __REGEX_PARSER_H__

#include "rgx.h"


AR_NAMESPACE_BEGIN


static const wchar_t*	__transform_char(const wchar_t *input, wchar_t *c, rgxError_t *err)
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
		case L'|':
		case L'^':
		case L'$':
		case L'"':
		case L'.':
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
				ret = AR_wtou32_s(p,p+4, &val, 10);
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



static rgxResult_t	__handle_quote(const wchar_t *input)
{
		const wchar_t *p; 
		rgxResult_t		g_res; 
		AR_ASSERT(input != NULL);
		p = input;
		g_res.next = NULL; 
		g_res.node = RGX_CreateNode(RGX_CAT_T);

		while(*p != L'"')
		{
				wchar_t c;
				rgxCharRange_t range;
				rgxNode_t		*tmp = NULL;

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
				tmp = RGX_CreateNode(RGX_CSET_T);
				tmp->range.beg = tmp->range.end = c;
				RGX_InsertToNode(g_res.node, tmp);
				++p;
		}
		AR_ASSERT(*p == L'"');
		
		if(g_res.node->left == NULL && g_res.node->right == NULL)
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
				RGX_DestroyNode(g_res.node);
				g_res.node = NULL;
		}
		return g_res;
}


static const wchar_t* __get_charset(const wchar_t *input, wchar_t *c, rgxError_t *err)
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
		case L'|':
		case L'"':
		case L'^':
		case L'$':
		case L'.':
				err->pos = input;
				return NULL;
		case L'\\':
				return __transform_char(input + 1, c, err);
		default:
				*c = *input;
				return ++input;
		}
}




static rgxResult_t	__handle_cset_range(const wchar_t *input)
{
		const wchar_t *p; 
		rgxResult_t		g_res; 
		rgxCharSet_t	cset;
		rgxCharRange_t	*range = NULL;
		AR_ASSERT(input != NULL);
		p = input;

		g_res.next = NULL; 
		g_res.node = NULL;
		AR_memset(&g_res, 0, sizeof(g_res));
		
		RGX_InitCharSet(&cset);
		
		if(*p == L'^') {cset.is_neg = true; p++;}
		
		while(*p != L']')
		{
				rgxCharRange_t	range;	rgxError_t err;
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
				RGX_InsertRangeToCharSet(&cset, &range);
		}

		AR_ASSERT(*p == L']');

		if(cset.is_neg)
		{
				RGX_ReverseNegativeCharSet(&cset);
		}

		g_res.next = ++p;
		AR_ASSERT(cset.range != NULL);

		g_res.node = RGX_CreateNode(RGX_BRANCH_T);

		for(range = cset.range; range; range = range->next)
		{
				rgxNode_t *tmp = RGX_CreateNode(RGX_CSET_T);
				tmp->range.beg = range->beg;
				tmp->range.end = range->end;
				RGX_InsertToNode(g_res.node, tmp);
		}
		RGX_UnInitCharSet(&cset);

		
		return g_res;
INVALID_POINT:
		RGX_UnInitCharSet(&cset);
		g_res.next = NULL;
		if(g_res.node != NULL) 
		{
				RGX_DestroyNode(g_res.node);
				g_res.node = NULL;
		}
		return g_res;
}


static rgxResult_t	__handle_charset(const wchar_t *input)
{
		const wchar_t *p; 
		rgxResult_t		g_res; 
		AR_ASSERT(input != NULL);
		p = input;

		AR_memset(&g_res, 0, sizeof(g_res));

		if(*p == L'[')
		{
				return __handle_cset_range(p+1);
		}else if(*p == L'.')
		{
				g_res = __handle_charset(L"[^\\0]");
				AR_ASSERT(g_res.node != NULL && g_res.err.pos == NULL);
				g_res.next = ++p;
				return g_res;
		}else
		{
				rgxCharRange_t range;
				
				p = __get_charset(p, &range.beg, &g_res.err);
				
				if(p == NULL)
				{
						return g_res;
				}else
				{
						range.end = range.beg;
						g_res.node = RGX_CreateNode(RGX_CSET_T);
						g_res.node->range.beg = range.beg;
						g_res.node->range.end = range.end;
						g_res.next = p;
						return g_res;
				}
		}
}


static rgxNode_t*	__handle_loopcount(rgxNode_t *expr, size_t min, size_t max, bool_t non_greedy)
{
		bool_t is_infinite;
		rgxNode_t *cat, *loop;
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
								rgxNode_t *quest = RGX_CreateNode(RGX_QUEST_T);
								quest->left = RGX_CopyNode(expr);
								quest->non_greedy = non_greedy;
								//branch = RGX_CreateNode(RGX_BRANCH_T);
								//RGX_InsertToNode(branch, RGX_CopyNode(expr));
								cat = RGX_CreateNode(RGX_CAT_T);
								for(i = 0; i < max; ++i)RGX_InsertToNode(cat, RGX_CopyNode(quest));
								
								RGX_DestroyNode(quest);
								RGX_DestroyNode(expr);

						}
				}else
				{
						cat = RGX_CreateNode(RGX_CAT_T);
						for(i = 0; i < min; ++i)RGX_InsertToNode(cat, RGX_CopyNode(expr));

						if(is_infinite)
						{
								loop = RGX_CreateNode(RGX_STAR_T);
								loop->left = RGX_CopyNode(expr);
								loop->non_greedy = non_greedy;
								RGX_InsertToNode(cat, loop);
						}else
						{
								rgxNode_t *quest = RGX_CreateNode(RGX_QUEST_T);
								quest->left = RGX_CopyNode(expr);
								quest->non_greedy = non_greedy;
								for(; i < max; i++)RGX_InsertToNode(cat, RGX_CopyNode(quest));
								RGX_DestroyNode(quest);
								RGX_DestroyNode(expr);
						}
						
						
				}

				return cat;

		}else/* if(min == max)*/
		{

				if(is_infinite)
				{
						loop = RGX_CreateNode(RGX_STAR_T);
						loop->left = expr;
						return loop;
				}else /*if(max > 0)*/
				{
						cat = RGX_CreateNode(RGX_CAT_T);
						for(i = 0; i < min; ++i) RGX_InsertToNode(cat, RGX_CopyNode(expr));
						RGX_DestroyNode(expr);
						return cat;
				}

		}
}


/*static rgxResult_t	__handle_factor(const wchar_t *input, const rgxNameSet_t *name_set);*/
static rgxResult_t	__handle_expr(const wchar_t *input, wchar_t tc, const rgxNameSet_t *name_set);

static rgxResult_t	__handle_postfix(rgxNode_t *expr, const wchar_t *input, const rgxNameSet_t *name_set)
{
		const wchar_t *p; 
		rgxResult_t		g_res; 
		AR_ASSERT(input != NULL && expr != NULL);
		p = input;

		AR_memset(&g_res, 0, sizeof(g_res));
		
		switch(*p)
		{
		case '*':
		{
				//g_res.node = __handle_loopcount(expr, 0, AR_SIZE_MAX);
				g_res.node = RGX_CreateNode(RGX_STAR_T);
				g_res.node->left = expr;
				++p;
				if(*p == L'?')
				{
						g_res.node->non_greedy = true;
						++p;
				}
				g_res.next = p;
		}
				break;
		case '+':
		{
				g_res.node = RGX_CreateNode(RGX_PLUS_T);
				g_res.node->left = expr;
				
				++p;
				if(*p == L'?')
				{
						g_res.node->non_greedy = true;
						++p;
				}
				g_res.next = p;
		}
				break;
		case '?':
		{
				//g_res.node = __handle_loopcount(expr, 0,1);
				g_res.node = RGX_CreateNode(RGX_QUEST_T);
				g_res.node->left = expr;
				
				++p;
				if(*p == L'?')
				{
						g_res.node->non_greedy = true;
						++p;
				}
				g_res.next = p;
		}
				break;
		case '{':
		{
				bool_t non_greedy = false;
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
				
				++beg;

				if(*beg == L'?')
				{
						non_greedy = true;
						++beg;
				}
				
				g_res.next = beg;
				g_res.node = __handle_loopcount(expr, min,max, non_greedy);
		}
				break;
		default:
				g_res.err.pos = p;
				g_res.next = input;
				g_res.node = expr;
				return g_res;
		}

		//return __handle_postfix(g_res.node, g_res.next, name_set);
		return g_res;
INVALID_POINT:
		RGX_DestroyNode(expr);
		g_res.next = NULL;
		g_res.node = NULL;
		return g_res;
}



static rgxResult_t __handle_factor(const wchar_t *input, const rgxNameSet_t *name_set)
{
		const wchar_t *p; 
		rgxResult_t		g_res; 
		
		AR_ASSERT(input != NULL);
		p = input;
		g_res.err.pos = NULL; g_res.next = NULL; g_res.node = NULL;
		
		switch(*p)
		{
		case L'^':
		case L'$':
		{
				g_res.node =  RGX_CreateNode(*p == L'^' ? RGX_BEGIN_T : RGX_END_T);
				g_res.next = ++p;
				return g_res;
				break;
		}
		case L'"':
				g_res = __handle_quote(p + 1);
				break;
		case L'(':
		{
				++p;
				
				if(*p == L'?')
				{
						++p;
						
						if(*p == L'=' || *p == L'!')
						{
								rgxResult_t		result;
								result = __handle_expr(p + 1, L')', name_set);

								if(result.node == NULL)return result;
								g_res.next = result.next;
								g_res.err = result.err;
								g_res.node = RGX_CreateNode(RGX_LOOKAHEAD_T);
								g_res.node->left = result.node;

								if(*p == L'=')
								{
										g_res.node->negative_lookahead = false;
								}else
								{
										g_res.node->negative_lookahead = true;
								}

						}else
						{
								g_res.err.pos = p;
								g_res.next = NULL;
								g_res.node = NULL;
								return g_res;

						}
				}else
				{
						g_res = __handle_expr(p, L')', name_set);
				}

		}
				break;
		case L'{':
		{
				const wchar_t *beg;
				size_t i, count;
				p += 1;
				p = AR_wcstrim_space(p);
				if(AR_iswalnum(*p) || *p == L'_')
				{
						beg = p; count = 0;
						
						while(AR_iswalnum(*p) || *p == L'_')
						{
								p++;
								count++;
						}
						
						p = AR_wcstrim_space(p);

						if(*p != L'}' || count >= AR_RGX_MAXNAME)
						{
							//	AR_ASSERT(0);
								g_res.err.pos = p;
								g_res.next = NULL;
								g_res.node = NULL;
								return g_res;
						}else
						{
								const rgxNode_t *new_node; wchar_t name[AR_RGX_MAXNAME];
								
								for(i = 0; i < count; ++i) name[i] = beg[i];
								name[count] = L'\0';
								new_node = RGX_FindFromNameSet(name_set, name);
								
								if(new_node == NULL)
								{
										g_res.err.pos = p - count - 1;
										g_res.next = NULL;
										g_res.node = NULL;
										return g_res;
								}else
								{
										g_res.node = RGX_CopyNode(new_node);
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
				return __handle_postfix(g_res.node, g_res.next,name_set);
		}else
		{
				return g_res;
		}
}



static rgxResult_t __handle_expr(const wchar_t *input, wchar_t tc, const rgxNameSet_t *name_set)
{

		rgxNode_t		*branch, *cat;
		const wchar_t *p; rgxResult_t g_res; 
		
		AR_ASSERT(input != NULL);

		branch = NULL; cat = NULL;
		
		p = input; g_res.err.pos = NULL; g_res.next = NULL; g_res.node = NULL;

		while(*p != L'\0' && *p != tc)
		{
				rgxResult_t tmp = __handle_factor(p, name_set);
				if(tmp.next == NULL)
				{
						g_res.err = tmp.err;
						goto INVALID_POINT;
				}
				
				p = tmp.next;
				
				if(*p == L'|')
				{
						if(branch == NULL) branch = RGX_CreateNode(RGX_BRANCH_T);
						
						if(cat == NULL)
						{
								RGX_InsertToNode(branch, tmp.node);
						}else
						{
								RGX_InsertToNode(cat, tmp.node);
								RGX_InsertToNode(branch, cat);
								cat = NULL;
						}
						p++;
				}else
				{
						if(cat == NULL)cat =  RGX_CreateNode(RGX_CAT_T);
						RGX_InsertToNode(cat, tmp.node);
				}
		}
		
		if(*p != tc){ g_res.err.pos = p; goto INVALID_POINT;}
		if(cat == NULL && branch == NULL) { g_res.err.pos = p; goto INVALID_POINT;}
		
		
		if(cat != NULL && branch != NULL)
		{
				RGX_InsertToNode(branch, cat);
				cat = NULL;
		}
		
		g_res.next = (*p == L'\0' ? p : ++p);
		g_res.node = (branch != NULL ? branch : cat);
		
		return g_res;

INVALID_POINT:
		if(cat != NULL)RGX_DestroyNode(cat);
		if(branch != NULL) RGX_DestroyNode(branch);
		if(g_res.node != NULL) RGX_DestroyNode(g_res.node);
		
		g_res.node = NULL; g_res.next = NULL;
		return g_res;
}



rgxResult_t	RGX_ParseExpr(const wchar_t *expr, const rgxNameSet_t *name_set)
{
		AR_ASSERT(expr != NULL && name_set != NULL);
		return __handle_expr(expr, L'\0', name_set);
}






AR_NAMESPACE_END



#endif
