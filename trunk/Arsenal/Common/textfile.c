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

#if defined(OS_FAMILY_UNIX)
		#include <iconv.h>
#endif

#include "common.h"

/******************************************************TextFile**************************************************************/

AR_NAMESPACE_BEGIN


/***********************************************************Misc**********************************************************/

#if defined(OS_FAMILY_WINDOWS)

static FILE*	__AR_open_file(const wchar_t *path, const wchar_t *mode)
{
		AR_ASSERT(path != NULL && mode != NULL);
		return _wfopen(path, mode);
}


#elif defined(OS_FAMILY_UNIX)




FILE*	__AR_open_file(const wchar_t *path, const wchar_t *mode)
{
        FILE    *file;
        char    *str_path;
        char     *str_mode;
		AR_ASSERT(path != NULL && mode != NULL);

		str_path = AR_wcs_convto_str(AR_CP_UTF8, path, AR_wcslen(path));
		str_mode = AR_wcs_convto_str(AR_CP_UTF8, mode, AR_wcslen(mode));

		file = NULL;

		if(str_path && str_mode)
		{
            file = fopen(str_path, str_mode);
		}

        if(str_path)
        {
            AR_DEL(str_path);
            str_path = NULL;
        }

        if(str_mode)
        {
            AR_DEL(str_mode);
            str_mode = NULL;
        }

        return file;
}

#else

#error Unknow platform

#endif



/***************************************Read File**********************************************************/
#if(0)
static bool_t	__dectect_encoding(FILE *file, arTxtBom_t *bom)
{
		byte_t b[2];

		AR_ASSERT(file != NULL && bom != NULL);

		if(fread((void*)b, 1, 2, file) != 2)
		{
				return false;
		}else
		{
				if(b[0] == 0xFF && b[1] == 0xFE)
				{
						byte_t tmp[2] = {0xcc,0xcc};
						fread((void*)tmp, 1, 2, file);

						if(tmp[0] == 0x00 && tmp[1] == 0x00)
						{
								*bom = AR_TXT_BOM_UTF32_LE;
						}else
						{
								*bom = AR_TXT_BOM_UTF16_LE;
								fseek(file, 2, 0);
						}
				}else if(b[0] == 0xFE && b[1] == 0xFF)
				{
						*bom = AR_TXT_BOM_UTF16_BE;
				}else if(b[0] == 0x00 && b[1] == 0x00)
				{
						byte_t tmp[2] = {0xcc,0xcc};
						fread((void*)tmp, 1, 2, file);

						if(tmp[0] == 0xFE && tmp[1] == 0xFF)
						{
								*bom = AR_TXT_BOM_UTF32_BE;
						}else
						{
								fseek(file, 0, 0);
								*bom = AR_TXT_BOM_ASCII;
						}
				}else if(b[0] == 0xEF && b[1] == 0xBB)
				{
						byte_t	tmp = 0;
						fread((void*)&tmp, 1, 1, file);

						if( tmp == 0xBF)
						{
								*bom = AR_TXT_BOM_UTF_8;
						}else
						{
								*bom = AR_TXT_BOM_ASCII;
								fseek(file, 0,0);
						}
				}
				else
				{
						*bom = AR_TXT_BOM_ASCII;
						fseek(file, 0,0);
				}
		}

		return true;
}





typedef enum
{
		TXT_READ_OK = 0x00,
		TXT_READ_INVALID,
		TXT_READ_EOF
}txtReadStatus_t;

