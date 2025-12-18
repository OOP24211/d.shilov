#include "ConfigParser.h"
#include "WAV_Reader.h"
#include "WAV_Writer.h"
#include "Converter.h"
#include "Factory.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include <cstdio>
#ifndef LAB_3_CONVERT_H
#define LAB_3_CONVERT_H

const std::string TEMP_FILE = "temp.wav";
void print_help();
void convert_c(
    const std::string &config_file,
    const std::string &output_file,
    const std::vector<std::string> &input_files);
int convert(int argc, char *argv[]);
#endif //LAB_3_CONVERT_H