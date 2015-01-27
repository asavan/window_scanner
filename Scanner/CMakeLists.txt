cmake_minimum_required(VERSION 2.6)

project(AutoTranslatorScanner)

set(SCANNER_SOURCES color.cpp image.cpp storage.cpp)
set(SCANNER_HEADERS color.h image.h storage.h pattern.h pattern.tpl)

set (Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS system filesystem ${BOOST_COMPONENTS} REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_library(scannerlib STATIC ${SCANNER_SOURCES} ${SCANNER_HEADERS})
add_executable(AutoTranslatorScanner autotrans.cpp)


target_link_libraries(scannerlib ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
# ${Boost_LIBRARIES}
target_link_libraries(AutoTranslatorScanner scannerlib)
