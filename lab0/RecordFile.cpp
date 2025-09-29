#include "RecordFile.hpp"
#include "Error.hpp"

RecordFile::RecordFile(std::string file_name, int precision) {
    file_.open(file_name);
    if (!file_.is_open()) {
        throw Error("ERROR: Output file "  + file_name + " can't be open\n");
    }
    file_.imbue(std::locale("ru_RU.utf8"));
    precision_ = precision;
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

