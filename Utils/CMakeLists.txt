cmake_minimum_required(VERSION 3.27)

project(Sleeper)

set(SLEEPER_SOURCES sleeper.cpp)
set(SLEEPER_HEADERS sleeper.h)

add_library(sleeperlib STATIC ${SLEEPER_SOURCES} ${SLEEPER_HEADERS})
