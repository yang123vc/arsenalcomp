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
#include "node.h"

AR_NAMESPACE_BEGIN

void LEX_InitCharSet(lexCharSet_t *cset)
{
		AR_MSET0(cset, sizeof(lexCharSet_t));
}

void LEX_UnInitCharSet(lexCharSet_t *cset)
{
		AR_ASSERT(cset != NULL);

		while(cset->range != NULL)
		{
				lexCharRange_t *tmp = cset->range;
				cset->range = cset->range->next;
				AR_DEL(tmp);
		}
}

static lexCharRange_t* __new_cset_range(wchar_t beg, wchar_t end, lexCharRange_t *next)
{
		lexCharRange_t *res;
		AR_ASSERT(beg <= end);
		res = AR_NEW0(lexCharRange_t);
		
		AR_CHECK(res != NULL,AR_FATAL, L"alloc memory failed");

		res->beg = beg; res->end = end; res->next = next;
		return res;
}

#define __NEW_RANGE		__new_cset_range


void LEX_CopyCharSet(lexCharSet_t *dest, const lexCharSet_t *sour)
{
		lexCharRange_t *curr;
		dest->is_neg = sour->is_neg;
		for(curr = sour->range; curr; curr = curr->next)
		{
				LEX_InsertRangeToCharSet(dest, curr);
		}


}


void LEX_InsertRangeToCharSet(lexCharSet_t *cset, const lexCharRange_t *new_range)
{
		lexCharRange_t		*curr, *prev;
		lexCharRange_t		range;
		AR_ASSERT(cset != NULL && new_range != NULL && new_range->beg <= new_range->end);
		
		prev = NULL; curr = cset->range; 
		range	= *new_range;

		while(curr != NULL)
		{
				if(curr->beg < range.beg)
				{
						if(curr->end < range.beg)
						{
								/*
							[ curr ]
										[ range ]
								*/


						}else if(curr->end < range.end)
						{
								/*
							[ curr ]
								[ range ]
								*/
								
								lexCharRange_t		*tmp = __NEW_RANGE(curr->beg, range.beg - 1, curr);
								
								if(prev == NULL)
								{
										cset->range = tmp;
										prev = tmp;
								}else 
								{
										prev->next = tmp; 
								}

								curr->beg = range.beg; range.beg = curr->end + 1;
						
						}else if(curr->end == range.end)
						{
								/*
							[     curr      ]
									 [ range]
							
								*/
								
								lexCharRange_t		*tmp = __NEW_RANGE(curr->beg, range.beg - 1, curr);
								if(prev == NULL)
								{
										cset->range = tmp;
										prev = tmp;
								}else 
								{
										prev->next = tmp; 
								}

								curr->beg = range.beg;
								return;
						}else if(curr->end > range.end)
						{
								
								/*
								[  curr      ]
								  [ range ]
								*/
								lexCharRange_t		*tmp1, *tmp2;
								tmp1 = __NEW_RANGE(curr->beg, range.beg - 1, NULL);
								if(prev == NULL)
								{
										cset->range = tmp1;
										prev = tmp1;
								}else 
								{
										prev->next = tmp1; 
								}

								tmp2 = __NEW_RANGE(range.beg, range.end, curr);
								tmp1->next = tmp2;
								curr->beg = range.end + 1;
								return;
						}

				}else if(curr->beg == range.beg)
				{
						if(curr->end < range.end)
						{
								/*
								[curr]
								[ range ]
								*/
								range.beg = curr->end + 1;
						
						}else if(curr->end == range.end)
						{
								/*
								[curr ]
								[range]
								*/

								return;
						}else if(curr->end > range.end)
						{
								/*
								[curr         ]
								[  range  ]

								*/
								
								lexCharRange_t		*tmp;
								tmp = __NEW_RANGE(range.beg, range.end, curr);
								if(prev == NULL)
								{
										cset->range = tmp;
										prev = tmp;
								}else 
								{
										prev->next = tmp;
								}
								curr->beg = range.end + 1;
								return;
						}

				}else /*if(curr->beg > range->beg)*/
				{
						if(curr->end < range.end)
						{
								/*
								  [curr]
								[ range  ]
								*/

								lexCharRange_t		*tmp;
								tmp = __NEW_RANGE(range.beg, curr->beg - 1, curr);
								if(prev == NULL)
								{
										cset->range = tmp;
										prev = tmp;
								}else 
								{
										prev->next = tmp;
								}

								range.beg = curr->end + 1;

						}else if(curr->end == range.end)
						{
								/*
								    [curr]
								[ range  ]
								*/

								lexCharRange_t		*tmp;
								tmp = __NEW_RANGE(range.beg, curr->beg - 1, curr);
								if(prev == NULL)
								{
										cset->range = tmp;
										prev = tmp;
								}else 
								{
										prev->next = tmp;
								}
								return;
						}else/*curr->end > range->end*/
						{
								if(curr->beg <=range.end)
								{

										/*
									     	[curr]
										[range]
										*/

										lexCharRange_t		*tmp1,*tmp2;

										tmp1 = __NEW_RANGE(range.beg, curr->beg - 1, NULL);
										if(prev == NULL)
										{
												cset->range = tmp1;
												prev = tmp1;
										}else 
										{
												prev->next = tmp1;
										}

										tmp2 = __NEW_RANGE(curr->beg, range.end, curr);
										tmp1->next = tmp2;
										curr->beg = range.end + 1;
										return;
								}else
								{
										/*
										       [curr]
									   [range]
										*/

										lexCharRange_t		*tmp;
										tmp = __NEW_RANGE(range.beg, range.end, curr);

										if(prev == NULL)
										{
												cset->range = tmp;
												prev = tmp;
										}else
										{
												prev->next = tmp;
										}
										return;
								}
						}
				}
				
				prev = curr;
				curr = curr->next;
		}

		if(cset->range == NULL)
		{
				cset->range = __NEW_RANGE(range.beg, range.end, NULL);
		}else
		{
				prev->next = __NEW_RANGE(range.beg, range.end, NULL);
		}
		
}

