cmake_minimum_required(VERSION 3.27)

project(BMPGenerator)

SET(BMP_SOURSES 
	window2bmp.cpp
    BmpAdaptor.cpp
    config.cpp
)

SET( BMP_HEADERS window2bmp.h BmpAdaptor.h config.h)

add_library(bmplib STATIC ${BMP_SOURSES} ${BMP_HEADERS})

add_executable(bmp_scan main.cpp)

target_link_libraries(bmp_scan bmplib scannerlib sleeperlib)
