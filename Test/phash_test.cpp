

#ifndef _UNICODE
		#define _UNICODE
#endif

#ifndef UNICODE
		#define UNICODE
#endif


#if defined(__LIB)

#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <algorithm>


#include "test.h"

//#define cimg_use_jpeg
#define cimg_display	0
#include "./extlib/CImg/CImg.h"


 #define _WTL_NO_CSTRING
 #include <atlstr.h>

#include <atlbase.h>
#include <atlapp.h>
#include <atlimage.h>
#include <atlgdi.h>
#include <atlwin.h>





AR_NAMESPACE_BEGIN



using namespace cimg_library;


typedef CImg<ar_byte_t> CByteImage_t;



class CScreenImage : public CImage
{
public:
    BOOL CaptureRect(const CRect& rect)
    {
        // detach and destroy the old bitmap if any attached
        CImage::Destroy();

        // create a screen and a memory device context
        HDC hDCScreen = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
        HDC hDCMem = ::CreateCompatibleDC(hDCScreen);
        // create a compatible bitmap and select it in the memory DC
        HBITMAP hBitmap = ::CreateCompatibleBitmap(hDCScreen, rect.Width(), rect.Height());
        HBITMAP hBmpOld = (HBITMAP)::SelectObject(hDCMem, hBitmap);

        // bit-blit from screen to memory device context
        // note: CAPTUREBLT flag is required to capture layered windows
        DWORD dwRop = SRCCOPY | CAPTUREBLT;
        BOOL bRet = ::BitBlt(hDCMem, 0, 0, rect.Width(), rect.Height(), hDCScreen, rect.left, rect.top, dwRop);
        // attach bitmap handle to this object



        Attach(hBitmap);

        // restore the memory DC and perform cleanup
        ::SelectObject(hDCMem, hBmpOld);
        ::DeleteDC(hDCMem);
        ::DeleteDC(hDCScreen);

        return bRet;
    }

