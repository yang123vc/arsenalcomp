#ifndef __PARSER_H__
#define __PARSER_H__

#include "..\Common\Common.h"
#include "..\Lex\lex.h"

typedef enum
{
		PSR_NO_ERROR = 0,
		PSR_INVALID_NAMELEN,
		PSR_INVALID_TOKEN,
		PSR_DUPLICATE_TOKEN_DEFINE,
		PSR_INVALID_PRIO,
		PSR_INVALID_NONTERM_ID,
		PSR_INVALID_RULE,
		PSR_DUPLICATE_NONTERM_DEFINE,
		PSR_INVALID_EPSILON_DEFINE,
		PSR_INVALID_GRAMMAR_PATTERN
}psrError_t;



#define PARSER_MAXNAME	256		/*non-term || term 最长名称*/

#define PSR_MAX_NAMELEN			(PARSER_MAXNAME-1)
#define PSR_MIN_TOKENVAL		257
#define PSR_MIN_NONTERM_ID		129
#define PSR_MIN_PRIOVAL			1
#define PSR_MAX_PRIOVAL			128
#define PSR_MIN_PRIOTERMVAL		0xFFFF
#define PSR_NONTERM_ID_INRULE	(PSR_MIN_NONTERM_ID-1)/*在产生式体中的非终结符ID*/

/******************************************************Parser********************************************************/

#define PSR_MAX_TOKENLEN		4096
#define PSR_IGNORE_TOKEN_TYPE	0

typedef void	  psrNode_t;

typedef struct __psr_token_tag
{
		size_t			lex_type;
		wchar_t			value[PSR_MAX_TOKENLEN];
		size_t			line;
		size_t			col;
}psrToken_t;

typedef struct __parser_node_set_tag
{
		psrNode_t		**nodes;
		size_t			count;
		size_t			cap;
}psrNodeSet_t;

AR_INLINE void PSR_InitNodeSet(psrNodeSet_t *nodes)
{
		AR_MSET0(nodes, sizeof(*nodes));
}

AR_INLINE void PSR_UnInitNodeSet(psrNodeSet_t *nodes)
{
		AR_DEL(nodes->nodes);
		AR_MSET0(nodes, sizeof(*nodes));
}

AR_INLINE void PSR_InsertToNodeSet(psrNodeSet_t *nodes, psrNode_t *node)
{
		if(nodes->count == nodes->cap)
		{
				nodes->cap = (nodes->cap + 1)*2;
				nodes->nodes = AR_REALLOC(psrNode_t*, nodes->nodes, nodes->cap);
		}

		nodes->nodes[nodes->count++] = node;
}

AR_INLINE void PSR_ClearNodeSet(psrNodeSet_t *set)
{
		set->count = 0;
}


typedef psrNode_t* (*psrLeaf_func)(const psrToken_t *leaf);
typedef psrNode_t* (*psrNode_func)(size_t rule_id,  const psrNodeSet_t *set);
typedef void	   (*psrDestroy_func)(psrNode_t *node);


typedef struct __parser_tag parser_t;

typedef enum
{
		PSR_SLR,
		PSR_LR1,
		PSR_LALR
}psrEngineType_t;

parser_t* PSR_CreateParser(const wchar_t *lex, const wchar_t *grammar, psrLeaf_func leaf_func, psrNode_func node_func, psrDestroy_func destroy_func, psrEngineType_t type);

void	  PSR_DestroyParser(parser_t *parser);

psrNode_t* PSR_Parse(parser_t *parser, const wchar_t *sources);






#endif
