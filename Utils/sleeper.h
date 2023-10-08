#ifndef SLEEPER_H
#define SLEEPER_H
#include <chrono>

class Sleeper
{
public:
    explicit Sleeper(std::chrono::milliseconds sec);
    ~Sleeper();
private:
    Sleeper(const Sleeper&) = delete;
    Sleeper& operator=(const Sleeper&) = delete;

    std::chrono::milliseconds sec_;
    std::chrono::steady_clock::time_point start;
};

#endif
