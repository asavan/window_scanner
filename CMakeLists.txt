cmake_minimum_required(VERSION 2.6)

project(AutoTranslator)

add_subdirectory(Scanner)
add_subdirectory(Logic)
add_subdirectory(BMPGenerator)

set(SOURCES main.cpp)
set(HEADERS deck.h)


add_executable(AutoTranslator ${SOURCES})

target_link_libraries(AutoTranslator scannerlib fixedlib bmplib)