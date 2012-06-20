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


#ifndef __LEX_H__
#define __LEX_H__

#include "Common/common.h"


AR_NAMESPACE_BEGIN

struct __regex_node_tag;
typedef struct __regex_node_tag	regex_node_t;


/************************************************Regex Node***************************************************/
typedef enum
{
		regex_begin_t,
		regex_end_t,
		regex_line_begin_t,
		regex_line_end_t,
		regex_any_char_t,
		regex_cset_t,
		regex_cat_t,
		regex_branch_t,
		regex_star_t,
		regex_quest_t,
		regex_plus_t,
		regex_fixcount_t,
		regex_lookahead_t,
		regex_final_t
}node_type_t;

struct __regex_node_tag
{
		node_type_t			type;
		regex_node_t		*left;
		regex_node_t		*right;
		
		size_t				ref_count;

		union{
				struct {
						wchar_t	beg;
						wchar_t end;
				}range;
				
				size_t					fix_count;
				bool_t					negative_lookahead;
				bool_t					non_greedy;
				size_t					final_val;
		};
};

regex_node_t*	regex_node_create(node_type_t t);
regex_node_t*	regex_node_copynew(const regex_node_t *node);
void			regex_node_destroy(regex_node_t *node);
arStatus_t		regex_node_insert(regex_node_t *node, regex_node_t *child);
arStatus_t		regex_node_tostring(const regex_node_t *node);



typedef struct __regex_error_tag
{
		regex_node_t	*node;
		const wchar_t	*errpos;
}regex_result_t;


arStatus_t	regex_parse(const wchar_t *pattern,  regex_result_t *result);

struct __arsenal_regex_tag;
typedef struct __arsenal_regex_tag		arRegex_t;

#define AR_REGEX_STACK_COUNT		1024
#define AR_REGEX_ERROR_BUFFER		1024
struct __arsenal_regex_tag
{
		regex_node_t	*node;

		wchar_t			*sp_stack[AR_REGEX_STACK_COUNT];
		size_t			count_stack[AR_REGEX_STACK_COUNT];
		size_t			stack_idx;

		const wchar_t	*sp;
		size_t			count;


		const wchar_t	*input;
		size_t			input_len;
		

		bool_t			is_ignorecase;
		bool_t			is_singleline;

		wchar_t			last_error[AR_REGEX_ERROR_BUFFER];
};

typedef struct __arsenal_regex_match_result_tag
{
		const wchar_t	*start;
		size_t			count;
}arRegexMatch_t;


arStatus_t		AR_RegexMatch(arRegex_t *regex, const wchar_t *input);

AR_NAMESPACE_END


















#endif


