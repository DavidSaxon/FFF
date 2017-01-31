cmake_minimum_required(VERSION 3.2)
project(FFF)

# set default build directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)

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

# test source files
set(FFF_TEST_SRC
    tests/TestsMain.cpp
    tests/fff_TestSuite.cpp
)

# build the tests
add_executable(tests ${FFF_TEST_SRC})

# link libraries to the tests
target_link_libraries(tests
    arcanecore_test
    arcanecore_io
    arcanecore_base
)
