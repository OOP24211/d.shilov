set(LIBRARY CacheApp.cpp fibonachi.cpp LFUCache.cpp LRUCache.cpp)

add_library(lib
            ${LIBRARY}
)

target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