#undef __NEW_RANGE


void LEX_ReverseNegativeCharSet(lexCharSet_t *cset)
{
		lexCharRange_t			*curr, *prev, old;
		lexCharSet_t			new_set;
		
		AR_ASSERT(cset != NULL);
		if(!cset->is_neg)return;

		cset->is_neg = False;

		LEX_InitCharSet(&new_set);
		
		old.beg =  L'\0'; old.end = AR_CHARMAX;
		
		prev = NULL;
		for(curr = cset->range; curr != NULL; curr = curr->next)
		{
				if(curr->beg > old.beg)
				{
						old.end = curr->beg - 1;
						LEX_InsertRangeToCharSet(&new_set, &old);
				}
				
				old.beg = curr->end < AR_CHARMAX ? curr->end + 1 : curr->end;
				old.end = AR_CHARMAX;
				prev = curr;
		}

		if(old.beg < AR_CHARMAX) LEX_InsertRangeToCharSet(&new_set, &old);
		
		curr = cset->range;
		cset->range = new_set.range;
		new_set.range = curr;
		LEX_UnInitCharSet(&new_set);
}




void LEX_InitBranch(lexBranch_t *branch)
{
		AR_MSET0(branch, sizeof(lexBranch_t));
}

void LEX_UnInitBranch(lexBranch_t *branch)
{
		size_t i;
		AR_ASSERT(branch != NULL);

		for(i = 0; i < branch->count; ++i)
		{
				LEX_DestroyNode(branch->arr[i]);
		}
		
		AR_DEL(branch->arr);
		branch->arr = NULL; branch->count = 0; branch->cap = 0;
}

void LEX_CopyBranch(lexBranch_t *dest, const lexBranch_t *sour)
{
		size_t i;
		dest->count = 0;
		for(i = 0; i < sour->count; ++i)
		{
				LEX_InsertNodeToBranch(dest, LEX_CopyNode(sour->arr[i]));
		}
}

void LEX_InsertNodeToBranch(lexBranch_t *branch, lexNode_t *node)
{
		AR_ASSERT(branch != NULL && node != NULL);

		if(branch->count == branch->cap)
		{
				branch->cap = (branch->cap + 1) * 2;
				branch->arr = AR_REALLOC(lexNode_t*, branch->arr, branch->cap);
		}
		branch->arr[branch->count++] = node;
}





void LEX_InitCat(lexCat_t		*cat)
{
		AR_ASSERT(cat != NULL);
		AR_MSET0(cat, sizeof(*cat));
}

void LEX_UnInitCat(lexCat_t		*cat)
{
		AR_ASSERT(cat != NULL);
		if(cat->left != NULL)LEX_DestroyNode(cat->left);
		
		if(cat->right != NULL)LEX_DestroyNode(cat->right);
}



void LEX_CopyCat(lexCat_t *dest, const lexCat_t *sour)
{
		if(sour->left != NULL)dest->left = LEX_CopyNode(sour->left);
		if(sour->right != NULL)dest->right = LEX_CopyNode(sour->right);

}

void LEX_InsertNodeToCat(lexCat_t *cat, lexNode_t *node)
{
		if(cat->left == NULL)
		{
				AR_ASSERT(cat->right == NULL);
				cat->left = node;
		}else if(cat->right == NULL)
		{
				AR_ASSERT(cat->left != NULL);
				cat->right = node;
		}else
		{
				lexNode_t *new_cat = LEX_CreateNode(LEX_CAT);
				
				new_cat->cat.left = cat->left;
				new_cat->cat.right = cat->right;
				cat->left = new_cat; cat->right = node;
		}
}
/***********************************************************lexPositionSet_t*****************************/

void LEX_ClearPositionSet(lexPositionSet_t *self)
{
		self->count = 0;
}



void LEX_UnionPositionSet(lexPositionSet_t *self, const lexPositionSet_t *other)
{
		size_t old;
		AR_ASSERT(self != NULL && other != NULL);
		if(other->count == 0)return;
		old = self->count;
		self->count += other->count;
		
		if(self->count >= self->cap)
		{
				self->cap = (self->count + 1)*2;
				self->pos = AR_REALLOC(size_t, self->pos, self->cap);
		}
		
		AR_MCPYARR(&self->pos[old], other->pos, sizeof(size_t), other->count);
		
}


void LEX_UniquePositionSet(lexPositionSet_t *self)
{
		size_t i,j;
		if(self->count == 0)return;
		

		for(i = 0, j = 1; j < self->count; ++j)
		{
				if(self->pos[i] != self->pos[j])
				{
						++i;
						AR_MCPY(self->pos + i, self->pos + j, sizeof(size_t));
				}
		}

		self->count = i + 1;
}


