/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#ifndef __LEX_NODE_H
#define __LEX_NODE_H

#include "lex.h"



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




typedef struct __lex_node_tag		lexNode_t;



/* range == [beg,end];*/

typedef struct __lex_char_range_tag
{
		wchar_t									beg;
		wchar_t									end;
		struct __lex_char_range_tag				*next;

}lexCharRange_t;


typedef struct __lex_charset_tag
{
		bool_t					is_neg;
		lexCharRange_t			*range;
}lexCharSet_t;


void LEX_InitCharSet(lexCharSet_t *cset);
void LEX_UnInitCharSet(lexCharSet_t *cset);
void LEX_InsertRangeToCharSet(lexCharSet_t *cset, const lexCharRange_t *new_range);
void LEX_ReverseNegativeCharSet(lexCharSet_t *cset);
void LEX_CopyCharSet(lexCharSet_t *dest, const lexCharSet_t *sour);


/**************************************************************/


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

/******************************************************lexPositionSet_t*************************************************************/

typedef struct __lex_position_set_tag
{
		size_t	*pos;
		size_t	count;
		size_t	cap;
}lexPositionSet_t;


void LEX_InsertPosToPositionSet(lexPositionSet_t *self, size_t pos);
void LEX_UnInitPositionSet(lexPositionSet_t *self);
void LEX_InitPositionSet(lexPositionSet_t *self);
bool_t LEX_ComparePositionSet(const lexPositionSet_t *l, const lexPositionSet_t *r);
void LEX_SortPositionSet(lexPositionSet_t *self);
void LEX_UniquePositionSet(lexPositionSet_t *self);
void LEX_UnionPositionSet(lexPositionSet_t *self, const lexPositionSet_t *other);
void LEX_ClearPositionSet(lexPositionSet_t *self);


/*******************************************************************************************************************/

typedef struct __lex_node_attr_tag
{
		lexPositionSet_t		first_set;
		lexPositionSet_t		last_set;
		
		size_t					leaf_id;
		bool_t					nullable;
		bool_t					is_leaf;
}lexNodeAttr_t;

void LEX_InitNodeAttr(lexNodeAttr_t *attr);
void LEX_UnInitNodeAttr(lexNodeAttr_t *attr);
void LEX_CopyNodeAttr(lexNodeAttr_t *attr);


typedef struct __lex_node_tag
{
		lexNodeType_t			t;
		//int						ref_cnt;
		union{
				lexCharSet_t	charset;
				lexBranch_t		branch;
				lexCat_t		cat;
				lexNode_t		*loop;
				size_t			cclass_id;
				lexAction_t		action;
		};
		lexNodeAttr_t			attr;
}LexNodeT;


lexNode_t*		LEX_CreateNode(lexNodeType_t type);

lexNode_t*		LEX_CopyNode(const lexNode_t *node);

void			LEX_DestroyNode(lexNode_t *node);

void			LEX_TraverseNode(lexNode_t	*root, void (*visitor)(lexNode_t *node, void *ctx), void *ctx, bool_t is_preorder);


/*****************************************************************************************************************************************/

typedef struct __lex_name_table_tag		lexNameTable_t;

typedef struct __lex_patcomp_result_tag
{
		lexNode_t*		node;
		const wchar_t	*next;
		lexError_t		err;
}lexResult_t;




lexResult_t		LEX_CompilePattern(const wchar_t *pattern, lexNameTable_t *tbl);


/*****************************************************************************************************************************************/


typedef struct __lex_name_table_tag
{
		lexName_t		*name;
		lexNode_t		**expr;
		size_t			count;
		size_t			cap;
}lexNameTable_t;


void	LEX_InitNameTable(lexNameTable_t *tbl);
void	LEX_UnInitNameTable(lexNameTable_t *tbl);
lexNode_t* LEX_LookupNameFromNameTable(lexNameTable_t *tbl, const wchar_t *name);
lexError_t LEX_InsertNameToNameTable(lexNameTable_t *tbl, const wchar_t *name, const wchar_t *pattern);



/*************************************************attribute***********************************************************************************/


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
int LEX_FindIndexFromCClass(const lexCClass_t *cclass, wchar_t c);
void LEX_CalcCClass(lexCClass_t *cclass, const lexNode_t *root);
void LEX_CopyCClass(lexCClass_t *dest, const lexCClass_t *other);






/***********************************LeafSet****************************************/

typedef struct __lex_leafset_tag
{

		lexNode_t				**leaf;
		
		size_t					count;
		size_t					cap;
		
		lexPositionSet_t		*follow_set;

		lexPositionSet_t		root_first;

}lexLeafSet_t;




void LEX_InitLeafSet(lexLeafSet_t *set);
void LEX_UnInitLeafSet(lexLeafSet_t *set);
void LEX_BuildLeafSet(lexNode_t *root, lexCClass_t *cclass, lexLeafSet_t *leafset);
void LEX_FindPositionByCClassID(lexLeafSet_t *leaf, size_t ccid, lexPositionSet_t *out);




#endif


