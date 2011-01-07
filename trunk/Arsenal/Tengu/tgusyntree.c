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


tguTableInit_t*	TGU_CreateTableInitializer(bool_t empty)
{
		return NULL;
}

void			TGU_DestroyTableInitializer(tguTableInit_t *initializer)
{

}


tguInitializer_t*		TGU_CreateInitializer(tguInitializerType_t type)
{
		tguInitializer_t		*initializer;
		
		initializer = AR_NEW0(tguInitializer_t);
		initializer->type = type;

		return initializer;
}

void					TGU_DestroyInitializer(tguInitializer_t *initializer)
{
		AR_ASSERT(initializer != NULL);

		switch(initializer->type)
		{
		default:
				AR_ASSERT(false);
				break;
		case TGU_INIT_EXPR_T:
				if(initializer->expr)
				{
						TGU_DestroyExpr(initializer->expr);
						initializer->expr = NULL;
				}
				break;
		case TGU_INIT_TABLE_T:
				if(initializer->table)
				{
						TGU_DestroyTableInitializer(initializer->table);
						initializer->table = NULL;
				}
				break;
		}
}


/****************************************Expr***************************************************************/


tguExpr_t*		TGU_CreateExpr(tguExprType_t type)
{
		tguExpr_t		*expr;
		
		expr = AR_NEW0(tguExpr_t);
		expr->expr_type = type;
		return expr;
}


void			TGU_DestroyExpr(tguExpr_t *expr)
{
		AR_ASSERT(expr != NULL);

		switch(expr->expr_type)
		{
		default:
				AR_ASSERT(false);
				break;
		case TGU_ET_NULL_CONST:
		case TGU_ET_INT_CONST:
		case TGU_ET_FLOAT_CONST:
				break;
		case TGU_ET_STRING_CONST:
				if(expr->constant.string)
				{
						AR_DEL(expr->constant.string);
						expr->constant.string = NULL;
				}
				break;
		case TGU_ET_VAR:
				TGU_DestroyVar(expr->var);
				expr->var = NULL;
				break;
		case TGU_ET_INDEX:
				if(expr->index_expr.expr)
				{
						TGU_DestroyExpr(expr->index_expr.expr);
						expr->index_expr.expr = NULL;
				}

				if(expr->index_expr.index_expr)
				{
						TGU_DestroyExpr(expr->index_expr.index_expr);
						expr->index_expr.index_expr = NULL;
				}

				break;
		case TGU_ET_FUNC_CALL:
				if(expr->func_call_expr.arg_list)
				{
						tguExpr_t *arg = expr->func_call_expr.arg_list;

						while(arg)
						{
								tguExpr_t *tmp = arg->next;
								TGU_DestroyExpr(arg);
								arg = tmp;
						}
				}

				if(expr->func_call_expr.func_call)
				{
						TGU_DestroyExpr(expr->func_call_expr.func_call);
						expr->func_call_expr.func_call = NULL;
				}
				break;
		case TGU_ET_BINARY:
				if(expr->binary_expr.left != NULL)
				{
						TGU_DestroyExpr(expr->binary_expr.left);
						expr->binary_expr.left = NULL;
				}

				if(expr->binary_expr.right != NULL)
				{
						TGU_DestroyExpr(expr->binary_expr.right);
						expr->binary_expr.right = NULL;
				}
				break;
		case TGU_ET_UNARY:
				if(expr->unary_expr.expr)
				{
						TGU_DestroyExpr(expr->unary_expr.expr);
						expr->unary_expr.expr = NULL;
				}
				break;
		case TGU_ET_CONDITIONAL:
				if(expr->cond_expr.cond)
				{
						TGU_DestroyExpr(expr->cond_expr.cond);
						expr->cond_expr.cond = NULL;
				}

				if(expr->cond_expr.if_true)
				{
						TGU_DestroyExpr(expr->cond_expr.if_true);
						expr->cond_expr.if_true = NULL;
				}

				if(expr->cond_expr.if_false)
				{
						TGU_DestroyExpr(expr->cond_expr.if_false);
						expr->cond_expr.if_false = NULL;
				}

				break;
		case TGU_ET_UNDEF_NAME:
				if(expr->name)
				{
						AR_DEL(expr->name);
						expr->name = NULL;
				}
				break;
		}

		AR_DEL(expr);
}





/********************************************Stmt********************************************************/

tguStmt_t*	TGU_CreateStmt(tguStmtType_t			stmt_type)
{
		tguStmt_t		*stmt;
		
		stmt = AR_NEW0(tguStmt_t);
		stmt->stmt_type = stmt_type;
		return stmt;
}


