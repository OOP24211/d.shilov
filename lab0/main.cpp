#include "ReadFile.hpp"
#include "WordCounter.hpp"
#include "RecordFile.hpp"

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    if(argc < 3){
        std::cout << "Error of count arguments";
        return 1;
    }
    ReadFile buff(argv[1]);
    WordCounter Table;
    RecordFile CSV_FILE(argv[2]);
    while (!buff.isEof())
    {
        Table.count(buff.read());
    }
    CSV_FILE.record(Table.sort());
    return 0;
}

