#include "test.h"
#include "../Arsenal/Lex/lex.h"
#include "../Arsenal/Lex/rgx.h"



#if defined(__LIB)


AR_NAMESPACE_BEGIN


#if(1)

void rgx_compile_test(rgxNode_t *node)
{
		rgxProg_t		prog;
		arString_t		*out;
		AR_ASSERT(node != NULL);
		
		out = AR_CreateString();
		

		RGX_InitProg(&prog);


		if(RGX_Compile(&prog, node) == AR_S_YES)
		{
				RGX_ProgToString(&prog, out);
				AR_printf(L"%ls\r\n", AR_GetStringCString(out));
		}
		

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

				if(result.err.status == AR_S_YES)
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
						AR_printf(L"%ls\r\n", AR_GetStringCString(out));

						rgx_compile_test(cat);

						RGX_DestroyNode(cat);
				}else
				{
						//AR_printf(L"error  code == %d, position == %ls\r\n", result.err.status.m_code, result.err.pos);
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
		AR_ASSERT(Lex_InsertRule(lex, L"a{3,5}", &act) == AR_S_YES);

		act.value = 1;
		AR_ASSERT(Lex_InsertRule(lex, L"[ \\r\\n\\t]", &act)  == AR_S_YES);
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act)  == AR_S_YES );

		match = Lex_CreateMatch(lex);

		Lex_ResetInput(match, L"aaa\r\naaa\r\naaaaa\r\naa");
		//Lex_ResetInput(match, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaa\r\n");
		

		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
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
		AR_ASSERT(Lex_InsertRule(lex, L"a{3,5}?(?=(a){3,5})", &act) == AR_S_YES);

		act.value = 1;
		AR_ASSERT(Lex_InsertRule(lex, L"[ \\r\\n\\t]", &act) == AR_S_YES);
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act) == AR_S_YES);

		match = Lex_CreateMatch(lex);

		Lex_ResetInput(match, L"aaaaaa");
		

		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
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
		AR_ASSERT(Lex_InsertRule(lex, L"[a-z_A-Z\\u{	19968   }-\\u{   40869	}]+", &act) == AR_S_YES);
		
		act.value = 1;
		AR_ASSERT(Lex_InsertRule(lex, L"[ \\r\\n\\t]", &act) == AR_S_YES);
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act) == AR_S_YES);

		match = Lex_CreateMatch(lex);

		//Lex_ResetInput(match, L"aaa\r\naaa\r\naaaaa\r\n");
		Lex_ResetInput(match, L"中国字abc真麻烦\r\n");
		

		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
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
		AR_ASSERT(Lex_InsertRule(lex, L"\\B[ \\t]*;[^\\r\\n]*\\E", &act) == AR_S_YES);

		act.value = 201;
		AR_ASSERT(Lex_InsertRule(lex, L"[\\r\\n]", &act)== AR_S_YES);
		

		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act)== AR_S_YES);

		match = Lex_CreateMatch(lex);
		
		//Lex_ResetInput(match, L";中国\r;字abc\n;真麻烦\r\n;太烦了\r\n");
		Lex_ResetInput(match, L";中国\n\r;字abc\r\n");
		

		
		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d action == %Iu\r\n", s, tok.line, tok.col, tok.value);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
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
		AR_ASSERT(Lex_InsertRule(lex, L"(\\r\\n)|\\r|\\n", &act) == AR_S_YES);
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act)  == AR_S_YES );

		match = Lex_CreateMatch(lex);
		
		Lex_ResetInput(match, L"\r\n\n\r");
		

		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d action == %Iu\r\n", s, tok.line, tok.col, tok.value);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
		}

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
		AR_ASSERT(Lex_InsertRule(lex, L"\\r\\n", &act) == AR_S_YES);


		act.priority = 1;
		act.value = 201;
		AR_ASSERT(Lex_InsertRule(lex, L"\\n\\r", &act) == AR_S_YES);


		act.priority = 0;
		act.value = 301;
		AR_ASSERT(Lex_InsertRule(lex, L"\\r", &act) == AR_S_YES);


		act.priority = 0;
		act.value = 302;
		AR_ASSERT(Lex_InsertRule(lex, L"\\n", &act) == AR_S_YES);
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act) == AR_S_YES);

		match = Lex_CreateMatch(lex);
		
		//Lex_ResetInput(match, L"\n\r\r\n");
		Lex_ResetInput(match, L"\n");

		
		printf("\r\n");

		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d action == %Iu\r\n", s, tok.line, tok.col, tok.value);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
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
		AR_ASSERT(Lex_InsertRule(lex, L"[\\r\\n]{1,2}", &act) == AR_S_YES);


		
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act) == AR_S_YES);

		match = Lex_CreateMatch(lex);
		
		Lex_ResetInput(match, L"\n\r\r\r\n\n");
		

		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d action == %Iu\r\n", s, tok.line, tok.col, tok.value);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);

}




