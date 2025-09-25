#include "ReadFile.hpp"

std::vector<std::wstring> ReadFile::realloc_buffer(std::vector<std::wstring>* a) {
    a->clear();
    return *a;
}

void ReadFile::toLower(std::wstring* word) {
    std::wstring new_word;
    for (auto c : *word) {
        new_word += towlower(c);
    }
    *word = new_word;
}

void ReadFile::clear_word(std::wstring *word){
    std::wstring new_word;
    for (auto c : *word) {
        if(!iswpunct(c)) {
            new_word += c;
        }
    }
    ReadFile::toLower(&new_word);
    *word = new_word;
}

ReadFile::ReadFile(char* file_name) {
    this->file.open(file_name);
}

bool ReadFile::isEof() {
    if (this->file.eof()) {
        return true;
    }
    return false;
}

std::vector<std::wstring> ReadFile::read() {
        realloc_buffer(&buff);
        int i = 0;
        while (this->file >> this->symbol && i != LENGTH) {
            clear_word(&this->symbol);
            this->buff.push_back(this->symbol);
            i++;
        }
        return this->buff;
    }

ReadFile::~ReadFile() {
    this->file.close();
}

