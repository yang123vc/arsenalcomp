#include "window_capture.h"




ar_bool_t CaptureAnImage(HWND hWnd, const wchar_t *path)
{
		//HDC hdcScreen;
		HDC hdcWindow;
		HDC hdcMemDC = NULL;
		HBITMAP hbmScreen = NULL;
		BITMAP bmpScreen;
		ar_bool_t result = true;
		AR_ASSERT(path != NULL);

		// Retrieve the handle to a display device context for the client 
		// area of the window. 
		//hdcScreen = GetDC(NULL);
		hdcWindow = GetDC(hWnd);

		// Create a compatible DC which is used in a BitBlt from the window DC
		hdcMemDC = CreateCompatibleDC(hdcWindow); 

		if(!hdcMemDC)
		{
				AR_error(AR_ERR_WARNING,L"CreateCompatibleDC has failed",L"Failed");
				result = false;
				goto done;
		}

		// Get the client area for size calculation
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		//This is the best stretch mode
		SetStretchBltMode(hdcWindow,HALFTONE);



		// Create a compatible bitmap from the Window DC
		hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top);

		if(!hbmScreen)
		{
				AR_error(AR_ERR_WARNING, L"CreateCompatibleBitmap Failed",L"Failed");
				result = false;
				goto done;
		}

		// Select the compatible bitmap into the compatible memory DC.
		SelectObject(hdcMemDC,hbmScreen);

		// Bit block transfer into our compatible memory DC.
		if(!BitBlt(hdcMemDC, 
				0,0, 
				rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, 
				hdcWindow, 
				0,0,
				SRCCOPY))
		{
				AR_error(AR_ERR_WARNING, L"BitBlt has failed", L"Failed");
				result = false;
				goto done;
		}

		// Get the BITMAP from the HBITMAP
		GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);

		BITMAPFILEHEADER   bmfHeader;    
		BITMAPINFOHEADER   bi;

		bi.biSize = sizeof(BITMAPINFOHEADER);    
		bi.biWidth = bmpScreen.bmWidth;    
		bi.biHeight = bmpScreen.bmHeight;  
		bi.biPlanes = 1;    
		bi.biBitCount = 32;    
		bi.biCompression = BI_RGB;    
		bi.biSizeImage = 0;  
		bi.biXPelsPerMeter = 0;    
		bi.biYPelsPerMeter = 0;    
		bi.biClrUsed = 0;    
		bi.biClrImportant = 0;

		DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

		// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
		// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
		// have greater overhead than HeapAlloc.
		HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
		char *lpbitmap = (char *)GlobalLock(hDIB);    

		// Gets the "bits" from the bitmap and copies them into a buffer 
		// which is pointed to by lpbitmap.
		GetDIBits(hdcWindow, hbmScreen, 0,
				(UINT)bmpScreen.bmHeight,
				lpbitmap,
				(BITMAPINFO *)&bi, DIB_RGB_COLORS);



		// A file is created, this is where we will save the screen capture.
		HANDLE hFile = CreateFileW(path,
				GENERIC_WRITE,
				0,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL, NULL);   

		// Add the size of the headers to the size of the bitmap to get the total file size
		DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		//Offset to where the actual bitmap bits start.
		bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 

		//Size of the file
		bmfHeader.bfSize = dwSizeofDIB; 

		//bfType must always be BM for Bitmaps
		bmfHeader.bfType = 0x4D42; //BM   

		DWORD dwBytesWritten = 0;
		WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
		WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
		WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

		//Unlock and Free the DIB from the heap
		GlobalUnlock(hDIB);    
		GlobalFree(hDIB);

		//Close the handle for the file that was created
		CloseHandle(hFile);

		//Clean up
done:
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
		//    ReleaseDC(NULL,hdcScreen);
		ReleaseDC(hWnd,hdcWindow);

		return result;
}



