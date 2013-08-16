//
//  plist_test.c
//  Test
//
//  Created by solidus on 13-8-9.
//  Copyright (c) 2013年 none. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#include "Arsenal.h"





AR_NAMESPACE_BEGIN


static void base64_test_for_plist()
{
        const char *b = "ENi/X/9/AACupOyH/38AADAGAAAAAAAAMAQQAAEAAADwv552/38AADgugAABAAAAYNi/\r\n"
                        "X/9/AABupm2S/38AAAAAAAAAAAAAACiAAAEAAAAAMAAAAQAAAGjYHHj/fwAAAQAAAAAA\r\n"
                        "AADwv552/38AAAAAAAAAAAAAAZGhdv9/AABg279f/38AAD6jbpL/fwAAAQAAAAAAAAAB\r\n"
                        "2r9f/38AAAAAAAAAAAAA0K2edv9/AAAAAAAAAAAAAAH4v1//fwAAoNu/X/9/AACwahAA\r\n"
                        "AQAAABBznXb/fwAAoHMQAAEAAADAKZ52/38AAAIAAAAAAAAA//////////8AAAAAAAAA\r\n"
                        "AAAwAAABAAAAAAAAAAAAAAAAagAAAQAAAABqAAABAAAAwNm/X/9/AAC5e+2H/38AANDZ\r\n"
                        "v1//fwAAAQAAAAAAAAABAAAAAAAAAKqwbZL/fwAA8EAAAAEAAAABAO2HAAAAAARqAAAB\r\n"
                        "AAAAAQAAAAEAAABADhAAAQAAABA8gAABAAAAADAAAAEAAAD//////////wAAAAAAAAAA\r\n"
                        "AAAAAAAAAAAAMAAAAQAAAJCxnnb/fwAAAGoAAAEAAAABAAAAAAAAAP//////////AAAA\r\n"
                        "AAAAAAAAMAAAAQAAAAAAAAAAAAAAAGoAAAEAAAAAagAAAQAAAJDav1//fwAAuXvth/9/\r\n"
                        "AACQ2r9f/38AAAEAAAAAAAAA/v////////+qsG2S/38AAAAwAAABAAAAAAAAAAAAAAAA\r\n"
                        "agAAAQAAAIDBnnb/fwAAQA4QAAEAAAAQPIAAAQAAAJCxnnb/fwAAAgAAAAAAAAD4////\r\n"
                        "/////+F7EAABAAAAADAAAAEAAADQwZ52/38AAABqAAABAAAAAGoAAAEAAAAw279f/38A\r\n"
                        "ALl77Yf/fwAAAAAAAAAAAAAEAAAAAAAAADgAAAAAAAAA+HkQAAEAAADwQAAAAQAAAAQA\r\n"
                        "gAABAAAABGoAAAEAAAAEAAAAAAAAAAAAAAAAAAAAINu/X/9/AAAQ279f/38AAHCm7If/\r\n"
                        "fwAAACiAAAEAAAAfAAAAAAAAADAEEAABAAAA8CmAAAEAAADQwZ52/38AAAAAAAAAAAAA\r\n"
                        "YNu/X/9/AAD8pW2S/38AAAAAAAAAAAAAACiAAAEAAAAAMAAAAQAAAGjYHHj/fwAAAQAA\r\n"
                        "AAAAAADQwZ52/38AAAAAAAAAAAAAAZGhdv9/AABg3r9f/38AAD6jbpL/fwAAEHQQAAEA\r\n"
                        "AAABAAAAAAAAAAEAAAAAAAAAATADAAEAAABQ3L9f/38AADh77Yf/fwAAAAAAAAAAAAAI\r\n"
                        "AAAAAAAAAIAAAAAAAAAAsH0QAAEAAACQL552/38AAAgAAAAAAAAABGoAAAEAAABA2Bx4\r\n"
                        "/38AAA==\r\n";
        
        
        //ar_int_t AR_base64_decode(ar_byte_t  *out, size_t olen, const ar_byte_t *input, size_t ilen)
        
        ar_byte_t out[2048];
        
        ar_int_t l = AR_base64_decode(out, 2048, (const ar_byte_t*)b, AR_strlen(b));
        
        AR_ASSERT(l == 1024);
}

static void format_test()
{
        wprintf(L"%hs\r\n", "abcdef");
        
        AR_printf(L"%hc\r\n", 'a');
        //AR_printf(L"%c\r\n", L'中');
}

static void load_test()
{
        arStatus_t status;
        arString_t *xml = AR_CreateString();
        
        system("pwd");
        status = PList_LoadXMLFromFile(L"/Users/solidus/Documents/Code/Solidus/Arsenal/misc/plist_enc_test/plist_big5.plist", xml);
        
        AR_printf(L"%ls\r\n", AR_CSTR(xml));
        
        AR_DestroyString(xml);
        xml = NULL;
}



