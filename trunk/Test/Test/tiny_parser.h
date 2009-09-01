#include "tiny.h"
#include <cassert>



class Node
{
private:
		size_t			m_type;
public:
		Node(size_t type) : m_type(type)
		{

		}

		virtual ~Node() = 0;
public:
		virtual size_t	GetType()const
		{
				return m_type;
		}

		virtual const	std::wstring& ToString()const = 0;
		
};

class NodeBuilder
{
public:
		virtual Node*	BuildLeaf(const AR_Token_t *tok) = 0;
		virtual Node*	BuildNode(size_t rule_id, const std::wstring &rule_name, std::vector<Node*> &nodes) = 0;
		virtual void	FreeNode(Node *node) = 0;

		virtual ~NodeBuilder() {}
};



enum ParserErr{
		INVALID_TOKEN = 0,
		INVALID_SYNTAX =1,
		ACCEPT = 2,
		OK	 = 3
};



class Parser
{
private:
		AR_parser_t		*m_parser;
		NodeBuilder		*m_builder;
public:
		Parser(const std::wstring &pattern, NodeBuilder *builder);
		~Parser();

public:
		ParserErr AddToken(const AR_Token_t *token);
		Node*	  GetTree();
};

