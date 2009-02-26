
#include "..\Parser\parser.h"
#include "..\Parser\lr.h"

#if(0)
extern void __print_bnf(const psrGrammar_t *gmr);

extern void __print_symb(const psrSymb_t *symb);


extern void __print_symblist(const psrSymb_t *symb);


void __print_lr0_item(const psrLRItem_t *item)
{
		if(item != NULL)
		{
				printf("rule_id == %d : delim == %d\n", item->rule_id, item->delim);
		}
}
void __print_lr0_itemlist(const psrLRItem_t *head)
{
		if(head == NULL)
		{
				printf("null ");
				return;
		}
		while(head != NULL)
		{
				__print_lr0_item(head);
				head = head->next;
		}

}

void __print_lr0_item_name(const psrGrammar_t *gmr,const psrLRItem_t *item)
{
		if(item != NULL)
		{
				const psrSymb_t *head, *body;
				const psrSymb_t *delim, *curr;
				const wchar_t *str;

				head = PSR_GetRuleHeadByRuleID(gmr,item->rule_id);
				body = PSR_GetRuleBodyByRuleID(gmr,item->rule_id);
				AR_ASSERT(head != NULL && body != NULL);

				delim = PSR_IndexOfSymbList(body, item->delim);
				
				__print_symb(head);
				printf("->");
				for(curr = body; curr != delim; curr = curr->next)
				{
						__print_symb(curr);
				}
				printf(".");
				
				for(delim; delim != NULL; delim = delim->next)
				{
						__print_symb(delim);
				}
				
				printf(" : ");
				__print_symblist(item->symb);
				printf("\n");

				if(head->asso == PSR_ASSO_LEFT)
				{
						str = L"Left";
				}else if(head->asso == PSR_ASSO_RIGHT)
				{
						str = L"Right";
				}else
				{
						str = L"NoAssoc";
				}

				
				//wprintf(L"  prio == %d assoc == %s  ",head->priority, str);
				

				
		}
}

void __print_lr0_itemlist_name(const psrGrammar_t *gmr,const psrLRItem_t *head)
{
		if(head == NULL)
		{
				printf("null ");
				return;
		}
		while(head != NULL)
		{
				__print_lr0_item_name(gmr,head);
				head = head->next;
		}

}




static const wchar_t *test_input = 
L"%token{\r\n"
//L"'(' : 259;\r\n"
//L"')'->260;\r\n"
//L"'+' : 280; '-' = 281; '*' : 282; '/' : 283;\r\n"
//L"a  : 284, b : 285, c :286, d : 287, e : 300;"
L"'*' : 282;"
L"id : 288;"
L"'=' : 289;"
L"%}\r\n"

L"%prec{\r\n"

L"%right		'=';"
//L"%left			'+' '-';\r\n"
//L"%left			'*' '/';\r\n"
//L"%right		RIGHT_MINUE;\r\n"

L"%}\r\n"

L"%rules{\r\n"
//L"E	->	E '+' E | E '-' E|E '*' E | E '/' E | '(' E ')'|'-'E %prec RIGHT_MINUE;"
//L"E -> id | E '-' E;"
//L"/*A -> '('A')'|a;*/"
L"S-> L '=' R | R;"
L"L -> id | '*' R /*%prec RIGHT_MINUE*/;"
L"R ->L;"

L"%}\r\n"
;


AR_API void lr0_test1()
{

		size_t i;
		psrGrammar_t *gmr;
		lrItemsCollection_t *clt;

		gmr = PSR_BuildGrammar(test_input);
		AR_ASSERT(gmr != NULL);

		__print_bnf(gmr);

		getchar();
		
		clt = LR0_CreateItemsCollection(gmr);

		for(i = 0; i < clt->count; ++i)
		{
				printf("itemset[%d]:\n", i);
				__print_lr0_itemlist_name(gmr,clt->items[i]);
				getchar();
		}

		printf("delete non-kernel item:\n");
		LR_ClearNonKernelItems(clt);
		
		
		for(i = 0; i < clt->count; ++i)
		{
				printf("itemset[%d]:\n", i);
				__print_lr0_itemlist_name(gmr,clt->items[i]);
				getchar();
		}
}


