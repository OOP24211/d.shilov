cmake_minimum_required(VERSION 3.28)

project(
        lab0
        VERSION 1.0
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True) 

set(BUILD_SHARED_LIBS True)

set(value 1)


add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(test EXCLUDE_FROM_ALL)

