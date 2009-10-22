//#include "stdafx.h"
#include "AR.h"


AR_NAMESPACE_BEGIN




#if(0)
class Parser : private NonCopyable
{
public:
		typedef enum 
		{
				PARSER_SLR = PSR_SLR,
				PARSER_LR1 = PSR_LR1,
				PARSER_LALR = PSR_LALR
		}ParserType_t;
private:
		psrGrammar_t	*m_grammar;
		parser_t		*m_parser;
		printNode_t		*m_result;
		arString_t		*m_strbuf;
public:
		bool SetGrammar(const wchar_t *gmr, Parser::ParserType_t type = PARSER_LALR);
		bool Parse(const wchar_t *source);
public:
		void PrintGrammar();
		void PrintFirstFollow();
		void ReportConflict();
		void PrintActionTable();
public:
		Parser();
		~Parser();
};
#endif


Parser::Parser() 
		: m_grammar(PSR_CreateGrammar())
		, m_strbuf(AR_CreateString())
		, m_parser(NULL)
{


}

Parser::~Parser()
{
		Clear();
		AR_DestroyString(m_strbuf);
		PSR_DestroyGrammar(m_grammar);


}



void Parser::Clear()
{
		assert(m_grammar != NULL && m_strbuf != NULL);

		PSR_ClearGrammar(m_grammar);
		AR_ClearString(m_strbuf);

		if(m_parser)
		{
				PSR_DestroyParser(m_parser);
		}

//		if(m_result)PSR_DestroyPrintNode(m_result);

		m_parser = NULL;

}


void Parser::PrintGrammar()const
{
		assert(m_grammar != NULL && m_strbuf != NULL && m_parser != NULL);
		
		AR_ClearString(m_strbuf);
		PSR_PrintGrammar(m_grammar, m_strbuf);
		AR_printf(L"%s\r\n", AR_GetStrString(m_strbuf));
		AR_ClearString(m_strbuf);
}

void Parser::PrintFirstFollow()const
{
		psrSymbMap_t first_set, follow_set;

		assert(m_grammar != NULL && m_strbuf != NULL && m_parser != NULL);

		

		PSR_InitSymbMap(&first_set);
		PSR_InitSymbMap(&follow_set);


		PSR_CalcFirstSet(m_grammar, &first_set);
		PSR_CalcFollowSet(m_grammar, &follow_set, &first_set);

		AR_ClearString(m_strbuf);
		PSR_PrintSymbolMap(&first_set, m_strbuf);
		AR_printf(L"%s\r\n", AR_GetStrString(m_strbuf));

		AR_ClearString(m_strbuf);
		PSR_PrintSymbolMap(&follow_set, m_strbuf);
		AR_printf(L"%s\r\n", AR_GetStrString(m_strbuf));
		
		PSR_UnInitSymbMap(&first_set);
		PSR_UnInitSymbMap(&follow_set);

		AR_ClearString(m_strbuf);
}

/*

void PSR_PrintActionTable(const psrActionTable_t *tbl, const psrGrammar_t *grammar, size_t width, arString_t *str);
void PSR_ReportConflict(const psrActionTable_t *tbl, const psrGrammar_t *grammar, arString_t *str);

*/
void Parser::ReportConflict()const
{
		assert(m_grammar != NULL && m_strbuf != NULL && m_parser != NULL);
		AR_ClearString(m_strbuf);
		PSR_PrintParserActionTable(m_parser,m_strbuf,80);
		AR_printf(L"%s\r\n", AR_GetStrString(m_strbuf));
		AR_ClearString(m_strbuf);
		
}


void Parser::PrintActionTable()const
{
		assert(m_grammar != NULL && m_strbuf != NULL && m_parser != NULL);
		AR_ClearString(m_strbuf);
		PSR_PrintParserConflict(m_parser,m_strbuf);
		AR_printf(L"%s\r\n", AR_GetStrString(m_strbuf));
		AR_ClearString(m_strbuf);

}



bool Parser::SetGrammar(const wchar_t *gmr, Parser::ParserType_t type)
{
		assert(gmr != NULL);
		Clear();
		if(!PSR_ConfigGrammar(m_grammar, gmr))return false;
		m_parser = PSR_CreateParser(m_grammar, (psrModeType_t)type, PSR_GetPrintNodeContext());
		assert(m_parser != NULL);
		
		return true;
}

bool Parser::AddToken(const psrToken_t *tok)
{
		assert(tok != NULL);
		assert(m_parser != NULL);
		return PSR_AddToken(m_parser, tok);
}


const printNode_t* Parser::GetResult()const
{
		assert(m_parser != NULL);
		return (printNode_t*)PSR_GetResult(m_parser);

}


AR_NAMESPACE_END

