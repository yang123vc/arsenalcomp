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


/******************************************************Matrix**************************************************************/

struct __arsenal_matrix_tag
{
		size_t			nrows;
		size_t			ncols;
		double			*m;
};

arMatrix_t*		AR_CreateMatrix(size_t rows, size_t cols)
{
		arMatrix_t *mat;
		AR_ASSERT(rows > 0 && cols > 0);
		mat = AR_NEW(arMatrix_t);
		mat->nrows = rows;
		mat->ncols = cols;
		mat->m = AR_NEWARR0(double, rows * cols);
		return mat;

}

void			AR_DestroyMatrix(arMatrix_t *mat)
{
		if(mat)
		{
				if(mat->m)
				{
						AR_DEL(mat->m);
						mat->m = NULL;
				}

				AR_DEL(mat);
				mat = NULL;
		}

}


arMatrix_t*		AR_CopyNewMatrix(const arMatrix_t *mat)
{
		arMatrix_t *ret;
		AR_ASSERT(mat != NULL);
		ret = AR_CreateMatrix(mat->nrows, mat->ncols);
		AR_memcpy(ret->m, mat->m, mat->nrows * mat->ncols * sizeof(double));
		return ret;
}

void			AR_CopyMatrix(arMatrix_t *dest, const arMatrix_t *src)
{
		AR_ASSERT(dest != NULL && src != NULL);

		if(dest == src)
		{
				return;
		}

		AR_SetMatrixSize(dest, src->nrows, src->ncols);

		AR_memcpy(dest->m, src->m, src->nrows * src->ncols * sizeof(double));
		
		
}


void			AR_SetMatrixSize(arMatrix_t *mat, size_t rows, size_t cols)
{
		AR_ASSERT(mat != NULL);
		AR_ASSERT(rows > 0 && cols > 0);
		
		if(mat->nrows != rows || mat->ncols != cols)
		{
				mat->nrows = rows;
				mat->ncols = cols;
				mat->m = AR_REALLOC(double, mat->m, rows * cols);
		}
}

void			AR_ZeroMatrix(arMatrix_t *mat)
{
		AR_ASSERT(mat != NULL);
		AR_memset(mat->m, 0, sizeof(double) * mat->nrows * mat->ncols);
}


size_t			AR_GetMatrixNumRows(const arMatrix_t *mat)
{
		AR_ASSERT(mat != NULL);
		return mat->nrows;

}

size_t			AR_GetMatrixNumColumns(const arMatrix_t *mat)
{
		AR_ASSERT(mat != NULL);
		return mat->ncols;
}

int_t			AR_CompareMatrix(const arMatrix_t *l, const arMatrix_t *r, double epsilon)
{
		size_t i;
		AR_ASSERT(l != NULL && r != NULL);
		AR_ASSERT(l->nrows == r->nrows && l->ncols == r->ncols);

		for(i = 0; i < l->nrows * l->ncols; ++i)
		{
				double e = l->m[i] - r->m[i];

				if(AR_abs_dbl(e) > epsilon)
				{
						if(e < 0.0)
						{
								return -1;
						}else
						{
								return 1;
						}
				}
		}
		
		return 0;

}



void			AR_SetMatrixValue(arMatrix_t *mat, size_t row, size_t col, double val)
{
		AR_ASSERT(mat != NULL);
		AR_ASSERT(row < mat->nrows);
		AR_ASSERT(col < mat->ncols);

		mat->m[row * mat->ncols + col] = val;
}

double			AR_GetMatrixValue(const arMatrix_t *mat, size_t row, size_t col)
{
		AR_ASSERT(mat != NULL);
		AR_ASSERT(row < mat->nrows);
		AR_ASSERT(col < mat->ncols);
		return mat->m[row * mat->ncols + col];
}

void			AR_GetMatrixRow(const arMatrix_t *mat, size_t row,  arVector_t *out)
{
		size_t i;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(row < mat->nrows);
		AR_ASSERT(out != NULL);
		AR_ChangeVectorSize(out, mat->ncols);

		for(i = 0; i < mat->ncols; ++i)
		{
				double v = mat->m[row * mat->ncols + i];
				AR_SetVectorValue(out, i, v);
		}

}

void			AR_GetMatrixColumn(const arMatrix_t *mat, size_t col,  arVector_t *out)
{
		size_t i;

		AR_ASSERT(mat != NULL);
		AR_ASSERT(col < mat->ncols);
		AR_ASSERT(out != NULL);

		AR_ChangeVectorSize(out, mat->ncols);

		for(i = 0; i < mat->nrows; ++i)
		{
				double v = mat->m[i * mat->ncols + col];
				AR_SetVectorValue(out, i, v);
		}
}

const double*	AR_GetMatrixRawData(const arMatrix_t *mat)
{
		AR_ASSERT(mat != NULL);
		return mat->m;
}



void			AR_IdentityMatrix(arMatrix_t *mat)
{
		size_t i;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(mat->nrows == mat->ncols);

		AR_ZeroMatrix(mat);

		for(i = 0; i < mat->nrows; ++i)
		{
				AR_SetMatrixValue(mat, i, i, 1.0);
		}

}

void			AR_DiagonalMatrix(arMatrix_t *mat, const arVector_t *vec)
{
		size_t i;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(vec != NULL);
		AR_ASSERT(AR_GetVectorSize(vec) > 0);
		AR_SetMatrixSize(mat, AR_GetVectorSize(vec), AR_GetVectorSize(vec));

		for(i = 0; i < AR_GetVectorSize(vec); ++i)
		{
				double v = AR_GetVectorValue(vec, i);
				AR_SetMatrixValue(mat, i,i,v);
		}

}

