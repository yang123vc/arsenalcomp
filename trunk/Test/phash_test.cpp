

#if defined(__LIB)

#include "../Arsenal/Common/common.h"
#include "../Arsenal/Lex/lex.h"
#include "../Arsenal/Parser/parser.h"
#include "../Arsenal/Parser/lr_action.h"

#include "./extlib/CImg/CImg.h"
#include <string>
#include <vector>

AR_NAMESPACE_BEGIN



using namespace cimg_library;

extern void __print_matrix(const arMatrix_t *mat, size_t precision);

typedef CImg<ar_byte_t> CByteImage_t;

static ar_int_t __cmp_dbl(const void *l, const void *r)
{
		double ld = *(const double*)l;
		double rd = *(const double*)r;
		if(AR_DBL_LE(ld,rd))
		{
				return -1;
		}else if(AR_DBL_EQ(ld,rd))
		{
				return 0;
		}else
		{
				return 1;
		}
}


static double get_nth_elem(const std::vector<double> &vd, size_t nth)
{
		AR_ASSERT(nth <= vd.size() && !vd.empty());

		std::vector<double> tmp = vd;

		AR_nth_elem(&tmp[0], tmp.size(), sizeof(double), nth, __cmp_dbl);

		return tmp[nth];

}


static arStatus_t phash_image(CByteImage_t &src, std::string &hash, const size_t SAMPLE_SIZE = 8)
{


		arStatus_t status = AR_S_YES;
		CImg<float> meanfilter(7,7,1,1,1);
		CImg<float> filtered_img;
		
		arMatrix_t *img_mat = NULL;
		arMatrix_t *dct_matrix = NULL;


		try{

				if (src.spectrum() == 3)
				{
						filtered_img = src.RGBtoYCbCr().channel(0).get_convolve(meanfilter);

				} else if (src.spectrum() == 4)
				{
						int width = filtered_img.width();
						int height = filtered_img.height();
						int depth = filtered_img.depth();
						filtered_img = src.crop(0,0,0,0,width-1,height-1,depth-1,2).RGBtoYCbCr().channel(0).get_convolve(meanfilter);
				} else {
						filtered_img = src.channel(0).get_convolve(meanfilter);
				}


				filtered_img.resize(SAMPLE_SIZE*4,SAMPLE_SIZE*4);


				dct_matrix = AR_CreateMatrix(SAMPLE_SIZE*4,SAMPLE_SIZE*4);

				if(dct_matrix == NULL)
				{
						AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
						status = AR_E_NOMEM;
						goto END_POINT;
				}
				

				status = AR_GenerateTransformMatrix_DCT2(dct_matrix, SAMPLE_SIZE*4);
				if(status != AR_S_YES)
				{
						goto END_POINT;
				}
				

				img_mat = AR_CreateMatrix(SAMPLE_SIZE*4, SAMPLE_SIZE*4);
				
				if(img_mat == NULL)
				{
						AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
						status = AR_E_NOMEM;
						goto END_POINT;
				}


				for(size_t x = 0; x < SAMPLE_SIZE*4; ++x)
				{
						for(size_t y = 0; y < SAMPLE_SIZE*4; ++y)
						{
								float v = *filtered_img.data(x,y);
								AR_SetMatrixValue(img_mat, x,y, v);
						}
				}

				status = AR_MatrixTransform_DCT2(img_mat, dct_matrix);
				
				if(status != AR_S_YES)
				{
						goto END_POINT;
				}

				//__print_matrix(img_mat, 3);

				std::vector<double> result(SAMPLE_SIZE * SAMPLE_SIZE);

				for(size_t x = 0; x < SAMPLE_SIZE; ++x)
				{
						for(size_t y = 0; y < SAMPLE_SIZE; ++y)
						{
								result[x * SAMPLE_SIZE + y] = AR_GetMatrixValue(img_mat, x,y);
						}
				}


				double mid = get_nth_elem(result, result.size() / 2);


				hash = "";
				for (int i=0; i < SAMPLE_SIZE * SAMPLE_SIZE;i++)
				{
						double val = result[i];

						if(val > mid)
						{
								hash += "1";
						}else
						{
								hash += "0";
						}
				}

		}catch(...)
		{
				AR_error(AR_ERR_WARNING, L"failed to phash a image : %hs\r\n", AR_FUNC_NAME);
				status = AR_E_FAIL;
				goto END_POINT;
		}

END_POINT:
		
		if(dct_matrix)
		{
				AR_DestroyMatrix(dct_matrix);
				dct_matrix = NULL;
		}
		
		if(img_mat)
		{
				AR_DestroyMatrix(img_mat);
				img_mat = NULL;
		}

		return status;
}

static ar_bool_t phash_image_file(const wchar_t *file, std::string &hash)
{
		CByteImage_t src;
		char *s = AR_wcs_to_str(AR_CP_UTF8, file, AR_wcslen(file));
		if(s == NULL)
		{
				return false;
		}

		try{
				src.load(s);
				if(phash_image(src, hash, 16) != AR_S_YES)
				{
						throw std::exception();
				}

		}catch(...)
		{
				if(s)
				{
						AR_DEL(s);
						s = NULL;
				}
				return false;
		}

		if(s)
		{
				AR_DEL(s);
				s = NULL;
		}

		return true;
}


size_t hamming_distance(const std::string &l, const std::string &r)
{
		size_t count;
		AR_ASSERT(l.size() == r.size());
		
		count = 0;
		for(size_t i = 0; i < l.size(); ++i)
		{
				if(l[i] != r[i])
				{
						count++;
				}
		}

		return count;
}


