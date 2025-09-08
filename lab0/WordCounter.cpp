#include "WordCounter.hpp"

WordCounter::WordCounter() {
    word_count = 0;
}

void WordCounter::Calculate_freq() {
    for(auto it = this->table.begin(); it != this->table.end(); it++) {
        it->second.precent = (static_cast<float>(it->second.count) / word_count) * 100;
    }
}

void WordCounter::count(std::vector<std::wstring> text) {
    for (auto word : text) {
        this->table.emplace(word, this->words);
        this->table[word].count++;
        word_count++;
    }
}

std::vector<std::pair<std::wstring, WordCounter::frequency>> WordCounter::Sort() {
    Calculate_freq();
    this->vec.insert(vec.end(), table.begin(), table.end());
    table.clear();
    std::sort(vec.begin(), vec.end(), 
    [](const std::pair<std::wstring, frequency>& a, const std::pair<std::wstring, frequency>& b)
    {return a.second.count > b.second.count;});
    return vec;
}