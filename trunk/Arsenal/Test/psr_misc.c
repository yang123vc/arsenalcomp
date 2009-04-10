
#if(0)
#include "test.h"
#include "..\Parser\parser_in.h"
#include "..\Parser\symbol.h"
#include "..\Parser\lr_item.h"
#include "..\Parser\grammar.h"

#if defined(AR_DEBUG)

#include <windows.h>

AR_NAMESPACE_BEGIN



void str_tbl_test()
{
		const wchar_t *l, *r;
		const psrSymb_t	*tmp;

		l = PSR_AllocString( L"abcdef");
		r = PSR_AllocString( L"abcdef");
		AR_ASSERT(l == r);


		l = PSR_AllocString( L"abcdef");
		r = PSR_AllocString( L"abcdef2");

		AR_ASSERT(l != r);


		AR_ASSERT(PSR_CompSymb(PSR_EpsilonSymb, PSR_EpsilonSymb) == 0);
		AR_ASSERT(PSR_CompSymb(PSR_EpsilonSymb, PSR_EOISymb) != 0);
		AR_ASSERT(PSR_CompSymb(PSR_EpsilonSymb, PSR_StartSymb) != 0);
		AR_ASSERT(PSR_CompSymb(PSR_StartSymb, PSR_StartSymb) == 0);

		tmp = PSR_CreateSymb(L"%Start", PSR_NONTERM, 0, PSR_ASSOC_NOASSOC,0);

		AR_ASSERT(PSR_CompSymb(tmp, PSR_StartSymb) == 0);

		wprintf(L"%s\r\n", PSR_AllocStringInt(33, 10));
		wprintf(L"%s\r\n", PSR_AllocStringInt(-33, 10));

		wprintf(L"%s\r\n", PSR_AllocStringInt(0x01AF, 16));
		wprintf(L"%s\r\n", PSR_AllocStringInt(-0x02AF, 16));
/*
		wprintf(L"%s\r\n", PSR_AllocStringUInt(33, 10));
		wprintf(L"%s\r\n", PSR_AllocStringUInt(88, 10));

		wprintf(L"%s\r\n", PSR_AllocStringUInt(33, 8));
		wprintf(L"%s\r\n", PSR_AllocStringUInt(88, 8));

		wprintf(L"%s\r\n", PSR_AllocStringUInt(33, 16));
		wprintf(L"%s\r\n", PSR_AllocStringUInt(88, 16));
		*/
}


void psr_symb_lst_test()
{
		size_t i;
		psrSymbList_t	slst;
		PSR_InitSymbList(&slst);

		for(i = 0; i < 100; ++i)
		{
				const psrSymb_t *p = PSR_CreateSymb(PSR_AllocStringInt((int64_t)i, 10), PSR_NONTERM, 0, PSR_ASSOC_NOASSOC,0);
				PSR_InsertToSymbList(&slst, PSR_CopyNewSymb(p));
				PSR_DestroySymb(p);
		}

		PSR_SortSymbList(&slst);

		for(i = 0; i < slst.count - 1; ++i)
		{
				const psrSymb_t *l, *r;
				l = slst.lst[i];
				r = slst.lst[i + 1];
				AR_ASSERT(PSR_CompSymb(l,r) < 0);
		}
		{
				int_t n;
				const psrSymb_t *key = PSR_CreateSymb(PSR_AllocStringInt((int64_t)33, 10), PSR_NONTERM, 0, PSR_ASSOC_NOASSOC,0);

				n = PSR_BSearchFromSymbList(&slst, key);
				//n = PSR_FindFromSymbList(&slst, key);

				wprintf(L"name == %s\r\n", slst.lst[n]->name);
				getchar();
		}
		
		for(i = 0; i < slst.count; ++i)
		{
				wprintf(L"name == %s\r\n", slst.lst[i]->name);
				PSR_DestroySymb(slst.lst[i]);
		}
		PSR_UnInitSymbList(&slst);
}


