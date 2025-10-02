add_executable(test
        CsvParserTest.cpp
)

target_link_libraries(test PRIVATE lib)

add_compile_definitions(TEST_DATA="${CMAKE_CURRENT_SOURCE_DIR}/tested_text/")
