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

auto table::insert(std::map<std::string, std::string> cell) -> bool {
    auto data = std::vector<uint8_t>(_list->getSizeData(), 0);
    for (auto& el : cell) {
        bool isFind = false;
        for (auto i = 0; i < _column.size(); i++) {
            if (el.first == _column[i].name) {
                isFind = true;
                std::vector<uint8_t> buff;
                if (_column[i].type == INT) {
                    buff = int_to_bits(std::stoi(el.second));
                } else {
                    buff = string_to_bits(el.second, _column[i].size);
                }
                for (auto j = 0; j < buff.size(); j++) {
                    data[j + _seekData[i]] = buff[j];
                }
                break;
            }
        }
        if (!isFind) return false;
    }
    _list->append(data);
    return true;
}

auto table::update(uint16_t row, std::map<std::string, std::string> cell) -> bool {
    auto data = (*_list)[row];
    for (auto& el : cell) {
        bool isFind = false;
        for (auto i = 0; i < _column.size(); i++) {
            if (el.first == _column[i].name) {
                isFind = true;
                std::vector<uint8_t> buff;
                if (_column[i].type == INT) {
                    buff = int_to_bits(std::stoi(el.second));
                } else {
                    buff = string_to_bits(el.second, _column[i].size);
                }
                for (auto j = 0; j < buff.size(); j++) {
                    data[j + _seekData[i]] = buff[j];
                }
                break;
            }
        }
        if (!isFind) return false;
    }
    _list->updateAt(row, data);
    return true;
}
