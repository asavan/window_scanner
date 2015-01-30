#include <windows.h>
#include <string>
#include "BmpAdaptor.h"
void SaveWindow2BMP(HWND find, const std::string& filename);
void SaveWindow2BMPRaw(HWND find, BmpAdaptor& adaptor);
std::string getFilename(const std::string& basename, int i);
std::string loadBaseName();
int getProcessId();
void scan_all_windows();
int make_many_pictures();