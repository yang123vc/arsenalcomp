
#include "test.h"


#include <math.h>
#include <time.h>

#if defined(__LIB)

AR_NAMESPACE_BEGIN




void misc_test()
{
		AR_printf(L"%d\r\n", AR_abs_32(-1));
		AR_printf(L"%d\r\n", AR_abs_32(1));

		AR_printf(L"%qd\r\n", AR_abs_64(-1));
		AR_printf(L"%qd\r\n", AR_abs_64(1));

		AR_printf(L"%f\r\n", AR_abs_dbl(-0.111f));
		AR_printf(L"%f\r\n", AR_abs_dbl(0.111f));
		
}


void __print_vector(const arVector_t *vec)
{
		arString_t *str = AR_CreateString();
		AR_VectorToString(vec, str, 2, NULL);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));
		AR_DestroyString(str);
		str = NULL;
}


void vector_test()
{
		//AR_srand(time(NULL));

		arVector_t *vec = AR_CreateVector(2);

#if(0)
		for(size_t i = 0; i < 3; ++i)
		{
				AR_SetVectorValue(vec, i, AR_rand_dbl());
		}
#endif

		AR_SetVectorValue(vec, 0, 3.0);
		AR_SetVectorValue(vec, 1, 4.0);


		{
		arString_t *out;

		out = AR_CreateString();

		AR_VectorToString(vec, out, 30, L",");

		AR_printf(L"%ls\r\n", AR_GetStringCString(out));

		AR_DestroyString(out);
		out = NULL;
		}

		double res = AR_CalcInnerProduct(vec, vec);

		AR_printf(L"%f\r\n", res);


		res = AR_CalcVectorLength(vec);

		AR_printf(L"%f\r\n", res);

		res = AR_CalcVectorLengthSqr(vec);

		AR_printf(L"%f\r\n", res);


		AR_NormalizeVector(vec);

		{
		arString_t *out;

		out = AR_CreateString();

		AR_VectorToString(vec, out, 30, L",");

		AR_printf(L"%ls\r\n", AR_GetStringCString(out));

		AR_DestroyString(out);
		out = NULL;
		}



		AR_DestroyVector(vec);
}


/*

arMatrix_t*		AR_CreateMatrix(size_t rows, size_t cols);
void			AR_DestroyMatrix(arMatrix_t *mat);

arMatrix_t*		AR_CopyNewMatrix(const arMatrix_t *mat);
void			AR_CopyMatrix(arMatrix_t *dest, const arMatrix_t *src);

void			AR_SetMatrixSize(arMatrix_t *mat, size_t rows, size_t cols);
void			AR_ZeroMatrix(arMatrix_t *mat);

int_t			AR_CompareMatrix(const arMatrix_t *l, const arMatrix_t *r, double epsilon);

size_t			AR_GetMatrixNumRows(const arMatrix_t *mat);
size_t			AR_GetMatrixNumColumns(const arMatrix_t *mat);

void			AR_SetMatrixValue(arMatrix_t *mat, size_t row, size_t col, double val);
double			AR_GetMatrixValue(const arMatrix_t *mat, size_t row, size_t col);
void			AR_GetMatrixRow(const arMatrix_t *mat, size_t row,  arVector_t *out);
void			AR_GetMatrixColumn(const arMatrix_t *mat, size_t col,  arVector_t *out);
const double*	AR_GetMatrixRawData(const arMatrix_t *mat);



/****************************************生成标准矩阵类型******************************************

void			AR_IdentityMatrix(arMatrix_t *mat);
void			AR_DiagonalMatrix(arMatrix_t *mat, const arVector_t *vec);
void			AR_RandomMatrix(arMatrix_t *mat);
void			AR_NegateMatrix(arMatrix_t *mat);
void			AR_ClampMatrix(arMatrix_t *mat, double min_val, double max_val);
void			AR_SwapMatrixRows(arMatrix_t *mat, size_t l, size_t r);
void			AR_SwapMatrixColumns(arMatrix_t *mat, size_t l, size_t r);
void			AR_RemoveMatrixRow(arMatrix_t *mat, size_t r);
void			AR_RemoveMatrixColumn(arMatrix_t *mat, size_t c);
void			AR_ClearMatrixUpperTriangle(arMatrix_t *mat);
void			AR_ClearMatrixLowerTriangle(arMatrix_t *mat);
*/

