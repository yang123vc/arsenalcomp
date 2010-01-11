#include "test.h"



AR_NAMESPACE_BEGIN

#if defined(__LIB)

extern void parser_test();
extern void com_test();
extern void str_test();
extern void expr_test();
extern void lex_test();
extern void lex_test20();
extern void lr_dfa_test();
extern void action_tbl_test();
extern void parser_test();
extern void lex_test2();
extern void lex_test3();
extern void lex_test4();
extern void trans_char_test();

extern void str_tbl_test();

extern void psr_symb_lst_test();

extern void psr_symb_htbl_test();

extern void lr_item_tbl_test();

extern void PSR_Init();
extern void PSR_UnInit();

extern void cache_test();
extern void rule_test();
extern void grammar_test2();
extern void calc_test();
extern void calc_test1();

extern void db_test();

extern void pcg_test();

extern void pcg_test2();

extern void create_file_test();

extern void trie_tree_test();

extern void parser_test();
extern void parser_test_lex();

extern void rgx_test();

extern void calc_test3();

extern void parser_perf_test();

#endif






void AR_Test()
{
#if defined(__LIB)
		//com_test();

		//trie_tree_test();
		//expr_test();
		//lex_test();
		//lex_test2();
		//lex_test3();
		//lex_test4();
		
		//lr_dfa_test();
		//action_tbl_test();
		//parser_test();
		//str_test();
		//str_tbl_test();
		//rule_test();
		//grammar_test2();
		//psr_symb_lst_test();
		
		//psr_symb_htbl_test();

		//lr_item_tbl_test();
		//cache_test();
		//calc_test1();
		//calc_test();
		//calc_test3();

		//db_test();

		//pcg_test();

		//pcg_test2();

		//trans_char_test();
		
		//lex_test();
		//parser_test();

		//parser_perf_test();
		
		//rgx_test();
		
		calc_test();

#else
		AR_printf(L"dll version\r\n");
#endif
}




AR_NAMESPACE_END



