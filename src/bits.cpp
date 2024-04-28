#include <iostream>
#include "bits.hpp"

auto int_to_bits(int num) -> std::vector<uint8_t> {
    auto res = std::vector<uint8_t>(4, 0);

    for (int i = 0; i < res.size(); i++) {
        res[i] |= uint8_t(num >> i * 8);
    }

    return res;
}

auto string_to_bits(std::string str, size_t size) -> std::vector<uint8_t> {
    auto res = std::vector<uint8_t>(size, 0);

    for (int i = 0; i < res.size() && i < str.length(); i++) {
        res[i] = str[i];
    }

    return res;
}

auto bytes_to_int (std::vector<uint8_t> bytes) -> int {
    if (bytes.size() != 4) throw "bytes.size() != 4";
    return  int(bytes[0])
            | int(bytes[1]) << 8
            | int(bytes[2]) << 16
            | int(bytes[3]) << 24;
}

template<typename T>
auto bytes_to_type (std::vector<uint8_t> bytes) -> T {
    T res{0}; int c{-1};
    for (auto byte = bytes.begin(); byte != bytes.end() && ++c < sizeof(T); ++byte) {
        T a{*byte}; a <<= c * 8;
        res |= a;
    }
    return res;
}

template<typename T>
auto type_to_bits(T num) -> std::vector<bool> {
    std::vector<bool> res{};
    for (int i = sizeof(num) * 8 - 1; i >= 0; --i) res.push_back(num & (1 << i));
    return res;
}

template<typename T>
auto print_bits(T num) -> void {
    int c{0};
    for (auto i : type_to_bits<T>(num)) {
        std::cout << i << (++c % 8 == 0 ? " " : "");
    }
}
