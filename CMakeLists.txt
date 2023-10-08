cmake_minimum_required(VERSION 3.27)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DISABLE_PRECOMPILE_HEADERS ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()


add_subdirectory(Scanner)
add_subdirectory(Logic)
add_subdirectory(BMPGenerator)
add_subdirectory(Utils)

set(SOURCES main.cpp)
set(HEADERS deck.h)


add_executable(${ProjectId} ${SOURCES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${ProjectId})


target_link_libraries(${ProjectId} scannerlib fixedlib bmplib sleeperlib)