static txtReadStatus_t		__read_wchar(FILE *file, arTxtBom_t enc, wchar_t *out)
{

		uint_32_t e;


		AR_ASSERT(file != NULL);

		e = 0;

		switch(enc)
		{
		case AR_TXT_BOM_UTF_8:
		{
				byte_t		b;
				byte_t		buf[5];
				size_t		rn;
				uint_32_t	v;

				b = 0;
				rn = 0;
				v = 0;

				rn = fread((void*)&b, 1, 1, file);

				if(rn != 1)
				{
						return feof(file) ? TXT_READ_EOF : TXT_READ_INVALID;
				}

				v = (uint_32_t)b;

				if(v >= 0xfc)
				{
						/*6:<11111100>*/
						/*读剩下的字节*/
						rn = fread((void*)buf, 1, 5, file);
						if(rn != 5)
						{
								return TXT_READ_INVALID;
						}

						e = (v & 0x01) << 30;
						e |= (buf[0] & 0x3f) << 24;
						e |= (buf[1] & 0x3f) << 18;
						e |= (buf[2] & 0x3f) << 12;
						e |= (buf[3] & 0x3f) << 6;
						e |= (buf[4] & 0x3f);
				}else if(v >= 0xf8)
				{
						/*5:<11111000>*/
						rn = fread((void*)buf, 1,4, file);
						if(rn != 4)
						{
								return TXT_READ_INVALID;
						}

						e = (v & 0x03) << 24;
						e |= (buf[0] & 0x3f) << 18;
						e |= (buf[1] & 0x3f) << 12;
						e |= (buf[2] & 0x3f) << 6;
						e |= (buf[3] & 0x3f);

				}else if(v >= 0xf0)
				{
						/*4:<11110000>*/
						rn = fread((void*)buf, 1, 3, file);
						if(rn != 3)
						{
								return TXT_READ_INVALID;
						}
						e = (v & 0x07) << 18;
						e |= (buf[0] & 0x3f) << 12;
						e |= (buf[1] & 0x3f) << 6;
						e |= (buf[2] & 0x3f);

				}else if(v >= 0xe0)
				{
						/*3:<11100000>*/
						rn = fread((void*)buf, 1, 2, file);
						if(rn != 2)
						{
								return TXT_READ_INVALID;
						}

						e = (v & 0x0f) << 12;
						e |= (buf[0] & 0x3f) << 6;
						e |= (buf[1] & 0x3f);

				}else if(v >= 0xc0)
				{
						/*3:<11000000>*/
						rn = fread((void*)buf, 1,1,file);
						if(rn != 1)
						{
								return TXT_READ_INVALID;
						}
						e = (v & 0x1f) << 6;
						e |= (buf[0] & 0x3f);
				}else
				{
						e = (wchar_t)v;
				}
		}
				break;
		case AR_TXT_BOM_UTF16_BE:
		case AR_TXT_BOM_UTF16_LE:
		{
				byte_t buf[2];
				size_t	rn;
				rn = fread((void*)buf, 1, 2, file);
				if(rn != 2)
				{
						return feof(file) ? TXT_READ_EOF : TXT_READ_INVALID;
				}

				if(enc == AR_TXT_BOM_UTF16_BE)
				{
						e = ((uint_32_t)buf[0]) << 8 | (uint_32_t)buf[1];
				}else
				{
						e = ((uint_32_t)buf[1]) << 8 | (uint_32_t)buf[0];
				}
		}
				break;
		case AR_TXT_BOM_UTF32_BE:
		case AR_TXT_BOM_UTF32_LE:
		{
				byte_t buf[4];
				size_t	rn;
				rn = fread((void*)buf, 1, 4, file);
				if(rn != 4)
				{
						return feof(file) ? TXT_READ_EOF : TXT_READ_INVALID;
				}

				if(enc == AR_TXT_BOM_UTF32_BE)
				{
						e = ((uint_32_t)buf[0]) << 24 | (uint_32_t)buf[1] << 16 | (uint_32_t)buf[2] << 8 | (uint_32_t)buf[3];
				}else
				{
						e = (uint_32_t)buf[0] | (uint_32_t)buf[1] << 8 | (uint_32_t)buf[2] << 16 | ((uint_32_t)buf[3]) << 24;
				}
		}
				break;
		case AR_TXT_BOM_ASCII:
		default:
				return TXT_READ_INVALID;
				break;
		}

		if(out)*out = (wchar_t)e;
		return TXT_READ_OK;
}



