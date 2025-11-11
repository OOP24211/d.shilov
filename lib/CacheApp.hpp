#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
#include <memory>
#include <cstdint>
#include <utility>
#include "../lib/fibonachi.hpp"
#include "../lib/ICache.hpp"

template <typename Key, typename Value>
class ICache;

class CacheApp {
 private:
    std::shared_ptr<ICache<int, std::int64_t>> cache;

 public:
    explicit CacheApp(std::shared_ptr<ICache<int, std::int64_t>> c);

    std::int64_t compute(int n);

    void run();
};
