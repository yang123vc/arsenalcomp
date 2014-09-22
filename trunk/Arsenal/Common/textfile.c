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


arStatus_t      AR_DetectTextBom(const ar_byte_t *data, size_t length, arTxtBom_t *bom, size_t *bom_len)
{
        ar_byte_t tmp[4] = {0xcc, 0xcc,0xcc,0xcc};
		size_t read_n;
        arTxtBom_t data_bom;
        AR_ASSERT(data != NULL);
        
		read_n = 0;
        data_bom = AR_TXT_BOM_NONE;
        
		AR_memcpy(tmp, data, AR_MIN(length, 4));
        
		if(tmp[0] == 0xFF && tmp[1] == 0xFE)
		{
				if(tmp[2] == 0x00 && tmp[3] == 0x00)
				{
                        data_bom = AR_TXT_BOM_UTF32_LE;
                        read_n = 4;
				}else
				{
						data_bom = AR_TXT_BOM_UTF16_LE;
						read_n = 2;
				}
		}else if(tmp[0] == 0xFE && tmp[1] == 0xFF)
		{
				data_bom = AR_TXT_BOM_UTF16_BE;
				read_n = 2;
                
		}else if(tmp[0] == 0x00 && tmp[1] == 0x00)
		{
				if(tmp[2] == 0xFE && tmp[3] == 0xFF)
				{
						data_bom = AR_TXT_BOM_UTF32_BE;
						read_n = 4;
				}else
				{
						data_bom = AR_TXT_BOM_NONE;
						read_n = 0;
				}
		}else if(tmp[0] == 0xEF && tmp[1] == 0xBB)
		{
				if( tmp[2] == 0xBF)
				{
						data_bom = AR_TXT_BOM_UTF_8;
						read_n = 3;
				}else
				{
						data_bom = AR_TXT_BOM_NONE;
						read_n = 0;
				}
		}else
		{
                data_bom = AR_TXT_BOM_NONE;
				read_n = 0;
		}
        
        if(bom)
        {
                *bom = data_bom;
        }
        
        if(bom_len)
        {
                *bom_len = read_n;
        }
        
        return AR_S_YES;
}








/***************************************Read File**********************************************************/

/*****************************************************************************************/


