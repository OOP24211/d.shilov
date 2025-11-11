#include <iostream>
#include "../lib/ICache.hpp"
#include "../lib/LRUCache.hpp"
#include "../lib/LFUCache.hpp"
#include "../lib/CacheApp.hpp"

int main() {
    std::cout << "Выберите тип кэша:\n1 - LRU\n2 - LFU\n> ";
    int choice;
    std::cin >> choice;

    std::shared_ptr<ICache<int, std::int64_t>> cache;

    if (choice == 1)
        cache = std::make_shared<LRUCache<int, std::int64_t>>(5);
    else
        cache = std::make_shared<LFUCache<int, std::int64_t>>(5);

    CacheApp app(cache);
    app.run();
    return 0;
}