void lex_quote_test7()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 1;
		act.value = 200;

		

		AR_ASSERT(Lex_InsertRule(lex, L"\"abc\"", &act) == AR_S_YES);

		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act) == AR_S_YES);
		
		match = Lex_CreateMatch(lex);
		
		Lex_ResetInput(match, L"abcabcabcabc");
		

		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d action == %Iu\r\n", s, tok.line, tok.col, tok.value);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);
}


void lex_test_skip_line_test()
{
		//const wchar_t *s = L"a\r\nb\n\rc\rd\ne\r\rf";

		const wchar_t *s = L"a\n\rb";

		lexMatch_t match;

		AR_memset(&match, 0, sizeof(match));


		match.next = s;
		match.input = s;

		while(*match.next != L'\0')
		{
				printf("line : %I64d col : %I64d\r\n", match.line, match.col);
				Lex_SkipN(&match, 1);
		}

}





void lex_line_num_test()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 1;
		act.value = 200;


		AR_ASSERT(Lex_InsertName(lex, L"keyword_lhd", L"[A-Z_a-z0-9]") == AR_S_YES);
		AR_ASSERT(Lex_InsertName(lex, L"hex_digit", L"[0-9a-fA-F]") == AR_S_YES);
		AR_ASSERT(Lex_InsertName(lex, L"hex_literal", L"0(x|X){hex_digit}+") == AR_S_YES);
		AR_ASSERT(Lex_InsertName(lex, L"oct_literal", L"0[0-7]+") == AR_S_YES);
		AR_ASSERT(Lex_InsertName(lex, L"dec_literal", L"(0|[1-9][0-9]*)") == AR_S_YES);


		AR_ASSERT(Lex_InsertName(lex, L"hex_constant", L"{hex_literal}(?!{keyword_lhd})") == AR_S_YES);
		AR_ASSERT(Lex_InsertName(lex, L"oct_constant", L"{oct_literal}(?!{keyword_lhd})") == AR_S_YES);
		AR_ASSERT(Lex_InsertName(lex, L"dec_constant", L"{dec_literal}(?!{keyword_lhd})") == AR_S_YES);



		act.priority = 1;
		act.value = 261;
		
		AR_ASSERT( Lex_InsertRule(lex, L"{hex_constant}|{oct_constant}|{dec_constant}", &act) == AR_S_YES  );

		act.priority = 0;
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act) == AR_S_YES);


		match = Lex_CreateMatch(lex);
		
		Lex_ResetInput(match, L"333");

		
		printf("\r\n");

		lexToken_t tok;
		arStatus_t lex_status;
		
		

		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d action == %Iu\r\n", s, tok.line, tok.col, tok.value);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);

}
#endif





#if(0)

void lex_line_test7()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 0;
		act.value = 200;
		
		AR_ASSERT(Lex_InsertRule(lex, L".*(\\r\\n|\\n\\r|\\r|\\n)?", &act));
		
		act.priority = 1;
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));
		
		match = Lex_CreateMatch(lex);
		
		Lex_ResetInput(match, L"\r\n\r\n\r\n%token\tline\t:\t\".*(\\r\\n|\\n\\r|\\r|\\n)?\"\r\n\t\t;\r\n\r\n\r\n%start\tprogram;\r\n\r\n\r\nprogram\t:\tline_list\r\n\t\t|\t.\r\n\t\t;\r\n\r\n\r\nline_list\t:\tline_list\tline \r\n\t\t|\tline\r\n\t\t;\r\n\r\n\r\n");
		

		lexToken_t tok;
		ar_bool_t is_ok = false;
		
		

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




