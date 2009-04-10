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

#include "grammar.h"
#include "parser_in.h"

#include "..\Lex\lex.h"

AR_NAMESPACE_BEGIN



/********************************************configGrammar******************************************************/
typedef struct __gmr_name_tag
{
		const wchar_t	*name;
		const wchar_t	*pat;
}gmrName_t;



static gmrName_t		__gmr_name[]=
{
		{L"any", L"[^\\0]"},
		{L"delim", L"[\\r\\n\\t ]*"},
		{L"letter", L"[A-Z_a-z]"},
		{L"digit",  L"[0-9]"},
		{L"number",  L"0|[1-9]{digit}*"},
		{L"name",  L"{letter}({letter}|{digit})*"},
		{L"lexeme",  L"{name}|('[^']+')|(\\\"[^\\\"]+\\\")"},

		{L"assign",  L"(\":\")|(\"=\")|(\"->\")"},
		{L"rule_sep",  L"\\|"},
		{L"end_char",  L"\";\""},
		{L"sep_char",  L"\",\""},
		{L"epsilon_char",  L"\".\""},
		{L"tokdef_begin",  L"\"%token{\""},
		{L"prec_begin",  L"\"%prec{\""},
		{L"rules_begin",  L"\"%rules{\""},
		{L"end",  L"\"%}\""},
		{L"asso_def",  L"\"%\"(left|right|noassoc)"},
		{L"prec_def",  L"\"%prec\""},
		{L"comment",  L"\\/\\*([^\\*]|\\*+[^\\*\\/])*\\*+\\/"},
		
		{L"comment_line", L"\\/\\/([^\\n])*\\n"},

		{L"start_id_def",  L"\"%id\""},
		{L"EOI",		L"\\0"}
};


typedef struct __gmr_pat_tag
{
		size_t			type;
		const wchar_t	*pat;
}gmrPattern_t;


typedef enum
{
		DELIM = 0,
		ASSIGN = 1,
		RULE_SEP = 2,
		END_SEP_CHAR = 3,
		TOK_DEF_BEGIN = 4,
		PREC_DEF_BEGIN =  5,
		RULES_DEF_BEGIN = 6,
		DEF_END = 7,
		COMMENT = 8,
		LEXEME = 9,
		NUMBER = 10,
		ASSOC_DEF = 11,
		PREC_DEF = 12,
		EPSILON_CHAR = 13,
		START_ID = 14,
		EOI		= 15,
		COMMENT_LINE = 16
}psrGrammarTokenType_t;

static gmrPattern_t		__gmr_pattern[]=
{
		{DELIM, L"{delim}"},
		{ASSIGN, L"{assign}"},
		{RULE_SEP,L"{rule_sep}"},


		{END_SEP_CHAR, L"({end_char}|{sep_char})"},
		{TOK_DEF_BEGIN, L"{tokdef_begin}"},
		{PREC_DEF_BEGIN, L"{prec_begin}"},
		{RULES_DEF_BEGIN, L"{rules_begin}"},
		{DEF_END, L"{end}"},
		{COMMENT, L"{comment}"},
		{LEXEME, L"{lexeme}"},
		{NUMBER, L"{number}"},
		{ASSOC_DEF, L"{asso_def}"},
		{PREC_DEF, L"{prec_def}"},
		{EPSILON_CHAR, L"{epsilon_char}"},
		{START_ID, L"{start_id_def}"},
		{EOI,	L"{EOI}"},
		{COMMENT_LINE, L"{comment_line}"}
	
};


