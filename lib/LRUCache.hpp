#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
#include <utility>
#include <cstdint>
#include <memory>
#include "../lib/ICache.hpp"

template <typename Key, typename Value>
class LRUCache : public ICache<Key, Value> {
 private:
    size_t capacity;
    std::list<Key> order;
    std::unordered_map<Key, std::pair<Value,
    typename std::list<Key>::iterator>> cache;

 public:
    explicit LRUCache(size_t cap) : capacity(cap) {}

    Value get(const Key& key) override;

    void put(const Key& key, const Value& value) override;

    Value operator[](const Key& key) override;
};
