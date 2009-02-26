
#if(0)
#include "..\Parser\parser_in.h"
#include "..\Lex\lex.h"



/**************************Test********************************/



void __print_symb(const psrSymb_t *symb)
{
		if(symb->non_term)
		{
				wprintf(L"<%s> ", symb->name);
		}else
		{
				wprintf(L"%s ", symb->name);
		}
}

void __print_symblist(const psrSymb_t *symb)
{
		while(symb != NULL)
		{
				__print_symb(symb);
				symb = symb->next;
		}
}

void __print_bnf(const psrGrammar_t *gmr)
{
		size_t i;
		for(i = 0; i < gmr->count; ++i)
		{
				__print_symblist(gmr->head[i]);
				printf(" ::= ");
				__print_symblist(gmr->body[i]);
				printf(" == %d ",gmr->head[i]->id);
				printf("\n");
		}
}


void __print_first_follow(const psrGrammar_t *gmr)
{
		size_t i;
		printf("first:\n");
		for(i = 0; i < gmr->non_count; ++i)
		{
				__print_symb(gmr->nonterm_set[i]);
				printf(" = ");
				__print_symblist(gmr->first[i]);
				printf("\n");
		}
		printf("------------------------\n");
		printf("follow:\n");
		for(i = 0; i < gmr->non_count; ++i)
		{
				__print_symb(gmr->nonterm_set[i]);
				printf(" = ");
				__print_symblist(gmr->follow[i]);
				printf("\n");
		}


}



static const wchar_t *test_input = 
L"%token{\r\n"
L"'(' : 259;\r\n"
L"')'->260;\r\n"
L"'+' : 280; '-' = 281; '*' : 282; '/' : 283;\r\n"
L"%}\r\n"

L"%prec{\r\n"

L"%left			'+' '-';\r\n"
L"%left			'*' '/';\r\n"
L"%noassoc		RIGHT_MINUE;\r\n"
L"%}\r\n"

L"%rules{\r\n"
L"/*%id : 381\r\n*/"
L"E	->	E OP E | E '-' E|E '*' E | E '/' E | '(' E ')'%prec RIGHT_MINUE|'-'E %prec RIGHT_MINUE;"
L"%}\r\n"

;




AR_API void grammar_test1()
{
		psrGrammar_t *gmr;
		size_t i;
		gmr = PSR_BuildGrammar(test_input);
		
		if(gmr == NULL)
		{
				abort();
		}
		
		__print_bnf(gmr);

		for(i = 0; i < gmr->count; ++i)
		{
				wprintf(L"head. name == %s : id == %d : prio == %d\n", gmr->head[i]->name, gmr->head[i]->id, gmr->head[i]->priority);
		}

}

static const wchar_t *lex_pattern = 
LEX_DEF("delim = [ \\r\\n\\t]*")
LEX_DEF("digit = [0-9]")
LEX_DEF("number =[0-9]+")
LEX_DEF("letter =[A-Z_a-z]")
LEX_DEF("id ={letter}+")
LEX_DEF("(0) {delim}*")
LEX_DEF("(300) \\(")
LEX_DEF("(301) \\)")
LEX_DEF("(302) \\+")
LEX_DEF("(303) \"-\"")
LEX_DEF("(304) \"*\"")
LEX_DEF("(305) \"/\"")
LEX_DEF("(306) \"=\"")
LEX_DEF("(307) \",\"")
LEX_DEF("(400) {number}")
LEX_DEF("(401) {id}")
;


static const wchar_t *gmr_pattern = 
L"%token{\r\n"
L"'(' : 300, ')' : 301, '+' : 302, '-' : 303, '*' : 304, '/' = 305, '=' : 306,',' = 307;"
L"number : 400;"
L"id : 401;"
L"%}\r\n"

L"%prec{\r\n"
L"%right '=';"
L"%left '+' '-';"
L"%left '*' '/';"
L"%right UMINUS;"
L"%}\r\n"

L"%rules{\r\n"
//L"E -> E '+' E | E '-' E | E '*' E | E '/' E | '('E')' | '-' E %prec UMINUS| number;"
L"E -> E add E %prec '+' | E mul E %prec '-' | E assign E %prec '='| '('E')' | '-' E %prec UMINUS | call | number | id;"
L"assign -> '=';"
L"add -> '+' | '-';"
L"mul -> '*' | '/';"
L"varlist -> varlist ',' id | id | .;"
L"call    -> id '(' varlist ')';"

L"%}\r\n"
;


typedef struct __calc_tag CalcNode_t;

typedef struct __calc_tag 
{
		int op;
		wchar_t name[1024];
		CalcNode_t *chd;
}CalcNode_t;



psrNode_t* CreateLeaf(const psrToken_t *tok)
{
		wprintf(L"in leaf == %d : value == %s\n", tok->lex_type, tok->value);
		return (psrNode_t*)AR_NEW0(CalcNode_t*);
}

