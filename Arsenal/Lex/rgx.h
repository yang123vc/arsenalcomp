﻿/*
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
#include "lex.h"

AR_NAMESPACE_BEGIN


/*NameSet*/
#define	AR_RGX_MAXNAME					256

/*
#define AR_RGX_MAX_MINLOOPCOUNT			512
*/

struct __rgx_name_tag;
typedef struct __rgx_name_tag			rgxName_t;

struct __rgx_name_set_tag;
typedef struct __rgx_name_set_tag		rgxNameSet_t;

struct __rgx_node_tag;
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
arStatus_t				RGX_InsertToNameSet(rgxNameSet_t	*set, const wchar_t	*name, rgxNode_t *node);
arStatus_t				RGX_RemoveFromNameSet(rgxNameSet_t	*set, const wchar_t	*name);
const rgxNode_t*		RGX_FindFromNameSet(const rgxNameSet_t	*set, const wchar_t *name);
void					RGX_ClearNameSet(rgxNameSet_t *set);


/*NameSet end*/


/*CharSet*/
struct __rgx_char_range_tag;
typedef struct __rgx_char_range_tag		rgxCharRange_t;

struct __rgx_charset_tag;
typedef struct __rgx_charset_tag		rgxCharSet_t;

struct __rgx_char_range_tag
{
		wchar_t									beg;
		wchar_t									end;
		rgxCharRange_t							*next;
};

struct __rgx_charset_tag
{
		ar_bool_t					is_neg;
		rgxCharRange_t				*range;
};


void			RGX_InitCharSet(rgxCharSet_t *cset);
void			RGX_UnInitCharSet(rgxCharSet_t *cset);
arStatus_t		RGX_InsertRangeToCharSet(rgxCharSet_t *cset, const rgxCharRange_t *new_range);
arStatus_t		RGX_ReverseNegativeCharSet(rgxCharSet_t *cset);
arStatus_t		RGX_CopyCharSet(rgxCharSet_t *dest, const rgxCharSet_t *sour);

/*CharSetEnd*/



/*rgx node*/

typedef enum
{
		RGX_BEGIN_T,
		RGX_END_T,
		RGX_LINE_BEGIN_T,
		RGX_LINE_END_T,
		RGX_ANY_CHAR_T,

		RGX_CSET_T,
		RGX_POSIXCSET_T,

		RGX_CAT_T,
		RGX_BRANCH_T,

		RGX_STAR_T,
		RGX_QUEST_T,
		RGX_PLUS_T,
		RGX_FIXCOUNT_T,
		RGX_LOOKAHEAD_T,
		RGX_FINAL_T
}rgxNodeType_t;


typedef enum
{
		RGX_PCSET_ALPHA_T,
		RGX_PCSET_BLANK_T,
		RGX_PCSET_ALNUM_T,
		RGX_PCSET_CNTRL_T,
		RGX_PCSET_DIGIT_T,
		RGX_PCSET_GRAPH_T,
		RGX_PCSET_LOWER_T,
		RGX_PCSET_UPPER_T,

		RGX_PCSET_PUNCT_T,
		RGX_PCSET_PRINT_T,
		RGX_PCSET_SPACE_T,
		RGX_PCSET_XDIGIT_T,
		RGX_PCSET_MAX_T,
}rgxPosixCSetType_t;


struct __rgx_node_tag
{
		rgxNodeType_t		type;
		rgxNode_t			*left;
		rgxNode_t			*right;
		
		size_t				ref_count;

		union{
				struct {
						wchar_t	beg;
						wchar_t end;
				}range;

				struct {
						ar_bool_t				is_neg;
						rgxPosixCSetType_t		set_type;
				}posix_range;
				
				size_t					fix_count;
				ar_bool_t				negative_lookahead;
				ar_bool_t				non_greedy;
				size_t					final_val;
		};
};



rgxNode_t*		RGX_CreateNode(rgxNodeType_t type);

rgxNode_t*		RGX_CopyNewNode(const rgxNode_t *node);

void			RGX_DestroyNode(rgxNode_t *node);

arStatus_t		RGX_InsertToNode(rgxNode_t *root, rgxNode_t *node);


arStatus_t		RGX_ToString(const rgxNode_t *node, arString_t *str);



/*void			RGX_CorrectTree(rgxNode_t *root);*/



/*rgx node end*/



/*parser*/
typedef struct __rgx_result_tag	rgxResult_t;
typedef struct __rgx_error_tag	rgxError_t;

struct __rgx_error_tag
{
		arStatus_t		status;
		const wchar_t	*pos;
};


struct __rgx_result_tag
{
		rgxNode_t*		node;
		const wchar_t	*next;
		rgxError_t		err;
};


rgxResult_t	RGX_ParseExpr(const wchar_t *expr, const rgxNameSet_t *name_set);

/*parser end*/




/*rgx instruction*/


