#include <windows.h>
#include <string>
#include "BmpAdaptor.h"
void SaveWindow2BMP(HWND find, const std::string& filename);
void SaveWindow2BMPRaw(HWND find, BmpAdaptor& adaptor);