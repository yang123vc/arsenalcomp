#ifndef __AR_LEX_CONFIG_H__
#define __AR_LEX_CONFIG_H__

#include "lex.h"


AR_NAMESPACE_BEGIN




typedef enum {LEX_NAME, LEX_PATTERN} arPatternT_t;

typedef struct __lex_name_tag
{
		wchar_t	*name;
		wchar_t	*expr;
}lexCfgName_t;


typedef struct __lex_pattern_tag
{
		bool			skip;
		wchar_t			*pattern;
		lexAction_t		action;	/*action∫Õpattern”√”⁄LEX_PATTERN*/
}lexCfgPattern_t;


typedef struct __lex_config_tag
{
		arPatternT_t	type;
		union{
				lexCfgName_t		name;
				lexCfgPattern_t		pattern;
		};
		struct __lex_config_tag	*next;
}lexConfig_t;


const lexConfig_t*	LEX_CreateConfig(const wchar_t *pattern, const wchar_t **next_input);
void				LEX_DestroyConfig(const lexConfig_t *pattern);


const wchar_t*		  LEX_Config(lex_t *lex, const wchar_t *pattern);










AR_NAMESPACE_END







#endif

