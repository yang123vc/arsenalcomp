#include "test.h"


#include "../Arsenal/Tools/tools.h"






#if defined(__LIB)

AR_NAMESPACE_BEGIN

#if(0)
snObject_t*		__get_int(arBuffer_t	*buffer);
snObject_t*		__get_str(arBuffer_t	*buffer);
snObject_t*		__get_list(arBuffer_t	*buffer);
snObject_t*		__get_dict(arBuffer_t	*buffer);


void sn_test_int()
{
		arBuffer_t		*buf = AR_CreateBuffer(0);
		const char *str = "19840120e";
		AR_InsertBuffer(buf, (const byte_t*)str, strlen(str));
		snObject_t *obj = __get_int(buf);
		printf("remain buf == %d\r\n", AR_GetBufferAvailable(buf));
}

void sn_test_str()
{
		arBuffer_t		*buf = AR_CreateBuffer(0);
		const char *str = ":abeeeee";
		AR_InsertBuffer(buf, (const byte_t*)str, strlen(str));
		snObject_t *obj = __get_str(buf);
		printf("remain buf == %d\r\n", AR_GetBufferAvailable(buf));

}


void sn_test_list()
{
		arBuffer_t		*buf = AR_CreateBuffer(0);
		const char *str = "2:abi3e2:dei888ee";
		AR_InsertBuffer(buf, (const byte_t*)str, strlen(str));

		snObject_t *obj = __get_list(buf);
		printf("remain buf == %d\r\n", AR_GetBufferAvailable(buf));
}


void sn_test_dict()
{
		arBuffer_t		*buf = AR_CreateBuffer(0);
		const char *str = "2:abi3e2:dei888ee";
		AR_InsertBuffer(buf, (const byte_t*)str, strlen(str));
		snObject_t *obj = __get_dict(buf);
		printf("remain buf == %d\r\n", AR_GetBufferAvailable(buf));
}


void sn_test_obj()
{
		arBuffer_t		*buf = AR_CreateBuffer(0);
		const char *str = "d2:abi3e2:dei888ee";
		AR_InsertBuffer(buf, (const byte_t*)str, strlen(str));
		snObject_t *obj = SN_GetObject(buf);
		printf("remain buf == %d\r\n", AR_GetBufferAvailable(buf));

		AR_DestroyBuffer(buf);
		SN_DestroyObject(obj);
}



/************************************/

void		__put_int(arBuffer_t	*buffer, const snInteger_t *integer);
void __put_string(arBuffer_t	*buffer, const snString_t *string);

void sn_put_int_test()
{
		arBuffer_t		*buffer;
		snInteger_t		integer;
		buffer = AR_CreateBuffer(0);

		integer.is_signed = true;
		integer.u = 102400000;
		__put_int(buffer, &integer);

		AR_DestroyBuffer(buffer);
}


void sn_put_str_test()
{
		arBuffer_t		*buffer;
		snString_t		str;
		buffer = AR_CreateBuffer(0);

		SN_InitString(&str);
		SN_SetStringByStr(&str, "abcdef");

		__put_string(buffer, &str);
		
		AR_DestroyBuffer(buffer);
}

#endif


void sn_test_torrent()
{
		arBuffer_t		*buffer;
		buffer = AR_CreateBuffer(0);

		FILE *f = fopen("d:\\temp\\1.torrent", "rb");
		assert(f != NULL);

		byte_t buf[4096];
		
		while(!ferror(f) && !feof(f))
		{
				size_t rn = fread(buf, sizeof(byte_t), sizeof(buf), f);
				if(rn == 0)break;
				AR_InsertBuffer(buffer, buf, rn);
		}
		fclose(f);
		
		snObject_t *obj = SN_GetObject(buffer);
		SN_PutObject(buffer, obj);
		SN_DestroyObject(obj);


		FILE *new_f = fopen("d:\\temp\\new.torrent", "wb");
		assert(new_f != NULL);

		fwrite(AR_GetBufferData(buffer), sizeof(byte_t), AR_GetBufferAvailable(buffer), new_f);

		fclose(new_f);

		AR_DestroyBuffer(buffer);

}




