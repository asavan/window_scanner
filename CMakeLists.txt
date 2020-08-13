cmake_minimum_required(VERSION 3.16)

project(AutoTranslator)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Scanner)
add_subdirectory(Logic)
add_subdirectory(BMPGenerator)

set(SOURCES main.cpp)
set(HEADERS deck.h)


add_executable(AutoTranslator ${SOURCES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AutoTranslator)


target_link_libraries(AutoTranslator scannerlib fixedlib bmplib)
