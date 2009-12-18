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


#include "rgx_node.h"


AR_NAMESPACE_BEGIN



rgxNode_t*		RGX_CreateNode(rgxNodeType_t type)
{
		rgxNode_t		*node;

		node = AR_NEW0(rgxNode_t);
		node->type = type;
		switch(type)
		{
		case RGX_BEGIN_T:
		{

				break;
		}
		case RGX_END_T:
		{
				break;
		}
		case RGX_CSET_T:
		{
				RGX_InitCharSet(&node->cset);
				break;
		}
		case RGX_CAT_T:
		{
				break;
		}
		case RGX_BRANCH_T:
		{
				break;
		}
		case RGX_STAR_T:
		{
				break;
		}
		case RGX_QUEST_T:
		{
				break;
		}
		case RGX_PLUS_T:
		{
				break;
		}
		case RGX_LOOKAHEAD_T:
		{
				break;
		}
		case RGX_CCLASS_ID_T:
		case RGX_FINAL_T:
		{
				break;
		}
		default:
		{
				AR_ASSERT(false);
				AR_abort();
				return NULL;
				break;
		}
		}

		return node;

}

rgxNode_t*		RGX_CopyNode(const rgxNode_t *node)
{
		rgxNode_t *res;

		AR_ASSERT(node != NULL);
		res = NULL;
		switch(node->type)
		{
		case RGX_CCLASS_ID_T:
		{
				res = RGX_CreateNode(node->type);
				res->cclass_id = node->cclass_id;
				break;
		}
		case RGX_FINAL_T:
		{
				res = RGX_CreateNode(node->type);
				res->final_val = node->final_val;
				break;
		}
		case RGX_BEGIN_T:
		case RGX_END_T:
		{
				res = RGX_CreateNode(node->type);
				break;
		}
		case RGX_CSET_T:
		{
				res = RGX_CreateNode(node->type);
				RGX_CopyCharSet(&res->cset, &node->cset);
				break;
		}
		case RGX_CAT_T:
		case RGX_BRANCH_T:
		{
				res = RGX_CreateNode(node->type);
				if(node->left) res->left = RGX_CopyNode(node->left);
				if(node->right) res->right = RGX_CopyNode(node->right);
				break;
		}
		case RGX_STAR_T:
		case RGX_QUEST_T:
		case RGX_PLUS_T:
		case RGX_LOOKAHEAD_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);
				res = RGX_CreateNode(node->type);
				res->left = RGX_CopyNode(node->left);

				if(node->type == RGX_STAR_T || node->type == RGX_QUEST_T || node->type == RGX_PLUS_T)
				{
						res->non_greedy = node->non_greedy;
				}
				break;
		}
		default:
		{
				AR_ASSERT(false);
				AR_abort();
				break;
		}
		}

		return res;

}