#define __DATE_TIME_MILLISECONDS        (1000)
#define __DATE_TIME_SECONDS             (1000    *       __DATE_TIME_MILLISECONDS)
#define __DATE_TIME_MINUTES             (60      *       __DATE_TIME_SECONDS)
#define __DATE_TIME_HOURS               (60      *       __DATE_TIME_MINUTES)
#define __DATE_TIME_DAYS                (24      *       __DATE_TIME_HOURS)

static double __to_julian_day(ar_int_t year, ar_int_t month, ar_int_t day, ar_int_t hour, ar_int_t minute, ar_int_t second, ar_int_t millisecond, ar_int_t microsecond)
{
        // lookup table for (153*month - 457)/5 - note that 3 <= month <= 14.
        static const ar_int_t lookup[] = {-91, -60, -30, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337};
        
        // day to double
        double dday = double(day) + ((double((hour*60 + minute)*60 + second)*1000 + millisecond)*1000 + microsecond)/86400000000.0;
        
        if (month < 3)
        {
                month += 12;
                --year;
        }
        
        double dyear = (double)year;
        return dday + lookup[month] + 365*year + AR_floor_dbl(dyear/4) - AR_floor_dbl(dyear/100) + AR_floor_dbl(dyear/400) + 1721118.5;
}

static ar_int_64_t __to_utc_time(ar_int_t year, ar_int_t month, ar_int_t day, ar_int_t hour, ar_int_t minute, ar_int_t second, ar_int_t millisecond, ar_int_t microsecond)
{
        double julian_day = __to_julian_day(year, month, day, 0, 0, 0, 0, 0);
        ar_int_64_t utc_time = (ar_int_64_t)(((julian_day - 2299160.5)*864000000000.0));
        
        return utc_time + 10 * (hour*__DATE_TIME_HOURS + minute*__DATE_TIME_MINUTES + second*__DATE_TIME_SECONDS + millisecond*__DATE_TIME_MILLISECONDS + microsecond);
}


static void time_test()
{
        CFAbsoluteTime at = 0.0;
        
        CFGregorianDate date = {1984, 1, 1, 1, 0, 0};
      
        
        at = CFGregorianDateGetAbsoluteTime(date, NULL);
        
        AR_printf(L"%f\r\n", at);

        
        //ar_int_64_t utc_time = __to_utc_time(1984, 1,1,1,0,0,0,0);
        
        
        time_t t;
        struct tm tm;
        AR_memset(&tm, 0, sizeof(tm));
        
        tm.tm_year = 1984 - 1900;
        tm.tm_mon = 0;
        tm.tm_mday = 1;
        tm.tm_hour = 1;
        tm.tm_min = 1;
        tm.tm_sec = 1;
        //tm.tm_zone = "CST";
        
        const char *ts = asctime(&tm);
        printf("%s\r\n", ts);
        
        t = mktime(&tm);
        
        struct tm tm2;
        localtime_r(&t, &tm2);
        
#if(0)
        time_t timep;
        struct tm *p;
        time(&timep); /*获得time_t结构的时间，UTC时间*/
        p = gmtime(&timep); /*转换为struct tm结构的UTC时间*/

//        t = mktime(p);
#endif

        NSDate *ns_date = [NSDate dateWithTimeIntervalSince1970 : (NSTimeInterval)t];
        NSLog(@"%@", ns_date);
        
       // localtime_r(&time, &tm);
        
//        const struct tm *gm_tm = gmtime(&tm);
}

static void real_test()
{
        AR_ASSERT(!AR_is_inf_flt(0.14f));
        AR_ASSERT(!AR_is_inf_dbl(0.14));
        
        AR_ASSERT(!AR_is_nan_flt(0.14f));
        AR_ASSERT(!AR_is_nan_dbl(0.14));
        
        AR_ASSERT(AR_is_nan_flt(NAN));
        AR_ASSERT(AR_is_nan_dbl(NAN));
        
        AR_ASSERT(!AR_is_inf_dbl(HUGE_VAL));
}


