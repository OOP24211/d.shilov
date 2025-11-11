#include <gtest/gtest.h>
#include "../lib/ICache.hpp"
#include "../lib/LRUCache.hpp"
#include "../lib/LFUCache.hpp"

// тесты для LRUCache
TEST(LRUCacheTest, PutAndGet) {
    LRUCache<int, int> cache(3);
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    EXPECT_EQ(cache.get(1), 100);
    EXPECT_EQ(cache.get(2), 200);
    EXPECT_EQ(cache.get(3), 300);
}

TEST(LRUCacheTest, EvictionOrder) {
    LRUCache<int, int> cache(2);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.get(1);
    cache.put(3, 30);

    EXPECT_EQ(cache.get(1), 10);
    EXPECT_THROW(cache.get(2), std::runtime_error);
    EXPECT_EQ(cache.get(3), 30);
}

TEST(LRUCacheTest, OperatorBracket) {
    LRUCache<int, int> cache(2);
    cache.put(5, 50);
    EXPECT_EQ(cache[5], 50);
}

// тесты для LFUCache
TEST(LFUCacheTest, PutAndGet) {
    LFUCache<int, int> cache(2);
    cache.put(1, 10);
    cache.put(2, 20);

    EXPECT_EQ(cache.get(1), 10);
    EXPECT_EQ(cache.get(2), 20);
}

TEST(LFUCacheTest, EvictLeastFrequent) {
    LFUCache<int, int> cache(2);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.get(1);
    cache.put(3, 30);

    EXPECT_THROW(cache.get(2), std::runtime_error);
    EXPECT_EQ(cache.get(1), 10);
    EXPECT_EQ(cache.get(3), 30);
}

TEST(LFUCacheTest, OperatorBracket) {
    LFUCache<int, int> cache(2);
    cache.put(7, 70);
    EXPECT_EQ(cache[7], 70);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
