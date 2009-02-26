#include "..\Lex\node.h"
#include "..\Lex\dfa.h"


#if(0)

extern const wchar_t*	__transform_char(const wchar_t *input, wchar_t *c, lexError_t *err);

AR_API void transchar_test()
{
		lexError_t	err; wchar_t c;

		wprintf(L"next == %s\n", __transform_char(L"u6bAabcde", &c, &err));

		wprintf(L"%x\n", c);
		printf("err == %d\n", err);

}

extern lexResult_t	__handle_quote(const wchar_t *input);

void showCatNode(lexNode_t *node)
{
		if(node == NULL)return;
		
		if(node->t == LEX_CHARSET)
		{
				wprintf(L"%c", node->charset.range->beg);
		}else
		{
				showCatNode(node->cat.left);
				showCatNode(node->cat.right);
		}
		
		
		
		



}

AR_API void quote_str_test()
{
		
		lexResult_t res = __handle_quote(L"\"abcdef|||abcdefg");
		wprintf(L"res.next == %s\n", res.next);
		printf("err == %d\n", res.err);

		showCatNode(res.node);printf("\n");

}


extern lexResult_t	__handle_cset_range(const wchar_t *input);

AR_API void cset_range_test()
{
		lexCharRange_t	*curr;

		lexResult_t res = __handle_cset_range(L"a-zwADW-Z]|abcdef");
		wprintf(L"res.next == %s\n", res.next);
		printf("err == %d\n", res.err);
		if(res.err == LEX_NO_ERROR)
		{
				for(curr = res.node->charset.range; curr; curr = curr->next)
				{
						wprintf(L"beg == %c : end == %c\n", curr->beg, curr->end);
				}
		}

}

extern lexResult_t	__handle_charset(const wchar_t *input);

AR_API void charset_test()
{
		lexCharRange_t	*curr;

		lexResult_t res = __handle_charset(L"[^a-cb-dA-F]|axxxx");
		wprintf(L"res.next == %s\n", res.next);
		printf("err == %d\n", res.err);
		
		res.node->charset.range = NULL;

		if(res.err == LEX_NO_ERROR)
		{
				for(curr = res.node->charset.range; curr; curr = curr->next)
				{
						wprintf(L"beg == %c : end == %c\n", curr->beg, curr->end);
				}
		}
		printf("------------------------------------------------------\n");
		if(res.err == LEX_NO_ERROR)
		{
				for(curr = res.node->charset.range; curr; curr = curr->next)
				{
						wprintf(L"beg == %d : end == %d\n", curr->beg, curr->end);
				}
		}
		printf("------------------------------------------------------\n");
		LEX_ReverseNegativeCharSet(&res.node->charset);

		for(curr = res.node->charset.range; curr; curr = curr->next)
		{
				wprintf(L"beg == %d : end == %d\n", curr->beg, curr->end);
		}

}


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
				wprintf(fmt, c);
				break;
		default:
				fmt = is_digit ? L"%u" : L"%c";
				wprintf(fmt, c);
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
						charsetShowChar(root->charset.range->beg, False);
				}else
				{
						wprintf(L"[");
						if(root->charset.is_neg) printf("^");
						for(curr = root->charset.range; curr; curr = curr->next)
						{
								if(curr->beg < curr->end)
								{
										charsetShowChar(curr->beg, False);
										printf("-");
										charsetShowChar(curr->end, False);
										//wprintf(L"%c-%c", curr->beg, curr->end);
								}else
								{
										charsetShowChar(curr->beg, False);
								}
						}
						wprintf(L"]");
				}
				break;
		case LEX_BRANCH:
				wprintf(L"(");
				for(i = 0; i < root->branch.count; ++i)
				{
						__in_lexnodeShow(root->branch.arr[i]);
						if(i < root->branch.count - 1)wprintf(L"|");
				}
				wprintf(L")");
				break;
		case LEX_CAT:
				wprintf(L"(");
				__in_lexnodeShow(root->cat.left);
				if(root->cat.right != NULL) __in_lexnodeShow(root->cat.right);
				
				wprintf(L")");
				break;
		case LEX_LOOP:
				wprintf(L"(");__in_lexnodeShow(root->loop);wprintf(L")");
				wprintf(L"*");
				break;
		case LEX_FINAL:
				printf("final");
				break;
		case LEX_CCLASS_ID:
				printf("%d ", root->cclass_id);
				break;
		default:
				charsetShowChar(L'#', False);
				break;
		}
}
static void LEX_ShowNode(LexNodeT *root)
{
		__in_lexnodeShow(root);
		printf("\n");
}


