#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cwchar>
#define LENGTH 1000

class ReadFile {
 private:
    std::wifstream file_;
    std::vector<std::wstring> buff_;
    std::wstring symbol_;

    std::vector<std::wstring> reallocBuffer(std::vector<std::wstring>* a);

    void toLower(std::wstring* word);

    void clearWord(std::wstring *word);

 public:
    explicit ReadFile(char* file_name);

    bool isEof();

    std::vector<std::wstring> read();

    ~ReadFile();
};

