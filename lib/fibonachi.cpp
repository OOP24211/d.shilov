#include "../lib/fibonachi.hpp"

std::int64_t fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
