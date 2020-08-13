#ifndef WINDOW_2_BMP_H
#define WINDOW_2_BMP_H
#include <string>
#include <chrono>

std::string loadBaseName();
int getProcessId();
void scan_all_windows();
int make_many_pictures();

class Sleeper
{
public:
    explicit Sleeper(int sec);
    ~Sleeper();
private:
    Sleeper(const Sleeper&);
    Sleeper& operator=(const Sleeper&);

    int sec_;
    std::chrono::high_resolution_clock::time_point start;
};

#endif