static void phash_test1()
{
		
		for(size_t i = 0; i < 1000; ++i)
		{
		std::string w1h, w2h, w3h, w4h, w5h, w6h, w7h, w8h, w9h;

		std::string nw1h, nw2h, nw3h, nw4h;

		ar_bool_t is_ok;
		is_ok = phash_image_file(L"H:\\Code\\Test\\image\\Test\\worm.bmp", w1h);
		AR_ASSERT(is_ok);
		AR_printf(L"%hs\r\n", w1h.c_str());

		is_ok = phash_image_file(L"H:\\Code\\Test\\image\\Test\\worm2.bmp", w2h);
		AR_ASSERT(is_ok);
		AR_printf(L"%hs\r\n", w2h.c_str());

		is_ok = phash_image_file(L"H:\\Code\\Test\\image\\Test\\worm3.bmp", w3h);
		AR_ASSERT(is_ok);
		AR_printf(L"%hs\r\n", w3h.c_str());

		is_ok = phash_image_file(L"H:\\Code\\Test\\image\\Test\\worm4.bmp", w4h);
		AR_ASSERT(is_ok);
		AR_printf(L"%hs\r\n", w4h.c_str());
		

		is_ok = phash_image_file(L"H:\\Code\\Test\\image\\Test\\worm5.bmp", w5h);
		AR_ASSERT(is_ok);
		AR_printf(L"%hs\r\n", w5h.c_str());

		is_ok = phash_image_file(L"H:\\Code\\Test\\image\\Test\\worm6.bmp", w6h);
		AR_ASSERT(is_ok);
		AR_printf(L"%hs\r\n", w6h.c_str());

		is_ok = phash_image_file(L"H:\\Code\\Test\\image\\Test\\worm7.bmp", w7h);
		AR_ASSERT(is_ok);
		AR_printf(L"%hs\r\n", w7h.c_str());

		is_ok = phash_image_file(L"H:\\Code\\Test\\image\\Test\\worm8.bmp", w8h);
		AR_ASSERT(is_ok);
		AR_printf(L"%hs\r\n", w8h.c_str());

		is_ok = phash_image_file(L"H:\\Code\\Test\\image\\Test\\worm9.bmp", w9h);
		AR_ASSERT(is_ok);
		AR_printf(L"%hs\r\n", w9h.c_str());


		is_ok = phash_image_file(L"H:\\Code\\Test\\image\\Test\\non_worm.bmp", nw1h);
		AR_ASSERT(is_ok);
		AR_printf(L"%hs\r\n", nw1h.c_str());


		AR_printf(L"w1-w2 : %d\r\n", hamming_distance(w1h, w2h));
		AR_printf(L"w1-w3 : %d\r\n", hamming_distance(w1h, w3h));
		AR_printf(L"w1-w4 : %d\r\n", hamming_distance(w1h, w4h));
		AR_printf(L"w1-w5 : %d\r\n", hamming_distance(w1h, w5h));
		AR_printf(L"w1-w6 : %d\r\n", hamming_distance(w1h, w6h));
		AR_printf(L"w1-w7 : %d\r\n", hamming_distance(w1h, w7h));
		AR_printf(L"w1-w8 : %d\r\n", hamming_distance(w1h, w8h));
		AR_printf(L"w1-w9 : %d\r\n", hamming_distance(w1h, w9h));

		AR_printf(L"-----------------------------------\r\n");
		AR_printf(L"w2-w3 : %d\r\n", hamming_distance(w2h, w3h));
		AR_printf(L"w2-w4 : %d\r\n", hamming_distance(w2h, w4h));
		AR_printf(L"w2-w5 : %d\r\n", hamming_distance(w2h, w5h));
		AR_printf(L"w2-w6 : %d\r\n", hamming_distance(w2h, w6h));
		AR_printf(L"w2-w7 : %d\r\n", hamming_distance(w2h, w7h));
		AR_printf(L"w2-w8 : %d\r\n", hamming_distance(w2h, w8h));
		AR_printf(L"w2-w9 : %d\r\n", hamming_distance(w2h, w9h));

		
		AR_printf(L"-----------------------------------\r\n");
		AR_printf(L"w3-w4 : %d\r\n", hamming_distance(w3h, w4h));
		AR_printf(L"w3-w5 : %d\r\n", hamming_distance(w3h, w5h));
		AR_printf(L"w3-w6 : %d\r\n", hamming_distance(w3h, w6h));
		AR_printf(L"w3-w7 : %d\r\n", hamming_distance(w3h, w7h));
		AR_printf(L"w3-w8 : %d\r\n", hamming_distance(w3h, w8h));
		AR_printf(L"w3-w9 : %d\r\n", hamming_distance(w3h, w9h));


		AR_printf(L"***********************************************\r\n");
		AR_printf(L"w1-nw1h : %d\r\n", hamming_distance(w1h, nw1h));
		AR_printf(L"w2-nw1h : %d\r\n", hamming_distance(w2h, nw1h));
		AR_printf(L"w3-nw1h : %d\r\n", hamming_distance(w3h, nw1h));
		AR_printf(L"w4-nw1h : %d\r\n", hamming_distance(w4h, nw1h));
		AR_printf(L"w5-nw1h : %d\r\n", hamming_distance(w5h, nw1h));
		AR_printf(L"w6-nw1h : %d\r\n", hamming_distance(w6h, nw1h));
		AR_printf(L"w7-nw1h : %d\r\n", hamming_distance(w7h, nw1h));
		AR_printf(L"w8-nw1h : %d\r\n", hamming_distance(w8h, nw1h));
		AR_printf(L"w9-nw1h : %d\r\n", hamming_distance(w9h, nw1h));

		
		}

}



void phash_test()
{
		phash_test1();


}




AR_NAMESPACE_END



#endif

