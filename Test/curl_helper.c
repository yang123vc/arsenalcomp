
#include "curl_helper.h"



#if defined(__LIB)

AR_NAMESPACE_BEGIN




static arStatus_t      __set_response_header_field(httpRequestCallback_t *cb, const char *type, const char *content)
{
        arStatus_t      status;
        AR_ASSERT(cb != NULL && type != NULL && content != NULL);
		AR_ASSERT(cb->response_header != NULL);
        status = AR_InsertToHash(cb->response_header, (void*)type, (void*)content);
        return status;
}


static const char*             __get_response_header_field(httpRequestCallback_t *cb, const char *type)
{
        const char *val = NULL;
        AR_ASSERT(cb != NULL && type != NULL);
		AR_ASSERT(cb->response_header != NULL);
        
        if(AR_FindFromHash(cb->response_header, (void*)type, (void**)&val) == AR_S_YES)
        {
                return val;
        }else
        {
                return NULL;
        }
        
}



static arStatus_t		__default_OnStatusFunc_t(httpRequestCallback_t *cb, uint_32_t status, const char *reason)
{
		AR_ASSERT(cb != NULL);
		cb->http_status = status;
		if(reason)
		{
				AR_error(AR_ERR_WARNING, L"http response  : '%u : %hs'\r\n", status, reason);
		}
		return AR_S_YES;
}

arStatus_t		__default_OnHeaderFunc_t(httpRequestCallback_t *cb, const char *header_type, const char *header_val)
{
		AR_ASSERT(cb != NULL && cb->response_header != NULL && header_type != NULL);
		return __set_response_header_field(cb, header_type, header_val == NULL ? "" : header_val);
}


arStatus_t		__default_OnResponseFunc_t(httpRequestCallback_t *cb, const arBuffer_t *response_body)
{

		AR_ASSERT(cb != NULL);
		size_t response_bytes;
		AR_error(AR_ERR_MESSAGE, L"enter function : '%hs'\r\n", AR_FUNC_NAME);
		response_bytes = response_body == NULL ? 0 : AR_GetBufferAvailable(response_body);
		AR_error(AR_ERR_MESSAGE, L"recv response bytes : '%Iu'\r\n", response_bytes);
		
		return AR_S_YES;
}




static arStatus_t	__header_copy_func(void *data, void **pnew_data, void *ctx)
{
        char *nwcs;
        AR_ASSERT(data != NULL && pnew_data != NULL);
		AR_UNUSED(ctx);
        
        nwcs = AR_strdup((const char*)data);
        
        if(nwcs)
        {
                *pnew_data = (void*)nwcs;
                return AR_S_YES;
        }else
        {
                return AR_E_NOMEM;
        }
        
}


static void __header_destroy_func(void *data, void *ctx)
{
        AR_ASSERT(data != NULL);
        AR_UNUSED(ctx);
        AR_DEL(data);
        data = NULL;
}


static uint_64_t	__header_hash_func(void *key, void *ctx)
{
        AR_ASSERT(key != NULL);
        AR_UNUSED(ctx);
        return (uint_64_t)AR_strhash((const char*)key);
}

static int_t		__header_comp_func(void *l, void *r, void *ctx)
{
        AR_ASSERT(l != NULL && r != NULL);
        AR_UNUSED(ctx);
        return AR_stricmp((const char*)l, (const char*)r);
}







httpRequestCallback_t*	Http_CreateRequestCallback(void	*usr_ctx)
{
		httpRequestCallback_t *cb;
		cb = AR_NEW0(httpRequestCallback_t);
		if(cb == NULL)
		{
				goto INVALID_POINT;
		}

		cb->response_header = AR_CreateHash(41,__header_hash_func, __header_comp_func, __header_copy_func, __header_copy_func, __header_destroy_func,__header_destroy_func, NULL);
        
        if(cb->response_header == NULL)
        {
                goto INVALID_POINT;
        }

		cb->http_status = 0;
		cb->on_status = __default_OnStatusFunc_t;
		cb->on_header = __default_OnHeaderFunc_t;
		cb->on_response = __default_OnResponseFunc_t;
		
		cb->usr_ctx = usr_ctx;

		return cb;
INVALID_POINT:
		if(cb != NULL)
		{
				AR_DEL(cb);
				cb = NULL;
		}

		return NULL;
}

void					Http_DestroyRequestCallback(httpRequestCallback_t *cb)
{
		AR_ASSERT(cb != NULL);
		AR_DestroyHash(cb->response_header);
		AR_DEL(cb);
		cb = NULL;
}


const char*				Http_GetResponseHeaderValue(const httpRequestCallback_t *cb, const char *type)
{
		AR_ASSERT(cb != NULL && type != NULL);
		return __get_response_header_field((httpRequestCallback_t*)cb, type);
}

AR_NAMESPACE_END





#endif
