#include "BmpAdaptor.h"
#include "../Scanner/image.h"

#include <string>
#include <iostream>


#include <windows.h>

namespace {

    constexpr int scale = 2;

    bool StoreBitmapToAdaptor(BmpAdaptor& adaptor, HBITMAP HBM)
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
        std::cout << "Res " << res << std::endl;
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

    HBITMAP CreateClientWindwowBitmap(HWND wnd)
    {
        RECT r;
        BOOL res = GetClientRect(wnd, &r);
        if (!res)
        {
            return NULL;
        }
        // DEVICE_SCALE_FACTOR dcf;
        // HMONITOR monitor = MonitorFromWindow(wnd, MONITOR_DEFAULTTONEAREST);
        // https://stackoverflow.com/questions/33507031/detect-if-non-dpi-aware-application-has-been-scaled-virtualized
        //HRESULT scRes = GetScaleFactorForMonitor(
        //    monitor,
        //    &dcf
        //);
        // std::cout <<"Scale "<< dcf << std::endl;
        HDC DC = GetDC(wnd);
        HBITMAP hbm = CreateBitmap(DC, r.right * scale, r.bottom * scale);
        ReleaseDC(wnd, DC);
        return hbm;
    }

    static void SaveWindow2BMPRaw(unsigned long long find, BmpAdaptor& adaptor)
    {
        if (find == 0) return;
        HBITMAP bmp = CreateClientWindwowBitmap(reinterpret_cast<HWND>(find));
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

bool BmpAdaptor::captureWindow(unsigned long long hwndId)
{
    SaveWindow2BMPRaw(hwndId, *this);
    return isValid();
}
