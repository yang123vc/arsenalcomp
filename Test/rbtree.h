#if defined(__LIB)


#ifndef __RBTREE__
#define __RBTREE__


#include "test.h"


AR_NAMESPACE_BEGIN


typedef enum { RB_RED,  RB_BLACK } rbColor_t;

struct __rb_node_tag;
typedef struct __rb_node_tag	rbNode_t;

struct __rb_tree_tag;
typedef struct __rb_tree_tag	rbTree_t;

struct __rb_node_tag
{
		rbColor_t		color;
		rbNode_t		*parent;
		rbNode_t		*left;
		rbNode_t		*right;
		
		int_t			data;
};

static AR_INLINE rbNode_t*		__create_node(int d, rbColor_t color)
{
		rbNode_t		*node = AR_NEW0(rbNode_t);
		node->data = d;
		node->color = color;
		return node;
}

static AR_INLINE void			__destroy_node(rbNode_t *node)
{
		AR_ASSERT(node != NULL);
		AR_DEL(node);
}


struct __rb_tree_tag
{
		rbNode_t		*root;
		rbNode_t		*left_most;
		rbNode_t		*right_most;
		size_t			count;
};







AR_NAMESPACE_END



















#endif

#endif

