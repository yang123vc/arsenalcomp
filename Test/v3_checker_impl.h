
 
/******************************************************************************************/
 

 
enum{
TOK_DELIM_ID = 257,
TOK_CONSTANT = 258,
TOK_FILE_VAR = 259,
TOK_VAR = 260,
TOK_NAME = 261,
TOK_LE = 262,
TOK_GE = 263,
TOK_EQ = 264,
TOK_NE = 265,
TOK_ANDAND = 266,
TOK_OROR = 267,
TOK_LESS = 268,
TOK_GREATER = 269,
TOK_NOT = 270,
TOK_L_PAREN = 271,
TOK_R_PAREN = 272,
TOK_COMMA = 273,
};


typedef enum
{
	VARIABLE_T,
	CONSTANT_T,
	NAME_T,
	EXPR_T,
	CALL_T,
}checkerNodeType_t;


struct __checker_expression_node_tag;
typedef struct __checker_expression_node_tag	exprNode_t;

typedef enum
{
	CHECKER_EXPR_OR,
	CHECKER_EXPR_AND,

	CHECKER_EXPR_LESS,
	CHECKER_EXPR_GREATER,
	CHECKER_EXPR_LE,
	CHECKER_EXPR_GE,

	CHECKER_EXPR_EQ,
	CHECKER_EXPR_NE,

	CHECKER_EXPR_NOT,

}exprOperator_t;

#define IsBinaryOperator(_op)	((_op) < CHECKER_EXPR_NOT)

typedef struct __var_node_tag
{
	ar_bool_t	is_file_check;
	wchar_t	name[1024];
}var_t;


typedef struct __constant_node_tag
{
	wchar_t	constant[1024];			//根据需要解析此字符串
}constant_t;

typedef struct __name_node_tag
{
	wchar_t	name[1024];				//根据此名称调用函数
}name_t;


typedef struct __expr_node_tag
{
	exprOperator_t	op;
	exprNode_t		*left;
	exprNode_t		*right;
}expr_t;

typedef struct __call_node_tag
{
	wchar_t	name[256];		//最大256个名称
	exprNode_t	*params[32];		//最大32个参数
	size_t		param_cnt;
}call_t;


struct __checker_expression_node_tag
{
	checkerNodeType_t	t;
	size_t			line;
	size_t			col;

	union{
		var_t			var;
		constant_t		constant;
		name_t			name;
		expr_t			expr;
		call_t			call;
	};
};


exprNode_t*	CreateExprNode(checkerNodeType_t	t)
{
	exprNode_t	*node;

	node = AR_NEW0(exprNode_t);

	if(node == NULL)
	{
		AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
		return NULL;
	}

	node->t = t;
	return node;
}

void		DestroyExprNode(exprNode_t *node)
{
	AR_ASSERT(node != NULL);

	switch(node->t)
	{
	case 	VARIABLE_T:
		break;
	case 	CONSTANT_T:
		break;
	case 	NAME_T:
		break;
	case	EXPR_T:
	{
		if(node->expr.left)
		{
			DestroyExprNode(node->expr.left);
			node->expr.left = NULL;
		}

		if(node->expr.right)
		{
			DestroyExprNode(node->expr.right);
			node->expr.right= NULL;
		}
	}
		break;
	case	CALL_T:
	{
		size_t i;
		for(i = 0; i < node->call.param_cnt; ++i)
		{
			if(node->call.params[i] != NULL)
			{
				DestroyExprNode(node->call.params[i]);
				node->call.params[i] = NULL;
			}
		}
	}
		break;
	default:
		AR_ASSERT(false);
	}
	
	if(node)
	{
		AR_DEL(node);
		node = NULL;
	}
}




 


