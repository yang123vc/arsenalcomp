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


#include "lex.h"



AR_NAMESPACE_BEGIN


static bool_t __insert_lexcfg(lexConfig_t **pcfg, const wchar_t *line)
{
		lexConfig_t tmp;
		uint32_t val, prio;
		const wchar_t *p;
		bool_t	is_skip;
		AR_ASSERT(pcfg != NULL && line != NULL);

		/*p = AR_wcstrim(line, L" \t");*/
		p = AR_wcstrim_space(line);

		if(*p == L'\0')return false;
		val = 0; prio = 0;
		is_skip = false;
		tmp.pattern.action.is_skip = false;
		
		if(*p == L'%')
		{
				p = AR_wcsstr(p, L"%skip");
				if(p == NULL)return false;
				is_skip = true;
				/*p = AR_wcstrim(p + 5, L"\t ");*/
				p = AR_wcstrim_space(p + 5);
		}
		
		if(AR_iswdigit(*p))
		{
				tmp.type = LEX_PATTERN;
				p = AR_wtou32(p,&val, 10);
				if(p == NULL)return false;
				/*p = AR_wcstrim(p, L"\t ");*/
				p = AR_wcstrim_space(p);
				
				if(*p == L',')
				{
						/*p = AR_wcstrim(p + 1, L"\t ");*/
						p = AR_wcstrim_space(p+1);
						p = AR_wtou32(p,&prio, 10);

						/*p = AR_wcstrim(p, L"\t ");*/
						p = AR_wcstrim_space(p);

				}
				
				tmp.pattern.action.type = val;
				tmp.pattern.action.priority = prio;
				if(is_skip)tmp.pattern.action.is_skip = is_skip;

				{
						size_t len;
						len = AR_wcslen(p);
						tmp.pattern.pattern = AR_NEWARR(wchar_t, len + 1);
						AR_wcsncpy(tmp.pattern.pattern, p, len);
						tmp.pattern.pattern[len] = L'\0';
				}
		}else
		{
				const wchar_t *beg, *end;
				tmp.type = LEX_NAME;
				if(is_skip)return false;
				if(!AR_iswalpha(*p) && *p != L'_')return false;
				beg = p;
				while(*p != L'\0' && (AR_iswalpha(*p) || *p == L'_'))++p;

				if(*p == L'\0')return false;
				end = p;
				/*p = AR_wcstrim(p, L" \t");*/
				p = AR_wcstrim_space(p);
				
				if(*p != L'=')return false;
				/*p = AR_wcstrim(p+1, L" \t");*/
				p = AR_wcstrim_space(p+1);
				if(*p == L'\0')return false;
				
				{
						size_t len = end - beg;
						tmp.name.name = AR_NEWARR0(wchar_t, len + 1);
						AR_wcsncpy(tmp.name.name, beg, len);
						tmp.name.name[len] = L'\0';
						
						len = AR_wcslen(p);
						tmp.name.expr = AR_NEWARR0(wchar_t, len + 1);
						AR_wcsncpy(tmp.name.expr, p, len);
						tmp.name.expr[len] = L'\0';
				}
		}

		{
				lexConfig_t *new_cfg;
				lexConfig_t *curr, *prev;
				curr = *pcfg;
				prev = NULL;
				
				while(curr)
				{
						prev = curr;
						curr = curr->next;
				}
				new_cfg = AR_NEW0(lexConfig_t);
				*new_cfg = tmp;
				new_cfg->next = NULL;
				if(prev == NULL)
				{
						AR_ASSERT(*pcfg == NULL);
						*pcfg = new_cfg;
				}else
				{
						prev->next = new_cfg;
						new_cfg->next = NULL;
				}
		}
		
		return true;
}

static void __destroy_lexcfg(lexConfig_t *cfg)
{
		lexConfig_t		*curr, *tmp;
		curr = cfg;
		while(curr)
		{
				tmp = curr->next;
				if(curr->type == LEX_NAME)
				{
						AR_DEL(curr->name.name);
						AR_DEL(curr->name.expr);
				}else
				{
						AR_DEL(curr->pattern.pattern);
				}
				AR_DEL(curr);
				curr = tmp;
		}
}


const lexConfig_t*	LEX_CreateConfig(const wchar_t *pattern, const wchar_t **next_input)
{
		lexConfig_t		*result;

		const wchar_t *p;
		bool_t			is_ok;
		AR_ASSERT(pattern != NULL);
		p = AR_wcsstr(pattern, L"%lex");
		if(p == NULL)return NULL;
		p += 4;
				
		p = AR_wcsstr(pattern, L"{");
		if(p == NULL)return NULL;
		/*p = AR_wcstrim(p + 1, L"\t ");*/
		p = AR_wcstrim_space(p + 1);
		
		is_ok = true;
		result = NULL;
		while(*p != L'\0' && *p != L'}')
		{
				const wchar_t *beg;
				wchar_t *line;
				const wchar_t *test;
				
				size_t len;
				beg = p;
				while(*p != L'\0')
				{
						if(*p == L'\r' || *p == L'\n')break;
						++p;
				}
				
				if(*p == L'\0')
				{
						is_ok = false;
						goto END_POINT;
				}
				len = p - beg;
				if(len > 0)
				{
						line = AR_NEWARR(wchar_t, len + 1);

						AR_wcsncpy(line, beg, len);
						line[len] = L'\0';

						/*test = AR_wcstrim(line, L" \t");*/
						AR_wcstrim_right(line, L" \t\r\n");
						test = AR_wcstrim_space(line);
						
						if(*test == L'\0')
						{
								
						}else
						{
								if(!__insert_lexcfg(&result, test))
								{
										AR_printf(L"Invalid Pattern : %ls\r\n", test);
										AR_DEL(line);
										is_ok = false;
										goto END_POINT;
								}
						}
						AR_DEL(line);
				}

				/*if(*p == L'\r')p++;*/
				while(*p == L'\r' || *p == L'\n')p++;
				/*if(*p == L'\n')p++;*/
				/*p = AR_wcstrim(p, L"\t ");*/
				p = AR_wcstrim_space(p);
		}
		
		if(*p != L'}')is_ok = false;
		p++;
		
		
END_POINT:
		if(!is_ok)
		{
				__destroy_lexcfg(result);
				result = NULL;
		}else
		{
				if(next_input)*next_input = p;
		}
		return result;
}

void				LEX_DestroyConfig(const lexConfig_t *pattern)
{
		__destroy_lexcfg((lexConfig_t*)pattern);

}



const wchar_t*		  LEX_Config(lex_t *lex, const wchar_t *pattern)
{
		const wchar_t *next;
		const lexConfig_t *cfg, *curr;
		bool_t is_ok;
		AR_ASSERT(lex != NULL && pattern != NULL);
		LEX_Clear(lex);
		is_ok = true;
		next = NULL;
		cfg = LEX_CreateConfig(pattern, &next);
		if(cfg == NULL)return false;
		curr = cfg;
		while(curr)
		{
				if(curr->type == LEX_NAME)
				{
						is_ok  = LEX_InsertName(lex, curr->name.name, curr->name.expr);
				}else
				{
						is_ok = LEX_InsertRule(lex, curr->pattern.pattern, &curr->pattern.action);
				}

				if(!is_ok)break;
				curr = curr->next;
		}
		
		LEX_DestroyConfig(cfg);
		if(!is_ok)LEX_Clear(lex);
		is_ok = LEX_GenerateTransTable(lex);
		if(!is_ok)LEX_Clear(lex);
		
		if(is_ok)
		{
				return next;
		}
		else
		{
				return NULL;
		}
		
}





AR_NAMESPACE_END