static ar_bool_t	__dectect_encoding(arBuffer_t *input, arTxtBom_t *bom)
{
		ar_byte_t tmp[4] = {0xcc, 0xcc,0xcc,0xcc};
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
						*bom = AR_TXT_BOM_NONE;
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
						*bom = AR_TXT_BOM_NONE;
						read_n = 0;
				}
		}
		else
		{
				*bom = AR_TXT_BOM_NONE;
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

		ar_uint_32_t e;

		AR_ASSERT(input != NULL);

		e = 0;

		switch(enc)
		{
		case AR_TXT_BOM_UTF_8:
		{
				ar_byte_t		b;
				ar_byte_t		buf[5];
				size_t		rn;
				ar_uint_32_t	v;

				b = 0;
				rn = 0;
				v = 0;
				
				rn = AR_ReadBufferData(input, &b, 1);
				
				if(rn != 1)
				{
						return TXT_READ_EOF;
				}

				v = (ar_uint_32_t)b;

				if(v >= 0xfc)
				{
						/*6:<11111100>*/
						/*Reading the remaining bytes*/
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
				ar_byte_t buf[2];
				size_t	rn;
				rn = AR_ReadBufferData(input, buf, 2);
				if(rn != 2)
				{
						return TXT_READ_EOF;
				}

				if(enc == AR_TXT_BOM_UTF16_BE)
				{
						e = ((ar_uint_32_t)buf[0]) << 8 | (ar_uint_32_t)buf[1];
				}else
				{
						e = ((ar_uint_32_t)buf[1]) << 8 | (ar_uint_32_t)buf[0];
				}
		}
				break;
		case AR_TXT_BOM_UTF32_BE:
		case AR_TXT_BOM_UTF32_LE:
		{
				ar_byte_t buf[4];
				size_t	rn;
				rn = AR_ReadBufferData(input, buf, 4);
				if(rn != 4)
				{
						return TXT_READ_EOF;
				}

				if(enc == AR_TXT_BOM_UTF32_BE)
				{
						e = ((ar_uint_32_t)buf[0]) << 24 | (ar_uint_32_t)buf[1] << 16 | (ar_uint_32_t)buf[2] << 8 | (ar_uint_32_t)buf[3];
				}else
				{
						e = (ar_uint_32_t)buf[0] | (ar_uint_32_t)buf[1] << 8 | (ar_uint_32_t)buf[2] << 16 | ((ar_uint_32_t)buf[3]) << 24;
				}
		}
				break;
		case AR_TXT_BOM_NONE:
		default:
				return TXT_READ_INVALID;
				break;
		}

		if(out)*out = (wchar_t)e;
		return TXT_READ_OK;
}

/******************************************util**************************************/

static ar_bool_t __is_utf8(const ar_byte_t * string, size_t len)
{
		const ar_byte_t *bytes, *end;
		if(string == NULL || len == 0)
		{
				return false;
		}

		bytes = string;
		end = string + len;

		while(bytes < end)
		{
				if(*bytes == 0)
				{
						bytes += 1;
						continue;
				}

				if( (// ASCII
						// use bytes[0] <= 0x7F to allow ASCII control characters
						bytes[0] == 0x09 ||
						bytes[0] == 0x0A ||
						bytes[0] == 0x0D ||
						(0x20 <= bytes[0] && bytes[0] <= 0x7E)
						)
						) 
				{
						bytes += 1;
						continue;
				}

				if( (// non-overlong 2-byte
						(0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
						(0x80 <= bytes[1] && bytes[1] <= 0xBF)
						)
						) 
				{
								bytes += 2;
								continue;
				}

				if( (// excluding overlongs
						bytes[0] == 0xE0 &&
						(0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
						(0x80 <= bytes[2] && bytes[2] <= 0xBF)
						) ||
						(// straight 3-byte
						((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
						bytes[0] == 0xEE ||
						bytes[0] == 0xEF) &&
						(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
						(0x80 <= bytes[2] && bytes[2] <= 0xBF)
						) ||
						(// excluding surrogates
						bytes[0] == 0xED &&
						(0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
						(0x80 <= bytes[2] && bytes[2] <= 0xBF)
						)
						) 
				{
								bytes += 3;
								continue;
				}

				if( (// planes 1-3
						bytes[0] == 0xF0 &&
						(0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
						(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
						(0x80 <= bytes[3] && bytes[3] <= 0xBF)
						) ||
						(// planes 4-15
						(0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
						(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
						(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
						(0x80 <= bytes[3] && bytes[3] <= 0xBF)
						) ||
						(// plane 16
						bytes[0] == 0xF4 &&
						(0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
						(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
						(0x80 <= bytes[3] && bytes[3] <= 0xBF)
						)
						) 
				{
								bytes += 4;
								continue;
				}

				return false;
		}

		return true;
}


arStatus_t	AR_LoadBomTextFromBinaryWithCodePage(arBuffer_t *input, arTxtBom_t *bom, arString_t *out, arCodePage_t code_page)
{
		arStatus_t ret;
		arTxtBom_t	enc;
		
		
		txtReadStatus_t	status;
		AR_ASSERT(input != NULL && out != NULL);

		ret = AR_S_YES;
		status = TXT_READ_OK;
		AR_ClearString(out);
		
		if(AR_GetBufferAvailable(input) == 0)
		{
				if(bom)
				{
						*bom = AR_TXT_BOM_NONE;
				}
				return AR_S_YES;
		}

		
		if(!__dectect_encoding(input, &enc))
		{
				status = TXT_READ_INVALID;
				goto FAILED_POINT;
		}

		if(enc == AR_TXT_BOM_NONE)
		{
				
				wchar_t *str = NULL;

				

				if(code_page < AR_CP_MAX)
				{
						str = AR_str_to_wcs(code_page, (const char*)AR_GetBufferData(input), AR_GetBufferAvailable(input));
				}

				if(str == NULL)
				{
						if(__is_utf8(AR_GetBufferData(input),AR_GetBufferAvailable(input)))
						{
								str = AR_str_to_wcs(AR_CP_UTF8, (const char*)AR_GetBufferData(input), AR_GetBufferAvailable(input));

								if(str)
								{
										enc = AR_TXT_BOM_UTF_8_WITHOUT_BOM;
								}
						}
				}

				if(str == NULL)
				{
						size_t cp;

						for(cp = AR_CP_ACP; cp < AR_CP_MAX; ++cp)
						{
								if(AR_CP_ACP == AR_CP_UTF8) /*前面已经判断过了，不是utf8*/
								{
										continue;
								}

								str = AR_str_to_wcs((arCodePage_t)cp, (const char*)AR_GetBufferData(input), AR_GetBufferAvailable(input));
								if(str != NULL)
								{
										break;
								}

						}
				}


				if(!str)
				{
						ret = AR_E_BADENCCONV;
						status = TXT_READ_INVALID;
						goto FAILED_POINT;
				}

				if(out && str)
				{
						ret = AR_AppendString(out, str);
						if(ret != AR_S_YES)
						{
								status = TXT_READ_INVALID;
								goto FAILED_POINT;
						}
				}

				if(str)
				{
						AR_DEL(str);
						str = NULL;
				}

		}else
		{
				do{
						wchar_t c;
						status = __read_wchar(input, enc, &c);

						if(status == TXT_READ_OK && out)
						{
								ret = AR_AppendCharToString(out, c);
								if(ret != AR_S_YES)
								{
										status = TXT_READ_INVALID;
										goto FAILED_POINT;
								}
						}
				}while(status == TXT_READ_OK);
		}

		if(bom)
		{
				*bom = enc;
		}

FAILED_POINT:
		
		if(status != TXT_READ_INVALID)
		{
				return AR_S_YES;
		}else
		{
				return ret;
		}
}

arStatus_t	AR_LoadBomTextFileWithCodePage(const wchar_t *path, arTxtBom_t *bom, arString_t *out, arCodePage_t code_page)
{
		arStatus_t		ret;
		arFile_t		*file = NULL;
		arBuffer_t		*buf;
		
		AR_ASSERT(path != NULL && out != NULL);

		ret = AR_S_YES;
		buf = NULL;
		file = NULL;
		

		ret = AR_open_file(&file, path, L"rb");

		if(ret != AR_S_YES)
		{
				AR_error(AR_ERR_WARNING, L"AR_open_file failed for %ls in function '%hs'\r\n", path, AR_FUNC_NAME);
				goto FAILED_POINT;
		}


		buf = AR_CreateBuffer(1024);

		if(buf == NULL)
		{
				ret = AR_E_NOMEM;
				goto FAILED_POINT;
		}

		{
				size_t	rn;
				ar_byte_t	tmp[256];
				
				do{
						ret = AR_read_file(file, tmp, 256, &rn);
						/*rn = fread((void*)tmp, 1, sizeof(tmp), file);*/
						if(rn > 0)
						{
								ret = AR_InsertToBuffer(buf, tmp, rn);
								if(ret == AR_E_NOMEM)
								{
										goto FAILED_POINT;
								}
						}
				//}while(!feof(file) && !ferror(file));
				}while(AR_eof_file(file) != AR_S_YES && AR_error_file(file) != AR_S_YES);

				if(AR_error_file(file) == AR_S_YES)
				{
						ret = AR_E_FILE;
						AR_error(AR_ERR_WARNING, L"fread failed for %ls in function '%hs'\r\n", path, AR_FUNC_NAME);
						goto FAILED_POINT;
				}else
				{
						tmp[0] = '\0';
						
						ret = AR_InsertToBuffer(buf, tmp, 1);
						if(ret != AR_S_YES)
						{
								goto FAILED_POINT;
						}
				}
		}

		ret = AR_LoadBomTextFromBinaryWithCodePage(buf, bom, out, code_page);

FAILED_POINT:
		if(file)
		{
				AR_close_file(file);
				file = NULL;
		}

		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}

		return ret;
}


arStatus_t	AR_LoadBomTextFromBinary(arBuffer_t *input, arTxtBom_t *bom, arString_t *out)
{
		return AR_LoadBomTextFromBinaryWithCodePage(input, bom, out, AR_CP_MAX);
}

arStatus_t	AR_LoadBomTextFile(const wchar_t *path, arTxtBom_t *bom, arString_t *out)
{
		return AR_LoadBomTextFileWithCodePage(path, bom, out, AR_CP_MAX);
}

/***************************************Write File**********************************************************/

static arStatus_t __write_bom(arBuffer_t *out, arTxtBom_t bom)
{
		ar_byte_t buf[4];
		size_t wn;
		arStatus_t ret;
		AR_ASSERT(out != NULL);
		ret = AR_S_YES;

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
		case AR_TXT_BOM_UTF_8_WITHOUT_BOM:
		{
				wn = 0;
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
		case AR_TXT_BOM_NONE:
		default:
				wn = 0;
				ret = AR_E_INVAL;
				break;
		}

		if(wn > 0)
		{
				ret = AR_InsertToBuffer(out, (ar_byte_t*)buf, wn);
				
		}
		return ret;
}






static arStatus_t __write_wchar(arBuffer_t *out, arTxtBom_t bom, wchar_t c)
{
		arStatus_t ret;
		AR_ASSERT(out != NULL);

		ret = AR_S_YES;

		switch(bom)
		{
		case AR_TXT_BOM_UTF_8:
		case AR_TXT_BOM_UTF_8_WITHOUT_BOM:
		{
				ar_byte_t utf8[10];
				size_t n;
				ar_byte_t *e;
				ar_uint_32_t	uc = (ar_uint_32_t)c;

				e = utf8;

				if(uc < 0x80)
				{
						*e++ = (ar_byte_t)uc;
				}else if(uc < 0x800)
				{
						/*<11011111> < 000 0000 0000>*/
						*e++ = (ar_byte_t)((uc >> 6) & 0x1f)|0xc0;
						*e++ = (ar_byte_t)(uc & 0x3f)|0x80;
				}else if(uc < 0x10000)
				{
						/*<11101111> <0000 0000 0000 0000>*/
						*e++ = (ar_byte_t)(((uc >> 12) & 0x0f)|0xe0);
						*e++ = (ar_byte_t)(((uc >> 6) & 0x3f)|0x80);
						*e++ = (ar_byte_t)((uc & 0x3f)|0x80);
				}else if(uc < 0x200000)
				{
						/*<11110111> <0 0000 0000 0000 0000 0000>*/
						*e++ = (ar_byte_t)(((uc >> 18) & 0x07)|0xf0);
						*e++ = (ar_byte_t)(((uc >> 12) & 0x3f)|0x80);
						*e++ = (ar_byte_t)(((uc >> 6) & 0x3f)|0x80);
						*e++ = (ar_byte_t)((uc & 0x3f)|0x80);
				}else if(uc < 0x4000000)
				{
						/*<11111011> <00 0000 0000 0000 0000 0000 0000>*/
						*e++ = (ar_byte_t)(((uc >> 24) & 0x03)|0xf8);
						*e++ = (ar_byte_t)(((uc >> 18) & 0x3f)|0x80);
						*e++ = (ar_byte_t)(((uc >> 12) & 0x3f)|0x80);
						*e++ = (ar_byte_t)(((uc >> 6) & 0x3f)|0x80);
						*e++ = (ar_byte_t)((uc & 0x3f)|0x80);
				}else
				{
						/*<11111101> <0000 0000 0000 0000 0000 0000 0000 0000>*/
						*e++ = (ar_byte_t)(((uc >> 30) & 0x01)|0xfc);
						*e++ = (ar_byte_t)(((uc >> 24) & 0x3f)|0x80);
						*e++ = (ar_byte_t)(((uc >> 18) & 0x3f)|0x80);
						*e++ = (ar_byte_t)(((uc >> 12) & 0x3f)|0x80);
						*e++ = (ar_byte_t)(((uc >> 6) & 0x3f)|0x80);
						*e++ = (ar_byte_t)((uc & 0x3f)|0x80);
				}

				n =  e - utf8;

				ret = AR_InsertToBuffer(out, (ar_byte_t*)utf8, n);
				
		}
				break;
		case AR_TXT_BOM_UTF16_BE:
		case AR_TXT_BOM_UTF16_LE:
		{
				ar_byte_t buf[2];
				ar_uint_16_t	uc = (ar_uint_16_t)c;


				if(bom == AR_TXT_BOM_UTF16_BE)
				{
						buf[0] = (ar_byte_t)(uc >> 8);
						buf[1] = (ar_byte_t)(uc & 0x00FF);

				}else
				{
						buf[1] = (ar_byte_t)(uc >> 8);
						buf[0] = (ar_byte_t)(uc & 0x00FF);
				}

				ret = AR_InsertToBuffer(out, (ar_byte_t*)buf, 2);
				
		}
				break;
		case AR_TXT_BOM_UTF32_BE:
		case AR_TXT_BOM_UTF32_LE:
		{
				ar_byte_t buf[4];
				ar_uint_32_t	uc = (ar_uint_32_t)c;


				if(bom == AR_TXT_BOM_UTF32_BE)
				{
						buf[0] = (ar_byte_t)(uc >> 24);
						buf[1] = (ar_byte_t)(uc >> 16);
						buf[2] = (ar_byte_t)(uc >> 8);
						buf[3] = (ar_byte_t)(uc & 0x000000FF);
				}else
				{
						buf[3] = (ar_byte_t)(uc >> 24);
						buf[2] = (ar_byte_t)(uc >> 16);
						buf[1] = (ar_byte_t)(uc >> 8);
						buf[0] = (ar_byte_t)(uc & 0x000000FF);
				}

				ret = AR_InsertToBuffer(out, (ar_byte_t*)buf, 4);
		}
				break;
		case AR_TXT_BOM_NONE:
		default:
				break;
		}

		return ret;
}




arStatus_t	AR_SaveBomTextToBinaryWithCodePage(arBuffer_t *output, arTxtBom_t bom, const wchar_t *input, arCodePage_t code_page)
{
		arStatus_t	ret;
		const wchar_t  *p;

		AR_ASSERT(output != NULL && input != NULL);


		ret = AR_S_YES;

		ret = AR_ReserveBuffer(output, AR_wcslen(input) * 2);

		if(ret != AR_S_YES)
		{
				goto FAILED_POINT;
		}


		if(bom == AR_TXT_BOM_NONE)
		{
				size_t n;
				char *s = NULL;

				if(code_page < AR_CP_MAX)
				{
						s = AR_wcs_to_str(code_page, input, AR_wcslen(input));
				}else
				{
						s = AR_wcs_to_str(AR_CP_ACP, input, AR_wcslen(input));
				}
				
				if(!s)
				{
						ret = AR_E_BADENCCONV;
						goto CLEAR_LOCAL;
				}

				n = strlen(s);

				if(n == 0)
				{
						goto CLEAR_LOCAL;
				}else
				{
						ret = AR_InsertToBuffer(output, (ar_byte_t*)s, n);
						if(ret != AR_S_YES)
						{
								goto CLEAR_LOCAL;
						}
				}
CLEAR_LOCAL:
				if(s)
				{
						AR_DEL(s);
						s = NULL;
				}
		}else
		{
				ret = __write_bom(output, bom);
				if(ret != AR_S_YES)
				{
						goto FAILED_POINT;
				}
				p = input;

				while(*p)
				{
						ret = __write_wchar(output, bom, *p);
						if(ret != AR_S_YES)
						{
								goto FAILED_POINT;
						}
						++p;
				}
		}


FAILED_POINT:
		
		return ret;
}





arStatus_t	AR_SaveBomTextFileWithCodePage(const wchar_t *path, arTxtBom_t bom, const wchar_t *input, arCodePage_t code_page)
{
		arFile_t		*file;
		arStatus_t		ret;
		arBuffer_t		*buf;
		size_t wn;
		AR_ASSERT(path != NULL && input != NULL);



		ret = AR_S_YES;
		file = NULL;
		buf = NULL;

		buf = AR_CreateBuffer(1024);

		if(buf == NULL)
		{
				ret = AR_E_NOMEM;
				goto FAILED_POINT;
		}

		ret = AR_SaveBomTextToBinaryWithCodePage(buf, bom, input, code_page);
		if(ret != AR_S_YES)
		{
				goto FAILED_POINT;
		}


		
		ret = AR_open_file(&file, path, L"wb");

		if(ret != AR_S_YES)
		{
				AR_error(AR_ERR_WARNING, L"AR_open_file failed for %ls in function '%hs'\r\n", path, AR_FUNC_NAME);
				goto FAILED_POINT;
		}


		while(AR_GetBufferAvailable(buf) > 0 && AR_eof_file(file) != AR_S_YES && AR_error_file(file) != AR_S_YES)
		{
				ar_byte_t tmp[256];
				size_t read_n;
				wn = 0;
				read_n = AR_ReadBufferData(buf, tmp, 256);
				
				AR_ASSERT(read_n > 0);

				ret = AR_write_file(file, tmp, read_n, &wn);
				
				if(ret != AR_S_YES || wn != read_n)
				{
						ret = AR_E_FILE;
						AR_error(AR_ERR_WARNING, L"AR_write_file failed for %ls in function '%hs'\r\n", path, AR_FUNC_NAME);
						goto FAILED_POINT;
				}

		}

		if(AR_error_file(file) == AR_S_YES)
		{
				ret = AR_E_FILE;
				AR_error(AR_ERR_WARNING, L"AR_write_file failed for %ls in function '%hs'\r\n", path, AR_FUNC_NAME);
				goto FAILED_POINT;
		}


FAILED_POINT:
		if(file)
		{
				AR_close_file(file);
				file = NULL;
		}

		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}

		return ret;
}


arStatus_t	AR_SaveBomTextToBinary(arBuffer_t *output, arTxtBom_t bom, const wchar_t *input)
{
		return AR_SaveBomTextToBinaryWithCodePage(output, bom, input, AR_CP_MAX);
}


arStatus_t	AR_SaveBomTextFile(const wchar_t *path, arTxtBom_t bom, const wchar_t *input)
{
		return AR_SaveBomTextFileWithCodePage(path, bom, input, AR_CP_MAX);
}




AR_NAMESPACE_END