static ar_bool_t load_bmp(HBITMAP bmp, CByteImage_t &img) 
{
		BITMAP bmpobj;
		if(!::GetObject(bmp,sizeof(bmpobj),&bmpobj) || bmpobj.bmBitsPixel<16)
		{
				return false;
		}
		size_t height = bmpobj.bmHeight;
		size_t width = bmpobj.bmWidth;

		LONG cbBuffer = bmpobj.bmWidthBytes*(bmpobj.bmHeight);
		BYTE *lvbit = new BYTE[cbBuffer];

		if(!::GetBitmapBits(bmp, cbBuffer, lvbit))
		{
				delete []lvbit;
				return false;

		}

		unsigned char* ptrs = lvbit;
		int align = (4-bmpobj.bmWidthBytes%4)%4;

		// Read pixel data
		img.assign(bmpobj.bmWidth,bmpobj.bmHeight,1,3);

		switch (bmpobj.bmBitsPixel) 
		{
		case 16 :
		{ // 16 bits colors
				
				for (int y=height-1; y>=0; --y)
				{ 
						cimg_forX(img,x) 
						{
								const unsigned char c1 = *(ptrs++), c2 = *(ptrs++);
								const unsigned short col = (unsigned short)(c1|(c2<<8));
								(img)(x,y,2) = (ar_byte_t)(col&0x1F);
								(img)(x,y,1) = (ar_byte_t)((col>>5)&0x1F);
								(img)(x,y,0) = (ar_byte_t)((col>>10)&0x1F);
						} 
						ptrs += align; 
				}
		}
				break;
		case 24 : 
		{ // 24 bits colors
				
				for (int y=height-1; y>=0; --y) 
				{ 
						cimg_forX(img,x) 
						{
								(img)(x,y,2) = (ar_byte_t)*(ptrs++);
								(img)(x,y,1) = (ar_byte_t)*(ptrs++);
								(img)(x,y,0) = (ar_byte_t)*(ptrs++);
						}
						
						ptrs+=align; 
				}
		}
				break;
		case 32 : 
		{ // 32 bits colors
				
				for (int y=height-1; y>=0; --y) 
				{ 
						cimg_forX(img,x) 
						{
								(img)(x,y,2) = (ar_byte_t)*(ptrs++);
								(img)(x,y,1) = (ar_byte_t)*(ptrs++);
								(img)(x,y,0) = (ar_byte_t)*(ptrs++);
								++ptrs;
						}
						
						ptrs+=align; 
				}
		} 
				break;
		}
		img.mirror('y');

		delete []lvbit;
		return true;
}



ar_bool_t CaptureAnImageSaveToCImg(HWND hWnd, CByteImage_t &img)
{
		ar_bool_t result;
		HDC hdcWindow;
		HDC hdcMemDC = NULL;
		HBITMAP hbmScreen = NULL;
		BITMAP bmpScreen;

		result = true;
		// Retrieve the handle to a display device context for the client 
		// area of the window. 
		hdcWindow = GetDC(hWnd);

		// Create a compatible DC which is used in a BitBlt from the window DC
		hdcMemDC = CreateCompatibleDC(hdcWindow); 

		if(!hdcMemDC)
		{
				AR_error(AR_ERR_WARNING, L"CreateCompatibleDC has failed",L"Failed");
				result = false;
				goto done;
		}

		// Get the client area for size calculation
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);


		//This is the best stretch mode
		SetStretchBltMode(hdcWindow,HALFTONE);



		// Create a compatible bitmap from the Window DC
		hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top);

		if(!hbmScreen)
		{
				AR_error(AR_ERR_WARNING, L"CreateCompatibleBitmap Failed",L"Failed");
				result = false;
				goto done;
		}

		// Select the compatible bitmap into the compatible memory DC.
		SelectObject(hdcMemDC,hbmScreen);

		// Bit block transfer into our compatible memory DC.
		if(!BitBlt(hdcMemDC, 
				0,0, 
				rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, 
				hdcWindow, 
				0,0,
				SRCCOPY))
		{
				AR_error(AR_ERR_WARNING, L"BitBlt has failed", L"Failed");
				result = false;
				goto done;
		}

		// Get the BITMAP from the HBITMAP
		GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);

		BITMAPFILEHEADER   bmfHeader;    
		BITMAPINFOHEADER   bi;

		bi.biSize = sizeof(BITMAPINFOHEADER);    
		bi.biWidth = bmpScreen.bmWidth;    
		bi.biHeight = bmpScreen.bmHeight;  
		bi.biPlanes = 1;    
		bi.biBitCount = 32;    
		bi.biCompression = BI_RGB;    
		bi.biSizeImage = 0;  
		bi.biXPelsPerMeter = 0;    
		bi.biYPelsPerMeter = 0;    
		bi.biClrUsed = 0;    
		bi.biClrImportant = 0;

		DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

		// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
		// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
		// have greater overhead than HeapAlloc.
		HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
		char *lpbitmap = (char *)GlobalLock(hDIB);    

		// Gets the "bits" from the bitmap and copies them into a buffer 
		// which is pointed to by lpbitmap.
		GetDIBits(hdcWindow, hbmScreen, 0,
				(UINT)bmpScreen.bmHeight,
				lpbitmap,
				(BITMAPINFO *)&bi, DIB_RGB_COLORS);



		if(!load_bmp(hbmScreen, img))
		{
				AR_error(AR_ERR_WARNING, L"failed to convert HBITMAP to CImg Object : %hs\r\n", AR_FUNC_NAME);
				result = false;
				goto done;
		}


		//Clean up
done:
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);

		ReleaseDC(hWnd,hdcWindow);

		return result;
}





