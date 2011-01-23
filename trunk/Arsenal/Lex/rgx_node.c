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

		if(__g_node_list == NULL)
		{
				node = AR_NEW0(rgxNode_t);
				
		}else
		{
				AR_LockSpinLock(&__g_lock);
				
				if(__g_node_list == NULL)
				{
						node = AR_NEW0(rgxNode_t);
						
				}else
				{
						node = __g_node_list;
						__g_node_list = __g_node_list->left;
						AR_memset(node, 0, sizeof(*node));
				}
				AR_UnLockSpinLock(&__g_lock);
		}
		
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
		case RGX_LINE_BEGIN_T:
		{
				break;
		}
		case RGX_LINE_END_T:
		{
				break;
		}
		case RGX_ANY_CHAR_T:
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
		case RGX_FIXCOUNT_T:
		{
				break;
		}
		default:
		{
				AR_CHECK(false, L"Arsenal : regex parser error %hs\r\n", AR_FUNC_NAME);
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

		return res;
}



void			RGX_DestroyNode(rgxNode_t *node)
{
		AR_ASSERT(node != NULL);
		
		AR_ASSERT(node->ref_count >= 1);

		if(AR_AtomicDec((volatile int_t*)&node->ref_count) > 0)
		{
				return;
		}

		
		
		switch(node->type)
		{
		case RGX_FINAL_T:
		case RGX_BEGIN_T:
		case RGX_LINE_BEGIN_T:
		case RGX_LINE_END_T:
		case RGX_END_T:
		case RGX_ANY_CHAR_T:
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
		case RGX_FIXCOUNT_T:
		case RGX_LOOKAHEAD_T:
		{
				AR_ASSERT(node->left != NULL && node->right == NULL);
				RGX_DestroyNode(node->left);
				break;
		}
		default:
		{
				AR_CHECK(false, L"Arsenal : regex parser error %hs\r\n", AR_FUNC_NAME);
				break;
		}
		}

		__free_node(node);
}


#if(0)

typedef struct __rgx_node_list_tag
{
		rgxNode_t **lst;
		size_t	  cnt;
		size_t	cap;
}rgxNodeList_t;

static void __init_node_list(rgxNodeList_t *lst)
{
		AR_ASSERT(lst != NULL);
		AR_memset(lst, 0, sizeof(*lst));
}

static void __uninit_node_list(rgxNodeList_t *lst)
{
		AR_ASSERT(lst != NULL);
		
		if(lst && lst->lst)
		{
				AR_DEL(lst->lst);
		}
		AR_memset(lst, 0, sizeof(*lst));
}


static void __insert_to_node_list(rgxNodeList_t *lst, rgxNode_t *node)
{
		AR_ASSERT(lst != NULL && node != NULL);
		if(lst->cnt == lst->cap)
		{
				lst->cap += 32;
				lst->cap *= 2;
				lst->lst = AR_REALLOC(rgxNode_t*, lst->lst, lst->cap);
		}
		lst->lst[lst->cnt++] = node;
}

void			RGX_DestroyNode(rgxNode_t *node)
{
		
		rgxNodeList_t lst;
		size_t i;
		AR_ASSERT(node != NULL);
		AR_ASSERT(node->ref_count >= 1);

		__init_node_list(&lst);
		
		__insert_to_node_list(&lst, node);

		for(i = 0; i < lst.cnt; ++i)
		{
				node = lst.lst[i];
				AR_ASSERT(node && node->ref_count >= 1);
				
				switch(node->type)
				{
				case RGX_FINAL_T:
				case RGX_BEGIN_T:
				case RGX_LINE_BEGIN_T:
				case RGX_LINE_END_T:
				case RGX_END_T:
				case RGX_ANY_CHAR_T:
				case RGX_CSET_T:
				{
						if(AR_AtomicDec((volatile int_t*)&node->ref_count) == 0)
						{
								__free_node(node);
						}
				}
						break;
				case RGX_CAT_T:
				case RGX_BRANCH_T:
				{
						if(AR_AtomicDec((volatile int_t*)&node->ref_count) == 0)
						{
								AR_ASSERT(node->left != node && node->right != node); 
								if(node->left)
								{
										
										__insert_to_node_list(&lst, node->left);
								}

								if(node->right)
								{
										__insert_to_node_list(&lst, node->right);
								}
								__free_node(node);
						}
				}
						break;
				case RGX_STAR_T:
				case RGX_QUEST_T:
				case RGX_PLUS_T:
				case RGX_LOOKAHEAD_T:
				{
						AR_ASSERT(node->left != NULL && node->right == NULL);
						if(AR_AtomicDec((volatile int_t*)&node->ref_count) == 0)
						{
								AR_ASSERT(node->left != node && node->right != node);
								
								if(node->left)
								{
										__insert_to_node_list(&lst, node->left);
								}
								__free_node(node);
						}
				}
						break;
				default:
				{
						AR_CHECK(false, L"Arsenal : regex parser error %hs\r\n", AR_FUNC_NAME);
						break;
				}
				}
		}

		__uninit_node_list(&lst);
}
#endif

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
		case RGX_FIXCOUNT_T:
		{
				wchar_t buf[128];
				AR_ASSERT(node->left != NULL && node->right == NULL);

				AR_AppendString(str, L"(");
				RGX_ToString(node->left, str);
				AR_AppendString(str, L")");
				AR_AppendString(str, L"{");
				AR_u64tow_buf(buf, 128, (uint_64_t)node->fix_count, 10);
				AR_AppendString(str, buf);		
				AR_AppendString(str, L"}");
				break;
		}
				break;
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
		case RGX_LINE_BEGIN_T:
		{
				AR_AppendString(str, L"\\B");
		}
				break;
		
		case RGX_LINE_END_T:
		{
				AR_AppendString(str, L"\\E");
		}
				break;
		case RGX_ANY_CHAR_T:
		{
				AR_AppendString(str, L".");
				break;
		}
		case RGX_FINAL_T:
		{
				AR_AppendFormatString(str, L"[final : %" AR_PLAT_INT_FMT L"d]", node->final_val);
		}
				break;
		default:
				AR_CHECK(false, L"Arsenal : regex parser error %hs\r\n", AR_FUNC_NAME);
		}
}





AR_NAMESPACE_END








