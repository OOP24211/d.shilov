find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(tests)

target_link_libraries(test PUBLIC
    ${GTEST_LIBRARIES}
    pthread
)

add_compile_definitions(TEST_DATA="${CMAKE_CURRENT_SOURCE_DIR}/tested_text/")
