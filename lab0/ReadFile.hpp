#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cwchar>
#define LENGTH 1000

class ReadFile 
{
private:
    std::wifstream file;
    std::vector<std::wstring> buff;
    std::wstring symbol;

    std::vector<std::wstring> realloc_buffer(std::vector<std::wstring>* a);

    void toLower(std::wstring* word);

    void clear_word(std::wstring *word);

public: 
    ReadFile(char* file_name);

    bool isEof();

    std::vector<std::wstring> read();
    
    ~ReadFile();
    
};