/*
CreateOperatorExprNode
*/
 
	
	exprNode_t*	CreateOperatorExprNode(const psrToken_t *tok)
	{
		exprNode_t	*expr_node;
		AR_ASSERT(tok != NULL);

		expr_node = NULL;
		expr_node =	CreateExprNode(EXPR_T);

		if(expr_node == NULL)
		{
			AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
			return NULL;
		}

		expr_node->line = tok->line;
		expr_node->col = tok->col;

		switch(tok->term_val)
		{
		case TOK_LE:
				expr_node->expr.op = CHECKER_EXPR_LE;
				break;
		case TOK_GE:
				expr_node->expr.op = CHECKER_EXPR_GE;
				break;
		case TOK_EQ:
				expr_node->expr.op = CHECKER_EXPR_EQ;
				break;
		case TOK_NE:
				expr_node->expr.op = CHECKER_EXPR_NE;
				break;
		case TOK_ANDAND:
				expr_node->expr.op = CHECKER_EXPR_AND;
				break;
		case TOK_OROR:
				expr_node->expr.op = CHECKER_EXPR_OR;
				break;
		case TOK_LESS:
				expr_node->expr.op = CHECKER_EXPR_LESS;
				break;
		case TOK_GREATER:
				expr_node->expr.op = CHECKER_EXPR_GREATER;
				break;
		case TOK_NOT:
				expr_node->expr.op = CHECKER_EXPR_NOT;
				break;
		default:
		{
				wchar_t tmp[128];
				if(tok->term_val != 0)
				{
						AR_wcsncpy(tmp, tok->str, AR_MIN(tok->str_cnt, 32));
				}else
				{
						AR_wcscpy(tmp, L"%EOI");
				}

				AR_error(AR_ERR_WARNING, L"invalid token type : %ls\r\n", tmp);
				return NULL;
		}
		}
		return expr_node;
	}

 

static const wchar_t *__g_lex_name[] = {
L"delim = [ \\r\\n\\t]",
L"comment = /\\*([^\\*]|\\*+[^\\*/])*\\*+/",
L"comment_line = //[^\\n]*\\n",
L"skip_lexem = {delim}|{comment}|{comment_line}",
L"escape_seq = (\\\\(\\x22|\\x27))",
L"string_dq = (\\x22({escape_seq}|[^\\x22])*\\x22)",
L"string_sq = \\x27({escape_seq}|[^\\x27])*\\x27",
L"file_variable = %fver_([^%])+%",
L"variable = %([^%])+%",
L"letter = [A-Z_a-z\\x{0800}-\\x{4E00}\\x{4E00}-\\x{9FA5}\\x{3130}-\\x{318F}\\x{AC00}-\\x{D7AF}]",
L"digit = [0-9]",
L"number = {digit}+"
};

#define __NAME_COUNT__ ((size_t)12)

