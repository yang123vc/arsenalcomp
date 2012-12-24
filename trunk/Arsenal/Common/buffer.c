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


#include "common.h"


AR_NAMESPACE_BEGIN



struct arsenal_buffer_tag
{
		ar_byte_t	*first;
		ar_byte_t	*last;
		ar_byte_t	*read_cur;
		ar_byte_t	*write_cur;
};


#if defined(AR_DEBUG)

static AR_INLINE bool_t	__buffer_is_valid(const arBuffer_t *pbuf)
{
		if(pbuf == NULL)return false;

		if(pbuf->read_cur < pbuf->first || pbuf->read_cur > pbuf->last || pbuf->write_cur < pbuf->first || pbuf->write_cur > pbuf->last)return false;

		if(pbuf->first > pbuf->last)return false;

		return true;
}

#else

#define __buffer_is_valid(_pbuf)		AR_NOOP


#endif

static AR_INLINE arStatus_t		__increase_capability(arBuffer_t *pbuf, size_t inc_len)
{
		size_t cur_len = 0;
		size_t data_len = 0;
		ar_byte_t *new_buf = NULL;
		ar_byte_t *old_buf = NULL;
		AR_ASSERT(__buffer_is_valid(pbuf));
		
		if(inc_len == 0)
		{
				return AR_S_YES;
		}

		cur_len = pbuf->last - pbuf->first;
		data_len = pbuf->write_cur - pbuf->read_cur;

		new_buf = AR_NEWARR(ar_byte_t, inc_len + cur_len);

		if(new_buf == NULL)
		{
				return AR_E_NOMEM;
		}

		if(data_len > 0)
		{
				AR_memcpy(new_buf, pbuf->read_cur, data_len);
		}

		old_buf = pbuf->first;
		pbuf->first = new_buf;
		pbuf->last = pbuf->first + inc_len + cur_len;
		pbuf->read_cur = pbuf->first;
		pbuf->write_cur = pbuf->read_cur + data_len;
		AR_DEL(old_buf);

		return AR_S_YES;
}


static AR_INLINE  bool_t __move_internal(arBuffer_t *pbuf, size_t len)
{
		size_t data_len = 0;	/* r***w read_cur 至write_cur之间的可读数据成都*/
		size_t vacancy_len = 0;	/* ***r read_cur之前的空白空间  */
		size_t free_len = 0;	/* ***r  w*** first 至read_cur和 write_cur 至 last的空闲空间数量之和*/
		AR_ASSERT(__buffer_is_valid(pbuf) && len > 0);

		data_len = pbuf->write_cur - pbuf->read_cur;
		vacancy_len = pbuf->read_cur - pbuf->first;
		free_len = (pbuf->last - pbuf->write_cur) + vacancy_len;

		if(free_len < len) return false;
		
		if(data_len > 0)
		{
				AR_memmove(pbuf->first, pbuf->read_cur, data_len);
		}

		pbuf->read_cur = pbuf->first;
		pbuf->write_cur = pbuf->first + data_len;

		AR_ASSERT((size_t)(pbuf->last - pbuf->write_cur) >= len);
		return true;

}




#define INIT_BUF_LEN	1

arBuffer_t*		AR_CreateBuffer(size_t nbytes)
{
		arBuffer_t		*buf;
		
		buf = AR_NEW0(arBuffer_t);
		
		if(buf == NULL)
		{
				return NULL;
		}

		if(nbytes > 0)
		{
				nbytes = AR_MAX(nbytes, INIT_BUF_LEN);
				
				if(__increase_capability(buf, nbytes) != AR_S_YES)
				{
						AR_DEL(buf);
						buf = NULL;
				}
		}

		return buf;
}

void			AR_DestroyBuffer(arBuffer_t		*buffer)
{
		AR_ASSERT(__buffer_is_valid(buffer));
		AR_ClearBuffer(buffer);
		AR_DEL(buffer);

}


void			AR_ClearBuffer(arBuffer_t		*buffer)
{
		AR_ASSERT(__buffer_is_valid(buffer));
		if(buffer->first)
		{
				AR_DEL(buffer->first);
		}
		AR_memset(buffer, 0, sizeof(*buffer));
}



void            AR_SwapBuffer(arBuffer_t *l, arBuffer_t *r)
{
        AR_ASSERT(l != NULL && r != NULL);
        
        if(l != r)
        {
                arBuffer_t tmp = *l;
                *l = *r;
                *r = tmp;
        }
        
}


arStatus_t		AR_CopyBuffer(arBuffer_t *dest, const arBuffer_t *src)
{
		AR_ASSERT(dest != NULL && src != NULL);
		AR_ClearBuffer(dest);
		return AR_InsertToBuffer(dest, AR_GetBufferData(src), AR_GetBufferAvailable(src));
}

arBuffer_t*		AR_CopyNewBuffer(const arBuffer_t *buf)
{
		arBuffer_t *new_buf;
		AR_ASSERT(buf != NULL);

		new_buf = AR_CreateBuffer(AR_GetBufferAvailable(buf));
		if(new_buf == NULL)
		{
				return NULL;
		}

		if(AR_CopyBuffer(new_buf, buf) != AR_S_YES)
		{
				AR_DestroyBuffer(new_buf);
				new_buf = NULL;
		}


		return new_buf;

}



arStatus_t			AR_ReserveBuffer(arBuffer_t *pbuf, size_t nbytes)
{
		size_t inc_len = 0;
		size_t capa_len = 0;
		size_t cur_buf_len = 0;
		AR_ASSERT(__buffer_is_valid(pbuf));
		
		capa_len = AR_GetBufferCapacity(pbuf);
		cur_buf_len = pbuf->last - pbuf->first;
		if(capa_len < nbytes)
		{
				inc_len = (size_t)AR_MAX((nbytes - capa_len),  cur_buf_len * 2 - cur_buf_len);
				if(__increase_capability(pbuf, inc_len) != AR_S_YES)
				{
						return AR_E_NOMEM;
				}
		}

		return AR_S_YES;
}

