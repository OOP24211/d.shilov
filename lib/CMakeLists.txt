set(LIB_FILES ReadFile.cpp WordCounter.cpp RecordFile.cpp Error.cpp App.cpp)

add_library(lib
            ${LIB_FILES}
)

target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
