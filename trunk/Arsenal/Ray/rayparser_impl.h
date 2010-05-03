#include "rayparser.h"




AR_NAMESPACE_BEGIN

void				RAY_InitParserImpl();
void				RAY_UnInitParserImpl();

lex_t*				RAY_BuildLexer();
void				RAY_ReleaseLexer(lex_t *lexer);

psrContext_t*		RAY_BuildParserContext(void *ctx);
void				RAY_ReleaseParserContext(psrContext_t		*parser_context);



AR_NAMESPACE_END