static void parse_xml_test1()
{
        //L"/Users/solidus/Documents/Code/Solidus/Arsenal/misc/test.plist"
//#define PLIST_FILE_PATH L"/Users/solidus/Desktop/Code/Arsenal/misc/test.plist"
        #define PLIST_FILE_PATH L"/Users/solidus/Desktop/test2.plist"
        
        arStatus_t status;
        arString_t *xml = AR_CreateString();
        status = PList_LoadXMLFromFile(PLIST_FILE_PATH, xml);
        
        AR_printf(L"%ls\r\n", AR_CSTR(xml));
                
        plistXMLParser_t *parser = PList_CreateXMLParser();
        
        status = PList_SetXMLParserWithWcs(parser, AR_CSTR(xml), AR_GetStringLength(xml));
        AR_ASSERT(status == AR_S_YES);
        
        plistElem_t *elem = PList_ParseXML(parser);
        
        if(elem == NULL)
        {
                AR_ASSERT(PList_XMLParserInError(parser));
                
                AR_error(AR_ERR_WARNING, L"%ls\r\n", PList_GetXMLParserErrorMessage(parser));
                //AR_ASSERT(false);
                
        }else
        {
                /*
                const wchar_t *s = PList_GetElemCString(elem);
                AR_printf(L"%ls\r\n", s);
                */
                
                
                //const char *s = (const char*)PList_GetElemData(elem);
                //AR_printf(L"%hs\r\n", s);
                /*
                const plistNumber_t *number = PList_GetElemNumber(elem);
                if(number->integer.is_signed)
                {
                        AR_printf(L"%qd\r\n", number->integer.signed_num);
                }else
                {
                        AR_printf(L"%qd\r\n", number->integer.unsigned_num);
                }
                */
                

                size_t arr_cnt = PList_GetElemArrayCount(elem);
                AR_printf(L"array count : %Iu\r\n", arr_cnt);
                
                for(size_t i = 0; i < arr_cnt; ++i)
                {
                        plistElem_t *val = PList_GetElemArrayByIndex(elem, i);
                        
                        AR_printf(L"type : %u\r\n", val->type);
                }

                
                /*
                size_t dict_cnt = PList_GetElemDictCount(elem);
                AR_printf(L"dict count : %Iu\r\n", dict_cnt);
                
                for(size_t i = 0; i < dict_cnt; ++i)
                {
                        
                }
                */
                
                
                //plistElem_t *val = PList_FindElemDictValueByWcs(elem, L"a");
                
                
                
                PList_DestroyElem(elem);
                elem = NULL;
        }
        
        PList_DestroyXMLParser(parser);
        parser = NULL;
        AR_DestroyString(xml);
        xml = NULL;
}


static void save_plist_elem_test()
{
        //L"/Users/solidus/Documents/Code/Solidus/Arsenal/misc/test.plist"
        //#define PLIST_FILE_PATH L"/Users/solidus/Desktop/Code/Arsenal/misc/test.plist"
#define PLIST_FILE_PATH L"/Users/solidus/Desktop/test2.plist"
        
        arStatus_t status;
        arString_t *xml = AR_CreateString();
        status = PList_LoadXMLFromFile(PLIST_FILE_PATH, xml);
        
        AR_printf(L"%ls\r\n", AR_CSTR(xml));
        
        plistXMLParser_t *parser = PList_CreateXMLParser();
        
        status = PList_SetXMLParserWithWcs(parser, AR_CSTR(xml), AR_GetStringLength(xml));
        AR_ASSERT(status == AR_S_YES);
        
        plistElem_t *elem = PList_ParseXML(parser);
        
        if(elem == NULL)
        {
                AR_ASSERT(PList_XMLParserInError(parser));
                
                AR_error(AR_ERR_WARNING, L"%ls\r\n", PList_GetXMLParserErrorMessage(parser));
                //AR_ASSERT(false);
                
        }else
        {
                arString_t *xml = AR_CreateString();
                AR_ASSERT(xml != NULL);
                
                status = PList_SaveElemToXML(elem,xml);
                AR_printf(L"%ls\r\n", AR_CSTR(xml));
                
                
                AR_SaveBomTextFile(L"/Users/solidus/Desktop/1.txt", AR_TXT_BOM_UTF_8, AR_CSTR(xml));
                
                AR_DestroyString(xml);
                xml = NULL;
                
                PList_DestroyElem(elem);
                elem = NULL;
        }
        
        PList_DestroyXMLParser(parser);
        parser = NULL;
        AR_DestroyString(xml);
        xml = NULL;
}

static void plist_parse_binary_test1()
{
        
#define BINPLIST_FILE_PATH L"/Users/solidus/Desktop/test2_bin.plist"

        
        arStatus_t status;
        arBuffer_t *bin = AR_CreateBuffer(1024);
        AR_ASSERT(bin != NULL);
        
        status = PList_LoadBinaryFromFile(BINPLIST_FILE_PATH, bin);
        AR_ASSERT(status == AR_S_YES);
        
        plistElem_t *elem = NULL;
        
        status = PList_TryParseBinaryPlist(AR_GetBufferData(bin), AR_GetBufferAvailable(bin),&elem, NULL);
        
        
        if(bin)
        {
                AR_DestroyBuffer(bin);
                bin = NULL;
        }
        
        if(elem)
        {
                PList_DestroyElem(elem);
                elem = NULL;
        }

        
}

void plist_test()
{
        //format_test();
        //base64_test_for_plist();
        //load_test();
        //time_test();
        //real_test();
        //parse_xml_test1();
        
        save_plist_elem_test();
        
        plist_parse_binary_test1();
}



AR_NAMESPACE_END


