#ifndef WINDOW_2_BMP_H
#define WINDOW_2_BMP_H
#include <string>
#include "BmpAdaptor.h"

// void SaveWindow2BMPRaw(HWND find, BmpAdaptor& adaptor);
std::string getFilename(const std::string& basename, int i);
std::string loadBaseName();
int getProcessId();
void scan_all_windows();
int make_many_pictures();

#include <boost/chrono.hpp> 

class Sleeper
{
public:	
	explicit Sleeper(int sec);
	~Sleeper();
private:
	Sleeper(const Sleeper&);
	Sleeper& operator=(const Sleeper&);

	int sec_;
	boost::chrono::high_resolution_clock::time_point start;
};

#endif