void			AR_RandomMatrix(arMatrix_t *mat)
{
		size_t i;
		AR_ASSERT(mat != NULL);

		for(i = 0; i < mat->nrows * mat->ncols; ++i)
		{
				mat->m[i] = AR_rand_dbl();
		}
}

void			AR_NegateMatrix(arMatrix_t *mat)
{
		size_t i;
		AR_ASSERT(mat != NULL);

		for(i = 0; i < mat->nrows * mat->ncols; ++i)
		{
				mat->m[i] = -mat->m[i];
		}

}

void			AR_ClampMatrix(arMatrix_t *mat, double min_val, double max_val)
{
		size_t i;
		AR_ASSERT(mat != NULL);

		for(i = 0; i < mat->nrows * mat->ncols; ++i)
		{
				if(AR_DBL_LE(mat->m[i], min_val))
				{
						mat->m[i] = min_val;
				}else if(AR_DBL_GE(mat->m[i], max_val))
				{
						mat->m[i] = max_val;
				}
		}

}

void			AR_SwapMatrixRows(arMatrix_t *mat, size_t l, size_t r)
{
		double *tmp, *lptr, *rptr;
		
		AR_ASSERT(mat != NULL);
		AR_ASSERT(l < mat->nrows);
		AR_ASSERT(r < mat->nrows);

		tmp = NULL; lptr = NULL; rptr = NULL;

		if(l == r)
		{
				return;
		}
		
		tmp = AR_NEWARR(double, mat->ncols);

		lptr = &mat->m[l * mat->ncols];
		rptr = &mat->m[r * mat->ncols];

		
		AR_memcpy(tmp, lptr, sizeof(double) * mat->ncols);
		AR_memcpy(lptr, rptr, sizeof(double) * mat->ncols);
		AR_memcpy(rptr, tmp, sizeof(double) * mat->ncols);

		AR_DEL(tmp);
		tmp = NULL;
}


void			AR_SwapMatrixColumns(arMatrix_t *mat, size_t l, size_t r)
{
		
		size_t i;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(l < mat->ncols);
		AR_ASSERT(r < mat->ncols);

		for(i = 0; i < mat->nrows; ++i)
		{
				double lv,rv;

				lv = AR_GetMatrixValue(mat, i, l);
				rv = AR_GetMatrixValue(mat, i, r);
				AR_SetMatrixValue(mat, i, l, rv);
				AR_SetMatrixValue(mat, i, r, lv);
		}
}


void			AR_RemoveMatrixRow(arMatrix_t *mat, size_t r)
{
		size_t i;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(mat->nrows > 1);
		AR_ASSERT(r < mat->nrows);

		mat->nrows -= 1;

		for(i = r; i < mat->nrows; ++i)
		{
				double *np = &mat->m[i * mat->ncols];
				double  *op = &mat->m[(i + 1) * mat->ncols];
				AR_memcpy(np, op, sizeof(double) * mat->ncols);
		}


}


void			AR_RemoveMatrixColumn(arMatrix_t *mat, size_t c)
{
		size_t i;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(c < mat->ncols);
		AR_ASSERT(mat->ncols > 1);

		mat->ncols--;

		for(i = 0; i < mat->nrows-1; ++i)
		{
				double *p1 = &mat->m[i * mat->ncols + c];
				double *p2 = &mat->m[i * (mat->ncols + 1) + c + 1];
				AR_memmove(p1, p2, sizeof(double) * mat->ncols);
		}

		if(mat->ncols - c > 0)
		{
				AR_memmove(&mat->m[i * mat->ncols + c], &mat->m[i * (mat->ncols + 1) + c + 1], sizeof(double) * (mat->ncols - c));
		}
}


void			AR_ClearMatrixUpperTriangle(arMatrix_t *mat)
{
		size_t i,j;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(mat->nrows == mat->ncols);
		for(i = 0; i < mat->nrows - 1; ++i)
		{
				for(j = i + 1; j < mat->ncols; ++j)
				{
						AR_SetMatrixValue(mat, i,j, 0.0);
				}
		}
}

void			AR_ClearMatrixLowerTriangle(arMatrix_t *mat)
{
		size_t i,j;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(mat->nrows == mat->ncols);

		for(i = 1; i < mat->nrows; ++i)
		{
				for(j = 0; j < i; ++j)
				{
						AR_SetMatrixValue(mat, i,j, 0.0);
				}
		}
}



bool_t			AR_IsSquareMatrix(const arMatrix_t *mat)
{
		AR_ASSERT(mat != NULL);
		return mat->nrows == mat->ncols ? true : false;
}


bool_t			AR_IsZeroMatrix(const arMatrix_t *mat, double epsilon)
{
		size_t i;
		AR_ASSERT(mat != NULL);
		for(i = 0; i < mat->ncols * mat->nrows; ++i)
		{
				if(AR_abs_dbl(mat->m[i]) > epsilon)
				{
						return false;
				}
		}

		return true;
}

bool_t			AR_IsIdentityMatrix(const arMatrix_t *mat, double epsilon)
{
		size_t i,j;
		AR_ASSERT(mat != NULL);

		if(!AR_IsSquareMatrix(mat))
		{
				return false;
		}

		for(i = 0; i < mat->nrows; ++i)
		{
				for(j = 0; j < mat->ncols; ++j)
				{
						const double v = i == j ? 1.0 : 0.0;

						if(AR_abs_dbl(mat->m[i * mat->ncols + j] - v) > epsilon)
						{
								return false;
						}
				}

		}
		return true;
}