lex_t* __build_lex()
{
		size_t i;
		lex_t *lex;
		lex = LEX_Create();
		
		for(i = 0; i < AR_NELEMS(__gmr_name); ++i)
		{
				if(!LEX_InsertName(lex, __gmr_name[i].name, __gmr_name[i].pat))
				{
						AR_ASSERT(false);
						AR_error(AR_GRAMMAR, L"Grammar Builder Error : (name %s: pattern %s\r\n", __gmr_name[i].name, __gmr_name[i].pat);
						LEX_Destroy(lex);
						return NULL;
				}
		}
		

		for(i = 0; i < AR_NELEMS(__gmr_pattern); ++i)
		{
				lexAction_t action;
				action.priority = 0;

				action.type = __gmr_pattern[i].type;

				if(!LEX_InsertRule(lex, __gmr_pattern[i].pat, &action))
				{
						AR_ASSERT(false);
						AR_error(AR_GRAMMAR, L"Grammar Builder Error : (type %d: pattern %s\r\n", __gmr_pattern[i].type, __gmr_pattern[i].pat);
						LEX_Destroy(lex);
						return NULL;
				}
		}
		
		if(!LEX_GenerateTransTable(lex))
		{
				LEX_Destroy(lex);
				return NULL;
		}

		LEX_InsertSkipAction(lex, DELIM);
		LEX_InsertSkipAction(lex, COMMENT);
		LEX_InsertSkipAction(lex, COMMENT_LINE);
		return lex;
}




static bool  __get_token(lex_t *lex, lexMatch_t *match, lexToken_t *tok)
{

__GMR_LOOP__:

		if(LEX_Match(lex, match, tok))
		{
				if(tok->type == DELIM || tok->type == COMMENT || tok->type == COMMENT_LINE)
				{
						AR_ASSERT(false);
						goto __GMR_LOOP__;
				}else
				{
						return true;
				}
		}else 
		{
				return false;
		}
}

static const wchar_t *type_str[] = 
{
		L"",
		L"=|:=|->",
		L"|",
		L";",
		L"%token",
		L"%prec",
		L"%rules",
		L"%}",
		L"comment",
		L"token name",
		L"token value",
		L"assoc define",
		L"prec define",
		L"epsilon",
		L"%id",
		L"EOI",
		L"comment line"
};



static bool __match(lex_t *lex, lexMatch_t *match, lexToken_t *tok, size_t type)
{
		bool is_ok;

		is_ok = __get_token(lex, match, tok);

		if(is_ok && tok->type == type)/*tok->str不可能为NULL;*/
		{
				return true;
		}else
		{
				if(is_ok)
				{
						AR_ASSERT(tok->str != NULL);
						AR_error(AR_GRAMMAR, L"Grammar Error : %s expected in (%d : %d)\r\n", tok->line, tok->col);
				}else
				{
						AR_error(AR_GRAMMAR, L"Grammar Error : %s expected in (%d : %d)\r\n", match->line, match->col);
				}
				return false;
		}
}

static bool __match_check(lex_t *lex, lexMatch_t *match, lexToken_t *tok, size_t type)
{
		if(__get_token(lex, match, tok) && tok->type == type)
		{
				return true;
		}else
		{
				return false;
		}
}




static bool __handle_token_define(lex_t *lex, lexMatch_t *match, psrGrammar_t *grammar)
{
		lexToken_t tok;
		const psrSymb_t *symb;
		wchar_t name[1024];
		wchar_t buf[1024];
		uint_t	val;
		
		AR_ASSERT(lex != NULL && match != NULL && grammar != NULL);
		AR_memset(&tok, 0, sizeof(tok));
		val = 0;
		symb = NULL;

		while(true)
		{
				if(!__match_check(lex, match, &tok, LEXEME))
				{
						break;
				}
				
/***************************************************************************/
				if(tok.str[0] == '\'' || tok.str[0] == '"')
				{
						AR_ASSERT(tok.count >= 3);
						AR_wcsncpy(name, tok.str + 1, tok.count-2);
						name[tok.count-2] = L'\0';
				}else
				{
						AR_wcsncpy(name, tok.str, tok.count);
						name[tok.count] = L'\0';
				}
/***************************************************************************/

				
				if(!__match(lex, match, &tok, ASSIGN))
				{
						return false;
				}
				

				if(!__match(lex, match, &tok, NUMBER))
				{
						return false;
				}

				AR_wcsncpy(buf, tok.str, tok.count);
				buf[tok.count] = L'\0';

				if(AR_wtou(buf, &val, 10) == NULL)
				{
						return false;
				}

				if(!__match(lex, match, &tok, END_SEP_CHAR))
				{
						return false;
				}

				symb = PSR_CreateSymb(name, PSR_TERM, 0, PSR_ASSOC_NOASSOC, val);
				if(!PSR_InsertTerm(grammar, symb))
				{
						AR_error(AR_GRAMMAR, L"Grammar Error : invalid token name %s\r\n", name);
						PSR_DestroySymb(symb);
						return false;
				}
		}
		
		if(tok.type != DEF_END)
		{
				AR_error(AR_GRAMMAR, L"Grammar Error : %s expected  in %s\r\n", type_str[DEF_END], tok.str);
				return false;
		}else
		{
				return true;
		}
}