void lex_line_test5()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 1;
		act.value = 200;
		AR_ASSERT(Lex_InsertRule(lex, L"[\\r\\n]", &act));

		
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));

		match = Lex_CreateMatch(lex);
		
		Lex_ResetInput(match, L"\r\n\f\f\r\n");
		

		lexToken_t tok;
		ar_bool_t is_ok = false;
		
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




void lex_line_test6()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 1;
		act.value = 200;

		

		AR_ASSERT(Lex_InsertRule(lex, L"\"\\\"\\b\\f\\n\\r\\t\\v\\a\\\\\\\"\\'\\x67\\012\\\"\"", &act));

		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));
		
		match = Lex_CreateMatch(lex);
		
		Lex_ResetInput(match, L"\\\\\\\\");
		

		lexToken_t tok;
		ar_bool_t is_ok = false;
		
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



#endif




void lex_test_loop3()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 0;
		act.value = 200;
		AR_ASSERT(Lex_InsertRule(lex, L"(a*)*", &act) == AR_S_YES);
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act));

		match = Lex_CreateMatch(lex);

		Lex_ResetInput(match, L"aaaaa");
		

		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);

}



static const wchar_t *__cfg_lex_name[] =
{
		L"	delim		= 	[\\x{000B}\\x{0020}\\x{00A0}\\x{2028}\\x{2029} \\f\\v\\t\\r\\n]+",
		L"  skip_lexem		= {delim}",
		L"  key_lookahead   = {skip_lexem}+|\"{\""
};



void lex_test_loop4()
{
		lexMatch_t		*match;
		lex_t			*lex;

		arStatus_t		ar_status;
		lex = Lex_Create();
		
		ar_status = AR_S_YES;
		

		for(size_t i = 0; i < AR_NELEMS(__cfg_lex_name); ++i)
		{
				ar_status = Lex_Insert(lex, __cfg_lex_name[i]);
				AR_ASSERT(ar_status == AR_S_YES);

		}

		lexAction_t act;
		act.is_skip = false;
		act.priority = 0;
		act.value = 200;

		AR_ASSERT(Lex_InsertRule(lex, L"(a|bcdef|g|ab|c|d|e|efg|fg)", &act) == AR_S_YES);
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act) == AR_S_YES);

		match = Lex_CreateMatch(lex);

		Lex_ResetInput(match, L"abcdefg");
		

		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);

}



