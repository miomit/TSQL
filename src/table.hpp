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

struct TableHader
{
    char name[50];
    Type type;
    uint8_t size;

    TableHader() = default;
	TableHader(std::string str, Type t, uint8_t s): type{t}, size{s} {
		if (str.length() > sizeof(name)) throw "str.length() > sizeof(name)";
		str.copy(name, sizeof(name));
		name[str.length()] = '\0';
	}
    static auto NUM(std::string name) -> TableHader {
    	return TableHader(name, INT, 4);
    }
    static auto SYMBOL(std::string name) -> TableHader {
    	return TableHader(name, CHAR, 1);
    }
 	static auto TEXT(std::string name, uint8_t size) -> TableHader {
    	return TableHader(name, CHAR, size);
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
	static auto create(std::string path, std::vector<TableHader> column) -> table;
    table(std::string path, std::vector<TableHader> column): 
    _path{path},
    _column{column} {
		_list = std::make_unique<flist>(path, 1 + sizeof(TableHader) * _column.size());
		unsigned int seek{0};
		for (auto& col : _column) {
			_seekData.push_back(seek);
			seek += col.size;
		}
    };
    auto insert(std::map<std::string, std::string> cell) -> bool;
    auto update(uint16_t row, std::map<std::string, std::string> cell) -> bool;
    auto remove(uint16_t row) -> void;
private:
	std::unique_ptr<flist> _list;
    std::string _path;
    std::vector<TableHader> _column;
    std::vector<unsigned int> _seekData;
};