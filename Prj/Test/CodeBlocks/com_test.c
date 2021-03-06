
#include "test.h"
#include <locale.h>


AR_NAMESPACE_BEGIN





void text_test_save()
{
#define OUTPUT_FILE		L"/root/Desktop/x.txt"
#define OUTPUT_TYPE		AR_TXT_BOM_NONE

		arString_t *str = AR_CreateString();

		if(AR_LoadBomTextFile(L"/root/Desktop/Programming/Arsenal/misc/txt_enc_test/utf16_be_read_test.txt",NULL, str) != AR_S_YES)
		{
				AR_ASSERT(false);
		}

		if(AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStringCString(str)) != AR_S_YES)
		{
		}

		if(AR_SaveBomTextFile(OUTPUT_FILE, OUTPUT_TYPE, AR_GetStringCString(str)) != AR_S_YES)
		{
				AR_ASSERT(false);
		}


		arTxtBom_t bom;
		if(AR_LoadBomTextFile(OUTPUT_FILE, &bom, str)  != AR_S_YES)
		{
				AR_ASSERT(false);
		}

		AR_ASSERT(bom == OUTPUT_TYPE);


		AR_DestroyString(str);
}


static void mem_test()
{
    ar_byte_t *b = AR_NEWARR(ar_byte_t, 1024);
}


static void file_seek_test()
{
        arFile_t *f;
        ar_uint_64_t offset;

        arStatus_t		status = AR_open_file(&f, L"/bin/bash", L"rb");

        status = AR_seek_file(f, 0, AR_FILE_SEEK_END);
        AR_ASSERT(status == AR_S_YES);
        status = AR_tell_file(f, &offset);
        AR_ASSERT(status == AR_S_YES);
        AR_printf(L"%qu\r\n", offset);

        status = AR_seek_file(f, -100, AR_FILE_SEEK_END);
        AR_ASSERT(status == AR_S_YES);
        status = AR_tell_file(f, &offset);
        AR_ASSERT(status == AR_S_YES);
        AR_printf(L"%qu\r\n", offset);



END_POINT:
        if(f)
        {
                AR_close_file(f);
                f = NULL;

        }
}


static void dir_copy_test()
{
    arStatus_t status = AR_path_copydir(L"/home/solidus/Desktop/1/Test", L"/home/solidus/Desktop/2/Test/");

}


static void mtx_test_1()
{
    arMutex_t *mtx = AR_CreateMutex();
    AR_ASSERT(mtx != NULL);

    arStatus_t status = AR_LockMutex(mtx);
    AR_ASSERT(status == AR_S_YES);

    status = AR_LockMutex(mtx);
    AR_ASSERT(status == AR_S_YES);


    status = AR_UnLockMutex(mtx);
    AR_ASSERT(status == AR_S_YES);

    status = AR_UnLockMutex(mtx);
    AR_ASSERT(status == AR_S_YES);

    AR_DestroyMutex(mtx);
    mtx = NULL;


}

static void spinlock_test()
{
		arSpinLock_t spl;
		AR_InitSpinLock(&spl);

		AR_LockSpinLock(&spl);
		AR_ASSERT(!AR_TryLockSpinLock(&spl));
		AR_UnLockSpinLock(&spl);

		AR_ASSERT(AR_TryLockSpinLock(&spl));
		AR_UnLockSpinLock(&spl);

		AR_UnInitSpinLock(&spl);



}

static void tickcount_test()
{
    ar_int_64_t beg,end;

    for(size_t i = 0; i < 5; ++i)
    {

        beg = AR_GetTime_TickCount();

        AR_Sleep(1000);

        end = AR_GetTime_TickCount();

        AR_printf(L"elapsed milliseconds == %qd\r\n", end - beg);
    }




    getchar();


}

void com_test()
{


        setlocale(LC_CTYPE, "utf-8");

        AR_error(AR_ERR_WARNING, L"test message : %ls", L"zhongguozi");

        //dir_copy_test();

        //file_seek_test();


       //text_test_save();

        //mem_test();

       // mtx_test_1();

       //spinlock_test();

       tickcount_test();

}








AR_NAMESPACE_END