static psrRetVal_t AR_STDCALL on_constant(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL on_file_variable(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL on_variable(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL on_name(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL on_operator(const psrToken_t *tok,void *ctx);


static psrRetVal_t AR_STDCALL autoreturn_null(const psrToken_t *tok,void *ctx);







static struct {const wchar_t *name;
size_t tokval;
size_t lex_prec;
const wchar_t *regex;
ar_bool_t skip;
psrTermFunc_t leaf;
}__g_term_pattern[] =  {
{NULL, TOK_DELIM_ID,1, L"{skip_lexem}+", true, NULL},
{L"CONSTANT", TOK_CONSTANT, 2, L"{string_sq}|{string_dq}", false, on_constant},
{L"FILE_VAR", TOK_FILE_VAR, 2, L"{file_variable}", false, on_file_variable},
{L"VAR", TOK_VAR, 2, L"{variable}", false, on_variable},
{L"NAME", TOK_NAME, 2, L"{letter}({letter}|{digit})*", false, on_name},
{L"<=", TOK_LE, 1, L"\"<=\"", false, on_operator},
{L">=", TOK_GE, 1, L"\">=\"", false, on_operator},
{L"==", TOK_EQ, 1, L"\"==\"", false, on_operator},
{L"!=", TOK_NE, 1, L"\"!=\"", false, on_operator},
{L"&&", TOK_ANDAND, 1, L"\"&&\"", false, on_operator},
{L"||", TOK_OROR, 1, L"\"||\"", false, on_operator},
{L"<", TOK_LESS, 0, L"\"<\"", false, on_operator},
{L">", TOK_GREATER, 0, L"\">\"", false, on_operator},
{L"!", TOK_NOT, 0, L"\"!\"", false, on_operator},
{L"(", TOK_L_PAREN, 0, L"\"(\"", false, autoreturn_null},
{L")", TOK_R_PAREN, 0, L"\")\"", false, autoreturn_null},
{L",", TOK_COMMA, 0, L"\",\"", false, autoreturn_null},
{L"CFG_LEXVAL_EOI", 0, 2, L"$", false, NULL}
};

#define __TERM_COUNT__ ((size_t)18)

static struct {const wchar_t *name; size_t tokval; size_t prec_level; psrAssocType_t	assoc;}__g_prec_pattern[] =  {
{L"||", TOK_OROR,1, PARSER_ASSOC_LEFT},
{L"&&", TOK_ANDAND,2, PARSER_ASSOC_LEFT},
{L">", TOK_GREATER,3, PARSER_ASSOC_LEFT},
{L"<", TOK_LESS,3, PARSER_ASSOC_LEFT},
{L"<=", TOK_LE,3, PARSER_ASSOC_LEFT},
{L">=", TOK_GE,3, PARSER_ASSOC_LEFT},
{L"==", TOK_EQ,4, PARSER_ASSOC_LEFT},
{L"!=", TOK_NE,4, PARSER_ASSOC_LEFT},
{L"NOTEXPR", 274,5, PARSER_ASSOC_RIGHT}
};

#define __PREC_COUNT__ ((size_t)9)

/*expression	:	expr */
/*expr	:	unary_expr */
/*expr	:	call */
/*expr	:	term */
/*term	:	VAR */
/*term	:	CONSTANT */
/*term	:	FILE_VAR */
/*param	:	CONSTANT */
/*param	:	VAR */
/*param	:	FILE_VAR */
static psrRetVal_t AR_STDCALL autoreturn_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expr	:	( expr ) */
static psrRetVal_t AR_STDCALL autoreturn_1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*expr	:	expr && expr */
/*expr	:	expr || expr */
/*expr	:	expr <= expr */
/*expr	:	expr >= expr */
/*expr	:	expr < expr */
/*expr	:	expr > expr */
/*expr	:	expr == expr */
/*expr	:	expr != expr */
static psrRetVal_t AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*unary_expr	:	! expr */
static psrRetVal_t AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*call	:	NAME ( params ) */
static psrRetVal_t AR_STDCALL on_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	params , param */
static psrRetVal_t AR_STDCALL insert_new_params(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	param */
static psrRetVal_t AR_STDCALL on_call_params_expr(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

/*params	:	 */
static psrRetVal_t AR_STDCALL on_empty_call_params(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);




static struct { const wchar_t	*rule; const wchar_t	*prec_token; psrRuleFunc_t	handler; size_t	auto_ret; } __g_rule_pattern[] = {
{L"expression  :  expr ", NULL, autoreturn_0, 0},
{L"expr  :  ( expr ) ", NULL, autoreturn_1, 0},
{L"expr  :  expr && expr ", NULL, on_binary_expression, 0},
{L"expr  :  expr || expr ", NULL, on_binary_expression, 0},
{L"expr  :  expr <= expr ", NULL, on_binary_expression, 0},
{L"expr  :  expr >= expr ", NULL, on_binary_expression, 0},
{L"expr  :  expr < expr ", NULL, on_binary_expression, 0},
{L"expr  :  expr > expr ", NULL, on_binary_expression, 0},
{L"expr  :  expr == expr ", NULL, on_binary_expression, 0},
{L"expr  :  expr != expr ", NULL, on_binary_expression, 0},
{L"expr  :  unary_expr ", NULL, autoreturn_0, 0},
{L"expr  :  call ", NULL, autoreturn_0, 0},
{L"expr  :  term ", NULL, autoreturn_0, 0},
{L"unary_expr  :  ! expr ", L"NOTEXPR", on_unary_expression, 0},
{L"term  :  VAR ", NULL, autoreturn_0, 0},
{L"term  :  CONSTANT ", NULL, autoreturn_0, 0},
{L"term  :  FILE_VAR ", NULL, autoreturn_0, 0},
{L"call  :  NAME ( params ) ", NULL, on_call_expression, 0},
{L"params  :  params , param ", NULL, insert_new_params, 0},
{L"params  :  param ", NULL, on_call_params_expr, 0},
{L"params  :   ", NULL, on_empty_call_params, 0},
{L"param  :  CONSTANT ", NULL, autoreturn_0, 0},
{L"param  :  VAR ", NULL, autoreturn_0, 0},
{L"param  :  FILE_VAR ", NULL, autoreturn_0, 0}
};

#define __RULE_COUNT__ ((size_t)24)
#define START_RULE L"expression"


static lex_t*	__build_lex()													
{																				
		lex_t	*lex;															
		size_t i;																
		lex = Lex_Create();														
		if(lex == NULL)
		{
				return NULL;
		}

		for(i = 0; i < __NAME_COUNT__; ++i)										
		{
				arStatus_t status;
				status = Lex_Insert(lex, __g_lex_name[i]);
				if(status != AR_S_YES)							
				{
					AR_error(AR_ERR_WARNING, L"failed to build lexer : name '%ls'!\r\n", __g_lex_name);
					Lex_Destroy(lex);										
					AR_ASSERT(false);										
					return NULL;									
				}
		}

		for(i = 0; i < __TERM_COUNT__; ++i)										
		{																		
				lexAction_t		act;											
				arStatus_t status;
				act.is_skip		=		__g_term_pattern[i].skip;				
				act.priority	=		__g_term_pattern[i].lex_prec;			
				act.value		=		__g_term_pattern[i].tokval;		

				
				status = Lex_InsertRule(lex, __g_term_pattern[i].regex, &act);
				if(status != AR_S_YES)							
				{
					AR_error(AR_ERR_WARNING, L"failed to build lexer : regex '%ls'!\r\n", __g_term_pattern[i].regex);
					Lex_Destroy(lex);										
					AR_ASSERT(false);										
					return NULL;
				}
		}


		if(Lex_GenerateTransTable(lex) != AR_S_YES)													
		{
				AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);		
		}																					
		return lex;																			
}


static psrGrammar_t*	__build_grammar(const psrHandler_t	*handler)															
{																																
		psrGrammar_t	*grammar;																								
		size_t i;																												
		AR_ASSERT(handler != NULL);																								
		grammar = Parser_CreateGrammar(handler);	
		if(grammar == NULL)
		{
				return NULL;
		}

		for(i = 0; i < __TERM_COUNT__; ++i)																						
		{		
				arStatus_t status;
				if(__g_term_pattern[i].skip || __g_term_pattern[i].tokval == 0)
				{
						continue;										
				}

				status = Parser_InsertTerm(grammar, __g_term_pattern[i].name, __g_term_pattern[i].tokval, PARSER_ASSOC_NONASSOC,0, __g_term_pattern[i].leaf);

				if(status != AR_S_YES)
				{
						AR_error(AR_ERR_WARNING, L"failed to build grammar : term '%ls'!\r\n", __g_term_pattern[i].name);
						Parser_DestroyGrammar(grammar);																			
						grammar = NULL;																							
						AR_ASSERT(false);																						
						return NULL;														
				}
		}					

		for(i = 0; i < __PREC_COUNT__; ++i)																						
		{																														
				psrTermInfo_t	*info;																							
				info = Parser_GetTermSymbInfoByName(grammar, __g_prec_pattern[i].name);											
				if(info == NULL)																								
				{						
						arStatus_t status;
						status = Parser_InsertTerm(grammar, __g_prec_pattern[i].name, __g_prec_pattern[i].tokval, __g_prec_pattern[i].assoc, __g_prec_pattern[i].prec_level, NULL);

						if(status != AR_S_YES)
						{
								AR_error(AR_ERR_WARNING, L"failed to build grammar : prec '%ls'!\r\n", __g_term_pattern[i].name);
								Parser_DestroyGrammar(grammar);																												
								grammar = NULL;																																
								AR_ASSERT(false);																															
								return NULL;									

						}
				}else																																						
				{																																							
						info->assoc = __g_prec_pattern[i].assoc;																											
						info->prec = __g_prec_pattern[i].prec_level;																										
				}																																							
		}		

		for(i = 0; i < __RULE_COUNT__; ++i)																													
		{				
				arStatus_t status;
				status = Parser_InsertRuleByStr(grammar, __g_rule_pattern[i].rule, __g_rule_pattern[i].prec_token, __g_rule_pattern[i].handler, __g_rule_pattern[i].auto_ret);
				
				if(status != AR_S_YES)
				{		
						AR_error(AR_ERR_WARNING, L"failed to build grammar : rule '%ls'!\r\n", __g_rule_pattern[i].rule);
						Parser_DestroyGrammar(grammar);																														
						grammar = NULL;																																		
						AR_ASSERT(false);																																	
						return NULL;																																		
				}
		}				


		if(Parser_SetStartRule(grammar,START_RULE) != AR_S_YES || Parser_CheckIsValidGrammar(grammar) != AR_S_YES)
		{
				AR_error(AR_ERR_WARNING, L"failed to build grammar!\r\n");
				Parser_DestroyGrammar(grammar);
				grammar = NULL;
				AR_ASSERT(false);
				return NULL;																																				
		}

		return grammar;																																						
}





static psrRetVal_t AR_STDCALL on_constant(const psrToken_t *tok,void *ctx)
{
	 { 

					psrRetVal_t	ret;
					exprNode_t	*expr_node;

					AR_ASSERT(tok != NULL);
					AR_UNUSED(ctx);
					AR_memset(&ret, 0, sizeof(ret));

					expr_node = NULL;
					expr_node =	CreateExprNode(CONSTANT_T);
						
					if(expr_node == NULL)
					{
						AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
						ret.status = AR_E_NOMEM;
						ret.node = NULL;
						return ret;
					}

					expr_node->line = tok->line;
					expr_node->col = tok->col;

					if(tok->str_cnt >= 1024 || tok->str_cnt <= 2)
					{
						AR_error(AR_ERR_WARNING, L"invalid constant length, (%Iu, %Iu)!", tok->line, tok->col);
						DestroyExprNode(expr_node);
						ret.status = AR_E_MALFORMAT;
						ret.node = NULL;
						return ret;
					}
						
					AR_wcsncpy(expr_node->constant.constant, tok->str + 1, tok->str_cnt - 1);	//消去'xxx'或"xxx"中的'和"
					expr_node->constant.constant[tok->str_cnt - 2] = L'\0';
					ret.status = AR_S_YES;
					ret.node = (psrToken_t*)expr_node;
					return ret;
			 }
}


static psrRetVal_t AR_STDCALL on_file_variable(const psrToken_t *tok,void *ctx)
{
	 { 
						psrRetVal_t	ret;
						exprNode_t	*expr_node;

						AR_ASSERT(tok != NULL);
						AR_UNUSED(ctx);

						AR_memset(&ret, 0, sizeof(ret));

						expr_node = NULL;
						expr_node =	CreateExprNode(VARIABLE_T);
						
						if(expr_node == NULL)
						{
							AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
							ret.status = AR_E_NOMEM;
							ret.node = NULL;
							return ret;
						}

						expr_node->line = tok->line;
						expr_node->col = tok->col;

						if(tok->str_cnt >= 1024 || tok->str_cnt <= 7)		
						{
							AR_error(AR_ERR_WARNING, L"invalid variable length, (%Iu, %Iu)!", tok->line, tok->col);
							DestroyExprNode(expr_node);
							ret.status = AR_E_MALFORMAT;
							ret.node = NULL;
							return ret;
						}
						
						AR_wcsncpy(expr_node->var.name, tok->str + 6, tok->str_cnt - 1);	//消去%fver_...%中的%fver_和%
						expr_node->var.name[tok->str_cnt - 7] = L'\0';
						expr_node->var.is_file_check = true;
						ret.status = AR_S_YES;
						ret.node = (psrNode_t*)expr_node;
						return ret;
			 }
}


static psrRetVal_t AR_STDCALL on_variable(const psrToken_t *tok,void *ctx)
{
	 { 
						psrRetVal_t	ret;
						exprNode_t	*expr_node;

						AR_ASSERT(tok != NULL);
						AR_UNUSED(ctx);

						AR_memset(&ret, 0, sizeof(ret));

						expr_node = NULL;
						expr_node =	CreateExprNode(VARIABLE_T);
						
						if(expr_node == NULL)
						{
							AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
							ret.status = AR_E_NOMEM;
							ret.node = NULL;
							return ret;
						}

						expr_node->line = tok->line;
						expr_node->col = tok->col;

						if(tok->str_cnt >= 1024 || tok->str_cnt <= 2)	
						{
							AR_error(AR_ERR_WARNING, L"invalid variable length, (%Iu, %Iu)!", tok->line, tok->col);
							DestroyExprNode(expr_node);
							ret.status = AR_E_MALFORMAT;
							ret.node = NULL;
							return ret;
						}
						
						AR_wcsncpy(expr_node->var.name, tok->str + 1, tok->str_cnt - 1);	//消去%xxx%中的'%'
						expr_node->var.name[tok->str_cnt - 2] = L'\0';
						expr_node->var.is_file_check = false;
						ret.status = AR_S_YES;
						ret.node = (psrNode_t*)expr_node;
						return ret;
			 }
}


static psrRetVal_t AR_STDCALL on_name(const psrToken_t *tok,void *ctx)
{
	 { 
						psrRetVal_t	ret;
						exprNode_t	*expr_node;

						AR_ASSERT(tok != NULL);
						AR_UNUSED(ctx);

						AR_memset(&ret, 0, sizeof(ret));

						expr_node = NULL;
						expr_node =	CreateExprNode(NAME_T);
						
						if(expr_node == NULL)
						{
							AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
							ret.status = AR_E_NOMEM;
							ret.node = NULL;
							return ret;
						}

						expr_node->line = tok->line;
						expr_node->col = tok->col;

						if(tok->str_cnt >= 1024)	
						{
							AR_error(AR_ERR_WARNING, L"invalid variable length, (%Iu, %Iu)!", tok->line, tok->col);
							DestroyExprNode(expr_node);
							ret.status = AR_E_MALFORMAT;
							ret.node = NULL;
							return ret;
						}
						
						AR_wcsncpy(expr_node->name.name, tok->str, tok->str_cnt);
						expr_node->name.name[tok->str_cnt] = L'\0';
						ret.status = AR_S_YES;
						ret.node = (psrNode_t*)expr_node;
						return ret;
	
			 }
}


static psrRetVal_t AR_STDCALL on_operator(const psrToken_t *tok,void *ctx)
{
	 { 
						psrRetVal_t	ret;
						exprNode_t	*expr_node;

						AR_ASSERT(tok != NULL);
						AR_UNUSED(ctx);

						AR_memset(&ret, 0, sizeof(ret));

						expr_node = NULL;
						expr_node =	CreateOperatorExprNode(tok);
						
						if(expr_node == NULL)
						{
							AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
							ret.status = AR_E_NOMEM;
							ret.node = NULL;
						}else
						{
							ret.status = AR_S_YES;
							ret.node = (psrNode_t*)expr_node;
						}
						return ret;
			 }
}


static psrRetVal_t AR_STDCALL autoreturn_null(const psrToken_t *tok,void *ctx)
{
	 { 
				psrRetVal_t ret = {AR_S_YES, NULL};
				AR_UNUSED(tok);
				AR_UNUSED(ctx);
				return ret;
			 }
}


/*expression	:	expr */
/*expr	:	unary_expr */
/*expr	:	call */
/*expr	:	term */
/*term	:	VAR */
/*term	:	CONSTANT */
/*term	:	FILE_VAR */
/*param	:	CONSTANT */
/*param	:	VAR */
/*param	:	FILE_VAR */
static psrRetVal_t AR_STDCALL autoreturn_0(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
			psrRetVal_t		ret = {AR_S_YES, NULL};
			AR_ASSERT(count == 1 && nodes != NULL);
	
			AR_UNUSED(count);
			AR_UNUSED(name);
			AR_UNUSED(ctx);

			ret.status = AR_S_YES;
			ret.node = (psrNode_t*)nodes[0];
			nodes[0] = NULL;
			return ret;
		 }
}




/*expr	:	( expr ) */
static psrRetVal_t AR_STDCALL autoreturn_1(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
			psrRetVal_t		ret = {AR_S_YES, NULL};
			AR_ASSERT(count == 3 && nodes != NULL);
			
			AR_UNUSED(count);
			AR_UNUSED(name);
			AR_UNUSED(ctx);


			ret.status = AR_S_YES;
			ret.node = (psrNode_t*)nodes[1];
			nodes[1] = NULL;
			return ret;
		 }
}




/*expr	:	expr && expr */
/*expr	:	expr || expr */
/*expr	:	expr <= expr */
/*expr	:	expr >= expr */
/*expr	:	expr < expr */
/*expr	:	expr > expr */
/*expr	:	expr == expr */
/*expr	:	expr != expr */
static psrRetVal_t AR_STDCALL on_binary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t		ret = {AR_S_YES, NULL};
				exprNode_t		**ns = (exprNode_t**)nodes;
				exprNode_t		*res_node;
				AR_ASSERT(ns != NULL && count == 3);
				
				AR_UNUSED(count);
				AR_UNUSED(name);
				AR_UNUSED(ctx);


				res_node = ns[1];
				ns[1] = NULL;

				if(res_node == NULL)
				{
					ret.status = AR_E_ABORT;
					ret.node = NULL;
					return ret;
				}

				res_node->expr.left = ns[0];
				ns[0] = NULL;
				res_node->expr.right = ns[2];
				ns[2] = NULL;
				
				ret.status = AR_S_YES;
				ret.node = res_node;
				return ret;

		 }
}




/*unary_expr	:	! expr */
static psrRetVal_t AR_STDCALL on_unary_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t		ret = {AR_S_YES, NULL};
				exprNode_t		**ns = (exprNode_t**)nodes;
				exprNode_t		*res_node;
				AR_ASSERT(ns != NULL && count == 2);
				
				AR_UNUSED(count);
				AR_UNUSED(ctx);
				AR_UNUSED(name);

				res_node = ns[0];
				ns[0] = NULL;

				if(res_node == NULL)
				{
					ret.status = AR_E_ABORT;
					ret.node = NULL;
					return ret;
				}

				res_node ->expr.left = ns[1];
				ns[1] = NULL;

				ret.status = AR_S_YES;
				ret.node = res_node;
				return ret;
		 }
}