AR_API void lr1_test()
{
		size_t i;
		psrGrammar_t *gmr;
		lrItemsCollection_t *clt;

		gmr = PSR_BuildGrammar(test_input);
		AR_ASSERT(gmr != NULL);

		__print_bnf(gmr);

		getchar();
		
		clt = LR1_CreateItemsCollection(gmr);

		for(i = 0; i < clt->count; ++i)
		{
				printf("itemset[%d]:\n", i);
				__print_lr0_itemlist_name(gmr,clt->items[i]);
				getchar();
		}

		printf("delete non-kernel item:\n");
		LR_ClearNonKernelItems(clt);

		
		for(i = 0; i < clt->count; ++i)
		{
				printf("itemset[%d]:\n", i);
				__print_lr0_itemlist_name(gmr,clt->items[i]);
				getchar();
		}

}



AR_API void lalr_test()
{
		size_t i;
		psrGrammar_t *gmr;
		lrItemsCollection_t *clt;

		gmr = PSR_BuildGrammar(test_input);
		AR_ASSERT(gmr != NULL);

		__print_bnf(gmr);
		
		//getchar();
		
		clt = LALR_CreateItemsCollection(gmr);
		
		

		for(i = 0; i < clt->count; ++i)
		{
				printf("itemset[%d]:\n", i);
				__print_lr0_itemlist_name(gmr,clt->items[i]);
				getchar();
		}

		printf("delete non-kernel item:\n");
		LR_ClearNonKernelItems(clt);

		
		for(i = 0; i < clt->count; ++i)
		{
				printf("itemset[%d]:\n", i);
				__print_lr0_itemlist_name(gmr,clt->items[i]);
				getchar();
		}


}

static const wchar_t *test_input2 = 
L"%token{\r\n"
L"'(' : 259; ')' : 260;\r\n"
L"%}\r\n"

L"%rules{\r\n"
L"S -> '(' ')'|'(' ')'|'('S')'S|.;"

L"%}\r\n"
;
//psrActionTable_t* PSR_BuildActionTable_SLR(const psrGrammar_t *grammar)
AR_API void action_table_test()
{
		
		psrActionTable_t	*tbl;
		size_t i;
		psrGrammar_t *gmr;

		gmr = PSR_BuildGrammar(test_input2);
		AR_ASSERT(gmr != NULL);

		__print_bnf(gmr);
		getchar();
		tbl = PSR_BuildActionTable_SLR(gmr);
		for(i = 0; i < tbl->term_count; ++i)__print_symblist(&tbl->term_set[i]);
		
		printf("\n");
		for(i = 0; i < tbl->row; i++)
		{
				size_t j;
				for(j = 0; j < tbl->term_count; ++j)
				{
						const psrAction_t *pact, *tmp;
						const psrSymb_t *symb;
						symb = &tbl->term_set[j];
						pact = PSR_GetAction(tbl, i, symb);
						if(pact->next != NULL)
						{
								printf("conflict: ");
								tmp = pact;
								while(tmp != NULL)
								{
									//	assert(0);
										if(tmp->type == PSR_SHIFT)
										{
												printf("shift ");
										}else
										{
												printf("reduece ");
										}
										printf("%d : prio == %d ", tmp->item.rule_id, tmp->priority);
										tmp = tmp->next;
								}
								
								
								//break;
						}
						switch(pact->type)
						{
						case PSR_ACCEPT:
								printf("accept ");
								break;
						case PSR_REDUCE:
								printf("reduce(%d) ",pact->reduce_id);
								break;
						case PSR_SHIFT:
								printf("shift(%d) ",pact->shift_to);
								break;
						default:
								printf("       ");
								break;
						}
				}
				printf("\n");
		}
		printf("\n");

		for(i = 0; i < tbl->nonterm_count; ++i)__print_symblist(&tbl->non_term_set[i]);
		printf("\n");

		for(i = 0; i < tbl->goto_row; ++i)
		{
				size_t j;
				for(j = 0; j < tbl->nonterm_count; ++j)
				{
						printf("%d ", PSR_GetState(tbl,i, &tbl->non_term_set[j]));
				}
				printf("\n");
		}
		printf("\n");

		getchar();



		PSR_DestroyActionTable(tbl);
}


#endif




