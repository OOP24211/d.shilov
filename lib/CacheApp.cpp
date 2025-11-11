#include "../lib/CacheApp.hpp"
#include <utility>

CacheApp::CacheApp(std::shared_ptr<ICache<int, std::int64_t>> c)
    : cache(std::move(c)) {}

std::int64_t CacheApp::compute(int n) {
    try {
        return (*cache)[n];
    } catch (const std::exception& e) {
        std::int64_t result = fibonacci(n);
        cache->put(n, result);
        return result;
    }
}

void CacheApp::run() {
    std::cout << "Введите число (или -1 для выхода):\n";
    int n;
    while (true) {
        std::cout << "> ";
        std::cin >> n;
        if (n < 0) break;
        std::int64_t res = compute(n);
        std::cout << "F(" << n << ") = " << res << "\n";
    }
}

