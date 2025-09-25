#include "RecordFile.hpp"

RecordFile::RecordFile(char* file_name) {
    this->file.open(file_name);
}

void RecordFile::record(std::vector<std::pair<std::wstring, typename WordCounter::frequency>> table) {
        file << "word" << ";" << "count" << ";" << "frequency" << std::endl;
    for(auto i = 0; i < table.size(); i++) {
        file << std::setprecision(4) << table[i].first << ";" << table[i].second.count << ";" << table[i].second.precent << std::endl;
    }   
}

RecordFile::~RecordFile() {
    this->file.close();
}