bool_t	AR_LoadBomTextFile(const wchar_t *path, arTxtBom_t *bom, arString_t *out)
{

		FILE	*file = NULL;
		arTxtBom_t	enc;
		wchar_t c;
		bool_t	is_ok;
		txtReadStatus_t	status;
		AR_ASSERT(path != NULL && out != NULL);

		is_ok = true;
		AR_ClearString(out);

/***************************************************/
		file = __AR_open_file(path, L"rb");
/***************************************************/

		if(!file)
		{
				is_ok = false;
				goto FAILED_POINT;
		}


		if(!__dectect_encoding(file, &enc))
		{
				goto FAILED_POINT;
		}

		if(enc == AR_TXT_BOM_ASCII)
		{
				byte_t	tmp[1024];
				size_t	rn;
				arBuffer_t *ascii_buf = AR_CreateBuffer(1024);

				do{
						rn = fread((void*)tmp, 1, 1024, file);
						if(rn > 0)
						{
								AR_InsertBuffer(ascii_buf, tmp, rn);
						}
				}while(!feof(file) && !ferror(file));

				if(ferror(file))
				{
						status = TXT_READ_INVALID;
				}else
				{
						tmp[0] = '\0';
						AR_InsertBuffer(ascii_buf, tmp, 1);
						status = TXT_READ_EOF;
				}

				if(status != TXT_READ_INVALID)
				{
						wchar_t *str;
						
						size_t cp;
						for(cp = AR_CP_ACP; cp < AR_CP_MAX; ++cp)
						{

								str = AR_str_convto_wcs((arCodePage_t)cp, (const char*)AR_GetBufferData(ascii_buf), AR_GetBufferAvailable(ascii_buf));
								if(str != NULL)
								{
										break;
								}

						}
						
						if(!str)
						{
								status = TXT_READ_INVALID;
						}

						if(out && str)
						{
								AR_AppendString(out, str);
						}

						if(str)
						{
								AR_DEL(str);
								str = NULL;
						}

						if(ascii_buf)
						{
								AR_DestroyBuffer(ascii_buf);
						}
						ascii_buf = NULL;
				}
		}else
		{
				do{

						status = __read_wchar(file, enc, &c);

						if(status == TXT_READ_OK && out)
						{
								AR_AppendCharToString(out, c);
						}
				}while(status == TXT_READ_OK);
		}

		if(status == TXT_READ_INVALID)
		{
				is_ok = false;
				goto FAILED_POINT;
		}

		if(bom)
		{
				*bom = enc;
		}

FAILED_POINT:
		if(file)
		{
				fclose(file);
				file = NULL;
		}

		return is_ok;
}

#endif










/*返回消耗掉的数据长度，<0则错误*/

static bool_t	__dectect_encoding(arBuffer_t *input, arTxtBom_t *bom)
{
		byte_t tmp[4] = {0xcc, 0xcc,0xcc,0xcc};
		size_t input_len;
		size_t read_n;
		AR_ASSERT(input != NULL && bom != NULL);
		input_len = AR_GetBufferAvailable(input);

		
		read_n = 0;
		AR_memcpy(tmp, AR_GetBufferData(input), AR_MIN(input_len, 4));


		if(tmp[0] == 0xFF && tmp[1] == 0xFE)
		{
				if(tmp[2] == 0x00 && tmp[3] == 0x00)
				{
						read_n = 4;
						*bom = AR_TXT_BOM_UTF32_LE;
				}else
				{
						*bom = AR_TXT_BOM_UTF16_LE;
						read_n = 2;
				}
		}else if(tmp[0] == 0xFE && tmp[1] == 0xFF)
		{
				*bom = AR_TXT_BOM_UTF16_BE;
				read_n = 2;
		}else if(tmp[0] == 0x00 && tmp[1] == 0x00)
		{
				if(tmp[2] == 0xFE && tmp[3] == 0xFF)
				{
						*bom = AR_TXT_BOM_UTF32_BE;
						read_n = 4;
				}else
				{
						*bom = AR_TXT_BOM_ASCII;
						read_n = 0;
				}
		}else if(tmp[0] == 0xEF && tmp[1] == 0xBB)
		{
				if( tmp[2] == 0xBF)
				{
						*bom = AR_TXT_BOM_UTF_8;
						read_n = 3;
				}else
				{
						*bom = AR_TXT_BOM_ASCII;
						read_n = 0;
				}
		}
		else
		{
				*bom = AR_TXT_BOM_ASCII;
				read_n = 0;
		}

		if(input_len < read_n)
		{
				return false;
		}else
		{
				AR_ReadBufferData(input, NULL, read_n);
		}

		return true;
}