void psr_symb_htbl_test()
{
		size_t i;
		psrSymbTbl_t	tbl,tbl2;
		
		psrSymbTblIter_t		iter;
		size_t count;
		psrSymbList_t	lst;
		PSR_InitSymbTbl(&tbl, 10);
		PSR_InitSymbTbl(&tbl2, 10);

		for(i = 0; i < 100; ++i)
		{
				const psrSymb_t *p = PSR_CreateSymb(PSR_AllocStringInt((int64_t)i, 10), PSR_TERM, 0, PSR_ASSOC_NOASSOC,0);
				AR_ASSERT(PSR_InsertToSymbTbl(&tbl, PSR_CopyNewSymb(p)));
				PSR_DestroySymb(p);
		}

		AR_ASSERT(tbl.item_count == 100);
		count = 0;
		for(iter = PSR_FirstFromSymbTbl(&tbl); iter.curr != NULL; PSR_NextFromSymbTbl(&tbl, &iter))
		{
				wprintf(L"name == %s\r\n", iter.curr->symb->name);
				count++;
		}
		AR_ASSERT(count == 100);
		printf("---------------------------------------------\r\n");

		for(i = 0; i < 100; ++i)
		{
				const psrSymb_t *k, *v;
				k = PSR_CreateSymb(PSR_AllocStringInt((int64_t)i, 10), PSR_TERM, 0, PSR_ASSOC_NOASSOC,0);
				
				v = PSR_FindFromSymbTbl(&tbl, k);
				wprintf(L"name == %s\r\n", v->name);
				PSR_DestroySymb(k);

		}
		
		PSR_InitSymbList(&lst);
		
		PSR_SymbTblToSymbList(&tbl, &lst);
		
		printf("---------------------------------------------\r\n");
		
		for(i = 0; i < lst.count; ++i)
		{
				wprintf(L"name == %s\r\n", lst.lst[i]->name);
		}
		

		printf("tbl2---------------------------------------------\r\n");

		PSR_CopySymbTbl(&tbl2, &tbl);

		
		for(i = 0; i < 100; ++i)
		{
				const psrSymb_t *k, *v;
				k = PSR_CreateSymb(PSR_AllocStringInt((int64_t)i, 10), PSR_TERM, 0, PSR_ASSOC_NOASSOC,0);
				
				v = PSR_FindFromSymbTbl(&tbl2, k);
				wprintf(L"name == %s\r\n", v->name);
				PSR_DestroySymb(k);

		}


		PSR_UnInitSymbList(&lst);
		PSR_UnInitSymbTbl(&tbl);
}


void lr_item_tbl_test()
{
		psrLRItemTbl_t tbl,tbl2;
		int_t i,j;
		{
		DWORD beg, end;
		
		PSR_InitLRItemTbl(&tbl);
		PSR_InitLRItemTbl(&tbl2);

		beg = GetTickCount();
		for(i = 0; i <  100; i++)
		{
				for(j = 0; j < 100; ++j)
				{
						psrLRItem_t item;
						PSR_InitLRItem(&item, i,j, PSR_StartSymb);
						PSR_InsertToLRItemTbl(&tbl, &item);
						PSR_UnInitLRItem(&item);
				}
		}

		for(i = 99; i >= 0; --i)
		{
				for(j = 99; j >= 0; --j)
				{
						psrLRItem_t item;
						PSR_InitLRItem(&item, i,j, PSR_StartSymb);
						PSR_InsertToLRItemTbl(&tbl2, &item);
						PSR_UnInitLRItem(&item);
				}
		}

		printf("CompLRItem == %d\r\n", PSR_EqualLRItemTbl(&tbl, &tbl2));
		/*
		for(i = 99; i >= 0; --i)
		{
				for(j = 99; j >= 0; --j)
				{
						psrLRItem_t item;
						PSR_InitLRItem(&item, i,j, PSR_StartSymb);
						AR_ASSERT(PSR_RemoveFromLRItemTbl(&tbl, &item));
						PSR_UnInitLRItem(&item);
				}
		}
		*/
		printf("CompLRItem == %d\r\n", PSR_EqualLRItemTbl(&tbl, &tbl2));

		end = GetTickCount();
		printf("elapsed == %d\r\n", end - beg);
		
		{
				psrLRItemTblIter_t iter = PSR_GetLRItemTblFirst(&tbl);
				beg = GetTickCount();
				
				
				while(iter.curr)
				{
						//printf("%d : %d\r\n", iter.curr->item.rule_id, iter.curr->item.delim);
						
						if(!PSR_GetLRItemTblNext(&tbl, &iter))
						{
								printf("finished\r\n");
								break;
						}
				}

				end = GetTickCount();

				printf("elipsed == %d\r\n", end -beg);
		}
		{

				beg = GetTickCount();
				PSR_ClearLRItemTbl(&tbl2);
				PSR_SwapLRItemTbl(&tbl2, &tbl);
				//printf("CompLRItem == %d\r\n", PSR_EqualLRItemTbl(&tbl, &tbl2));
				end = GetTickCount();
				printf("clear elipsed == %d\r\n", end -beg);
		}
		
		{
				psrLRItem_t key;
				psrLRItem_t *result;
				PSR_InitLRItem(&key, 3,4, PSR_StartSymb);
				result = PSR_FindFromLRItemTbl(&tbl2, &key);
				PSR_UnInitLRItem(&key);
		}
		
		PSR_UnInitLRItemTbl(&tbl);
		PSR_UnInitLRItemTbl(&tbl2);
		}
}



