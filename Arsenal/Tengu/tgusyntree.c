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


#include "tgusyntree.h"
#include "tguutility.h"

AR_NAMESPACE_BEGIN

/****************************************Expr***************************************************************/


tguExpr_t*		TGU_CreateExpr(tguExprType_t type)
{
		return NULL;
}


void			TGU_DestroyExpr(tguExpr_t *expr)
{
		AR_ASSERT(expr != NULL);
}





/********************************************Stmt********************************************************/

tguStmt_t*	TGU_CreateStmt(tguStmtType_t			stmt_type)
{
		return NULL;
}


void		TGU_DestroyStmt(tguStmt_t *stmt)
{
		AR_ASSERT(stmt != NULL);
}


/**********************************************Var*********************************************************/


tguVar_t*		TGU_CreateVar(const wchar_t *name)
{
		tguVar_t		*var;
		AR_ASSERT(name != NULL && AR_wcslen(name) > 0);

		var = AR_NEW0(tguVar_t);
		var->name = AR_wcsdup(name);
		var->ref_cnt = 1;
		return var;
}


void			TGU_DestroyVar(tguVar_t	*var)
{
		AR_ASSERT(var != NULL);
}


tguVar_t*		TGU_CopyVar(tguVar_t	*var)
{
		AR_ASSERT(var != NULL);
		AR_ASSERT(var->ref_cnt > 0);

		var->ref_cnt++;
		return var;
}


/*******************************************SymbolTable***********************************************************/

void			TGU_CreateSymbTable(tguSymbTbl_t *tbl)
{
		AR_ASSERT(tbl != NULL);
		AR_memset(tbl, 0, sizeof(*tbl));
}

void			TGU_DestroySymbTable(tguSymbTbl_t *tbl)
{
		AR_ASSERT(tbl != NULL);
}

tguSymb_t*		TGU_FindSymb(tguSymbTbl_t *tbl, const wchar_t *name)
{
		AR_ASSERT(tbl != NULL && name != NULL);
		return NULL;
}

tguSymb_t*		TGU_InsertVarToSymbTable(tguSymbTbl_t *tbl, const wchar_t *name, tguVar_t		*var)
{
		AR_ASSERT(tbl != NULL && name != NULL && var != NULL);
		return NULL;
}

tguSymb_t*		TGU_InsertFuncToSymbTable(tguSymbTbl_t *tbl, const wchar_t *name, tguFunc_t		*func)
{
		AR_ASSERT(tbl != NULL && name != NULL && func != NULL);
		return NULL;
}



/*********************************************Block******************************************************/

void			TGU_InsertVarToBlock(tguBlock_t	*block, tguVar_t*	var)
{
		tguSymb_t		*symb;
		AR_ASSERT(block != NULL && var != NULL);
		AR_ASSERT(var->name != NULL);

#if defined(AR_DEBUG)
		AR_ASSERT(TGU_FindFromBlockByName(block, var->name, true) == NULL);
#endif
		symb = TGU_InsertVarToSymbTable(block->symb_tbl, var->name, var);
}

void			TGU_InsertStmtToBlock(tguBlock_t	*block, tguStmt_t	*stmt)
{
		AR_ASSERT(block != NULL && stmt != NULL);
		
		if(block->stmt_cnt == block->stmt_cap)
		{
				block->stmt_cap += 2;
				block->stmt_cap *= 2;
				block->stmts = AR_REALLOC(tguStmt_t*, block->stmts, block->stmt_cap);
		}

		block->stmts[block->stmt_cnt] = stmt;
		block->stmt_cnt++;
}


void			TGU_InsertSubBlockToBlock(tguBlock_t	*block, tguBlock_t	*sub)
{
		tguBlock_t		*tmp;
		AR_ASSERT(block != NULL && sub != NULL);
		AR_ASSERT(block != sub);

		/*
		如果sub->next不为空，则证明此block已被插入到其它Block中
		*/
		AR_ASSERT(sub->next == NULL && sub->parent == NULL);

		if(block->sub_blocks == NULL)
		{
				block->sub_blocks = sub;
				sub->next = NULL;
		}else
		{
				for(tmp = block->sub_blocks; tmp->next != NULL; tmp = tmp->next);
				tmp->next = sub;
				sub->next = NULL;
		}
		sub->parent = block;
}