static void lex_perf_test1()
{

		typedef enum
		{
				CFG_LEXVAL_EOI	= 0,
				CFG_LEXVAL_DELIM ,
				CFG_LEXVAL_SKIP = 600,
				CFG_LEXVAL_CODE,
				CFG_LEXVAL_VALUE,
				CFG_LEXVAL_START,
				CFG_LEXVAL_NAME,
				CFG_LEXVAL_TOKEN,
				CFG_LEXVAL_PREC,
				CFG_LEXVAL_ASSOC,
				CFG_LEXVAL_LEXEME,
				CFG_LEXVAL_NUMBER,
				CFG_LEXVAL_DOT,
				CFG_LEXVAL_COMMA,
				CFG_LEXVAL_COLON,
				CFG_LEXVAL_SEMI,
				CFG_LEXVAL_OR,
				CFG_LEXVAL_FAKE_EOI,
				CFG_LEXVAL_ACTION,
				CFG_LEXVAL_ACTION_INS,
				CFG_LEXVAL_COMMENT
		}cfgLexValue_t;



		static const wchar_t *__cfg_lex_name[] =
		{
				L"	delim		= 	[\\x{000B}\\x{0020}\\x{00A0}\\x{2028}\\x{2029} \\f\\v\\t\\r\\n]+",
				L"	letter		= 	[A-Z_a-z]",
				L"	digit		=	[0-9]",
				L"	number		=	0|[1-9]{digit}*",
				L"	name		=	{letter}({letter}|{digit})*",
				L"	lexeme		=	{name}|(\\\"([^\\\"\\n\\r])+\\\")|('([^'\\n\\r])+')",
				L"	comment		= 	/\\*([^\\*]|\\*+[^\\*/])*\\*+/",
				L"	comment_line	= //[^\\n\\r]*(\\n|$|\\r)",
				L"  skip_lexem		= {delim}",
				L"	comment_lexem	= {comment}|{comment_line}",
				L"  key_lookahead   = {skip_lexem}+|\"{\""
		};



		typedef struct  __cfg_lex_pattern_tag
		{
				size_t			val;
				const wchar_t	*regex;
				ar_bool_t			is_skip;
				size_t			prec;
		}cfgLexPattern_t;


		static const cfgLexPattern_t	__cfg_pattern[] =
		{
				{CFG_LEXVAL_EOI,	L"$", false,2},
				{CFG_LEXVAL_DELIM, L"{skip_lexem}+", true, 1},

				{CFG_LEXVAL_COMMENT, L"{comment_lexem}", false, 1},

				{CFG_LEXVAL_ASSOC,	L"\"%\"(\"left\"|\"right\"|\"nonassoc\")(?={key_lookahead})", false,1},

				{CFG_LEXVAL_CODE,	L"\"%code\"(?={key_lookahead})", false,0},
				{CFG_LEXVAL_VALUE,	L"\"%value\"(?={key_lookahead})", false,0},

				{CFG_LEXVAL_SKIP,	L"\"%skip\"(?={key_lookahead})", false,0},
				{CFG_LEXVAL_START,	L"\"%start\"(?={key_lookahead})", false,0},
				{CFG_LEXVAL_NAME,	L"\"%name\"(?={key_lookahead})", false,0},
				{CFG_LEXVAL_TOKEN,	L"\"%token\"(?={key_lookahead})", false,0},
				{CFG_LEXVAL_PREC,	L"\"%prec\"(?={key_lookahead})", false,0},

				{CFG_LEXVAL_ACTION, L"\"%action\"(?={skip_lexem}+)", false, 0},
				{CFG_LEXVAL_ACTION_INS, L"\\{:[^\\u0]*?:\\}", false, 0},

				{CFG_LEXVAL_LEXEME,	L"{lexeme}", false,0},
				{CFG_LEXVAL_NUMBER,	L"{number}", false,0},

				{CFG_LEXVAL_DOT,		L"\".\"",	false,1},
				{CFG_LEXVAL_COMMA,		L"\",\"",	false,1},

				{CFG_LEXVAL_COLON,		L"\":\"",	false,1},
				{CFG_LEXVAL_SEMI,		L"\";\"",	false,1},

				{CFG_LEXVAL_OR,		L"\"|\"",	false,1}
		};




		lex_t *lex;
		size_t i;
		arStatus_t status;
		lex = Lex_Create();
		AR_ASSERT(lex != NULL);


		

		for(i = 0; i < AR_NELEMS(__cfg_lex_name); ++i)
		{
			
				status = Lex_Insert(lex, __cfg_lex_name[i]);
				AR_ASSERT(status == AR_S_YES);
		}

		for(i = 0; i < AR_NELEMS(__cfg_pattern); ++i)
		{
				lexAction_t action;
				action.is_skip = __cfg_pattern[i].is_skip;
				action.priority = __cfg_pattern[i].prec;
				action.value = (size_t)__cfg_pattern[i].val;

				status = Lex_InsertRule(lex, __cfg_pattern[i].regex, &action);

				AR_ASSERT(status == AR_S_YES);
		}

		status = Lex_GenerateTransTable(lex);
		if(status != AR_S_YES)
		{
				AR_ASSERT(false);
				goto INVALID_POINT;
		}


		/*************************************************************/

		
		lexMatch_t *match = Lex_CreateMatch(lex);
		AR_ASSERT(match != NULL);
		arString_t *s = AR_CreateString();
		status = AR_LoadBomTextFile(L"C:\\Users\\solidus\\Desktop\\lex_test_1.gmr", NULL, s);

		AR_ASSERT(status == AR_S_YES);

		AR_ASSERT(AR_GetStringLength(s) > 0);
		
		
		
		for(size_t i = 0; i < 5; ++i)
		{
				lexToken_t tok;
				arStatus_t lex_status;

				Lex_ResetInput(match, AR_CSTR(s));

				ar_uint_64_t beg = AR_GetTime_Milliseconds();

				while(true)
				{
						lex_status = Lex_Match(match, &tok);

						if(lex_status == AR_S_YES || lex_status == AR_S_YES)
						{
								if(tok.value == 0)
								{
										break;
								}
#if(0)
								wchar_t *s = AR_wcsndup(tok.str, tok.count);
								AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
								AR_DEL(s);
#endif

						}else
						{
								break;
						}
				}

				ar_uint_64_t end = AR_GetTime_Milliseconds();

				AR_printf(L"round %d elapsed time : %qu ms\r\n", i, end - beg);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						AR_printf(L"%ls\r\n", L"success\r\n");
				}else
				{
						AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");

				}

		}



		/*************************************************************/





