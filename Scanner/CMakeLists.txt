cmake_minimum_required(VERSION 3.27)

project(AutoTranslatorScanner)

set(SCANNER_SOURCES color.cpp image.cpp storage.cpp)
set(SCANNER_HEADERS color.h image.h storage.h pattern.h pattern.tcc)

add_library(scannerlib STATIC ${SCANNER_SOURCES} ${SCANNER_HEADERS})
add_executable(AutoTranslatorScanner autotrans.cpp)

target_link_libraries(AutoTranslatorScanner scannerlib)
