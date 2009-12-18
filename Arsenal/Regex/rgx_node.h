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

#ifndef __REGEX_NODE_H__
#define __REGEX_NODE_H__

#include "rgx_in.h"



AR_NAMESPACE_BEGIN



typedef enum
{
		RGX_BEGIN_T,
		RGX_END_T,
		RGX_CSET_T,

		RGX_CAT_T,
		RGX_BRANCH_T,

		RGX_STAR_T,
		RGX_QUEST_T,
		RGX_PLUS_T,
		
		RGX_LOOKAHEAD_T,

		RGX_CCLASS_ID_T,
		RGX_FINAL_T
}rgxNodeType_t;


struct __rgx_node_tag
{
		rgxNodeType_t		type;
		rgxNode_t			*left;
		rgxNode_t			*right;
		
		union{
				rgxCharSet_t			cset;
				bool_t					non_greedy;
				size_t					cclass_id;
				size_t					final_val;
		};
};



rgxNode_t*		RGX_CreateNode(rgxNodeType_t type);

rgxNode_t*		RGX_CopyNode(const rgxNode_t *node);

void			RGX_DestroyNode(rgxNode_t *node);

void			RGX_InsertToNode(rgxNode_t *root, rgxNode_t *node);


void			RGX_CalcCharClass(const rgxNode_t *node, rgxCClass_t *cclass);
void			RGX_SplitCharSetToCClassID(rgxNode_t *node, const rgxCClass_t *cclass);

void			RGX_ToString(const rgxNode_t *node, arString_t *str);

void			RGX_CorrectTree(rgxNode_t *root);



AR_NAMESPACE_END


#endif