bool_t			TGU_RemoveSubBlockFromBlock(tguBlock_t	*block, tguBlock_t	*sub)
{
		tguBlock_t		*tmp, *prev;
		AR_ASSERT(block != NULL && sub != NULL);
		AR_ASSERT(block != sub);
		
		for(prev = NULL, tmp = block->sub_blocks; tmp != NULL; tmp = tmp->next)
		{
				if(tmp == sub)
				{
						break;
				}else
				{
						prev = tmp;
				}
		}

		if(tmp != sub)return false;
		
		if(prev == NULL)
		{
				AR_ASSERT(sub == block->sub_blocks);
				block->sub_blocks = block->sub_blocks->next;
		}else
		{
				prev->next = sub->next;
		}
		
		sub->next = NULL;
		sub->parent = NULL;
		return true;
}


tguSymb_t*		TGU_FindFromBlockByName(tguBlock_t	*block, const wchar_t *name, bool_t current_block)
{
		tguSymb_t*		ret;
		tguBlock_t		*curr;
		AR_ASSERT(block != NULL && name != NULL);

		ret = NULL;
		curr = block;

		do{
				if(curr->symb_tbl)
				{
						ret = TGU_FindSymb(curr->symb_tbl, name);
				}

				if(ret != NULL)
				{
						return ret;
				}
				
				curr = block->parent;

		}while(!current_block && curr);

		return ret;
}




tguBlock_t*		TGU_CreateBlock(const tguBlock_t	*parent)
{
		tguBlock_t		*block;
		
		block = AR_NEW(tguBlock_t);

		block->begin.col = 0;
		block->begin.linenum = 0;

		block->end.col = 0;
		block->end.linenum = 0;

		block->symb_tbl = NULL;

		block->stmts = NULL;
		block->stmt_cnt = 0;
		block->stmt_cap = 0;

		
		block->next = NULL;
		block->parent = NULL;
		block->sub_blocks = NULL;
		
		if(parent)
		{
				TGU_InsertSubBlockToBlock((tguBlock_t*)parent, block);
		}

		return block;
}


void			TGU_DestroyBlock(tguBlock_t	*block)
{
		size_t i;
		tguBlock_t *parent;
		bool_t		find;
		AR_ASSERT(block != NULL);
		
		parent = block->parent;
		block->parent = NULL;

		find = TGU_RemoveSubBlockFromBlock(parent, block);
		
		AR_ASSERT(find);

		
		while(block->sub_blocks)
		{
				AR_ASSERT(block->sub_blocks->parent == block);
				TGU_DestroyBlock(block->sub_blocks);
		}

		

		

		for(i = 0; i < block->stmt_cnt; ++i)
		{
				TGU_DestroyStmt(block->stmts[i]);
		}
		
		if(block->stmts)AR_DEL(block->stmts);

		if(block->symb_tbl)
		{
				TGU_DestroySymbTable(block->symb_tbl);
				block->symb_tbl = NULL;
		}

		AR_DEL(block);

}



/*********************************************************************/

tguSyntaxTree_t*		TGU_CreateSyntaxTree(const wchar_t	*sources_name, const tguBlock_t *build_in_block)
{
		tguSyntaxTree_t	*tree;
		
		AR_ASSERT(build_in_block != NULL);

		tree = AR_NEW0(tguSyntaxTree_t);
		tree->has_error = false;
		tree->sources_name = AR_wcsdup(sources_name == NULL ? L"" : sources_name);
		tree->global_block = TGU_CreateBlock(build_in_block);
		return tree;
}


void					TGU_DestroySyntaxTree(tguSyntaxTree_t	*tree)
{
		AR_ASSERT(tree != NULL);

		if(tree->sources_name)
		{
				AR_DEL(tree->sources_name);
		}
		
		if(tree->global_block)
		{
				TGU_DestroyBlock(tree->global_block);
		}

		AR_DEL(tree);

}





AR_NAMESPACE_END






