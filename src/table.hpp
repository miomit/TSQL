#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "bits.hpp"
#include "flist.hpp"

/*
|----------------------------------|
|_countClumns|_column[_countClumns]|
|------------+---------------------|
|_list                             |
|----------------------------------|
*/

enum Type { INT, CHAR };

struct TableHeader
{
    char name[50];
    Type type;
    uint8_t size;

    TableHeader() = default;
	TableHeader(std::string str, Type t, uint8_t s): type{t}, size{s} {
		if (str.length() > sizeof(name)) throw "str.length() > sizeof(name)";
		str.copy(name, sizeof(name));
		name[str.length()] = '\0';
	}
    static auto NUM(std::string name) -> TableHeader {
    	return TableHeader(name, INT, 4);
    }
    static auto SYMBOL(std::string name) -> TableHeader {
    	return TableHeader(name, CHAR, 1);
    }
 	static auto TEXT(std::string name, uint8_t size) -> TableHeader {
    	return TableHeader(name, CHAR, size);
    }

    auto print() -> void {
    	std::cout << name << " ";
    	if (type == INT) {
    		std::cout << "INT" << std::endl;
    	} else {
    		std::cout << "CHAR";
    		if (size > 1) std::cout << "[" << int(size) << "]";
    		std::cout << std::endl;
    	}
    }
};

class table {
public:
	static auto open(std::string path) -> table;
	static auto create(std::string path, std::vector<TableHeader> column) -> table;
    table(std::string path, std::vector<TableHeader> column):
    _path{path},
    _column{column} {
		_list = std::make_unique<flist>(path, 1 + sizeof(TableHeader) * _column.size());
		unsigned int seek{0};
		for (auto& col : _column) {
			_seekData.push_back(seek);
			seek += col.size;
		}
    };
    auto insert(std::map<std::string, std::string> cell) -> bool;
    auto insert(std::vector<std::string> row) -> bool;
    auto update(uint16_t row, std::map<std::string, std::string> cell) -> bool;
    auto remove(uint16_t row) -> void;

    auto getIntByCell(std::string column, uint16_t row) -> int;
    auto getStringByCell(std::string column, uint16_t row) -> std::string;
private:
    std::unique_ptr<flist> _list;
    std::string _path;
    std::vector<TableHeader> _column;
    std::vector<unsigned int> _seekData;
};