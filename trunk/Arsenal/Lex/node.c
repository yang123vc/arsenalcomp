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




bool LEX_InsertToNodeList_Unique(lexNodeList_t **dest, lexNode_t *node)
{
		lexNodeList_t *prev, *curr;
		AR_ASSERT(dest != NULL && node != NULL);
		
		AR_ASSERT(node->attr.is_leaf);
		curr = *dest;
		prev = NULL;
		
		while(curr != NULL)
		{
				if(curr->node == node)
				{
						return false;
				}else if(curr->node < node)
				{
						prev = curr;
						curr = curr->next;
				}else /* curr > pos*/
				{
						break;
				}
		}

		if(prev == NULL)
		{
				AR_ASSERT(curr == *dest);
				*dest = AR_NEW0(lexNodeList_t);
				(*dest)->node = node;
				(*(*dest)).next = curr;
		}else
		{
				prev->next = AR_NEW0(lexNodeList_t);
				prev->next->node = node;
				prev->next->next = curr;
		}
		return true;
}



void LEX_DestroyNodeList(lexNodeList_t *lst)
{
		lexNodeList_t *curr, *tmp;

		curr = lst;
		while(curr != NULL)
		{
				tmp = curr->next;
				AR_DEL(curr);
				curr = tmp;
		}
}

void LEX_MergeNodeList(lexNodeList_t **dest, const lexNodeList_t *sour)
{
		const lexNodeList_t *curr;
		
		AR_ASSERT(dest != NULL);
		curr = sour;
		while(curr != NULL)
		{
				LEX_InsertToNodeList_Unique(dest, curr->node);
				curr = curr->next;
		}
}


int	 LEX_CompareNodeList(const lexNodeList_t *left, const lexNodeList_t *right)
{
		const lexNodeList_t *l,*r;

		l = left; r = right;

		while(l != NULL && r != NULL)
		{
				if(l->node < r->node)
				{
						return -1;
				}else if(l->node == r->node)
				{
						l = l->next; r = r->next;
				}else
				{
						return 1;
				}
		}
		
		if(l != NULL)return 1;
		if(r != NULL)return -1;
		return 0;
}


/************************************************************************************************************************/



void LEX_InitCharSet(lexCharSet_t *cset)
{
		AR_memset(cset, 0, sizeof(lexCharSet_t));
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
		
		AR_CHECK(res != NULL,AR_LEX, L"alloc memory failed");

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

		cset->is_neg = false;

		LEX_InitCharSet(&new_set);
		
		old.beg =  L'\0'; old.end = AR_WCHARMAX;
		
		prev = NULL;
		for(curr = cset->range; curr != NULL; curr = curr->next)
		{
				if(curr->beg > old.beg)
				{
						old.end = curr->beg - 1;
						LEX_InsertRangeToCharSet(&new_set, &old);
				}
				
				old.beg = curr->end < AR_WCHARMAX ? curr->end + 1 : curr->end;
				old.end = AR_WCHARMAX;
				prev = curr;
		}

		if(old.beg < AR_WCHARMAX) LEX_InsertRangeToCharSet(&new_set, &old);
		
		curr = cset->range;
		cset->range = new_set.range;
		new_set.range = curr;
		LEX_UnInitCharSet(&new_set);
}