void		TGU_DestroyStmt(tguStmt_t *stmt)
{
		AR_ASSERT(stmt != NULL);

		switch(stmt->stmt_type)
		{
		default:
				AR_ASSERT(false);
				break;
		case TGU_STT_EMPTY:
		case TGU_STT_CONTINUE:
		case TGU_STT_BREAK:
				break;
		case TGU_STT_EXPR:
				if(stmt->expr)
				{
						TGU_DestroyExpr(stmt->expr);
						stmt->expr = NULL;
				}
				break;
		case TGU_STT_COMPOUND:
				if(stmt->compound_stmt.block)
				{
						TGU_DestroyBlock(stmt->compound_stmt.block);
						stmt->compound_stmt.block = NULL;
				}
				break;
		case TGU_STT_IF:
				if(stmt->if_stmt.expr)
				{
						TGU_DestroyExpr(stmt->if_stmt.expr);
						stmt->if_stmt.expr = NULL;
				}

				if(stmt->if_stmt.true_part)
				{
						TGU_DestroyStmt(stmt->if_stmt.true_part);
						stmt->if_stmt.true_part = NULL;
				}

				if(stmt->if_stmt.false_part)
				{
						TGU_DestroyStmt(stmt->if_stmt.false_part);
						stmt->if_stmt.false_part = NULL;
				}

				break;
		case TGU_STT_WHILE:
		case TGU_STT_DO:
				if(stmt->while_stmt.expr)
				{
						TGU_DestroyExpr(stmt->while_stmt.expr);
						stmt->while_stmt.expr = NULL;
				}

				if(stmt->while_stmt.loop_part)
				{
						TGU_DestroyStmt(stmt->while_stmt.loop_part);
						stmt->while_stmt.loop_part = NULL;
				}
				break;
		case TGU_STT_FOR:
				if(stmt->for_stmt.init_expr)
				{
						TGU_DestroyExpr(stmt->for_stmt.init_expr);
						stmt->for_stmt.init_expr = NULL;
				}

				if(stmt->for_stmt.cond_expr)
				{
						TGU_DestroyExpr(stmt->for_stmt.cond_expr);
						stmt->for_stmt.cond_expr = NULL;
				}

				if(stmt->for_stmt.step_expr)
				{
						TGU_DestroyExpr(stmt->for_stmt.step_expr);
						stmt->for_stmt.step_expr = NULL;
				}

				if(stmt->for_stmt.loop_part)
				{
						TGU_DestroyStmt(stmt->for_stmt.loop_part);
						stmt->for_stmt.loop_part = NULL;
				}

				break;
		case TGU_STT_RETURN:
				if(stmt->return_stmt.expr)
				{
						TGU_DestroyExpr(stmt->return_stmt.expr);
						stmt->return_stmt.expr = NULL;
				}
				break;

		}

		AR_DEL(stmt);
}



/*********************************************Function**************************************************************/


tguFunc_t*		TGU_CreateFunction(const wchar_t *name, const tguBlock_t *parent_block)
{
		tguFunc_t *func;
		AR_ASSERT(name != NULL && AR_wcslen(name) > 0);
		AR_ASSERT(parent_block != NULL);
		
		func = AR_NEW0(tguFunc_t);
		
		func->name = AR_wcsdup(name);
		func->block = TGU_CreateBlock(parent_block);
		return func;
}


void			TGU_DestroyFunction(tguFunc_t *func)
{
		size_t i;
		AR_ASSERT(func != NULL);

		for(i = 0; i < func->param_cnt; ++i)
		{
				AR_ASSERT(func->params[i] != NULL);
				AR_DEL(func->params[i]);
				func->params[i] = NULL;
		}
		
		if(func->params)
		{
				AR_DEL(func->params);
				func->params = NULL;
		}
		
		TGU_DestroyBlock(func->block);
		func->block = NULL;

		AR_DEL(func->name);
		func->name = NULL;
		AR_DEL(func);
}

size_t			TGU_InsertParam(tguFunc_t *func, const wchar_t *param_name)
{
		AR_ASSERT(func != NULL && param_name != NULL && AR_wcslen(param_name) > 0);
		func->params = AR_REALLOC(wchar_t*, func->params, func->param_cnt + 1);
		func->params[func->param_cnt] = AR_wcsdup(param_name);
		return func->param_cnt++;
}


/**********************************************Var*********************************************************/

tguVar_t*		TGU_CreateVar(const wchar_t *name, tguInitializer_t	*initializer)
{
		tguVar_t		*var;
		AR_ASSERT(name != NULL && AR_wcslen(name) > 0);

		var = AR_NEW0(tguVar_t);
		var->name = AR_wcsdup(name);
		var->initializer = initializer;
		var->ref_cnt = 1;
		return var;
}


void			TGU_DestroyVar(tguVar_t	*var)
{
		AR_ASSERT(var != NULL && var->ref_cnt > 0);

		if(--var->ref_cnt != 0)
		{
				return;
		}
		TGU_DestroyInitializer(var->initializer);
		var->initializer = NULL;

		AR_DEL(var->name);
		AR_DEL(var);
}


tguVar_t*		TGU_CopyVar(tguVar_t	*var)
{
		AR_ASSERT(var != NULL);
		AR_ASSERT(var->ref_cnt > 0);
		var->ref_cnt++;
		return var;
}