void LEX_SortPositionSet(lexPositionSet_t *self)
{
		int i,j;
		AR_ASSERT(self != NULL);
		
		for(i = 1; i < (int)self->count; ++i)
		{
				for(j = i; j > 0; j--)
				{
						if(self->pos[j] < self->pos[j-1])
						{
								size_t tmp = self->pos[j];
								self->pos[j] = self->pos[j-1];
								self->pos[j-1] = tmp;
						}
				}
		}

}


bool_t LEX_ComparePositionSet(const lexPositionSet_t *l, const lexPositionSet_t *r)
{
		AR_ASSERT(l != NULL && r != NULL);
		if(l->count != r->count)return False;
		if(l->count == 0)return True;
		
		return (AR_MCMPARR(l->pos, r->pos, sizeof(size_t), l->count) == 0);
}


void LEX_InitPositionSet(lexPositionSet_t *self)
{
		AR_MSET0(self, sizeof(*self));

}


void LEX_UnInitPositionSet(lexPositionSet_t *self)
{
		if(self != NULL)
		{
				AR_DEL(self->pos);
				AR_MSET0(self, sizeof(*self));
		}
}



void LEX_InsertPosToPositionSet(lexPositionSet_t *self, size_t pos)
{
		AR_ASSERT(self != NULL);

		if(self->count == self->cap)
		{
				self->cap = (self->cap + 1)*2;
				self->pos = AR_REALLOC(size_t, self->pos, self->cap);
		}
		
		
		
		self->pos[self->count++] = pos;
}



/**************************************************************/

/*
typedef struct __lex_node_attr_tag
{
		lexPositionSet_t		first_set;
		lexPositionSet_t		last_set;
		
		size_t					leaf_id;
		bool_t					nullable;
		bool_t					is_leaf;
}lexNodeAttr_t;*/

void LEX_InitNodeAttr(lexNodeAttr_t *attr)
{
		AR_ASSERT(attr != NULL);
		AR_MSET0(attr, sizeof(*attr));
		LEX_InitPositionSet(&attr->first_set);
		LEX_InitPositionSet(&attr->last_set);
}


void LEX_UnInitNodeAttr(lexNodeAttr_t *attr)
{
		if(attr != NULL)
		{
				LEX_UnInitPositionSet(&attr->first_set);
				LEX_UnInitPositionSet(&attr->last_set);
		}
}





/**************************************************************/


lexNode_t*		LEX_CreateNode(lexNodeType_t type)
{
		lexNode_t* res;
		
		res = AR_NEW0(lexNode_t);
	//	res->ref_cnt = 1;
		res->t = type;
		LEX_InitNodeAttr(&res->attr);
		
		switch(type)
		{
		case LEX_CHARSET:
				LEX_InitCharSet(&res->charset);
				break;
		case LEX_BRANCH:
				LEX_InitBranch(&res->branch);
				break;
		case LEX_CAT:
				LEX_InitCat(&res->cat);
				break;
		case LEX_LOOP:
				res->loop = NULL;
				break;
		case LEX_EPSILON:
				break;
		case LEX_CCLASS_ID:
				break;
		case LEX_FINAL:
				break;
		default:
				AR_ASSERT(0);
				break;
		}
		return res;


}

lexNode_t*		LEX_CopyNode(const lexNode_t *node)
{
		lexNode_t *new_node;
		//AR_ASSERT(node != NULL && node->ref_cnt > 0);
		//node->ref_cnt++;
		AR_ASSERT(node != NULL);
		new_node = LEX_CreateNode(node->t);
		switch(new_node->t)
		{
		case LEX_CHARSET:
				LEX_CopyCharSet(&new_node->charset, &node->charset);
				break;
		case LEX_BRANCH:
				LEX_CopyBranch(&new_node->branch, &node->branch);
				break;
		case LEX_CAT:
				LEX_CopyCat(&new_node->cat, &node->cat);
				break;
		case LEX_LOOP:
				new_node->loop = LEX_CopyNode(node->loop);
				break;
		case LEX_EPSILON:
				break;
		case LEX_CCLASS_ID:
				AR_ASSERT(0);
				break;
		case LEX_FINAL:
				AR_ASSERT(0);
				break;
		default:
				AR_ASSERT(0);
				break;
		}
		return new_node;
}

void			LEX_DestroyNode(lexNode_t *node)
{
		if(node == NULL)return;
	//	AR_ASSERT(node->ref_cnt == 1);
		
	//	if(--node->ref_cnt <= 0)
		{
				switch(node->t)
				{
				case LEX_CHARSET:
						LEX_UnInitCharSet(&node->charset);
						break;
				case LEX_BRANCH:
						LEX_UnInitBranch(&node->branch);
						break;
				case LEX_CAT:
						LEX_UnInitCat(&node->cat);
						break;
				case LEX_LOOP:
						LEX_DestroyNode(node->loop);
						break;
				case LEX_EPSILON:
						break;
				case LEX_CCLASS_ID:
						break;
				case LEX_FINAL:
						break;
				default:
						AR_ASSERT(0);
						break;
				}
				LEX_UnInitNodeAttr(&node->attr);
				AR_DEL(node);
		}

}