extern lexResult_t	__handle_postfix(lexNode_t *expr, const wchar_t *input);

AR_API void postfix_test()
{
		lexCharRange_t *curr;
		lexResult_t res = __handle_charset(L"abcdef");

		wprintf(L"res.next == %s\n", res.next);
		printf("err == %d\n", res.err);
		
		if(res.err == LEX_NO_ERROR)
		{
				for(curr = res.node->charset.range; curr; curr = curr->next)
				{
						wprintf(L"beg == %c : end == %c\n", curr->beg, curr->end);
				}
		}
		
		res = __handle_postfix(res.node, L"{1  , z}abcdef");

		wprintf(L"res.next == %s\n", res.next);
		printf("err == %d\n", res.err);


		if(res.err == LEX_NO_ERROR)
		{
				LEX_ShowNode(res.node);
		}

		

}

extern lexResult_t __handle_expr(const wchar_t *input, wchar_t tc, lexNameTable_t *tbl);
extern lexResult_t __handle_factor(const wchar_t *input,lexNameTable_t *tbl);


AR_API void factor_test()
{

		lexNameTable_t	tbl; lexResult_t res;

		LEX_InitNameTable(&tbl);
		
		AR_ASSERT(LEX_InsertNameToNameTable(&tbl, L"_name", L"a*") == LEX_NO_ERROR);

		res = __handle_factor(L"{       _name  }{0,}{1,4}bcdef", &tbl);

		wprintf(L"res.next == %s\n", res.next);
		printf("err == %d\n", res.err);


		if(res.err == LEX_NO_ERROR)
		{
				LEX_ShowNode(res.node);
		}

		LEX_DestroyNode(res.node);


}





AR_API void expr_test()
{
		lexNameTable_t	tbl; lexResult_t res;

		LEX_InitNameTable(&tbl);
		
		AR_ASSERT(LEX_InsertNameToNameTable(&tbl, L"_name", L"a*") == LEX_NO_ERROR);

		res = __handle_expr(L"{       _name  }{0,}{1,4}|(bcdef)|\"xxx\\\"\"+",L'\0', &tbl);

		wprintf(L"xres.next == %s\n", res.next);
		printf("err == %d\n", res.err);


		if(res.err == LEX_NO_ERROR)
		{
				LEX_ShowNode(res.node);
		}

		LEX_DestroyNode(res.node);
		LEX_UnInitNameTable(&tbl);
}




AR_API void cclass_test()
{
		size_t i;
		lexNameTable_t	tbl; lexResult_t res; lexCClass_t cclass;

		LEX_InitNameTable(&tbl);
		LEX_InitCClass(&cclass);
		
		AR_ASSERT(LEX_InsertNameToNameTable(&tbl, L"name1", L"a*") == LEX_NO_ERROR);
		AR_ASSERT(LEX_InsertNameToNameTable(&tbl, L"name2", L"[^a]") == LEX_NO_ERROR);

		res = __handle_expr(L"{ name1  }{0,}{name2}{1,3}|(bcdef)|\"xxx\\\"\"+",L'\0', &tbl);
		
		//res = __handle_expr(L"{ name1  }{0,}{name2}{1,3}", L'\0',&tbl);
		wprintf(L"xres.next == %s\n", res.next);
		printf("err == %d\n", res.err);


		if(res.err == LEX_NO_ERROR)
		{
				LEX_ShowNode(res.node);
		}

		
		LEX_CalcCClass(&cclass, res.node);

		for(i = 0; i < cclass.count; ++i)
		{
				printf("beg == %d : end == %d\n", cclass.ranges[i].beg, cclass.ranges[i].end);
		}


		LEX_DestroyNode(res.node);
		LEX_UnInitNameTable(&tbl);
		LEX_UnInitCClass(&cclass);

}