void __print_matrix(const arMatrix_t *mat)
{
		arString_t *str = AR_CreateString();
		AR_MatrixToString(mat, str, 2, NULL, NULL);
		AR_printf(L"%ls\r\n", AR_GetStringCString(str));
		AR_DestroyString(str);
		str = NULL;
}

void matrix_test()
{

		arMatrix_t *mat = NULL, *mat2 = NULL;
		arString_t *str = NULL;

		str = AR_CreateString();


		mat = AR_CreateMatrix(2,2);
		mat2 = AR_CreateMatrix(2,2);
		
		AR_SetMatrixSize(mat, 9,5);
		AR_ZeroMatrix(mat);

		AR_RandomMatrix(mat);
		__print_matrix(mat);

		AR_NegateMatrix(mat);
		__print_matrix(mat);
		
		AR_CopyMatrix(mat2, mat);
		__print_matrix(mat);

		int ret = AR_CompareMatrix(mat, mat2, DBL_EPSILON);
		AR_printf(L"cmp == %d\r\n", ret);

		AR_NegateMatrix(mat2);

		ret = AR_CompareMatrix(mat, mat2, DBL_EPSILON);
		AR_printf(L"cmp == %d\r\n", ret);

		ret = AR_CompareMatrix(mat2, mat, DBL_EPSILON);
		AR_printf(L"cmp == %d\r\n", ret);


		{
				arMatrix_t *nm = AR_CopyNewMatrix(mat);

				ret = AR_CompareMatrix(mat, nm, DBL_EPSILON);
				AR_printf(L"nm cmp == %d\r\n", ret);
				AR_DestroyMatrix(nm);
				nm = NULL;
		}

		{
				arMatrix_t *nm = AR_CopyNewMatrix(mat);

				AR_NegateMatrix(nm);
				AR_ClampMatrix(nm, 0.1, 0.2);
				__print_matrix(nm);
				
				AR_DestroyMatrix(nm);
				nm = NULL;
		}

		{
				arMatrix_t *nm = AR_CopyNewMatrix(mat);

				AR_SetMatrixSize(nm, 3,3);
				AR_IdentityMatrix(nm);

				__print_matrix(nm);

				
				arVector_t *vtmp = AR_CreateVector(3);
				AR_RandomVector(vtmp);
				__print_vector(vtmp);
				AR_printf(L"----\r\n");
				AR_DiagonalMatrix(nm, vtmp);
				__print_matrix(nm);
				
				AR_DestroyMatrix(nm);
				nm = NULL;

				if(vtmp)
				{
						AR_DestroyVector(vtmp);
						vtmp = NULL;
				}

		}

		{
				AR_SetMatrixSize(mat, 4,5);
				AR_RandomMatrix(mat);
				__print_matrix(mat);

				getchar();
				AR_printf(L"change 0-3\r\n");
				AR_SwapMatrixRows(mat, 0,3);
				__print_matrix(mat);

				AR_printf(L"change 1-2\r\n");
				AR_SwapMatrixRows(mat, 1,2);
				__print_matrix(mat);
				getchar();

				AR_printf(L"change 2-2\r\n");
				AR_SwapMatrixRows(mat, 2,2);
				__print_matrix(mat);
				getchar();


				AR_printf(L"change 0-4\r\n");
				AR_SwapMatrixColumns(mat, 0,4);
				__print_matrix(mat);

				AR_printf(L"change 1-3\r\n");
				AR_SwapMatrixColumns(mat, 1,3);
				__print_matrix(mat);
				getchar();

				AR_printf(L"change 2-2\r\n");
				AR_SwapMatrixColumns(mat, 2,2);
				__print_matrix(mat);
				getchar();


				AR_printf(L"remove 2\r\n");
				AR_RemoveMatrixRow(mat, 2);
				__print_matrix(mat);
				getchar();

				AR_printf(L"remove 0\r\n");
				AR_RemoveMatrixRow(mat, 0);
				__print_matrix(mat);
				getchar();

				AR_printf(L"remove 1\r\n");
				AR_RemoveMatrixRow(mat, 1);
				__print_matrix(mat);
				getchar();
				
				AR_SetMatrixSize(mat, 4,5);
				AR_RandomMatrix(mat);
				__print_matrix(mat);

				while(AR_GetMatrixNumColumns(mat) > 1)
				{
						uint_32_t n = AR_rand32() % AR_GetMatrixNumColumns(mat);
						AR_printf(L"remove %d\r\n", n);
						AR_RemoveMatrixColumn(mat, n);
						__print_matrix(mat);
						getchar();

				}
				
				AR_SetMatrixSize(mat, 5,5);
				AR_RandomMatrix(mat);
				__print_matrix(mat);

				AR_ClearMatrixUpperTriangle(mat);
				__print_matrix(mat);

				getchar();

				AR_SetMatrixSize(mat, 5,5);
				AR_RandomMatrix(mat);
				__print_matrix(mat);

				AR_ClearMatrixLowerTriangle(mat);
				__print_matrix(mat);

				getchar();
		}

		{
				AR_SetMatrixSize(mat, 5,5);
				AR_ZeroMatrix(mat);
				__print_matrix(mat);

				for(size_t i = 0; i < 5; ++i)
				{
						for(size_t j = 0; j < 5; ++j)
						{
								AR_SetMatrixValue(mat, i,j, j);
						}
				}

				__print_matrix(mat);

				/*
				void			(arMatrix_t *mat, size_t row, size_t col, double val);
				double			AR_GetMatrixValue(const arMatrix_t *mat, size_t row, size_t col);
				*/
		}


		{
				arMatrix_t *nm = AR_CopyNewMatrix(mat);

				AR_SetMatrixSize(nm, 4,4);
				AR_IdentityMatrix(nm);
				__print_matrix(nm);

				arVector_t *vtmp = AR_CreateVector(5);

				for(size_t i = 0; i < 4; ++i)
				{
						AR_GetMatrixColumn(nm, i,vtmp);
						__print_vector(vtmp);
						AR_printf(L"----\r\n");
				}

				
				

				
				

				AR_DestroyMatrix(nm);
				nm = NULL;

				if(vtmp)
				{
						AR_DestroyVector(vtmp);
						vtmp = NULL;
				}

				
		}
		

		


		if(mat)
		{
				AR_DestroyMatrix(mat);
				mat = NULL;
		}

		if(mat2)
		{
				AR_DestroyMatrix(mat2);
				mat2 = NULL;
		}

		if(str)
		{
				AR_DestroyString(str);
				str = NULL;
		}

}