/************************************************符号管理************************************************************************/



/*******************************************SymbolTable***********************************************************/


tguSymb_t*		TGU_CreateSymb(tguSymbType_t	type, const wchar_t *name)
{
		tguSymb_t *symb;
		AR_ASSERT(name != NULL && AR_wcslen(name) > 0);
		symb = AR_NEW0(tguSymb_t);
		symb->type = type;
		symb->name = AR_wcsdup(name);
		return symb;
}

void			TGU_DestroySymb(tguSymb_t *symb)
{
		AR_ASSERT(symb != NULL);

		if(symb->type == TGU_SYMB_VAR_T)
		{
				AR_ASSERT(symb->var != NULL);
				TGU_DestroyVar(symb->var);
				symb->var = NULL;
		}else if(symb->type == TGU_SYMB_FUNC_T)
		{
				AR_ASSERT(symb->func != NULL);
				TGU_DestroyFunction(symb->func);
				symb->func = NULL;
		}else
		{
				AR_ASSERT(false);
		}

		AR_DEL(symb->name);
		AR_DEL(symb);
}


tguSymbTbl_t*	TGU_CreateSymbTable()
{
		tguSymbTbl_t *tbl;
		tbl = AR_NEW0(tguSymbTbl_t);
		return tbl;
}

void			TGU_DestroySymbTable(tguSymbTbl_t *tbl)
{
		size_t i;
		AR_ASSERT(tbl != NULL);

		for(i = 0; i < TENGU_SYMB_BUCKET_SIZE; ++i)
		{
				tguSymb_t		*symb = tbl->tbl[i];

				while(symb)
				{
						tguSymb_t *next = symb->next;
						TGU_DestroySymb(symb);
						symb = next;
				}
		}
}


#define __hash_name(_name)		(((size_t)(_name)) % TENGU_SYMB_BUCKET_SIZE)

tguSymb_t*		TGU_FindSymb(tguSymbTbl_t *tbl, const wchar_t *name)
{
		size_t index;
		tguSymb_t *symb;
		AR_ASSERT(tbl != NULL && name != NULL);
		
		index = __hash_name(name);
		
		symb = tbl->tbl[index];

		while(symb && AR_wcscmp(symb->name, name) != 0)
		{
				symb = symb->next;
		}

		return symb;
}


tguSymb_t*		TGU_InsertVarToSymbTable(tguSymbTbl_t *tbl, const wchar_t *name, tguVar_t		*var)
{
		size_t index;
		tguSymb_t *new_symb;
		AR_ASSERT(tbl != NULL && name != NULL && var != NULL);

#if defined(AR_DEBUG)
		AR_ASSERT(TGU_FindSymb(tbl, name) == NULL);
#endif
		
		new_symb = TGU_CreateSymb(TGU_SYMB_VAR_T, name);
		new_symb->var = var;
		
		index = __hash_name(name);
		
		new_symb->next = tbl->tbl[index];
		tbl->tbl[index] = new_symb;
		tbl->item_cnt++;
		return new_symb;
}


tguSymb_t*		TGU_InsertFuncToSymbTable(tguSymbTbl_t *tbl, const wchar_t *name, tguFunc_t		*func)
{
		
		size_t index;
		tguSymb_t *new_symb;
		AR_ASSERT(tbl != NULL && name != NULL && func != NULL);

#if defined(AR_DEBUG)
		AR_ASSERT(TGU_FindSymb(tbl, name) == NULL);
#endif
		
		new_symb = TGU_CreateSymb(TGU_SYMB_FUNC_T, name);
		new_symb->func = func;
		
		index = __hash_name(name);
		
		new_symb->next = tbl->tbl[index];
		tbl->tbl[index] = new_symb;
		tbl->item_cnt++;
		return new_symb;
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

		if(tmp != sub)
		{
				return false;
		}
		
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
				ret = TGU_FindSymb(curr->symb_tbl, name);
				
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

		block->symb_tbl = TGU_CreateSymbTable();

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
		
		TGU_DestroySymbTable(block->symb_tbl);
		block->symb_tbl = NULL;
		
		AR_DEL(block);

}



/*********************************************************************/

tguSyntaxTree_t*		TGU_CreateSyntaxTree(const tguBlock_t *build_in_block)
{
		tguSyntaxTree_t	*tree;
		
		AR_ASSERT(build_in_block != NULL);

		tree = AR_NEW0(tguSyntaxTree_t);
		tree->has_error = false;
		tree->global_block = TGU_CreateBlock(build_in_block);
		return tree;
}


void					TGU_DestroySyntaxTree(tguSyntaxTree_t	*tree)
{
		AR_ASSERT(tree != NULL);
		
		if(tree->global_block)
		{
				TGU_DestroyBlock(tree->global_block);
		}

		AR_DEL(tree);

}





AR_NAMESPACE_END






