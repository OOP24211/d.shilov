#pragma once
#include "WordCounter.hpp"

class RecordFile
{
private:
    std::wofstream file;
public:
    RecordFile(char* file_name);

    void record(std::vector<std::pair<std::wstring, WordCounter::frequency>> table);

    ~RecordFile();

};