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
		byte_b_t	*first;
		byte_b_t	*last;
		byte_b_t	*read_cur;
		byte_b_t	*write_cur;
};

static AR_INLINE ar_bool_t	__buffer_is_valid(const arBuffer_t *pbuf)
{
		if(pbuf == NULL)return false;

		if(pbuf->read_cur < pbuf->first || pbuf->read_cur > pbuf->last || pbuf->write_cur < pbuf->first || pbuf->write_cur > pbuf->last)return false;

		if(pbuf->first > pbuf->last)return false;

		return true;
}

static AR_INLINE void		__increase_capability(arBuffer_t *pbuf, size_t inc_len)
{
		size_t cur_len = 0;
		size_t data_len = 0;
		byte_b_t *new_buf = NULL;
		byte_b_t *old_buf = NULL;
		AR_ASSERT(__buffer_is_valid(pbuf));
		
		if(inc_len == 0)return;

		cur_len = pbuf->last - pbuf->first;
		data_len = pbuf->write_cur - pbuf->read_cur;

		new_buf = AR_NEWARR(byte_b_t, inc_len + cur_len);

		AR_ASSERT(new_buf != NULL);

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
}


static AR_INLINE  ar_bool_t __move_internal(arBuffer_t *pbuf, size_t len)
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




#define INIT_BUF_LEN	4096

arBuffer_t*		AR_CreateBuffer(size_t nbytes)
{
		arBuffer_t		*buf;
		
		buf = AR_NEW0(arBuffer_t);
		if(nbytes > 0)
		{
				nbytes = AR_MAX(nbytes, INIT_BUF_LEN);
				__increase_capability(buf, nbytes);
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

void			AR_ReserveBuffer(arBuffer_t *pbuf, size_t nbytes)
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
				__increase_capability(pbuf, inc_len);
		}
}

byte_b_t*			AR_AllocBuffer(arBuffer_t *buffer, size_t	nbytes)
{
		size_t write_able = 0;
		byte_b_t *res = NULL;
		AR_ASSERT(__buffer_is_valid(buffer));

		if(nbytes == 0) return NULL;

		write_able = buffer->last - buffer->write_cur;
		if(write_able < nbytes && !__move_internal(buffer, nbytes))
		{
				AR_ReserveBuffer(buffer, nbytes);
		}
		
		AR_ASSERT((size_t)(buffer->last- buffer->write_cur) >= nbytes);

		res = buffer->write_cur;
		buffer->write_cur += nbytes;
		return res;
}


void			AR_InsertBuffer(arBuffer_t *buffer, const byte_b_t *data, size_t len)
{
		byte_b_t *ptr = NULL;
		AR_ASSERT(__buffer_is_valid(buffer));
		if(len == 0)return;
		ptr = AR_AllocBuffer(buffer, len);

		AR_ASSERT(ptr != NULL);
		memcpy(ptr, data, len);
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


size_t			AR_GetBufferCapacity(const arBuffer_t *buffer)
{
		AR_ASSERT(__buffer_is_valid(buffer));
		return buffer->last - buffer->first - AR_GetBufferAvailable(buffer);
}


const byte_b_t*	AR_GetBufferData(const arBuffer_t *pbuf)
{
		AR_ASSERT(__buffer_is_valid(pbuf));
		return AR_GetBufferAvailable(pbuf) > 0 ? pbuf->read_cur : NULL;
}

size_t			AR_GetBufferAvailable(const arBuffer_t *buffer)
{
		AR_ASSERT(__buffer_is_valid(buffer));
		return (buffer->write_cur - buffer->read_cur);
}








AR_NAMESPACE_END