static bool __handle_prec_define(lex_t *lex, lexMatch_t *match, psrGrammar_t *grammar)
{
		lexToken_t tok;
		wchar_t buf[1024];
		size_t prio;
		psrAssocType_t assoc;
		AR_ASSERT(lex != NULL && match != NULL && grammar != NULL);
		AR_memset(&tok, 0, sizeof(tok));
		
		

		prio = 1;
		assoc = PSR_ASSOC_NOASSOC;
		//__match_check(lex, match, &tok, ASSOC_DEF);
		//while(tok.type == ASSOC_DEF)
		while(__match_check(lex, match, &tok, ASSOC_DEF))
		{
				if(tok.str[1] == L'l')
				{
						assoc = PSR_ASSOC_LEFT;
				}else if(tok.str[1] == L'r')
				{
						assoc = PSR_ASSOC_RIGHT;
				}else
				{
						assoc = PSR_ASSOC_NOASSOC;
				}
				
				while(__match_check(lex, match, &tok, LEXEME))
				{
						psrSymb_t *symb;

/***************************************************************************/
						if(tok.str[0] == '\'' || tok.str[0] == '"')
						{
								AR_ASSERT(tok.count >= 3);
								AR_wcsncpy(buf, tok.str + 1, tok.count-2);
								buf[tok.count-2] = L'\0';
						}else
						{
								AR_wcsncpy(buf, tok.str, tok.count);
								buf[tok.count] = L'\0';
						}

						
/*
						AR_wcsncpy(buf, tok.str, tok.count);
						buf[tok.count] = L'\0';
*/
/***************************************************************************/

						

						symb = (psrSymb_t*)PSR_FindTermByName(grammar, buf);
						
						if(symb == NULL)
						{
								const psrSymb_t *holder;
								size_t hl_val;
								hl_val = PSR_MIN_PRIOTERMVAL;
								while(PSR_FindTermByValue(grammar, hl_val) != NULL)
								{
										hl_val++;
								}

								holder = PSR_CreateSymb(buf, PSR_TERM, prio, assoc, hl_val);
								PSR_InsertTerm(grammar, holder);
								PSR_DestroySymb(holder);
						}else
						{
								PSR_SetTerm(grammar, buf, assoc, prio);
						}

						if(!__match(lex, match, &tok, END_SEP_CHAR))return false;

						if(tok.str[0] == L';')break;
				}
				prio++;
		}

		if(tok.type != DEF_END)
		{
				AR_error(AR_GRAMMAR, L"Grammar Error : %s expected in (%d : %d)\r\n", type_str[DEF_END], tok.line, tok.col);
				return false;
		}else
		{
				return true;
		}
}