void matrix_test2()
{

		arMatrix_t *mat = NULL, *mat2 = NULL;
		arString_t *str = NULL;
		arVector_t *vtmp = AR_CreateVector(5);

		str = AR_CreateString();


		mat = AR_CreateMatrix(2,2);
		mat2 = AR_CreateMatrix(2,2);
		


		
		AR_SetMatrixSize(mat, 5,5);
		AR_RandomMatrix(mat);


		AR_ASSERT(AR_IsSquareMatrix(mat));

		AR_IdentityMatrix(mat);
		AR_ASSERT(AR_IsIdentityMatrix(mat, DBL_EPSILON));

		AR_ZeroMatrix(mat);
		AR_ASSERT(AR_IsZeroMatrix(mat, DBL_EPSILON));

		AR_ChangeVectorSize(vtmp, AR_GetMatrixNumRows(mat));
		AR_RandomVector(vtmp);

		AR_DiagonalMatrix(mat, vtmp);
		
		AR_ASSERT(AR_IsDiagonalMatrix(mat, DBL_EPSILON));

/****************************************判断矩阵类型******************************************



bool_t			AR_IsTriDiagonalMatrix(const arMatrix_t *mat, double epsilon);
bool_t			AR_IsSymmetricMatrix(const arMatrix_t *mat, double epsilon);
bool_t			AR_IsOrthogonalMatrix(const arMatrix_t *mat, double epsilon);
bool_t			AR_IsOrthonormalMatrix(const arMatrix_t *mat, double epsilon);

bool_t			AR_IsPositiveDefiniteMatrix(const arMatrix_t *mat, double epsilon);
bool_t			AR_IsSymmetricPositiveDefinite(const arMatrix_t *mat, double epsilon);
bool_t			AR_IsPositiveSemiDefinite(const arMatrix_t *mat, double epsilon);
bool_t			AR_IsSymmetricPositiveSemiDefinite(const arMatrix_t *mat, double epsilon);
*/




		


END_POINT:
		if(mat)
		{
				AR_DestroyMatrix(mat);
				mat = NULL;
		}

		if(mat2)
		{
				AR_DestroyMatrix(mat2);
				mat2 = NULL;
		}

		if(vtmp)
		{
				AR_DestroyVector(vtmp);
				vtmp = NULL;
		}

		if(str)
		{
				AR_DestroyString(str);
				str = NULL;
		}



}



void math_test()
{
		AR_srand(time(NULL));
		//misc_test();
		//vector_test();

		//matrix_test();
		matrix_test2();


}

AR_NAMESPACE_END

#endif
