#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
#include <memory>
#include <cstdint>

template <typename Key, typename Value>
class ICache {
 public:
    virtual Value get(const Key& key) = 0;
    virtual void put(const Key& key, const Value& value) = 0;
    virtual Value operator[](const Key& key) = 0;
    virtual ~ICache() = default;
};