bool_t			AR_IsDiagonalMatrix(const arMatrix_t *mat, double epsilon)
{
		size_t i,j;
		AR_ASSERT(mat != NULL);

		if(!AR_IsSquareMatrix(mat))
		{
				return false;
		}

		for(i = 0; i < mat->nrows; ++i)
		{
				for(j = 0; j < mat->ncols; ++j)
				{
						if(i != j && AR_abs_dbl(mat->m[i * mat->ncols + j]) > epsilon)
						{
								return false;
						}
				}
		}

		return true;
}

bool_t			AR_IsTriDiagonalMatrix(const arMatrix_t *mat, double epsilon)
{
		size_t i,j;
		double v;
		AR_ASSERT(mat != NULL);

		if(!AR_IsSquareMatrix(mat))
		{
				return false;
		}

		for(i = 0; i < mat->nrows - 2; ++i)
		{
				for(j = i + 2; j < mat->ncols; ++j)
				{
						v = AR_GetMatrixValue(mat, i,j);

						if(AR_abs_dbl(v) > epsilon)
						{
								return false;
						}

						v = AR_GetMatrixValue(mat, j,i);
						
						if(AR_abs_dbl(v) > epsilon)
						{
								return false;
						}

				}
		}

		return true;

}

bool_t			AR_IsSymmetricMatrix(const arMatrix_t *mat, double epsilon)
{
		size_t i,j;
		double l,r;
		AR_ASSERT(mat != NULL);

		if(!AR_IsSquareMatrix(mat))
		{
				return false;
		}

		for(i = 0; i < mat->nrows; ++i)
		{
				for(j = i + 1; j < mat->ncols; ++j)
				{
						l = AR_GetMatrixValue(mat, i,j);
						r = AR_GetMatrixValue(mat, j,i);

						if(AR_abs_dbl(l - r) > epsilon)
						{
								return false;
						}
				}
		}

		return true;
}

bool_t			AR_IsOrthogonalMatrix(const arMatrix_t *mat, double epsilon)
{
		size_t i,j,k;
		
		double s;
		AR_ASSERT(mat != NULL);


		for(i = 0; i < mat->ncols; ++i)
		{
				for(j = i + 1; j < mat->ncols; ++j)
				{
						
						s = 0.0;

						for(k = 0; k < mat->nrows; ++k)
						{
								double l,r;
								l = AR_GetMatrixValue(mat,k,i);
								r = AR_GetMatrixValue(mat,k,j);
								s += (l * r);
						}

						if(AR_abs_dbl(s) > epsilon)
						{
								return false;
						}
				}
		}

		return true;
}


bool_t			AR_IsOrthonormalMatrix(const arMatrix_t *mat, double epsilon)
{
		size_t i,j,k;
		
		double s;
		AR_ASSERT(mat != NULL);


		for(i = 0; i < mat->ncols; ++i)
		{
				for(j = i; j < mat->ncols; ++j)
				{
						s = i == j ? -1.0 : 0.0;
						
						for(k = 0; k < mat->nrows; ++k)
						{
								double l,r;
								l = AR_GetMatrixValue(mat,k,i);
								r = AR_GetMatrixValue(mat,k,j);
								s += (l * r);
						}

						if(AR_abs_dbl(s) > epsilon)
						{
								return false;
						}
				}
		}

		return true;


}

/*********************************未实现**************************************/

bool_t			AR_IsPositiveDefiniteMatrix(const arMatrix_t *mat, double epsilon);
bool_t			AR_IsSymmetricPositiveDefinite(const arMatrix_t *mat, double epsilon);
bool_t			AR_IsPositiveSemiDefinite(const arMatrix_t *mat, double epsilon);
bool_t			AR_IsSymmetricPositiveSemiDefinite(const arMatrix_t *mat, double epsilon);

/*********************************************************************************/
void			AR_MultiplyMatrixByScalar(const arMatrix_t *mat, double value, arMatrix_t *dest)
{
		size_t i;
		AR_ASSERT(mat != NULL && dest != NULL);

		AR_SetMatrixSize(dest, mat->nrows, mat->ncols);

		for(i = 0; i < mat->nrows * mat->ncols; ++i)
		{
				dest->m[i] = mat->m[i] * value;
		}
}

void			AR_MultiplyMatrixByScalarSelf(arMatrix_t *mat, double value)
{
		size_t i;
		AR_ASSERT(mat != NULL);
		for(i = 0; i < mat->nrows * mat->ncols; ++i)
		{
				mat->m[i] *= value;
		}
}

void			AR_MultiplyMatrixByVector(const arMatrix_t *mat, const arVector_t *other, arVector_t *dest)
{
		size_t m,n,i,j;
		double sum;
		
		AR_ASSERT(mat != NULL);
		AR_ASSERT(other != NULL && dest != NULL);
		m = AR_GetMatrixNumRows(mat);
		n = AR_GetMatrixNumColumns(mat);
		AR_ASSERT(n == AR_GetVectorSize(other));

		AR_ChangeVectorSize(dest, m);
		AR_ZeroVector(dest);

		for(i = 0; i < m; ++i)
		{
				sum = 0.0;
				for(j = 0; j < n; ++j)
				{
						sum += AR_GetMatrixValue(mat, i,j) * AR_GetVectorValue(other, j);
				}

				AR_SetVectorValue(dest, i, sum);
		}

}