typedef enum
{
		RGX_NOP_I,
		RGX_CHAR_I,
		RGX_POSIXCSET_I,
		RGX_BEGIN_I,
		RGX_END_I,
		RGX_LINE_BEGIN_I,
		RGX_LINE_END_I,
		RGX_ANY_CHAR_I,
		RGX_LOOP_BEG_I,
		RGX_LOOP_END_I,
		RGX_LOOKAHEAD_BEG_I,
		RGX_LOOKAHEAD_END_I,
		RGX_JMP_I,
		RGX_BRANCH_I,
		RGX_MATCH_I
}rgxInsType_t;


extern const wchar_t *RGX_INS_NAME[];


typedef struct __regex_instruction_tag	rgxIns_t;

struct __regex_instruction_tag
{
		rgxInsType_t	opcode;
		
		rgxIns_t		*left;
		rgxIns_t		*right;

		/*
		ar_int_t			mark;
		*/

		union{
				ar_int_t			final;

				size_t	fix_count;

				struct {
						wchar_t	beg;
						wchar_t end;
				}range;

				struct {
						ar_bool_t				is_neg;
						rgxPosixCSetType_t		posix_cset_type;
				}posix_cset;
				
				struct {
						ar_bool_t			negative;
				}lookahead;
				
				
		};
};






typedef enum 
{
		RGX_ACT_NOACTION		=		0x0000,
		RGX_ACT_INCLINE			=		0x0001
}rgxThreadAction_t;


typedef struct __thread_tag
{
		rgxIns_t				*pc;
		const wchar_t			*sp;
		size_t					line;
		size_t					col;
		ar_uint_32_t            act;
}rgxThread_t;

/*
void		RGX_BuildThread(rgxThread_t *pthd, rgxIns_t *pc, const wchar_t *sp, size_t x, size_t y, ar_uint_32_t act);
*/


#define RGX_BuildThread(_pthd, _pc, _sp, _x, _y, _act)                  \
        do{                                                             \
                AR_ASSERT((_pthd) != NULL);                             \
                AR_ASSERT((_pc) != NULL && (_sp) != NULL);              \
                (_pthd)->pc = (_pc);                                    \
                (_pthd)->sp = (_sp);                                    \
                (_pthd)->line = (_x);                                   \
                (_pthd)->col = (_y);                                    \
                (_pthd)->act = (_act);                                  \
        }while(0)


#define AR_RGX_MAX_THREAD_CNT	512

typedef struct __thd_list_tag
{
		rgxThread_t		*lst;
		size_t			count;
		struct __thd_list_tag	*next;
}rgxThreadList_t;


rgxThreadList_t*	RGX_CreateThreadList();
void				RGX_DestroyThreadList(rgxThreadList_t *lst);

/*
void				RGX_InsertToThreadList(rgxThreadList_t *lst, rgxThread_t *thd);
void				RGX_SwapThreadList(rgxThreadList_t *l, rgxThreadList_t *r);
void				RGX_ClearThreadList(rgxThreadList_t *l);

*/



#define RGX_InsertToThreadList(_lst, _pthd)                                             \
        do{                                                                             \
                AR_ASSERT((_lst) != NULL);                                              \
                AR_ASSERT((_pthd) != NULL);                                             \
                AR_ASSERT((_pthd)->pc != NULL && (_pthd)->sp != NULL);                  \
                AR_ASSERT((_lst)->count < AR_RGX_MAX_THREAD_CNT);                       \
                (_lst)->lst[(_lst)->count++] = *(_pthd);                                \
        }while(0)




#define RGX_SwapThreadList(_l, _r)                              \
        do{                                                     \
                rgxThreadList_t tmp;                            \
                AR_ASSERT((_l) != NULL && (_r) != NULL);        \
                tmp = *(_l);                                    \
                *(_l) = *(_r);                                  \
                *(_r) = tmp;                                    \
        }while(0)



#define RGX_ClearThreadList(_l)                 \
        do{                                     \
                AR_ASSERT((_l) != NULL);        \
                (_l)->count = 0;                \
        }while(0)





struct __regex_program_tag;
typedef struct __regex_program_tag		rgxProg_t;

#define AR_RGX_MAX_INS_RECURRSIVE_LEVEL	128

struct __regex_program_tag
{

		ar_uint_t				mark;

		rgxIns_t				*start;
		ar_uint_t				*marks;
		size_t					count;

		rgxThread_t				thd_stk[AR_RGX_MAX_INS_RECURRSIVE_LEVEL];
		
		rgxIns_t				*pc;
		
		rgxThreadList_t			*curr;
		rgxThreadList_t			*next;
};


void			RGX_InitProg(rgxProg_t *prog);
void			RGX_UnInitProg(rgxProg_t *prog);
arStatus_t		RGX_Compile(rgxProg_t *prog, const rgxNode_t *tree);

arStatus_t		RGX_ProgToString(const rgxProg_t *prog, arString_t *str);



/*rgx instruction end*/




arStatus_t			RGX_Match(rgxProg_t *prog, lexMatch_t *match, lexToken_t *tok);



void	RGX_InitNode();
void	RGX_UnInitNode();
void	RGX_InitMisc();
void	RGX_UnInitMisc();


AR_NAMESPACE_END


#endif
