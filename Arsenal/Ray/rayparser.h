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




rayParser_t*	RAY_CreateParser(const rayReport_t *report);
void			RAY_DestroyParser(rayParser_t		*parser);

bool_t			RAY_ParserHasError(const rayParser_t	*parser);
void			RAY_SetParserError(rayParser_t	*parser, bool_t is_error);

void			RAY_SetOuterBlock(rayParser_t	*parser, rayBlock_t		*block);
rayBlock_t*		RAY_GetOuterBlock(const rayParser_t *parser);
bool_t			RAY_IsOuterBlock(const rayParser_t *parser, const rayBlock_t *block);

rayBlock_t*		RAY_GetCurrentBlock(const rayParser_t *parser);



typedef enum
{
		NODE_TOKEN_T,
		NODE_TYPE_FLAGS_T,
		NODE_SPECIFIER_T,
		NODE_QUALIFIER_T,
		NODE_TYPE_T,
		NODE_STORAGE_CLASS_T,
		NODE_DECLARATOR_T,
		NODE_DECLARATION_T,

}rayNodeType_t;

typedef struct __ray_parser_node_tag
{
		rayNodeType_t	node_type;
		
		union{
				psrToken_t				tok;
				
				size_t					type_flags;
				size_t					type_specifier;
				size_t					qualifier;
				rayStorageClass_t		storage_class;

				rayType_t				*type;
				rayDeclarator_t			*declarator;
				rayDeclaration_t		*declaration;

		};
}rayNode_t;

rayNode_t*		RAY_CreateParserNode(rayNodeType_t type);
void			RAY_DestroyParserNode(rayNode_t	   *node);


AR_NAMESPACE_END





#endif


