#include "test.h"


#include "../Arsenal/Tools/tools.h"





#if defined(__LIB)

AR_NAMESPACE_BEGIN

#if(0)
snObject_t*		__get_int(arBuffer_t	*buffer);
snObject_t*		__get_str(arBuffer_t	*buffer);
snObject_t*		__get_list(arBuffer_t	*buffer);
snObject_t*		__get_dict(arBuffer_t	*buffer);




void sn_test_path()
{
		arBuffer_t		*buffer;
		buffer = AR_CreateBuffer(0);

		FILE *f = fopen("d:\\temp\\1.torrent", "rb");
		assert(f != NULL);

		ar_byte_t buf[4096];
		
		while(!ferror(f) && !feof(f))
		{
				size_t rn = fread(buf, sizeof(ar_byte_t), sizeof(buf), f);
				if(rn == 0)break;
				AR_InsertToBuffer(buffer, buf, rn);
		}
		fclose(f);
		
		snObject_t *obj = SN_GetObject(buffer);
		AR_ASSERT(obj != NULL);
		AR_DestroyBuffer(buffer);
		
		ar_int_t l;
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

#endif











/************************************/



void sn_test_str()
{
		snObject_t *obj;
		
		ar_int_t	ret;
		ar_byte_t	data[512];
		char	str[512];
		wchar_t wcs[512];

		obj = SN_CreateObject(SN_STRING_T);

		ar_byte_t buf[] = 
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
		str[ret] = '\0';
		AR_printf(L"SN_GetStrFromStringObject : str == %hs, len == %d\r\n", str, ret);


		ret = SN_GetWcsFromStringObject(obj, NULL, 0);
		ret = SN_GetWcsFromStringObject(obj, wcs, sizeof(wcs)/2);
		wcs[ret] = L'\0';
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
		
		ar_int_t	ret;
		ar_byte_t	data[512];
		char	str[512];
		wchar_t wcs[512];

		obj = SN_CreateObject(SN_STRING_T);

		ar_byte_t buf[] = 
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
		str[ret] = '\0';
		AR_printf(L"SN_GetStrFromStringObject : str == %hs, len == %d\r\n", str, ret);


		ret = SN_GetWcsFromStringObject(obj, NULL, 0);
		ret = SN_GetWcsFromStringObject(obj, wcs, sizeof(wcs)/2);
		wcs[ret] = L'\0';
		AR_printf(L"SN_GetWcsFromStringObject : wcs == %s, len == %d\r\n", wcs, ret);

		
		SN_DestroyObject(obj);
}





void sn_test_str3()
{
		snObject_t *obj;
		
		ar_int_t	ret;
		ar_byte_t	data[512];
		char	str[512];
		wchar_t wcs[512];

		obj = SN_CreateObject(SN_STRING_T);

		//ar_byte_t buf[] = "字符集问题太麻烦了，操！！！abcdefg";

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
		str[ret] = '\0';
		AR_printf(L"SN_GetStrFromStringObject : str == %hs, len == %d\r\n", str, ret);


		ret = SN_GetWcsFromStringObject(obj, NULL, 0);
		ret = SN_GetWcsFromStringObject(obj, wcs, sizeof(wcs)/2);
		wcs[ret] = L'\0';
		AR_printf(L"SN_GetWcsFromStringObject : wcs == %ls, len == %d\r\n", wcs, ret);

		
		SN_DestroyObject(obj);
}







void sn_test_torrent()
{
		arBuffer_t		*buffer;
		snRetVal_t		ret;
		FILE *new_f = NULL;
		buffer = AR_CreateBuffer(0);

		FILE *f = fopen("d:\\1.torrent", "rb");
		assert(f != NULL);

		ar_byte_t buf[4096];
		
		while(!ferror(f) && !feof(f))
		{
				size_t rn = fread(buf, sizeof(ar_byte_t), sizeof(buf), f);
				if(rn == 0)break;
				AR_InsertToBuffer(buffer, buf, rn);
		}
		fclose(f);
		
		ret = SN_GetObject(buffer);

		if(ret.status != AR_S_YES)
		{
				goto END_POINT;
		}


		SN_PutObject(buffer, ret.obj);
		


		new_f = fopen("d:\\new.torrent", "wb");
		assert(new_f != NULL);

		fwrite(AR_GetBufferData(buffer), sizeof(ar_byte_t), AR_GetBufferAvailable(buffer), new_f);

END_POINT:
		if(new_f)
		{
				fclose(new_f);
		}

		if(buffer)
		{
				AR_DestroyBuffer(buffer);
				buffer = NULL;
		}

		if(ret.obj)
		{
				SN_DestroyObject(ret.obj);
				ret.obj = NULL;
		}

}



void sn_test_find()
{
		arBuffer_t		*buffer;
		buffer = AR_CreateBuffer(0);

		FILE *f = fopen("d:\\1.torrent", "rb");
		assert(f != NULL);

		ar_byte_t buf[4096];
		
		while(!ferror(f) && !feof(f))
		{
				size_t rn = fread(buf, sizeof(ar_byte_t), sizeof(buf), f);
				if(rn == 0)break;
				AR_InsertToBuffer(buffer, buf, rn);
		}
		fclose(f);
		
		snObject_t *root = SN_GetObject(buffer).obj;
		AR_ASSERT(root != NULL);
		AR_DestroyBuffer(buffer);
		
		ar_int_t l;
		wchar_t wcs[1024];

		snObject_t *announce = SN_FindObjectByWcsPath(root, L"////announce").obj;
		if(announce == NULL)
		{
				goto END_POINT;
		}

		l = SN_GetWcsFromStringObject(announce, wcs, 1024);

		AR_ASSERT(l > 0);

		::MessageBoxW(NULL, wcs, 0, 0);

		snObject_t *announce_list = SN_FindObjectByWcsPath(root, L"announce-list").obj;

		if(announce_list != NULL)
		{
				for(size_t i = 0; i < SN_GetListObjectCount(announce_list); ++i)
				{
						wchar_t buf[200];
						AR_swprintf(buf, 200, L"/announce-list/%d", i);
						snObject_t *obj = SN_FindObjectByWcsPath(root, buf).obj;
						if(obj == NULL)
						{
								goto END_POINT;
						}

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



END_POINT:
		if(root)
		{
				SN_DestroyObject(root);
				root = NULL;
		}
}



void sn_test_float()
{
		snObject_t		*key = SN_CreateObject(SN_STRING_T);
		SN_SetStringObjectByWcs(key, L"key");

		snObject_t		*val = SN_CreateObject(SN_FLOAT_T);
		SN_SetFloatObject(val, 3.14159265);

		snObject_t		*obj = SN_CreateObject(SN_DICT_T);

		SN_InsertToDictObject(obj, key, val);

		arBuffer_t *buffer  = AR_CreateBuffer(1024);

		if(SN_PutObject(buffer, obj) != AR_S_YES)
		{
				SN_DestroyObject(obj);
				AR_DestroyBuffer(buffer);
				buffer = NULL;
				return;

		}

		SN_DestroyObject(obj);
		obj = NULL;



		obj = SN_GetObject(buffer).obj;
		
		if(obj != NULL)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}

		AR_DestroyBuffer(buffer);
		buffer = NULL;
}


/*

arStatus_t			SN_InsertToListObjectByData(snObject_t *obj, const ar_byte_t *data, size_t len)
arStatus_t			SN_InsertToListObjectByUInt(snObject_t *obj, const ar_uint_64_t val)
arStatus_t			SN_InsertToListObjectByInt(snObject_t *obj, const ar_int_64_t val)
arStatus_t			SN_InsertToListObjectByWcs(snObject_t *obj, const wchar_t *val)
arStatus_t			SN_InsertToListObjectByStr(snObject_t *obj, const char *val)
arStatus_t			SN_InsertToDictObjectByWcsData(snObject_t *obj, const wchar_t *key, const ar_byte_t *data, size_t len)
arStatus_t			SN_InsertToDictObjectByWcsWcs(snObject_t *obj, const wchar_t *key, const wchar_t *val)
arStatus_t			SN_InsertToDictObjectByWcsStr(snObject_t *obj, const wchar_t *key, const char *val)
arStatus_t			SN_InsertToDictObjectByWcsUInt(snObject_t *obj, const wchar_t *key, ar_uint_64_t val)
arStatus_t			SN_InsertToDictObjectByWcsInt(snObject_t *obj, const wchar_t *key, ar_int_64_t val)
arStatus_t			SN_InsertToDictObjectByStrData(snObject_t *obj, const char *key, const ar_byte_t *data, size_t len)
arStatus_t			SN_InsertToDictObjectByStrWcs(snObject_t *obj, const char *key, const wchar_t *val)
arStatus_t			SN_InsertToDictObjectByStrStr(snObject_t *obj, const char *key, const char *val)
arStatus_t			SN_InsertToDictObjectByStrUInt(snObject_t *obj, const char *key, ar_uint_64_t val)
arStatus_t			SN_InsertToDictObjectByStrInt(snObject_t *obj, const char *key, ar_int_64_t val)
*/



		/*
		SN_InsertToListObjectByData(obj, (const ar_byte_t*)"abcdefg", strlen("abcdefg"));
		SN_InsertToListObjectByUInt(obj,  33)
		SN_InsertToListObjectByInt(obj, -55)
		SN_InsertToListObjectByWcs(obj, L"中国字");
		SN_InsertToListObjectByStr(obj, "中国字");
		*/

static void util_sn_test()
{
		arBuffer_t *buf = AR_CreateBuffer(100);
		snObject_t *obj = NULL;
		obj = SN_CreateObject(SN_DICT_T);


		
		SN_InsertToDictObjectByStrData(obj, "中国字0", (const ar_byte_t*)"abc", 3);
		SN_InsertToDictObjectByStrWcs(obj, "中国字1", L"a中国字k");
		SN_InsertToDictObjectByStrStr(obj, "中国字2", "a中国字");
		SN_InsertToDictObjectByStrUInt(obj, "中国字3", 66);
		SN_InsertToDictObjectByStrInt(obj, "中国字4", -66);


		SN_InsertToDictObjectByWcsData(obj, L"中国字5", (const ar_byte_t*)"abc", 3);
		SN_InsertToDictObjectByWcsWcs(obj, L"中国字6", L"abc中国字");
		SN_InsertToDictObjectByWcsStr(obj, L"中国字7", "abc中国字");
		SN_InsertToDictObjectByWcsUInt(obj, L"中国字8", 88);
		SN_InsertToDictObjectByWcsInt(obj, L"中国字9", -88);
		

		SN_InsertToDictObjectByWcsWcs(obj, L"中国字6", L"");

		

		{
				AR_ClearBuffer(buf);
				SN_PutObject(buf, obj);

				FILE *f = fopen("d:\\1.txt", "w+");

				assert(f != NULL);

				fwrite(AR_GetBufferData(buf), 1, AR_GetBufferAvailable(buf), f);

				fclose(f);
				f = NULL;
		}

		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}

		snRetVal_t ret = SN_GetObject(buf);

		if(ret.obj)
		{
				SN_DestroyObject(ret.obj);
				ret.obj = NULL;
		}


		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}
}