void LEX_InitBranch(lexBranch_t *branch)
{
		AR_memset(branch, 0, sizeof(lexBranch_t));
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
		AR_memset(cat, 0, sizeof(*cat));
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




/*******************************************************************************************************/



void LEX_InitNodeAttr(lexNodeAttr_t *attr)
{

		AR_memset(attr, 0, sizeof(*attr));

}

void LEX_UnInitNodeAttr(lexNodeAttr_t *attr)
{
		LEX_DestroyNodeList(attr->first);
		LEX_DestroyNodeList(attr->last);
		LEX_DestroyNodeList(attr->follow);
}

/**************************************************lexNode_t**************************************************************/

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




void			LEX_TraverseNode(lexNode_t	*root, void (*visitor)(lexNode_t *node, void *ctx), void *ctx, bool is_preorder)
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



static lexNode_t __epsilon_node = {LEX_EPSILON, {true, true, NULL, NULL}};

lexNode_t* EPSILON_NODE()
{
		return LEX_CopyNode(&__epsilon_node);
}







/*********************************************************calc attribute**************************************************************************************/
void LEX_InitCClass(lexCClass_t *cclass)
{
		AR_memset(cclass, 0, sizeof(*cclass));

}

void LEX_UnInitCClass(lexCClass_t *cclass)
{
		if(cclass != NULL)
		{
				AR_DEL(cclass->ranges);
		}
}

int_t LEX_FindIndexFromCClass(const lexCClass_t *cclass, wchar_t c)
{
		wchar_t beg, end;
		int_t l,r;

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
						return m;
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
		AR_memcpy(dest->ranges, other->ranges, sizeof(lexCClassRange_t) * dest->count);
}


void LEX_CalcCClass(lexCClass_t *cclass, const lexNode_t *root)
{
		lexCharSet_t cset; lexCharRange_t range, *curr; size_t i,count;

		LEX_InitCharSet(&cset);

		range.beg = L'\0'; range.end = AR_WCHARMAX;
		LEX_InsertRangeToCharSet(&cset, &range);

		LEX_TraverseNode((lexNode_t*)root, __calc_charset_range, (void*)&cset, true);

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


/******************************************LeafSet*****************************/

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


/*此函数必须在后序遍历中才可以得到正确结果*/



static void __calc_follow(lexNode_t *node)
{
		lexNodeList_t *curr;
		AR_ASSERT(node != NULL);
		
		if(node->t == LEX_CAT)
		{
				lexNodeAttr_t tmp_r;
				lexNodeAttr_t *la, *ra;
				LEX_InitNodeAttr(&tmp_r);
				tmp_r.nullable = true;
				
				la = &node->cat.left->attr;
				ra = (node->cat.right != NULL ?  &node->cat.right->attr : &tmp_r);

				for(curr = la->last; curr; curr = curr->next)
				{
						AR_ASSERT(curr->node != NULL && curr->node->attr.is_leaf);
				
						/*如果是cat节点，则需要将其右节点的first集合加入到其左节点的last集合中的每一个节点中去*/
						LEX_MergeNodeList(&curr->node->attr.follow, ra->first);
				}
		}else if(node->t == LEX_LOOP)
		{
				for(curr = node->attr.last; curr; curr = curr->next)
				{
						AR_ASSERT(curr->node != NULL && curr->node->attr.is_leaf);

						LEX_MergeNodeList(&curr->node->attr.follow, node->attr.first);
				}
		}
		
}



static void __calc_attr(lexNode_t *node, void *ctx)
{
		
		lexLeafSet_t *set;
		AR_ASSERT(node != NULL && ctx != NULL);

		set = (lexLeafSet_t*)ctx;
		
		switch(node->t)
		{
		case LEX_CHARSET:
				AR_ASSERT(0);
				break;
		case LEX_CCLASS_ID:
		case LEX_FINAL:
				/*对于非空叶节点，其first和follow都是其自身*/
				node->attr.is_leaf = true;
				node->attr.nullable = false;
				LEX_InsertToNodeList_Unique(&node->attr.first, node); 
				LEX_InsertToNodeList_Unique(&node->attr.last, node);
				break;
		case LEX_EPSILON:
				node->attr.is_leaf = true;
				node->attr.nullable = true;
				break;
		case LEX_BRANCH:
		{
				size_t i;
				node->attr.is_leaf = false;
				node->attr.nullable = false;
				
				/*有一个节点可空则branch节点就是可空节点*/
				for(i = 0; i < node->branch.count; ++i)
				{
						lexNode_t *tmp = node->branch.arr[i];
						if(tmp->attr.nullable)
						{
								node->attr.nullable = true;
								break;
						}
				}
				
				for(i = 0; i < node->branch.count; ++i)
				{
						lexNode_t *tmp = node->branch.arr[i];
						LEX_MergeNodeList(&node->attr.first, tmp->attr.first);
						LEX_MergeNodeList(&node->attr.last, tmp->attr.last);
				}
		}
				break;
		case LEX_LOOP:
		{
				node->attr.is_leaf = false;
				node->attr.nullable = true;

				LEX_MergeNodeList(&node->attr.first, node->loop->attr.first);
				LEX_MergeNodeList(&node->attr.last, node->loop->attr.last);
		}
				break;
		case LEX_CAT:
		{
				lexNodeAttr_t tmp_r;
				lexNodeAttr_t *la, *ra;
				
				LEX_InitNodeAttr(&tmp_r);
				tmp_r.nullable = true;
				
				la = &node->cat.left->attr;
				ra = (node->cat.right != NULL ?  &node->cat.right->attr : &tmp_r);

				
				node->attr.is_leaf = false;
				node->attr.nullable = (!la->nullable ? false : ra->nullable);
				

				if(!la->nullable)
				{
						LEX_MergeNodeList(&node->attr.first, la->first);
				}else
				{
						LEX_MergeNodeList(&node->attr.first, la->first);
						LEX_MergeNodeList(&node->attr.first, ra->first);
				}

				if(!ra->nullable)
				{
						LEX_MergeNodeList(&node->attr.last, ra->last);
				}else
				{
						LEX_MergeNodeList(&node->attr.last, ra->last);
						LEX_MergeNodeList(&node->attr.last, la->last);
				}
		}
				break;
		default:
				AR_ASSERT(0);
				break;
		}

		__calc_follow(node);

		if(node->attr.is_leaf)
		{
				LEX_InsertToNodeList_Unique(&set->leafset, node);
				
		}
}

void			LEX_DestroyLeafSet(lexLeafSet_t *set)
{
		LEX_DestroyNodeList(set->root_first);
		LEX_DestroyNodeList(set->leafset);
		AR_DEL(set);
}



lexLeafSet_t*	LEX_BuildLeafSet(lexNode_t *root, lexCClass_t *cclass)
{
		lexLeafSet_t	*res;
		AR_ASSERT(root != NULL && cclass != NULL);
		res = AR_NEW0(lexLeafSet_t);
		
		LEX_TraverseNode(root, __replace_cset_to_ccid, (void*)cclass, true);
		LEX_TraverseNode(root, __calc_attr, (void*)res, false);
		
		LEX_MergeNodeList(&res->root_first, root->attr.first);
		return res;
}


const lexNodeList_t*  LEX_FindNodeByCClassID(const lexLeafSet_t *set, size_t ccid)
{
		lexNodeList_t	*lst;
		
		const lexNodeList_t* curr;
		AR_ASSERT(set != NULL);
		lst = NULL;

		for(curr = set->leafset; curr; curr = curr->next)
		{
				if(curr->node->t == LEX_CCLASS_ID && curr->node->cclass_id == ccid)
				{
						if(!LEX_InsertToNodeList_Unique(&lst, curr->node))
						{
								AR_ASSERT(false);
						}
				}
		}
		return lst;
}





AR_NAMESPACE_END




