#pragma once
#include <utility>
#include <vector>
#include <string>
#include "WordCounter.hpp"


class RecordFile {
 private:
    std::wofstream file_;
    int precision_;

 public:
    explicit RecordFile(std::string file_name);

    void record(std::vector<std::pair<std::wstring, \
    WordCounter::frequency>> table);

    ~RecordFile();
};

