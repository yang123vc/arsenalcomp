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

#ifndef __LEX_NODE_H__
#define __LEX_NODE_H__

#include "lex.h"



AR_NAMESPACE_BEGIN


typedef enum
{
		LEX_CHARSET,
		LEX_BRANCH,
		LEX_CAT,
		LEX_LOOP,
		LEX_EPSILON,
		LEX_CCLASS_ID,
		LEX_FINAL
}lexNodeType_t;


typedef struct __lex_node_tag	lexNode_t;



typedef struct __node_list_tag
{
		lexNode_t						*node;
		struct __node_list_tag			*next;
}lexNodeList_t;

bool LEX_InsertToNodeList_Unique(lexNodeList_t **dest, lexNode_t *pos);
void LEX_DestroyNodeList(lexNodeList_t *dest);
void LEX_MergeNodeList(lexNodeList_t **dest, const lexNodeList_t *sour);
int	 LEX_CompareNodeList(const lexNodeList_t *l, const lexNodeList_t *r);




typedef struct __lex_char_range_tag
{
		wchar_t									beg;
		wchar_t									end;
		struct __lex_char_range_tag				*next;

}lexCharRange_t;


typedef struct __lex_charset_tag
{
		bool					is_neg;
		lexCharRange_t			*range;
}lexCharSet_t;


void LEX_InitCharSet(lexCharSet_t *cset);
void LEX_UnInitCharSet(lexCharSet_t *cset);
void LEX_InsertRangeToCharSet(lexCharSet_t *cset, const lexCharRange_t *new_range);
void LEX_ReverseNegativeCharSet(lexCharSet_t *cset);
void LEX_CopyCharSet(lexCharSet_t *dest, const lexCharSet_t *sour);



typedef struct __lex_branch_tag
{
		lexNode_t		**arr;
		size_t			count;
		size_t			cap;
}lexBranch_t;

void LEX_InitBranch(lexBranch_t *branch);
void LEX_UnInitBranch(lexBranch_t *branch);
void LEX_InsertNodeToBranch(lexBranch_t *branch, lexNode_t *node);
void LEX_CopyBranch(lexBranch_t *dest, const lexBranch_t *sour);
/**************************************************************/

typedef struct __lex_cat_tag
{
		lexNode_t		*left;
		lexNode_t		*right;
}lexCat_t;

void LEX_InitCat(lexCat_t		*cat);
void LEX_UnInitCat(lexCat_t		*cat);
void LEX_InsertNodeToCat(lexCat_t *cat, lexNode_t *node);
void LEX_CopyCat(lexCat_t *dest, const lexCat_t *sour);
/**************************************************************/




typedef struct __lex_node_attr_tag
{
		bool					nullable;
		bool					is_leaf;

		lexNodeList_t			*first;
		lexNodeList_t			*last;
		lexNodeList_t			*follow;/*只在叶节点中使用*/
}lexNodeAttr_t;

void LEX_InitNodeAttr(lexNodeAttr_t *attr);
void LEX_UnInitNodeAttr(lexNodeAttr_t *attr);


typedef struct __lex_node_tag
{
		lexNodeType_t			t;
		lexNodeAttr_t			attr;
		union{
				lexCharSet_t	charset;
				lexBranch_t		branch;
				lexCat_t		cat;
				lexNode_t		*loop;
				size_t			cclass_id;
				lexAction_t		action;
		};
}LexNodeT;


lexNode_t*		LEX_CreateNode(lexNodeType_t type);

lexNode_t*		LEX_CopyNode(const lexNode_t *node);

void			LEX_DestroyNode(lexNode_t *node);

void			LEX_TraverseNode(lexNode_t	*root, void (*visitor)(lexNode_t *node, void *ctx), void *ctx, bool is_preorder);




lexNode_t* EPSILON_NODE();


/***************************************lexCClass_t*******************************************/

typedef struct __lex_cclass_range_t
{
		wchar_t	beg;
		wchar_t	end;
}lexCClassRange_t;

typedef struct __lex_charclass_tag
{
		lexCClassRange_t		*ranges;
		size_t					count;
}lexCClass_t;

void LEX_InitCClass(lexCClass_t *cclass);
void LEX_UnInitCClass(lexCClass_t *cclass);
int	 LEX_FindIndexFromCClass(const lexCClass_t *cclass, wchar_t c);
void LEX_CalcCClass(lexCClass_t *cclass, const lexNode_t *root);
void LEX_CopyCClass(lexCClass_t *dest, const lexCClass_t *other);


/**************************************Node attr***************************************************/


/*返回所有叶结点的链表*/

typedef struct __lex_leafset_tag
{
		lexNodeList_t	*root_first;
		lexNodeList_t	*leafset;
}lexLeafSet_t;



lexLeafSet_t*	LEX_BuildLeafSet(lexNode_t *root, lexCClass_t *cclass);
void			LEX_DestroyLeafSet(lexLeafSet_t *set);
const lexNodeList_t*  LEX_FindNodeByCClassID(const lexLeafSet_t *set, size_t ccid);









AR_NAMESPACE_END

























#endif


