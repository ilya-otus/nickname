#pragma once
#include <string>

namespace Patricia {

template <typename T>
T radixSubstr(const T &value, size_t begin, size_t num);
template <>
inline std::string radixSubstr(const std::string &value, size_t begin, size_t num) {
    return value.substr(begin, num);
}

template <typename T>
T radixJoin(const T &value1, const T &value2);
template <>
inline std::string radixJoin(const std::string &value1, const std::string &value2) {
    return value1 + value2;
}

template <typename T>
size_t radixSize(const T &value);
template <>
size_t radixSize(const std::string &value) {
    return value.size();
}

} // namespace Patricia
