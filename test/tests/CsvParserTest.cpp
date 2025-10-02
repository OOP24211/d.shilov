#include <gtest/gtest.h>
#include <string>
#include "../../lib/ReadFile.hpp"
#include "../../lib/RecordFile.hpp"
#include "../../lib/WordCounter.hpp"

struct FixtureTest : public testing::TestWithParam<std::string> {
    std::vector<std::string> argv;
    ReadFile *text;
    RecordFile *out_text;
    WordCounter table;
    std::wifstream expected_file;
    void SetUp() {
        argv = {TEST_DATA + GetParam() + "/input.txt", \
        TEST_DATA + GetParam() + "/out.txt", \
        TEST_DATA + GetParam() + "/out_exp.txt"};
        text = new ReadFile(argv[0]);
        out_text = new RecordFile(argv[1]);
        expected_file.open(argv[2]);
    }
    void TearDown() {
        delete text;
        delete out_text;
    }
};

TEST_P(FixtureTest, Tests) {
    table.count(text->read());
    out_text->record(table.sort());

    std::wstring expected_res{std::istreambuf_iterator<wchar_t>(expected_file),
    std::istreambuf_iterator<wchar_t>()};
    std::wifstream actual_file(argv[1]);
    std::wstring actual_res{std::istreambuf_iterator<wchar_t>(actual_file),
    std::istreambuf_iterator<wchar_t>()};

    ASSERT_EQ(actual_res, expected_res);
}

INSTANTIATE_TEST_SUITE_P(
    AllCases,
    FixtureTest,
    testing::Values("digit_text", "ru_text", "eng_text")
);

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
