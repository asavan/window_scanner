#include "window2bmp.h"
#include <fstream>
#include <iostream>
#include <thread>

using namespace std::chrono;

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
	start = high_resolution_clock::now();
}

Sleeper::~Sleeper()
{
	high_resolution_clock::time_point end = high_resolution_clock::now();
	auto d = duration_cast<milliseconds>(end - start);
	auto ms_wait = milliseconds(sec_ * 1000) - d;
	std::cout << "Time " << d.count() << std::endl;
	std::cout << "TimeToWait " << ms_wait.count() << std::endl;

	if (ms_wait > milliseconds::zero())
	{
		std::this_thread::sleep_for(ms_wait);
	}
	else
	{
		std::cout << "Diff is so big " << d.count() << " " << sec_ << std::endl;
	}
}
