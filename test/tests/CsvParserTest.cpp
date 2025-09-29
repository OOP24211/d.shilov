#include <gtest/gtest.h>
#include "../../lab0/ReadFile.hpp"
#include "../../lab0/RecordFile.hpp"
#include "../../lab0/WordCounter.hpp"

TEST(CsvParser, Russian_Word) {
    const char *argv[3] = {"test/tests/tested_text/ru_text/input.txt", \
        "test/tests/tested_text/ru_text/out.txt", "test/tests/tested_text/ru_text/out_exp.txt"};
    ReadFile text(argv[0]);
    RecordFile out_text(argv[1]);
    WordCounter table;

    table.count(text.read());
    out_text.record(table.sort());

    std::wifstream expected_file(argv[2]);
    std::wstring expected_res{std::istreambuf_iterator<wchar_t>(expected_file),
    std::istreambuf_iterator<wchar_t>()};
    std::wifstream actual_file(argv[1]);
    std::wstring actual_res{std::istreambuf_iterator<wchar_t>(actual_file),
    std::istreambuf_iterator<wchar_t>()};

    ASSERT_EQ(actual_res, expected_res);
}

TEST(CsvParser, English_Word) {
    const char *argv[3] = {"test/tests/tested_text/eng_text/input.txt", \
        "test/tests/tested_text/eng_text/out.txt", "test/tests/tested_text/eng_text/out_exp.txt"};
    ReadFile text(argv[0]);
    RecordFile out_text(argv[1]);
    WordCounter table;

    table.count(text.read());
    out_text.record(table.sort());

    std::wifstream expected_file(argv[2]);
    std::wstring expected_res{std::istreambuf_iterator<wchar_t>(expected_file),
    std::istreambuf_iterator<wchar_t>()};
    std::wifstream actual_file(argv[1]);
    std::wstring actual_res{std::istreambuf_iterator<wchar_t>(actual_file),
    std::istreambuf_iterator<wchar_t>()};

    ASSERT_EQ(actual_res, expected_res);
}

TEST(CsvParser, Digit) {
    const char *argv[3] = {"test/tests/tested_text/digit_text/input.txt", \
        "test/tests/tested_text/digit_text/out.txt", "test/tests/tested_text/digit_text/out_exp.txt"};
    ReadFile text(argv[0]);
    RecordFile out_text(argv[1]);
    WordCounter table;

    table.count(text.read());
    out_text.record(table.sort());

    std::wifstream expected_file(argv[2]);
    std::wstring expected_res{std::istreambuf_iterator<wchar_t>(expected_file),
    std::istreambuf_iterator<wchar_t>()};
    std::wifstream actual_file(argv[1]);
    std::wstring actual_res{std::istreambuf_iterator<wchar_t>(actual_file),
    std::istreambuf_iterator<wchar_t>()};

    ASSERT_EQ(actual_res, expected_res);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
