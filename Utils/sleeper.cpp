#include "sleeper.h"
#include <thread>
#include <iostream>

using namespace std::chrono;


Sleeper::Sleeper(std::chrono::milliseconds sec) : sec_(sec), start(steady_clock::now())
{
}

Sleeper::~Sleeper()
{
	steady_clock::time_point end = steady_clock::now();
	auto d = end - start;
	auto ms_wait = sec_ - d;
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
