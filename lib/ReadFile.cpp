#include "../lib/ReadFile.hpp"
#include "../lib/Error.hpp"

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

ReadFile::ReadFile(std::string file_name) {
    file_.open(file_name);
    if (!file_.is_open()) {
        throw Error("ERROR: Input file "  + file_name + " can't be open");
    }
    file_.imbue(std::locale("ru_RU.utf8"));
}

bool ReadFile::isEof() {
    return file_.eof();
}

std::vector<std::wstring> ReadFile::read() {
        clearBuffer();
        int i = 0;
        while (i != LENGTH && file_ >> symbol_) {
            clearWord(symbol_);
            buff_.push_back(symbol_);
            i++;
        }
        return buff_;
    }

ReadFile::~ReadFile() {
    file_.close();
}

