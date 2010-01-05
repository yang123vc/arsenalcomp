//#include "stdafx.h"
#include "Global.h"

namespace ArsenalCPP{

static void AR_STDCALL __error_func(int_t level, const wchar_t *msg, void *ctx)
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




Arsenal::Arsenal(ARContext		*ctx)
{
		AR_ASSERT(ctx != NULL);
		
		arCtx_t		ar_ctx;
		m_ctx = ctx;
		ar_ctx.init.global_io_ctx.ctx = (void*)ctx;
		ar_ctx.init.global_io_ctx.on_error = __error_func;
		ar_ctx.init.global_io_ctx.on_print = __print_func;
		Arsenal_Init(&ar_ctx);
}

Arsenal::~Arsenal()
{
		Arsenal_UnInit();
		delete m_ctx;
}


ARContext*	Arsenal::Context()const
{
		AR_ASSERT(m_ctx != NULL);
		return m_ctx;
}


static Arsenal	*__g_arsenal = NULL;

Arsenal* Arsenal::Instance(ARContext *ctx)
{
		AR_ASSERT(__g_arsenal == NULL && ctx != NULL);

		if(__g_arsenal == NULL)
		{
				__g_arsenal = new Arsenal(ctx);
		}
		return __g_arsenal;
}


void Arsenal::UnInstance()
{
		AR_ASSERT(__g_arsenal != NULL);

		if(__g_arsenal)
		{
				delete __g_arsenal;
				__g_arsenal = NULL;
		}
}


Arsenal* Arsenal::Get()
{
		AR_ASSERT(__g_arsenal != NULL);
		return __g_arsenal;
}


}
