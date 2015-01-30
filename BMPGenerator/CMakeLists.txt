cmake_minimum_required(VERSION 2.6)

project(BMPGenerator)

SET( BMP_SOURSES 
window2bmp.cpp
FindWindow.cpp
)

SET( BMP_HEADERS window2bmp.h BmpAdaptor.h)

add_library(bmplib STATIC ${BMP_SOURSES} ${BMP_HEADERS})

add_executable(bmp_scan main.cpp)

target_link_libraries(bmp_scan bmplib scannerlib)