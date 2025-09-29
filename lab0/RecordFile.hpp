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
    explicit RecordFile(std::string file_name, int precision_ = 4);

    void record(std::vector<std::pair<std::wstring, \
    WordCounter::frequency>> table);

    ~RecordFile();
};
