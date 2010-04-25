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


#ifndef __RAY_PARSER_H__
#define __RAY_PARSER_H__


#include "ray.h"
#include "raysymbol.h"



AR_NAMESPACE_BEGIN










typedef enum
{
		NODE_SPECIFIER_T,
		NODE_QUALIFIER_T,
		NODE_TYPE_T,
		NODE_DECLARATOR_T

}rayNodeType_t;

typedef struct __ray_parser_node_tag
{
		rayNodeType_t	node_type;
		
		union{
				rayType_t		*type;

		};
}rayNode_t;




AR_NAMESPACE_END





#endif


