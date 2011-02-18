#include "test.h"
#include "../Arsenal/Lex/lex.h"
#include "../Arsenal/Lex/rgx.h"



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
		case Lex_CHARSET:
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
		case Lex_BRANCH:
				AR_printf(L"(");
				for(i = 0; i < root->branch.count; ++i)
				{
						__in_lexnodeShow(root->branch.arr[i]);
						if(i < root->branch.count - 1)AR_printf(L"|");
				}
				AR_printf(L")");
				break;
		case Lex_CAT:
				AR_printf(L"(");
				__in_lexnodeShow(root->cat.left);
				if(root->cat.right != NULL) __in_lexnodeShow(root->cat.right);
				
				AR_printf(L")");
				break;
		case Lex_LOOP:
				AR_printf(L"(");__in_lexnodeShow(root->loop);AR_printf(L")");
				AR_printf(L"*");
				break;
		case Lex_FINAL:
				printf("final");
				break;
		case Lex_CCLASS_ID:
				printf("%d ", root->cclass_id);
				break;
		default:
				charsetShowChar(L'#', false);
				break;
		}
}
static void Lex_ShowNode(LexNodeT *root)
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
		//AR_ASSERT(Lex_InsertNameToNameTable(&tbl, L"_name", L"a*") == Lex_NO_ERROR);
		tbl = NULL;
		Lex_InitCClass(&cclass);
		Lex_InitStateTable(&stat_tbl);
		//res = __handle_expr(L"(abc){0,}{1,4}|(b)|\"xxx\\\"\"+",L'\0', tbl);
		
		//res = __handle_expr(L"\\\"([^\\\\\\\"])*\\\"",L'\0', tbl);
		//res = __handle_expr(L"\\\"([^\\\\]|[^\\\"])*\\\"",L'\0', tbl);

		//res = __handle_expr(L"([^\\\\\\\"])",L'\0', tbl);
		res = __handle_expr(L"([^\\\\]|[^\\\"])*",L'\0', tbl);

		AR_printf(L"xres.next == %ls\n", res.next);
		AR_printf(L"err == %ls\n", res.err.pos);


		
		cat = Lex_CreateNode(Lex_CAT);
		tmp = Lex_CreateNode(Lex_FINAL);
		tmp->action.type = 20;
		Lex_InsertNodeToCat(&cat->cat, res.node);
		Lex_InsertNodeToCat(&cat->cat, tmp);
		
		

		Lex_CalcCClass(&cclass, cat);

		
		set = Lex_BuildLeafSet(cat, &cclass);
		
		
		dfa = Lex_BuildDFA(&cclass, set);
		
		Lex_BuildStateTable(&stat_tbl, dfa, &cclass);
		
		__print_dfatbl(&stat_tbl);


		Lex_UnInitCClass(&cclass);
		Lex_DestroyNode(res.node);
		Lex_DestroyNameList(tbl);
		Lex_DestroyLeafSet(set);
		Lex_DestroyDFA_ALL(dfa);

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

		lex = Lex_Create(NULL);
		Lex_InitMatch(&match, L"   345 abc ef\r\n 674  adfsd");

		for(i = 0; i < sizeof(name_expr) / sizeof(name_expr[0]); ++i)
		{
				Lex_InsertName(lex, name_expr[i].name, name_expr[i].expr);
		}

		
		for(i = 0; i < sizeof(expr_act) / sizeof(expr_act[0]); ++i)
		{

				Lex_InsertRule(lex, expr_act[i].expr, &expr_act[i].action);

		}
		
		Lex_GenerateTransTable(lex);
		
		while(Lex_Match(lex, &match, &tok))
		{
				AR_wcsncpy(buf, tok.str, tok.count);
				buf[tok.count] = L'\0';
				AR_printf(L"%ls:%d  (%d:%d)\n", buf, tok.type, tok.line, tok.col);

				if(tok.type == 3)break;
				
			//	getchar();
		}
		
		AR_ASSERT(match.is_ok);

		getchar();
		Lex_UnInitMatch(&match);
		Lex_Destroy(lex);
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

		lex = Lex_Create(NULL);
		Lex_InitMatch(&match, L"  \"abc\\\"\"\"def\"  ");

		for(i = 0; i < sizeof(pat2)/sizeof(pat2[0]);  ++i)
		{
				if(!Lex_Insert(lex, pat2[i]))
				{
						AR_abort();
				}
		}

		Lex_GenerateTransTable(lex);

		while(Lex_Match(lex, &match, &tok))
		{
				wchar_t buf[1024];
				AR_wcsncpy(buf, tok.str, tok.count);
				buf[tok.count] = L'\0';

				AR_printf(L"%ls : type == %d : count == %d\r\n", buf, tok.type, tok.count);
				
				if(tok.type == 0)break;
		}

		getchar();

		Lex_UnInitMatch(&match);
}