    /****************************************************************************************
    Function:   CScreenImage::CaptureScreen
    Purpose:    captures the whole screen
    Parameters: none
    Return:     non-zero value if successful
    ****************************************************************************************/
    BOOL CaptureScreen()
    {
        CRect rect(0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
        return CaptureRect(rect);
    }

    /****************************************************************************************
    Function:   CScreenImage::CaptureWindow
    Purpose:    captures a window
    Parameters: hWnd - the window handle
    Return:     non-zero value if successful
    ****************************************************************************************/
    BOOL CaptureWindow(HWND hWnd)
    {
        BOOL bRet = FALSE;
        if(::IsWindow(hWnd))
        {
            CRect rect;
            ::GetWindowRect(hWnd, rect);
            bRet = CaptureRect(rect);
        }
        return bRet;
    }


	BOOL CaptureWindowEx(HWND hWnd)
	{
			CRect rect;
            ::GetWindowRect(hWnd, rect);
            int width = rect.Width();
            int height = rect.Height();

			//if(rect.Width() == 0 || rect.Height() == 0)
            if(width == 0 || height == 0)
			{
					return FALSE;
			}

			this->Destroy();

			if(!this->Create(rect.Width(), rect.Height(), 24))
			{
					return FALSE;
			}

			HDC dc = this->GetDC();

			if(!PrintWindow(hWnd, dc, 0))
			{
					this->Destroy();
					return FALSE;
			}
			
			this->ReleaseDC();

			return TRUE;
	}




	BOOL ScaleTo(CImage &dest_img, size_t width, size_t height)
	{
			BOOL result = FALSE;

			HDC dest_dc = NULL;
			dest_img.Destroy();

			if(!dest_img.Create((int)width, (int)height, 24/*this->GetBPP()*/))
			{
					result = FALSE;
					goto END_POINT;
			}


			dest_dc = dest_img.GetDC();
			::SetStretchBltMode(dest_dc,HALFTONE);
			::SetBrushOrgEx(dest_dc,0,0,NULL);

			result = this->StretchBlt(dest_dc,CRect(0,0, (int)width, (int)height)/*DestRect*/,CRect(0,0,this->GetWidth(),this->GetHeight())/*SourceRect*/,SRCCOPY);

			if(!result)
			{
					goto END_POINT;
			}

			result = TRUE;
END_POINT:
			if(dest_dc)
			{
				dest_img.ReleaseDC();
				dest_dc = NULL;
			}

			return result;
	}



};



#if(0)

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

#else

static double get_nth_elem(const std::vector<double> &vd, size_t nth)
{
		AR_ASSERT(nth <= vd.size() && !vd.empty());

        //因为稳定的输入N和调用频率，这种方式不会成为性能问题，所以没实现qsort_select一类算法。
		std::vector<double> tmp = vd;
        std::sort(tmp.begin(), tmp.end());     
        return tmp[nth];
}



#endif

#define HASH_SAMPLE_SIZE    16
#define SCALED_IMAGE_SIZE   64     //dct变换仅支持方阵using namespace ATL;



static inline std::wstring __bitset_to_hex_string(const std::vector<bool> &vbits)
{

		std::wstring ret = L"";
		
		static const wchar_t *tbl = L"0123456789ABCDEF";

		AR_ASSERT(vbits.size() > 0);

        std::vector<BYTE>		vb(vbits.size() / 8 + (vbits.size() % 8 ? 1 : 0), (BYTE)0);

		size_t k;
		for(k = 0; k < vbits.size() - vbits.size() % 8; ++k)
		{
				size_t pos = k / 8;
				BYTE &b = vb[pos];
				if(vbits[k])
				{
						b |= (BYTE)1 << (BYTE)(8 - k % 8 - 1);
				}
		}
		

		for(k; k < vbits.size(); ++k)
		{
				size_t pos = k / 8;
				BYTE &b = vb[pos];

				if(vbits[k])
				{
						BYTE bit_pos = 8 - (8 - vbits.size() % 8) - k % 8 - 1;

						b |= (BYTE)1 << bit_pos;
				}
		}
		

		for(size_t i = 0; i < vb.size(); ++i)
		{
				wchar_t *p;
				BYTE b = vb[i];
				wchar_t buf[4];
				p = buf + 4;
				*--p = L'\0';

				*--p = tbl[b%16];
				b /= 16;
				*--p = tbl[b%16];
				
				ret += p;
				
		}


		return ret;
}


static inline bool __hex_string_to_bitset(const std::wstring &hex_str, size_t bits, std::vector<bool> &vbits)
{
		const wchar_t *p = NULL;
		
		AR_ASSERT(hex_str.size() > 0 && hex_str.size() % 2 == 0 && bits > 0);

		size_t current_bits = 0;
		size_t hex_str_bits = hex_str.size() / 2 * 8;
		
		AR_ASSERT(hex_str_bits - bits < 8);

		p = hex_str.c_str();

		vbits.resize(bits);
		std::fill(vbits.begin(), vbits.end(), false);

		while(*p != L'\0')
		{
				BYTE b = 0;

				if(*p >= L'0' && *p <= L'9')
				{
						b += (*p - L'0');
				}else if(*p >= L'a' && *p <= L'f')
				{
						b += (*p - L'a' + 10);

				}else if(*p >= L'A' && *p <= L'F')
				{
						b += (*p - L'A' + 10);

				}else
				{
						AR_ASSERT(false);
						return false;
				}

				b *= 16;
				++p;

				if(*p == L'\0')
				{
                        AR_error(AR_ERR_WARNING, L"malformat input : %ls!\r\n", hex_str.c_str());
						return false;
				}

				if(*p >= L'0' && *p <= L'9')
				{
						b += (*p - L'0');
				}else if(*p >= L'a' && *p <= L'f')
				{
						b += (*p - L'a' + 10);

				}else if(*p >= L'A' && *p <= L'F')
				{
						b += (*p - L'A' + 10);

				}else
				{
						AR_ASSERT(false);
						return false;
				}

				++p;

				if(current_bits + 8 <= bits)
				{
						for(int i = 7; i >= 0; --i)
						{
								vbits[current_bits] = b & (1 << i) ? true : false;
								current_bits++;
						}

				}else
				{
						for(int i = 8 - (hex_str_bits - bits) - 1; i >= 0; --i)
						{
								vbits[current_bits] = b & (1 << i) ? true : false;
								current_bits++;
						}
						break;
				}
		}

		return true;

}



arStatus_t phash_image(CByteImage_t &src, std::wstring &hash)
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
				}else 
				{
						filtered_img = src.channel(0).get_convolve(meanfilter);
				}

				filtered_img.resize(SCALED_IMAGE_SIZE,SCALED_IMAGE_SIZE);

				


				dct_matrix = AR_CreateMatrix(SCALED_IMAGE_SIZE,SCALED_IMAGE_SIZE);

				if(dct_matrix == NULL)
				{
						AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
						status = AR_E_NOMEM;
						goto END_POINT;
				}
				

				status = AR_GenerateTransformMatrix_DCT2(dct_matrix, SCALED_IMAGE_SIZE);
				if(status != AR_S_YES)
				{
						AR_error(AR_ERR_WARNING, L"failed to generate dct2 transform matrix : %hs\r\n", AR_FUNC_NAME);
						goto END_POINT;
				}
				

				img_mat = AR_CreateMatrix(SCALED_IMAGE_SIZE, SCALED_IMAGE_SIZE);
				
				if(img_mat == NULL)
				{
						AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
						status = AR_E_NOMEM;
						goto END_POINT;
				}


				for(size_t x = 0; x < SCALED_IMAGE_SIZE; ++x)
				{
						for(size_t y = 0; y < SCALED_IMAGE_SIZE; ++y)
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



				std::vector<double> result(HASH_SAMPLE_SIZE * HASH_SAMPLE_SIZE);

				for(size_t x = 0; x < HASH_SAMPLE_SIZE; ++x)
				{
						for(size_t y = 0; y < HASH_SAMPLE_SIZE; ++y)
						{
								result[x * HASH_SAMPLE_SIZE + y] = AR_GetMatrixValue(img_mat, x,y);
						}
				}


				double mid = get_nth_elem(result, result.size() / 2);


				hash = L"";
				for (int i=0; i < HASH_SAMPLE_SIZE * HASH_SAMPLE_SIZE;i++)
				{
						double val = result[i];

						if(val > mid)
						{
								hash += L"1";
						}else
						{
								hash += L"0";
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


ar_bool_t phash_image_file(const wchar_t *file, std::wstring &hash)
{

		const wchar_t *p = AR_reverse_wcschr(file, AR_wcslen(file), L'.');

		if(p == NULL || AR_wcsicmp(p, L".bmp") != 0)
		{
				return false;
		}

		CByteImage_t src;
		char *s = AR_wcs_to_str(AR_CP_UTF8, file, AR_wcslen(file));
		if(s == NULL)
		{
				return false;
		}

		try{
				src.load(s);
				if(phash_image(src, hash) != AR_S_YES)
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





static inline BOOL __convert_image_ycrcb(CImage &img)
{
    BOOL ret;
    int x,y;
    
    ret = TRUE;
    
	for(x = 0; x < img.GetWidth(); ++x)
    {
        for(y = 0; y < img.GetHeight(); ++y)
        {
            BYTE *pix_addr = (BYTE*)img.GetPixelAddress(x,y);
            AR_ASSERT(pix_addr != NULL);

            double R,G,B;

            R = (double)pix_addr[2];
            G = (double)pix_addr[1];
            B = (double)pix_addr[0];

            double Y =   (R * 66  + G * 129  + B * 25 + 128) / 256.0 + 16.0;
            double Cb =  (-R * 38 - G * 74 + B * 112 + 128) / 256.0 + 128.0;
            double Cr =  (R * 112 - G * 94 - B * 18 + 128) / 256.0 + 128.0;

            Y = Y < 0 ? 0 : ( Y > 255 ? 255 : Y);
            Cb = Cb < 0 ? 0 : ( Cb > 255 ? 255 : Cb);
            Cr = Cr < 0 ? 0 : ( Cr > 255 ? 255 : Cr);

			pix_addr[2] = (ar_byte_t)Y;
            pix_addr[1] = (ar_byte_t)Cb;
            pix_addr[0] = (ar_byte_t)Cr;
        }
    }
END_POINT:
    return ret;
}



static BOOL __build_Ychannel_matrix(CImage &scaled_img, arMatrix_t *img_mat)
{
		BOOL ret;
		int x,y;
		AR_ASSERT(img_mat != NULL);
		AR_ASSERT(scaled_img.GetWidth() == SCALED_IMAGE_SIZE && scaled_img.GetHeight() == SCALED_IMAGE_SIZE);
		ret = TRUE;

		if(AR_SetMatrixSize(img_mat, SCALED_IMAGE_SIZE, SCALED_IMAGE_SIZE) != AR_S_YES)
		{
				AR_printf(L"low mem : %hs\r\n", AR_FUNC_NAME);
				ret = FALSE;
				goto END_POINT;
		}

		for(x = 0; x < scaled_img.GetWidth(); ++x)
		{
				for(y = 0; y < scaled_img.GetHeight(); ++y)
				{

						const BYTE *pix_addr = (const BYTE*)scaled_img.GetPixelAddress(x,y);
						AR_ASSERT(pix_addr != NULL);

						double Y, Cb, Cr;

						Y = (double)pix_addr[2];
						Cb = (double)pix_addr[1];
						Cr = (double)pix_addr[0];

						AR_SetMatrixValue(img_mat, (size_t)x, (size_t)y, Y);
				}
		}

END_POINT:
		return ret;
}



static BOOL HashImage_DCT(CScreenImage &img, std::wstring &hex_str)
{
		BOOL ret;
		arMatrix_t *img_mat = NULL;
		arMatrix_t *dct_matrix = NULL;
		CImage scaled_img;
		ret = TRUE;
		std::vector<bool> hash;
		std::vector<double> pca_data;
		
		hex_str = L"";

		if(!__convert_image_ycrcb(img))
		{
				ret = FALSE;
				AR_error(AR_ERR_WARNING, L"convert to YCbCr image failed : %hs\r\n", AR_FUNC_NAME);
				goto END_POINT;
		}


		ret = img.ScaleTo(scaled_img, SCALED_IMAGE_SIZE, SCALED_IMAGE_SIZE);
		if(!ret)
		{
				ret = FALSE;
				AR_error(AR_ERR_WARNING, L"failed to scale YCbCr image : %hs\r\n", AR_FUNC_NAME);
				goto END_POINT;
		}


		img_mat = AR_CreateMatrix(SCALED_IMAGE_SIZE, SCALED_IMAGE_SIZE);

		if(img_mat == NULL)
		{
				ret = FALSE;
				AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
				goto END_POINT;
		}

		ret = __build_Ychannel_matrix(scaled_img, img_mat);

		if(!ret)
		{
				AR_error(AR_ERR_WARNING, L"failed to convert RGB to YCbCr : %hs\r\n", AR_FUNC_NAME);
				goto END_POINT;
		}



		if(dct_matrix == NULL)
		{
				dct_matrix = AR_CreateMatrix(SCALED_IMAGE_SIZE, SCALED_IMAGE_SIZE);

				if(dct_matrix == NULL)
				{
						AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
						ret = FALSE;
						goto END_POINT;
				}

				AR_GenerateTransformMatrix_DCT2(dct_matrix, SCALED_IMAGE_SIZE);
		}


		if(AR_MatrixTransform_DCT2(img_mat, dct_matrix) != AR_S_YES)
		{
				ret = FALSE;
				AR_error(AR_ERR_WARNING, L"transform matrix to dct2 form failed : %hs\r\n", AR_FUNC_NAME);
				goto END_POINT;
		}

		pca_data.resize(HASH_SAMPLE_SIZE * HASH_SAMPLE_SIZE);
		hash.resize(HASH_SAMPLE_SIZE * HASH_SAMPLE_SIZE, false);

		for(size_t x = 0; x < HASH_SAMPLE_SIZE; ++x)
		{
				for(size_t y = 0; y < HASH_SAMPLE_SIZE; ++y)
				{
						pca_data[x * HASH_SAMPLE_SIZE + y] = AR_GetMatrixValue(img_mat, x,y);
				}
		}

		hex_str = L"";

		double mid = get_nth_elem(pca_data, pca_data.size() / 2);

		for(size_t i = 0; i < pca_data.size(); ++i)
		{
				double v = pca_data[i];
				if(v > mid)
				{
						hash[i] = true;
						//hex_str += L"1";
				}else
				{
						hash[i] = false;
						//hex_str += L"0";
				}
		}

		hex_str = __bitset_to_hex_string(hash);

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

		return ret;
}

BOOL HashImageFile_DCT(const std::wstring &path, std::wstring &hex_str)
{
		CScreenImage img;
		BOOL ret = img.Load(path.c_str()) == S_OK;

		if(!ret)
		{
				return ret;
		}

		ret = HashImage_DCT(img, hex_str);

		return ret;
}


size_t phash_hamming_distance(const std::wstring &l, const std::wstring &r)
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


size_t phash_hamming_distance_vb(const std::vector<bool> &l, const std::vector<bool> &r)
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


typedef std::map<std::wstring, std::wstring>	fhMap_t;




static void GetSampleFiles(const std::wstring &path, fhMap_t &samples)
{

		std::wstring samples_path = path;
		std::vector<std::wstring> samples_file_list;

		AR_error(AR_ERR_MESSAGE, L"samples path : %ls\r\n", samples_path.c_str());
		
		arPathIter_t *iter = 	AR_CreatePathIterator();

		if(samples_path.back() == L'\\')
		{
				samples_path.pop_back();
		}

		if(AR_PathIteratorSetPath(iter, samples_path.c_str()) != AR_S_YES)
		{
				goto END_POINT;
		}

		

		while(!AR_PathIteratorIsDone(iter))
		{
				const wchar_t *name = AR_PathIteratorCurrent(iter);
				
				//AR_DPRINT(L"file name : %ls\r\n", name);

				const wchar_t *p = AR_reverse_wcschr(name, AR_wcslen(name), L'.');

				if(p != NULL && (AR_wcsicmp(p, L".bmp") == 0 || AR_wcsicmp(p, L".png") == 0 || AR_wcsicmp(p, L".jpg") == 0 || AR_wcsicmp(p, L".gif") == 0))
				{
						std::wstring file_path = samples_path + L"\\" + std::wstring(name);
						samples_file_list.push_back(file_path);
				}else
				{
						int x = 3;
						x++;
				}


				if(AR_PathIteratorNext(iter) != AR_S_YES)
				{
						break;
				}
		}


		for(size_t i = 0; i < samples_file_list.size(); ++i)
		{
				const std::wstring &sample_path = samples_file_list[i];
				const wchar_t *sample_name = AR_reverse_wcschr(sample_path.c_str(), sample_path.size(), L'\\');

				if(sample_name == NULL || AR_wcslen(sample_name) == 0)
				{
						AR_error(AR_ERR_WARNING, L"invalid sample file path : %ls\r\n", sample_path.c_str());
						continue;
				}

				sample_name++;

				std::wstring hash;

				
#if(1)
				if(!HashImageFile_DCT(sample_path, hash))
				{
						AR_error(AR_ERR_WARNING, L"failed to phash image file : %ls\r\n", sample_path.c_str());
						continue;
				}
				
#else

				if(!phash_image_file(sample_path.c_str(), hash))
				{
						AR_error(AR_ERR_WARNING, L"failed to phash image file : %ls\r\n", sample_path.c_str());
						continue;
				}
#endif

				AR_DPRINT(L"%ls -> %ls\r\n", sample_name, hash.c_str());

				samples[sample_name] = hash;
		}

END_POINT:
		if(iter)
		{
				AR_DestroyPathIterator(iter);
				iter = NULL;
		}

}








void hash_test0()
{
		std::wstring hash;
		phash_image_file(L"C:\\Users\\solidus\\Desktop\\New folder\\qd_org.bmp",hash);
		AR_printf(L"%ls\r\n", hash.c_str());
}




void hash_test1()
{
		std::wstring hash;
		
		CScreenImage img;
		//img.Load(L"C:\\Users\\solidus\\Desktop\\New folder\\qd_org.bmp");
		img.Load(L"C:\\Users\\solidus\\Desktop\\New folder\\1.gif");
		BOOL ret = HashImage_DCT(img, hash);

		AR_printf(L"%ls\r\n", hash.c_str());
}


void hash_test2()
{
		std::wstring hash1;
		phash_image_file(L"C:\\Users\\solidus\\Desktop\\New folder\\qd_org.bmp",hash1);
		AR_printf(L"%ls\r\n", hash1.c_str());

		std::wstring hash2;
		
		CScreenImage img;
		img.Load(L"C:\\Users\\solidus\\Desktop\\New folder\\qd_org.bmp");

		BOOL ret = HashImage_DCT(img, hash2);

		AR_printf(L"%ls\r\n", hash2.c_str());

		AR_ASSERT(hash1.size() == hash2.size());

		size_t diff_count = 0;
		for(size_t i = 0; i < hash2.size(); ++i)
		{

				if(hash1[i] != hash2[i])
				{
						diff_count++;
				}

		}

		AR_printf(L"hash distance : %qu\r\n", diff_count);
}




void hash_test3()
{
		AR_printf(L"CImage:\r\n");

		std::wstring hash1, hash2, hash3, hash4, hash5;
		BOOL ret;



		ret = HashImageFile_DCT(L"C:\\Users\\solidus\\Desktop\\New folder\\worm.bmp", hash1);
		AR_ASSERT(ret);
		AR_printf(L"%ls\r\n", hash1.c_str());

		ret = HashImageFile_DCT(L"C:\\Users\\solidus\\Desktop\\New folder\\worm2.bmp", hash2);
		AR_ASSERT(ret);
		AR_printf(L"%ls\r\n", hash2.c_str());
		

		ret = HashImageFile_DCT(L"C:\\Users\\solidus\\Desktop\\New folder\\worm3.bmp", hash3);
		AR_ASSERT(ret);
		AR_printf(L"%ls\r\n", hash3.c_str());


		ret = HashImageFile_DCT(L"C:\\Users\\solidus\\Desktop\\New folder\\worm4.bmp", hash4);
		AR_ASSERT(ret);
		AR_printf(L"%ls\r\n", hash4.c_str());
		

		ret = HashImageFile_DCT(L"C:\\Users\\solidus\\Desktop\\New folder\\worm5.bmp", hash5);
		AR_ASSERT(ret);
		AR_printf(L"%ls\r\n", hash5.c_str());
		
		
		size_t diff_count;
		
		diff_count = phash_hamming_distance(hash1,hash2);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		diff_count = phash_hamming_distance(hash1,hash3);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		diff_count = phash_hamming_distance(hash1,hash4);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		diff_count = phash_hamming_distance(hash1,hash5);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		AR_printf(L"---------------------------------------\r\n");

}





void hash_test4()
{
		AR_printf(L"CImg.h:\r\n");
		std::wstring hash1, hash2, hash3, hash4, hash5;

		phash_image_file(L"C:\\Users\\solidus\\Desktop\\New folder\\worm.bmp",hash1);
		AR_printf(L"%ls\r\n", hash1.c_str());

		phash_image_file(L"C:\\Users\\solidus\\Desktop\\New folder\\worm2.bmp",hash2);
		AR_printf(L"%ls\r\n", hash2.c_str());

		phash_image_file(L"C:\\Users\\solidus\\Desktop\\New folder\\worm3.bmp",hash3);
		AR_printf(L"%ls\r\n", hash3.c_str());

		phash_image_file(L"C:\\Users\\solidus\\Desktop\\New folder\\worm4.bmp",hash4);
		AR_printf(L"%ls\r\n", hash4.c_str());

		phash_image_file(L"C:\\Users\\solidus\\Desktop\\New folder\\worm5.bmp",hash5);
		AR_printf(L"%ls\r\n", hash5.c_str());
		


		size_t diff_count;

		
		diff_count = phash_hamming_distance(hash1,hash2);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		diff_count = phash_hamming_distance(hash1,hash3);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		diff_count = phash_hamming_distance(hash1,hash4);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		diff_count = phash_hamming_distance(hash1,hash5);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		AR_printf(L"---------------------------------------\r\n");


}




void hash_test5()
{
		std::wstring hash1, hash2, hash3, hash4, hash5;

		hash1 = L"1001010000111110001010000111111101100011110001110101010111001010111100011100000011001111100000001110101111100000010101110010001011000011011101010001011000110110010100100111000001000011110000110110000111000011001111000001110101111100101010100010110101011111";
		hash2 = L"1000000000001110001000001111001101100001111000010100011110001100010001111000110001001011100000000110101101110000010111100011101000100011011101110101011100110110010001110011001001111110010101010011110011011101001111011100111101100001111010100010111101010101";
		hash3 = L"1000000000011111001010000111111100101001111110000101100111011000111000011100000011101111100000001110001101000100010101110010011011000011011001010001111000001111010100001110000000111100001111010111100111110001001111100001111101110101101010100010100111010101";
		hash4 = L"1000000000011110001000100011111101100001111100010101110001100010010011100110001111101011111000011100101111000001010100011000101000001011110111010000101110001101010000011110101001101011110001010011111000001110001110111101010001110101101010100010111001110101";
		hash5 = L"1000000000011110001000001111001101100001111000010100010110001100010011111000110001001011100000000100101111110000010111100011101000001011011101010000111100110110010101111010101001111110010100010011110001010111001011110111011101100001111010100010111101011101";


		size_t diff_count;

		diff_count = phash_hamming_distance(hash1,hash2);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		diff_count = phash_hamming_distance(hash1,hash3);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		diff_count = phash_hamming_distance(hash1,hash4);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		diff_count = phash_hamming_distance(hash1,hash5);
		AR_printf(L"hash distance : %Iu\r\n", diff_count);

		AR_printf(L"---------------------------------------\r\n");
}



static void hash_test6()
{

		while(true)
		{
				AR_printf(L"---------------------------------------------------------------------------------\r\n----------------------------------------------------\r\n");
				std::wstring path = L"";
				fhMap_t m;
				std::vector<std::wstring> hash_set;
				BOOL ret = FALSE;

				GetSampleFiles(L"C:\\Users\\solidus\\Desktop\\New folder\\", m);

				std::map<int, std::wstring> name_map;
				size_t i = 0;
				for(fhMap_t::iterator it = m.begin(); it != m.end(); ++it)
				{
						name_map[i++] = it->first;
						hash_set.push_back(it->second);
						//AR_printf(L"index %d name : %ls\r\n", (int)i++, it->first.c_str());
				}


				struct  record_t
				{
						std::wstring	l,r;
						std::wstring	lh,rh;
						size_t			diff;

						bool operator<(const record_t &other)
						{
								if(this == &other)
								{
										return true;
								}

								return other.diff < this->diff;
						}
				};

				std::vector<record_t>	rec_vec;

				for(size_t i = 0; i < hash_set.size(); ++i)
				{
						std::wstring l = hash_set[i];

						
						std::vector<bool>		lvb;

						bool is_ok = __hex_string_to_bitset(l, HASH_SAMPLE_SIZE * HASH_SAMPLE_SIZE, lvb);
						AR_ASSERT(is_ok);

						for(size_t k = i + 1; k < hash_set.size(); ++k)
						{
								std::vector<bool>		rvb;
								is_ok = __hex_string_to_bitset(hash_set[k], HASH_SAMPLE_SIZE * HASH_SAMPLE_SIZE, rvb);
								AR_ASSERT(is_ok);

								//size_t diff = phash_hamming_distance(l, hash_set[k]);
								
								size_t diff = phash_hamming_distance_vb(lvb, rvb);

								record_t rec;

								rec.l = name_map[i];
								rec.r = name_map[k];

								rec.lh = l;
								rec.rh = hash_set[k];
								rec.diff = diff;
								rec_vec.push_back(rec);
								//AR_printf(L"hash distance %ls vs %ls: %Iu\r\n", name_map[i].c_str(), name_map[k].c_str(), diff);
						}
				}


				std::sort(rec_vec.begin(), rec_vec.end());

				for(size_t i = 0; i < rec_vec.size(); ++i)
				{
						const record_t &rec = rec_vec[i];

						if((AR_wcsistr(rec.l.c_str(), L"worm") != NULL || AR_wcsistr(rec.r.c_str(), L"worm") != NULL) && rec.diff < (HASH_SAMPLE_SIZE * HASH_SAMPLE_SIZE))
						{
								if(AR_wcsistr(rec.l.c_str(), L"worm12") != NULL || AR_wcsistr(rec.r.c_str(), L"worm12") != NULL)
								{
										AR_printf(L"hash distance %ls vs %ls: %Iu\r\n", rec.l.c_str(), rec.r.c_str(), rec.diff);
								}
						}

				}

				::MessageBoxW(NULL, L"Done", 0, 0);
				getchar();
		}

}



CImg<float>* GetMHKernel(float alpha, float level)
{
		int sigma = (int)4*pow((float)alpha,(float)level);
		static CImg<float> *pkernel = NULL;
		float xpos, ypos, A;
		if (!pkernel)
		{
				pkernel = new CImg<float>(2*sigma+1,2*sigma+1,1,1,0);
				cimg_forXY(*pkernel,X,Y){
						xpos = pow(alpha,-level)*(X-sigma);
						ypos = pow(alpha,-level)*(Y-sigma);
						A = xpos*xpos + ypos*ypos;
						pkernel->atXY(X,Y) = (2-A)*exp(-A/2);
				}
		}
		return pkernel;
}

unsigned __int8* ph_mh_imagehash(const char *filename, int &N,float alpha, float lvl, std::wstring &hash_str){
		if (filename == NULL){
				return NULL;
		}
		unsigned __int8 *hash = (unsigned char*)malloc(72*sizeof(unsigned __int8));
		N = 72;
		
		CImg<unsigned __int8> src(filename);
		CImg<unsigned __int8> img;

		if (src.spectrum() == 3)
		{
				img = src.get_RGBtoYCbCr().channel(0)/*.blur(1.0)*/.resize(512,512,1,1,5).get_equalize(256);
		} else{
				img = src.channel(0)/*.get_blur(1.0)*/.resize(512,512,1,1,5).get_equalize(256);
		}
		src.clear();

		CImg<float> *pkernel = GetMHKernel(alpha,lvl);
		CImg<float> fresp =  img.get_correlate(*pkernel);
		img.clear();
		fresp.normalize(0,1.0);
		CImg<float> blocks(31,31,1,1,0);
		for (int rindex=0;rindex < 31;rindex++){
				for (int cindex=0;cindex < 31;cindex++)
				{
						blocks(rindex,cindex) = fresp.get_crop(rindex*16,cindex*16,rindex*16+16-1,cindex*16+16-1).sum();
				}
		}


		int hash_index;
		int nb_ones = 0, nb_zeros = 0;
		int bit_index = 0;
		unsigned char hashbyte = 0;
		for (int rindex=0;rindex < 31-2;rindex+=4)
		{
				CImg<float> subsec;
				for (int cindex=0;cindex < 31-2;cindex+=4)
				{
						subsec = blocks.get_crop(cindex,rindex, cindex+2, rindex+2).unroll('x');
						float ave = subsec.mean();
						cimg_forX(subsec, I)
						{
								hashbyte <<= 1;
								if (subsec(I) > ave){
										hashbyte |= 0x01;
										nb_ones++;
										hash_str += L"1";
								} else {
										nb_zeros++;
										hash_str += L"0";
								}
								bit_index++;
								if ((bit_index%8) == 0){
										hash_index = (int)(bit_index/8) - 1; 
										hash[hash_index] = hashbyte;
										hashbyte = 0x00;
								}
						}
				}
		}

		return hash;
}


int ph_bitcount8(unsigned __int8 val)
{
		int num = 0;
		while (val)
		{
				++num;
				val &= val - 1;
		}
		return num;
}




static void hash_test7()
{
		int l,r;
		std::wstring hash1, hash2;

		unsigned __int8 *h1 = ph_mh_imagehash("C:\\Users\\solidus\\Desktop\\New folder\\attacked1.bmp", l, 1, 1.0, hash1);

		unsigned __int8 *h2 = ph_mh_imagehash("C:\\Users\\solidus\\Desktop\\New folder\\worm10.bmp", r, 1, 1.0, hash2);

		size_t diff = phash_hamming_distance(hash1, hash2);


}

void phash_test()
{
		//hash_test5();

		//hash_test0();
		//hash_test1();

		//hash_test2();
		
		//hash_test3();
		
		//hash_test4();

		hash_test6();

		//hash_test7();

		getchar();
}




AR_NAMESPACE_END



#endif

