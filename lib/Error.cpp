#include "../lib/Error.hpp"

Error::Error(const std::string &err): mes_(err) {}

const char* Error::what() const noexcept {
    return mes_.c_str();
}
