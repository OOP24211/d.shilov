#include <gtest/gtest.h>
#include <string>
#include "../../lib/App.hpp"

struct FixtureTest : public testing::TestWithParam<std::string> {
    std::vector<std::string> argv;
    std::wifstream expected_file;
    App *app;
    void SetUp() {
        argv = {TEST_DATA + GetParam() + "/out_exp.txt", \
        TEST_DATA + GetParam() + "/input.txt", \
        TEST_DATA + GetParam() + "/out.txt"};
        app = new App(argv.size(), argv);
        expected_file.open(argv[0]);
    }
    void TearDown() {
        delete app;
    }
};

TEST_P(FixtureTest, Tests) {
    EXPECT_EQ(EXIT_SUCCESS, app->run());
    std::wstring expected_res{std::istreambuf_iterator<wchar_t>(expected_file),
    std::istreambuf_iterator<wchar_t>()};
    std::wifstream actual_file(argv[2]);
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
