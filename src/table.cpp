#include <cstring>
#include "table.hpp"

auto table::open(std::string path) -> table {
    uint8_t columnCount;
    std::ifstream file(path, std::ios::binary);
    file.read(reinterpret_cast<char*>(&columnCount), sizeof(uint8_t));

    std::vector<TableHeader> column(columnCount);
    TableHeader buff;

    for (int i = 0; i < columnCount; i++) {
        file.read(reinterpret_cast<char*>(&buff), sizeof(TableHeader));
        column[i] = buff;
    }

    file.close();

    return table(path, column);
}

auto table::create(std::string path, std::vector<TableHeader> column) -> table {
    std::ofstream file(path, std::ios::binary);
    uint8_t columnCount = uint8_t(column.size());

    file.write(reinterpret_cast<char*>(&columnCount), sizeof(uint8_t));

    uint16_t sizeData{0};

    struct T {
        char name[50];
        Type type;
        uint8_t size;
    };

    for (auto& col : column) {
        sizeData += col.size;
        T d = T{"", col.type, col.size};
        strcpy(d.name, col.name);
        file.write(reinterpret_cast<char*>(&d), sizeof(TableHeader));
    }

    file.write(reinterpret_cast<char*>(&sizeData), sizeof(uint16_t));

    sizeData = 0;
    
    file.write(reinterpret_cast<char*>(&sizeData), sizeof(uint16_t));
    file.write(reinterpret_cast<char*>(&sizeData), sizeof(uint16_t));
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

auto table::insert(std::vector<std::string> row) -> bool {
    if (row.size() != _column.size()) return false;
    std::map<std::string, std::string> cell;
    for (auto i = 0; i < row.size(); i++) {
        cell[_column[i].name] = row[i];
    }
    return insert(cell);
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

auto table::remove(uint16_t row) -> void {
    _list->removeAt(row);
}

auto table::getIntByCell(std::string column, uint16_t row) -> int {
    for (auto i = 0; i < _column.size(); i++) {
        if (column == _column[i].name) {
            if (_column[i].type != INT) throw "Cell is not int";
            auto data = (*_list)[row];
            auto buff = std::vector<uint8_t>(_column[i].size, 0);
            for (auto j = 0; j < buff.size(); j++) {
                buff[j] = data[j + _seekData[i]];
            }
            return bytes_to_int(buff);
        }
    }
    throw "column is not exist";
}

auto table::getStringByCell(std::string column, uint16_t row) -> std::string {
    for (auto i = 0; i < _column.size(); i++) {
        if (column == _column[i].name) {
            if (_column[i].type != CHAR) throw "Cell is not int";
            auto data = (*_list)[row];
            auto buff = std::vector<uint8_t>(_column[i].size, 0);
            for (auto j = 0; j < buff.size(); j++) {
                buff[j] = data[j + _seekData[i]];
            }
            return bytes_to_string(buff);
        }
    }
    throw "column is not exist";
}

auto table::getHeaderTypeByName(std::string name) -> Type {
    for (auto& col : _column) {
        if (col.name == name) {
            return col.type;
        }
    }
    throw "column is not exist";
}

auto table::getHeaderNames() -> std::vector<std::string> {
    std::vector<std::string> res;

    for (auto& col : _column) {
        res.push_back(col.name);
    }

    return res;
}

auto table::size() -> unsigned int {
    return _list->size();
}
