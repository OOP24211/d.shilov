#include "../lib/App.hpp"

App::App(int argc, char* argv[]) {
    argc_ = argc;
    if (argc_ != 3) {
        throw Error("Error of count arguments");
    } else {
        argv1_ = argv[1];
        argv2_ = argv[2];
    }
}

void App::run() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    ReadFile buff(argv1_);
    RecordFile CSV_FILE(argv2_);
    WordCounter Table;
    while (!buff.isEof()) {
        Table.count(buff.read());
    }
    CSV_FILE.record(Table.sort());
}