INVALID_POINT:

		if(s)
		{
				AR_DestroyString(s);
				s = NULL;
		}


		if(match)
		{
				Lex_DestroyMatch(match);
				match = NULL;
		}

		if(lex)
		{
				Lex_Destroy(lex);
				lex = NULL;
		}

}





static void lex_perf_test2()
{

		



		
		static const wchar_t *__g_lex_name[] =
		{
				L"delim = [\\x{000A}\\x{000B}\\x{000C}\\x{000D}\\x{0085}\\x{2028}\\x{2029}\\x{0020}\\f\\n\\r\\t\\v\\x{0009}\\x{0020}\\x{00A0}\\x{1680}\\x{180E}\\x{2000}-\\x{200A}\\x{202F}\\x{205F}\\x{3000}]",
				L"comment = /\\*([^\\*]|\\*+[^\\*/])*\\*+/",
				L"comment_line = (//[^\\x{000A}\\x{000B}\\x{000C}\\x{000D}\\x{0085}\\x{2028}\\x{2029}]*(\\x{000A}|\\x{000B}|\\x{000C}|\\x{000D}|\\x{0085}|\\x{2028}|\\x{2029}|$))",
				L"skip_lexem = {delim}|{comment_line}|{comment}",
				L"digit = [0-9]",
				L"number = {digit}+",
				L"letter = [A-Z_a-z\\x{0800}-\\x{4E00}\\x{4E00}-\\x{9FA5}\\x{3130}-\\x{318F}\\x{AC00}-\\x{D7AF}]",
				L"hex_digit = [0-9a-fA-F]",
				L"hex_literal = 0(x|X){hex_digit}+",
				L"oct_literal = 0[0-7]+",
				L"dec_literal = (0|[1-9][0-9]*)",
				L"exponet = (e|E)(\\+|\\-)?[0-9]+",
				L"float_literal = (((([0-9]\\.[0-9]*)){exponet}?)|([0-9]+{exponet}))",
				L"escape_seq = (\\\\(\\x22|\\x27))",
				L"string_dq = (\\x22({escape_seq}|[^\\x22])*\\x22)",
				L"string_sq = \\x27({escape_seq}|[^\\x27])*\\x27",
				L"keyword_lhd = {letter}|[0-9]",
				L"float_constant = {float_literal}(?!{keyword_lhd})",
				L"hex_constant = {hex_literal}(?!{keyword_lhd})",
				L"oct_constant = {oct_literal}(?!{keyword_lhd})",
				L"dec_constant = {dec_literal}(?!{keyword_lhd})",
				
#if(0)
				L"line_terminator = [\\x{000A}\\x{000D}]",
				L"source_character = [\\x{0000}-\\x{FFFF}]",
				L"regex_srccharset = [^\\\\]",
				L"regex_firstchar = ([^\\*/\\\\\\[])",
				L"regex_char = ([^\\*/\\\\\\[])",
				L"regex_str = (/({regex_firstchar}{})/)",
#endif

		};




		enum
		{
				TOK_DELIM_ID = 257,
				TOK_NAME = 258,
				TOK_STRING = 259,
				TOK_FLOAT_NUMBER = 260,
				TOK_INT_NUMBER = 261,
				TOK_IMPORT = 262,
				TOK_FROM = 263,
				TOK_FOR = 264,
				TOK_DO = 265,
				TOK_WHILE = 266,
				TOK_IF = 267,
				TOK_ELSE = 268,
				TOK_CONTINUE = 269,
				TOK_BREAK = 270,
				TOK_RETURN = 271,
				TOK_NULL = 272,
				TOK_TRUE = 273,
				TOK_FALSE = 274,
				TOK_VAR = 275,
				TOK_IN = 276,
				TOK_LIST = 277,
				TOK_TABLE = 278,
				TOK_ELLIPSIS = 279,
				TOK_INC = 280,
				TOK_DEC = 281,
				TOK_ANDAND = 282,
				TOK_OROR = 283,
				TOK_LE = 284,
				TOK_GE = 285,
				TOK_EQ = 286,
				TOK_NE = 287,
				TOK_LESS = 288,
				TOK_GREATER = 289,
				TOK_L_BRACES = 290,
				TOK_R_BRACES = 291,
				TOK_L_PAREN = 292,
				TOK_R_PAREN = 293,
				TOK_L_SQUARE = 294,
				TOK_R_SQUARE = 295,
				TOK_SEMICOLON = 296,
				TOK_COMMA = 297,
				TOK_ASSIGN = 298,
				TOK_ADD = 299,
				TOK_SUB = 300,
				TOK_MUL = 301,
				TOK_DIV = 302,
				TOK_MOD = 303,
				TOK_NOT = 304,
				TOK_COLON = 305,
				TOK_QUEST = 306,
				TOK_DOT = 307,
				TOK_OREQ = 308,
				TOK_AND = 309,
				TOK_OR = 310,
				TOK_XOR = 311,
				TOK_DOLLAR = 312,
				TOK_TILDE = 313,
				REGEX_STR = 314,
		};




		static struct 
		{
				const wchar_t *name;
				size_t val;
				size_t prec;
				const wchar_t *regex;
				ar_bool_t is_skip;
		}__g_term_pattern[] =  {

				{NULL, TOK_DELIM_ID,3, L"{skip_lexem}", true},
				{L"NAME", TOK_NAME, 0, L"{letter}({letter}|{digit})*", false},
				{L"STRING", TOK_STRING, 0, L"{string_dq}|{string_sq}", false},
				//{L"REGEX_STR", REGEX_STR, 0, L"{regex_str}", false},
				
				{L"FLOAT_NUMBER", TOK_FLOAT_NUMBER, 2, L"{float_constant}", false},
				{L"INT_NUMBER", TOK_INT_NUMBER, 2, L"{hex_constant}|{oct_constant}|{dec_constant}", false},
				{L"import", TOK_IMPORT, 1, L"\"import\"(?!{keyword_lhd})", false},
				{L"from", TOK_FROM, 1, L"\"from\"(?!{keyword_lhd})", false},
				{L"for", TOK_FOR, 1, L"\"for\"(?!{keyword_lhd})", false},
				{L"do", TOK_DO, 1, L"\"do\"(?!{keyword_lhd})", false},
				{L"while", TOK_WHILE, 1, L"\"while\"(?!{keyword_lhd})", false},
				{L"if", TOK_IF, 1, L"\"if\"(?!{keyword_lhd})", false},
				{L"else", TOK_ELSE, 1, L"\"else\"(?!{keyword_lhd})", false},
				{L"continue", TOK_CONTINUE, 1, L"\"continue\"(?!{keyword_lhd})", false},
				{L"break", TOK_BREAK, 1, L"\"break\"(?!{keyword_lhd})", false},
				{L"return", TOK_RETURN, 1, L"\"return\"(?!{keyword_lhd})", false},
				{L"null", TOK_NULL, 1, L"\"null\"(?!{keyword_lhd})", false},
				{L"true", TOK_TRUE, 1, L"\"true\"(?!{keyword_lhd})", false},
				{L"false", TOK_FALSE, 1, L"\"false\"(?!{keyword_lhd})", false},
				{L"var", TOK_VAR, 1, L"\"var\"(?!{keyword_lhd})", false},
				{L"in", TOK_IN, 1, L"\"in\"(?!{keyword_lhd})", false},
				{L"list", TOK_LIST, 1, L"\"list\"(?!{keyword_lhd})", false},
				{L"table", TOK_TABLE, 1, L"\"table\"(?!{keyword_lhd})", false,},
				{L"...", TOK_ELLIPSIS, 2, L"\"...\"", false},
				{L"++", TOK_INC, 1, L"\"++\"", false},
				{L"--", TOK_DEC, 1, L"\"--\"", false},
				{L"&&", TOK_ANDAND, 1, L"\"&&\"", false},
				{L"||", TOK_OROR, 1, L"\"||\"", false},
				{L"<=", TOK_LE, 1, L"\"<=\"", false},
				{L">=", TOK_GE, 1, L"\">=\"", false},
				{L"==", TOK_EQ, 1, L"\"==\"", false},
				{L"!=", TOK_NE, 1, L"\"!=\"", false},
				{L"|=", TOK_OREQ, 1, L"\"|=\"", false},
				{L"<", TOK_LESS, 0, L"\"<\"", false},
				{L">", TOK_GREATER, 0, L"\">\"", false},
				{L"{", TOK_L_BRACES, 0, L"\"{\"", false},
				{L"}", TOK_R_BRACES, 0, L"\"}\"", false},
				{L"(", TOK_L_PAREN, 0, L"\"(\"", false},
				{L")", TOK_R_PAREN, 0, L"\")\"", false},
				{L"[", TOK_L_SQUARE, 0, L"\"[\"", false},
				{L"]", TOK_R_SQUARE, 0, L"\"]\"", false},
				{L";", TOK_SEMICOLON, 0, L"\";\"", false},
				{L",", TOK_COMMA, 0, L"\",\"", false},
				{L"=", TOK_ASSIGN, 0, L"\"=\"", false},
				{L"+", TOK_ADD, 0, L"\"+\"", false},
				{L"-", TOK_SUB, 0, L"\"-\"", false},
				{L"*", TOK_MUL, 0, L"\"*\"", false},
				{L"/", TOK_DIV, 0, L"\"/\"", false},
				{L"%", TOK_MOD, 0, L"\"%\"", false},
				{L"!", TOK_NOT, 0, L"\"!\"", false},
				{L":", TOK_COLON, 0, L"\":\"", false},
				{L"?", TOK_QUEST, 0, L"\"?\"", false},
				{L".", TOK_DOT, 0, L"\".\"", false},
				{L"&", TOK_AND, 0, L"\"&\"", false},
				{L"|", TOK_OR, 0, L"\"|\"", false},
				{L"~", TOK_TILDE, 0, L"\"~\"", false},
				{L"^", TOK_XOR, 0, L"\"^\"", false},
				{L"$", TOK_DOLLAR, 0, L"\"$\"", false},
				{L"CFG_LEXVAL_EOI", 0, 2, L"$", false}
		};


		lex_t *lex;
		size_t i;
		arStatus_t status;
		lex = Lex_Create();
		AR_ASSERT(lex != NULL);


		

		for(i = 0; i < AR_NELEMS(__g_lex_name); ++i)
		{
			
				status = Lex_Insert(lex, __g_lex_name[i]);
				AR_ASSERT(status == AR_S_YES);
		}

		for(i = 0; i < AR_NELEMS(__g_term_pattern); ++i)
		{
				lexAction_t action;
				action.is_skip = __g_term_pattern[i].is_skip;
				action.priority = __g_term_pattern[i].prec;
				action.value = (size_t)__g_term_pattern[i].val;

				status = Lex_InsertRule(lex, __g_term_pattern[i].regex, &action);

				AR_ASSERT(status == AR_S_YES);
		}

		status = Lex_GenerateTransTable(lex);
		if(status != AR_S_YES)
		{
				AR_ASSERT(false);
				goto INVALID_POINT;
		}


		/*************************************************************/

		
		lexMatch_t *match = Lex_CreateMatch(lex);
		AR_ASSERT(match != NULL);
		arString_t *s = AR_CreateString();
		status = AR_LoadBomTextFile(L"C:\\Users\\solidus\\Desktop\\js_lex_test_2.js", NULL, s);

		AR_ASSERT(status == AR_S_YES);

		AR_ASSERT(AR_GetStringLength(s) > 0);
		
		
		
		for(size_t i = 0; i < 5; ++i)
		{
				lexToken_t tok;
				arStatus_t lex_status;

				Lex_ResetInput(match, AR_CSTR(s));

				ar_uint_64_t beg = AR_GetTime_Milliseconds();

				while(true)
				{
						lex_status = Lex_Match(match, &tok);

						if(lex_status == AR_S_YES || lex_status == AR_S_YES)
						{
								if(tok.value == 0)
								{
										break;
								}

								/*
								wchar_t *s = AR_wcsndup(tok.str, tok.count);
								AR_printf(L"%ls : row == %d : col == %d\r\n", s, tok.line, tok.col);
								AR_DEL(s);
								*/

						}else
						{
								break;
						}
				}

				ar_uint_64_t end = AR_GetTime_Milliseconds();

				AR_printf(L"round %d elapsed time : %qu ms\r\n", i, end - beg);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						AR_printf(L"%ls\r\n", L"success\r\n");
				}else
				{
						AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");

				}

		}



		/*************************************************************/





