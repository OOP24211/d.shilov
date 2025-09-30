#include "../lab0/ReadFile.hpp"
#include "../lab0/WordCounter.hpp"
#include "../lab0/RecordFile.hpp"
#include "../lab0/Error.hpp"

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    if (argc < 3) {
        std::cerr << "Error of count arguments";
        return 1;
    }
    try {
        ReadFile buff(argv[1]);
        RecordFile CSV_FILE(argv[2]);
        WordCounter Table;
        while (!buff.isEof()) {
            Table.count(buff.read());
        }
        CSV_FILE.record(Table.sort());
    }
    catch(const Error mes) {
        std::cerr << mes.what() << '\n';
    }
    return 0;
}