static void __printf_poset(lexPositionSet_t *set)
{
		size_t i;
		for(i = 0; i < set->count; ++i)
		{
				printf("%d ", set->pos[i]);
		}
}

static void __print_nodeset(lexNode_t *node, void *ctx)
{
		printf("node type == %d\n", node->t);
		printf("first_set: "); __printf_poset(&node->attr.first_set); printf("\n");
		printf("last_set: "); __printf_poset(&node->attr.last_set); printf("\n----------------\n");

}


AR_API void calc_attr_test()
{
		size_t i; 
		lexNameTable_t	tbl; lexResult_t res; lexCClass_t cclass; lexLeafSet_t leaf;

		LEX_InitNameTable(&tbl);
		LEX_InitCClass(&cclass);
		LEX_InitLeafSet(&leaf);
		
		//AR_ASSERT(LEX_InsertNameToNameTable(&tbl, L"name1", L"a") == LEX_NO_ERROR);
		//AR_ASSERT(LEX_InsertNameToNameTable(&tbl, L"name2", L"[^a]") == LEX_NO_ERROR);

		//res = __handle_expr(L"{ name1  }{0,}{name2}{1,3}|(bcdef)|\"xxx\\\"\"+",L'\0', &tbl);
		
		//res = __handle_expr(L"{ name1  }{name2}{name1}", L'\0',&tbl);

		res = __handle_expr(L"(a|b)*abb" , L'\0', &tbl);

		wprintf(L"xres.next == %s\n", res.next);
		printf("err == %d\n", res.err);
		
		{
				lexNode_t *tmp, *final;
				tmp = LEX_CreateNode(LEX_CAT);
				final = LEX_CreateNode(LEX_FINAL);
				LEX_InsertNodeToCat(&tmp->cat, res.node);
				LEX_InsertNodeToCat(&tmp->cat, final);
				res.node = tmp;
				

		}
		
		if(res.err == LEX_NO_ERROR)
		{
				LEX_ShowNode(res.node);
		}

		
		LEX_CalcCClass(&cclass, res.node);

		for(i = 0; i < cclass.count; ++i)
		{
				printf("beg == %d : end == %d\n", cclass.ranges[i].beg, cclass.ranges[i].end);
		}
		printf("-------------------------------------------------------\n");

		LEX_BuildLeafSet(res.node, &cclass, &leaf);

		
		LEX_ShowNode(res.node);
		
		/*for(i = 0; i < leaf.count; ++i)
		{
				lexNode_t *tmp = leaf.leaf[i];
				printf("leaf id == %d\n",tmp->attr.leaf_id);
				printf("classid == %d\n", tmp->cclass_id);
				printf("first set: ");  __printf_poset(&tmp->attr.first_set); printf("\n");
				printf("----\n");
		}*/
		
		for(i = 0; i < leaf.count; ++i)
		{
				lexNode_t *tmp = leaf.leaf[i];
				printf("leaf id == %d\n",tmp->attr.leaf_id);
				//printf("classid == %d\n", tmp->cclass_id);
				printf("follow set: ");  __printf_poset(&leaf.follow_set[tmp->attr.leaf_id]); printf("\n");
				printf("----\n");
		}
		
		
		
		//LEX_TraverseNode(res.node, __print_nodeset, NULL, True);
		
		LEX_DestroyNode(res.node);
		LEX_UnInitNameTable(&tbl);
		LEX_UnInitCClass(&cclass);		
		LEX_UnInitLeafSet(&leaf);
}

static void __print_dfaname(lexPositionSet_t *pos)
{
		size_t i;
		for(i = 0; i < pos->count; ++i)printf("%d ", pos->pos[i]);


}

static void __print_vtx(lexDFAVertex_t *start)
{
		size_t i;
		if(start->is_final)
		{
				printf("final dfa name :");
		}else
		{
				printf("dfa name : ");
		}

		 __print_dfaname(&start->pos_set); printf("\n");
		
		
		printf("out to:\n");

		for(i = 0; i < start->out_count; ++i)
		{
				printf("out arc ccid == %d ", start->out[i].match_id);
				printf("state name == "); __print_dfaname(&start->out[i].to->pos_set); printf("\n");
				
		}

		if(start->is_final)printf("final action count == %d\n", start->act_count);

		printf("\n");

}

