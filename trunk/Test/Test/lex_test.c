#include "test.h"
#include "../../Arsenal/Lex/lex.h"

#if defined(__LIB)

AR_NAMESPACE_BEGIN



#if(0)
void charsetShowChar(wchar_t c, bool_t is_digit)
{
		const wchar_t *fmt;

		switch(c)
		{
		case L'{':
		case L'}':
		case L'[':
		case L']':
		case L'(':
		case L')':
		case L'^':
		case L'$':
		case L'?':
		case L'+':
		case L'-':
		case L'*':
		case L'/':
		case L'|':
		case L'<':
		case L'>':
		case L'#':
		case L'.':
				fmt = is_digit ? L"%u" : L"%c";
				AR_printf(fmt, c);
				break;
		default:
				fmt = is_digit ? L"%u" : L"%c";
				AR_printf(fmt, c);
				break;
		}
}

void charsetShow(const lexCharSet_t	*cset, bool_t is_digit)
{
		const lexCharRange_t	*curr;

		curr = cset->range;

		while(curr != NULL)
		{
				charsetShowChar(curr->beg, is_digit);
				printf("-");
				charsetShowChar(curr->end, is_digit);
				
				curr = curr->next;
		}
		printf("\n");
}


static void __in_lexnodeShow(lexNode_t *root)
{
		size_t i;
		lexCharRange_t *curr;
		switch(root->t)
		{
		case LEX_CHARSET:
				if(!root->charset.is_neg && root->charset.range->next == NULL && root->charset.range->beg == root->charset.range->end)
				{
						charsetShowChar(root->charset.range->beg, true);
				}else
				{
						AR_printf(L"[");
						if(root->charset.is_neg) printf("^");
						for(curr = root->charset.range; curr; curr = curr->next)
						{
								if(curr->beg < curr->end)
								{
										charsetShowChar(curr->beg, true);
										printf("-");
										charsetShowChar(curr->end, true);
										//AR_printf(L"%c-%c", curr->beg, curr->end);
								}else
								{
										charsetShowChar(curr->beg, true);
								}
						}
						AR_printf(L"]");
				}
				break;
		case LEX_BRANCH:
				AR_printf(L"(");
				for(i = 0; i < root->branch.count; ++i)
				{
						__in_lexnodeShow(root->branch.arr[i]);
						if(i < root->branch.count - 1)AR_printf(L"|");
				}
				AR_printf(L")");
				break;
		case LEX_CAT:
				AR_printf(L"(");
				__in_lexnodeShow(root->cat.left);
				if(root->cat.right != NULL) __in_lexnodeShow(root->cat.right);
				
				AR_printf(L")");
				break;
		case LEX_LOOP:
				AR_printf(L"(");__in_lexnodeShow(root->loop);AR_printf(L")");
				AR_printf(L"*");
				break;
		case LEX_FINAL:
				printf("final");
				break;
		case LEX_CCLASS_ID:
				printf("%d ", root->cclass_id);
				break;
		default:
				charsetShowChar(L'#', false);
				break;
		}
}
static void LEX_ShowNode(LexNodeT *root)
{
		__in_lexnodeShow(root);
		printf("\n");
}


extern lexResult_t __handle_expr(const wchar_t *input, wchar_t tc, lexName_t *lst);

void __print_dfatbl(lexStateTable_t *tbl)
{
		size_t i,j;

		for(i = 0; i < tbl->row; ++i)
		{
				for(j = 0; j < tbl->col; ++j)
				{
						printf("%d ", tbl->tbl[AR_TBL_IDX_R(i,j,tbl->col)]);
				}
				printf("\n");
		}

}

#if(0)
%lex{
	str 	=	\"([^\\\"])*\"
	EOI	=	\0

	100			{str}
	0			{EOI}
}
#endif


extern void __replace_cset_to_ccid(lexNode_t *node, void *ctx);
extern void __calc_follow(lexNode_t *node);

