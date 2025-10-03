#include "../lib/App.hpp"

int main(int argc, char* argv[]) {
    try {
        App app(argc, argv);
        app.run();
    }
    catch(const Error mes) {
        std::cerr << mes.what() << "\n";
        return 1;
    }
    return 0;
}
