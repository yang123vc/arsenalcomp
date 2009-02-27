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
#include "node.h"
#include "dfa.h"
#include "lex.h"




typedef struct __lex_tag
{
		lexCClass_t		cclass;
		
		/*lexDFAVertex_t	*dfa;*/

		lexStateTable_t tbl;
}lex_t;




lex_t*			LEX_CreateLex()
{
		lex_t *res = AR_NEW0(lex_t);
		LEX_InitCClass(&res->cclass);
		LEX_InitStateTable(&res->tbl);
		return res;

}

void			LEX_Destroy(lex_t *lex)
{
		LEX_UnInitCClass(&lex->cclass);
		LEX_UnInitStateTable(&lex->tbl);
		/*
		if(lex->dfa != NULL)
		{
				LEX_DestroyDFA(lex->dfa);
		}
		*/
}










#define TRIM_STR(p) do{ while(*p != L'\0' && (*p == L' ' || *p == L'\t'))p++;}while(0)



const wchar_t* __parse_pattern(lexNode_t *uniroot, lexNameTable_t *tbl, const wchar_t *pattern, lexError_t *res_err)
{
		const wchar_t *p, *beg; 
		

		lexError_t err;
		
		p = pattern; err = LEX_NO_ERROR; *res_err = err;
		

		while(*p != L'\0')
		{
				
				TRIM_STR(p);

				if(*p == L'(')
				{
						size_t val; int prio; size_t count;
						
						p++;
						TRIM_STR(p);
						if(!iswdigit(*p))
						{
								err = LEX_BUILD_INVALID_PATTERN;
								goto INVALID_POINT;
						}
						val = 0;
						while(*p != L'\0' && (*p >= L'0' && *p <= L'9'))
						{
								val *= 10;
								val += ((*p) - L'0');
								p++;
						}

						TRIM_STR(p);

						if(*p == L',')
						{
								bool_t is_ok = False;
								++p;
								TRIM_STR(p);
								prio = 0;
								
								while(*p != L'\0' && (*p >= L'0' && *p <= L'9'))
								{
										is_ok = True;
										prio *= 10;
										prio += ((*p) - L'0');
										p++;
								}
								prio = -1;
						}else
						{
								prio = -1;
						}

						TRIM_STR(p);
						if(*p != L')')
						{
								err = LEX_BUILD_INVALID_PATTERN;
								goto INVALID_POINT;
						}

						++p;
						TRIM_STR(p);

						if(*p == L'\0')
						{
								err = LEX_BUILD_INVALID_PATTERN;
								goto INVALID_POINT;
						}


						beg = p; count= 0;
						while(*p != L'\0' && *p != L'\r' && *p != L'\n')
						{
								p++; count++;
						}

						
						
						if(count == 0)
						{
								err = LEX_BUILD_INVALID_PATTERN;
								goto INVALID_POINT;
						}
						{
								wchar_t *def; lexResult_t result;
								def = AR_NEWARR0(wchar_t, count + 1);
								AR_WSTRNCPY(def, beg, count);
								
								result = LEX_CompilePattern(def, tbl);

								AR_DEL(def);

								if(result.err != LEX_NO_ERROR)
								{
										err = result.err;
										goto INVALID_POINT;
								}
								
								{
										lexNode_t *cat, *final;
										final	= LEX_CreateNode(LEX_FINAL);
										cat		= LEX_CreateNode(LEX_CAT);
										final->action.priority = prio;
										final->action.type = val;
										LEX_InsertNodeToCat(&cat->cat, result.node);
										LEX_InsertNodeToCat(&cat->cat, final);
										LEX_InsertNodeToBranch(&uniroot->branch, cat);
								}
						}

						if(*p == L'\0')
						{
								return p;
						}else if(*p == L'\r')
						{
								return p + 2;
						}else /*\n*/
						{
								return p+1;
						}

				}else if(iswalpha(*p) || *p == L'_')
				{
						wchar_t *name, *def;
						const  wchar_t *pname, *pdef;
						size_t name_count, def_count;

						pname = p;
						name_count = 0;
						while(iswalnum(*p) || *p == L'_')
						{
								p++;name_count++;
						}
						
						TRIM_STR(p);

						if(*p != L'=' || name_count == 0)
						{
								err = LEX_BUILD_INVALID_NAME;
								goto INVALID_POINT;
						}

						++p;
						TRIM_STR(p);

						pdef = p;
						def_count = 0;
						while(*p != L'\0' && *p != L'\r' && *p != L'\n')
						{
								p++;
								def_count++;
						}

						if(def_count == 0)
						{
								err = LEX_BUILD_INVALID_NAME;
								goto INVALID_POINT;
						}

						name = AR_NEWARR0(wchar_t, name_count+1);
						AR_WSTRNCPY(name, pname, name_count);

						def = AR_NEWARR0(wchar_t, def_count+1);
						AR_WSTRNCPY(def, pdef, def_count);

						
						err = LEX_InsertNameToNameTable(tbl, name, def);

						AR_DEL(name);
						AR_DEL(def);


						if(err != LEX_NO_ERROR)
						{
								goto INVALID_POINT;
						}

						if(*p == L'\0')
						{
								return p;
						}else if(*p == L'\r')
						{
								return p+2;
						}else/*\n*/
						{
								return p+1;
						}
				
				}else if(*p == L'\r' || *p == L'\n' || *p == L'\t' || *p == L' ')
				{
						++p;

				}else if(*p != L'\0')
				{
						err = LEX_BUILD_INVALID_PATTERN;
						goto INVALID_POINT;
				}
		}
		
		return p;
		
INVALID_POINT:
		AR_Error(AR_CRITICAL, L"Lex Pattern Error : %s : %d\n", pattern, err);
		*res_err =  err;
		return NULL;
}

