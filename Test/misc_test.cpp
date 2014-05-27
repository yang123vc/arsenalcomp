#include "test.h"
#include <vector>
#include <string>

#if defined(__LIB)

AR_NAMESPACE_BEGIN



std::wstring bitset_to_hex_string(const std::vector<bool> &vbits)
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



bool hex_string_to_bitset(const std::wstring &hex_str, size_t bits, std::vector<bool> &vbits)
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
						AR_error(AR_ERR_WARNING, L"invalid input format!\r\n");
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


static void vbits_test1()
{
		AR_srand(time(NULL));
		for(size_t i = 0; i < 100000000; ++i)
		{
				size_t bits = AR_rand32() % 8;
				if(bits == 0)
				{
						bits = 123;
				}

				std::vector<bool>		src(bits, 0);

				for(size_t i = 0; i < src.size(); ++i)
				{
						src[i] = AR_rand32() % 2;
				}

				for(size_t i = 0; i < src.size(); ++i)
				{
						AR_printf(L"%d ", src[i] ? 1 : 0);
				}

				AR_printf(L"\r\n");

				std::wstring hex_str = bitset_to_hex_string(src);

				AR_printf(L"%ls\r\n", hex_str.c_str());

				std::vector<bool> dest;

				hex_string_to_bitset(hex_str, bits, dest);

				std::wstring dest_hex_str = bitset_to_hex_string(dest);

				AR_ASSERT(dest_hex_str == hex_str);
				AR_ASSERT(src == dest);


		}
}



static void vbits_debug_test1()
{
		bool arr[15] = {true,true,false,false,true,true,true,true,false,true,false,false,false,true,true};

		std::vector<bool>		src(AR_NELEMS(arr), 0);

		for(size_t i = 0; i < AR_NELEMS(arr); ++i)
		{
				src[i] = arr[i];
		}
		
		for(size_t i = 0; i < src.size(); ++i)
		{
				AR_printf(L"%d ", src[i] ? 1 : 0);
		}

		AR_printf(L"\r\n");

		std::wstring hex_str = bitset_to_hex_string(src);
		AR_printf(L"%ls\r\n", hex_str.c_str());

		std::vector<bool> dest;

		hex_string_to_bitset(hex_str, src.size(), dest);


		std::wstring dest_hex_str = bitset_to_hex_string(dest);

		AR_ASSERT(src == dest);

}

void misc_test()
{
		vbits_test1();
		//vbits_debug_test1();
}



AR_NAMESPACE_END


#endif