#if(0)

typedef struct __first_record_tag
{
		bool									has_epsilon;
		bool									is_init;
		psrSymbList_t							lst;
}psrFirstRec_t;

void PSR_InitFirstRec(psrFirstRec_t *rec)
{
		AR_memset(rec, 0, sizeof(*rec));
		PSR_InitSymbList(&rec->lst);
		rec->is_init = true;
}

void PSR_UnInitFirstRec(psrFirstRec_t *rec)
{
		PSR_UnInitSymbList(&rec->lst);
		rec->is_init = false;
}


typedef struct __first_table_tag
{
		psrFirstRec_t	*tbl;
		size_t			row;
		size_t			col;
}psrFirstTbl_t;


bool  __calc_first(const psrSymbList_t *body, size_t start_idx, const psrSymbMap_t *first_set, psrSymbList_t *out)
{
		size_t i;
		bool has_epsilon;
		
		AR_ASSERT(body != NULL &&  first_set != NULL && out != NULL);
		PSR_ClearSymbList(out);
		
		has_epsilon = true;

		for(i = start_idx; i < body->count; ++i)
		{
				const psrSymb_t *curr;
				const psrSymbTbl_t *tbl;
				psrSymbTblIter_t iter;
				curr = body->lst[i];
				
				tbl = &(PSR_GetSymbolFromSymbMap(first_set, curr)->tbl);
				
				if(tbl->item_count == 0)continue;
				
				has_epsilon = false;
				for(iter = PSR_FirstFromSymbTbl(tbl); iter.curr; PSR_NextFromSymbTbl(tbl, &iter))
				{
						if(PSR_CompSymb(iter.curr->symb, PSR_EpsilonSymb) == 0)
						{
								has_epsilon = true;
						}else
						{
								if(PSR_FindFromSymbList(out, iter.curr->symb) == -1)
								{
										PSR_InsertToSymbList(out, iter.curr->symb);
								}
						}
				}
				
				if(!has_epsilon)
				{
						return false;
				}
		}
		return true;
}

void PSR_InitFirstTbl(psrFirstTbl_t *tbl, const psrGrammar_t *grammar)
{
		psrSymbMap_t	first_set;
		size_t i,j;
		AR_ASSERT(grammar != NULL && tbl != NULL && grammar->count > 1);

		
		PSR_InitSymbMap(&first_set);
		PSR_CalcFirstSet(grammar, &first_set);

		
		AR_memset(tbl, 0, sizeof(*tbl));
		
		tbl->row = grammar->count;
		tbl->col = 0;

		for(i = 0; i < grammar->count; ++i)
		{
				if(AR_CMP(tbl->col, grammar->rules[i].body.count) < 0)
				{
						tbl->col = grammar->rules[i].body.count;
				}
		}
		tbl->col++;
		tbl->tbl = AR_NEWARR0(psrFirstRec_t, tbl->row * tbl->col * sizeof(psrFirstRec_t));
		
		


		for(i = 0; i < tbl->row; ++i)
		{
				const psrRule_t *rule = &grammar->rules[i];
				
				for(j = 0; j <= rule->body.count; ++j)
				{
						psrFirstRec_t *first_rec = &tbl->tbl[AR_TBL_IDX_R(i, j, tbl->col)];
						PSR_InitFirstRec(first_rec);
						first_rec->has_epsilon = __calc_first(&rule->body, j + 1, &first_set, &first_rec->lst);
				}
		}

		PSR_UnInitSymbMap(&first_set);
}