void			LEX_TraverseNode(lexNode_t	*root, void (*visitor)(lexNode_t *node, void *ctx), void *ctx, bool_t is_preorder)
{
		lexNode_t *node;
		
		if(root == NULL)return;
		AR_ASSERT(visitor != NULL);
		node = root;
		
		switch(node->t)
		{
		case LEX_CHARSET:
		case LEX_CCLASS_ID:
		case LEX_EPSILON:
		case LEX_FINAL:
				visitor(node, ctx);
				break;
		case LEX_BRANCH:
		{
				size_t i;
				if(is_preorder)
				{
						visitor(node,ctx);
						for(i = 0; i < node->branch.count; ++i)
						{
								LEX_TraverseNode(node->branch.arr[i], visitor, ctx, is_preorder);
						}
				}else
				{
						for(i = 0; i < node->branch.count; ++i)
						{
								LEX_TraverseNode(node->branch.arr[i], visitor, ctx, is_preorder);
						}
						visitor(node,ctx);
				}
		}
				break;
		case LEX_CAT:
				if(is_preorder)
				{
						visitor(node,ctx);
						LEX_TraverseNode(node->cat.left, visitor, ctx, is_preorder);
						LEX_TraverseNode(node->cat.right, visitor, ctx, is_preorder);
				}else
				{
						LEX_TraverseNode(node->cat.left, visitor, ctx, is_preorder);
						LEX_TraverseNode(node->cat.right, visitor, ctx, is_preorder);
						visitor(node,ctx);
				}
				break;
		case LEX_LOOP:
				if(is_preorder)
				{
						visitor(node,ctx);
						LEX_TraverseNode(node->loop, visitor, ctx, is_preorder);
				}else
				{
						LEX_TraverseNode(node->loop, visitor, ctx, is_preorder);
						visitor(node,ctx);
				}
				break;
		default:
				AR_ASSERT(0);
				break;
		}
}

static lexNode_t __epsilon_node = {LEX_EPSILON, (bool_t)2};

#define EPSILON_NODE() LEX_CopyNode(&__epsilon_node)







#if(0)

void	lexnodeCalcCharClass(LexNodeT *node, LexCharSetT *cset)
{
		LexCharSetRangeT *curr; size_t i;
		AR_ASSERT(node != NULL && cset != NULL);


		switch(node->t)
		{
		case LexCharSet:
				for(curr = node->charset.range; curr; curr = curr->next)
				{
						charsetInsert(cset, curr);
				}
						break;
		case LexBranch:
				for(i = 0; i < node->branch.count; ++i)
				{
						lexnodeCalcCharClass(node->branch.arr[i], cset);
				}
				break;
		case LexCat:
				lexnodeCalcCharClass(node->cat.left, cset);
				
				if(node->cat.right != NULL)lexnodeCalcCharClass(node->cat.right, cset);
				
				break;
		case LexLoop:
				lexnodeCalcCharClass(node->expr, cset);
				break;
		case LexEpsilon:
				break;
		case LexCClassID:
				AR_ASSERT(0);
				break;
		case LexFinal:
				
				break;
		default:
				AR_ASSERT(0);
				break;
		}

}

#endif






/***********************************************************************************************/




/**

\a
 响铃符 = \x07
 
\f
 换页符 = \x0C
 
\n
 换行符 = \x0A
 
\r
 回车符 = \x0D
 
\t
 制表符 = \x09
 
\v
 垂直制表符 = \x0B
 
 
\0
NUL 符	= \x00


\x20
 使用两位十六进制表示形式，可与该编号的字符匹配
 
\u002B
 使用四位十六进制表示形式，可与该编号的字符匹配
 
\x{20A060}
 使用任意位十六进制表示形式，可与该编号的字符匹配

**/


const wchar_t*	__transform_char(const wchar_t *input, wchar_t *c, lexError_t *err)
{
		const wchar_t	*p; 
		AR_ASSERT(input != NULL && c != NULL && err != NULL);
		*err = LEX_NO_ERROR;
		p = input;
		switch(*p)
		{
		case L'\0':
				*err = LEX_PAT_INVALID_LEN;
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
								*err = 	LEX_PAT_INVALID_LEN;
								return NULL;	
						}else
						{
								*err = LEX_PAT_INVALID_TRANSFORMCHAR;
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
										*err = 	LEX_PAT_INVALID_LEN;
										return NULL;	
								}else
								{
										*err = LEX_PAT_INVALID_TRANSFORMCHAR;
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
										*err = 	LEX_PAT_INVALID_LEN;
										return NULL;	
								}else
								{
										*err = LEX_PAT_INVALID_TRANSFORMCHAR;
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
				*err = LEX_PAT_INVALID_TRANSFORMCHAR;
				return NULL;
		}

}