void sn_test_path()
{
		arBuffer_t		*buffer;
		buffer = AR_CreateBuffer(0);

		FILE *f = fopen("d:\\temp\\1.torrent", "rb");
		assert(f != NULL);

		byte_t buf[4096];
		
		while(!ferror(f) && !feof(f))
		{
				size_t rn = fread(buf, sizeof(byte_t), sizeof(buf), f);
				if(rn == 0)break;
				AR_InsertBuffer(buffer, buf, rn);
		}
		fclose(f);
		
		snObject_t *obj = SN_GetObject(buffer);
		AR_ASSERT(obj != NULL);
		AR_DestroyBuffer(buffer);
		
		int_t l;
		wchar_t wcs[1024];

		snObject_t *announce = SN_FindObjectByWcsPath(obj, L"announce");

		l = SN_GetWcsFromStringObject(announce, wcs, 1024);

		AR_ASSERT(l > 0);

		::MessageBoxW(NULL, wcs, 0, 0);

		snObject_t *announce_list = SN_FindObjectByWcsPath(obj, L"announce-list");

		if(announce_list != NULL)
		{
				for(size_t i = 0; i < SN_GetListObjectCount(announce_list); ++i)
				{
						snObject_t *obj = SN_GetFromListObject(announce_list, i);
						AR_ASSERT(obj != NULL && SN_GetObjectType(obj) == SN_LIST_T);

						for(size_t k = 0; k < SN_GetListObjectCount(obj); ++k)
						{
								const snObject_t *str = SN_GetFromListObject(obj, k);
								AR_ASSERT(str != NULL && SN_GetObjectType(str) == SN_STRING_T);

								l = SN_GetWcsFromStringObject(str, wcs, 1024);

								AR_ASSERT(l > 0);

								::MessageBoxW(NULL, wcs, 0, 0);
						}

				}
		}



		
		SN_DestroyObject(obj);
}



void sn_test_str()
{
		snObject_t *obj;
		
		int_t	ret;
		byte_t	data[512];
		char	str[512];
		wchar_t wcs[512];

		obj = SN_CreateObject(SN_STRING_T);

		byte_t buf[] = 
		{
				'a',
				'b',
				'c',
				'd'
		};

		SN_SetStringObjectByData(obj, buf, sizeof(buf));

		
		ret = SN_GetDataFromStringObject(obj, NULL, 0);

		AR_printf(L"SN_GetDataFromStringObject : len == %d\r\n", ret);

		ret = SN_GetStrFromStringObject(obj, NULL, 0);
		AR_printf(L"SN_GetStrFromStringObject : len == %d\r\n", ret);


		ret = SN_GetWcsFromStringObject(obj, NULL, 0);
		AR_printf(L"SN_GetWcsFromStringObject : len == %d\r\n", ret);




		ret = SN_GetDataFromStringObject(obj, NULL, 0);
		ret = SN_GetDataFromStringObject(obj, data, sizeof(data));
		AR_printf(L"SN_GetDataFromStringObject : len == %d\r\n", ret);

		
		ret = SN_GetStrFromStringObject(obj, NULL, 0);
		ret = SN_GetStrFromStringObject(obj, str, sizeof(str));
		AR_printf(L"SN_GetStrFromStringObject : str == %hs, len == %d\r\n", str, ret);


		ret = SN_GetWcsFromStringObject(obj, NULL, 0);
		ret = SN_GetWcsFromStringObject(obj, wcs, sizeof(wcs)/2);
		AR_printf(L"SN_GetWcsFromStringObject : wcs == %s, len == %d\r\n", wcs, ret);
		
		/*
		ret = SN_GetWcsFromStringObject(obj, NULL, 0);
		AR_printf(L"SN_GetWcsFromStringObject : len == %d\r\n", ret);
		*/


		SN_DestroyObject(obj);
}




void sn_test_str2()
{
		snObject_t *obj;
		
		int_t	ret;
		byte_t	data[512];
		char	str[512];
		wchar_t wcs[512];

		obj = SN_CreateObject(SN_STRING_T);

		byte_t buf[] = 
		{
				'a',
				'b',
				0,
				'c',
				'd'
		};

		SN_SetStringObjectByData(obj, buf, sizeof(buf));

		
		ret = SN_GetDataFromStringObject(obj, NULL, 0);

		AR_printf(L"SN_GetDataFromStringObject : len == %d\r\n", ret);

		ret = SN_GetStrFromStringObject(obj, NULL, 0);
		AR_printf(L"SN_GetStrFromStringObject : len == %d\r\n", ret);


		ret = SN_GetWcsFromStringObject(obj, NULL, 0);
		AR_printf(L"SN_GetWcsFromStringObject : len == %d\r\n", ret);




		ret = SN_GetDataFromStringObject(obj, NULL, 0);
		ret = SN_GetDataFromStringObject(obj, data, sizeof(data));
		AR_printf(L"SN_GetDataFromStringObject : len == %d\r\n", ret);

		
		ret = SN_GetStrFromStringObject(obj, NULL, 0);
		ret = SN_GetStrFromStringObject(obj, str, sizeof(str));
		AR_printf(L"SN_GetStrFromStringObject : str == %hs, len == %d\r\n", str, ret);


		ret = SN_GetWcsFromStringObject(obj, NULL, 0);
		ret = SN_GetWcsFromStringObject(obj, wcs, sizeof(wcs)/2);
		AR_printf(L"SN_GetWcsFromStringObject : wcs == %s, len == %d\r\n", wcs, ret);

		
		SN_DestroyObject(obj);
}





