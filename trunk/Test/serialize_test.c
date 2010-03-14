#include "test.h"


#include "../Arsenal/Tools/tools.h"






#if defined(__LIB)

AR_NAMESPACE_BEGIN

snObject_t*		__get_int(arBuffer_t	*buffer);
snObject_t*		__get_str(arBuffer_t	*buffer);
snObject_t*		__get_list(arBuffer_t	*buffer);
snObject_t*		__get_dict(arBuffer_t	*buffer);


void sn_test_int()
{
		arBuffer_t		*buf = AR_CreateBuffer(0);
		const char *str = "19840120e";
		AR_InsertBuffer(buf, (const byte_b_t*)str, strlen(str));
		snObject_t *obj = __get_int(buf);
		printf("remain buf == %d\r\n", AR_GetBufferAvailable(buf));
}

void sn_test_str()
{
		arBuffer_t		*buf = AR_CreateBuffer(0);
		const char *str = ":abeeeee";
		AR_InsertBuffer(buf, (const byte_b_t*)str, strlen(str));
		snObject_t *obj = __get_str(buf);
		printf("remain buf == %d\r\n", AR_GetBufferAvailable(buf));

}


void sn_test_list()
{
		arBuffer_t		*buf = AR_CreateBuffer(0);
		const char *str = "2:abi3e2:dei888ee";
		AR_InsertBuffer(buf, (const byte_b_t*)str, strlen(str));

		snObject_t *obj = __get_list(buf);
		printf("remain buf == %d\r\n", AR_GetBufferAvailable(buf));
}


void sn_test_dict()
{
		arBuffer_t		*buf = AR_CreateBuffer(0);
		const char *str = "2:abi3e2:dei888ee";
		AR_InsertBuffer(buf, (const byte_b_t*)str, strlen(str));
		snObject_t *obj = __get_dict(buf);
		printf("remain buf == %d\r\n", AR_GetBufferAvailable(buf));
}


void sn_test_obj()
{
		arBuffer_t		*buf = AR_CreateBuffer(0);
		const char *str = "d2:abi3e2:dei888ee";
		AR_InsertBuffer(buf, (const byte_b_t*)str, strlen(str));
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




void sn_test_torrent()
{
		arBuffer_t		*buffer;
		buffer = AR_CreateBuffer(0);

		FILE *f = fopen("d:\\temp\\1.torrent", "rb");
		assert(f != NULL);

		byte_b_t buf[4096];
		
		while(!ferror(f) && !feof(f))
		{
				size_t rn = fread(buf, sizeof(byte_b_t), sizeof(buf), f);
				if(rn == 0)break;
				AR_InsertBuffer(buffer, buf, rn);
		}
		fclose(f);
		
		snObject_t *obj = SN_GetObject(buffer);
		SN_PutObject(buffer, obj);
		SN_DestroyObject(obj);


		FILE *new_f = fopen("d:\\temp\\new.torrent", "wb");
		assert(new_f != NULL);

		fwrite(AR_GetBufferData(buffer), sizeof(byte_b_t), AR_GetBufferAvailable(buffer), new_f);

		fclose(new_f);

		AR_DestroyBuffer(buffer);

}




void sn_test_path()
{
		arBuffer_t		*buffer;
		buffer = AR_CreateBuffer(0);

		FILE *f = fopen("d:\\temp\\1.torrent", "rb");
		assert(f != NULL);

		byte_b_t buf[4096];
		
		while(!ferror(f) && !feof(f))
		{
				size_t rn = fread(buf, sizeof(byte_b_t), sizeof(buf), f);
				if(rn == 0)break;
				AR_InsertBuffer(buffer, buf, rn);
		}
		fclose(f);
		
		snObject_t *obj = SN_GetObject(buffer);
		AR_ASSERT(obj != NULL);
		AR_DestroyBuffer(buffer);
		
		snObject_t *announce = SN_FindPathByWcs(obj, L"announce");

		snObject_t *announce_list = SN_FindPathByWcs(obj, L"announce-list/5");
		
		

		
		SN_DestroyObject(obj);
}


void sn_test()
{
		sn_test_path();
}

AR_NAMESPACE_END

#endif
