//#include "stdafx.h"
#include "AR.h"

AR_NAMESPACE_BEGIN


static void AR_STDCALL __error_func(int level, const wchar_t *msg, void *ctx)
{
		ARContext		*context = (ARContext*)ctx;
		
		AR_ASSERT(context != NULL);
		context->OnError(level, msg);
}

static void	AR_STDCALL __print_func(const wchar_t *msg, void *ctx)
{
		ARContext		*context = (ARContext*)ctx;
		AR_ASSERT(context != NULL);
		context->OnPrint(msg);
}

static void	AR_STDCALL __fatal_func(const wchar_t *msg, void *ctx)
{
		ARContext		*context = (ARContext*)ctx;
		AR_ASSERT(context != NULL);
		context->OnFatal(msg);
		abort();
}


Arsenal::Arsenal(ARContext		*ctx) : m_ctx(ctx)
{
		AR_ASSERT(ctx != NULL);
		
		arInit_t init;
		init.ctx = (void*)m_ctx;
		init.error_f = __error_func;
		init.print_f = __print_func;
		init.fatal_f = __fatal_func;

		arCtx_t ar_ctx;
		ar_ctx.init = init;
		Arsenal_Init(&ar_ctx);
}


Arsenal::~Arsenal()
{
		AR_ASSERT(m_ctx != NULL);

		Arsenal_UnInit();
		delete m_ctx;
}



static Arsenal *__g_arsenal = NULL;

Arsenal* Arsenal::Instance(ARContext *ctx)
{
		AR_ASSERT(ctx != NULL);
		if(__g_arsenal == NULL)
		{
				__g_arsenal = new Arsenal(ctx);
		}
		/*AR_report();*/
		return __g_arsenal;
}

void	Arsenal::UnInstance()
{
		/*AR_report();*/
		if(__g_arsenal)
		{
				delete __g_arsenal;
				__g_arsenal = NULL;
		}
}




AR_NAMESPACE_END

