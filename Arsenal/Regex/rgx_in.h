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

#ifndef __REGEX_REGEX_IN_H__
#define __REGEX_REGEX_IN_H__


#include "../Common/common.h"

AR_NAMESPACE_BEGIN


/*NameSet*/
#define	AR_RGX_MAXNAME			256


typedef struct __rgx_name_tag			rgxName_t;
typedef struct __rgx_name_set_tag		rgxNameSet_t;

typedef struct __rgx_node_tag			rgxNode_t;

struct __rgx_name_tag
{
		wchar_t			name[AR_RGX_MAXNAME];
		rgxNode_t		*node;
};

struct __rgx_name_set_tag
{
		rgxName_t		*name;
		size_t			count;
		size_t			cap;
};


void					RGX_InitNameSet(rgxNameSet_t	*set);
void					RGX_UnInitNameSet(rgxNameSet_t	*set);
bool_t					RGX_InsertToNameSet(rgxNameSet_t	*set, const wchar_t	*name, rgxNode_t *node);
bool_t					RGX_RemoveFromNameSet(rgxNameSet_t	*set, const wchar_t	*name);
const rgxNode_t*		RGX_FindFromNameSet(const rgxNameSet_t	*set, const wchar_t *name);



/*NameSet end*/



/*char class id*/

/*******************char class id **********************/

typedef struct __rgx_cclass_range_tag	rgxCClassRange_t;
typedef struct __rgx_charclass_tag		rgxCClass_t;

struct __rgx_cclass_range_tag
{
		wchar_t			beg;
		wchar_t			end;
};

struct __rgx_charclass_tag
{
		rgxCClassRange_t		*cclass;
		size_t					count;
		size_t					cap;
};

void	RGX_InitCClass(rgxCClass_t *cclass);
void	RGX_UnInitCClass(rgxCClass_t *cclass);
int_t	RGX_FindIndexFromCClass(const rgxCClass_t *cclass, wchar_t c);
void	RGX_CopyCClass(rgxCClass_t *dest, const rgxCClass_t *other);
void	RGX_InsertToCClass(rgxCClass_t *dest, const rgxCClassRange_t *range);

/*char class id end*/





/*CharSet*/
typedef struct __rgx_char_range_tag		rgxCharRange_t;
typedef struct __rgx_charset_tag		rgxCharSet_t;

struct __rgx_char_range_tag
{
		wchar_t									beg;
		wchar_t									end;
		rgxCharRange_t							*next;
};



struct __rgx_charset_tag
{
		bool_t					is_neg;
		rgxCharRange_t			*range;
};


void RGX_InitCharSet(rgxCharSet_t *cset);
void RGX_UnInitCharSet(rgxCharSet_t *cset);
void RGX_InsertRangeToCharSet(rgxCharSet_t *cset, const rgxCharRange_t *new_range);
void RGX_ReverseNegativeCharSet(rgxCharSet_t *cset);
void RGX_CopyCharSet(rgxCharSet_t *dest, const rgxCharSet_t *sour);
/*CharSetEnd*/






/*rgx_action*/
typedef struct __regex_action_tag
{
		size_t			type;
		size_t			priority;
		bool_t			is_skip;
}rgxAction_t;

/*rgx_action end*/


























AR_NAMESPACE_END


#endif
