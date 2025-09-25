#pragma once
#include "ReadFile.hpp"
#include <map>
#include <iomanip>
#include <algorithm>

class WordCounter 
{
public:
    WordCounter();
 
    struct frequency {
        int count = 0;
        float precent = 0;
    };

private:
    int word_count;
    std::map<std::wstring, frequency> table;
    std::vector<std::pair<std::wstring, frequency>> vec;
    frequency words;

    void Calculate_freq();

public:
    void count(std::vector<std::wstring> text);

    std::vector<std::pair<std::wstring, frequency>> Sort();

};