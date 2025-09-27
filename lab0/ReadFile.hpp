#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cwchar>
#include <string>
#include <locale>
#define LENGTH 1000

class ReadFile {
 private:
    std::wifstream file_;
    std::vector<std::wstring> buff_;
    std::wstring symbol_;

    void clearBuffer();

    void toLower(std::wstring &word);

    void clearWord(std::wstring &word);

 public:
    explicit ReadFile(std::string file_name);

    bool isEof();

    std::vector<std::wstring> read();

    ~ReadFile();
};