void			RGX_DestroyNode(rgxNode_t *node)
{
		AR_ASSERT(node != NULL);

		switch(node->type)
		{
		case RGX_CCLASS_ID_T:
		case RGX_FINAL_T:
		case RGX_BEGIN_T:
		case RGX_END_T:
		{
				AR_DEL(node);
				break;
		}
		
		case RGX_CSET_T:
		{
				RGX_UnInitCharSet(&node->cset);
				AR_DEL(node);
				break;
		}
		case RGX_CAT_T:
		case RGX_BRANCH_T:
		{
				if(node->left)RGX_DestroyNode(node->left);
				if(node->right)RGX_DestroyNode(node->right);
				AR_DEL(node);
				break;
		}
		case RGX_STAR_T:
		case RGX_QUEST_T:
		case RGX_PLUS_T:
		case RGX_LOOKAHEAD_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);
				RGX_DestroyNode(node->left);
				AR_DEL(node);
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


void			RGX_InsertToNode(rgxNode_t *root, rgxNode_t *node)
{
		AR_ASSERT(root != NULL && node != NULL);

		AR_ASSERT(root->type == RGX_CAT_T || root->type == RGX_BRANCH_T);

		if(root->left == NULL)
		{
				AR_ASSERT(root->right == NULL);
				root->left = node;
		}else if(root->right == NULL)
		{
				root->right = node;
		}else
		{
				rgxNode_t *tmp = RGX_CreateNode(root->type);
				tmp->left = root->left;
				tmp->right = root->right;
				root->left = tmp;
				root->right = node;
		}

}






void			RGX_ToString(const rgxNode_t *node, arString_t *str)
{
		

		AR_ASSERT(node != NULL && str != NULL);
		
		switch(node->type)
		{
		case RGX_CSET_T:
		{
				rgxCharRange_t *range;
				bool_t			need_set = false;
				AR_ASSERT(!node->cset.is_neg && node->cset.range != NULL);

				need_set = (node->cset.range->beg < node->cset.range->end || node->cset.range->next != NULL);
				range = node->cset.range;

				if(need_set)AR_AppendString(str, L"[");

				while(range)
				{
						if(range->beg == range->end)
						{
								if(AR_iswgraph(range->beg) && range->beg < (wchar_t)128)
								{
										AR_AppendFormatString(str, L"%c", range->beg);
								}else
								{
										AR_AppendFormatString(str, L"\\u%d", range->beg);
								}
						}else
						{
								if(AR_iswgraph(range->beg) && range->beg < (wchar_t)128)
								{
										AR_AppendFormatString(str, L"%c", range->beg);
								}else
								{
										AR_AppendFormatString(str, L"\\u%d", range->beg);
								}

								AR_AppendString(str, L"-");

								if(AR_iswgraph(range->end) && range->end < (wchar_t)128)
								{
										AR_AppendFormatString(str, L"%c", range->end);
								}else
								{
										AR_AppendFormatString(str, L"\\u%d", range->end);
								}
						}
						range = range->next;
				}
				
				if(need_set)AR_AppendString(str, L"]");
		}
				break;
		case RGX_CAT_T:
		{
				/*AR_AppendString(str, L"(");*/

				if(node->left)RGX_ToString(node->left,str);
				if(node->right)RGX_ToString(node->right,str);
				/*AR_AppendString(str, L")");*/
		}
				break;

		case RGX_BRANCH_T:
		{
				
				/*AR_AppendString(str, L"(");*/
				if(node->left)RGX_ToString(node->left,str);

				if(node->right)
				{
						AR_AppendString(str, L"|");
						RGX_ToString(node->right,str);
				}

				/*AR_AppendString(str, L")");*/
		}
				break;
		case RGX_STAR_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);

				AR_AppendString(str, L"(");
				RGX_ToString(node->left, str);
				AR_AppendString(str, L")");
				AR_AppendString(str, L"*");

				if(node->non_greedy)
				{
						AR_AppendString(str, L"?");
				}
				
				break;
		}
		case RGX_QUEST_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);

				AR_AppendString(str, L"(");
				RGX_ToString(node->left, str);
				AR_AppendString(str, L")");
				AR_AppendString(str, L"?");

				if(node->non_greedy)
				{
						AR_AppendString(str, L"?");
				}

				break;
		}
		case RGX_PLUS_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);

				AR_AppendString(str, L"(");
				RGX_ToString(node->left, str);
				AR_AppendString(str, L")");
				AR_AppendString(str, L"+");
				
				if(node->non_greedy)
				{
						AR_AppendString(str, L"?");
				}
				
				break;
		}
				break;
		case RGX_LOOKAHEAD_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);
				AR_AppendString(str, L"(?=");
				RGX_ToString(node->left, str);
				AR_AppendString(str, L")");
		}
				break;
		case RGX_BEGIN_T:
		{
				AR_AppendString(str, L"^");
		}
				break;
		case RGX_END_T:
		{
				AR_AppendString(str, L"$");
		}
				break;
		case RGX_CCLASS_ID_T:
		{
				AR_AppendFormatString(str, L"[id%" AR_PLAT_INT_FMT L"d]", node->cclass_id);
		}
				break;
		case RGX_FINAL_T:
		{
				AR_AppendFormatString(str, L"[final : %" AR_PLAT_INT_FMT L"d]", node->final_val);
		}
				break;
		default:
				AR_ASSERT(false);
				AR_abort();
		}
}









