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

/******************************************************TextFile**************************************************************/

AR_NAMESPACE_BEGIN



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
		case AR_TXT_BOM_ASCII:
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
						e = ((uint_32_t)buf[3]) << 24 | (uint_32_t)buf[2] << 16 | (uint_32_t)buf[1] << 8 | (uint_32_t)buf[0];
				}
		}
				break;
		default:
				return TXT_READ_INVALID;
				break;
		}

		if(out)*out = (wchar_t)e;
		return TXT_READ_OK;
}

bool_t	AR_LoadBomTextFile(const wchar_t *path, arTxtBom_t *bom, const wchar_t *line_sp, arString_t *out)
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
		file = _wfopen(path, L"rb");
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
		
		
		do{
				bool_t	insert_line_sep = false;
				bool_t	insert_char		= true;

				status = __read_wchar(file, enc, &c);
				
				if(status == TXT_READ_OK)
				{
						if(c == L'\r')
						{
								insert_line_sep = true;			/*到此处，一定需要插入line-sep*/
								
								status = __read_wchar(file, enc, &c);

								if(status == TXT_READ_OK)		
								{
										if(c == L'\n')					/*下一个字符为\n,则不必插入c*/
										{
												insert_char = false;
										}else
										{
												insert_char = true;		/*插入c*/
										}
								}else if(status == TXT_READ_EOF)
								{
										/*\r即为结尾，则结尾插入line-sep*/
								}else
								{
										/*读取存在错误,所以不必插入任何数据了*/
										insert_line_sep	 = false;
										insert_char		 = false;
								}
						}else if(c == L'\n')
						{
								insert_line_sep = true;			/*如果为\n，则需要插入line-sep*/
								insert_char = false;
						}else
						{
								insert_char = true;
						}

						
						if(out && insert_line_sep)
						{
								AR_AppendString(out, line_sp ? line_sp : AR_LINE_SP);
						}

						if(out && insert_char)
						{
								AR_AppendCharToString(out, c);
						}
				}
		}while(status == TXT_READ_OK);
		

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


AR_NAMESPACE_END