static bool __handle_one_rule(lex_t *lex, lexMatch_t *match, psrGrammar_t *grammar, const psrSymb_t *head, psrSymbList_t *lst, lexToken_t *ptok)
{
		wchar_t name[1024];
		const psrSymb_t *prec_tok;
		
		lexToken_t tok;
		tok = *ptok;
		prec_tok = NULL;
RE_CHECK_POINT:
		if(tok.type == PREC_DEF)
		{

				if(!__match(lex, match, &tok, LEXEME))
				{
						return false;
				}

				/************************************************************************************************************/
				if(tok.str[0] == '\'' || tok.str[0] == '"')
				{
						AR_ASSERT(tok.count >= 3);
						AR_wcsncpy(name, tok.str + 1, tok.count-2);
						name[tok.count-2] = L'\0';
				}else
				{
						AR_wcsncpy(name, tok.str, tok.count);
						name[tok.count] = L'\0';
				}
				/*
				AR_wcsncpy(buf, tok.str, tok.count);
				buf[tok.count] = L'\0';
				*/
				/************************************************************************************************************/

				prec_tok = PSR_FindTermByName(grammar, name);

				if(prec_tok == NULL)
				{
						AR_error(AR_GRAMMAR, L"Grammar Error : prec token %s not defined\r\n", name);
						return false;
				}

				if(!__get_token(lex, match, &tok))
				{
						AR_error(AR_GRAMMAR, L"Grammar Error : invalid rule : %s\r\n", match->next);
						return false;
				}

				if(tok.type != RULE_SEP && tok.type != END_SEP_CHAR)
				{
						AR_error(AR_GRAMMAR, L"Grammar Error : ';' or '|' expected in (%d : %d)\r\n", tok.line, tok.col);
						return false;
				}else
				{
						goto RE_CHECK_POINT;
				}
				

		}else if(tok.type == RULE_SEP)
		{
				if(lst->count == 0)
				{
						PSR_InsertToSymbList(lst, PSR_CopyNewSymb(PSR_EpsilonSymb));
				}

				if(!PSR_InsertRule(grammar, head, lst, prec_tok))
				{
						return false;
				}else
				{
						return true;
				}

		}else if(tok.type == END_SEP_CHAR)
		{
				if(lst->count == 0)
				{
						PSR_InsertToSymbList(lst, PSR_CopyNewSymb(PSR_EpsilonSymb));
				}

				if(!PSR_InsertRule(grammar, head, lst, prec_tok))
				{
						return false;
				}else
				{
						*ptok = tok;
						return true;
				}

		}else if(tok.type == EPSILON_CHAR)
		{
				if(lst->count != 0)
				{
						return false;
				}
				__get_token(lex, match, &tok);
				goto RE_CHECK_POINT;
		}else
		{
				return false;
		}
}


static bool __handle_rule(lex_t *lex, lexMatch_t *match, psrGrammar_t *grammar, const psrSymb_t *head, psrSymbList_t *lst)
{
		lexToken_t tok;
		
		wchar_t name[1024];
		AR_ASSERT(lex != NULL && match != NULL && grammar != NULL);
		AR_memset(&tok, 0, sizeof(tok));
		
		while(true)
		{
				bool is_ok;
				const psrSymb_t *tmp;
				tmp = NULL;
				while(true)
				{
						
						if(!__match_check(lex, match, &tok, LEXEME))break;

						/************************************************************************************************************/
						if(tok.str[0] == '\'' || tok.str[0] == '"')
						{
								AR_ASSERT(tok.count >= 3);
								AR_wcsncpy(name, tok.str + 1, tok.count-2);
								name[tok.count-2] = L'\0';
						}else
						{
								AR_wcsncpy(name, tok.str, tok.count);
								name[tok.count] = L'\0';
						}

						/************************************************************************************************************/

						tmp = PSR_FindTermByName(grammar,name);
						if(tmp == NULL)
						{
								tmp = PSR_CreateSymb(name, PSR_NONTERM, 0, PSR_ASSOC_NOASSOC, 0);
								PSR_InsertToSymbList(lst, tmp);
						}else
						{
								tmp = PSR_CreateSymb(tmp->name, tmp->type, tmp->prec, tmp->assoc, tmp->val);
								PSR_InsertToSymbList(lst, tmp);
						}
				}
				
				is_ok = __handle_one_rule(lex, match, grammar, head, lst, &tok);
				
				{
						size_t k;
						for(k = 0; k < lst->count; ++k)
						{
								PSR_DestroySymb(lst->lst[k]);
						}
						PSR_ClearSymbList(lst);
				}

				if(!is_ok)return false;

				if(tok.type == END_SEP_CHAR)break;

		}
		
		if(tok.type != END_SEP_CHAR)
		{
				AR_error(AR_GRAMMAR, L"Grammar Error : ';' expected in %s\r\n", tok.str);
		}
		return tok.type == END_SEP_CHAR;
}