void PSR_UnInitFirstTbl(psrFirstTbl_t *tbl)
{
		size_t i,j;
		for(i = 0; i < tbl->row; ++i)
		{
				for(j = 0; j < tbl->col; ++j)
				{
						psrFirstRec_t *first_rec = &tbl->tbl[AR_TBL_IDX_R(0, i, tbl->col)];
						if(first_rec->is_init)PSR_UnInitFirstRec(first_rec);
				}
		}
}




static const wchar_t *gmr_pattern = 
L"  %token{\r\n"
L"'(' : 300, ')' : 301, '+' : 302, '-' : 303, '*' : 304, '/' = 305;"
L"';' : 308;"
L"number : 400;"
L"id : 401;"
L"%}\r\n"

L"%prec{\r\n"
//L"%left '+', '-';"
//L"%left '*', '/';"
L"%right UMINUS;"
L"%}\r\n"

L"%rules{\r\n"
L"E -> E '+' E | E '-' E | E '*' E | E '/' E | '('E')' | '-' E %prec UMINUS| number; "

L"%}\r\n"
;



static const wchar_t *gmr_pattern5 = 
L"  %token{\r\n"
L" \"(\" : 300, ')' : 301, \"++\" : 303;"
L"%}\r\n"


L"%rules{\r\n"
L"S -> \"(\" S ')' S | .;"

L"%}\r\n"
;

static void print_str(arString_t *str)
{
		AR_printf(L"%s\n", AR_GetStrString(str));
		AR_ClearString(str);
}

void cache_test()
{
		arString_t		*str;
		psrFirstTbl_t	tbl;
		psrSymbMap_t	first, follow;
		psrGrammar_t grammar;
		size_t i,j;
		PSR_InitGrammar(&grammar);
		
		if(!PSR_ConfigGrammar(&grammar, gmr_pattern))
		{
				AR_ASSERT(false);
				return;
		}
		str = AR_CreateString();
		PSR_PrintGrammar(&grammar, str);
		print_str(str);
		
		{
				

				PSR_InitSymbMap(&first);
				PSR_InitSymbMap(&follow);

				PSR_CalcFirstSet(&grammar, &first);
				PSR_PrintSymbolMap(&first, str);
				AR_AppendFormatString(str, L"-------------------------------------\r\n");
				PSR_CalcFollowSet(&grammar, &follow, &first);
				
				PSR_PrintSymbolMap(&follow, str);
				AR_AppendFormatString(str, L"-------------------------------------\r\n");


				print_str(str);
		}
		
		getchar();
		
		{
				__int64 beg, end;
				beg = GetTime();
				PSR_InitFirstTbl(&tbl, &grammar);
				end = GetTime();

				printf("elipsed == %d\r\n", end - beg);
		}
		
		for(i = 0; i < tbl.row; ++i)
		{
				for(j = 0; j < tbl.col; ++j)
				{
						const psrFirstRec_t *first_rec = &tbl.tbl[AR_TBL_IDX_R(i, j, tbl.col)];
						if(first_rec->is_init)
						{
								printf("-----------------------------------\r\n");
								if(first_rec->lst.count == 0)
								{
										AR_ASSERT(first_rec->has_epsilon);
								}else
								{
										PSR_PrintSymbolList(&first_rec->lst, false, str);
										print_str(str);
								}
						}
				}
		}
		getchar();

		PSR_UnInitFirstTbl(&tbl);

}
#endif






AR_NAMESPACE_END

#endif



#endif
