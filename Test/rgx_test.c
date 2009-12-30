#include "deelx.h"
#include "test.h"

#include "../Arsenal/Regex/rgx_in.h"
#include "../Arsenal/Lex/lex.h"




#if defined(__LIB)

AR_NAMESPACE_BEGIN

/*
bool_t			RGX_Run(rgxProg_t *prog,  lexMatch_t *match)
{
		
		int_t cnt = __thompson(prog, match->next, match->input);

		
		if(cnt > 0)
		{
				AR_printf(L"%ls\r\n", AR_wcsndup(match->next, (size_t)cnt));
				return true;
		}else
		{
				return false;
		}
}
*/



void rgx_test_1(const wchar_t *e1, const wchar_t *input)
{
		rgxNameSet_t	name_set;
		rgxResult_t		result;

		rgxProg_t		prog;
		
		

		RGX_InitNameSet(&name_set);
		
		//result = RGX_ParseExpr(L"(?=abc)(([a-z]*)+)?(?=.(?=\"/\"(?=[^c][\\r\\n])))+", &name_set);

		result = RGX_ParseExpr(e1, &name_set);
		
		if(result.node == NULL)
		{
				AR_printf(L"error == %ls\r\n", result.err.pos);
				return;
		}

		if(result.node)
		{
				rgxNode_t *cat, *final;
				final = RGX_CreateNode(RGX_FINAL_T);
				final->final_val = 2;
				cat = RGX_CreateNode(RGX_CAT_T);
				cat->left = result.node;
				cat->right = final;
				result.node = cat;

				RGX_CorrectTree(result.node);
		}


		if(result.node)
		{
				arString_t *str = AR_CreateString();
				arString_t *str2 = AR_CreateString();

				RGX_ToString(result.node, str);
				RGX_ToString(RGX_CopyNode(result.node), str2);

				
				AR_printf(L"%ls\r\n", AR_GetStrString(str));
				AR_printf(L"%ls\r\n", AR_GetStrString(str2));

				AR_DestroyString(str);
		}

		RGX_InitProg(&prog);

		if(result.node)
		{
				
				arString_t *str;

				
				
				RGX_Compile(&prog, result.node);
				
				str = AR_CreateString();
				
				RGX_PringProg(&prog, str);

				AR_StrPrint(str);
				
				AR_DestroyString(str);

		}
		
		if(result.node)
		{
				getchar();
				lexMatch_t match;
				lexToken_t tok;
				rgxThreadList_t curr, next;
				RGX_InitThreadList(&curr);
				RGX_InitThreadList(&next);
				LEX_InitMatch(&match, input);
				
				//AR_printf(L"RGX_Run result == %d\r\n", RGX_Run(&prog, &match));

				while(RGX_Match(&prog, &match, &tok, &curr, &next))
				{
						if(tok.count == 0)break;

						AR_printf(L"tok.str ==%ls\r\n", AR_wcsndup(tok.str, tok.count));
						AR_printf(L"tok.line == %d tok.col == %d\r\n", tok.line, tok.col);
						AR_printf(L"tok.type == %d\r\n", tok.type);

						AR_printf(L"next == %ls\r\n", match.next);
				}
				
				AR_printf(L"isok == %d\r\n", match.is_ok);

				LEX_UnInitMatch(&match);

				RGX_DestroyNode(result.node);
		}

		RGX_UnInitProg(&prog);
}






void rgx_test()
{
		while(true)
		{
				wchar_t buf[1024], input[1024];

				_getws(buf);
				_getws(input);
				if(AR_wcscmp(buf, L"quit") == 0)break;
				rgx_test_1(buf, input);
		}

}



AR_NAMESPACE_END



#endif
