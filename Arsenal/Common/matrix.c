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

/******************************************************Misc**************************************************************/

AR_NAMESPACE_BEGIN


/******************************************************Vector**************************************************************/

struct __arsenal_vector_tag
{
		double			*v;
		size_t			count;
};


arVector_t*		AR_CreateVector(size_t size)
{
		arVector_t *vec;
		AR_ASSERT(size > 0);
		vec = AR_NEW(arVector_t);
		vec->count = size;
		vec->v = AR_NEWARR0(double, size);
		return vec;
}

void			AR_DestroyVector(arVector_t *vec)
{
		AR_ASSERT(vec != NULL);

		if(vec->v)
		{
				AR_DEL(vec->v);
		};

		AR_DEL(vec);
		vec = NULL;
}

arVector_t*		AR_CopyNewVector(const arVector_t *vec)
{
		arVector_t *new_v;
		AR_ASSERT(vec != NULL);

		new_v = AR_CreateVector(vec->count);
		
		AR_CopyVector(new_v, vec);

		return new_v;
}

void			AR_CopyVector(arVector_t *dest, const arVector_t *src)
{
		AR_ASSERT(dest != NULL && src != NULL);

		if(dest == src)
		{
				return;
		}


		if(dest->count != src->count)
		{
				AR_ChangeVectorSize(dest, src->count);
		}

		AR_memcpy(dest->v, src->v, sizeof(double) * src->count);
}


void			AR_ChangeVectorSize(arVector_t *vec, size_t size)
{
		AR_ASSERT(vec != NULL);
		AR_ASSERT(size > 0);

		if(vec->v)
		{
				AR_DEL(vec->v);
				vec->v = NULL;
		}

		vec->count = size;
		vec->v = AR_NEWARR0(double, size);
}



size_t			AR_GetVectorSize(const arVector_t *vec)
{
		AR_ASSERT(vec != NULL);
		return vec->count;
}


void			AR_ZeroVector(arVector_t *vec)
{
		AR_ASSERT(vec != NULL && vec->count > 0);
		AR_memset(vec->v, 0, sizeof(double) * vec->count);
}

void			AR_NegateVector(arVector_t *vec)
{
		size_t i;
		AR_ASSERT(vec != NULL);

		for(i = 0; i < vec->count; ++i)
		{
				vec->v[i] = -vec->v[i];
		}

}

void			AR_ClampVector(arVector_t *vec, double minval, double maxval)
{
		size_t i;
		AR_ASSERT(vec != NULL);

		for(i = 0; i < vec->count; ++i)
		{
				if(AR_DBL_LE(vec->v[i], minval))
				{
						vec->v[i] = minval;
				
				}else if(AR_DBL_GE(vec->v[i], maxval))
				{
						vec->v[i] = maxval;
				}

		}
}



double			AR_CalcVectorLength(const arVector_t *vec)
{
		double ret;
		AR_ASSERT(vec != NULL);

		ret = AR_CalcVectorLengthSqr(vec);

		return AR_sqrt(ret);
}

double			AR_CalcVectorLengthSqr(const arVector_t *vec)
{
		size_t i;
		double ret;
		AR_ASSERT(vec != NULL);

		ret = 0.0;

		for(i = 0; i < vec->count; ++i)
		{
				ret += vec->v[i] * vec->v[i];
		}

		return ret;
		
}

void			AR_NormalizeVector(arVector_t *vec)
{
		double len = 0.0;
		AR_ASSERT(vec != NULL);

		len = AR_CalcVectorLength(vec);

		if(!AR_DBL_EQ(len, 0.0))
		{
				AR_DivVectorByVal(vec, len);
		}
}


double			AR_CalcVectorDistanceByVector(const arVector_t *vec, const arVector_t *other)
{
		arVector_t *tmp;
		double ret;
		AR_ASSERT(vec != NULL && other != NULL);
		AR_ASSERT(vec->count == other->count);
		
		tmp = NULL;
		ret = 0.0;

		tmp = AR_CopyNewVector(vec);
		AR_SubVectorByVector(tmp, other);
		ret = AR_CalcVectorLength(tmp);
		AR_DestroyVector(tmp);
		return ret;
}


