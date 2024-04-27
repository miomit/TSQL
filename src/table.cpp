#include "table.hpp"

auto table::open(std::string path) -> table {
    uint8_t columnCount;
    std::ifstream file(path, std::ios::binary);
    file.read(reinterpret_cast<char*>(&columnCount), sizeof(uint8_t));

    std::vector<TableHader> column(columnCount);
    TableHader buff;

    for (int i = 0; i < columnCount; i++) {
        file.read(reinterpret_cast<char*>(&buff), sizeof(TableHader));
        column[i] = buff;
    }

    file.close();

    return table(path, column);
}

auto table::create(std::string path, std::vector<TableHader> column) -> table {
    std::ofstream file(path, std::ios::binary);
    uint8_t columnCount = uint8_t(column.size());

    file.write(reinterpret_cast<char*>(&columnCount), sizeof(uint8_t));

    uint16_t sizeData{0};
    for (auto& col : column) {
        sizeData += col.size;
        file.write(reinterpret_cast<char*>(&col), sizeof(TableHader));
    }

    file.write(reinterpret_cast<char*>(&sizeData), sizeof(uint16_t));

    file.close();

    return table(path, column);
}
