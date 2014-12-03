#include "stdhdr.h"


#include <math.h>
#include <time.h>





AR_NAMESPACE_BEGIN



static void ini_test1()
{
        
        iniObject_t *ini = Ini_CreateObject();
        AR_ASSERT(ini != NULL);

        
        arStatus_t status = Ini_LoadObjectFromFile(ini, L"/Users/solidus/Desktop/index.dat");

        AR_ASSERT(status == AR_S_YES);
        
        
        
        

}


void Ini_Test()
{
        
        ini_test1();
        
}





AR_NAMESPACE_END