void expr_test()
{
		lexName_t	*tbl; lexResult_t res;
		lexCClass_t cclass;
		lexLeafSet_t *set;
		lexDFA_t	 *dfa;
		lexStateTable_t stat_tbl;
		lexNode_t *cat,*tmp;
		//AR_ASSERT(LEX_InsertNameToNameTable(&tbl, L"_name", L"a*") == LEX_NO_ERROR);
		tbl = NULL;
		LEX_InitCClass(&cclass);
		LEX_InitStateTable(&stat_tbl);
		//res = __handle_expr(L"(abc){0,}{1,4}|(b)|\"xxx\\\"\"+",L'\0', tbl);
		
		//res = __handle_expr(L"\\\"([^\\\\\\\"])*\\\"",L'\0', tbl);
		//res = __handle_expr(L"\\\"([^\\\\]|[^\\\"])*\\\"",L'\0', tbl);

		//res = __handle_expr(L"([^\\\\\\\"])",L'\0', tbl);
		res = __handle_expr(L"([^\\\\]|[^\\\"])*",L'\0', tbl);

		AR_printf(L"xres.next == %ls\n", res.next);
		AR_printf(L"err == %ls\n", res.err.pos);


		
		cat = LEX_CreateNode(LEX_CAT);
		tmp = LEX_CreateNode(LEX_FINAL);
		tmp->action.type = 20;
		LEX_InsertNodeToCat(&cat->cat, res.node);
		LEX_InsertNodeToCat(&cat->cat, tmp);
		
		

		LEX_CalcCClass(&cclass, cat);

		
		set = LEX_BuildLeafSet(cat, &cclass);
		
		
		dfa = LEX_BuildDFA(&cclass, set);
		
		LEX_BuildStateTable(&stat_tbl, dfa, &cclass);
		
		__print_dfatbl(&stat_tbl);


		LEX_UnInitCClass(&cclass);
		LEX_DestroyNode(res.node);
		LEX_DestroyNameList(tbl);
		LEX_DestroyLeafSet(set);
		LEX_DestroyDFA_ALL(dfa);

}


typedef struct __name_pair_tag
{
		const wchar_t *name;
		const wchar_t *expr;
}testNamePair_t;

testNamePair_t  name_expr[] = 
{
		{L"digit", L"[0-9]"},
		{L"number", L"0|[1-9]{digit}*"},
		{L"letter", L"[a-z_A-Z]"},
		{L"name",	L"{letter}({letter}|{digit})*"},
		{L"delim",	L"[\\n\\r\\t ]"},
		{L"EOI",	L"\\0"}
};


typedef struct __expr_action_tag
{
		const wchar_t *expr;
		lexAction_t		action;
}testExprAction_t;

testExprAction_t	expr_act[] = 
{
		{L"{delim}*", {0,0}},
		{L"{number}", {1,0}},
		{L"{name}", {2,0}},
		{L"{EOI}", {3,0}}
};

wchar_t buf[1024];

void lex_test()
{
		lex_t *lex;
		size_t i;
		lexToken_t tok;
		lexMatch_t match;

		lex = LEX_Create(NULL);
		LEX_InitMatch(&match, L"   345 abc ef\r\n 674  adfsd");

		for(i = 0; i < sizeof(name_expr) / sizeof(name_expr[0]); ++i)
		{
				LEX_InsertName(lex, name_expr[i].name, name_expr[i].expr);
		}

		
		for(i = 0; i < sizeof(expr_act) / sizeof(expr_act[0]); ++i)
		{

				LEX_InsertRule(lex, expr_act[i].expr, &expr_act[i].action);

		}
		
		LEX_GenerateTransTable(lex);
		
		while(LEX_Match(lex, &match, &tok))
		{
				AR_wcsncpy(buf, tok.str, tok.count);
				buf[tok.count] = L'\0';
				AR_printf(L"%ls:%d  (%d:%d)\n", buf, tok.type, tok.line, tok.col);

				if(tok.type == 3)break;
				
			//	getchar();
		}
		
		AR_ASSERT(match.is_ok);

		getchar();
		LEX_UnInitMatch(&match);
		LEX_Destroy(lex);
}

const wchar_t *pat[] = 
{
		L"delim = [ \\t\\n\\r]",
		L"digit = [0-9]",
		L"number = [1-9]{digit}*",
		L"EOI = \\0",
		
		
		L" 0,0 {EOI}",
		L"1,0 { delim}*",
		L"2,0 {number}",
};


const wchar_t *pat2[] = 
{
		L"delim = [ \\t\\n\\r]",
		L"strchr   = [^\\\"]|(\\\\\\\")",
		L"str   = \\\"{strchr}*\\\"",
		L"EOI = \\0",
		
		
		L" 0,0 {EOI}",
		L"1,0 { delim}*",
		L"2,0 { str }",
		
};


