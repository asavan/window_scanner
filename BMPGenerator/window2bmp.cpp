
#include "window2bmp.h"
#include "BmpAdaptor.h"

#include "../Scanner/image.h"

#include <windows.h>
#include <sstream>
#include <iostream>




namespace {

	std::string getFilename(const std::string& prefix, const std::string& basename, int i)
	{
		std::stringstream str;
		str << prefix;
		str << i;
		str << basename;
		str << ".bmp";
		return str.str();
	}


	bool check(LPBYTE Buf, DWORD DataSize)
	{
		if (DataSize == 0) return false;
		BYTE first = Buf[0];
		for (DWORD i = 0; i < DataSize; ++i)
		{
			if (Buf[i] != first)
			{
				return true;
			}
		}
		return false;
	}


	BOOL StoreBitmapFile(LPCTSTR lpszFileName, HBITMAP HBM)
	{
		BITMAP BM = { 0 };
		BITMAPFILEHEADER BFH = { 0 };
		LPBITMAPINFO BIP = { 0 };
		HDC DC;
		LPBYTE Buf;
		DWORD ColorSize, DataSize;
		WORD BitCount;
		HANDLE FP;
		DWORD dwTemp;
		GetObject(HBM, sizeof(BITMAP), (LPSTR)&BM);
		BitCount = (WORD)BM.bmPlanes * BM.bmBitsPixel;
		switch (BitCount) {
		case 1:
		case 4:
		case 8:
		case 32:
			ColorSize = sizeof(RGBQUAD) * (1 << BitCount);
			break;
		case 16:
		case 24:
			ColorSize = 0;
		}

		if (BitCount == 1)
		{
			// std::cout << "No 32" << std::endl;
			return false;
		}


		if (BitCount != 32)
		{
			std::cout << "No 32" << std::endl;
			return false;
		}
		DataSize = ((BM.bmWidth * BitCount + 31) & ~31) / 8 * BM.bmHeight;
		BIP = (LPBITMAPINFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(BITMAPINFOHEADER) + ColorSize);
		BIP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		BIP->bmiHeader.biWidth = BM.bmWidth;
		BIP->bmiHeader.biHeight = BM.bmHeight;
		BIP->bmiHeader.biPlanes = 1;
		BIP->bmiHeader.biBitCount = BitCount;
		BIP->bmiHeader.biCompression = 0;
		BIP->bmiHeader.biSizeImage = DataSize;
		BIP->bmiHeader.biXPelsPerMeter = 0;
		BIP->bmiHeader.biYPelsPerMeter = 0;
		if (BitCount < 16) BIP->bmiHeader.biClrUsed = (1 << BitCount);
		BIP->bmiHeader.biClrImportant = 0;
		BFH.bfType = 0x4d42;
		BFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + BIP->bmiHeader.biClrUsed *
			sizeof(RGBQUAD);
		BFH.bfReserved1 = 0;
		BFH.bfReserved2 = 0;
		BFH.bfSize = BFH.bfOffBits + DataSize;
		Buf = (LPBYTE)GlobalAlloc(GMEM_FIXED, DataSize);
		DC = GetDC(0);
		GetDIBits(DC, HBM, 0, (WORD)BM.bmHeight, Buf, BIP, DIB_RGB_COLORS);
		ReleaseDC(0, DC);
		if (check(Buf, DataSize))
		{
			FP = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			WriteFile(FP, &BFH, sizeof(BITMAPFILEHEADER), &dwTemp, NULL);
			WriteFile(FP, BIP, sizeof(BITMAPINFOHEADER) + BIP->bmiHeader.biClrUsed *
				sizeof(RGBQUAD), &dwTemp, NULL);
			WriteFile(FP, Buf, DataSize, &dwTemp, NULL);
			CloseHandle(FP);
		}
		GlobalFree((HGLOBAL)Buf);
		HeapFree(GetProcessHeap(), 0, (LPVOID)BIP);
		return(true);
	}


	static bool StoreBitmapToAdaptor(BmpAdaptor& adaptor, HBITMAP HBM)
	{
		BITMAP BM = { 0 };
		GetObject(HBM, sizeof(BITMAP), (LPSTR)&BM);
		WORD BitCount = BM.bmPlanes * BM.bmBitsPixel;
		DWORD DataSize = ((BM.bmWidth * BitCount + 31) & ~31) / 8 * BM.bmHeight;
		if (BitCount != 32)
		{
			std::cout << BitCount << std::endl;
			std::cout << DataSize << std::endl;
			return false;
		}
		BITMAPINFO BIP = { 0 };
		BIP.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		BIP.bmiHeader.biWidth = BM.bmWidth;
		BIP.bmiHeader.biHeight = BM.bmHeight;
		BIP.bmiHeader.biPlanes = 1;
		BIP.bmiHeader.biBitCount = BitCount;
		BIP.bmiHeader.biCompression = 0;
		BIP.bmiHeader.biSizeImage = DataSize;
		BIP.bmiHeader.biXPelsPerMeter = 0;
		BIP.bmiHeader.biYPelsPerMeter = 0;
		// if (BitCount < 16) BIP.bmiHeader.biClrUsed = (1<<BitCount);
		BIP.bmiHeader.biClrImportant = 0;
		// Buf = (LPBYTE)GlobalAlloc(GMEM_FIXED, DataSize);
		HDC DC = GetDC(0);
		unsigned char* Buf = new unsigned char[DataSize];
		int res = GetDIBits(DC, HBM, 0, (WORD)BM.bmHeight, Buf, &BIP, DIB_RGB_COLORS);
		// std::cout << "Res "<<res << std::endl;
		ReleaseDC(0, DC);
		adaptor.setBitCountWidthHeightBuf(BitCount, BM.bmWidth, BM.bmHeight, Buf);
		return true;
	}