ar_byte_t*			AR_AllocFromBuffer(arBuffer_t *buffer, size_t	nbytes)
{
		size_t write_able = 0;
		ar_byte_t *res = NULL;
		AR_ASSERT(__buffer_is_valid(buffer));

		if(nbytes == 0) return NULL;

		write_able = buffer->last - buffer->write_cur;
		if(write_able < nbytes && !__move_internal(buffer, nbytes))
		{
				if(AR_ReserveBuffer(buffer, nbytes) != AR_S_YES)
				{
						return NULL;
				}
		}
		
		AR_ASSERT((size_t)(buffer->last- buffer->write_cur) >= nbytes);

		res = buffer->write_cur;
		buffer->write_cur += nbytes;
		return res;
}


arStatus_t			AR_InsertToBuffer(arBuffer_t *buffer, const ar_byte_t *data, size_t len)
{
		ar_byte_t *ptr = NULL;
		AR_ASSERT(__buffer_is_valid(buffer));
		if(len == 0)
		{
				return AR_S_YES;
		}

		ptr = AR_AllocFromBuffer(buffer, len);

		if(ptr)
		{
				memcpy(ptr, data, len);
				return AR_S_YES;
		}else
		{
				return AR_E_NOMEM;
		}
}


arStatus_t		AR_InsertCStringToBuffer(arBuffer_t *buffer, const char *str)
{
		size_t l;
		AR_ASSERT(buffer != NULL && str != NULL);
		l = AR_strlen(str);
		if(l > 0)
		{
				return AR_InsertToBuffer(buffer, (const ar_byte_t*)str, l * sizeof(char));
		}else
		{
				return AR_S_YES;
		}


}
arStatus_t		AR_InsertBufferToBuffer(arBuffer_t *buffer, const arBuffer_t *other)
{
		AR_ASSERT(buffer != NULL && other != NULL);
		if(AR_GetBufferAvailable(other) > 0)
		{
				return AR_InsertToBuffer(buffer, AR_GetBufferData(other), AR_GetBufferAvailable(other));
		}else
		{
				return AR_S_YES;
		}
}

size_t			AR_EraseBuffer(arBuffer_t *pbuf, size_t nbytes)
{
		size_t rm_data_len = 0;
		AR_ASSERT(__buffer_is_valid(pbuf));
		if(nbytes == 0)return 0;

		rm_data_len = AR_MIN(AR_GetBufferAvailable(pbuf), nbytes);
		
		pbuf->read_cur += rm_data_len;

		AR_ASSERT(pbuf->read_cur <= pbuf->write_cur);

		if(pbuf->read_cur == pbuf->write_cur)
		{
				pbuf->read_cur = pbuf->first;
				pbuf->write_cur = pbuf->read_cur;
		}
		return rm_data_len;
}


size_t			AR_EraseBufferBack(arBuffer_t *pbuf, size_t nbytes)
{
		size_t rm_data_len = 0;
		AR_ASSERT(__buffer_is_valid(pbuf));
		if(nbytes == 0)return 0;

		rm_data_len = AR_MIN(AR_GetBufferAvailable(pbuf), nbytes);
		
		pbuf->write_cur -= rm_data_len;

		AR_ASSERT(pbuf->read_cur <= pbuf->write_cur);

		if(pbuf->read_cur == pbuf->write_cur)
		{
				pbuf->read_cur = pbuf->first;
				pbuf->write_cur = pbuf->read_cur;
		}
		return rm_data_len;

}

size_t			AR_GetBufferCapacity(const arBuffer_t *buffer)
{
		AR_ASSERT(__buffer_is_valid(buffer));
		return buffer->last - buffer->first - AR_GetBufferAvailable(buffer);
}


const ar_byte_t*	AR_GetBufferData(const arBuffer_t *pbuf)
{
		AR_ASSERT(__buffer_is_valid(pbuf));
		return AR_GetBufferAvailable(pbuf) > 0 ? pbuf->read_cur : NULL;
}

size_t			AR_GetBufferAvailable(const arBuffer_t *buffer)
{
		AR_ASSERT(__buffer_is_valid(buffer));
		return (buffer->write_cur - buffer->read_cur);
}



void            AR_ResetBufferData(arBuffer_t *buffer, size_t offset, const ar_byte_t *data, size_t len)
{
        size_t available_bytes;
        size_t write_bytes;

		available_bytes =  AR_GetBufferAvailable(buffer);
        
		AR_ASSERT(buffer != NULL && offset < available_bytes && data != NULL && len < available_bytes);
        
        if(available_bytes == 0 || len == 0)
        {
                return;
        }
        
        write_bytes = AR_MIN(available_bytes - offset, len);

        AR_memcpy(buffer->read_cur + offset, data, write_bytes);
}



size_t			AR_ReadBufferData(arBuffer_t *buffer, ar_byte_t *dest, size_t len)
{
		size_t read_n;
		const ar_byte_t *data;
		AR_ASSERT(buffer != NULL);
		
		data  = AR_GetBufferData(buffer);
		read_n = AR_MIN(AR_GetBufferAvailable(buffer), len);


		if(read_n > 0)
		{
				if(dest)
				{
						AR_memcpy(dest, data, read_n);
				}
				AR_EraseBuffer(buffer, read_n);
		}

		return read_n;

}





AR_NAMESPACE_END


