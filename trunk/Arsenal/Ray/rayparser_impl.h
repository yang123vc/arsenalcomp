#include "rayparser.h"




AR_NAMESPACE_BEGIN

lex_t*			RAY_BuildLexer(const arIOCtx_t *io);
psrGrammar_t*	RAY_BuildGrammar(const psrHandler_t	*handler, const arIOCtx_t *io);


AR_NAMESPACE_END
