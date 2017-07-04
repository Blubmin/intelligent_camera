#pragma once

#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> split(const std::string& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

template<typename T, size_t N>
int array_len(const T(&t)[N]) {
    return sizeof(t) / sizeof(*t);
}