typedef enum
{
		TXT_READ_OK = 0x00,
		TXT_READ_INVALID,
		TXT_READ_EOF
}txtReadStatus_t;

static txtReadStatus_t		__read_wchar(arBuffer_t *input, arTxtBom_t enc, wchar_t *out)
{

		uint_32_t e;

		AR_ASSERT(input != NULL);

		e = 0;

		switch(enc)
		{
		case AR_TXT_BOM_UTF_8:
		{
				byte_t		b;
				byte_t		buf[5];
				size_t		rn;
				uint_32_t	v;

				b = 0;
				rn = 0;
				v = 0;
				
				rn = AR_ReadBufferData(input, &b, 1);
				
				if(rn != 1)
				{
						return TXT_READ_EOF;
				}

				v = (uint_32_t)b;

				if(v >= 0xfc)
				{
						/*6:<11111100>*/
						/*读剩下的字节*/
						rn = AR_ReadBufferData(input, buf, 5);
						if(rn != 5)
						{
								return TXT_READ_INVALID;
						}

						e = (v & 0x01) << 30;
						e |= (buf[0] & 0x3f) << 24;
						e |= (buf[1] & 0x3f) << 18;
						e |= (buf[2] & 0x3f) << 12;
						e |= (buf[3] & 0x3f) << 6;
						e |= (buf[4] & 0x3f);
				}else if(v >= 0xf8)
				{
						/*5:<11111000>*/
						rn = AR_ReadBufferData(input, buf, 4);
						if(rn != 4)
						{
								return TXT_READ_INVALID;
						}

						e = (v & 0x03) << 24;
						e |= (buf[0] & 0x3f) << 18;
						e |= (buf[1] & 0x3f) << 12;
						e |= (buf[2] & 0x3f) << 6;
						e |= (buf[3] & 0x3f);

				}else if(v >= 0xf0)
				{
						/*4:<11110000>*/
						rn = AR_ReadBufferData(input, buf, 3);
						if(rn != 3)
						{
								return TXT_READ_INVALID;
						}
						e = (v & 0x07) << 18;
						e |= (buf[0] & 0x3f) << 12;
						e |= (buf[1] & 0x3f) << 6;
						e |= (buf[2] & 0x3f);

				}else if(v >= 0xe0)
				{
						/*3:<11100000>*/
						rn = AR_ReadBufferData(input, buf, 2);
						if(rn != 2)
						{
								return TXT_READ_INVALID;
						}

						e = (v & 0x0f) << 12;
						e |= (buf[0] & 0x3f) << 6;
						e |= (buf[1] & 0x3f);

				}else if(v >= 0xc0)
				{
						/*3:<11000000>*/
						rn = AR_ReadBufferData(input, buf, 1);
						if(rn != 1)
						{
								return TXT_READ_INVALID;
						}
						e = (v & 0x1f) << 6;
						e |= (buf[0] & 0x3f);
				}else
				{
						e = (wchar_t)v;
				}
		}
				break;
		case AR_TXT_BOM_UTF16_BE:
		case AR_TXT_BOM_UTF16_LE:
		{
				byte_t buf[2];
				size_t	rn;
				rn = AR_ReadBufferData(input, buf, 2);
				if(rn != 2)
				{
						return TXT_READ_EOF;
				}

				if(enc == AR_TXT_BOM_UTF16_BE)
				{
						e = ((uint_32_t)buf[0]) << 8 | (uint_32_t)buf[1];
				}else
				{
						e = ((uint_32_t)buf[1]) << 8 | (uint_32_t)buf[0];
				}
		}
				break;
		case AR_TXT_BOM_UTF32_BE:
		case AR_TXT_BOM_UTF32_LE:
		{
				byte_t buf[4];
				size_t	rn;
				rn = AR_ReadBufferData(input, buf, 4);
				if(rn != 4)
				{
						return TXT_READ_EOF;
				}

				if(enc == AR_TXT_BOM_UTF32_BE)
				{
						e = ((uint_32_t)buf[0]) << 24 | (uint_32_t)buf[1] << 16 | (uint_32_t)buf[2] << 8 | (uint_32_t)buf[3];
				}else
				{
						e = (uint_32_t)buf[0] | (uint_32_t)buf[1] << 8 | (uint_32_t)buf[2] << 16 | ((uint_32_t)buf[3]) << 24;
				}
		}
				break;
		case AR_TXT_BOM_ASCII:
		default:
				return TXT_READ_INVALID;
				break;
		}

		if(out)*out = (wchar_t)e;
		return TXT_READ_OK;
}








