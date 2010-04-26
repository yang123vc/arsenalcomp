#include "rayparser.h"




AR_NAMESPACE_BEGIN

psrGrammar_t*	RAY_BuildGrammar(const psrCtx_t	*psr_ctx, const arIOCtx_t *io);
lex_t*			RAY_BuildLexer(const arIOCtx_t *io);

AR_NAMESPACE_END