void			AR_MultiplyTransposeMatrixByVector(const arMatrix_t *mat, const arVector_t *other, arVector_t *dest)
{
		size_t i,j,m,n;
		double sum;
		
		AR_ASSERT(mat != NULL && other != NULL && dest != NULL);

		m = AR_GetMatrixNumRows(mat);
		n = AR_GetMatrixNumColumns(mat);

		AR_ASSERT(m == AR_GetVectorSize(other));

		AR_ChangeVectorSize(dest, n);

		for(i = 0; i < n; ++i)
		{
				sum = 0.0;
				for(j = 0; j < m; ++j)
				{
						sum += AR_GetMatrixValue(mat, j,i) * AR_GetVectorValue(other,j);
				}
				AR_SetVectorValue(dest, i,sum);
		}

}


void			AR_MultiplyMatrixByMatrix(const arMatrix_t *mat, const arMatrix_t *other, arMatrix_t *dest)
{
		size_t i,j,k,lm,ln,rm,rn;
		double sum;

		AR_ASSERT(mat != NULL && other != NULL && dest != NULL);

		lm = AR_GetMatrixNumRows(mat);
		ln = AR_GetMatrixNumColumns(mat);

		rm = AR_GetMatrixNumRows(other);
		rn = AR_GetMatrixNumColumns(other);

		AR_ASSERT(ln == rm);

		AR_SetMatrixSize(dest, lm, rn);
		AR_ZeroMatrix(dest);

		for(i = 0; i < lm; ++i)
		{
				for(j = 0; j < rn; ++j)
				{
						sum = 0.0;
						for(k = 0; k < rm; ++k)
						{
								sum += AR_GetMatrixValue(mat, i,k) * AR_GetMatrixValue(other, k,j);
						}

						AR_SetMatrixValue(dest, i,j,sum);
				}
		}

}

void			AR_MultiplyTransposeMatrixByMatrix(const arMatrix_t *mat, const arMatrix_t *other, arMatrix_t *dest)
{
		size_t i,j,k,lrows,lcols,rrows,rcols;
		double sum;

		AR_ASSERT(mat != NULL && other != NULL && dest != NULL);

		lrows = AR_GetMatrixNumRows(mat);
		lcols = AR_GetMatrixNumColumns(mat);

		rrows = AR_GetMatrixNumRows(other);
		rcols = AR_GetMatrixNumColumns(other);

		AR_ASSERT(lrows == rrows);

		AR_SetMatrixSize(dest, lcols, rcols);
		AR_ZeroMatrix(dest);

		for(i = 0; i < lcols; ++i)
		{
				for(j = 0; j < rcols; ++j)
				{
						sum = 0.0;
						for(k = 0; k < lrows; ++k)
						{
								sum += AR_GetMatrixValue(mat, k,i) * AR_GetMatrixValue(other, k,j);
						}

						AR_SetMatrixValue(dest, i,j,sum);
				}
		}
}


void			AR_MultiplyMatrixByMatrixSelf(arMatrix_t *mat, const arMatrix_t *other)
{
		arMatrix_t *tmp;
		AR_ASSERT(mat != NULL && other != NULL);

		tmp = AR_CreateMatrix(mat->nrows, mat->ncols);
		
		AR_MultiplyMatrixByMatrix(mat, other, tmp);

		AR_CopyMatrix(mat, tmp);

		AR_DestroyMatrix(tmp);
		tmp = NULL;
}

void			AR_MultiplyTransposeMatrixByMatrixSelf(arMatrix_t *mat, const arMatrix_t *other)
{
		arMatrix_t *tmp;
		AR_ASSERT(mat != NULL && other != NULL);

		tmp = AR_CreateMatrix(mat->nrows, mat->ncols);
		
		AR_MultiplyTransposeMatrixByMatrix(mat, other, tmp);

		AR_CopyMatrix(mat, tmp);

		AR_DestroyMatrix(tmp);
		tmp = NULL;
}


void			AR_AddMatrixByMatrix(const arMatrix_t *mat, const arMatrix_t *other, arMatrix_t *dest)
{
		size_t i;
		AR_ASSERT(mat != NULL && other != NULL);
		AR_ASSERT(mat->nrows == other->nrows && mat->ncols == other->ncols);

		AR_SetMatrixSize(dest, mat->nrows, mat->ncols);

		for(i = 0; i < mat->nrows * mat->ncols; ++i)
		{
				dest->m[i] = mat->m[i] + other->m[i];
		}

}

void			AR_SubMatrixByMatrix(const arMatrix_t *mat, const arMatrix_t *other, arMatrix_t *dest)
{
		size_t i;
		AR_ASSERT(mat != NULL && other != NULL);
		AR_ASSERT(mat->nrows == other->nrows && mat->ncols == other->ncols);

		AR_SetMatrixSize(dest, mat->nrows, mat->ncols);

		for(i = 0; i < mat->nrows * mat->ncols; ++i)
		{
				dest->m[i] = mat->m[i] - other->m[i];
		}
}


void			AR_AddMatrixByMatrixSelf(arMatrix_t *mat, const arMatrix_t *other)
{
		size_t i;
		AR_ASSERT(mat != NULL && other != NULL);
		AR_ASSERT(mat->nrows == other->nrows && mat->ncols == other->ncols);

		for(i = 0; i < mat->nrows * mat->ncols; ++i)
		{
				mat->m[i] += other->m[i];
		}
}


void			AR_SubMatrixByMatrixSelf(arMatrix_t *mat, const arMatrix_t *other)
{
		size_t i;
		AR_ASSERT(mat != NULL && other != NULL);
		AR_ASSERT(mat->nrows == other->nrows && mat->ncols == other->ncols);

		for(i = 0; i < mat->nrows * mat->ncols; ++i)
		{
				mat->m[i] -= other->m[i];
		}
}





