#pragma once
#include <map>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <utility>
#include "lab0/ReadFile.hpp"

class WordCounter {
 public:
    WordCounter();

    struct frequency {
        int count = 0;
        float precent = 0;
    };

 private:
    int wordCount_;
    std::map<std::wstring, frequency> table_;
    std::vector<std::pair<std::wstring, frequency>> vec_;
    frequency words_;

    void calculateFreq();

 public:
    void count(std::vector<std::wstring> text);

    std::vector<std::pair<std::wstring, frequency>> sort();
};