static void __print_dfa(lexDFAVertex_t *start)
{
		size_t i;
		lexDFAVertexSet_t set;

		LEX_InitDFAVertexSet(&set);

		LEX_CollectDFAVertex(&set, start);

		for(i = 0; i < set.count; ++i)
		{		
				__print_vtx(set.set[i]);
		}
		printf("----------------------------------\n");

		LEX_UnInitDFAVertexSet(&set);
}


AR_API void dfa_basic_test()
{
		lexDFAVertex_t *root, *tmp1, *tmp2;

		root = LEX_CreateDFAVertex();
		
		 LEX_InsertPosToPositionSet(&root->pos_set, 0);
		LEX_InsertPosToPositionSet(&root->pos_set, 1);
		LEX_InsertPosToPositionSet(&root->pos_set, 3);
		LEX_InsertPosToPositionSet(&root->pos_set, 5);
		
		__print_dfa(root);
		

		tmp1 = LEX_CreateDFAVertex();

		LEX_InsertPosToPositionSet(&tmp1->pos_set, 2);
		LEX_InsertPosToPositionSet(&tmp1->pos_set, 4);
		LEX_ConnectDFAVertex(root, tmp1, 0);

		tmp2 = LEX_CreateDFAVertex();
		LEX_InsertPosToPositionSet(&tmp2->pos_set, 6);
		LEX_InsertPosToPositionSet(&tmp2->pos_set, 8);
		LEX_ConnectDFAVertex(root, tmp2, 1);

		__print_dfa(root);

		LEX_ConnectDFAVertex(tmp1, tmp2, 2);
		LEX_ConnectDFAVertex(tmp2, root, 3);
		tmp2->is_final = True;
		__print_dfa(root);

		LEX_DestroyDFA(root);
}


AR_API void dfa_build_test()
{
		size_t i; 
		lexNameTable_t	tbl; lexResult_t res; lexCClass_t cclass; lexLeafSet_t leaf;
		
		lexDFAVertex_t *dfa;

		LEX_InitNameTable(&tbl);
		LEX_InitCClass(&cclass);
		LEX_InitLeafSet(&leaf);
		
		res = __handle_expr(L"(a|b)*abb" , L'\0', &tbl);
		//res = __handle_expr(L"a" , L'\0', &tbl);
		wprintf(L"xres.next == %s\n", res.next);
		printf("err == %d\n", res.err);
		
		{
				lexNode_t *tmp, *final;
				tmp = LEX_CreateNode(LEX_CAT);
				final = LEX_CreateNode(LEX_FINAL);
				LEX_InsertNodeToCat(&tmp->cat, res.node);
				LEX_InsertNodeToCat(&tmp->cat, final);
				res.node = tmp;
				

		}
		
		if(res.err == LEX_NO_ERROR)
		{
				LEX_ShowNode(res.node);
		}

		
		LEX_CalcCClass(&cclass, res.node);

		for(i = 0; i < cclass.count; ++i)
		{
				printf("beg == %d : end == %d\n", cclass.ranges[i].beg, cclass.ranges[i].end);
		}
		printf("-------------------------------------------------------\n");

		LEX_BuildLeafSet(res.node, &cclass, &leaf);
		
		printf("leaf.root_first count == %d\n", leaf.root_first.count);
		getchar();
		
		LEX_ShowNode(res.node);
				
		dfa = LEX_BuildDFA(&cclass, &leaf);

		__print_dfa(dfa);
		
		getchar();
		LEX_DestroyDFA(dfa);

		LEX_DestroyNode(res.node);
		LEX_UnInitNameTable(&tbl);
		LEX_UnInitCClass(&cclass);		
		LEX_UnInitLeafSet(&leaf);

}

extern void LEX_BuildStateTable(lexStateTable_t *tbl, lexDFAVertex_t *dfa, const lexCClass_t *cclass);

static void __print_lextable(lexStateTable_t *tbl)
{
		size_t i,j;
		for(i = 0;i < tbl->row; ++i)
		{
				for(j = 0; j < tbl->col; ++j)
				{
						printf("%d ", tbl->tbl[i*tbl->col + j]);
				}
				printf("\n");
		}
}