lexResult_t	__handle_quote(const wchar_t *input)
{
		const wchar_t *p; lexResult_t		g_res; 
		AR_ASSERT(input != NULL);
		p = input;
		g_res.err = LEX_NO_ERROR; g_res.next = NULL; g_res.node = LEX_CreateNode(LEX_CAT);

		while(*p != L'"')
		{
				wchar_t c;
				lexCharRange_t range;
				lexNode_t		*tmp = NULL;

				if(*p == L'\0')
				{
						g_res.err = LEX_PAT_INVALID_LEN;
						goto INVALID_POINT;
				}else if(*p == L'\\')
				{
						c = *(p + 1);

						if(c == L'\0')
						{
								g_res.err = LEX_PAT_INVALID_LEN;
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
				g_res.err = LEX_PAT_INVALID_EMPTYQUOTE;
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
				*err = LEX_PAT_INVALID_LEN;
				return NULL;
		case L'\a': 
		case L'\f': 
		case L'\n': 
		case L'\r': 
		case L'\t': 
		case L'\v': 
//				AR_ASSERT(0);
				*err = LEX_PAT_INVALID_CHARSET;
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
				*err = LEX_PAT_INVALID_OPERCHAR;
				return NULL;
		case L'\\':
				return __transform_char(input + 1, c, err);
		default:
				*c = *input;
				*err = LEX_NO_ERROR;
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
		g_res.err = LEX_NO_ERROR; g_res.next = NULL; g_res.node = NULL;
		
		LEX_InitCharSet(&cset);
		
		if(*p == L'^') {cset.is_neg = True; p++;}
		
		while(*p != L']')
		{
				lexCharRange_t	range;	lexError_t err;
				p = __get_charset(p, &range.beg, &err);
				if(err != LEX_NO_ERROR)
				{
						g_res.err = err;
						goto INVALID_POINT;
				}else
				{
						if(*p == L'\0')
						{
								g_res.err = LEX_PAT_INVALID_LEN;
								goto INVALID_POINT;
						}

						if(*p == L'-')
						{
								p = __get_charset(p + 1, &range.end, &err);
								if(err != LEX_NO_ERROR)
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
		g_res.err = LEX_NO_ERROR; g_res.next = NULL; g_res.node = NULL;

		if(*p == L'[')
		{
				return __handle_cset_range(p+1);
		}else
		{
				lexCharRange_t range;
				p = __get_charset(p, &range.beg, &g_res.err);
				if(g_res.err != LEX_NO_ERROR)
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






#define __TRIM_LSTR(str, delim) do{  while(*str != L'\0' && *str == delim)str++;  }while(0)

AR_INLINE const wchar_t* __str_to_digit(const wchar_t *str, size_t radix, size_t *res)
{
		size_t num; bool_t is_ok;
		AR_ASSERT(res != NULL && str != NULL && radix <= 16);
		is_ok = False;
		num = 0;
		while(*str != L'\0')
		{
				wchar_t c = *str;
				
				if(c >= L'0' && c <= L'9')
				{
						is_ok = True;
						num *= radix;
						num += (c - L'0');
				}else if(c >= L'a' && c <= L'f')
				{
						is_ok = True;
						num *= radix;
						num += 10;
						num += (c - L'a');
				}else if(c >= L'A' && c <= L'F')
				{
						is_ok = True;
						num *= radix;
						num += 10;
						num += (c - L'A');
				}else
				{
						break;
				}
				str++;
		}
		
		if(is_ok)
		{
				*res = num;
				return str;
		}else
		{
				*res = 0;
				return NULL;
		}
}


lexNode_t*	__handle_loopcount(lexNode_t *expr, size_t min, size_t max)
{
		bool_t is_infinite;
		lexNode_t *cat, *branch, *loop;
		size_t i;
		AR_ASSERT(expr != NULL && min <= max && max > 0);

		is_infinite = (max == AR_SIZE_MAX ? True : False);
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
		g_res.err = LEX_NO_ERROR; g_res.next = NULL; g_res.node = NULL;
		
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
				size_t min,max;
				__TRIM_LSTR(beg, L' ');
				
				if(*beg < L'0' || *beg > L'9')
				{
						g_res.err = LEX_NO_ERROR;
						g_res.next = p;
						g_res.node = expr;
						return g_res;
				}

				beg = __str_to_digit(beg, 10, &min);
				
				if(beg == NULL) { g_res.err = LEX_PAT_INVALID_LOOPCOUNT; goto INVALID_POINT;}
				
				__TRIM_LSTR(beg, L' ');
				
				if(*beg == L',')
				{
						beg++;
						__TRIM_LSTR(beg, L' ');
						
						if(*beg == L'}')
						{
								max = AR_SIZE_MAX;
						}else
						{
								beg = __str_to_digit(beg, 10, &max);
								if(beg == NULL) { g_res.err = LEX_PAT_INVALID_LOOPCOUNT; goto INVALID_POINT;}
								__TRIM_LSTR(beg, L' ');
								if(*beg != L'}'){ g_res.err = LEX_PAT_INVALID_LOOPCOUNT; goto INVALID_POINT;}
						}
				}else if(*beg == L'}')
				{
						max = min;

				}else
				{
						g_res.err = LEX_PAT_INVALID_LOOPCOUNT; goto INVALID_POINT;
				}

				if(min > max || max == 0){ g_res.err = LEX_PAT_INVALID_LOOPCOUNT; goto INVALID_POINT;}

				g_res.next = ++beg;
				g_res.node = __handle_loopcount(expr, min,max);
		}
				break;
		default:
				g_res.err = LEX_NO_ERROR;
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


lexResult_t __handle_expr(const wchar_t *input, wchar_t tc, lexNameTable_t *tbl);

lexResult_t __handle_factor(const wchar_t *input,lexNameTable_t *tbl)
{
		const wchar_t *p; 
		lexResult_t		g_res; 
		AR_ASSERT(input != NULL);
		p = input;
		g_res.err = LEX_NO_ERROR; g_res.next = NULL; g_res.node = NULL;
		
		switch(*p)
		{
		case L'"':
				g_res = __handle_quote(p + 1);
				break;
		case L'(':
				g_res = __handle_expr(p + 1, L')', tbl);
				break;
		case L'{':
		{
				const wchar_t *beg;
				size_t i, count;
				p += 1;
				__TRIM_LSTR(p, L' ');
				if((*p >= L'A' && *p <= L'Z') || (*p >= L'a' && *p <= L'z') || *p == L'_')
				{
						beg = p; count = 0;
						
						while((*p >= L'A' && *p <= L'Z') || (*p >= L'a' && *p <= L'z') || *p == L'_' || (*p >= L'0' && *p <= L'9'))
						{
								p++;
								count++;
						}

						__TRIM_LSTR(p, L' ');

						if(*p != L'}' || count >= AR_MAX_LEXNAME)
						{
								AR_ASSERT(0);
								g_res.err = LEX_PAT_INVALID_EXPRNAME;
								g_res.next = NULL;
								g_res.node = NULL;
								return g_res;
						}else
						{
								lexNode_t *new_node; wchar_t name[AR_MAX_LEXNAME];
								
								for(i = 0; i < count; ++i) name[i] = beg[i];
								name[count] = L'\0';
								new_node = LEX_LookupNameFromNameTable(tbl, name);
								
								if(new_node == NULL)
								{
/*										AR_ASSERT(0);*/
										g_res.err = LEX_DEF_NAMENOTFOUND;
										g_res.next = NULL;
										g_res.node = NULL;
										return g_res;
								}else
								{
										g_res.node = LEX_CopyNode(new_node);
										g_res.next = ++p;
										g_res.err = LEX_NO_ERROR;
								}
						}

				}else
				{
						AR_ASSERT(0);
						g_res.err = LEX_PAT_INVALID_EXPRNAME;
						g_res.next = NULL;
						g_res.node = NULL;
						return g_res;
				}
		}
				break;
		default:
				g_res = __handle_charset(p);
		}

		if(g_res.err == LEX_NO_ERROR)
		{
				return __handle_postfix(g_res.node, g_res.next);
		}else
		{
				return g_res;
		}


}


lexResult_t __handle_expr(const wchar_t *input, wchar_t tc, lexNameTable_t *tbl)
{

		lexNode_t		*branch, *cat;
		const wchar_t *p; lexResult_t g_res; 
		
		AR_ASSERT(input != NULL);

		branch = NULL; cat = NULL;
		
		p = input; g_res.err = LEX_NO_ERROR; g_res.next = NULL; g_res.node = NULL;


		while(*p != L'\0' && *p != tc)
		{
				lexResult_t tmp = __handle_factor(p, tbl);
				if(tmp.err != LEX_NO_ERROR)
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
		
		if(*p != tc){ g_res.err = LEX_PAT_INVALID_LEN; goto INVALID_POINT;}
		if(cat == NULL && branch == NULL) { g_res.err = LEX_PAT_INVALID_EMPTYEXPR; goto INVALID_POINT;}
		
		
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



lexResult_t		LEX_CompilePattern(const wchar_t *pattern, lexNameTable_t *tbl)
{
		return __handle_expr(pattern, L'\0', tbl);
}



/*****************************************************************************************************************************************/



void	LEX_InitNameTable(lexNameTable_t *tbl)
{
		AR_ASSERT(tbl != NULL);
		AR_MSET0(tbl, sizeof(*tbl));

}

void	LEX_UnInitNameTable(lexNameTable_t *tbl)
{
		size_t i;
		AR_ASSERT(tbl != NULL);
		
		for(i = 0; i < tbl->count; ++i)
		{
				LEX_DestroyNode(tbl->expr[i]);
		}
		AR_DEL(tbl->expr);
		AR_DEL(tbl->name);

}


lexNode_t* LEX_LookupNameFromNameTable(lexNameTable_t *tbl, const wchar_t *name)
{
		size_t i;
		for(i = 0; i < tbl->count; ++i)
		{
				
				if(AR_WSTRCMP(tbl->name[i], name)==0)
				{
						return tbl->expr[i];
				}
		}
				
		return NULL;
}



lexError_t LEX_InsertNameToNameTable(lexNameTable_t *tbl, const wchar_t *name, const wchar_t *pattern)
{
		
		lexResult_t		res;
		if(LEX_LookupNameFromNameTable(tbl, name) != NULL) return LEX_DEF_NAMEDUP;
		
		res = LEX_CompilePattern(pattern, tbl);

		if(res.err != LEX_NO_ERROR)return res.err;

		if(tbl->count == tbl->cap)
		{
				tbl->cap = (tbl->cap + 1 *2);
				tbl->name = AR_REALLOC(lexName_t, tbl->name, tbl->cap);
				tbl->expr = AR_REALLOC(lexNode_t*, tbl->expr, tbl->cap);
		}
		
		AR_WSTRCPY(tbl->name[tbl->count], name);
		tbl->expr[tbl->count] = res.node;
		tbl->count++;
		return LEX_NO_ERROR;
}


/*********************************************************calc attribute**************************************************************************************/
void LEX_InitCClass(lexCClass_t *cclass)
{
		AR_MSET0(cclass, sizeof(*cclass));

}

void LEX_UnInitCClass(lexCClass_t *cclass)
{
		if(cclass != NULL)
		{
				AR_DEL(cclass->ranges);
		}
}

int LEX_FindIndexFromCClass(const lexCClass_t *cclass, wchar_t c)
{
		wchar_t beg, end;
		int l,r;

		l = 0; r = (int)cclass->count;
		while(l <= r)
		{
				int m = (l + r)/2;
				beg = cclass->ranges[m].beg; end = cclass->ranges[m].end;
				if(c < beg)
				{
						r = m - 1;
				}else if(c > end)
				{
						l = m + 1;
				}else
				{
						return (size_t)m;
				}
		}
		return -1;
}


static void __calc_charset_range(lexNode_t *node, void *ctx)
{
		lexCharSet_t *cset;
		lexCharRange_t *curr;

		AR_ASSERT(node != NULL && ctx != NULL);
		
		
		if(node->t == LEX_CHARSET)
		{
				cset = (lexCharSet_t*)ctx;
				for(curr = node->charset.range; curr; curr = curr->next)
				{
						LEX_InsertRangeToCharSet(cset, curr); 
				}
		}
}

void LEX_CopyCClass(lexCClass_t *dest, const lexCClass_t *other)
{
		AR_ASSERT(dest != NULL && other != NULL && other->count > 0);
		if(dest->count > 0)
		{
				if(dest->count < other->count)
				{
						AR_DEL(dest->ranges);
						dest->ranges = AR_NEWARR0(lexCClassRange_t, other->count);
				}

		}else
		{
				dest->ranges = AR_NEWARR0(lexCClassRange_t, other->count);
		}
		dest->count = other->count;
		AR_MCPYARR(dest->ranges, other->ranges, sizeof(lexCClassRange_t), dest->count);

}


void LEX_CalcCClass(lexCClass_t *cclass, const lexNode_t *root)
{
		lexCharSet_t cset; lexCharRange_t range, *curr; size_t i,count;

		LEX_InitCharSet(&cset);

		range.beg = L'\0'; range.end = AR_CHARMAX;
		LEX_InsertRangeToCharSet(&cset, &range);

		LEX_TraverseNode((lexNode_t*)root, __calc_charset_range, (void*)&cset, True);

		AR_ASSERT(cset.range != NULL);
		
		count = 0;
		for(curr = cset.range; curr; curr = curr->next)count++;

		cclass->ranges = AR_NEWARR0(lexCClassRange_t, count);
		
		i = 0;
		for(curr = cset.range; curr; curr = curr->next)
		{
				cclass->ranges[i].beg = curr->beg;
				cclass->ranges[i].end = curr->end;
				i++;
		}
		cclass->count = count;
		LEX_UnInitCharSet(&cset);
}


/******************************LeafSet********************************/



static void LEX_InsertLeafToLeafSet(lexLeafSet_t *set, lexNode_t *node)
{
		if(set->count == set->cap)
		{
				set->cap = (set->cap + 1) * 2;
				set->leaf = AR_REALLOC(lexNode_t*, set->leaf, set->cap);
		}

		set->leaf[set->count++] = node;
}


void LEX_InitLeafSet(lexLeafSet_t *set)
{
		AR_MSET0(set, sizeof(*set));
		LEX_InitPositionSet(&set->root_first);
}


void LEX_UnInitLeafSet(lexLeafSet_t *set)
{
		size_t i;
		
		LEX_UnInitPositionSet(&set->root_first);
		
		for(i = 0; i < set->count; ++i)
		{
				LEX_UnInitPositionSet(&set->follow_set[i]);
		}

		AR_DEL(set->leaf);
		AR_DEL(set->follow_set);

		AR_MSET0(set, sizeof(*set));
}






static void __replace_cset_to_ccid(lexNode_t *node, void *ctx)
{
		lexCClass_t *cclass;
		lexCharRange_t *curr;
		lexBranch_t		branch;
		size_t i;
		AR_ASSERT(node != NULL && ctx != NULL);

		if(node->t == LEX_CHARSET)
		{
				cclass = (lexCClass_t*)ctx;

				LEX_InitBranch(&branch);
				AR_ASSERT(node->charset.range != NULL);
				
				/*
						因为cclass id是根据所有charset节点分割得到的，因此cclass的颗粒度一定小于任何一个charset节点的颗粒度
						因此，cclass->ranges[i]可以被看做为单一的字符，所以每个[curr->beg, curr->end]区间会包含几个class_id
				*/

				for(i = 0; i < cclass->count; ++i)
				{
						for(curr = node->charset.range; curr; curr = curr->next)
						{
								if(curr->beg <= cclass->ranges[i].beg && curr->end >= cclass->ranges[i].end)
								{
										lexNode_t *tmp;
										tmp = LEX_CreateNode(LEX_CCLASS_ID);
										tmp->cclass_id = i;
										LEX_InsertNodeToBranch(&branch, tmp);
										break;
								}
						}
				}


				
				AR_ASSERT(branch.count > 0);

				LEX_UnInitCharSet(&node->charset);
				node->t = LEX_BRANCH;
				node->branch = branch;
		}
}






static void __calc_attr(lexNode_t *node, void *ctx)
{
		lexLeafSet_t	*leaf_set;
		AR_ASSERT(node != NULL && ctx != NULL);
		leaf_set = (lexLeafSet_t*)ctx;

		switch(node->t)
		{
		case LEX_CHARSET:
				AR_ASSERT(0);
				break;
		case LEX_CCLASS_ID:
		case LEX_FINAL:
				node->attr.is_leaf = True;
				node->attr.nullable = False;
				node->attr.leaf_id = leaf_set->count;
				LEX_InsertLeafToLeafSet(leaf_set, node);
				
				LEX_InsertPosToPositionSet(&node->attr.first_set, node->attr.leaf_id);
				LEX_InsertPosToPositionSet(&node->attr.last_set, node->attr.leaf_id);
				break;
		case LEX_EPSILON:
				node->attr.is_leaf = True;
				node->attr.nullable = True;
				break;
		case LEX_BRANCH:
		{
				size_t i;
				node->attr.is_leaf = False;
				node->attr.nullable = False;
				
				for(i = 0; i < node->branch.count; ++i)
				{
						lexNode_t *tmp = node->branch.arr[i];
						if(tmp->attr.nullable)
						{
								node->attr.nullable = True;
								break;
						}
				}
				
				for(i = 0; i < node->branch.count; ++i)
				{
						lexNode_t *tmp = node->branch.arr[i];
						
						LEX_UnionPositionSet(&node->attr.first_set, &tmp->attr.first_set);
						LEX_UnionPositionSet(&node->attr.last_set, &tmp->attr.last_set);
				}
		}
				break;
		case LEX_LOOP:
		{
				node->attr.is_leaf = False;
				node->attr.nullable = True;
				LEX_UnionPositionSet(&node->attr.first_set, &node->loop->attr.first_set);
				LEX_UnionPositionSet(&node->attr.last_set, &node->loop->attr.last_set);
		}
				break;
		case LEX_CAT:
		{
				lexNodeAttr_t tmp_r;
				lexNodeAttr_t *la, *ra;
				
				LEX_InitNodeAttr(&tmp_r);
				tmp_r.nullable = True;
				
				la = &node->cat.left->attr;
				ra = (node->cat.right != NULL ?  &node->cat.right->attr : &tmp_r);

				
				node->attr.is_leaf = False;
				node->attr.nullable = (!la->nullable ? False : ra->nullable);
				

				if(!la->nullable)
				{
						LEX_UnionPositionSet(&node->attr.first_set, &la->first_set);
				}else
				{
						LEX_UnionPositionSet(&node->attr.first_set, &la->first_set);
						LEX_UnionPositionSet(&node->attr.first_set, &ra->first_set);
				}

				if(!ra->nullable)
				{
						LEX_UnionPositionSet(&node->attr.last_set, &ra->last_set);
				}else
				{
						LEX_UnionPositionSet(&node->attr.last_set, &ra->last_set);
						LEX_UnionPositionSet(&node->attr.last_set, &la->last_set);
				}
		}
				break;
		default:
				AR_ASSERT(0);
				break;
		}

		LEX_SortPositionSet(&node->attr.first_set);
		LEX_SortPositionSet(&node->attr.last_set);
		
		LEX_UniquePositionSet(&node->attr.first_set);
		LEX_UniquePositionSet(&node->attr.last_set);
}


static void __calc_follow(lexNode_t *node, void *ctx)
{
		lexLeafSet_t	*leaf_set;
		AR_ASSERT(node != NULL && ctx != NULL);
		leaf_set = (lexLeafSet_t*)ctx;
		
		if(node->t == LEX_CAT)
		{
				lexNodeAttr_t tmp_r;
				lexNodeAttr_t *la, *ra;
				size_t i;
				LEX_InitNodeAttr(&tmp_r);
				tmp_r.nullable = True;
				
				la = &node->cat.left->attr;
				ra = (node->cat.right != NULL ?  &node->cat.right->attr : &tmp_r);
				
				for(i = 0; i < la->last_set.count; ++i)
				{
						LEX_UnionPositionSet(&leaf_set->follow_set[la->last_set.pos[i]], &ra->first_set);
				}
		}else if(node->t == LEX_LOOP)
		{
				size_t i;
				for(i = 0; i < node->attr.last_set.count; ++i)
				{
						
						LEX_UnionPositionSet(&leaf_set->follow_set[node->attr.last_set.pos[i]], &node->attr.first_set);
				}

		}
		
}



void LEX_BuildLeafSet(lexNode_t *root, lexCClass_t *cclass, lexLeafSet_t *leafset)
{
		size_t i;
		AR_ASSERT(root != NULL && leafset != NULL && leafset != NULL);
		LEX_TraverseNode(root, __replace_cset_to_ccid, (void*)cclass, True);

		/*此时必须采用后序遍历才可以计算正确值*/
		LEX_TraverseNode(root, __calc_attr, (void*)leafset, False);

		LEX_UnionPositionSet(&leafset->root_first, &root->attr.first_set);

		if(leafset->count > 0)
		{
				leafset->follow_set = AR_NEWARR(lexPositionSet_t, leafset->count);

				for(i = 0; i < leafset->count; ++i)
				{
						LEX_InitPositionSet(&leafset->follow_set[i]);
				}
				
				LEX_TraverseNode(root, __calc_follow, (void*)leafset, True);

				for(i = 0; i < leafset->count; ++i)
				{
						LEX_SortPositionSet(&leafset->follow_set[i]);
						LEX_UniquePositionSet(&leafset->follow_set[i]);
				}
		}
}

void LEX_FindPositionByCClassID(lexLeafSet_t *leaf, size_t ccid, lexPositionSet_t *out)
{
		size_t i;
		AR_ASSERT(leaf != NULL &&  out != NULL);
		LEX_ClearPositionSet(out);
		for(i = 0; i < leaf->count; ++i)
		{
				if(leaf->leaf[i]->t == LEX_CCLASS_ID && leaf->leaf[i]->cclass_id == ccid)
				{
						LEX_InsertPosToPositionSet(out, i);
				}
		}

}


AR_NAMESPACE_END
