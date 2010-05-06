#include "rayparser.h"




AR_NAMESPACE_BEGIN

void				RAY_InitParserImpl();
void				RAY_UnInitParserImpl();

lexMatch_t*			RAY_BuildLexer();
void				RAY_ReleaseLexer(lexMatch_t	*match);

psrContext_t*		RAY_BuildParserContext(void *ctx);
void				RAY_ReleaseParserContext(psrContext_t		*parser_context);



AR_NAMESPACE_END