void lex_test3()
{
		lex_t *lex;
//		lexToken_t tok;
//		lexMatch_t match;
//		size_t i;

		lex = Lex_Create(NULL);
		
		Lex_GenerateTransTable(lex);


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




		lex = Lex_Create();
		Lex_InitMatchStream(&match, get_char, skip_char, (void*)&ctx);

		for(i = 0; i < sizeof(pat)/sizeof(pat[0]);  ++i)
		{
				if(!Lex_Insert(lex, pat[i]))
				{
						abort();
				}
		}

		Lex_GenerateTransTable(lex);

		while(Lex_MatchStream(lex, &match, &tok))
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

		Lex_UnInitMatchStream(&match);
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


void rgx_compile_test(rgxNode_t *node)
{
		rgxProg_t		prog;
		arString_t		*out;
		AR_ASSERT(node != NULL);
		
		out = AR_CreateString();
		RGX_InitProg(&prog);

		RGX_Compile(&prog, node);
		
		RGX_ProgToString(&prog, out);

		AR_printf(L"%ls\r\n", AR_GetStrString(out));


		AR_DestroyString(out);
		RGX_UnInitProg(&prog);
}



void rgx_test_loop()
{
		rgxNameSet_t	set;
		rgxResult_t		result;
		arString_t		*out;

		RGX_InitNameSet(&set);
		out = AR_CreateString();
		
		while(true)
		{
				wchar_t buf[1024];
				_getws(buf);

				if(AR_wcscmp(buf, L"quit") == 0)break;

				result = RGX_ParseExpr(buf, &set);

				if(result.node)
				{
						rgxNode_t *cat, *final;
						
						final = RGX_CreateNode(RGX_FINAL_T);
						final->final_val = 21;
						cat = RGX_CreateNode(RGX_CAT_T);
						
						RGX_InsertToNode(cat, result.node);
						RGX_InsertToNode(cat, final);
						result.node = NULL;

						AR_ClearString(out);
						RGX_ToString(cat, out);
						AR_printf(L"%ls\r\n", AR_GetStrString(out));

						rgx_compile_test(cat);

						RGX_DestroyNode(cat);
				}else
				{
						AR_printf(L"error position == %ls\r\n", result.err.pos);
				}
		}

		

		AR_DestroyString(out);
		RGX_UnInitNameSet(&set);

}






void lex_test_loop()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 0;
		act.value = 200;
		AR_ASSERT(Lex_InsertRule(lex, L"(a{3,5}?){3,5}?", &act));

		act.value = 1;
		AR_ASSERT(Lex_InsertRule(lex, L"[ \\r\\n\\t]", &act));
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));

		match = Lex_CreateMatch(lex);

		//Lex_ResetInput(match, L"aaa\r\naaa\r\naaaaa\r\n");
		Lex_ResetInput(match, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaa\r\n");
		

		lexToken_t tok;
		bool_t is_ok = false;
		while(Lex_Match(match, &tok))
		{
				if(tok.value == 0)
				{
						is_ok = true;
						break;
				}
				wchar_t *s = AR_wcsndup(tok.str, tok.count);
				AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
				AR_DEL(s);
		}

		if(!is_ok)
		{
				AR_printf(L"%ls\r\n", L"failed\r\n");
		}else
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);

}



void lex_test_loop2()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 0;
		act.value = 200;
		AR_ASSERT(Lex_InsertRule(lex, L"a{3,5}?(?=(a){3,5})", &act));

		act.value = 1;
		AR_ASSERT(Lex_InsertRule(lex, L"[ \\r\\n\\t]", &act));
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));

		match = Lex_CreateMatch(lex);

		Lex_ResetInput(match, L"aaaaaa");
		

		lexToken_t tok;
		bool_t is_ok = false;
		while(Lex_Match(match, &tok))
		{
				if(tok.value == 0)
				{
						is_ok = true;
						break;
				}
				wchar_t *s = AR_wcsndup(tok.str, tok.count);
				AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
				AR_DEL(s);
		}

		if(!is_ok)
		{
				AR_printf(L"%ls\r\n", L"failed\r\n");
		}else
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);

}





