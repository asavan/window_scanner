cmake_minimum_required(VERSION 2.6)
project(FIXEDLIMIT)

set(SOURCES_FIXED preflop.cpp)
SET( FIXED_HEADERS preflop.h)
add_library(fixedlib STATIC ${SOURCES_FIXED} ${FIXED_HEADERS})