/*call	:	NAME ( params ) */
static psrRetVal_t AR_STDCALL on_call_expression(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t		ret = {AR_S_YES, NULL};
				exprNode_t		**ns = (exprNode_t**)nodes;
				exprNode_t		*name_node,*res_node;
				AR_ASSERT(ns != NULL && count == 4);
				
				AR_UNUSED(count);
				AR_UNUSED(ctx);
				AR_UNUSED(name);

				name_node = ns[0];
				res_node = ns[2];
				

				if(name_node == NULL || res_node == NULL)
				{
					ret.status = AR_E_ABORT;
					ret.node = NULL;
					return ret;
				}
				
				AR_wcscpy(res_node->call.name, name_node->name.name);
				ns[2] = NULL;

				ret.status = AR_S_YES;
				ret.node = res_node;
				return ret;
		 }
}




/*params	:	params , param */
static psrRetVal_t AR_STDCALL insert_new_params(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t		ret = {AR_S_YES, NULL};	//嗨~~，为啥非要在这里创建call Expression呢？因为我懒得写nodelist一类的东西塞进exprNode_t里了，先这样，以后再说~~
				exprNode_t		*expr_node, *new_node;
				AR_ASSERT(nodes != NULL && count == 3);
				AR_UNUSED(ctx);
				AR_memset(&ret, 0, sizeof(ret));
				
				AR_UNUSED(count);
				AR_UNUSED(ctx);
				AR_UNUSED(name);

				if(nodes[0] == NULL || nodes[2] == NULL)
				{
					ret.status = AR_E_ABORT;
					ret.node = NULL;
					return ret;
				}	

				expr_node = (exprNode_t*)nodes[0];
				new_node = (exprNode_t*)nodes[2];

				if(expr_node->call.param_cnt >= AR_NELEMS(expr_node->call.params))
				{
					AR_error(AR_ERR_WARNING, L"invalid params count : %Iu\r\n", expr_node->call.param_cnt);
					ret.status = AR_E_NOTENOUGH;
					ret.node = NULL;
					return ret;
				}
	
				expr_node->call.params[expr_node->call.param_cnt] = new_node;
				expr_node->call.param_cnt++;

				nodes[0] = nodes[2] = NULL;
				ret.status = AR_S_YES;
				ret.node = (psrNode_t*)expr_node;
				return ret;

		 }
}