double			AR_CalcMatrixTrace(const arMatrix_t *mat)
{
		double val;
		size_t i;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(mat->nrows == mat->ncols);

		val = 0.0;
		for(i = 0; i < mat->nrows; ++i)
		{
				val += AR_GetMatrixValue(mat, i,i);
		}

		return val;
}

double			AR_CalcMatrixDeterminant(const arMatrix_t *mat)
{
		arMatrix_t *tmp;
		size_t *index;
		double det;
		AR_ASSERT(mat != NULL);

		det = 0.0;
		tmp = AR_CopyNewMatrix(mat);
		index = AR_NEWARR0(size_t, tmp->nrows);

		if(!AR_LUFactorMatrixSelf(tmp, index, &det))
		{
				det = 0.0;
		}

		AR_DestroyMatrix(tmp);
		tmp = NULL;

		AR_DEL(index);
		index = NULL;
		return det;
}


void			AR_TransposeMatrix(const arMatrix_t *mat, arMatrix_t *dest)
{
		size_t i,j;
		AR_ASSERT(mat != NULL && dest != NULL);

		AR_SetMatrixSize(dest, mat->ncols, mat->nrows);
		
		for(i = 0; i < mat->nrows; ++i)
		{
				for(j = 0; j < mat->ncols; ++j)
				{
						double v = AR_GetMatrixValue(mat, i,j);
						AR_SetMatrixValue(dest, j,i,v);
				}
		}

}


void			AR_TransposeMatrixSelf(arMatrix_t *mat)
{
		arMatrix_t *tmp;
		AR_ASSERT(mat != NULL);

		tmp = AR_CreateMatrix(mat->ncols, mat->nrows);

		AR_TransposeMatrix(mat, tmp);

		AR_CopyMatrix(mat, tmp);

		AR_DestroyMatrix(tmp);
		tmp = NULL;
}



/**************************通过LU取逆*************************************************

A = [ 0, 1, 2;   
      3, 4, 5;
      6, 7, 8
    ]
	;


inv(A) = [ x0, x1, x2;
           x3, x4, x5;
		   x6, x7, x8
		 ]
		 ;

A * inv(A) = I;

类似于

A * col0(inv(A) = [ 1;
					0;
					0
					];

A * col1(inv(A) = [ 0;
					1;
					0
					];


A * col2(inv(A) = [ 0;
					0;
					2
					];


***************************************************************************/

bool_t			AR_InverseMatrixSelf(arMatrix_t *mat)
{
		bool_t ret;
		size_t i,j;
		size_t *index;

		arMatrix_t *m_tmp;
		arVector_t *b_v_tmp, *x_v_tmp;

		AR_ASSERT(mat != NULL);
		AR_ASSERT(mat->nrows == mat->ncols);

		ret = true;
		index = AR_NEWARR0(size_t, mat->nrows);
		m_tmp = AR_CopyNewMatrix(mat);
		b_v_tmp = AR_CreateVector(mat->nrows);
		x_v_tmp = AR_CreateVector(mat->nrows);

		if(!AR_LUFactorMatrixSelf(m_tmp, index, NULL))
		{
				ret = false;
				goto END_POINT;
		}

		AR_ZeroVector(b_v_tmp);
		AR_ZeroVector(x_v_tmp);

		for(i = 0; i < mat->nrows; ++i)
		{
				AR_SetVectorValue(b_v_tmp, i, 1.0);

				AR_LUSolveMatrix(m_tmp, index, x_v_tmp, b_v_tmp);

				for(j = 0; j < mat->nrows; ++j)
				{
						AR_SetMatrixValue(mat, j,i, AR_GetVectorValue(x_v_tmp, j));
				}

				AR_SetVectorValue(b_v_tmp, i, 0.0);
		}

END_POINT:
		AR_DestroyVector(b_v_tmp);
		b_v_tmp = NULL;

		AR_DestroyVector(x_v_tmp);
		x_v_tmp = NULL;

		AR_DestroyMatrix(m_tmp);
		m_tmp = NULL;

		AR_DEL(index);
		index = NULL;

		return ret;
}


bool_t			AR_InverseLowerTriangularMatrixSelf(arMatrix_t *mat)
{
		size_t i,j,k;
		double d,sum,l,r;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(mat->nrows == mat->ncols);

		for(i = 0; i < mat->nrows; ++i)
		{
				d = AR_GetMatrixValue(mat, i,i);

				if(AR_DBL_EQ(d, 0.0))
				{
						return false;
				}

				d = 1.0 / d;
				AR_SetMatrixValue(mat, i,i, d);

				for(j = 0; j < i; ++j)
				{
						sum = 0.0;
						for(k = j; k < i; ++k)
						{
								l = AR_GetMatrixValue(mat, i,k);
								r = AR_GetMatrixValue(mat, k,j);
								sum -= l * r;
						}

						AR_SetMatrixValue(mat, i,j,sum * d);
				}

		}

		return true;
}

bool_t			AR_InverseUpperTriangularMatrixSelf(arMatrix_t *mat)
{
		int_t i,j,k;
		double d,sum,l,r;
		AR_ASSERT(mat != NULL);
		AR_ASSERT(mat->nrows == mat->ncols);

		for(i = (int_t)(mat->nrows - 1); i >= 0; --i)
		{
				d = AR_GetMatrixValue(mat, (size_t)i, (size_t)i);

				if(AR_DBL_EQ(d, 0.0))
				{
						return false;
				}

				d = 1.0 / d;
				AR_SetMatrixValue(mat, i,i, d);

				for(j = (int_t)(mat->nrows - 1); j > i; --j)
				{
						sum = 0.0;
						for(k = j; k > i; --k)
						{
								l = AR_GetMatrixValue(mat, i,k);
								r = AR_GetMatrixValue(mat, k,j);
								sum -= l * r;
						}

						AR_SetMatrixValue(mat, i,j,sum * d);
				}

		}

		return true;
}


