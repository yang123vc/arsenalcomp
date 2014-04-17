
#include "image_hash.h"








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


arStatus_t phash_image(CByteImage_t &src, std::string &hash, const size_t SAMPLE_SIZE)
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

				}else if (src.spectrum() == 4)
				{
						int width = filtered_img.width();
						int height = filtered_img.height();
						int depth = filtered_img.depth();
						filtered_img = src.crop(0,0,0,0,width-1,height-1,depth-1,2).RGBtoYCbCr().channel(0).get_convolve(meanfilter);
				} else 
				{
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
						AR_error(AR_ERR_WARNING, L"failed to generate dct2 transform matrix : %hs\r\n", AR_FUNC_NAME);
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
						AR_error(AR_ERR_WARNING, L"failed to transform matrix to dct2 form : %hs\r\n", AR_FUNC_NAME);
						goto END_POINT;
				}



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


ar_bool_t phash_image_file(const wchar_t *file, std::string &hash)
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



