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
    Table.count(buff.read());
    do 
    {
        Table.count(buff.read());
    }
    while (!buff.isEof());
    CSV_FILE.record(Table.Sort());
    return 0;
}