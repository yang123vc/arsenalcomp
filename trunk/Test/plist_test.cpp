#include "test.h"


#include "../Arsenal/Tools/tools.h"





#if defined(__LIB)

AR_NAMESPACE_BEGIN


static void parse_xml_test1()
{
        //L"/Users/solidus/Documents/Code/Solidus/Arsenal/misc/test.plist"
//#define PLIST_FILE_PATH L"/Users/solidus/Desktop/Code/Arsenal/misc/test.plist"
        #define PLIST_FILE_PATH L"Z:\\Desktop\\test.plist"
        
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
                

                size_t arr_cnt = PList_GetElemDictCount(elem);
                AR_printf(L"array count : %Iu\r\n", arr_cnt);
                
                for(size_t i = 0; i < arr_cnt; ++i)
                {
                        plistElem_t *val = PList_GetElemDictKeyByIndex(elem, i);
                        
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
#define PLIST_FILE_PATH L"Z:\\Desktop\\test.plist"
        
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
                
                plistElem_t *new_elem = PList_CopyNewElem(elem);
                AR_ASSERT(new_elem != NULL);
                status = PList_SaveElemToXML(new_elem,xml);
                AR_printf(L"%ls\r\n", AR_CSTR(xml));
                PList_DestroyElem(new_elem);
                new_elem = NULL;
                
                AR_SaveBomTextFile(L"C:\\Documents and Settings\\Administrator\\Desktop\\New Folder\\1.plist", AR_TXT_BOM_UTF_8, AR_CSTR(xml));
                
                AR_DestroyString(xml);
                xml = NULL;
                
                PList_DestroyElem(elem);
                elem = NULL;
        }

        if(elem)
        {
                PList_DestroyElem(elem);
                elem = NULL;
        }
        PList_DestroyXMLParser(parser);
        parser = NULL;
        AR_DestroyString(xml);
        xml = NULL;
#undef PLIST_FILE_PATH

}


static void plist_parse_binary_test1()
{
        

#define BINPLIST_FILE_PATH L"Z:\\Desktop\\Bookmarks.plist"

        
        arStatus_t status;
        arBuffer_t *bin = AR_CreateBuffer(1024);
        AR_ASSERT(bin != NULL);
        
        status = PList_LoadBinaryFromFile(BINPLIST_FILE_PATH, bin);
        AR_ASSERT(status == AR_S_YES);
        
        plistElem_t *elem = NULL;
        
        status = PList_TryParseBinaryPlist(AR_GetBufferData(bin), AR_GetBufferAvailable(bin),&elem, NULL);
        
        if(status == AR_S_YES && elem != NULL)
        {
                
                arString_t *xml = AR_CreateString();
                AR_ASSERT(xml != NULL);
                
                status = PList_SaveElemToXML(elem,xml);
                //AR_printf(L"%ls\r\n", AR_CSTR(xml));
                
                AR_SaveBomTextFile(L"C:\\Documents and Settings\\Administrator\\Desktop\\New Folder\\Bookmarks_xml.plist", AR_TXT_BOM_UTF_8, AR_CSTR(xml));
                
                AR_DestroyString(xml);
                xml = NULL;
                
                PList_DestroyElem(elem);
                elem = NULL;
                
        }
        
        
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

#undef BINPLIST_FILE_PATH

}





static void plist_parse_binary_test2()
{

#define BINPLIST_FILE_PATH L"Z:\\Desktop\\Bookmarks.plist"
        
        arStatus_t status;
        arBuffer_t *bin = AR_CreateBuffer(1024);
        AR_ASSERT(bin != NULL);
        
        status = PList_LoadBinaryFromFile(BINPLIST_FILE_PATH, bin);
        AR_ASSERT(status == AR_S_YES);
        
        plistElem_t *elem = NULL;
        
        status = PList_TryParseBinaryPlist(AR_GetBufferData(bin), AR_GetBufferAvailable(bin),&elem, NULL);
        
        if(status == AR_S_YES && elem != NULL)
        {
                
                arBuffer_t *out = AR_CreateBuffer(1024);
                AR_ASSERT(out != NULL);
                
                status = PList_SaveElemToBinary(elem,out);
                
                if(status != AR_S_YES)
                {
                        AR_DestroyBuffer(out);
                        out = NULL;
                        goto END_POINT;
                }
                
                AR_ASSERT(status == AR_S_YES);
                //AR_printf(L"%ls\r\n", AR_CSTR(xml));
                
                
                arFile_t *f = NULL;
                
                status = AR_open_file(&f, L"C:\\Documents and Settings\\Administrator\\Desktop\\New Folder\\Bookmarks_bin.plist", L"wb+");
                
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(f != NULL);
                
                size_t wn;
                status = AR_write_file(f, AR_GetBufferData(out), AR_GetBufferAvailable(out), &wn);
                
                AR_ASSERT(status == AR_S_YES);
                AR_ASSERT(wn == AR_GetBufferAvailable(out));
                
                AR_DestroyBuffer(out);
                out = NULL;
                
                PList_DestroyElem(elem);
                elem = NULL;
                
        }
        
END_POINT:
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
        
#undef BINPLIST_FILE_PATH

}





extern "C" void plist_test()
{
		//parse_xml_test1();
		//save_plist_elem_test();
		//plist_parse_binary_test1();

		//plist_parse_binary_test2();

		plist_parse_binary_test2();

}



AR_NAMESPACE_END



#endif


