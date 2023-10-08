
#include "window2bmp.h"
#include "config.h"

#include "../Utils/sleeper.h"

#include <windows.h>

#include <string>
#include <iostream>

using namespace std::chrono_literals;


namespace {
    std::string getWinName(HWND find)
    {
        char wname[512];
        GetWindowText(find, wname, 512);
        std::string winname = wname;
        return winname;
    }

    std::string getFilename(const std::string& prefix, const std::string&, int i)
    {
        return prefix + std::to_string(i) + ".bmp";
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

    bool StoreBitmapFile(LPCTSTR lpszFileName, HBITMAP HBM)
    {
        BITMAP BM = { 0 };
        BITMAPFILEHEADER BFH = { 0 };
        LPBITMAPINFO BIP = { 0 };
        HDC DC;
        LPBYTE Buf;
        DWORD ColorSize = 0, DataSize = 0;
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
            ColorSize = sizeof(RGBQUAD) * (1llu << BitCount);
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
        if (!BIP) {
            return false;
        }
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
        bool result = check(Buf, DataSize);
        if (result)
        {
            FP = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            BOOL r = WriteFile(FP, &BFH, sizeof(BITMAPFILEHEADER), &dwTemp, NULL);
            if (!r) {
                result = false;
                std::cout << lpszFileName << " failed" << std::endl;
            }
            WriteFile(FP, BIP, sizeof(BITMAPINFOHEADER) + BIP->bmiHeader.biClrUsed *
                sizeof(RGBQUAD), &dwTemp, NULL);
            WriteFile(FP, Buf, DataSize, &dwTemp, NULL);
            CloseHandle(FP);
        }
        GlobalFree((HGLOBAL)Buf);
        HeapFree(GetProcessHeap(), 0, (LPVOID)BIP);
        return result;
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
        HDC DC = GetDC(wnd);
        HBITMAP hbm = CreateBitmap(DC, r.right * 2, r.bottom * 2);
        ReleaseDC(wnd, DC);
        return hbm;
    }


    bool SaveWindow2BMP(HWND find, const std::string& filename)
    {
        if (find == 0) {
            return false;
        }
        std::string winname = getWinName(find);
        if (winname.empty())
        {
            // std::cout << "No title " << find << std::endl;
            return false;
        }
        HBITMAP bmp = CreateClientWindwowBitmap(find);
        if (bmp == 0)
        {
            // std::cout << "No window " << find << std::endl;
            return false;
        }
        else
        {
            bool res = StoreBitmapFile(filename.c_str(), bmp);
            DeleteObject(bmp);
            if (!res)
            {
                // std::cout << "window minimized " << winname << std::endl;
            }
            return res;
        }
    }
}

void scan_all_windows()
{
    Sleeper s(0s);
    int i = 0;
    int count = 0;
    for (HWND find = FindWindow(NULL, NULL); find; ++i)
    {
        std::string winname = getWinName(find);
        bool res = SaveWindow2BMP(find, getFilename("tmp3/", winname, count));
        if (res) {
            ++count;
        }
        find = FindWindowEx(NULL, find, NULL, NULL);
    }
    std::cout << "All " << i << " good " << count << std::endl;
}

int make_many_pictures()
{
    ConfigParser config("config.txt");
    
    for (int i = 0; i < 1000; ++i)
    {
        Sleeper sl(1s);
        SaveWindow2BMP(reinterpret_cast<HWND>(config.processId()), getFilename(config.baseName(), "", i));
    }
    return 0;
}
