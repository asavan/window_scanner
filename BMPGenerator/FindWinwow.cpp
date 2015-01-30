#include "window2bmp.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <ctime>

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

void scan_all_windows()
{
	int i = 0;	
	for ( HWND find2 = FindWindow(NULL, NULL); find2; ++i)
	{
		SaveWindow2BMP(find2, getFilename("tmp3/", i));
		find2 = FindWindowEx(NULL, find2, NULL, NULL);		
	}
}



class Sleeper
{
public:
	Sleeper(int sec) : sec_(sec)
	{
		t_ = time(0);
	}
	~Sleeper()
	{
		time_t finish = time(0);
		time_t diff = finish - t_;
		if (sec_ > diff)
		{
			Sleep(1000*(sec_-diff));
		}
		else 
		{
			std::cout << "Diff is so big " << diff << " " << sec_<< std::endl;
		}
	}
private:
	int sec_;
	time_t t_;
};



int make_many_pictures()
{
	for (int i = 0; i < 2000; ++i)
	{
		Sleeper sl(1);		
		int hwndInt = getProcessId();
		std::string baseName = loadBaseName();
		SaveWindow2BMP(HWND(hwndInt), getFilename(baseName, i));
	}
	return 0;
}






