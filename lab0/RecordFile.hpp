#pragma once
#include <utility>
#include <vector>
#include "WordCounter.hpp"


class RecordFile {
 private:
    std::wofstream file_;
 public:
    explicit RecordFile(char* file_name);

    void record(std::vector<std::pair<std::wstring, \
    WordCounter::frequency>> table);

    ~RecordFile();
};