	HBITMAP CreateBitmap(HDC DC, int w, int h)
	{
		HBITMAP bm, oldBM;
		HDC memDC = CreateCompatibleDC(DC);
		bm = CreateCompatibleBitmap(DC, w, h);
		oldBM = (HBITMAP)SelectObject(memDC, bm);
		BitBlt(memDC, 0, 0, w, h, DC, 0, 0, SRCCOPY);
		DeleteDC(memDC);
		DeleteObject(oldBM);
		return bm;
	}
	/*
	HBITMAP CreateWindwowBitmap(HWND wnd)
	{
	RECT r;
	POINT pt;
	GetWindowRect(wnd,&r);
	pt.x=r.right;
	pt.y=r.bottom;
	ScreenToClient(wnd,&pt);
	return(CreateBitmap(GetWindowDC(wnd),pt.x,pt.y));
	}
	*/

	HBITMAP CreateClientWindwowBitmap(HWND wnd)
	{
		RECT r;
		BOOL res = GetClientRect(wnd, &r);
		if (!res)
		{
			return NULL;
		}
		HDC DC = GetDC(wnd);
		HBITMAP hbm = CreateBitmap(DC, r.right, r.bottom);
		ReleaseDC(wnd, DC);
		return hbm;
	}

	static void SaveWindow2BMPRaw(int find, BmpAdaptor& adaptor)
	{
		if (find == 0) return;
		HBITMAP bmp = CreateClientWindwowBitmap((HWND)find);
		if (bmp == 0)
		{
			std::cout << "No window " << find << std::endl;
		}
		else
		{
			StoreBitmapToAdaptor(adaptor, bmp);
			DeleteObject(bmp);
		}
	}


	std::string getWinName(HWND find)
	{
		char wname[256];
		GetWindowText(find, wname, 256);
		std::string winname = wname;
		return winname;
	}

	void SaveWindow2BMP(HWND find, const std::string& filename)
	{
		if (find == 0) return;
		std::string winname = getWinName(find);
		if (winname.empty())
		{
			std::cout << "No title " << find << std::endl;
			return;
		}
		HBITMAP bmp = CreateClientWindwowBitmap(find);
		if (bmp == 0)
		{
			std::cout << "No window " << find << std::endl;
			return;
		}
		else
		{
			bool res = StoreBitmapFile(filename.c_str(), bmp);
			DeleteObject(bmp);
			if (!res)
			{
				std::cout << "window minimized " << winname << std::endl;
			}
		}

	}

}

BmpAdaptor::BmpAdaptor() : isValid_(false), bit_count_(0), width_(0), height_(0), Buf_(0), bmp(0)
{
};

BmpAdaptor::~BmpAdaptor()
{
	if (isValid_)
	{
		// GlobalFree((HGLOBAL)Buf_);
		delete[] Buf_;
		delete bmp;
	}
	else
	{
		printf("Not delete this %p\n", Buf_);
	}
}


Image BmpAdaptor::getImage() const
{
	return Image::getImageFromBmp(getBmp());
}

void BmpAdaptor::setBitCountWidthHeightBuf(unsigned int bit_count, unsigned int width, unsigned int height, unsigned char* Buf)
{
	bit_count_ = bit_count;
	width_ = width;
	height_ = height;
	setBuf(Buf);
}

void BmpAdaptor::setBuf(unsigned char* Buf)
{
	Buf_ = Buf;
	isValid_ = true;
	bmp = new BMP(reinterpret_cast<const unsigned char*>(Buf_), bit_count_, width_, height_);
}

const BMP& BmpAdaptor::getBmp() const
{
	if (!isValid_)
	{
		throw std::runtime_error("Invalid adapter");
	}

	return *bmp;
}

bool BmpAdaptor::captureWindow(int hwndId)
{
	SaveWindow2BMPRaw(hwndId, *this);
	return isValid();
}



void scan_all_windows()
{
	Sleeper s(0);
	int i = 0;	
	for ( HWND find = FindWindow(NULL, NULL); find; ++i)
	{
		std::string winname = getWinName(find);
		SaveWindow2BMP(find, getFilename("tmp3/", winname, i));
		find = FindWindowEx(NULL, find, NULL, NULL);		
	}
}

int make_many_pictures()
{
	for (int i = 0; i < 1000; ++i)
	{
		Sleeper sl(1);		
		int hwndInt = getProcessId();
		std::string baseName = loadBaseName();
		SaveWindow2BMP(HWND(hwndInt), getFilename(baseName, "", i));
	}
	return 0;
}

