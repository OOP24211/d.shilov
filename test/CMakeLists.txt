add_executable(test
        tests.cpp
)

target_link_libraries(test PRIVATE lib)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(test PUBLIC
    ${GTEST_LIBRARIES}
    pthread
)

