#include "ReadFile.hpp"

void ReadFile::clearBuffer() {
    buff_.clear();
}

void ReadFile::toLower(std::wstring &word) {
    std::wstring new_word;
    for (auto c : word) {
        new_word += towlower(c);
    }
    word = new_word;
}

void ReadFile::clearWord(std::wstring &word) {
    std::wstring new_word;
    for (auto c : word) {
        if (!iswpunct(c)) {
            new_word += c;
        }
    }
    toLower(new_word);
    word = new_word;
}

ReadFile::ReadFile(char* file_name) {
    file_.open(file_name);
}

bool ReadFile::isEof() {
    return file_.eof();
}

std::vector<std::wstring> ReadFile::read() {
        clearBuffer();
        int i = 0;
        while (file_ >> symbol_ && i != LENGTH) {
            clearWord(symbol_);
            buff_.push_back(symbol_);
            i++;
        }
        return buff_;
    }

ReadFile::~ReadFile() {
    file_.close();
}