/*

此算法取自《C++数值算法》第二章

此算法逻辑类似于从

A = [	1,		2;
		3,		4
	];

则 
A =  [	1,		2,		1,		0;
		3,		4,		0,		1
     ];

->	[	1,		2,		1,		0;
		3/4,	1,		0,		1/4
     ];
->因为这里选择的主元是A(1,1) 则，消去A(0,1)这列，则将第二行乘以A(0,1)，则

	[	1,		2,		1,		0;
		3/2,	2,		0,		1/2
     ];

->相减
	[	-1/2,	0,		1,		-1/2;
		3/2,	2,		0,		1/2
     ];

->继续选择主元A(0,0)，则将A(0,0)变为1，需要乘以-2则

	[	1,		0,		-2,		1;
		3/4,	1,		0,		1/4
     ];
->这里需要消去A(1,0),则将第一行乘以3/2，则

	[	3/4,	0,		-3/2,	3/4;
		3/4,	1,		0,		1/4
     ];
->
	[	3/4,	0,		-3/2,	3/4;
		0,		1,		3/2,	-1/2
     ];

->除以3/4
	[	1,		0,		-2,		1;
		0,		1,		3/2,	-1/2
     ];

注意，算法中为了节约空间，去掉了1,0所占用的信息
例如：

A =  [	1,		2；	
		3,		4；	
     ];

->	[	1,		2;
		3/4,	1/4
	];

-> d = 2;则此处2被消去，变换为0 - 1/4 * 2; 1 - 3/4*2为
	
	[	-1/2,	-1/2;
		3/4,	1/4
	];

之后主元为-1/2 乘以-2得到1，对应 (-1/2,	0,		1,		-1/2; )-> (1,0,-2,1)
-> 1,0被丢弃，保留-2,1										对应于：[	1,		0,		-2,		1;
																		3/4,	1,		0,		1/4
																	];
		[		-2,		1;		
				3/4,	1/4
		]
->																对应于：[	3/4,	0,		-3/2,	3/4;
																			3/4,	1,		0,		1/4
																		];
		d = 3/4; 
		[		-2,		1;		
				0,		1/4
		]

		所以 A(1,0) = 0 - -2 * d = 3/2 ; A(1,1) = 1/4 - 1 * d = -1/2;
->		[		-2,		1;		
				3/2,	-1/2
		]
		

*/

bool_t			AR_InverseMatrixByGaussJordanSelf(arMatrix_t *mat)
{
		bool_t ret;
		size_t i, j, k, r, c;
		double d, max_val, t;
		int_t x;
		size_t *col_index, *row_index;
		bool_t *pivot_mark;

		AR_ASSERT(mat != NULL);
		AR_ASSERT(mat->nrows == mat->ncols);

		ret = true;
		col_index = AR_NEWARR0(size_t, mat->nrows);
		row_index = AR_NEWARR0(size_t, mat->nrows);
		pivot_mark = AR_NEWARR0(bool_t, mat->nrows);
		

		for(i = 0; i < mat->nrows; ++i)
		{
				max_val = 0.0;
				r = 0; 
				c = 0;

				for(j = 0; j < mat->nrows; ++j)
				{
						if(pivot_mark[i])
						{
								continue;
						}
						
						for(k = 0; k < mat->ncols; ++k)
						{
								d = AR_abs_dbl(AR_GetMatrixValue(mat,j,k));

								if(AR_DBL_GE(d, max_val))
								{
										max_val = d;
										r = j;
										c = k;
								}
						}
				}

				if(AR_DBL_EQ(max_val, 0.0))
				{
						ret = false;
						goto END_POINT;
				}

				pivot_mark[c] = true;

				if(r != c)
				{
						
						for(k = 0; k < mat->ncols; ++k)
						{
								t = AR_GetMatrixValue(mat, r,k);
								AR_SetMatrixValue(mat, r,k, AR_GetMatrixValue(mat, c,k));
								AR_SetMatrixValue(mat, c,k, t);
						}
				}

				row_index[i] = r;
				col_index[i] = c;

				d = 1.0 / AR_GetMatrixValue(mat, c,c);
				AR_SetMatrixValue(mat, c,c, 1.0);

				for(k = 0; k < mat->ncols; ++k)
				{
						double t = AR_GetMatrixValue(mat, c,k);
						AR_SetMatrixValue(mat, c,k, t * d);
				}

				for(j = 0; j < mat->nrows; ++j)
				{
						if(j == c)
						{
								continue;
						}

						d = AR_GetMatrixValue(mat, j, c);
						AR_SetMatrixValue(mat, j, c, 0.0);
						for(k = 0; k < mat->ncols; ++k)
						{
								double l,r;
								l = AR_GetMatrixValue(mat, j,k);
								r = AR_GetMatrixValue(mat, c,k);
								AR_SetMatrixValue(mat, j,k, l - r * d);
						}
				}

		}

		
		for(x = (int_t)(mat->nrows - 1); x >= 0; x--)
		{
				if(row_index[x] != col_index[x])
				{
						for(k = 0; k < mat->nrows; ++k)
						{
								double t;
								d = AR_GetMatrixValue(mat, k, row_index[x]);
								t = AR_GetMatrixValue(mat, k, col_index[x]);
								AR_SetMatrixValue(mat, k, row_index[x], t);
								AR_SetMatrixValue(mat, k, col_index[x], d);
						}
				}
		}

END_POINT:
		AR_DEL(col_index);
		col_index = NULL;

		AR_DEL(row_index);
		row_index = NULL;

		AR_DEL(pivot_mark);
		pivot_mark = NULL;

		return ret;
}


