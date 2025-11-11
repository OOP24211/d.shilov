#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
#include <cstdint>
#include <memory>
#include "../lib/ICache.hpp"

template <typename Key, typename Value>
class LFUCache : public ICache<Key, Value> {
 private:
    struct Node {
        Value value;
        int freq;
    };

    size_t capacity;
    std::unordered_map<Key, Node> cache;

 public:
    explicit LFUCache(size_t cap);

    Value get(const Key& key) override;

    void put(const Key& key, const Value& value) override;

    Value operator[](const Key& key) override;
};