void sn_test_str3()
{
		snObject_t *obj;
		
		int_t	ret;
		byte_t	data[512];
		char	str[512];
		wchar_t wcs[512];

		obj = SN_CreateObject(SN_STRING_T);

		//byte_t buf[] = "字符集问题太麻烦了，操！！！abcdefg";

		const wchar_t *txt = L"字符集问题太麻烦了，操！！！abcdefg";


		//SN_SetStringObjectByData(obj, buf, sizeof(buf));

		SN_SetStringObjectByWcs(obj, txt);
		
		ret = SN_GetDataFromStringObject(obj, NULL, 0);

		AR_printf(L"SN_GetDataFromStringObject : len == %d\r\n", ret);

		ret = SN_GetStrFromStringObject(obj, NULL, 0);
		AR_printf(L"SN_GetStrFromStringObject : len == %d\r\n", ret);


		ret = SN_GetWcsFromStringObject(obj, NULL, 0);
		AR_printf(L"SN_GetWcsFromStringObject : len == %d\r\n", ret);




		ret = SN_GetDataFromStringObject(obj, NULL, 0);
		ret = SN_GetDataFromStringObject(obj, data, sizeof(data));
		AR_printf(L"SN_GetDataFromStringObject : len == %d\r\n", ret);

		
		ret = SN_GetStrFromStringObject(obj, NULL, 0);
		ret = SN_GetStrFromStringObject(obj, str, sizeof(str));
		AR_printf(L"SN_GetStrFromStringObject : str == %hs, len == %d\r\n", str, ret);


		ret = SN_GetWcsFromStringObject(obj, NULL, 0);
		ret = SN_GetWcsFromStringObject(obj, wcs, sizeof(wcs)/2);
		AR_printf(L"SN_GetWcsFromStringObject : wcs == %ls, len == %d\r\n", wcs, ret);

		
		SN_DestroyObject(obj);
}






void sn_test_find()
{
		arBuffer_t		*buffer;
		buffer = AR_CreateBuffer(0);

		FILE *f = fopen("d:\\user\\temp\\1.torrent", "rb");
		assert(f != NULL);

		byte_t buf[4096];
		
		while(!ferror(f) && !feof(f))
		{
				size_t rn = fread(buf, sizeof(byte_t), sizeof(buf), f);
				if(rn == 0)break;
				AR_InsertBuffer(buffer, buf, rn);
		}
		fclose(f);
		
		snObject_t *root = SN_GetObject(buffer);
		AR_ASSERT(root != NULL);
		AR_DestroyBuffer(buffer);
		
		int_t l;
		wchar_t wcs[1024];

		snObject_t *announce = SN_FindObjectByWcsPath(root, L"////announce");

		l = SN_GetWcsFromStringObject(announce, wcs, 1024);

		AR_ASSERT(l > 0);

		::MessageBoxW(NULL, wcs, 0, 0);

		snObject_t *announce_list = SN_FindObjectByWcsPath(root, L"announce-list");

		if(announce_list != NULL)
		{
				for(size_t i = 0; i < SN_GetListObjectCount(announce_list); ++i)
				{
						wchar_t buf[200];
						AR_swprintf(buf, 200, L"/announce-list/%d", i);
						snObject_t *obj = SN_FindObjectByWcsPath(root, buf);
						AR_ASSERT(obj != NULL && SN_GetObjectType(obj) == SN_LIST_T);

						for(size_t k = 0; k < SN_GetListObjectCount(obj); ++k)
						{
								const snObject_t *str = SN_GetFromListObject(obj, k);
								AR_ASSERT(str != NULL && SN_GetObjectType(str) == SN_STRING_T);

								l = SN_GetWcsFromStringObject(str, wcs, 1024);

								AR_ASSERT(l > 0);

								::MessageBoxW(NULL, wcs, 0, 0);
						}

				}
		}



		
		SN_DestroyObject(root);
}


void sn_test()
{
		//sn_test_torrent();
		//sn_test_path();
		//sn_test_str();
		//sn_test_str2();
		//sn_test_str3();

		sn_test_find();
}

AR_NAMESPACE_END

#endif
