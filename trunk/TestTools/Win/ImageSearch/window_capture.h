#pragma once

#include "stdhdr.h"

ar_bool_t CaptureAnImage(HWND hWnd, const wchar_t *path);
ar_bool_t CaptureAnImageSaveToCImg(HWND hWnd, CByteImage_t &img);
