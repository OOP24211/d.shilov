cmake_minimum_required(VERSION 3.16)
project(Snake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "D:\\proga\\oop\\Snake_Game\\SFML\\lib\\cmake\\SFML")

find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

add_subdirectory(src)
add_subdirectory(lib)