const double*	AR_GetVectorData(const arVector_t *vec)
{
		AR_ASSERT(vec != NULL);
		return vec->v;
}

void			AR_SwapElements(arVector_t *vec, size_t l, size_t r)
{
		double t;
		AR_ASSERT(vec != NULL);
		AR_ASSERT(l < vec->count && r < vec->count);
		t = vec->v[l];
		vec->v[l] = vec->v[r];
		vec->v[r] = t;
}


int_t			AR_CompareVector(const arVector_t *l, const arVector_t *r)
{
		size_t i;
		AR_ASSERT(l != NULL && r != NULL);
		AR_ASSERT(l->count == r->count);

		for(i = 0; i < l->count; ++i)
		{
				if(!AR_DBL_EQ(l->v[i], r->v[i]))
				{
						return AR_DBL_LE(l->v[i], r->v[i]) ? -1 : 1;
				}
		}

		return 0;
}


int_t			AR_CompareVectorWithEpsilon(const arVector_t *l, const arVector_t *r, double epsilon)
{
		size_t i;
		AR_ASSERT(l != NULL && r != NULL);
		AR_ASSERT(l->count == r->count);

		for(i = 0; i < l->count; ++i)
		{
				double e = AR_abs_dbl(l->v[i] - r->v[i]);
				if(AR_DBL_GE(e, epsilon))
				{
						return AR_DBL_LE(l->v[i], r->v[i]) ? -1 : 1;
				}
		}

		return 0;

}


double			AR_GetVectorValue(const arVector_t *vec, size_t idx)
{
		AR_ASSERT(vec != NULL);
		AR_ASSERT(idx < vec->count);

		return vec->v[idx];
}

void			AR_SetVectorValue(arVector_t *vec, size_t idx, double val)
{
		AR_ASSERT(vec != NULL);
		AR_ASSERT(idx < vec->count);

		vec->v[idx] = val;
}

void			AR_AddVectorByVector(arVector_t *vec, const arVector_t *other)
{
		size_t i;
		AR_ASSERT(vec != NULL && other != NULL);
		AR_ASSERT(vec->count == other->count);

		for(i = 0; i < vec->count; ++i)
		{
				vec->v[i] += other->v[i];
		}
}

void			AR_SubVectorByVector(arVector_t *vec, const arVector_t *other)
{
		size_t i;
		AR_ASSERT(vec != NULL && other != NULL);
		AR_ASSERT(vec->count == other->count);

		for(i = 0; i < vec->count; ++i)
		{
				vec->v[i] -= other->v[i];
		}
}


void			AR_MulVectorByVal(arVector_t *vec, double val)
{
		size_t i;
		AR_ASSERT(vec != NULL);
		for(i = 0; i < vec->count; ++i)
		{
				vec->v[i] *= val;
		}
}

void			AR_DivVectorByVal(arVector_t *vec, double val)
{
		AR_ASSERT(vec != NULL);
		AR_ASSERT(val != 0.0);

		AR_MulVectorByVal(vec, 1.0 / val);

}

double			AR_MulVectorByVector(arVector_t *vec, const arVector_t *other)
{
		size_t i;
		double ret = 0.0;
		AR_ASSERT(vec != NULL && other != NULL);
		AR_ASSERT(vec->count == other->count);

		for(i = 0; i < vec->count; ++i)
		{
				ret += (vec->v[i] * other->v[i]);
		}
		
		return ret;
}


void			AR_VectorToString(const arVector_t *vec, arString_t *str)
{
		size_t i;
		AR_ASSERT(vec != NULL && str != NULL);

		i = 0;

		return;
}

/******************************************************Matrix**************************************************************/

struct __arsenal_matrix_tag
{
		size_t			nrows;
		size_t			ncols;
		double			*m;
};




	



AR_NAMESPACE_END
