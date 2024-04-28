#include <vector>
#include <cstdint>
#include <string>

auto int_to_bits(int num) -> std::vector<uint8_t>;
auto string_to_bits(std::string str, size_t size) -> std::vector<uint8_t>;
auto bytes_to_int (std::vector<uint8_t> bytes) -> int;

template<typename T>
auto bytes_to_type (std::vector<uint8_t> bytes) -> T;

template<typename T>
auto type_to_bits(T num) -> std::vector<bool>;

template<typename T>
auto print_bits(T num) -> void;