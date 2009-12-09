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


#include "../Parser/grammar.h"
#include "../Parser/parser.h"
#include "../Lex/lex.h"



AR_NAMESPACE_BEGIN




struct __cfg_node_tag;
typedef struct __cfg_node_tag	cfgNode_t;

typedef struct __cfg_lexeme_tag
{
		size_t			line;
		const wchar_t	*lexeme;
		size_t			lex_val;
}cfgLexeme_t;

typedef struct __cfg_lex_name_tag
{
		size_t			line;
		const wchar_t	*name;
		const wchar_t	*regex;
}cfgName_t;

typedef struct __cfg_token_tag
{
		size_t			line;
		const wchar_t	*name;
		size_t			tokval;
		size_t			lex_prec;
		const wchar_t	*regex;
		bool_t			is_skip;
}cfgToken_t;

typedef struct __cfg_prec_tag
{
		size_t	line;
		const wchar_t	*prec_tok;
		psrAssocType_t	assoc;
}cfgPrec_t;


typedef struct __rule_tag
{
		size_t					line;
		const wchar_t			*lhs;
		const wchar_t			*rhs;
		const wchar_t			*prec_tok;
		const wchar_t			*handler_name;
}cfgRule_t;





typedef struct __cfg_node_list_tag
{
		cfgNode_t		**lst;
		size_t			count;
		size_t			cap;
}cfgNodeList_t;



typedef struct __cfg_config_tag
{
		cfgName_t		*name;
		size_t			name_cnt;

		cfgToken_t		*tok;
		size_t			tok_cnt;

		cfgPrec_t		*prec;
		size_t			prec_cnt;

		cfgRule_t		*rule;
		size_t			rule_cnt;
}cfgConfig_t;




cfgConfig_t*	CFG_CollectGrammarConfig(const wchar_t *gmr_txt, void *io);
void			CFG_DestroyGrammarConfig(cfgConfig_t *cfg);


/*void			CFG_ConfigToCode(const cfgConfig_t *cfg, arString_t	*code);*/



AR_NAMESPACE_END