/*params	:	param */
static psrRetVal_t AR_STDCALL on_call_params_expr(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t		ret = {AR_S_YES, NULL};	//嗨~~，为啥非要在这里创建call Expression呢？因为我懒得写nodelist一类的东西塞进exprNode_t里了，先这样，以后再说~~
				exprNode_t		*expr_node;
				AR_ASSERT(nodes != NULL && count == 1);

				AR_UNUSED(count);
				AR_UNUSED(ctx);
				AR_UNUSED(name);

				AR_memset(&ret, 0, sizeof(ret));

				expr_node = NULL;
				expr_node =	CreateExprNode(CALL_T);
						
				if(expr_node == NULL)
				{
					AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
					ret.status = AR_E_NOMEM;
					ret.node = NULL;
					return ret;
				}
				
				expr_node->call.params[0] = (exprNode_t*)nodes[0];
				nodes[0] = NULL;
				expr_node->call.param_cnt = 1;

				ret.status = AR_S_YES;
				ret.node = (psrNode_t*)expr_node;
				return ret;

		 }
}




/*params	:	 */
static psrRetVal_t AR_STDCALL on_empty_call_params(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		{ 
				psrRetVal_t		ret = {AR_S_YES, NULL};	//看上头~~
				exprNode_t		*expr_node;
				AR_ASSERT(nodes != NULL && count == 0);

				AR_UNUSED(nodes);
				AR_UNUSED(count);
				AR_UNUSED(ctx);
				AR_UNUSED(name);

				AR_memset(&ret, 0, sizeof(ret));

				expr_node = NULL;
				expr_node =	CreateExprNode(CALL_T);
						
				if(expr_node == NULL)
				{
					AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
					ret.status = AR_E_NOMEM;
					ret.node = NULL;
					return ret;
				}
				
				expr_node->call.param_cnt = 0;

				ret.status = AR_S_YES;
				ret.node = (psrNode_t*)expr_node;
				return ret;	
		 }
}






/*
main
*/
 




static arStatus_t		AR_STDCALL handle_on_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
{
		wchar_t msg[33];
		AR_ASSERT(tok != NULL);
		
		AR_UNUSED(expected);
		AR_UNUSED(count);
		AR_UNUSED(ctx);
	
		if(tok->str_cnt > 0)
		{
				AR_wcsncpy(msg, tok->str, AR_MIN(tok->str_cnt, 32));
				msg[AR_MIN(tok->str_cnt, 32)] = L'\0';
		}else
		{
				AR_wcscpy(msg, L"%EOI");
		}

		AR_error(AR_ERR_WARNING, L"invalid token : '%ls' (%Iu,%Iu)\r\n", msg, tok->line, tok->col);
		return AR_S_YES;
}



static void		AR_STDCALL free_node(psrNode_t *node, void *ctx)
{
		AR_ASSERT(node != NULL);
		AR_UNUSED(ctx);
		DestroyExprNode((exprNode_t*)node);
}


static const psrHandler_t		__g_expr_handler = 
{
		handle_on_error,
		free_node
};







 