bool_t	AR_LoadBomTextFromBinary(arBuffer_t *input, arTxtBom_t *bom, arString_t *out)
{


		arTxtBom_t	enc;
		wchar_t c;
		
		txtReadStatus_t	status;
		AR_ASSERT(input != NULL && out != NULL);

		status = TXT_READ_OK;
		AR_ClearString(out);


		if(!__dectect_encoding(input, &enc))
		{
				status = TXT_READ_INVALID;
				goto FAILED_POINT;
		}

		if(enc == AR_TXT_BOM_ASCII)
		{
				
				wchar_t *str = NULL;

				size_t cp;
				for(cp = AR_CP_ACP; cp < AR_CP_MAX; ++cp)
				{

						str = AR_str_convto_wcs((arCodePage_t)cp, (const char*)AR_GetBufferData(input), AR_GetBufferAvailable(input));
						if(str != NULL)
						{
								break;
						}

				}

				if(!str)
				{
						status = TXT_READ_INVALID;
						goto FAILED_POINT;
				}

				if(out && str)
				{
						AR_AppendString(out, str);
				}

				if(str)
				{
						AR_DEL(str);
						str = NULL;
				}

		}else
		{
				do{

						status = __read_wchar(input, enc, &c);

						if(status == TXT_READ_OK && out)
						{
								AR_AppendCharToString(out, c);
						}
				}while(status == TXT_READ_OK);
		}

		if(bom)
		{
				*bom = enc;
		}

FAILED_POINT:
		
		return status != TXT_READ_INVALID;
}






bool_t	AR_LoadBomTextFile(const wchar_t *path, arTxtBom_t *bom, arString_t *out)
{

		FILE	*file = NULL;
		arBuffer_t *buf;
		bool_t	is_ok;
		
		AR_ASSERT(path != NULL && out != NULL);

		is_ok = true;
		buf = NULL;
		file = NULL;
		

		file = __AR_open_file(path, L"rb");
		if(!file)
		{
				is_ok = false;
				goto FAILED_POINT;
		}


		buf = AR_CreateBuffer(1024);


		{
				size_t	rn;
				byte_t	tmp[256];
				
				do{
						rn = fread((void*)tmp, 1, sizeof(tmp), file);
						if(rn > 0)
						{
								AR_InsertBuffer(buf, tmp, rn);
						}
				}while(!feof(file) && !ferror(file));

				if(ferror(file))
				{
						is_ok = false;
						goto FAILED_POINT;
				}else
				{
						tmp[0] = '\0';
						AR_InsertBuffer(buf, tmp, 1);
				}
		}

		is_ok = AR_LoadBomTextFromBinary(buf, bom, out);

FAILED_POINT:
		if(file)
		{
				fclose(file);
				file = NULL;
		}

		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}

		return is_ok;
}







/***************************************Write File**********************************************************/

