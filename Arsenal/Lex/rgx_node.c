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


#include "rgx.h"

AR_NAMESPACE_BEGIN


/***************************************************global*****************************/

static rgxNode_t*		__g_node_list = NULL;
static arSpinLock_t		__g_lock;

void RGX_InitNode()
{
		AR_InitSpinLock(&__g_lock);
		__g_node_list = NULL;

}


void RGX_UnInitNode()
{
		while(__g_node_list)
		{
				rgxNode_t *node = __g_node_list;
				__g_node_list = __g_node_list->left;
				AR_DEL(node);
		}
		AR_UnInitSpinLock(&__g_lock);
}


static rgxNode_t* __alloc_node()
{
		rgxNode_t *node;
		
		AR_LockSpinLock(&__g_lock);
		if(__g_node_list == NULL)
		{
				__g_node_list = AR_NEW0(rgxNode_t);
		}

		node = __g_node_list;
		__g_node_list = __g_node_list->left;
		AR_UnLockSpinLock(&__g_lock);
		
		AR_memset(node, 0, sizeof(*node));
		return node;
}


static void		__free_node(rgxNode_t *node)
{
		AR_ASSERT(node != NULL);
		
		AR_LockSpinLock(&__g_lock);

		node->left = __g_node_list;
		__g_node_list = node;
		AR_UnLockSpinLock(&__g_lock);
}


/**************************************************************************************/

rgxNode_t*		RGX_CreateNode(rgxNodeType_t type)
{
		rgxNode_t		*node;

		/*node = AR_NEW0(rgxNode_t);*/
		
		node = __alloc_node();

		node->type = type;

		node->ref_count = 1;

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

		AR_ASSERT(node->ref_count >= 1);

		res = (rgxNode_t*)node;

		AR_AtomicInc((volatile int_t*)&node->ref_count);

		/*res->ref_count++;*/
		return res;

		/*
		res = NULL;
		switch(node->type)
		{
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
				res->range = node->range;
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
		*/
}




void			RGX_DestroyNode(rgxNode_t *node)
{
		AR_ASSERT(node != NULL);
		
		AR_ASSERT(node->ref_count >= 1);

		if(AR_AtomicDec((volatile int_t*)&node->ref_count) > 0)
		{
				return;
		}
		
		/*if(--node->ref_count > 0)return;*/

		
		
		switch(node->type)
		{
		case RGX_FINAL_T:
		case RGX_BEGIN_T:
		case RGX_END_T:
		{
				break;
		}
		
		case RGX_CSET_T:
		{
				break;
		}
		case RGX_CAT_T:
		case RGX_BRANCH_T:
		{
				if(node->left)RGX_DestroyNode(node->left);
				if(node->right)RGX_DestroyNode(node->right);
				
				break;
		}
		case RGX_STAR_T:
		case RGX_QUEST_T:
		case RGX_PLUS_T:
		case RGX_LOOKAHEAD_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);
				RGX_DestroyNode(node->left);
				break;
		}
		default:
		{
				AR_ASSERT(false);
				AR_abort();
				break;
		}
		}

		/*AR_DEL(node);*/

		__free_node(node);
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
				
				if(node->range.beg == node->range.end)
				{
						if(AR_iswgraph(node->range.beg) && node->range.beg < (wchar_t)128)
						{
								AR_AppendFormatString(str, L"%c", node->range.beg);
						}else
						{
								AR_AppendFormatString(str, L"\\u%" AR_PLAT_INT_FMT L"d", node->range.beg);
						}
				}else
				{
						AR_AppendString(str, L"[");
						if(AR_iswgraph(node->range.beg) && node->range.beg < (wchar_t)128)
						{
								AR_AppendFormatString(str, L"%c", node->range.beg);
						}else
						{
								AR_AppendFormatString(str, L"\\u%" AR_PLAT_INT_FMT L"d", node->range.beg);
						}

						AR_AppendString(str, L"-");

						if(AR_iswgraph(node->range.end) && node->range.end < (wchar_t)128)
						{
								AR_AppendFormatString(str, L"%c", node->range.end);
						}else
						{
								AR_AppendFormatString(str, L"\\u%" AR_PLAT_INT_FMT L"d", node->range.end);
						}

						AR_AppendString(str, L"]");
				}
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
				
				AR_AppendString(str, L"(");
				if(node->left)RGX_ToString(node->left,str);

				if(node->right)
				{
						AR_AppendString(str, L"|");
						RGX_ToString(node->right,str);
				}

				AR_AppendString(str, L")");
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
		case RGX_LOOKAHEAD_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);
				if(node->negative_lookahead)
				{
						AR_AppendString(str, L"(?!");
				}else
				{
						AR_AppendString(str, L"(?=");
				}
				
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





AR_NAMESPACE_END