AR_API void dfa_tbl_test()
{
		size_t i; 
		lexNameTable_t	tbl; lexResult_t res; lexCClass_t cclass; lexLeafSet_t leaf;
		
		lexDFAVertex_t *dfa;

		LEX_InitNameTable(&tbl);
		LEX_InitCClass(&cclass);
		LEX_InitLeafSet(&leaf);
		
		res = __handle_expr(L"(a|b)*abbc" , L'\0', &tbl);
		//res = __handle_expr(L"a" , L'\0', &tbl);
		wprintf(L"xres.next == %s\n", res.next);
		printf("err == %d\n", res.err);
		
		{
				lexNode_t *tmp, *final;
				tmp = LEX_CreateNode(LEX_CAT);
				final = LEX_CreateNode(LEX_FINAL);
				LEX_InsertNodeToCat(&tmp->cat, res.node);
				LEX_InsertNodeToCat(&tmp->cat, final);
				res.node = tmp;
				

		}
		
		if(res.err == LEX_NO_ERROR)
		{
				LEX_ShowNode(res.node);
		}

		
		LEX_CalcCClass(&cclass, res.node);

		for(i = 0; i < cclass.count; ++i)
		{
				printf("beg == %d : end == %d\n", cclass.ranges[i].beg, cclass.ranges[i].end);
		}
		printf("-------------------------------------------------------\n");

		LEX_BuildLeafSet(res.node, &cclass, &leaf);
		dfa = LEX_BuildDFA(&cclass, &leaf);

		__print_dfa(dfa);
		
		getchar();
		
		{
				lexStateTable_t tbl;
				LEX_InitStateTable(&tbl);
				
				LEX_BuildStateTable(&tbl, dfa, &cclass);

				printf("tbl->row = %d: col = %d\n", tbl.row, tbl.col);

				__print_lextable(&tbl);



				LEX_UnInitStateTable(&tbl);



		}


		LEX_DestroyDFA(dfa);
		LEX_DestroyNode(res.node);
		LEX_UnInitNameTable(&tbl);
		LEX_UnInitCClass(&cclass);		
		LEX_UnInitLeafSet(&leaf);






}



/*
%{
delim	=	[\\r\\n\\t ]+
letter	=	[A-Za-z_]
digit   =	[0-9]
hexdig	=	[0-9A-Fa-f]
octdig	=	[0-7]
dec		=	"-"?{digit}+
hex		=	"-"?0x{hexdig} +
oct		=	"-"?0{octdig}+
float	=	"-"{digit}+"."{digit}+

id		=	{letter}({lettern}|{digit})*

%}


%{

{delim}


%}
*/


#if(0)
const wchar_t* __parse_pattern(lexNode_t *uniroot, lexNameTable_t *tbl, const wchar_t *pattern, lexError_t *res_err);

AR_API void parse_pattern_test()
{
		
		size_t i; 
		lexNameTable_t	tbl; lexResult_t res; lexCClass_t cclass; lexLeafSet_t leaf;
		lexNode_t *root;
		lexDFAVertex_t *dfa;

		LEX_InitNameTable(&tbl);
		LEX_InitCClass(&cclass);
		LEX_InitLeafSet(&leaf);
		root = LEX_CreateNode(LEX_BRANCH);

		

		{
				const wchar_t *pattern = L" name1   =  [abcd]+\r\n(3,5) [\\r\\n\\t]+\n   (7,8)[A-Za-z_] \r\n   ";
				const wchar_t *next = pattern;
		
				while(next != NULL && *next != L'\0')
				{
						lexError_t err;
						next = __parse_pattern(root, &tbl,next, &err);

						printf("err = %d\n", err);
						wprintf(L"next == %s\n", next);
						getchar();
				}
		
		
		
		}

}