static void util_sn2_test()
{
		arBuffer_t *buf = AR_CreateBuffer(100);
		snObject_t *obj = NULL;
		obj = SN_CreateObject(SN_LIST_T);


		SN_InsertToListObjectByData(obj, (const ar_byte_t*)"abcdefg", strlen("abcdefg"));
		SN_InsertToListObjectByUInt(obj,  33);
		SN_InsertToListObjectByInt(obj, -55);
		SN_InsertToListObjectByWcs(obj, L"中国字");
		SN_InsertToListObjectByStr(obj, "中国字");
		SN_InsertToListObjectByWcs(obj, L"");


		{
				AR_ClearBuffer(buf);
				SN_PutObject(buf, obj);

				FILE *f = fopen("d:\\2.txt", "w+");

				assert(f != NULL);

				fwrite(AR_GetBufferData(buf), 1, AR_GetBufferAvailable(buf), f);

				fclose(f);
				f = NULL;
		}

		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}

		snRetVal_t ret = SN_GetObject(buf);

		if(ret.obj)
		{
				SN_DestroyObject(ret.obj);
				ret.obj = NULL;
		}


		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}
}




static void util_sn3_test()
{
		arBuffer_t *buf = AR_CreateBuffer(100);
		snObject_t *obj = NULL;
		obj = SN_CreateObject(SN_DICT_T);


		
		SN_InsertToDictObjectByStrData(obj, "", (const ar_byte_t*)"abc", 3);
		SN_InsertToDictObjectByStrWcs(obj, "", L"a中国字k");
		SN_InsertToDictObjectByWcsData(obj, L"", (const ar_byte_t*)"abc", 3);
		SN_InsertToDictObjectByWcsWcs(obj, L"", L"abc中国字");
		SN_InsertToDictObjectByWcsStr(obj, L"", "abc中国字");
		SN_InsertToDictObjectByWcsStr(obj, L"   ", "空格！！");
		

		{
				AR_ClearBuffer(buf);
				SN_PutObject(buf, obj);

				FILE *f = fopen("d:\\3.txt", "w+");

				assert(f != NULL);

				fwrite(AR_GetBufferData(buf), 1, AR_GetBufferAvailable(buf), f);

				fclose(f);
				f = NULL;
		}

		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}

		snRetVal_t ret = SN_GetObject(buf);

		if(ret.obj)
		{
				{
						AR_ClearBuffer(buf);
						SN_PutObject(buf, ret.obj);

						FILE *f = fopen("d:\\3_v.txt", "w+");

						assert(f != NULL);

						fwrite(AR_GetBufferData(buf), 1, AR_GetBufferAvailable(buf), f);

						fclose(f);
						f = NULL;
				}		

				SN_DestroyObject(ret.obj);
				ret.obj = NULL;
		}



		if(buf)
		{
				AR_DestroyBuffer(buf);
				buf = NULL;
		}

}

void sn_test()
{
		//sn_test_torrent();
		//sn_test_path();
		//sn_test_str();
		//sn_test_str2();
		//sn_test_str3();
		//sn_test_find();

		//sn_test_float();
		//util_sn_test();
		//util_sn2_test();

		util_sn3_test();
}



AR_NAMESPACE_END

#endif

