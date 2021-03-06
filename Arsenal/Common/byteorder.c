﻿/*
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



ar_int_16_t		AR_BYTEFLIP_16(ar_int_16_t val)
{
		ar_byte_t t;
		union{
				ar_byte_t b[2]; ar_int_16_t v;
		}v16;
		v16.v = val;
		t = v16.b[0];
		v16.b[0] = v16.b[1];
		v16.b[1] = t;
		return v16.v;
}

ar_uint_16_t		AR_BYTEFLIP_U16(ar_uint_16_t val)
{
		ar_byte_t t;
		union{
				ar_byte_t b[2]; ar_uint_16_t v;
		}v16;
		v16.v = val;
		t = v16.b[0];
		v16.b[0] = v16.b[1];
		v16.b[1] = t;
		return v16.v;
}


ar_int_32_t		AR_BYTEFLIP_32(ar_int_32_t val)
{
		ar_byte_t t;
		union{
				ar_byte_t b[4]; ar_int_32_t v;
		}v32;
		v32.v = val;
		t = v32.b[0];
		v32.b[0] = v32.b[3];
		v32.b[3] = t;

		t = v32.b[1];
		v32.b[1] = v32.b[2];
		v32.b[2] = t;
		return v32.v;
}

ar_uint_32_t		AR_BYTEFLIP_U32(ar_uint_32_t val)
{
		ar_byte_t t;
		union{
				ar_byte_t b[4]; ar_uint_32_t v;
		}v32;
		v32.v = val;
		t = v32.b[0];
		v32.b[0] = v32.b[3];
		v32.b[3] = t;

		t = v32.b[1];
		v32.b[1] = v32.b[2];
		v32.b[2] = t;
		return v32.v;
}


ar_int_64_t		AR_BYTEFLIP_64(ar_int_64_t val)
{
		ar_byte_t t;
		union{
				ar_byte_t b[8]; ar_int_64_t v;
		}v64;
		v64.v = val;
		t = v64.b[0];
		v64.b[0] = v64.b[7];
		v64.b[7] = t;


		t = v64.b[1];
		v64.b[1] = v64.b[6];
		v64.b[6] = t;


		t = v64.b[2];
		v64.b[2] = v64.b[5];
		v64.b[5] = t;


		t = v64.b[3];
		v64.b[3] = v64.b[4];
		v64.b[4] = t;
		
		return v64.v;
}


ar_uint_64_t		AR_BYTEFLIP_U64(ar_uint_64_t val)
{
		ar_byte_t t;
		union{
				ar_byte_t b[8]; ar_uint_64_t v;
		}v64;
		v64.v = val;
		t = v64.b[0];
		v64.b[0] = v64.b[7];
		v64.b[7] = t;


		t = v64.b[1];
		v64.b[1] = v64.b[6];
		v64.b[6] = t;


		t = v64.b[2];
		v64.b[2] = v64.b[5];
		v64.b[5] = t;


		t = v64.b[3];
		v64.b[3] = v64.b[4];
		v64.b[4] = t;
		
		return v64.v;
}


float           AR_BYTEFLIP_FLOAT(float val)
{
        union
        {
                float           f;
                ar_uint_32_t    u32;
        }swapper;
        
        swapper.f = val;
        swapper.u32 = AR_BYTEFLIP_U32(swapper.u32);
        return swapper.f;
}

double          AR_BYTEFLIP_DOUBLE(double val)
{
        union
        {
                double          dbl;
                ar_uint_64_t    u64;
        }swapper;
        
        swapper.dbl = val;
        swapper.u64 = AR_BYTEFLIP_U64(swapper.u64);
        return swapper.dbl;
}


void			AR_BYTEFLIP_16B(ar_byte_t *b)
{
		ar_byte_t t;
		AR_ASSERT(b != NULL);
		t = b[0];
		b[0] = b[1];
		b[1] = t;
		
}

void			AR_BYTEFLIP_32B(ar_byte_t *b)
{
		ar_byte_t t;
		AR_ASSERT(b != NULL);

		t = b[0];
		b[0] = b[3];
		b[3] = t;

		t = b[1];
		b[1] = b[2];
		b[2] = t;
}

void			AR_BYTEFLIP_64B(ar_byte_t *b)
{
		ar_byte_t t;
		AR_ASSERT(b != NULL);

		t = b[0];
		b[0] = b[7];
		b[7] = t;


		t = b[1];
		b[1] = b[6];
		b[6] = t;


		t = b[2];
		b[2] = b[5];
		b[5] = t;


		t = b[3];
		b[3] = b[4];
		b[4] = t;

}

AR_NAMESPACE_END