#undef TRIM_STR



lexError_t		LEX_Build(lex_t *lex, const wchar_t *pattern)
{
		lexNameTable_t tbl;
		lexCClass_t		cclass;
		lexLeafSet_t	leafset;
		lexDFAVertex_t	*dfa;
		lexNode_t	*uniroot; lexError_t err;
		const wchar_t *next;

		AR_ASSERT(pattern != NULL);
		
		LEX_InitNameTable(&tbl);
		LEX_InitLeafSet(&leafset);
		LEX_InitCClass(&cclass);

		uniroot = LEX_CreateNode(LEX_BRANCH);

		err = LEX_NO_ERROR;

		next = pattern;
		while(*next != L'\0')
		{
				next = __parse_pattern(uniroot, &tbl, next, &err);

				if(err != LEX_NO_ERROR)
				{
						goto INVALID_POINT;
				}
		}

		



		LEX_CalcCClass(&cclass, uniroot);
		LEX_BuildLeafSet(uniroot, &cclass, &leafset);
		dfa = LEX_BuildDFA(&cclass, &leafset);
		
		/*
		if(lex->dfa != NULL)
		{
				LEX_DestroyDFA(lex->dfa);
		}

		lex->dfa = dfa;
		*/

		LEX_CopyCClass(&lex->cclass, &cclass);
		LEX_BuildStateTable(&lex->tbl, dfa, &lex->cclass);
		
		LEX_DestroyDFA(dfa);
		LEX_UnInitNameTable(&tbl);
		LEX_UnInitLeafSet(&leafset);
		LEX_UnInitCClass(&cclass);
		LEX_DestroyNode(uniroot);

		return LEX_NO_ERROR;

INVALID_POINT:
		if(uniroot != NULL)LEX_DestroyNode(uniroot);
		LEX_UnInitNameTable(&tbl);
		LEX_UnInitLeafSet(&leafset);
		LEX_UnInitCClass(&cclass);
		return err;
}




#if(0)
bool_t	LEX_Match(lex_t *lex, lexMatch_t *match)
{
		const wchar_t *forward;
		int id;
		size_t count;
		lexDFAVertex_t	*curr, *accepted;
		size_t	x,y;
		
		if(match->err != LEX_NO_ERROR)return False;
		AR_ASSERT(lex != NULL && match != NULL && match->next != NULL);// && match->err == LEX_NO_ERROR);

		x = match->x;
		y = match->y;
		curr = lex->dfa;
		accepted = NULL;
		count = 0;
		
		match->token.x = match->x;
		match->token.y = match->y;
		
		
		
		
		if(*match->next == L'\0')
		{
				match->err = LEX_RUN_EOI;
				return False;
		}

		forward = match->next;
		match->token.count = 0;
		match->token.tok = forward;



		while(*forward != L'\0' && curr != NULL)
		{
				wchar_t c = *forward;

				if(c == L'\n')
				{
						x++; y = 0;
				}else
				{
						y++;
				}

				id = LEX_FindIndexFromCClass(&lex->cclass, c);
				curr = LEX_GetNextVertex(curr, id);
				
				if(curr != NULL)
				{
						count++;
						forward++;
						
						if(curr->is_final)
						{
								accepted = curr;
								match->x = x;
								match->y = y;
						}
				}
		}

		if(accepted != NULL)
		{
				size_t act_id,i;
				for(act_id = 0, i = 0; i < accepted->act_count; ++i)
				{
						
						if(accepted->action[act_id].priority < accepted->action[i].priority)
						{
								act_id = i;
						}
				}
				match->token.type = accepted->action[act_id].type;
				match->token.count = count;
				match->next = match->next + count;
				return True;
		}else
		{
				match->err = LEX_RUN_NOTFOUND_PATTERN;
				return False;
		}
}
#endif


bool_t	LEX_Match(lex_t *lex, lexMatch_t *match)
{
		const wchar_t *forward;
		int id;
		size_t count;
		int		curr, accepted;
		size_t	x,y;
		
		if(match->err != LEX_NO_ERROR)return False;
		AR_ASSERT(lex != NULL && match != NULL && match->next != NULL);// && match->err == LEX_NO_ERROR);

		x = match->x;
		y = match->y;
		curr = 0;
		accepted = -1;
		count = 0;
		
		match->token.x = match->x;
		match->token.y = match->y;
		
		
		
		
		if(*match->next == L'\0')
		{
				match->err = LEX_RUN_EOI;
				return False;
		}

		forward = match->next;
		match->token.count = 0;
		match->token.tok = forward;



		while(*forward != L'\0' && curr != -1)
		{
				wchar_t c = *forward;

				if(c == L'\n')
				{
						x++; y = 0;
				}else
				{
						y++;
				}

				id = LEX_FindIndexFromCClass(&lex->cclass, c);
				AR_ASSERT((size_t)id < lex->tbl.col);
				curr = LEX_GetNextState(&lex->tbl, curr, id);
				
				if(curr != -1)
				{
						count++;
						forward++;
						
						if(lex->tbl.is_final[curr])
						{
								accepted = curr;
								match->x = x;
								match->y = y;
						}
				}
		}

		if(accepted != -1)
		{
				match->token.type = lex->tbl.actions[accepted].type;
				match->token.count = count;
				match->next = match->next + count;
				return True;
		}else
		{
				match->err = LEX_RUN_NOTFOUND_PATTERN;
				return False;
		}


}