psrNode_t* CreateNode(size_t rule_id,  const psrNodeSet_t *node_set)
{
		printf("rule_id == %d,node_set.count == %d\n", rule_id,node_set->count);
		return (psrNode_t*)AR_NEW0(CalcNode_t*);
}

void	   DestroyNode(psrNode_t *node)
{
		printf("destroy\n");
		//wprintf(L"%s\n", node->name);
}


#include <windows.h>

AR_API void parser_test()
{
		psrNode_t *node;
		parser_t *parser;
		DWORD beg,end;
		printf("--------------------\n");
		beg = GetTickCount();

		parser = PSR_CreateParser(lex_pattern, gmr_pattern,CreateLeaf, CreateNode, DestroyNode,PSR_LALR);

		end = GetTickCount();
		printf("elapsed == %d\n", end - beg);

		__print_bnf(parser->grammar);

		getchar();

		node = PSR_Parse(parser, L"a = test(a,b,c)");
		
		
		node = node;
		getchar();

}



#if(0)

const wchar_t *bnf_input1 = 
L"%{"
L"'+' ::= (257)"
L"'-' ::= (258)"
L"'*' ::= (259)"
L"'/' ::= (260)"
L"if  ::= (261)"
L"else ::= (262)"
L"id	::= (263)"
L"'('	::= (264)"
L"')'	::= (265)"
L"%}"
L"<  expr > ::=   < expr> '+' <expr>			(259,1)"
L"		   | <expr> '-' <expr>					(262,1)"
L"		   | <expr> '*' <expr>					(263,2)"
L"		   | <expr> '/' < expr>					(264,2)"
L"		   | '('<expr> ')'						(265,3)"
L"		   | '-'<expr> 							(266,4)"
L"		   | id 								(267,5);"
;

const wchar_t *bnf_input=
L"%{"
L"'+' ::= (257)"
L"'-' ::= (258)"
L"'*' ::= (259)"
L"'/' ::= (260)"
L"if  ::= (261)"
L"else ::= (262)"
L"id	::= (263)"
L"'('	::= (264)"
L"')'	::= (265)"
L"%}"
/*L"<E> ::= <T><E_1>(301);\
<E_1> ::= '+'<T><E_1> (302)| Epsilon(303);\
<T> ::= <F> <T_1>(304);\
<T_1>::=  '*'<F><T_1> (305)| Epsilon(306);\
<F> ::= '('<E>')' (307)| id(308);\*/
L"<E> ::= <E> '+' <E> (301)|<E> '-' <E> (302)| <E> '*' <E> (303)| <E> '/' <E> (304)| '('<E>')' (305)| '-'<E> (306)| id(307) /*| Epsilon(308)*/; "\
;


static void __print_token_table(psrTokenTable_t *tbl)
{
		size_t i;
		for(i = 0; i < tbl->count; ++i)
		{
				psrTokVal_t tok = tbl->tokset[i];
				wprintf(L"tok.str == %s : val = %d\n", tok.str, tok.val);
		}
}

static void __print_gmr_node(gmrNode_t *node)
{
		if(node->type == GMR_TERM)
		{
				wprintf(L"%s ", node->tokval.str);
		}else
		{
				wprintf(L"<%s> ", node->rule.name);
		}
}

static void __print_rules(Grammar_t *gmr)
{
		size_t i,j;
		for(i = 0; i < gmr->count; ++i)
		{
				__print_gmr_node(&gmr->head[i]);
				printf("id == %d : prio == %d ", gmr->head[i].rule.rule_id, gmr->head[i].rule.prio);
				printf(" ::= ");
				for(j = 0; j < gmr->body[i].count; ++j)
				{
						__print_gmr_node(&gmr->body[i].nodes[j]);
				}
				printf("\n");

		}
}

void __print_map(const gmrNodeMap_t *map)
{
		size_t i,j;
		for(i = 0; i < map->count; ++i)
		{
				if(map->key[i].type == GMR_TERM)
				{
						wprintf(L"node[%s] == {",map->key[i].tokval.str);
				}else
				{
						wprintf(L"node[<%s>] == {",map->key[i].rule.name);
				}
				
				for(j = 0; j < map->val[i].count; ++j)
				{
						AR_ASSERT(map->val[i].nodes[j].type == GMR_TERM);
						wprintf(L"%s ", map->val[i].nodes[j].tokval.str);
				}
				printf("}\n");
		}
}

AR_API void bnf_pattern_test()
{
		
		Grammar_t gmr;
		GMR_InitGrammar(&gmr);
		
		printf("err = %d\n", GMR_BuildGrammar(&gmr, bnf_input));


		__print_token_table(&gmr.tok_tbl);
		
		__print_rules(&gmr);
		printf("-------------------------\n");
		printf("first set:\n");
		__print_map(&gmr.first);
		printf("-------------------------\n");
		printf("follow set:\n");
		__print_map(&gmr.follow);
		printf("-------------------------\n");
		GMR_UnInitGrammar(&gmr);
}

#endif

#endif
