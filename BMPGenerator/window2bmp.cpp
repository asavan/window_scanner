#include "window2bmp.h"
#include "BmpAdaptor.h"

BmpAdaptor::BmpAdaptor() : isValid_(false), bit_count_(0), width_(0), height_(0), Buf_(0)
{
};

BmpAdaptor::~BmpAdaptor() 
{
	if(isValid_) 
	{
		// GlobalFree((HGLOBAL)Buf_);
		delete [] Buf_;
	}
	else 
	{
		std::cout << "Not delete this " << Buf_ << std::endl;
	}
}

void BmpAdaptor::setBuf(unsigned char* Buf) 
{
	Buf_ = Buf;
	isValid_ = true;
}

BMP BmpAdaptor::getBmp() const
{
	if (!isValid_) 
	{
		throw std::runtime_error("Invalid adapter");
	}
	BMP bmp(reinterpret_cast<const unsigned char*>(Buf_), bit_count_, width_, height_);
	return bmp;
}

static bool check(LPBYTE Buf, DWORD DataSize)
{
	if (DataSize == 0) return false;
	BYTE first = Buf[0];
	for (DWORD i = 0; i < DataSize; ++i) 
	{
		if(Buf[i] != first) 
		{
			return true;
		}
	}
	return false;
}


BOOL StoreBitmapFile(LPCTSTR lpszFileName, HBITMAP HBM)
{
	BITMAP BM = {0}; 
	BITMAPFILEHEADER BFH = {0}; 
	LPBITMAPINFO BIP = {0}; 
HDC DC; 
LPBYTE Buf; 
DWORD ColorSize,DataSize; 
WORD BitCount; 
HANDLE FP; 
DWORD dwTemp;
GetObject(HBM, sizeof(BITMAP), (LPSTR)&BM);
BitCount = (WORD)BM.bmPlanes * BM.bmBitsPixel;
switch (BitCount){
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
DataSize = ((BM.bmWidth*BitCount+31) & ~31)/8*BM.bmHeight;
BIP=(LPBITMAPINFO)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,
sizeof(BITMAPINFOHEADER)+ColorSize);
BIP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
BIP->bmiHeader.biWidth = BM.bmWidth;
BIP->bmiHeader.biHeight = BM.bmHeight;
BIP->bmiHeader.biPlanes = 1;
BIP->bmiHeader.biBitCount = BitCount;
BIP->bmiHeader.biCompression = 0;
BIP->bmiHeader.biSizeImage = DataSize;
BIP->bmiHeader.biXPelsPerMeter = 0;
BIP->bmiHeader.biYPelsPerMeter = 0;
if (BitCount < 16) BIP->bmiHeader.biClrUsed = (1<<BitCount);
BIP->bmiHeader.biClrImportant = 0;
BFH.bfType = 0x4d42; 
BFH.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+ BIP->bmiHeader.biClrUsed * 
sizeof(RGBQUAD);
BFH.bfReserved1 = 0;
BFH.bfReserved2 = 0;
BFH.bfSize = BFH.bfOffBits + DataSize; 
Buf = (LPBYTE)GlobalAlloc(GMEM_FIXED, DataSize);
DC = GetDC(0); 
GetDIBits(DC, HBM, 0,(WORD)BM.bmHeight, Buf, BIP, DIB_RGB_COLORS); 
ReleaseDC(0, DC);
if (check(Buf, DataSize))
{
FP=CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE, 0, NULL,
CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
WriteFile(FP,&BFH,sizeof(BITMAPFILEHEADER),&dwTemp,NULL); 
WriteFile(FP,BIP,sizeof(BITMAPINFOHEADER) + BIP->bmiHeader.biClrUsed *
sizeof(RGBQUAD),&dwTemp,NULL); 
WriteFile(FP,Buf,DataSize,&dwTemp,NULL);
CloseHandle(FP); 
}
GlobalFree((HGLOBAL)Buf);
HeapFree(GetProcessHeap(),0,(LPVOID)BIP); 
return(true);
}


bool StoreBitmapToAdaptor(BmpAdaptor& adaptor, HBITMAP HBM)
{
	BITMAP BM = {0}; 
	GetObject(HBM, sizeof(BITMAP), (LPSTR)&BM);
	WORD BitCount = (WORD)BM.bmPlanes * BM.bmBitsPixel;
	std::cout << BitCount << std::endl;
	DWORD DataSize = ((BM.bmWidth*BitCount+31) & ~31)/8*BM.bmHeight;
	std::cout << DataSize << std::endl;
	BITMAPINFO BIP = {0};
	BIP.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BIP.bmiHeader.biWidth = BM.bmWidth;
	BIP.bmiHeader.biHeight = BM.bmHeight;
	BIP.bmiHeader.biPlanes = 1;
	BIP.bmiHeader.biBitCount = BitCount;
	BIP.bmiHeader.biCompression = 0;
	BIP.bmiHeader.biSizeImage = DataSize;
	BIP.bmiHeader.biXPelsPerMeter = 0;
	BIP.bmiHeader.biYPelsPerMeter = 0;
	if (BitCount < 16) BIP.bmiHeader.biClrUsed = (1<<BitCount);
	BIP.bmiHeader.biClrImportant = 0;
	// Buf = (LPBYTE)GlobalAlloc(GMEM_FIXED, DataSize);
	HDC DC = GetDC(0); 
unsigned char* Buf = new unsigned char[DataSize];
GetDIBits(DC, HBM, 0,(WORD)BM.bmHeight, Buf, &BIP, DIB_RGB_COLORS); 
ReleaseDC(0, DC);
{
	adaptor.setWidth(BM.bmWidth);
	adaptor.setBitCount(BitCount);
	adaptor.setHeight(BM.bmHeight);
	adaptor.setBuf(Buf);
} 
DeleteObject(&BM);
return true;
}


HBITMAP CreateBitmap(HDC DC, int w, int h)
{
HBITMAP bm,oldBM;
HDC memDC=CreateCompatibleDC(DC);
bm=CreateCompatibleBitmap(DC,w,h);
oldBM=(HBITMAP)SelectObject(memDC,bm);
BitBlt(memDC, 0,0, w, h, DC, 0,0, SRCCOPY);
SelectObject(memDC,oldBM);
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
	BOOL res = GetClientRect(wnd,&r);
	if (!res) 
	{
		return NULL;
	}
	return(CreateBitmap(GetDC(wnd),r.right,r.bottom));
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
		StoreBitmapFile(filename.c_str(), bmp);
		DeleteObject(bmp);
	}
	
}


void SaveWindow2BMPRaw(HWND find, BmpAdaptor& adaptor)
{
	if (find == 0) return;
    HBITMAP bmp = CreateClientWindwowBitmap(find);
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