static bool_t __write_bom(arBuffer_t *out, arTxtBom_t bom)
{
		byte_t buf[4];
		size_t wn;
		bool_t is_ok;
		AR_ASSERT(out != NULL);
		is_ok = true;

		switch(bom)
		{
		case AR_TXT_BOM_UTF_8:
		{
				wn = 3;
				buf[0] = 0xEF;
				buf[1] = 0xBB;
				buf[2] = 0xBF;
		}
				break;
		case AR_TXT_BOM_UTF16_BE:
		{
				wn = 2;
				buf[0] = 0xFE;
				buf[1] = 0xFF;
		}
				break;
		case AR_TXT_BOM_UTF16_LE:
		{
				wn = 2;
				buf[0] = 0xFF;
				buf[1] = 0xFE;
		}
				break;
		case AR_TXT_BOM_UTF32_BE:
		{
				wn = 4;
				buf[0] = 0x00;
				buf[1] = 0x00;
				buf[2] = 0xFE;
				buf[3] = 0xFF;
		}
				break;
		case AR_TXT_BOM_UTF32_LE:
		{
				wn = 4;
				buf[0] = 0xFF;
				buf[1] = 0xFE;
				buf[2] = 0x00;
				buf[3] = 0x00;
		}
				break;
		case AR_TXT_BOM_ASCII:
		default:
				wn = 0;
				is_ok = false;
				break;
		}

		if(wn > 0)
		{
				AR_InsertBuffer(out, (byte_t*)buf, wn);
		}
		return is_ok;
}






static bool_t __write_wchar(arBuffer_t *out, arTxtBom_t bom, wchar_t c)
{
		bool_t is_ok;
		AR_ASSERT(out != NULL);

		is_ok = true;

		switch(bom)
		{
		case AR_TXT_BOM_UTF_8:
		{
				byte_t utf8[10];
				size_t n;
				byte_t *e;
				uint_32_t	uc = (uint_32_t)c;

				e = utf8;

				if(uc < 0x80)
				{
						*e++ = (byte_t)uc;
				}else if(uc < 0x800)
				{
						/*<11011111> < 000 0000 0000>*/
						*e++ = (byte_t)((uc >> 6) & 0x1f)|0xc0;
						*e++ = (byte_t)(uc & 0x3f)|0x80;
				}else if(uc < 0x10000)
				{
						/*<11101111> <0000 0000 0000 0000>*/
						*e++ = (byte_t)(((uc >> 12) & 0x0f)|0xe0);
						*e++ = (byte_t)(((uc >> 6) & 0x3f)|0x80);
						*e++ = (byte_t)((uc & 0x3f)|0x80);
				}else if(uc < 0x200000)
				{
						/*<11110111> <0 0000 0000 0000 0000 0000>*/
						*e++ = (byte_t)(((uc >> 18) & 0x07)|0xf0);
						*e++ = (byte_t)(((uc >> 12) & 0x3f)|0x80);
						*e++ = (byte_t)(((uc >> 6) & 0x3f)|0x80);
						*e++ = (byte_t)((uc & 0x3f)|0x80);
				}else if(uc < 0x4000000)
				{
						/*<11111011> <00 0000 0000 0000 0000 0000 0000>*/
						*e++ = (byte_t)(((uc >> 24) & 0x03)|0xf8);
						*e++ = (byte_t)(((uc >> 18) & 0x3f)|0x80);
						*e++ = (byte_t)(((uc >> 12) & 0x3f)|0x80);
						*e++ = (byte_t)(((uc >> 6) & 0x3f)|0x80);
						*e++ = (byte_t)((uc & 0x3f)|0x80);
				}else
				{
						/*<11111101> <0000 0000 0000 0000 0000 0000 0000 0000>*/
						*e++ = (byte_t)(((uc >> 30) & 0x01)|0xfc);
						*e++ = (byte_t)(((uc >> 24) & 0x3f)|0x80);
						*e++ = (byte_t)(((uc >> 18) & 0x3f)|0x80);
						*e++ = (byte_t)(((uc >> 12) & 0x3f)|0x80);
						*e++ = (byte_t)(((uc >> 6) & 0x3f)|0x80);
						*e++ = (byte_t)((uc & 0x3f)|0x80);
				}

				n =  e - utf8;

				AR_InsertBuffer(out, (byte_t*)utf8, n);
		}
				break;
		case AR_TXT_BOM_UTF16_BE:
		case AR_TXT_BOM_UTF16_LE:
		{
				byte_t buf[2];
				uint_16_t	uc = (uint_16_t)c;


				if(bom == AR_TXT_BOM_UTF16_BE)
				{
						buf[0] = (byte_t)(uc >> 8);
						buf[1] = (byte_t)(uc & 0x00FF);

				}else
				{
						buf[1] = (byte_t)(uc >> 8);
						buf[0] = (byte_t)(uc & 0x00FF);
				}

				AR_InsertBuffer(out, (byte_t*)buf, 2);
		}
				break;
		case AR_TXT_BOM_UTF32_BE:
		case AR_TXT_BOM_UTF32_LE:
		{
				byte_t buf[4];
				uint_32_t	uc = (uint_32_t)c;


				if(bom == AR_TXT_BOM_UTF32_BE)
				{
						buf[0] = (byte_t)(uc >> 24);
						buf[1] = (byte_t)(uc >> 16);
						buf[2] = (byte_t)(uc >> 8);
						buf[3] = (byte_t)(uc & 0x000000FF);
				}else
				{
						buf[3] = (byte_t)(uc >> 24);
						buf[2] = (byte_t)(uc >> 16);
						buf[1] = (byte_t)(uc >> 8);
						buf[0] = (byte_t)(uc & 0x000000FF);
				}

				AR_InsertBuffer(out, (byte_t*)buf, 4);
		}
				break;
		case AR_TXT_BOM_ASCII:
		default:
				break;
		}

		return is_ok;
}