void			AR_InverseSolveMatrix(const arMatrix_t *mat, arVector_t *x, const arVector_t *b)
{
		AR_MultiplyMatrixByVector(mat, b, x);
}




/*
LU分解

非方阵

L =  [	1	,	0
		l10	,	1
		l20	,	l21
     ];

U = [ 	u00	,	u01
		0	,	u11
    ];


则L = [	u00		,		u01
		l10 * u00	,		l10 * u01 + u11
		l20 * u00	,		l20 * u01 + l21 * u11
      ];

	  
方阵：
	L= 	[	1,		0,		0
			l10,	1,		0
			l20,	l21,	1
		];
	
	U=	[	u00,	u01,	u02
			0,		u11,	u12
			0,		0,		u22
		];


	A=	[	u00,				u01,					u02
			l10 * u00,			l10 * u01 + u11			l10 * u02 + u12
			l20 * u00,			l20 * u01 + l21 * u11,	l20 * u02 + l21 * u12 + u22
		];


算法：
A初始化为

	A=	[		u00,		u01,					u02
				l10 * u00,	l10 * u01 + u11			l10 * u02 + u12
				l20 * u00,	l20 * u01 + l21 * u11,	l20 * u02 + l21 * u12 + u22
		];


pivot = A(0,...) = [u00, u01, u02];
A10 = A10 / pivot[0] = l10;
A20 = A20 / pivot[0] = l20;

	A=	[	u00,		u01,					u02
			l10,		l10 * u01 + u11			l10 * u02 + u12
			l20,		l20 * u01 + l21 * u11,	l20 * u02 + l21 * u12 + u22
		];


A11 = A11 - l10 * pivot[1] = u11
A12 = A12 - l10 * pivot[2] = u12

A21 = A21 - l20 * pivot[1] = l21 * u11
A22 = A22 - l20 * pivot[2] = l21 * u12 + u22;

变换后A为：

	A=	[	u00,		u01,			u02
			l10,		u11				u12
			l20,		l21 * u11,		l21 * u12 + u22
		];


pivot = A(1,...) = [l10, u11, u12];
A21 = A21 / pivot[1] = l21 * u11 / u11 = l21;

	A=	[	u00,		u01,			u02
			l10,		u11			u12
			l20,		l21,			l21 * u12 + u22
		];


A22 = A22 - l21 * pivot[2] = l21 * u12 + u22 - l21 * u12 = u22;


	A=	[	u00,		u01,			u02
			l10,		u11			u12
			l20,		l21,			u22
		];
	


则LU分解完成
*/

bool_t			AR_LUFactorMatrixSelf(arMatrix_t *mat, size_t *index, double *det)
{
		size_t i,j,k,new_i,min_rc;
		double d, w, s,t;
		AR_ASSERT(mat != NULL);


		if(index)
		{
				for(i = 0; i < mat->nrows; ++i)
				{
						index[i] = i;
				}
		}

		w = 1.0f;

		min_rc = AR_MIN(mat->nrows, mat->ncols);

		for(i = 0; i < min_rc; ++i)
		{
				new_i = i;
				s = AR_abs_dbl(AR_GetMatrixValue(mat, i,i));

				if(index)
				{
						double t;
						for(j = i + 1; j < mat->nrows; ++j)
						{
								t = AR_abs_dbl(AR_GetMatrixValue(mat, j,i));

								if(AR_DBL_GE(t,s))
								{
										new_i = j;
										s = t;
								}
						}
				}

				if(AR_DBL_EQ(s, 0.0))
				{
						return false;
				}

				if(new_i != i )
				{
						w = -w; /*换行后，行列式符号改变*/

						k = index[i];
						index[i] = index[new_i];
						index[new_i] = k;

						for(k = 0; k < mat->ncols; ++k)
						{
								t = AR_GetMatrixValue(mat, new_i,k);
								AR_SetMatrixValue(mat, new_i, k, AR_GetMatrixValue(mat, i,k));
								AR_SetMatrixValue(mat, i, k, t);
						}
				}

				d = 1.0 / AR_GetMatrixValue(mat, i,i);
				for(j = i + 1; j < mat->nrows; ++j)
				{
						t = AR_GetMatrixValue(mat, j, i);
						AR_SetMatrixValue(mat, j, i, t * d);
				}

				if(i < min_rc - 1)
				{
						for(j = i + 1; j < mat->nrows; ++j)
						{
								double l_x = AR_GetMatrixValue(mat, j,i);
								
								for(k = i + 1; k < mat->ncols; ++k)
								{
										t = AR_GetMatrixValue(mat, j,k);
										t -= AR_GetMatrixValue(mat, i,k) * l_x;
										AR_SetMatrixValue(mat, j,k,t);
								}
						}
				}
		}
		
		if(det != NULL)
		{
				for(i = 0; i < min_rc; ++i)
				{
						w *= AR_GetMatrixValue(mat, i,i);
				}

				*det = w;
		}

		return true;
}



