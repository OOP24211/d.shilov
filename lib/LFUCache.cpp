#include "../lib/LFUCache.hpp"

template <typename Key, typename Value>
LFUCache<Key, Value>::LFUCache(size_t cap) : capacity(cap) {}

template <typename Key, typename Value>
Value LFUCache<Key, Value>::get(const Key& key) {
    auto it = cache.find(key);
    if (it == cache.end()) throw std::runtime_error("Ключ не найден");
    it->second.freq++;
    return it->second.value;
}

template <typename Key, typename Value>
void LFUCache<Key, Value>::put(const Key& key, const Value& value) {
    if (cache.size() >= capacity && cache.find(key) == cache.end()) {
        Key minKey = cache.begin()->first;
        int minFreq = cache.begin()->second.freq;
        for (auto& [k, node] : cache) {
            if (node.freq < minFreq) {
                minFreq = node.freq;
                minKey = k;
            }
        }
        cache.erase(minKey);
    }
    cache[key] = {value, 1};
}

template <typename Key, typename Value>
Value LFUCache<Key, Value>::operator[](const Key& key) {
    return get(key);
}

template class LFUCache<int, int64_t>;
template class LFUCache<int, int>;
