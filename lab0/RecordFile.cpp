#include "RecordFile.hpp"

RecordFile::RecordFile(char* file_name) : precision_(4) {
    file_.open(file_name);
    if (!file_.is_open()) {
        std::cerr << "Exit code" << file_.rdstate();
    }
}

void RecordFile::record(std::vector<std::pair<std::wstring, \
typename WordCounter::frequency>> table) {
        file_ << "word" << ";" << "count" << ";" << "frequency" << std::endl;
    for (auto i = 0; i < table.size(); i++) {
        file_ << std::setprecision(precision_) << table[i].first << ";"
        << table[i].second.count << ";" << table[i].second.precent << std::endl;
    }
}

RecordFile::~RecordFile() {
    file_.close();
}