void lex_chinese_char()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 0;
		act.value = 200;
		AR_ASSERT(Lex_InsertRule(lex, L"[a-z_A-Z\\u{	19968   }-\\u{   40869	}]+", &act));
		
		act.value = 1;
		AR_ASSERT(Lex_InsertRule(lex, L"[ \\r\\n\\t]", &act));
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));

		match = Lex_CreateMatch(lex);

		//Lex_ResetInput(match, L"aaa\r\naaa\r\naaaaa\r\n");
		Lex_ResetInput(match, L"中国字abc真麻烦\r\n");
		

		lexToken_t tok;
		bool_t is_ok = false;
		while(Lex_Match(match, &tok))
		{
				if(tok.value == 0)
				{
						is_ok = true;
						break;
				}
				wchar_t *s = AR_wcsndup(tok.str, tok.count);
				AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
				AR_DEL(s);
		}

		if(!is_ok)
		{
				AR_printf(L"%ls\r\n", L"failed\r\n");
		}else
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);

}




void lex_line_test()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 0;
		act.value = 200;
		AR_ASSERT(Lex_InsertRule(lex, L"\\B[ \\t]*;[^\\r\\n]*\\E", &act));

		act.value = 201;
		AR_ASSERT(Lex_InsertRule(lex, L"[\\r\\n]", &act));
		

		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));

		match = Lex_CreateMatch(lex);
		
		//Lex_ResetInput(match, L";中国\r;字abc\n;真麻烦\r\n;太烦了\r\n");
		Lex_ResetInput(match, L";中国\n\r;字abc\r\n");
		

		lexToken_t tok;
		bool_t is_ok = false;
		while(Lex_Match(match, &tok))
		{
				if(tok.value == 0)
				{
						is_ok = true;
						break;
				}
				wchar_t *s = AR_wcsndup(tok.str, tok.count);
				AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
				AR_DEL(s);
		}

		if(!is_ok)
		{
				AR_printf(L"%ls\r\n", L"failed\r\n");
		}else
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);
}


void lex_line_test2()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 0;
		act.value = 200;
		AR_ASSERT(Lex_InsertRule(lex, L"(\\r\\n)|\\r|\\n", &act));
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));

		match = Lex_CreateMatch(lex);
		
		Lex_ResetInput(match, L"\r\n\n\r");
		

		lexToken_t tok;
		bool_t is_ok = false;
		
		printf("\r\n");

		while(Lex_Match(match, &tok))
		{
				if(tok.value == 0)
				{
						is_ok = true;
						break;
				}
				wchar_t *s = AR_wcsndup(tok.str, tok.count);
				AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
				AR_DEL(s);
		}

		if(!is_ok)
		{
				AR_printf(L"%ls\r\n", L"failed\r\n");
		}else
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);

}




void lex_line_test3()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 1;
		act.value = 200;
		AR_ASSERT(Lex_InsertRule(lex, L"\\r\\n", &act));


		act.priority = 1;
		act.value = 201;
		AR_ASSERT(Lex_InsertRule(lex, L"\\n\\r", &act));


		act.priority = 0;
		act.value = 301;
		AR_ASSERT(Lex_InsertRule(lex, L"\\r", &act));


		act.priority = 0;
		act.value = 302;
		AR_ASSERT(Lex_InsertRule(lex, L"\\n", &act));
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));

		match = Lex_CreateMatch(lex);
		
		Lex_ResetInput(match, L"\n\r\r\n");
		

		lexToken_t tok;
		bool_t is_ok = false;
		
		printf("\r\n");

		while(Lex_Match(match, &tok))
		{
				wchar_t *s = AR_wcsndup(tok.str, tok.count);
				AR_printf(L"%ls : row == %d : col == %d : count == %d\r\n", s, tok.line, tok.col, tok.count);
				AR_DEL(s);
				
				if(tok.value == 0)
				{
						is_ok = true;
						break;
				}
		}

		if(!is_ok)
		{
				AR_printf(L"%ls\r\n", L"failed\r\n");
		}else
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);

}








void lex_line_test4()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 1;
		act.value = 200;
		AR_ASSERT(Lex_InsertRule(lex, L"[\\r\\n]{1,2}", &act));


		
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));

		match = Lex_CreateMatch(lex);
		
		Lex_ResetInput(match, L"\n\r\r\r\n\n");
		

		lexToken_t tok;
		bool_t is_ok = false;
		
		printf("\r\n");

		while(Lex_Match(match, &tok))
		{
				wchar_t *s = AR_wcsndup(tok.str, tok.count);
				AR_printf(L"%ls : row == %d : col == %d : count == %d\r\n", s, tok.line, tok.col, tok.count);
				AR_DEL(s);
				
				if(tok.value == 0)
				{
						is_ok = true;
						break;
				}
		}

		if(!is_ok)
		{
				AR_printf(L"%ls\r\n", L"failed\r\n");
		}else
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);

}


void lex_test()
{
		
		//lex_line_test();
		//lex_line_test2();
		//lex_line_test3();
		lex_line_test4();
		//lex_chinese_char();
		//lex_test_loop2();

		//rgx_test_loop();
		
		//lex_test20();
}

AR_NAMESPACE_END



#endif
