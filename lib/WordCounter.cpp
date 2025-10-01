#include "../lib/WordCounter.hpp"

WordCounter::WordCounter() : wordCount_(0) {}
// оставил для примера себе, понял что у int zero-initialization

void WordCounter::calculateFreq() {
    for (auto& it : table_) {
        it.second.precent = (static_cast<float>(it.second.count) \
        / wordCount_) * 100;
    }
}

void WordCounter::count(std::vector<std::wstring> text) {
    for (auto word : text) {
        table_.emplace(word, words_);
        table_[word].count++;
        wordCount_++;
    }
}

std::vector<std::pair<std::wstring, \
WordCounter::frequency>> WordCounter::sort() {
    calculateFreq();
    vec_.insert(vec_.end(), table_.begin(), table_.end());
    table_.clear();
    std::sort(vec_.begin(), vec_.end(),
    [](const std::pair<std::wstring, frequency>& a,
    const std::pair<std::wstring, frequency>& b) {
        return a.second.count > b.second.count;
    });
    return vec_;
}

