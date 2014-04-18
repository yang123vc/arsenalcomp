#pragma once

#include <QtGui>
#include <QPicture>
/*
#include <QtGui/QDialog>
#include <QtGui/QMainWindow>
*/

#include <QtCore/QtCore>


#include "Arsenal.h"
#define cimg_display	0
#include "./CImg/CImg.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace cimg_library;
using namespace ARSpace;



typedef CImg<ar_byte_t> CByteImage_t;






#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")