static bool __handle_rule_define(lex_t *lex, lexMatch_t *match, psrGrammar_t *grammar)
{
		lexToken_t tok;
		wchar_t name[1024];
		
		AR_ASSERT(lex != NULL && match != NULL && grammar != NULL);
		AR_memset(&tok, 0, sizeof(tok));
		
		

		while(__match_check(lex, match, &tok, LEXEME))
		{
				const psrSymb_t *head;
				psrSymbList_t	body;
				
				head = NULL;
				PSR_InitSymbList(&body);

/***************************************************************************/
				if(tok.str[0] == '\'' || tok.str[0] == '"')
				{
						AR_ASSERT(tok.count >= 3);
						AR_wcsncpy(name, tok.str + 1, tok.count-2);
						name[tok.count-2] = L'\0';
				}else
				{
						AR_wcsncpy(name, tok.str, tok.count);
						name[tok.count] = L'\0';
				}

/***************************************************************************/

				if(PSR_FindTermByName(grammar, name) != NULL)
				{
						AR_error(AR_GRAMMAR, L"Grammar Error : invalid rule head \"%s\" is a term symbol\r\n", name);
						PSR_UnInitSymbList(&body);
						return false;
				}
				
				head = PSR_CreateSymb(name, PSR_NONTERM, 0, PSR_ASSOC_NOASSOC, 0);
				

				if(!__match(lex, match, &tok, ASSIGN))return false;
				
				if(!__handle_rule(lex, match, grammar, head, &body))
				{
						

						PSR_DestroySymb(head);
						PSR_UnInitSymbList(&body);
						return false;
				}else
				{
						PSR_DestroySymb(head);
						PSR_UnInitSymbList(&body);
				}
		}

		if(tok.type != DEF_END)
		{
				AR_error(AR_GRAMMAR, L"Grammar Error : %s expected in %s\r\n", type_str[DEF_END], tok.str);
				return false;
		}else
		{
				return true;
		}
}


bool PSR_ConfigGrammar(psrGrammar_t *grammar, const wchar_t *pattern)
{
		bool is_ok;
		lex_t *lex;
		lexMatch_t match;
		lexToken_t tok;
		
		is_ok = true;
		lex = __build_lex();
		LEX_InitMatch(&match, pattern);
		AR_ASSERT(lex != NULL);
		
		__get_token(lex, &match, &tok);

		if(tok.type == TOK_DEF_BEGIN)
		{
				is_ok = __handle_token_define(lex, &match, grammar);
				if(!is_ok)goto END_POINT;
				__get_token(lex, &match, &tok);
		}
		
		if(tok.type == PREC_DEF_BEGIN)
		{
				is_ok = __handle_prec_define(lex, &match, grammar);
				if(!is_ok)goto END_POINT;
				__get_token(lex, &match, &tok);
		}
		
		if(tok.type == RULES_DEF_BEGIN)
		{
				is_ok = __handle_rule_define(lex, &match, grammar);
				if(!is_ok)goto END_POINT;
		}else
		{
				goto END_POINT;
		}
END_POINT:
		LEX_Destroy(lex);
		LEX_UnInitMatch(&match);
		
		if(!is_ok)return false;
		return PSR_CheckIsValidGrammar(grammar);
}



AR_NAMESPACE_END