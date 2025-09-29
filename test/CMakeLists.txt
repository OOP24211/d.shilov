cmake_minimum_required(VERSION 3.28)

project(test
        VERSION 1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(tests)

target_link_libraries(test
    ${GTEST_LIBRARIES}
    pthread
)