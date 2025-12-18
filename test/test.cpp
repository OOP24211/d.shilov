#include <gtest/gtest.h>
#include "../src/WAV_Reader.h"
#include "../src/convert.h"
#include <iostream>

TEST(Reader, read1) {
    WAV_Reader read("1.wav");
    for (int i = 0; i < 1000000; i++)
        read.get_sample();
    ASSERT_EQ(read.sample, 7119);
}

TEST(Convert, convert1) {
    char *argv[6] = {"main.cpp", "-c", "conf.txt", "out.wav", "1.wav", "2.wav"};
    convert(6, argv);
    std::ifstream good_res("good_out.wav");
    std::ifstream res("out.wav");
    char out1;
    char out2;
    while (good_res.read(&out1, 1) && res.read(&out2, 1)) {
        ASSERT_EQ(out1, out2);
    }
}