void lex_test2()
{
		lex_t *lex;
		lexToken_t tok;
		lexMatch_t match;
		size_t i;

		lex = LEX_Create(NULL);
		LEX_InitMatch(&match, L"  \"abc\\\"\"\"def\"  ");

		for(i = 0; i < sizeof(pat2)/sizeof(pat2[0]);  ++i)
		{
				if(!LEX_Insert(lex, pat2[i]))
				{
						AR_abort();
				}
		}

		LEX_GenerateTransTable(lex);

		while(LEX_Match(lex, &match, &tok))
		{
				wchar_t buf[1024];
				AR_wcsncpy(buf, tok.str, tok.count);
				buf[tok.count] = L'\0';

				AR_printf(L"%ls : type == %d : count == %d\r\n", buf, tok.type, tok.count);
				
				if(tok.type == 0)break;
		}

		getchar();

		LEX_UnInitMatch(&match);
}

void lex_test3()
{
		lex_t *lex;
//		lexToken_t tok;
//		lexMatch_t match;
//		size_t i;

		lex = LEX_Create(NULL);
		
		LEX_GenerateTransTable(lex);


}


typedef struct __lex_test4_ctx_tag
{
		wchar_t buf[1024];
		size_t	curr;
		size_t	len;
}lexTestCtx4_t;



wchar_t AR_STDCALL get_char(void *ctx)
{
		lexTestCtx4_t	*ctx4 = (lexTestCtx4_t*)ctx;
		
		return ctx4->buf[ctx4->curr];
}

void	AR_STDCALL skip_char(void *ctx)
{
		lexTestCtx4_t	*ctx4 = (lexTestCtx4_t*)ctx;
		ctx4->curr++;
}


void lex_test4()
{
#if(0)
		lex_t					*lex;
		lexToken_t				tok;
		lexMatchStream_t		match;
		size_t i;
		lexTestCtx4_t			ctx;
		memset(&ctx, 0, sizeof(ctx));
		wcscpy(ctx.buf,L" 3 4 4 \r\n5 5 6 6   ");




		lex = LEX_Create();
		LEX_InitMatchStream(&match, get_char, skip_char, (void*)&ctx);

		for(i = 0; i < sizeof(pat)/sizeof(pat[0]);  ++i)
		{
				if(!LEX_Insert(lex, pat[i]))
				{
						abort();
				}
		}

		LEX_GenerateTransTable(lex);

		while(LEX_MatchStream(lex, &match, &tok))
		{
				wchar_t buf[1024];
				AR_wcsncpy(buf, tok.str, tok.count);
				buf[tok.count] = L'\0';

				AR_printf(L"%ls : type == %d : count == %d line == %d col == %d\r\n", buf, tok.type, tok.count, tok.line, tok.col);
				
				if(tok.type == 0)break;
		}
		
		if(match.is_ok)
		{
				printf("success\r\n");
		}else
		{
				printf("failed\r\n");
		}

		getchar();

		LEX_UnInitMatchStream(&match);
#endif

}


extern const wchar_t*	__transform_char(const wchar_t *input, wchar_t *c, lexError_t *err);

void trans_char_test()
{
		const wchar_t *ret;
		wchar_t c = 0;
		lexError_t		err;

		ret = __transform_char(L"x{  4567   gef", &c, &err);

		wprintf(L"ret == %s : c == %x : err->pos == %s\r\n", ret,c, err.pos);

}

#endif


void lex_test20()
{
		lex_t *lex;
		lexToken_t tok;
		lexMatch_t match;
		size_t i;

		lex = LEX_Create(NULL);
		LEX_InitMatch(&match,L"aaa" );

		if(!LEX_Insert(lex, L"2,0 a{1,2}?"))
		{
				AR_abort();
		}


		if(!LEX_Insert(lex, L"0,1 [\\0]"))
		{
				AR_abort();
		}

		LEX_GenerateTransTable(lex);

		while(LEX_Match(lex, &match, &tok))
		{
				wchar_t buf[1024];
				AR_wcsncpy(buf, tok.str, tok.count);
				buf[tok.count] = L'\0';

				AR_printf(L"%ls : type == %d : count == %d\r\n", buf, tok.type, tok.count);
				
				if(tok.type == 0)break;
		}

		getchar();

		LEX_UnInitMatch(&match);
}


void lex_test()
{

		lex_test20();
}

AR_NAMESPACE_END



#endif
