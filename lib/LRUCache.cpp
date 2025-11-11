#include "../lib/LRUCache.hpp"

template<typename Key, typename Value>
Value LRUCache<Key, Value>::get(const Key& key) {
    auto it = cache.find(key);
    if (it == cache.end()) throw std::runtime_error("Ключ не найден");
    order.erase(it->second.second);
    order.push_front(key);
    it->second.second = order.begin();
    return it->second.first;
}

template<typename Key, typename Value>
void LRUCache<Key, Value>::put(const Key& key, const Value& value) {
    auto it = cache.find(key);
    if (it != cache.end()) {
        order.erase(it->second.second);
        order.push_front(key);
        it->second = {value, order.begin()};
    } else {
        if (cache.size() >= capacity) {
            Key last = order.back();
            order.pop_back();
            cache.erase(last);
        }
        order.push_front(key);
        cache[key] = {value, order.begin()};
    }
}

template<typename Key, typename Value>
Value LRUCache<Key, Value>::operator[](const Key& key) {
    return get(key);
}

template class LRUCache<int, int64_t>;
