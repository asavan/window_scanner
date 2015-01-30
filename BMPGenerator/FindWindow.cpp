#include "window2bmp.h"

// #include <windows.h>

#include <sstream>
#include <fstream>
#include <iostream>
#include <ctime>
#include <boost/thread/thread.hpp>

std::string getFilename(const std::string& basename, int i)
{
	std::stringstream str;
	str << basename;
	str << i;
	str << ".bmp";
	return str.str();
}

std::string loadBaseName()
{
	FILE* f;
	f = fopen("config.txt", "r");
	if (!f) 
	{
		throw std::runtime_error("No such file");
	};
	char name[256];
	fscanf(f, "%s", name);
	std::string filename = name;
	fclose(f);
	return filename;
}

int getProcessId()
{
	FILE* f;
	f = fopen("config.txt", "r");
	if (!f) throw std::runtime_error("No config file");
	char name[256];
	fscanf(f, "%s", name);
	std::string filename = name;
	// ifs >> filename;
	int hwndInt;
	// ifs >> hwndInt;
	fscanf(f, "%x", &hwndInt);
	printf("%x\n", hwndInt);
	fclose(f);
	return hwndInt;
}


Sleeper::Sleeper(int sec) : sec_(sec)
{
	start = boost::chrono::high_resolution_clock::now();
}

Sleeper::~Sleeper()
{
	boost::chrono::high_resolution_clock::time_point end = boost::chrono::high_resolution_clock::now();
	typedef boost::chrono::milliseconds ms;
	ms d = boost::chrono::duration_cast<ms>(end - start);
	ms ms_wait = ms(sec_*1000) - d;
	std::cout <<"Time "<< d << std::endl;
	std::cout <<"TimeToWait "<< ms_wait << std::endl;
	
	if (ms_wait > ms::zero())
	{		
		boost::this_thread::sleep_for( ms_wait );
	}
	else 
	{
		std::cout << "Diff is so big " << d << " " << sec_<< std::endl;
	}
}