void			AR_LUSolveMatrix(const arMatrix_t *mat, const size_t *index, arVector_t *x,const arVector_t *b)
{
		int_t i,j;
		double s;
		AR_ASSERT(mat != NULL && b != NULL && x != NULL);

		AR_ASSERT(AR_GetVectorSize(b) == AR_GetMatrixNumRows(mat));
		AR_ChangeVectorSize(x, AR_GetMatrixNumColumns(mat));

		for(i = 0; i < (int_t)mat->nrows; ++i)
		{
				if(index)
				{
						s = AR_GetVectorValue(b, index[i]);
				}else
				{
						s = AR_GetVectorValue(b, i);
				}

				for(j = 0; j < i; ++j)
				{
						s -= AR_GetMatrixValue(mat, (size_t)i,(size_t)j) * AR_GetVectorValue(x, (size_t)j);
				}

				AR_SetVectorValue(x, (size_t)i,s);
		}


		for(i = (int_t)(AR_GetMatrixNumRows(mat) - 1); i >= 0; --i)
		{
				s = AR_GetVectorValue(x, i);
				for(j = i + 1; j < (int_t)mat->nrows; ++j)
				{
						s -= AR_GetMatrixValue(mat, i,j) * AR_GetVectorValue(x, j);
				}

				AR_SetVectorValue(x, i, s / AR_GetMatrixValue(mat, i,i));
		}

}

void			AR_LUInverseMatrix(const arMatrix_t *mat, const size_t *index, arMatrix_t *inv)
{
		size_t i,j;
		arVector_t *x,*b;
		AR_ASSERT(mat != NULL && inv != NULL);

		AR_ASSERT(mat->nrows == mat->ncols);

		x = AR_CreateVector(mat->ncols);
		b = AR_CreateVector(mat->nrows);
		AR_ZeroVector(b);
		AR_ZeroVector(x);
		AR_SetMatrixSize(inv, mat->nrows, mat->ncols);

		for(i = 0; i < mat->nrows; ++i)
		{
				AR_SetVectorValue(b, i, 1.0);
				AR_LUSolveMatrix(mat, index, x, b);
				for(j = 0; j < mat->nrows; ++j)
				{
						AR_SetMatrixValue(inv, j,i, AR_GetVectorValue(x,j));
				}

				AR_SetVectorValue(b, i, 0.0);
		}

		AR_DestroyVector(x);
		x = NULL;
		AR_DestroyVector(b);
		b = NULL;
}


void			AR_UnpackMatrixLUFactors(const arMatrix_t *mat, arMatrix_t *L, arMatrix_t *U)
{
		size_t i,j;
		size_t lm,ln,um,un;
		AR_ASSERT(mat != NULL && L != NULL && U != NULL);

		if(mat->nrows == mat->ncols)
		{
				lm = ln = um = un = mat->nrows;
		}else if(mat->nrows > mat->ncols)
		{
				lm = mat->nrows;
				ln = mat->ncols;

				um = un = mat->ncols;
		}else
		{
				lm = ln = mat->nrows;
				um = mat->nrows;
				un = mat->ncols;
		}

		AR_SetMatrixSize(L, lm,ln);
		AR_ZeroMatrix(L);
		AR_SetMatrixSize(U, um,un);
		AR_ZeroMatrix(U);

		for ( i = 0; i < mat->nrows; i++ )
		{
				for (j = 0; j < i; j++ ) 
				{
						if(i < lm && j < ln)
						{
								AR_SetMatrixValue(L, i,j, AR_GetMatrixValue(mat, i,j));
						}
				}

				if(i < lm && i < ln)
				{
						AR_SetMatrixValue(L, i,i, 1.0);
				}

				for(j = i; j < mat->ncols; ++j)
				{
						if(i < um && j < un)
						{
								AR_SetMatrixValue(U, i,j, AR_GetMatrixValue(mat, i,j));
						}
				}
		}

}


/*
输入：
		A=	[	u00,		u01,			u02
				l10,		u11			u12
				l20,		l21,			u22
			];

输出：
		A=	[		u00,		u01,					u02
					l10 * u00,	l10 * u01 + u11			l10 * u02 + u12
					l20 * u00,	l20 * u01 + l21 * u11,	l20 * u02 + l21 * u12 + u22
		];




*/

void			AR_MultiplyMatrixLUFactors(const arMatrix_t *mat, const size_t *index, arMatrix_t *original_mat)
{
		size_t r, original_r, c, k;
		double sum;

		AR_ASSERT(mat != NULL && original_mat != NULL);
		AR_SetMatrixSize(original_mat, mat->nrows, mat->ncols);
		AR_ZeroMatrix(original_mat);

		for ( r = 0; r < mat->nrows; r++ ) 
		{

				if ( index != NULL ) 
				{
						original_r = index[r];
				} else 
				{
						original_r = r;
				}

				for ( c = 0; c < mat->ncols; c++ ) 
				{
						if ( c >= r ) 
						{
								sum = AR_GetMatrixValue(mat, r,c);
						} else 
						{
								sum = 0.0f;
						}

						for ( k = 0; k <= c && k < r; k++) 
						{
								sum += AR_GetMatrixValue(mat, r,k) * AR_GetMatrixValue(mat, k,c);
						}

						AR_SetMatrixValue(original_mat, original_r, c, sum);
				}
		}
}



void			AR_MatrixToString(const arMatrix_t *mat, arString_t *str, size_t precision, const wchar_t *sp_str, const wchar_t *row_sp)
{
		size_t i;
		arVector_t *tmp;
		AR_ASSERT(mat != NULL && str != NULL);

		if(row_sp == NULL)
		{
				row_sp = AR_LINE_SP;
		}

		tmp = AR_CreateVector(mat->ncols);

		for(i = 0; i < mat->nrows; ++i)
		{
				AR_GetMatrixRow(mat, i, tmp);
				AR_VectorToString(tmp, str, precision, sp_str);
				AR_AppendString(str, row_sp);
		}

		AR_DestroyVector(tmp);
		tmp = NULL;
}



AR_NAMESPACE_END