bool_t	AR_SaveBomTextToBinary(arBuffer_t *output, arTxtBom_t bom, const wchar_t *input)
{
		bool_t	is_ok;
		const wchar_t  *p;

		AR_ASSERT(output != NULL && input != NULL);



		is_ok = true;


		if(bom == AR_TXT_BOM_ASCII)
		{
				size_t n;
				char *s = AR_wcs_convto_str(AR_CP_ACP, input, AR_wcslen(input));
				n = strlen(s);

				if(!s)
				{
						is_ok = false;
						goto CLEAR_LOCAL;
				}

				if(n == 0)
				{
						goto CLEAR_LOCAL;
				}else
				{
						AR_InsertBuffer(output, (byte_t*)s, n);
				}
CLEAR_LOCAL:
				if(s)
				{
						AR_DEL(s);
						s = NULL;
				}
		}else
		{

				if(!__write_bom(output, bom))
				{
						is_ok = false;
						goto FAILED_POINT;
				}
				p = input;

				while(*p)
				{

						if(!__write_wchar(output, bom, *p))
						{
								is_ok = false;
								goto FAILED_POINT;
						}
						++p;
				}
		}


FAILED_POINT:
		
		return is_ok;
}




/****************************************************AR_SaveBomTextFile***********************************************************************************/




bool_t	AR_SaveBomTextFile(const wchar_t *path, arTxtBom_t bom, const wchar_t *input)
{
		FILE	*file;
		bool_t	is_ok;
		arBuffer_t *buf;
		AR_ASSERT(path != NULL && input != NULL);



		is_ok = true;
		file = NULL;
		buf = NULL;

		buf = AR_CreateBuffer(1024);

		if(!AR_SaveBomTextToBinary(buf, bom, input))
		{
				is_ok = false;
				goto FAILED_POINT;
		}


		file = __AR_open_file(path, L"wb");

		if(!file)
		{
				is_ok = false;
				goto FAILED_POINT;
		}

		
		if(fwrite((const byte_t*)AR_GetBufferData(buf), 1, AR_GetBufferAvailable(buf), file) != AR_GetBufferAvailable(buf))
		{
				is_ok = false;
				goto FAILED_POINT;
		}

FAILED_POINT:
		if(file)
		{
				fclose(file);
				file = NULL;
		}

		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}

		return is_ok;
}
AR_NAMESPACE_END