INVALID_POINT:

		if(s)
		{
				AR_DestroyString(s);
				s = NULL;
		}


		if(match)
		{
				Lex_DestroyMatch(match);
				match = NULL;
		}

		if(lex)
		{
				Lex_Destroy(lex);
				lex = NULL;
		}

}




void lex_line_test9()
{
		lexMatch_t		*match;
		lex_t			*lex;

		lex = Lex_Create();
		
		lexAction_t act;
		act.is_skip = false;
		act.priority = 0;
		act.value = 200;
		AR_ASSERT(Lex_InsertRule(lex, L".+", &act) == AR_S_YES);
		
		act.value = 0;
		AR_ASSERT(Lex_InsertRule(lex, L"$", &act)== AR_S_YES);

		match = Lex_CreateMatch(lex);
		
		//Lex_ResetInput(match, L";中国\r;字abc\n;真麻烦\r\n;太烦了\r\n");
		Lex_ResetInput(match, L";中国;字abc");

		
		lexToken_t tok;
		arStatus_t lex_status;
		
		while(true)
		{
				lex_status = Lex_Match(match, &tok);

				if(lex_status == AR_S_YES || lex_status == AR_S_YES)
				{
						if(tok.value == 0)
						{
								break;
						}
						wchar_t *s = AR_wcsndup(tok.str, tok.count);
						AR_printf(L"%ls : row == %d : col == %d action == %Iu\r\n", s, tok.line, tok.col, tok.value);
						AR_DEL(s);
				}else
				{
						break;
				}
		}

		if(lex_status == AR_S_YES || lex_status == AR_S_YES)
		{
				AR_printf(L"%ls\r\n", L"success\r\n");
		}else
		{
				AR_printf(L"error code = %d, %ls\r\n", AR_GET_STATUS(lex_status), L"failed\r\n");
				
		}


		Lex_DestroyMatch(match);
		Lex_Destroy(lex);
}




void lex_test()
{
		//rgx_test_loop();
		//lex_test_loop4();
		

		//lex_test_loop();
		//lex_test_loop2();
		//lex_chinese_char();
		//lex_line_test();
		//lex_line_test2();
		//lex_line_test3();
		//lex_line_test4();

		//lex_line_test5();
		//lex_line_test6();
		//lex_line_test7();
		

		

		//lex_quote_test7();

		//lex_test_skip_line_test();

		//lex_line_num_test();

		lex_perf_test1();

		lex_perf_test2();

		//lex_line_test9();

		
}

AR_NAMESPACE_END



#endif

