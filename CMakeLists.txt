cmake_minimum_required(VERSION 3.2)
project(Riptide)

# set default build directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)

# TODO: compiler optimisations (for C)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fPIC -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wno-varargs -fPIC")

include_directories(
    ${INCLUDE_DIRECTORIES}
    src
    # for testing
    ${CMAKE_BINARY_DIR}/../../ArcaneCore/ArcaneCore/src/cpp
)
link_directories(
    ${LINK_DIRECTORIES}
    # for testing
    ${CMAKE_BINARY_DIR}/../../ArcaneCore/ArcaneCore/build/linux_x86
)

# main source files
set(FFF_SRC
    src/fff/fff.c
)

# test source files
set(FFF_TEST_SRC
    tests/TestsMain.cpp
    tests/fff_TestSuite.cpp
)

# build the library
add_library(fff STATIC ${FFF_SRC})

# build the tests
add_executable(tests ${FFF_TEST_SRC})

# link libraries to the tests
target_link_libraries(tests
    fff
    arcanecore_test
    arcanecore_io
    arcanecore_base
)