static void __calc_cclass(const rgxNode_t *node, rgxCharSet_t *cset)
{
		AR_ASSERT(node != NULL && cset != NULL);
		
		switch(node->type)
		{
		case RGX_CCLASS_ID_T:
		case RGX_FINAL_T:
		case RGX_BEGIN_T:
		case RGX_END_T:
		{
				break;
		}
		case RGX_CSET_T:
		{
				rgxCharRange_t *range;
				AR_ASSERT(!node->cset.is_neg);
				range = node->cset.range;
				while(range)
				{
						RGX_InsertRangeToCharSet(cset, range);
						range = range->next;
				}

				break;
		}
		case RGX_CAT_T:
		case RGX_BRANCH_T:
		{
				if(node->left)__calc_cclass(node->left, cset);
				if(node->right)__calc_cclass(node->right, cset);
				break;
		}
		case RGX_STAR_T:
		case RGX_QUEST_T:
		case RGX_PLUS_T:
		case RGX_LOOKAHEAD_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);
				__calc_cclass(node->left, cset);
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


void			RGX_CalcCharClass(const rgxNode_t *node, rgxCClass_t *cclass)
{
		rgxCharSet_t cset;
		rgxCharRange_t	*prange, range;
		AR_ASSERT(node != NULL && cclass != NULL);
		AR_ASSERT(cclass->count == 0);

		RGX_InitCharSet(&cset);
		range.beg = L'\0'; range.end = AR_WCHARMAX;
		RGX_InsertRangeToCharSet(&cset, &range);

		__calc_cclass(node, &cset);

		for(prange = cset.range; prange != NULL; prange = prange->next)
		{
				rgxCClassRange_t tmp;
				tmp.beg = prange->beg;
				tmp.end = prange->end;
				RGX_InsertToCClass(cclass, &tmp);
		}
		
		RGX_UnInitCharSet(&cset);
}

static void __split_charset(rgxNode_t *node, const rgxCClass_t *cclass)
{
		AR_ASSERT(node != NULL && cclass != NULL);
		
		switch(node->type)
		{
		case RGX_CCLASS_ID_T:
		case RGX_FINAL_T:
		case RGX_BEGIN_T:
		case RGX_END_T:
		{
				break;
		}
		case RGX_CSET_T:
		{
				size_t id;
				rgxCharRange_t *range;
				rgxNode_t		*branch;
				branch = RGX_CreateNode(RGX_BRANCH_T);
				
				
				/*
						因为cclass id是根据所有charset节点分割得到的，因此cclass的颗粒度一定小于任何一个charset节点的颗粒度
						因此，cclass->ranges[i]可以被看做为单一的字符，所以每个[curr->beg, curr->end]区间会包含几个class_id
				*/

				for(id = 0; id < cclass->count; ++id)
				{
						range = node->cset.range;
						while(range)
						{
								if(range->beg <= cclass->cclass[id].beg && range->end >= cclass->cclass[id].end)
								{
										rgxNode_t *tmp = RGX_CreateNode(RGX_CCLASS_ID_T);
										tmp->cclass_id = id;
										RGX_InsertToNode(branch, tmp);
										break;
								}else
								{
										range = range->next;
								}
						}
				}


				RGX_UnInitCharSet(&node->cset);
				node->type = RGX_CAT_T;
				node->left = branch;
				node->right = NULL;
				break;
		}
		case RGX_CAT_T:
		case RGX_BRANCH_T:
		{
				if(node->left)__split_charset(node->left,cclass);
				if(node->right)__split_charset(node->right,cclass);
				break;
		}
		case RGX_STAR_T:
		case RGX_QUEST_T:
		case RGX_PLUS_T:
		case RGX_LOOKAHEAD_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);
				__split_charset(node->left,cclass);
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



void			RGX_SplitCharSetToCClassID(rgxNode_t *node, const rgxCClass_t *cclass)
{
		AR_ASSERT(node != NULL && cclass != NULL);
		__split_charset(node, cclass);
}



static void __correct_tree(rgxNode_t *node)
{
		AR_ASSERT(node != NULL);

		switch(node->type)
		{
		case RGX_CCLASS_ID_T:
		case RGX_FINAL_T:
		case RGX_BEGIN_T:
		case RGX_END_T:
		{
				break;
		}
		
		case RGX_CAT_T:
		case RGX_BRANCH_T:
		{
				if(node->left)__correct_tree(node->left);
				if(node->right)__correct_tree(node->right);
				
				if(node->right == NULL)
				{
						rgxNode_t *tmp = node->left;
						*node = *(node->left);
						AR_DEL(tmp);
				}
		
#if defined(AR_DEBUG)
				if((node->type == RGX_CAT_T || node->type == RGX_BRANCH_T) && (node->left == NULL || node->right == NULL))
				{
						AR_ASSERT(false);
						AR_abort();
				}
#endif


				break;
		}
		case RGX_STAR_T:
		case RGX_QUEST_T:
		case RGX_PLUS_T:
		case RGX_LOOKAHEAD_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);
				__correct_tree(node->left);
				break;
		}
		case RGX_CSET_T:
		default:
		{
				AR_ASSERT(false);
				AR_abort();
				break;
		}
		}
}


void RGX_CorrectTree(rgxNode_t *root)
{
		__correct_tree(root);
}


AR_NAMESPACE_END