AR_API void dfa_exec_test()
{

		size_t i; 
		lexNameTable_t	tbl; lexResult_t res; lexCClass_t cclass; lexLeafSet_t leaf;
		
		lexDFAVertex_t *dfa;

		LEX_InitNameTable(&tbl);
		LEX_InitCClass(&cclass);
		LEX_InitLeafSet(&leaf);
		
		//res = __handle_expr(L"(a|b)*abb" , L'\0', &tbl);
		res = __handle_expr(L"\\/\\*([^\\\\\\*]|\\*+[^\\*\\/])*\\*+\\/" , L'\0', &tbl);
		//res = __handle_expr(L"a" , L'\0', &tbl);
		wprintf(L"xres.next == %s\n", res.next);
		printf("err == %d\n", res.err);
		
		{
				lexNode_t *tmp, *final;
				tmp = LEX_CreateNode(LEX_CAT);
				final = LEX_CreateNode(LEX_FINAL);
				final->action.type = 8888;

				LEX_InsertNodeToCat(&tmp->cat, res.node);
				LEX_InsertNodeToCat(&tmp->cat, final);
				res.node = tmp;
				

		}
		
		if(res.err == LEX_NO_ERROR)
		{
				LEX_ShowNode(res.node);
		}

		
		LEX_CalcCClass(&cclass, res.node);

		for(i = 0; i < cclass.count; ++i)
		{
				printf("beg == %d : end == %d\n", cclass.ranges[i].beg, cclass.ranges[i].end);
		}
		printf("-------------------------------------------------------\n");

		LEX_BuildLeafSet(res.node, &cclass, &leaf);
		
		printf("leaf.root_first count == %d\n", leaf.root_first.count);
		getchar();
		
		LEX_ShowNode(res.node);
				
		dfa = LEX_BuildDFA(&cclass, &leaf);

		__print_dfa(dfa);
		
		getchar();


/**********************************************************************************************/


		{
				lexDFAVertex_t *last;
				lexDFAVertex_t *curr = dfa;
		
				last = NULL;

				while(curr != NULL)
				{
						
						int id;
						wchar_t c;

						if(curr->is_final)
						{
								last = curr;
								printf("final\n");
						}

						printf("input: \n");
						
						while(True)
						{
								c = getwchar();
								if(c != L'\n')break;
						}

						wprintf(L"char c == %c\n", c);
						
						id = LEX_FindIndexFromCClass(&cclass, c);

						if(id == -1)break;

						curr = LEX_GetNextVertex(curr, (size_t)id);
				}

				if(last != NULL)
				{
						printf("%d\n", last->action[0].type);
				}else
				{
						printf("lex error\n");
						
				}
				getchar();
				getchar();
		}




/**********************************************************************************************/

		LEX_DestroyDFA(dfa);

		LEX_DestroyNode(res.node);
		LEX_UnInitNameTable(&tbl);
		LEX_UnInitCClass(&cclass);		
		LEX_UnInitLeafSet(&leaf);

}

#endif

AR_API void lex_test()
{
		lex_t  *lex; lexMatch_t match;

const wchar_t *p = 
L"name = \\/\\*([^\\\\\\*]|\\*+[^\\*\\/])*\\*+\\/\r\n\
(  13 , 31 )   {name}\r\n\
";
		lexError_t err;
		const wchar_t *input = L"/*aaaaa\r\nabsscdef\n*//*abc*//*abc*/";
		
		LEX_InitMatch(&match, input);

		lex = LEX_CreateLex();
		err = LEX_Build(lex, p);
		printf("err = %d\n", err);
		if(err != LEX_NO_ERROR)
		{
				abort();
		}
		
		
		if(LEX_Match(lex, &match))
		{
				wprintf(L"match == %d, %d,%d, %d\n", match.token.count, match.token.type, match.token.x, match.token.y);
				wprintf(L"next == %s\n", match.next);

		}

		if(LEX_Match(lex, &match))
		{
				wprintf(L"match == %d, %d,%d, %d\n", match.token.count, match.token.type, match.token.x, match.token.y);
				wprintf(L"next == %s\n", match.next);

		}

		if(LEX_Match(lex, &match))
		{
				wprintf(L"match == %d, %d,%d, %d\n", match.token.count, match.token.type, match.token.x, match.token.y);
				wprintf(L"next == %s\n", match.next);
		}

		if(!LEX_Match(lex, &match))
		{
				printf("match.err == %d\n", match.err);
		}

		LEX_Destroy(lex);
}




#endif
