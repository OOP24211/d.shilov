#include "../lib/App.hpp"

App::App(int argc, char* argv[]) {
    argv1_ = argv[1];
    argv2_ = argv[2];
    argc_ = argc;
}

int App::run() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    if (argc_ < 3) {
        std::cerr << "Error of count arguments";
        return 1;
    }
    try {
        ReadFile buff(argv1_);
        RecordFile CSV_FILE(argv2_);
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
