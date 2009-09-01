#include "tiny_parser.h"



AR_psrNode_t*	AR_STDCALL_MARK __build_leaf_func(const AR_Token_t *tok, void *ctx)
{
		NodeBuilder *builder;
		assert(tok != NULL && ctx != NULL);
		builder = (NodeBuilder*)ctx;
		return (AR_psrNode_t*)builder->BuildLeaf(tok);
}


AR_psrNode_t*	AR_STDCALL_MARK __build_node_func(AR_uint_t rule_id, const AR_wchar_t *rule_name, AR_psrNode_t **nodes, AR_uint_t count, void *ctx)
{
		NodeBuilder *builder;
		assert(rule_name != NULL && nodes != NULL && count > 0 && ctx != NULL);
		builder = (NodeBuilder*)ctx;
		
		std::wstring name = std::wstring((const wchar_t*)rule_name);
		Node **ns = (Node**)nodes;
		std::vector<Node*> node_vec;

		for(size_t i = 0; i < count; ++i)
		{
				node_vec.push_back(ns[i]);
		}
		return (AR_psrNode_t*)builder->BuildNode((size_t)rule_id, name, node_vec);
}


void			AR_STDCALL_MARK __free_node_func(AR_psrNode_t *node, void *ctx)
{
		NodeBuilder *builder;
		assert(ctx != NULL);
		builder = (NodeBuilder*)ctx;
		if(node)
		{
				builder->FreeNode((Node*)node);
		}
}


Parser::Parser(const std::wstring &pattern, NodeBuilder *builder) : m_builder(builder)
{
		assert(builder != NULL);
		AR_grammar_t	*gmr = AR_PSR_CreateGrammar();

		if(!AR_PSR_ConfigGrammar(gmr, (const AR_wchar_t*)pattern.c_str()))
		{
				throw TinyException(L"Invalid Grammar\r\n");
		}

		AR_Parser_Ctx_t	ctx = {__build_leaf_func, __build_node_func, __free_node_func, (void*)builder};

		m_parser = AR_PSR_Create(gmr, &ctx, AR_PARSER_LALR);
		assert(m_parser != NULL);
		AR_PSR_DestroyGrammar(gmr);
}

Parser::~Parser()
{
		AR_PSR_Destroy(m_parser);
		delete m_builder;
}



ParserErr Parser::AddToken(const AR_Token_t *token)
{
		assert(token != NULL);
		return (ParserErr)AR_PSR_AddToken(m_parser, token);
}



Node*	  Parser::GetTree()
{
		return (Node*)AR_PSR_GetRoot(m_parser);
}
